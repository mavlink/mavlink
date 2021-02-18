/*******************************************************************************
 Copyright (C) 2010  Bryan Godbolt godbolt ( a t ) ualberta.ca

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 ****************************************************************************/
/*
 This program sends some data to qgroundcontrol using the mavlink protocol. The sent packets
 cause qgroundcontrol to respond with heartbeats. Any settings or custom commands sent from
 qgroundcontrol are printed by this program along with the heartbeats.


 The rt library is needed for the clock_gettime on linux.
 */

/* These headers are for QNX, but should all be standard on unix/linux */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

#if (defined __QNX__) | (defined __QNXNTO__)
/* QNX specific headers */
#include <unix.h>
#else
/* Linux / MacOS POSIX timer headers */
#include <sys/time.h>
#include <time.h>
#include <arpa/inet.h>
#include <stdbool.h> /* required for the definition of bool in C99 */

#endif

/* Path to generated mavlink headers must be pointed in Android.mk file. */
#include <common/mavlink.h>

#define BUFFER_LENGTH 2041 // minimum buffer size that can be used with qnx (I don't know why)

// global, used also outside heartbeat thread
int sock;
struct sockaddr_in gcAddr;
struct sockaddr_in locAddr;
//struct sockaddr_in fromAddr;
uint8_t buf[BUFFER_LENGTH];
int bytes_sent;

uint64_t microsSinceEpoch();

#define EXIT_FAILURE -1

#define THIS_SYSTEM 2
#define THIS_COMPONENT 1

/* Android specific */
#include <pthread.h>
#include <jni.h>

bool running = false;
static pthread_t app_thread, rcv_thread;
static pthread_key_t current_jni_env;
static JavaVM *java_vm;
static jmethodID set_message_method_id, set_buttons_method_id, set_log_method_id, set_progress_method_id;

char ground_station_ip[100];
float hdg = 0;
int battery_voltage, battery_level;

/** https://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format */

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

/** https://gstreamer.freedesktop.org/documentation/tutorials/android/a-running-pipeline.html */
static JNIEnv *attach_current_thread(void) {
    JNIEnv *env;
    JavaVMAttachArgs args;
    printf("Attaching thread %p", pthread_self());
    args.version = JNI_VERSION_1_4;
    args.name = NULL;
    args.group = NULL;

    if ((*java_vm)->AttachCurrentThread(java_vm, &env, &args) < 0) {
        printf("Failed to attach current thread");
        return NULL;
    }
    return env;
}

static void detach_current_thread(void *env) {
    printf("Detaching thread %p", pthread_self());
    (*java_vm)->DetachCurrentThread(java_vm);
}

static JNIEnv *get_jni_env(void) {
    JNIEnv *env;

    if ((env = pthread_getspecific(current_jni_env)) == NULL) {
        env = attach_current_thread();
        pthread_setspecific(current_jni_env, env);
    }
    return env;
}

/** prints a message in Logcat */
static void set_log_message(jobject obj, const char *message) {
    JNIEnv *env = get_jni_env();

    //printf("Setting message to: %s", message);
    jstring jmessage = (*env)->NewStringUTF(env, message);
    (*env)->CallVoidMethod(env, obj, set_log_method_id, jmessage);
    if ((*env)->ExceptionCheck(env)) {
        printf("Failed to call Java method");
        (*env)->ExceptionClear(env);
    }
    (*env)->DeleteLocalRef(env, jmessage);
}

/** displays a message in the app's GUI */
static void set_ui_message(jobject obj, const char *message, bool blink) {
    JNIEnv *env = get_jni_env();

    printf("Setting message to: %s", message);
    jstring jmessage = (*env)->NewStringUTF(env, message);
    (*env)->CallVoidMethod(env, obj, set_message_method_id, jmessage, blink);
    if ((*env)->ExceptionCheck(env)) {
        printf("Failed to call Java method");
        (*env)->ExceptionClear(env);
    }
    (*env)->DeleteLocalRef(env, jmessage);
}

/** displays a message in the app's GUI, but in a different TextView */
static void set_ui_message_buttons(jobject obj, const char *message, bool blink) {
    JNIEnv *env = get_jni_env();

    printf("Setting message to: %s", message);
    jstring jmessage = (*env)->NewStringUTF(env, message);
    (*env)->CallVoidMethod(env, obj, set_buttons_method_id, jmessage, blink);
    if ((*env)->ExceptionCheck(env)) {
        printf("Failed to call Java method");
        (*env)->ExceptionClear(env);
    }
    (*env)->DeleteLocalRef(env, jmessage);
}

