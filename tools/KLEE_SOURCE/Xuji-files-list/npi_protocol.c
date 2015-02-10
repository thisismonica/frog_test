/*ethernet npi interface protocal, by shenchen@xjgc.com,20101103*/

#include <string.h>
#include "npi_protocol.h"
#include "Es_plat.h"

#define FRAME_PREFIX_0  0xEB
#define FRAME_PREFIX_1  0x90
#define FRAME_SUBFFIX   0xBD

#define FRAME_DATA_OFFSET   0x06
#define FRAME_EXT_LENGTH    9   /*frame prefix + length * 2 + CRC + subffix */

#pragma pack(push)
#pragma pack(1)
struct npi_frame_header 
{
	u8  prefix0;
    u8  prefix1;
    u16 length0;
    u16 length1;
};
#pragma pack(pop)

#define swap16(x) (u16)( ((u16)(x) >> 8)  | ((u16)(x) << 8) )

int npi_frame_pack(u8 * data, u32 data_length, u8 * buffer, u32 buffer_size)
{
    u8 *ptr;
    UNSIGNED crc;
    struct npi_frame_header *header = (struct npi_frame_header *)buffer;

    if( (data_length + FRAME_EXT_LENGTH) > buffer_size)
        return 0;

    header->prefix0 = FRAME_PREFIX_0;
    header->prefix1 = FRAME_PREFIX_1;
    header->length0 = swap16(data_length);
    header->length1 = header->length0;
    ptr = buffer + FRAME_DATA_OFFSET;
    memcpy(ptr, data, data_length);
    ptr += data_length;
    
    XJP_CRC_Calculate( data, data_length, 1, 16, 0,& crc );
    crc ^= 0x0000;
    * ptr++ = crc;
    * ptr++ = crc >> 8;
    * ptr++ = FRAME_SUBFFIX;

    return (data_length + FRAME_EXT_LENGTH);
}

u8 *npi_frame_unpack(u8 *frame, u32 frame_length, u32 *data_length)
{
    u8 *ptr;
    UNSIGNED calc_crc, crc;
    u16 len;
    struct npi_frame_header *header = (struct npi_frame_header *)frame;

    if( (header->prefix0 != FRAME_PREFIX_0) 
        || (header->prefix1 != FRAME_PREFIX_1)
        || (header->length0 != header->length1) )
        return NULL;
    len = swap16(header->length0);
    if( (len + FRAME_EXT_LENGTH ) > frame_length)
        return NULL;

    ptr = frame + FRAME_DATA_OFFSET;
    XJP_CRC_Calculate( ptr, len, 1, 16, 0,& calc_crc );
    calc_crc ^= 0x0000;
    
    ptr += len;
    crc = *ptr++;
    crc |= (*ptr ++) << 8;

    if( ( calc_crc != crc ) || (*ptr != FRAME_SUBFFIX) )
        return NULL;

    *data_length = len;
    return (frame + FRAME_DATA_OFFSET);
}

