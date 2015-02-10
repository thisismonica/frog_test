/*
 * YAFFS: Yet another FFS. A NAND-flash specific file system.
 * yaffscfg.c  The configuration for the "direct" use of yaffs.
 *
 * This file is intended to be modified to your requirements.
 * There is no need to redistribute this file.
 */
 
/*yaffs文件系统初始化代码			沈沉，20080923*/
#include <errno.h> 
#include "yaffscfg.h" 
#include "yaffsfs.h" 
#include "xj_cosc.h"
#include "es_plat.h"

extern volatile UNSIGNED TMD_System_Clock;
unsigned yaffs_traceMask = 0xFFFFFFFF; 
int yaffs_errno; 

void yaffsfs_SetError( int err ){    //Do whatever to set error
    yaffs_errno = err;
}

#define YAFFS_SEMA_NAME  "yaffs"
/*static*/ NU_SEMAPHORE yaffs_sema;
unsigned int yaffs_lock_counter = 0;

void yaffsfs_Lock( void )
{
    XJP_Mutex_Obtain(& yaffs_sema, NU_SUSPEND );
    yaffs_lock_counter ++;
}

void yaffsfs_Unlock( void )
{
    XJP_Mutex_Release(& yaffs_sema );
}

#ifdef WIN32
#include <time.h>
__u32 yaffsfs_CurrentTime( void )
{   
    return ( __u32 )time(NULL);
}
#else
UNSIGNED PGFC_Get_Time_Second_1970( VOID );
__u32 yaffsfs_CurrentTime( void )
{   
    return ( __u32 )PGFC_Get_Time_Second_1970();
}
#endif

static void yaffsfs_LocalInitialisation( void )
{
    NU_Create_Semaphore(& yaffs_sema, YAFFS_SEMA_NAME, 1, NU_FIFO );
}

/*static*/ yaffsfs_DeviceConfiguration yaffsfs_config[YAFFS_DEV_MAX + 1];
yaffs_Device  yaffs_devs[YAFFS_DEV_MAX + 1];
int yaffs_load( struct yaffscfg * cfgs, int n )
{
    //static yaffs_Device  devs[YAFFS_DEV_MAX + 1];
    yaffs_Device  *devs = yaffs_devs;
    struct yaffscfg * cfg = cfgs;
    int i = 0;

    memset( devs, 0, sizeof( devs ));
    if( ( n <= 0 )  || ( n > YAFFS_DEV_MAX ) )
        return - 1;

    for( i = 0; i < n; i ++ ,cfg ++)
    {
        if( ( cfg->nChunksPerBlock == 0 ) ||
            ( cfg->nReservedBlocks == 0 ) ||
            ( cfg->writeChunkToNAND == 0 ) ||
            ( cfg->readChunkFromNAND == 0 ) ||
            ( cfg->eraseBlockInNAND == 0 ) ||
            ( cfg->initialiseNAND == 0 ) ||
            ( cfg->devName == 0 ) ||
            ( strlen( cfg->devName )) ==  0 )
            return - 1;

        devs[i].endBlock   = cfg->endBlock;
        devs[i].genericDevice = ( void *)i;
        devs[i].nBytesPerChunk = YAFFS_BYTES_PER_CHUNK;
        devs[i].nChunksPerBlock = cfg->nChunksPerBlock;
        devs[i].nReservedBlocks = cfg->nReservedBlocks;
        devs[i].nShortOpCaches = cfg->nShortOpCaches;
        devs[i].startBlock = cfg->startBlock;
        devs[i].writeChunkToNAND = cfg->writeChunkToNAND;  
        devs[i].readChunkFromNAND = cfg->readChunkFromNAND; 
        devs[i].eraseBlockInNAND = cfg->eraseBlockInNAND;  
        devs[i].initialiseNAND = cfg->initialiseNAND;   
        yaffsfs_config[i].prefix = cfg->devName;
        yaffsfs_config[i].dev = & devs[i];

        if( cfg->initialiseNAND( yaffsfs_config[i].dev ) != YAFFS_OK )
            return - 1;
    }
    yaffsfs_config[i].prefix = NULL;
    yaffsfs_config[i].dev = NULL;

    yaffsfs_LocalInitialisation();
    yaffs_initialise( yaffsfs_config );    

    for( i = 0; i < n; i ++)
    { //mount all partion
        if( yaffs_mount( yaffsfs_config[i].prefix ) != 0 )
            break;
    }

    if( i && ( i !=  n ))
    { //clean up if failed
        for( n = 0; n < i; n ++)
            yaffs_unmount( yaffsfs_config[i].prefix );
        return - 1;
    }

    return 0;
}
