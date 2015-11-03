#!/usr/bin/env python
'''
mavlink python utility functions

Copyright Andrew Tridgell 2011
Released under GNU GPL version 3 or later
'''

import socket, math, struct, time, os, fnmatch, array, sys, errno
import select

# adding these extra imports allows pymavlink to be used directly with pyinstaller
# without having complex spec files. To allow for installs that don't have ardupilotmega
# at all we avoid throwing an exception if it isn't installed
try:
    import json
    from pymavlink.dialects.v10 import ardupilotmega
except Exception:
    pass

# these imports allow for mavgraph and mavlogdump to use maths expressions more easily
from math import *
from .mavextra import *

'''
Support having a $HOME/.pymavlink/mavextra.py for extra graphing functions
'''
home = os.getenv('HOME')
if home is not None:
    extra = os.path.join(home, '.pymavlink', 'mavextra.py')
    if os.path.exists(extra):
        import imp
        mavuser = imp.load_source('pymavlink.mavuser', extra)
        from pymavlink.mavuser import *

# Store the MAVLink library for the currently-selected dialect
# (set by set_dialect())
mavlink = None

# Store the mavlink file currently being operated on
# (set by mavlink_connection())
mavfile_global = None

# If the caller hasn't specified a particular native/legacy version, use this
default_native = False

# Use a globally-set MAVLink dialect if one has been specified as an environment variable.
if not 'MAVLINK_DIALECT' in os.environ:
    os.environ['MAVLINK_DIALECT'] = 'ardupilotmega'

def mavlink10():
    '''return True if using MAVLink 1.0'''
    return not 'MAVLINK09' in os.environ

def evaluate_expression(expression, vars):
    '''evaluation an expression'''
    try:
        v = eval(expression, globals(), vars)
    except NameError:
        return None
    except ZeroDivisionError:
        return None
    return v

def evaluate_condition(condition, vars):
    '''evaluation a conditional (boolean) statement'''
    if condition is None:
        return True
    v = evaluate_expression(condition, vars)
    if v is None:
        return False
    return v

class location(object):
    '''represent a GPS coordinate'''
    def __init__(self, lat, lng, alt=0, heading=0):
        self.lat = lat
        self.lng = lng
        self.alt = alt
        self.heading = heading

    def __str__(self):
        return "lat=%.6f,lon=%.6f,alt=%.1f" % (self.lat, self.lng, self.alt)

def set_dialect(dialect):
    '''set the MAVLink dialect to work with.
    For example, set_dialect("ardupilotmega")
    '''
    global mavlink, current_dialect
    from .generator import mavparse
    if mavlink is None or mavlink.WIRE_PROTOCOL_VERSION == "1.0" or not 'MAVLINK09' in os.environ:
        wire_protocol = mavparse.PROTOCOL_1_0
        modname = "pymavlink.dialects.v10." + dialect
    else:
        wire_protocol = mavparse.PROTOCOL_0_9
        modname = "pymavlink.dialects.v09." + dialect

    try:
        mod = __import__(modname)
    except Exception:
        # auto-generate the dialect module
        from .generator.mavgen import mavgen_python_dialect
        mavgen_python_dialect(dialect, wire_protocol)
        mod = __import__(modname)
    components = modname.split('.')
    for comp in components[1:]:
        mod = getattr(mod, comp)
    current_dialect = dialect
    mavlink = mod

# Set the default dialect. This is done here as it needs to be after the function declaration
set_dialect(os.environ['MAVLINK_DIALECT'])

