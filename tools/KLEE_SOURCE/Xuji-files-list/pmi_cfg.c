
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
/*      pmi_cfg.c                                       Ver  1.0         */
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
#include "plat/inc/esp_cfg.h"
#include "plat_specific/plat_specific.h"
#include "plat/inc/apf_extr.h"
#include "plat/inc/xj_out_check.h"
#include "plat/net_inc/lc_extr.h"
#include "plat/inc/ctb_extr.h"
#include "plat/inc/pgf_defs.h"
#include "plat/inc/csmv_extr.h"

#define MAX_POWER_CHECK_NUMBER          8               /* 最大电源自检的通道个数 */
#define MAX_FREQ_CHANNEL_NUMBER         2               /* 最大测频的通道个数 */
#define BOUNDARY_CHECK(x,y) ((x > y) ? y : x )          /* 边界检查 */
#define DEVICE_CPU_TYPE                 0x0B /* CPU板类型(bit0-启动;bit1-跳闸;bit2-启动回路串连;bit3-主CPU板) */

/* 品质因素地址定义 */
#define SIN_QUA(x)                      (VOID *)(&PLAT_SoftInputQuality[x]) /* 数字开入量品质因素地址 */
#define SOUT_QUA(x)                     (VOID *)(&PLAT_SoftOutputQuality[x]) /* 数字开出量品质因素地址 */
#define SOUT_CHECKQUA(x)                (VOID *)(&PLAT_SoftOutputCheckQuality[x])  /* 开出品质因素检修位地址 */
#define SIN_CHECKQUA(x)                 (VOID *)(&PLAT_SoftInputCheckQuality[x])   /* 开入品质因素检修位地址 */
#define OUTR_QUA(x)                     (VOID *)(&PLAT_OutputRQuality[x])    /* 保留品质因素地址 */
#define OUTR_CHECKQUA(x)                (VOID *)(&PLAT_RCheckQuality[x])     /* 保留品质因素检修位地址 */
/* 数字量开入端口定义 */
#define SIN1_NUM                        1               /* 端口数目 */
#define SIN1_BITS                       32              /* 端口数据位数 */
#define SIN1_MASKS                      0xFFFFFFFF      /* 端口数据掩码 */
#define SIN1_REVERSE                    0               /* 数据反转位 */
#define SIN1_PORT0(x)                   (VOID *)(&PLAT_SoftInputMemory[x]) /* 数字开入量端口地址1 */
#define SIN1_PORT1(x)                   0               /* 端口地址2 */
#define SIN1_PORT2(x)                   0               /* 端口地址3 */
#define SIN1_PORT3(x)                   0               /* 端口地址4 */
/* 数字量开出端口定义 */
#define SOUT1_NUM                       1               /* 端口数目 */
#define SOUT1_BITS                      32              /* 端口数据位数 */
#define SOUT1_MASKS                     0xFFFFFFFF      /* 端口数据掩码 */
#define SOUT1_REVERSE                   0x00000000      /* 数据反转位 */
#define SOUT1_PORT0(x)                  (VOID *)(&PLAT_SoftOutputMemory[x]) /* 数字开出量端口地址1 */
#define SOUT1_PORT1(x)                  0               /* 端口地址2 */
#define SOUT1_PORT2(x)                  0               /* 端口地址3 */
#define SOUT1_PORT3(x)                  0               /* 端口地址4 */
/* 硬件仿真开出检测端口定义 */
#define SHCOUT1_NUM                     1               /* 端口数目 */
#define SHCOUT1_BITS                    32              /* 端口数据位数 */
#define SHCOUT1_MASKS                   0xFFFFFFFF      /* 端口数据掩码 */
#define SHCOUT1_REVERSE                 0x00000000      /* 数据反转位 */
#define SHCOUT1_PORT0(x)                (VOID *)(&PLAT_HardOutputCheckPort[x]) /* 硬件仿真开出检测端口1 */
#define SHCOUT1_PORT1(x)                0               /* 端口地址2 */
#define SHCOUT1_PORT2(x)                0               /* 端口地址3 */
#define SHCOUT1_PORT3(x)                0               /* 端口地址4 */
/* 硬件仿真扩展开出端口定义 */
#define SEHOUT1_NUM                     1               /* 端口数目 */
#define SEHOUT1_BITS                    32              /* 端口数据位数 */
#define SEHOUT1_MASKS                   0xFFFFFFFF      /* 端口数据掩码 */
#define SEHOUT1_REVERSE                 0x00000000      /* 数据反转位 */
#define SEHOUT1_PORT0(x)                (VOID *)(&PLAT_HardExOutPort[x]) /* 硬件仿真扩展开出端口1 */
#define SEHOUT1_PORT1(x)                0               /* 端口地址2 */
#define SEHOUT1_PORT2(x)                0               /* 端口地址3 */
#define SEHOUT1_PORT3(x)                0               /* 端口地址4 */
/* 硬件仿真扩展开出检测端口定义 */
#define SEHCOUT1_NUM                    1               /* 端口数目 */
#define SEHCOUT1_BITS                   32              /* 端口数据位数 */
#define SEHCOUT1_MASKS                  0xFFFFFFFF      /* 端口数据掩码 */
#define SEHCOUT1_REVERSE                0x00000000      /* 数据反转位 */
#define SEHCOUT1_PORT0(x)               (VOID *)(&PLAT_HardExOutCheckPort[x]) /* 硬件仿真扩展开出检测端口1 */
#define SEHCOUT1_PORT1(x)               0               /* 端口地址2 */
#define SEHCOUT1_PORT2(x)               0               /* 端口地址3 */
#define SEHCOUT1_PORT3(x)               0               /* 端口地址4 */

