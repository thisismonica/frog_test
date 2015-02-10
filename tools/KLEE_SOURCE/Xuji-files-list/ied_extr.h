#include "plat/inc/xj_cosc.h"
#include "plat/net_inc/ieddb.h"

#ifndef IED_DB_EXTR_H
#define IED_DB_EXTR_H

/* 写测控SOE数据函数 */
INT iedDB_WriteSOE( VOID * pBuffer, UNSIGNED timeout );
/* 读测控SOE数据函数 */
INT iedDB_ReadSOE( VOID * pBuffer, UNSIGNED timeout );
/* 存储用户命令函数 */
INT iedDB_WriteCommand( VOID * pBuffer, UNSIGNED timeout );
/* 读取用户命令函数 */
INT iedDB_ReadCommand( VOID * pBuffer, UNSIGNED timeout );
/* 优先级表初始化函数 */
VOID iedDB_ProirityInit( void );
#endif
