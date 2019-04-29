#!/usr/bin/env python
'''
mavlink python utility functions

Copyright Andrew Tridgell 2011
Released under GNU GPL version 3 or later
'''
from __future__ import print_function
from builtins import object

import socket, math, struct, time, os, fnmatch, array, sys, errno
import select
from pymavlink import mavexpression

# adding these extra imports allows pymavlink to be used directly with pyinstaller
# without having complex spec files. To allow for installs that don't have ardupilotmega
# at all we avoid throwing an exception if it isn't installed
try:
    import json
    from pymavlink.dialects.v10 import ardupilotmega
except Exception:
    pass

# maximum packet length for a single receive call - use the UDP limit
UDP_MAX_PACKET_LEN = 65535

# Store the MAVLink library for the currently-selected dialect
# (set by set_dialect())
mavlink = None

# Store the mavlink file currently being operated on
# (set by mavlink_connection())
mavfile_global = None

# If the caller hasn't specified a particular native/legacy version, use this
default_native = False

# link_id used for signing
global_link_id = 0

# Use a globally-set MAVLink dialect if one has been specified as an environment variable.
if not 'MAVLINK_DIALECT' in os.environ:
    os.environ['MAVLINK_DIALECT'] = 'ardupilotmega'

def mavlink10():
    '''return True if using MAVLink 1.0 or later'''
    return not 'MAVLINK09' in os.environ

def mavlink20():
    '''return True if using MAVLink 2.0'''
    return 'MAVLINK20' in os.environ

def evaluate_expression(expression, vars):
    '''evaluation an expression'''
    return mavexpression.evaluate_expression(expression, vars)

def evaluate_condition(condition, vars):
    '''evaluation a conditional (boolean) statement'''
    if condition is None:
        return True
    v = evaluate_expression(condition, vars)
    if v is None:
        return False
    return v

def u_ord(c):
	return ord(c) if sys.version_info.major < 3 else c

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
    if 'MAVLINK20' in os.environ:
        wire_protocol = mavparse.PROTOCOL_2_0
        modname = "pymavlink.dialects.v20." + dialect
    elif mavlink is None or mavlink.WIRE_PROTOCOL_VERSION == "1.0" or not 'MAVLINK09' in os.environ:
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

class mavfile_state(object):
    '''state for a particular system id'''
    def __init__(self):
        self.messages = { 'MAV' : self }
        self.flightmode = "UNKNOWN"
        self.vehicle_type = "UNKNOWN"
        self.mav_type = mavlink.MAV_TYPE_FIXED_WING
        self.base_mode = 0
        self.armed = False # canonical arm state for the vehicle as a whole

        if float(mavlink.WIRE_PROTOCOL_VERSION) >= 1:
            self.messages['HOME'] = mavlink.MAVLink_gps_raw_int_message(0,0,0,0,0,0,0,0,0,0)
            mavlink.MAVLink_waypoint_message = mavlink.MAVLink_mission_item_message
        else:
            self.messages['HOME'] = mavlink.MAVLink_gps_raw_message(0,0,0,0,0,0,0,0,0)

class param_state(object):
    '''state for a particular system id/component id pair'''
    def __init__(self):
        self.params = {}

