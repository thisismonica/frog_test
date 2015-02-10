#ifndef     _VLD_DEFS_H
#define     _VLD_DEFS_H

#include "plat/inc/xj_cosc.h"
#include "plat/net_inc/lc_defs.h"
#include "plat/net_inc/commu.h"

/* 基本数据定义 */
#define VLD_BYTE_BITS             8           /* the bits of the byte */
#define VLD_DATA_MASKS8           0xFF        /* the masks bits for 8 bits */
#define VLD_DATA_SIZE_2           2           /* 一个数据为两个字节 */
#define VLD_DATA_SIZE_4           4           /* 一个数据为四个字节 */
#define VLD_ONE_MAXDATA_SIZE      50          /* 一次读取背景数据的最大个数 */
#define VLD_MENU_AVALABLE         0x55AA962E  /* VLD数据是否有效标志 */

/* 通讯报文中位置偏移定义 */
#define VLD_LOCAL_OFFSET_TI       1      /* 报文中命令类型位置 */
#define VLD_LOCAL_OFFSET_CPU      2      /* cpu号位置 */
#define VLD_LOCAL_OFFSET_SERIAL_L 3      /* 序列号低位 */
#define VLD_LOCAL_OFFSET_SERIAL_H 4      /* 序列号高位 */
#define VLD_LOCAL_OFFSET_CONTEXT  5      /* 数据位置 */

/* 执行结果定义 */
#define VLD_DATA_OK               0      /* 数据正确 */
#define VLD_DATA_ERROR            1      /* 数据错误 */
#define VLD_DATA_TIMEOUT          2      /* 数据超时 */

typedef struct STRUCT_XJ_VLD_MENU
{
    UNSIGNED            config_CRC;  /* 配置CRC */
    UNSIGNED            crc;         /* 数据 CRC */
    UNSIGNED            version;     /* 数据版本 */
    INT                 length;      /* 数据长度 */
    UNSIGNED            available;   /* 有效标志 */
    INT                 scrNumber;   /* 屏总数 */
}XJ_VLD_MENU;

typedef struct STRUCT_GW_VLD_CB
{
    GW_REQUEST_CB   head;
    INT             Serial_L;
    INT             Serial_H;
    INT             offset;
    INT             number;
}GW_VLD_CB;

#endif
