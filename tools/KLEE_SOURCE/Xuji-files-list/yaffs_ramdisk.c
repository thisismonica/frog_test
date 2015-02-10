/*yaffs ramdisk driver by shenchen*/
#include "yaffs_ramdisk.h" 

#define SIZE_IN_MB         2 
#define PAGE_SPARE_SIZE      YAFFS_BYTES_PER_SPARE 
#define PAGE_DATA_SIZE       YAFFS_BYTES_PER_CHUNK 
#define PAGES_PER_BLOCK      32   //pages per block
#define PAGE_SIZE           (PAGE_DATA_SIZE + PAGE_SPARE_SIZE)
#define BLOCK_SIZE          (PAGES_PER_BLOCK * PAGE_SIZE)
#define BLOCKS_PER_MEG      ((1024*1024)/(PAGE_DATA_SIZE * PAGES_PER_BLOCK))  //64 blocks

//ramdisk buffer, blocks,pages,data [2 * 64][32][528]
static __u8 ramdisk[SIZE_IN_MB * BLOCKS_PER_MEG][PAGES_PER_BLOCK][PAGE_SIZE];  

static int  CheckInit( yaffs_Device * dev )
{
#pragma unused(dev)
    static int initialised = 0; 

    if( initialised ) 
        return YAFFS_OK;
    memset( ramdisk, 0xff, sizeof( ramdisk ));
    initialised = 1;    

    return YAFFS_OK;
}

int yramdisk_WriteChunkToNAND( yaffs_Device * dev, int chunkInNAND, const __u8 * data, yaffs_Spare * spare )
{
#pragma unused(dev)
    int blk, pg; 

    blk = chunkInNAND / PAGES_PER_BLOCK;
    pg = chunkInNAND % PAGES_PER_BLOCK;   

    if( data )
        memcpy(& ramdisk[blk][pg][0], data, PAGE_DATA_SIZE );

    if( spare )
        memcpy(& ramdisk[blk][pg][PAGE_DATA_SIZE], spare, PAGE_SPARE_SIZE );

    return YAFFS_OK;    
}

int yramdisk_ReadChunkFromNAND( yaffs_Device * dev, int chunkInNAND, __u8 * data, yaffs_Spare * spare )
{
#pragma unused(dev)
    int blk, pg;

    blk = chunkInNAND / PAGES_PER_BLOCK;
    pg = chunkInNAND % PAGES_PER_BLOCK;

    if( data )
        memcpy( data,& ramdisk[blk][pg][0], PAGE_DATA_SIZE );

    if( spare )
        memcpy( spare,& ramdisk[blk][pg][PAGE_DATA_SIZE], PAGE_SPARE_SIZE );

    return YAFFS_OK;
}

int yramdisk_EraseBlockInNAND( yaffs_Device * dev, int blockNumber )
{
#pragma unused(dev)
    if( blockNumber < 0 || blockNumber >= ( SIZE_IN_MB * BLOCKS_PER_MEG ) )
    {
        T( YAFFS_TRACE_ALWAYS,( "Attempt to erase non-existant block %d\n", blockNumber ));
        return YAFFS_FAIL;
    }
    else
    {
        memset( ramdisk[blockNumber], 0xFF, BLOCK_SIZE );
        return YAFFS_OK;
    }   
}

int yramdisk_InitialiseNAND( yaffs_Device * dev )
{
    CheckInit( dev ); 
    dev->useNANDECC = 0;
    return YAFFS_OK;
}

struct yaffscfg ramdiskcfg = 
{
    "/ram",
    PAGES_PER_BLOCK,
    0, //start block
    SIZE_IN_MB * BLOCKS_PER_MEG - 1, //end block
    2, //reserved blocks
    0, //caches
    yramdisk_WriteChunkToNAND,
    yramdisk_ReadChunkFromNAND,
    yramdisk_EraseBlockInNAND,
    yramdisk_InitialiseNAND
};
