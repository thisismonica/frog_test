/*************************************************************************/
/*                                                                       */
/*        Copyright (c) 1999-2002 XJ ELECTRIC CO.LTD.                    */
/*                                                                       */
/* PROPRIETARY RIGHTS of Accelerated Technology are involved in the      */
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
/*      esp_cfg.h                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      ESP - Embed Soft Platform Config                                 */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains function prototypes of all functions          */
/*      accessible to other components.                                  */
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
/*      None                                                             */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhideYang.      02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"

/* Check to see if the file has been included already.  */

#ifndef ESP_CFG_H
#define ESP_CFG_H

#define ESP_MAX_PORT_ADDRESS        4  /* 最大端口地址数目 */

/* 电源通道配置参数. */
typedef struct ESP_CFG_SMVC_POWER_struct
{
    /* 电源通道号 */
    INT ChannelNumber;
    /* 电源上限量值 */
    float MaxValue;
    /* 电源下限量值 */
    float MinValue;
}ESP_CFG_SMVC_POWER, * PESP_CFG_SMVC_POWER;

/* 采样值配置参数. */
typedef struct ESP_CFG_SMVC_struct
{
    /* 使用AD片数 */
    INT AdNumber;
    /* 物理的模拟量通道数 */
    INT PhysicalAnalogNumber;
    /* 双采样SV通道个数 */
    INT DSampleSVChlNum;
    /* 双采样标识 */
    INT DSampleFlag;
    /* 虚拟的模拟量通道数 */
    INT VirtualAnalogNumber;
    /* 模拟量品质因数的通道组数 */
    INT AnalogQualityGroups;
    /* 每周期采样点数 */
    INT PeriodPoint;
    /* 线路频率 */
    float LineFrequency;
    /* 采样率(每秒采样点数) */
    float SampleRate;
    /* 每个通道的采样值点数 */
    INT ChannelPoint;
    /* 采样序号的最大值 */
    INT SampleCounter;
    /* 电源自检的通道个数 */
    INT PowerCheckNumber;
    /* 电源自检配置信息 */
    ESP_CFG_SMVC_POWER * PowerChannel;
    /* 采样值丢点品质因素处理标志 */
    INT LoseSV_Quality;
    /* 丢失一点采样值插值处理标志 */
    INT LoseOneSV_Insert;
}ESP_CFG_SMVC, * PESP_CFG_SMVC;

/* 测频通道配置参数. */
typedef struct ESP_CFG_FREQ_CHANNEL_struct
{
    /* 测频电压通道号 */
    INT ChannelNumber;
    /* 频率变化率 */
    float FrequencyDlt;
}ESP_CFG_FREQ_CHANNEL, * PESP_CFG_FREQ_CHANNEL;

/* 测频配置参数. */
typedef struct ESP_CFG_FREQUENCY_struct
{
    /* 测频通道个数 */
    INT FrequencyNumber;
    /* 测频通道配置参数. */
    ESP_CFG_FREQ_CHANNEL * FrequencyChannel;
}ESP_CFG_FREQUENCY, * PESP_CFG_FREQUENCY;

/* 开入端口配置参数. */
typedef struct ESP_CFG_INPUT_PORT_struct
{
    /* 开入端口数目 */
    INT PortNumber;
    /* 开入端口数据位数 */
    INT PortBits;
    /* 开入端口数据掩码 */
    UNSIGNED PortMasks;
    /* 开入端口地址 */
    VOID * PortAddress[ ESP_MAX_PORT_ADDRESS ];
    /* 开入量反转位掩码 */
    UNSIGNED ReverseBits;
    /* 开入品质因素地址 */
    VOID * QualityAddress;
    /* 开入品质因素检修位地址 */
    VOID * QualityCheckAddr;
}ESP_CFG_INPUT_PORT, * PESP_CFG_INPUT_PORT;

/* 开入量配置参数. */
typedef struct ESP_CFG_INPUT_struct
{
    /* 开入量个数 */
    INT InputNumbers;
    /* 开入量组数 */
    INT InputGroups;
    /* 开入量端口配置 */
    ESP_CFG_INPUT_PORT * InputPort;
    /* 硬件开入量组数 */
    INT HardInputGroups;
    /* 软件开入量组数 */
    INT SoftInputGroups;
    /* 复归开入位(从1开始) */
    INT ResetInputBits;
    /* 录波开入量组数 */
    INT WaveInputGroups;
}ESP_CFG_INPUT, * PESP_CFG_INPUT;

