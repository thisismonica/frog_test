/*yaffs flash driver by shenchen*/     
#include "yaffs_flashif.h"
#include "fcm_nand.h"
#include <assert.h>

static struct nand_chip chip;
static int nand_initialized = 0;

int yflash_WriteChunkToNAND( yaffs_Device * dev, int chunkInNAND, const __u8 * data, yaffs_Spare * spare )
{   
#pragma unused(dev)
    return ( chip.write_page( chip.priv,( unsigned int )chunkInNAND, data,( unsigned char *)spare ) ? YAFFS_OK : YAFFS_FAIL );  
}

int yflash_ReadChunkFromNAND( yaffs_Device * dev, int chunkInNAND, __u8 * data, yaffs_Spare * spare )
{   
#pragma unused(dev)
    return ( chip.read_page( chip.priv,( unsigned int )chunkInNAND, data,( unsigned char *)spare ) ? YAFFS_OK : YAFFS_FAIL );   
}

int yflash_EraseBlockInNAND( yaffs_Device * dev, int blockNumber )
{
#pragma unused(dev)
    return ( chip.erase_block( chip.priv,( unsigned int )blockNumber ) ? YAFFS_OK : YAFFS_FAIL );
}

int yflash_InitialiseNAND( yaffs_Device * dev )
{
#pragma unused(dev)
    dev->useNANDECC = 0;        
    if(! nand_initialized )
    {
        if( NAND_Init(& chip, 1 ) == 1 )
        {
            nand_initialized = 1;          
            return YAFFS_OK;
        }
        else
            return YAFFS_FAIL;
    }
    else
        return YAFFS_OK;
}

/*

struct yaffscfg flashdiskcfg = 
{
    "/flash", //disk root name
    32,       //pages per block
    1024,     //start block
    4095,     //end block,total 48M bytes
    10,       //reserved blocks
    32,       //caches
    yflash_WriteChunkToNAND,
    yflash_ReadChunkFromNAND,
    yflash_EraseBlockInNAND,
    yflash_InitialiseNAND
};
*/