/** moves seekbars */
static void set_progress(jobject obj, int16_t x, int16_t y, int16_t z, int16_t r) {
    JNIEnv *env = get_jni_env();

    (*env)->CallVoidMethod(env, obj, set_progress_method_id, x, y, z, r);
    if ((*env)->ExceptionCheck(env)) {
        printf("Failed to call Java method");
        (*env)->ExceptionClear(env);
    }
}

static void sendDiscreteParameter(char *name, bool value) {
/** https://mavlink.io/en/services/parameter.html
  * https://ardupilot.org/dev/docs/mavlink-get-set-params.html
  */
    mavlink_param_union_t param;
    int32_t asInt = value ? 1 : 0;
    param.param_int32 = asInt;
    param.type = MAV_PARAM_TYPE_INT32;

    mavlink_message_t msgParameter;
    mavlink_msg_param_value_pack(THIS_SYSTEM, THIS_COMPONENT, &msgParameter, name,
                                 param.param_float, param.type, 1, 0);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msgParameter);

    bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr *) &gcAddr, sizeof(struct sockaddr_in));
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    java_vm = vm;

    if ((*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        printf("Could not retrieve JNIEnv");
        return 0;
    }

    pthread_key_create(&current_jni_env, detach_current_thread);
    return JNI_VERSION_1_4;
}

jstring Java_pl_bezzalogowe_mavlink_MAVLink_stringFromJNI(JNIEnv *env, jobject thiz) {
    /** send string from C code to Java code */
    return (*env)->NewStringUTF(env, "JNI is working");
}

void Java_pl_bezzalogowe_mavlink_MAVLink_classInit(JNIEnv *env, jclass klass) {
    /** binds Java methods that can be called from C code */
    set_message_method_id = (*env)->GetMethodID(env, klass, "setMessage", "(Ljava/lang/String;Z)V");
    set_buttons_method_id = (*env)->GetMethodID(env, klass, "setButtons", "(Ljava/lang/String;Z)V");
    set_log_method_id = (*env)->GetMethodID(env, klass, "setLog", "(Ljava/lang/String;)V");
    set_progress_method_id = (*env)->GetMethodID(env, klass, "setProgress", "(SSSS)V");
}

void Java_pl_bezzalogowe_mavlink_MAVLink_setGroundStationIP(JNIEnv *env, jclass klass, jstring ip) {
    /** receives ip address from java code and saves it into global variable.*/
    /* https://stackoverflow.com/questions/4247002/passing-string-from-java-into-jni */
    const char *ipArray = (*env)->GetStringUTFChars(env, ip, 0);
    strcpy(ground_station_ip, ipArray);
    (*env)->ReleaseStringUTFChars(env, ip, ipArray);
}

void
Java_pl_bezzalogowe_mavlink_MAVLink_setHeadingDegrees(JNIEnv *env, jclass klass, double heading) {
    if (running) {
        /* https://stackoverflow.com/questions/14920675/is-there-a-function-in-c-language-to-calculate-degrees-radians */
        hdg = heading * M_PI / 180.0;
    }
}

void Java_pl_bezzalogowe_mavlink_MAVLink_sendAttitude(JNIEnv *env, jclass klass, float roll,
                                                      float pitch /*, float heading*/) {
    if (running) {
        mavlink_message_t msgAttitude;

        /* in radians, heading comes from a different sensor */
        mavlink_msg_attitude_pack(THIS_SYSTEM, THIS_COMPONENT, &msgAttitude, microsSinceEpoch(),
                                  roll, pitch, hdg, 0.00, 0.00, 0.00);

        /* in degrees */
        //mavlink_msg_attitude_pack(THIS_SYSTEM, THIS_COMPONENT, &msgAttitude, microsSinceEpoch(), roll * M_PI / 180.0, pitch * M_PI / 180.0, hdg * M_PI / 180.0, 0.00, 0.00, 0.00);

        uint16_t len = mavlink_msg_to_send_buffer(buf, &msgAttitude);
        bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr *) &gcAddr,
                            sizeof(struct sockaddr_in));
    }
}

void Java_pl_bezzalogowe_mavlink_MAVLink_setBattery(JNIEnv *env, jclass klass, jint voltage,
                                                    jint level) {
    battery_voltage = voltage;
    battery_level = level;
}

