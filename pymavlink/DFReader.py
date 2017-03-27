#!/usr/bin/env python
'''
APM DataFlash log file reader

Copyright Andrew Tridgell 2011
Released under GNU GPL version 3 or later

Partly based on SDLog2Parser by Anton Babushkin
'''
from __future__ import print_function
from builtins import range
from builtins import object

import struct
from . import mavutil

FORMAT_TO_STRUCT = {
    "b": ("b", None, int),
    "B": ("B", None, int),
    "h": ("h", None, int),
    "H": ("H", None, int),
    "i": ("i", None, int),
    "I": ("I", None, int),
    "f": ("f", None, float),
    "n": ("4s", None, str),
    "N": ("16s", None, str),
    "Z": ("64s", None, str),
    "c": ("h", 0.01, float),
    "C": ("H", 0.01, float),
    "e": ("i", 0.01, float),
    "E": ("I", 0.01, float),
    "L": ("i", 1.0e-7, float),
    "d": ("d", None, float),
    "M": ("b", None, int),
    "q": ("q", None, long),  # Backward compat
    "Q": ("Q", None, long),  # Backward compat
    }

class DFFormat(object):
    def __init__(self, type, name, flen, format, columns):
        self.type = type
        self.name = name
        self.len = flen
        self.format = format
        self.columns = columns.split(',')

        if self.columns == ['']:
            self.columns = []

        msg_struct = "<"
        msg_mults = []
        msg_types = []
        for c in format:
            if ord(c) == 0:
                break
            try:
                (s, mul, type) = FORMAT_TO_STRUCT[c]
                msg_struct += s
                msg_mults.append(mul)
                msg_types.append(type)
            except KeyError as e:
                raise Exception("Unsupported format char: '%s' in message %s" % (c, name))

        self.msg_struct = msg_struct
        self.msg_types = msg_types
        self.msg_mults = msg_mults
        self.colhash = {}
        for i in range(len(self.columns)):
            self.colhash[self.columns[i]] = i

    def __str__(self):
        return "DFFormat(%s,%s,%s,%s)" % (self.type, self.name, self.format, self.columns)

def null_term(str):
    '''null terminate a string'''
    idx = str.find("\0")
    if idx != -1:
        str = str[:idx]
    return str

class DFMessage(object):
    def __init__(self, fmt, elements, apply_multiplier):
        self.fmt = fmt
        self._elements = elements
        self._apply_multiplier = apply_multiplier
        self._fieldnames = fmt.columns

    def to_dict(self):
        d = {'mavpackettype': self.fmt.name}

        for field in self._fieldnames:
            d[field] = self.__getattr__(field)

        return d

    def __getattr__(self, field):
        '''override field getter'''
        try:
            i = self.fmt.colhash[field]
        except Exception:
            raise AttributeError(field)
        v = self._elements[i]
        if self.fmt.format[i] != 'M' or self._apply_multiplier:
            v = self.fmt.msg_types[i](v)
        if self.fmt.msg_types[i] == str:
            v = null_term(v)
        if self.fmt.msg_mults[i] is not None and self._apply_multiplier:
            v *= self.fmt.msg_mults[i]
        return v

    def get_type(self):
        return self.fmt.name

    def __str__(self):
        ret = "%s {" % self.fmt.name
        col_count = 0
        for c in self.fmt.columns:
            ret += "%s : %s, " % (c, self.__getattr__(c))
            col_count += 1
        if col_count != 0:
            ret = ret[:-2]
        return ret + '}'

    def get_msgbuf(self):
        '''create a binary message buffer for a message'''
        values = []
        for i in range(len(self.fmt.columns)):
            if i >= len(self.fmt.msg_mults):
                continue
            mul = self.fmt.msg_mults[i]
            name = self.fmt.columns[i]
            if name == 'Mode' and 'ModeNum' in self.fmt.columns:
                name = 'ModeNum'
            v = self.__getattr__(name)
            if mul is not None:
                v /= mul
            values.append(v)
        return struct.pack("BBB", 0xA3, 0x95, self.fmt.type) + struct.pack(self.fmt.msg_struct, *values)

    def get_fieldnames(self):
        return self._fieldnames

class DFReaderClock(object):
    '''base class for all the different ways we count time in logs'''

    def __init__(self):
        self.set_timebase(0)
        self.timestamp = 0

    def _gpsTimeToTime(self, week, msec):
        '''convert GPS week and TOW to a time in seconds since 1970'''
        epoch = 86400*(10*365 + (1980-1969)/4 + 1 + 6 - 2)
        return epoch + 86400*7*week + msec*0.001 - 15

    def set_timebase(self, base):
        self.timebase = base

    def message_arrived(self, m):
        pass

    def rewind_event(self):
        pass

