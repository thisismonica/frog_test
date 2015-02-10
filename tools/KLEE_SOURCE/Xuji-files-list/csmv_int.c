
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
/*      SMV - Common Device Driver                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common device       */
/*      driver management component.                                     */
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
#include <stdio.h>
#include <math.h>
#include "plat/inc/xj_cosc.h"
#include "plat/es_plat.h"
#include "plat/inc/pro_defs.h"
#include "plat/inc/csmv_extr.h"
#include "plat_specific/plat_specific.h"
#include "plat/inc/esp_cfg.h"
#include "plat/inc/xj_out_check.h"

#define CVMC_MAX_ANALOG_VALUE_NUMBER    CPC_MAX_PHYSICAL_CHL_NUMBER /* 最大的模拟量通道个数 */
#define CVMC_MAX_ANALOG_QUALITY_GROUPS  ((CPC_MAX_PHYSICAL_CHL_NUMBER+31)/32) /* 最大的模拟量品质因素数目 */
#define CVMC_POWER_ERROR_NUMBER         2  /* 电源自检出错点数允许最大值 */
#define CVMC_SAMPLE_INITIALIZE_POINT    960 /* 采样初始化点数 */
#define CSMV_GET_MAX_PROTECT_GROUPS     (CPC_MAX_PROTECT_GROUPS) /* 最大保护状态量组数 */
#define CSMV_GET_MAX_PROTECT_LOG_GROUPS (CPC_MAX_PROTECT_LOG_GROUPS) /* 最大保护逻辑状态量组数 */
#define CVMC_MAX_SAMPLE_COUNTER         1200 /* 最大的采样序号 */

/* 以下为OS相关配置 */
#ifdef WIN32
/* 全局变量申明 */
/* Windows 宏定义 */
#endif

#ifndef WIN32 /* 非windows缺省定义一些宏以便通过编译 */
/* 全局变量申明 */
/* Nucleus宏定义 */
#endif

extern ESP_CFG PLAT_cfg;
extern CWM_WCB * WavePointer;
extern CWM_WCB * WavePointerCB[];

SMVC_SAMPLE_RECORD SMVC_SampleRecord;
INT SMVC_InitializeFlag; /* 采样数据初始化完成标志 */
INT SMVC_SampleInitFlag = NU_FALSE; /* 采样准备完成标识 */
UNSIGNED SMVC_SampleCount; /* 采样计数器 */
UNSIGNED SMVC_Power_Error_Counter = 0; /* 电源自检出错计数器（上升沿计数器） */
UNSIGNED SMVC_Power_Error_Flag = 0; /* 电源自检出错标志 */
volatile UNSIGNED SMVC_Power_Error_Resume_Counter = 0; /* 电源自检出错恢复计数器 */
static ESP_CFG_SMVC * pSMVC_AnalogConfig = &( PLAT_cfg.smvc ); /* 模拟量配置 */
static ESP_CFG_INPUT * pSMVC_InputConfig = &( PLAT_cfg.input ); /* 开入量配置 */
static ESP_CFG_OUTPUT * pSMVC_OutputConfig = &( PLAT_cfg.output ); /* 开出量配置 */
float SMVC_AnalogValueBuf[ CVMC_MAX_ANALOG_VALUE_NUMBER ]; /* 原始模拟量采样值 */
UNSIGNED SMVC_AnalogQualityBuf[CVMC_MAX_ANALOG_QUALITY_GROUPS]; /* 模拟量采样值的品质因素 */
UNSIGNED SMVC_AnalogCheckQualityBuf[CVMC_MAX_ANALOG_QUALITY_GROUPS]; /* 模拟量采样值的品质因素检修位 */
VOID (* SMVC_User_AD_Interrupt )( VOID ) = NU_NULL; /* 用户A/D中断处理函数 */
INT (* SMVC_User_Sample_Conversion )( float *, INT ) = NU_NULL; /* 用户采样转换函数 */

SMVC_ERR SMVC_Error;

/* Define internal function calls.  */
VOID SMVC_SampleSave( float * sample_data, UNSIGNED * pQualityBuf, UNSIGNED pulse_counter );
INT SMVC_LosePointQMan( INT iLoseCount, UNSIGNED StartLoseSerl ); /* 采样值丢失后品质因素处理函数 */
INT SMVC_LoseOneInsertValue( UNSIGNED pulse_counter ); /* 丢失一点插入前一点数据 */

/* 注册A/D中断用户处理函数 */
VOID SMVC_Register_User_AD_Interrupt( VOID (* User_AD_Interrupt )( VOID ))
{
    SMVC_User_AD_Interrupt = User_AD_Interrupt;
}

/* 注册A/D中断采样数据转换函数 */
VOID SMVC_Register_Sample_Conversion( INT (* User_Sample_Conversion )( float *, INT ))
{
    SMVC_User_Sample_Conversion = User_Sample_Conversion;
}

