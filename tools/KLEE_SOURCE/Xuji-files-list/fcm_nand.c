#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "fcm_nand.h"
#include "cfg_8313xj.h"
#include "immap_83xx.h"
#include "mpc83xx.h"
#include "xj_cosc.h"

#define NAND_OP_TIMEOUT 0x4000

struct nand_flash_dev nand_flash_ids[] = {
    {   "NAND 1MiB 5V 8-bit",      0x6e, 256, 1, 0x1000, 0},
    {   "NAND 2MiB 5V 8-bit",      0x64, 256, 2, 0x1000, 0},
    {   "NAND 4MiB 5V 8-bit",      0x6b, 512, 4, 0x2000, 0},
    {   "NAND 1MiB 3,3V 8-bit",    0xe8, 256, 1, 0x1000, 0},
    {   "NAND 1MiB 3,3V 8-bit",    0xec, 256, 1, 0x1000, 0},
    {   "NAND 2MiB 3,3V 8-bit",    0xea, 256, 2, 0x1000, 0},
    {   "NAND 4MiB 3,3V 8-bit",    0xd5, 512, 4, 0x2000, 0},
    {   "NAND 4MiB 3,3V 8-bit",    0xe3, 512, 4, 0x2000, 0},
    {   "NAND 4MiB 3,3V 8-bit",    0xe5, 512, 4, 0x2000, 0},
    {   "NAND 8MiB 3,3V 8-bit",    0xd6, 512, 8, 0x2000, 0},
    {   "NAND 8MiB 1,8V 8-bit",    0x39, 512, 8, 0x2000, 0},
    {   "NAND 8MiB 3,3V 8-bit",    0xe6, 512, 8, 0x2000, 0},
    {   "NAND 16MiB 1,8V 8-bit",   0x33, 512, 16, 0x4000, 0},
    {   "NAND 16MiB 3,3V 8-bit",   0x73, 512, 16, 0x4000, 0},
    {   "NAND 32MiB 1,8V 8-bit",   0x35, 512, 32, 0x4000, 0},
    {   "NAND 32MiB 3,3V 8-bit",   0x75, 512, 32, 0x4000, 0},
    {   "NAND 64MiB 1,8V 8-bit",   0x36, 512, 64, 0x4000, 0},
    {   "NAND 64MiB 3,3V 8-bit",   0x76, 512, 64, 0x4000, 0},
    {   "NAND 128MiB 1,8V 8-bit",  0x78, 512, 128, 0x4000, 0},
    {   "NAND 128MiB 3,3V 8-bit",  0x79, 512, 128, 0x4000, 0},
    {   "NAND 256MiB 3,3V 8-bit",  0x71, 512, 256, 0x4000, 0},
    {   "NAND 512MiB 3,3V 8-bit",  0xDC, 512, 512, 0x4000, 0},
    {   0, }
};

/* Private structure holding NAND Flash device specific information */
struct fcm_nand 
{
    struct nand_chip  * chip;
    volatile lbus83xx_t * regs;    
    unsigned int  bank;  
    unsigned int  base;
    unsigned char * buf; 
};

/*
 * Set up the FCM hardware block and page address fields, and the fcm
 * structure addr field to point to the correct FCM buffer in memory
 */
static void set_addr( struct fcm_nand * fcm, u32 page, const u8 * data, const u8 * spare )
{
    volatile lbus83xx_t * regs = fcm->regs; 
    regs->fbar = page / NAND_PAGES_PER_BLOCK( fcm->chip ); 
    page %= NAND_PAGES_PER_BLOCK( fcm->chip );
    regs->fpar = (( page << FPAR_SP_PI_SHIFT ) & FPAR_SP_PI );
    fcm->buf = ( unsigned char *)( fcm->base + ( page & 7 ) * 1024 ); //get buf ptr,1024bytes buf per page
    if((! data ) && spare ) //only spare data need to be handle
    {
        regs->fpar |= FPAR_SP_MS;
        fcm->buf += NAND_PAGE_SIZE;
    }
}

static int fcm_run_command( struct fcm_nand * fcm )
{
    unsigned int timeout;
    volatile lbus83xx_t * regs = fcm->regs;

    regs->lteatr = 0;
    regs->ltesr |= ( LTESR_FCT | LTESR_CC );
    regs->fmr |= FMR_OP;
    regs->lsor = fcm->bank;   
    for( timeout = 0; timeout < NAND_OP_TIMEOUT; timeout ++) //timeout loop
    {
        if( regs->ltesr & LTESR_FCT )
        {
            return 0;    
        }
        if( regs->ltesr & LTESR_CC )
        {
            return 1;    
        }
    }   
    return 0;
}

