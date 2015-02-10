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
/*      cwm_extr.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CWM - Common Wave Management                                     */
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
/*      pro_defs.h                          Protect Control constants    */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhideYang.      02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"
#include "plat/es_plat.h"
#include "plat_specific/plat_specific.h"

/* Check to see if the file has been included already.  */

#ifndef CWM_EXTR_H
#define CWM_EXTR_H

/* 宏定义 */
#define CWM_MAX_DATA_NUMBER     100         /* 录波最大数据个数 */
#define CWM_WAVE_START          0           /* 写录波头数据 */
#define CWM_WAVE_DATA           1           /* 写录波数据 */
#define CWM_WAVE_END            2           /* 录波结束 */
#define CWM_END_FLAG            0x55aa5a5a  /* 录波结束标志 */
#define CWM_MAX_WAVE_POINTER    7           /* 最大录波指针 */
#define CWM_WAVE_CTRL_NEW       1           /* 新生成录波 */

/* 偏移量定义 */
#define CWM_SIZE_OFFSET         0           /* 大小 */
#define CWM_FAULT_NUMBER_OFFSET 1           /* 故障序号偏移量 */
#define CWM_ELEC_FAULT_OFFSET   2           /* 电网故障序号偏移量 */
#define CWM_TYPE_OFFSET         3           /* 类型偏移量 */
#define CWM_ANALOG_NUMBER       4           /* 模拟量组数 */
#define CWM_TOTAL_POINTER       12          /* 总的录波点数 */

#define CWM_FAULT_NUMBER_MIN    1           /* 故障序号最小值 */
#define CWM_FAULT_NUMBER_MAX    0xFFFF      /* 故障序号最大值 */

#define CWM_PROTECT_OPERATE     2           /* 录波保护状态量动作 */
#define CWM_PROTECT_RETURN      1           /* 录波保护状态量返回 */

/* 模拟量信息结构 */
typedef struct CWM_ANALOG_INFO_STRUCT
{
    /* 通道号 */
    UNSIGNED Number;
    /* 信息序号 */
    UNSIGNED Inf;
    /* 计算类型 */
    UNSIGNED Type;
    /* 计算系数 */
    float CalculateCoefficient;
    /* 额定值系数 */
    float RateCoefficient;
    /* 一次额定值 */
    float PrimaryRate;
    /* 二次额定值 */
    float SecondaryRate;
    /* 录波系数 */
    float WaveCoefficient;
    /* 录波一二次转换系数 */
    float WaveConverCoef;
}CWM_ANALOG;

/* 开入量信息结构 */
typedef struct CWM_INPUT_INFO_STRUCT
{
    /* 组号 */
    UNSIGNED GroupNumber;
    /* 掩码 */
    UNSIGNED MaskBits;
}CWM_INPUT;

/* 开出量信息结构 */
typedef struct CWM_OUTPUT_INFO_STRUCT
{
    /* 组号 */
    UNSIGNED GroupNumber;
    /* 掩码 */
    UNSIGNED MaskBits;
}CWM_OUTPUT;

/* 状态量信息结构 */
typedef struct CWM_STATE_INFO_STRUCT
{
    /* 组号 */
    UNSIGNED GroupNumber;
    /* 掩码 */
    UNSIGNED MaskBits;
}CWM_STATE;

/* 逻辑量信息结构 */
typedef struct CWM_LOGICAL_INFO_STRUCT
{
    /* 组号 */
    UNSIGNED GroupNumber;
    /* 掩码 */
    UNSIGNED MaskBits;
}CWM_LOGICAL;

