
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
/*      cemc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CEM - Common Event Management                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common event        */
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
/*      CRMC_Create_Report_Head             Create a report head         */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*      cem_extr.h                          Protect Control functions    */
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
#include "plat/inc/xj_file.h"
#include "plat/inc/xj_cfdc.h"
#include "plat_specific/plat_specific.h"

#define CEMC_MAX_EVENT_RECORD           CPC_MAX_EVENT_RECORD /* 最大事件组数 */
#define CEMC_ALARM_DIR_NAME             CPC_ALARM_DIR_NAME   /* 告警事件存储路径 */
#define CEMC_REVERSE_DIR_NAME           CPC_REVERSE_DIR_NAME /* 内部事件存储路径 */
#define CEMC_OPERATE_DIR_NAME           CPC_OPERATE_DIR_NAME /* 操作事件存储路径 */
#define CEMC_EVENT_ERR_SRAM             CPC_EVENT1_ERR_SRAM  /* EEPROM出错 */
#define CEMC_EVENT_TIMEOUT              CPC_EVENT_TIMEOUT    /* 事件超时时间 */
#define CEMC_MAX_NVRAM_EVENT            CPC_MAX_NVRAM_EVENT_NUMBER /* 最大存储事件数目 */
/*==============================================================*/
/* 全局变量申明 */
extern ESP_CFG PLAT_cfg;
extern HDCM RamEventHandle;
extern HDCM NVRamEventHandle;
extern NU_EVENT_GROUP Event_1_CB;
extern XJ_PROTECT pReport_Protect;

/* 用户事件定义 */
CEVENT PRO_EventRecord[CEMC_MAX_EVENT_RECORD];
/* 事件设备驱动 */
XJ_DRIVER CEMC_hEventDriver = XJ_NULL;
/* 事件信号量 */
XJ_SEMAPHORE CEMC_hSemaphore = XJ_NULL;
/* 告警事件存储路径 */
CHAR AlarmEventDirName[256] = CEMC_ALARM_DIR_NAME;
/* 内部事件存储路径 */
CHAR ReserveEventDirName[256] = CEMC_REVERSE_DIR_NAME;
/* 操作事件事件存储路径 */
CHAR OperateEventDirName[256] = CEMC_OPERATE_DIR_NAME;

/* 宏定义 */
#define CEMC_GET_USER_EVENT_PTR( x ) x = PRO_EventRecord;  // 用户事件地址指针
#define CEMC_GET_USER_EVENT_GROUPS() (PLAT_cfg.history.UserEventGroups) //用户事件组数
#define CEMC_CREATE_EVENT_DRIVER(x) x=XJ_Create_Driver("Event",CEMC_EventDriverEntry) //创建事件驱动
#define CEMC_GET_EVENT_HANDLE() CEMC_hEventDriver //获取事件句柄
#define CEMC_SET_EVENT_HANDLE(x) CEMC_hEventDriver = x //设置事件句柄
#define CEMC_CREATE_EVENT_LOCK(x) x=XJ_Create_Semaphore("Event",1,XJ_FIFO) //创建事件互斥量
#define CEMC_GET_EVENT_LOCK() CEMC_hSemaphore //获取事件互斥量句柄
#define CEMC_SET_EVENT_LOCK(x) CEMC_hSemaphore = x //设置事件互斥量句柄
#define CEMC_LOCK_EVENT(x,y) x=XJP_Mutex_Obtain(CEMC_hSemaphore,y) //获取事件互斥量
#define CEMC_UNLOCK_EVENT(x) x=XJP_Mutex_Release(CEMC_hSemaphore) //释放事件互斥量
#define CEMC_GET_EVENT_ERROR(x,y) x=XJ_Retrieve_Events(&Event_1_CB,CEMC_EVENT_ERR_SRAM, \
          XJ_AND,y, XJ_NO_SUSPEND) //事件硬件错误
#define CEMC_PROTECT_EVENT() XJ_Protect(pReport_Protect)  //事件数据保护 */
#define CEMC_UNPROTECT_EVENT() XJ_Unprotect()  //释放事件数据保护 */
#define CEMC_GET_TIME(x) XJP_TM_Get_Time_Dev(x) //获取取时间
#define CEMC_GET_EVENT_SIZE() (sizeof(CEM_NVD)/sizeof(UNSIGNED_CHAR)) //事件大小
#define CEMC_DATA_MASKS8()  0xff //8位数据掩码
#define CEMC_DATA_MASKS16() 0xffff //16位数据掩码
#define CEMC_CHECK_EVENT(x,y) x=XJP_Queue_Find( NVRamEventHandle, y ) //检查事件
#define CEMC_READ_EVENT_BUFFER(x,y) x=XJP_Queue_Read( NVRamEventHandle , y )//读事件数据
#define CEMC_GET_MAX_EVENT_TIME() CEMC_EVENT_TIMEOUT //最大事件超时时间
#define CEMC_MAX_EVENT_FILE_NUMBER (CEMC_MAX_NVRAM_EVENT + 10) //最大事件文件数目
#define CEMC_LISTFILE(x,y,z) xj_ListFile(x,y,z)

/* Define internal function calls.  */
INT CEMC_InitializeEvent( CEVENT * this, INT iType, UNSIGNED uTimeOut );
INT CEMC_ReadEventData( CEVENT * this, UNSIGNED uEventIndex, VOID * pBuffer, UNSIGNED uTimeOut );
INT CEMC_ClearEventData( CEVENT * this, UNSIGNED TimeOut );
INT CEMC_EventInformation( CEVENT * this, UNSIGNED * Type, UNSIGNED * Number,
    UNSIGNED * StartSerial, UNSIGNED * EndSerial, UNSIGNED uTimeOut );
INT CEMC_FindIndexNumber( CEVENT * this, VOID * pBuffer, INT * Actual_Number,
    UNSIGNED uTimeOut );
INT CEMC_WriteEventData( CEVENT * this, VOID * pBuffer, UNSIGNED uTimeOut );
VOID CEMC_EventDriverEntry( XJ_DRIVER hDriver, XJ_DRIVER_REQUEST * pRequest );
INT CEMC_GetEventDataBlock( CHAR * fileName, UNSIGNED Index, UNSIGNED_CHAR * dst, UNSIGNED size );
INT CEMC_WriteEventDataBlock( CHAR * fileName, UNSIGNED_CHAR * src, UNSIGNED size );
int CEMC_ListEventIndexCallBack( struct FileInformation * info, void * context );
INT CEMC_ListEventIndex( CHAR * dirName, HITM eventHandle, UNSIGNED * Dst, UNSIGNED * pActual_size,
    UNSIGNED Number );