class mavfile(object):
    '''a generic mavlink port'''
    def __init__(self, fd, address, source_system=255, source_component=0, notimestamps=False, input=True, use_native=default_native):
        global mavfile_global
        if input:
            mavfile_global = self
        self.fd = fd
        self.sysid = 0
        self.param_sysid = (0,0)
        self.address = address
        self.timestamp = 0
        self.last_seq = {}
        self.mav_loss = 0
        self.mav_count = 0
        self.param_fetch_start = 0

        # state for each sysid
        self.sysid_state = {}
        self.sysid_state[self.sysid] = mavfile_state()

        # param state for each sysid/compid tuple
        self.param_state = {}
        self.param_state[self.param_sysid] = param_state()
        
        # status of param fetch, indexed by sysid,compid tuple
        self.source_system = source_system
        self.source_component = source_component
        self.first_byte = True
        self.robust_parsing = True
        self.mav = mavlink.MAVLink(self, srcSystem=self.source_system, srcComponent=self.source_component, use_native=use_native)
        self.mav.robust_parsing = self.robust_parsing
        self.logfile = None
        self.logfile_raw = None
        self.start_time = time.time()
        self.message_hooks = []
        self.idle_hooks = []
        self.uptime = 0.0
        self.notimestamps = notimestamps
        self._timestamp = None
        self.WIRE_PROTOCOL_VERSION = mavlink.WIRE_PROTOCOL_VERSION
        self.stop_on_EOF = False
        self.portdead = False

    @property
    def target_system(self):
        return self.sysid

    @property
    def target_component(self):
        return self.param_sysid[1]
    
    @target_system.setter
    def target_system(self, value):
        self.sysid = value
        if not self.sysid in self.sysid_state:
            self.sysid_state[self.sysid] = mavfile_state()
        if self.sysid != self.param_sysid[0]:
            self.param_sysid = (self.sysid, self.param_sysid[1])
            if not self.param_sysid in self.param_state:
                self.param_state[self.param_sysid] = param_state()

    @target_component.setter
    def target_component(self, value):
        if value != self.param_sysid[1]:
            self.param_sysid = (self.param_sysid[0], value)
            if not self.param_sysid in self.param_state:
                self.param_state[self.param_sysid] = param_state()

    @property
    def params(self):
        if self.param_sysid[1] == 0:
            eff_tuple = (self.sysid, 1)
            if eff_tuple in self.param_state:
                return getattr(self.param_state[eff_tuple],'params')
        return getattr(self.param_state[self.param_sysid],'params')

    @property
    def messages(self):
        return getattr(self.sysid_state[self.sysid],'messages')

    @property
    def flightmode(self):
        return getattr(self.sysid_state[self.sysid],'flightmode')

    @flightmode.setter
    def flightmode(self, value):
        setattr(self.sysid_state[self.sysid],'flightmode',value)

    @property
    def vehicle_type(self):
        return getattr(self.sysid_state[self.sysid],'vehicle_type')

    @vehicle_type.setter
    def vehicle_type(self, value):
        setattr(self.sysid_state[self.sysid],'vehicle_type',value)

    @property
    def mav_type(self):
        return getattr(self.sysid_state[self.sysid],'mav_type')

    @mav_type.setter
    def mav_type(self, value):
        setattr(self.sysid_state[self.sysid],'mav_type',value)
    
    @property
    def base_mode(self):
        return getattr(self.sysid_state[self.sysid],'base_mode')

    @base_mode.setter
    def base_mode(self, value):
        setattr(self.sysid_state[self.sysid],'base_mode',value)
    
    def auto_mavlink_version(self, buf):
        '''auto-switch mavlink protocol version'''
        global mavlink
        if len(buf) == 0:
            return
        try:
            magic = ord(buf[0])
        except:
            magic = buf[0]
        if not magic in [ 85, 254, 253 ]:
            return
        self.first_byte = False
        if self.WIRE_PROTOCOL_VERSION == "0.9" and magic == 254:
            self.WIRE_PROTOCOL_VERSION = "1.0"
            set_dialect(current_dialect)
        elif self.WIRE_PROTOCOL_VERSION == "1.0" and magic == 85:
            self.WIRE_PROTOCOL_VERSION = "0.9"
            os.environ['MAVLINK09'] = '1'
            set_dialect(current_dialect)
        elif self.WIRE_PROTOCOL_VERSION != "2.0" and magic == 253:
            self.WIRE_PROTOCOL_VERSION = "2.0"
            os.environ['MAVLINK20'] = '1'
            set_dialect(current_dialect)
        else:
            return
        # switch protocol 
        (callback, callback_args, callback_kwargs) = (self.mav.callback,
                                                      self.mav.callback_args,
                                                      self.mav.callback_kwargs)
        self.mav = mavlink.MAVLink(self, srcSystem=self.source_system, srcComponent=self.source_component)
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

    def probably_vehicle_heartbeat(self, msg):
        if msg.get_srcComponent() == mavlink.MAV_COMP_ID_GIMBAL:
            return False
        if msg.type in (mavlink.MAV_TYPE_GCS,
                        mavlink.MAV_TYPE_GIMBAL,
                        mavlink.MAV_TYPE_ADSB,
                        mavlink.MAV_TYPE_ONBOARD_CONTROLLER):
            return False
        return True

    def post_message(self, msg):
        '''default post message call'''
        if '_posted' in msg.__dict__:
            return
        msg._posted = True
        msg._timestamp = time.time()
        type = msg.get_type()

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

        if not src_system in self.sysid_state:
            # we've seen a new system
            self.sysid_state[src_system] = mavfile_state()

        self.sysid_state[src_system].messages[type] = msg

        if src_tuple == radio_tuple:
            # as a special case radio msgs are added for all sysids
            for s in self.sysid_state.keys():
                self.sysid_state[s].messages[type] = msg

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
                #print("lost %u seq=%u seq2=%u last_seq=%u src_tupe=%s %s" % (diff, seq, seq2, last_seq, str(src_tuple), msg.get_type()))
            self.last_seq[src_tuple] = seq2
            self.mav_count += 1
        
        self.timestamp = msg._timestamp
        if type == 'HEARTBEAT' and self.probably_vehicle_heartbeat(msg):
            if self.sysid == 0:
                # lock onto id tuple of first vehicle heartbeat
                self.sysid = src_system
            if float(mavlink.WIRE_PROTOCOL_VERSION) >= 1:
                self.flightmode = mode_string_v10(msg)
                self.mav_type = msg.type
                self.base_mode = msg.base_mode
                self.sysid_state[self.sysid].armed = (msg.base_mode & mavlink.MAV_MODE_FLAG_SAFETY_ARMED)

        elif type == 'PARAM_VALUE':
            if not src_tuple in self.param_state:
                self.param_state[src_tuple] = param_state()
            self.param_state[src_tuple].params[msg.param_id] = msg.param_value
        elif type == 'SYS_STATUS' and mavlink.WIRE_PROTOCOL_VERSION == '0.9':
            self.flightmode = mode_string_v09(msg)
        elif type == 'GPS_RAW':
            if self.sysid_state[src_system].messages['HOME'].fix_type < 2:
                self.sysid_state[src_system].messages['HOME'] = msg
        elif type == 'GPS_RAW_INT':
            if self.sysid_state[src_system].messages['HOME'].fix_type < 3:
                self.sysid_state[src_system].messages['HOME'] = msg
        for hook in self.message_hooks:
            hook(self, msg)

        if (msg.get_signed() and
            self.mav.signing.link_id == 0 and
            msg.get_link_id() != 0 and
            self.target_system == msg.get_srcSystem() and
            self.target_component == msg.get_srcComponent()):
            # change to link_id from incoming packet
            self.mav.signing.link_id = msg.get_link_id()


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
        if type is not None and not isinstance(type, list) and not isinstance(type, set):
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
        '''return True if using MAVLink 1.0 or later'''
        return float(self.WIRE_PROTOCOL_VERSION) >= 1

    def mavlink20(self):
        '''return True if using MAVLink 2.0 or later'''
        return float(self.WIRE_PROTOCOL_VERSION) >= 2

    def setup_logfile(self, logfile, mode='w'):
        '''start logging to the given logfile, with timestamps'''
        self.logfile = open(logfile, mode=mode)

    def setup_logfile_raw(self, logfile, mode='w'):
        '''start logging raw bytes to the given logfile, without timestamps'''
        self.logfile_raw = open(logfile, mode=mode)

    def wait_heartbeat(self, blocking=True, timeout=None):
        '''wait for a heartbeat so we know the target system IDs'''
        return self.recv_match(type='HEARTBEAT', blocking=blocking, timeout=timeout)

    def param_fetch_all(self):
        '''initiate fetch of all parameters'''
        if time.time() - self.param_fetch_start < 2.0:
            # don't fetch too often
            return
        self.param_fetch_start = time.time()
        self.mav.param_request_list_send(self.target_system, self.target_component)

    def param_fetch_one(self, name):
        '''initiate fetch of one parameter'''
        try:
            idx = int(name)
            self.mav.param_request_read_send(self.target_system, self.target_component, b"", idx)
        except Exception:
            if sys.version_info.major >= 3 and not isinstance(name, bytes):
                name = bytes(name,'ascii')
            self.mav.param_request_read_send(self.target_system, self.target_component, name, -1)

    def time_since(self, mtype):
        '''return the time since the last message of type mtype was received'''
        if not mtype in self.messages:
            return time.time() - self.start_time
        return time.time() - self.messages[mtype]._timestamp

    def param_set_send(self, parm_name, parm_value, parm_type=None):
        '''wrapper for parameter set'''
        if self.mavlink10():
            if parm_type is None:
                parm_type = mavlink.MAVLINK_TYPE_FLOAT
            self.mav.param_set_send(self.target_system, self.target_component,
                                    parm_name.encode('utf8'), parm_value, parm_type)
        else:
            self.mav.param_set_send(self.target_system, self.target_component,
                                    parm_name.encode('utf8'), parm_value)

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
            if enable:
                mode = mode | flag
            elif not enable:
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
        mav_autopilot = self.field('HEARTBEAT', 'autopilot', None)
        if mav_autopilot == mavlink.MAV_AUTOPILOT_PX4:
            return px4_map
        if mav_type is None:
            return None
        map = None
        if mav_type in [mavlink.MAV_TYPE_QUADROTOR,
                        mavlink.MAV_TYPE_HELICOPTER,
                        mavlink.MAV_TYPE_HEXAROTOR,
                        mavlink.MAV_TYPE_OCTOROTOR,
                        mavlink.MAV_TYPE_DODECAROTOR,
                        mavlink.MAV_TYPE_COAXIAL,
                        mavlink.MAV_TYPE_TRICOPTER]:
            map = mode_mapping_acm
        if mav_type == mavlink.MAV_TYPE_FIXED_WING:
            map = mode_mapping_apm
        if mav_type == mavlink.MAV_TYPE_GROUND_ROVER:
            map = mode_mapping_rover
        if mav_type == mavlink.MAV_TYPE_SURFACE_BOAT:
            map = mode_mapping_rover # for the time being
        if mav_type == mavlink.MAV_TYPE_ANTENNA_TRACKER:
            map = mode_mapping_tracker
        if mav_type == mavlink.MAV_TYPE_SUBMARINE:
            map = mode_mapping_sub
        if map is None:
            return None
        inv_map = dict((a, b) for (b, a) in map.items())
        return inv_map

    def set_mode_apm(self, mode, custom_mode = 0, custom_sub_mode = 0):
        '''enter arbitrary mode'''
        if isinstance(mode, str):
            mode_map = self.mode_mapping()
            if mode_map is None or mode not in mode_map:
                print("Unknown mode '%s'" % mode)
                return
            mode = mode_map[mode]
        # set mode by integer mode number for ArduPilot
        self.mav.set_mode_send(self.target_system,
                               mavlink.MAV_MODE_FLAG_CUSTOM_MODE_ENABLED,
                               mode)

    def set_mode_px4(self, mode, custom_mode, custom_sub_mode):
        '''enter arbitrary mode'''
        if isinstance(mode, str):
            mode_map = self.mode_mapping()
            if mode_map is None or mode not in mode_map:
                print("Unknown mode '%s'" % mode)
                return
            # PX4 uses two fields to define modes
            mode, custom_mode, custom_sub_mode = px4_map[mode]
        self.mav.command_long_send(self.target_system, self.target_component,
                                   mavlink.MAV_CMD_DO_SET_MODE, 0, mode, custom_mode, custom_sub_mode, 0, 0, 0, 0)

    def set_mode(self, mode, custom_mode = 0, custom_sub_mode = 0):
        '''set arbitrary flight mode'''
        mav_autopilot = self.field('HEARTBEAT', 'autopilot', None)
        if mav_autopilot == mavlink.MAV_AUTOPILOT_PX4:
            self.set_mode_px4(mode, custom_mode, custom_sub_mode)
        else:
            self.set_mode_apm(mode)
        
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
                            condition='GPS_RAW_INT.fix_type>=3 and GPS_RAW_INT.lat != 0')
        else:
            self.recv_match(type='GPS_RAW', blocking=True,
                            condition='GPS_RAW.fix_type>=2 and GPS_RAW.lat != 0')

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
        return self.sysid_state[self.sysid].armed

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

    def setup_signing(self, secret_key, sign_outgoing=True, allow_unsigned_callback=None, initial_timestamp=None, link_id=None):
        '''setup for MAVLink2 signing'''
        self.mav.signing.secret_key = secret_key
        self.mav.signing.sign_outgoing = sign_outgoing
        self.mav.signing.allow_unsigned_callback = allow_unsigned_callback
        if link_id is None:
            # auto-increment the link_id for each link
            global global_link_id
            link_id = global_link_id
            global_link_id = min(global_link_id + 1, 255)
        self.mav.signing.link_id = link_id
        if initial_timestamp is None:
            # timestamp is time since 1/1/2015
            epoch_offset = 1420070400
            now = max(time.time(), epoch_offset)
            initial_timestamp = now - epoch_offset
            initial_timestamp = int(initial_timestamp * 100 * 1000)
        # initial_timestamp is in 10usec units
        self.mav.signing.timestamp = initial_timestamp

    def disable_signing(self):
        '''disable MAVLink2 signing'''
        self.mav.signing.secret_key = None
        self.mav.signing.sign_outgoing = False
        self.mav.signing.allow_unsigned_callback = None
        self.mav.signing.link_id = 0
        self.mav.signing.timestamp = 0