void
Java_pl_bezzalogowe_mavlink_MAVLink_sendGlobalPosition(JNIEnv *env, jclass klass,
        double latitude, double longitude, double altitude) {
    if (running) {
        mavlink_message_t msgLocation;

        int32_t lat = latitude * 1E7;
        int32_t lon = longitude * 1E7;
        int32_t alt = altitude * 1000;
        int32_t relative_alt = alt;

        mavlink_msg_global_position_int_pack(THIS_SYSTEM, THIS_COMPONENT, &msgLocation,
                                             microsSinceEpoch(), lat, lon, alt, relative_alt, 0, 0,
                                             0, /*hdg*/ 0);
        uint16_t len = mavlink_msg_to_send_buffer(buf, &msgLocation);
        bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr *) &gcAddr,
                            sizeof(struct sockaddr_in));
    }
}

/** https://mavlink.io/en/services/mission.html#download_mission */
void sendMissionCount(uint8_t system_id, uint8_t component_id,
                      mavlink_mission_request_list_t request_list) {
    mavlink_mission_ack_t msgCount;
    mavlink_msg_mission_count_pack(THIS_SYSTEM, THIS_COMPONENT, &msgCount, system_id, component_id,
                                   0, request_list.mission_type);

    uint16_t len = mavlink_msg_to_send_buffer(buf, &msgCount);
    bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr *) &gcAddr, sizeof(struct sockaddr_in));
}

