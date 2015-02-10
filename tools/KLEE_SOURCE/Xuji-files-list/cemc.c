
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

#define CEMC_MAX_EVENT_RECORD           CPC_MAX_EVENT_RECORD /* ����¼����� */
#define CEMC_ALARM_DIR_NAME             CPC_ALARM_DIR_NAME   /* �澯�¼��洢·�� */
#define CEMC_REVERSE_DIR_NAME           CPC_REVERSE_DIR_NAME /* �ڲ��¼��洢·�� */
#define CEMC_OPERATE_DIR_NAME           CPC_OPERATE_DIR_NAME /* �����¼��洢·�� */
#define CEMC_EVENT_ERR_SRAM             CPC_EVENT1_ERR_SRAM  /* EEPROM���� */
#define CEMC_EVENT_TIMEOUT              CPC_EVENT_TIMEOUT    /* �¼���ʱʱ�� */
#define CEMC_MAX_NVRAM_EVENT            CPC_MAX_NVRAM_EVENT_NUMBER /* ���洢�¼���Ŀ */
/*==============================================================*/
/* ȫ�ֱ������� */
extern ESP_CFG PLAT_cfg;
extern HDCM RamEventHandle;
extern HDCM NVRamEventHandle;
extern NU_EVENT_GROUP Event_1_CB;
extern XJ_PROTECT pReport_Protect;

/* �û��¼����� */
CEVENT PRO_EventRecord[CEMC_MAX_EVENT_RECORD];
/* �¼��豸���� */
XJ_DRIVER CEMC_hEventDriver = XJ_NULL;
/* �¼��ź��� */
XJ_SEMAPHORE CEMC_hSemaphore = XJ_NULL;
/* �澯�¼��洢·�� */
CHAR AlarmEventDirName[256] = CEMC_ALARM_DIR_NAME;
/* �ڲ��¼��洢·�� */
CHAR ReserveEventDirName[256] = CEMC_REVERSE_DIR_NAME;
/* �����¼��¼��洢·�� */
CHAR OperateEventDirName[256] = CEMC_OPERATE_DIR_NAME;

/* �궨�� */
#define CEMC_GET_USER_EVENT_PTR( x ) x = PRO_EventRecord;  // �û��¼���ַָ��
#define CEMC_GET_USER_EVENT_GROUPS() (PLAT_cfg.history.UserEventGroups) //�û��¼�����
#define CEMC_CREATE_EVENT_DRIVER(x) x=XJ_Create_Driver("Event",CEMC_EventDriverEntry) //�����¼�����
#define CEMC_GET_EVENT_HANDLE() CEMC_hEventDriver //��ȡ�¼����
#define CEMC_SET_EVENT_HANDLE(x) CEMC_hEventDriver = x //�����¼����
#define CEMC_CREATE_EVENT_LOCK(x) x=XJ_Create_Semaphore("Event",1,XJ_FIFO) //�����¼�������
#define CEMC_GET_EVENT_LOCK() CEMC_hSemaphore //��ȡ�¼����������
#define CEMC_SET_EVENT_LOCK(x) CEMC_hSemaphore = x //�����¼����������
#define CEMC_LOCK_EVENT(x,y) x=XJP_Mutex_Obtain(CEMC_hSemaphore,y) //��ȡ�¼�������
#define CEMC_UNLOCK_EVENT(x) x=XJP_Mutex_Release(CEMC_hSemaphore) //�ͷ��¼�������
#define CEMC_GET_EVENT_ERROR(x,y) x=XJ_Retrieve_Events(&Event_1_CB,CEMC_EVENT_ERR_SRAM, \
          XJ_AND,y, XJ_NO_SUSPEND) //�¼�Ӳ������
