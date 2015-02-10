
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
/*      cerc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CER - Common Error Management                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common error        */
/*      management component.                                            */
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
/*      cem_extr.h                          Protect Control functions    */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       08-13-2008      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "plat/inc/cer_extr.h"
#include "plat/inc/csmv_extr.h"
#include "plat/inc/xj_out_check.h"
#include "plat/inc/ctb_extr.h"
#include "plat_specific/plat_specific.h"

/* 以下为OS相关配置 */
#ifdef WIN32
/* 全局变量申明 */
/* Windows 宏定义 */
#endif

#ifndef WIN32 /* 非windows缺省定义一些宏以便通过编译 */
/* 全局变量申明 */
/* Nucleus宏定义 */
#endif

/* Define external inner-component global data references.  */
extern NU_EVENT_GROUP Event_1_CB;
extern volatile UNSIGNED TMD_System_Clock;
extern INT SMVC_InitializeFlag; /* 采样数据初始化完成标志 */
extern UNSIGNED SMVC_Power_Error_Counter; /* 电源自检出错计数器（上升沿计数器） */
extern volatile UNSIGNED SMVC_Power_Error_Resume_Counter;  /* 智能装置电源恢复计数器 */
extern UNSIGNED SMVC_Power_Error_Flag;    /* 电源自检出错标志 */
extern UNSIGNED SMVC_SampleCount; /* 采样计数器 */
extern CTABLE CTable;
/* Define internal function calls.  */

UNSIGNED CER_AD_ErrTime = CER_AD_ERR_TIME;        /* A/D error delay time */
UNSIGNED CER_AD_ResumeTime = CER_AD_RESUME_TIME;  /* A/D error resume time */

/* 读取HMI错误信息 */
UNSIGNED CERC_ReadHMIErrorBits( VOID )
{
    return( 0 );
}

/* 读取错误信息 */
INT CERC_ReadErrorBits( UNSIGNED * Actual_ErrorBits )
{
#ifndef WIN32
    CHAR group_name[8];  /* buffer size */
    UNSIGNED event_flags;
    UNSIGNED tasks_suspended;
    NU_TASK * first_task;

    NU_Event_Group_Information(& Event_1_CB, group_name,& event_flags,
        & tasks_suspended,& first_task );
    * Actual_ErrorBits = event_flags;
    return( XJ_SUCCESS );
#else
    * Actual_ErrorBits = ( 1 << 13 );
    return( XJ_SUCCESS );
#endif
}

/* 采样数据初始化完成检测 */
VOID CERC_SampleIniCheckMain( VOID )
{
    int chl_number;
    static INT SampleIniFlag = 0;

    chl_number = CSMV_GET_PHY_ANALOG_NUMBER;
    if( chl_number != 0 )
    {
        if( SampleIniFlag != SMVC_InitializeFlag )
        {
            /* 检查采样初始化完成 */
            if( SMVC_InitializeFlag )
            {
                /* 设置采样完成标志 */
                NU_Set_Events(& Event_1_CB, CPC_EVENT1_SAMPLE_OK, XJ_OR );
                SampleIniFlag = SMVC_InitializeFlag;
            }
            else
            {
                /* 清除采样完成标志 */
                NU_Set_Events(& Event_1_CB,( UNSIGNED )(~ CPC_EVENT1_SAMPLE_OK ), XJ_AND );
                SampleIniFlag = SMVC_InitializeFlag;
            }
        }
    }
    else
    {
        if( SampleIniFlag == XJ_FALSE )
        {
            /* 设置采样完成标志 */
            NU_Set_Events(& Event_1_CB, CPC_EVENT1_SAMPLE_OK, XJ_OR );
            SampleIniFlag = XJ_TRUE;
        }
    }
}