/* this function is called after Program and Erase Operations to
 * check for success or failure */
static int fcm_wait_opstatus( struct fcm_nand * fcm )
{   
    volatile lbus83xx_t * regs = fcm->regs;
    unsigned int timeout = NAND_OP_TIMEOUT;
    unsigned char status;
    while( timeout --)
    {
        regs->fir = ( FIR_OP_CM0 << FIR_OP0_SHIFT ) |
            ( FIR_OP_RS << FIR_OP1_SHIFT );
        regs->fcr = ( NAND_CMD_STATUS << FCR_CMD0_SHIFT );   
        if( fcm_run_command( fcm ))
        {
            status = regs->mdr;
            if( status & NAND_STATUS_READY )
            {
                if(  status & NAND_STATUS_FAIL )
                    return 0;
                else
                    return 1;
            }
        }
    }
    return 0;
}

static int get_chip_id( struct fcm_nand * fcm )
{
    volatile lbus83xx_t * regs = fcm->regs;
    regs->fir = ( FIR_OP_CW0 << FIR_OP0_SHIFT ) 
        | ( FIR_OP_UA  << FIR_OP1_SHIFT ) 
        | ( FIR_OP_RS << FIR_OP2_SHIFT ) 
        | ( FIR_OP_RS << FIR_OP3_SHIFT ) 
        | ( FIR_OP_RS << FIR_OP4_SHIFT ) 
        | ( FIR_OP_RS << FIR_OP5_SHIFT ) ;
    regs->fcr = ( NAND_CMD_READID << FCR_CMD0_SHIFT );
    regs->mdr = 0;
    if( fcm_run_command( fcm ))
        return (( regs->mdr >> 8 ) & 0xff ); //return device code
    return 0;
}

static struct nand_flash_dev * get_chip_info( int chip_id )
{
    struct nand_flash_dev * inf = nand_flash_ids;
    while( inf->name )
    {
        if( inf->id == chip_id )
            return inf;
        else
            inf ++;
    }
    return 0;
}

static int fcm_write_page( void * priv, u32 page, const u8 * data, const u8 * spare )
{
    struct fcm_nand * fcm = ( struct fcm_nand *)priv;
    volatile lbus83xx_t * regs = fcm->regs;

    if(! data && ! spare )
        return 0;        

    regs->fir = ( FIR_OP_CW0 << FIR_OP0_SHIFT ) |
        ( FIR_OP_CM2 << FIR_OP1_SHIFT ) |
        ( FIR_OP_CA  << FIR_OP2_SHIFT ) |
        ( FIR_OP_PA  << FIR_OP3_SHIFT ) |
        ( FIR_OP_WB  << FIR_OP4_SHIFT ) |
        ( FIR_OP_CW1 << FIR_OP5_SHIFT );           

    if (! data && spare ) 
    {
        regs->fcr = ( NAND_CMD_READOOB << FCR_CMD0_SHIFT )
            | ( NAND_CMD_PAGEPROG << FCR_CMD1_SHIFT )
            | ( NAND_CMD_SEQIN << FCR_CMD2_SHIFT );     
    }
    else
    {
        regs->fcr = ( NAND_CMD_READ0 << FCR_CMD0_SHIFT )
            | ( NAND_CMD_PAGEPROG << FCR_CMD1_SHIFT )
            | ( NAND_CMD_SEQIN << FCR_CMD2_SHIFT );
    } 

    set_addr( fcm, page, data, spare );

    if(! data && spare )
    {
        regs->fbcr = NAND_SPARE_SIZE;
        memcpy( fcm->buf, spare, NAND_SPARE_SIZE );
    }
    else if( data && ! spare )
    {
        regs->fbcr = NAND_PAGE_SIZE;
        memcpy( fcm->buf, data, NAND_PAGE_SIZE );
    }
    else //if(data && spare)
    {
        regs->fbcr = NAND_SPARE_SIZE + NAND_PAGE_SIZE;
        memcpy( fcm->buf, data, NAND_PAGE_SIZE );
        memcpy( fcm->buf + NAND_PAGE_SIZE, spare, NAND_SPARE_SIZE );
    }

    if( fcm_run_command( fcm ))
    {
        if(  fcm_wait_opstatus( fcm ) )
            return 1;
    }
    return 0;
}