class mavfile(object):
    '''a generic mavlink port'''
    def __init__(self, fd, address, source_system=255, notimestamps=False, input=True, use_native=default_native):
        global mavfile_global
        if input:
            mavfile_global = self
        self.fd = fd
        self.address = address
        self.messages = { 'MAV' : self }
        if mavlink.WIRE_PROTOCOL_VERSION == "1.0":
            self.messages['HOME'] = mavlink.MAVLink_gps_raw_int_message(0,0,0,0,0,0,0,0,0,0)
            mavlink.MAVLink_waypoint_message = mavlink.MAVLink_mission_item_message
        else:
            self.messages['HOME'] = mavlink.MAVLink_gps_raw_message(0,0,0,0,0,0,0,0,0)
        self.params = {}
        self.target_system = 0
        self.target_component = 0
        self.source_system = source_system
        self.first_byte = True
        self.robust_parsing = True
        self.mav = mavlink.MAVLink(self, srcSystem=self.source_system, use_native=use_native)
        self.mav.robust_parsing = self.robust_parsing
        self.logfile = None
        self.logfile_raw = None
        self.param_fetch_in_progress = False
        self.param_fetch_complete = False
        self.start_time = time.time()
        self.flightmode = "UNKNOWN"
        self.vehicle_type = "UNKNOWN"
        self.mav_type = mavlink.MAV_TYPE_FIXED_WING
        self.base_mode = 0
        self.timestamp = 0
        self.message_hooks = []
        self.idle_hooks = []
        self.uptime = 0.0
        self.notimestamps = notimestamps
        self._timestamp = None
        self.ground_pressure = None
        self.ground_temperature = None
        self.altitude = 0
        self.WIRE_PROTOCOL_VERSION = mavlink.WIRE_PROTOCOL_VERSION
        self.last_seq = {}
        self.mav_loss = 0
        self.mav_count = 0
        self.stop_on_EOF = False
        self.portdead = False

    def auto_mavlink_version(self, buf):
        '''auto-switch mavlink protocol version'''
        global mavlink
        if len(buf) == 0:
            return
        try:
            magic = ord(buf[0])
        except:
            magic = buf[0]
        if not magic in [ 85, 254 ]:
            return
        self.first_byte = False
        if self.WIRE_PROTOCOL_VERSION == "0.9" and magic == 254:
            self.WIRE_PROTOCOL_VERSION = "1.0"
            set_dialect(current_dialect)
        elif self.WIRE_PROTOCOL_VERSION == "1.0" and magic == 85:
            self.WIRE_PROTOCOL_VERSION = "0.9"
            set_dialect(current_dialect)
            os.environ['MAVLINK09'] = '1'
        else:
            return
        # switch protocol 
        (callback, callback_args, callback_kwargs) = (self.mav.callback,
                                                      self.mav.callback_args,
                                                      self.mav.callback_kwargs)
        self.mav = mavlink.MAVLink(self, srcSystem=self.source_system)
        self.mav.robust_parsing = self.robust_parsing
        self.WIRE_PROTOCOL_VERSION = mavlink.WIRE_PROTOCOL_VERSION
        (self.mav.callback, self.mav.callback_args, self.mav.callback_kwargs) = (callback,
                                                                                 callback_args,
                                                                                 callback_kwargs)

    def recv(self, n=None):
        '''default recv method'''
        raise RuntimeError('no recv() method supplied')

    def close(self, n=None):
        '''default close method'''
        raise RuntimeError('no close() method supplied')

    def write(self, buf):
        '''default write method'''
        raise RuntimeError('no write() method supplied')


    def select(self, timeout):
        '''wait for up to timeout seconds for more data'''
        if self.fd is None:
            time.sleep(min(timeout,0.5))
            return True
        try:
            (rin, win, xin) = select.select([self.fd], [], [], timeout)
        except select.error:
            return False
        return len(rin) == 1

    def pre_message(self):
        '''default pre message call'''
        return

    def set_rtscts(self, enable):
        '''enable/disable RTS/CTS if applicable'''
        return

    def post_message(self, msg):
        '''default post message call'''
        if '_posted' in msg.__dict__:
            return
        msg._posted = True
        msg._timestamp = time.time()
        type = msg.get_type()
        if type != 'HEARTBEAT' or (msg.type != mavlink.MAV_TYPE_GCS and msg.type != mavlink.MAV_TYPE_GIMBAL):
            self.messages[type] = msg

        if 'usec' in msg.__dict__:
            self.uptime = msg.usec * 1.0e-6
        if 'time_boot_ms' in msg.__dict__:
            self.uptime = msg.time_boot_ms * 1.0e-3

        if self._timestamp is not None:
            if self.notimestamps:
                msg._timestamp = self.uptime
            else:
                msg._timestamp = self._timestamp

        src_system = msg.get_srcSystem()
        src_component = msg.get_srcComponent()
        src_tuple = (src_system, src_component)
        radio_tuple = (ord('3'), ord('D'))
        if not (src_tuple == radio_tuple or msg.get_type() == 'BAD_DATA'):
            if not src_tuple in self.last_seq:
                last_seq = -1
            else:
                last_seq = self.last_seq[src_tuple]
            seq = (last_seq+1) % 256
            seq2 = msg.get_seq()
            if seq != seq2 and last_seq != -1:
                diff = (seq2 - seq) % 256
                self.mav_loss += diff
                #print("lost %u seq=%u seq2=%u last_seq=%u src_system=%u %s" % (diff, seq, seq2, last_seq, src_system, msg.get_type()))
            self.last_seq[src_tuple] = seq2
            self.mav_count += 1
        
        self.timestamp = msg._timestamp
        if type == 'HEARTBEAT' and msg.get_srcComponent() != mavlink.MAV_COMP_ID_GIMBAL:
            self.target_system = msg.get_srcSystem()
            self.target_component = msg.get_srcComponent()
            if mavlink.WIRE_PROTOCOL_VERSION == '1.0' and msg.type != mavlink.MAV_TYPE_GCS:
                self.flightmode = mode_string_v10(msg)
                self.mav_type = msg.type
                self.base_mode = msg.base_mode
        elif type == 'PARAM_VALUE':
            s = str(msg.param_id)
            self.params[str(msg.param_id)] = msg.param_value
            if msg.param_index+1 == msg.param_count:
                self.param_fetch_in_progress = False
                self.param_fetch_complete = True
        elif type == 'SYS_STATUS' and mavlink.WIRE_PROTOCOL_VERSION == '0.9':
            self.flightmode = mode_string_v09(msg)
        elif type == 'GPS_RAW':
            if self.messages['HOME'].fix_type < 2:
                self.messages['HOME'] = msg
        elif type == 'GPS_RAW_INT':
            if self.messages['HOME'].fix_type < 3:
                self.messages['HOME'] = msg
        for hook in self.message_hooks:
            hook(self, msg)


    def packet_loss(self):
        '''packet loss as a percentage'''
        if self.mav_count == 0:
            return 0
        return (100.0*self.mav_loss)/(self.mav_count+self.mav_loss)


    def recv_msg(self):
        '''message receive routine'''
        self.pre_message()
        while True:
            n = self.mav.bytes_needed()
            s = self.recv(n)
            numnew = len(s)

            if numnew != 0:
                if self.logfile_raw:
                    self.logfile_raw.write(str(s))
                if self.first_byte:
                    self.auto_mavlink_version(s)

            # We always call parse_char even if the new string is empty, because the existing message buf might already have some valid packet
            # we can extract
            msg = self.mav.parse_char(s)
            if msg:
                if self.logfile and  msg.get_type() != 'BAD_DATA' :
                    usec = int(time.time() * 1.0e6) & ~3
                    self.logfile.write(str(struct.pack('>Q', usec) + msg.get_msgbuf()))
                self.post_message(msg)
                return msg
            else:
                # if we failed to parse any messages _and_ no new bytes arrived, return immediately so the client has the option to
                # timeout
                if numnew == 0:
                    return None
                
    def recv_match(self, condition=None, type=None, blocking=False, timeout=None):
        '''recv the next MAVLink message that matches the given condition
        type can be a string or a list of strings'''
        if type is not None and not isinstance(type, list):
            type = [type]
        start_time = time.time()
        while True:
            if timeout is not None:
                now = time.time()
                if now < start_time:
                    start_time = now # If an external process rolls back system time, we should not spin forever.
                if start_time + timeout < time.time():
                    return None
            m = self.recv_msg()
            if m is None:
                if blocking:
                    for hook in self.idle_hooks:
                        hook(self)
                    if timeout is None:
                        self.select(0.05)
                    else:
                        self.select(timeout/2)
                    continue
                return None
            if type is not None and not m.get_type() in type:
                continue
            if not evaluate_condition(condition, self.messages):
                continue
            return m

    def check_condition(self, condition):
        '''check if a condition is true'''
        return evaluate_condition(condition, self.messages)

    def mavlink10(self):
        '''return True if using MAVLink 1.0'''
        return self.WIRE_PROTOCOL_VERSION == "1.0"

    def setup_logfile(self, logfile, mode='w'):
        '''start logging to the given logfile, with timestamps'''
        self.logfile = open(logfile, mode=mode)

    def setup_logfile_raw(self, logfile, mode='w'):
        '''start logging raw bytes to the given logfile, without timestamps'''
        self.logfile_raw = open(logfile, mode=mode)

    def wait_heartbeat(self, blocking=True):
        '''wait for a heartbeat so we know the target system IDs'''
        return self.recv_match(type='HEARTBEAT', blocking=blocking)

    def param_fetch_all(self):
        '''initiate fetch of all parameters'''
        if time.time() - getattr(self, 'param_fetch_start', 0) < 2.0:
            # don't fetch too often
            return
        self.param_fetch_start = time.time()
        self.param_fetch_in_progress = True
        self.mav.param_request_list_send(self.target_system, self.target_component)

    def param_fetch_one(self, name):
        '''initiate fetch of one parameter'''
        try:
            idx = int(name)
            self.mav.param_request_read_send(self.target_system, self.target_component, "", idx)
        except Exception:
            self.mav.param_request_read_send(self.target_system, self.target_component, name, -1)

    def time_since(self, mtype):
        '''return the time since the last message of type mtype was received'''
        if not mtype in self.messages:
            return time.time() - self.start_time
        return time.time() - self.messages[mtype]._timestamp

    def param_set_send(self, parm_name, parm_value, parm_type=None):
        '''wrapper for parameter set'''
        if self.mavlink10():
            if parm_type == None:
                parm_type = mavlink.MAVLINK_TYPE_FLOAT
            self.mav.param_set_send(self.target_system, self.target_component,
                                    parm_name, parm_value, parm_type)
        else:
            self.mav.param_set_send(self.target_system, self.target_component,
                                    parm_name, parm_value)

    def waypoint_request_list_send(self):
        '''wrapper for waypoint_request_list_send'''
        if self.mavlink10():
            self.mav.mission_request_list_send(self.target_system, self.target_component)
        else:
            self.mav.waypoint_request_list_send(self.target_system, self.target_component)

    def waypoint_clear_all_send(self):
        '''wrapper for waypoint_clear_all_send'''
        if self.mavlink10():
            self.mav.mission_clear_all_send(self.target_system, self.target_component)
        else:
            self.mav.waypoint_clear_all_send(self.target_system, self.target_component)

    def waypoint_request_send(self, seq):
        '''wrapper for waypoint_request_send'''
        if self.mavlink10():
            self.mav.mission_request_send(self.target_system, self.target_component, seq)
        else:
            self.mav.waypoint_request_send(self.target_system, self.target_component, seq)

    def waypoint_set_current_send(self, seq):
        '''wrapper for waypoint_set_current_send'''
        if self.mavlink10():
            self.mav.mission_set_current_send(self.target_system, self.target_component, seq)
        else:
            self.mav.waypoint_set_current_send(self.target_system, self.target_component, seq)

    def waypoint_current(self):
        '''return current waypoint'''
        if self.mavlink10():
            m = self.recv_match(type='MISSION_CURRENT', blocking=True)
        else:
            m = self.recv_match(type='WAYPOINT_CURRENT', blocking=True)
        return m.seq

    def waypoint_count_send(self, seq):
        '''wrapper for waypoint_count_send'''
        if self.mavlink10():
            self.mav.mission_count_send(self.target_system, self.target_component, seq)
        else:
            self.mav.waypoint_count_send(self.target_system, self.target_component, seq)

    def set_mode_flag(self, flag, enable):
        '''
        Enables/ disables MAV_MODE_FLAG
        @param flag The mode flag, 
          see MAV_MODE_FLAG enum
        @param enable Enable the flag, (True/False)
        '''
        if self.mavlink10():
            mode = self.base_mode
            if (enable == True):
                mode = mode | flag
            elif (enable == False):
                mode = mode & ~flag
            self.mav.command_long_send(self.target_system, self.target_component,
                                           mavlink.MAV_CMD_DO_SET_MODE, 0,
                                           mode,
                                           0, 0, 0, 0, 0, 0)
        else:
            print("Set mode flag not supported")

    def set_mode_auto(self):
        '''enter auto mode'''
        if self.mavlink10():
            self.mav.command_long_send(self.target_system, self.target_component,
                                       mavlink.MAV_CMD_MISSION_START, 0, 0, 0, 0, 0, 0, 0, 0)
        else:
            MAV_ACTION_SET_AUTO = 13
            self.mav.action_send(self.target_system, self.target_component, MAV_ACTION_SET_AUTO)

    def mode_mapping(self):
        '''return dictionary mapping mode names to numbers, or None if unknown'''
        mav_type = self.field('HEARTBEAT', 'type', self.mav_type)
        if mav_type is None:
            return None
        map = None
        if mav_type in [mavlink.MAV_TYPE_QUADROTOR,
                        mavlink.MAV_TYPE_HELICOPTER,
                        mavlink.MAV_TYPE_HEXAROTOR,
                        mavlink.MAV_TYPE_OCTOROTOR,
                        mavlink.MAV_TYPE_TRICOPTER]:
            map = mode_mapping_acm
        if mav_type == mavlink.MAV_TYPE_FIXED_WING:
            map = mode_mapping_apm
        if mav_type == mavlink.MAV_TYPE_GROUND_ROVER:
            map = mode_mapping_rover
        if mav_type == mavlink.MAV_TYPE_ANTENNA_TRACKER:
            map = mode_mapping_tracker
        if map is None:
            return None
        inv_map = dict((a, b) for (b, a) in list(map.items()))
        return inv_map

    def set_mode(self, mode):
        '''enter arbitrary mode'''
        if isinstance(mode, str):
            map = self.mode_mapping()
            if map is None or mode not in map:
                print("Unknown mode '%s'" % mode)
                return
            mode = map[mode]
        self.mav.set_mode_send(self.target_system,
                               mavlink.MAV_MODE_FLAG_CUSTOM_MODE_ENABLED,
                               mode)

    def set_mode_rtl(self):
        '''enter RTL mode'''
        if self.mavlink10():
            self.mav.command_long_send(self.target_system, self.target_component,
                                       mavlink.MAV_CMD_NAV_RETURN_TO_LAUNCH, 0, 0, 0, 0, 0, 0, 0, 0)
        else:
            MAV_ACTION_RETURN = 3
            self.mav.action_send(self.target_system, self.target_component, MAV_ACTION_RETURN)

    def set_mode_manual(self):
        '''enter MANUAL mode'''
        if self.mavlink10():
            self.mav.command_long_send(self.target_system, self.target_component,
                                       mavlink.MAV_CMD_DO_SET_MODE, 0,
                                       mavlink.MAV_MODE_MANUAL_ARMED,
                                       0, 0, 0, 0, 0, 0)
        else:
            MAV_ACTION_SET_MANUAL = 12
            self.mav.action_send(self.target_system, self.target_component, MAV_ACTION_SET_MANUAL)

    def set_mode_fbwa(self):
        '''enter FBWA mode'''
        if self.mavlink10():
            self.mav.command_long_send(self.target_system, self.target_component,
                                       mavlink.MAV_CMD_DO_SET_MODE, 0,
                                       mavlink.MAV_MODE_STABILIZE_ARMED,
                                       0, 0, 0, 0, 0, 0)
        else:
            print("Forcing FBWA not supported")

    def set_mode_loiter(self):
        '''enter LOITER mode'''
        if self.mavlink10():
            self.mav.command_long_send(self.target_system, self.target_component,
                                       mavlink.MAV_CMD_NAV_LOITER_UNLIM, 0, 0, 0, 0, 0, 0, 0, 0)
        else:
            MAV_ACTION_LOITER = 27
            self.mav.action_send(self.target_system, self.target_component, MAV_ACTION_LOITER)

    def set_servo(self, channel, pwm):
        '''set a servo value'''
        self.mav.command_long_send(self.target_system, self.target_component,
                                   mavlink.MAV_CMD_DO_SET_SERVO, 0,
                                   channel, pwm,
                                   0, 0, 0, 0, 0)


    def set_relay(self, relay_pin=0, state=True):
        '''Set relay_pin to value of state'''
        if self.mavlink10():
            self.mav.command_long_send(
                self.target_system,  # target_system
                self.target_component, # target_component
                mavlink.MAV_CMD_DO_SET_RELAY, # command
                0, # Confirmation
                relay_pin, # Relay Number
                int(state), # state (1 to indicate arm)
                0, # param3 (all other params meaningless)
                0, # param4
                0, # param5
                0, # param6
                0) # param7
        else:
            print("Setting relays not supported.")

    def calibrate_level(self):
        '''calibrate accels (1D version)'''
        self.mav.command_long_send(self.target_system, self.target_component,
                                   mavlink.MAV_CMD_PREFLIGHT_CALIBRATION, 0,
                                   1, 1, 0, 0, 0, 0, 0)

    def calibrate_pressure(self):
        '''calibrate pressure'''
        if self.mavlink10():
            self.mav.command_long_send(self.target_system, self.target_component,
                                       mavlink.MAV_CMD_PREFLIGHT_CALIBRATION, 0,
                                       0, 0, 1, 0, 0, 0, 0)
        else:
            MAV_ACTION_CALIBRATE_PRESSURE = 20
            self.mav.action_send(self.target_system, self.target_component, MAV_ACTION_CALIBRATE_PRESSURE)

    def reboot_autopilot(self, hold_in_bootloader=False):
        '''reboot the autopilot'''
        if self.mavlink10():
            if hold_in_bootloader:
                param1 = 3
            else:
                param1 = 1
            self.mav.command_long_send(self.target_system, self.target_component,
                                       mavlink.MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN, 0,
                                       param1, 0, 0, 0, 0, 0, 0)
            # send an old style reboot immediately afterwards in case it is an older firmware
            # that doesn't understand the new convention
            self.mav.command_long_send(self.target_system, self.target_component,
                                       mavlink.MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN, 0,
                                       1, 0, 0, 0, 0, 0, 0)

    def wait_gps_fix(self):
        self.recv_match(type='VFR_HUD', blocking=True)
        if self.mavlink10():
            self.recv_match(type='GPS_RAW_INT', blocking=True,
                            condition='GPS_RAW_INT.fix_type==3 and GPS_RAW_INT.lat != 0 and GPS_RAW_INT.alt != 0')
        else:
            self.recv_match(type='GPS_RAW', blocking=True,
                            condition='GPS_RAW.fix_type==2 and GPS_RAW.lat != 0 and GPS_RAW.alt != 0')

    def location(self, relative_alt=False):
        '''return current location'''
        self.wait_gps_fix()
        # wait for another VFR_HUD, to ensure we have correct altitude
        self.recv_match(type='VFR_HUD', blocking=True)
        self.recv_match(type='GLOBAL_POSITION_INT', blocking=True)
        if relative_alt:
            alt = self.messages['GLOBAL_POSITION_INT'].relative_alt*0.001
        else:
            alt = self.messages['VFR_HUD'].alt
        return location(self.messages['GPS_RAW_INT'].lat*1.0e-7,
                        self.messages['GPS_RAW_INT'].lon*1.0e-7,
                        alt,
                        self.messages['VFR_HUD'].heading)

    def arducopter_arm(self):
        '''arm motors (arducopter only)'''
        if self.mavlink10():
            self.mav.command_long_send(
                self.target_system,  # target_system
                self.target_component,
                mavlink.MAV_CMD_COMPONENT_ARM_DISARM, # command
                0, # confirmation
                1, # param1 (1 to indicate arm)
                0, # param2 (all other params meaningless)
                0, # param3
                0, # param4
                0, # param5
                0, # param6
                0) # param7

    def arducopter_disarm(self):
        '''calibrate pressure'''
        if self.mavlink10():
            self.mav.command_long_send(
                self.target_system,  # target_system
                self.target_component,
                mavlink.MAV_CMD_COMPONENT_ARM_DISARM, # command
                0, # confirmation
                0, # param1 (0 to indicate disarm)
                0, # param2 (all other params meaningless)
                0, # param3
                0, # param4
                0, # param5
                0, # param6
                0) # param7

    def motors_armed(self):
        '''return true if motors armed'''
        if not 'HEARTBEAT' in self.messages:
            return False
        m = self.messages['HEARTBEAT']
        return (m.base_mode & mavlink.MAV_MODE_FLAG_SAFETY_ARMED) != 0

    def motors_armed_wait(self):
        '''wait for motors to be armed'''
        while True:
            m = self.wait_heartbeat()
            if self.motors_armed():
                return

    def motors_disarmed_wait(self):
        '''wait for motors to be disarmed'''
        while True:
            m = self.wait_heartbeat()
            if not self.motors_armed():
                return


    def field(self, type, field, default=None):
        '''convenient function for returning an arbitrary MAVLink
           field with a default'''
        if not type in self.messages:
            return default
        return getattr(self.messages[type], field, default)

    def param(self, name, default=None):
        '''convenient function for returning an arbitrary MAVLink
           parameter with a default'''
        if not name in self.params:
            return default
        return self.params[name]