/* A/D检测主函数 */
VOID CERC_AD_CheckMain( VOID )
{
    int chl_number;
    static INT IniFlag = XJ_FALSE;
    static INT ErrFlag = XJ_FALSE;
    static UNSIGNED SampleCount;
    static UNSIGNED CheckDelay;
    static UNSIGNED CheckLoseDelay;
    static UNSIGNED ResumeDelay;
    static INT iInitFlag = NU_FALSE;
    float  fTemp;

    if( iInitFlag == NU_FALSE )
    {
        if( CSMV_GET_SAMPLE_RATE > 1 )
        {
            /* 采样间隔(单位毫秒) */
            fTemp = ( float )1000.0 / CSMV_GET_SAMPLE_RATE;
            fTemp *= CER_AD_ERR_COUNT;
            /* 校正系数 */
            fTemp += 1;
            /* 判断数值是否大于最小值 */
            if(( UNSIGNED )fTemp > CER_AD_ErrTime )
            {
                CER_AD_ErrTime = ( UNSIGNED )fTemp;
            }
            /* 计算返回时间 */
            fTemp *= CER_AD_ERR_RESUME_COEF;
            /* 判断数值是否大于最小值 */
            if(( UNSIGNED )fTemp > CER_AD_ResumeTime )
            {
                CER_AD_ResumeTime = ( UNSIGNED )fTemp;
            }

            iInitFlag = NU_TRUE;
        }
    }
    chl_number = CSMV_GET_PHY_ANALOG_NUMBER;
    /* 判断采样准备是否完成,如果完成才进行A/D自检 */
    if(( chl_number != 0 ) && ( CSMV_GetSampleInitFlag() == NU_SUCCESS ))
    {
        /* 仅有模拟量采集通道时，才进行A/D自检 */
        if( IniFlag == XJ_FALSE )
        {
            SampleCount = SMVC_SampleCount;
            CheckDelay = TMD_System_Clock;
            ResumeDelay = TMD_System_Clock;
            CheckLoseDelay = TMD_System_Clock;
            ErrFlag = XJ_FALSE;
            IniFlag = XJ_TRUE;
            return;
        }

        /* 判断检查时间是否到,如果没到直接返回 */
        if(( TMD_System_Clock - CheckLoseDelay ) < CER_AD_ErrTime )
        {
            return;
        }
        /* 更新检查时间 */
        CheckLoseDelay = TMD_System_Clock;
        if( SampleCount != SMVC_SampleCount )
        {
            /* 采样数据变化 */
            SampleCount = SMVC_SampleCount;
            CheckDelay = TMD_System_Clock;
            if( ErrFlag == XJ_TRUE )
            {
                /* A/D错误恢复检查 */
                if(( TMD_System_Clock - ResumeDelay ) >= CER_AD_ResumeTime )
                {
                    NU_Set_Events(& Event_1_CB,( UNSIGNED )(~ CPC_EVENT1_ERR_AD ), NU_AND );
                    ErrFlag = XJ_FALSE;
                }
            }
        }
        else
        {
            /* 采样数据无变化 */
            ResumeDelay = TMD_System_Clock;
            if( ErrFlag == XJ_FALSE )
            {
                if(( TMD_System_Clock - CheckDelay ) > CER_AD_ErrTime )
                {
                    /* 设置AD自检出错标志 */
                    NU_Set_Events(& Event_1_CB, CPC_EVENT1_ERR_AD, XJ_OR );
                    ErrFlag = XJ_TRUE;
                }
            }
        }
    }
}

