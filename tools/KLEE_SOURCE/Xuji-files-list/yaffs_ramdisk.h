/*yaffs ramdisk driver by shenchen*/
#ifndef __YAFFS_RAMDISK_H__
#define __YAFFS_RAMDISK_H__

#include "yaffscfg.h"
int yramdisk_EraseBlockInNAND( yaffs_Device * dev, int blockNumber );
int yramdisk_WriteChunkToNAND( yaffs_Device * dev, int chunkInNAND, const __u8 * data, yaffs_Spare * spare );
int yramdisk_ReadChunkFromNAND( yaffs_Device * dev, int chunkInNAND, __u8 * data, yaffs_Spare * spare );
int yramdisk_EraseBlockInNAND( yaffs_Device * dev, int blockNumber );
int yramdisk_InitialiseNAND( yaffs_Device * dev );

extern struct yaffscfg ramdiskcfg;

#endif