/* 更新硬件采样数据 */
VOID SMVC_SampleUpdateHard( float * sample_data, UNSIGNED pulse_counter )
{
    INT status;
    register INT k;
    register INT Number;
    register INT ChannelPoint;
    register float * pSrcAnalog;
    register float * pDstAnalog;
    PRO_CHANNEL        * pChannel;      /* 通道数据模型指针,有效值录波时使用 */
    ESP_CFG_SMVC_POWER * pPowerChlInfo; /* 自检电源通道信息指针 */
    float fValue;
    float fPowChlMinBasValue;
    float fPowChlMaxBasValue;
    int PowerCheckErrorFlag;            /* 电源自检标志 */
    static UNSIGNED PowerErrNumber = 0; /* 电源自检出错点数计数器 */
    float buffer[256];                  /* 临时缓冲区 */
    UNSIGNED uTotalSerial;

    if( SMVC_User_Sample_Conversion )
    {
        /* 用户采样数据转换函数 */
        Number = CSMV_GET_PHY_ANALOG_NUMBER;
        status = SMVC_User_Sample_Conversion( sample_data, Number );
        if( status == NU_FALSE )
        {
            return;
        }
    }

    /* 判断是否为双采样装置 */
    if( CSMV_GET_DSAMPLE_FLAG )
    {
        /* 模拟量通道零漂、系数调整 */
        pSrcAnalog = sample_data;
        Number = CSMV_GET_PHY_ANALOG_NUMBER - CSMV_GET_DSAMPLE_SV_CHL_NUM;
        pDstAnalog = buffer;
        pChannel = CSMV_GET_CHL_TABLE_MEMORY;
        for( k = 0; k < Number; k ++)
        {
            fValue = * pSrcAnalog;
            /* 零漂调整 */
            fValue -= ( pChannel->chl_channel_excursion );
            /* 系数调整 */
            fValue *= ( pChannel->chl_channel_coefficient );
            * pDstAnalog = fValue;
            pSrcAnalog ++;
            pChannel ++;
            pDstAnalog ++;
        }
        /* 存储SV数据信息 */
        pSrcAnalog = CSMV_MSG_GetSVDataInfo( & uTotalSerial );
        Number = CSMV_GET_DSAMPLE_SV_CHL_NUM;
        /* 软采样需要调整到高16位,低低16位为硬件采样序号 */
        uTotalSerial <<= CPC_DATA_BITS16;
        uTotalSerial |= pulse_counter;
        for( k = 0; k < Number; k ++)
        {
            fValue = * pSrcAnalog;
            /* 零漂调整 */
            fValue -= ( pChannel->chl_channel_excursion );
            /* 系数调整 */
            fValue *= ( pChannel->chl_channel_coefficient );
            * pDstAnalog = fValue;
            pSrcAnalog ++;
            pChannel ++;
            pDstAnalog ++;
        }
    }
    else
    {
        /* 模拟量通道零漂、系数调整 */
        pSrcAnalog = sample_data;
        Number = CSMV_GET_PHY_ANALOG_NUMBER;
        pDstAnalog = buffer;
        pChannel = CSMV_GET_CHL_TABLE_MEMORY;
        for( k = 0; k < Number; k ++)
        {
            fValue = * pSrcAnalog;
            /* 零漂调整 */
            fValue -= ( pChannel->chl_channel_excursion );
            /* 系数调整 */
            fValue *= ( pChannel->chl_channel_coefficient );
            * pDstAnalog = fValue;
            pSrcAnalog ++;
            pChannel ++;
            pDstAnalog ++;
        }
        uTotalSerial = pulse_counter;
    }

    /* 获取通道数据模型指针 */
    pChannel = CSMV_GET_CHL_TABLE_MEMORY;
    /* 自检电源通道信息指针 */
    pPowerChlInfo = CSMV_GET_POWER_CHL_INFO_MEMORY;
    /* 最新采样数据地址 */
    pSrcAnalog = buffer;
    /* 清电源自检出错标志 */
    PowerCheckErrorFlag = NU_FALSE;
    for( k = 0; k < CSMV_GET_POWER_CHECK_NUMBER; k ++)
    {
        /* 通道当前采样值 */
        fValue = *( pSrcAnalog + pPowerChlInfo->ChannelNumber );
        /* 通道额定值比率系数 */
        fValue *= (( pChannel + pPowerChlInfo->ChannelNumber )->chl_rate_coefficient );
        fValue = ( float )fabs( fValue );

        /* 通道检测基准值 */
        /* 下限值 */
        fPowChlMinBasValue = pPowerChlInfo->MinValue *
            (( pChannel + pPowerChlInfo->ChannelNumber )->chl_calculate_coefficient );
        /* 上限值 */
        fPowChlMaxBasValue = pPowerChlInfo->MaxValue *
            (( pChannel + pPowerChlInfo->ChannelNumber )->chl_calculate_coefficient );
        /* 判断是否在允许的范围之内 */
        if(( fValue < fPowChlMinBasValue ) || ( fValue > fPowChlMaxBasValue ))
        {
            /* 置电源自检出错标志 */
            PowerCheckErrorFlag = NU_TRUE;
            break;
        }
        /* 调整电源自检信息指针 */
        pPowerChlInfo ++;
    }
    /* 判断电源自检标志 */
    if( PowerCheckErrorFlag == NU_TRUE )
    {
        if( SMVC_Power_Error_Flag == NU_FALSE )
        {
            /* 调整电源自检出错点数计数器值 */
            PowerErrNumber ++;
            /* 判断电源自检出错点数是否已到允许的最大值 */
            if( PowerErrNumber >= CVMC_POWER_ERROR_NUMBER )
            {
                /* 调整电源自检出错计数器 */
                SMVC_Power_Error_Counter ++;
                SMVC_Power_Error_Flag = NU_TRUE; /* 错误标志 */
            }
        }
    }
    else
    {
        /* 清电源自检出错点数计数器 */
        PowerErrNumber = 0;
        SMVC_Power_Error_Flag = NU_FALSE;
    }

    if(( PowerCheckErrorFlag == NU_TRUE ) && ( SMVC_Power_Error_Flag == NU_FALSE ))
    {
        /* 电源一点出错，用先前采样点代替当前点 */
        pSrcAnalog = CSMV_GET_ANALOG_MEMORY + CSMV_GET_SAMPLE_OFFSET;
        pDstAnalog = buffer;
        Number = CSMV_GET_PHY_ANALOG_NUMBER;
        ChannelPoint = CSMV_GET_CHANNEL_POINT;
        for( k = 0; k < Number; k ++)
        {
            * pDstAnalog = * pSrcAnalog;
            pSrcAnalog += ChannelPoint;
            pDstAnalog ++;
        }
    }
    /* 保存采样数据 */
    SMVC_SampleSave( buffer, CSMV_GET_ORIGINL_ANA_QUA_MEM, uTotalSerial );
}