#define CEMC_PROTECT_EVENT() XJ_Protect(pReport_Protect)  //�¼����ݱ��� */
#define CEMC_UNPROTECT_EVENT() XJ_Unprotect()  //�ͷ��¼����ݱ��� */
#define CEMC_GET_TIME(x) XJP_TM_Get_Time_Dev(x) //��ȡȡʱ��
#define CEMC_GET_EVENT_SIZE() (sizeof(CEM_NVD)/sizeof(UNSIGNED_CHAR)) //�¼���С
#define CEMC_DATA_MASKS8()  0xff //8λ��������
#define CEMC_DATA_MASKS16() 0xffff //16λ��������
#define CEMC_CHECK_EVENT(x,y) x=XJP_Queue_Find( NVRamEventHandle, y ) //����¼�
#define CEMC_READ_EVENT_BUFFER(x,y) x=XJP_Queue_Read( NVRamEventHandle , y )//���¼�����
#define CEMC_GET_MAX_EVENT_TIME() CEMC_EVENT_TIMEOUT //����¼���ʱʱ��
#define CEMC_MAX_EVENT_FILE_NUMBER (CEMC_MAX_NVRAM_EVENT + 10) //����¼��ļ���Ŀ
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
        /* �����¼�Ŀ¼ */
        if(( xj_DirExists == XJ_NULL ) || ( xj_MakeDir == XJ_NULL ))
        {
            /* �������������ڣ�ֱ�ӷ���ʧ�� */
            CEMC_ErrInf.makeDirCounter ++;
            return( XJ_UNAVAILABLE );
        }
        if( xj_DirExists( this->dirName ) == XJ_NULL )
        {
            /* Ŀ¼�����ڣ�����Ŀ¼ */
            if( xj_MakeDir( this->dirName ) != XJ_SUCCESS )
            {
                CEMC_ErrInf.makeDirCounter ++;
                return( XJ_UNAVAILABLE );
            }
        }
        /* �����¼����� */
        if( CEMC_GET_EVENT_HANDLE() == XJ_NULL )
        {
            CEMC_CREATE_EVENT_DRIVER( hDriver );
            if( hDriver == XJ_NULL )
            {
                /* �����¼�����ʧ�� */
                CEMC_ErrInf.createDriver ++;
                return( XJ_UNAVAILABLE );
            }
            CEMC_SET_EVENT_HANDLE( hDriver );
        }
        this -> EventDriver = CEMC_GET_EVENT_HANDLE();
        /* �����¼������� */
        if( CEMC_GET_EVENT_LOCK() == XJ_NULL )
        {
            CEMC_CREATE_EVENT_LOCK( hSemaphore );
            if( hSemaphore == XJ_NULL )
            {
                /* �����¼�������ʧ�� */
                CEMC_ErrInf.createSemaphore ++;
                return( XJ_UNAVAILABLE );
            }
            CEMC_SET_EVENT_LOCK( hSemaphore );
        }
        /* �����¼������������ */
        this -> EventHandle = XJP_Index_Create( CEMC_MAX_EVENT_FILE_NUMBER );
        this -> InitFlag = 1;
    }

    /* ���¼��豸 */
    OPENDRIVER( status, CEMC_GET_EVENT_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* �豸��ʼ�� */
        variable = ( this -> EventId ) & CEMC_DATA_MASKS16(); //�¼�Id
        variable += (( iType & CEMC_DATA_MASKS16()) << 16 );  //������־
        INITIALIZEDRIVER( status, CEMC_GET_EVENT_HANDLE(), variable, uTimeOut );

        /* �ر��¼��豸 */
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
        /* ��ʼ����� */
        return( XJ_UNAVAILABLE );
    }

    /* ���¼��豸 */
    OPENDRIVER( status, CEMC_GET_EVENT_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* ���¼����� */
        READDRIVER( status, CEMC_GET_EVENT_HANDLE(), this->EventId, pBuffer, 0, 0,
            uEventIndex, uTimeOut );

        /* �ر��¼��豸 */
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
        /* ��ʼ����� */
        return( XJ_UNAVAILABLE );
    }

    /* д�¼����� */
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
        /* ��ʼ����� */
        return( XJ_UNAVAILABLE );
    }

    /* ���¼��豸 */
    OPENDRIVER( status, CEMC_GET_EVENT_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* ����¼� */
        status = this->Initialize( this, 1, uTimeOut );

        /* �ر��¼��豸 */
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
        /* ��ʼ����� */
        return( XJ_UNAVAILABLE );
    }

    /* ���¼��豸 */
    OPENDRIVER( status, CEMC_GET_EVENT_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* ��ȡ�¼���� */
        STATUSDRIVER( status, CEMC_GET_EVENT_HANDLE(), buffer,& ActualNumber,
            this->EventId, uTimeOut );
        /* �¼����� */
        * Type = this -> EventId;
        /* �ܵ��¼����� */
        * Number = ActualNumber;
        /* �����¼���� */
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

        /* �ر��¼��豸 */
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
        /* ��ʼ����� */
        return( XJ_UNAVAILABLE );
    }

    /* ���¼��豸 */
    OPENDRIVER( status, CEMC_GET_EVENT_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* ��ȡ�¼���� */
        STATUSDRIVER( status, CEMC_GET_EVENT_HANDLE(), pBuffer,
            ( UNSIGNED *)Actual_Number, this->EventId, uTimeOut );

        /* �ر��¼��豸 */
        CLOSEDRIVER( CEMC_GET_EVENT_HANDLE());
    }

    /* Return the completion status.  */
    return( status );
}

