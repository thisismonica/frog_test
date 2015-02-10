
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
/*      pmi_led.c                                       Ver  1.0         */
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
#include "plat/es_plat.h"
#include "plat_specific/inc/pmi_extr.h"
#include "plat_specific/plat_specific.h"
#include "plat/inc/devi_extr.h"

/* Define external inner-component global data references.  */
/* 平台运行灯管理句柄 */
HLEM PlatFormRunLedHandle = 0;

/* 以下为OS相关配置 */
#ifdef WIN32
#endif

#ifndef WIN32 /* 非windows缺省定义一些宏以便通过编译 */
#endif

typedef struct PMI_LED_struct
{
    UNSIGNED  PlatAlarmLedBits;      /* 平台点告警灯事件位 */
    UNSIGNED  RunLedAbnorEventBits;  /* 平台运行灯异常事件位 */
}PMI_LED;

PMI_LED PmiLed =
{
    CPC_EVENT1_DEV_ERR,         /* 平台点告警灯事件位 */
    CPC_EVENT1_RUNLED_ABNORMAL  /* 平台运行灯异常事件位 */
};

/* Define internal function calls.  */

/* 初始化装置运行灯 */
INT PMI_InitializeDeviceLed( VOID )
{
    /* 创建平台运行灯管理句柄 */
    PlatFormRunLedHandle = XJP_Led_Create();

    return( XJ_SUCCESS );
}

/* 设置平台点告警灯事件位 */
INT  PMI_SetPlatAlarmLedBits( UNSIGNED uPlatAlarmBits )
{
    PmiLed.PlatAlarmLedBits = uPlatAlarmBits;
    return( XJ_SUCCESS );
}

/* 设置平台运行灯异常事件位函数 */
INT PMI_SetPlatRunLedAbnorBits( UNSIGNED uRunAbnorEventBits )
{
    PmiLed.RunLedAbnorEventBits = uRunAbnorEventBits;
    return( XJ_SUCCESS );
}

/* 设置告警灯与运行灯状态函数 */
INT PMI_SetAlarmANDRunLedStatus( VOID )
{
    UNSIGNED ActualEvent;
    UNSIGNED uEvent;
    /* 将运行灯异常事件位及告警事件位相或 */
    uEvent = ( PmiLed.PlatAlarmLedBits | PmiLed.RunLedAbnorEventBits );
    /* 装置故障点告警灯 */
    if( XJ_Retrieve_Events(& Event_1_CB, uEvent , XJ_OR,
        & ActualEvent, XJ_NO_SUSPEND ) == XJ_SUCCESS )
    {
        /* 设置装置运行灯异常状态 */
        if( ActualEvent & ( PmiLed.RunLedAbnorEventBits ) )
        {
            XJP_Led_Change_Status( PlatFormRunLedHandle, XJP_LED_ABNORMAL );
        }
        else
        {
            XJP_Led_Change_Status( PlatFormRunLedHandle, XJP_LED_NORMAL );
        }
        /* 设置告警灯状态 */
        if( ActualEvent & ( PmiLed.PlatAlarmLedBits ) )
        {
            DEVI_WriteAlarmLed(0x01); /* 告警灯亮 */
        }
        else
        {
            DEVI_WriteAlarmLed(0x00); /* 告警灯灭 */
        }
    }
    else
    {
        /* 设置装置运行灯状态 */
        XJP_Led_Change_Status( PlatFormRunLedHandle, XJP_LED_NORMAL );
        DEVI_WriteAlarmLed(0x00); /* 告警灯灭 */
    }
    return( XJ_SUCCESS );
}