/* 更新数字量采样数据 */
VOID SMVC_SampleUpdateSoft( float * pSampleData, UNSIGNED pulse_counter )
{
    INT status;
    register INT k;
    register INT Number;
    register INT ChannelPoint;
    register float * pSrcAnalog;
    PRO_CHANNEL * pChannel;      /* 通道数据模型指针,有效值录波时使用 */
    float fValue;
    static UNSIGNED PowerErrNumber = 0; /* 电源自检出错点数计数器 */
    register UNSIGNED * pSrcValue;
    static INT iniFlag = XJ_FALSE;
    register UNSIGNED uVariable;
    register UNSIGNED uCounter;
    /*--------------------------------------------------*/

    if( SMVC_User_Sample_Conversion )
    {
        /* 用户采样数据转换函数 */
        Number = CSMV_GET_PHY_ANALOG_NUMBER;
        status = SMVC_User_Sample_Conversion( pSampleData, Number );
        if( status == NU_FALSE )
        {
            return;
        }
    }

    if( iniFlag == XJ_TRUE )
    {
        /* 采样序号丢点检查 */
        pSrcValue = CSMV_GET_SAMPLE_PULSE_MEMORY + CSMV_GET_SAMPLE_OFFSET;
        uVariable = * pSrcValue;
        if((++ uVariable ) >= ( UNSIGNED )CSMV_GET_SAMPLE_COUNTER )
        {
            uVariable -= CSMV_GET_SAMPLE_COUNTER;
        }

        if( pulse_counter == uVariable )
        {
            /* 采样序号连续 */
            if( SMVC_Power_Error_Resume_Counter )
            {
                SMVC_Power_Error_Resume_Counter --;
            }
            /* 清电源自检出错点数计数器 */
            PowerErrNumber = 0;
            SMVC_Power_Error_Flag = NU_FALSE;
        }
        else
        {
            SMVC_Error.sampleCounter ++;
            /* 采样序号不连续,判断丢点个数 */
            if( pulse_counter >= uVariable )
            {
                uCounter = pulse_counter - uVariable;
            }
            else
            {
                uCounter = pulse_counter + CSMV_GET_SAMPLE_COUNTER - uVariable;
            }
            uCounter &= CPC_DATA_MASKS16;
            if( uCounter == 1 )
            {
                /* 记录丢失一点的次数 */
                SMVC_Error.OneSampleCounter ++;
            }
            if( PowerErrNumber < CVMC_POWER_ERROR_NUMBER )
            {
                PowerErrNumber = uCounter;
                if( PowerErrNumber >= CVMC_POWER_ERROR_NUMBER )
                {
                    /* 置电源自检出错标识 */
                    SMVC_Power_Error_Flag = NU_TRUE; /* 错误标志 */
                    /* 电源自检出错置电源恢复计数器的值为初值 */
                    SMVC_Power_Error_Resume_Counter = CSMV_GET_CHANNEL_POINT;
                }
                else if( PowerErrNumber == 1 ) /* 丢失一点是否进行插值处理 */
                {
                    /* 丢失一点插入前一点数据 */
                    if( CSMV_IF_LOSE_ONE_SV_INSERT == XJ_TRUE )
                    {
                        SMVC_LoseOneInsertValue(  uVariable );
                    }
                }
            }
            else
            {
                /* 置电源自检出错标识 */
                SMVC_Power_Error_Flag = NU_TRUE; /* 错误标志 */
                /* 电源自检出错置电源恢复计数器的值为初值 */
                SMVC_Power_Error_Resume_Counter = CSMV_GET_CHANNEL_POINT;
            }

            /* 丢点后品质因素管理函数 */
            if( CSMV_IF_LOSE_SV_QUALITY == XJ_TRUE )
            {
                SMVC_LosePointQMan( uCounter, uVariable );
            }
        }
    }
    else
    {
        /* 初始第1个点不进行序号检查 */
        PowerErrNumber = 0;
        SMVC_Power_Error_Flag = NU_FALSE;
        iniFlag = XJ_TRUE;
    }

    /* 处理原始采样数据 */
    /* 更新实通道模拟量值,虚通道由应用设置 */
    pSrcAnalog = pSampleData;
    Number = CSMV_GET_PHY_ANALOG_NUMBER;
    pChannel = CSMV_GET_CHL_TABLE_MEMORY;
    ChannelPoint = CSMV_GET_CHANNEL_POINT;
    for( k = 0; k < Number; k ++)
    {
        fValue = * pSrcAnalog;

        /* 通道采样值系数 */
        fValue *= pChannel->chl_sample_coefficient;
        /* 零漂调整 */
        fValue -= ( pChannel->chl_channel_excursion );
        /* 系数调整 */
        fValue *= ( pChannel->chl_channel_coefficient );
        SMVC_AnalogValueBuf[k] = fValue;
        pSrcAnalog ++;
        pChannel ++;
    }

    /* 保存采样数据 */
    SMVC_SampleSave( SMVC_AnalogValueBuf, CSMV_GET_ORIGINL_ANA_QUA_MEM, pulse_counter );
}

