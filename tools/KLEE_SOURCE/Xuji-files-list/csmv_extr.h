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
/*      cdr_extr.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CDD - Common Device Driver                                       */
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
#include "plat/inc/esp_cfg.h"
#include "plat/inc/pro_defs.h"
#include "plat/inc/tab_defs.h"
#include "plat/inc/cin_extr.h"

/* Check to see if the file has been included already.  */

#ifndef CSMV_EXTR_H
#define CSMV_EXTR_H

/* 采样值记录数据结构 */
typedef struct SMVC_SAMPLE_RECORD_STRUCT
{
    /* 最新采样位置偏移 */
    INT sample_offset;
    /* 采样计数 */
    UNSIGNED sample_counter;
    /* 采样脉冲计数器内存指针 */
    UNSIGNED * pSamplePulseMemory;
    //
    /* A/D模拟量通道数 */
    INT AdAnalogCounter;
    /* A/D模拟量通道值 */
    float * pAdAnalogValue;
    /* 模拟量数据内存指针 */
    float * pAnalogMemory;
    /* 模拟量品质因素组数 */
    INT AnalogQualityGroups;
    /* 模拟量品质因素值 */
    UNSIGNED * pAnalogQualityValue;
    /* 模拟量品质因素内存指针 */
    UNSIGNED * pAanlogQualityMemory;
    //
    /* 开入量组数 */
    INT InputGroupsNumber;
    /* 开入量数据内存指针 */
    UNSIGNED * pInputMemory;
    /* 开入量品质因素值 */
    UNSIGNED * pInputQualityValue;
    /* 开入量品质因素内存指针 */
    UNSIGNED * pInputQualityMemory;
    //
    /* 开出量组数 */
    INT OutputGroupsNumber;
    /* 开出量状态值 */
    UNSIGNED * pOutputValue;
    /* 开出量数据内存指针 */
    UNSIGNED * pOutputMemory;
    /* 开出量品质因素值 */
    UNSIGNED * pOutputQualityValue;
    /* 开出量品质因素内存指针 */
    UNSIGNED * pOutputQualityMemory;
    //
    /* 保护状态量组数 */
    INT ProStatusGroupsNumber;
    /* 保护状态量值 */
    UNSIGNED * pProStatusValue;
    /* 保护状态量数据内存指针 */
    UNSIGNED * pProStatusMemory;
    /* 保护状态量品质因素值 */
    UNSIGNED * pProStatusQualityValue;
    /* 保护状态量品质因素指针 */
    UNSIGNED * pProStatusQualityMemory;
    //
    /* 逻辑状态量组数 */
    INT LogicGroupsNumber;
    /* 逻辑状态量值 */
    UNSIGNED * pLogicValue;
    /* 逻辑状态量内存指针 */
    UNSIGNED * pLogicMemory;
    /* 逻辑状态量品质因素值 */
    UNSIGNED * pLogicQualityValue;
    /* 逻辑状态量品质因素内存指针 */
    UNSIGNED * pLogicQualityMemory;
}SMVC_SAMPLE_RECORD, * PSMVC_SAMPLE_RECORD;

/* SV错误信息结构 */
typedef struct SMVC_ERR_STRUCT
{
    /* 采样序号不连续次数 */
    UNSIGNED sampleCounter;
    /* 丢失一点次数 */
    UNSIGNED OneSampleCounter;
}SMVC_ERR;

