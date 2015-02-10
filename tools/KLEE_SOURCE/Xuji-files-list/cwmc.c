
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
/*      cwmc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CWM - Common Wave Management                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common wave         */
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
/*      CWMC_CreateState                    Create a wave status         */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*      cwm_extr.h                          Protect Control functions    */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "plat/inc/xj_cosc.h"
#include "plat/es_plat.h"
#include "plat/inc/dr_extr.h"
#include "plat/inc/esp_cfg.h"
#include "plat/inc/tab_defs.H"
#include "plat/inc/csmv_extr.h"
#include "plat/inc/xj_cfdc.h"
#include "plat_specific/plat_specific.h"
#include "plat/inc/xj_file.h"

#define CWMC_WAVE_DIR_NAME              CPC_WAVE_DIR_NAME /* 录波存储路径 */
#define CWMC_MAX_RAM_WAVE_NUMBER        CPC_MAX_RAM_WAVE_DATA_NUMBER /* 最大内存缓存录波数据个数 */
#define CWMC_MAX_CHL_DATA_GROUPS        CPC_MAX_CHL_NUMBER /* 最大录波模拟量数据值组数 */
#define CWMC_MAX_STATE_DATA_GROUPS      CPC_MAX_STATE_GROUPS /* 最大录波状态量数据值组数 */
#define CWMC_MAX_WAVE_DATA_GROUPS (CWMC_MAX_CHL_DATA_GROUPS+CWMC_MAX_STATE_DATA_GROUPS) /* 最大录波数据值组数 */
#define CWMC_MAX_WAVE_DATA_SIZE         CPC_MAX_WAVE_DATA_SIZE   /* 最大录波数据点数 */
#define CWMC_MAX_NVRAM_WAVE             CPC_MAX_NVRAM_WAVE_NUMBER /* 最大存储录波数目 */
#define CWMC_GET_WAVE_ANALOG_NUMBER     CSMV_GET_ANALOG_NUMBER //录波最大模拟量个数
#define CWMC_GET_WAVE_INPUT_GROUPS      CSMV_GET_WAVE_INPUT_GROUPS //录波最大开入量组
#define CWMC_GET_WAVE_OUTPUT_GROUPS     CSMV_GET_OUTPUT_GROUPS //录波最大开出量组
#define CWMC_MAX_WAVE_STATE_GROUPS      CPC_MAX_PROTECT_GROUPS //录波最大状态量组
#define CWMC_MAX_WAVE_LOGICAL_GROUPS    CPC_MAX_PROTECT_LOG_GROUPS //录波最大逻辑量组
#define CWMC_EVENT_WAVE_CLR             CPC_EVENT2_WAVE_CLR /* 清录波数据 */
#define CWMC_WAVE_TIMEOUT               CPC_WAVE_TIMEOUT /* 录波超时时间 */
#define CWMC_MAX_SAMPLE_POINT           CSMV_GET_CHANNEL_POINT /* 通道最大采样值点数 */
/*==============================================================*/
/* 全局变量申明 */
extern ESP_CFG PLAT_cfg;
extern NU_EVENT_GROUP Event_2_CB;
extern XJ_PROTECT pWave_Protect;
extern PRO_CHANNEL * pChannelTab;

/* 录波设备驱动句柄 */
XJ_DRIVER CWMC_hWaveDriver = XJ_NULL;
XJ_SEMAPHORE CWMC_hSemaphore = XJ_NULL;
CWM_WCB * WavePointer = XJ_NULL;
CWM_WCB * WavePointerCB[ CWM_MAX_WAVE_POINTER ] = { XJ_NULL, XJ_NULL, XJ_NULL }; /* 录波记录控制块 */
UNSIGNED CWMC_Protect_Status[CWMC_MAX_WAVE_STATE_GROUPS];
UNSIGNED CWMC_Protect_Logic_Status[CWMC_MAX_WAVE_LOGICAL_GROUPS];
/* 录波存储路径 */
CHAR CWMC_WaveDirName[256] = CWMC_WAVE_DIR_NAME;
/* 录波索引表管理句柄 */
HITM CWMC_WaveHandle;

/* 宏定义 */
#define CWMC_CREATE_WAVE_DRIVER(x) x=XJ_Create_Driver("Wave",CWMC_WaveDriverEntry) //创建录波驱动
#define CWMC_GET_WAVE_HANDLE() CWMC_hWaveDriver //获取录波句柄
#define CWMC_SET_WAVE_HANDLE(x) CWMC_hWaveDriver = x //设置录波句柄
#define CWMC_CREATE_WAVE_LOCK(x) x=XJ_Create_Semaphore("Wave",1,XJ_FIFO) //创建录波互斥量
#define CWMC_GET_WAVE_LOCK() CWMC_hSemaphore //获取录波互斥量句柄
#define CWMC_SET_WAVE_LOCK(x) CWMC_hSemaphore = x //设置录波互斥量句柄
#define CWMC_LOCK_WAVE(x,y) x=XJP_Mutex_Obtain(CWMC_hSemaphore,y) //获取录波互斥量
#define CWMC_UNLOCK_WAVE(x) x=XJP_Mutex_Release(CWMC_hSemaphore) //释放录波互斥量
#define CWMC_GET_WAVE_CLEAR(x,y) x=XJ_Retrieve_Events(&Event_2_CB,CWMC_EVENT_WAVE_CLR,XJ_AND,y,XJ_NO_SUSPEND) //清除录波
#define CWMC_SET_WAVE_CLEAR() XJ_Set_Events(&Event_2_CB,(UNSIGNED)(~CWMC_EVENT_WAVE_CLR),XJ_AND) //清除录波完成
#define CWMC_FILL_RESERVE_EVENT(x) CEMC_FillEvent(x,RESERVE_EVENT_ID) //填充保留事件
#define CWMC_PROTECT_WAVE() XJ_Protect(pWave_Protect)  //录波数据保护 */
#define CWMC_UNPROTECT_WAVE() XJ_Unprotect()  //释放录波数据保护 */
#define CWMC_CHANNEL_MEMORY_TAB() pChannelTab //通道内存
#define CWMC_SAMPLE_POINT(x) x = (UNSIGNED)(PLAT_cfg.smvc.PeriodPoint) //采样点数
#define CWMC_SAMPLE_FREQUENCY(x) x = (PLAT_cfg.smvc.LineFrequency) //线路频率
#define CWMC_SAMPLE_RATE(x,y) x = ((PLAT_cfg.smvc.SampleRate)/y) //采样率(每秒采样点数)
#define CWMC_DATA_MASKS16() 0xffff //16位数据掩码
#define CWMC_DATA_BITS16() 16 //16位数据位数
#define CWMC_DATA_MASKSH16() 0xffff0000 /* 高16位数据掩码 */
#define CWMC_CHECK_DATA_LIMIT CHECKCYCDATALIMIT //检查数据范围
#define CWMC_LOCK_INTERRUPTS(x) x=XJ_Local_Control_Interrupts(0) //锁中断
#define CWMC_UNLOCK_INTERRUPTS(x) XJ_Local_Control_Interrupts(x) //开中断
#define CWMC_WAVE_ALLOCATE(x,y,z) x=XJP_Memory_Allocate(CWave.DmHandle,y,z) //申请录波动态内存
#define CWMC_WAVE_DELLOCATE(x,y) x=XJP_Memory_Deallocate(CWave.DmHandle,y) //释放录波动态内存
#define CWMC_GET_MAX_WAVE_TIME() CWMC_WAVE_TIMEOUT //最大录波超时时间
#define CWMC_MAX_WAVE_POINT() CWMC_MAX_WAVE_DATA_SIZE //录波最大点数
#define CWMC_ANALOG_BANK_WIDTH() 2 //字宽度/模拟量数据存储宽度
#define CWMC_ANALOG_BANK_MASKS() 0xffff //模拟量数据掩码
#define CWMC_ANALOG_BANK_BITS() 16 //模拟量位数
#define CWMC_MAX_ANALOG_VALUE() 32767 //模拟量最大值
#define CWMC_MIN_ANALOG_VALUE() -32767 //模拟量最小值
#define CWMC_WAVE_FAULT_BITS() 16 //故障序号、电网故障序号位数
#define CWMC_GET_MIN_RECORD_NUMBER() 5 //最小记录数据
#define CWMC_CREATE_WAVE_LIST(x) XJP_CSC_Place_On(&(CWave.WaveList),x) //添加录波列表
#define CWMC_REMOVE_WAVE_LIST(x) XJP_CSC_Remove((&(CWave.WaveList)),x) //删除录波列表
#define CWMC_ANALOG_WIDTH() 2 //模拟量数据的字节数
#define CWMC_STATE_WIDTH() 4  //状态量数据的字节数
#define CWMC_MAX_WAVE_FILE_NUMBER (CWMC_MAX_NVRAM_WAVE + 10) //最大录波文件数目
#define CWMC_WAVE_HEAD_WORD() (sizeof(CWM_WHD)/sizeof(UNSIGNED)) /* 录波头字数 */
#define CWMC_MAX_WAVE_CHL_SIZE() (sizeof(CWM_VHD)+sizeof(UNSIGNED)+(CWMC_MAX_WAVE_DATA_SIZE*CWMC_ANALOG_WIDTH()))
#define CWMC_MAX_WAVE_CHL_WORD() ((CWMC_MAX_WAVE_CHL_SIZE()+sizeof(UNSIGNED)-1)/sizeof(UNSIGNED)) /* 最大模拟量通道字数 */
#define CWMC_MAX_WAVE_STATE_SIZE() (sizeof(CWM_VHD)+sizeof(UNSIGNED)+(CWMC_MAX_WAVE_DATA_SIZE*CWMC_STATE_WIDTH()))
#define CWMC_MAX_WAVE_STATE_WORD() ((CWMC_MAX_WAVE_STATE_SIZE()+sizeof(UNSIGNED)-1)/sizeof(UNSIGNED)) /* 最大状态量通道字数 */
#define CWM_MAX_WAVE_VALUE_WORD \
(( CWMC_MAX_CHL_DATA_GROUPS * CWMC_MAX_WAVE_CHL_WORD()) + (CWMC_MAX_STATE_DATA_GROUPS*CWMC_MAX_WAVE_STATE_WORD()))
/* 最大录波数据值字数 */
#define CWMC_GET_WAVE_ELEC_NUMBER() CWave.UserElecNumber    //获取录波电网故障序号
#define CWMC_GET_WAVE_INDEX() CWave.UserFaultNumber        //获取录波索引号
#define CWMC_SET_WAVE_ELEC_NUMBER(x) CWave.UserElecNumber=x //设置录波电网故障序号
#define CWMC_LISTFILE(x,y,z) xj_ListFile(x,y,z)

/* Define internal function calls.  */
INT CWMC_InitializeWave( CWAVE * this, INT iType, UNSIGNED uTimeOut );
INT CWMC_ReadWaveData( CWAVE * this, UNSIGNED uFaultNumber, VOID * pBuffer,
    UNSIGNED uOffset, INT iDataType, UNSIGNED uNumber, UNSIGNED uTimeOut );
INT CWMC_WriteWaveData( CWAVE * this, VOID * pBuffer, INT iDataType, UNSIGNED uSize,
    UNSIGNED uTimeOut );
INT CWMC_FindWaveFaultNumber( CWAVE * this, INT iType, VOID * pBuffer,
    INT * Actual_Number, UNSIGNED uTimeOut );
VOID CWMC_WaveDriverEntry( XJ_DRIVER hDriver, XJ_DRIVER_REQUEST * pRequest );
INT CWMC_GetWaveHead( CWM_WCB * source, VOID * destination );
INT CWMC_ReadRamWaveData( UNSIGNED uFaultNumber, VOID * pBuffer, UNSIGNED uOffset,
    INT iDataType, UNSIGNED uNumber );
INT CWMC_FindRamWaveFaultNumber( INT iType, VOID * pBuffer, INT * Actual_Number );
INT CWMC_GetWaveFaultNumber( CHAR * dirName, UNSIGNED * Dst,
    UNSIGNED * pActual_size, INT type, UNSIGNED Number );
INT CWMC_GetWaveDataBlock( CHAR * fileName, UNSIGNED Offset, UNSIGNED * dst, UNSIGNED size );
INT CWMC_WriteWaveDataBlock( CHAR * fileName, UNSIGNED * src, UNSIGNED size );
int CWMC_DeleteDirFileCallBack( struct FileInformation * info, void * context );
INT CWMC_DeleteDirFile( CHAR * dirName );
int CWMC_FullFileCheckCallBack( struct FileInformation * info, void * context );
INT CWMC_FullFileCheck( CHAR * dirName, UNSIGNED * Dst, UNSIGNED * pDeleteNumber,
    UNSIGNED MaxNumber );
INT CWMC_WaveFullFileCheckMain( VOID );
INT CWMC_FindWaveIndex( CHAR * dirName, UNSIGNED * waveIndex, UNSIGNED faultIndex );
int CWMC_ListWaveIndexCallBack( struct FileInformation * info, void * context );
INT CWMC_ListWaveIndex( CHAR * dirName, UNSIGNED * Dst, UNSIGNED * pActual_size,
    UNSIGNED Number );
INT CWMC_WaveSerial( CWAVE * this, UNSIGNED * Serial );
INT CWMC_WaveFaultNumber( CWAVE * this, UNSIGNED * FaultNumber );
INT CWMC_GetReadyWave( CWAVE * this, UNSIGNED uTimeOut );
INT CWMC_SetWaveStatus( CWAVE * this, INT iStatus );
/* 生成录波链表 */
INT CWMC_CreateWaveList( INT iElecNumber, INT iType, INT iStartPoint, INT iSpace,
    INT iTotalPoint, WAVE_CHANNEL * WaveChannel, TIME * pTime, INT ctrlType,
    STATE * pState, INT iState );
/* 获取录波序号 */
static INT CWMC_GetWaveIndexNumber( CHAR * dirName, UNSIGNED * Dst,
    UNSIGNED * pActual_size, UNSIGNED Number, UNSIGNED FaultNumber );
/* 获取录波信息函数 */
static INT CWMC_WaveInformation( CWAVE * this, UNSIGNED * Number, UNSIGNED * Serial,
    UNSIGNED * StartFaultNumber, UNSIGNED * EndFaultNumber, UNSIGNED uTimeOut );
/* 查找故障总数 */
static INT CWMC_FindFaultNumber( CWAVE * this, VOID * pBuffer, INT * Actual_Number,
    INT Type, UNSIGNED uTimeOut );
/* 初始化录波RCD数据模型 */
static INT CWMC_InitWaveRcd( INT FaultIndex, INT ElecFaultIndex, INT ElecNum );

CWM_ERR CWMC_ErrInf =
{
    0,                          /* makeDirCounter */
    0,                          /* craetePool */
    0,                          /* openFileR */
    0,                          /* openFileW */
    0,                          /* closeFileR */
    0,                          /* closeFileW */
    0,                          /* fseekFile */
    0,                          /* readFile */
    0,                          /* writeFile */
    0,                          /* deleteFileW */
    0,                          /* deleteFileL */
    0,                          /* deleteDirFile */
    0,                          /* deleteCheckFile */
    0,                          /* deleteFullFile */
    0,                          /* getInfoFile */
    0,                          /* writeWave */
    0,                          /* renameFile */
    0,                          /* waveMalloc */
    0,                          /* runTimes */
    0,                          /* createDriver */
    0,                          /* createSemaphore */
};

