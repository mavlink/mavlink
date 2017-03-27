'''MAVLink X25 CRC code'''


class x25crc(object):
    '''x25 CRC - based on checksum.h from mavlink library'''
    def __init__(self, buf=None):
        self.crc = 0xffff
        if buf is not None:
            if isinstance(buf, str):
                self.accumulate_str(buf)
            else:
                self.accumulate(buf)

    def accumulate(self, buf):
        '''add in some more bytes'''
        accum = self.crc
        for b in buf:
            tmp = b ^ (accum & 0xff)
            tmp = (tmp ^ (tmp<<4)) & 0xFF
            accum = (accum>>8) ^ (tmp<<8) ^ (tmp<<3) ^ (tmp>>4)
        self.crc = accum

    def accumulate_str(self, buf):
        '''add in some more bytes'''
        accum = self.crc
        import array
        bytes = array.array('B')
        bytes.fromstring(buf)
        self.accumulate(bytes)