/* 接收命令报文计数器 */
typedef struct CSMV_MSG_Count_struct
{
    /*private: */
    /* 长度错误计数器 */
    UNSIGNED iLenErr;
    /* 接收8C命令计数器;0-命令个数,1-通道个数 */
    UNSIGNED iRecSmvCount[3];
    /* 接收88命令计数器;0-命令个数,1-开入组数 */
    UNSIGNED iGooseRecCount[2];
    /* 接收83命令计数器 */
    UNSIGNED iRecSynCount;
    /* 接收83命令长度错误计数器 */
    UNSIGNED iRecSynLenErrCount[2];
    /* 接收83命令年错误计数器 */
    UNSIGNED iRecSynYearErrCount[2];
    /* 接收0x8D命令计数器 */
    UNSIGNED iRecGooseTimeCount;
    /* 接收0x8D命令长度错误计数器 */
    UNSIGNED iRecGooseTimeLenErrCount;
    /* 最新错误长度 */
    UNSIGNED iRecGooseTimeErrLen;
    /* 开入位错误计数器 */
    UNSIGNED iRecGooseTimeBitErrCount;
    /* 最新错误的开入位 */
    UNSIGNED iRecGooseTimeErrBits;
    /* 发送对时命令成功个数计数器 */
    UNSIGNED iSendSynSucCount;
    /* 发送对时命令失败个数计数器 */
    UNSIGNED iSendSynErrCount;
    /* 发送83错误时间 */
    UNSIGNED iSendSynErrTime;
    /* 发送GOOSE成功个数计数器 */
    UNSIGNED iSendGooseSucCount;
    /* 发送GOOSE失败个数计数器 */
    UNSIGNED iSendGooseErrCount;
    /* 发送错误当前时间 */
    UNSIGNED uSendGooseErrTime;
    /* 其它命令 */
    UNSIGNED iOtherCount;
    /* 解析报文函数执行最大时间 */
    UNSIGNED uMsgParseMaxTime;
    /* 解析报文函数执行实时时间 */
    UNSIGNED uMsgParseRealTime;
}CSMV_MSG_COUNT;

/*  Initialization functions.  */

/* Error checking for supplemental functions.  */

/* Core processing functions.  */

extern float SMVC_AnalogValueBuf[]; /* 原始模拟量采样值缓冲区 */
extern UNSIGNED SMVC_AnalogQualityBuf[]; /* 原始模拟量品质因素缓冲区 */
extern UNSIGNED SMVC_AnalogCheckQualityBuf[]; /* 原始模拟量品质因素检修位缓冲区 */
extern UNSIGNED CWMC_Protect_Status[]; /* 原始保护状态量 */
extern UNSIGNED CWMC_Protect_Logic_Status[]; /* 原始保护逻辑状态量 */

