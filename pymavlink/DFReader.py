#!/usr/bin/env python
'''
APM DataFlash log file reader

Copyright Andrew Tridgell 2011
Released under GNU GPL version 3 or later

Partly based on SDLog2Parser by Anton Babushkin
'''

import struct, time, os
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
    "M": ("b", None, int),
    "q": ("q", None, int),
    "Q": ("Q", None, int),
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

    def __getattr__(self, field):
        '''override field getter'''
        try:
            i = self.fmt.colhash[field]
        except Exception:
            raise AttributeError
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
                

class DFReader(object):
    '''parse a generic dataflash file'''
    def __init__(self):
        # read the whole file into memory for simplicity
        self.msg_rate = {}
        self.new_timestamps = False
        self.px4_timestamps = False
        self.px4_timebase = 0
        self.timestamp = 0
        self.mav_type = mavutil.mavlink.MAV_TYPE_FIXED_WING
        self.verbose = False
        self.params = {}
        
    def _rewind(self):
        '''reset counters on rewind'''
        self.counts = {}
        self.counts_since_gps = {}
        self.messages = { 'MAV' : self }
        self.flightmode = "UNKNOWN"
        self.percent = 0

    def _gpsTimeToTime(self, week, sec):
        '''convert GPS week and TOW to a time in seconds since 1970'''
        epoch = 86400*(10*365 + (1980-1969)/4 + 1 + 6 - 2)
        return epoch + 86400*7*week + sec - 15

    def _find_time_base_new(self, gps):
        '''work out time basis for the log - new style'''
        t = self._gpsTimeToTime(gps.Week, gps.TimeMS*0.001)
        self.timebase = t - gps.T*0.001
        self.new_timestamps = True

    def _find_time_base_px4(self, gps):
        '''work out time basis for the log - PX4 native'''
        t = gps.GPSTime * 1.0e-6
        self.timebase = t - self.px4_timebase
        self.px4_timestamps = True

    def _find_time_base(self):
        '''work out time basis for the log'''
        self.timebase = 0
        if self._zero_time_base:
            return
        gps1 = self.recv_match(type='GPS', condition='getattr(GPS,"Week",0)!=0 or getattr(GPS,"GPSTime",0)!=0')
        if gps1 is None:
            self._rewind()
            return

        if 'GPSTime' in gps1._fieldnames:
            self._find_time_base_px4(gps1)
            self._rewind()
            return
            
        if 'T' in gps1._fieldnames:
            # it is a new style flash log with full timestamps
            self._find_time_base_new(gps1)
            self._rewind()
            return
        
        counts1 = self.counts.copy()
        gps2 = self.recv_match(type='GPS', condition='GPS.Week!=0')
        counts2 = self.counts.copy()

        if gps1 is None or gps2 is None:
            self._rewind()
            return
        
        t1 = self._gpsTimeToTime(gps1.Week, gps1.TimeMS*0.001)
        t2 = self._gpsTimeToTime(gps2.Week, gps2.TimeMS*0.001)
        if t2 == t1:
            self._rewind()
            return
        for type in counts2:
            self.msg_rate[type] = (counts2[type] - counts1[type]) / float(t2-t1)
            if self.msg_rate[type] == 0:
                self.msg_rate[type] = 1
        self._rewind()
        
    def _adjust_time_base(self, m):
        '''adjust time base from GPS message'''
        if self._zero_time_base:
            return
        if self.new_timestamps:
            return
        if self.px4_timestamps:
            return
        if getattr(m, 'Week', None) is None:
            return
        t = self._gpsTimeToTime(m.Week, m.TimeMS*0.001)
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

    def _set_time(self, m):
        '''set time for a message'''
        if self.px4_timestamps:
            m._timestamp = self.timebase + self.px4_timebase
        elif len(m._fieldnames) > 0 and (self._zero_time_base or self.new_timestamps):
            if 'TimeMS' == m._fieldnames[0]:
                m._timestamp = self.timebase + m.TimeMS*0.001
            elif m.get_type() in ['GPS','GPS2']:
                m._timestamp = self.timebase + m.T*0.001
            else:
                m._timestamp = self.timestamp
        else:
            rate = self.msg_rate.get(m.fmt.name, 50.0)
            count = self.counts_since_gps.get(m.fmt.name, 0)
            m._timestamp = self.timebase + count/rate
        self.timestamp = m._timestamp

    def recv_msg(self):
        return self._parse_next()

    def _add_msg(self, m):
        '''add a new message'''
        type = m.get_type()
        self.messages[type] = m
        if not type in self.counts:
            self.counts[type] = 0
        else:
            self.counts[type] += 1
        if not type in self.counts_since_gps:
            self.counts_since_gps[type] = 0
        else:
            self.counts_since_gps[type] += 1

        if type == 'TIME' and 'StartTime' in m._fieldnames:
            self.px4_timebase = m.StartTime * 1.0e-6
            self.px4_timestamps = True
        if type == 'GPS':
            self._adjust_time_base(m)
        if type == 'MSG':
            if m.Message.startswith("ArduRover"):
                self.mav_type = mavutil.mavlink.MAV_TYPE_GROUND_ROVER
            elif m.Message.startswith("ArduPlane"):
                self.mav_type = mavutil.mavlink.MAV_TYPE_FIXED_WING
            elif m.Message.startswith("ArduCopter"):
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
        if type == 'STAT':
            if 'MainState' in m._fieldnames:
                self.flightmode = mavutil.mode_string_px4(m.MainState)
            else:
                self.flightmode = "UNKNOWN"
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
        self._rewind()
        self._zero_time_base = zero_time_base
        self._find_time_base()
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
            skip_bytes += 1
            self.offset += 1
            if self.data_len - self.offset < 3:
                return None
            hdr = self.data[self.offset:self.offset+3]
        msg_type = ord(hdr[2])
        if skip_bytes != 0:
            if self.remaining < 528:
                return None
            print("Skipped %u bad bytes in log %s remaining=%u" % (skip_bytes, skip_type, self.remaining))

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
        try:
            elements = list(struct.unpack(fmt.msg_struct, body))
        except Exception:
            if self.remaining < 528:
                # we can have garbage at the end of an APM2 log
                return None
            print("Failed to parse %s/%s with len %u (remaining %u)" % (fmt.name, fmt.msg_struct, len(body), self.remaining))
            raise
        name = null_term(fmt.name)
        if name == 'FMT' and elements[0] not in self.formats:
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
        self._find_time_base()
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
        while self.line < len(self.lines):
            s = self.lines[self.line].rstrip()
            elements = s.split(", ")
            # move to next line
            self.line += 1
            if len(elements) >= 2:
                break

        if self.line >= len(self.lines):
            return None

        # cope with empty structures
        if len(elements) == 5 and elements[-1] == ',':
            elements[-1] = ''
            elements.append('')

        self.percent = 100.0 * (self.line / float(len(self.lines)))

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