def set_close_on_exec(fd):
    '''set the clone on exec flag on a file descriptor. Ignore exceptions'''
    try:
        import fcntl
        flags = fcntl.fcntl(fd, fcntl.F_GETFD)
        flags |= fcntl.FD_CLOEXEC
        fcntl.fcntl(fd, fcntl.F_SETFD, flags)
    except Exception:
        pass

class FakeSerial():
    def __init__(self):
        pass
    def read(self, len):
        return ""
    def write(self, buf):
        raise Exception("write always fails")
    def inWaiting(self):
        return 0
    def close(self):
        pass

class mavserial(mavfile):
    '''a serial mavlink port'''
    def __init__(self, device, baud=115200, autoreconnect=False, source_system=255, source_component=0, use_native=default_native, force_connected=False):
        import serial
        if ',' in device and not os.path.exists(device):
            device, baud = device.split(',')
        self.baud = baud
        self.device = device
        self.autoreconnect = autoreconnect
        self.force_connected = force_connected
        # we rather strangely set the baudrate initially to 1200, then change to the desired
        # baudrate. This works around a kernel bug on some Linux kernels where the baudrate
        # is not set correctly
        try:
            self.port = serial.Serial(self.device, 1200, timeout=0,
                                      dsrdtr=False, rtscts=False, xonxoff=False)
        except serial.SerialException as e:
            if not force_connected:
                raise e
            self.port = FakeSerial()

        try:
            fd = self.port.fileno()
            set_close_on_exec(fd)
        except Exception:
            fd = None
        self.set_baudrate(self.baud)
        mavfile.__init__(self, fd, device, source_system=source_system, source_component=source_component, use_native=use_native)
        self.rtscts = False

    def set_rtscts(self, enable):
        '''enable/disable RTS/CTS if applicable'''
        try:
            self.port.setRtsCts(enable)
        except Exception:
            self.port.rtscts = enable
        self.rtscts = enable

    def set_baudrate(self, baudrate):
        '''set baudrate'''
        try:
            self.port.setBaudrate(baudrate)
        except Exception:
            # for pySerial 3.0, which doesn't have setBaudrate()
            self.port.baudrate = baudrate
    
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
            return self.port.write(bytes(buf))
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
            try:
                newport = serial.Serial(self.device, self.baud, timeout=0,
                                        dsrdtr=False, rtscts=False, xonxoff=False)
            except serial.SerialException as e:
                if not self.force_connected:
                    raise e
                newport = FakeSerial()
                return False
            self.port.close()
            self.port = newport
            print("Device %s reopened OK" % self.device)
            self.portdead = False
            try:
                self.fd = self.port.fileno()
            except Exception:
                self.fd = None
            self.set_baudrate(self.baud)
            if self.rtscts:
                self.set_rtscts(self.rtscts)
            return True
        except Exception:
            return False
        