extern float DC_Coefficient;
extern ESP_CFG PLAT_cfg;
extern SMVC_SAMPLE_RECORD SMVC_SampleRecord;
extern INT ChannelTabNumber;
extern PRO_CHANNEL * pChannelTab;
#define CSMV_GET_CHANNEL_POINT      (PLAT_cfg.smvc.ChannelPoint)        /* 每通道的采样值点数 */
#define CSMV_GET_ANALOG_MEMORY      (SMVC_SampleRecord.pAnalogMemory)   /* 模拟量内存首地址 */
#define CSMV_GET_SAMPLE_OFFSET      (SMVC_SampleRecord.sample_offset)   /* 最新采样值位置偏移 */
#define CSMV_GET_PERIOD_POINT       (PLAT_cfg.smvc.PeriodPoint)         /* 每周期采样点数 */
#define CSMV_GET_SAMPLE_RATE        (PLAT_cfg.smvc.SampleRate)          /* 采样率(每秒采样点数) */
#define CSMV_GET_AD_NUMBER          (PLAT_cfg.smvc.AdNumber)            /* A/D片数 */
#define CSMV_GET_SAMPLE_COUNTER     (PLAT_cfg.smvc.SampleCounter)       /* 采样序号的最大值 */
#define CSMV_GET_PHY_ANALOG_NUMBER  (PLAT_cfg.smvc.PhysicalAnalogNumber) /* 物理的模拟量通道数 */
#define CSMV_GET_DSAMPLE_SV_CHL_NUM (PLAT_cfg.smvc.DSampleSVChlNum)      /* 双采样SV通道个数 */
#define CSMV_GET_DSAMPLE_FLAG       (PLAT_cfg.smvc.DSampleFlag)          /* 双采样标识 */
#define CSMV_GET_VIRT_ANALOG_NUMBER (PLAT_cfg.smvc.VirtualAnalogNumber) /* 虚通道个数 */
#define CSMV_GET_ANALOG_NUMBER       (CSMV_GET_PHY_ANALOG_NUMBER+CSMV_GET_VIRT_ANALOG_NUMBER)
/* 模拟量通道总个数（虚通道+物理通道）*/
#define CSMV_ANALOG_MAX_GROUPS       ((CPC_MAX_PHYSICAL_CHL_NUMBER + 31)/32) /* 最大模拟量通道组数 */
#define CSMV_GET_PHY_ANALOG_GROUPS   ((CSMV_GET_PHY_ANALOG_NUMBER + 31)/32)  /* 物理的模拟量通道组数 */
#define CSMV_GET_ANALOG_GROUPS       (PLAT_cfg.smvc.AnalogQualityGroups)     /* 模拟量通道组数 */
#define CSMV_GET_DC_COEFFICIENT      DC_Coefficient                     /* 直流计算系数 */
#define CSMV_GET_CHL_TABLE_MEMORY    GET_CHANNEL_TAB_MEMORY             /* 模拟量通道数据表内存首址 */
#define CSMV_GET_CHL_TABLE_NUMBER    GET_CHANNEL_TAB_NUMBER             /* 模拟量通道数据表的数据个数 */
#define CSMV_GET_ORIGINL_ANA_QUA_MEM SMVC_AnalogQualityBuf              /* 获取原始模拟量品质因素缓冲区 */
#define CSMV_GET_ORIGINL_ANA_CHECKQUA_MEM SMVC_AnalogCheckQualityBuf    /* 获取原始模拟量品质因素检修位缓冲区 */
#define CSMV_GET_ANALOG_QUA_MEMORY   (SMVC_SampleRecord.pAanlogQualityMemory) /* 模拟量品质因素内存首址 */
#define CSMV_GET_SAMPLE_PULSE_MEMORY (SMVC_SampleRecord.pSamplePulseMemory) /* 采样脉冲计数器内存首址 */
#define CSMV_GET_INPUT_GROUPS        CINC_INPUT_GROUP_NUMBER            /* 开入端口组数 */
#define CSMV_GET_WAVE_INPUT_GROUPS   CINC_WAVE_INPUT_GROUP_NUMBER       /* 录波开入量组数 */
#define CSMV_GET_INPUT_MEMORY        (SMVC_SampleRecord.pInputMemory)   /* 开入量内存首地址 */
#define CSMV_GET_INPUT_QUA_MEMORY    (SMVC_SampleRecord.pInputQualityMemory)  /* 开入量品质因素内存首地址 */
#define CSMV_GET_INPUT_PORT          CINC_INPUT_PORT_MEMORY             /* 开入端口首地址 */
#define CSMV_GET_OUTPUT_GROUPS       (PLAT_cfg.output.OutputGroups)     /* 开出端口组数 */
#define CSMV_GET_OUTPUT_MEMORY       (SMVC_SampleRecord.pOutputMemory)  /* 开出量内存首地址 */
#define CSMV_GET_OUTPUT_QUA_MEMORY   (SMVC_SampleRecord.pOutputQualityMemory)  /* 开出量品质因素内存首地址 */
#define CSMV_GET_POWER_CHECK_NUMBER  (PLAT_cfg.smvc.PowerCheckNumber)      /* 获取自检电源通道个数 */
#define CSMV_GET_POWER_CHL_INFO_MEMORY (PLAT_cfg.smvc.PowerChannel)        /* 获取自检电源通道信息内存首址 */
#define CSMV_GET_ORIGINL_PRO_STS_MEM CWMC_Protect_Status                   /* 获取原始保护状态量缓冲区 */
#define CSMV_GET_PRO_STS_MEMORY      (SMVC_SampleRecord.pProStatusMemory)  /* 保护状态量内存首地址 */
#define CSMV_GET_PRO_STS_QUA_MEMORY  (SMVC_SampleRecord.pProStatusQualityMemory)  /* 保护状态量品质因素内存首地址 */
#define CSMV_GET_PRO_STS_GROUPS      (SMVC_SampleRecord.ProStatusGroupsNumber)  /* 保护状态量组数 */
#define CSMV_GET_ORIGINL_PRO_LOG_MEM CWMC_Protect_Logic_Status          /* 获取原始保护逻辑状态量缓冲区 */
#define CSMV_GET_PRO_LOG_MEMORY      (SMVC_SampleRecord.pLogicMemory)   /* 保护逻辑状态量内存首地址 */
#define CSMV_GET_PRO_LOG_QUA_MEMORY  (SMVC_SampleRecord.pLogicQualityMemory)   /* 保护逻辑状态量品质因素内存首地址 */
#define CSMV_GET_PRO_LOG_GROUPS      (SMVC_SampleRecord.ProStatusGroupsNumber) /* 保护逻辑状态量组数 */

