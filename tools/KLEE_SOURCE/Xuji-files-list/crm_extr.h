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
/*      crm_extr.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CRM - Common Report Management                                   */
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

/* Check to see if the file has been included already.  */

#ifndef CRM_EXTR_H
#define CRM_EXTR_H

/* 宏定义 */
#define CRM_MAX_REPORT_SIZE     1500       /* 报告最大数据个数 */
#define CRM_SIZE_OFFSET         0         /* 大小 */
#define CRM_INDEX_OFFSET        1         /* 索引号偏移量 */
#define CRM_FAULT_NUMBER_OFFSET 2         /* 故障序号偏移量 */
#define CRM_TYPE_OFFSET         3         /* 故障类型偏移量 */
#define CRM_END_FLAG           0x55aa5a5a /* 报告结束标志 */
#define CRM_FAULT_NUMBER_MAX    0xFFFF    /* 故障序号最大值 */
#define CRM_FAULT_NUMBER_MIN    1         /* 故障序号最小值 */
#define CRM_DIR_NAME            256       /* 故障报告路径名称 */
#define CRM_VERSION             1         /* 故障报告版本号 */
#define CRM_IDENT_REPORT        ".report" /* 报告标识名称 */

/* 报告头数据结构. */
typedef struct REPORT_HEAD_STRUCT
{
    /* 大小 */
    UNSIGNED Size;
    /* CPU号码 */
    INT CpuNumber;
    /* 序号 */
    UNSIGNED Index;
    /* 故障序号 */
    UNSIGNED FaultNumber;
    /* 报告类型 */
    INT Type;
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
    /* 毫秒 */
    UNSIGNED Millionsecond;
    /* 纳秒 */
    UNSIGNED Nanosecond;
    /* 品质 */
    UNSIGNED Quality;
}CRM_RHD;

/* 定长报告数据结构. */
typedef struct REPORT_FIXED_STRUCT
{
    /* 报告头 */
    CRM_RHD Head;
    /* 数据实际字节数 */
    UNSIGNED DataSize;
    /* 数据 */
    UNSIGNED Data[CRM_MAX_REPORT_SIZE];
    /* 结束标志 */
    UNSIGNED EndFlag;
}CRM_RFD;

/*动态内存管理操作接口数据结构和操作*/
typedef void * (* RPT_MemAlloc )( size_t );
typedef void (* RPT_MemFree )( void *);
struct RPTMemMService
{
    /* 申请内存 */
    RPT_MemAlloc MemAlloc;
    /* 释放内存 */
    RPT_MemFree MemFree;
};

/* 报告类结构 */
typedef struct REPORT_Class_struct CREPORT;
/* 报告类结构 */
struct REPORT_Class_struct{
    /*private: */
    /* 链接节点 */
    CSF_NODE Link;
    /* 初始化标志 */
    UNSIGNED InitFlag;
    /* CPU号码 */
    INT CpuNumber;
    /* 最大的报告个数 */
    UNSIGNED MaxNumber;
    /* 最新的报告序号（已保存） */
    UNSIGNED Serial;
    /* 最新的报告序号（未保存） */
    UNSIGNED Serial1;
    /* 保存的最新故障序号（已保存） */
    UNSIGNED FaultNumber;
    /* 保存的最新故障序号（未保存） */
    UNSIGNED FaultNumber1;
    /* 最新的动作时间（已保存） */
    TIME OpTime;
    /* 最新的动作时间（未保存） */
    TIME OpTime1;
    /* 故障序号（用户使用） */
    UNSIGNED UserFaultNumber;
    /* 报告的ID号（报告读写） */
    UNSIGNED ReportId;
    /* 报告驱动指针（报告读写） */
    NU_DRIVER * ReportDriver;
    /* 报告控制信息驱动指针（控制信息读写） */
    NU_DRIVER * CtrlDriver;
    /* NVRAM起始缓冲区 */
    UNSIGNED * NVStart;
    /* NVRAM结束缓冲区 */
    UNSIGNED * NVEnd;
    /* NVRAM区操作指针（IN） */
    UNSIGNED * NVIn;
    /* NVRAM区操作指针（OUT） */
    UNSIGNED * NVOut;
    /* 更新动作报告标志 */
    INT ReportBusy;
    /* 准备好的最新报告序号（包括保存完成） */
    UNSIGNED ReadySerialNumber;
    /* 准备好的最新故障序号（包括保存完成） */
    UNSIGNED ReadyFaultNumber;
    /* 准备好的最新的动作时间（包括保存完成） */
    TIME ReadyOpTime;
    /* 准备好的NVRAM区操作指针（IN） */
    UNSIGNED * ReadyNVIn;
    /* 文件路径名称 */
    CHAR dirName[ CRM_DIR_NAME ];
    /* 报告索引表管理句柄 */
    HITM ReportHandle;
    /* 最大报告文件索引个数 */
    UNSIGNED MaxReportFileIndex;
    /* 报告文件索引缓冲区首址 */
    UNSIGNED * ReportFileIndexPtr;
    /* 清除报告次数统计 */
    UNSIGNED ClearNumber;
    /* 报告记录句柄 */
    UNSIGNED RecordHandle;
    /* RAM区动作报告循环缓冲区管理句柄 */
    HDCM RamReportHandle;
    /* 报告数据标志 */
    INT ReportDataFlag;
    /* 报告数据处理内存空间 */
    CRM_RFD * ReportDataPtr;
    /* 流程控制主代码 */
    UNSIGNED GroupNumber;
    /* 流程控制副代码 */
    UNSIGNED SubNumber;
    /* 当前故障序号 */
    UNSIGNED CurFaultNumber;