INT CEMC_GetEventIndexNumber( CHAR * dirName, HITM eventHandle, UNSIGNED * Dst,
    UNSIGNED * pActual_size, UNSIGNED Number );
int CEMC_DeleteDirFileCallBack( struct FileInformation * info, void * context );
INT CEMC_DeleteDirFile( CHAR * dirName, HITM eventHandle );
INT CEMC_GetEventData( VOID * pBuffer );
INT CEMC_EventConvert( CEM_RAD * pDst, CEM_NVD * pSrc );
int CEMC_FullFileCheckCallBack( struct FileInformation * info, void * context );
INT CEMC_FullFileCheck( CHAR * dirName, HITM eventHandle, UNSIGNED * Dst, UNSIGNED * pDeleteNumber,
    UNSIGNED MaxNumber );
INT CEMC_EventFullFileCheckMain( CEVENT * Event );

CEM_ERR CEMC_ErrInf = 
{
    0,                          /* makeDirCounter */
    0,                          /* openFileR */
    0,                          /* openFileW */
    0,                          /* closeFileR */
    0,                          /* closeFileW */
    0,                          /* readFile */
    0,                          /* writeFile */
    0,                          /* deleteFileW */
    0,                          /* deleteFileL */
    0,                          /* deleteDirFile */
    0,                          /* deleteCheckFile */
    0,                          /* deleteFullFile */
    0,                          /* getInfoFile */
    0,                          /* writeEvent */
    0,                          /* renameFile */
    0,                          /* runTimes */
    0,                          /* createDriver */
    0,                          /* createSemaphore */
};

CEVENT CAlarmEvent = 
{
    0,                          /* InitFlag */
    CEMC_MAX_NVRAM_EVENT,       /* MaxNumber */
    0,                          /* Number */
    0,                          /* Serial */
    0,                          /* Serial1 */
    ALARM_EVENT_ID,             /* EventId */
    XJ_NULL,                    /* EventDriver */
    XJ_NULL,                    /* CtrlDriver */
    XJ_NULL,                    /* NVStart */
    XJ_NULL,                    /* NVEnd */
    XJ_NULL,                    /* NVIn */
    XJ_NULL,                    /* NVOut */
    AlarmEventDirName,          /* dirName */
    0,                          /* EventHandle */
    0,                          /* hWatchDogId */

    & CEMC_InitializeEvent,     /* Initialize */
    & CEMC_ReadEventData,       /* Read */
    & CEMC_WriteEventData,      /* Write */
    & CEMC_ClearEventData,      /* Clear */
    & CEMC_EventInformation,    /* Information */
    & CEMC_FindIndexNumber,     /* Find */
};

CEVENT CReserveEvent = 
{
    0,                          /* InitFlag */
    CEMC_MAX_NVRAM_EVENT,       /* MaxNumber */
    0,                          /* Number */
    0,                          /* Serial */
    0,                          /* Serial1 */
    RESERVE_EVENT_ID,           /* EventId */
    XJ_NULL,                    /* EventDriver */
    XJ_NULL,                    /* CtrlDriver */
    XJ_NULL,                    /* NVStart */
    XJ_NULL,                    /* NVEnd */
    XJ_NULL,                    /* NVIn */
    XJ_NULL,                    /* NVOut */
    ReserveEventDirName,        /* dirName */
    0,                          /* EventHandle */
    0,                          /* hWatchDogId */

    & CEMC_InitializeEvent,     /* Initialize */
    & CEMC_ReadEventData,       /* Read */
    & CEMC_WriteEventData,      /* Write */
    & CEMC_ClearEventData,      /* Clear */
    & CEMC_EventInformation,    /* Information */
    & CEMC_FindIndexNumber,     /* Find */
};