int *openSocket(jobject thiz) {
    char target_ip[100];
    float position[6] = {};
    sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    /* Change the target ip if parameter was given */
    strcpy(target_ip, "127.0.0.1");
    if (ground_station_ip != NULL) {
        strcpy(target_ip, ground_station_ip);
    }

    char feedback[128];
    sprintf(feedback, "starting heartbeat");
    set_ui_message(thiz, feedback, false);

    /** listening address */
    memset(&locAddr, 0, sizeof(locAddr));
    locAddr.sin_family = AF_INET;
    locAddr.sin_addr.s_addr = INADDR_ANY;
    locAddr.sin_port = htons(14550);

    /* Bind the socket to default port 14550 - necessary to receive packets from QGroundControl */
    if (-1 == bind(sock, (struct sockaddr *) &locAddr, sizeof(struct sockaddr))) {
        perror("error bind failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

/* Attempt to make it non blocking */
#if (defined __QNX__) | (defined __QNXNTO__)
    if (fcntl(sock, F_SETFL, O_NONBLOCK | FASYNC) < 0)
#else
    if (fcntl(sock, F_SETFL, O_NONBLOCK | O_ASYNC) < 0)
#endif
    {
        fprintf(stderr, "error setting nonblocking: %s\n", strerror(errno));
        close(sock);
        exit(EXIT_FAILURE);
    }

    /** ground station address */
    memset(&gcAddr, 0, sizeof(gcAddr));
    gcAddr.sin_family = AF_INET;
    gcAddr.sin_addr.s_addr = inet_addr(target_ip);
    gcAddr.sin_port = htons(14550);
}

int *receiveFunction(jobject thiz) {

    uint8_t rcv_buf[BUFFER_LENGTH];
    ssize_t recsize;
    socklen_t fromlen = sizeof(gcAddr);
    int i = 0;
    unsigned int temp = 0;

    while (running) {
        memset(rcv_buf, 0, BUFFER_LENGTH);
        recsize = recvfrom(sock, (void *) rcv_buf, BUFFER_LENGTH, 0, (struct sockaddr *) &gcAddr,
                           &fromlen);
        if (recsize > 0) {
            // Something received - print out all bytes and parse packet.
            mavlink_message_t rcv_msg;

            mavlink_status_t status;

            char feedback[128];

            printf("Bytes Received: %d\nDatagram: ", (int) recsize);
            for (i = 0; i < recsize; ++i) {
                sprintf(feedback, "byte received: %02d", (unsigned char) rcv_buf[i]);
                set_log_message(thiz, feedback);

                if (mavlink_parse_char(MAVLINK_COMM_0, rcv_buf[i], &rcv_msg, &status) == 1) {
                    // Packet received.
                    printf("\nReceived packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n",
                           rcv_msg.sysid,
                           rcv_msg.compid, rcv_msg.len, rcv_msg.msgid);

                    switch (rcv_msg.msgid) {
                        case 0: {
                            /** https://mavlink.io/en/messages/common.html#HEARTBEAT */
                            //sprintf(feedback, "HEARTBEAT: %d", rcv_msg.seq);
                            //set_ui_message_heartbeat(thiz, feedback, true);
                        }
                            break;
                        case 2: {
                            /** https://mavlink.io/en/messages/common.html#SYSTEM_TIME */
                            set_ui_message(thiz, "SYSTEM_TIME", true);
                        }
                            break;
                        case 20: {
                            /** https://mavlink.io/en/messages/common.html#PARAM_REQUEST_READ */
                            set_ui_message(thiz, "PARAM_REQUEST_READ", true);

                            sprintf(feedback, "SYS: %d, COMP: %d, LEN: %d, MSG ID: %d, SEQ: %d",
                                    rcv_msg.sysid, rcv_msg.compid, rcv_msg.len, rcv_msg.msgid,
                                    rcv_msg.seq);
                            set_log_message(thiz, feedback);
                        }
                            break;
                        case 21: {
                            /** https://mavlink.io/en/messages/common.html#PARAM_REQUEST_LIST */
                            set_ui_message(thiz, "PARAM_REQUEST_LIST", true);
                            /* This app sends magnetometer indication (not GPS) as heading */
                            sendDiscreteParameter("COMPASS_USE", true);
                        }
                            break;
                        case 43: {
                            /** https://mavlink.io/en/messages/common.html#MISSION_REQUEST_LIST */
                            set_ui_message(thiz, "MISSION_REQUEST_LIST", true);

                            sprintf(feedback,
                                    "MISSION_REQUEST_LIST\tSYS: %d, COMP: %d, LEN: %d, MSG ID: %d, SEQ: %d",
                                    rcv_msg.sysid, rcv_msg.compid, rcv_msg.len, rcv_msg.msgid,
                                    rcv_msg.seq);
                            set_log_message(thiz, feedback);

                            mavlink_mission_request_list_t request_list;
                            mavlink_msg_mission_request_list_decode(&rcv_msg, &request_list);
                            sendMissionCount(rcv_msg.sysid, rcv_msg.compid, request_list);
                        }
                            break;
                        case 69: {
                            /** https://mavlink.io/en/messages/common.html#MANUAL_CONTROL */
                            mavlink_manual_control_t manual_control;
                            mavlink_msg_manual_control_decode(&rcv_msg, &manual_control);

                            char ui_string[128];
                            char buttons[64];

                            /** displays bit field representing pressed buttons */
                            sprintf(buttons, BYTE_TO_BINARY_PATTERN BYTE_TO_BINARY_PATTERN,
                                    BYTE_TO_BINARY(manual_control.buttons >> 8),
                                    BYTE_TO_BINARY(manual_control.buttons));

                            /** sets positions of seekbars representing joystick/gamepad axes */
                            set_progress(thiz, manual_control.x, manual_control.y,
                                    manual_control.z, manual_control.r);
                            /*
                            sprintf(ui_string, "axes: X: %d, Y: %d, Z: %d, R: %d\r\nbuttons: %s",
                            manual_control.x, manual_control.y, manual_control.z, manual_control.r, buttons);
                            */
                            sprintf(ui_string, "buttons: %s", buttons);
                            set_ui_message_buttons(thiz, ui_string, true);

                            sprintf(feedback,
                                    "MANUAL_CONTROL\tX: %d, Y: %d, Z: %d, R: %d, buttons: %s",
                                    manual_control.x, manual_control.y,
                                    manual_control.z, manual_control.r, buttons);
                            set_log_message(thiz, feedback);
                        }
                            break;
                        case 75: {
                            /** https://mavlink.io/en/messages/common.html#COMMAND_INT */
                            set_ui_message(thiz, "COMMAND_INT", true);

                            sprintf(feedback,
                                    "COMMAND_INT\tSYS: %d, COMP: %d, LEN: %d, MSG ID: %d, SEQ: %d",
                                    rcv_msg.sysid, rcv_msg.compid, rcv_msg.len, rcv_msg.msgid,
                                    rcv_msg.seq);
                            set_log_message(thiz, feedback);
                        }
                            break;
                        case 76: {
                            /** https://mavlink.io/en/messages/common.html#COMMAND_LONG */
                            set_ui_message(thiz, "COMMAND_LONG", true);

                            sprintf(feedback,
                                    "COMMAND_LONG\tSYS: %d, COMP: %d, LEN: %d, MSG ID: %d, SEQ: %d",
                                    rcv_msg.sysid, rcv_msg.compid, rcv_msg.len, rcv_msg.msgid,
                                    rcv_msg.seq);
                            set_log_message(thiz, feedback);
                        }
                            break;
                        default: {
                            sprintf(feedback, "SYS: %d, COMP: %d, LEN: %d, MSG ID: %d, SEQ: %d",
                                    rcv_msg.sysid, rcv_msg.compid, rcv_msg.len, rcv_msg.msgid,
                                    rcv_msg.seq);
                            set_log_message(thiz, feedback);
                        }
                            break;
                    }
                }
            }
            printf("\n");
        }
        memset(rcv_buf, 0, BUFFER_LENGTH);
        /** The typical RC servo expects to receive a pulse every 20 ms... */
        usleep(20);
    }
    return 0;
}

/** https://github.com/mavlink/mavlink/blob/master/examples/linux/mavlink_udp.c */
int *mainAppFunction(jobject thiz) {
    mavlink_message_t msg;
    uint16_t len;

    while (running) {
        /* send Heartbeat message */
        mavlink_msg_heartbeat_pack(THIS_SYSTEM, THIS_COMPONENT, &msg, MAV_TYPE_GENERIC,
                                   MAV_AUTOPILOT_GENERIC, MAV_MODE_MANUAL_ARMED, 0,
                                   MAV_STATE_ACTIVE);

        len = mavlink_msg_to_send_buffer(buf, &msg);
        bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr *) &gcAddr,
                            sizeof(struct sockaddr_in));

        /* prints a log entry every time a heartbeat message is sent */
        /*
        char seqArr [32];
        sprintf(seqArr, "heartbeat: %d\n", msg.seq);
        set_log_message(thiz, seqArr);
        */

        if (battery_voltage != NULL && battery_level != NULL) {
            mavlink_msg_sys_status_pack(THIS_SYSTEM, THIS_COMPONENT, &msg, 0, 0, 0, 500,
                                        battery_voltage, -1, battery_level, 0, 0, 0, 0, 0, 0);

            len = mavlink_msg_to_send_buffer(buf, &msg);
            bytes_sent = sendto(sock, buf, len, 0, (struct sockaddr *) &gcAddr,
                                sizeof(struct sockaddr_in));

            char feedback[128];
            if (bytes_sent == -1) {
                sprintf(feedback, "ERROR: bytes_sent == -1");
                set_log_message(thiz, feedback);
            }
        }
        sleep(1);
    }
    close(sock);
    return 0;
}