class mavudp(mavfile):
    '''a UDP mavlink socket'''
    def __init__(self, device, input=True, broadcast=False, source_system=255, source_component=0, use_native=default_native):
        a = device.split(':')
        if len(a) != 2:
            print("UDP ports must be specified as host:port")
            sys.exit(1)
        self.port = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.udp_server = input
        self.broadcast = False
        if input:
            self.port.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.port.bind((a[0], int(a[1])))
        else:
            self.destination_addr = (a[0], int(a[1]))
            if broadcast:
                self.port.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
                self.broadcast = True
        set_close_on_exec(self.port.fileno())
        self.port.setblocking(0)
        self.last_address = None
        self.resolved_destination_addr = None
        mavfile.__init__(self, self.port.fileno(), device, source_system=source_system, source_component=source_component, input=input, use_native=use_native)

    def close(self):
        self.port.close()

    def recv(self,n=None):
        try:
            data, new_addr = self.port.recvfrom(UDP_MAX_PACKET_LEN)
        except socket.error as e:
            if e.errno in [ errno.EAGAIN, errno.EWOULDBLOCK, errno.ECONNREFUSED ]:
                return ""
            raise
        if self.udp_server or self.broadcast:
            self.last_address = new_addr
        return data

    def write(self, buf):
        try:
            if self.udp_server:
                if self.last_address:
                    self.port.sendto(buf, self.last_address)
            else:
                if self.last_address and self.broadcast:
                    self.destination_addr = self.last_address
                    self.broadcast = False
                    self.port.connect(self.destination_addr)
                # turn a (possible) hostname into an IP address to
                # avoid resolving the hostname for every packet sent:
                if self.destination_addr[0] != self.resolved_destination_addr:
                    self.resolved_destination_addr = self.destination_addr[0]
                    self.destination_addr = (socket.gethostbyname(self.destination_addr[0]), self.destination_addr[1])
                self.port.sendto(buf, self.destination_addr)
        except socket.error:
            pass

    def recv_msg(self):
        '''message receive routine for UDP link'''
        self.pre_message()
        s = self.recv()
        if len(s) > 0:
            if self.first_byte:
                self.auto_mavlink_version(s)

        m = self.mav.parse_char(s)
        if m is not None:
            self.post_message(m)

        return m

class mavmcast(mavfile):
    '''a UDP multicast mavlink socket'''
    def __init__(self, device, broadcast=False, source_system=255, source_component=0, use_native=default_native):
        a = device.split(':')
        mcast_ip = "239.255.145.50"
        mcast_port = 14550
        if len(a) == 1 and len(a[0]) > 0:
            mcast_port = int(a[0])
        elif len(a) > 1:
            mcast_ip = a[0]
            mcast_port = int(a[1])

        # first the receiving socket. We use separate sending and receiving
        # sockets so we can use the port number of the sending socket to detect
        # packets from ourselves
        self.port = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        self.port.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.port.bind((mcast_ip, mcast_port))
        mreq = struct.pack("4sl", socket.inet_aton(mcast_ip), socket.INADDR_ANY)
        self.port.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)
        self.port.setblocking(0)
        set_close_on_exec(self.port.fileno())

        # now the sending socket
        self.port_out = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.port_out.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.port_out.setblocking(0)
        self.port_out.connect((mcast_ip, mcast_port))
        set_close_on_exec(self.port_out.fileno())
        self.myport = None

        mavfile.__init__(self, self.port.fileno(), device,
                         source_system=source_system, source_component=source_component,
                         input=False, use_native=use_native)

    def close(self):
        self.port.close()
        self.port_out.close()

    def recv(self,n=None):
        try:
            data, new_addr = self.port.recvfrom(UDP_MAX_PACKET_LEN)
            if self.myport is None:
                try:
                    (myaddr,self.myport) = self.port_out.getsockname()
                except Exception:
                    pass
        except socket.error as e:
            if e.errno in [ errno.EAGAIN, errno.EWOULDBLOCK, errno.ECONNREFUSED ]:
                return ""
            raise
        if self.myport == new_addr[1]:
            # data from ourselves, discard
            return ''
        return data

    def write(self, buf):
        try:
            self.port_out.send(buf)
        except socket.error as e:
            pass

    def recv_msg(self):
        '''message receive routine for UDP link'''
        self.pre_message()
        s = self.recv()
        if len(s) > 0:
            if self.first_byte:
                self.auto_mavlink_version(s)

        m = self.mav.parse_char(s)
        if m is not None:
            self.post_message(m)

        return m
    

class mavtcp(mavfile):
    '''a TCP mavlink socket'''
    def __init__(self,
                 device,
                 autoreconnect=False,
                 source_system=255,
                 source_component=0,
                 retries=3,
                 use_native=default_native):
        a = device.split(':')
        if len(a) != 2:
            print("TCP ports must be specified as host:port")
            sys.exit(1)
        self.destination_addr = (a[0], int(a[1]))

        self.autoreconnect = autoreconnect

        self.retries = retries
        self.do_connect()

        mavfile.__init__(self, self.port.fileno(), "tcp:" + device, source_system=source_system, source_component=source_component, use_native=use_native)

    def do_connect(self):
        self.port = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        retries = self.retries
        if retries <= 0:
            # try to connect at least once:
            retries = 1
        while retries >= 0:
            retries -= 1
            try:
                self.port.connect(self.destination_addr)
                break
            except Exception as e:
                if retries == 0:
                    raise e
                print(e, "sleeping")
                time.sleep(1)
        self.port.setblocking(0)
        set_close_on_exec(self.port.fileno())
        self.port.setsockopt(socket.SOL_TCP, socket.TCP_NODELAY, 1)

    def close(self):
        self.port.close()

    def handle_eof(self):
        # EOF
        print("EOF on TCP socket")
        if self.autoreconnect:
            print("Attempting reconnect")
            if self.port is not None:
                self.port.close()
                self.port = None
            self.do_connect()

    def recv(self,n=None):
        if self.port is None:
            self.handle_eof()
        if n is None:
            n = self.mav.bytes_needed()
        try:
            data = self.port.recv(n)
        except socket.error as e:
            if e.errno in [ errno.EAGAIN, errno.EWOULDBLOCK ]:
                return ""
            raise
        if len(data) == 0:
            self.handle_eof()

        return data

    def write(self, buf):
        if self.port is None:
            self.do_connect()
        try:
            self.port.send(buf)
        except socket.error:
            pass


