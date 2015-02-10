
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
/*      smvc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      SMV - Sample Management                                          */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common sample       */
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
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       11-15-2007      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "plat/inc/xj_cosc.h"
#include "plat/inc/pro_defs.h"
#include "plat/inc/csmv_extr.h"
#include "plat_specific/plat_specific.h"

#define CSMV_MAX_ANALOGNUMBER       (CPC_MAX_PHYSICAL_CHL_NUMBER+CPC_MAX_VIRTUAL_CHL_NUMBER)
/* 平台支持的最大模拟量通道个数 */

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
extern ESP_CFG PLAT_cfg;
extern SMVC_SAMPLE_RECORD SMVC_SampleRecord;
extern SMVC_ERR SMVC_Error;

float DC_Coefficient;
float FOURIER01_R[ CPC_MAX_SAMPLE_POINT ];
float FOURIER01_I[ CPC_MAX_SAMPLE_POINT ];
float FOURIER02_R[ CPC_MAX_SAMPLE_POINT ];
float FOURIER02_I[ CPC_MAX_SAMPLE_POINT ];
float FOURIER03_R[ CPC_MAX_SAMPLE_POINT ];
float FOURIER03_I[ CPC_MAX_SAMPLE_POINT ];
float FOURIER05_R[ CPC_MAX_SAMPLE_POINT ];
float FOURIER05_I[ CPC_MAX_SAMPLE_POINT ];

UNSIGNED SamplePulseMemory[ CPC_MAX_CHANNEL_POINT ]; /* 采样脉冲计数器缓冲区 */
float AnalogSampleMemory[( CSMV_MAX_ANALOGNUMBER * CPC_MAX_CHANNEL_POINT )]; /* 模拟量采样值缓冲区 */
UNSIGNED AnalogQualityMemory[(( CSMV_MAX_ANALOGNUMBER + 31 )/ 32 )* CPC_MAX_CHANNEL_POINT];
/* 模拟量品质因素缓冲区(按位存放) */
UNSIGNED InputSampleMemory[CPC_MAX_INPUT_GROUPS * CPC_MAX_CHANNEL_POINT]; /* 开入量缓冲区 */
UNSIGNED InputQualityMemory[CPC_MAX_INPUT_GROUPS * CPC_MAX_CHANNEL_POINT]; /* 开入量品质因素缓冲区 */
UNSIGNED OutputSampleMemory[CPC_MAX_OUTPUT_GROUPS * CPC_MAX_CHANNEL_POINT]; /* 开出量缓冲区 */
UNSIGNED OutputQualityMemory[CPC_MAX_OUTPUT_GROUPS * CPC_MAX_CHANNEL_POINT]; /* 开出量品质因素缓冲区 */
UNSIGNED ProStateSampleMemory[CPC_MAX_PROTECT_GROUPS * CPC_MAX_CHANNEL_POINT]; /* 保护状态量缓冲区 */
UNSIGNED ProStateQualityMemory[CPC_MAX_PROTECT_GROUPS * CPC_MAX_CHANNEL_POINT]; /* 保护状态量品质因素缓冲区 */
UNSIGNED LogStateSampleMemory[CPC_MAX_PROTECT_LOG_GROUPS * CPC_MAX_CHANNEL_POINT]; /* 逻辑状态量缓冲区 */
UNSIGNED LogStateQualityMemory[CPC_MAX_PROTECT_LOG_GROUPS * CPC_MAX_CHANNEL_POINT]; /* 逻辑状态量品质因素缓冲区 */

/* Define internal function calls.  */

