/*ethernet npi interface protocal, by shenchen@xjgc.com,20101103*/

#ifndef _NPI_PROTOCOL_H
#define _NPI_PROTOCOL_H

#include "types.h"

/*
pack user data into npi protocol frame,return frame length.
data:   user data;
buffer: to hold packet frame;
*/
int npi_frame_pack(u8 * data, u32 data_length, u8 * buffer, u32 buffer_size);

/*
unpack received npi frame, return user data buffer.
frame:          received npi frame.
data_length:    user data length.
*/
u8 *npi_frame_unpack(u8 *frame, u32 frame_length, u32 *data_length);

#endif /*_NPI_PROTOCOL_H*/