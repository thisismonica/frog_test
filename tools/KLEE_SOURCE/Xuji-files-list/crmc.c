
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
/*      crmc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CRM - Common Report Management                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common report       */
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
/*      crm_extr.h                          Protect Control functions    */
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
#include "plat/inc/xj_file.h"
#include "plat_specific/plat_specific.h"
#include "plat/es_plat.h"
#include "plat/inc/hrf.h"

#define CRMC_MAX_NVRAM_REPORT           CPC_MAX_NVRAM_REPORT_NUMBER /* 最大存储报告个数 */
#define CRMC_MAX_FAULT_REPORT           CPC_MAX_FAULT_REPORT_NUMBER /* 最大访问故障报告个数 */
#define CRMC_MAX_REPORT_FAULT           CPC_MAX_ONE_FAULT_REPORT_NUMBER /* 一次故障最大报告个数 */
#define CRMC_RES_EVENT_REPORT_CLR       CPC_RES_EVENT_REPORT_CLR    /* 清除报告 */
#define CRMC_REPORT_TIMEOUT             CPC_REPORT_TIMEOUT          /* 报告超时时间 */
#define CRMC_EVENT_REPORT_CLR           CPC_DEV_EVENT2_REPORT_CLR   /* 清报告数据 */
#define CRMC_USER_ROOT_DIR_NAME         CPC_USER_ROOT_DIR_NAME      /* 用户根目录名称 */
/*==============================================================*/
/* 以下为OS相关配置 */
#ifdef WIN32
#include <stdio.h>
#include <malloc.h>
/* 全局变量申明 */
struct RPTMemMService RPTMMS = {malloc, free};
/* Windows 宏定义 */
#endif

#ifndef WIN32 /* 非windows缺省定义一些宏以便通过编译 */
/* 全局变量申明 */
struct RPTMemMService RPTMMS = {malloc, free};
/* Nucleus宏定义 */
#endif

/* 全局变量申明 */
extern XJ_PROTECT pReport_Protect;

/* 报告设备驱动句柄 */
XJ_DRIVER CRMC_hReportDriver = XJ_NULL;
/* 报告信号量 */
XJ_SEMAPHORE CRMC_hSemaphore = XJ_NULL;
/* 报告索引表管理句柄 */
HITM CRMC_ReportHandle;
CSF_NODE * CRMC_Created_Report_List = XJ_NULL; /* 创建报告的链接指针 */
/* 报告看门狗句柄 */
HWDM CRMC_hWatchDogId = 0;
/* 报告设备事件控制块 */
NU_EVENT_GROUP CRMC_EventClrCB;
/* 报告设备事件句柄 */
NU_EVENT_GROUP * CRMC_hEventClrCB = XJ_NULL;

/* 宏定义 */
#define CRMC_CREATE_REPORT_DRIVER(x) x=XJ_Create_Driver("Report",CRMC_ReportDriverEntry) //创建报告驱动
#define CRMC_GET_REPORT_HANDLE() CRMC_hReportDriver //获取报告句柄
#define CRMC_SET_REPORT_HANDLE(x) CRMC_hReportDriver = x //设置报告句柄
#define CRMC_CREATE_CLEAR_EVENT(x) x=NU_Create_Event_Group(&CRMC_EventClrCB,"ClrRpt") //创建报告清除事件
#define CRMC_GET_CLEAR_EVENT_HANDLE() CRMC_hEventClrCB //获取报告清除事件句柄
#define CRMC_SET_CLEAR_EVENT_HANDLE() CRMC_hEventClrCB = &CRMC_EventClrCB //设置报告清除事件句柄
#define CRMC_GET_CLEAR_EVENT(x,y,z) x=XJ_Retrieve_Events(CRMC_hEventClrCB,y,XJ_AND,z,XJ_NO_SUSPEND) //清除报告
#define CRMC_SET_CLEAR_EVENT(x,y) XJ_Set_Events(CRMC_hEventClrCB,x,y) //设置清除报告事件
#define CRMC_CREATE_REPORT_LOCK(x) x=XJ_Create_Semaphore("Report",1,XJ_FIFO) //创建报告互斥量
#define CRMC_GET_REPORT_LOCK() CRMC_hSemaphore //获取报告互斥量句柄
#define CRMC_SET_REPORT_LOCK(x) CRMC_hSemaphore = x //设置报告互斥量句柄
#define CRMC_FILL_RESERVE_EVENT(y) CEMC_FillEvent(y,RESERVE_EVENT_ID) //填充保留事件
#define CRMC_PROTECT_REPORT() XJ_Protect(pReport_Protect)  //事件数据保护 */
#define CRMC_UNPROTECT_REPORT() XJ_Unprotect()  //释放报告数据保护 */
#define CRMC_DATA_MASKS8() 0xff //8位数据掩码
#define CRMC_DATA_BITS8() 8 //8位数据位数
#define CRMC_DATA_MASKS16() 0xffff //16位数据掩码
#define CRMC_DATA_BITS16() 16 //16位数据位数
#define CRMC_LOCK_REPORT(x,y) x=XJP_Mutex_Obtain(CRMC_hSemaphore,y) //获取报告互斥量
#define CRMC_UNLOCK_REPORT(x) x=XJP_Mutex_Release(CRMC_hSemaphore) //释放报告互斥量
#define CRMC_CHECK_DATA_LIMIT CHECKCYCDATALIMIT //检查数据范围
#define CRMC_GET_MAX_REPORT_TIME() CRMC_REPORT_TIMEOUT //最大报告超时时间
#define CRMC_CLEAR_REPORT_BUFFER(x) XJP_Queue_Clear( x ) //清报告缓冲区
#define CRMC_CHECK_REPORT(x,y,z) x=XJP_Queue_Find( y , z ) //检查动作报告
#define CRMC_READ_REPORT_BUFFER(x,y,z) x=XJP_Queue_Read(y , z ) //读报告数据
#define CRMC_COPY_REPORT_DATA(x,y,z) x=XJP_Queue_Write(y , z )  //拷贝报告到缓冲区
#define CRMC_APPEND_REPORT_FILE_NUMBER  10  //附加报告文件数目
#define CRMC_LISTFILE(x,y,z) xj_ListFile(x,y,z)
#define CRMC_GET_REPORT_TYPE_ID(x,y) (x+ (y << CRMC_DATA_BITS8())) /* 获取报告类型标识：CPU+ID */
#define CRMC_MAX_REPORT_CLEAR_NUMBER 5 /* 报告最大清除次数 */

/* Define internal function calls.  */
CREPORT * CRMC_GetReportHandle( INT CpuNumber );
INT CRMC_InitializeReport( CREPORT * this, INT iType, UNSIGNED uTimeOut );
CREPORT * CRMC_Create( INT CpuNumber, INT MaxNumber, CHAR * dirName );
INT CRMC_ReadReportData( CREPORT * this, UNSIGNED ElecNumber, INT startIndex,
    INT IndexNumber, UNSIGNED Offset, UNSIGNED Size, VOID * pBuffer,
    UNSIGNED uTimeOut );
INT CRMC_WriteReportData( CREPORT * this, VOID * pBuffer, UNSIGNED uSize, UNSIGNED uTimeOut );
INT CRMC_ReportInformation( CREPORT * this, UNSIGNED * ElecCounter,
    UNSIGNED * EndFaultCounter, UNSIGNED * StartFaultIndex,
    UNSIGNED * EndFaultIndex, UNSIGNED uTimeOut );
INT CRMC_ReadyReportSerial( CREPORT * this, UNSIGNED * Serial );
INT CRMC_ReadyReportFaultNumber( CREPORT * this, UNSIGNED * FaultNumber );
INT CRMC_ReadyReportInformation( CREPORT * this, UNSIGNED * FaultNumber,
    UNSIGNED * Serial, TIME * Time );
INT CRMC_FindIndexCount( CREPORT * this, INT * Actual_Number,
    UNSIGNED FaultNumber, INT Type, UNSIGNED uTimeOut );
INT CRMC_FindFaultNumber( CREPORT * this, VOID * pBuffer, INT * Actual_Number,
    INT Type, UNSIGNED uTimeOut );
INT CRMC_GetReadyReport( CREPORT * this, UNSIGNED uTimeOut );
INT CRMC_SetReportStatus( CREPORT * this, INT iStatus );
VOID CRMC_ReportDriverEntry( XJ_DRIVER hDriver, XJ_DRIVER_REQUEST * pRequest );
INT CRMC_GetReportData( CREPORT * this, VOID * pBuffer );
INT CRMC_UpdataFaultNumber( CREPORT * this, VOID * pBuffer );
INT CRMC_GetReportDataBlock( CHAR * fileName, UNSIGNED * dst, UNSIGNED size );
INT CRMC_WriteReportDataBlock( CHAR * fileName, UNSIGNED_CHAR * src, UNSIGNED size );
int CRMC_ListReportIndexCallBack( struct FileInformation * info, void * context );
INT CRMC_ListReportIndex( CHAR * dirName, HITM ReportHandle, UNSIGNED * Dst,
    UNSIGNED * pActual_size, UNSIGNED Number );
