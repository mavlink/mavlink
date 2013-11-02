#!/usr/bin/env python
'''
APM DataFlash log file reader

Copyright Andrew Tridgell 2011
Released under GNU GPL version 3 or later

Partly based on SDLog2Parser by Anton Babushkin
'''

import struct, time
from pymavlink import mavutil

FORMAT_TO_STRUCT = {
    "b": ("b", None),
    "B": ("B", None),
    "h": ("h", None),
    "H": ("H", None),
    "i": ("i", None),
    "I": ("I", None),
    "f": ("f", None),
    "n": ("4s", None),
    "N": ("16s", None),
    "Z": ("64s", None),
    "c": ("h", 0.01),
    "C": ("H", 0.01),
    "e": ("i", 0.01),
    "E": ("I", 0.01),
    "L": ("i", 1.0e-7),
    "M": ("b", None),
    "q": ("q", None),
    "Q": ("Q", None),
    }

class DFFormat(object):
    def __init__(self, name, len, format, columns):

        # hack for broken 2.75 format
        if format == 'IHIBBBhhhhB':
            format = 'IHIBBBhhhB'
            
        self.name = name
        self.len = len
        self.format = format
        self.columns = columns.split(',')

        msg_struct = "<"
        msg_mults = []
        msg_strip = []
        for c in format:
            if ord(c) == 0:
                break
            try:
                (s, mul) = FORMAT_TO_STRUCT[c]
                msg_struct += s
                msg_mults.append(mul)
                if c in ['n', 'N', 'Z']:
                    msg_strip.append(True)
                else:
                    msg_strip.append(False)
            except KeyError as e:
                raise Exception("Unsupported format char: '%s' in message %s" % (c, name))

        self.msg_struct = msg_struct
        self.msg_strip = msg_strip
        self.msg_mults = msg_mults

class DFMessage(object):
    def __init__(self, fmt, elements):
        self._d = {}
        self.fmt = fmt
        for i in range(len(fmt.columns)):
            mul = fmt.msg_mults[i]
            name = fmt.columns[i]
            self._d[name] = elements[i]
            if fmt.msg_strip[i]:
                self._d[name] = self._d[name].rstrip('\0')
            if mul is not None:
                self._d[name] = float(self._d[name]) * mul
        self.__dict__.update(self._d)

    def get_type(self):
        return self.fmt.name

    def __str__(self):
        ret = "%s {" % self.fmt.name
        for c in self.fmt.columns:
            ret += "%s : %s, " % (c, self._d[c])
        ret = ret[:-2] + "}"
        return ret
                

class DFReader(object):
    '''parse a dataflash file'''
    def __init__(self, filename):
        # read the whole file into memory for simplicity
        f = open(filename, mode='rb')
        self.data = f.read()
        f.close()
        self.HEAD1 = 0xA3
        self.HEAD2 = 0x95
        self.formats = {
            0x80 : DFFormat('FMT', 89, 'BBnNZ', "Type,Length,Name,Format,Columns")
        }
        self.msg_rate = {}
        self._rewind()
        self._find_time_base()

    def _rewind(self):
        self.offset = 0
        self.counts = {}
        self.counts_since_gps = {}
        self.remaining = len(self.data)
        self.messages = {}

    def _gpsTimeToTime(self, week, sec):
        '''convert GPS week and TOW to a time in seconds since 1970'''
        epoch = 86400*(10*365 + (1980-1969)/4 + 1 + 6 - 2)
        return epoch + 86400*7*week + sec - 15

    def _find_time_base(self):
        '''work out time basis for the log'''
        self.timebase = 0
        gps1 = self.recv_match(type='GPS', condition='GPS.Week!=0')
        counts1 = self.counts.copy()
        gps2 = self.recv_match(type='GPS', condition='GPS.Week!=0')
        counts2 = self.counts.copy()

        if gps1 is None or gps2 is None:
            return
        
        t1 = self._gpsTimeToTime(gps1.Week, gps1.TimeMS*0.001)
        t2 = self._gpsTimeToTime(gps2.Week, gps2.TimeMS*0.001)
        if t2 == t1:
            return
        for type in counts2:
            self.msg_rate[type] = (counts2[type] - counts1[type]) / float(t2-t1)
            if self.msg_rate[type] == 0:
                self.msg_rate[type] = 1
        self._rewind()
        
    def _adjust_time_base(self, m):
        '''adjust time base from GPS message'''
        t = self._gpsTimeToTime(m.Week, m.TimeMS*0.001)
        deltat = t - self.timebase
        if deltat <= 0:
            return
        for type in self.counts_since_gps:
            rate = self.counts_since_gps[type] / deltat
            if rate > self.msg_rate.get(type, 0):
                self.msg_rate[type] = rate
        self.timebase = t
        self.counts_since_gps = {}        

    def _set_time(self, m):
        '''set time for a message'''
        rate = self.msg_rate.get(m.fmt.name, 50.0)
        count = self.counts_since_gps.get(m.fmt.name, 0)
        m._timestamp = self.timebase + count/rate

    def _parse_next(self):
        '''read one message, returning it as an object'''
        if len(self.data) - self.offset < 3:
            return None
            
        hdr = self.data[self.offset:self.offset+3]
        if (ord(hdr[0]) != self.HEAD1 or ord(hdr[1]) != self.HEAD2):
            raise Exception("Invalid header: %02X %02X, must be %02X %02X" % (ord(hdr[0]), ord(hdr[1]), self.HEAD1, self.HEAD2))
        msg_type = ord(hdr[2])

        self.offset += 3
        self.remaining -= 3

        if not msg_type in self.formats:
            raise Exception("Unknown message type %02x" % msg_type)
        fmt = self.formats[msg_type]
        if self.remaining < fmt.len-3:
            # out of data - can often happen half way through a message
            return None
        body = self.data[self.offset:self.offset+(fmt.len-3)]
        try:
            elements = list(struct.unpack(fmt.msg_struct, body))
        except Exception:
            print("Failed to parse %s/%s with len %u (remaining %u)" % (fmt.name, fmt.msg_struct, len(body), self.remaining))
            raise
        name = fmt.name.rstrip('\0')
        if name == 'FMT':
            # add to formats
            # name, len, format, headings
            self.formats[elements[0]] = DFFormat(elements[2].rstrip('\0'), elements[1],
                                                 elements[3].rstrip('\0'), elements[4].rstrip('\0'))

        self.offset += fmt.len-3
        self.remaining -= fmt.len-3
        m = DFMessage(fmt, elements)
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

        if name == 'GPS':
            self._adjust_time_base(m)
        self._set_time(m)
            
        return m


    def recv_msg(self):
        return self._parse_next()

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

if __name__ == "__main__":
    import sys
    log = DFReader(sys.argv[1])
    while True:
        m = log.recv_msg()
        if m is None:
            break
        #print m

    
