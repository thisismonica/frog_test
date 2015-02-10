/*
mpc8313 nand flash driver by shenchen,200810
hardware ecc, big nand page and multi chips not support
*/
#ifndef _fcm_nand_defined_
#define _fcm_nand_defined_

#include "types.h"
/*
 * Standard NAND flash commands
 */
#define NAND_CMD_READ0      0
#define NAND_CMD_READ1      1
#define NAND_CMD_PAGEPROG   0x10
#define NAND_CMD_READOOB    0x50
#define NAND_CMD_ERASE1     0x60
#define NAND_CMD_STATUS     0x70
#define NAND_CMD_STATUS_MULTI   0x71
#define NAND_CMD_SEQIN      0x80
#define NAND_CMD_READID     0x90
#define NAND_CMD_ERASE2     0xd0
#define NAND_CMD_RESET      0xff

/* Status bits */
#define NAND_STATUS_FAIL	0x01
#define NAND_STATUS_READY	0x40
#define NAND_STATUS_WP	0x80

/*
 * NAND Flash Manufacturer ID Codes
 */
#define NAND_MFR_TOSHIBA    0x98
#define NAND_MFR_SAMSUNG    0xec
#define NAND_MFR_FUJITSU    0x04
#define NAND_MFR_NATIONAL   0x8f
#define NAND_MFR_RENESAS    0x07
#define NAND_MFR_STMICRO    0x20
#define NAND_MFR_MICRON     0x2c

/**
 * struct nand_flash_dev - NAND Flash Device ID Structure
 *
 * @name:   Identify the device type
 * @id:     device ID code
 * @pagesize:   Pagesize in bytes. Either 256 or 512 or 0
 *      If the pagesize is 0, then the real pagesize
 *      and the eraseize are determined from the
 *      extended id bytes in the chip
 * @erasesize:  Size of an erase block in the flash device.
 * @chipsize:   Total chipsize in Mega Bytes
 * @options:    Bitfield to store chip relevant options
 */
struct nand_flash_dev {
    char * name;
    int id;
    unsigned long pagesize;
    unsigned long chipsize;
    unsigned long erasesize;
    unsigned long options;
};

/**
 * struct nand_manufacturers - NAND Flash Manufacturer ID Structure
 * @name:   Manufacturer name
 * @id:     manufacturer ID code of device.
*/
struct nand_manufacturers {
    int id;
    char * name;
};

extern struct nand_flash_dev nand_flash_ids[];
extern struct nand_manufacturers nand_manuf_ids[];

struct nand_chip {
    struct nand_flash_dev * nand_info;
    int (* write_page )( void * priv, u32 page, const u8 * data, const u8 * spare );
    int (* read_page )( void * priv, u32 page, u8 * data, u8 * spare );
    int (* erase_block )( void * priv, u32 block );
    int (* initialize )( void * priv );
    void * priv;
};

#define NAND_PAGE_SIZE             512
#define NAND_SPARE_SIZE            16
#define NAND_MANUFACTURER(chip)    ((chip)->manufacturer)
#define NAND_INFO(chip)            ((chip)->nand_info)
#define NAND_BLOCK_SIZE(chip)      ((chip)->nand_info->erasesize)
#define NAND_SIZE(chip)            ((chip)->nand_info->chipsize * 1024 * 1024)
#define NAND_PAGES_PER_BLOCK(chip) (NAND_BLOCK_SIZE(chip) / NAND_PAGE_SIZE)
#define NAND_BLOCKS(chip)          (NAND_SIZE(chip) / NAND_BLOCK_SIZE(chip))
#define NAND_PAGES(chip)           (NAND_SIZE(chip) / NAND_PAGE_SIZE)

int  NAND_Init( struct nand_chip * chips, int n );

#endif //_fcm_nand_defined_