INT CRMC_GetReportFaultNumber( CHAR * dirName, HITM reportHandle, UNSIGNED * Dst,
    UNSIGNED * pActual_size, UNSIGNED Number );
int CRMC_DeleteDirFileCallBack( struct FileInformation * info, void * context );
INT CRMC_DeleteDirFile( CHAR * dirName, HITM reportHandle );
INT CRMC_GetReportIndexCount( CHAR * dirName, HITM ReportHandle, UNSIGNED * Dst,
    UNSIGNED * pActual_size, UNSIGNED Number, UNSIGNED FaultNumber ) ;
INT CRMC_FindReportFaultIndex( CHAR * dirName, HITM ReportHandle,
    UNSIGNED * reportIndex, UNSIGNED FaultNumber );
int CRMC_FullFileCheckCallBack( struct FileInformation * info, void * context );
INT CRMC_FullFileCheck( CHAR * dirName, HITM reportHandle, UNSIGNED * Dst,
    UNSIGNED * pDeleteNumber, UNSIGNED MaxNumber );
INT CRMC_ReportFullFileCheckMain( CREPORT * this );
INT CRMC_DeleteFaultFile( CREPORT * this, INT FaultNumber );
INT CRMC_ReportWriteMain( CREPORT * this );

CRM_ERR CRMC_ErrInf =
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
    0,                          /* writeReport */
    0,                          /* renameFile */
    0,                          /* runTimes */
    0,                          /* createDriver */
    0,                          /* createSemaphore */
    0,                          /* createClrEvent */
};

CREPORT CReport;
/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CRMC_Create                                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function create report handle.                              */
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
/*      ZhideYang.      05-13-2011      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
CREPORT * CRMC_Create( INT CpuNumber, INT MaxNumber, CHAR * dirName )
{
    INT status;
    CREPORT * this;
    unsigned int MemSize;
    UNSIGNED OneDataSize;
    UNSIGNED TotalDataSize;

    /* 申请内存空间 */
    MemSize = sizeof( CREPORT );
    this = ( CREPORT *)RPTMMS.MemAlloc( MemSize );
    if( this != XJ_NULL )
    {
        /* 初始化数据结构 */
        this -> InitFlag = XJ_FALSE;
        this -> CpuNumber = CpuNumber & CRMC_DATA_MASKS8();
        this -> MaxNumber = MaxNumber;
        this -> Serial = 0;
        this -> Serial1 = 0;
        this -> FaultNumber = 0;
        this -> FaultNumber1 = 0;
        this -> UserFaultNumber = 0;
        this -> ReportId = 0;
        this -> ReportDriver = XJ_NULL;
        this -> CtrlDriver = XJ_NULL;
        this -> NVStart = XJ_NULL;
        this -> NVEnd = XJ_NULL;
        this -> NVIn = XJ_NULL;
        this -> NVOut = XJ_NULL;
        this -> ReportBusy = 0;
        this -> ReadySerialNumber = 0;
        this -> ReadyFaultNumber = 0;
        this -> ReadyNVIn = XJ_NULL;
        this -> ReportHandle = 0;
        this -> MaxReportFileIndex = 0;
        this -> ClearNumber = 0;
        this -> RecordHandle = 0;
        this -> RamReportHandle = 0;
        this -> ReportDataPtr = XJ_NULL;
        this -> GroupNumber = 0;
        this -> SubNumber = 0;
        this -> ReportDataFlag = XJ_FALSE;
        this -> CurFaultNumber = 0;
        memset( &( this->OpTime ), 0, sizeof( TIME ));
        memset( &( this->OpTime1 ), 0, sizeof( TIME ));
        memset( &( this->ReadyOpTime ), 0, sizeof( TIME ));

        /* 申请报告文件索引内存空间 */
        MemSize = sizeof( UNSIGNED ) * ( MaxNumber + CRMC_APPEND_REPORT_FILE_NUMBER );
        this -> ReportFileIndexPtr = ( UNSIGNED *)RPTMMS.MemAlloc( MemSize );
        if( this -> ReportFileIndexPtr != XJ_NULL )
        {
            this -> MaxReportFileIndex = MaxNumber + CRMC_APPEND_REPORT_FILE_NUMBER;
        }
        /* 申请报告数据处理内存空间 */
        MemSize = sizeof( CRM_RFD );
        this -> ReportDataPtr = ( CRM_RFD *)RPTMMS.MemAlloc( MemSize );

        /* 创建报告记录操作对象 */
        this -> RecordHandle = Hrf_Create( this -> CpuNumber, CRMC_MAX_REPORT_FAULT,
            ( sizeof( CRM_RFD )* CRMC_MAX_REPORT_FAULT ), CRM_VERSION );

        /* 初始化动作报告循环缓冲区 */
        OneDataSize = sizeof( CRM_RFD );
        TotalDataSize = CPC_MAX_RAM_REPORT_NUMBER * OneDataSize;
        this->RamReportHandle = XJP_Queue_Create( TotalDataSize, XJP_EVENT_VARIABLE_SIZE,
            OneDataSize );

        /* 创建报告清除事件 */
        if( CRMC_GET_CLEAR_EVENT_HANDLE() == XJ_NULL )
        {
            CRMC_CREATE_CLEAR_EVENT( status );
            if( status == XJ_SUCCESS )
            {
                CRMC_SET_CLEAR_EVENT_HANDLE();
            }
            else
            {
                /* 创建报告清除事件失败 */
                CRMC_ErrInf.createClrEvent ++;
            }
        }

        /* 设置报告存储路径名称 */
        sprintf( this -> dirName, "%s", dirName );

        /* 添加数据到链表 */
        XJP_CSC_Place_On(& CRMC_Created_Report_List, &( this->Link ));

        this -> Initialize = CRMC_InitializeReport;
        this -> Read = CRMC_ReadReportData;
        this -> Write = CRMC_WriteReportData;
        this -> GetReadySerial = CRMC_ReadyReportSerial;
        this -> GetReadyFaultNumber = CRMC_ReadyReportFaultNumber;
        this -> GetReadyInformation = CRMC_ReadyReportInformation;
        this -> Information = CRMC_ReportInformation;
        this -> FindIndexNumber = CRMC_FindIndexCount;
        this -> FindFaultNumber = CRMC_FindFaultNumber;
        this -> Ready = CRMC_GetReadyReport;
        this -> SetReportStatus = CRMC_SetReportStatus;
    }
    return( this );
}