class DFReaderClock_usec(DFReaderClock):
    '''DFReaderClock_usec - use microsecond timestamps from messages'''
    def __init__(self):
        DFReaderClock.__init__(self)

    def find_time_base(self, gps, first_us_stamp):
        '''work out time basis for the log - even newer style'''
        t = self._gpsTimeToTime(gps.GWk, gps.GMS)
        self.set_timebase(t - gps.TimeUS*0.000001)
        # this ensures FMT messages get appropriate timestamp:
        self.timestamp = self.timebase + first_us_stamp*0.000001

    def type_has_good_TimeMS(self, type):
        '''The TimeMS in some messages is not from *our* clock!'''
        if type.startswith('ACC'):
            return False;
        if type.startswith('GYR'):
            return False;
        return True

    def should_use_msec_field0(self, m):
        if not self.type_has_good_TimeMS(m.get_type()):
            return False
        if 'TimeMS' != m._fieldnames[0]:
            return False
        if self.timebase + m.TimeMS*0.001 < self.timestamp:
            return False
        return True;

    def set_message_timestamp(self, m):
        if 'TimeUS' == m._fieldnames[0]:
            # only format messages don't have a TimeUS in them...
            m._timestamp = self.timebase + m.TimeUS*0.000001
        elif self.should_use_msec_field0(m):
            # ... in theory. I expect there to be some logs which are not
            # "pure":
            m._timestamp = self.timebase + m.TimeMS*0.001
        else:
            m._timestamp = self.timestamp
        self.timestamp = m._timestamp

class DFReaderClock_msec(DFReaderClock):
    '''DFReaderClock_msec - a format where many messages have TimeMS in their formats, and GPS messages have a "T" field giving msecs '''
    def find_time_base(self, gps, first_ms_stamp):
        '''work out time basis for the log - new style'''
        t = self._gpsTimeToTime(gps.Week, gps.TimeMS)
        self.set_timebase(t - gps.T*0.001)
        self.timestamp = self.timebase + first_ms_stamp*0.001

    def set_message_timestamp(self, m):
        if 'TimeMS' == m._fieldnames[0]:
            m._timestamp = self.timebase + m.TimeMS*0.001
        elif m.get_type() in ['GPS','GPS2']:
            m._timestamp = self.timebase + m.T*0.001
        else:
            m._timestamp = self.timestamp
        self.timestamp = m._timestamp

class DFReaderClock_px4(DFReaderClock):
    '''DFReaderClock_px4 - a format where a starting time is explicitly given in a message'''
    def __init__(self):
        DFReaderClock.__init__(self)
        self.px4_timebase = 0

    def find_time_base(self, gps):
        '''work out time basis for the log - PX4 native'''
        t = gps.GPSTime * 1.0e-6
        self.timebase = t - self.px4_timebase

    def set_px4_timebase(self, time_msg):
        self.px4_timebase = time_msg.StartTime * 1.0e-6

    def set_message_timestamp(self, m):
        m._timestamp = self.timebase + self.px4_timebase

    def message_arrived(self, m):
        type = m.get_type()
        if type == 'TIME' and 'StartTime' in m._fieldnames:
            self.set_px4_timebase(m)

class DFReaderClock_gps_interpolated(DFReaderClock):
    '''DFReaderClock_gps_interpolated - for when the only real references in a message are GPS timestamps '''
    def __init__(self):
        DFReaderClock.__init__(self)
        self.msg_rate = {}
        self.counts = {}
        self.counts_since_gps = {}

    def rewind_event(self):
        '''reset counters on rewind'''
        self.counts = {}
        self.counts_since_gps = {}

    def message_arrived(self, m):
        type = m.get_type()
        if not type in self.counts:
            self.counts[type] = 1
        else:
            self.counts[type] += 1
        # this preserves existing behaviour - but should we be doing this
        # if type == 'GPS'?
        if not type in self.counts_since_gps:
            self.counts_since_gps[type] = 1
        else:
            self.counts_since_gps[type] += 1

        if type == 'GPS' or type == 'GPS2':
            self.gps_message_arrived(m)

    def gps_message_arrived(self, m):
        '''adjust time base from GPS message'''
        # msec-style GPS message?
        gps_week = getattr(m, 'Week', None)
        gps_timems = getattr(m, 'TimeMS', None)
        if gps_week is None:
            # usec-style GPS message?
            gps_week = getattr(m, 'GWk', None)
            gps_timems = getattr(m, 'GMS', None)
            if gps_week is None:
                if getattr(m, 'GPSTime', None) is not None:
                    # PX4-style timestamp; we've only been called
                    # because we were speculatively created in case no
                    # better clock was found.
                    return;

        t = self._gpsTimeToTime(gps_week, gps_timems) 

        deltat = t - self.timebase
        if deltat <= 0:
            return

        for type in self.counts_since_gps:
            rate = self.counts_since_gps[type] / deltat
            if rate > self.msg_rate.get(type, 0):
                self.msg_rate[type] = rate
        self.msg_rate['IMU'] = 50.0
        self.timebase = t
        self.counts_since_gps = {}

    def set_message_timestamp(self, m):
        rate = self.msg_rate.get(m.fmt.name, 50.0)
        if int(rate) == 0:
            rate = 50
        count = self.counts_since_gps.get(m.fmt.name, 0)
        m._timestamp = self.timebase + count/rate


