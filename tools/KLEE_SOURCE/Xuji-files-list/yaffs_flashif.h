/*yaffs flash driver by shenchen*/
#ifndef __YAFFS_FLASH_H__
#define __YAFFS_FLASH_H__

#include "yaffscfg.h"

extern struct yaffscfg flashdiskcfg;
int yflash_EraseBlockInNAND( yaffs_Device * dev, int blockNumber );
int yflash_WriteChunkToNAND( yaffs_Device * dev, int chunkInNAND, const __u8 * data, yaffs_Spare * spare );
int yflash_ReadChunkFromNAND( yaffs_Device * dev, int chunkInNAND, __u8 * data, yaffs_Spare * spare );
int yflash_EraseBlockInNAND( yaffs_Device * dev, int blockNumber );
int yflash_InitialiseNAND( yaffs_Device * dev );

#endif