/* 初始化模拟量参数 */
INT CSMV_InitializeChannelPara( VOID )
{
    INT k;
    INT PeriodPoint;
    float base_angle;

    PeriodPoint = CSMV_GET_PERIOD_POINT;
    /* 初始化傅氏计算系数 */
    base_angle = ( float )( 2.0 * CONSTANT_PI / PeriodPoint );
    /* 计算各次谐波的傅氏系数 */
    for( k = 0; k < PeriodPoint; k ++)
    {
        /* fourier 01 */
        FOURIER01_R[k] = ( float )( 2.0 * cos( base_angle * k )/ PeriodPoint );
        /* fourier 01 */
        FOURIER01_I[k] = ( float )( 2.0 * sin( base_angle * k )/ PeriodPoint );

        /* fourier 02 */
        FOURIER02_R[k] = ( float )( 2.0 * cos( 2.0 * base_angle * k )/ PeriodPoint );
        /* fourier 02 */
        FOURIER02_I[k] = ( float )( 2.0 * sin( 2.0 * base_angle * k )/ PeriodPoint );

        /* fourier 03 */
        FOURIER03_R[k] = ( float )( 2.0 * cos( 3.0 * base_angle * k )/ PeriodPoint );
        /* fourier 03 */
        FOURIER03_I[k] = ( float )( 2.0 * sin( 3.0 * base_angle * k )/ PeriodPoint );

        /* fourier 05 */
        FOURIER05_R[k] = ( float )( 2.0 * cos( 5.0 * base_angle * k )/ PeriodPoint );
        /* fourier 05 */
        FOURIER05_I[k] = ( float )( 2.0 * sin( 5.0 * base_angle * k )/ PeriodPoint );
    }

    /* 计算直流系数（直流算法用） */
    DC_Coefficient = ( float )( 1.0 / PeriodPoint );

    /* 初始化采样值记录信息 */
    SMVC_SampleRecord.sample_offset = 0; /* 最新采样位置偏移 */
    SMVC_SampleRecord.sample_counter = 0; /* 采样计数 */
    SMVC_SampleRecord.pSamplePulseMemory = SamplePulseMemory; /* 采样脉冲计数器缓冲区 */
    SMVC_SampleRecord.AdAnalogCounter = CSMV_GET_PHY_ANALOG_NUMBER; /* A/D模拟量通道数 */
    SMVC_SampleRecord.pAnalogMemory = AnalogSampleMemory; /* 模拟量数据内存指针 */
    SMVC_SampleRecord.AnalogQualityGroups = CSMV_GET_PHY_ANALOG_GROUPS; /* 模拟量品质因素组数 */
    SMVC_SampleRecord.pAanlogQualityMemory = AnalogQualityMemory; /* 模拟量品质因素内存指针 */
    SMVC_SampleRecord.InputGroupsNumber = CSMV_GET_INPUT_GROUPS; /* 开入量组数 */
    SMVC_SampleRecord.pInputMemory = InputSampleMemory; /* 开入量数据内存指针 */
    SMVC_SampleRecord.pInputQualityMemory = InputQualityMemory; /* 开入量品质因素内存指针 */
    SMVC_SampleRecord.OutputGroupsNumber = CSMV_GET_OUTPUT_GROUPS; /* 开出量组数 */
    SMVC_SampleRecord.pOutputMemory = OutputSampleMemory; /* 开出量数据内存指针 */
    SMVC_SampleRecord.pOutputQualityMemory = OutputQualityMemory; /* 开出量品质因素内存指针 */
    SMVC_SampleRecord.ProStatusGroupsNumber = CPC_MAX_PROTECT_GROUPS; /* 保护状态量组数 */
    SMVC_SampleRecord.pProStatusMemory = ProStateSampleMemory; /* 保护状态量数据内存指针 */
    SMVC_SampleRecord.pProStatusQualityMemory = ProStateQualityMemory; /* 保护状态量品质因素指针 */
    SMVC_SampleRecord.LogicGroupsNumber = CPC_MAX_PROTECT_LOG_GROUPS; /* 逻辑状态量组数 */
    SMVC_SampleRecord.pLogicMemory = LogStateSampleMemory; /* 逻辑状态量内存指针 */
    SMVC_SampleRecord.pLogicQualityMemory = LogStateQualityMemory; /* 逻辑状态量品质因素内存指针 */
    /* 初始化采样错误信息 */
    memset(& SMVC_Error, 0, sizeof( SMVC_ERR ));

    return( XJ_SUCCESS );
}

/* 获取采样基本信息 */
VOID * CSMV_SampleInformation( UNSIGNED * Actual_Number, UNSIGNED * Actual_Size,
    UNSIGNED * Actual_Point )
{
    VOID * pSampleDataAddr;

    * Actual_Number = ( UNSIGNED )CSMV_GET_PHY_ANALOG_NUMBER;
    * Actual_Size = ( UNSIGNED )CSMV_GET_CHANNEL_POINT ;
    * Actual_Point = ( UNSIGNED )CSMV_GET_PERIOD_POINT;

    pSampleDataAddr = ( VOID *)CSMV_GET_ANALOG_MEMORY;

    return( pSampleDataAddr );
}