class DFReader(object):
    '''parse a generic dataflash file'''
    def __init__(self):
        # read the whole file into memory for simplicity
        self.clock = None
        self.timestamp = 0
        self.mav_type = mavutil.mavlink.MAV_TYPE_FIXED_WING
        self.verbose = False
        self.params = {}

    def _rewind(self):
        '''reset state on rewind'''
        self.messages = { 'MAV' : self }
        self.flightmode = "UNKNOWN"
        self.percent = 0
        if self.clock:
            self.clock.rewind_event()

    def init_clock_px4(self, px4_msg_time, px4_msg_gps):
        self.clock = DFReaderClock_px4()
        if not self._zero_time_base:
            self.clock.set_px4_timebase(px4_msg_time)
            self.clock.find_time_base(px4_msg_gps)
        return True

    def init_clock_msec(self):
        # it is a new style flash log with full timestamps
        self.clock = DFReaderClock_msec()

    def init_clock_usec(self):
        self.clock = DFReaderClock_usec()

    def init_clock_gps_interpolated(self, clock):
        self.clock = clock

    def init_clock(self):
        '''work out time basis for the log'''

        self._rewind()

        # speculatively create a gps clock in case we don't find anything
        # better
        gps_clock = DFReaderClock_gps_interpolated()
        self.clock = gps_clock

        px4_msg_time = None
        px4_msg_gps = None
        gps_interp_msg_gps1 = None
        first_us_stamp = None
        first_ms_stamp = None

        have_good_clock = False
        while True:
            m = self.recv_msg()
            if m is None:
                break;

            type = m.get_type()

            if first_us_stamp is None:
                first_us_stamp = getattr(m, "TimeUS", None);

            if first_ms_stamp is None and (type != 'GPS' and type != 'GPS2'):
                # Older GPS messages use TimeMS for msecs past start
                # of gps week
                first_ms_stamp = getattr(m, "TimeMS", None);

            if type == 'GPS' or type == 'GPS2':
                if getattr(m, "TimeUS", 0) != 0 and \
                   getattr(m, "GWk", 0) != 0: # everything-usec-timestamped
                    self.init_clock_usec()
                    if not self._zero_time_base:
                        self.clock.find_time_base(m, first_us_stamp)
                    have_good_clock = True
                    break
                if getattr(m, "T", 0) != 0 and \
                   getattr(m, "Week", 0) != 0: # GPS is msec-timestamped
                    if first_ms_stamp is None:
                        first_ms_stamp = m.T
                    self.init_clock_msec()
                    if not self._zero_time_base:
                        self.clock.find_time_base(m, first_ms_stamp)
                    have_good_clock = True
                    break
                if getattr(m, "GPSTime", 0) != 0: # px4-style-only
                    px4_msg_gps = m
                if getattr(m, "Week", 0) != 0:
                    if gps_interp_msg_gps1 is not None and \
                       (gps_interp_msg_gps1.TimeMS != m.TimeMS or \
                        gps_interp_msg_gps1.Week != m.Week):
                        # we've received two distinct, non-zero GPS
                        # packets without finding a decent clock to
                        # use; fall back to interpolation. Q: should
                        # we wait a few more messages befoe doing
                        # this?
                        self.init_clock_gps_interpolated(gps_clock)
                        have_good_clock = True
                        break
                    gps_interp_msg_gps1 = m

            elif type == 'TIME':
                '''only px4-style logs use TIME'''
                if getattr(m, "StartTime", None) != None:
                    px4_msg_time = m;

            if px4_msg_time is not None and px4_msg_gps is not None:
                self.init_clock_px4(px4_msg_time, px4_msg_gps)
                have_good_clock = True
                break

