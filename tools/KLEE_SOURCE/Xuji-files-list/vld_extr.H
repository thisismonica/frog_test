#ifndef _VLD_EXTR_H
#define _VLD_EXTR_H

#include "plat/net_inc/lc_defs.h"
#include "plat/net_inc/vld_defs.h"

INT VLD_AckData( UNSIGNED_CHAR * dst, GW_SOE * pSoe );
INT VLD_LoadMenu( UNSIGNED_CHAR * dst, GW_CHANNEL   * pChl, VOID  * pCB );
INT VLD_LoadConfig( UNSIGNED_CHAR * dst, GW_CHANNEL   * pChl, VOID  * pCB );
INT VLD_LoadData( GW_CHANNEL * pChl, GW_COMMAND * pCmd, UNSIGNED_CHAR * src, INT port );
/* 上载VLD数据帧数和CRC码 */
INT VLD_ProcessVldCFG( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port );
/* 上载VLD数据 */
INT VLD_ProcessVldData( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port );
/* 上载背景数据 */
INT VLD_ProcessVldMenu( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port );
/* 注册上调vld数据所需的函数 */
VOID VLD_RegisterCommand_VLD( GW_CHANNEL * pChl );
#endif
