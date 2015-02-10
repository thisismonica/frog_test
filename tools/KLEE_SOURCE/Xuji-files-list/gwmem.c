
#include "plat/inc/xj_cosc.h"
#include "plat/net_inc/gwmem.h"

NU_MEMORY_POOL  GW_DM_Memory;

GW_BOOL   gwmem_CreateMemory( GW_ULONG  size, GW_VOID  * address )
{
    return ( NU_Create_Memory_Pool(& GW_DM_Memory, "GW DM",
        address, size, 1, NU_FIFO ) == NU_SUCCESS );
}

GW_VOID * gwmem_SuspendAlloc( GW_USHORT size, GW_ULONG timeout )
{
    GW_VOID * pointer;
    pointer = GW_NULL;
    if ( NU_Allocate_Memory(& GW_DM_Memory,
        & pointer, size, timeout ) == NU_SUCCESS )
    {
        return pointer;
    }
    return GW_NULL;
}

GW_VOID * gwmem_alloc( GW_USHORT size )
{
    GW_VOID * pointer;
    pointer = GW_NULL;
    if ( NU_Allocate_Memory(& GW_DM_Memory,
        & pointer, size, NU_NO_SUSPEND ) == NU_SUCCESS )
    {
        return pointer;
    }
    return GW_NULL;
}

GW_VOID gwmem_free( GW_VOID * memory_ptr )
{
    NU_Deallocate_Memory( memory_ptr );
    return;
}
