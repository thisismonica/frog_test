#ifndef IED_DB_DEFS_H
#define IED_DB_DEFS_H

#include "plat/net_inc/ieddb.h"
#include "plat_specific/inc/cpc_defs.h"

#define IED_SOE_DATA_OFFSET     2                       /* 在SOE管理缓冲区中数据偏移位置 */
#define IED_FCK_SOE_MAX_COUNT   CPC_FCK_SOE_MAX_COUNT   /* 支持的最大测控SOE的个数 */
#define IED_FCK_SOE_SIZE       (sizeof( GW_SOE )) /* 测控SOE的size */
/* 测控SOE的管理句柄最大数据长度(数据UNSIGNED个数 + 长度本身 + 报文实际长度) */
#define IED_FCK_SOE_BUFFER_LENGTH  (((IED_FCK_SOE_SIZE+sizeof(UNSIGNED)-1)/sizeof(UNSIGNED))+IED_SOE_DATA_OFFSET)

#define IED_COMMAND_DATA_OFFSET      2                           /* 在测控读命令管理缓冲区中数据偏移位置 */
#define IED_READ_COMMAND_MAX_COUNT  CPC_READ_COMMAND_MAX_COUNT   /* 支持的最大测控读命令的个数 */
#define IED_READ_COMMAND_SIZE  (sizeof( GW_COMMAND ))/* 测控读命令的缓冲区 */
/* 测控读命令的管理句柄最大数据长度(数据UNSIGNED个数 + 长度本身 + 报文实际长度) */
#define IED_READ_COMMAND_BUFFER_LENGTH  (((IED_READ_COMMAND_SIZE+sizeof(UNSIGNED)-1)/sizeof(UNSIGNED))+IED_COMMAND_DATA_OFFSET)

#define IED_FCK_SOE_MAX_HANDLE          2   /* 测控SOE最大管理句柄个数 */
#define IED_FCK_COMMAND_MAX_HANDLE      2   /* 测控读命令最大管理句柄个数 */
#define IED_SOE_PROIRITY_BUFF_SIZE      32  /* 优先级缓冲区大小 */
#define IED_COMMAND_PROIRITY_BUFF_SIZE  32  /* 优先级缓冲区大小 */

#endif
