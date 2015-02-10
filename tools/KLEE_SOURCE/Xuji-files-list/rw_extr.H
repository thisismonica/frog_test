#ifndef  RW_EXTR_H
#define  RW_EXTR_H

#include "plat/net_inc/rw_defs.h"

INT     lcApp_ProcessDISTORDR( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port );
INT     lcApp_ProcessDISTACK( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port );
INT     lcApp_AckRW( UNSIGNED_CHAR * dst, GW_CHANNEL    * pChl, VOID    * pCB );
INT     RWC_Check_Disturbance( GW_CHANNEL * pChl );
INT     RWC_Fill_Disturbance( GW_CHANNEL * pChl, UNSIGNED_CHAR * dst );
STATUS  RWC_Initialize( GW_CHANNEL * pChl );
UNSIGNED gwDIST_GetInputInf( UNSIGNED group, UNSIGNED mask_bit );
UNSIGNED gwDIST_GetOutputInf( UNSIGNED group, UNSIGNED mask_bit );
UNSIGNED gwDIST_GetStateInf( UNSIGNED group, UNSIGNED mask_bit );
UNSIGNED gwDIST_GetLogicInf( UNSIGNED group, UNSIGNED mask_bit );
VOID gwDIST_CheckDTAG( GW_RW_CB * pCB, UNSIGNED total, UNSIGNED * groups,
    UNSIGNED * mask, DIST_GET_INF gwDIST_GetInf );

#endif