/* 电源自检 */
void CERC_Power_Check( void )
{
    static UNSIGNED PowerErrorCounter = 0;
    static UNSIGNED uErrorDelay1 = 0, uErrorDelay2 = 0;
    static INT iIniFlag = 0;
    INT    iFlag;
    //--------------------------------------------------------------------------
    if( iIniFlag == 0 )
    {
        iIniFlag = 1;
        uErrorDelay1 = TMD_System_Clock;
        uErrorDelay2 = TMD_System_Clock;
    }

    /* 判断电源是否出错 */
    if(( SMVC_Power_Error_Flag == 1 )
        || ( PowerErrorCounter != SMVC_Power_Error_Counter ))
    {
        /* 置闭锁出口标志 */
        OUT_SetClockRelayFlag( 1 );
        NU_Set_Events(& Event_1_CB, CPC_EVENT1_CLOSE_RELAY, XJ_OR );
        /* 更新闭锁出口的延时时间 */
        uErrorDelay1 = TMD_System_Clock;
        /* 判断置电源自检出错标志的延时时间 */
        if(( TMD_System_Clock - uErrorDelay2 ) > CER_POWER_ERR_ALARME )
        {
            NU_Set_Events(& Event_1_CB, CPC_EVENT1_ERR_5VPOWER, XJ_OR );
        }
        /* 更新电源自检出错错误次数 */
        PowerErrorCounter = SMVC_Power_Error_Counter;
    }
    else
    {
        /* 更新电源自检出错标志的延时时间 */
        uErrorDelay2 = TMD_System_Clock;
        /* 判断闭锁出口标志 */
        OUT_GetClockRelayFlag(& iFlag );
        if( iFlag == 1 )
        {
            /* 判断闭锁出口的延时时间 */
            if(( TMD_System_Clock - uErrorDelay1 ) > CER_POWER_ERR_RESUME )
            {
                /* 清闭锁出口标志 */
                OUT_SetClockRelayFlag( 0 );
                NU_Set_Events(& Event_1_CB, ( UNSIGNED )(~ CPC_EVENT1_CLOSE_RELAY ), XJ_AND );
            }
        }
        else
        {
            /* 更新闭锁出口的延时时间 */
            uErrorDelay1 = TMD_System_Clock;
        }
        /* 智能装置电源自检要有恢复机制 */
        if( ( SMVC_Power_Error_Resume_Counter == 0 ) && ( PLAT_cfg.device.FunSelect & FUN_IED_DEVICE ))
        {
            /* 电源自检出错恢复 */
            NU_Set_Events(& Event_1_CB,( UNSIGNED )(~ CPC_EVENT1_ERR_5VPOWER ), NU_AND );
        }
    }
}

/* 设置用户告警位 */
VOID CERC_SetUserEventBits( UNSIGNED uEventBits, INT iFlag )
{
    UNSIGNED uActBits;

    /* 剔除平台事件位,平台事件位状态不受应用程序的控制 */
    uActBits = ( uEventBits & (~ CPC_EVENT1_PLAT_BITS ));

    if( iFlag != 0 )
    {
        /* 设置告警事件位，允许设置用户事件外的其它事件位 */
        NU_Set_Events(& Event_1_CB, uEventBits, XJ_OR );
    }
    else
    {
        /* 清除告警事件位，只允许清除用户事件位 */
        NU_Set_Events(& Event_1_CB,( UNSIGNED )(~ uActBits ), XJ_AND );
    }
}

/* RAM区程序文件自检主函数 */
VOID CERC_FileCheck_Main( VOID )
{
    static INT Flag = XJ_FALSE;

    if( CERC_FileCheck() != XJ_NULL )
    {
        if( Flag == XJ_FALSE )
        {
            /* RAM程序校验出错 */
            NU_Set_Events(& Event_1_CB, CPC_EVENT1_ERR_PROGRAM, XJ_OR );
            CEMC_FillEvent( "程序校验出错", ALARM_EVENT_ID );
            Flag = XJ_TRUE;
        }
    }
}

/* 整定值数据自检 */
VOID CERC_SettingCheck_Main( VOID )
{
    INT status;

    /* 修复整定值数据 */
    status = XJP_NVData_Repair( ( HNDM )CTable.NVDataId, CPC_DATA_TIMEOUT );
    if( status == XJ_END_OF_LOG )
    {
        /* 定值自检出错 */
        NU_Set_Events(& Event_1_CB, CPC_EVENT1_ERR_SETTING, XJ_OR );
    }
}