def set_close_on_exec(fd):
    '''set the clone on exec flag on a file descriptor. Ignore exceptions'''
    try:
        import fcntl
        flags = fcntl.fcntl(fd, fcntl.F_GETFD)
        flags |= fcntl.FD_CLOEXEC
        fcntl.fcntl(fd, fcntl.F_SETFD, flags)
    except Exception:
        pass

class mavserial(mavfile):
    '''a serial mavlink port'''
    def __init__(self, device, baud=115200, autoreconnect=False, source_system=255, use_native=default_native):
        import serial
        if ',' in device and not os.path.exists(device):
            device, baud = device.split(',')
        self.baud = baud
        self.device = device
        self.autoreconnect = autoreconnect
        # we rather strangely set the baudrate initially to 1200, then change to the desired
        # baudrate. This works around a kernel bug on some Linux kernels where the baudrate
        # is not set correctly
        self.port = serial.Serial(self.device, 1200, timeout=0,
                                  dsrdtr=False, rtscts=False, xonxoff=False)
        try:
            fd = self.port.fileno()
            set_close_on_exec(fd)
        except Exception:
            fd = None
        self.set_baudrate(self.baud)
        mavfile.__init__(self, fd, device, source_system=source_system, use_native=use_native)
        self.rtscts = False

    def set_rtscts(self, enable):
        '''enable/disable RTS/CTS if applicable'''
        self.port.setRtsCts(enable)
        self.rtscts = enable

    def set_baudrate(self, baudrate):
        '''set baudrate'''
        self.port.setBaudrate(baudrate)
    
    def close(self):
        self.port.close()

    def recv(self,n=None):
        if n is None:
            n = self.mav.bytes_needed()
        if self.fd is None:
            waiting = self.port.inWaiting()
            if waiting < n:
                n = waiting
        ret = self.port.read(n)
        return ret

    def write(self, buf):
        try:
            if not isinstance(buf, str):
                buf = str(buf)
            return self.port.write(buf)
        except Exception:
            if not self.portdead:
                print("Device %s is dead" % self.device)
            self.portdead = True
            if self.autoreconnect:
                self.reset()
            return -1
            
    def reset(self):
        import serial
        try:
            newport = serial.Serial(self.device, self.baud, timeout=0,
                                    dsrdtr=False, rtscts=False, xonxoff=False)
            self.port.close()
            self.port = newport
            print("Device %s reopened OK" % self.device)
            self.portdead = False
            try:
                self.fd = self.port.fileno()
            except Exception:
                self.fd = None
            if self.rtscts:
                self.set_rtscts(self.rtscts)
            return True
        except Exception:
            return False
        