class mavtcpin(mavfile):
    '''a TCP input mavlink socket'''
    def __init__(self, device, source_system=255, source_component=0, retries=3, use_native=default_native):
        a = device.split(':')
        if len(a) != 2:
            print("TCP ports must be specified as host:port")
            sys.exit(1)
        self.listen = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.listen_addr = (a[0], int(a[1]))
        self.listen.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.listen.bind(self.listen_addr)
        self.listen.listen(1)
        self.listen.setblocking(0)
        set_close_on_exec(self.listen.fileno())
        self.listen.setsockopt(socket.SOL_TCP, socket.TCP_NODELAY, 1)
        mavfile.__init__(self, self.listen.fileno(), "tcpin:" + device, source_system=source_system, source_component=source_component, use_native=use_native)
        self.port = None

    def close(self):
        self.listen.close()

    def recv(self,n=None):
        if not self.port:
            try:
                (self.port, addr) = self.listen.accept()
            except Exception:
                return ''
            self.port.setsockopt(socket.SOL_TCP, socket.TCP_NODELAY, 1) 
            self.port.setblocking(0) 
            set_close_on_exec(self.port.fileno())
            self.fd = self.port.fileno()

        if n is None:
            n = self.mav.bytes_needed()
        try:
            data = self.port.recv(n)
        except socket.error as e:
            if e.errno in [ errno.EAGAIN, errno.EWOULDBLOCK ]:
                return ""
            self.port.close()
            self.port = None
            self.fd = self.listen.fileno()
            return ''
        return data

    def write(self, buf):
        if self.port is None:
            return
        try:
            self.port.send(buf)
        except socket.error as e:
            if e.errno in [ errno.EPIPE ]:
                self.port.close()
                self.port = None
                self.fd = self.listen.fileno()
            pass


class mavlogfile(mavfile):
    '''a MAVLink logfile reader/writer'''
    def __init__(self, filename, planner_format=None,
                 write=False, append=False,
                 robust_parsing=True, notimestamps=False, source_system=255, source_component=0, use_native=default_native):
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
        mavfile.__init__(self, None, filename, source_system=source_system, source_component=source_component, notimestamps=notimestamps, use_native=use_native)
        if self.notimestamps:
            self._timestamp = 0
        else:
            self._timestamp = time.time()
        self.stop_on_EOF = True
        self._last_message = None
        self._last_timestamp = None
        self._link = 0

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
        msg._link = self._link


class mavmmaplog(mavlogfile):
    '''a MAVLink log file accessed via mmap. Used for fast read-only
    access with low memory overhead where particular message types are wanted'''
    def __init__(self, filename, progress_callback=None):
        import platform, mmap
        mavlogfile.__init__(self, filename)
        self.f.seek(0, 2)
        self.data_len = self.f.tell()
        self.f.seek(0)
        if platform.system() == "Windows":
            self.data_map = mmap.mmap(self.f.fileno(), self.data_len, None, mmap.ACCESS_READ)
        else:
            self.data_map = mmap.mmap(self.f.fileno(), self.data_len, mmap.MAP_PRIVATE, mmap.PROT_READ)
        self._rewind()
        self.init_arrays(progress_callback)
        self._flightmodes = None

    def _rewind(self):
        '''rewind to start of log'''
        self.flightmode = "UNKNOWN"
        self.offset = 0
        self.type_nums = None
        self.f.seek(0)

    def rewind(self):
        '''rewind to start of log'''
        self._rewind()
        
    def init_arrays(self, progress_callback=None):
        '''initialise arrays for fast recv_match()'''

        # dictionary indexed by msgid, mapping to arrays of file offsets where
        # each instance of a msg type is found
        self.offsets = {}

        # number of msgs of each msg type
        self.counts = {}
        self._count = 0

        # mapping from msg name to msg id
        self.name_to_id = {}

        # mapping from msg id to name
        self.id_to_name = {}

        self.type_nums = None

        ofs = 0
        pct = 0

        MARKER_V1 = 0xFE
        MARKER_V2 = 0xFD
        
        while ofs+8+6 < self.data_len:
            marker = u_ord(self.data_map[ofs+8])
            mlen = u_ord(self.data_map[ofs+9]) + 8
            if marker == MARKER_V1:
                mtype = u_ord(self.data_map[ofs+13])
                mlen += 8
            elif marker == MARKER_V2:
                mtype = u_ord(self.data_map[ofs+15]) | (u_ord(self.data_map[ofs+16])<<8) | (u_ord(self.data_map[ofs+17])<<16)
                mlen += 12
                incompat_flags = u_ord(self.data_map[ofs+10])
                if incompat_flags & mavlink.MAVLINK_IFLAG_SIGNED:
                    mlen += mavlink.MAVLINK_SIGNATURE_BLOCK_LEN
            else:
                # unrecognised marker; probably a malformed log
                ofs += 1
                continue

            if not mtype in self.offsets:
                if not mtype in mavlink.mavlink_map:
                    ofs += mlen
                    continue
                self.offsets[mtype] = []
                self.counts[mtype] = 0
                msg = mavlink.mavlink_map[mtype]
                self.name_to_id[msg.name] = mtype
                self.id_to_name[mtype] = msg.name
                self.f.seek(ofs)
                m = self.recv_msg()
                self.messages[msg.name] = m

            self.offsets[mtype].append(ofs)
            self.counts[mtype] += 1

            ofs += mlen
            new_pct = (100 * ofs) // self.data_len
            if progress_callback is not None and new_pct != pct:
                progress_callback(new_pct)
                pct = new_pct

        for mtype in self.counts:
            self._count += self.counts[mtype]
        self.offset = 0
        self._rewind()

    def skip_to_type(self, type):
        '''skip fwd to next msg matching given type set'''
        if self.type_nums is None:
            # always add some key msg types so we can track flightmode, params etc
            type = type.copy()
            type.update(set(['HEARTBEAT','PARAM_VALUE']))
            self.indexes = []
            self.type_nums = []
            for t in type:
                if not t in self.name_to_id:
                    continue
                self.type_nums.append(self.name_to_id[t])
                self.indexes.append(0)
        smallest_index = -1
        smallest_offset = self.data_len
        for i in range(len(self.type_nums)):
            mtype = self.type_nums[i]
            if self.indexes[i] >= self.counts[mtype]:
                continue
            ofs = self.offsets[mtype][self.indexes[i]]
            if ofs < smallest_offset:
                smallest_offset = ofs
                smallest_index = i
        if smallest_index >= 0:
            self.indexes[smallest_index] += 1
            self.offset = smallest_offset
            self.f.seek(smallest_offset)

    def recv_match(self, condition=None, type=None, blocking=False, timeout=None):
        '''recv the next message that matches the given condition
        type can be a string or a list of strings'''
        if type is not None:
            if isinstance(type, str):
                type = set([type])
            elif isinstance(type, list):
                type = set(type)
        while True:
            if type is not None:
                self.skip_to_type(type)
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
        
    def flightmode_list(self):
        '''return an array of tuples for all flightmodes in log. Tuple is (modestring, t0, t1)'''
        tstamp = None
        fmode = None
        if self._flightmodes is None:
            self._rewind()
            self._flightmodes = []
            types = set(['HEARTBEAT'])
            while True:
                m = self.recv_match(type=types)
                if m is None:
                    break
                tstamp = m._timestamp
                if self.flightmode == fmode:
                    continue
                if len(self._flightmodes) > 0:
                    (mode, t0, t1) = self._flightmodes[-1]
                    self._flightmodes[-1] = (mode, t0, tstamp)
                self._flightmodes.append((self.flightmode, tstamp, None))
                fmode = self.flightmode
        if tstamp is not None:
            (mode, t0, t1) = self._flightmodes[-1]
            self._flightmodes[-1] = (mode, t0, tstamp)

        self._rewind()
        return self._flightmodes