#        print("clock is " + str(self.clock))
        if not have_good_clock:
            # we failed to find any GPS messages to set a time
            # base for usec and msec clocks.  Also, not a
            # PX4-style log
            if first_us_stamp is not None:
                self.init_clock_usec()
            elif first_ms_stamp is not None:
                self.init_clock_msec()

        self._rewind()

        return

    def _set_time(self, m):
        '''set time for a message'''
        # really just left here for profiling
        m._timestamp = self.timestamp
        if len(m._fieldnames) > 0 and self.clock is not None:
            self.clock.set_message_timestamp(m)

    def recv_msg(self):
        return self._parse_next()

    def _add_msg(self, m):
        '''add a new message'''
        type = m.get_type()
        self.messages[type] = m

        if self.clock:
            self.clock.message_arrived(m)

        if type == 'MSG':
            if m.Message.find("Rover") != -1:
                self.mav_type = mavutil.mavlink.MAV_TYPE_GROUND_ROVER
            elif m.Message.find("Plane") != -1:
                self.mav_type = mavutil.mavlink.MAV_TYPE_FIXED_WING
            elif m.Message.find("Copter") != -1:
                self.mav_type = mavutil.mavlink.MAV_TYPE_QUADROTOR
            elif m.Message.startswith("Antenna"):
                self.mav_type = mavutil.mavlink.MAV_TYPE_ANTENNA_TRACKER
        if type == 'MODE':
            if isinstance(m.Mode, str):
                self.flightmode = m.Mode.upper()
            elif 'ModeNum' in m._fieldnames:
                mapping = mavutil.mode_mapping_bynumber(self.mav_type)
                if mapping is not None and m.ModeNum in mapping:
                    self.flightmode = mapping[m.ModeNum]
            else:
                self.flightmode = mavutil.mode_string_acm(m.Mode)
        if type == 'STAT' and 'MainState' in m._fieldnames:
            self.flightmode = mavutil.mode_string_px4(m.MainState)
        if type == 'PARM' and getattr(m, 'Name', None) is not None:
            self.params[m.Name] = m.Value
        self._set_time(m)

    def recv_match(self, condition=None, type=None, blocking=False):
        '''recv the next message that matches the given condition
        type can be a string or a list of strings'''
        if type is not None and not isinstance(type, list):
            type = [type]
        while True:
            m = self.recv_msg()
            if m is None:
                return None
            if type is not None and not m.get_type() in type:
                continue
            if not mavutil.evaluate_condition(condition, self.messages):
                continue
            return m

    def check_condition(self, condition):
        '''check if a condition is true'''
        return mavutil.evaluate_condition(condition, self.messages)

    def param(self, name, default=None):
        '''convenient function for returning an arbitrary MAVLink
           parameter with a default'''
        if not name in self.params:
            return default
        return self.params[name]