class mavudp(mavfile):
    '''a UDP mavlink socket'''
    def __init__(self, device, input=True, broadcast=False, source_system=255, use_native=default_native):
        a = device.split(':')
        if len(a) != 2:
            print("UDP ports must be specified as host:port")
            sys.exit(1)
        self.port = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.udp_server = input
        if input:
            self.port.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.port.bind((a[0], int(a[1])))
        else:
            self.destination_addr = (a[0], int(a[1]))
            if broadcast:
                self.port.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
        set_close_on_exec(self.port.fileno())
        self.port.setblocking(0)
        self.last_address = None
        mavfile.__init__(self, self.port.fileno(), device, source_system=source_system, input=input, use_native=use_native)

    def close(self):
        self.port.close()

    def recv(self,n=None):
        try:
            data, self.last_address = self.port.recvfrom(300)
        except socket.error as e:
            if e.errno in [ errno.EAGAIN, errno.EWOULDBLOCK, errno.ECONNREFUSED ]:
                return ""
            raise
        return data

    def write(self, buf):
        try:
            if self.udp_server:
                if self.last_address:
                    self.port.sendto(buf, self.last_address)
            else:
                self.port.sendto(buf, self.destination_addr)
        except socket.error:
            pass

    def recv_msg(self):
        '''message receive routine for UDP link'''
        self.pre_message()
        s = self.recv()
        if len(s) == 0:
            return None
        if self.first_byte:
            self.auto_mavlink_version(s)
        msg = self.mav.parse_buffer(s)
        if msg is not None:
            for m in msg:
                self.post_message(m)
            return msg[0]
        return None