CEVENT COperateEvent = 
{
    0,                          /* InitFlag */
    CEMC_MAX_NVRAM_EVENT,       /* MaxNumber */
    0,                          /* Number */
    0,                          /* Serial */
    0,                          /* Serial1 */
    OPERATE_EVENT_ID,           /* EventId */
    XJ_NULL,                    /* EventDriver */
    XJ_NULL,                    /* CtrlDriver */
    XJ_NULL,                    /* NVStart */
    XJ_NULL,                    /* NVEnd */
    XJ_NULL,                    /* NVIn */
    XJ_NULL,                    /* NVOut */
    OperateEventDirName,        /* dirName */
    0,                          /* EventHandle */
    0,                          /* hWatchDogId */

    & CEMC_InitializeEvent,     /* Initialize */
    & CEMC_ReadEventData,       /* Read */
    & CEMC_WriteEventData,      /* Write */
    & CEMC_ClearEventData,      /* Clear */
    & CEMC_EventInformation,    /* Information */
    & CEMC_FindIndexNumber,     /* Find */
};

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CEMC_InitializeEvent                                             */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function initialize the event.                              */
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
INT CEMC_InitializeEvent( CEVENT * this, INT iType, UNSIGNED uTimeOut )
{
    INT status;
    UNSIGNED variable;
    XJ_DRIVER hDriver;
    XJ_SEMAPHORE hSemaphore;

    if( this->InitFlag == 0 )
    {
        /* 创建事件目录 */
        if(( xj_DirExists == XJ_NULL ) || ( xj_MakeDir == XJ_NULL ))
        {
            /* 操作函数不存在，直接返回失败 */
            CEMC_ErrInf.makeDirCounter ++;
            return( XJ_UNAVAILABLE );
        }
        if( xj_DirExists( this->dirName ) == XJ_NULL )
        {
            /* 目录不存在，创建目录 */
            if( xj_MakeDir( this->dirName ) != XJ_SUCCESS )
            {
                CEMC_ErrInf.makeDirCounter ++;
                return( XJ_UNAVAILABLE );
            }
        }
        /* 创建事件驱动 */
        if( CEMC_GET_EVENT_HANDLE() == XJ_NULL )
        {
            CEMC_CREATE_EVENT_DRIVER( hDriver );
            if( hDriver == XJ_NULL )
            {
                /* 创建事件驱动失败 */
                CEMC_ErrInf.createDriver ++;
                return( XJ_UNAVAILABLE );
            }
            CEMC_SET_EVENT_HANDLE( hDriver );
        }
        this -> EventDriver = CEMC_GET_EVENT_HANDLE();
        /* 创建事件互斥量 */
        if( CEMC_GET_EVENT_LOCK() == XJ_NULL )
        {
            CEMC_CREATE_EVENT_LOCK( hSemaphore );
            if( hSemaphore == XJ_NULL )
            {
                /* 创建事件互斥量失败 */
                CEMC_ErrInf.createSemaphore ++;
                return( XJ_UNAVAILABLE );
            }
            CEMC_SET_EVENT_LOCK( hSemaphore );
        }
        /* 创建事件索引表管理句柄 */
        this -> EventHandle = XJP_Index_Create( CEMC_MAX_EVENT_FILE_NUMBER );
        this -> InitFlag = 1;
    }

    /* 打开事件设备 */
    OPENDRIVER( status, CEMC_GET_EVENT_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* 设备初始化 */
        variable = ( this -> EventId ) & CEMC_DATA_MASKS16(); //事件Id
        variable += (( iType & CEMC_DATA_MASKS16()) << 16 );  //擦除标志
        INITIALIZEDRIVER( status, CEMC_GET_EVENT_HANDLE(), variable, uTimeOut );

        /* 关闭事件设备 */
        CLOSEDRIVER( CEMC_GET_EVENT_HANDLE());
    }

    /* Return the completion status.  */
    return( status );
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CEMC_ReadEventData                                               */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function read event data.                                   */
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
INT CEMC_ReadEventData( CEVENT * this, UNSIGNED uEventIndex, VOID * pBuffer,
    UNSIGNED uTimeOut )
{
    INT status;

    if( this -> InitFlag == 0 )
    {
        /* 初始化检查 */
        return( XJ_UNAVAILABLE );
    }

    /* 打开事件设备 */
    OPENDRIVER( status, CEMC_GET_EVENT_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* 读事件数据 */
        READDRIVER( status, CEMC_GET_EVENT_HANDLE(), this->EventId, pBuffer, 0, 0,
            uEventIndex, uTimeOut );

        /* 关闭事件设备 */
        CLOSEDRIVER( CEMC_GET_EVENT_HANDLE());
    }

    /* Return the completion status.  */
    return status;
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CEMC_WriteEventData                                              */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function write event data.                                  */
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
INT CEMC_WriteEventData( CEVENT * this, VOID * pBuffer, UNSIGNED uTimeOut )
{
    INT status;

    if( this -> InitFlag == 0 )
    {
        /* 初始化检查 */
        return( XJ_UNAVAILABLE );
    }

    /* 写事件数据 */
    WRITEDRIVER( status, CEMC_GET_EVENT_HANDLE(), this->EventId, pBuffer, 0,
        CEMC_GET_EVENT_SIZE(), uTimeOut );

    /* Return the completion status.  */
    return status;
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CEMC_ClearEventData                                              */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function clear event data.                                  */
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
INT CEMC_ClearEventData( CEVENT * this, UNSIGNED uTimeOut )
{
    INT status;

    if( this -> InitFlag == 0 )
    {
        /* 初始化检查 */
        return( XJ_UNAVAILABLE );
    }

    /* 打开事件设备 */
    OPENDRIVER( status, CEMC_GET_EVENT_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* 清除事件 */
        status = this->Initialize( this, 1, uTimeOut );

        /* 关闭事件设备 */
        CLOSEDRIVER( CEMC_GET_EVENT_HANDLE());
    }

    /* Return the completion status.  */
    return status;
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CEMC_EventInformation                                            */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function retrieve event information.                        */
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
INT CEMC_EventInformation( CEVENT * this, UNSIGNED * Type, UNSIGNED * Number,
    UNSIGNED * StartSerial, UNSIGNED * EndSerial, UNSIGNED uTimeOut )
{
    INT status;
    UNSIGNED ActualNumber;
    UNSIGNED buffer[CEMC_MAX_NVRAM_EVENT];

    if( this -> InitFlag == 0 )
    {
        /* 初始化检查 */
        return( XJ_UNAVAILABLE );
    }

    /* 打开事件设备 */
    OPENDRIVER( status, CEMC_GET_EVENT_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* 获取事件序号 */
        STATUSDRIVER( status, CEMC_GET_EVENT_HANDLE(), buffer,& ActualNumber,
            this->EventId, uTimeOut );
        /* 事件类型 */
        * Type = this -> EventId;
        /* 总的事件个数 */
        * Number = ActualNumber;
        /* 最新事件序号 */
        if( ActualNumber == 0 )
        {
            * StartSerial = 0;
            * EndSerial = 0;
        }
        else
        {
            * StartSerial = buffer[0];
            * EndSerial = buffer[( ActualNumber - 1 )];
        }

        /* 关闭事件设备 */
        CLOSEDRIVER( CEMC_GET_EVENT_HANDLE());
    }

    /* Return the completion status.  */
    return status;
}

INT CEMC_FindIndexNumber( CEVENT * this, VOID * pBuffer, INT * Actual_Number,
    UNSIGNED uTimeOut )
{
    INT status;                         /* complete status */

    if( this -> InitFlag == 0 )
    {
        /* 初始化检查 */
        return( XJ_UNAVAILABLE );
    }

    /* 打开事件设备 */
    OPENDRIVER( status, CEMC_GET_EVENT_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* 获取事件序号 */
        STATUSDRIVER( status, CEMC_GET_EVENT_HANDLE(), pBuffer,
            ( UNSIGNED *)Actual_Number, this->EventId, uTimeOut );

        /* 关闭事件设备 */
        CLOSEDRIVER( CEMC_GET_EVENT_HANDLE());
    }

    /* Return the completion status.  */
    return( status );
}

/* 读取事件数据 */
INT CEMC_GetEventDataBlock( CHAR * fileName, UNSIGNED Index, UNSIGNED_CHAR * dst, UNSIGNED size )
{
    INT status;
    FILE * fp;
    UNSIGNED readSize;

    status = XJ_UNAVAILABLE;
    /* 读事件数据 */
    fp = xj_fopen( fileName, "rb" );
    if( fp != XJ_NULL )
    {
        /* 读取事件数据 */
        readSize = ( UNSIGNED )xj_fread( dst, 1, size, fp );
        if( xj_fclose( fp ))
        {
            /* 关闭文件失败 */
            CEMC_ErrInf.closeFileR ++;
        }
        if( readSize == size )
        {
            status = XJ_SUCCESS;
        }
        else
        {
            /* 读取文件数据失败 */
            CEMC_ErrInf.readFile ++;
        }
    }
    else
    {
        CEMC_ErrInf.openFileR ++;
    }

    return( status );
}

/* 写事件数据 */
INT CEMC_WriteEventDataBlock( CHAR * fileName, UNSIGNED_CHAR * src, UNSIGNED size )
{
    INT status;
    FILE * fp;
    UNSIGNED writeSize;

    status = XJ_UNAVAILABLE;
    /* 写事件数据 */
    fp = xj_fopen( fileName, "wb" );
    if( fp != XJ_NULL )
    {
        /* 写事件数据 */
        writeSize = ( UNSIGNED )xj_fwrite( src, sizeof( UNSIGNED_CHAR ), size, fp );
        if( xj_fclose( fp ))
        {
            /* 关闭文件失败 */
            CEMC_ErrInf.closeFileW ++;
        }
        if( writeSize == size )
        {
            status = XJ_SUCCESS;
        }
        else
        {
            /* 写文件数据失败 */
            CEMC_ErrInf.writeFile ++;
            if( xj_RemoveFile( fileName ))
            {
                /* 删除文件失败 */
                CEMC_ErrInf.deleteFileW ++;
            }
        }
    }
    else
    {
        CEMC_ErrInf.openFileW ++;
    }

    return( status );
}

/* 事件序号回调函数参数定义 */
#define CEM_INDEX_PARA_NUM      3       /* 参数个数 */
#define CEM_INDEX_O_PARA_NUM    0       /* 参数个数位置偏移 */
#define CEM_INDEX_O_DIR_NAME    1       /* 事件路径名称位置偏移 */
#define CEM_INDEX_O_HANDLE      2       /* 事件索引表句柄偏移 */
/* 事件序号回调函数 */
int CEMC_ListEventIndexCallBack( struct FileInformation * info, void * context )
{
    INT status;
    UNSIGNED variable;
    CHAR * dirName;
    CHAR * eventIndexStr;
    UNSIGNED eventIndex;
    UNSIGNED * p;
    HITM eventHandle;
    CHAR buffer[64]; /* 完整路径名（包括'/'） */

    /* 获取事件序号字符串 */
    p = ( UNSIGNED *)context;
    dirName = ( CHAR *)(*( p + CEM_INDEX_O_DIR_NAME )); /* 事件路径名称 */
    eventHandle = ( HITM )(*( p + CEM_INDEX_O_HANDLE )); /* 事件索引表句柄 */
    sprintf( buffer, "%s/", dirName );
    eventIndexStr = XJP_CSC_Strcmp( info->Name, buffer );
    eventIndex = ( UNSIGNED )( atol( eventIndexStr ));
    if( eventIndex == 0 )
    {
        return( XJ_UNAVAILABLE );
    }
    /* 写索引表数据 */
    XJP_Index_Write( eventHandle, eventIndex, & variable, & status );
    if( status == XJ_TRUE )
    {
        /* 超过最大支持数目，删除事件文件 */
        sprintf( buffer, "%s/%d", dirName, variable );
        if( xj_RemoveFile( buffer ))
        {
            /* 删除文件失败 */
            CEMC_ErrInf.deleteFileL ++;
            /* 增加删除文件列表 */
            CFDC_Delete_File( buffer );
        }
    }

    return( XJ_UNAVAILABLE );
}

/* 获取事件的序号 */
INT CEMC_ListEventIndex( CHAR * dirName, HITM eventHandle, UNSIGNED * Dst,
    UNSIGNED * pActual_size, UNSIGNED Number )
{
    INT status;
    INT ActualNumber;
    UNSIGNED FuncPara[CEM_INDEX_PARA_NUM]; /* 回调函数参数 */

    /* 清除数据 */
    XJP_Index_Clear( eventHandle );
    /* 事件数据列表 */
    FuncPara[CEM_INDEX_O_PARA_NUM] = CEM_INDEX_PARA_NUM; /* 参数个数 */
    FuncPara[CEM_INDEX_O_DIR_NAME] = ( UNSIGNED )dirName; /* 事件路径名称 */
    FuncPara[CEM_INDEX_O_HANDLE] = ( UNSIGNED )eventHandle; /* 事件索引表句柄 */
    ActualNumber = CEMC_LISTFILE( dirName, CEMC_ListEventIndexCallBack, FuncPara );
    /* 获取指定数据个数的索引表数据(从最新开始) */
    status = XJP_Index_List( eventHandle, Dst, pActual_size, Number );

    return( status );
}

/* 获取事件的序号 */
INT CEMC_GetEventIndexNumber( CHAR * dirName, HITM eventHandle, UNSIGNED * Dst,
    UNSIGNED * pActual_size, UNSIGNED Number )
{
    INT status;

    status = XJP_Index_List( eventHandle, Dst, pActual_size, Number );

    return( status );
}

/* 删除事件目录全部文件回调函数参数定义 */
#define CEM_DEL_F_PARA_NUM      2       /* 参数个数 */
#define CEM_DEL_F_O_PARA_NUM    0       /* 参数个数位置偏移 */
#define CEM_DEL_F_O_ACT_NUM     1       /* 删除文件个数 */
/* 删除事件指定目录文件回调函数 */
int CEMC_DeleteDirFileCallBack( struct FileInformation * info, void * context )
{
    UNSIGNED * p;
    UNSIGNED actDelFileNumber;

    /* 删除事件文件 */
    if( xj_RemoveFile( info->Name ) == 0 )
    {
        /* 删除成功 */
        p = ( UNSIGNED *)context;
        actDelFileNumber = *( p + CEM_DEL_F_O_ACT_NUM ); /* 删除的事件个数 */
        *( p + CEM_DEL_F_O_ACT_NUM ) = (++ actDelFileNumber );
    }
    else
    {
        /* 删除目录文件失败 */
        CEMC_ErrInf.deleteDirFile ++;
        /* 增加删除文件列表 */
        CFDC_Delete_File( info->Name );
    }
    if( CAlarmEvent.hWatchDogId )
    {
        XJP_Watchdog_Reset( CAlarmEvent.hWatchDogId ); /* 复位被监视对象 */
    }
    return( XJ_UNAVAILABLE );
}

/* 删除指定目录下的所有文件 */
INT CEMC_DeleteDirFile( CHAR * dirName, HITM eventHandle )
{
    UNSIGNED ActualNumber;
    UNSIGNED FuncPara[CEM_DEL_F_PARA_NUM]; /* 回调函数参数 */

    /* 删除事件文件的回调参数 */
    FuncPara[CEM_DEL_F_O_PARA_NUM] = CEM_DEL_F_PARA_NUM; /* 参数个数 */
    FuncPara[CEM_DEL_F_O_ACT_NUM] = 0; /* 删除事件文件的个数 */
    ActualNumber = ( UNSIGNED )CEMC_LISTFILE( dirName, CEMC_DeleteDirFileCallBack, FuncPara );
    /* 清除事件索引表数据 */
    XJP_Index_Clear( eventHandle );

    if( ActualNumber == FuncPara[CEM_DEL_F_O_ACT_NUM] )
    {
        return( XJ_SUCCESS );
    }
    else
    {
        return( XJ_UNAVAILABLE );
    }
}

/* 事件目录文件数目检查回调函数参数定义 */
#define CEM_FULL_F_PARA_NUM     7       /* 参数个数 */
#define CEM_FULL_F_O_PARA_NUM   0       /* 参数个数位置偏移 */
#define CEM_FULL_F_O_DIR_NAME   1       /* 路径名称位置偏移 */
#define CEM_FULL_F_O_REQ_NUM    2       /* 请求的最大数据个数位置偏移 */
#define CEM_FULL_F_O_ACT_NUM    3       /* 实际数据个数位置偏移 */
#define CEM_FULL_F_O_DST        4       /* 缓冲区地址位置偏移 */
#define CEM_FULL_F_O_DEL_NUM    5       /* 删除文件个数位置偏移 */
#define CEM_FULL_F_O_HANDLE     6       /* 事件索引表句柄 */
/* 事件目录文件数目检查回调函数 */
int CEMC_FullFileCheckCallBack( struct FileInformation * info, void * context )
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
    HITM eventHandle;
    CHAR buffer[64]; /* 完整路径名（包括'/'） */

    /* 获取序号字符串 */
    p = ( UNSIGNED *)context;
    dirName = ( CHAR *)(*( p + CEM_FULL_F_O_DIR_NAME )); /* 路径名称 */
    reqDataNumber = *( p + CEM_FULL_F_O_REQ_NUM ); /* 请求的数据个数 */
    pDst = ( UNSIGNED *)(*( p + CEM_FULL_F_O_DST )); /* 缓冲区地址 */
    eventHandle = ( HITM )(*( p + CEM_FULL_F_O_HANDLE )); /* 事件索引表句柄 */
    sprintf( buffer, "%s/", dirName );
    IndexStr = XJP_CSC_Strcmp( info->Name, buffer );
    Index = ( UNSIGNED )( atol( IndexStr ));
    if( Index == 0 )
    {
        return( XJ_UNAVAILABLE );
    }
    /* 数据升序排列 */
    status = XJP_CSC_Arrage_Insert_Full( pDst, ( p + CEM_FULL_F_O_ACT_NUM ),
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
            actDelFileNumber = *( p + CEM_FULL_F_O_DEL_NUM ); /* 删除的文件个数 */
            *( p + CEM_FULL_F_O_DEL_NUM ) = (++ actDelFileNumber );
        }
        else
        {
            /* 删除检查无效文件失败 */
            CEMC_ErrInf.deleteCheckFile ++;
            /* 增加删除文件列表 */
            CFDC_Delete_File( buffer );
        }
        /* 删除事件索引表数据 */
        XJP_Index_Delete( eventHandle, Index );
    }

    return( XJ_UNAVAILABLE );
}

/* 事件目录文件数目检查 */
INT CEMC_FullFileCheck( CHAR * dirName, HITM eventHandle, UNSIGNED * Dst,
    UNSIGNED * pDeleteNumber, UNSIGNED MaxNumber )
{
    INT ActualNumber;
    UNSIGNED FuncPara[CEM_FULL_F_PARA_NUM]; /* 回调函数参数 */

    /* 数据参数列表 */
    FuncPara[CEM_FULL_F_O_PARA_NUM] = CEM_FULL_F_PARA_NUM; /* 参数个数 */
    FuncPara[CEM_FULL_F_O_DIR_NAME] = ( UNSIGNED )dirName; /* 路径名称 */
    FuncPara[CEM_FULL_F_O_REQ_NUM] = MaxNumber; /* 请求的最大数据个数 */
    FuncPara[CEM_FULL_F_O_ACT_NUM] = 0; /* 实际数据个数 */
    FuncPara[CEM_FULL_F_O_DST] = ( UNSIGNED )Dst; /* 缓冲区地址 */
    FuncPara[CEM_FULL_F_O_DEL_NUM] = 0; /* 删除文件个数 */
    FuncPara[CEM_FULL_F_O_HANDLE] = ( UNSIGNED )eventHandle; /* 事件索引表句柄 */
    ActualNumber = CEMC_LISTFILE( dirName, CEMC_FullFileCheckCallBack, FuncPara );

    if( ActualNumber >= 0 )
    {
        * pDeleteNumber = FuncPara[CEM_FULL_F_O_DEL_NUM];
        return( XJ_SUCCESS );
    }
    else
    {
        * pDeleteNumber = 0;
        return( XJ_UNAVAILABLE );
    }
}

/* 事件目录文件检查主函数 */
static UNSIGNED CEMC_MaxEventFileIndex[CEMC_MAX_EVENT_FILE_NUMBER];
INT CEMC_EventFullFileCheckMain( CEVENT * Event )
{
    INT status;
    UNSIGNED delNumber;
    CEVENT * pEvent;

    pEvent = Event;
    CEMC_FullFileCheck( pEvent->dirName, pEvent->EventHandle,
        CEMC_MaxEventFileIndex, & delNumber, CEMC_MAX_EVENT_FILE_NUMBER );
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

static UNSIGNED CEMC_EventIndexBuf[CEMC_MAX_NVRAM_EVENT];
VOID CEMC_EventDriverEntry( XJ_DRIVER hDriver, XJ_DRIVER_REQUEST * pRequest )
{
    UNSIGNED k;
    INT status;
    UNSIGNED index;
    UNSIGNED size;
    UNSIGNED number;
    UNSIGNED variable;
    UNSIGNED * pEventIndex;
    UNSIGNED_CHAR * destination;
    UNSIGNED_CHAR * p;
    CEVENT * pEvent,* pEventGroup;
    CEM_NVD * pNVEventData;
    struct FileInformation fileInfo;
    CHAR fileName[128]; /* 完整路径名（包括'/'） */
    CHAR tempFileName[128]; /* 临时文件名 */

    switch( pRequest -> nu_function )
    {
        /* 初始化 */
    case XJ_INITIALIZE:
        /* 初始化事件信息 */
        pEvent = XJ_NULL;
        pRequest -> nu_status = XJ_INVALID_ENTRY;
        if(( pRequest->nu_supplemental & CEMC_DATA_MASKS16()) == CAlarmEvent.EventId )
        {
            pEvent = & CAlarmEvent;
            /* 告警事件初始化 */
            pEvent->Number = 0;
            pEvent->Serial = 0;
        }
        else if(( pRequest->nu_supplemental & CEMC_DATA_MASKS16()) == CReserveEvent.EventId )
        {
            pEvent = & CReserveEvent;
            /* 内部事件初始化 */
            pEvent->Number = 0;
            pEvent->Serial = 0;
        }
        else if(( pRequest->nu_supplemental & CEMC_DATA_MASKS16()) == COperateEvent.EventId )
        {
            pEvent = & COperateEvent;
            /* 内部事件初始化 */
            pEvent->Number = 0;
            pEvent->Serial = 0;
        }
        else
        {
            /* 用户事件初始化 */
            CEMC_GET_USER_EVENT_PTR( pEventGroup );
            for( k = 0; k < ( UNSIGNED )CEMC_GET_USER_EVENT_GROUPS(); k ++)
            {
                if(( pRequest->nu_supplemental & CEMC_DATA_MASKS16()) == pEventGroup->EventId )
                {
                    pEvent = pEventGroup;
                    pEventGroup->MaxNumber = CAlarmEvent.MaxNumber;  //最大事件个数
                    pEventGroup->Number = 0;
                    pEventGroup->Serial = 0;
                    pRequest -> nu_status = XJ_SUCCESS;
                    break;
                }
                pEventGroup ++;
            }
        }

        if( pEvent != XJ_NULL )
        {
            /* 清除事件数据 */
            if(((( pRequest->nu_supplemental ) >> 16 ) & CEMC_DATA_MASKS16()) != 0 )
            {
                status = CEMC_DeleteDirFile( pEvent->dirName, pEvent->EventHandle );
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
                /* 获取最新事件索引号 */
                CEMC_ListEventIndex( pEvent->dirName, pEvent->EventHandle, CEMC_EventIndexBuf,
                    & number, CEMC_MAX_NVRAM_EVENT );

                if( number > 0 )
                {
                    pEvent->Serial = CEMC_EventIndexBuf[( number - 1 )];
                }
                pRequest -> nu_status = XJ_SUCCESS;
            }
        }
        break;

        /* 获取事件控制权 */
    case XJ_ASSIGN:
        CEMC_LOCK_EVENT( pRequest -> nu_status, pRequest -> nu_timeout );
        break;

        /* 释放事件控制权 */
    case XJ_RELEASE:
        CEMC_UNLOCK_EVENT( pRequest -> nu_status );
        break;

        /* 读事件数据 */
    case XJ_INPUT:
        pEvent = XJ_NULL;
        if( pRequest -> nu_supplemental == CAlarmEvent.EventId )
        {
            pEvent = & CAlarmEvent;
        }
        else if( pRequest -> nu_supplemental == CReserveEvent.EventId )
        {
            pEvent = & CReserveEvent;
        }
        else if( pRequest -> nu_supplemental == COperateEvent.EventId )
        {
            pEvent = & COperateEvent;
        }
        else
        {
            CEMC_GET_USER_EVENT_PTR( pEventGroup );
            for( k = 0; k < ( UNSIGNED )CEMC_GET_USER_EVENT_GROUPS(); k ++)
            {
                if( pRequest -> nu_supplemental == pEventGroup->EventId )
                {
                    pEvent = pEventGroup;
                    break;
                }
                pEventGroup ++;
            }
        }

        if( pEvent == XJ_NULL )
        {
            pRequest -> nu_status = XJ_INVALID_ENTRY;
            return;
        }

        destination = ( UNSIGNED_CHAR *)( pRequest ->
            nu_request_info.nu_input.nu_buffer_ptr );
        index = ( pRequest->nu_request_info.nu_input.nu_logical_unit ) & CEMC_DATA_MASKS16();
        /* 获取事件信息 */
        sprintf( fileName, "%s/%d", pEvent->dirName, index );
        if( xj_GetFileInformation( fileName,& fileInfo ) == XJ_SUCCESS )
        {
            /* 获取信息成功,读事件数据 */
            if( CEMC_GetEventDataBlock( fileName, index, destination, fileInfo.Size ) == XJ_SUCCESS )
            {
                /* 读取成功 */
                pRequest -> nu_status = XJ_SUCCESS;
            }
            else
            {
                /* 读取失败 */
                pRequest -> nu_status = XJ_END_OF_LOG;
            }
        }
        else
        {
            /* 获取信息失败 */
            CEMC_ErrInf.getInfoFile ++;
            pRequest -> nu_status = XJ_END_OF_LOG;
        }
        break;

        /* 写事件数据 */
    case XJ_OUTPUT:
        pEvent = XJ_NULL;
        if( pRequest -> nu_supplemental == CAlarmEvent.EventId )
        {
            pEvent = & CAlarmEvent;
        }
        else if( pRequest -> nu_supplemental == CReserveEvent.EventId )
        {
            pEvent = & CReserveEvent;
        }
        else if( pRequest -> nu_supplemental == COperateEvent.EventId )
        {
            pEvent = & COperateEvent;
        }
        else
        {
            CEMC_GET_USER_EVENT_PTR( pEventGroup );
            for( k = 0; k < ( UNSIGNED )CEMC_GET_USER_EVENT_GROUPS(); k ++)
            {
                if( pRequest -> nu_supplemental == pEventGroup->EventId )
                {
                    pEvent = pEventGroup;
                    break;
                }
                pEventGroup ++;
            }
        }

        if( pEvent == XJ_NULL )
        {
            pRequest -> nu_status = XJ_INVALID_ENTRY;
            return;
        }

        p = ( UNSIGNED_CHAR *)( pRequest->nu_request_info.nu_output.nu_buffer_ptr );
        size = pRequest -> nu_request_info.nu_output.nu_request_size;
        pNVEventData = ( CEM_NVD *)p;
        index = pNVEventData->IndexL + ( pNVEventData->IndexH << 8 ); /* 16位数据 */
        /* 写事件数据 */
        sprintf( fileName, "%s/%d", pEvent->dirName, index );
        sprintf( tempFileName, "%s/tempEvent", pEvent->dirName );
        if( CEMC_WriteEventDataBlock( tempFileName, p, size ) == XJ_SUCCESS )
        {
            if( xj_RenameFile( tempFileName, fileName )== XJ_SUCCESS )
            {
                /* 写事件成功 */
                pEvent -> Serial = pEvent -> Serial1;
                /* 更新索引表数据 */
                XJP_Index_Write( pEvent -> EventHandle, index, & variable, & status );
                if( status == XJ_TRUE )
                {
                    /* 超过最大支持数目，删除事件文件 */
                    sprintf( fileName, "%s/%d", pEvent->dirName, variable );
                    if( xj_RemoveFile( fileName ))
                    {
                        /* 删除文件失败 */
                        CEMC_ErrInf.deleteFullFile ++;
                        /* 增加删除文件列表 */
                        CFDC_Delete_File( fileName );
                    }
                }
                pRequest -> nu_status = XJ_SUCCESS;
            }
            else
            {
                /* 重命名文件名失败 */
                CEMC_ErrInf.renameFile ++;
                pRequest -> nu_status = XJ_UNAVAILABLE;
            }
        }
        else
        {
            /* 写事件失败 */
            CEMC_ErrInf.writeEvent ++;
            pRequest -> nu_status = XJ_END_OF_LOG;
        }
        break;

    case XJ_STATUS:
        pEvent = XJ_NULL;
        if( pRequest -> nu_supplemental == CAlarmEvent.EventId )
        {
            pEvent = & CAlarmEvent;
        }
        else if( pRequest -> nu_supplemental == CReserveEvent.EventId )
        {
            pEvent = & CReserveEvent;
        }
        else if( pRequest -> nu_supplemental == COperateEvent.EventId )
        {
            pEvent = & COperateEvent;
        }
        else
        {
            CEMC_GET_USER_EVENT_PTR( pEventGroup );
            for( k = 0; k < ( UNSIGNED )CEMC_GET_USER_EVENT_GROUPS(); k ++)
            {
                if( pRequest -> nu_supplemental == pEventGroup->EventId )
                {
                    pEvent = pEventGroup;
                    break;
                }
                pEventGroup ++;
            }
        }

        if( pEvent == XJ_NULL )
        {
            pRequest->nu_request_info.nu_status.nu_logical_unit = 0;
            pRequest -> nu_status = XJ_INVALID_ENTRY;
            return;
        }

        pEventIndex = ( UNSIGNED *)( pRequest->nu_request_info.nu_status.nu_extra_status );
        status = CEMC_GetEventIndexNumber( pEvent->dirName, pEvent->EventHandle, pEventIndex,
            & size, CEMC_MAX_NVRAM_EVENT );
        pRequest->nu_request_info.nu_status.nu_logical_unit = size;
        pRequest -> nu_status = status;
        break;
    }
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CEMC_EventMain                                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function save the event data.                               */
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
/* 获取事件数据 */
INT CEMC_GetEventData( VOID * pBuffer )
{
    CEM_RAD EventData,* pEventSrc;      /* RAM event control block ptr */
    CEM_NVD * pEventDst;                /* NVRAM event control block ptr */
    INT status;                         /* Completion status */

    /* 读事件数据 */
    CEMC_PROTECT_EVENT();
    CEMC_READ_EVENT_BUFFER( status,& EventData );
    CEMC_UNPROTECT_EVENT();

    if( status == XJ_SUCCESS )
    {
        /* 转换事件数据 */
        pEventSrc = ( CEM_RAD *)& EventData;
        pEventDst = ( CEM_NVD *)pBuffer;
        pEventDst -> IndexL = 0;
        pEventDst -> IndexH = 0;
        pEventDst -> Flag = ( UNSIGNED_CHAR )( pEventSrc -> Flag & CEM_BYTE_MASKS );
        strncpy( ( CHAR *)( pEventDst->Name ), ( CHAR *)( pEventSrc->Name ), CEM_EVENT_NAME );
        pEventDst -> Year = ( UNSIGNED_CHAR )( pEventSrc -> Year & CEM_BYTE_MASKS );
        pEventDst -> Month = ( UNSIGNED_CHAR )( pEventSrc -> Month & CEM_BYTE_MASKS );
        pEventDst -> Day = ( UNSIGNED_CHAR )( pEventSrc -> Day & CEM_BYTE_MASKS );
        pEventDst -> Hour = ( UNSIGNED_CHAR )( pEventSrc -> Hour & CEM_BYTE_MASKS );
        pEventDst -> Minute = ( UNSIGNED_CHAR )( pEventSrc -> Minute & CEM_BYTE_MASKS );
        pEventDst -> MillionsecondL = ( UNSIGNED_CHAR )( pEventSrc->Millionsecond & CEM_BYTE_MASKS );
        pEventDst -> MillionsecondH = 
            ( UNSIGNED_CHAR )(( pEventSrc->Millionsecond >> CEM_BYTE_BITS ) & CEM_BYTE_MASKS );
        pEventDst -> EndFlag = ( UNSIGNED_CHAR )CEM_END_FLAG;
    }

    return( status );
}

/* 事件数据转换 */
INT CEMC_EventConvert( CEM_RAD * pDst, CEM_NVD * pSrc )
{
    CEM_RAD * pEventDst;                 /* RAM event control block ptr */
    CEM_NVD * pEventSrc;                 /* NVRAM event control block ptr */

    /* 转换事件数据 */
    pEventSrc = pSrc;
    pEventDst = pDst;
    pEventDst -> Index = ( pEventSrc->IndexH << CEM_BYTE_BITS ) + pEventSrc->IndexL;
    pEventDst -> Flag = ( INT )pEventSrc -> Flag;
    memcpy( pEventDst->Name, pEventSrc->Name, CEM_EVENT_NAME );
    pEventDst -> Year = pEventSrc -> Year;
    pEventDst -> Month = pEventSrc -> Month;
    pEventDst -> Day = pEventSrc -> Day;
    pEventDst -> Hour = pEventSrc -> Hour;
    pEventDst -> Minute = pEventSrc -> Minute;
    pEventDst -> Millionsecond = ( pEventSrc->MillionsecondH << CEM_BYTE_BITS )
        + pEventSrc->MillionsecondL;

    return( XJ_SUCCESS );
}

INT CEMC_EventMain( VOID )
{
    UNSIGNED Size;                      /* buffer size */
    UNSIGNED type;                      /* event type */
    CEVENT * pEventGroup;               /* event control block ptr */
    static CEM_NVD NVEventData;         /* NVRAM event data buffer */
    static CEVENT * pEvent;
    volatile static UNSIGNED GroupNumber = 0;
    CEM_RAD EventData;
    UNSIGNED ActualEvent;
    STATUS Status;
    UNSIGNED k;

    /* 事件错误标志检查 */
    CEMC_GET_EVENT_ERROR( Status,& ActualEvent );
    if( Status == XJ_SUCCESS )
    {
        if( GroupNumber != 0 )
        {
            /* 关闭事件设备 */
            CLOSEDRIVER( CEMC_GET_EVENT_HANDLE());
        }
        GroupNumber = 0;

        /* EEPROM错误,直接上送事件信息 */
        if( CEMC_GetEventData(& NVEventData ) == XJ_SUCCESS )
        {
            CEMC_EventConvert(& EventData,& NVEventData );
            CEMC_PROTECT_EVENT();
            XJP_Queue_Write( RamEventHandle , & EventData );
            CEMC_UNPROTECT_EVENT();
        }
        return( XJ_TRUE );
    }

    CEMC_ErrInf.runTimes ++;

    /* 事件记录 */
    if( GroupNumber == 0 )
    {
        /* 检查事件 */
        CEMC_CHECK_EVENT( Status,& Size );
        if( Status == XJ_TRUE )
        {
            /* 打开事件设备 */
            OPENDRIVER( Status, CEMC_GET_EVENT_HANDLE(), CEMC_GET_MAX_EVENT_TIME());
            if( Status == XJ_SUCCESS )
            {
                /* 读事件数据 */
                if( CEMC_GetEventData(& NVEventData ) == XJ_SUCCESS )
                {
                    type = NVEventData.Flag;
                    /* 查找事件 */
                    pEvent = XJ_NULL;
                    if( type == CAlarmEvent.EventId )
                    {
                        pEvent = & CAlarmEvent;
                    }
                    else if( type == CReserveEvent.EventId )
                    {
                        pEvent = & CReserveEvent;
                    }
                    else if( type == COperateEvent.EventId )
                    {
                        pEvent = & COperateEvent;
                    }
                    else
                    {
                        CEMC_GET_USER_EVENT_PTR( pEventGroup );
                        for( k = 0; k < ( UNSIGNED )CEMC_GET_USER_EVENT_GROUPS(); k ++)
                        {
                            if( type == pEventGroup->EventId )
                            {
                                pEvent = pEventGroup;
                                break;
                            }
                            pEventGroup ++;
                        }
                    }

                    if( pEvent == XJ_NULL )
                    {
                        /* 关闭事件设备 */
                        CLOSEDRIVER( CEMC_GET_EVENT_HANDLE());
                    }
                    else
                    {
                        GroupNumber ++;
                    }
                }
                else
                {
                    /* 关闭事件设备 */
                    CLOSEDRIVER( CEMC_GET_EVENT_HANDLE());
                }
            }
        }
    }

    /* 写事件数据 */
    if( GroupNumber == 1 )
    {
        /* 调整事件序号 */
        pEvent->Serial1 = pEvent->Serial + 1;
        /* 检查事件序号是否在(1~0xFFFF)范围内 */
        CHECKCYCDATALIMIT( pEvent->Serial1, CEM_SERIAL_NUMBER_MIN, CEM_SERIAL_NUMBER_MAX );

        NVEventData.IndexL = ( UNSIGNED_CHAR )pEvent->Serial1 & CEM_BYTE_MASKS;
        NVEventData.IndexH = ( UNSIGNED_CHAR )(( pEvent->Serial1 ) >> CEM_BYTE_BITS ) & CEM_BYTE_MASKS;

        /* 写事件 */
        Status = pEvent->Write( pEvent,& NVEventData, CEMC_GET_MAX_EVENT_TIME());
        if( Status != XJ_TIMEOUT )
        {
            if( Status != XJ_SUCCESS )
            {
                /* 事件文件数目检查 */
                CEMC_EventFullFileCheckMain( pEvent );
            }
            /* 填充就地显示事件 */
            CEMC_EventConvert(& EventData,& NVEventData );
            CEMC_PROTECT_EVENT();
            XJP_Queue_Write( RamEventHandle , & EventData );
            CEMC_UNPROTECT_EVENT();
            GroupNumber ++;
        }
    }

    /* 存事件信息 */
    if( GroupNumber >= 2 )
    {
        GroupNumber = 0;
        /* 关闭事件设备 */
        CLOSEDRIVER( CEMC_GET_EVENT_HANDLE());
    }

    /* Return the completion status.  */
    return XJ_TRUE;
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CEMC_FillEvent                                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function fill event data.                                   */
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
INT CEMC_FillEvent( const VOID * pName, INT iEventType )
{
    CEM_RAD Event;                      /* event control block */
    TIME RunTime,* pTime;               /* time ptr */
    INT status;                         /* Completion status */

    /* Initialize the status as successful.  */
    status =  XJ_SUCCESS;

    CEMC_PROTECT_EVENT();

    Event.Index = 0x00;
    /* 事件类型 */
    Event.Flag = iEventType;
    strncpy( ( CHAR *)( Event.Name ), ( CHAR *)pName, ( CEM_EVENT_NAME - 1 ));
    * ( Event.Name + CEM_EVENT_NAME - 1 ) = 0; /*字符串尾部添加结束符 */
    /* 获取时间 */
    CEMC_GET_TIME(& RunTime );
    pTime = ( TIME *)& RunTime;
    Event.Year = pTime -> Year;
    Event.Month = pTime -> Month;
    Event.Day = pTime -> Day;
    Event.Hour = pTime -> Hour;
    Event.Minute = pTime -> Minute;
    Event.Millionsecond = pTime -> Millionsecond;

    if( iEventType != 0 )
    {
        /* 写存储事件 */
        status = XJP_Queue_Write( NVRamEventHandle , & Event );
    }
    else
    {
        status = XJP_Queue_Write( RamEventHandle , & Event );
    }

    CEMC_UNPROTECT_EVENT();

    /* Return the completion status.  */
    return( status );
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CEMC_FillEventFull                                               */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function fill the full event data.                          */
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
INT CEMC_FillEventFull( const VOID * pName, TIME * pTime, INT iEventType )
{
    CEM_RAD Event;                      /* event control block */
    INT status;                         /* Completion status */

    /* Initialize the status as successful.  */
    status =  XJ_SUCCESS;

    Event.Index = 0x00;
    /* 事件类型 */
    Event.Flag = iEventType;
    strncpy( ( CHAR *)( Event.Name ), ( CHAR *)pName, ( CEM_EVENT_NAME - 1 ));
    * ( Event.Name + CEM_EVENT_NAME - 1 ) = 0; /*字符串尾部添加结束符 */
    Event.Year = pTime -> Year;
    Event.Month = pTime -> Month;
    Event.Day = pTime -> Day;
    Event.Hour = pTime -> Hour;
    Event.Minute = pTime -> Minute;
    Event.Millionsecond = pTime -> Millionsecond;

    if( iEventType != 0 )
    {
        /* 写存储事件 */
        status = XJP_Queue_Write( NVRamEventHandle , & Event );
    }
    else
    {
        /* 写RAM事件 */
        status = XJP_Queue_Write( RamEventHandle , & Event );
    }

    /* Return the completion status.  */
    return( status );
}

/* 设置事件任务看门狗句柄 */
INT CEMC_SetWatchDogId( HWDM hWatchDogId )
{
    CAlarmEvent.hWatchDogId = hWatchDogId;
    CReserveEvent.hWatchDogId = hWatchDogId;
    COperateEvent.hWatchDogId = hWatchDogId;
    return( XJ_SUCCESS );
}