/* 更新采样数据 */
VOID SMVC_SampleSave( float * sample_data, UNSIGNED * pQualityBuf, UNSIGNED pulse_counter )
{
    register INT k;
    register INT Number;
    register INT iSampleOffset;
    register UNSIGNED uVariable;
    register INT ChannelPoint;
    register float * pSrcAnalog;
    register float * pDstAnalog;
    register float fVariable;
    register SIGNED * pWaveAnalog;
    register UNSIGNED * pSrcValue;
    register UNSIGNED * pDstValue;
    register UNSIGNED uOffset;       /* 获取录波数据偏移量 */
    register UNSIGNED uWaveOffset;   /* 录波记录位置 */
    register UNSIGNED * pWaveMomery; /* 录波数据存储缓冲区 */
    register int      iRecordFlag;   /* 是否记录录波数据标志 */
    PRO_CHANNEL       * pChannel;    /* 通道数据模型指针,有效值录波时使用 */
    TIME * pTime;

    /* 调整采样位置 */
    iSampleOffset = CSMV_GET_SAMPLE_OFFSET + 1; /* 调整采样位置偏移 */
    if( iSampleOffset >= CSMV_GET_CHANNEL_POINT )
    {
        iSampleOffset = 0; /* 采样值循环存储 */
    }

    /* 更新实通道模拟量值,虚通道由应用设置 */
    pSrcAnalog = sample_data;
    Number = CSMV_GET_PHY_ANALOG_NUMBER;
    pDstAnalog = CSMV_GET_ANALOG_MEMORY + iSampleOffset;
    pChannel = CSMV_GET_CHL_TABLE_MEMORY;
    ChannelPoint = CSMV_GET_CHANNEL_POINT;
    for( k = 0; k < Number; k ++)
    {
        * pDstAnalog = * pSrcAnalog;
        pSrcAnalog ++;
        pChannel ++;
        pDstAnalog += ChannelPoint;
    }
    /* 更新采样脉冲计数器 */
    pDstValue = CSMV_GET_SAMPLE_PULSE_MEMORY + iSampleOffset;
    * pDstValue = pulse_counter;

    /* 更新模拟量通道品质因素 */
    Number = CSMV_GET_PHY_ANALOG_GROUPS;
    pSrcValue = pQualityBuf;
    pDstValue = CSMV_GET_ANALOG_QUA_MEMORY + iSampleOffset;
    for( k = 0; k < Number; k ++)
    {
        * pDstValue = * pSrcValue;
        pSrcValue ++;
        pDstValue += ChannelPoint;
    }

    /* 更新开入状态量 */
    Number = CSMV_GET_INPUT_GROUPS;
    pDstValue = CSMV_GET_INPUT_MEMORY + iSampleOffset;
    for( k = 0; k < Number; k ++)
    {
        /* 读取开入状态量 */
        uVariable = CINC_ReadInput( k );
        * pDstValue = uVariable;
        pDstValue += ChannelPoint;
    }

    /* 更新开出状态量值 */
    Number = CSMV_GET_OUTPUT_GROUPS;
    pDstValue = CSMV_GET_OUTPUT_MEMORY + iSampleOffset;
    for( k = 0; k < Number; k ++)
    {
        uVariable = OUT_GetOutPickupBits( k );
        * pDstValue = uVariable;
        pDstValue += ChannelPoint;
    }

    /* 更新保护状态量值 */
    Number = CSMV_GET_MAX_PROTECT_GROUPS;
    pSrcValue = CSMV_GET_ORIGINL_PRO_STS_MEM;
    pDstValue = CSMV_GET_PRO_STS_MEMORY + iSampleOffset;
    for( k = 0; k < Number; k ++)
    {
        * pDstValue = * pSrcValue;
        pSrcValue ++;
        pDstValue += ChannelPoint;
    }

    /* 更新逻辑状态量值 */
    Number = CSMV_GET_MAX_PROTECT_LOG_GROUPS;
    pSrcValue = CSMV_GET_ORIGINL_PRO_LOG_MEM;
    pDstValue = CSMV_GET_PRO_LOG_MEMORY + iSampleOffset;
    for( k = 0; k < Number; k ++)
    {
        * pDstValue = * pSrcValue;
        pSrcValue ++;
        pDstValue += ChannelPoint;
    }

    /* 更新采样指针位置 */
    CSMV_SET_SAMPLE_OFFSET( iSampleOffset );

    SMVC_SampleCount ++;
    if( SMVC_InitializeFlag == 0 )
    {
        if( SMVC_SampleCount >= CVMC_SAMPLE_INITIALIZE_POINT )
        {
            SMVC_InitializeFlag = 1;
        }
        return;
    }

    if( SMVC_User_AD_Interrupt )
    {
        /* 用户A/D中断处理函数 */
        SMVC_User_AD_Interrupt();
    }

    /* 清记录录波数据标志 */
    iRecordFlag = 0;

    if( WavePointer == NU_NULL )
    {
        /* 无触发录波数据记录，直接返回 */
        return;
    }

    /* 录波数据记录 */
    if( WavePointer->RecordNumber == 0 )
    {
        /* 录波数据的故障时间在触发录波时记录 */
        /* 置记录录波数据标志 */
        iRecordFlag = 1;
    }

    /* 判断是否允许记录录波数据(录波间隔是否到) */
    if(( WavePointer->SampleNumber ) >= ( WavePointer->SampleSpace ))
    {
        /* 置记录录波数据标志 */
        iRecordFlag = 1;
        /* 清录波间隔计数器 */
        WavePointer->SampleNumber = 0;
    }

    /* 判断是否记录录波数据 */
    if( iRecordFlag == 1 )
    {
        /* 根据故障前采样点数获取录波数据偏移位置 */
        /* 判断录波故障前点数与当前最新采样点数的偏移 */
        if(( WavePointer -> StartPointer )> ( UNSIGNED )CSMV_GET_SAMPLE_OFFSET )
        {
            uOffset = ( CSMV_GET_SAMPLE_OFFSET + CSMV_GET_CHANNEL_POINT ) - ( WavePointer -> StartPointer );
        }
        else
        {
            uOffset = CSMV_GET_SAMPLE_OFFSET - ( WavePointer -> StartPointer );
        }

        /* 录波记录位置 */
        uWaveOffset = WavePointer->RecordNumber;
        /* 录波数据存储缓冲区 */
        pWaveMomery = WavePointer->WaveMemory;
        /* 采样通道size */
        ChannelPoint = CSMV_GET_CHANNEL_POINT;

        if( WavePointer->AnalogNumber != 0 )
        {
            /* 模拟量通道数据模型起始地址 */
            pChannel = CSMV_GET_CHL_TABLE_MEMORY;
            /* 记录模拟量数据 */
            pWaveAnalog = ( SIGNED *)pWaveMomery;
            if(( WavePointer->Type ) & WAVE_TYPE_COEFFECT )
            {
                /* 有效值录波 */
                for( k = 0; k < (( INT )WavePointer -> AnalogNumber ); k ++)
                {
                    /* 记录模拟量录波数据 */
                    fVariable = (( pChannel +
                        (( WavePointer->AnalogInf )+ k )->Number )->chl_virtual_value );
                    fVariable *= ((( WavePointer->AnalogInf )+ k )->WaveCoefficient );
                    *( pWaveAnalog + uWaveOffset ) = ( SIGNED )fVariable;
                    /* 调整录波位置偏移量 */
                    uWaveOffset += WavePointer->TotalPointer;
                }
            }
            else
            {
                /* 模拟量采样数据 */
                pDstAnalog = CSMV_GET_ANALOG_MEMORY + uOffset;
                /* 瞬时值录波 */
                for( k = 0; k < (( INT )WavePointer -> AnalogNumber ); k ++)
                {
                    /* 记录模拟量录波数据 */
                    fVariable = (* ( pDstAnalog + (( WavePointer->AnalogInf )+ k )->Number * ChannelPoint ));
                    fVariable *= ((( WavePointer->AnalogInf )+ k )->WaveCoefficient );
                    *( pWaveAnalog + uWaveOffset ) = ( SIGNED )fVariable;
                    /* 调整录波位置偏移量 */
                    uWaveOffset += WavePointer->TotalPointer;
                }
            }
        }

        if( WavePointer->InputNumber != 0 )
        {
            /* 记录开入量数据 */
            pDstValue = CSMV_GET_INPUT_MEMORY + uOffset;
            for( k = 0; k < (( INT )WavePointer -> InputNumber ); k ++)
            {
                /* 记录开入量数据 */
                *( pWaveMomery + uWaveOffset ) = * ( pDstValue +
                    (( WavePointer->InputInf )+ k )->GroupNumber * ChannelPoint );
                /* 调整录波位置偏移量 */
                uWaveOffset += WavePointer->TotalPointer;
            }
        }

        if( WavePointer->OutputNumber != 0 )
        {
            /* 记录开出量数据 */
            pDstValue = CSMV_GET_OUTPUT_MEMORY + uOffset;
            for( k = 0; k < (( INT )WavePointer -> OutputNumber ); k ++)
            {
                /* 记录开出量数据 */
                *( pWaveMomery + uWaveOffset ) = * ( pDstValue +
                    (( WavePointer->OutputInf )+ k )->GroupNumber * ChannelPoint );
                /* 调整录波位置偏移量 */
                uWaveOffset += WavePointer->TotalPointer;
            }
        }

        if( WavePointer->StateNumber != 0 )
        {
            /* 记录保护状态量数据 */
            pDstValue = CSMV_GET_PRO_STS_MEMORY + uOffset;
            for( k = 0; k < (( INT )WavePointer -> StateNumber ); k ++)
            {
                /* 记录保护状态量数据 */
                *( pWaveMomery + uWaveOffset ) = * ( pDstValue +
                    (( WavePointer->StateInf )+ k )->GroupNumber * ChannelPoint );
                /* 调整录波位置偏移量 */
                uWaveOffset += WavePointer->TotalPointer;
            }
        }

        if( WavePointer->LogicNumber != 0 )
        {
            /* 记录逻辑状态量数据 */
            pDstValue = CSMV_GET_PRO_LOG_MEMORY + uOffset;
            for( k = 0; k < (( INT )WavePointer -> LogicNumber ); k ++)
            {
                /* 记录逻辑状态量数据 */
                *( pWaveMomery + uWaveOffset ) = * ( pDstValue +
                    (( WavePointer->LogicalInf )+ k )->GroupNumber * ChannelPoint );
                /* 调整录波位置偏移量 */
                uWaveOffset += WavePointer->TotalPointer;
            }
        }
        /* 调整录波记录的点数 */
        WavePointer->RecordNumber += 1;
        /* 判断录波是否结束 */
        if(( WavePointer->RecordNumber ) >= ( WavePointer->TotalPointer ))
        {
            /* 置录波完成标志 */
            WavePointer->EndFlag = 1;
            /* 记录下一段录波数据 */
            for( k = 0; k < ( CWM_MAX_WAVE_POINTER - 1 ); k ++ )
            {
                WavePointerCB[ k ] = WavePointerCB[( k + 1 )];
            }
            WavePointerCB[( CWM_MAX_WAVE_POINTER - 1 )] = XJ_NULL;
            /* 设置录波数据指针 */
            WavePointer = WavePointerCB[0];
            if( WavePointer )
            {
                pTime = XJP_TM_Get_Time_Ptr();
                WavePointer -> Year = pTime -> Year;
                WavePointer -> Month = pTime -> Month;
                WavePointer -> Day = pTime -> Day;
                WavePointer -> Hour = pTime -> Hour;
                WavePointer -> Minute = pTime -> Minute;
                WavePointer -> Millionsecond = pTime -> Millionsecond;
                WavePointer -> Nanosecond = pTime -> Nanosecond;
            }
            /* 录波结束,直接返回 */
            return;
        }
    }

    /* 调整录波间隔计数器 */
    WavePointer->SampleNumber += 1;
}