class mavtcp(mavfile):
    '''a TCP mavlink socket'''
    def __init__(self, device, source_system=255, retries=3, use_native=default_native):
        a = device.split(':')
        if len(a) != 2:
            print("TCP ports must be specified as host:port")
            sys.exit(1)
        self.port = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.destination_addr = (a[0], int(a[1]))
        while retries >= 0:
            retries -= 1
            if retries <= 0:
                self.port.connect(self.destination_addr)
            else:
                try:
                    self.port.connect(self.destination_addr)
                    break
                except Exception as e:
                    if retries > 0:
                        print(e, "sleeping")
                        time.sleep(1)
                    continue
        self.port.setblocking(0)
        set_close_on_exec(self.port.fileno())
        self.port.setsockopt(socket.SOL_TCP, socket.TCP_NODELAY, 1)
        mavfile.__init__(self, self.port.fileno(), "tcp:" + device, source_system=source_system, use_native=use_native)

    def close(self):
        self.port.close()

    def recv(self,n=None):
        if n is None:
            n = self.mav.bytes_needed()
        try:
            data = self.port.recv(n)
        except socket.error as e:
            if e.errno in [ errno.EAGAIN, errno.EWOULDBLOCK ]:
                return ""
            raise
        return data

    def write(self, buf):
        try:
            self.port.send(buf)
        except socket.error:
            pass


class mavlogfile(mavfile):
    '''a MAVLink logfile reader/writer'''
    def __init__(self, filename, planner_format=None,
                 write=False, append=False,
                 robust_parsing=True, notimestamps=False, source_system=255, use_native=default_native):
        self.filename = filename
        self.writeable = write
        self.robust_parsing = robust_parsing
        self.planner_format = planner_format
        self._two64 = math.pow(2.0, 63)
        mode = 'rb'
        if self.writeable:
            if append:
                mode = 'ab'
            else:
                mode = 'wb'
        self.f = open(filename, mode)
        self.filesize = os.path.getsize(filename)
        self.percent = 0
        mavfile.__init__(self, None, filename, source_system=source_system, notimestamps=notimestamps, use_native=use_native)
        if self.notimestamps:
            self._timestamp = 0
        else:
            self._timestamp = time.time()
        self.stop_on_EOF = True
        self._last_message = None
        self._last_timestamp = None

    def close(self):
        self.f.close()

    def recv(self,n=None):
        if n is None:
            n = self.mav.bytes_needed()
        return self.f.read(n)

    def write(self, buf):
        self.f.write(buf)

    def scan_timestamp(self, tbuf):
        '''scan forward looking in a tlog for a timestamp in a reasonable range'''
        while True:
            (tusec,) = struct.unpack('>Q', tbuf)
            t = tusec * 1.0e-6
            if abs(t - self._last_timestamp) <= 3*24*60*60:
                break
            c = self.f.read(1)
            if len(c) != 1:
                break
            tbuf = tbuf[1:] + c
        return t


    def pre_message(self):
        '''read timestamp if needed'''
        # read the timestamp
        if self.filesize != 0:
            self.percent = (100.0 * self.f.tell()) / self.filesize
        if self.notimestamps:
            return
        if self.planner_format:
            tbuf = self.f.read(21)
            if len(tbuf) != 21 or tbuf[0] != '-' or tbuf[20] != ':':
                raise RuntimeError('bad planner timestamp %s' % tbuf)
            hnsec = self._two64 + float(tbuf[0:20])
            t = hnsec * 1.0e-7         # convert to seconds
            t -= 719163 * 24 * 60 * 60 # convert to 1970 base
            self._link = 0
        else:
            tbuf = self.f.read(8)
            if len(tbuf) != 8:
                return
            (tusec,) = struct.unpack('>Q', tbuf)
            t = tusec * 1.0e-6
            if (self._last_timestamp is not None and
                self._last_message.get_type() == "BAD_DATA" and
                abs(t - self._last_timestamp) > 3*24*60*60):
                t = self.scan_timestamp(tbuf)
            self._link = tusec & 0x3
        self._timestamp = t

    def post_message(self, msg):
        '''add timestamp to message'''
        # read the timestamp
        super(mavlogfile, self).post_message(msg)
        if self.planner_format:
            self.f.read(1) # trailing newline
        self.timestamp = msg._timestamp
        self._last_message = msg
        if msg.get_type() != "BAD_DATA":
            self._last_timestamp = msg._timestamp