class DFReader_binary(DFReader):
    '''parse a binary dataflash file'''
    def __init__(self, filename, zero_time_base=False):
        DFReader.__init__(self)
        # read the whole file into memory for simplicity
        f = open(filename, mode='rb')
        self.data = f.read()
        self.data_len = len(self.data)
        f.close()
        self.HEAD1 = 0xA3
        self.HEAD2 = 0x95
        self.formats = {
            0x80 : DFFormat(0x80, 'FMT', 89, 'BBnNZ', "Type,Length,Name,Format,Columns")
        }
        self._zero_time_base = zero_time_base
        self.init_clock()
        self._rewind()

    def _rewind(self):
        '''rewind to start of log'''
        DFReader._rewind(self)
        self.offset = 0
        self.remaining = self.data_len

    def _parse_next(self):
        '''read one message, returning it as an object'''
        if self.data_len - self.offset < 3:
            return None
            
        hdr = self.data[self.offset:self.offset+3]
        skip_bytes = 0
        skip_type = None
        # skip over bad messages
        while (ord(hdr[0]) != self.HEAD1 or ord(hdr[1]) != self.HEAD2 or ord(hdr[2]) not in self.formats):
            if skip_type is None:
                skip_type = (ord(hdr[0]), ord(hdr[1]), ord(hdr[2]))
                skip_start = self.offset
            skip_bytes += 1
            self.offset += 1
            if self.data_len - self.offset < 3:
                return None
            hdr = self.data[self.offset:self.offset+3]
        msg_type = ord(hdr[2])
        if skip_bytes != 0:
            if self.remaining < 528:
                return None
            print("Skipped %u bad bytes in log at offset %u, type=%s" % (skip_bytes, skip_start, skip_type))
            self.remaining -= skip_bytes

        self.offset += 3
        self.remaining -= 3

        if not msg_type in self.formats:
            if self.verbose:
                print("unknown message type %02x" % msg_type)
            raise Exception("Unknown message type %02x" % msg_type)
        fmt = self.formats[msg_type]
        if self.remaining < fmt.len-3:
            # out of data - can often happen half way through a message
            if self.verbose:
                print("out of data")
            return None
        body = self.data[self.offset:self.offset+(fmt.len-3)]
        elements = None
        try:
            elements = list(struct.unpack(fmt.msg_struct, body))
        except Exception:
            if self.remaining < 528:
                # we can have garbage at the end of an APM2 log
                return None
            # we should also cope with other corruption; logs
            # transfered via DataFlash_MAVLink may have blocks of 0s
            # in them, for example
            print("Failed to parse %s/%s with len %u (remaining %u)" % (fmt.name, fmt.msg_struct, len(body), self.remaining))
        if elements is None:
            return self._parse_next()
        name = null_term(fmt.name)
        if name == 'FMT':
            # add to formats
            # name, len, format, headings
            self.formats[elements[0]] = DFFormat(elements[0],
                                                 null_term(elements[2]), elements[1],
                                                 null_term(elements[3]), null_term(elements[4]))

        self.offset += fmt.len-3
        self.remaining -= fmt.len-3
        m = DFMessage(fmt, elements, True)
        self._add_msg(m)

        self.percent = 100.0 * (self.offset / float(self.data_len))
        
        return m

def DFReader_is_text_log(filename):
    '''return True if a file appears to be a valid text log'''
    f = open(filename)
    ret = (f.read(8000).find('FMT, ') != -1)
    f.close()
    return ret

class DFReader_text(DFReader):
    '''parse a text dataflash file'''
    def __init__(self, filename, zero_time_base=False):
        DFReader.__init__(self)
        # read the whole file into memory for simplicity
        f = open(filename, mode='r')
        self.lines = f.readlines()
        f.close()
        self.formats = {
            'FMT' : DFFormat(0x80, 'FMT', 89, 'BBnNZ', "Type,Length,Name,Format,Columns")
        }
        self._rewind()
        self._zero_time_base = zero_time_base
        self.init_clock()
        self._rewind()

    def _rewind(self):
        '''rewind to start of log'''
        DFReader._rewind(self)
        self.line = 0
        # find the first valid line
        while self.line < len(self.lines):
            if self.lines[self.line].startswith("FMT, "):
                break
            self.line += 1

    def _parse_next(self):
        '''read one message, returning it as an object'''

        this_line = self.line
        while self.line < len(self.lines):
            s = self.lines[self.line].rstrip()
            elements = s.split(", ")
            this_line = self.line
            # move to next line
            self.line += 1
            if len(elements) >= 2:
                # this_line is good
                break

        if this_line >= len(self.lines):
            return None

        # cope with empty structures
        if len(elements) == 5 and elements[-1] == ',':
            elements[-1] = ''
            elements.append('')

        self.percent = 100.0 * (this_line / float(len(self.lines)))

        msg_type = elements[0]

        if not msg_type in self.formats:
            return self._parse_next()
        
        fmt = self.formats[msg_type]

        if len(elements) < len(fmt.format)+1:
            # not enough columns
            return self._parse_next()

        elements = elements[1:]
        
        name = fmt.name.rstrip('\0')
        if name == 'FMT':
            # add to formats
            # name, len, format, headings
            self.formats[elements[2]] = DFFormat(int(elements[0]), elements[2], int(elements[1]), elements[3], elements[4])

        try:
            m = DFMessage(fmt, elements, False)
        except ValueError:
            return self._parse_next()

        self._add_msg(m)

        return m


if __name__ == "__main__":
    import sys
    use_profiler = False
    if use_profiler:
        from line_profiler import LineProfiler
        profiler = LineProfiler()
        profiler.add_function(DFReader_binary._parse_next)
        profiler.add_function(DFReader_binary._add_msg)
        profiler.add_function(DFReader._set_time)
        profiler.enable_by_count()
                    
    filename = sys.argv[1]
    if filename.endswith('.log'):
        log = DFReader_text(filename)
    else:
        log = DFReader_binary(filename)
    while True:
        m = log.recv_msg()
        if m is None:
            break
        #print(m)
    if use_profiler:
        profiler.print_stats()