class mavchildexec(mavfile):
    '''a MAVLink child processes reader/writer'''
    def __init__(self, filename, source_system=255, source_component=0, use_native=default_native):
        from subprocess import Popen, PIPE
        import fcntl
        
        self.filename = filename
        self.child = Popen(filename, shell=False, stdout=PIPE, stdin=PIPE, bufsize=0)
        self.fd = self.child.stdout.fileno()

        fl = fcntl.fcntl(self.fd, fcntl.F_GETFL)
        fcntl.fcntl(self.fd, fcntl.F_SETFL, fl | os.O_NONBLOCK)

        fl = fcntl.fcntl(self.child.stdout.fileno(), fcntl.F_GETFL)
        fcntl.fcntl(self.child.stdout.fileno(), fcntl.F_SETFL, fl | os.O_NONBLOCK)

        mavfile.__init__(self, self.fd, filename, source_system=source_system, source_component=source_component, use_native=use_native)

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


def mavlink_connection(device, baud=115200, source_system=255, source_component=0,
                       planner_format=None, write=False, append=False,
                       robust_parsing=True, notimestamps=False, input=True,
                       dialect=None, autoreconnect=False, zero_time_base=False,
                       retries=3, use_native=default_native,
                       force_connected=False, progress_callback=None):
    '''open a serial, UDP, TCP or file mavlink connection'''
    global mavfile_global

    if force_connected:
        # force_connected implies autoreconnect
        autoreconnect = True

    if dialect is not None:
        set_dialect(dialect)
    if device.startswith('tcp:'):
        return mavtcp(device[4:],
                      autoreconnect=autoreconnect,
                      source_system=source_system,
                      source_component=source_component,
                      retries=retries,
                      use_native=use_native)
    if device.startswith('tcpin:'):
        return mavtcpin(device[6:], source_system=source_system, source_component=source_component, retries=retries, use_native=use_native)
    if device.startswith('udpin:'):
        return mavudp(device[6:], input=True, source_system=source_system, source_component=source_component, use_native=use_native)
    if device.startswith('udpout:'):
        return mavudp(device[7:], input=False, source_system=source_system, source_component=source_component, use_native=use_native)
    if device.startswith('udpbcast:'):
        return mavudp(device[9:], input=False, source_system=source_system, source_component=source_component, use_native=use_native, broadcast=True)
    # For legacy purposes we accept the following syntax and let the caller to specify direction
    if device.startswith('udp:'):
        return mavudp(device[4:], input=input, source_system=source_system, source_component=source_component, use_native=use_native)
    if device.startswith('mcast:'):
        return mavmcast(device[6:], source_system=source_system, source_component=source_component, use_native=use_native)

    if device.lower().endswith('.bin') or device.lower().endswith('.px4log'):
        # support dataflash logs
        from pymavlink import DFReader
        m = DFReader.DFReader_binary(device, zero_time_base=zero_time_base, progress_callback=progress_callback)
        mavfile_global = m
        return m

    if device.endswith('.log'):
        # support dataflash text logs
        from pymavlink import DFReader
        if DFReader.DFReader_is_text_log(device):
            m = DFReader.DFReader_text(device, zero_time_base=zero_time_base, progress_callback=progress_callback)
            mavfile_global = m
            return m    

    # list of suffixes to prevent setting DOS paths as UDP sockets
    logsuffixes = ['mavlink', 'log', 'raw', 'tlog' ]
    suffix = device.split('.')[-1].lower()
    if device.find(':') != -1 and not suffix in logsuffixes:
        return mavudp(device, source_system=source_system, source_component=source_component, input=input, use_native=use_native)
    if os.path.isfile(device):
        if device.endswith(".elf") or device.find("/bin/") != -1:
            print("executing '%s'" % device)
            return mavchildexec(device, source_system=source_system, source_component=source_component, use_native=use_native)
        elif not write and not append and not notimestamps:
            return mavmmaplog(device, progress_callback=progress_callback)
        else:
            return mavlogfile(device, planner_format=planner_format, write=write,
                              append=append, robust_parsing=robust_parsing, notimestamps=notimestamps,
                              source_system=source_system, source_component=source_component, use_native=use_native)
    return mavserial(device,
                     baud=baud,
                     source_system=source_system,
                     source_component=source_component,
                     autoreconnect=autoreconnect,
                     use_native=use_native,
                     force_connected=force_connected)

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
        if not is_printable(c) and not c in ['\r', '\n', '\t'] and not c in [ord('\r'), ord('\n'), ord('\t')]:
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
    16 : 'INITIALISING',
    17 : 'QSTABILIZE',
    18 : 'QHOVER',
    19 : 'QLOITER',
    20 : 'QLAND',
    21 : 'QRTL',
    22 : 'QAUTOTUNE',
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
    16 : 'POSHOLD',
    17 : 'BRAKE',
    18 : 'THROW',
    19 : 'AVOID_ADSB',
    20 : 'GUIDED_NOGPS',
    21 : 'SMART_RTL',
    22 : 'FLOWHOLD',
    23 : 'FOLLOW',
}
mode_mapping_rover = {
    0 : 'MANUAL',
    1 : 'ACRO',
    2 : 'LEARNING',
    3 : 'STEERING',
    4 : 'HOLD',
    5 : 'LOITER',
    10 : 'AUTO',
    11 : 'RTL',
    12 : 'SMART_RTL',
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

mode_mapping_sub = {
    0: 'STABILIZE',
    1: 'ACRO',
    2: 'ALT_HOLD',
    3: 'AUTO',
    4: 'GUIDED',
    7: 'CIRCLE',
    9: 'SURFACE',
    16: 'POSHOLD',
    19: 'MANUAL',
    }

# map from a PX4 "main_state" to a string; see msg/commander_state.msg
# This allows us to map sdlog STAT.MainState to a simple "mode"
# string, used in DFReader and possibly other places.  These are
# related but distict from what is found in mavlink messages; see
# "Custom mode definitions", below.
mainstate_mapping_px4 = {
    0 : 'MANUAL',
    1 : 'ALTCTL',
    2 : 'POSCTL',
    3 : 'AUTO_MISSION',
    4 : 'AUTO_LOITER',
    5 : 'AUTO_RTL',
    6 : 'ACRO',
    7 : 'OFFBOARD',
    8 : 'STAB',
    9 : 'RATTITUDE',
    10 : 'AUTO_TAKEOFF',
    11 : 'AUTO_LAND',
    12 : 'AUTO_FOLLOW_TARGET',
    13 : 'MAX',
}
def mode_string_px4(MainState):
    return mainstate_mapping_px4.get(MainState, "Unknown")


# Custom mode definitions from PX4
PX4_CUSTOM_MAIN_MODE_MANUAL            = 1
PX4_CUSTOM_MAIN_MODE_ALTCTL            = 2
PX4_CUSTOM_MAIN_MODE_POSCTL            = 3
PX4_CUSTOM_MAIN_MODE_AUTO              = 4
PX4_CUSTOM_MAIN_MODE_ACRO              = 5
PX4_CUSTOM_MAIN_MODE_OFFBOARD          = 6
PX4_CUSTOM_MAIN_MODE_STABILIZED        = 7
PX4_CUSTOM_MAIN_MODE_RATTITUDE         = 8

PX4_CUSTOM_SUB_MODE_OFFBOARD           = 0
PX4_CUSTOM_SUB_MODE_AUTO_READY         = 1
PX4_CUSTOM_SUB_MODE_AUTO_TAKEOFF       = 2
PX4_CUSTOM_SUB_MODE_AUTO_LOITER        = 3
PX4_CUSTOM_SUB_MODE_AUTO_MISSION       = 4
PX4_CUSTOM_SUB_MODE_AUTO_RTL           = 5
PX4_CUSTOM_SUB_MODE_AUTO_LAND          = 6
PX4_CUSTOM_SUB_MODE_AUTO_RTGS          = 7
PX4_CUSTOM_SUB_MODE_AUTO_FOLLOW_TARGET = 8

auto_mode_flags  = mavlink.MAV_MODE_FLAG_AUTO_ENABLED \
                 | mavlink.MAV_MODE_FLAG_STABILIZE_ENABLED \
                 | mavlink.MAV_MODE_FLAG_GUIDED_ENABLED

px4_map = { "MANUAL":        (mavlink.MAV_MODE_FLAG_CUSTOM_MODE_ENABLED | mavlink.MAV_MODE_FLAG_STABILIZE_ENABLED | mavlink.MAV_MODE_FLAG_MANUAL_INPUT_ENABLED,   PX4_CUSTOM_MAIN_MODE_MANUAL,      0                                       ),
            "STABILIZED":    (mavlink.MAV_MODE_FLAG_CUSTOM_MODE_ENABLED | mavlink.MAV_MODE_FLAG_STABILIZE_ENABLED | mavlink.MAV_MODE_FLAG_MANUAL_INPUT_ENABLED,   PX4_CUSTOM_MAIN_MODE_STABILIZED,  0                                       ),
            "ACRO":          (mavlink.MAV_MODE_FLAG_CUSTOM_MODE_ENABLED |                                           mavlink.MAV_MODE_FLAG_MANUAL_INPUT_ENABLED,   PX4_CUSTOM_MAIN_MODE_ACRO,        0                                       ),
            "RATTITUDE":     (mavlink.MAV_MODE_FLAG_CUSTOM_MODE_ENABLED |                                           mavlink.MAV_MODE_FLAG_MANUAL_INPUT_ENABLED,   PX4_CUSTOM_MAIN_MODE_RATTITUDE,   0                                       ),
            "ALTCTL":        (mavlink.MAV_MODE_FLAG_CUSTOM_MODE_ENABLED | mavlink.MAV_MODE_FLAG_STABILIZE_ENABLED | mavlink.MAV_MODE_FLAG_MANUAL_INPUT_ENABLED,   PX4_CUSTOM_MAIN_MODE_ALTCTL,      0                                       ),
            "POSCTL":        (mavlink.MAV_MODE_FLAG_CUSTOM_MODE_ENABLED | mavlink.MAV_MODE_FLAG_STABILIZE_ENABLED | mavlink.MAV_MODE_FLAG_MANUAL_INPUT_ENABLED,   PX4_CUSTOM_MAIN_MODE_POSCTL,      0                                       ),
            "LOITER":        (mavlink.MAV_MODE_FLAG_CUSTOM_MODE_ENABLED | auto_mode_flags,                                                                        PX4_CUSTOM_MAIN_MODE_AUTO,        PX4_CUSTOM_SUB_MODE_AUTO_LOITER         ),
            "MISSION":       (mavlink.MAV_MODE_FLAG_CUSTOM_MODE_ENABLED | auto_mode_flags,                                                                        PX4_CUSTOM_MAIN_MODE_AUTO,        PX4_CUSTOM_SUB_MODE_AUTO_MISSION        ),
            "RTL":           (mavlink.MAV_MODE_FLAG_CUSTOM_MODE_ENABLED | auto_mode_flags,                                                                        PX4_CUSTOM_MAIN_MODE_AUTO,        PX4_CUSTOM_SUB_MODE_AUTO_RTL            ),
            "LAND":          (mavlink.MAV_MODE_FLAG_CUSTOM_MODE_ENABLED | auto_mode_flags,                                                                        PX4_CUSTOM_MAIN_MODE_AUTO,        PX4_CUSTOM_SUB_MODE_AUTO_LAND           ),
            "RTGS":          (mavlink.MAV_MODE_FLAG_CUSTOM_MODE_ENABLED | auto_mode_flags,                                                                        PX4_CUSTOM_MAIN_MODE_AUTO,        PX4_CUSTOM_SUB_MODE_AUTO_RTGS           ),
            "FOLLOWME":      (mavlink.MAV_MODE_FLAG_CUSTOM_MODE_ENABLED | auto_mode_flags,                                                                        PX4_CUSTOM_MAIN_MODE_AUTO,        PX4_CUSTOM_SUB_MODE_AUTO_FOLLOW_TARGET  ),
            "OFFBOARD":      (mavlink.MAV_MODE_FLAG_CUSTOM_MODE_ENABLED | auto_mode_flags,                                                                        PX4_CUSTOM_MAIN_MODE_OFFBOARD,    0                                       ),
            "TAKEOFF":       (mavlink.MAV_MODE_FLAG_CUSTOM_MODE_ENABLED | auto_mode_flags,                                                                        PX4_CUSTOM_MAIN_MODE_AUTO,        PX4_CUSTOM_SUB_MODE_AUTO_TAKEOFF        )}


def interpret_px4_mode(base_mode, custom_mode):
    custom_main_mode = (custom_mode & 0xFF0000)   >> 16
    custom_sub_mode  = (custom_mode & 0xFF000000) >> 24

    if base_mode & mavlink.MAV_MODE_FLAG_MANUAL_INPUT_ENABLED != 0: #manual modes
        if custom_main_mode == PX4_CUSTOM_MAIN_MODE_MANUAL:
            return "MANUAL"
        elif custom_main_mode == PX4_CUSTOM_MAIN_MODE_ACRO:
            return "ACRO"
        elif custom_main_mode == PX4_CUSTOM_MAIN_MODE_RATTITUDE:
            return "RATTITUDE"
        elif custom_main_mode == PX4_CUSTOM_MAIN_MODE_STABILIZED:
            return "STABILIZED"
        elif custom_main_mode == PX4_CUSTOM_MAIN_MODE_ALTCTL:
            return "ALTCTL"
        elif custom_main_mode == PX4_CUSTOM_MAIN_MODE_POSCTL:
            return "POSCTL"
    elif (base_mode & auto_mode_flags) == auto_mode_flags: #auto modes
        if custom_main_mode & PX4_CUSTOM_MAIN_MODE_AUTO != 0:
            if custom_sub_mode == PX4_CUSTOM_SUB_MODE_AUTO_MISSION:
                return "MISSION"
            elif custom_sub_mode == PX4_CUSTOM_SUB_MODE_AUTO_TAKEOFF:
                return "TAKEOFF"
            elif custom_sub_mode == PX4_CUSTOM_SUB_MODE_AUTO_LOITER:
                return "LOITER"
            elif custom_sub_mode == PX4_CUSTOM_SUB_MODE_AUTO_FOLLOW_TARGET:
                return "FOLLOWME"
            elif custom_sub_mode == PX4_CUSTOM_SUB_MODE_AUTO_RTL:
                return "RTL"
            elif custom_sub_mode == PX4_CUSTOM_SUB_MODE_AUTO_LAND:
                return "LAND"
            elif custom_sub_mode == PX4_CUSTOM_SUB_MODE_AUTO_RTGS:
                return "RTGS"
            elif custom_sub_mode == PX4_CUSTOM_SUB_MODE_OFFBOARD:
                return "OFFBOARD"
    return "UNKNOWN"

def mode_mapping_byname(mav_type):
    '''return dictionary mapping mode names to numbers, or None if unknown'''
    map = None
    if mav_type in [mavlink.MAV_TYPE_QUADROTOR,
                    mavlink.MAV_TYPE_HELICOPTER,
                    mavlink.MAV_TYPE_HEXAROTOR,
                    mavlink.MAV_TYPE_OCTOROTOR,
                    mavlink.MAV_TYPE_COAXIAL,
                    mavlink.MAV_TYPE_TRICOPTER]:
        map = mode_mapping_acm
    if mav_type == mavlink.MAV_TYPE_FIXED_WING:
        map = mode_mapping_apm
    if mav_type == mavlink.MAV_TYPE_GROUND_ROVER:
        map = mode_mapping_rover
    if mav_type == mavlink.MAV_TYPE_SURFACE_BOAT:
        map = mode_mapping_rover # for the time being
    if mav_type == mavlink.MAV_TYPE_ANTENNA_TRACKER:
        map = mode_mapping_tracker
    if mav_type == mavlink.MAV_TYPE_SUBMARINE:
        map = mode_mapping_sub
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
                    mavlink.MAV_TYPE_DODECAROTOR,
                    mavlink.MAV_TYPE_COAXIAL,
                    mavlink.MAV_TYPE_TRICOPTER]:
        map = mode_mapping_acm
    if mav_type == mavlink.MAV_TYPE_FIXED_WING:
        map = mode_mapping_apm
    if mav_type == mavlink.MAV_TYPE_GROUND_ROVER:
        map = mode_mapping_rover
    if mav_type == mavlink.MAV_TYPE_SURFACE_BOAT:
        map = mode_mapping_rover # for the time being
    if mav_type == mavlink.MAV_TYPE_ANTENNA_TRACKER:
        map = mode_mapping_tracker
    if mav_type == mavlink.MAV_TYPE_SUBMARINE:
        map = mode_mapping_sub
    if map is None:
        return None
    return map


