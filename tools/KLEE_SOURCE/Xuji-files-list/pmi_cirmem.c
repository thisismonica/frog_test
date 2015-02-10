
/*************************************************************************/
/*                                                                       */
/*        Copyright (c) 1999-2002 XJ ELECTRIC CO.LTD.                    */
/*                                                                       */
/* PROPRIETARY RIGHTS of XJ ELECTRIC CO.LTD are involved in the          */
/* subject matter of this material.  All manufacturing, reproduction,    */
/* use, and sales rights pertaining to this subject matter are governed  */
/* by the license agreement.  The recipient of this software implicitly  */
/* accepts the terms of the license.                                     */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*                                                                       */
/* FILE NAME                                            VERSION          */
/*                                                                       */
/*      pmi_cirmem.c                                    Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      PMI - Platform Initialize                                        */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the initialization routine for this           */
/*      component.                                                       */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* DATA STRUCTURES                                                       */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* FUNCTIONS                                                             */
/*                                                                       */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include <string.h>
#include <math.h>
#include "plat/inc/xj_cosc.h"
#include "plat_specific/plat_specific.h"
#include "plat/inc/esp_cfg.h"
#include "plat/net_inc/rcc_defs.h"
#include "plat/net_inc/ied_defs.h"
#include "plat/net_inc/ied_extr.h"
#include "plat/es_plat.h"

/* Define external inner-component global data references.  */

extern ESP_CFG PLAT_cfg;

HDCM RamEventHandle;      /* RAM区事件管理句柄 */
HDCM NVRamEventHandle;    /* NVRAM区事件管理句柄 */
HDCM RamReportHandle;     /* RAM区动作报告循环缓冲区管理句柄 */
HDCM CommEventHandle;     /* 通讯中事件管理句柄 */
HDCM CommFltRetHandle;    /* 通讯中故障返回管理句柄 */
HDCM FckPrimarySOEHandle; /* 测控SOE管理句柄(一级数据) */
HDCM FckSecondSOEHandle;  /* 测控SOE管理句柄(二级数据) */
HDCM FckPrimaryCommandHandle; /* 测控用户命令管理句柄(一级数据) */
HDCM FckSecondCommandHandle;  /* 测控用户命令管理句柄(二级数据) */
HDCM NPIUserDataHandle;       /* 接收NPI用户数据管理句柄 */

extern HDCM IEDDB_FckSoeHandle[]; /* FckSoe数据句柄 */
extern HDCM IEDDB_FckCommandHandle[]; /* FckCommand句柄 */

/* 以下为OS相关配置 */
#ifdef WIN32
#endif

#ifndef WIN32 /* 非windows缺省定义一些宏以便通过编译 */
#endif

/* Define internal function calls.  */

/* 初始化循环缓冲区 */
INT PMI_InitializeCircleMemory( VOID )
{
    UNSIGNED OneDataSize;
    UNSIGNED TotalDataSize;

    /* 初始化事件循环缓冲区 */
    OneDataSize = ( sizeof( CEM_RAD )+ sizeof( UNSIGNED ) - 1 )/ sizeof( UNSIGNED );
    TotalDataSize = ( PLAT_cfg.history.TempEventNumber ) * OneDataSize;
    RamEventHandle = XJP_Queue_Create( TotalDataSize, XJP_EVENT_FIXED_SIZE, OneDataSize );
    NVRamEventHandle = XJP_Queue_Create( TotalDataSize, XJP_EVENT_FIXED_SIZE, OneDataSize );

    /* 初始化动作报告循环缓冲区 */
    OneDataSize = sizeof( CRM_RFD );
    TotalDataSize = ( PLAT_cfg.history.TempReportNumber ) * OneDataSize;
    RamReportHandle = XJP_Queue_Create( TotalDataSize, XJP_EVENT_VARIABLE_SIZE, OneDataSize );

    /* 初始化变位事件循环缓冲区 */
    TotalDataSize = ONE_COMM_ESIZE() * MAX_CHECK_INFO;
    CommEventHandle = XJP_Queue_Create( TotalDataSize, XJP_EVENT_FIXED_SIZE, ONE_COMM_ESIZE());
    TotalDataSize = ONE_COMM_RSIZE() * MAX_FAULT_RETURN;
    CommFltRetHandle = XJP_Queue_Create( TotalDataSize, XJP_EVENT_VARIABLE_SIZE, ONE_COMM_RSIZE());

    /* 初始化测控SOE循环缓冲区(一级数据) */
    OneDataSize = IED_FCK_SOE_BUFFER_LENGTH;
    TotalDataSize = IED_FCK_SOE_MAX_COUNT * OneDataSize;
    FckPrimarySOEHandle = XJP_Queue_Create( TotalDataSize, XJP_EVENT_VARIABLE_SIZE, OneDataSize );
    /* 初始化测控SOE循环缓冲区(二级数据) */
    FckSecondSOEHandle = XJP_Queue_Create( TotalDataSize, XJP_EVENT_VARIABLE_SIZE, OneDataSize );
    IEDDB_FckSoeHandle[0] = FckPrimarySOEHandle; /* 测控SOE循环缓冲区(一级数据) */
    IEDDB_FckSoeHandle[1] = FckSecondSOEHandle; /* 测控SOE循环缓冲区(二级数据) */

    /* 测控用户命令循环缓冲区(一级数据) */
    OneDataSize = IED_READ_COMMAND_BUFFER_LENGTH;
    TotalDataSize = IED_READ_COMMAND_MAX_COUNT * OneDataSize;
    FckPrimaryCommandHandle = XJP_Queue_Create( TotalDataSize, XJP_EVENT_VARIABLE_SIZE, OneDataSize );
    /* 测控用户命令循环缓冲区(二级数据) */
    FckSecondCommandHandle = XJP_Queue_Create( TotalDataSize, XJP_EVENT_VARIABLE_SIZE, OneDataSize );
    IEDDB_FckCommandHandle[0] = FckPrimaryCommandHandle; /* 测控用户命令循环缓冲区(一级数据) */
    IEDDB_FckCommandHandle[1] = FckSecondCommandHandle;  /* 测控用户命令循环缓冲区(二级数据) */
    /* 优先级表初始化函数 */
    iedDB_ProirityInit();
    /* 接收NPI用户数据管理句柄 */
    OneDataSize = CPC_MAX_LENGTH_FRAME;
    TotalDataSize = CPC_MAX_USER_FRAME_NUMBER * OneDataSize;
    NPIUserDataHandle = XJP_Queue_Create( TotalDataSize, XJP_EVENT_VARIABLE_SIZE, OneDataSize );

    return( XJ_SUCCESS );
}