/* 录波记录数据结构  */
typedef struct CWM_WCB_STRUCT
{
    /* 链接节点 */
    CSF_NODE Created;
    /* 启动标志 */
    UNSIGNED StartFlag;
    /* 结束标志 */
    UNSIGNED EndFlag;
    /* 故障序号 */
    UNSIGNED FaultNumber;
    /* 电网故障序号 */
    UNSIGNED ElecNumber;
    /* 类型标志 */
    UNSIGNED Type;
    /* 模拟量组数 */
    UNSIGNED AnalogNumber;
    /* 开入量组数 */
    UNSIGNED InputNumber;
    /* 开出量组数 */
    UNSIGNED OutputNumber;
    /* 状态量组数 */
    UNSIGNED StateNumber;
    /* 逻辑状态量组数 */
    UNSIGNED LogicNumber;
    /* 每周期采样点数 */
    UNSIGNED SamplePointer;
    /* 线路频率 */
    float LineFrequency;
    /* 采样率(每秒采样点数) */
    float SampleRate;
    /* 采样记录间距 */
    UNSIGNED SampleSpace;
    /* 记录录波点数 */
    UNSIGNED RecordNumber;
    /* 采样点记数 */
    UNSIGNED SampleNumber;
    /* 故障前点数 */
    UNSIGNED StartPointer;
    /* 总的故障点数 */
    UNSIGNED TotalPointer;
    /* 年 */
    UNSIGNED Year;
    /* 月 */
    UNSIGNED Month;
    /* 日 */
    UNSIGNED Day;
    /* 时 */
    UNSIGNED Hour;
    /* 分 */
    UNSIGNED Minute;
    /* 秒（毫秒） */
    UNSIGNED Millionsecond;
    /* 纳秒 */
    UNSIGNED Nanosecond;
    /* 时间品质因素 */
    UNSIGNED tQuality;
    /* 数据类型 */
    UNSIGNED WaveDataType;
    /* 模拟量信息 */
    CWM_ANALOG * AnalogInf;
    /* 开入量信息 */
    CWM_INPUT * InputInf;
    /* 开出量信息 */
    CWM_OUTPUT * OutputInf;
    /* 状态量信息 */
    CWM_STATE * StateInf;
    /* 逻辑量信息 */
    CWM_LOGICAL * LogicalInf;
    /* 录波数据内存 */
    UNSIGNED * WaveMemory;
}CWM_WCB;

/* 录波头数据结构. */
typedef struct WAVE_HEAD_STRUCT
{
    /* 大小 */
    UNSIGNED Size;
    /* 故障序号 */
    UNSIGNED FaultNumber;
    /* 电网故障序号 */
    UNSIGNED ElecNumber;
    /* SOF */
    UNSIGNED Sof;
    /* 模拟量组数 */
    UNSIGNED AnalogNumber;
    /* 开入量组数 */
    UNSIGNED InputNumber;
    /* 开出量组数 */
    UNSIGNED OutputNumber;
    /* 状态量组数 */
    UNSIGNED ProtectNumber;
    /* 逻辑状态量组数 */
    UNSIGNED LogicNumber;
    /* 每周期采样点数 */
    UNSIGNED SamplePointer;
    /* 线路频率 */
    float LineFrequency;
    /* 采样率(每秒采样点数) */
    float SampleRate;
    /* 采样间距 */
    UNSIGNED Space;
    /* 故障前的录波点数 */
    UNSIGNED StartPointer;
    /* 总的录波点数 */
    UNSIGNED TotalPointer;
    /* 年 */
    UNSIGNED Year;
    /* 月 */
    UNSIGNED Month;
    /* 日 */
    UNSIGNED Day;
    /* 时 */
    UNSIGNED Hour;
    /* 分 */
    UNSIGNED Minute;
    /* 秒（毫秒） */
    UNSIGNED Millionsecond;
    /* 纳秒 */
    UNSIGNED Nanosecond;
    /* 时间品质因素 */
    UNSIGNED tQuality;
    /* 数据类型 */
    UNSIGNED WaveDataType;
    /* 模拟量信息点号 */
    UNSIGNED ChannelInf[CPC_MAX_CHL_NUMBER];
    /* 开入量组号 */
    UNSIGNED InputGroups[CPC_MAX_WAVE_INPUT_GROUPS];
    /* 开入量投退位 */
    UNSIGNED InputActiveBits[CPC_MAX_WAVE_INPUT_GROUPS];
    /* 开出量组号 */
    UNSIGNED OutputGroups[CPC_MAX_OUTPUT_GROUPS];
    /* 开出量投退位 */
    UNSIGNED OutputActiveBits[CPC_MAX_OUTPUT_GROUPS];
    /* 状态量组号 */
    UNSIGNED StateGroups[CPC_MAX_PROTECT_GROUPS];
    /* 状态量投退位 */
    UNSIGNED ProtectActiveBits[CPC_MAX_PROTECT_GROUPS];
    /* 逻辑量组号 */
    UNSIGNED LogicalGroups[CPC_MAX_PROTECT_LOG_GROUPS];
    /* 逻辑量投退位 */
    UNSIGNED LogicalActiveBits[CPC_MAX_PROTECT_LOG_GROUPS];
    /* 结束标志 */
    UNSIGNED EndFlag;
}CWM_WHD;