/* 端口数据定义 */
#define PORT_DATA_COMM(MNAME,QNAME,CHECKQNAME,NUMBER) \
    MNAME##_NUM, \
    MNAME##_BITS, \
    MNAME##_MASKS, \
    MNAME##_PORT0(NUMBER), \
    MNAME##_PORT1(NUMBER), \
    MNAME##_PORT2(NUMBER), \
    MNAME##_PORT3(NUMBER), \
    MNAME##_REVERSE, \
    QNAME##_QUA(NUMBER), \
    CHECKQNAME##_CHECKQUA(NUMBER),

/* Define external inner-component global data references.  */
/* 平台出口管理句柄 */
HIOM PlatHIOM;
ESP_CFG PLAT_cfg;
UNSIGNED PLAT_SoftInputMemory[CPC_MAX_INPUT_GROUPS];    /* 数字开入量端口 */
UNSIGNED PLAT_SoftInputQuality[CPC_MAX_INPUT_GROUPS];   /* 数字开入量品质因素 */
UNSIGNED PLAT_SoftInputCheckQuality[CPC_MAX_INPUT_GROUPS];   /* 数字开入量品质因素 */
UNSIGNED PLAT_SoftOutputMemory[CPC_MAX_OUTPUT_GROUPS];  /* 数字开出量端口 */
UNSIGNED PLAT_SoftOutputQuality[CPC_MAX_OUTPUT_GROUPS]; /* 数字开出量品质因素 */
UNSIGNED PLAT_SoftOutputCheckQuality[CPC_MAX_OUTPUT_GROUPS]; /* 数字开出量品质因素检修位 */
UNSIGNED PLAT_HardOutputCheckPort[CPC_MAX_OUTPUT_GROUPS]; /* 硬件开出检测仿真端口 */
UNSIGNED PLAT_HardExOutPort[CPC_MAX_OUTPUT_GROUPS];       /* 硬件扩展开出仿真端口 */
UNSIGNED PLAT_HardExOutCheckPort[CPC_MAX_OUTPUT_GROUPS];  /* 硬件扩展开出检测仿真端口 */
/* 此缓冲区用于那些需要配置端口但是不需要品质因素的端口使用 */
UNSIGNED PLAT_OutputRQuality[CPC_MAX_OUTPUT_GROUPS];
/* 此缓冲区用于那些需要配置端口但是不需要品质因素检修位的端口使用 */
UNSIGNED PLAT_RCheckQuality[CPC_MAX_OUTPUT_GROUPS];

static ESP_CFG_SMVC_POWER PLAT_PowerCfg[MAX_POWER_CHECK_NUMBER];
static ESP_CFG_FREQ_CHANNEL PLAT_FreqChlCfg[MAX_FREQ_CHANNEL_NUMBER];
static ESP_CFG_OUTPUT_PORT PLAT_OutputPortCfg[CPC_MAX_OUTPUT_GROUPS];
static UNSIGNED PLAT_OutputCheckMasks[CPC_MAX_OUTPUT_GROUPS];
static UNSIGNED PLAT_OutputHoldMasks[CPC_MAX_OUTPUT_GROUPS];
static UNSIGNED PLAT_OutputSignalLed[CPC_MAX_OUTPUT_GROUPS];
static UNSIGNED PLAT_OutputTripLed[CPC_MAX_OUTPUT_GROUPS];
static UNSIGNED PLAT_OutputAlarm[CPC_MAX_OUTPUT_GROUPS];
static UNSIGNED PLAT_OutputReset[CPC_MAX_OUTPUT_GROUPS];
static ESP_CFG_INPUT_PORT PLAT_InputPortCfg[CPC_MAX_INPUT_GROUPS];
/* 内部函数 */