/* 获取采样次数计数器 */
INT SMVC_GetSampleCount( UNSIGNED * pSampleCount )
{
    * pSampleCount = SMVC_SampleCount;
    return( NU_SUCCESS );
}

/* 丢失一点插入前一点数据 */
static INT SMVC_LoseOneInsertValue( UNSIGNED pulse_counter )
{
    register INT k;
    register INT Number;
    register INT iVariable;
    register INT ChannelPoint;
    register float * pSrcAnalog;
    register UNSIGNED * pDstValue;
    register UNSIGNED * pSrcValue;
    UNSIGNED uQualityBuf[CVMC_MAX_ANALOG_QUALITY_GROUPS];

    /* 电源一点出错，用先前采样点代替当前点 */
    iVariable = CSMV_GET_SAMPLE_OFFSET;
    pSrcAnalog = CSMV_GET_ANALOG_MEMORY + iVariable;
    /* 通道个数 */
    Number = CSMV_GET_PHY_ANALOG_NUMBER;
    /* 每个通道大小 */
    ChannelPoint = CSMV_GET_CHANNEL_POINT;
    for( k = 0; k < Number; k ++)
    {
        SMVC_AnalogValueBuf[k] = * pSrcAnalog;
        pSrcAnalog += ChannelPoint;
    }

    /* 获取前一点的品质因素 */
    Number = CSMV_GET_PHY_ANALOG_GROUPS;
    pDstValue = ( register UNSIGNED *)& uQualityBuf[0];
    pSrcValue = CSMV_GET_ANALOG_QUA_MEMORY + iVariable;
    for( k = 0; k < Number; k ++)
    {
        * pDstValue = * pSrcValue;
        pDstValue ++;
        pSrcValue += ChannelPoint;
    }

    SMVC_SampleSave( ( float *)& SMVC_AnalogValueBuf[0], uQualityBuf, pulse_counter );
    return( NU_SUCCESS );
}