def mode_string_v10(msg):
    '''mode string for 1.0 protocol, from heartbeat'''
    if msg.autopilot == mavlink.MAV_AUTOPILOT_PX4:
        return interpret_px4_mode(msg.base_mode, msg.custom_mode)
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
    if msg.type == mavlink.MAV_TYPE_SURFACE_BOAT:
        if msg.custom_mode in mode_mapping_rover:
            return mode_mapping_rover[msg.custom_mode]
    if msg.type == mavlink.MAV_TYPE_ANTENNA_TRACKER:
        if msg.custom_mode in mode_mapping_tracker:
            return mode_mapping_tracker[msg.custom_mode]
    if msg.type == mavlink.MAV_TYPE_SUBMARINE:
        if msg.custom_mode in mode_mapping_sub:
            return mode_mapping_sub[msg.custom_mode]
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

class x25crc(object):
    '''x25 CRC - based on checksum.h from mavlink library'''
    def __init__(self, buf=''):
        self.crc = 0xffff
        self.accumulate(buf)

    def accumulate(self, buf):
        '''add in some more bytes'''
        byte_buf = array.array('B')
        if isinstance(buf, array.array):
            byte_buf.extend(buf)
        else:
            byte_buf.fromstring(buf)
        accum = self.crc
        for b in byte_buf:
            tmp = b ^ (accum & 0xff)
            tmp = (tmp ^ (tmp<<4)) & 0xFF
            accum = (accum>>8) ^ (tmp<<8) ^ (tmp<<3) ^ (tmp>>4)
            accum = accum & 0xFFFF
        self.crc = accum

class MavlinkSerialPort(object):
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