/* 开出端口配置参数. */
typedef struct ESP_CFG_OUTPUT_PORT_struct
{
    /* 开出端口数目 */
    INT PortNumber;
    /* 开出端口数据位数 */
    INT PortBits;
    /* 开出端口数据掩码 */
    INT PortMasks;
    /* 开出端口地址 */
    VOID * PortAddress[ ESP_MAX_PORT_ADDRESS ];
    /* 开出量反转位掩码 */
    UNSIGNED ReverseBits;
    /* 开出品质因素地址 */
    VOID * QualityAddress;
    /* 开出品质因素检修位地址 */
    VOID * QualityCheckAddr;
}ESP_CFG_OUTPUT_PORT, * PESP_CFG_OUTPUT_PORT;

/* 开出量配置参数. */
typedef struct ESP_CFG_OUTPUT_struct
{
    /* 开出量个数 */
    INT OutputNumber;
    /* 开出量组数 */
    INT OutputGroups;
    /* 开出量端口配置 */
    ESP_CFG_OUTPUT_PORT * OutputPort;
    /* 硬件开出量组数 */
    INT HardOutputGroups;
    /* 硬件开出量检测位掩码 */
    UNSIGNED * HardOutputCheckMasks;
    /* 硬件开出量保持位掩码 */
    UNSIGNED * HardOutputHoldMasks;
    /* 硬件开出量信号灯 */
    UNSIGNED * HardOutputSignalLed;
    /* 硬件开出量跳闸灯 */
    UNSIGNED * HardOutputTripLed;
    /* 告警继电器动作位 */
    UNSIGNED * HardOutputAlarm;
    /* 复归继电器动作位 */
    UNSIGNED * HardOutputReset;
    /* 软件开出量组数 */
    INT SoftOutputGroups;
    /* 扩展插件个数 */
    INT HardExtendCount;
}ESP_CFG_OUTPUT, * PESP_CFG_OUTPUT;

/* 装置配置参数. */
typedef struct ESP_CFG_DEVICE_struct
{
    /* CPU号码 */
    INT CpuNumber;
    /* CPU类型 */
    INT CpuType;
    /* 功能选择 */
    UNSIGNED FunSelect;
    /* 装置功能类型 */
    UNSIGNED DeviceFun;
    /* 装置定值区个数 */
    INT SetZoneCount;
}ESP_CFG_DEVICE, * PESP_CFG_DEVICE;

/* 历史记录配置参数. */
typedef struct ESP_CFG_HISTORY_struct
{
    /* 用户事件组数 */
    INT UserEventGroups;
    /* 临时缓存事件个数 */
    INT TempEventNumber;
    /* 临时缓存报告个数 */
    INT TempReportNumber;
}ESP_CFG_HISTORY, * PESP_CFG_HISTORY;

/* 数据表配置参数. */
typedef struct ESP_CFG_TABLE_struct
{
    /* 数据表个数 */
    INT TableNumber;
    /* 字段个数 */
    INT FieldNumber;
    /* 整定值数据个数 */
    INT SettingNumber;
}ESP_CFG_TABLE, * PESP_CFG_TABLE;

/* ESP 配置参数 */
typedef struct ESP_CFG_struct
{
    /* 装置配置参数. */
    ESP_CFG_DEVICE device;
    /* 采样值配置参数. */
    ESP_CFG_SMVC smvc;
    /* 测频配置参数. */
    ESP_CFG_FREQUENCY frequency;
    /* 开入量配置参数. */
    ESP_CFG_INPUT input;
    /* 开出量配置参数. */
    ESP_CFG_OUTPUT output;
    /* 数据表配置参数. */
    ESP_CFG_TABLE table;
    /* 历史记录配置参数. */
    ESP_CFG_HISTORY history;
}ESP_CFG, * PESP_CFG;

/*  Initialization functions.  */

/* Error checking for supplemental functions.  */

/* Core processing functions.  */

#endif