/* ��ȡ�¼����� */
INT CEMC_GetEventDataBlock( CHAR * fileName, UNSIGNED Index, UNSIGNED_CHAR * dst, UNSIGNED size )
{
    INT status;
    FILE * fp;
    UNSIGNED readSize;

    status = XJ_UNAVAILABLE;
    /* ���¼����� */
    fp = xj_fopen( fileName, "rb" );
    if( fp != XJ_NULL )
    {
        /* ��ȡ�¼����� */
        readSize = ( UNSIGNED )xj_fread( dst, 1, size, fp );
        if( xj_fclose( fp ))
        {
            /* �ر��ļ�ʧ�� */
            CEMC_ErrInf.closeFileR ++;
        }
        if( readSize == size )
        {
            status = XJ_SUCCESS;
        }
        else
        {
            /* ��ȡ�ļ�����ʧ�� */
            CEMC_ErrInf.readFile ++;
        }
    }
    else
    {
        CEMC_ErrInf.openFileR ++;
    }

    return( status );
}

/* д�¼����� */
INT CEMC_WriteEventDataBlock( CHAR * fileName, UNSIGNED_CHAR * src, UNSIGNED size )
{
    INT status;
    FILE * fp;
    UNSIGNED writeSize;

    status = XJ_UNAVAILABLE;
    /* д�¼����� */
    fp = xj_fopen( fileName, "wb" );
    if( fp != XJ_NULL )
    {
        /* д�¼����� */
        writeSize = ( UNSIGNED )xj_fwrite( src, sizeof( UNSIGNED_CHAR ), size, fp );
        if( xj_fclose( fp ))
        {
            /* �ر��ļ�ʧ�� */
            CEMC_ErrInf.closeFileW ++;
        }
        if( writeSize == size )
        {
            status = XJ_SUCCESS;
        }
        else
        {
            /* д�ļ�����ʧ�� */
            CEMC_ErrInf.writeFile ++;
            if( xj_RemoveFile( fileName ))
            {
                /* ɾ���ļ�ʧ�� */
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

/* �¼���Żص������������� */
#define CEM_INDEX_PARA_NUM      3       /* �������� */
#define CEM_INDEX_O_PARA_NUM    0       /* ��������λ��ƫ�� */
#define CEM_INDEX_O_DIR_NAME    1       /* �¼�·������λ��ƫ�� */
#define CEM_INDEX_O_HANDLE      2       /* �¼���������ƫ�� */
/* �¼���Żص����� */
int CEMC_ListEventIndexCallBack( struct FileInformation * info, void * context )
{
    INT status;
    UNSIGNED variable;
    CHAR * dirName;
    CHAR * eventIndexStr;
    UNSIGNED eventIndex;
    UNSIGNED * p;
    HITM eventHandle;
    CHAR buffer[64]; /* ����·����������'/'�� */

    /* ��ȡ�¼�����ַ��� */
    p = ( UNSIGNED *)context;
    dirName = ( CHAR *)(*( p + CEM_INDEX_O_DIR_NAME )); /* �¼�·������ */
    eventHandle = ( HITM )(*( p + CEM_INDEX_O_HANDLE )); /* �¼��������� */
    sprintf( buffer, "%s/", dirName );
    eventIndexStr = XJP_CSC_Strcmp( info->Name, buffer );
    eventIndex = ( UNSIGNED )( atol( eventIndexStr ));
    if( eventIndex == 0 )
    {
        return( XJ_UNAVAILABLE );
    }
    /* д���������� */
    XJP_Index_Write( eventHandle, eventIndex, & variable, & status );
    if( status == XJ_TRUE )
    {
        /* �������֧����Ŀ��ɾ���¼��ļ� */
        sprintf( buffer, "%s/%d", dirName, variable );
        if( xj_RemoveFile( buffer ))
        {
            /* ɾ���ļ�ʧ�� */
            CEMC_ErrInf.deleteFileL ++;
            /* ����ɾ���ļ��б� */
            CFDC_Delete_File( buffer );
        }
    }

    return( XJ_UNAVAILABLE );
}

/* ��ȡ�¼������ */
INT CEMC_ListEventIndex( CHAR * dirName, HITM eventHandle, UNSIGNED * Dst,
    UNSIGNED * pActual_size, UNSIGNED Number )
{
    INT status;
    INT ActualNumber;
    UNSIGNED FuncPara[CEM_INDEX_PARA_NUM]; /* �ص��������� */

    /* ������� */
    XJP_Index_Clear( eventHandle );
    /* �¼������б� */
    FuncPara[CEM_INDEX_O_PARA_NUM] = CEM_INDEX_PARA_NUM; /* �������� */
    FuncPara[CEM_INDEX_O_DIR_NAME] = ( UNSIGNED )dirName; /* �¼�·������ */
    FuncPara[CEM_INDEX_O_HANDLE] = ( UNSIGNED )eventHandle; /* �¼��������� */
    ActualNumber = CEMC_LISTFILE( dirName, CEMC_ListEventIndexCallBack, FuncPara );
    /* ��ȡָ�����ݸ���������������(�����¿�ʼ) */
    status = XJP_Index_List( eventHandle, Dst, pActual_size, Number );

    return( status );
}

/* ��ȡ�¼������ */
INT CEMC_GetEventIndexNumber( CHAR * dirName, HITM eventHandle, UNSIGNED * Dst,
    UNSIGNED * pActual_size, UNSIGNED Number )
{
    INT status;

    status = XJP_Index_List( eventHandle, Dst, pActual_size, Number );

    return( status );
}

/* ɾ���¼�Ŀ¼ȫ���ļ��ص������������� */
#define CEM_DEL_F_PARA_NUM      2       /* �������� */
#define CEM_DEL_F_O_PARA_NUM    0       /* ��������λ��ƫ�� */
#define CEM_DEL_F_O_ACT_NUM     1       /* ɾ���ļ����� */
/* ɾ���¼�ָ��Ŀ¼�ļ��ص����� */
int CEMC_DeleteDirFileCallBack( struct FileInformation * info, void * context )
{
    UNSIGNED * p;
    UNSIGNED actDelFileNumber;

    /* ɾ���¼��ļ� */
    if( xj_RemoveFile( info->Name ) == 0 )
    {
        /* ɾ���ɹ� */
        p = ( UNSIGNED *)context;
        actDelFileNumber = *( p + CEM_DEL_F_O_ACT_NUM ); /* ɾ�����¼����� */
        *( p + CEM_DEL_F_O_ACT_NUM ) = (++ actDelFileNumber );
    }
    else
    {
        /* ɾ��Ŀ¼�ļ�ʧ�� */
        CEMC_ErrInf.deleteDirFile ++;
        /* ����ɾ���ļ��б� */
        CFDC_Delete_File( info->Name );
    }
    if( CAlarmEvent.hWatchDogId )
    {
        XJP_Watchdog_Reset( CAlarmEvent.hWatchDogId ); /* ��λ�����Ӷ��� */
    }
    return( XJ_UNAVAILABLE );
}

/* ɾ��ָ��Ŀ¼�µ������ļ� */
INT CEMC_DeleteDirFile( CHAR * dirName, HITM eventHandle )
{
    UNSIGNED ActualNumber;
    UNSIGNED FuncPara[CEM_DEL_F_PARA_NUM]; /* �ص��������� */

    /* ɾ���¼��ļ��Ļص����� */
    FuncPara[CEM_DEL_F_O_PARA_NUM] = CEM_DEL_F_PARA_NUM; /* �������� */
    FuncPara[CEM_DEL_F_O_ACT_NUM] = 0; /* ɾ���¼��ļ��ĸ��� */
    ActualNumber = ( UNSIGNED )CEMC_LISTFILE( dirName, CEMC_DeleteDirFileCallBack, FuncPara );
    /* ����¼����������� */
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

/* �¼�Ŀ¼�ļ���Ŀ���ص������������� */
#define CEM_FULL_F_PARA_NUM     7       /* �������� */
#define CEM_FULL_F_O_PARA_NUM   0       /* ��������λ��ƫ�� */
#define CEM_FULL_F_O_DIR_NAME   1       /* ·������λ��ƫ�� */
#define CEM_FULL_F_O_REQ_NUM    2       /* �����������ݸ���λ��ƫ�� */
#define CEM_FULL_F_O_ACT_NUM    3       /* ʵ�����ݸ���λ��ƫ�� */
#define CEM_FULL_F_O_DST        4       /* ��������ַλ��ƫ�� */
#define CEM_FULL_F_O_DEL_NUM    5       /* ɾ���ļ�����λ��ƫ�� */
#define CEM_FULL_F_O_HANDLE     6       /* �¼��������� */
/* �¼�Ŀ¼�ļ���Ŀ���ص����� */
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
    CHAR buffer[64]; /* ����·����������'/'�� */

    /* ��ȡ����ַ��� */
    p = ( UNSIGNED *)context;
    dirName = ( CHAR *)(*( p + CEM_FULL_F_O_DIR_NAME )); /* ·������ */
    reqDataNumber = *( p + CEM_FULL_F_O_REQ_NUM ); /* ��������ݸ��� */
    pDst = ( UNSIGNED *)(*( p + CEM_FULL_F_O_DST )); /* ��������ַ */
    eventHandle = ( HITM )(*( p + CEM_FULL_F_O_HANDLE )); /* �¼��������� */
    sprintf( buffer, "%s/", dirName );
    IndexStr = XJP_CSC_Strcmp( info->Name, buffer );
    Index = ( UNSIGNED )( atol( IndexStr ));
    if( Index == 0 )
    {
        return( XJ_UNAVAILABLE );
    }
    /* ������������ */
    status = XJP_CSC_Arrage_Insert_Full( pDst, ( p + CEM_FULL_F_O_ACT_NUM ),
        reqDataNumber, Index, & InvalidData, 1 );
    if( status == XJ_TRUE )
    {
        /* �ļ��� */
        sprintf( buffer, "%s/%d", dirName, InvalidData );
        /* ɾ�����ļ� */
        if( xj_RemoveFile( buffer ) == 0 )
        {
            /* ɾ���ɹ� */
            p = ( UNSIGNED *)context;
            actDelFileNumber = *( p + CEM_FULL_F_O_DEL_NUM ); /* ɾ�����ļ����� */
            *( p + CEM_FULL_F_O_DEL_NUM ) = (++ actDelFileNumber );
        }
        else
        {
            /* ɾ�������Ч�ļ�ʧ�� */
            CEMC_ErrInf.deleteCheckFile ++;
            /* ����ɾ���ļ��б� */
            CFDC_Delete_File( buffer );
        }
        /* ɾ���¼����������� */
        XJP_Index_Delete( eventHandle, Index );
    }

    return( XJ_UNAVAILABLE );
}

/* �¼�Ŀ¼�ļ���Ŀ��� */
INT CEMC_FullFileCheck( CHAR * dirName, HITM eventHandle, UNSIGNED * Dst,
    UNSIGNED * pDeleteNumber, UNSIGNED MaxNumber )
{
    INT ActualNumber;
    UNSIGNED FuncPara[CEM_FULL_F_PARA_NUM]; /* �ص��������� */

    /* ���ݲ����б� */
    FuncPara[CEM_FULL_F_O_PARA_NUM] = CEM_FULL_F_PARA_NUM; /* �������� */
    FuncPara[CEM_FULL_F_O_DIR_NAME] = ( UNSIGNED )dirName; /* ·������ */
    FuncPara[CEM_FULL_F_O_REQ_NUM] = MaxNumber; /* �����������ݸ��� */
    FuncPara[CEM_FULL_F_O_ACT_NUM] = 0; /* ʵ�����ݸ��� */
    FuncPara[CEM_FULL_F_O_DST] = ( UNSIGNED )Dst; /* ��������ַ */
    FuncPara[CEM_FULL_F_O_DEL_NUM] = 0; /* ɾ���ļ����� */
    FuncPara[CEM_FULL_F_O_HANDLE] = ( UNSIGNED )eventHandle; /* �¼��������� */
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

/* �¼�Ŀ¼�ļ���������� */
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
    CHAR fileName[128]; /* ����·����������'/'�� */
    CHAR tempFileName[128]; /* ��ʱ�ļ��� */

    switch( pRequest -> nu_function )
    {
        /* ��ʼ�� */
    case XJ_INITIALIZE:
        /* ��ʼ���¼���Ϣ */
        pEvent = XJ_NULL;
        pRequest -> nu_status = XJ_INVALID_ENTRY;
        if(( pRequest->nu_supplemental & CEMC_DATA_MASKS16()) == CAlarmEvent.EventId )
        {
            pEvent = & CAlarmEvent;
            /* �澯�¼���ʼ�� */
            pEvent->Number = 0;
            pEvent->Serial = 0;
        }
        else if(( pRequest->nu_supplemental & CEMC_DATA_MASKS16()) == CReserveEvent.EventId )
        {
            pEvent = & CReserveEvent;
            /* �ڲ��¼���ʼ�� */
            pEvent->Number = 0;
            pEvent->Serial = 0;
        }
        else if(( pRequest->nu_supplemental & CEMC_DATA_MASKS16()) == COperateEvent.EventId )
        {
            pEvent = & COperateEvent;
            /* �ڲ��¼���ʼ�� */
            pEvent->Number = 0;
            pEvent->Serial = 0;
        }
        else
        {
            /* �û��¼���ʼ�� */
            CEMC_GET_USER_EVENT_PTR( pEventGroup );
            for( k = 0; k < ( UNSIGNED )CEMC_GET_USER_EVENT_GROUPS(); k ++)
            {
                if(( pRequest->nu_supplemental & CEMC_DATA_MASKS16()) == pEventGroup->EventId )
                {
                    pEvent = pEventGroup;
                    pEventGroup->MaxNumber = CAlarmEvent.MaxNumber;  //����¼�����
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
            /* ����¼����� */
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
                /* ��ȡ�����¼������� */
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

        /* ��ȡ�¼�����Ȩ */
    case XJ_ASSIGN:
        CEMC_LOCK_EVENT( pRequest -> nu_status, pRequest -> nu_timeout );
        break;

        /* �ͷ��¼�����Ȩ */
    case XJ_RELEASE:
        CEMC_UNLOCK_EVENT( pRequest -> nu_status );
        break;

        /* ���¼����� */
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
        /* ��ȡ�¼���Ϣ */
        sprintf( fileName, "%s/%d", pEvent->dirName, index );
        if( xj_GetFileInformation( fileName,& fileInfo ) == XJ_SUCCESS )
        {
            /* ��ȡ��Ϣ�ɹ�,���¼����� */
            if( CEMC_GetEventDataBlock( fileName, index, destination, fileInfo.Size ) == XJ_SUCCESS )
            {
                /* ��ȡ�ɹ� */
                pRequest -> nu_status = XJ_SUCCESS;
            }
            else
            {
                /* ��ȡʧ�� */
                pRequest -> nu_status = XJ_END_OF_LOG;
            }
        }
        else
        {
            /* ��ȡ��Ϣʧ�� */
            CEMC_ErrInf.getInfoFile ++;
            pRequest -> nu_status = XJ_END_OF_LOG;
        }
        break;

        /* д�¼����� */
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
        index = pNVEventData->IndexL + ( pNVEventData->IndexH << 8 ); /* 16λ���� */
        /* д�¼����� */
        sprintf( fileName, "%s/%d", pEvent->dirName, index );
        sprintf( tempFileName, "%s/tempEvent", pEvent->dirName );
        if( CEMC_WriteEventDataBlock( tempFileName, p, size ) == XJ_SUCCESS )
        {
            if( xj_RenameFile( tempFileName, fileName )== XJ_SUCCESS )
            {
                /* д�¼��ɹ� */
                pEvent -> Serial = pEvent -> Serial1;
                /* �������������� */
                XJP_Index_Write( pEvent -> EventHandle, index, & variable, & status );
                if( status == XJ_TRUE )
                {
                    /* �������֧����Ŀ��ɾ���¼��ļ� */
                    sprintf( fileName, "%s/%d", pEvent->dirName, variable );
                    if( xj_RemoveFile( fileName ))
                    {
                        /* ɾ���ļ�ʧ�� */
                        CEMC_ErrInf.deleteFullFile ++;
                        /* ����ɾ���ļ��б� */
                        CFDC_Delete_File( fileName );
                    }
                }
                pRequest -> nu_status = XJ_SUCCESS;
            }
            else
            {
                /* �������ļ���ʧ�� */
                CEMC_ErrInf.renameFile ++;
                pRequest -> nu_status = XJ_UNAVAILABLE;
            }
        }
        else
        {
            /* д�¼�ʧ�� */
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
/* ��ȡ�¼����� */
INT CEMC_GetEventData( VOID * pBuffer )
{
    CEM_RAD EventData,* pEventSrc;      /* RAM event control block ptr */
    CEM_NVD * pEventDst;                /* NVRAM event control block ptr */
    INT status;                         /* Completion status */

    /* ���¼����� */
    CEMC_PROTECT_EVENT();
    CEMC_READ_EVENT_BUFFER( status,& EventData );
    CEMC_UNPROTECT_EVENT();

    if( status == XJ_SUCCESS )
    {
        /* ת���¼����� */
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

/* �¼�����ת�� */
INT CEMC_EventConvert( CEM_RAD * pDst, CEM_NVD * pSrc )
{
    CEM_RAD * pEventDst;                 /* RAM event control block ptr */
    CEM_NVD * pEventSrc;                 /* NVRAM event control block ptr */

    /* ת���¼����� */
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

    /* �¼������־��� */
    CEMC_GET_EVENT_ERROR( Status,& ActualEvent );
    if( Status == XJ_SUCCESS )
    {
        if( GroupNumber != 0 )
        {
            /* �ر��¼��豸 */
            CLOSEDRIVER( CEMC_GET_EVENT_HANDLE());
        }
        GroupNumber = 0;

        /* EEPROM����,ֱ�������¼���Ϣ */
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

    /* �¼���¼ */
    if( GroupNumber == 0 )
    {
        /* ����¼� */
        CEMC_CHECK_EVENT( Status,& Size );
        if( Status == XJ_TRUE )
        {
            /* ���¼��豸 */
            OPENDRIVER( Status, CEMC_GET_EVENT_HANDLE(), CEMC_GET_MAX_EVENT_TIME());
            if( Status == XJ_SUCCESS )
            {
                /* ���¼����� */
                if( CEMC_GetEventData(& NVEventData ) == XJ_SUCCESS )
                {
                    type = NVEventData.Flag;
                    /* �����¼� */
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
                        /* �ر��¼��豸 */
                        CLOSEDRIVER( CEMC_GET_EVENT_HANDLE());
                    }
                    else
                    {
                        GroupNumber ++;
                    }
                }
                else
                {
                    /* �ر��¼��豸 */
                    CLOSEDRIVER( CEMC_GET_EVENT_HANDLE());
                }
            }
        }
    }

    /* д�¼����� */
    if( GroupNumber == 1 )
    {
        /* �����¼���� */
        pEvent->Serial1 = pEvent->Serial + 1;
        /* ����¼�����Ƿ���(1~0xFFFF)��Χ�� */
        CHECKCYCDATALIMIT( pEvent->Serial1, CEM_SERIAL_NUMBER_MIN, CEM_SERIAL_NUMBER_MAX );

        NVEventData.IndexL = ( UNSIGNED_CHAR )pEvent->Serial1 & CEM_BYTE_MASKS;
        NVEventData.IndexH = ( UNSIGNED_CHAR )(( pEvent->Serial1 ) >> CEM_BYTE_BITS ) & CEM_BYTE_MASKS;

        /* д�¼� */
        Status = pEvent->Write( pEvent,& NVEventData, CEMC_GET_MAX_EVENT_TIME());
        if( Status != XJ_TIMEOUT )
        {
            if( Status != XJ_SUCCESS )
            {
                /* �¼��ļ���Ŀ��� */
                CEMC_EventFullFileCheckMain( pEvent );
            }
            /* ���͵���ʾ�¼� */
            CEMC_EventConvert(& EventData,& NVEventData );
            CEMC_PROTECT_EVENT();
            XJP_Queue_Write( RamEventHandle , & EventData );
            CEMC_UNPROTECT_EVENT();
            GroupNumber ++;
        }
    }

    /* ���¼���Ϣ */
    if( GroupNumber >= 2 )
    {
        GroupNumber = 0;
        /* �ر��¼��豸 */
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
    /* �¼����� */
    Event.Flag = iEventType;
    strncpy( ( CHAR *)( Event.Name ), ( CHAR *)pName, ( CEM_EVENT_NAME - 1 ));
    * ( Event.Name + CEM_EVENT_NAME - 1 ) = 0; /*�ַ���β����ӽ����� */
    /* ��ȡʱ�� */
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
        /* д�洢�¼� */
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
    /* �¼����� */
    Event.Flag = iEventType;
    strncpy( ( CHAR *)( Event.Name ), ( CHAR *)pName, ( CEM_EVENT_NAME - 1 ));
    * ( Event.Name + CEM_EVENT_NAME - 1 ) = 0; /*�ַ���β����ӽ����� */
    Event.Year = pTime -> Year;
    Event.Month = pTime -> Month;
    Event.Day = pTime -> Day;
    Event.Hour = pTime -> Hour;
    Event.Minute = pTime -> Minute;
    Event.Millionsecond = pTime -> Millionsecond;

    if( iEventType != 0 )
    {
        /* д�洢�¼� */
        status = XJP_Queue_Write( NVRamEventHandle , & Event );
    }
    else
    {
        /* дRAM�¼� */
        status = XJP_Queue_Write( RamEventHandle , & Event );
    }

    /* Return the completion status.  */
    return( status );
}

/* �����¼������Ź���� */
INT CEMC_SetWatchDogId( HWDM hWatchDogId )
{
    CAlarmEvent.hWatchDogId = hWatchDogId;
    CReserveEvent.hWatchDogId = hWatchDogId;
    COperateEvent.hWatchDogId = hWatchDogId;
    return( XJ_SUCCESS );
}