/* 获取当前采样指针 */
VOID * CSMV_SamplePointer( VOID )
{
    float * SamplePointer;

    SamplePointer = CSMV_GET_ANALOG_MEMORY + CSMV_GET_SAMPLE_OFFSET;

    return( ( VOID *)SamplePointer );
}

/* 获取其它采样数据起始指针 */
INT CSMV_GetDataStartPointer( INT  iType, VOID ** valueMem, VOID ** qualityMemr, INT * number )
{
    INT status;
    ESP_CFG_INPUT_PORT * pPort;
    /*======================================*/
    status = XJ_UNAVAILABLE;
    switch( iType )
    {
        /* 模拟量通道品质因素 */
    case PRO_SAMPLE_ANALONG:
        if( valueMem )
            * valueMem = CSMV_GET_ANALOG_MEMORY;
        if( qualityMemr )
            * qualityMemr = CSMV_GET_ANALOG_QUA_MEMORY;
        if( number )
            * number = CSMV_GET_PHY_ANALOG_NUMBER;
        status = XJ_SUCCESS;
        break;
        /* 开入状态量 */
    case PRO_SAMPLE_INPUT:
        if( valueMem )
            * valueMem = CSMV_GET_INPUT_MEMORY;
        if( qualityMemr )
            * qualityMemr = CSMV_GET_INPUT_QUA_MEMORY;
        if( number )
            * number = CSMV_GET_INPUT_GROUPS;
        status = XJ_SUCCESS;
        break;
        /* 开出状态量 */
    case PRO_SAMPLE_OUTPUT:
        if( valueMem )
            * valueMem = CSMV_GET_OUTPUT_MEMORY;
        if( qualityMemr )
            * qualityMemr = CSMV_GET_OUTPUT_QUA_MEMORY;
        if( number )
            * number = CSMV_GET_OUTPUT_GROUPS;
        status = XJ_SUCCESS;
        break;
        /* 保护状态量 */
    case PRO_SAMPLE_PRO_STS:
        if( valueMem )
            * valueMem = CSMV_GET_PRO_STS_MEMORY;
        if( qualityMemr )
            * qualityMemr = CSMV_GET_PRO_STS_QUA_MEMORY;
        if( number )
            * number = CSMV_GET_PRO_STS_GROUPS;
        status = XJ_SUCCESS;
        break;
        /* 逻辑状态量 */
    case PRO_SAMPLE_PRO_LOG:
        if( valueMem )
            * valueMem = CSMV_GET_PRO_LOG_MEMORY;
        if( qualityMemr )
            * qualityMemr = CSMV_GET_PRO_LOG_QUA_MEMORY;
        if( number )
            * number = CSMV_GET_PRO_LOG_GROUPS;
        status = XJ_SUCCESS;
        break;
        /* 采样脉冲计数指针 */
    case PRO_SAMPLE_PULSE:
        if( valueMem )
            * valueMem = CSMV_GET_SAMPLE_PULSE_MEMORY;
        if( number )
            * number = 1;
        status = XJ_SUCCESS;
        break;
        /* 软开入品质因素信息指针(实时数据) */
    case PRO_SOFT_INPUT_QUA_INFO:
        pPort = CINC_INPUT_PORT_MEMORY; /* 开入端口首地址 */
        pPort += CINC_HARD_INPUT_GROUPS;
        if( valueMem )
            * valueMem = pPort->QualityAddress;
        if( qualityMemr )
            * qualityMemr = pPort->QualityCheckAddr;
        if( number )
            * number = CSMV_GET_INPUT_GROUPS;
        status = XJ_SUCCESS;
        break;
        /* 模拟量通道品质因素检修位(实时数据) */
    case PRO_ANALONG_CHECK_QUALITY:
        if( valueMem )
            * valueMem = CSMV_GET_ORIGINL_ANA_CHECKQUA_MEM;
        if( number )
            * number = CSMV_GET_PHY_ANALOG_GROUPS;
        status = XJ_SUCCESS;
        break;
    default:
        /* 不是支持的类型直接返回失败 */
        break;
    }
    /* 返回值 */
    return( status );
}

/* 获取采样位置偏移 */
UNSIGNED CSMV_GetSampleOffset( VOID )
{
    return( CSMV_GET_SAMPLE_OFFSET );
}