jint Java_pl_bezzalogowe_mavlink_MAVLink_heartBeatInit(JNIEnv *env, jobject thiz) {
    jobject *data = (*env)->NewGlobalRef(env, thiz);
    running = true;
    openSocket(data);
    /* start receiving thread */
    pthread_create(&rcv_thread, NULL, &receiveFunction, data);
    /* start heartbeat sending thread */
    pthread_create(&app_thread, NULL, &mainAppFunction, data);
    return 0;
}

jint Java_pl_bezzalogowe_mavlink_MAVLink_heartBeatStop(JNIEnv *env, jobject thiz) {
    set_ui_message(thiz, "stopping heartbeat", false);
    running = false;
    pthread_join(rcv_thread, NULL);
    pthread_join(app_thread, NULL);
}

/* currently unused */
jint Java_pl_bezzalogowe_mavlink_MAVLink_sendProtocol(JNIEnv *env, jobject thiz) {
    mavlink_message_t *message;
    mavlink_protocol_version_t *version = 200;
    mavlink_msg_protocol_version_encode(1, 200, &message, &version);
    int length;
    length = mavlink_msg_to_send_buffer(buf, &message);
    bytes_sent = sendto(sock, buf, length, 0, (struct sockaddr *) &gcAddr,
                        sizeof(struct sockaddr_in));
    char feedback[128];
    sprintf(feedback, "version sent: %d, %d bytes", version, bytes_sent);
    set_ui_message(thiz, feedback, false);
    free(message);
}

/* currently unused */
jint Java_pl_bezzalogowe_mavlink_MAVLink_sendHello(JNIEnv *env, jobject thiz) {
    char message[5] = "HELLO";
    int length = sizeof(message);
    bytes_sent = sendto(sock, message, length, 0, (struct sockaddr *) &gcAddr,
                        sizeof(struct sockaddr_in));
    char feedback[128];
    sprintf(feedback, "sent: %s, %d bytes", message, bytes_sent);
    set_ui_message(thiz, feedback, false);
}

/* QNX timer version */
#if (defined __QNX__) | (defined __QNXNTO__)
uint64_t microsSinceEpoch()
{

    struct timespec time;

    uint64_t micros = 0;

    clock_gettime(CLOCK_REALTIME, &time);
    micros = (uint64_t)time.tv_sec * 1000000 + time.tv_nsec/1000;

    return micros;
}
#else

uint64_t microsSinceEpoch() {
    struct timeval tv;
    uint64_t micros = 0;
    gettimeofday(&tv, NULL);
    micros = ((uint64_t) tv.tv_sec) * 1000000 + tv.tv_usec;
    return micros;
}
#endif