class mavmemlog(mavfile):
    '''a MAVLink log in memory. This allows loading a log into
    memory to make it easier to do multiple sweeps over a log'''
    def __init__(self, mav):
        mavfile.__init__(self, None, 'memlog')
        self._msgs = []
        self._index = 0
        self._count = 0
        self.messages = {}
        while True:
            m = mav.recv_msg()
            if m is None:
                break
            self._msgs.append(m)
        self._count = len(self._msgs)

    def recv_msg(self):
        '''message receive routine'''
        if self._index >= self._count:
            return None
        m = self._msgs[self._index]
        self._index += 1
        self.percent = (100.0 * self._index) / self._count
        self.messages[m.get_type()] = m
        return m

    def rewind(self):
        '''rewind to start'''
        self._index = 0
        self.percent = 0
        self.messages = {}

class mavchildexec(mavfile):
    '''a MAVLink child processes reader/writer'''
    def __init__(self, filename, source_system=255, use_native=default_native):
        from subprocess import Popen, PIPE
        import fcntl
        
        self.filename = filename
        self.child = Popen(filename, shell=False, stdout=PIPE, stdin=PIPE, bufsize=0)
        self.fd = self.child.stdout.fileno()

        fl = fcntl.fcntl(self.fd, fcntl.F_GETFL)
        fcntl.fcntl(self.fd, fcntl.F_SETFL, fl | os.O_NONBLOCK)

        fl = fcntl.fcntl(self.child.stdout.fileno(), fcntl.F_GETFL)
        fcntl.fcntl(self.child.stdout.fileno(), fcntl.F_SETFL, fl | os.O_NONBLOCK)

        mavfile.__init__(self, self.fd, filename, source_system=source_system, use_native=use_native)

    def close(self):
        self.child.close()

    def recv(self,n=None):
        try:
            x = self.child.stdout.read(1)
        except Exception:
            return ''
        return x

    def write(self, buf):
        self.child.stdin.write(buf)


def mavlink_connection(device, baud=115200, source_system=255,
                       planner_format=None, write=False, append=False,
                       robust_parsing=True, notimestamps=False, input=True,
                       dialect=None, autoreconnect=False, zero_time_base=False,
                       retries=3, use_native=default_native):
    '''open a serial, UDP, TCP or file mavlink connection'''
    global mavfile_global

    if dialect is not None:
        set_dialect(dialect)
    if device.startswith('tcp:'):
        return mavtcp(device[4:], source_system=source_system, retries=retries, use_native=use_native)
    if device.startswith('udpin:'):
        return mavudp(device[6:], input=True, source_system=source_system, use_native=use_native)
    if device.startswith('udpout:'):
        return mavudp(device[7:], input=False, source_system=source_system, use_native=use_native)
    # For legacy purposes we accept the following syntax and let the caller to specify direction
    if device.startswith('udp:'):
        return mavudp(device[4:], input=input, source_system=source_system, use_native=use_native)

    if device.lower().endswith('.bin') or device.lower().endswith('.px4log'):
        # support dataflash logs
        from pymavlink import DFReader
        m = DFReader.DFReader_binary(device, zero_time_base=zero_time_base)
        mavfile_global = m
        return m

    if device.endswith('.log'):
        # support dataflash text logs
        from pymavlink import DFReader
        if DFReader.DFReader_is_text_log(device):
            m = DFReader.DFReader_text(device, zero_time_base=zero_time_base)
            mavfile_global = m
            return m    

    # list of suffixes to prevent setting DOS paths as UDP sockets
    logsuffixes = ['mavlink', 'log', 'raw', 'tlog' ]
    suffix = device.split('.')[-1].lower()
    if device.find(':') != -1 and not suffix in logsuffixes:
        return mavudp(device, source_system=source_system, input=input, use_native=use_native)
    if os.path.isfile(device):
        if device.endswith(".elf") or device.find("/bin/") != -1:
            print("executing '%s'" % device)
            return mavchildexec(device, source_system=source_system, use_native=use_native)
        else:
            return mavlogfile(device, planner_format=planner_format, write=write,
                              append=append, robust_parsing=robust_parsing, notimestamps=notimestamps,
                              source_system=source_system, use_native=use_native)
    return mavserial(device, baud=baud, source_system=source_system, autoreconnect=autoreconnect, use_native=use_native)

class periodic_event(object):
    '''a class for fixed frequency events'''
    def __init__(self, frequency):
        self.frequency = float(frequency)
        self.last_time = time.time()

    def force(self):
        '''force immediate triggering'''
        self.last_time = 0
        
    def trigger(self):
        '''return True if we should trigger now'''
        tnow = time.time()

        if tnow < self.last_time:
            print("Warning, time moved backwards. Restarting timer.")
            self.last_time = tnow

        if self.last_time + (1.0/self.frequency) <= tnow:
            self.last_time = tnow
            return True
        return False


try:
    from curses import ascii
    have_ascii = True
except:
    have_ascii = False

def is_printable(c):
    '''see if a character is printable'''
    global have_ascii
    if have_ascii:
        return ascii.isprint(c)
    if isinstance(c, int):
        ic = c
    else:
        ic = ord(c)
    return ic >= 32 and ic <= 126

def all_printable(buf):
    '''see if a string is all printable'''
    for c in buf:
        if not is_printable(c) and not c in ['\r', '\n', '\t']:
            return False
    return True

class SerialPort(object):
    '''auto-detected serial port'''
    def __init__(self, device, description=None, hwid=None):
        self.device = device
        self.description = description
        self.hwid = hwid

    def __str__(self):
        ret = self.device
        if self.description is not None:
            ret += " : " + self.description
        if self.hwid is not None:
            ret += " : " + self.hwid
        return ret

def auto_detect_serial_win32(preferred_list=['*']):
    '''try to auto-detect serial ports on win32'''
    try:
        from serial.tools.list_ports_windows import comports
        list = sorted(comports())
    except:
        return []
    ret = []
    others = []
    for port, description, hwid in list:
        matches = False
        p = SerialPort(port, description=description, hwid=hwid)
        for preferred in preferred_list:
            if fnmatch.fnmatch(description, preferred) or fnmatch.fnmatch(hwid, preferred):
                matches = True
        if matches:
            ret.append(p)
        else:
            others.append(p)
    if len(ret) > 0:
        return ret
    # now the rest
    ret.extend(others)
    return ret
        

        

def auto_detect_serial_unix(preferred_list=['*']):
    '''try to auto-detect serial ports on unix'''
    import glob
    glist = glob.glob('/dev/ttyS*') + glob.glob('/dev/ttyUSB*') + glob.glob('/dev/ttyACM*') + glob.glob('/dev/serial/by-id/*')
    ret = []
    others = []
    # try preferred ones first
    for d in glist:
        matches = False
        for preferred in preferred_list:
            if fnmatch.fnmatch(d, preferred):
                matches = True
        if matches:
            ret.append(SerialPort(d))
        else:
            others.append(SerialPort(d))
    if len(ret) > 0:
        return ret
    ret.extend(others)
    return ret

