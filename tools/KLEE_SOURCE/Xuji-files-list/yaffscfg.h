/*
* Header file for using yaffs in an application via
* a direct interface.
*/


#ifndef __YAFFSCFG_H__
#define __YAFFSCFG_H__

#include "yaffs_guts.h"

#define YAFFSFS_N_HANDLES 200

typedef struct {
	const char *prefix;
	struct yaffs_DeviceStruct *dev;
} yaffsfs_DeviceConfiguration;


void yaffsfs_Lock(void);
void yaffsfs_Unlock(void);

__u32 yaffsfs_CurrentTime(void);

void yaffsfs_SetError(int err);



#define YAFFS_DEV_MAX        8
#define YAFFS_DEV_NAME_LEN   32

struct yaffscfg
{
    char  * devName;         // for example: /tmp /flash ...
    int   nChunksPerBlock;  // does not need to be a power of 2
    int   startBlock;       // Start block we're allowed to use
    int   endBlock;         // End block we're allowed to use
    int   nReservedBlocks;	// We want this tuneable so that we can reduce
    						// reserved blocks on NOR and RAM.

    int   nShortOpCaches;   // If <= 0, then short op caching is disabled, else
   							// the number of short op caches (don't use too many).

    int (* writeChunkToNAND )( struct yaffs_DeviceStruct * dev, int chunkInNAND, const __u8 * data, yaffs_Spare * spare );
    int (* readChunkFromNAND )( struct yaffs_DeviceStruct * dev, int chunkInNAND, __u8 * data, yaffs_Spare * spare );
    int (* eraseBlockInNAND )( struct yaffs_DeviceStruct * dev, int blockInNAND );
    int (* initialiseNAND )( struct yaffs_DeviceStruct * dev );
};

int yaffs_load( struct yaffscfg *cfgs, int n );

#endif


