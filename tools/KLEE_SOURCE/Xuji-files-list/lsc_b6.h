
#include "plat/inc/xj_cosc.h"
#include "plat_specific/plat_specific.h"
#include "plat/net_inc/ls_extr.h"
#include "plat/net_inc/commu.h"
#include "plat/es_plat.h"

#ifndef LSC_B6_H
#define LSC_B6_H

#define LSC_B6_MAX_EVENT           6     /* b6命令中最大管理事件个数 */
#define LSC_B6_PROIRITY_BUFF_SIZE  32    /* 优先级缓冲区大小 */

/* B6命令中突变报文上送类型定义 */
#define LSC_B6_TYPE_DYNAMIC_DATA    0  /* 突变上送,后台遥控,动态数据 */
#define LSC_B6_TYPE_REPORT          1  /* 突变报告上送 */
#define LSC_B6_TYPE_EVENT           2  /* 突变事件上送 */
#define LSC_B6_TYPE_FCK_YC          5  /* 突变遥测数据上送 */
#define LSC_B6_TYPE_FAULT_RETURN    6  /* 故障返回上送 */
#define LSC_B6_TYPE_WAVE            7  /* 录波序号发生变化 */
#define LSC_B6_TYPE_TABFIELD_SUDDEN 8  /* 数据表指定字段突变信息 */

/* 全局变量 */
extern HDCM       RamEventHandle;
extern HDCM       CommEventHandle;
extern HDCM       CommFltRetHandle;
extern NU_PROTECT LSD_Report_Protect;
extern CREPORT CReport;
/*========使用函数声明========*/
/* 突变上送事件管理 */
INT LSC_B6_CommEvent( VOID * Dst );
/* 故障报告发生变化,上送故障报告序号 */
INT LSC_B6_FaultReport( VOID * Dst );
/* 突变事件上送 */
INT LSC_B6_RamEvent( VOID * Dst );
/* 故障返回 */
INT LSC_B6_FaultRet( VOID * Dst );
/* 录波序号发生变化 */
INT LSC_B6_WaveIndexChange( VOID * Dst );
/* B6命令报文管理函数 */
INT LSC_B6_MessageManagFun( VOID * Dst );
/* 数据表指定字段突变报文 */
INT LSC_B6_TableFieldSudden( VOID * Dst );
/*============================*/
/* 函数类型定义 */
typedef INT (* B6_ManagFun )( VOID * Dst );
/*============================*/
/* 优先级缓冲区初始化函数 */
VOID LSC_B6_ProirityInit( void );
#endif