/* 数字量开入端口配置 */
static ESP_CFG_INPUT_PORT PLAT_SoftInputPortCfg[] =
{
    PORT_DATA_COMM( SIN1, SIN, SIN, 0 )   /* 开入1  */
    PORT_DATA_COMM( SIN1, SIN, SIN, 1 )   /* 开入2  */
    PORT_DATA_COMM( SIN1, SIN, SIN, 2 )   /* 开入3  */
    PORT_DATA_COMM( SIN1, SIN, SIN, 3 )   /* 开入4  */
    PORT_DATA_COMM( SIN1, SIN, SIN, 4 )   /* 开入5  */
    PORT_DATA_COMM( SIN1, SIN, SIN, 5 )   /* 开入6  */
    PORT_DATA_COMM( SIN1, SIN, SIN, 6 )   /* 开入7  */
    PORT_DATA_COMM( SIN1, SIN, SIN, 7 )   /* 开入8  */
    PORT_DATA_COMM( SIN1, SIN, SIN, 8 )   /* 开入9  */
    PORT_DATA_COMM( SIN1, SIN, SIN, 9 )   /* 开入10 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 10 )  /* 开入11 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 11 )  /* 开入12 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 12 )  /* 开入13 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 13 )  /* 开入14 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 14 )  /* 开入15 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 15 )  /* 开入16 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 16 )  /* 开入17 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 17 )  /* 开入18 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 18 )  /* 开入19 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 19 )  /* 开入20 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 20 )  /* 开入21 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 21 )  /* 开入22 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 22 )  /* 开入23 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 23 )  /* 开入24 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 24 )  /* 开入25 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 25 )  /* 开入26 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 26 )  /* 开入27 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 27 )  /* 开入28 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 28 )  /* 开入29 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 29 )  /* 开入30 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 30 )  /* 开入31 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 31 )  /* 开入32 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 32 )  /* 开入33 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 33 )  /* 开入34 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 34 )  /* 开入35 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 35 )  /* 开入36 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 36 )  /* 开入37 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 37 )  /* 开入38 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 38 )  /* 开入39 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 39 )  /* 开入40 */
};

/* 数字量开出端口配置 */
static ESP_CFG_OUTPUT_PORT PLAT_SoftOutputPortCfg[] =
{
    PORT_DATA_COMM( SOUT1, SOUT, SOUT, 0 )  /* 开出1 */
    PORT_DATA_COMM( SOUT1, SOUT, SOUT, 1 )  /* 开出2 */
    PORT_DATA_COMM( SOUT1, SOUT, SOUT, 2 )  /* 开出3 */
    PORT_DATA_COMM( SOUT1, SOUT, SOUT, 3 )  /* 开出4 */
    PORT_DATA_COMM( SOUT1, SOUT, SOUT, 4 )  /* 开出5 */
	PORT_DATA_COMM( SOUT1, SOUT, SOUT, 5 )  /* 开出6 */
    PORT_DATA_COMM( SOUT1, SOUT, SOUT, 6 )  /* 开出7 */
    PORT_DATA_COMM( SOUT1, SOUT, SOUT, 7 )  /* 开出8 */
    PORT_DATA_COMM( SOUT1, SOUT, SOUT, 8 )  /* 开出9 */
    PORT_DATA_COMM( SOUT1, SOUT, SOUT, 9 )  /* 开出10*/
};

/* Define internal function calls.  */

