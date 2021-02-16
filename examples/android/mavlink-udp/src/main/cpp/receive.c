        //FIXME: przenieść odbieranie wiadomości do osobnego wątku, bez oczekiwania 1 sekundy
        memset(buf, 0, BUFFER_LENGTH);
        recsize = recvfrom(sock, (void *) buf, BUFFER_LENGTH, 0, (struct sockaddr *) &gcAddr,
                           &fromlen);
        if (recsize > 0) {
            // Something received - print out all bytes and parse packet.
            mavlink_message_t msg;

            mavlink_status_t status;

            printf("Bytes Received: %d\nDatagram: ", (int) recsize);
            for (i = 0; i < recsize; ++i) {
                sprintf(feedback, "byte received: %02d", (unsigned char) buf[i]);
                set_log_message(thiz, feedback);

                if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status) == 1) {
                    // Packet received.
                    printf("\nReceived packet: SYS: %d, COMP: %d, LEN: %d, MSG ID: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);

                    switch (msg.msgid) {
                        case 0: {
                            sprintf(feedback, "Packet sequence: %d", msg.seq);
                            set_ui_message_heartbeat(thiz, feedback, true);
                        }
                            break;
                        case 2: {
                            /* https://mavlink.io/en/messages/common.html#SYSTEM_TIME */
                            set_ui_message(thiz, "SYSTEM_TIME", true);
                        }
                            break;
                        case 20: {
                            /* https://mavlink.io/en/messages/common.html#PARAM_REQUEST_READ */
                            set_ui_message(thiz, "PARAM_REQUEST_READ", true);

                            sprintf(feedback, "System ID: %d, Component ID: %d, Payload length: %d, Message ID: %d, Packet sequence: %d", msg.sysid, msg.compid, msg.len, msg.msgid, msg.seq);
                            set_log_message(thiz, feedback);
                        }
                            break;
                        case 21: {
                            /* https://mavlink.io/en/messages/common.html#PARAM_REQUEST_LIST */
                            set_ui_message(thiz, "PARAM_REQUEST_LIST", true);
                            /* This app sends magnetometer indication (not GPS) as heading */
                            sendDiscreteParameter("COMPASS_USE", true);
                        }
                            break;
                        case 43: {
                            /* https://mavlink.io/en/messages/common.html#MISSION_REQUEST_LIST */
                            set_ui_message(thiz, "MISSION_REQUEST_LIST", true);
                        }
                            break;
                        case 69: {
                            mavlink_manual_control_t manual_control;
                            mavlink_msg_manual_control_decode(&msg, &manual_control);

                            char axes[128];
                            sprintf(axes, "MANUAL_CONTROL X: %d, Y: %d, Z: %d, R: %d, buttons: %d", manual_control.x, manual_control.y, manual_control.z, manual_control.r, manual_control.buttons);
                            set_ui_message(thiz, axes, true);
                            set_progress(thiz, manual_control.x, manual_control.y, manual_control.z, manual_control.r);

                            //TODO: test
                            char buttons[128];
                            sprintf(buttons, "[BYTE_TO_BINARY_PATTERN][BYTE_TO_BINARY_PATTERN]", BYTE_TO_BINARY(manual_control.buttons>>8), BYTE_TO_BINARY(manual_control.buttons));
                            sprintf(feedback, "X: %d, Y: %d, Z: %d, R: %d, buttons: %d", manual_control.x, manual_control.y, manual_control.z, manual_control.r, buttons);
                            set_log_message(thiz, feedback);
                        }
                            break;
                        case 75: {
                            set_ui_message(thiz, "COMMAND_INT", true);

                            sprintf(feedback, "System ID: %d, Component ID: %d, Payload length: %d, Message ID: %d, Packet sequence: %d",
                                    msg.sysid, msg.compid, msg.len, msg.msgid, msg.seq);
                            set_log_message(thiz, feedback);
                        }
                            break;
                        case 76: {
                            set_ui_message(thiz, "COMMAND_LONG", true);

                            sprintf(feedback, "System ID: %d, Component ID: %d, Payload length: %d, Message ID: %d, Packet sequence: %d",
                                    msg.sysid, msg.compid, msg.len, msg.msgid, msg.seq);
                            set_log_message(thiz, feedback);
                        }
                            break;
                        default: {
                            sprintf(feedback, "System ID: %d, Component ID: %d, Payload length: %d, Message ID: %d, Packet sequence: %d",
                                    msg.sysid, msg.compid, msg.len, msg.msgid, msg.seq);
                            set_ui_message(thiz, feedback, true);
                        }
                            break;
                    }
                }
            }
            printf("\n");
        }
        memset(buf, 0, BUFFER_LENGTH);