def auto_detect_serial(preferred_list=['*']):
    '''try to auto-detect serial port'''
    # see if 
    if os.name == 'nt':
        return auto_detect_serial_win32(preferred_list=preferred_list)
    return auto_detect_serial_unix(preferred_list=preferred_list)

def mode_string_v09(msg):
    '''mode string for 0.9 protocol'''
    mode = msg.mode
    nav_mode = msg.nav_mode

    MAV_MODE_UNINIT = 0
    MAV_MODE_MANUAL = 2
    MAV_MODE_GUIDED = 3
    MAV_MODE_AUTO = 4
    MAV_MODE_TEST1 = 5
    MAV_MODE_TEST2 = 6
    MAV_MODE_TEST3 = 7

    MAV_NAV_GROUNDED = 0
    MAV_NAV_LIFTOFF = 1
    MAV_NAV_HOLD = 2
    MAV_NAV_WAYPOINT = 3
    MAV_NAV_VECTOR = 4
    MAV_NAV_RETURNING = 5
    MAV_NAV_LANDING = 6
    MAV_NAV_LOST = 7
    MAV_NAV_LOITER = 8
    
    cmode = (mode, nav_mode)
    mapping = {
        (MAV_MODE_UNINIT, MAV_NAV_GROUNDED)  : "INITIALISING",
        (MAV_MODE_MANUAL, MAV_NAV_VECTOR)    : "MANUAL",
        (MAV_MODE_TEST3,  MAV_NAV_VECTOR)    : "CIRCLE",
        (MAV_MODE_GUIDED, MAV_NAV_VECTOR)    : "GUIDED",
        (MAV_MODE_TEST1,  MAV_NAV_VECTOR)    : "STABILIZE",
        (MAV_MODE_TEST2,  MAV_NAV_LIFTOFF)   : "FBWA",
        (MAV_MODE_AUTO,   MAV_NAV_WAYPOINT)  : "AUTO",
        (MAV_MODE_AUTO,   MAV_NAV_RETURNING) : "RTL",
        (MAV_MODE_AUTO,   MAV_NAV_LOITER)    : "LOITER",
        (MAV_MODE_AUTO,   MAV_NAV_LIFTOFF)   : "TAKEOFF",
        (MAV_MODE_AUTO,   MAV_NAV_LANDING)   : "LANDING",
        (MAV_MODE_AUTO,   MAV_NAV_HOLD)      : "LOITER",
        (MAV_MODE_GUIDED, MAV_NAV_VECTOR)    : "GUIDED",
        (MAV_MODE_GUIDED, MAV_NAV_WAYPOINT)  : "GUIDED",
        (100,             MAV_NAV_VECTOR)    : "STABILIZE",
        (101,             MAV_NAV_VECTOR)    : "ACRO",
        (102,             MAV_NAV_VECTOR)    : "ALT_HOLD",
        (107,             MAV_NAV_VECTOR)    : "CIRCLE",
        (109,             MAV_NAV_VECTOR)    : "LAND",
        }
    if cmode in mapping:
        return mapping[cmode]
    return "Mode(%s,%s)" % cmode

mode_mapping_apm = {
    0 : 'MANUAL',
    1 : 'CIRCLE',
    2 : 'STABILIZE',
    3 : 'TRAINING',
    4 : 'ACRO',
    5 : 'FBWA',
    6 : 'FBWB',
    7 : 'CRUISE',
    8 : 'AUTOTUNE',
    10 : 'AUTO',
    11 : 'RTL',
    12 : 'LOITER',
    14 : 'LAND',
    15 : 'GUIDED',
    16 : 'INITIALISING'
    }
mode_mapping_acm = {
    0 : 'STABILIZE',
    1 : 'ACRO',
    2 : 'ALT_HOLD',
    3 : 'AUTO',
    4 : 'GUIDED',
    5 : 'LOITER',
    6 : 'RTL',
    7 : 'CIRCLE',
    8 : 'POSITION',
    9 : 'LAND',
    10 : 'OF_LOITER',
    11 : 'DRIFT',
    13 : 'SPORT',
    14 : 'FLIP',
    15 : 'AUTOTUNE',
    16 : 'POSHOLD'
    }
mode_mapping_rover = {
    0 : 'MANUAL',
    2 : 'LEARNING',
    3 : 'STEERING',
    4 : 'HOLD',
    10 : 'AUTO',
    11 : 'RTL',
    15 : 'GUIDED',
    16 : 'INITIALISING'
    }

mode_mapping_tracker = {
    0 : 'MANUAL',
    1 : 'STOP',
    2 : 'SCAN',
    10 : 'AUTO',
    16 : 'INITIALISING'
    }

mode_mapping_px4 = {
    0 : 'MANUAL',
    1 : 'ATTITUDE',
    2 : 'EASY',
    3 : 'AUTO'
    }


def mode_mapping_byname(mav_type):
    '''return dictionary mapping mode names to numbers, or None if unknown'''
    map = None
    if mav_type in [mavlink.MAV_TYPE_QUADROTOR,
                    mavlink.MAV_TYPE_HELICOPTER,
                    mavlink.MAV_TYPE_HEXAROTOR,
                    mavlink.MAV_TYPE_OCTOROTOR,
                    mavlink.MAV_TYPE_TRICOPTER]:
        map = mode_mapping_acm
    if mav_type == mavlink.MAV_TYPE_FIXED_WING:
        map = mode_mapping_apm
    if mav_type == mavlink.MAV_TYPE_GROUND_ROVER:
        map = mode_mapping_rover
    if mav_type == mavlink.MAV_TYPE_ANTENNA_TRACKER:
        map = mode_mapping_tracker
    if map is None:
        return None
    inv_map = dict((a, b) for (b, a) in map.items())
    return inv_map

def mode_mapping_bynumber(mav_type):
    '''return dictionary mapping mode numbers to name, or None if unknown'''
    map = None
    if mav_type in [mavlink.MAV_TYPE_QUADROTOR,
                    mavlink.MAV_TYPE_HELICOPTER,
                    mavlink.MAV_TYPE_HEXAROTOR,
                    mavlink.MAV_TYPE_OCTOROTOR,
                    mavlink.MAV_TYPE_TRICOPTER]:
        map = mode_mapping_acm
    if mav_type == mavlink.MAV_TYPE_FIXED_WING:
        map = mode_mapping_apm
    if mav_type == mavlink.MAV_TYPE_GROUND_ROVER:
        map = mode_mapping_rover
    if mav_type == mavlink.MAV_TYPE_ANTENNA_TRACKER:
        map = mode_mapping_tracker
    if map is None:
        return None
    return map