static int fcm_read_page( void * priv, u32 page, u8 * data, u8 * spare )
{
    struct fcm_nand * fcm = ( struct fcm_nand *)priv;
    volatile lbus83xx_t * regs = fcm->regs;

    if(! data && ! spare )
        return 0;       
    regs->fir = ( FIR_OP_CW0 << FIR_OP0_SHIFT ) |
        ( FIR_OP_CA  << FIR_OP1_SHIFT ) |
        ( FIR_OP_PA  << FIR_OP2_SHIFT ) |
        ( FIR_OP_RBW << FIR_OP3_SHIFT );

    if(! data && spare )
        regs->fcr = ( NAND_CMD_READOOB << FCR_CMD0_SHIFT );
    else        
        regs->fcr = ( NAND_CMD_READ0 << FCR_CMD0_SHIFT );    

    set_addr( fcm, page, data, spare );    
    if(! data && spare )
        regs->fbcr = NAND_SPARE_SIZE;
    else if( data && ! spare )
        regs->fbcr = NAND_PAGE_SIZE;
    else //if(data && spare)
        regs->fbcr = NAND_SPARE_SIZE + NAND_PAGE_SIZE;
    if( fcm_run_command( fcm ))
    {
        if(! data && spare )
            memcpy( spare, fcm->buf, NAND_SPARE_SIZE );
        else if( data && ! spare )
            memcpy( data, fcm->buf, NAND_PAGE_SIZE );
        else //if(data && spare)
        {
            memcpy( data, fcm->buf, NAND_PAGE_SIZE );
            memcpy( spare, fcm->buf + NAND_PAGE_SIZE, NAND_SPARE_SIZE );
        }
        return 1;
    }
    else
        return 0;  
}

static unsigned char local_data[512], local_spare[16];
static int fcm_is_block_erased( void * priv, u32 block )
{
    int i, j;
    for( i = 0; i < 32; i ++)
    {
        if( fcm_read_page( priv, block * 32 + i, local_data, local_spare ))
        {
            for( j = 0; j < 512; j ++)
            {
                if( local_data[j] != 0xff )
                    return 0;               
            }

            for( j = 0; j < 16; j ++)
            {
                if( local_spare[j] != 0xff )
                    return 0;
            }
        }
    }   
    return 1;
}

static int fcm_erase_block( void * priv, u32 block )
{
    struct fcm_nand * fcm = ( struct fcm_nand *)priv;
    volatile lbus83xx_t * regs = fcm->regs;
    int i = 0, j = 0;

    regs->fir = ( FIR_OP_CW0 << FIR_OP0_SHIFT )|
        ( FIR_OP_PA  << FIR_OP1_SHIFT )|
        ( FIR_OP_CM1 << FIR_OP2_SHIFT ) ; 
    regs->fcr = ( NAND_CMD_ERASE1 << FCR_CMD0_SHIFT )|
        ( NAND_CMD_ERASE2 << FCR_CMD1_SHIFT );
    regs->fbar = block; 
    regs->fpar = 0;
    if( fcm_run_command( fcm ))
    {
		NU_Sleep(1);
        if( fcm_wait_opstatus( fcm ))
        {            
			if(fcm_is_block_erased(priv, block))
	            return 1;
        }
    }

    return 0;
}

static int fcm_initialize( void * priv )
{
#pragma unused(priv)
}

int  NAND_Init( struct nand_chip * chips, int n )
{
    static struct fcm_nand fcm;
    volatile immap_t * im = ( immap_t *) CFG_IMMR;
    volatile lbus83xx_t * regs = & im->lbus;
    struct nand_chip * chip = chips;
    unsigned int bank;

    if((! n ) || (! chips ))
        return 0;

    /* Enable FCM detection of timeouts and completion */
    regs->ltedr &= ~( LTESR_FCT | LTESR_CC ); 

    /* add to ECCM mode set in fcm_init */
    regs->fmr = 12 << FMR_CWTO_SHIFT |  /* Timeout > 12 mSecs */
        2 << FMR_AL_SHIFT;

    /* Find which chip select bank is being used for this device */
    for ( bank = 0; bank < 8; bank ++) 
    {
        if ( ( regs->bank[bank].br & BR_V ) &&
            ( ( regs->bank[bank].br & BR_MSEL ) == BR_MS_FCM ) &&
            ( ( regs->bank[bank].br & BR_BA ) == 
            (( regs->bank[bank].or & OR_FCM_AM ) & CFG_NAND_BASE ) ) ) 
        {
            fcm.bank = bank;
            fcm.base = regs->bank[bank].br & BR_BA;
            fcm.buf = ( unsigned char *)fcm.base;
            fcm.regs = regs;
            chip->nand_info = get_chip_info( get_chip_id(& fcm ));
            if(! chip->nand_info )
                return 0;
            else
            {
                fcm.chip = chip;
                chip->priv = ( void *)& fcm;
                chip->write_page = fcm_write_page;
                chip->read_page = fcm_read_page;
                chip->erase_block = fcm_erase_block;
                chip->initialize = fcm_initialize;          
                return 1;
            }
        }       
    }    
    return 0;
}
