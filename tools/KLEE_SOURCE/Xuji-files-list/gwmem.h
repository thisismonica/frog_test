#ifndef     _GW_MEM_H
#define     _GW_MEM_H

#include "plat/net_inc/ieddb.h"

typedef GW_UCHAR GW_MEM_TYPE;
#define GW_DM_HEADER_SIZE 4 /* ÄÚ´æ³ØÍ·²¿size */
#ifdef __cplusplus
extern "C"
{
#endif

GW_BOOL     gwmem_CreateMemory( GW_ULONG  size, GW_VOID    * address );
GW_STATUS   gwmem_RegisterMemPool( GW_USHORT    max , GW_USHORT    size );
GW_VOID    * gwmem_alloc( GW_USHORT size );
GW_VOID     gwmem_free( GW_VOID * memory_ptr );
GW_VOID    * gwmem_SuspendAlloc( GW_USHORT size, GW_ULONG timeout );

#ifdef __cplusplus
}
#endif

#endif