def mode_string_v10(msg):
    '''mode string for 1.0 protocol, from heartbeat'''
    if not msg.base_mode & mavlink.MAV_MODE_FLAG_CUSTOM_MODE_ENABLED:
        return "Mode(0x%08x)" % msg.base_mode
    if msg.type in [ mavlink.MAV_TYPE_QUADROTOR, mavlink.MAV_TYPE_HEXAROTOR,
                     mavlink.MAV_TYPE_OCTOROTOR, mavlink.MAV_TYPE_TRICOPTER,
                     mavlink.MAV_TYPE_COAXIAL,
                     mavlink.MAV_TYPE_HELICOPTER ]:
        if msg.custom_mode in mode_mapping_acm:
            return mode_mapping_acm[msg.custom_mode]
    if msg.type == mavlink.MAV_TYPE_FIXED_WING:
        if msg.custom_mode in mode_mapping_apm:
            return mode_mapping_apm[msg.custom_mode]
    if msg.type == mavlink.MAV_TYPE_GROUND_ROVER:
        if msg.custom_mode in mode_mapping_rover:
            return mode_mapping_rover[msg.custom_mode]
    if msg.type == mavlink.MAV_TYPE_ANTENNA_TRACKER:
        if msg.custom_mode in mode_mapping_tracker:
            return mode_mapping_tracker[msg.custom_mode]
    return "Mode(%u)" % msg.custom_mode

def mode_string_apm(mode_number):
    '''return mode string for APM:Plane'''
    if mode_number in mode_mapping_apm:
        return mode_mapping_apm[mode_number]
    return "Mode(%u)" % mode_number

def mode_string_acm(mode_number):
    '''return mode string for APM:Copter'''
    if mode_number in mode_mapping_acm:
        return mode_mapping_acm[mode_number]
    return "Mode(%u)" % mode_number

def mode_string_px4(mode_number):
    '''return mode string for PX4 flight stack'''
    if mode_number in mode_mapping_px4:
        return mode_mapping_px4[mode_number]
    return "Mode(%u)" % mode_number

class x25crc(object):
    '''x25 CRC - based on checksum.h from mavlink library'''
    def __init__(self, buf=''):
        self.crc = 0xffff
        self.accumulate(buf)

    def accumulate(self, buf):
        '''add in some more bytes'''
        bytes = array.array('B')
        if isinstance(buf, array.array):
            bytes.extend(buf)
        else:
            bytes.fromstring(buf)
        accum = self.crc
        for b in bytes:
            tmp = b ^ (accum & 0xff)
            tmp = (tmp ^ (tmp<<4)) & 0xFF
            accum = (accum>>8) ^ (tmp<<8) ^ (tmp<<3) ^ (tmp>>4)
            accum = accum & 0xFFFF
        self.crc = accum

class MavlinkSerialPort():
        '''an object that looks like a serial port, but
        transmits using mavlink SERIAL_CONTROL packets'''
        def __init__(self, portname, baudrate, devnum=0, devbaud=0, timeout=3, debug=0):
                from . import mavutil

                self.baudrate = 0
                self.timeout = timeout
                self._debug = debug
                self.buf = ''
                self.port = devnum
                self.debug("Connecting with MAVLink to %s ..." % portname)
                self.mav = mavutil.mavlink_connection(portname, autoreconnect=True, baud=baudrate)
                self.mav.wait_heartbeat()
                self.debug("HEARTBEAT OK\n")
                if devbaud != 0:
                    self.setBaudrate(devbaud)
                self.debug("Locked serial device\n")

        def debug(self, s, level=1):
                '''write some debug text'''
                if self._debug >= level:
                        print(s)

        def write(self, b):
                '''write some bytes'''
                from . import mavutil
                self.debug("sending '%s' (0x%02x) of len %u\n" % (b, ord(b[0]), len(b)), 2)
                while len(b) > 0:
                        n = len(b)
                        if n > 70:
                                n = 70
                        buf = [ord(x) for x in b[:n]]
                        buf.extend([0]*(70-len(buf)))
                        self.mav.mav.serial_control_send(self.port,
                                                         mavutil.mavlink.SERIAL_CONTROL_FLAG_EXCLUSIVE |
                                                         mavutil.mavlink.SERIAL_CONTROL_FLAG_RESPOND,
                                                         0,
                                                         0,
                                                         n,
                                                         buf)
                        b = b[n:]

        def _recv(self):
                '''read some bytes into self.buf'''
                from . import mavutil
                start_time = time.time()
                while time.time() < start_time + self.timeout:
                        m = self.mav.recv_match(condition='SERIAL_CONTROL.count!=0',
                                                type='SERIAL_CONTROL', blocking=False, timeout=0)
                        if m is not None and m.count != 0:
                                break
                        self.mav.mav.serial_control_send(self.port,
                                                         mavutil.mavlink.SERIAL_CONTROL_FLAG_EXCLUSIVE |
                                                         mavutil.mavlink.SERIAL_CONTROL_FLAG_RESPOND,
                                                         0,
                                                         0,
                                                         0, [0]*70)
                        m = self.mav.recv_match(condition='SERIAL_CONTROL.count!=0',
                                                type='SERIAL_CONTROL', blocking=True, timeout=0.01)
                        if m is not None and m.count != 0:
                                break
                if m is not None:
                        if self._debug > 2:
                                print(m)
                        data = m.data[:m.count]
                        self.buf += ''.join(str(chr(x)) for x in data)

        def read(self, n):
                '''read some bytes'''
                if len(self.buf) == 0:
                        self._recv()
                if len(self.buf) > 0:
                        if n > len(self.buf):
                                n = len(self.buf)
                        ret = self.buf[:n]
                        self.buf = self.buf[n:]
                        if self._debug >= 2:
                            for b in ret:
                                self.debug("read 0x%x" % ord(b), 2)
                        return ret
                return ''

        def flushInput(self):
                '''flush any pending input'''
                self.buf = ''
                saved_timeout = self.timeout
                self.timeout = 0.5
                self._recv()
                self.timeout = saved_timeout
                self.buf = ''
                self.debug("flushInput")

        def setBaudrate(self, baudrate):
                '''set baudrate'''
                from . import mavutil
                if self.baudrate == baudrate:
                        return
                self.baudrate = baudrate
                self.mav.mav.serial_control_send(self.port,
                                                 mavutil.mavlink.SERIAL_CONTROL_FLAG_EXCLUSIVE,
                                                 0,
                                                 self.baudrate,
                                                 0, [0]*70)
                self.flushInput()
                self.debug("Changed baudrate %u" % self.baudrate)

if __name__ == '__main__':
        serial_list = auto_detect_serial(preferred_list=['*FTDI*',"*Arduino_Mega_2560*", "*3D_Robotics*", "*USB_to_UART*", '*PX4*', '*FMU*'])
        for port in serial_list:
            print("%s" % port)