/* 模拟量信息头. */
struct WAVE_ANALOG_STRUCT
{
    /* 信息序号 */
    UNSIGNED Inf;
    /* 通道计算系数 */
    float Coefficient;
    /* 通道一次额定值 */
    float Primary_rate;
    /* 通道二次额定值 */
    float Secondary_rate;
};

/* 开入量信息头. */
struct WAVE_INPUT_STRUCT
{
    /* 开入量组号 */
    UNSIGNED GroupNumber;
    /* 掩码 */
    UNSIGNED MaskBits;
};

/* 开出量信息头. */
struct WAVE_OUTPUT_STRUCT
{
    /* 开出量组号 */
    UNSIGNED GroupNumber;
    /* 掩码 */
    UNSIGNED MaskBits;
};

/* 状态量信息头. */
struct WAVE_STATE_STRUCT
{
    /* 状态量组号 */
    UNSIGNED GroupNumber;
    /* 掩码 */
    UNSIGNED MaskBits;
};

/* 逻辑状态量信息头. */
struct WAVE_LOGICAL_STRUCT
{
    /* 逻辑状态量组号 */
    UNSIGNED GroupNumber;
    /* 掩码 */
    UNSIGNED MaskBits;
};

/* 录波量值头部结构. */
typedef struct WAVE_VALUE_HEAD_STRUCT
{
    /* 数据头部信息 */
    union VALUE_HEAD_INFO_UNION
    {
        struct WAVE_ANALOG_STRUCT  analog;
        struct WAVE_INPUT_STRUCT   input;
        struct WAVE_OUTPUT_STRUCT  output;
        struct WAVE_STATE_STRUCT   state;
        struct WAVE_LOGICAL_STRUCT logical;
    }head_info;
}CWM_VHD;

/* 录波类结构 */
typedef struct WAVE_Class_struct CWAVE;
/* 录波类结构 */
struct WAVE_Class_struct{
    /*private: */
    /* 初始化标志 */
    UNSIGNED InitFlag;
    /* 故障序号（已保存）  */
    UNSIGNED FaultNumber;
    /* 故障序号（未处理）  */
    UNSIGNED FaultNumber1;
    /* 电网故障序号（已保存） */
    UNSIGNED ElecNumber;
    /* 电网故障序号（未处理） */
    UNSIGNED ElecNumber1;
    /* 故障序号（用户使用） */
    UNSIGNED UserFaultNumber;
    /* 电网故障序号（用户使用） */
    UNSIGNED UserElecNumber;
    /* 录波记录头指针 */
    CSF_NODE  * WaveList;
    /* 录波的ID号（录波读写） */
    UNSIGNED WaveId;
    /* 录波驱动指针（录波读写） */
    NU_DRIVER * WaveDriver;
    /* 录波动态内存的句柄号 */
    HDMM DmHandle;
    /* 录波控制信息的ID号（控制信息读写） */
    UNSIGNED CtrlId;
    /* 录波控制信息驱动指针（控制信息读写） */
    NU_DRIVER * CtrlDriver;
    /* NVRAM起始缓冲区 */
    UNSIGNED * NVStart;
    /* NVRAM结束缓冲区 */
    UNSIGNED * NVEnd;
    /* NVRAM区操作指针（IN） */
    UNSIGNED * NVIn;
    /* NVRAM区临时操作指针（IN1） */
    UNSIGNED * NVIn1;
    /* NVRAM区操作指针（OUT） */
    UNSIGNED * NVOut;
    /* 更新录波标志 */
    INT WaveBusy;
    /* 准备好的最新录波序号（包括保存完成） */
    UNSIGNED ReadySerialNumber;
    /* 准备好的最新电网故障序号（包括保存完成） */
    UNSIGNED ReadyFaultNumber;
    /* 文件路径名称 */
    CHAR * dirName;
    /* 录波任务看门狗句柄 */
    HWDM hWatchDogId;