    /*public: */
    /* 报告初始化操作 */
    INT (* Initialize )( CREPORT * this, INT iType, UNSIGNED TimeOut );
    /* 读报告数据 */
    INT (* Read )( CREPORT * this, UNSIGNED ElecNumber, INT startIndex,
        INT IndexNumber, UNSIGNED Offset, UNSIGNED Size, VOID * pBuffer,
        UNSIGNED uTimeOut );
    /* 写报告数据 */
    INT (* Write )( CREPORT * this, VOID * pBuffer, UNSIGNED uSize, UNSIGNED TimeOut );
    /* 获取就绪报告序号 */
    INT (* GetReadySerial )( CREPORT * this, UNSIGNED * Serial );
    /* 获取就绪故障序号 */
    INT (* GetReadyFaultNumber )( CREPORT * this, UNSIGNED * FaultNumber );
    /* 获取就绪报告信息 */
    INT (* GetReadyInformation )( CREPORT * this, UNSIGNED * Serial,
        UNSIGNED * FaultNumber, TIME * Time );
    /* 获取报告基本信息 */
    INT (* Information )( CREPORT * this, UNSIGNED * ElecCounter, UNSIGNED * EndFaultCounter,
        UNSIGNED * StartFaultIndex, UNSIGNED * EndFaultIndex, UNSIGNED uTimeOut );
    /* 查找指定故障序号的索引个数 */
    INT (* FindIndexNumber )( CREPORT * this, INT * Actual_Number, UNSIGNED FaultNumber,
        INT Type, UNSIGNED TimeOut );
    /* 查找故障总数 */
    INT (* FindFaultNumber )( CREPORT * this, VOID * pBuffer, INT * Actual_Number,
        INT Type, UNSIGNED TimeOut );
    /* 获取准备好的报告信息 */
    INT (* Ready )( CREPORT * this, UNSIGNED TimeOut );
    /* 设置报告状态 */
    INT (* SetReportStatus )( CREPORT * this, INT iStatus );
};

/* 报告误信息结构 */
typedef struct CRM_ERR_STRUCT
{
    /* 创建录波目录失败次数 */
    UNSIGNED makeDirCounter;
    /* 打开文件读操作失败次数 */
    UNSIGNED openFileR;
    /* 打开文件写操作失败次数 */
    UNSIGNED openFileW;
    /* 关闭文件读操作失败次数 */
    UNSIGNED closeFileR;
    /* 关闭文件写操作失败次数 */
    UNSIGNED closeFileW;
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
    /* 写报告失败次数 */
    UNSIGNED writeReport;
    /* 重命名文件失败次数 */
    UNSIGNED renameFile;
    /* 调度次数 */
    UNSIGNED runTimes;
    /* 创建报告驱动 */
    UNSIGNED createDriver;
    /* 创建报告互斥量 */
    UNSIGNED createSemaphore;
    /* 创建报告清除事件失败 */
    UNSIGNED createClrEvent;
}CRM_ERR;

/*  Initialization functions.  */
extern CREPORT CReport;

/* Error checking for supplemental functions.  */

/* Core processing functions.  */

INT CRMC_ReportMain( VOID );
/* 填充故障报告 */
INT CRMC_FillReport( INT CpuNumber, UNSIGNED FaultNumber, INT Type,
    UNSIGNED Size, const VOID * Src, TIME * pTime );
/* 读取报告故障序号 */
INT CRMC_ReadFaultNumber( INT CpuNumber );
/* 自动调整报告故障序号 */
INT CRMC_AdjustFaultNumber( INT CpuNumber );
/* 设置报告故障序号 */
INT CRMC_SetFaultNumber( INT CpuNumber, INT iFaultNumber );
/* 设置报告故障状态 */
INT CRMC_SetFaultStatus( INT CpuNumber, INT iFaultStatus );
/* 清除故障报告数据 */
INT CRMC_ClearReport( INT CpuNumber );
/* 创建报告操作对象句柄 */
CREPORT * CRMC_Create( INT CpuNumber, INT MaxNumber, CHAR * dirName );
/* 获取指定CPU报告操作句柄 */
CREPORT * CRMC_GetReportHandle( INT CpuNumber );
/* 设置报告任务看门狗句柄 */
INT CRMC_SetWatchDogId( HWDM hWatchDogId );
/* 应用读取动作报告数据函数 */
INT CRMC_UserReadReportData( INT CpuNumber , UNSIGNED ElecNumber, INT startIndex,
    INT IndexNumber, UNSIGNED Offset, UNSIGNED Size, VOID * pBuffer,
    UNSIGNED uTimeOut );

#endif