/* 设置应用程序配置参数 */
INT PMI_SetApplicationCfg( VOID )
{
    INT k;
    INT status;
    INT number;
    SETDEVPARAM SetDevParam;
    ANALOGSET AnalogSet;
    SETOUTBITS SetOutBits;
    SETTABNUMBER SetTabNumber;
    INT Groups;
    INT SubGroups;
    INT HardGroups;
    INT SoftGroups;
    ESP_CFG_INPUT_PORT * pInputPort;
    ESP_CFG_OUTPUT_PORT * pOutputPort;
    ESP_CFG_SMVC_POWER * PowerChannel;
    ESP_CFG_FREQ_CHANNEL * FrequencyChannel;

    /* 设置装置基本参数 */
    status = APFC_GetParameter( DEVICE_PARAM_SET_ID,& SetDevParam );
    if( status == NU_SUCCESS )
    {
        PLAT_cfg.device.CpuNumber = SetDevParam.Cpu_Number; /* CPU号码 */
        PLAT_cfg.device.CpuType = SetDevParam.Cpu_Type;     /* CPU板类型 */
        PLAT_cfg.device.FunSelect = SetDevParam.Fun_Select; /* 功能选择 */
        PLAT_cfg.device.DeviceFun = SetDevParam.Device_Fun; /* 装置功能类型 */
        PLAT_cfg.device.SetZoneCount = SetDevParam.SetZoneCount; /* 装置定值区个数 */
        if( PLAT_cfg.device.FunSelect & FUN_OPTICAL )
        {
            PLAT_cfg.smvc.SampleCounter = PLAT_MAX_SAMPLE_COUNTER_OLT; /* 光纤最大的采样序号值 */
            PLAT_cfg.smvc.LoseSV_Quality = XJ_TRUE; /* 丢点采样值数据品质因素处理 */
            PLAT_cfg.smvc.LoseOneSV_Insert = XJ_FALSE; /* 丢一点采样值数据不插值 */
        }
        else
        {
            PLAT_cfg.smvc.SampleCounter = PLAT_MAX_SAMPLE_COUNTER; /* 最大的采样序号值 */
            PLAT_cfg.smvc.LoseSV_Quality = XJ_FALSE; /* 丢点采样值数据品质因素不处理 */
            PLAT_cfg.smvc.LoseOneSV_Insert = XJ_TRUE; /* 丢一点采样值数据插值 */
        }

        /* 如果为智能装置将对时间隔设置为1秒 */
        if( PLAT_cfg.device.FunSelect & FUN_IED_DEVICE )
        {
            /* 设置对时时间间隔 */
            XJP_TM_SET_Time_Interval( 1000 );
        }
        /* 设置装置运行灯状态 */
        XJP_Led_Change_Type( SetDevParam.Led_NormalStatus,
            SetDevParam.Led_StartStatus, SetDevParam.Led_AbnorStatus );

        PLAT_cfg.history.UserEventGroups =
            BOUNDARY_CHECK(( SetDevParam.Max_EventRecordGroup ), CPC_MAX_EVENT_RECORD ); /* 用户记录事件组数 */
        PLAT_cfg.history.TempEventNumber =
            BOUNDARY_CHECK(( SetDevParam.Max_Event_Size ), CPC_MAX_RAM_EVENT_NUMBER ); /* 临时缓存事件个数 */
        PLAT_cfg.history.TempReportNumber =
            BOUNDARY_CHECK(( SetDevParam.Max_Report_Size ), CPC_MAX_RAM_REPORT_NUMBER ); /* 临时缓存报告个数 */

        HardGroups = SetDevParam.Hard_Input_Group;
        SoftGroups = SetDevParam.Soft_Input_Group;
        Groups = HardGroups + SoftGroups;
        PLAT_cfg.input.InputNumbers = SetDevParam.Max_Input; /* 开入量个数 */
        PLAT_cfg.input.InputGroups = BOUNDARY_CHECK( Groups, CPC_MAX_INPUT_GROUPS ); /* 开入量组数 */
        PLAT_cfg.input.WaveInputGroups = BOUNDARY_CHECK( CPC_MAX_WAVE_INPUT_GROUPS, PLAT_cfg.input.InputGroups ); /* 录波开入量组数 */
        PLAT_cfg.input.HardInputGroups =
            BOUNDARY_CHECK(( SetDevParam.Hard_Input_Group ),( UNSIGNED )( PLAT_cfg.input.InputGroups ) ); /* 硬件开入量组数 */
        PLAT_cfg.input.SoftInputGroups =
            ( PLAT_cfg.input.InputGroups ) - ( PLAT_cfg.input.HardInputGroups ); /* 软件开入量组数 */

        pInputPort = PLAT_cfg.input.InputPort;
        pInputPort += PLAT_cfg.input.HardInputGroups;
        for( k = 0; k < ( PLAT_cfg.input.SoftInputGroups ); k ++)
        {
            /* 数字量开入映射 */
            pInputPort -> PortNumber = PLAT_SoftInputPortCfg[k].PortNumber;
            pInputPort -> PortBits = PLAT_SoftInputPortCfg[k].PortBits;
            pInputPort -> PortMasks = PLAT_SoftInputPortCfg[k].PortMasks;
            pInputPort -> ReverseBits = PLAT_SoftInputPortCfg[k].ReverseBits;
            pInputPort -> PortAddress[0] = PLAT_SoftInputPortCfg[k].PortAddress[0]; /* 端口地址1 */
            pInputPort -> PortAddress[1] = PLAT_SoftInputPortCfg[k].PortAddress[1]; /* 端口地址2 */
            pInputPort -> PortAddress[2] = PLAT_SoftInputPortCfg[k].PortAddress[2]; /* 端口地址3 */
            pInputPort -> PortAddress[3] = PLAT_SoftInputPortCfg[k].PortAddress[3]; /* 端口地址4 */
            pInputPort -> QualityAddress = PLAT_SoftInputPortCfg[k].QualityAddress;
            pInputPort -> QualityCheckAddr = PLAT_SoftInputPortCfg[k].QualityCheckAddr; /* 品质因素检修位地址 */
            pInputPort ++;
        }
        PLAT_cfg.output.OutputNumber = SetDevParam.Max_Output; /* 开出量个数 */
    }

    /* 设置模拟量基本通道 */
    if( status == NU_SUCCESS )
    {
        status = APFC_GetParameter( ANALOG_CHANNEL_SET_ID,& AnalogSet );
        if( status == NU_SUCCESS )
        {
            PLAT_cfg.smvc.AdNumber =
                BOUNDARY_CHECK(( AnalogSet.AD_Number ), CPC_MAX_AD_NUMBER ); /* 使用AD片数 */
            PLAT_cfg.smvc.PhysicalAnalogNumber =
                BOUNDARY_CHECK(( AnalogSet.Analog_Number ), CPC_MAX_PHYSICAL_CHL_NUMBER ); /* 物理的模拟量通道数 */
            /* 双采样SV通道个数 */
            PLAT_cfg.smvc.DSampleSVChlNum = BOUNDARY_CHECK(( AnalogSet.DSampleSVChlNum ), ( AnalogSet.Analog_Number ));
            /* 双采样标识设置false,等判别具体是否为双采样的时候再设置 */
            PLAT_cfg.smvc.DSampleFlag = NU_FALSE;
            PLAT_cfg.smvc.VirtualAnalogNumber =
                BOUNDARY_CHECK(( AnalogSet.Virtual_Analog_Number ), CPC_MAX_VIRTUAL_CHL_NUMBER ); /* 虚拟的模拟量通道数 */
            PLAT_cfg.smvc.AnalogQualityGroups =
                ( PLAT_cfg.smvc.PhysicalAnalogNumber + PLAT_cfg.smvc.VirtualAnalogNumber + 31 )/ 32; /* 模拟量品质因素组数 */

            PLAT_cfg.smvc.PeriodPoint =
                BOUNDARY_CHECK(( AnalogSet.Sample_Point ), CPC_MAX_SAMPLE_POINT ); /* 每周期采样点数 */
            PLAT_cfg.smvc.LineFrequency = AnalogSet.LineFrequency;     /* 线路频率 */
            PLAT_cfg.smvc.SampleRate = AnalogSet.SampleRate;           /* 采样率(每秒采样点数) */
            PLAT_cfg.smvc.PowerCheckNumber = 0;
            PowerChannel = PLAT_cfg.smvc.PowerChannel;
            for( k = 0; k < ( AnalogSet.PowerNumber ); k ++ )
            {
                if((( AnalogSet.PowerChannel + k )->ChannelNumber != - 1 )
                    && ( PLAT_cfg.smvc.PowerCheckNumber < MAX_POWER_CHECK_NUMBER ))
                {
                    PowerChannel -> ChannelNumber = ( AnalogSet.PowerChannel + k )->ChannelNumber;
                    PowerChannel -> MaxValue = ( AnalogSet.PowerChannel + k )->MaxValue;
                    PowerChannel -> MinValue = ( AnalogSet.PowerChannel + k )->MinValue;
                    PLAT_cfg.smvc.PowerCheckNumber ++;
                    PowerChannel ++;
                }
            }

            PLAT_cfg.frequency.FrequencyNumber = 0;
            FrequencyChannel = PLAT_cfg.frequency.FrequencyChannel;
            /* 测频电压通道1 */
            number = PLAT_cfg.smvc.PhysicalAnalogNumber;
            if((( AnalogSet.Frequency_Num1 ) < number ) && (( AnalogSet.Frequency_Num1 ) >= 0 ))
            {
                FrequencyChannel -> ChannelNumber = AnalogSet.Frequency_Num1;
                FrequencyChannel -> FrequencyDlt = AnalogSet.FrequencyDlt; /* 跟踪频率最大变化率(HZ/秒) */
                ( PLAT_cfg.frequency.FrequencyNumber )++;
                FrequencyChannel ++;
            }

            /* 测频电压通道2 */
            if((( AnalogSet.Frequency_Num1 ) < number ) && (( AnalogSet.Frequency_Num2 ) >= 0 ))
            {
                FrequencyChannel -> ChannelNumber = AnalogSet.Frequency_Num2;
                FrequencyChannel -> FrequencyDlt = AnalogSet.FrequencyDlt; /* 跟踪频率最大变化率(HZ/秒) */
                ( PLAT_cfg.frequency.FrequencyNumber )++;
                FrequencyChannel ++;
            }
        }
    }

    /* 设置装置出口位相关参数 */
    if( status == NU_SUCCESS )
    {
        status = APFC_GetParameter( OUT_BITS_SET_ID,& SetOutBits );
        if( status == NU_SUCCESS )
        {
            HardGroups = SetOutBits.Hard_Output_Group;
            SoftGroups = SetOutBits.Soft_Output_Group;
            Groups = HardGroups + SoftGroups;
            PLAT_cfg.output.OutputGroups =
                BOUNDARY_CHECK( Groups, CPC_MAX_OUTPUT_GROUPS ); /* 开出量组数 */
            PLAT_cfg.output.HardOutputGroups =
                BOUNDARY_CHECK(( SetOutBits.Hard_Output_Group ),( UNSIGNED )PLAT_cfg.output.OutputGroups ); /* 硬件开出量组数 */
            Groups = SetOutBits.Hard_Extend_Count;
            /* 扩展插件个数 */
            PLAT_cfg.output.HardExtendCount = BOUNDARY_CHECK( Groups, CPC_MAX_OUTPUT_GROUPS );
            PLAT_cfg.output.SoftOutputGroups =
                ( PLAT_cfg.output.OutputGroups ) - ( PLAT_cfg.output.HardOutputGroups ); /* 软件开出量组数 */
            /* 开出端口映射 */
            pOutputPort = PLAT_cfg.output.OutputPort;
            pOutputPort += PLAT_cfg.output.HardOutputGroups;
            for( k = 0; k < ( PLAT_cfg.output.SoftOutputGroups ); k ++)
            {
                /* 数字开出端口 */
                pOutputPort -> PortNumber = PLAT_SoftOutputPortCfg[k].PortNumber;
                pOutputPort -> PortBits = PLAT_SoftOutputPortCfg[k].PortBits;
                pOutputPort -> PortMasks = PLAT_SoftOutputPortCfg[k].PortMasks;
                pOutputPort -> ReverseBits = PLAT_SoftOutputPortCfg[k].ReverseBits;
                pOutputPort -> PortAddress[0] = PLAT_SoftOutputPortCfg[k].PortAddress[0]; /* 端口地址1 */
                pOutputPort -> PortAddress[1] = PLAT_SoftOutputPortCfg[k].PortAddress[1]; /* 端口地址2 */
                pOutputPort -> PortAddress[2] = PLAT_SoftOutputPortCfg[k].PortAddress[2]; /* 端口地址3 */
                pOutputPort -> PortAddress[3] = PLAT_SoftOutputPortCfg[k].PortAddress[3]; /* 端口地址4 */
                pOutputPort -> QualityAddress = PLAT_SoftOutputPortCfg[k].QualityAddress;
                pOutputPort -> QualityCheckAddr = PLAT_SoftOutputPortCfg[k].QualityCheckAddr; /* 开出品质因素检修位地址 */
                pOutputPort ++;
            }

            *( PLAT_cfg.output.HardOutputCheckMasks + 0 ) = SetOutBits.Out_Check_Bits;  /* 装置出口检测位(0-31) */
            *( PLAT_cfg.output.HardOutputCheckMasks + 1 ) = SetOutBits.Out_Check_Bits1; /* 装置出口检测位(32-63) */
            *( PLAT_cfg.output.HardOutputCheckMasks + 2 ) = SetOutBits.Out_Check_Bits2; /* 装置出口检测位(64-95) */

            if( SetOutBits.Signal_Led_Bits != 0 )
            {
                Groups = ( SetOutBits.Signal_Led_Bits - 1 ) >> CPC_GROUP_MASK32;
                SubGroups = ( 1 << ( ( SetOutBits.Signal_Led_Bits - 1 ) & CPC_SUB_GROUP_MASK32 ));
                if( Groups < PLAT_cfg.output.HardOutputGroups )
                {
                    *( PLAT_cfg.output.HardOutputSignalLed + Groups ) = SubGroups;  /* 信号灯出口位 */
                }
            }

            if( SetOutBits.Trip_Led_Bits != 0 )
            {
                Groups = ( SetOutBits.Trip_Led_Bits - 1 ) >> CPC_GROUP_MASK32;
                SubGroups = ( 1 << ( ( SetOutBits.Trip_Led_Bits - 1 ) & CPC_SUB_GROUP_MASK32 ));
                if( Groups < PLAT_cfg.output.HardOutputGroups )
                {
                    *( PLAT_cfg.output.HardOutputTripLed + Groups ) = SubGroups;    /* 跳闸灯出口位 */
                }
            }

            *( PLAT_cfg.output.HardOutputAlarm + 0 ) = SetOutBits.Alarm_Relay_Bits; /* 告警继电器动作位 */
            *( PLAT_cfg.output.HardOutputReset + 0 ) = SetOutBits.Reset_Relay_Bits; /* 复归继电器动作位 */
            PLAT_cfg.input.ResetInputBits = SetOutBits.Reset_Input_Bits; /* 复归继电器开入位 */
            /* 平台出口管理句柄 */
            PlatHIOM = XJP_IO_Create_Out( PLAT_cfg.output.OutputGroups );
            /* 出口检测初始化函数 */
            OUTC_CheckInitialize( ( PLAT_cfg.output.HardOutputGroups ),( PLAT_cfg.output.OutputGroups ),
                ( PLAT_cfg.output.HardOutputCheckMasks ), CPC_MAX_CHECK_RELAY,
                & Event_1_CB, CEMC_FillEvent, NU_Set_Events, pReport_Protect, CSMV_Msg_SendGooseOut,
                ( PLAT_cfg.output.HardExtendCount ) );
        }
    }

    /* 设置数据表参数个数 */
    if( status == NU_SUCCESS )
    {
        status = APFC_GetParameter( SET_TABLE_PARAMNUM_ID,& SetTabNumber );
        if( status == NU_SUCCESS )
        {
            PLAT_cfg.table.TableNumber = SetTabNumber.Max_Table_Number;
            PLAT_cfg.table.FieldNumber = SetTabNumber.Max_Field_Number;
        }
    }

    return( status );
}