    /*public: */
    /* 录波初始化操作 */
    INT (* Initialize )( CWAVE * this, INT iType, UNSIGNED TimeOut );
    /* 读录波数据 */
    INT (* Read )( CWAVE * this, UNSIGNED FaultNumber, VOID * pBuffer,
        UNSIGNED Offset, INT DataType, UNSIGNED Number, UNSIGNED TimeOut );
    /* 写录波数据 */
    INT (* Write )( CWAVE * this, VOID * pBuffer, INT DataType, UNSIGNED Size,
        UNSIGNED TimeOut );
    /* 查找录波 */
    INT (* Find )( CWAVE * this, INT iType, VOID * pBuffer, INT * Actual_Number,
        UNSIGNED uTimeOut );
    /* 获取最新录波序号 */
    INT (* GetSerial )( CWAVE * this, UNSIGNED * Serial );
    /* 获取最新故障序号 */
    INT (* GetFaultNumber )( CWAVE * this, UNSIGNED * FaultNumber );
    /* 获取录波基本信息 */
    INT (* Information )( CWAVE * this, UNSIGNED * Number, UNSIGNED * Serial,
        UNSIGNED * StartFaultNumber, UNSIGNED * EndFaultNumber, UNSIGNED uTimeOut );
    /* 查找故障总数 */
    INT (* FindFaultNumber )( CWAVE * this, VOID * pBuffer, INT * Actual_Number,
        INT Type, UNSIGNED TimeOut );
    /* 获取准备好的录波信息 */
    INT (* Ready )( CWAVE * this, UNSIGNED TimeOut );
    /* 设置录波状态 */
    INT (* SetWaveStatus )( CWAVE * this, INT iStatus );
};

/* 录波误信息结构 */
typedef struct CWM_ERR_STRUCT
{
    /* 创建录波目录失败次数 */
    UNSIGNED makeDirCounter;
    /* 创建内存池失败次数 */
    UNSIGNED craetePool;
    /* 打开文件读操作失败次数 */
    UNSIGNED openFileR;
    /* 打开文件写操作失败次数 */
    UNSIGNED openFileW;
    /* 关闭文件读操作失败次数 */
    UNSIGNED closeFileR;
    /* 关闭文件写操作失败次数 */
    UNSIGNED closeFileW;
    /* 随机读写失败次数 */
    UNSIGNED fseekFile;
    /* 读文件失败次数 */
    UNSIGNED readFile;
    /* 写文件失败次数 */
    UNSIGNED writeFile;
    /* 删除写文件失败次数 */
    UNSIGNED deleteFileW;
    /* 删除列表文件失败次数 */
    UNSIGNED deleteFileL;
    /* 删除目录文件失败次数 */
    UNSIGNED deleteDirFile;
    /* 删除检查无效文件失败次数 */
    UNSIGNED deleteCheckFile;
    /* 删除过期文件失败次数 */
    UNSIGNED deleteFullFile;
    /* 获取信息失败次数 */
    UNSIGNED getInfoFile;
    /* 写录波失败次数 */
    UNSIGNED writeWave;
    /* 重命名文件失败次数 */
    UNSIGNED renameFile;
    /* 申请内存失败 */
    UNSIGNED waveMalloc;
    /* 运行次数 */
    UNSIGNED runTimes;
    /* 创建录波驱动 */
    UNSIGNED createDriver;
    /* 创建录波互斥量 */
    UNSIGNED createSemaphore;
}CWM_ERR;

/*  Initialization functions.  */
extern CWAVE CWave;

/* Error checking for supplemental functions.  */

/* Core processing functions.  */

INT CWMC_WaveMain( VOID );
/* 启动录波(单段) */
INT CWMC_WaveStart( INT iType, INT iSpace, INT iStartPoint, INT iTotalPoint,
    INT iElecNumber, WAVE_CHANNEL * WaveChannel, TIME * pTime, STATE * pState, INT iState );
/* 启动录波(多段) */
INT CWMC_WaveStartMul( INT waveSegments, INT iType, INT * spacePtr, INT iStartPoint,
    INT * totalPointPtr, INT iElecNumber, WAVE_CHANNEL * WaveChannel, TIME * pTime,
    STATE * pState, INT iState );
/* 读取录波故障序号 */
INT CWMC_ReadWaveIndexNumber( VOID );
/* 读取录波电网故障序号 */
INT CWMC_ReadElecFaultNumber( VOID );
/* 自动调整录波电网故障序号 */
INT CWMC_AdjustElecNumber( VOID );
/* 设置录波电网故障序号 */
INT CWMC_SetElecNumber( INT iElecNumber );
/* 设置录波状态量 */
INT CWMC_SetWaveState( STATE * pState, INT iState );
/* 获取录波逻辑状态量信息 */
VOID * CWMC_ProtectLogicInformation( UNSIGNED * Actual_Size );
/* 设置录波故障状态 */
INT CWMC_SetFaultStatus( INT iFaultStatus );
/* 设置录波任务看门狗句柄 */
INT CWMC_SetWatchDogId( HWDM hWatchDogId );

#endif