/* 采样值丢失后品质因素处理函数 */
static INT SMVC_LosePointQMan( INT iLoseCount, UNSIGNED StartLoseSerl )
{
    register INT k;
    register INT Number;
    register INT iVariable;
    register INT ChannelPoint;
    register UNSIGNED * pDstValue;
    register INT  uSerial;
    INT  iStartGroup;
    INT  iGroupCount;
    /*--------------------------------------------*/
    /* 获取模拟量通道品质因素录波状态量的起始组号及组数 */
    CSMV_MSG_GetChlQStateInfo(& iStartGroup, & iGroupCount );
    /* 调整采样位置 */
    iVariable = CSMV_GET_SAMPLE_OFFSET + 1; /* 调整采样位置偏移 */
    if( iVariable >= CSMV_GET_CHANNEL_POINT )
    {
        iVariable = 0; /* 采样值循环存储 */
    }
    CSMV_SET_SAMPLE_OFFSET( iVariable );

    /* 获取前一点的品质因素 */
    Number = CSMV_GET_PHY_ANALOG_GROUPS;
    /* 每个通道大小 */
    ChannelPoint = CSMV_GET_CHANNEL_POINT;
    /* 起始丢点序号 */
    uSerial = StartLoseSerl;
    /* 判断丢点个数是否已经达到最大值,如果达到最大值则将整个缓冲区的品质因素都设置为无效 */
    if( iLoseCount >= CSMV_GET_CHANNEL_POINT )
    {
        /* 模拟量通道品质因素起始位置 */
        pDstValue = CSMV_GET_ANALOG_QUA_MEMORY;
        /* 品质因素的整个缓冲区都设置为无效 */
        memset( pDstValue, 0,( sizeof( UNSIGNED )* CSMV_GET_CHANNEL_POINT * Number ));
        /* 存储通道品质因素的状态量 */
        pDstValue = CSMV_GET_PRO_STS_MEMORY + ChannelPoint * iStartGroup;
        /* 整个缓冲区设置为无效 */
        memset( pDstValue, 0,( sizeof( UNSIGNED )* CSMV_GET_CHANNEL_POINT * iGroupCount ));
        /* 将采样偏移位置设置为初值 */
        iVariable = 0;
    }
    else
    {
        pDstValue = CSMV_GET_ANALOG_QUA_MEMORY + CSMV_GET_SAMPLE_OFFSET;
        /* 判断存储位置 */
        if(( iLoseCount + CSMV_GET_SAMPLE_OFFSET ) <= CSMV_GET_CHANNEL_POINT )
        {
            /* 丢点个数 */
            for( k = 0; k < Number; k ++)
            {
                /* 设置品质因素为无效状态 */
                memset( pDstValue, 0,( sizeof( UNSIGNED )* iLoseCount ));
                pDstValue += ChannelPoint;
            }

            /* 存储通道品质因素的状态量 */
            pDstValue = CSMV_GET_PRO_STS_MEMORY + ChannelPoint * iStartGroup + CSMV_GET_SAMPLE_OFFSET;
            /* 丢点个数 */
            for( k = 0; k < iGroupCount; k ++)
            {
                /* 设置品质因素为无效状态 */
                memset( pDstValue, 0,( sizeof( UNSIGNED )* iLoseCount ));
                pDstValue += ChannelPoint;
            }

            /* 采样脉冲计数器 */
            pDstValue = CSMV_GET_SAMPLE_PULSE_MEMORY + CSMV_GET_SAMPLE_OFFSET;
            /* 采样序号的调整 */
            for( k = 0; k < iLoseCount; k ++)
            {
                * pDstValue ++ = ( register UNSIGNED )uSerial;
                if( ++ uSerial >= CSMV_GET_SAMPLE_COUNTER )
                {
                    uSerial = 0;
                }
            }

            /* 采样偏移位置调整 */
            iVariable = iLoseCount + CSMV_GET_SAMPLE_OFFSET;
        }
        else
        {
            iVariable = CSMV_GET_CHANNEL_POINT - CSMV_GET_SAMPLE_OFFSET;
            for( k = 0; k < Number; k ++)
            {
                /* 设置品质因素为无效状态 */
                memset( pDstValue, 0,( sizeof( UNSIGNED )* iVariable ));
                pDstValue += ChannelPoint;
            }
            /* 存储通道品质因素的状态量 */
            pDstValue = CSMV_GET_PRO_STS_MEMORY + ChannelPoint * iStartGroup + CSMV_GET_SAMPLE_OFFSET;
            /* 丢点个数 */
            for( k = 0; k < iGroupCount; k ++)
            {
                /* 设置品质因素为无效状态 */
                memset( pDstValue, 0,( sizeof( UNSIGNED )* iVariable ));
                pDstValue += ChannelPoint;
            }

            /* 采样脉冲计数器 */
            pDstValue = CSMV_GET_SAMPLE_PULSE_MEMORY + CSMV_GET_SAMPLE_OFFSET;
            /* 采样序号的调整 */
            for( k = 0; k < iVariable; k ++)
            {
                * pDstValue ++ = uSerial;
                if( ++ uSerial >= CSMV_GET_SAMPLE_COUNTER )
                {
                    uSerial = 0;
                }
            }
            /* 品质因素起始位置 */
            pDstValue = CSMV_GET_ANALOG_QUA_MEMORY;
            iVariable = CSMV_GET_SAMPLE_OFFSET + iLoseCount - CSMV_GET_CHANNEL_POINT;
            for( k = 0; k < Number; k ++)
            {
                /* 设置品质因素为无效状态 */
                memset( pDstValue, 0,( sizeof( UNSIGNED )* iVariable ));
                pDstValue += ChannelPoint;
            }

            /* 存储通道品质因素的状态量 */
            pDstValue = CSMV_GET_PRO_STS_MEMORY + ChannelPoint * iStartGroup;
            /* 丢点个数 */
            for( k = 0; k < iGroupCount; k ++)
            {
                /* 设置品质因素为无效状态 */
                memset( pDstValue, 0,( sizeof( UNSIGNED )* iVariable ));
                pDstValue += ChannelPoint;
            }

            /* 采样脉冲计数器 */
            pDstValue = CSMV_GET_SAMPLE_PULSE_MEMORY;
            /* 采样序号的调整 */
            for( k = 0; k < iVariable; k ++)
            {
                * pDstValue ++ = uSerial;
                if( ++ uSerial >= CSMV_GET_SAMPLE_COUNTER )
                {
                    uSerial = 0;
                }
            }
        }
        /* 调整到当前点的位置上,存储下一点数据的时候自己调整 */
        iVariable -= 1;
        /* 采样偏移位置调整 */
        if( iVariable >= CSMV_GET_CHANNEL_POINT )
        {
            iVariable = 0;
        }
    }
    /* 设置采样偏移位置 */
    CSMV_SET_SAMPLE_OFFSET( iVariable );
    return( NU_SUCCESS );
}

/* 设置采样准备标识 */
INT CSMV_SetSampleInitFlag( INT iFlag )
{
    SMVC_SampleInitFlag = iFlag;
    return( NU_SUCCESS );
}

/* 获取采样准备标识 */
INT CSMV_GetSampleInitFlag( VOID )
{
    INT iStatus = NU_UNAVAILABLE;
    if( SMVC_SampleInitFlag == NU_TRUE )
    {
        iStatus = NU_SUCCESS;
    }
    return( iStatus );
}
