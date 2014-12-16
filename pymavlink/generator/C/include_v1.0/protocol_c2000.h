#ifdef __cplusplus
extern "C" {
#endif

#ifndef PROTOCOL_C2000_H_
#define PROTOCOL_C2000_H_

#define X25_INIT_CRC_C2000 0xffff

static inline void mav_put_float_c2000(void* buf, int wire_offset, float b)
{
    float* dest_ptr = (float*)((uint8_t*)buf + (wire_offset / 2));
    *dest_ptr = b;
}

static inline void mav_put_uint32_t_c2000(void* buf, int wire_offset, uint32_t b)
{
    uint32_t* dest_ptr = (uint32_t*)((uint8_t*)buf + (wire_offset / 2));
    *dest_ptr = b;
}

static inline void mav_put_uint16_t_c2000(void* buf, int wire_offset, uint16_t b)
{
    uint16_t* dest_ptr = (uint16_t*)((uint8_t*)buf + (wire_offset / 2));
    *dest_ptr = b;
}

static inline void mav_put_uint8_t_c2000(void* buf, int wire_offset, uint8_t b)
{
    uint8_t* dest_ptr = (uint8_t*)buf;
    if ((wire_offset % 2) == 0) {
        dest_ptr += (wire_offset / 2);
        *dest_ptr &= 0xFF00;
        *dest_ptr |= (b & 0x00FF);
    } else {
        dest_ptr += ((wire_offset - 1) / 2);
        *dest_ptr &= 0x00FF;
        *dest_ptr |= ((b << 8) & 0xFF00);
    }
}

static inline uint32_t mav_get_uint32_t_c2000(void* buf, int wire_offset)
{
    uint32_t* dest_ptr = (uint32_t*)((uint8_t*)buf + (wire_offset / 2));
    return *dest_ptr;
}

static inline uint16_t mav_get_uint16_t_c2000(void* buf, int wire_offset)
{
    uint16_t* dest_ptr = (uint16_t*)((uint8_t*)buf + (wire_offset / 2));
    return *dest_ptr;
}

static inline uint8_t mav_get_uint8_t_c2000(void* buf, int wire_offset)
{
    uint8_t* dest_ptr = (uint8_t*)buf;
    if ((wire_offset % 2) == 0) {
        dest_ptr += (wire_offset / 2);
        return (*dest_ptr & 0x00FF);
    } else {
        dest_ptr += ((wire_offset - 1) / 2);
        return ((*dest_ptr >> 8) & 0x00FF);
    }
}

static inline void crc_init_c2000(uint16_t* crcAccum)
{
        *crcAccum = X25_INIT_CRC_C2000;
}

static inline void crc_accumulate_c2000(uint8_t data, uint16_t *crcAccum)
{
    /*Accumulate one byte of data into the CRC*/
    uint8_t tmp = 0x0000;

    // For C2000, since bytes are actually stored in the lower half of a 16-bit word,
    // need to make sure we mask off the upper half of the word so we don't accidentally accumulate
    // unwanted garbage data into the CRC
    tmp = (data & 0x00FF) ^ (uint8_t)(*crcAccum & 0x00ff);
    tmp ^= (tmp << 4);
    tmp &= 0x00FF; // Need to zero-out the upper byte of tmp, since it's intended to be an 8-bit value (but is actually 16-bits),
                   // and rolling it back into the 16-bit crcAccum with garbage in the upper byte will corrupt the CRC
    *crcAccum = (*crcAccum >> 8) ^ (tmp << 8) ^ (tmp << 3) ^ (tmp >> 4);
}

static inline uint16_t crc_calculate_c2000(const uint8_t* pBuffer, uint16_t length)
{
    // The only difference between this and the regular version is that this calls the C2000 specific
    // version of crc_accumulate to make sure that 16-bit "bytes" are handled correctly

    uint16_t crcTmp;
    crc_init(&crcTmp);
    while (length--) {
        crc_accumulate_c2000(*pBuffer++, &crcTmp);
    }
    return crcTmp;
}

static inline void crc_accumulate_msg_payload_c2000(uint16_t *crcAccum, void* payload, uint16_t length)
{
    int crc_bytes_accumulated = 0;
    while (crc_bytes_accumulated < length) {
        crc_accumulate_c2000(mav_get_uint8_t_c2000(payload, crc_bytes_accumulated++), crcAccum);
    }
}

#endif /* PROTOCOL_C2000_H_ */

#ifdef __cplusplus
}
#endif
