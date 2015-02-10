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
/*      cem_extr.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CRM - Common Event Management                                    */
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

#ifndef CEM_EXTR_H
#define CEM_EXTR_H

/* 宏定义 */
#define CEM_EVENT_NAME          31      /* 事件最大名称长度 */
#define CEM_END_FLAG            0x5a    /* 事件结束标志 */
#define CEM_EVENT_INDEXL        0       /* 事件低位偏移量 */
#define CEM_EVENT_INDEXH        1       /* 事件高位偏移量 */
#define CEM_BYTE_MASKS          0xff    /* 字节数据掩码 */
#define CEM_BYTE_BITS           8       /* 字节数据位数 */
#define CEM_SERIAL_NUMBER_MAX   0xFFFF  /* 事件序号最大值 */
#define CEM_SERIAL_NUMBER_MIN   1       /* 事件序号最小值 */

/* RAM区事件数据结构. */
typedef struct EVENT_RAM_STRUCT
{
    /* 序号 */
    UNSIGNED Index;
    /* 类型标志 */
    INT Flag;
    /* 名称 */
    UNSIGNED_CHAR Name[CEM_EVENT_NAME];
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
}CEM_RAD;

/* NVRAM区事件数据结构. */
typedef struct EVENT_NVRAM_STRUCT
{
    /* 序号低字节 */
    UNSIGNED_CHAR IndexL;
    /* 序号高字节 */
    UNSIGNED_CHAR IndexH;
    /* 类型标志 */
    UNSIGNED_CHAR Flag;
    /* 名称 */
    UNSIGNED_CHAR Name[CEM_EVENT_NAME];
    /* 年 */
    UNSIGNED_CHAR Year;
    /* 月 */
    UNSIGNED_CHAR Month;
    /* 日 */
    UNSIGNED_CHAR Day;
    /* 时 */
    UNSIGNED_CHAR Hour;
    /* 分 */
    UNSIGNED_CHAR Minute;
    /* 毫秒低字节 */
    UNSIGNED_CHAR MillionsecondL;
    /* 毫秒高字节 */
    UNSIGNED_CHAR MillionsecondH;
    /* 结束标志 */
    UNSIGNED_CHAR EndFlag;
}CEM_NVD;

/* 事件类结构 */
typedef struct EVENT_Class_struct CEVENT;
/* 事件类结构 */
struct EVENT_Class_struct{
    /*private: */
    /* 初始化标志 */
    UNSIGNED InitFlag;
    /* 最大的事件个数 */
    UNSIGNED MaxNumber;
    /* 总的事件个数 */
    UNSIGNED Number;
    /* 最新的事件序号（已保存）  */
    UNSIGNED Serial;
    /* 最新的事件序号（未保存）  */
    UNSIGNED Serial1;
    /* 事件的ID号（事件读写） */
    UNSIGNED EventId;
    /* 事件驱动指针（事件读写） */
    NU_DRIVER * EventDriver;
    /* 事件控制信息驱动指针（控制信息读写） */
    NU_DRIVER * CtrlDriver;
    /* NVRAM起始缓冲区 */
    UNSIGNED_CHAR * NVStart;
    /* NVRAM结束缓冲区 */
    UNSIGNED_CHAR * NVEnd;
    /* NVRAM区操作指针（IN） */
    UNSIGNED_CHAR * NVIn;
    /* NVRAM区操作指针（OUT） */
    UNSIGNED_CHAR * NVOut;
    /* 文件路径名称 */
    CHAR * dirName;
    /* 事件索引表管理句柄 */
    HITM EventHandle;
    /* 事件任务看门狗句柄 */
    HWDM hWatchDogId;

    /*public: */
    /* 事件初始化操作 */
    INT (* Initialize )( CEVENT * this, INT iType, UNSIGNED TimeOut );
    /* 读事件数据 */
    INT (* Read )( CEVENT * this, UNSIGNED EventIndex, VOID * Buffer, UNSIGNED TimeOut );
    /* 写事件数据 */
    INT (* Write )( CEVENT * this, VOID * pBuffer, UNSIGNED TimeOut );
    /* 清除事件控制信息 */
    INT (* Clear )( CEVENT * this, UNSIGNED TimeOut );
    /* 获取事件基本信息 */
    INT (* Information )( CEVENT * this, UNSIGNED * Type, UNSIGNED * Number, UNSIGNED * StartSerial,
        UNSIGNED * EndSerial, UNSIGNED uTimeOut );
    /* 查找事件索引号 */
    INT (* Find )( CEVENT * this, VOID * pBuffer, INT * Actual_Number, UNSIGNED uTimeOut );
};

/* 事件误信息结构 */
typedef struct CEM_ERR_STRUCT
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
    UNSIGNED writeEvent;
    /* 重命名文件失败次数 */
    UNSIGNED renameFile;
    /* 调度次数 */
    UNSIGNED runTimes;
    /* 创建驱动失败次数 */
    UNSIGNED createDriver;
    /* 创建互斥量失败次数 */
    UNSIGNED createSemaphore;
}CEM_ERR;

/*  Initialization functions.  */

/* Error checking for supplemental functions.  */

/* Core processing functions.  */

INT CEMC_EventMain( VOID );
INT CEMC_FillEvent( const VOID * pName, INT iEventType );
INT CEMC_FillEventFull( const VOID * pName, TIME * pTime, INT iEventType );
/* 设置事件任务看门狗句柄 */
INT CEMC_SetWatchDogId( HWDM hWatchDogId );

#endif