CWAVE CWave =
{
    0,                          /* InitFlag */
    0,                          /* FaultNumber */
    0,                          /* FaultNumber1 */
    0,                          /* ElecNumber */
    0,                          /* ElecNumber1 */
    0,                          /* UserFaultNumber */
    0,                          /* UserElecNumber */
    XJ_NULL,                    /* WaveList */
    0,                          /* WaveId */
    XJ_NULL,                    /* WaveDriver */
    0,                          /* DmHandle */
    0,                          /* CtrlId */
    XJ_NULL,                    /* CtrlDriver */
    XJ_NULL,                    /* NVStart */
    XJ_NULL,                    /* NVEnd */
    XJ_NULL,                    /* NVIn */
    XJ_NULL,                    /* NVIn1 */
    XJ_NULL,                    /* NVOut */
    0,                          /* WaveBusy */
    0,                          /* ReadySerialNumber */
    0,                          /* ReadyFaultNumber */
    CWMC_WaveDirName,           /* dirName */
    0,                          /* hWatchDogId */

    & CWMC_InitializeWave,      /* Initialize */
    & CWMC_ReadWaveData,        /* Read */
    & CWMC_WriteWaveData,       /* Write */
    & CWMC_FindWaveFaultNumber, /* Find */
    & CWMC_WaveSerial,          /* GetSerial */
    & CWMC_WaveFaultNumber,     /* GetFaultNumber */
    & CWMC_WaveInformation,     /* Information */
    & CWMC_FindFaultNumber,     /* FindFaultNumber */
    & CWMC_GetReadyWave,        /* Ready */
    & CWMC_SetWaveStatus,       /* SetWaveStatus */
};

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CWMC_InitializeWave                                              */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function clear the initialization the wave.                 */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*      NAME            DATE                    REMARKS                  */
/*                                                                       */
/*      ZhideYang.      02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
INT CWMC_InitializeWave( CWAVE * this, INT iType, UNSIGNED uTimeOut )
{
    INT status;
    unsigned int PoolSize;
    XJ_DRIVER hDriver;
    XJ_SEMAPHORE hSemaphore;

    if( this->InitFlag == 0 )
    {
        /* 创建录波目录 */
        if(( xj_DirExists == XJ_NULL ) || ( xj_MakeDir == XJ_NULL ))
        {
            /* 操作函数不存在，直接返回失败 */
            CWMC_ErrInf.makeDirCounter ++;
            return( XJ_UNAVAILABLE );
        }
        if( xj_DirExists( this->dirName ) == XJ_NULL )
        {
            /* 目录不存在，创建目录 */
            if( xj_MakeDir( this->dirName ) != XJ_SUCCESS )
            {
                CWMC_ErrInf.makeDirCounter ++;
                return( XJ_UNAVAILABLE );
            }
        }
        /* 创建动态内存操作句柄 */
        PoolSize = sizeof( CWM_WCB ); /* 内存录波头 */
        PoolSize += ( CWMC_MAX_WAVE_DATA_GROUPS * sizeof( CWM_VHD ) ); /* 内存通道数据信息 */
        PoolSize += ( CWMC_MAX_WAVE_DATA_GROUPS * CWMC_MAX_WAVE_POINT() * sizeof( UNSIGNED )); /* 内存通道数据值 */
        PoolSize *= CWMC_MAX_RAM_WAVE_NUMBER; /* 内存录波数据总大小 */
        this -> DmHandle = XJP_Memory_Pool_Create( PoolSize );
        if( this -> DmHandle == 0 )
        {
            CWMC_ErrInf.craetePool ++;
            /* 操作失败,直接返回 */
            return( XJ_UNAVAILABLE );
        }
        /* 创建录波驱动 */
        if( CWMC_GET_WAVE_HANDLE() == XJ_NULL )
        {
            CWMC_CREATE_WAVE_DRIVER( hDriver );
            if( hDriver == XJ_NULL )
            {
                /* 创建录波驱动失败 */
                CWMC_ErrInf.createDriver ++;
                return( XJ_UNAVAILABLE );
            }
            CWMC_SET_WAVE_HANDLE( hDriver );
        }
        this -> WaveDriver = CWMC_GET_WAVE_HANDLE();
        /* 创建录波互斥量 */
        if( CWMC_GET_WAVE_LOCK() == XJ_NULL )
        {
            CWMC_CREATE_WAVE_LOCK( hSemaphore );
            if( hSemaphore == XJ_NULL )
            {
                /* 创建录波互斥量失败 */
                CWMC_ErrInf.createSemaphore ++;
                return( XJ_UNAVAILABLE );
            }
            CWMC_SET_WAVE_LOCK( hSemaphore );
        }
        /* 创建录波索引表管理句柄 */
        CWMC_WaveHandle = XJP_Index_Create( CWMC_MAX_WAVE_FILE_NUMBER );
        this -> InitFlag = 1;
    }

    /* 打开录波设备 */
    OPENDRIVER( status, CWMC_GET_WAVE_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* 设备初始化 */
        INITIALIZEDRIVER( status, CWMC_GET_WAVE_HANDLE(), iType, uTimeOut );

        /* 关闭录波设备 */
        CLOSEDRIVER( CWMC_GET_WAVE_HANDLE());
    }

    return( status );
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CWMC_ReadWaveData                                                */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function read the wave data.                                */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*      NAME            DATE                    REMARKS                  */
/*                                                                       */
/*      ZhideYang.      07-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
INT CWMC_ReadWaveData( CWAVE * this, UNSIGNED uFaultNumber, VOID * pBuffer,
    UNSIGNED uOffset, INT iDataType, UNSIGNED uNumber, UNSIGNED uTimeOut )
{
    INT status;

    if( this -> InitFlag == 0 )
    {
        /* 初始化检查 */
        return( XJ_UNAVAILABLE );
    }

    /* 查找RAM录波数据 */
    //    status = CWMC_ReadRamWaveData( uFaultNumber, pBuffer, uOffset, iDataType, uNumber );
    //    if( status == XJ_SUCCESS )
    //    {
    //        return( XJ_SUCCESS );
    //    }

    /* 打开录波设备 */
    OPENDRIVER( status, CWMC_GET_WAVE_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* 读录波数据 */
        READDRIVER( status, CWMC_GET_WAVE_HANDLE(), iDataType, pBuffer, uOffset, uNumber,
            uFaultNumber, uTimeOut );

        /* 关闭录波设备 */
        CLOSEDRIVER( CWMC_GET_WAVE_HANDLE());
    }

    /* Return the completion status.  */
    return( status );
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CWMC_WriteWaveData                                               */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function write wave data.                                   */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*      NAME            DATE                    REMARKS                  */
/*                                                                       */
/*      ZhideYang.      02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
INT CWMC_WriteWaveData( CWAVE * this, VOID * pBuffer, INT iDataType, UNSIGNED uSize,
    UNSIGNED uTimeOut )
{
    INT status;

    if( this -> InitFlag == 0 )
    {
        /* 初始化检查 */
        return( XJ_UNAVAILABLE );
    }

    /* 写录波数据 */
    WRITEDRIVER( status, CWMC_GET_WAVE_HANDLE(), iDataType, pBuffer, 0, uSize, uTimeOut );

    /* Return the completion status.  */
    return status;
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CWMC_WaveInformation                                             */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function retrieve the wave information.                     */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*      NAME            DATE                    REMARKS                  */
/*                                                                       */
/*      ZhideYang.      02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
/* 获取录波信息函数 */
static INT CWMC_WaveInformation( CWAVE * this, UNSIGNED * Number, UNSIGNED * Serial,
    UNSIGNED * StartFaultNumber, UNSIGNED * EndFaultNumber, UNSIGNED uTimeOut )
{
    UNSIGNED Index;
    UNSIGNED uNumber = 0;
    UNSIGNED buffer[CWMC_MAX_NVRAM_WAVE];  /* report buffer */
    UNSIGNED Actual_Number = 0;                    /* total fault number */
    INT status;                                    /* complete status */

    /* 打开录波设备 */
    OPENDRIVER( status, CWMC_GET_WAVE_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* 获取录波电网故障序号 */
        STATUSDRIVER( status, CWMC_GET_WAVE_HANDLE(), buffer,& Actual_Number, 1, uTimeOut );
        if( status == XJ_SUCCESS )
        {
            if( Actual_Number == 0 )
            {
                * Number = 0;
                * StartFaultNumber = 0;
                * EndFaultNumber = 0;
                * Serial = 0;
            }
            else
            {
                /* 获取最新故障序号的录波序号 */
                status = CWMC_GetWaveIndexNumber( CWave.dirName, & Index,
                    ( UNSIGNED *)& uNumber, 1, buffer[( Actual_Number - 1 )] );
                if( status == XJ_SUCCESS )
                {
                    if( uNumber == 0 )
                    {
                        * Number = 0;
                        * StartFaultNumber = 0;
                        * EndFaultNumber = 0;
                        * Serial = 0;
                    }
                    else
                    {
                        * Number = Actual_Number;
                        * StartFaultNumber = buffer[0];
                        * EndFaultNumber = buffer[( Actual_Number - 1 )];
                        * Serial = Index;
                    }
                }
            }
        }

        /* 关闭录波设备 */
        CLOSEDRIVER( CWMC_GET_WAVE_HANDLE());
    }

    /* Return the completion status.  */
    return status;
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CWMC_WaveSerial                                                  */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function get the wave serial.                               */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*      NAME            DATE                    REMARKS                  */
/*                                                                       */
/*      ZhideYang.      02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
INT CWMC_WaveSerial( CWAVE * this, UNSIGNED * Serial )
{
    INT status = XJ_FALSE;              /* complete status */

    /* 检查是否有录波 */
    if( this -> ReadySerialNumber > 0 )
    {
        /* 最新的录波序号 */
        * Serial = this -> ReadySerialNumber;
        status = XJ_TRUE;
    }

    /* Return the completion status.  */
    return ( status );
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CWMC_WaveFaultNumber                                             */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function get the wave fault number.                         */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*      NAME            DATE                    REMARKS                  */
/*                                                                       */
/*      ZhideYang.      02-09-2009      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
INT CWMC_WaveFaultNumber( CWAVE * this, UNSIGNED * FaultNumber )
{
    INT status = XJ_FALSE;              /* complete status */

    /* 检查是否有故障录波 */
    if( this -> ReadyFaultNumber > 0 )
    {
        /* 最新的故障序号 */
        * FaultNumber = this -> ReadyFaultNumber;
        status = XJ_TRUE;
    }

    /* Return the completion status.  */
    return status;
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CWMC_GetReadyWave                                                */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function get ready wave information.                        */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*      NAME            DATE                    REMARKS                  */
/*                                                                       */
/*      ZhideYang.      02-20-2003      Created initial version 1.0      */
/*                                                                       */
/************************CWAVE*************************************************/
INT CWMC_GetReadyWave( CWAVE * this, UNSIGNED uTimeOut )
{
    INT status;                         /* complete status */

    /* 打开录波设备 */
    OPENDRIVER( status, CWMC_GET_WAVE_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        if( this->WaveBusy == 0 )
        {
            /* ready fault number */
            this->ReadyFaultNumber = this->ElecNumber;
            this->ReadySerialNumber = this->FaultNumber;
        }

        /* 关闭录波设备 */
        CLOSEDRIVER( CWMC_GET_WAVE_HANDLE());
    }

    /* Return the completion status.  */
    return status;
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CWMC_SetWaveStatus                                               */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function set wave status.                                   */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*      NAME            DATE                    REMARKS                  */
/*                                                                       */
/*      ZhideYang.      02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
INT CWMC_SetWaveStatus( CWAVE * this, INT iStatus )
{
    /* 设置录波状态 */
    this->WaveBusy = iStatus;

    /* Return the completion status.  */
    return XJ_SUCCESS;
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CWMC_FindWaveFaultNumber                                         */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function find wave data.                                    */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*      NAME            DATE                    REMARKS                  */
/*                                                                       */
/*      ZhideYang.      02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
INT CWMC_FindWaveFaultNumber( CWAVE * this, INT iType, VOID * pBuffer,
    INT * Actual_Number, UNSIGNED uTimeOut )
{
    INT status;

    if( this -> InitFlag == 0 )
    {
        /* 初始化检查 */
        return( XJ_UNAVAILABLE );
    }

    //    if(( iType & ( WAVE_TYPE_START + WAVE_TYPE_SOF_TEST )) != 0 )
    //    {
    //        /* 查找启动或手动录波 */
    //        status = CWMC_FindRamWaveFaultNumber( iType, pBuffer, Actual_Number );
    //    }
    //    else
    {
        /* 查找故障录波 */
        /* 打开录波设备 */
        OPENDRIVER( status, CWMC_GET_WAVE_HANDLE(), uTimeOut );
        if( status == XJ_SUCCESS )
        {
            /* 获取录波序号 */
            STATUSDRIVER( status, CWMC_GET_WAVE_HANDLE(), pBuffer,( UNSIGNED *)Actual_Number, 0, uTimeOut );

            /* 关闭录波设备 */
            CLOSEDRIVER( CWMC_GET_WAVE_HANDLE());
        }
    }

    /* Return the completion status.  */
    return( status );
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CWMC_ReadRamWaveData                                             */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function read the wave data.                                */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*      NAME            DATE                    REMARKS                  */
/*                                                                       */
/*      ZhideYang.      07-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
/* 录波量值信息结构. */
typedef struct WAVE_VALUE_INFO_STRUCT
{
    /* 数据头部信息 */
    union VALUE_INFO_UNION
    {
        CWM_ANALOG analog;
        CWM_INPUT input;
        CWM_OUTPUT output;
        CWM_STATE state;
        CWM_LOGICAL logical;
    }Info;
}CWM_INFO;

/* 录波通道存储数据结构 */
typedef struct WAVE_CHANNEL_STRUCT
{
    /* 录波量值结构 */
    CWM_VHD head;  //量值头部信息
    UNSIGNED value[CWMC_MAX_WAVE_DATA_SIZE]; //量值数据
}CWM_CHANNEL;

/* 录波数据存储结构 */
typedef struct WAVE_DATA_STRUCT
{
    /* 数据头部信息 */
    union DATA_UNION
    {
        CWM_WHD head;
        CWM_CHANNEL channel;
    }data;
}CWM_DATA;

INT CWMC_ReadRamWaveData( UNSIGNED uFaultNumber, VOID * pBuffer, UNSIGNED uOffset,
    INT iDataType, UNSIGNED uNumber )
{
    INT status = XJ_UNAVAILABLE;
    CSF_NODE * search_ptr;
    CWM_WCB * pRecord,* pWave;
    CWM_INFO WaveInf;
    CWM_DATA WaveData;
    UNSIGNED j, k;
    UNSIGNED GroupNumber;
    UNSIGNED variable, uData;
    UNSIGNED RecordNumber;
    UNSIGNED number;
    SIGNED point_value;
    UNSIGNED WaveType;
    UNSIGNED * pSrc,* pDst,* p;
    CWM_ANALOG * pAnalog;
    CWM_INPUT * pInput;
    CWM_OUTPUT * pOutput;
    CWM_STATE * pState;
    CWM_LOGICAL * pLogical;

    pRecord = XJ_NULL;
    /* 锁录波记录 */
    CWMC_PROTECT_WAVE();
    search_ptr = CWave.WaveList;
    if( search_ptr != XJ_NULL )
    {
        /* 查找录波数据 */
        do
        {
            pWave = ( CWM_WCB *)search_ptr;
            if((( pWave->FaultNumber & CWMC_DATA_MASKS16()) == ( uFaultNumber & CWMC_DATA_MASKS16()))
                && ( pWave->EndFlag != 0 ))
            {
                pRecord = ( CWM_WCB *)search_ptr;
                break;
            }
            search_ptr = search_ptr -> next;
        }while( CWave.WaveList != search_ptr );
    }

    if( pRecord != XJ_NULL )
    {
        WaveType = pRecord->Type;
        RecordNumber = pRecord->RecordNumber;
        /* 读取录波数据 */
        if( iDataType != 0 )
        {
            /* 读取量值数据 */
            p = pRecord -> WaveMemory;
            p += (( iDataType - 1 ) * ( pRecord->TotalPointer ));
            pDst = ( UNSIGNED *)( WaveData.data.channel.value );
            for( k = 0; k < pRecord->RecordNumber; k ++)
            {
                * pDst ++ = * p ++;
            }

            /* 读取量值信息 */
            variable = ( UNSIGNED )iDataType;
            if( variable <= pRecord->AnalogNumber )
            {
                /* 模拟量 */
                pAnalog = pRecord->AnalogInf;
                pAnalog += ( variable - 1 );
                WaveInf.Info.analog.Number = pAnalog->Number;
                WaveInf.Info.analog.Inf = pAnalog->Inf;
                WaveInf.Info.analog.Type = pAnalog->Type;
                WaveInf.Info.analog.CalculateCoefficient = pAnalog->CalculateCoefficient;
                WaveInf.Info.analog.RateCoefficient = pAnalog->RateCoefficient;
                WaveInf.Info.analog.PrimaryRate = pAnalog->PrimaryRate;
                WaveInf.Info.analog.SecondaryRate = pAnalog->SecondaryRate;
                GroupNumber = 1;  //模拟量组号标识
                status = XJ_SUCCESS;
            }
            else if( variable <= ( pRecord->AnalogNumber + pRecord->InputNumber ))
            {
                /* 开入量 */
                pInput = pRecord->InputInf;
                pInput += ( variable - pRecord->AnalogNumber - 1 );
                WaveInf.Info.input.GroupNumber = pInput->GroupNumber;
                WaveInf.Info.input.MaskBits = pInput->MaskBits;
                GroupNumber = 2;  //开入量组号标识
                status = XJ_SUCCESS;
            }
            else if( variable <= ( pRecord->AnalogNumber + pRecord->InputNumber +
                pRecord->OutputNumber ))
            {
                /* 开出量 */
                pOutput = pRecord->OutputInf;
                pOutput += ( variable - pRecord->AnalogNumber - pRecord->InputNumber - 1 );
                WaveInf.Info.output.GroupNumber = pOutput->GroupNumber;
                WaveInf.Info.output.MaskBits = pOutput->MaskBits;
                GroupNumber = 3;  //开出量组号标识
                status = XJ_SUCCESS;
            }
            else if( variable <= ( pRecord->AnalogNumber + pRecord->InputNumber +
                pRecord->OutputNumber + pRecord->StateNumber ))
            {
                /* 状态量 */
                pState = pRecord->StateInf;
                pState += ( variable - pRecord->AnalogNumber - pRecord->InputNumber - pRecord->OutputNumber - 1 );
                WaveInf.Info.state.GroupNumber = pState->GroupNumber;
                WaveInf.Info.state.MaskBits = pState->MaskBits;
                GroupNumber = 4;  //状态量组号标识
                status = XJ_SUCCESS;
            }
            else if( variable <= ( pRecord->AnalogNumber + pRecord->InputNumber +
                pRecord->OutputNumber + pRecord->StateNumber + pRecord->LogicNumber ))
            {
                /* 逻辑状态量 */
                pLogical = pRecord->LogicalInf;
                pLogical += ( variable - pRecord->AnalogNumber - pRecord->InputNumber -
                    pRecord->OutputNumber - pRecord->StateNumber - 1 );
                WaveInf.Info.logical.GroupNumber = pLogical->GroupNumber;
                WaveInf.Info.logical.MaskBits = pLogical->MaskBits;
                GroupNumber = 5;  //逻辑状态量组号标识
                status = XJ_SUCCESS;
            }
            else
            {
                status = XJ_UNAVAILABLE;
            }
        }
        else
        {
            /* 读取录波头部数据 */
            CWMC_GetWaveHead( pRecord,&( WaveData.data.head ));
            GroupNumber = 0;
            status = XJ_SUCCESS;
        }
    }
    else
    {
        /* 无录波数据 */
        status = XJ_UNAVAILABLE;
    }
    CWMC_UNPROTECT_WAVE();

    if( status == XJ_SUCCESS )
    {
        /* 转换录波数据 */
        pDst = ( UNSIGNED *)pBuffer;
        if( GroupNumber == 0 )
        {
            /* 录波头部数据 */
            pSrc = ( UNSIGNED *)&( WaveData.data.head );
            pSrc += uOffset;
            for( k = 0; k < uNumber; k ++)
            {
                * pDst ++ = * pSrc ++;
            }
        }
        /* 模拟量数据 */
        else if( GroupNumber == 1 )
        {
            /* 模拟量头部信息 */
            WaveData.data.channel.head.head_info.analog.Inf = WaveInf.Info.analog.Inf;
            WaveData.data.channel.head.head_info.analog.Primary_rate = WaveInf.Info.analog.PrimaryRate;
            WaveData.data.channel.head.head_info.analog.Secondary_rate = WaveInf.Info.analog.SecondaryRate;
            WaveData.data.channel.head.head_info.analog.Coefficient =
                WaveInf.Info.analog.CalculateCoefficient / WaveInf.Info.analog.RateCoefficient;
            if((( WaveType & WAVE_TYPE_COEFFECT ) == 0 )
                && (( WaveInf.Info.analog.Type & DC_COMPONENT ) == 0 ))
            {
                /* 瞬时值交流量计算系数除以2的开方 */
                WaveData.data.channel.head.head_info.analog.Coefficient *= (( float )0.707107 );
            }

            /* 模拟量数据 */
            pSrc = ( UNSIGNED *)WaveData.data.channel.value;
            pDst = ( UNSIGNED *)WaveData.data.channel.value;
            number = RecordNumber / CWMC_ANALOG_BANK_WIDTH();
            for( j = 0; j < number; j ++)
            {
                uData = 0;
                for( k = 0; k < CWMC_ANALOG_BANK_WIDTH(); k ++)
                {
                    point_value = ( SIGNED )(* pSrc ++);
                    if( point_value > CWMC_MAX_ANALOG_VALUE())
                    {
                        /* 上限检查 */
                        variable = 0x7fff;
                    }
                    else if( point_value < CWMC_MIN_ANALOG_VALUE())
                    {
                        /* 下限检查 */
                        variable = 0x8001;
                    }
                    else
                    {
                        variable = ( UNSIGNED )point_value;
                        variable &= CWMC_ANALOG_BANK_MASKS();
                    }
                    uData += ( variable ) << ( k * CWMC_ANALOG_BANK_BITS());
                }
                * pDst ++ = uData;
            }

            number = RecordNumber % CWMC_ANALOG_BANK_WIDTH();
            if( number != 0 )
            {
                point_value = ( SIGNED )(* pSrc ++);
                if( point_value > CWMC_MAX_ANALOG_VALUE())
                {
                    /* 上限检查 */
                    variable = 0x7fff;
                }
                else if( point_value < CWMC_MIN_ANALOG_VALUE())
                {
                    /* 下限检查 */
                    variable = 0x8001;
                }
                else
                {
                    variable = ( UNSIGNED )point_value;
                    variable &= CWMC_ANALOG_BANK_MASKS();
                }
                * pDst ++ = variable;
            }
            * pDst ++ = CWM_END_FLAG;

            /* 读取模拟量数据 */
            pDst = ( UNSIGNED *)pBuffer;
            pSrc = ( UNSIGNED *)&( WaveData.data.channel );
            pSrc += uOffset;
            for( k = 0; k < uNumber; k ++)
            {
                * pDst ++ = * pSrc ++;
            }
        }
        /* 开入量数据 */
        else if( GroupNumber == 2 )
        {
            /* 开入量头部信息 */
            WaveData.data.channel.head.head_info.input.GroupNumber = WaveInf.Info.input.GroupNumber;
            WaveData.data.channel.head.head_info.input.MaskBits = WaveInf.Info.input.MaskBits;

            /* 开入量数据 */
            pSrc = ( UNSIGNED *)WaveData.data.channel.value;
            pDst = ( UNSIGNED *)WaveData.data.channel.value;
            for( k = 0; k < RecordNumber; k ++)
            {
                * pDst ++ = * pSrc ++;
            }
            * pDst ++ = CWM_END_FLAG;

            /* 读取开入量数据 */
            pDst = ( UNSIGNED *)pBuffer;
            pSrc = ( UNSIGNED *)&( WaveData.data.channel );
            pSrc += uOffset;
            for( k = 0; k < uNumber; k ++)
            {
                * pDst ++ = * pSrc ++;
            }
        }
        /* 开出量数据 */
        else if( GroupNumber == 3 )
        {
            /* 开出量头部信息 */
            WaveData.data.channel.head.head_info.output.GroupNumber = WaveInf.Info.output.GroupNumber;
            WaveData.data.channel.head.head_info.output.MaskBits = WaveInf.Info.output.MaskBits;

            /* 开出量数据 */
            pSrc = ( UNSIGNED *)WaveData.data.channel.value;
            pDst = ( UNSIGNED *)WaveData.data.channel.value;
            for( k = 0; k < RecordNumber; k ++)
            {
                * pDst ++ = * pSrc ++;
            }
            * pDst ++ = CWM_END_FLAG;

            /* 读取开出量数据 */
            pDst = ( UNSIGNED *)pBuffer;
            pSrc = ( UNSIGNED *)&( WaveData.data.channel );
            pSrc += uOffset;
            for( k = 0; k < uNumber; k ++)
            {
                * pDst ++ = * pSrc ++;
            }
        }
        /* 状态量数据 */
        else if( GroupNumber == 4 )
        {
            /* 状态量头部信息 */
            WaveData.data.channel.head.head_info.state.GroupNumber = WaveInf.Info.state.GroupNumber;
            WaveData.data.channel.head.head_info.state.MaskBits = WaveInf.Info.state.MaskBits;

            /* 状态量数据 */
            pSrc = ( UNSIGNED *)WaveData.data.channel.value;
            pDst = ( UNSIGNED *)WaveData.data.channel.value;
            for( k = 0; k < RecordNumber; k ++)
            {
                * pDst ++ = * pSrc ++;
            }
            * pDst ++ = CWM_END_FLAG;

            /* 读取状态量数据 */
            pDst = ( UNSIGNED *)pBuffer;
            pSrc = ( UNSIGNED *)&( WaveData.data.channel );
            pSrc += uOffset;
            for( k = 0; k < uNumber; k ++)
            {
                * pDst ++ = * pSrc ++;
            }
        }
        /* 逻辑状态量数据 */
        else if( GroupNumber == 5 )
        {
            /* 逻辑量头部信息 */
            WaveData.data.channel.head.head_info.logical.GroupNumber = WaveInf.Info.logical.GroupNumber;
            WaveData.data.channel.head.head_info.logical.MaskBits = WaveInf.Info.logical.MaskBits;

            /* 逻辑量数据 */
            pSrc = ( UNSIGNED *)WaveData.data.channel.value;
            pDst = ( UNSIGNED *)WaveData.data.channel.value;
            for( k = 0; k < RecordNumber; k ++)
            {
                * pDst ++ = * pSrc ++;
            }
            * pDst ++ = CWM_END_FLAG;

            /* 读取逻辑量数据 */
            pDst = ( UNSIGNED *)pBuffer;
            pSrc = ( UNSIGNED *)&( WaveData.data.channel );
            pSrc += uOffset;
            for( k = 0; k < uNumber; k ++)
            {
                * pDst ++ = * pSrc ++;
            }
        }
    }

    /* Return the completion status.  */
    return( status );
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CWMC_FindRamWaveFaultNumber                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function find wave data.                                    */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*      NAME            DATE                    REMARKS                  */
/*                                                                       */
/*      ZhideYang.      02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
INT CWMC_FindRamWaveFaultNumber( INT iType, VOID * pBuffer, INT * Actual_Number )
{
    INT status = XJ_SUCCESS;
    CSF_NODE * search_ptr;
    CWM_WCB * pWave;
    UNSIGNED i;
    UNSIGNED * pSrc,* pDst;
    UNSIGNED uTotalNumber = 0;
    UNSIGNED variable;

    pDst = ( UNSIGNED *)pBuffer;
    /* 锁录波记录 */
    CWMC_PROTECT_WAVE();
    search_ptr = CWave.WaveList;
    if( search_ptr != XJ_NULL )
    {
        /* 查找录波数据 */
        do
        {
            pWave = ( CWM_WCB *)search_ptr;
            if((( pWave->Type & iType ) != 0 ) && ( pWave->EndFlag != 0 ))
            {
                variable = pWave->FaultNumber & CWMC_DATA_MASKS16();
                variable += (( pWave->ElecNumber & CWMC_DATA_MASKS16()) << CWMC_WAVE_FAULT_BITS());
                if( uTotalNumber >= CWMC_MAX_NVRAM_WAVE )
                {
                    /* 读取个数满,保留最新序号 */
                    pDst = ( UNSIGNED *)pBuffer;
                    pSrc = pDst;
                    pSrc ++;
                    for( i = 0; i < ( CWMC_MAX_NVRAM_WAVE - 1 ); i ++)
                    {
                        * pDst ++ = * pSrc ++;
                    }
                    * pDst = variable;
                }
                else
                {
                    /* 读取个数未到,顺序存放 */
                    * pDst ++ = variable;
                    uTotalNumber ++;
                }
            }
            search_ptr = search_ptr -> next;
        }while( CWave.WaveList != search_ptr );
    }
    CWMC_UNPROTECT_WAVE();

    * Actual_Number = uTotalNumber;

    /* Return the completion status.  */
    return( status );
}

/* 读取录波数据块 */
INT CWMC_GetWaveDataBlock( CHAR * fileName, UNSIGNED Offset, UNSIGNED * dst, UNSIGNED size )
{
    INT status;
    FILE * fp;
    UNSIGNED readSize;

    status = XJ_UNAVAILABLE;
    /* 读录波数据 */
    fp = xj_fopen( fileName, "rb" );
    if( fp != XJ_NULL )
    {
        /* 设置文件位置 */
        if( xj_fseek( fp, Offset, 0 ))
        {
            /* 随机读写失败 */
            CWMC_ErrInf.fseekFile ++;
        }
        else
        {
            /* 读取录波数据 */
            readSize = ( UNSIGNED )xj_fread( dst, 1, size, fp );
            if( readSize == size )
            {
                status = XJ_SUCCESS;
            }
            else
            {
                /* 读取文件数据失败 */
                CWMC_ErrInf.readFile ++;
            }
        }
        if( xj_fclose( fp ))
        {
            /* 关闭文件失败 */
            CWMC_ErrInf.closeFileR ++;
        }
    }
    else
    {
        CWMC_ErrInf.openFileR ++;
    }

    return( status );
}

/* 写录波数据 */
INT CWMC_WriteWaveDataBlock( CHAR * fileName, UNSIGNED * src, UNSIGNED size )
{
    INT status;
    FILE * fp;
    UNSIGNED writeSize;

    status = XJ_UNAVAILABLE;
    /* 写录波数据 */
    fp = xj_fopen( fileName, "wb" );
    if( fp != XJ_NULL )
    {
        /* 写录波数据 */
        writeSize = ( UNSIGNED )xj_fwrite( src, sizeof( UNSIGNED ), size, fp );
        if( xj_fclose( fp ))
        {
            /* 关闭文件失败 */
            CWMC_ErrInf.closeFileW ++;
        }
        if( writeSize == size )
        {
            status = XJ_SUCCESS;
        }
        else
        {
            /* 写文件数据失败 */
            CWMC_ErrInf.writeFile ++;
            if( xj_RemoveFile( fileName ))
            {
                /* 删除文件失败 */
                CWMC_ErrInf.deleteFileW ++;
            }
        }
    }
    else
    {
        CWMC_ErrInf.openFileW ++;
    }

    return( status );
}

/* 录波故障序号回调函数参数定义 */
#define CWM_LIST_PARA_NUM       2       /* 参数个数 */
#define CWM_LIST_O_PARA_NUM     0       /* 参数个数位置偏移 */
#define CWM_LIST_O_DIR_NAME     1       /* 录波路径名称位置偏移 */
/* 获取录波索引号(电网故障序号+故障序号)回调函数 */
int CWMC_ListWaveIndexCallBack( struct FileInformation * info, void * context )
{
    INT status;
    UNSIGNED variable;
    CHAR * dirName;
    CHAR * IndexStr;
    UNSIGNED Index;
    UNSIGNED * p;
    CHAR buffer[64]; /* 完整路径名（包括'/'） */

    /* 获取录波故障序号字符串 */
    p = ( UNSIGNED *)context;
    dirName = ( CHAR *)(*( p + CWM_LIST_O_DIR_NAME )); /* 录波路径名称 */
    sprintf( buffer, "%s/", dirName );
    IndexStr = XJP_CSC_Strcmp( info->Name, buffer );
    Index = ( UNSIGNED )( atol( IndexStr ));
    if( Index == 0 )
    {
        return( XJ_UNAVAILABLE );
    }
    /* 写索引表数据 */
    XJP_Index_Write( CWMC_WaveHandle, Index, & variable, & status );
    if( status == XJ_TRUE )
    {
        /* 超过最大支持数目，删除录波文件 */
        sprintf( buffer, "%s/%d", dirName, variable );
        if( xj_RemoveFile( buffer ))
        {
            /* 删除文件失败 */
            CWMC_ErrInf.deleteFileL ++;
            /* 增加删除文件列表 */
            CFDC_Delete_File( buffer );
        }
    }
    return( XJ_UNAVAILABLE );
}

/* 获取录波索引号(电网故障序号+故障序号) */
INT CWMC_ListWaveIndex( CHAR * dirName, UNSIGNED * Dst,
    UNSIGNED * pActual_size, UNSIGNED Number )
{
    INT status;
    INT ActualNumber;
    UNSIGNED FuncPara[CWM_LIST_PARA_NUM]; /* 回调函数参数 */

    /* 清除数据 */
    XJP_Index_Clear( CWMC_WaveHandle );
    /* 数据参数列表 */
    FuncPara[CWM_LIST_O_PARA_NUM] = CWM_LIST_PARA_NUM; /* 参数个数 */
    FuncPara[CWM_LIST_O_DIR_NAME] = ( UNSIGNED )dirName; /* 录波路径名称 */
    ActualNumber = CWMC_LISTFILE( dirName, CWMC_ListWaveIndexCallBack, FuncPara );
    /* 获取指定数据个数的索引表数据(从最新开始) */
    status = XJP_Index_List( CWMC_WaveHandle, Dst, pActual_size, Number );

    return( status );
}

/* 获取录波电网故障序号(故障序号) */
INT CWMC_GetWaveFaultNumber( CHAR * dirName, UNSIGNED * Dst,
    UNSIGNED * pActual_size, INT type, UNSIGNED Number )
{
    INT status;

    if( type == 0 )
    {
        /* 获取指定数据个数的索引表(录波电网故障序号+故障序号)数据(从最新开始) */
        status = XJP_Index_List( CWMC_WaveHandle, Dst, pActual_size, Number );
    }
    else
    {
        /* 获取指定数据个数的索引表(录波电网故障序号)数据(从最新开始) */
        status = XJP_Index_ListH( CWMC_WaveHandle, Dst, pActual_size, Number );
    }

    return( status );
}

/* 查找指定故障序号的录波索引序号 */
INT CWMC_FindWaveIndex( CHAR * dirName, UNSIGNED * waveIndex, UNSIGNED faultIndex )
{
    UNSIGNED Number;
    UNSIGNED ActualNumber;
    UNSIGNED CondMasks;
    UNSIGNED CondValue;
    INT status;

    /* 读取指定故障序号的索引表数据 */
    Number = 1; /* 数据个数 */
    CondMasks = CWMC_DATA_MASKS16();
    CondValue = faultIndex & CWMC_DATA_MASKS16();
    status = XJP_Index_Read( CWMC_WaveHandle, waveIndex, & ActualNumber, Number, CondMasks, CondValue );
    if(( status == XJ_SUCCESS ) && ( ActualNumber != 0 ))
    {
        return( XJ_SUCCESS );
    }
    else
    {
        return( XJ_UNAVAILABLE );
    }
}

/* 删除录波目录全部文件回调函数参数定义 */
#define CWM_DEL_F_PARA_NUM      2       /* 参数个数 */
#define CWM_DEL_F_O_PARA_NUM    0       /* 参数个数位置偏移 */
#define CWM_DEL_F_O_ACT_NUM     1       /* 删除文件个数 */
/* 删除录波指定目录文件回调函数 */
int CWMC_DeleteDirFileCallBack( struct FileInformation * info, void * context )
{
    UNSIGNED * p;
    UNSIGNED actDelFileNumber;

    /* 删除录波文件 */
    if( xj_RemoveFile( info->Name ) == 0 )
    {
        /* 删除成功 */
        p = ( UNSIGNED *)context;
        actDelFileNumber = *( p + CWM_DEL_F_O_ACT_NUM ); /* 删除的录波文件个数 */
        *( p + CWM_DEL_F_O_ACT_NUM ) = (++ actDelFileNumber );
    }
    else
    {
        /* 删除目录文件失败 */
        CWMC_ErrInf.deleteDirFile ++;
        /* 增加删除文件列表 */
        CFDC_Delete_File( info->Name );
    }
    if( CWave.hWatchDogId )
    {
        XJP_Watchdog_Reset( CWave.hWatchDogId ); /* 复位被监视对象 */
    }
    return( XJ_UNAVAILABLE );
}

/* 删除指定目录下的所有文件 */
INT CWMC_DeleteDirFile( CHAR * dirName )
{
    UNSIGNED ActualNumber;
    UNSIGNED FuncPara[CWM_DEL_F_PARA_NUM]; /* 回调函数参数 */

    /* 删除录波文件的回调参数 */
    FuncPara[CWM_DEL_F_O_PARA_NUM] = CWM_DEL_F_PARA_NUM; /* 参数个数 */
    FuncPara[CWM_DEL_F_O_ACT_NUM] = 0; /* 删除录波文件的个数 */
    ActualNumber = ( UNSIGNED )CWMC_LISTFILE( dirName, CWMC_DeleteDirFileCallBack, FuncPara );
    /* 清除录波索引表数据 */
    XJP_Index_Clear( CWMC_WaveHandle );

    if( ActualNumber == FuncPara[CWM_DEL_F_O_ACT_NUM] )
    {
        return( XJ_SUCCESS );
    }
    else
    {
        return( XJ_UNAVAILABLE );
    }
}

/* 录波目录文件数目检查回调函数参数定义 */
#define CWM_FULL_F_PARA_NUM     6       /* 参数个数 */
#define CWM_FULL_F_O_PARA_NUM   0       /* 参数个数位置偏移 */
#define CWM_FULL_F_O_DIR_NAME   1       /* 路径名称位置偏移 */
#define CWM_FULL_F_O_REQ_NUM    2       /* 请求的最大数据个数位置偏移 */
#define CWM_FULL_F_O_ACT_NUM    3       /* 实际数据个数位置偏移 */
#define CWM_FULL_F_O_DST        4       /* 缓冲区地址位置偏移 */
#define CWM_FULL_F_O_DEL_NUM    5       /* 删除文件个数位置偏移 */
/* 录波目录文件数目检查回调函数 */
int CWMC_FullFileCheckCallBack( struct FileInformation * info, void * context )
{
    INT status;
    CHAR * dirName;
    CHAR * IndexStr;
    UNSIGNED Index;
    UNSIGNED reqDataNumber;
    UNSIGNED InvalidData;
    UNSIGNED actDelFileNumber;
    UNSIGNED * pDst;
    UNSIGNED * p;
    CHAR buffer[64]; /* 完整路径名（包括'/'） */

    /* 获取序号字符串 */
    p = ( UNSIGNED *)context;
    dirName = ( CHAR *)(*( p + CWM_FULL_F_O_DIR_NAME )); /* 路径名称 */
    reqDataNumber = *( p + CWM_FULL_F_O_REQ_NUM ); /* 请求的数据个数 */
    pDst = ( UNSIGNED *)(*( p + CWM_FULL_F_O_DST )); /* 缓冲区地址 */
    sprintf( buffer, "%s/", dirName );
    IndexStr = XJP_CSC_Strcmp( info->Name, buffer );
    Index = ( UNSIGNED )( atol( IndexStr ));
    if( Index == 0 )
    {
        return( XJ_UNAVAILABLE );
    }
    /* 数据升序排列 */
    status = XJP_CSC_Arrage_Insert_Full( pDst, ( p + CWM_FULL_F_O_ACT_NUM ),
        reqDataNumber, Index, & InvalidData, 1 );
    if( status == XJ_TRUE )
    {
        /* 文件满 */
        sprintf( buffer, "%s/%d", dirName, InvalidData );
        /* 删除旧文件 */
        if( xj_RemoveFile( buffer ) == 0 )
        {
            /* 删除成功 */
            p = ( UNSIGNED *)context;
            actDelFileNumber = *( p + CWM_FULL_F_O_DEL_NUM ); /* 删除的文件个数 */
            *( p + CWM_FULL_F_O_DEL_NUM ) = (++ actDelFileNumber );
        }
        else
        {
            /* 删除检查无效文件失败 */
            CWMC_ErrInf.deleteCheckFile ++;
            /* 增加删除文件列表 */
            CFDC_Delete_File( buffer );
        }

        /* 删除录波索引表数据 */
        XJP_Index_Delete( CWMC_WaveHandle, InvalidData );
    }
    return( XJ_UNAVAILABLE );
}

/* 录波目录文件数目检查 */
INT CWMC_FullFileCheck( CHAR * dirName, UNSIGNED * Dst, UNSIGNED * pDeleteNumber, UNSIGNED MaxNumber )
{
    INT ActualNumber;
    UNSIGNED FuncPara[CWM_FULL_F_PARA_NUM]; /* 回调函数参数 */

    /* 数据参数列表 */
    FuncPara[CWM_FULL_F_O_PARA_NUM] = CWM_FULL_F_PARA_NUM; /* 参数个数 */
    FuncPara[CWM_FULL_F_O_DIR_NAME] = ( UNSIGNED )dirName; /* 路径名称 */
    FuncPara[CWM_FULL_F_O_REQ_NUM] = MaxNumber; /* 请求的最大数据个数 */
    FuncPara[CWM_FULL_F_O_ACT_NUM] = 0; /* 实际数据个数 */
    FuncPara[CWM_FULL_F_O_DST] = ( UNSIGNED )Dst; /* 缓冲区地址 */
    FuncPara[CWM_FULL_F_O_DEL_NUM] = 0; /* 删除文件个数 */
    ActualNumber = CWMC_LISTFILE( dirName, CWMC_FullFileCheckCallBack, FuncPara );

    if( ActualNumber >= 0 )
    {
        * pDeleteNumber = FuncPara[CWM_FULL_F_O_DEL_NUM];
        return( XJ_SUCCESS );
    }
    else
    {
        * pDeleteNumber = 0;
        return( XJ_UNAVAILABLE );
    }
}

/* 录波目录文件检查主函数 */
static UNSIGNED CWMC_MaxWaveFileIndex[CWMC_MAX_WAVE_FILE_NUMBER];
INT CWMC_WaveFullFileCheckMain( VOID )
{
    INT status;
    UNSIGNED delNumber;

    CWMC_FullFileCheck( CWave.dirName, CWMC_MaxWaveFileIndex, & delNumber, CWMC_MAX_WAVE_FILE_NUMBER );
    if( delNumber > 0 )
    {
        status = XJ_TRUE;
    }
    else
    {
        status = XJ_FALSE;
    }

    return( status );
}

static UNSIGNED CWMC_WaveIndex[CWMC_MAX_NVRAM_WAVE];
VOID CWMC_WaveDriverEntry( XJ_DRIVER hDriver, XJ_DRIVER_REQUEST * pRequest )
{
    INT status;
    UNSIGNED k;
    UNSIGNED index;
    UNSIGNED offset;
    UNSIGNED type;
    UNSIGNED Number;
    UNSIGNED variable;
    UNSIGNED TotalPoint;
    UNSIGNED AnalogNumber;
    UNSIGNED startOffset;
    UNSIGNED size;
    UNSIGNED * destination;
    UNSIGNED * p;
    struct FileInformation fileInfo;
    CHAR fileName[128]; /* 完整路径名（包括'/'） */
    CHAR tempFileName[128]; /* 临时文件名 */
    CWM_WHD waveHead;
    UNSIGNED uStartIndex;  /* 一次故障录波的起始录波索引号 */
    UNSIGNED uMaskBits;
    UNSIGNED uCheckData;

    switch( pRequest -> nu_function )
    {
        /* 初始化 */
    case XJ_INITIALIZE:
        /* 初始化录波信息 */
        CWave.FaultNumber = 0;
        CWave.ElecNumber = 0;
        CWave.UserFaultNumber = CWave.FaultNumber;
        CWave.UserElecNumber = CWave.ElecNumber;
        CWave.ReadyFaultNumber = CWave.ElecNumber;
        CWave.ReadySerialNumber = CWave.FaultNumber;

        /* 清除录波数据 */
        if( pRequest -> nu_supplemental != 0 )
        {
            status = CWMC_DeleteDirFile( CWave.dirName );
            if( status == XJ_SUCCESS )
            {
                pRequest -> nu_status = XJ_SUCCESS;
            }
            else
            {
                pRequest -> nu_status = XJ_UNAVAILABLE;
            }
        }
        else
        {
            /* 获取录波电网故障序号和录波故障序号 */
            CWMC_ListWaveIndex( CWave.dirName, CWMC_WaveIndex,& Number, CWMC_MAX_NVRAM_WAVE );
            if( Number > 0 )
            {
                variable = CWMC_WaveIndex[( Number - 1 )]; /* 最新录波电网故障序号和录波序号 */
                CWave.FaultNumber = variable & CWMC_DATA_MASKS16(); /* 最新录波序号 */
                CWave.UserFaultNumber = CWave.FaultNumber;
                CWave.ElecNumber = ( variable >> CWMC_DATA_BITS16() ) & CWMC_DATA_MASKS16(); /* 最新电网故障序号 */
                CWave.UserElecNumber = CWave.ElecNumber;
                CWave.ReadyFaultNumber = CWave.ElecNumber;
                CWave.ReadySerialNumber = CWave.FaultNumber;
                variable &= CWMC_DATA_MASKSH16(); /* 获取高16位数据 */
                uStartIndex = CWave.ReadySerialNumber;
                /* 查找一次电网故障起始录波索引号 */
                for( k = 1; k < Number; k ++)
                {
                    if(( CWMC_WaveIndex[( Number - 1 - k )] & CWMC_DATA_MASKSH16() ) == variable )
                    {
                        uStartIndex = ( CWMC_WaveIndex[( Number - 1 - k )] & CWMC_DATA_MASKS16() );
                    }
                }
                CWMC_InitWaveRcd( CWave.ReadySerialNumber, uStartIndex, CWave.ElecNumber );
            }
            pRequest -> nu_status = XJ_SUCCESS;
        }

        /* 录波记录信息初始化 */
        if( CWave.WaveList != 0 )
        {
            CWMC_PROTECT_WAVE();
            /* 清空录波记录指针 */
            WavePointer = XJ_NULL;
            for( k = 0; k < CWM_MAX_WAVE_POINTER; k ++ )
            {
                WavePointerCB[ k ] = XJ_NULL;
            }
            do
            {
                /* 释放空间 */
                CWMC_WAVE_DELLOCATE( status, CWave.WaveList );
                /* 删除链表 */
                CWMC_REMOVE_WAVE_LIST( CWave.WaveList );

                if( CWave.WaveList == 0 )
                {
                    break;
                }
            }while( 1 );

            CWMC_UNPROTECT_WAVE();
        }
        break;

        /* 获取录波控制权 */
    case XJ_ASSIGN:
        CWMC_LOCK_WAVE( pRequest -> nu_status, pRequest -> nu_timeout );
        break;

        /* 释放录波控制权 */
    case XJ_RELEASE:
        CWMC_UNLOCK_WAVE( pRequest -> nu_status );
        break;

        /* 读录波数据 */
    case XJ_INPUT:
        destination = ( UNSIGNED *)( pRequest ->
            nu_request_info.nu_input.nu_buffer_ptr );
        index = ( pRequest->nu_request_info.nu_input.nu_logical_unit ) & CWMC_DATA_MASKS16();
        size = pRequest -> nu_request_info.nu_input.nu_request_size;
        offset = pRequest -> nu_request_info.nu_input.nu_offset;
        type = pRequest -> nu_supplemental;
        /* 获取录波序号(故障序号(高16位)+录波索引号(低16位)) */
        if( CWMC_FindWaveIndex( CWave.dirName, & variable, index ) != XJ_SUCCESS )
        {
            /* 获取录波序号失败 */
            pRequest -> nu_status = XJ_END_OF_LOG;
            return;
        }
        sprintf( fileName, "%s/%d", CWave.dirName, variable );
        if( xj_GetFileInformation( fileName,& fileInfo ) == XJ_SUCCESS )
        {
            if( fileInfo.Size < sizeof( CWM_WHD ))
            {
                /* 文件大小不正确，直接返回失败 */
                pRequest -> nu_status = XJ_END_OF_LOG;
                return;
            }
            /* 读取录波数据头(Index-故障序号(高16位)+录波索引号(低16位)) */
            if( CWMC_GetWaveDataBlock( fileName, 0,( UNSIGNED *)& waveHead, sizeof( CWM_WHD )) == XJ_SUCCESS )
            {
                startOffset = 0;
                /* 录波偏移计算 */
                if( type != 0 )
                {
                    TotalPoint = waveHead.TotalPointer; /* 总点数 */
                    AnalogNumber = waveHead.AnalogNumber; /* 模拟量通道数 */
                    if( type <= AnalogNumber )
                    {
                        /* 读模拟量 */
                        variable = ( TotalPoint + CWMC_ANALOG_BANK_WIDTH()- 1 )/ CWMC_ANALOG_BANK_WIDTH();
                        variable += (( sizeof( CWM_VHD )+ sizeof( UNSIGNED ))/ sizeof( UNSIGNED ));
                        startOffset = sizeof( CWM_WHD )/ sizeof( UNSIGNED );
                        startOffset += (( type - 1 )* variable );
                    }
                    else
                    {
                        /* 计算模拟量位置 */
                        variable = ( TotalPoint + CWMC_ANALOG_BANK_WIDTH()- 1 )/ CWMC_ANALOG_BANK_WIDTH();
                        variable += (( sizeof( CWM_VHD )+ sizeof( UNSIGNED ))/ sizeof( UNSIGNED ));
                        startOffset = sizeof( CWM_WHD )/ sizeof( UNSIGNED );
                        startOffset += ( AnalogNumber * variable );
                        /* 计算实际位置 */
                        variable = TotalPoint +(( sizeof( CWM_VHD )+ sizeof( UNSIGNED ))/ sizeof( UNSIGNED ));
                        startOffset += (( type - AnalogNumber - 1 )* variable );
                    }
                }
                startOffset += offset;
                startOffset *= sizeof( UNSIGNED );
                /* 读录波数据块 */
                if( CWMC_GetWaveDataBlock( fileName, startOffset, destination, size * sizeof( UNSIGNED ))
                    == XJ_SUCCESS )
                {
                    pRequest -> nu_status = XJ_SUCCESS;
                }
                else
                {
                    pRequest -> nu_status = XJ_END_OF_LOG;
                }
            }
            else
            {
                /* 录波头信息获取不成功，设置失败状态 */
                pRequest -> nu_status = XJ_END_OF_LOG;
            }
        }
        else
        {
            /* 获取信息失败 */
            CWMC_ErrInf.getInfoFile ++;
            pRequest -> nu_status = XJ_END_OF_LOG;
        }
        break;

        /* 写录波数据 */
    case XJ_OUTPUT:
        p = ( UNSIGNED *)( pRequest->nu_request_info.nu_output.nu_buffer_ptr );
        size = pRequest -> nu_request_info.nu_output.nu_request_size;
        index = CWave.FaultNumber1 & CWMC_DATA_MASKS16(); /* 低16位:录波故障序号 */
        index += (( CWave.ElecNumber1 & CWMC_DATA_MASKS16() ) << CWMC_DATA_BITS16() ); /* 高16位:电网故障序号 */
        /* 写录波数据 */
        sprintf( fileName, "%s/%d", CWave.dirName, index );
        sprintf( tempFileName, "%s/tempWave", CWave.dirName );
        if( CWMC_WriteWaveDataBlock( tempFileName, p, size ) == XJ_SUCCESS )
        {
            if( xj_RenameFile( tempFileName, fileName )== XJ_SUCCESS )
            {
                /* 写录波成功,更新故障信息 */
                CWave.FaultNumber = CWave.FaultNumber1;
                if( CWave.ElecNumber1 )
                {
                    CWave.ElecNumber = CWave.ElecNumber1;
                }
                /* 更新索引表数据 */
                XJP_Index_Write( CWMC_WaveHandle, index, & variable, & status );
                if( status == XJ_TRUE )
                {
                    /* 超过最大支持数目，删除录波文件 */
                    sprintf( fileName, "%s/%d", CWave.dirName, variable );
                    if( xj_RemoveFile( fileName ))
                    {
                        /* 删除文件失败 */
                        CWMC_ErrInf.deleteFullFile ++;
                        /* 增加删除文件列表 */
                        CFDC_Delete_File( fileName );
                    }
                }
                pRequest -> nu_status = XJ_SUCCESS;
            }
            else
            {
                /* 重命名文件名失败 */
                CWMC_ErrInf.renameFile ++;
                pRequest -> nu_status = XJ_UNAVAILABLE;
            }
        }
        else
        {
            /* 写录波失败 */
            CWMC_ErrInf.writeWave ++;
            pRequest -> nu_status = XJ_UNAVAILABLE;
        }
        break;

    case XJ_STATUS:
        if( CWave.ReadyFaultNumber == 0 )
        {
            pRequest->nu_request_info.nu_status.nu_logical_unit = 0;
            pRequest -> nu_status = XJ_SUCCESS;
        }
        else
        {
            destination = ( UNSIGNED *)( pRequest->nu_request_info.nu_status.nu_extra_status );
            status = CWMC_GetWaveFaultNumber( CWave.dirName, destination,& size,
                ( INT )( pRequest -> nu_supplemental ), CWMC_MAX_NVRAM_WAVE );

            /* 判断检查数据的有效位数 */
            if( pRequest -> nu_supplemental )
            {
                uMaskBits = CPC_DATA_MASKS16;
                uCheckData = ( CWave.ReadyFaultNumber );
            }
            else
            {
                uMaskBits = CPC_DATA_MASKSH16;
                uCheckData = (( CWave.ReadyFaultNumber ) << CPC_DATA_BITS16 );
            }
            /* 检查有效数据个数 */
            size = XJP_CSC_Get_Valid_Number( destination, size, uCheckData, 1, uMaskBits );
            pRequest->nu_request_info.nu_status.nu_logical_unit = size;
            pRequest -> nu_status = status;
        }
        break;
    }
}

/* 启动录波(多段) */
INT CWMC_WaveStartMul( INT waveSegments, INT iType, INT * spacePtr, INT iStartPoint,
    INT * totalPointPtr, INT iElecNumber, WAVE_CHANNEL * WaveChannel, TIME * pTime,
    STATE * pState, INT iState )
{
    UNSIGNED i;
    INT status;
    INT newFault;
    INT iSpace;
    INT iTotalPoint;
    INT iWaveNumber;
    INT iWaveType;
    INT iCtrlType;
    CWM_WCB * pWave;
    CSF_NODE * search_ptr;
    UNSIGNED LastPriority, NextPriority;

    /* 检查是否有多段录波数据在排队等待记录 */
    for( i = 1; i < CWM_MAX_WAVE_POINTER; i ++ )
    {
        if( WavePointerCB[ i ] )
        {
            /* 多段录波等待记录，禁止再次触发，直接返回失败 */
            return( XJ_UNAVAILABLE );
        }
    }
    iSpace = * spacePtr;
    /* 新故障录波检查 */
    newFault = XJ_TRUE;
    pWave = WavePointer;
    if( pWave != XJ_NULL )
    {
        if( pWave->ElecNumber == ( UNSIGNED )iElecNumber )
            /* 本次录波为同一次故障的多次录波 */
            newFault = XJ_FALSE;
    }
    if( CWave.ElecNumber == ( UNSIGNED )iElecNumber )
        /* 本次录波为同一次故障的多次录波 */
        newFault = XJ_FALSE;
    if( CWave.WaveList != XJ_NULL )
    {
        search_ptr = CWave.WaveList;
        /* 查找故障波形数据 */
        do
        {
            pWave = ( CWM_WCB *)search_ptr;
            if( pWave->ElecNumber == ( UNSIGNED )iElecNumber )
            {
                /* 本次录波为同一次故障的多次录波 */
                newFault = XJ_FALSE;
                break;
            }
            search_ptr = search_ptr -> next;
        }while( CWave.WaveList != search_ptr );
    }

    pWave = WavePointer;
    if( pWave != XJ_NULL )
    {
        /* 两次同时录波,进行优先级判别 */
        if(( pWave->Type & WAVE_TYPE_START_FAULT ) != 0 )
            /* 新故障第一次录波 */
            LastPriority = 1;
        /* 上次录波的优先级 */
        else if(( pWave->Type & WAVE_TYPE_SOF_TEST ) != 0 )
            /* 手动录波 */
            LastPriority = 3;
        else if(( pWave->Type & WAVE_TYPE_START ) != 0 )
            /* 启动录波 */
            LastPriority = 2;
        else
            /* 故障录波 */
            LastPriority = 1;

        /* 本次录波的优先级 */
        if( newFault == XJ_TRUE )
            /* 新故障录波 */
            NextPriority = 1;
        else if(( iType & WAVE_TYPE_SOF_TEST ) != 0 )
            /* 手动录波 */
            NextPriority = 3;
        else if(( iType & WAVE_TYPE_START ) != 0 )
            /* 启动录波 */
            NextPriority = 2;
        else
            /* 故障录波 */
            NextPriority = 1;

        if( LastPriority < NextPriority )
        {
            /* 优先级先高后低,返回失败 */
            return( XJ_UNAVAILABLE );
        }
        else if( LastPriority > NextPriority )
        {
            /* 优先级先低后高,终止上次,启动本次 */
            /* 终止录波 */
            WavePointer = XJ_NULL;
            WavePointerCB[ 0 ] = XJ_NULL;
            /* 释放空间 */
            CWMC_WAVE_DELLOCATE( status, pWave );
            /* 删除链表 */
            CWMC_REMOVE_WAVE_LIST(&( pWave->Created ));
            /* 恢复原故障序号 */
            CWave.UserFaultNumber --;

            /* 检查录波自然序号是否在(1~0xFFFF)范围内 */
            CWMC_CHECK_DATA_LIMIT( CWave.UserFaultNumber, CWM_FAULT_NUMBER_MIN, CWM_FAULT_NUMBER_MAX );
        }
        else
        {
            /* 相同优先级,触发策略选择 */
            if( NextPriority == 1 )
            {
                /* 故障录波 */
                if( pWave->SampleSpace < (( UNSIGNED )iSpace ))
                {
                    /* 先高密度后低密度,返回失败 */
                    return( XJ_UNAVAILABLE );
                }

                if( pWave->SampleSpace == (( UNSIGNED )iSpace ))
                {
                    /* 同密度 */
                    if( pWave->TotalPointer > ( 5 * pWave->RecordNumber ))
                    {
                        /* 第一次录波数据小于20%返回失败 */
                        return( XJ_UNAVAILABLE );
                    }

                    /* 最小记录数据判别 */
                    if( pWave->RecordNumber < CWMC_GET_MIN_RECORD_NUMBER())
                    {
                        if(( pWave->Type & WAVE_TYPE_START_FAULT ) != 0 )
                        {
                            /* 新故障第一次录波的记录点数小于最小记录数据,直接返回失败 */
                            return( XJ_UNAVAILABLE );
                        }
                        else
                        {
                            /* 多次录波记录的数据点数小于最小记录数据,直接丢弃上次记录数据 */
                            WavePointer = XJ_NULL;
                            WavePointerCB[ 0 ] = XJ_NULL;
                            /* 释放空间 */
                            CWMC_WAVE_DELLOCATE( status, pWave );
                            /* 删除链表 */
                            CWMC_REMOVE_WAVE_LIST(&( pWave->Created ));
                            /* 恢复原故障序号 */
                            CWave.UserFaultNumber --;

                            /* 检查录波自然序号是否在(1~0xFFFF)范围内 */
                            CWMC_CHECK_DATA_LIMIT( CWave.UserFaultNumber, CWM_FAULT_NUMBER_MIN, CWM_FAULT_NUMBER_MAX );
                        }
                    }
                }
            }
            else
            {
                /* 启动或手动录波,返回失败 */
                return( XJ_UNAVAILABLE );
            }
        }
    }

    if((( iType & WAVE_TYPE_SOF_TEST ) != 0 ) && ( CWave.WaveList != XJ_NULL ))
    {
        /* 本次为手动录波,清除已经存在的手动录波 */
        search_ptr = CWave.WaveList;
        while( 1 )
        {
            if( CWave.WaveList != search_ptr->next )
            {
                pWave = ( CWM_WCB *)search_ptr;
                search_ptr = search_ptr -> next;
                if((( pWave->Type & WAVE_TYPE_SOF_TEST ) != 0 ))
                {
                    /* 释放手动录波空间 */
                    CWMC_WAVE_DELLOCATE( status, pWave );
                    /* 删除链表 */
                    CWMC_REMOVE_WAVE_LIST(&( pWave->Created ));
                }
            }
            else
            {
                pWave = ( CWM_WCB *)search_ptr;
                if((( pWave->Type & WAVE_TYPE_SOF_TEST ) != 0 ))
                {
                    /* 释放手动录波空间 */
                    CWMC_WAVE_DELLOCATE( status, pWave );
                    /* 删除链表 */
                    CWMC_REMOVE_WAVE_LIST(&( pWave->Created ));
                }
                break;
            }
        }
    }

    /* 录波段数检查 */
    if( waveSegments > CWM_MAX_WAVE_POINTER )
        iWaveNumber = CWM_MAX_WAVE_POINTER;
    else
        iWaveNumber = waveSegments;
    /* 起始电网故障检查 */
    if( newFault == XJ_TRUE )
        iWaveType = iType | WAVE_TYPE_START_FAULT;
    else
        iWaveType = iType;

    /* 生成录波链表 */
    status = XJ_SUCCESS;
    iCtrlType = CWM_WAVE_CTRL_NEW;
    for( i = 0; i < ( UNSIGNED )iWaveNumber; i ++ )
    {
        iSpace = * ( spacePtr + i ); /* 录波间距 */
        iTotalPoint = * ( totalPointPtr + i ); /* 总点数 */
        status = CWMC_CreateWaveList( iElecNumber, iWaveType, iStartPoint,
            iSpace, iTotalPoint, WaveChannel, pTime, iCtrlType, pState, iState );
        iCtrlType = 0;
        iWaveType = iType; /* 仅电网故障序号发生变化时设置起始电网故障标志 */
        if( status != XJ_SUCCESS ) /* 创建失败，直接返回 */
            break;
    }

    return( status );
}

/* 生成录波链表 */
INT CWMC_CreateWaveList( INT iElecNumber, INT iType, INT iStartPoint, INT iSpace,
    INT iTotalPoint, WAVE_CHANNEL * WaveChannel, TIME * pTime, INT ctrlType,
    STATE * pState, INT iState )
{
    UNSIGNED i;
    INT * p1,* p2;
    INT status;
    INT Number;
    UNSIGNED size;
    UNSIGNED variable;
    CWM_WCB * pWave;
    CSF_NODE * search_ptr;
    VOID * memory_ptr;
    UNSIGNED AnalogNumber;
    UNSIGNED InputNumber;
    UNSIGNED OutputNumber;
    UNSIGNED StateNumber;
    UNSIGNED LogicalNumber;
    UNSIGNED TotalPointNumber;
    PRO_CHANNEL * pChannel;
    WAVE_CHANNEL * pWaveChannel;
    TIME DevTime, * pDevTime;

    pWaveChannel = WaveChannel;
    /* 参数检查 */
    AnalogNumber = ( UNSIGNED )(( pWaveChannel->AnalogNumber > CWMC_GET_WAVE_ANALOG_NUMBER )
        ? CWMC_GET_WAVE_ANALOG_NUMBER : ( pWaveChannel->AnalogNumber ));
    InputNumber = ( UNSIGNED )(( pWaveChannel->InputNumber > CWMC_GET_WAVE_INPUT_GROUPS )
        ? CWMC_GET_WAVE_INPUT_GROUPS : ( pWaveChannel->InputNumber ));
    OutputNumber = ( UNSIGNED )(( pWaveChannel->OutputNumber > CWMC_GET_WAVE_OUTPUT_GROUPS )
        ? CWMC_GET_WAVE_OUTPUT_GROUPS : ( pWaveChannel->OutputNumber ));
    StateNumber = ( UNSIGNED )(( pWaveChannel->StateNumber > CWMC_MAX_WAVE_STATE_GROUPS )
        ? CWMC_MAX_WAVE_STATE_GROUPS : ( pWaveChannel->StateNumber ));
    LogicalNumber = ( UNSIGNED )(( pWaveChannel->LogicalNumber > CWMC_MAX_WAVE_LOGICAL_GROUPS )
        ? CWMC_MAX_WAVE_LOGICAL_GROUPS : ( pWaveChannel->LogicalNumber ));
    TotalPointNumber = ( UNSIGNED )(( iTotalPoint > CWMC_MAX_WAVE_POINT())
        ? CWMC_MAX_WAVE_POINT() : iTotalPoint );

    /* 申请动态内存 */
    /* 记录录波头内存 */
    size = sizeof( CWM_WCB );
    size += ( sizeof( CWM_ANALOG ) * AnalogNumber );
    size += ( sizeof( CWM_INPUT ) * InputNumber );
    size += ( sizeof( CWM_OUTPUT ) * OutputNumber );
    size += ( sizeof( CWM_STATE ) * StateNumber );
    size += ( sizeof( CWM_LOGICAL ) * LogicalNumber );
    /* 量值内存 */
    i = AnalogNumber;
    i += InputNumber;
    i += OutputNumber;
    i += StateNumber;
    i += LogicalNumber;
    variable = ( TotalPointNumber * sizeof( UNSIGNED )) + sizeof( CWM_VHD ) + sizeof( UNSIGNED );
    size += ( variable * i );

    /* 录波通道个数检查 */
    if( i == 0 )
    {
        /* 没有录波通道,返回错误 */
        return( XJ_UNAVAILABLE );
    }

    /* 申请动态内存 */
    CWMC_WAVE_ALLOCATE( status,& memory_ptr, size );
    if(( status != XJ_SUCCESS ) && ( CWave.WaveList != XJ_NULL ))
    {
        /* 内存空间不够,释放启动录波和手动录波 */
        search_ptr = CWave.WaveList;
        while( 1 )
        {
            if( CWave.WaveList != search_ptr->next )
            {
                pWave = ( CWM_WCB *)search_ptr;
                search_ptr = search_ptr -> next;
                if(( pWave->Type & WAVE_TYPE_START_FAULT ) == 0 )
                {
                    /* 禁止删除新故障第一次录波数据 */
                    if((( pWave->Type & WAVE_TYPE_SOF_TEST ) != 0 )
                        || (( pWave->Type & WAVE_TYPE_START ) != 0 ))
                    {
                        /* 释放启动（手动）录波空间 */
                        CWMC_WAVE_DELLOCATE( status, pWave );
                        /* 删除链表 */
                        CWMC_REMOVE_WAVE_LIST(&( pWave->Created ));
                        CWMC_WAVE_ALLOCATE( status,& memory_ptr, size );
                        if( status == XJ_SUCCESS )
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                pWave = ( CWM_WCB *)search_ptr;
                if(( pWave->Type & WAVE_TYPE_START_FAULT ) == 0 )
                {
                    /* 禁止删除新故障第一次录波数据 */
                    if((( pWave->Type & WAVE_TYPE_SOF_TEST ) != 0 )
                        || (( pWave->Type & WAVE_TYPE_START ) != 0 ))
                    {
                        /* 释放手动录波空间 */
                        CWMC_WAVE_DELLOCATE( status, pWave );
                        /* 删除链表 */
                        CWMC_REMOVE_WAVE_LIST(&( pWave->Created ));
                        CWMC_WAVE_ALLOCATE( status,& memory_ptr, size );
                    }
                }
                break;
            }
        }
    }

    if( status == XJ_SUCCESS )
    {
        /* 申请内存成功 */
        /* 调整故障序号 */
        CWave.UserFaultNumber ++;

        /* 检查录波自然序号是否在(1~0xFFFF)范围内 */
        CWMC_CHECK_DATA_LIMIT( CWave.UserFaultNumber, CWM_FAULT_NUMBER_MIN, CWM_FAULT_NUMBER_MAX );

        /* 添加链表 */
        pWave = ( CWM_WCB *)memory_ptr;
        CWMC_CREATE_WAVE_LIST(&( pWave->Created ));

        pWave -> StartFlag = 1;
        pWave -> EndFlag = 0;
        pWave -> FaultNumber = CWave.UserFaultNumber;
        pWave -> ElecNumber = ( UNSIGNED )iElecNumber;
        pWave -> Type = iType;
        pWave -> AnalogNumber = AnalogNumber;
        pWave -> InputNumber = InputNumber;
        pWave -> OutputNumber = OutputNumber;
        pWave -> StateNumber = StateNumber;
        pWave -> LogicNumber = LogicalNumber;
        CWMC_SAMPLE_POINT( pWave -> SamplePointer );
        /* 线路频率 */
        CWMC_SAMPLE_FREQUENCY( pWave -> LineFrequency );
        pWave -> SampleSpace = ( UNSIGNED )iSpace;
        if( pWave -> SampleSpace <= 0 )
        {
            pWave -> SampleSpace = 1;
        }
        /* 采样率(每秒采样点数) */
        CWMC_SAMPLE_RATE( pWave -> SampleRate, pWave -> SampleSpace );
        pWave -> RecordNumber = 0;
        pWave -> SampleNumber = 0;
        /* 故障前点数边界检查 */
        if( iStartPoint >= CWMC_MAX_SAMPLE_POINT )
            variable = ( UNSIGNED )( CWMC_MAX_SAMPLE_POINT - 1 );
        else
            variable = ( UNSIGNED )iStartPoint;
        pWave -> StartPointer = variable;
        pWave -> TotalPointer = TotalPointNumber;
        /* 填充装置时间 */
        if( pTime == XJ_NULL )
        {
            XJP_TM_Get_Time_Dev( & DevTime );
            pDevTime = & DevTime;
        }
        else
        {
            pDevTime = pTime;
        }
        pWave -> Year = pDevTime -> Year; /* 年 */
        pWave -> Month = pDevTime -> Month; /* 月 */
        pWave -> Day = pDevTime -> Day; /* 日 */
        pWave -> Hour = pDevTime -> Hour; /* 时 */
        pWave -> Minute = pDevTime -> Minute; /* 分 */
        pWave -> Millionsecond = pDevTime -> Millionsecond; /* 秒（毫秒） */
        pWave -> Nanosecond = pDevTime -> Nanosecond; /* 秒（毫秒） */
        pWave -> tQuality = pDevTime -> Quality; /* 时间品质因素 */
        if( pDeviceTab )
        {
            pWave -> WaveDataType = pDeviceTab -> ParaShowType; /* 录波数据类型(一/二次) */
        }
        else
        {
            /* 如果没有装置数据表固定为2次 */
            pWave -> WaveDataType = PRO_DATA_TYPE_SECONDARY;
        }

        /* 模拟量信息 */
        pWave -> AnalogInf = ( CWM_ANALOG *)( pWave + 1 );
        p1 = pWaveChannel->pAnalogNumber;
        for( i = 0; i < AnalogNumber; i ++)
        {
            Number = * p1 ++;
            pChannel = CWMC_CHANNEL_MEMORY_TAB();
            pChannel += Number;
            ( pWave->AnalogInf + i ) -> Number = pChannel -> chl_Num;
            ( pWave->AnalogInf + i ) -> Inf = pChannel -> chl_inf;
            ( pWave->AnalogInf + i ) -> Type = pChannel -> chl_type;
            ( pWave->AnalogInf + i ) -> CalculateCoefficient = pChannel->chl_calculate_coefficient;
            ( pWave->AnalogInf + i ) -> RateCoefficient = pChannel -> chl_rate_coefficient;
            ( pWave->AnalogInf + i ) -> PrimaryRate = pChannel -> chl_primary_rate;
            ( pWave->AnalogInf + i ) -> SecondaryRate = pChannel -> chl_secondary_rate;
            ( pWave->AnalogInf + i ) -> WaveCoefficient = pChannel->chl_wave_coefficient;
            ( pWave->AnalogInf + i ) -> WaveConverCoef = pChannel->chl_wave_conver_coef;
        }

        /* 开入量信息 */
        pWave -> InputInf = ( CWM_INPUT *)( pWave->AnalogInf + pWave->AnalogNumber );
        p1 = pWaveChannel->pInputNumber;
        p2 = pWaveChannel->pInputMasks;
        for( i = 0; i < InputNumber; i ++)
        {
            ( pWave->InputInf + i ) -> GroupNumber = * p1 ++;
            ( pWave->InputInf + i ) -> MaskBits = * p2 ++;
        }

        /* 开出量信息 */
        pWave -> OutputInf = ( CWM_OUTPUT *)( pWave->InputInf + pWave->InputNumber );
        p1 = pWaveChannel->pOutputNumber;
        p2 = pWaveChannel->pOutputMasks;
        for( i = 0; i < OutputNumber; i ++)
        {
            ( pWave->OutputInf + i ) -> GroupNumber = * p1 ++;
            ( pWave->OutputInf + i ) -> MaskBits = * p2 ++;
        }

        /* 状态量信息 */
        pWave -> StateInf = ( CWM_STATE *)( pWave->OutputInf + pWave->OutputNumber );
        p1 = pWaveChannel->pStateNumber;
        p2 = pWaveChannel->pStateMasks;
        for( i = 0; i < StateNumber; i ++)
        {
            ( pWave->StateInf + i ) -> GroupNumber = * p1 ++;
            ( pWave->StateInf + i ) -> MaskBits = * p2 ++;
        }

        /* 逻辑量信息 */
        pWave -> LogicalInf = ( CWM_LOGICAL *)( pWave->StateInf + pWave->StateNumber );
        p1 = pWaveChannel->pLogicalNumber;
        p2 = pWaveChannel->pLogicalMasks;
        for( i = 0; i < LogicalNumber; i ++)
        {
            ( pWave->LogicalInf + i ) -> GroupNumber = * p1 ++;
            ( pWave->LogicalInf + i ) -> MaskBits = * p2 ++;
        }

        /* 锁中断 */
        CWMC_LOCK_INTERRUPTS( Number );
        pWave -> WaveMemory = ( UNSIGNED *)( pWave->LogicalInf + pWave->LogicNumber );
        if( ctrlType == CWM_WAVE_CTRL_NEW )
        {
            /* 设置录波状态量 */
            if( pState )
                CWMC_SetWaveState( pState, iState );
            /* 新触发录波终止当前记录的录波数据  */
            if( WavePointer != XJ_NULL )
            {
                /* 置完成标志 */
                WavePointer -> EndFlag = 1;
                /* 终止录波 */
                WavePointer = XJ_NULL;
            }
            WavePointer = pWave;
            WavePointerCB[0] = pWave;
        }
        else
        {
            /* 同一次触发的多段录波，记录触发的录波信息 */
            if( WavePointer == XJ_NULL )
            {
                WavePointer = pWave;
            }
            /* 添加生成的录波信息到记录控制块中 */
            for( i = 0; i < CWM_MAX_WAVE_POINTER; i ++ )
            {
                if( WavePointerCB[i] == XJ_NULL )
                {
                    WavePointerCB[i] = pWave;
                    break;
                }
            }
        }
        /* 开中断 */
        CWMC_UNLOCK_INTERRUPTS( Number );
    }
    else
    {
        /* 申请内存失败 */
        CWMC_ErrInf.waveMalloc ++;
    }

    return( status );
}

/* 启动录波(一段) */
INT CWMC_WaveStart( INT iType, INT iSpace, INT iStartPoint, INT iTotalPoint,
    INT iElecNumber, WAVE_CHANNEL * WaveChannel, TIME * pTime, STATE * pState, INT iState )
{
    INT status;
    INT iWaveSpace;
    INT iWaveTotalNumber;

    iWaveSpace = iSpace;
    iWaveTotalNumber = iTotalPoint;
    status = CWMC_WaveStartMul( 1, iType, & iWaveSpace, iStartPoint, & iWaveTotalNumber,
        iElecNumber, WaveChannel, pTime, pState, iState );

    return( status );
}

/* 获取录波头部信息 */
INT CWMC_GetWaveHead( CWM_WCB * source, VOID * destination )
{
    UNSIGNED i;
    CWM_WHD * pHead;
    CWM_WCB * pSrc;
    UNSIGNED size;
    UNSIGNED variable;

    pSrc = source;
    pHead = ( CWM_WHD *)destination;
    /* 录波头数据大小计算 */
    size = ( sizeof( CWM_WHD )/ sizeof( UNSIGNED ));
    /* 模拟量大小计算 */
    variable = ( pSrc->RecordNumber + CWMC_ANALOG_BANK_WIDTH()- 1 )/ CWMC_ANALOG_BANK_WIDTH();
    variable += (( sizeof( CWM_VHD )+ sizeof( UNSIGNED ))/ sizeof( UNSIGNED ));
    variable *= pSrc -> AnalogNumber;
    size += variable;
    /* 开入量、开出量、状态量、逻辑量大小计算 */
    variable = pSrc->RecordNumber;
    variable += (( sizeof( CWM_VHD )+ sizeof( UNSIGNED ))/ sizeof( UNSIGNED ));
    i = pSrc -> InputNumber;
    i += pSrc -> OutputNumber;
    i += pSrc -> StateNumber;
    i += pSrc -> LogicNumber;
    size += ( variable * i );
    /* 录波数据的大小 */
    pHead -> Size = size;
    pHead -> FaultNumber = pSrc -> FaultNumber;
    if( pSrc -> ElecNumber != CWave.ElecNumber )
    {
        pHead -> Sof = pSrc -> Type | WAVE_TYPE_START_FAULT;
    }
    else
    {
        pHead -> Sof = pSrc -> Type;
    }
    pHead -> ElecNumber = pSrc -> ElecNumber;
    pHead -> AnalogNumber = pSrc -> AnalogNumber;
    pHead -> InputNumber = pSrc -> InputNumber;
    pHead -> OutputNumber = pSrc -> OutputNumber;
    pHead -> ProtectNumber = pSrc -> StateNumber;
    pHead -> LogicNumber = pSrc -> LogicNumber;
    pHead -> SamplePointer = pSrc -> SamplePointer;
    pHead -> LineFrequency = pSrc -> LineFrequency;
    pHead -> SampleRate = pSrc -> SampleRate;
    pHead -> Space = pSrc -> SampleSpace;
    pHead -> StartPointer = pSrc->StartPointer / pSrc->SampleSpace;
    pHead -> TotalPointer = pSrc -> RecordNumber;
    pHead -> Year = pSrc -> Year;
    pHead -> Month = pSrc -> Month;
    pHead -> Day = pSrc -> Day;
    pHead -> Hour = pSrc -> Hour;
    pHead -> Minute = pSrc -> Minute;
    pHead -> Millionsecond = pSrc -> Millionsecond;
    pHead -> Nanosecond = pSrc -> Nanosecond;
    pHead -> tQuality = pSrc -> tQuality;
    pHead -> WaveDataType = pSrc -> WaveDataType;

    for( i = 0; i < pSrc->AnalogNumber; i ++)
    {
        pHead->ChannelInf[i] = ( pSrc->AnalogInf + i ) -> Inf;
    }
    for( i = 0; i < pSrc->InputNumber; i ++)
    {
        pHead->InputGroups[i] = ( pSrc->InputInf + i ) -> GroupNumber;
        pHead->InputActiveBits[i] = ( pSrc->InputInf + i ) -> MaskBits;
    }
    for( i = 0; i < pSrc->OutputNumber; i ++)
    {
        pHead->OutputGroups[i] = ( pSrc->OutputInf + i ) -> GroupNumber;
        pHead->OutputActiveBits[i] = ( pSrc->OutputInf + i ) -> MaskBits;
    }

    for( i = 0; i < pSrc->StateNumber; i ++)
    {
        pHead->StateGroups[i] = ( pSrc->StateInf + i ) -> GroupNumber;
        pHead->ProtectActiveBits[i] = ( pSrc->StateInf + i ) -> MaskBits;
    }
    for( i = 0; i < pSrc->LogicNumber; i ++)
    {
        pHead->LogicalGroups[i] = ( pSrc->LogicalInf + i ) -> GroupNumber;
        pHead->LogicalActiveBits[i] = ( pSrc->LogicalInf + i ) -> MaskBits;
    }
    pHead->EndFlag = CWM_END_FLAG;

    return( XJ_SUCCESS );
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CWMC_WaveMain                                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function save the wave data.                                */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*      NAME            DATE                    REMARKS                  */
/*                                                                       */
/*      ZhideYang.      07-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

UNSIGNED CWMC_WaveDataBuf[CWM_MAX_WAVE_VALUE_WORD + CWMC_WAVE_HEAD_WORD() + 20];

INT CWMC_WaveMain( VOID )
{
    UNSIGNED TotalNumber;
    UNSIGNED variable, uData;
    UNSIGNED number;
    SIGNED point_value;
    CWM_WHD * pWaveHead;
    UNSIGNED size;
    CWM_WCB * pRecord,* pWave;
    CSF_NODE * search_ptr;
    CWM_VHD * pValueHead;
    CWM_ANALOG * pAnalog;
    CWM_INPUT * pInput;
    CWM_OUTPUT * pOutput;
    CWM_STATE * pState;
    CWM_LOGICAL * pLogical;
    register UNSIGNED * pSrc;
    register UNSIGNED * pDst;
    register SIGNED * pWaveAnalog;
    register UNSIGNED i, j, k;
    volatile static UNSIGNED GroupNumber = 0;
    volatile static UNSIGNED SubNumber = 0;
    static UNSIGNED * SrcWaveAddr;
    static INT ClearNumber = 0;
    static UNSIGNED ActualDataNumber;
    CHAR EventName[CEM_EVENT_NAME]; /* event buffer */
    INT Status;

    if( CWave.InitFlag == 0 )
    {
        /* 等待初始化完成 */
        return( XJ_TRUE );
    }

    /* 清除录波 */
    CWMC_GET_WAVE_CLEAR( Status,& variable );
    if( Status == XJ_SUCCESS )
    {
        /* 录波初始化,所有的数据全部丢失 */
        if( CWave.Initialize(& CWave, 1, CWMC_GET_MAX_WAVE_TIME()) == XJ_SUCCESS )
        {
            ClearNumber = 0;
            /* 清除录波成功 */
            sprintf( EventName, " %03d-Ok", CPC_RES_EVENT_WAVE_CLR );
            CWMC_FILL_RESERVE_EVENT( EventName );
            CWMC_SET_WAVE_CLEAR();
        }
        else
        {
            /* 最大执行次数 */
            if((++ ClearNumber ) > 5 )
            {
                ClearNumber = 0;
                /* 清除录波出错 */
                sprintf( EventName, " %03d-Err", CPC_RES_EVENT_WAVE_CLR );
                CWMC_FILL_RESERVE_EVENT( EventName );
                CWMC_SET_WAVE_CLEAR();
            }
        }

        if( GroupNumber != 0 )
        {
            /* 关闭录波设备 */
            CLOSEDRIVER( CWMC_GET_WAVE_HANDLE());
        }
        GroupNumber = 0;
        SubNumber = 0;

        return( XJ_TRUE );
    }
    else
    {
        ClearNumber = 0;
    }

    CWMC_ErrInf.runTimes ++;

    pRecord = XJ_NULL;
    /* 锁录波记录 */
    CWMC_PROTECT_WAVE();
    search_ptr = CWave.WaveList;
    if( search_ptr != XJ_NULL )
    {
        /* 查找故障波形数据 */
        do
        {
            pWave = ( CWM_WCB *)search_ptr;
            //            if((( pWave->Type & WAVE_TYPE_SOF_TEST ) == 0 )
            //                && (( pWave->Type & WAVE_TYPE_START ) == 0 ))
            {
                pRecord = ( CWM_WCB *)search_ptr;
                break;
            }
            search_ptr = search_ptr -> next;
        }while( CWave.WaveList != search_ptr );
    }
    CWMC_UNPROTECT_WAVE();

    if(( pRecord != XJ_NULL )
        && ( pRecord->EndFlag != 0 ))
    {
        /* 记录录波完成,写录波 */
        TotalNumber = pRecord->AnalogNumber + pRecord->InputNumber;
        TotalNumber += ( pRecord->OutputNumber + pRecord->StateNumber );
        TotalNumber += pRecord->LogicNumber;

        /* 打开录波设备 */
        if(( GroupNumber == 0 ) && ( SubNumber == 0 ))
        {
            OPENDRIVER( Status, CWMC_GET_WAVE_HANDLE(), CWMC_GET_MAX_WAVE_TIME());
            if( Status == XJ_SUCCESS )
            {
                ActualDataNumber = 0;
                GroupNumber ++;
                SubNumber = 0;
            }
        }
        /* 写录波头部数据 */
        else if( GroupNumber == 1 )
        {
            /* 读取录波头部信息 */
            CWMC_GetWaveHead( pRecord, CWMC_WaveDataBuf );

            ActualDataNumber = CWMC_WAVE_HEAD_WORD();
            pWaveHead = ( CWM_WHD *)CWMC_WaveDataBuf;

            GroupNumber ++;
            SubNumber = 0;
            if(( pWaveHead->ElecNumber != CWave.ElecNumber ) && ( CWave.ElecNumber ))
            {
                /* 完成的故障序号 */
                CWave.ReadyFaultNumber = CWave.ElecNumber;
                CWave.ReadySerialNumber = CWave.FaultNumber;
            }
            CWave.FaultNumber1 = pWaveHead->FaultNumber;
            CWave.ElecNumber1 = pWaveHead->ElecNumber;

            SrcWaveAddr = pRecord -> WaveMemory;
        }
        /* 写模拟量数据 */
        else if( GroupNumber == 2 )
        {
            for( i = SubNumber; i < ( pRecord->AnalogNumber ); i ++)
            {
                /* 写模拟量头部信息 */
                pValueHead = ( CWM_VHD *)& CWMC_WaveDataBuf[ActualDataNumber];
                pAnalog = pRecord->AnalogInf;
                pAnalog += i;
                /* 模拟量头部信息 */
                pValueHead->head_info.analog.Inf = pAnalog->Inf;
                pValueHead->head_info.analog.Primary_rate = pAnalog->PrimaryRate;
                pValueHead->head_info.analog.Secondary_rate = pAnalog->SecondaryRate;
                if(( pRecord->Type & WAVE_TYPE_COEFFECT ) != 0 )
                {
                    /* 有效值录波 */
                    pValueHead->head_info.analog.Coefficient =
                        pAnalog->CalculateCoefficient / pAnalog->RateCoefficient * pAnalog->WaveCoefficient
                        / pAnalog->WaveConverCoef;
                }
                else
                {
                    /* 瞬时值录波 */
                    if(( pAnalog->Type & DC_COMPONENT ) != 0 )
                    {
                        pValueHead->head_info.analog.Coefficient =
                            pAnalog->CalculateCoefficient / pAnalog->RateCoefficient * pAnalog->WaveCoefficient
                            / pAnalog->WaveConverCoef;
                    }
                    else
                    {
                        /* 交流量计算系数除以2的开方 */
                        pValueHead->head_info.analog.Coefficient =
                            pAnalog->CalculateCoefficient / pAnalog->RateCoefficient *(( float )0.707107 ) * pAnalog->WaveCoefficient
                            / pAnalog->WaveConverCoef;
                    }
                }

                /* 模拟量数据 */
                pWaveAnalog = ( SIGNED *)SrcWaveAddr;
                pDst = ( UNSIGNED *)( pValueHead + 1 );
                number = pRecord->RecordNumber / CWMC_ANALOG_BANK_WIDTH();
                for( j = 0; j < number; j ++)
                {
                    uData = 0;
                    for( k = 0; k < CWMC_ANALOG_BANK_WIDTH(); k ++)
                    {
                        point_value = * pWaveAnalog ++;
                        if( point_value > CWMC_MAX_ANALOG_VALUE())
                        {
                            /* 上限检查 */
                            variable = 0x7fff;
                        }
                        else if( point_value < CWMC_MIN_ANALOG_VALUE())
                        {
                            /* 下限检查 */
                            variable = 0x8001;
                        }
                        else
                        {
                            variable = ( UNSIGNED )( point_value & CWMC_ANALOG_BANK_MASKS());
                        }

                        uData += ( variable ) << ( k * CWMC_ANALOG_BANK_BITS());
                    }
                    * pDst ++ = uData;
                }

                number = pRecord->RecordNumber % CWMC_ANALOG_BANK_WIDTH();
                if( number != 0 )
                {
                    point_value = * pWaveAnalog ++;
                    if( point_value > CWMC_MAX_ANALOG_VALUE())
                    {
                        /* 上限检查 */
                        variable = 0x7fff;
                    }
                    else if( point_value < CWMC_MIN_ANALOG_VALUE())
                    {
                        /* 下限检查 */
                        variable = 0x8001;
                    }
                    else
                    {
                        variable = ( UNSIGNED )( point_value & CWMC_ANALOG_BANK_MASKS());
                    }
                    * pDst ++ = variable;
                }
                * pDst ++ = CWM_END_FLAG;

                number = ( pRecord->RecordNumber + CWMC_ANALOG_BANK_WIDTH()- 1 )/ CWMC_ANALOG_BANK_WIDTH();
                size = ( sizeof( CWM_VHD )+ sizeof( UNSIGNED ) )/ sizeof( UNSIGNED );
                size += number;

                ActualDataNumber += size;
                SrcWaveAddr += pRecord->TotalPointer;
            }

            GroupNumber ++;
            SubNumber = 0;
        }
        /* 写开入量数据 */
        else if( GroupNumber == 3 )
        {
            for( i = SubNumber; i < ( pRecord->InputNumber ); i ++)
            {
                pValueHead = ( CWM_VHD *)& CWMC_WaveDataBuf[ActualDataNumber];
                pInput = pRecord->InputInf;
                pInput += i;

                /* 开入量头部信息 */
                pValueHead->head_info.input.GroupNumber = pInput->GroupNumber;
                pValueHead->head_info.input.MaskBits = pInput->MaskBits;

                /* 开入量数据 */
                pSrc = ( UNSIGNED *)SrcWaveAddr;
                pDst = ( UNSIGNED *)( pValueHead + 1 );
                for( k = 0; k < pRecord->RecordNumber; k ++)
                {
                    * pDst ++ = * pSrc ++;
                }
                * pDst ++ = CWM_END_FLAG;

                size = ( sizeof( CWM_VHD )+ sizeof( UNSIGNED ) )/ sizeof( UNSIGNED );
                size += pRecord->RecordNumber;

                ActualDataNumber += size;
                SrcWaveAddr += pRecord->TotalPointer;
            }

            GroupNumber ++;
            SubNumber = 0;
        }
        /* 写开出量数据 */
        else if( GroupNumber == 4 )
        {
            for( i = SubNumber; i < ( pRecord->OutputNumber ); i ++)
            {
                pValueHead = ( CWM_VHD *)& CWMC_WaveDataBuf[ActualDataNumber];
                pOutput = pRecord->OutputInf;
                pOutput += i;

                /* 开出量头部信息 */
                pValueHead->head_info.output.GroupNumber = pOutput->GroupNumber;
                pValueHead->head_info.output.MaskBits = pOutput->MaskBits;

                /* 开出量数据 */
                pSrc = ( UNSIGNED *)SrcWaveAddr;
                pDst = ( UNSIGNED *)( pValueHead + 1 );
                for( k = 0; k < pRecord->RecordNumber; k ++)
                {
                    * pDst ++ = * pSrc ++;
                }
                * pDst ++ = CWM_END_FLAG;

                size = ( sizeof( CWM_VHD )+ sizeof( UNSIGNED ) )/ sizeof( UNSIGNED );
                size += pRecord->RecordNumber;

                ActualDataNumber += size;
                SrcWaveAddr += pRecord->TotalPointer;
            }

            GroupNumber ++;
            SubNumber = 0;
        }
        /* 写状态量数据 */
        else if( GroupNumber == 5 )
        {
            for( i = SubNumber; i < ( pRecord->StateNumber ); i ++)
            {
                pValueHead = ( CWM_VHD *)& CWMC_WaveDataBuf[ActualDataNumber];
                pState = pRecord->StateInf;
                pState += i;

                /* 状态量头部信息 */
                pValueHead->head_info.state.GroupNumber = pState->GroupNumber;
                pValueHead->head_info.state.MaskBits = pState->MaskBits;

                /* 状态量数据 */
                pSrc = ( UNSIGNED *)SrcWaveAddr;
                pDst = ( UNSIGNED *)( pValueHead + 1 );
                for( k = 0; k < pRecord->RecordNumber; k ++)
                {
                    * pDst ++ = * pSrc ++;
                }
                * pDst ++ = CWM_END_FLAG;

                size = ( sizeof( CWM_VHD )+ sizeof( UNSIGNED ) )/ sizeof( UNSIGNED );
                size += pRecord->RecordNumber;

                ActualDataNumber += size;
                SrcWaveAddr += pRecord->TotalPointer;
            }

            GroupNumber ++;
            SubNumber = 0;
        }
        /* 写逻辑量数据 */
        else if( GroupNumber == 6 )
        {
            for( i = SubNumber; i < ( pRecord->LogicNumber ); i ++)
            {
                pValueHead = ( CWM_VHD *)& CWMC_WaveDataBuf[ActualDataNumber];
                pLogical = pRecord->LogicalInf;
                pLogical += i;

                /* 逻辑量头部信息 */
                pValueHead->head_info.logical.GroupNumber = pLogical->GroupNumber;
                pValueHead->head_info.logical.MaskBits = pLogical->MaskBits;

                /* 逻辑量数据 */
                pSrc = ( UNSIGNED *)SrcWaveAddr;
                pDst = ( UNSIGNED *)( pValueHead + 1 );
                for( k = 0; k < pRecord->RecordNumber; k ++)
                {
                    * pDst ++ = * pSrc ++;
                }
                * pDst ++ = CWM_END_FLAG;

                size = ( sizeof( CWM_VHD )+ sizeof( UNSIGNED ))/ sizeof( UNSIGNED );
                size += pRecord->RecordNumber;

                ActualDataNumber += size;
                SrcWaveAddr += pRecord->TotalPointer;
            }

            GroupNumber ++;
            SubNumber = 0;
        }
        else
        {
            Status = CWave.Write(& CWave, CWMC_WaveDataBuf, 0, ActualDataNumber, CWMC_GET_MAX_WAVE_TIME());
            if( Status != XJ_TIMEOUT )
            {
                if( Status != XJ_SUCCESS )
                {
                    /* 录波文件数目检查 */
                    CWMC_WaveFullFileCheckMain();
                }

                GroupNumber = 0;
                SubNumber = 0;

                CWMC_PROTECT_WAVE();
                /* 释放空间 */
                CWMC_WAVE_DELLOCATE( Status,&( pRecord->Created ));
                /* 删除链表 */
                CWMC_REMOVE_WAVE_LIST(&( pRecord->Created ));
                CWMC_UNPROTECT_WAVE();

                /* 关闭录波设备 */
                CLOSEDRIVER( CWMC_GET_WAVE_HANDLE());
            }
        }
    }
    else
    {
        GroupNumber = 0;
        SubNumber = 0;
        if( pRecord == XJ_NULL )
        {
            CWave.Ready(& CWave, CWMC_GET_MAX_WAVE_TIME());
        }
    }

    return( XJ_TRUE );
}

/* 读取录波电网故障序号 */
INT CWMC_ReadElecFaultNumber( VOID )
{
    INT iElecNumber;

    iElecNumber = ( INT )CWMC_GET_WAVE_ELEC_NUMBER();

    return( iElecNumber );
}

/* 读取录波故障序号 */
INT CWMC_ReadWaveIndexNumber( VOID )
{
    INT iIndexNumber;

    iIndexNumber = ( INT )CWMC_GET_WAVE_INDEX();

    return( iIndexNumber );
}

/* 自动调整录波电网故障序号 */
INT CWMC_AdjustElecNumber( VOID )
{
    UNSIGNED Variable;
    INT iElecNumber;

    Variable = CWMC_GET_WAVE_ELEC_NUMBER(); /* 获取录波电网故障序号 */
    Variable ++;

    /* 检查录波电网故障序号是否在(1~0xFFFF)范围内 */
    CWMC_CHECK_DATA_LIMIT( Variable, CWM_FAULT_NUMBER_MIN, CWM_FAULT_NUMBER_MAX );

    CWMC_SET_WAVE_ELEC_NUMBER( Variable ); /* 设置录波电网故障序号 */
    iElecNumber = ( INT )Variable;

    return( iElecNumber );
}

/* 设置录波电网故障序号 */
INT CWMC_SetElecNumber( INT iElecNumber )
{
    UNSIGNED Variable;

    Variable = ( UNSIGNED )iElecNumber;

    /* 检查录波电网故障序号是否在(1~0xFFFF)范围内 */
    CWMC_CHECK_DATA_LIMIT( Variable, CWM_FAULT_NUMBER_MIN, CWM_FAULT_NUMBER_MAX );

    CWMC_SET_WAVE_ELEC_NUMBER( Variable );

    return(( INT )Variable );
}

/* 设置录波状态量 */
INT CWMC_SetWaveState( STATE * pState, INT iState )
{
    if( pState )
    {
        if( iState == CWM_PROTECT_OPERATE )
        {
            /* 状态量动作 */
            CWMC_Protect_Status[pState->state_protect_group] |=
                pState->state_sub_protect;
        }
        else
        {
            /* 状态量返回 */
            CWMC_Protect_Status[pState->state_protect_group] &=
                (~ pState->state_sub_protect );
        }
        return( XJ_SUCCESS );
    }
    else
    {
        return( XJ_UNAVAILABLE );
    }
}

/* 获取录波逻辑状态量信息 */
VOID * CWMC_ProtectLogicInformation( UNSIGNED * Actual_Size )
{
    * Actual_Size = CWMC_MAX_WAVE_LOGICAL_GROUPS;

    return( CWMC_Protect_Logic_Status );
}

/* 获取录波序号 */
static INT CWMC_GetWaveIndexNumber( CHAR * dirName, UNSIGNED * Dst,
    UNSIGNED * pActual_size, UNSIGNED Number, UNSIGNED FaultNumber )
{
    UNSIGNED k;
    UNSIGNED * p;
    UNSIGNED IndexNumber;
    UNSIGNED ActualNumber;
    UNSIGNED CondMasks;
    UNSIGNED CondValue;
    INT status;

    /* 读取指定故障序号的索引表数据 */
    CondMasks = CWMC_DATA_MASKS16() << CWMC_DATA_BITS16() ;
    CondValue = FaultNumber << CWMC_DATA_BITS16();
    status = XJP_Index_Read( CWMC_WaveHandle, Dst, pActual_size, Number, CondMasks, CondValue );
    if( status == XJ_SUCCESS )
    {
        /* 清除索引表中的故障序号，仅保留录波索引号 */
        p = Dst;
        ActualNumber = * pActual_size;
        for( k = 0; k < ActualNumber; k ++)
        {
            IndexNumber = (* p ) & CWMC_DATA_MASKS16();
            * p = IndexNumber;
            p ++;
        }
        return( XJ_SUCCESS );
    }
    else
    {
        * pActual_size = 0;
        return( XJ_UNAVAILABLE );
    }
}

/* 查找故障总数 */
static INT CWMC_FindFaultNumber( CWAVE * this, VOID * pBuffer, INT * Actual_Number,
    INT Type, UNSIGNED uTimeOut )
{
    INT status;                         /* complete status */

    if( this -> InitFlag == 0 )
    {
        /* 初始化检查 */
        return( XJ_UNAVAILABLE );
    }

    /* 打开录波设备 */
    OPENDRIVER( status, CWMC_GET_WAVE_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* 获取录波序号 */
        STATUSDRIVER( status, CWMC_GET_WAVE_HANDLE(), pBuffer,( UNSIGNED *)Actual_Number, 1, uTimeOut );

        /* 关闭录波设备 */
        CLOSEDRIVER( CWMC_GET_WAVE_HANDLE());
    }

    /* Return the completion status.  */
    return( status );
}

/* 设置录波故障状态 */
INT CWMC_SetFaultStatus( INT iFaultStatus )
{
    CWMC_SetWaveStatus(& CWave, iFaultStatus );

    return( XJ_SUCCESS );
}

/* 初始化录波RCD数据模型 */
static INT CWMC_InitWaveRcd( INT FaultIndex, INT ElecFaultIndex, INT ElecNum )
{
    TIME     uWaveTime;
    CWM_WHD  waveHead,* pWaveHead;
    WAVE_RCD * pWaveRcd;
    INT      iNumber;
    INT      k;
    UNSIGNED Quality1;
    UNSIGNED tSec1;
    UNSIGNED tFrac1;
    UNSIGNED tQuality1;
    INT      iStatus1;
    UNSIGNED Quality2;
    UNSIGNED tSec2;
    UNSIGNED tFrac2;
    UNSIGNED tQuality2;
    INT      iStatus2;
    static INT iInitFlag = 0;
    /*=======================================*/
    /* 仅初始化一次就可以 */
    if( iInitFlag == 1 )
    {
        return( XJ_SUCCESS );
    }

    /* 置初始化完成标志 */
    iInitFlag = 1;
    pWaveHead = & waveHead;
    /* 获取WAVE RCD数据模型 */
    pWaveRcd = ( WAVE_RCD *)XJP_DS_Get_Table_Memory( TAB_REC_ID,& iNumber );
    if( iNumber > 0 )
    {
        /* 电网故障的时间取的是一次电网故障首次录波的时间,
        故障时间取的是一次电网故障最后一次录波的时间,
        当一次电网故障有多次录波的时候两时间是不同的,要读取两次 */
        if( FaultIndex != ElecFaultIndex )
        {
            /* 读取录波头部信息 */
            iStatus1 = CWave.Read(& CWave, FaultIndex, pWaveHead, 0, 0,
                (( sizeof( CWM_WHD ) + sizeof( UNSIGNED )- 1 )/ sizeof( UNSIGNED )), 100 );
            if( iStatus1 == XJ_SUCCESS )
            {
                uWaveTime.Year = pWaveHead -> Year;        // year
                uWaveTime.Month = pWaveHead -> Month;      // month
                uWaveTime.Day = pWaveHead -> Day;          // day
                uWaveTime.Hour = pWaveHead -> Hour;        // hour
                uWaveTime.Minute = pWaveHead -> Minute;    // minute
                uWaveTime.Millionsecond = pWaveHead -> Millionsecond; // millionsecond
                uWaveTime.Nanosecond = pWaveHead -> Nanosecond; // Nanosecond
                uWaveTime.Quality = pWaveHead -> tQuality; // Quality
                tQuality1 = pWaveHead -> tQuality;
                /* 调用时间转换函数 */
                XJP_TM_Dev_To_NS_UTC( & tSec1, & tFrac1, & uWaveTime );
                /* 判断录波类型决定RCD模型中品质因素的测试位状态 */
                if( pWaveHead -> Sof & WAVE_TYPE_SOF_TEST )
                {
                    Quality1 = ( UNSIGNED )QUALITY_TEST;
                }
                else
                {
                    Quality1 = 0;
                }
            }
            /* 读取录波头部信息 */
            iStatus2 = CWave.Read(& CWave, ElecFaultIndex, pWaveHead, 0, 0,
                (( sizeof( CWM_WHD ) + sizeof( UNSIGNED )- 1 )/ sizeof( UNSIGNED )), 100 );
            if( iStatus2 == XJ_SUCCESS )
            {
                uWaveTime.Year = pWaveHead -> Year;        // year
                uWaveTime.Month = pWaveHead -> Month;      // month
                uWaveTime.Day = pWaveHead -> Day;          // day
                uWaveTime.Hour = pWaveHead -> Hour;        // hour
                uWaveTime.Minute = pWaveHead -> Minute;    // minute
                uWaveTime.Millionsecond = pWaveHead -> Millionsecond; // millionsecond
                uWaveTime.Nanosecond = pWaveHead -> Nanosecond; // Nanosecond
                uWaveTime.Quality = pWaveHead -> tQuality; // Quality
                tQuality2 = pWaveHead -> tQuality;
                /* 调用时间转换函数 */
                XJP_TM_Dev_To_NS_UTC( & tSec2, & tFrac2, & uWaveTime );
                /* 判断录波类型决定RCD模型中品质因素的测试位状态 */
                if( pWaveHead -> Sof & WAVE_TYPE_SOF_TEST )
                {
                    Quality2 = ( UNSIGNED )QUALITY_TEST;
                }
                else
                {
                    Quality2 = 0;
                }
            }
            /* 判断读取录波数据是否成功,决定是否更新RCD模型中的对应项 */
            if(( iStatus1 == XJ_SUCCESS ) && ( iStatus2 == XJ_SUCCESS ))
            {
                for( k = 0; k < iNumber; k ++ )
                {
                    ( pWaveRcd + k ) -> fltNum = FaultIndex;        /* 故障序号 */
                    ( pWaveRcd + k ) -> fltNum_q |= Quality1;       /* 故障序号品质因素 */
                    ( pWaveRcd + k ) -> fltNum_TSec = tSec1;        /* 故障序号时间秒数 */
                    ( pWaveRcd + k ) -> fltNum_TFrac = tFrac1;      /* 故障序号时间毫秒数 */
                    ( pWaveRcd + k ) -> fltNum_TQflag = tQuality1;  /* 故障序号时间品质因素 */
                    ( pWaveRcd + k ) -> GriFltNum = ElecNum;        /* 电网故障序号 */
                    ( pWaveRcd + k ) -> GriFltNum_q |= Quality2;    /* 电网故障序号品质因素 */
                    ( pWaveRcd + k ) -> GriFltNum_TSec = tSec2;     /* 电网故障序号时间秒数 */
                    ( pWaveRcd + k ) -> GriFltNum_TFrac = tFrac2;   /* 电网故障序号时间毫秒数 */
                    ( pWaveRcd + k ) -> GriFltNum_TQflag = tQuality2; /* 电网故障序号时间品质因素 */
                }
            }
            else
            {
                if( iStatus1 == XJ_SUCCESS )
                {
                    for( k = 0; k < iNumber; k ++ )
                    {
                        ( pWaveRcd + k ) -> fltNum = FaultIndex;        /* 故障序号 */
                        ( pWaveRcd + k ) -> fltNum_q |= Quality1;       /* 故障序号品质因素 */
                        ( pWaveRcd + k ) -> fltNum_TSec = tSec1;        /* 故障序号时间秒数 */
                        ( pWaveRcd + k ) -> fltNum_TFrac = tFrac1;      /* 故障序号时间毫秒数 */
                        ( pWaveRcd + k ) -> fltNum_TQflag = tQuality1;  /* 故障序号时间品质因素 */
                    }
                }
                if( iStatus2 == XJ_SUCCESS )
                {
                    for( k = 0; k < iNumber; k ++ )
                    {
                        ( pWaveRcd + k ) -> GriFltNum = ElecNum;        /* 电网故障序号 */
                        ( pWaveRcd + k ) -> GriFltNum_q |= Quality2;    /* 电网故障序号品质因素 */
                        ( pWaveRcd + k ) -> GriFltNum_TSec = tSec2;     /* 电网故障序号时间秒数 */
                        ( pWaveRcd + k ) -> GriFltNum_TFrac = tFrac2;   /* 电网故障序号时间毫秒数 */
                        ( pWaveRcd + k ) -> GriFltNum_TQflag = tQuality2; /* 电网故障序号时间品质因素 */
                    }
                }
            }
        }
        else
        {
            /* 读取录波头部信息 */
            iStatus1 = CWave.Read(& CWave, FaultIndex, pWaveHead, 0, 0,
                (( sizeof( CWM_WHD ) + sizeof( UNSIGNED )- 1 )/ sizeof( UNSIGNED )), 100 );
            if( iStatus1 == XJ_SUCCESS )
            {
                uWaveTime.Year = pWaveHead -> Year;        // year
                uWaveTime.Month = pWaveHead -> Month;      // month
                uWaveTime.Day = pWaveHead -> Day;          // day
                uWaveTime.Hour = pWaveHead -> Hour;        // hour
                uWaveTime.Minute = pWaveHead -> Minute;    // minute
                uWaveTime.Millionsecond = pWaveHead -> Millionsecond; // millionsecond
                uWaveTime.Nanosecond = pWaveHead -> Nanosecond; // Nanosecond
                uWaveTime.Quality = pWaveHead -> tQuality; // Quality
                tQuality1 = pWaveHead -> tQuality;
                /* 调用时间转换函数 */
                XJP_TM_Dev_To_NS_UTC( & tSec1, & tFrac1, & uWaveTime );
                /* 判断录波类型决定RCD模型中品质因素的测试位状态 */
                if( pWaveHead -> Sof & WAVE_TYPE_SOF_TEST )
                {
                    Quality1 = ( UNSIGNED )QUALITY_TEST;
                }
                else
                {
                    Quality1 = 0;
                }
                for( k = 0; k < iNumber; k ++ )
                {
                    ( pWaveRcd + k ) -> fltNum = FaultIndex;        /* 故障序号 */
                    ( pWaveRcd + k ) -> fltNum_q |= Quality1;       /* 故障序号品质因素 */
                    ( pWaveRcd + k ) -> fltNum_TSec = tSec1;        /* 故障序号时间秒数 */
                    ( pWaveRcd + k ) -> fltNum_TFrac = tFrac1;      /* 故障序号时间毫秒数 */
                    ( pWaveRcd + k ) -> fltNum_TQflag = tQuality1;  /* 故障序号时间品质因素 */
                    ( pWaveRcd + k ) -> GriFltNum = ElecNum;        /* 电网故障序号 */
                    ( pWaveRcd + k ) -> GriFltNum_q |= Quality1;    /* 电网故障序号品质因素 */
                    ( pWaveRcd + k ) -> GriFltNum_TSec = tSec1;     /* 电网故障序号时间秒数 */
                    ( pWaveRcd + k ) -> GriFltNum_TFrac = tFrac1;   /* 电网故障序号时间毫秒数 */
                    ( pWaveRcd + k ) -> GriFltNum_TQflag = tQuality1; /* 电网故障序号时间品质因素 */
                }
            }
        }
    }

    return( XJ_SUCCESS );
}

/* 设置录波任务看门狗句柄 */
INT CWMC_SetWatchDogId( HWDM hWatchDogId )
{
    CWave.hWatchDogId = hWatchDogId;
    return( XJ_SUCCESS );
}
