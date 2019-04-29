'''
module for loading/saving sets of mavlink parameters
'''
from __future__ import print_function


import fnmatch, math, time, struct
from pymavlink import mavutil

class MAVParmDict(dict):
    def __init__(self, *args):
        dict.__init__(self, args)
        # some parameters should not be loaded from files
        self.exclude_load = ['SYSID_SW_MREV', 'SYS_NUM_RESETS', 'ARSPD_OFFSET', 'GND_ABS_PRESS',
                             'GND_TEMP', 'CMD_TOTAL', 'CMD_INDEX', 'LOG_LASTFILE', 'FENCE_TOTAL',
                             'FORMAT_VERSION' ]
        self.mindelta = 0.000001


    def mavset(self, mav, name, value, retries=3, parm_type=None):
        '''set a parameter on a mavlink connection'''
        got_ack = False

        if parm_type is not None and parm_type != mavutil.mavlink.MAV_PARAM_TYPE_REAL32:
            # need to encode as a float for sending
            if parm_type == mavutil.mavlink.MAV_PARAM_TYPE_UINT8:
                vstr = struct.pack(">xxxB", int(value))
            elif parm_type == mavutil.mavlink.MAV_PARAM_TYPE_INT8:
                vstr = struct.pack(">xxxb", int(value))
            elif parm_type == mavutil.mavlink.MAV_PARAM_TYPE_UINT16:
                vstr = struct.pack(">xxH", int(value))
            elif parm_type == mavutil.mavlink.MAV_PARAM_TYPE_INT16:
                vstr = struct.pack(">xxh", int(value))
            elif parm_type == mavutil.mavlink.MAV_PARAM_TYPE_UINT32:
                vstr = struct.pack(">I", int(value))
            elif parm_type == mavutil.mavlink.MAV_PARAM_TYPE_INT32:
                vstr = struct.pack(">i", int(value))
            else:
                print("can't send %s of type %u" % (name, parm_type))
                return False
            vfloat, = struct.unpack(">f", vstr)
        else:
            vfloat = float(value)
                
        while retries > 0 and not got_ack:
            retries -= 1
            mav.param_set_send(name.upper(), vfloat, parm_type=parm_type)
            tstart = time.time()
            while time.time() - tstart < 1:
                ack = mav.recv_match(type='PARAM_VALUE', blocking=False)
                if ack is None:
                    time.sleep(0.1)
                    continue
                if str(name).upper() == str(ack.param_id).upper():
                    got_ack = True
                    self.__setitem__(name, float(value))
                    break
        if not got_ack:
            print("timeout setting %s to %f" % (name, vfloat))
            return False
        return True


    def save(self, filename, wildcard='*', verbose=False):
        '''save parameters to a file'''
        f = open(filename, mode='w')
        k = list(self.keys())
        k.sort()
        count = 0
        for p in k:
            if p and fnmatch.fnmatch(str(p).upper(), wildcard.upper()):
                value = self.__getitem__(p)
                if isinstance(value, float):
                    f.write("%-16.16s %f\n" % (p, value))
                else:
                    f.write("%-16.16s %s\n" % (p, str(value)))
                count += 1
        f.close()
        if verbose:
            print("Saved %u parameters to %s" % (count, filename))


    def load(self, filename, wildcard='*', mav=None, check=True, use_excludes=True):
        '''load parameters from a file'''
        try:
            f = open(filename, mode='r')
        except Exception as e:
            print("Failed to open file '%s': %s" % (filename, str(e)))
            return False
        count = 0
        changed = 0
        for line in f:
            line = line.strip()
            if not line or line[0] == "#":
                continue
            line = line.replace(',',' ')
            a = line.split()
            if len(a) != 2:
                print("Invalid line: %s" % line)
                continue
            # some parameters should not be loaded from files
            if use_excludes and a[0] in self.exclude_load:
                continue
            if not fnmatch.fnmatch(a[0].upper(), wildcard.upper()):
                continue
            if mav is not None:
                if check:
                    if a[0] not in list(self.keys()):
                        print("Unknown parameter %s" % a[0])
                        continue
                    old_value = self.__getitem__(a[0])
                    if math.fabs(old_value - float(a[1])) <= self.mindelta:
                        count += 1
                        continue
                    if self.mavset(mav, a[0], a[1]):
                        print("changed %s from %f to %f" % (a[0], old_value, float(a[1])))
                else:
                    print("set %s to %f" % (a[0], float(a[1])))
                    self.mavset(mav, a[0], a[1])
                changed += 1
            else:
                self.__setitem__(a[0], float(a[1]))
            count += 1
        f.close()
        if mav is not None:
            print("Loaded %u parameters from %s (changed %u)" % (count, filename, changed))
        else:
            print("Loaded %u parameters from %s" % (count, filename))
        return True

    def show_param_value(self, name, value):
        print("%-16.16s %s" % (name, value))

    def show(self, wildcard='*'):
        '''show parameters'''
        k = sorted(self.keys())
        for p in k:
            if fnmatch.fnmatch(str(p).upper(), wildcard.upper()):
                self.show_param_value(str(p), "%f" % self.get(p))

    def diff(self, filename, wildcard='*', use_excludes=True):
        '''show differences with another parameter file'''
        other = MAVParmDict()
        if not other.load(filename, use_excludes=use_excludes):
            return
        keys = sorted(list(set(self.keys()).union(set(other.keys()))))
        for k in keys:
            if not fnmatch.fnmatch(str(k).upper(), wildcard.upper()):
                continue
            if not k in other:
                value = float(self[k])
                print("%-16.16s              %12.4f" % (k, value))
            elif not k in self:
                print("%-16.16s %12.4f" % (k, float(other[k])))
            elif abs(self[k] - other[k]) > self.mindelta:
                value = float(self[k])
                print("%-16.16s %12.4f %12.4f" % (k, other[k], value))
                
        