#define CSMV_IF_LOSE_SV_QUALITY      (PLAT_cfg.smvc.LoseSV_Quality)     /* 丢点采样值数据品质因素处理标志 */
#define CSMV_IF_LOSE_ONE_SV_INSERT   (PLAT_cfg.smvc.LoseOneSV_Insert)   /* 丢一点采样值数据插值处理标志 */

/* 设置采样位置偏移 */
#define CSMV_SET_SAMPLE_OFFSET(offset_value)                            \
{   \
    SMVC_SampleRecord.sample_offset = offset_value ;                    \
}

/* 初始化模拟量参数 */
INT CSMV_InitializeChannelPara( VOID );
/* 计算模拟量通道的直流分量 */
float CSMV_CalculateDC( INT ChannelNumber, float Coefficient, INT SampleOffset );
/* 计算模拟量通道的谐波分量 */
float CSMV_CalculateFourier( INT ChannelNumber, float * ParaPtr, INT SampleOffset );
/* 计算模拟量通道的峰值 */
float CSMV_CalculatePeakValue( INT ChannelNumber, INT SampleOffset );
/* 获取采样基本信息 */
VOID * CSMV_SampleInformation( UNSIGNED * Actual_Number, UNSIGNED * Actual_Size,
    UNSIGNED * Actual_Point );
/* 获取当前采样指针 */
VOID * CSMV_SamplePointer( VOID );
/* 更新硬件采样数据 */
VOID SMVC_SampleUpdateHard( float * sample_data, UNSIGNED pulse_counter );
/* 更新数字量采样数据 */
VOID SMVC_SampleUpdateSoft( float * sample_data, UNSIGNED pulse_counter );
/* 报文解析函数 */
VOID CSMV_MessParse( UNSIGNED_CHAR * data, UINT length );
/* 注册A/D中断用户处理函数 */
VOID SMVC_Register_User_AD_Interrupt( VOID (* User_AD_Interrupt )( VOID ));
/* 注册A/D中断采样数据转换函数 */
VOID SMVC_Register_Sample_Conversion( INT (* User_Sample_Conversion )( float * , INT ));
/* 获取采样次数计数器 */
INT SMVC_GetSampleCount( UNSIGNED * pSampleCount );
/* 获取其它采样数据起始指针 */
INT CSMV_GetDataStartPointer( INT  iType, VOID ** valueMem, VOID ** qualityMemr, INT * number );
/* 获取采样位置偏移 */
UNSIGNED CSMV_GetSampleOffset( VOID );
/* 发送GOOSE开出管理函数 */
VOID CSMV_Msg_SendGooseOut( VOID );
/* 设置装置状态函数 */
INT CSMV_MSG_SetDevStatus( UNSIGNED uDevStatus );
/* 用户从NPI侧接收数据 */
INT CSMV_MSG_GetDataFromNpi( UNSIGNED_CHAR * buffer, INT * Length );
/* 用户向NPI侧发送数据函数 */
INT CSMV_MSG_UserSendDataTONpi( UNSIGNED_CHAR * buffer, INT Length );
/* 发送用户数据 */
INT  CSMV_MSG_UserSendData( VOID );
/* NPU向NPI发送对时命令 */
VOID CSMV_Msg_SynTimeCmd( VOID );
/* 设置采样准备标识 */
INT CSMV_SetSampleInitFlag( INT iFlag );
/* 获取采样准备标识 */
INT CSMV_GetSampleInitFlag( VOID );
/* 注册向NPI发送数据接口函数 */
VOID CSMV_RegTONpiSendData( INT (* RegSendDataFun )( UNSIGNED_CHAR * data, INT length ));
/* 设置模拟量通道品质因素录波状态量的其实组号及组数 */
INT CSMV_MSG_SetChlQStateInfo( INT iStartGroup, INT iGroupCount );
/* 获取模拟量通道品质因素录波状态量的起始组号及组数 */
INT CSMV_MSG_GetChlQStateInfo( INT * iStartGroup, INT * iGroupCount );
/* 获取采样缓冲区及采样序号 */
float * CSMV_MSG_GetSVDataInfo( UNSIGNED * SampleSerial );
#endif