/* 根据CPU号码查找报告管理句柄 */
CREPORT * CRMC_GetReportHandle( INT CpuNumber )
{
    CREPORT * this;
    CSF_NODE * pNode;
    CREPORT * Handle;

    Handle = XJ_NULL;
    if( CRMC_Created_Report_List == XJ_NULL )
    {
        /* 无创建对象，直接返回失败 */
        return( Handle );
    }
    /* 查找指定CPU的报告操作句柄 */
    pNode = CRMC_Created_Report_List;
    do
    {
        this = ( CREPORT *)pNode;
        if( this->CpuNumber == CpuNumber )
        {
            /* 找到定义表,返回找到的定义表首址 */
            Handle = ( CREPORT * )this;
            break;
        }
        pNode = pNode -> next;
    }while( pNode != CRMC_Created_Report_List );

    return( Handle );
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CRMC_InitializeReport                                            */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function initialize the report.                             */
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
INT CRMC_InitializeReport( CREPORT * this, INT iType, UNSIGNED uTimeOut )
{
    INT status;
    UNSIGNED variable;
    XJ_DRIVER hDriver;
    XJ_SEMAPHORE hSemaphore;

    if( this->InitFlag == 0 )
    {
        /* 创建报告目录 */
        if(( xj_DirExists == XJ_NULL ) || ( xj_MakeDir == XJ_NULL ))
        {
            /* 操作函数不存在，直接返回失败 */
            CRMC_ErrInf.makeDirCounter ++;
            return( XJ_UNAVAILABLE );
        }
        if( xj_DirExists( this->dirName ) == XJ_NULL )
        {
            /* 报告目录不存在，创建报告目录 */
            if( xj_MakeDir( this->dirName ) != XJ_SUCCESS )
            {
                CRMC_ErrInf.makeDirCounter ++;
                return( XJ_UNAVAILABLE );
            }
        }
        /* 创建报告驱动 */
        if( CRMC_GET_REPORT_HANDLE() == XJ_NULL )
        {
            CRMC_CREATE_REPORT_DRIVER( hDriver );
            if( hDriver == XJ_NULL )
            {
                /* 创建报告驱动失败 */
                CRMC_ErrInf.createDriver ++;
                return( XJ_UNAVAILABLE );
            }
            CRMC_SET_REPORT_HANDLE( hDriver );
        }
        this -> ReportDriver = CRMC_GET_REPORT_HANDLE();
        /* 创建报告互斥量 */
        if( CRMC_GET_REPORT_LOCK() == XJ_NULL )
        {
            CRMC_CREATE_REPORT_LOCK( hSemaphore );
            if( hSemaphore == XJ_NULL )
            {
                /* 创建报告互斥量失败 */
                CRMC_ErrInf.createSemaphore ++;
                return( XJ_UNAVAILABLE );
            }
            CRMC_SET_REPORT_LOCK( hSemaphore );
        }
        /* 创建报告索引表管理句柄 */
        this -> ReportHandle = XJP_Index_Create( this->MaxReportFileIndex );
        this -> InitFlag = 1;
    }

    /* 打开报告设备 */
    OPENDRIVER( status, CRMC_GET_REPORT_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* 设备初始化 */
        variable = CRMC_GET_REPORT_TYPE_ID( this -> ReportId, this -> CpuNumber );
        variable += (( iType & CRMC_DATA_MASKS16()) << CRMC_DATA_BITS16() );  //擦除标志
        INITIALIZEDRIVER( status, CRMC_GET_REPORT_HANDLE(), variable, uTimeOut );
        /* 关闭报告设备 */
        CLOSEDRIVER( CRMC_GET_REPORT_HANDLE());
    }

    /* Return the completion status.  */
    return( status );
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CRMC_ReadReportData                                              */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function read report data.                                  */
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
INT CRMC_ReadReportData( CREPORT * this, UNSIGNED ElecNumber, INT startIndex,
    INT IndexNumber, UNSIGNED Offset, UNSIGNED Size, VOID * pBuffer,
    UNSIGNED uTimeOut )
{
    INT status;
    UNSIGNED variable;
    UNSIGNED index;

    if( this -> InitFlag == 0 )
    {
        /* 初始化检查 */
        return( XJ_UNAVAILABLE );
    }

    /* 打开报告设备 */
    OPENDRIVER( status, CRMC_GET_REPORT_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* 读报告数据 */
        variable = CRMC_GET_REPORT_TYPE_ID( this -> ReportId, this -> CpuNumber );
        index = ( ElecNumber & CRMC_DATA_MASKS16()) << CRMC_DATA_BITS16(); /* 电网故障序号 */
        index += (( startIndex & CRMC_DATA_MASKS8()) << CRMC_DATA_BITS8()); /* 起始索引号 */
        index += ( IndexNumber & CRMC_DATA_MASKS8()); /* 索引个数 */
        READDRIVER( status, CRMC_GET_REPORT_HANDLE(), variable, pBuffer,
            Offset, Size, index, uTimeOut );
        /* 关闭报告设备 */
        CLOSEDRIVER( CRMC_GET_REPORT_HANDLE());
    }

    /* Return the completion status.  */
    return status;
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CRMC_WriteReportData                                             */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function write report data.                                 */
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
INT CRMC_WriteReportData( CREPORT * this, VOID * pBuffer, UNSIGNED uSize, UNSIGNED uTimeOut )
{
    INT status;
    UNSIGNED variable;

    if( this -> InitFlag == 0 )
    {
        /* 初始化检查 */
        return( XJ_UNAVAILABLE );
    }

    /* 写报告数据 */
    variable = CRMC_GET_REPORT_TYPE_ID( this -> ReportId, this -> CpuNumber );
    WRITEDRIVER( status, CRMC_GET_REPORT_HANDLE(), variable, pBuffer,
        0, uSize, uTimeOut );

    /* Return the completion status.  */
    return status;
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CRMC_ReportInformation                                           */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function retrieve the report information.                   */
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
INT CRMC_ReportInformation( CREPORT * this, UNSIGNED * ElecCounter,
    UNSIGNED * EndFaultCounter, UNSIGNED * StartFaultIndex,
    UNSIGNED * EndFaultIndex, UNSIGNED uTimeOut )
{
    UNSIGNED variable;
    UNSIGNED buffer[CRMC_MAX_FAULT_REPORT]; /* report buffer */
    UNSIGNED Actual_Number = 0;             /* total fault number */
    UNSIGNED Type;                          /* report type */
    INT status;                             /* complete status */

    /* 打开报告设备 */
    OPENDRIVER( status, CRMC_GET_REPORT_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* 查找类型 */
        Type = REPORT_SOE_FLAG + REPORT_STATE_FLAG + REPORT_SCL_FLAG
            + REPORT_LOCAL_FLAG + REPORT_START_FLAG + REPORT_KEEP_T_FLAG;
        /* 获取报告序号 */
        variable = CRMC_GET_REPORT_TYPE_ID( this -> ReportId, this -> CpuNumber );
        variable += (( Type & CRMC_DATA_MASKS16()) << CRMC_DATA_BITS16() );
        STATUSDRIVER( status, CRMC_GET_REPORT_HANDLE(), buffer,& Actual_Number, variable, uTimeOut );
        if( status == XJ_SUCCESS )
        {
            if( Actual_Number == 0 )
            {
                * ElecCounter = 0;
                * StartFaultIndex = 0;
                * EndFaultIndex = 0;
                * EndFaultCounter = 0;
            }
            else
            {
                * ElecCounter = Actual_Number;
                * StartFaultIndex = buffer[0] & CRMC_DATA_MASKS16();
                * EndFaultIndex = buffer[( Actual_Number - 1 )] & CRMC_DATA_MASKS16();
                * EndFaultCounter = ( buffer[( Actual_Number - 1 )] >> CRMC_DATA_BITS16()) & CRMC_DATA_MASKS16();
            }
        }

        /* 关闭报告设备 */
        CLOSEDRIVER( CRMC_GET_REPORT_HANDLE());
    }

    /* Return the completion status.  */
    return status;
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CRMC_ReadyReportSerial                                           */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function get the report serial.                             */
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
INT CRMC_ReadyReportSerial( CREPORT * this, UNSIGNED * Serial )
{
    INT status = XJ_FALSE;              /* complete status */

    /* 检查是否有报告 */
    if( this -> ReadySerialNumber > 0 )
    {
        /* 最新的报告序号 */
        * Serial = this -> ReadySerialNumber;
        status = XJ_TRUE;
    }

    /* Return the completion status.  */
    return status;
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CRMC_ReadyReportFaultNumber                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function get the report fault number.                       */
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
INT CRMC_ReadyReportFaultNumber( CREPORT * this, UNSIGNED * FaultNumber )
{
    INT status = XJ_FALSE;              /* complete status */

    /* 检查是否有故障报告 */
    if( this -> ReadyFaultNumber > 0 )
    {
        /* 最新的故障序号序号 */
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
/*      CRMC_ReadyReportInformation                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function get the report fault number and serial number.     */
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
/*      ZhideYang.      08-02-2011      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
INT CRMC_ReadyReportInformation( CREPORT * this, UNSIGNED * FaultNumber,
    UNSIGNED * Serial, TIME * Time )
{
    INT status = XJ_FALSE;              /* complete status */
    TIME * pTime;
    UNSIGNED intmask;

    /* 检查是否有故障报告 */
    if(( this -> ReadyFaultNumber > 0 ) && ( this -> ReadySerialNumber > 0 ))
    {
        intmask = NU_Control_Interrupts( 0 );
        /* 最新的故障序号序号 */
        * FaultNumber = this -> ReadyFaultNumber;
        * Serial = this -> ReadySerialNumber;
        if( Time )
        {
            pTime = Time;
            pTime->Year = ( this->ReadyOpTime ).Year;
            pTime->Month = ( this->ReadyOpTime ).Month;
            pTime->Day = ( this->ReadyOpTime ).Day;
            pTime->Hour = ( this->ReadyOpTime ).Hour;
            pTime->Minute = ( this->ReadyOpTime ).Minute;
            pTime->Millionsecond = ( this->ReadyOpTime ).Millionsecond;
            pTime->Nanosecond = ( this->ReadyOpTime ).Nanosecond;
            pTime->Quality = ( this->ReadyOpTime ).Quality;
        }
        NU_Control_Interrupts( intmask );

        status = XJ_TRUE;
    }

    /* Return the completion status.  */
    return status;
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CRMC_GetReadyReport                                              */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function get ready report information.                      */
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
INT CRMC_GetReadyReport( CREPORT * this, UNSIGNED uTimeOut )
{
    INT status;                         /* complete status */
    UNSIGNED intmask;

    /* 打开报告设备 */
    OPENDRIVER( status, CRMC_GET_REPORT_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        if( this->ReportBusy == 0 )
        {
            intmask = NU_Control_Interrupts( 0 );
            /* ready fault number */
            this->ReadyFaultNumber = this->FaultNumber;
            this->ReadySerialNumber = this->Serial;
            ( this->ReadyOpTime ).Year = ( this->OpTime ).Year;
            ( this->ReadyOpTime ).Month = ( this->OpTime ).Month;
            ( this->ReadyOpTime ).Day = ( this->OpTime ).Day;
            ( this->ReadyOpTime ).Hour = ( this->OpTime ).Hour;
            ( this->ReadyOpTime ).Minute = ( this->OpTime ).Minute;
            ( this->ReadyOpTime ).Millionsecond = ( this->OpTime ).Millionsecond;
            ( this->ReadyOpTime ).Nanosecond = ( this->OpTime ).Nanosecond;
            ( this->ReadyOpTime ).Quality = ( this->OpTime ).Quality;
            NU_Control_Interrupts( intmask );
        }

        /* 关闭报告设备 */
        CLOSEDRIVER( CRMC_GET_REPORT_HANDLE());
    }

    /* Return the completion status.  */
    return status;
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CRMC_SetReportStatus                                             */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function set report status.                                 */
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
INT CRMC_SetReportStatus( CREPORT * this, INT iStatus )
{
    /* 设置报告状态 */
    this->ReportBusy = iStatus;

    /* Return the completion status.  */
    return XJ_SUCCESS;
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CRMC_FindIndexCount                                              */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function find report data.                                  */
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
INT CRMC_FindIndexCount( CREPORT * this, INT * Actual_Number,
    UNSIGNED FaultNumber, INT Type, UNSIGNED uTimeOut )
{
    INT status;                             /* complete status */
    UNSIGNED variable;                      /* variable */
    UNSIGNED buffer[CRMC_MAX_FAULT_REPORT]; /* report buffer */

    /* 打开报告设备 */
    OPENDRIVER( status, CRMC_GET_REPORT_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        status = CRMC_GetReportIndexCount( this->dirName, this->ReportHandle, buffer,
            ( UNSIGNED *)& variable, CRMC_MAX_FAULT_REPORT, FaultNumber );
        if( status == XJ_SUCCESS )
        {
            if( variable )
                * Actual_Number = buffer[0];
            else
                * Actual_Number = 0;
        }
        /* 关闭报告设备 */
        CLOSEDRIVER( CRMC_GET_REPORT_HANDLE());
    }

    /* Return the completion status.  */
    return( status );
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CRMC_FindFaultNumber                                             */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function find total fault number.                           */
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
INT CRMC_FindFaultNumber( CREPORT * this, VOID * pBuffer, INT * Actual_Number,
    INT Type, UNSIGNED uTimeOut )
{
    UNSIGNED variable;
    INT status;                         /* complete status */

    if( this -> InitFlag == 0 )
    {
        /* 初始化检查 */
        return( XJ_UNAVAILABLE );
    }

    /* 打开报告设备 */
    OPENDRIVER( status, CRMC_GET_REPORT_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* 获取报告序号 */
        variable = CRMC_GET_REPORT_TYPE_ID( this -> ReportId, this -> CpuNumber );
        variable += (( Type & CRMC_DATA_MASKS16()) << CRMC_DATA_BITS16() );
        STATUSDRIVER( status, CRMC_GET_REPORT_HANDLE(), pBuffer,
            ( UNSIGNED *)Actual_Number, variable, uTimeOut );
        /* 关闭报告设备 */
        CLOSEDRIVER( CRMC_GET_REPORT_HANDLE());
    }

    /* Return the completion status.  */
    return( status );
}

/* 读取报告数据块 */
INT CRMC_GetReportDataBlock( CHAR * fileName, UNSIGNED * dst, UNSIGNED size )
{
    INT status;
    FILE * fp;
    UNSIGNED readSize;

    status = XJ_UNAVAILABLE;
    /* 读报告数据 */
    fp = xj_fopen( fileName, "rb" );
    if( fp != XJ_NULL )
    {
        /* 读取报告数据 */
        readSize = ( UNSIGNED )xj_fread( dst, 1, size, fp );
        if( xj_fclose( fp ))
        {
            /* 关闭文件失败 */
            CRMC_ErrInf.closeFileR ++;
        }
        if( readSize == size )
        {
            status = XJ_SUCCESS;
        }
        else
        {
            /* 读取文件数据失败 */
            CRMC_ErrInf.readFile ++;
        }
    }
    else
    {
        CRMC_ErrInf.openFileR ++;
    }

    return( status );
}

/* 写报告数据 */
INT CRMC_WriteReportDataBlock( CHAR * fileName, UNSIGNED_CHAR * src, UNSIGNED size )
{
    INT status;
    FILE * fp;
    UNSIGNED writeSize;

    status = XJ_UNAVAILABLE;
    /* 写报告数据 */
    fp = xj_fopen( fileName, "wb" );
    if( fp != XJ_NULL )
    {
        /* 写报告数据 */
        writeSize = ( UNSIGNED )xj_fwrite( src, sizeof( UNSIGNED_CHAR ), size, fp );
        if( xj_fclose( fp ))
        {
            /* 关闭文件失败 */
            CRMC_ErrInf.closeFileW ++;
        }
        if( writeSize == size )
        {
            status = XJ_SUCCESS;
        }
        else
        {
            /* 写文件数据失败 */
            CRMC_ErrInf.writeFile ++;
            if( xj_RemoveFile( fileName ))
            {
                /* 删除文件失败 */
                CRMC_ErrInf.deleteFileW ++;
            }
        }
    }
    else
    {
        CRMC_ErrInf.openFileW ++;
    }

    return( status );
}

/* 报告故障序号回调函数参数定义 */
#define CRM_LIST_PARA_NUM       3       /* 参数个数 */
#define CRM_LIST_O_PARA_NUM     0       /* 参数个数位置偏移 */
#define CRM_LIST_O_DIR_NAME     1       /* 报告路径名称位置偏移 */
#define CRM_LIST_O_HANDLE       2       /* 报告索引表句柄偏移 */
/* 报告故障序号回调函数 */
int CRMC_ListReportIndexCallBack( struct FileInformation * info, void * context )
{
    INT status;
    UNSIGNED variable;
    CHAR * dirName;
    CHAR * IndexStr;
    UNSIGNED Index;
    HITM reportHandle;
    UNSIGNED * p;
    CHAR buffer[64]; /* 完整路径名（包括'/'） */

    /* 获取报告故障序号字符串 */
    p = ( UNSIGNED *)context;
    dirName = ( CHAR *)(*( p + CRM_LIST_O_DIR_NAME )); /* 报告路径名称 */
    reportHandle = ( HITM )(*( p + CRM_LIST_O_HANDLE )); /* 报告索引表句柄 */
    sprintf( buffer, "%s/", dirName );
    IndexStr = XJP_CSC_Strcmp( info->Name, buffer );
    Index = ( UNSIGNED )( atol( IndexStr ));
    if( Index == 0 )
    {
        return( XJ_UNAVAILABLE );
    }
    /* 写索引表数据 */
    XJP_Index_Write( reportHandle, Index, & variable, & status );
    if( status == XJ_TRUE )
    {
        /* 超过最大支持数目，删除报告文件 */
        sprintf( buffer, "%s/%d", dirName, variable );
        if( xj_RemoveFile( buffer ))
        {
            /* 删除文件失败 */
            CRMC_ErrInf.deleteFileL ++;
        }
    }

    return( XJ_UNAVAILABLE );
}

/* 获取报告索引序号 */
INT CRMC_ListReportIndex( CHAR * dirName, HITM ReportHandle, UNSIGNED * Dst,
    UNSIGNED * pActual_size, UNSIGNED Number )
{
    INT status;
    INT ActualNumber;
    UNSIGNED FuncPara[CRM_LIST_PARA_NUM]; /* 回调函数参数 */

    /* 清除数据 */
    XJP_Index_Clear( ReportHandle );
    /* 数据参数列表 */
    FuncPara[CRM_LIST_O_PARA_NUM] = CRM_LIST_PARA_NUM; /* 参数个数 */
    FuncPara[CRM_LIST_O_DIR_NAME] = ( UNSIGNED )dirName; /* 报告路径名称 */
    FuncPara[CRM_LIST_O_HANDLE] = ( UNSIGNED )ReportHandle; /* 报告索引表句柄 */
    ActualNumber = CRMC_LISTFILE( dirName, CRMC_ListReportIndexCallBack, FuncPara );
    /* 获取指定数据个数的索引表数据(从最新开始) */
    status = XJP_Index_List( ReportHandle, Dst, pActual_size, Number );

    return( status );
}

/* 获取报告故障序号 */
INT CRMC_GetReportFaultNumber( CHAR * dirName, HITM reportHandle, UNSIGNED * Dst,
    UNSIGNED * pActual_size, UNSIGNED Number )
{
    INT status;

    status = XJP_Index_ListL( reportHandle, Dst, pActual_size, Number );

    return( status );
}

/* 删除报告目录全部文件回调函数参数定义 */
#define CRM_DEL_F_PARA_NUM      2       /* 参数个数 */
#define CRM_DEL_F_O_PARA_NUM    0       /* 参数个数位置偏移 */
#define CRM_DEL_F_O_ACT_NUM     1       /* 删除文件个数 */
/* 删除报告指定目录文件回调函数 */
int CRMC_DeleteDirFileCallBack( struct FileInformation * info, void * context )
{
    UNSIGNED * p;
    UNSIGNED actDelFileNumber;

    /* 删除报告文件 */
    if( xj_RemoveFile( info->Name ) == 0 )
    {
        /* 删除成功 */
        p = ( UNSIGNED *)context;
        actDelFileNumber = *( p + CRM_DEL_F_O_ACT_NUM ); /* 删除的报告个数 */
        *( p + CRM_DEL_F_O_ACT_NUM ) = (++ actDelFileNumber );
    }
    else
    {
        /* 删除目录文件失败 */
        CRMC_ErrInf.deleteDirFile ++;
    }
    if( CRMC_hWatchDogId )
    {
        XJP_Watchdog_Reset( CRMC_hWatchDogId ); /* 复位被监视对象 */
    }

    return( XJ_UNAVAILABLE );
}

/* 删除指定目录下的所有文件 */
INT CRMC_DeleteDirFile( CHAR * dirName, HITM reportHandle )
{
    UNSIGNED ActualNumber;
    UNSIGNED FuncPara[CRM_DEL_F_PARA_NUM]; /* 回调函数参数 */

    /* 删除报告文件的回调参数 */
    FuncPara[CRM_DEL_F_O_PARA_NUM] = CRM_DEL_F_PARA_NUM; /* 参数个数 */
    FuncPara[CRM_DEL_F_O_ACT_NUM] = 0; /* 删除报告文件的个数 */
    ActualNumber = ( UNSIGNED )CRMC_LISTFILE( dirName, CRMC_DeleteDirFileCallBack, FuncPara );
    /* 清除报告索引表数据 */
    XJP_Index_Clear( reportHandle );

    if( ActualNumber == FuncPara[CRM_DEL_F_O_ACT_NUM] )
    {
        return( XJ_SUCCESS );
    }
    else
    {
        return( XJ_UNAVAILABLE );
    }
}

/* 获取报告的索引个数 */
INT CRMC_GetReportIndexCount( CHAR * dirName, HITM ReportHandle, UNSIGNED * Dst,
    UNSIGNED * pActual_size, UNSIGNED Number, UNSIGNED FaultNumber )
{
    UNSIGNED k;
    UNSIGNED * p;
    UNSIGNED IndexCount;
    UNSIGNED ActualNumber;
    UNSIGNED CondMasks;
    UNSIGNED CondValue;
    INT status;

    /* 读取指定故障序号的索引表数据 */
    CondMasks = CRMC_DATA_MASKS16();
    CondValue = FaultNumber & CRMC_DATA_MASKS16();
    status = XJP_Index_Read( ReportHandle, Dst, pActual_size, Number, CondMasks, CondValue );
    if( status == XJ_SUCCESS )
    {
        /* 清除索引表中的故障序号，仅保留报告索引号 */
        p = Dst;
        ActualNumber = * pActual_size;
        for( k = 0; k < ActualNumber; k ++)
        {
            IndexCount = ((* p ) >>  CRMC_DATA_BITS16()) & CRMC_DATA_MASKS16();
            * p = IndexCount;
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

/* 查找故障序号的报告索引号 */
INT CRMC_FindReportFaultIndex( CHAR * dirName, HITM ReportHandle,
    UNSIGNED * reportIndex, UNSIGNED FaultNumber )
{
    UNSIGNED Number;
    UNSIGNED ActualNumber;
    UNSIGNED CondMasks;
    UNSIGNED CondValue;
    INT status;

    /* 读取指定报告序号的索引表数据 */
    Number = 1; /* 数据个数 */
    CondMasks = CRMC_DATA_MASKS16();
    CondValue = FaultNumber & CRMC_DATA_MASKS16();
    status = XJP_Index_Read( ReportHandle, reportIndex, & ActualNumber, Number, CondMasks, CondValue );
    if(( status == XJ_SUCCESS ) && ( ActualNumber != 0 ))
    {
        return( XJ_SUCCESS );
    }
    else
    {
        return( XJ_UNAVAILABLE );
    }
}

/* 报告目录文件数目检查回调函数参数定义 */
#define CRM_FULL_F_PARA_NUM     7       /* 参数个数 */
#define CRM_FULL_F_O_PARA_NUM   0       /* 参数个数位置偏移 */
#define CRM_FULL_F_O_DIR_NAME   1       /* 路径名称位置偏移 */
#define CRM_FULL_F_O_REQ_NUM    2       /* 请求的最大数据个数位置偏移 */
#define CRM_FULL_F_O_ACT_NUM    3       /* 实际数据个数位置偏移 */
#define CRM_FULL_F_O_DST        4       /* 故障序号缓冲区地址位置偏移 */
#define CRM_FULL_F_O_DEL_NUM    5       /* 删除文件个数位置偏移 */
#define CRM_FULL_F_O_HANDLE     6       /* 报告索引表句柄 */
/* 报告目录文件数目检查回调函数 */
int CRMC_FullFileCheckCallBack( struct FileInformation * info, void * context )
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
    HITM reportHandle;
    CHAR buffer[64]; /* 完整路径名（包括'/'） */

    /* 获取序号字符串 */
    p = ( UNSIGNED *)context;
    dirName = ( CHAR *)(*( p + CRM_FULL_F_O_DIR_NAME )); /* 路径名称 */
    reqDataNumber = *( p + CRM_FULL_F_O_REQ_NUM ); /* 请求的数据个数 */
    pDst = ( UNSIGNED *)(*( p + CRM_FULL_F_O_DST )); /* 缓冲区地址 */
    reportHandle = ( HITM )(*( p + CRM_FULL_F_O_HANDLE )); /* 报告索引表句柄 */
    sprintf( buffer, "%s/", dirName );
    IndexStr = XJP_CSC_Strcmp( info->Name, buffer );
    Index = ( UNSIGNED )( atol( IndexStr ));
    if( Index == 0 )
    {
        return( XJ_UNAVAILABLE );
    }
    /* 数据升序排列 */
    status = XJP_CSC_Arrage_Insert_Full( pDst, ( p + CRM_FULL_F_O_ACT_NUM ),
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
            actDelFileNumber = *( p + CRM_FULL_F_O_DEL_NUM ); /* 删除的文件个数 */
            *( p + CRM_FULL_F_O_DEL_NUM ) = (++ actDelFileNumber );
        }
        else
        {
            /* 删除检查无效文件失败 */
            CRMC_ErrInf.deleteCheckFile ++;
        }
        /* 删除报告索引表数据 */
        XJP_Index_Delete( reportHandle, InvalidData );
    }
    return( XJ_UNAVAILABLE );
}

/* 报告目录文件数目检查 */
INT CRMC_FullFileCheck( CHAR * dirName, HITM reportHandle, UNSIGNED * Dst,
    UNSIGNED * pDeleteNumber, UNSIGNED MaxNumber )
{
    INT ActualNumber;
    UNSIGNED FuncPara[CRM_FULL_F_PARA_NUM]; /* 回调函数参数 */

    /* 数据参数列表 */
    FuncPara[CRM_FULL_F_O_PARA_NUM] = CRM_FULL_F_PARA_NUM; /* 参数个数 */
    FuncPara[CRM_FULL_F_O_DIR_NAME] = ( UNSIGNED )dirName; /* 路径名称 */
    FuncPara[CRM_FULL_F_O_REQ_NUM] = MaxNumber; /* 请求的最大数据个数 */
    FuncPara[CRM_FULL_F_O_ACT_NUM] = 0; /* 实际数据个数 */
    FuncPara[CRM_FULL_F_O_DST] = ( UNSIGNED )Dst; /* 缓冲区地址 */
    FuncPara[CRM_FULL_F_O_DEL_NUM] = 0; /* 删除文件个数 */
    FuncPara[CRM_FULL_F_O_HANDLE] = ( UNSIGNED )reportHandle; /* 报告索引表句柄 */

    ActualNumber = CRMC_LISTFILE( dirName, CRMC_FullFileCheckCallBack, FuncPara );

    if( ActualNumber >= 0 )
    {
        * pDeleteNumber = FuncPara[CRM_FULL_F_O_DEL_NUM];
        return( XJ_SUCCESS );
    }
    else
    {
        * pDeleteNumber = 0;
        return( XJ_UNAVAILABLE );
    }
}

/* 报告目录文件检查主函数 */
INT CRMC_ReportFullFileCheckMain( CREPORT * this )
{
    INT status;
    UNSIGNED delNumber;

    CRMC_FullFileCheck( this->dirName, this->ReportHandle, this->ReportFileIndexPtr,
        & delNumber, this->MaxReportFileIndex );
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

/* 删除指定故障序号的报告文件 */
INT CRMC_DeleteFaultFile( CREPORT * this, INT FaultNumber )
{
    UNSIGNED k;
    INT status;
    UNSIGNED FaultNumberL;
    UNSIGNED variable;
    UNSIGNED actNumber;
    CHAR fileName[128]; /* 完整路径名（包括'/'） */
    UNSIGNED buffer[ CRMC_MAX_REPORT_FAULT ];

    actNumber = 0;
    /* 获取指定故障序号的报告索引个数 */
    status = CRMC_GetReportIndexCount( this->dirName, this->ReportHandle,
        buffer, & actNumber, CRMC_MAX_REPORT_FAULT, FaultNumber );
    if( status == XJ_SUCCESS )
    {
        FaultNumberL = ( FaultNumber & CRMC_DATA_MASKS16() ) ; /* 低16位：故障序号 */
        for( k = 0; k < actNumber; k ++ )
        {
            variable = FaultNumberL + (( buffer[ k ] & CRMC_DATA_MASKS16()) << CRMC_DATA_BITS16()); /* 高16位：报告索引个数 */
            sprintf( fileName, "%s/%d", CReport.dirName, variable );
            /* 首先判断文件是否存在 */
            if( xj_FileExists( fileName ) == XJ_TRUE )
            {
                if( xj_RemoveFile( fileName ))
                {
                    /* 删除文件失败 */
                    CRMC_ErrInf.deleteFullFile ++;
                }
            }
            /* 删除报告索引表数据 */
            XJP_Index_Delete( this->ReportHandle, variable );
        }
    }
    return status;
}

VOID CRMC_ReportDriverEntry( XJ_DRIVER hDriver, XJ_DRIVER_REQUEST * pRequest )
{
    INT k;
    INT status;
    UNSIGNED index;
    UNSIGNED offset;
    UNSIGNED faultNumber;
    UNSIGNED variable;
    UNSIGNED size;
    UNSIGNED Number;
    INT CpuNumber;
    UNSIGNED * destination;
    CREPORT * this;
    struct FileInformation fileInfo;
    CHAR fileName[128]; /* 完整路径名（包括'/'） */
    CHAR tempFileName[128]; /* 临时文件名 */
    UNSIGNED type;
    UNSIGNED_CHAR * fileBufptr;
    UNSIGNED_CHAR * ucPtr;
    UNSIGNED_CHAR * ucDstPtr;

    switch( pRequest -> nu_function )
    {
        /* 初始化 */
    case XJ_INITIALIZE:
        pRequest -> nu_status = XJ_INVALID_ENTRY;
        /* 根据CPU号码查找报告管理句柄 */
        CpuNumber = ( pRequest->nu_supplemental >> CRMC_DATA_BITS8()) & CRMC_DATA_MASKS8();
        type = ( pRequest -> nu_supplemental >> CRMC_DATA_BITS16()) & CRMC_DATA_MASKS16();
        this = CRMC_GetReportHandle( CpuNumber );
        if( this != XJ_NULL )
        {
            /* 初始化报告信息 */
            this -> Serial = 0;
            this -> FaultNumber = 0;
            this -> UserFaultNumber = this -> FaultNumber;
            this -> ReadyFaultNumber = this -> FaultNumber;
            this -> ReadySerialNumber = this -> Serial;

            /* 清除报告数据 */
            if( type )
            {
                /* 清报告缓冲区 */
                CRMC_PROTECT_REPORT();
                CRMC_CLEAR_REPORT_BUFFER( this->RamReportHandle );
                CRMC_UNPROTECT_REPORT();
                /* 删除全部报告文件 */
                status = CRMC_DeleteDirFile( this->dirName, this->ReportHandle );
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
                /* 获取报告索引序号 */
                CRMC_ListReportIndex( this->dirName, this->ReportHandle,
                    this->ReportFileIndexPtr, & Number, this->MaxReportFileIndex );
                if( Number > 0 )
                {
                    /* 最新报告索引表 */
                    variable = * ( this->ReportFileIndexPtr + Number - 1 );
                    this->FaultNumber =  variable & CRMC_DATA_MASKS16();
                    this->UserFaultNumber = this->FaultNumber;
                    this->ReadyFaultNumber = this->FaultNumber;
                    this->Serial = ( variable >> CRMC_DATA_BITS16()) & CRMC_DATA_MASKS16();
                    this->ReadySerialNumber = this->Serial;
                }
                pRequest -> nu_status = XJ_SUCCESS;
            }
        }
        break;

        /* 获取报告控制权 */
    case XJ_ASSIGN:
        CRMC_LOCK_REPORT( pRequest -> nu_status, pRequest -> nu_timeout );
        break;

        /* 释放报告控制权 */
    case XJ_RELEASE:
        CRMC_UNLOCK_REPORT( pRequest -> nu_status );
        break;

        /* 读报告数据 */
    case XJ_INPUT:
        CpuNumber = ( pRequest->nu_supplemental >> CRMC_DATA_BITS8()) & CRMC_DATA_MASKS8();
        this = CRMC_GetReportHandle( CpuNumber );
        if( this == XJ_NULL )
        {
            pRequest -> nu_status = XJ_INVALID_ENTRY;
            return;
        }

        ucDstPtr = ( UNSIGNED_CHAR *)( pRequest->nu_request_info.nu_input.nu_buffer_ptr );
        offset = pRequest->nu_request_info.nu_input.nu_offset;
        size = pRequest->nu_request_info.nu_input.nu_request_size;
        faultNumber = (( pRequest->nu_request_info.nu_input.nu_logical_unit )
            >> CRMC_DATA_BITS16()) & CRMC_DATA_MASKS16();
        /* 获取故障序号的报告索引号(故障序号(低16位)+报告索引个数(高16位)) */
        if( CRMC_FindReportFaultIndex( this->dirName, this->ReportHandle,& variable, faultNumber ) != XJ_SUCCESS )
        {
            /* 获取报告序号失败 */
            pRequest -> nu_status = XJ_END_OF_LOG;
            return;
        }
        /* 获取报告信息 */
        sprintf( fileName, "%s/%d", this->dirName, variable );
        if( xj_GetFileInformation( fileName,& fileInfo ) == XJ_SUCCESS )
        {
            /* 获取信息成功,读报告数据 */
            fileBufptr = ( UNSIGNED_CHAR *)malloc( fileInfo.Size );
            if( fileBufptr == NU_NULL )
            {
                /* 申请内存失败 */
                pRequest -> nu_status = XJ_END_OF_LOG;
                return;
            }
            if( CRMC_GetReportDataBlock( fileName, ( UNSIGNED *)fileBufptr, fileInfo.Size ) != XJ_SUCCESS )
            {
                /* 读文件失败 */
                free( fileBufptr );
                pRequest -> nu_status = XJ_END_OF_LOG;
                return;
            }
            index = ( pRequest->nu_request_info.nu_input.nu_logical_unit >> CRMC_DATA_BITS8()) & CRMC_DATA_MASKS8();
            /* 起始索引号 */
            Number = pRequest->nu_request_info.nu_input.nu_logical_unit & CRMC_DATA_MASKS8(); /* 索引个数 */
            /* 读取数据 */
            variable = 0;
            for( k = 0; k < ( INT )Number; k ++ )
            {
                ucPtr = ucDstPtr + variable;
                variable += Hrf_GetSectInf( fileBufptr, ( index + k ), offset, size, ucPtr );
            }
            free( fileBufptr );
            if( variable <= ( Number * size ) )
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
            CRMC_ErrInf.getInfoFile ++;
            pRequest -> nu_status = XJ_END_OF_LOG;
        }
        break;

        /* 写报告数据 */
    case XJ_OUTPUT:
        CpuNumber = ( pRequest->nu_supplemental >> CRMC_DATA_BITS8()) & CRMC_DATA_MASKS8();
        this = CRMC_GetReportHandle( CpuNumber );
        if( this == XJ_NULL )
        {
            pRequest -> nu_status = XJ_INVALID_ENTRY;
            return;
        }
        fileBufptr = ( UNSIGNED_CHAR *)( pRequest->nu_request_info.nu_output.nu_buffer_ptr );
        /* 获取节区头个数 */
        Number = Hrf_GetSectHeadCount( ( UNSIGNED_CHAR *)fileBufptr );
        /* 获取报告索引号 */
        index = this->CurFaultNumber & CRMC_DATA_MASKS16(); /* 低16位:故障序号 */
        index += ( Number << CRMC_DATA_BITS16() );       /* 高16位:报告索引个数 */
        /* 写报告数据 */
        size = pRequest -> nu_request_info.nu_output.nu_request_size;
        sprintf( fileName, "%s/%d", this->dirName, index );
        sprintf( tempFileName, "%s/tempReport", this->dirName );
        if( CRMC_WriteReportDataBlock( tempFileName, fileBufptr, size ) == XJ_SUCCESS )
        {
            if( xj_RenameFile( tempFileName, fileName )== XJ_SUCCESS )
            {
                /* 写报告成功,更新故障信息 */
                this->Serial = this->Serial1;
                this->FaultNumber = this->FaultNumber1;
                ( this->OpTime ).Year = ( this->OpTime1 ).Year;
                ( this->OpTime ).Month = ( this->OpTime1 ).Month;
                ( this->OpTime ).Day = ( this->OpTime1 ).Day;
                ( this->OpTime ).Hour = ( this->OpTime1 ).Hour;
                ( this->OpTime ).Minute = ( this->OpTime1 ).Minute;
                ( this->OpTime ).Millionsecond = ( this->OpTime1 ).Millionsecond;
                ( this->OpTime ).Nanosecond = ( this->OpTime1 ).Nanosecond;
                ( this->OpTime ).Quality = ( this->OpTime1 ).Quality;
                /* 更新索引表数据 */
                XJP_Index_Write( this->ReportHandle, index, & variable, & status );
                if( status == XJ_TRUE )
                {
                    /* 超过最大支持数目，删除报告文件 */
                    sprintf( fileName, "%s/%d", this->dirName, variable );
                    if( xj_RemoveFile( fileName ))
                    {
                        /* 删除文件失败 */
                        CRMC_ErrInf.deleteFullFile ++;
                    }
                    /* 删除同一故障的其余文件 */
                    CRMC_DeleteFaultFile( this, ( variable & CRMC_DATA_MASKS16() ));
                }
                pRequest -> nu_status = XJ_SUCCESS;
            }
            else
            {
                /* 重命名文件名失败 */
                CRMC_ErrInf.renameFile ++;
                pRequest -> nu_status = XJ_UNAVAILABLE;
            }
        }
        else
        {
            /* 写报告失败 */
            CRMC_ErrInf.writeReport ++;
            pRequest -> nu_status = XJ_UNAVAILABLE;
        }
        break;

    case XJ_STATUS:
        CpuNumber = ( pRequest->nu_supplemental >> CRMC_DATA_BITS8()) & CRMC_DATA_MASKS8();
        this = CRMC_GetReportHandle( CpuNumber );
        if( this == XJ_NULL )
        {
            pRequest->nu_request_info.nu_status.nu_logical_unit = 0;
            pRequest -> nu_status = XJ_INVALID_ENTRY;
            return;
        }

        if( this->ReadyFaultNumber == 0 )
        {
            pRequest->nu_request_info.nu_status.nu_logical_unit = 0;
            pRequest -> nu_status = XJ_SUCCESS;
        }
        else
        {
            destination = ( UNSIGNED *)( pRequest->nu_request_info.nu_status.nu_extra_status );
            status = CRMC_GetReportFaultNumber( this->dirName, this->ReportHandle,
                destination,& size, CRMC_MAX_FAULT_REPORT );
            /* 检查有效数据个数 */
            size = XJP_CSC_Get_Valid_Number( destination, size, this->ReadyFaultNumber, 1, CPC_DATA_MASKS16 );
            pRequest->nu_request_info.nu_status.nu_logical_unit = size;
            pRequest -> nu_status = status;
        }
        break;
    }
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CRMC_ReportMain                                                  */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function save the report data.                              */
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
/* 获取报告数据 */
INT CRMC_GetReportData( CREPORT * this, VOID * pBuffer )
{
    INT status;                          /* Completion status */

    /* 读报告数据 */
    CRMC_PROTECT_REPORT();
    CRMC_READ_REPORT_BUFFER( status, this->RamReportHandle, pBuffer );
    CRMC_UNPROTECT_REPORT();

    return( status );
}

/* 更新报告序号 */
INT CRMC_UpdataFaultNumber( CREPORT * this, VOID * pBuffer )
{
    CRM_RFD * pReport;
    CRM_RHD * head;                      /* Report head ptr */

    pReport = ( CRM_RFD *)pBuffer;
    this = CRMC_GetReportHandle( pReport->Head.CpuNumber );
    if( this == XJ_NULL )
        return( XJ_UNAVAILABLE );
    head = ( CRM_RHD *)&( pReport->Head );
    if( this->FaultNumber != head->FaultNumber )
    {
        /* 置新故障标志 */
        head -> Type |= REPORT_FAULT_FLAG;
    }
    else
    {
        head -> Type &= (~ REPORT_FAULT_FLAG );
    }

    /* 时间记录 */
    if(( this->FaultNumber1 != head->FaultNumber ) || ( head -> Type & REPORT_START_FLAG ))
    {
        ( this->OpTime1 ).Year = head->Year;
        ( this->OpTime1 ).Month = head->Month;
        ( this->OpTime1 ).Day = head->Day;
        ( this->OpTime1 ).Hour = head->Hour;
        ( this->OpTime1 ).Minute = head->Minute;
        ( this->OpTime1 ).Millionsecond = head->Millionsecond;
        ( this->OpTime1 ).Nanosecond = head->Nanosecond;
        ( this->OpTime1 ).Quality = head->Quality;
    }
    /* 序号调整 */
    this->FaultNumber1 = head->FaultNumber;
    this->Serial1 = ( this->Serial + 1 );

    /* 检查动作报告自然序号是否在(1~0xFFFF)范围内 */
    CRMC_CHECK_DATA_LIMIT( this->Serial1, CRM_FAULT_NUMBER_MIN, CRM_FAULT_NUMBER_MAX );

    /* 填充索引号 */
    head -> Index = this->Serial1;

    return( NU_SUCCESS );
}

INT CRMC_ReportMain( VOID )
{
    STATUS Status;
    CREPORT * this;
    static INT IniFlag = XJ_FALSE;
    static CSF_NODE * pReportNode;
    UNSIGNED RequestEvent;
    UNSIGNED ActualEvent;
    CHAR EventName[CEM_EVENT_NAME];

    if( CRMC_Created_Report_List == XJ_NULL )
    {
        /* 无创建对象，直接返回 */
        return( XJ_TRUE );
    }

    if( IniFlag == XJ_FALSE )
    {
        pReportNode = CRMC_Created_Report_List;
        IniFlag = XJ_TRUE;
    }

    this = ( CREPORT * )pReportNode;
    if( this->InitFlag == 0 )
    {
        /* 等待初始化完成 */
        return( XJ_TRUE );
    }

    /* 清除报告数据检查 */
    if( CRMC_GET_CLEAR_EVENT_HANDLE() != XJ_NULL )
    {
        /* 清除指定模块的报告数据 */
        RequestEvent = 1 << ( this->CpuNumber );
        CRMC_GET_CLEAR_EVENT( Status, RequestEvent,& ActualEvent );
        if( Status == XJ_SUCCESS )
        {
            /* 报告初始化,所有的数据全部丢失 */
            if( this->Initialize( this, 1, CRMC_GET_MAX_REPORT_TIME()) == XJ_SUCCESS )
            {
                this->ClearNumber = 0;
                /* 清除报告成功 */
                sprintf( EventName, " %03d-Ok", CRMC_RES_EVENT_REPORT_CLR );
                CRMC_FILL_RESERVE_EVENT(  EventName );
                CRMC_SET_CLEAR_EVENT( (~ RequestEvent ), XJ_AND );
            }
            else
            {
                /* 最大执行次数 */
                if((++ this->ClearNumber ) >= CRMC_MAX_REPORT_CLEAR_NUMBER )
                {
                    this->ClearNumber = 0;
                    /* 清除报告出错 */
                    sprintf( EventName, " %03d-Err", CRMC_RES_EVENT_REPORT_CLR );
                    CRMC_FILL_RESERVE_EVENT( EventName );
                    CRMC_SET_CLEAR_EVENT( (~ RequestEvent ), XJ_AND );
                }
            }
            if( this->GroupNumber != 0 )
            {
                /* 关闭报告设备 */
                CLOSEDRIVER( CRMC_GET_REPORT_HANDLE());
            }
            /* 清除记录信息 */
            Hrf_ClearRecordInf( this->RecordHandle );
            this->GroupNumber = 0;
            this->SubNumber = 0;
            this->ReportDataFlag = XJ_FALSE;
            this->CurFaultNumber = 0;
            return( XJ_TRUE );
        }
        else
        {
            this->ClearNumber = 0;
        }
    }

    CRMC_ErrInf.runTimes ++;

    /* 报告数据记录主函数 */
    CRMC_ReportWriteMain( this );

    pReportNode = pReportNode -> next;

    /* Return the completion status.  */
    return XJ_TRUE;
}

/* 报告流程定义 */
#define CRMC_PROCESS_CHECK      0         /* 检查故障报告 */
#define CRMC_PROCESS_READY      1         /* 准备故障报告 */
#define CRMC_PROCESS_SAVE       2         /* 保存故障报告 */
#define CRMC_PROCESS_END        3         /* 结束故障报告 */

INT CRMC_ReportWriteMain( CREPORT * this )
{
    UNSIGNED Size;
    STATUS Status;
    UNSIGNED intmask;
    UNSIGNED_CHAR * pBuf;

    if(( this->InitFlag == 0 ) || ( this->ReportDataPtr == XJ_NULL ))
    {
        /* 等待初始化完成 */
        return( XJ_FALSE );
    }

    /* 报告记录 */
    if(( this->GroupNumber == CRMC_PROCESS_CHECK ) && ( this->SubNumber == 0 )) /* 启动报告记录流程 */
    {
        this->CurFaultNumber = 0;
        /* 检查报告记录 */
        CRMC_CHECK_REPORT( Status, this->RamReportHandle, & Size );
        if(( Status == XJ_TRUE ) || ( this -> ReportDataFlag == XJ_TRUE ))
        {
            /* 动作报告,进入准备报告数据流程 */
            this->GroupNumber ++;
            this->SubNumber = 0;
        }
        else
        {
            this->Ready( this, CRMC_GET_MAX_REPORT_TIME());
        }
    }

    if( this->GroupNumber == CRMC_PROCESS_READY ) /* 准备报告数据 */
    {
        if( this -> ReportDataFlag == XJ_FALSE )
        {
            /* 无缓存报告，读取报告数据 */
            if( CRMC_GetReportData( this, this-> ReportDataPtr ) == XJ_SUCCESS )
                this -> ReportDataFlag = XJ_TRUE;
        }
        if( this -> ReportDataFlag == XJ_TRUE )
        {
            /* 有报告,处理动作报告 */
            if(( this->CurFaultNumber )
                && ( this->CurFaultNumber != (( this->ReportDataPtr )->Head ).FaultNumber ))
            {
                /* 不同故障序号,直接进入保存报告数据流程 */
                Hrf_RecordSectorHead( this->RecordHandle, ( unsigned char *)CRM_IDENT_REPORT );
                this->GroupNumber ++;
                this->SubNumber = 0;
            }
            else
            {
                /* 更新报告序号 */

                CRMC_UpdataFaultNumber( this, this-> ReportDataPtr );
                /* 记录节区信息 */
                Size = ( this->ReportDataPtr )->DataSize;
                Hrf_RecordSectorInf_Report( this->RecordHandle, this->ReportDataPtr->DataSize,
                    ( UNSIGNED_CHAR *)this->ReportDataPtr->Data, 0, 1 );
                this->CurFaultNumber = (( this->ReportDataPtr )->Head ).FaultNumber;
                this->ReportDataFlag = XJ_FALSE;
            }
        }
        else
        {
            /* 无报告,检查故障结束 */
            if( this->ReportBusy == XJ_FALSE )
            {
                /* 故障返回,直接进入保存报告数据流程 */
                Hrf_RecordSectorHead( this->RecordHandle, ( unsigned char *)CRM_IDENT_REPORT );
                this->GroupNumber ++;
                this->SubNumber = 0;
            }
        }
    }

    if( this->GroupNumber == CRMC_PROCESS_SAVE ) /* 保存报告数据 */
    {
        if( this->SubNumber == 0 ) /* 获取报告操作句柄 */
        {
            /* 打开报告设备 */
            OPENDRIVER( Status, CRMC_GET_REPORT_HANDLE(), CRMC_GET_MAX_REPORT_TIME());
            if( Status == XJ_SUCCESS )
                this->SubNumber ++;
        }
        if( this->SubNumber == 1 ) /* 保存报告数据 */
        {
            Status = Hrf_GetRecordInf( this->RecordHandle, & pBuf, & Size );
            if( Status == XJ_SUCCESS )
            {
                Status = this->Write( this, pBuf, Size, CRMC_GET_MAX_REPORT_TIME());
                if( Status != XJ_TIMEOUT )
                {
                    if( Status != XJ_SUCCESS )
                    {
                        /* 报告文件数目检查 */
                        CRMC_ReportFullFileCheckMain( this );
                    }
                    /* 清除记录信息 */
                    Hrf_ClearRecordInf( this->RecordHandle );
                    this->GroupNumber ++;
                    this->SubNumber = 0;
                }
            }
        }
    }

    if( this->GroupNumber >= CRMC_PROCESS_END ) /* 结束故障报告 */
    {
        /* 清除记录信息 */
        Hrf_ClearRecordInf( this->RecordHandle );
        this->GroupNumber = 0;
        this->SubNumber = 0;
        /* 更新当前故障信息 */
        intmask = NU_Control_Interrupts( 0 );
        this->ReadyFaultNumber = this->FaultNumber;
        this->ReadySerialNumber = this->Serial;
        ( this->ReadyOpTime ).Year = ( this->OpTime ).Year;
        ( this->ReadyOpTime ).Month = ( this->OpTime ).Month;
        ( this->ReadyOpTime ).Day = ( this->OpTime ).Day;
        ( this->ReadyOpTime ).Hour = ( this->OpTime ).Hour;
        ( this->ReadyOpTime ).Minute = ( this->OpTime ).Minute;
        ( this->ReadyOpTime ).Millionsecond = ( this->OpTime ).Millionsecond;
        ( this->ReadyOpTime ).Nanosecond = ( this->OpTime ).Nanosecond;
        ( this->ReadyOpTime ).Quality = ( this->OpTime ).Quality;
        NU_Control_Interrupts( intmask );
        /* 关闭报告设备 */
        CLOSEDRIVER( CRMC_GET_REPORT_HANDLE());
    }

    /* Return the completion status.  */
    return XJ_TRUE;
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CRMC_FillReport                                                  */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function fill report data.                                  */
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
INT CRMC_FillReport( INT CpuNumber, UNSIGNED FaultNumber, INT Type,
    UNSIGNED Size, const VOID * Src, TIME * pTime )
{
    CRM_RFD Report;
    UNSIGNED DataWordNumber;
    UNSIGNED * pSrc,* pDst, i;
    CREPORT * this;
    INT status;

    /* 清数据缓冲区 */
    memset(( VOID *)& Report, 0, sizeof( CRM_RFD ));
    status =  XJ_UNAVAILABLE;
    /* 报告容量检查 */
    DataWordNumber = ( Size + sizeof( UNSIGNED ) - 1 )/ sizeof( UNSIGNED ); /* 用户数据字数 */
    if( DataWordNumber <= (( UNSIGNED ) CRM_MAX_REPORT_SIZE ))
    {
        Report.Head.CpuNumber = CpuNumber;
        /* 填充报告头部信息 */
        ( Report.Head ).Size = ( sizeof( CRM_RFD ) + sizeof( UNSIGNED ) - 1 )/ sizeof( UNSIGNED );
        ( Report.Head ).Size -= ( CRM_MAX_REPORT_SIZE - DataWordNumber );
        ( Report.Head ).Index = 0;
        ( Report.Head ).FaultNumber = FaultNumber;
        ( Report.Head ).Type = Type;
        ( Report.Head ).Year = pTime->Year;
        ( Report.Head ).Month = pTime->Month;
        ( Report.Head ).Day = pTime->Day;
        ( Report.Head ).Hour = pTime->Hour;
        ( Report.Head ).Minute = pTime->Minute;
        ( Report.Head ).Millionsecond = pTime->Millionsecond;
        ( Report.Head ).Nanosecond = pTime->Nanosecond;
        ( Report.Head ).Quality = pTime->Quality;
        /* 填充数据信息 */
        Report.DataSize = Size;
        pSrc = ( UNSIGNED *)Src;
        pDst = Report.Data;
        for( i = 0; i < DataWordNumber; i ++)
        {
            * pDst ++ = * pSrc ++;
        }
        /* 结束标志 */
        * pDst = CRM_END_FLAG;

        this = CRMC_GetReportHandle( CpuNumber );

        /* 拷贝报告到缓冲区 */
        CRMC_COPY_REPORT_DATA( status, this->RamReportHandle, & Report );
    }

    /* Return the completion status.  */
    return( status );
}

/* 读取报告故障序号 */
INT CRMC_ReadFaultNumber( INT CpuNumber )
{
    CREPORT * this;
    INT iFaultNumber;

    iFaultNumber = 0;
    this = CRMC_GetReportHandle( CpuNumber );
    if( this != XJ_NULL )
    {
        iFaultNumber = this->UserFaultNumber;
    }

    return( iFaultNumber );
}

/* 自动调整报告故障序号 */
INT CRMC_AdjustFaultNumber( INT CpuNumber )
{
    CREPORT * this;
    INT iFaultNumber;
    UNSIGNED Variable;

    iFaultNumber = 0;
    this = CRMC_GetReportHandle( CpuNumber );
    if( this != XJ_NULL )
    {
        Variable = this->UserFaultNumber;
        Variable ++;

        /* 检查动作报告故障序号是否在(1~0xFFFF)范围内 */
        CRMC_CHECK_DATA_LIMIT( Variable, CRM_FAULT_NUMBER_MIN, CRM_FAULT_NUMBER_MAX );

        this->UserFaultNumber = Variable;
        iFaultNumber = ( INT )Variable;
    }

    return( iFaultNumber );
}

/* 设置报告故障序号 */
INT CRMC_SetFaultNumber( INT CpuNumber, INT iFaultNumber )
{
    CREPORT * this;
    UNSIGNED Variable;

    Variable = ( UNSIGNED )iFaultNumber;
    /* 检查动作报告故障序号是否在(1~0xFFFF)范围内 */
    CRMC_CHECK_DATA_LIMIT( Variable, CRM_FAULT_NUMBER_MIN, CRM_FAULT_NUMBER_MAX );
    this = CRMC_GetReportHandle( CpuNumber );
    if( this != XJ_NULL )
    {
        this->UserFaultNumber = Variable;
    }

    return(( INT )Variable );
}

/* 设置报告故障状态 */
INT CRMC_SetFaultStatus( INT CpuNumber, INT iFaultStatus )
{
    CREPORT * this;

    this = CRMC_GetReportHandle( CpuNumber );
    if( this != XJ_NULL )
    {
        CRMC_SetReportStatus( this, iFaultStatus );
    }

    return( XJ_SUCCESS );
}

/* 设置报告任务看门狗句柄 */
INT CRMC_SetWatchDogId( HWDM hWatchDogId )
{
    CRMC_hWatchDogId = hWatchDogId;

    return( XJ_SUCCESS );
}

/* 清除故障报告数据 */
INT CRMC_ClearReport( INT CpuNumber )
{
    INT status;
    UNSIGNED RequestEvent;

    RequestEvent = 1 << CpuNumber;
    status = CRMC_SET_CLEAR_EVENT( RequestEvent, XJ_OR );

    return( status );
}

/* 应用读取动作报告数据函数 */
INT CRMC_UserReadReportData( INT CpuNumber , UNSIGNED ElecNumber, INT startIndex,
    INT IndexNumber, UNSIGNED Offset, UNSIGNED Size, VOID * pBuffer,
    UNSIGNED uTimeOut )
{
    CREPORT  * pReport;
    INT iStatus = XJ_UNAVAILABLE;

    pReport = CRMC_GetReportHandle( CpuNumber );
    if( pReport != NU_NULL )
    {
        iStatus = CRMC_ReadReportData( pReport, ElecNumber, startIndex,
            IndexNumber, Offset, Size, pBuffer, uTimeOut );
    }
    return( iStatus );
}