/* 平台配置参数的初始化 */
INT PMI_InitializePlatCfg( VOID )
{
    INT i;

    /* Initialize the protect option.  */
    PLAT_cfg.device.CpuNumber = 1;                      /* CPU号码 */
    PLAT_cfg.device.CpuType = DEVICE_CPU_TYPE;          /* CPU板类型 */
    PLAT_cfg.device.FunSelect = 0;                      /* 功能选择 */
    PLAT_cfg.device.DeviceFun = 0;                      /* 装置功能类型 */
    PLAT_cfg.device.SetZoneCount = 1;                   /* 装置定值区个数 */

    PLAT_cfg.smvc.AdNumber = 0;                         /* 使用AD片数 */
    PLAT_cfg.smvc.PhysicalAnalogNumber = 0;             /* 物理的模拟量通道数 */
    PLAT_cfg.smvc.VirtualAnalogNumber = 0;              /* 虚拟的模拟量通道数 */
    PLAT_cfg.smvc.AnalogQualityGroups = 0;              /* 模拟量品质因数的通道组数 */
    PLAT_cfg.smvc.PeriodPoint = CPC_MAX_SAMPLE_POINT;   /* 每周期采样点数 */
    PLAT_cfg.smvc.ChannelPoint = CPC_MAX_CHANNEL_POINT; /* 每个通道的采样值点数 */
    PLAT_cfg.smvc.SampleCounter = PLAT_MAX_SAMPLE_COUNTER; /* 最大的采样序号值 */
    PLAT_cfg.smvc.PowerCheckNumber = 0;                 /* 电源自检的通道个数 */
    PLAT_cfg.smvc.PowerChannel = PLAT_PowerCfg;         /* 电源自检配置信息 */
    PLAT_cfg.smvc.LoseSV_Quality = XJ_FALSE;            /* 丢点采样值数据品质因素不处理 */
    PLAT_cfg.smvc.LoseOneSV_Insert = XJ_TRUE;           /* 丢一点采样值数据插值处理 */

    PLAT_cfg.frequency.FrequencyNumber = 0;             /* 测频通道个数 */
    PLAT_cfg.frequency.FrequencyChannel = PLAT_FreqChlCfg; /* 测频通道配置参数. */

    PLAT_cfg.input.InputNumbers = 0;                    /* 开入量个数 */
    PLAT_cfg.input.InputGroups = 0;                     /* 开入量组数 */
    PLAT_cfg.input.WaveInputGroups = 0;                 /* 录波开入量组数 */
    PLAT_cfg.input.InputPort = PLAT_InputPortCfg;       /* 开入量端口配置 */
    PLAT_cfg.input.HardInputGroups = 0;                 /* 硬件开入量组数 */
    PLAT_cfg.input.SoftInputGroups = 0;                 /* 软件开入量组数 */
    PLAT_cfg.input.ResetInputBits = 0;                  /* 复归开入位(从1开始) */

    PLAT_cfg.output.OutputNumber = 0;                   /* 开出量个数 */
    PLAT_cfg.output.OutputGroups = 0;                   /* 开出量组数 */
    PLAT_cfg.output.OutputPort = PLAT_OutputPortCfg;    /* 开出量端口配置 */
    PLAT_cfg.output.HardOutputGroups = 0;               /* 硬件开出量组数 */
    PLAT_cfg.output.HardOutputCheckMasks = PLAT_OutputCheckMasks; /* 硬件开出量检测位掩码 */
    PLAT_cfg.output.HardOutputHoldMasks = PLAT_OutputHoldMasks; /* 硬件开出量保持位掩码 */
    PLAT_cfg.output.HardOutputSignalLed = PLAT_OutputSignalLed; /* 硬件开出量信号灯 */
    PLAT_cfg.output.HardOutputTripLed = PLAT_OutputTripLed; /* 硬件开出量跳闸灯 */
    PLAT_cfg.output.HardOutputAlarm = PLAT_OutputAlarm; /* 告警继电器动作位 */
    PLAT_cfg.output.HardOutputReset = PLAT_OutputReset; /* 复归继电器动作位 */
    PLAT_cfg.output.SoftOutputGroups = 0;               /* 软件开出量组数 */
    for( i = 0; i < CPC_MAX_OUTPUT_GROUPS; i ++)
    {
        PLAT_OutputCheckMasks[i] = 0;
        PLAT_OutputHoldMasks[i] = 0;
        PLAT_OutputSignalLed[i] = 0;
        PLAT_OutputTripLed[i] = 0;
        PLAT_OutputAlarm[i] = 0;
        PLAT_OutputReset[i] = 0;
    }
    /* 初始化数字开入量品质因素为有效 */
    for( i = 0; i < CPC_MAX_INPUT_GROUPS; i ++)
    {
        PLAT_SoftInputQuality[i] = 0; /* 无效品质因素 */
        PLAT_SoftInputCheckQuality[i] = 0; /* 检修品质因素 */
    }
    /* 初始化数字开出量品质因素为有效 */
    for( i = 0; i < CPC_MAX_OUTPUT_GROUPS; i ++)
    {
        PLAT_SoftOutputQuality[i] = 0;       /* 无效品质因素 */
        PLAT_SoftOutputCheckQuality[i] = 0;  /* 检修品质因素 */
    }

    PLAT_cfg.history.UserEventGroups = 0;               /* 用户事件组数 */
    PLAT_cfg.history.TempEventNumber = CPC_MAX_RAM_EVENT_NUMBER; /* 临时缓存事件个数 */
    PLAT_cfg.history.TempReportNumber = CPC_MAX_RAM_REPORT_NUMBER; /* 临时缓存报告个数 */

    PLAT_cfg.table.TableNumber = MAX_TABLE_NUMBER;      /* 数据表个数 */
    PLAT_cfg.table.FieldNumber = MAX_FIELD_NUMBER;      /* 字段个数 */

    return( NU_SUCCESS );
}

/* 获取装置功能选择 */
INT PMI_get_device_fun_select( VOID )
{
    return PLAT_cfg.device.FunSelect;
}
