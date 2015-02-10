
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

#define CWMC_WAVE_DIR_NAME              CPC_WAVE_DIR_NAME /* ¼���洢·�� */
#define CWMC_MAX_RAM_WAVE_NUMBER        CPC_MAX_RAM_WAVE_DATA_NUMBER /* ����ڴ滺��¼�����ݸ��� */
#define CWMC_MAX_CHL_DATA_GROUPS        CPC_MAX_CHL_NUMBER /* ���¼��ģ��������ֵ���� */
#define CWMC_MAX_STATE_DATA_GROUPS      CPC_MAX_STATE_GROUPS /* ���¼��״̬������ֵ���� */
#define CWMC_MAX_WAVE_DATA_GROUPS (CWMC_MAX_CHL_DATA_GROUPS+CWMC_MAX_STATE_DATA_GROUPS) /* ���¼������ֵ���� */
#define CWMC_MAX_WAVE_DATA_SIZE         CPC_MAX_WAVE_DATA_SIZE   /* ���¼�����ݵ��� */
#define CWMC_MAX_NVRAM_WAVE             CPC_MAX_NVRAM_WAVE_NUMBER /* ���洢¼����Ŀ */
#define CWMC_GET_WAVE_ANALOG_NUMBER     CSMV_GET_ANALOG_NUMBER //¼�����ģ��������
#define CWMC_GET_WAVE_INPUT_GROUPS      CSMV_GET_WAVE_INPUT_GROUPS //¼�����������
#define CWMC_GET_WAVE_OUTPUT_GROUPS     CSMV_GET_OUTPUT_GROUPS //¼����󿪳�����
#define CWMC_MAX_WAVE_STATE_GROUPS      CPC_MAX_PROTECT_GROUPS //¼�����״̬����
#define CWMC_MAX_WAVE_LOGICAL_GROUPS    CPC_MAX_PROTECT_LOG_GROUPS //¼������߼�����
#define CWMC_EVENT_WAVE_CLR             CPC_EVENT2_WAVE_CLR /* ��¼������ */
#define CWMC_WAVE_TIMEOUT               CPC_WAVE_TIMEOUT /* ¼����ʱʱ�� */
#define CWMC_MAX_SAMPLE_POINT           CSMV_GET_CHANNEL_POINT /* ͨ��������ֵ���� */
/*==============================================================*/
/* ȫ�ֱ������� */
extern ESP_CFG PLAT_cfg;
extern NU_EVENT_GROUP Event_2_CB;
extern XJ_PROTECT pWave_Protect;
extern PRO_CHANNEL * pChannelTab;

/* ¼���豸������� */
XJ_DRIVER CWMC_hWaveDriver = XJ_NULL;
XJ_SEMAPHORE CWMC_hSemaphore = XJ_NULL;
CWM_WCB * WavePointer = XJ_NULL;
CWM_WCB * WavePointerCB[ CWM_MAX_WAVE_POINTER ] = { XJ_NULL, XJ_NULL, XJ_NULL }; /* ¼����¼���ƿ� */
UNSIGNED CWMC_Protect_Status[CWMC_MAX_WAVE_STATE_GROUPS];
UNSIGNED CWMC_Protect_Logic_Status[CWMC_MAX_WAVE_LOGICAL_GROUPS];
/* ¼���洢·�� */
CHAR CWMC_WaveDirName[256] = CWMC_WAVE_DIR_NAME;
/* ¼������������� */
HITM CWMC_WaveHandle;

/* �궨�� */
#define CWMC_CREATE_WAVE_DRIVER(x) x=XJ_Create_Driver("Wave",CWMC_WaveDriverEntry) //����¼������
#define CWMC_GET_WAVE_HANDLE() CWMC_hWaveDriver //��ȡ¼�����
#define CWMC_SET_WAVE_HANDLE(x) CWMC_hWaveDriver = x //����¼�����
#define CWMC_CREATE_WAVE_LOCK(x) x=XJ_Create_Semaphore("Wave",1,XJ_FIFO) //����¼��������
#define CWMC_GET_WAVE_LOCK() CWMC_hSemaphore //��ȡ¼�����������
#define CWMC_SET_WAVE_LOCK(x) CWMC_hSemaphore = x //����¼�����������
#define CWMC_LOCK_WAVE(x,y) x=XJP_Mutex_Obtain(CWMC_hSemaphore,y) //��ȡ¼��������
#define CWMC_UNLOCK_WAVE(x) x=XJP_Mutex_Release(CWMC_hSemaphore) //�ͷ�¼��������
#define CWMC_GET_WAVE_CLEAR(x,y) x=XJ_Retrieve_Events(&Event_2_CB,CWMC_EVENT_WAVE_CLR,XJ_AND,y,XJ_NO_SUSPEND) //���¼��
#define CWMC_SET_WAVE_CLEAR() XJ_Set_Events(&Event_2_CB,(UNSIGNED)(~CWMC_EVENT_WAVE_CLR),XJ_AND) //���¼�����
#define CWMC_FILL_RESERVE_EVENT(x) CEMC_FillEvent(x,RESERVE_EVENT_ID) //��䱣���¼�
#define CWMC_PROTECT_WAVE() XJ_Protect(pWave_Protect)  //¼�����ݱ��� */
#define CWMC_UNPROTECT_WAVE() XJ_Unprotect()  //�ͷ�¼�����ݱ��� */
#define CWMC_CHANNEL_MEMORY_TAB() pChannelTab //ͨ���ڴ�
#define CWMC_SAMPLE_POINT(x) x = (UNSIGNED)(PLAT_cfg.smvc.PeriodPoint) //��������
#define CWMC_SAMPLE_FREQUENCY(x) x = (PLAT_cfg.smvc.LineFrequency) //��·Ƶ��
#define CWMC_SAMPLE_RATE(x,y) x = ((PLAT_cfg.smvc.SampleRate)/y) //������(ÿ���������)
#define CWMC_DATA_MASKS16() 0xffff //16λ��������
#define CWMC_DATA_BITS16() 16 //16λ����λ��
#define CWMC_DATA_MASKSH16() 0xffff0000 /* ��16λ�������� */
#define CWMC_CHECK_DATA_LIMIT CHECKCYCDATALIMIT //������ݷ�Χ
#define CWMC_LOCK_INTERRUPTS(x) x=XJ_Local_Control_Interrupts(0) //���ж�
#define CWMC_UNLOCK_INTERRUPTS(x) XJ_Local_Control_Interrupts(x) //���ж�
#define CWMC_WAVE_ALLOCATE(x,y,z) x=XJP_Memory_Allocate(CWave.DmHandle,y,z) //����¼����̬�ڴ�
#define CWMC_WAVE_DELLOCATE(x,y) x=XJP_Memory_Deallocate(CWave.DmHandle,y) //�ͷ�¼����̬�ڴ�
#define CWMC_GET_MAX_WAVE_TIME() CWMC_WAVE_TIMEOUT //���¼����ʱʱ��
#define CWMC_MAX_WAVE_POINT() CWMC_MAX_WAVE_DATA_SIZE //¼��������
#define CWMC_ANALOG_BANK_WIDTH() 2 //�ֿ��/ģ�������ݴ洢���
#define CWMC_ANALOG_BANK_MASKS() 0xffff //ģ������������
#define CWMC_ANALOG_BANK_BITS() 16 //ģ����λ��
#define CWMC_MAX_ANALOG_VALUE() 32767 //ģ�������ֵ
#define CWMC_MIN_ANALOG_VALUE() -32767 //ģ������Сֵ
#define CWMC_WAVE_FAULT_BITS() 16 //������š������������λ��
#define CWMC_GET_MIN_RECORD_NUMBER() 5 //��С��¼����
#define CWMC_CREATE_WAVE_LIST(x) XJP_CSC_Place_On(&(CWave.WaveList),x) //���¼���б�
#define CWMC_REMOVE_WAVE_LIST(x) XJP_CSC_Remove((&(CWave.WaveList)),x) //ɾ��¼���б�
#define CWMC_ANALOG_WIDTH() 2 //ģ�������ݵ��ֽ���
#define CWMC_STATE_WIDTH() 4  //״̬�����ݵ��ֽ���
#define CWMC_MAX_WAVE_FILE_NUMBER (CWMC_MAX_NVRAM_WAVE + 10) //���¼���ļ���Ŀ
#define CWMC_WAVE_HEAD_WORD() (sizeof(CWM_WHD)/sizeof(UNSIGNED)) /* ¼��ͷ���� */
#define CWMC_MAX_WAVE_CHL_SIZE() (sizeof(CWM_VHD)+sizeof(UNSIGNED)+(CWMC_MAX_WAVE_DATA_SIZE*CWMC_ANALOG_WIDTH()))
#define CWMC_MAX_WAVE_CHL_WORD() ((CWMC_MAX_WAVE_CHL_SIZE()+sizeof(UNSIGNED)-1)/sizeof(UNSIGNED)) /* ���ģ����ͨ������ */
#define CWMC_MAX_WAVE_STATE_SIZE() (sizeof(CWM_VHD)+sizeof(UNSIGNED)+(CWMC_MAX_WAVE_DATA_SIZE*CWMC_STATE_WIDTH()))
#define CWMC_MAX_WAVE_STATE_WORD() ((CWMC_MAX_WAVE_STATE_SIZE()+sizeof(UNSIGNED)-1)/sizeof(UNSIGNED)) /* ���״̬��ͨ������ */
#define CWM_MAX_WAVE_VALUE_WORD \
(( CWMC_MAX_CHL_DATA_GROUPS * CWMC_MAX_WAVE_CHL_WORD()) + (CWMC_MAX_STATE_DATA_GROUPS*CWMC_MAX_WAVE_STATE_WORD()))
/* ���¼������ֵ���� */
#define CWMC_GET_WAVE_ELEC_NUMBER() CWave.UserElecNumber    //��ȡ¼�������������
#define CWMC_GET_WAVE_INDEX() CWave.UserFaultNumber        //��ȡ¼��������
#define CWMC_SET_WAVE_ELEC_NUMBER(x) CWave.UserElecNumber=x //����¼�������������
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
/* ����¼������ */
INT CWMC_CreateWaveList( INT iElecNumber, INT iType, INT iStartPoint, INT iSpace,
    INT iTotalPoint, WAVE_CHANNEL * WaveChannel, TIME * pTime, INT ctrlType,
    STATE * pState, INT iState );
/* ��ȡ¼����� */
static INT CWMC_GetWaveIndexNumber( CHAR * dirName, UNSIGNED * Dst,
    UNSIGNED * pActual_size, UNSIGNED Number, UNSIGNED FaultNumber );
/* ��ȡ¼����Ϣ���� */
static INT CWMC_WaveInformation( CWAVE * this, UNSIGNED * Number, UNSIGNED * Serial,
    UNSIGNED * StartFaultNumber, UNSIGNED * EndFaultNumber, UNSIGNED uTimeOut );
/* ���ҹ������� */
static INT CWMC_FindFaultNumber( CWAVE * this, VOID * pBuffer, INT * Actual_Number,
    INT Type, UNSIGNED uTimeOut );
/* ��ʼ��¼��RCD����ģ�� */
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
        /* ����¼��Ŀ¼ */
        if(( xj_DirExists == XJ_NULL ) || ( xj_MakeDir == XJ_NULL ))
        {
            /* �������������ڣ�ֱ�ӷ���ʧ�� */
            CWMC_ErrInf.makeDirCounter ++;
            return( XJ_UNAVAILABLE );
        }
        if( xj_DirExists( this->dirName ) == XJ_NULL )
        {
            /* Ŀ¼�����ڣ�����Ŀ¼ */
            if( xj_MakeDir( this->dirName ) != XJ_SUCCESS )
            {
                CWMC_ErrInf.makeDirCounter ++;
                return( XJ_UNAVAILABLE );
            }
        }
        /* ������̬�ڴ������� */
        PoolSize = sizeof( CWM_WCB ); /* �ڴ�¼��ͷ */
        PoolSize += ( CWMC_MAX_WAVE_DATA_GROUPS * sizeof( CWM_VHD ) ); /* �ڴ�ͨ��������Ϣ */
        PoolSize += ( CWMC_MAX_WAVE_DATA_GROUPS * CWMC_MAX_WAVE_POINT() * sizeof( UNSIGNED )); /* �ڴ�ͨ������ֵ */
        PoolSize *= CWMC_MAX_RAM_WAVE_NUMBER; /* �ڴ�¼�������ܴ�С */
        this -> DmHandle = XJP_Memory_Pool_Create( PoolSize );
        if( this -> DmHandle == 0 )
        {
            CWMC_ErrInf.craetePool ++;
            /* ����ʧ��,ֱ�ӷ��� */
            return( XJ_UNAVAILABLE );
        }
        /* ����¼������ */
        if( CWMC_GET_WAVE_HANDLE() == XJ_NULL )
        {
            CWMC_CREATE_WAVE_DRIVER( hDriver );
            if( hDriver == XJ_NULL )
            {
                /* ����¼������ʧ�� */
                CWMC_ErrInf.createDriver ++;
                return( XJ_UNAVAILABLE );
            }
            CWMC_SET_WAVE_HANDLE( hDriver );
        }
        this -> WaveDriver = CWMC_GET_WAVE_HANDLE();
        /* ����¼�������� */
        if( CWMC_GET_WAVE_LOCK() == XJ_NULL )
        {
            CWMC_CREATE_WAVE_LOCK( hSemaphore );
            if( hSemaphore == XJ_NULL )
            {
                /* ����¼��������ʧ�� */
                CWMC_ErrInf.createSemaphore ++;
                return( XJ_UNAVAILABLE );
            }
            CWMC_SET_WAVE_LOCK( hSemaphore );
        }
        /* ����¼������������� */
        CWMC_WaveHandle = XJP_Index_Create( CWMC_MAX_WAVE_FILE_NUMBER );
        this -> InitFlag = 1;
    }

    /* ��¼���豸 */
    OPENDRIVER( status, CWMC_GET_WAVE_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* �豸��ʼ�� */
        INITIALIZEDRIVER( status, CWMC_GET_WAVE_HANDLE(), iType, uTimeOut );

        /* �ر�¼���豸 */
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
        /* ��ʼ����� */
        return( XJ_UNAVAILABLE );
    }

    /* ����RAM¼������ */
    //    status = CWMC_ReadRamWaveData( uFaultNumber, pBuffer, uOffset, iDataType, uNumber );
    //    if( status == XJ_SUCCESS )
    //    {
    //        return( XJ_SUCCESS );
    //    }

    /* ��¼���豸 */
    OPENDRIVER( status, CWMC_GET_WAVE_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* ��¼������ */
        READDRIVER( status, CWMC_GET_WAVE_HANDLE(), iDataType, pBuffer, uOffset, uNumber,
            uFaultNumber, uTimeOut );

        /* �ر�¼���豸 */
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
        /* ��ʼ����� */
        return( XJ_UNAVAILABLE );
    }

    /* д¼������ */
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
/* ��ȡ¼����Ϣ���� */
static INT CWMC_WaveInformation( CWAVE * this, UNSIGNED * Number, UNSIGNED * Serial,
    UNSIGNED * StartFaultNumber, UNSIGNED * EndFaultNumber, UNSIGNED uTimeOut )
{
    UNSIGNED Index;
    UNSIGNED uNumber = 0;
    UNSIGNED buffer[CWMC_MAX_NVRAM_WAVE];  /* report buffer */
    UNSIGNED Actual_Number = 0;                    /* total fault number */
    INT status;                                    /* complete status */

    /* ��¼���豸 */
    OPENDRIVER( status, CWMC_GET_WAVE_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* ��ȡ¼������������� */
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
                /* ��ȡ���¹�����ŵ�¼����� */
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

        /* �ر�¼���豸 */
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

    /* ����Ƿ���¼�� */
    if( this -> ReadySerialNumber > 0 )
    {
        /* ���µ�¼����� */
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

    /* ����Ƿ��й���¼�� */
    if( this -> ReadyFaultNumber > 0 )
    {
        /* ���µĹ������ */
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

    /* ��¼���豸 */
    OPENDRIVER( status, CWMC_GET_WAVE_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        if( this->WaveBusy == 0 )
        {
            /* ready fault number */
            this->ReadyFaultNumber = this->ElecNumber;
            this->ReadySerialNumber = this->FaultNumber;
        }

        /* �ر�¼���豸 */
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
    /* ����¼��״̬ */
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
        /* ��ʼ����� */
        return( XJ_UNAVAILABLE );
    }

    //    if(( iType & ( WAVE_TYPE_START + WAVE_TYPE_SOF_TEST )) != 0 )
    //    {
    //        /* �����������ֶ�¼�� */
    //        status = CWMC_FindRamWaveFaultNumber( iType, pBuffer, Actual_Number );
    //    }
    //    else
    {
        /* ���ҹ���¼�� */
        /* ��¼���豸 */
        OPENDRIVER( status, CWMC_GET_WAVE_HANDLE(), uTimeOut );
        if( status == XJ_SUCCESS )
        {
            /* ��ȡ¼����� */
            STATUSDRIVER( status, CWMC_GET_WAVE_HANDLE(), pBuffer,( UNSIGNED *)Actual_Number, 0, uTimeOut );

            /* �ر�¼���豸 */
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
/* ¼����ֵ��Ϣ�ṹ. */
typedef struct WAVE_VALUE_INFO_STRUCT
{
    /* ����ͷ����Ϣ */
    union VALUE_INFO_UNION
    {
        CWM_ANALOG analog;
        CWM_INPUT input;
        CWM_OUTPUT output;
        CWM_STATE state;
        CWM_LOGICAL logical;
    }Info;
}CWM_INFO;

/* ¼��ͨ���洢���ݽṹ */
typedef struct WAVE_CHANNEL_STRUCT
{
    /* ¼����ֵ�ṹ */
    CWM_VHD head;  //��ֵͷ����Ϣ
    UNSIGNED value[CWMC_MAX_WAVE_DATA_SIZE]; //��ֵ����
}CWM_CHANNEL;

/* ¼�����ݴ洢�ṹ */
typedef struct WAVE_DATA_STRUCT
{
    /* ����ͷ����Ϣ */
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
    /* ��¼����¼ */
    CWMC_PROTECT_WAVE();
    search_ptr = CWave.WaveList;
    if( search_ptr != XJ_NULL )
    {
        /* ����¼������ */
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
        /* ��ȡ¼������ */
        if( iDataType != 0 )
        {
            /* ��ȡ��ֵ���� */
            p = pRecord -> WaveMemory;
            p += (( iDataType - 1 ) * ( pRecord->TotalPointer ));
            pDst = ( UNSIGNED *)( WaveData.data.channel.value );
            for( k = 0; k < pRecord->RecordNumber; k ++)
            {
                * pDst ++ = * p ++;
            }

            /* ��ȡ��ֵ��Ϣ */
            variable = ( UNSIGNED )iDataType;
            if( variable <= pRecord->AnalogNumber )
            {
                /* ģ���� */
                pAnalog = pRecord->AnalogInf;
                pAnalog += ( variable - 1 );
                WaveInf.Info.analog.Number = pAnalog->Number;
                WaveInf.Info.analog.Inf = pAnalog->Inf;
                WaveInf.Info.analog.Type = pAnalog->Type;
                WaveInf.Info.analog.CalculateCoefficient = pAnalog->CalculateCoefficient;
                WaveInf.Info.analog.RateCoefficient = pAnalog->RateCoefficient;
                WaveInf.Info.analog.PrimaryRate = pAnalog->PrimaryRate;
                WaveInf.Info.analog.SecondaryRate = pAnalog->SecondaryRate;
                GroupNumber = 1;  //ģ������ű�ʶ
                status = XJ_SUCCESS;
            }
            else if( variable <= ( pRecord->AnalogNumber + pRecord->InputNumber ))
            {
                /* ������ */
                pInput = pRecord->InputInf;
                pInput += ( variable - pRecord->AnalogNumber - 1 );
                WaveInf.Info.input.GroupNumber = pInput->GroupNumber;
                WaveInf.Info.input.MaskBits = pInput->MaskBits;
                GroupNumber = 2;  //��������ű�ʶ
                status = XJ_SUCCESS;
            }
            else if( variable <= ( pRecord->AnalogNumber + pRecord->InputNumber +
                pRecord->OutputNumber ))
            {
                /* ������ */
                pOutput = pRecord->OutputInf;
                pOutput += ( variable - pRecord->AnalogNumber - pRecord->InputNumber - 1 );
                WaveInf.Info.output.GroupNumber = pOutput->GroupNumber;
                WaveInf.Info.output.MaskBits = pOutput->MaskBits;
                GroupNumber = 3;  //��������ű�ʶ
                status = XJ_SUCCESS;
            }
            else if( variable <= ( pRecord->AnalogNumber + pRecord->InputNumber +
                pRecord->OutputNumber + pRecord->StateNumber ))
            {
                /* ״̬�� */
                pState = pRecord->StateInf;
                pState += ( variable - pRecord->AnalogNumber - pRecord->InputNumber - pRecord->OutputNumber - 1 );
                WaveInf.Info.state.GroupNumber = pState->GroupNumber;
                WaveInf.Info.state.MaskBits = pState->MaskBits;
                GroupNumber = 4;  //״̬����ű�ʶ
                status = XJ_SUCCESS;
            }
            else if( variable <= ( pRecord->AnalogNumber + pRecord->InputNumber +
                pRecord->OutputNumber + pRecord->StateNumber + pRecord->LogicNumber ))
            {
                /* �߼�״̬�� */
                pLogical = pRecord->LogicalInf;
                pLogical += ( variable - pRecord->AnalogNumber - pRecord->InputNumber -
                    pRecord->OutputNumber - pRecord->StateNumber - 1 );
                WaveInf.Info.logical.GroupNumber = pLogical->GroupNumber;
                WaveInf.Info.logical.MaskBits = pLogical->MaskBits;
                GroupNumber = 5;  //�߼�״̬����ű�ʶ
                status = XJ_SUCCESS;
            }
            else
            {
                status = XJ_UNAVAILABLE;
            }
        }
        else
        {
            /* ��ȡ¼��ͷ������ */
            CWMC_GetWaveHead( pRecord,&( WaveData.data.head ));
            GroupNumber = 0;
            status = XJ_SUCCESS;
        }
    }
    else
    {
        /* ��¼������ */
        status = XJ_UNAVAILABLE;
    }
    CWMC_UNPROTECT_WAVE();

    if( status == XJ_SUCCESS )
    {
        /* ת��¼������ */
        pDst = ( UNSIGNED *)pBuffer;
        if( GroupNumber == 0 )
        {
            /* ¼��ͷ������ */
            pSrc = ( UNSIGNED *)&( WaveData.data.head );
            pSrc += uOffset;
            for( k = 0; k < uNumber; k ++)
            {
                * pDst ++ = * pSrc ++;
            }
        }
        /* ģ�������� */
        else if( GroupNumber == 1 )
        {
            /* ģ����ͷ����Ϣ */
            WaveData.data.channel.head.head_info.analog.Inf = WaveInf.Info.analog.Inf;
            WaveData.data.channel.head.head_info.analog.Primary_rate = WaveInf.Info.analog.PrimaryRate;
            WaveData.data.channel.head.head_info.analog.Secondary_rate = WaveInf.Info.analog.SecondaryRate;
            WaveData.data.channel.head.head_info.analog.Coefficient =
                WaveInf.Info.analog.CalculateCoefficient / WaveInf.Info.analog.RateCoefficient;
            if((( WaveType & WAVE_TYPE_COEFFECT ) == 0 )
                && (( WaveInf.Info.analog.Type & DC_COMPONENT ) == 0 ))
            {
                /* ˲ʱֵ����������ϵ������2�Ŀ��� */
                WaveData.data.channel.head.head_info.analog.Coefficient *= (( float )0.707107 );
            }

            /* ģ�������� */
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
                        /* ���޼�� */
                        variable = 0x7fff;
                    }
                    else if( point_value < CWMC_MIN_ANALOG_VALUE())
                    {
                        /* ���޼�� */
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
                    /* ���޼�� */
                    variable = 0x7fff;
                }
                else if( point_value < CWMC_MIN_ANALOG_VALUE())
                {
                    /* ���޼�� */
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

            /* ��ȡģ�������� */
            pDst = ( UNSIGNED *)pBuffer;
            pSrc = ( UNSIGNED *)&( WaveData.data.channel );
            pSrc += uOffset;
            for( k = 0; k < uNumber; k ++)
            {
                * pDst ++ = * pSrc ++;
            }
        }
        /* ���������� */
        else if( GroupNumber == 2 )
        {
            /* ������ͷ����Ϣ */
            WaveData.data.channel.head.head_info.input.GroupNumber = WaveInf.Info.input.GroupNumber;
            WaveData.data.channel.head.head_info.input.MaskBits = WaveInf.Info.input.MaskBits;

            /* ���������� */
            pSrc = ( UNSIGNED *)WaveData.data.channel.value;
            pDst = ( UNSIGNED *)WaveData.data.channel.value;
            for( k = 0; k < RecordNumber; k ++)
            {
                * pDst ++ = * pSrc ++;
            }
            * pDst ++ = CWM_END_FLAG;

            /* ��ȡ���������� */
            pDst = ( UNSIGNED *)pBuffer;
            pSrc = ( UNSIGNED *)&( WaveData.data.channel );
            pSrc += uOffset;
            for( k = 0; k < uNumber; k ++)
            {
                * pDst ++ = * pSrc ++;
            }
        }
        /* ���������� */
        else if( GroupNumber == 3 )
        {
            /* ������ͷ����Ϣ */
            WaveData.data.channel.head.head_info.output.GroupNumber = WaveInf.Info.output.GroupNumber;
            WaveData.data.channel.head.head_info.output.MaskBits = WaveInf.Info.output.MaskBits;

            /* ���������� */
            pSrc = ( UNSIGNED *)WaveData.data.channel.value;
            pDst = ( UNSIGNED *)WaveData.data.channel.value;
            for( k = 0; k < RecordNumber; k ++)
            {
                * pDst ++ = * pSrc ++;
            }
            * pDst ++ = CWM_END_FLAG;

            /* ��ȡ���������� */
            pDst = ( UNSIGNED *)pBuffer;
            pSrc = ( UNSIGNED *)&( WaveData.data.channel );
            pSrc += uOffset;
            for( k = 0; k < uNumber; k ++)
            {
                * pDst ++ = * pSrc ++;
            }
        }
        /* ״̬������ */
        else if( GroupNumber == 4 )
        {
            /* ״̬��ͷ����Ϣ */
            WaveData.data.channel.head.head_info.state.GroupNumber = WaveInf.Info.state.GroupNumber;
            WaveData.data.channel.head.head_info.state.MaskBits = WaveInf.Info.state.MaskBits;

            /* ״̬������ */
            pSrc = ( UNSIGNED *)WaveData.data.channel.value;
            pDst = ( UNSIGNED *)WaveData.data.channel.value;
            for( k = 0; k < RecordNumber; k ++)
            {
                * pDst ++ = * pSrc ++;
            }
            * pDst ++ = CWM_END_FLAG;

            /* ��ȡ״̬������ */
            pDst = ( UNSIGNED *)pBuffer;
            pSrc = ( UNSIGNED *)&( WaveData.data.channel );
            pSrc += uOffset;
            for( k = 0; k < uNumber; k ++)
            {
                * pDst ++ = * pSrc ++;
            }
        }
        /* �߼�״̬������ */
        else if( GroupNumber == 5 )
        {
            /* �߼���ͷ����Ϣ */
            WaveData.data.channel.head.head_info.logical.GroupNumber = WaveInf.Info.logical.GroupNumber;
            WaveData.data.channel.head.head_info.logical.MaskBits = WaveInf.Info.logical.MaskBits;

            /* �߼������� */
            pSrc = ( UNSIGNED *)WaveData.data.channel.value;
            pDst = ( UNSIGNED *)WaveData.data.channel.value;
            for( k = 0; k < RecordNumber; k ++)
            {
                * pDst ++ = * pSrc ++;
            }
            * pDst ++ = CWM_END_FLAG;

            /* ��ȡ�߼������� */
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
    /* ��¼����¼ */
    CWMC_PROTECT_WAVE();
    search_ptr = CWave.WaveList;
    if( search_ptr != XJ_NULL )
    {
        /* ����¼������ */
        do
        {
            pWave = ( CWM_WCB *)search_ptr;
            if((( pWave->Type & iType ) != 0 ) && ( pWave->EndFlag != 0 ))
            {
                variable = pWave->FaultNumber & CWMC_DATA_MASKS16();
                variable += (( pWave->ElecNumber & CWMC_DATA_MASKS16()) << CWMC_WAVE_FAULT_BITS());
                if( uTotalNumber >= CWMC_MAX_NVRAM_WAVE )
                {
                    /* ��ȡ������,����������� */
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
                    /* ��ȡ����δ��,˳���� */
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

/* ��ȡ¼�����ݿ� */
INT CWMC_GetWaveDataBlock( CHAR * fileName, UNSIGNED Offset, UNSIGNED * dst, UNSIGNED size )
{
    INT status;
    FILE * fp;
    UNSIGNED readSize;

    status = XJ_UNAVAILABLE;
    /* ��¼������ */
    fp = xj_fopen( fileName, "rb" );
    if( fp != XJ_NULL )
    {
        /* �����ļ�λ�� */
        if( xj_fseek( fp, Offset, 0 ))
        {
            /* �����дʧ�� */
            CWMC_ErrInf.fseekFile ++;
        }
        else
        {
            /* ��ȡ¼������ */
            readSize = ( UNSIGNED )xj_fread( dst, 1, size, fp );
            if( readSize == size )
            {
                status = XJ_SUCCESS;
            }
            else
            {
                /* ��ȡ�ļ�����ʧ�� */
                CWMC_ErrInf.readFile ++;
            }
        }
        if( xj_fclose( fp ))
        {
            /* �ر��ļ�ʧ�� */
            CWMC_ErrInf.closeFileR ++;
        }
    }
    else
    {
        CWMC_ErrInf.openFileR ++;
    }

    return( status );
}

/* д¼������ */
INT CWMC_WriteWaveDataBlock( CHAR * fileName, UNSIGNED * src, UNSIGNED size )
{
    INT status;
    FILE * fp;
    UNSIGNED writeSize;

    status = XJ_UNAVAILABLE;
    /* д¼������ */
    fp = xj_fopen( fileName, "wb" );
    if( fp != XJ_NULL )
    {
        /* д¼������ */
        writeSize = ( UNSIGNED )xj_fwrite( src, sizeof( UNSIGNED ), size, fp );
        if( xj_fclose( fp ))
        {
            /* �ر��ļ�ʧ�� */
            CWMC_ErrInf.closeFileW ++;
        }
        if( writeSize == size )
        {
            status = XJ_SUCCESS;
        }
        else
        {
            /* д�ļ�����ʧ�� */
            CWMC_ErrInf.writeFile ++;
            if( xj_RemoveFile( fileName ))
            {
                /* ɾ���ļ�ʧ�� */
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

/* ¼��������Żص������������� */
#define CWM_LIST_PARA_NUM       2       /* �������� */
#define CWM_LIST_O_PARA_NUM     0       /* ��������λ��ƫ�� */
#define CWM_LIST_O_DIR_NAME     1       /* ¼��·������λ��ƫ�� */
/* ��ȡ¼��������(�����������+�������)�ص����� */
int CWMC_ListWaveIndexCallBack( struct FileInformation * info, void * context )
{
    INT status;
    UNSIGNED variable;
    CHAR * dirName;
    CHAR * IndexStr;
    UNSIGNED Index;
    UNSIGNED * p;
    CHAR buffer[64]; /* ����·����������'/'�� */

    /* ��ȡ¼����������ַ��� */
    p = ( UNSIGNED *)context;
    dirName = ( CHAR *)(*( p + CWM_LIST_O_DIR_NAME )); /* ¼��·������ */
    sprintf( buffer, "%s/", dirName );
    IndexStr = XJP_CSC_Strcmp( info->Name, buffer );
    Index = ( UNSIGNED )( atol( IndexStr ));
    if( Index == 0 )
    {
        return( XJ_UNAVAILABLE );
    }
    /* д���������� */
    XJP_Index_Write( CWMC_WaveHandle, Index, & variable, & status );
    if( status == XJ_TRUE )
    {
        /* �������֧����Ŀ��ɾ��¼���ļ� */
        sprintf( buffer, "%s/%d", dirName, variable );
        if( xj_RemoveFile( buffer ))
        {
            /* ɾ���ļ�ʧ�� */
            CWMC_ErrInf.deleteFileL ++;
            /* ����ɾ���ļ��б� */
            CFDC_Delete_File( buffer );
        }
    }
    return( XJ_UNAVAILABLE );
}

/* ��ȡ¼��������(�����������+�������) */
INT CWMC_ListWaveIndex( CHAR * dirName, UNSIGNED * Dst,
    UNSIGNED * pActual_size, UNSIGNED Number )
{
    INT status;
    INT ActualNumber;
    UNSIGNED FuncPara[CWM_LIST_PARA_NUM]; /* �ص��������� */

    /* ������� */
    XJP_Index_Clear( CWMC_WaveHandle );
    /* ���ݲ����б� */
    FuncPara[CWM_LIST_O_PARA_NUM] = CWM_LIST_PARA_NUM; /* �������� */
    FuncPara[CWM_LIST_O_DIR_NAME] = ( UNSIGNED )dirName; /* ¼��·������ */
    ActualNumber = CWMC_LISTFILE( dirName, CWMC_ListWaveIndexCallBack, FuncPara );
    /* ��ȡָ�����ݸ���������������(�����¿�ʼ) */
    status = XJP_Index_List( CWMC_WaveHandle, Dst, pActual_size, Number );

    return( status );
}

/* ��ȡ¼�������������(�������) */
INT CWMC_GetWaveFaultNumber( CHAR * dirName, UNSIGNED * Dst,
    UNSIGNED * pActual_size, INT type, UNSIGNED Number )
{
    INT status;

    if( type == 0 )
    {
        /* ��ȡָ�����ݸ�����������(¼�������������+�������)����(�����¿�ʼ) */
        status = XJP_Index_List( CWMC_WaveHandle, Dst, pActual_size, Number );
    }
    else
    {
        /* ��ȡָ�����ݸ�����������(¼�������������)����(�����¿�ʼ) */
        status = XJP_Index_ListH( CWMC_WaveHandle, Dst, pActual_size, Number );
    }

    return( status );
}

/* ����ָ��������ŵ�¼��������� */
INT CWMC_FindWaveIndex( CHAR * dirName, UNSIGNED * waveIndex, UNSIGNED faultIndex )
{
    UNSIGNED Number;
    UNSIGNED ActualNumber;
    UNSIGNED CondMasks;
    UNSIGNED CondValue;
    INT status;

    /* ��ȡָ��������ŵ����������� */
    Number = 1; /* ���ݸ��� */
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

/* ɾ��¼��Ŀ¼ȫ���ļ��ص������������� */
#define CWM_DEL_F_PARA_NUM      2       /* �������� */
#define CWM_DEL_F_O_PARA_NUM    0       /* ��������λ��ƫ�� */
#define CWM_DEL_F_O_ACT_NUM     1       /* ɾ���ļ����� */
/* ɾ��¼��ָ��Ŀ¼�ļ��ص����� */
int CWMC_DeleteDirFileCallBack( struct FileInformation * info, void * context )
{
    UNSIGNED * p;
    UNSIGNED actDelFileNumber;

    /* ɾ��¼���ļ� */
    if( xj_RemoveFile( info->Name ) == 0 )
    {
        /* ɾ���ɹ� */
        p = ( UNSIGNED *)context;
        actDelFileNumber = *( p + CWM_DEL_F_O_ACT_NUM ); /* ɾ����¼���ļ����� */
        *( p + CWM_DEL_F_O_ACT_NUM ) = (++ actDelFileNumber );
    }
    else
    {
        /* ɾ��Ŀ¼�ļ�ʧ�� */
        CWMC_ErrInf.deleteDirFile ++;
        /* ����ɾ���ļ��б� */
        CFDC_Delete_File( info->Name );
    }
    if( CWave.hWatchDogId )
    {
        XJP_Watchdog_Reset( CWave.hWatchDogId ); /* ��λ�����Ӷ��� */
    }
    return( XJ_UNAVAILABLE );
}

/* ɾ��ָ��Ŀ¼�µ������ļ� */
INT CWMC_DeleteDirFile( CHAR * dirName )
{
    UNSIGNED ActualNumber;
    UNSIGNED FuncPara[CWM_DEL_F_PARA_NUM]; /* �ص��������� */

    /* ɾ��¼���ļ��Ļص����� */
    FuncPara[CWM_DEL_F_O_PARA_NUM] = CWM_DEL_F_PARA_NUM; /* �������� */
    FuncPara[CWM_DEL_F_O_ACT_NUM] = 0; /* ɾ��¼���ļ��ĸ��� */
    ActualNumber = ( UNSIGNED )CWMC_LISTFILE( dirName, CWMC_DeleteDirFileCallBack, FuncPara );
    /* ���¼������������ */
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

/* ¼��Ŀ¼�ļ���Ŀ���ص������������� */
#define CWM_FULL_F_PARA_NUM     6       /* �������� */
#define CWM_FULL_F_O_PARA_NUM   0       /* ��������λ��ƫ�� */
#define CWM_FULL_F_O_DIR_NAME   1       /* ·������λ��ƫ�� */
#define CWM_FULL_F_O_REQ_NUM    2       /* �����������ݸ���λ��ƫ�� */
#define CWM_FULL_F_O_ACT_NUM    3       /* ʵ�����ݸ���λ��ƫ�� */
#define CWM_FULL_F_O_DST        4       /* ��������ַλ��ƫ�� */
#define CWM_FULL_F_O_DEL_NUM    5       /* ɾ���ļ�����λ��ƫ�� */
/* ¼��Ŀ¼�ļ���Ŀ���ص����� */
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
    CHAR buffer[64]; /* ����·����������'/'�� */

    /* ��ȡ����ַ��� */
    p = ( UNSIGNED *)context;
    dirName = ( CHAR *)(*( p + CWM_FULL_F_O_DIR_NAME )); /* ·������ */
    reqDataNumber = *( p + CWM_FULL_F_O_REQ_NUM ); /* ��������ݸ��� */
    pDst = ( UNSIGNED *)(*( p + CWM_FULL_F_O_DST )); /* ��������ַ */
    sprintf( buffer, "%s/", dirName );
    IndexStr = XJP_CSC_Strcmp( info->Name, buffer );
    Index = ( UNSIGNED )( atol( IndexStr ));
    if( Index == 0 )
    {
        return( XJ_UNAVAILABLE );
    }
    /* ������������ */
    status = XJP_CSC_Arrage_Insert_Full( pDst, ( p + CWM_FULL_F_O_ACT_NUM ),
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
            actDelFileNumber = *( p + CWM_FULL_F_O_DEL_NUM ); /* ɾ�����ļ����� */
            *( p + CWM_FULL_F_O_DEL_NUM ) = (++ actDelFileNumber );
        }
        else
        {
            /* ɾ�������Ч�ļ�ʧ�� */
            CWMC_ErrInf.deleteCheckFile ++;
            /* ����ɾ���ļ��б� */
            CFDC_Delete_File( buffer );
        }

        /* ɾ��¼������������ */
        XJP_Index_Delete( CWMC_WaveHandle, InvalidData );
    }
    return( XJ_UNAVAILABLE );
}

/* ¼��Ŀ¼�ļ���Ŀ��� */
INT CWMC_FullFileCheck( CHAR * dirName, UNSIGNED * Dst, UNSIGNED * pDeleteNumber, UNSIGNED MaxNumber )
{
    INT ActualNumber;
    UNSIGNED FuncPara[CWM_FULL_F_PARA_NUM]; /* �ص��������� */

    /* ���ݲ����б� */
    FuncPara[CWM_FULL_F_O_PARA_NUM] = CWM_FULL_F_PARA_NUM; /* �������� */
    FuncPara[CWM_FULL_F_O_DIR_NAME] = ( UNSIGNED )dirName; /* ·������ */
    FuncPara[CWM_FULL_F_O_REQ_NUM] = MaxNumber; /* �����������ݸ��� */
    FuncPara[CWM_FULL_F_O_ACT_NUM] = 0; /* ʵ�����ݸ��� */
    FuncPara[CWM_FULL_F_O_DST] = ( UNSIGNED )Dst; /* ��������ַ */
    FuncPara[CWM_FULL_F_O_DEL_NUM] = 0; /* ɾ���ļ����� */
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

/* ¼��Ŀ¼�ļ���������� */
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
    CHAR fileName[128]; /* ����·����������'/'�� */
    CHAR tempFileName[128]; /* ��ʱ�ļ��� */
    CWM_WHD waveHead;
    UNSIGNED uStartIndex;  /* һ�ι���¼������ʼ¼�������� */
    UNSIGNED uMaskBits;
    UNSIGNED uCheckData;

    switch( pRequest -> nu_function )
    {
        /* ��ʼ�� */
    case XJ_INITIALIZE:
        /* ��ʼ��¼����Ϣ */
        CWave.FaultNumber = 0;
        CWave.ElecNumber = 0;
        CWave.UserFaultNumber = CWave.FaultNumber;
        CWave.UserElecNumber = CWave.ElecNumber;
        CWave.ReadyFaultNumber = CWave.ElecNumber;
        CWave.ReadySerialNumber = CWave.FaultNumber;

        /* ���¼������ */
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
            /* ��ȡ¼������������ź�¼��������� */
            CWMC_ListWaveIndex( CWave.dirName, CWMC_WaveIndex,& Number, CWMC_MAX_NVRAM_WAVE );
            if( Number > 0 )
            {
                variable = CWMC_WaveIndex[( Number - 1 )]; /* ����¼������������ź�¼����� */
                CWave.FaultNumber = variable & CWMC_DATA_MASKS16(); /* ����¼����� */
                CWave.UserFaultNumber = CWave.FaultNumber;
                CWave.ElecNumber = ( variable >> CWMC_DATA_BITS16() ) & CWMC_DATA_MASKS16(); /* ���µ���������� */
                CWave.UserElecNumber = CWave.ElecNumber;
                CWave.ReadyFaultNumber = CWave.ElecNumber;
                CWave.ReadySerialNumber = CWave.FaultNumber;
                variable &= CWMC_DATA_MASKSH16(); /* ��ȡ��16λ���� */
                uStartIndex = CWave.ReadySerialNumber;
                /* ����һ�ε���������ʼ¼�������� */
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

        /* ¼����¼��Ϣ��ʼ�� */
        if( CWave.WaveList != 0 )
        {
            CWMC_PROTECT_WAVE();
            /* ���¼����¼ָ�� */
            WavePointer = XJ_NULL;
            for( k = 0; k < CWM_MAX_WAVE_POINTER; k ++ )
            {
                WavePointerCB[ k ] = XJ_NULL;
            }
            do
            {
                /* �ͷſռ� */
                CWMC_WAVE_DELLOCATE( status, CWave.WaveList );
                /* ɾ������ */
                CWMC_REMOVE_WAVE_LIST( CWave.WaveList );

                if( CWave.WaveList == 0 )
                {
                    break;
                }
            }while( 1 );

            CWMC_UNPROTECT_WAVE();
        }
        break;

        /* ��ȡ¼������Ȩ */
    case XJ_ASSIGN:
        CWMC_LOCK_WAVE( pRequest -> nu_status, pRequest -> nu_timeout );
        break;

        /* �ͷ�¼������Ȩ */
    case XJ_RELEASE:
        CWMC_UNLOCK_WAVE( pRequest -> nu_status );
        break;

        /* ��¼������ */
    case XJ_INPUT:
        destination = ( UNSIGNED *)( pRequest ->
            nu_request_info.nu_input.nu_buffer_ptr );
        index = ( pRequest->nu_request_info.nu_input.nu_logical_unit ) & CWMC_DATA_MASKS16();
        size = pRequest -> nu_request_info.nu_input.nu_request_size;
        offset = pRequest -> nu_request_info.nu_input.nu_offset;
        type = pRequest -> nu_supplemental;
        /* ��ȡ¼�����(�������(��16λ)+¼��������(��16λ)) */
        if( CWMC_FindWaveIndex( CWave.dirName, & variable, index ) != XJ_SUCCESS )
        {
            /* ��ȡ¼�����ʧ�� */
            pRequest -> nu_status = XJ_END_OF_LOG;
            return;
        }
        sprintf( fileName, "%s/%d", CWave.dirName, variable );
        if( xj_GetFileInformation( fileName,& fileInfo ) == XJ_SUCCESS )
        {
            if( fileInfo.Size < sizeof( CWM_WHD ))
            {
                /* �ļ���С����ȷ��ֱ�ӷ���ʧ�� */
                pRequest -> nu_status = XJ_END_OF_LOG;
                return;
            }
            /* ��ȡ¼������ͷ(Index-�������(��16λ)+¼��������(��16λ)) */
            if( CWMC_GetWaveDataBlock( fileName, 0,( UNSIGNED *)& waveHead, sizeof( CWM_WHD )) == XJ_SUCCESS )
            {
                startOffset = 0;
                /* ¼��ƫ�Ƽ��� */
                if( type != 0 )
                {
                    TotalPoint = waveHead.TotalPointer; /* �ܵ��� */
                    AnalogNumber = waveHead.AnalogNumber; /* ģ����ͨ���� */
                    if( type <= AnalogNumber )
                    {
                        /* ��ģ���� */
                        variable = ( TotalPoint + CWMC_ANALOG_BANK_WIDTH()- 1 )/ CWMC_ANALOG_BANK_WIDTH();
                        variable += (( sizeof( CWM_VHD )+ sizeof( UNSIGNED ))/ sizeof( UNSIGNED ));
                        startOffset = sizeof( CWM_WHD )/ sizeof( UNSIGNED );
                        startOffset += (( type - 1 )* variable );
                    }
                    else
                    {
                        /* ����ģ����λ�� */
                        variable = ( TotalPoint + CWMC_ANALOG_BANK_WIDTH()- 1 )/ CWMC_ANALOG_BANK_WIDTH();
                        variable += (( sizeof( CWM_VHD )+ sizeof( UNSIGNED ))/ sizeof( UNSIGNED ));
                        startOffset = sizeof( CWM_WHD )/ sizeof( UNSIGNED );
                        startOffset += ( AnalogNumber * variable );
                        /* ����ʵ��λ�� */
                        variable = TotalPoint +(( sizeof( CWM_VHD )+ sizeof( UNSIGNED ))/ sizeof( UNSIGNED ));
                        startOffset += (( type - AnalogNumber - 1 )* variable );
                    }
                }
                startOffset += offset;
                startOffset *= sizeof( UNSIGNED );
                /* ��¼�����ݿ� */
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
                /* ¼��ͷ��Ϣ��ȡ���ɹ�������ʧ��״̬ */
                pRequest -> nu_status = XJ_END_OF_LOG;
            }
        }
        else
        {
            /* ��ȡ��Ϣʧ�� */
            CWMC_ErrInf.getInfoFile ++;
            pRequest -> nu_status = XJ_END_OF_LOG;
        }
        break;

        /* д¼������ */
    case XJ_OUTPUT:
        p = ( UNSIGNED *)( pRequest->nu_request_info.nu_output.nu_buffer_ptr );
        size = pRequest -> nu_request_info.nu_output.nu_request_size;
        index = CWave.FaultNumber1 & CWMC_DATA_MASKS16(); /* ��16λ:¼��������� */
        index += (( CWave.ElecNumber1 & CWMC_DATA_MASKS16() ) << CWMC_DATA_BITS16() ); /* ��16λ:����������� */
        /* д¼������ */
        sprintf( fileName, "%s/%d", CWave.dirName, index );
        sprintf( tempFileName, "%s/tempWave", CWave.dirName );
        if( CWMC_WriteWaveDataBlock( tempFileName, p, size ) == XJ_SUCCESS )
        {
            if( xj_RenameFile( tempFileName, fileName )== XJ_SUCCESS )
            {
                /* д¼���ɹ�,���¹�����Ϣ */
                CWave.FaultNumber = CWave.FaultNumber1;
                if( CWave.ElecNumber1 )
                {
                    CWave.ElecNumber = CWave.ElecNumber1;
                }
                /* �������������� */
                XJP_Index_Write( CWMC_WaveHandle, index, & variable, & status );
                if( status == XJ_TRUE )
                {
                    /* �������֧����Ŀ��ɾ��¼���ļ� */
                    sprintf( fileName, "%s/%d", CWave.dirName, variable );
                    if( xj_RemoveFile( fileName ))
                    {
                        /* ɾ���ļ�ʧ�� */
                        CWMC_ErrInf.deleteFullFile ++;
                        /* ����ɾ���ļ��б� */
                        CFDC_Delete_File( fileName );
                    }
                }
                pRequest -> nu_status = XJ_SUCCESS;
            }
            else
            {
                /* �������ļ���ʧ�� */
                CWMC_ErrInf.renameFile ++;
                pRequest -> nu_status = XJ_UNAVAILABLE;
            }
        }
        else
        {
            /* д¼��ʧ�� */
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

            /* �жϼ�����ݵ���Чλ�� */
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
            /* �����Ч���ݸ��� */
            size = XJP_CSC_Get_Valid_Number( destination, size, uCheckData, 1, uMaskBits );
            pRequest->nu_request_info.nu_status.nu_logical_unit = size;
            pRequest -> nu_status = status;
        }
        break;
    }
}

/* ����¼��(���) */
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

    /* ����Ƿ��ж��¼���������Ŷӵȴ���¼ */
    for( i = 1; i < CWM_MAX_WAVE_POINTER; i ++ )
    {
        if( WavePointerCB[ i ] )
        {
            /* ���¼���ȴ���¼����ֹ�ٴδ�����ֱ�ӷ���ʧ�� */
            return( XJ_UNAVAILABLE );
        }
    }
    iSpace = * spacePtr;
    /* �¹���¼����� */
    newFault = XJ_TRUE;
    pWave = WavePointer;
    if( pWave != XJ_NULL )
    {
        if( pWave->ElecNumber == ( UNSIGNED )iElecNumber )
            /* ����¼��Ϊͬһ�ι��ϵĶ��¼�� */
            newFault = XJ_FALSE;
    }
    if( CWave.ElecNumber == ( UNSIGNED )iElecNumber )
        /* ����¼��Ϊͬһ�ι��ϵĶ��¼�� */
        newFault = XJ_FALSE;
    if( CWave.WaveList != XJ_NULL )
    {
        search_ptr = CWave.WaveList;
        /* ���ҹ��ϲ������� */
        do
        {
            pWave = ( CWM_WCB *)search_ptr;
            if( pWave->ElecNumber == ( UNSIGNED )iElecNumber )
            {
                /* ����¼��Ϊͬһ�ι��ϵĶ��¼�� */
                newFault = XJ_FALSE;
                break;
            }
            search_ptr = search_ptr -> next;
        }while( CWave.WaveList != search_ptr );
    }

    pWave = WavePointer;
    if( pWave != XJ_NULL )
    {
        /* ����ͬʱ¼��,�������ȼ��б� */
        if(( pWave->Type & WAVE_TYPE_START_FAULT ) != 0 )
            /* �¹��ϵ�һ��¼�� */
            LastPriority = 1;
        /* �ϴ�¼�������ȼ� */
        else if(( pWave->Type & WAVE_TYPE_SOF_TEST ) != 0 )
            /* �ֶ�¼�� */
            LastPriority = 3;
        else if(( pWave->Type & WAVE_TYPE_START ) != 0 )
            /* ����¼�� */
            LastPriority = 2;
        else
            /* ����¼�� */
            LastPriority = 1;

        /* ����¼�������ȼ� */
        if( newFault == XJ_TRUE )
            /* �¹���¼�� */
            NextPriority = 1;
        else if(( iType & WAVE_TYPE_SOF_TEST ) != 0 )
            /* �ֶ�¼�� */
            NextPriority = 3;
        else if(( iType & WAVE_TYPE_START ) != 0 )
            /* ����¼�� */
            NextPriority = 2;
        else
            /* ����¼�� */
            NextPriority = 1;

        if( LastPriority < NextPriority )
        {
            /* ���ȼ��ȸߺ��,����ʧ�� */
            return( XJ_UNAVAILABLE );
        }
        else if( LastPriority > NextPriority )
        {
            /* ���ȼ��ȵͺ��,��ֹ�ϴ�,�������� */
            /* ��ֹ¼�� */
            WavePointer = XJ_NULL;
            WavePointerCB[ 0 ] = XJ_NULL;
            /* �ͷſռ� */
            CWMC_WAVE_DELLOCATE( status, pWave );
            /* ɾ������ */
            CWMC_REMOVE_WAVE_LIST(&( pWave->Created ));
            /* �ָ�ԭ������� */
            CWave.UserFaultNumber --;

            /* ���¼����Ȼ����Ƿ���(1~0xFFFF)��Χ�� */
            CWMC_CHECK_DATA_LIMIT( CWave.UserFaultNumber, CWM_FAULT_NUMBER_MIN, CWM_FAULT_NUMBER_MAX );
        }
        else
        {
            /* ��ͬ���ȼ�,��������ѡ�� */
            if( NextPriority == 1 )
            {
                /* ����¼�� */
                if( pWave->SampleSpace < (( UNSIGNED )iSpace ))
                {
                    /* �ȸ��ܶȺ���ܶ�,����ʧ�� */
                    return( XJ_UNAVAILABLE );
                }

                if( pWave->SampleSpace == (( UNSIGNED )iSpace ))
                {
                    /* ͬ�ܶ� */
                    if( pWave->TotalPointer > ( 5 * pWave->RecordNumber ))
                    {
                        /* ��һ��¼������С��20%����ʧ�� */
                        return( XJ_UNAVAILABLE );
                    }

                    /* ��С��¼�����б� */
                    if( pWave->RecordNumber < CWMC_GET_MIN_RECORD_NUMBER())
                    {
                        if(( pWave->Type & WAVE_TYPE_START_FAULT ) != 0 )
                        {
                            /* �¹��ϵ�һ��¼���ļ�¼����С����С��¼����,ֱ�ӷ���ʧ�� */
                            return( XJ_UNAVAILABLE );
                        }
                        else
                        {
                            /* ���¼����¼�����ݵ���С����С��¼����,ֱ�Ӷ����ϴμ�¼���� */
                            WavePointer = XJ_NULL;
                            WavePointerCB[ 0 ] = XJ_NULL;
                            /* �ͷſռ� */
                            CWMC_WAVE_DELLOCATE( status, pWave );
                            /* ɾ������ */
                            CWMC_REMOVE_WAVE_LIST(&( pWave->Created ));
                            /* �ָ�ԭ������� */
                            CWave.UserFaultNumber --;

                            /* ���¼����Ȼ����Ƿ���(1~0xFFFF)��Χ�� */
                            CWMC_CHECK_DATA_LIMIT( CWave.UserFaultNumber, CWM_FAULT_NUMBER_MIN, CWM_FAULT_NUMBER_MAX );
                        }
                    }
                }
            }
            else
            {
                /* �������ֶ�¼��,����ʧ�� */
                return( XJ_UNAVAILABLE );
            }
        }
    }

    if((( iType & WAVE_TYPE_SOF_TEST ) != 0 ) && ( CWave.WaveList != XJ_NULL ))
    {
        /* ����Ϊ�ֶ�¼��,����Ѿ����ڵ��ֶ�¼�� */
        search_ptr = CWave.WaveList;
        while( 1 )
        {
            if( CWave.WaveList != search_ptr->next )
            {
                pWave = ( CWM_WCB *)search_ptr;
                search_ptr = search_ptr -> next;
                if((( pWave->Type & WAVE_TYPE_SOF_TEST ) != 0 ))
                {
                    /* �ͷ��ֶ�¼���ռ� */
                    CWMC_WAVE_DELLOCATE( status, pWave );
                    /* ɾ������ */
                    CWMC_REMOVE_WAVE_LIST(&( pWave->Created ));
                }
            }
            else
            {
                pWave = ( CWM_WCB *)search_ptr;
                if((( pWave->Type & WAVE_TYPE_SOF_TEST ) != 0 ))
                {
                    /* �ͷ��ֶ�¼���ռ� */
                    CWMC_WAVE_DELLOCATE( status, pWave );
                    /* ɾ������ */
                    CWMC_REMOVE_WAVE_LIST(&( pWave->Created ));
                }
                break;
            }
        }
    }

    /* ¼��������� */
    if( waveSegments > CWM_MAX_WAVE_POINTER )
        iWaveNumber = CWM_MAX_WAVE_POINTER;
    else
        iWaveNumber = waveSegments;
    /* ��ʼ�������ϼ�� */
    if( newFault == XJ_TRUE )
        iWaveType = iType | WAVE_TYPE_START_FAULT;
    else
        iWaveType = iType;

    /* ����¼������ */
    status = XJ_SUCCESS;
    iCtrlType = CWM_WAVE_CTRL_NEW;
    for( i = 0; i < ( UNSIGNED )iWaveNumber; i ++ )
    {
        iSpace = * ( spacePtr + i ); /* ¼����� */
        iTotalPoint = * ( totalPointPtr + i ); /* �ܵ��� */
        status = CWMC_CreateWaveList( iElecNumber, iWaveType, iStartPoint,
            iSpace, iTotalPoint, WaveChannel, pTime, iCtrlType, pState, iState );
        iCtrlType = 0;
        iWaveType = iType; /* ������������ŷ����仯ʱ������ʼ�������ϱ�־ */
        if( status != XJ_SUCCESS ) /* ����ʧ�ܣ�ֱ�ӷ��� */
            break;
    }

    return( status );
}

/* ����¼������ */
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
    /* ������� */
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

    /* ���붯̬�ڴ� */
    /* ��¼¼��ͷ�ڴ� */
    size = sizeof( CWM_WCB );
    size += ( sizeof( CWM_ANALOG ) * AnalogNumber );
    size += ( sizeof( CWM_INPUT ) * InputNumber );
    size += ( sizeof( CWM_OUTPUT ) * OutputNumber );
    size += ( sizeof( CWM_STATE ) * StateNumber );
    size += ( sizeof( CWM_LOGICAL ) * LogicalNumber );
    /* ��ֵ�ڴ� */
    i = AnalogNumber;
    i += InputNumber;
    i += OutputNumber;
    i += StateNumber;
    i += LogicalNumber;
    variable = ( TotalPointNumber * sizeof( UNSIGNED )) + sizeof( CWM_VHD ) + sizeof( UNSIGNED );
    size += ( variable * i );

    /* ¼��ͨ��������� */
    if( i == 0 )
    {
        /* û��¼��ͨ��,���ش��� */
        return( XJ_UNAVAILABLE );
    }

    /* ���붯̬�ڴ� */
    CWMC_WAVE_ALLOCATE( status,& memory_ptr, size );
    if(( status != XJ_SUCCESS ) && ( CWave.WaveList != XJ_NULL ))
    {
        /* �ڴ�ռ䲻��,�ͷ�����¼�����ֶ�¼�� */
        search_ptr = CWave.WaveList;
        while( 1 )
        {
            if( CWave.WaveList != search_ptr->next )
            {
                pWave = ( CWM_WCB *)search_ptr;
                search_ptr = search_ptr -> next;
                if(( pWave->Type & WAVE_TYPE_START_FAULT ) == 0 )
                {
                    /* ��ֹɾ���¹��ϵ�һ��¼������ */
                    if((( pWave->Type & WAVE_TYPE_SOF_TEST ) != 0 )
                        || (( pWave->Type & WAVE_TYPE_START ) != 0 ))
                    {
                        /* �ͷ��������ֶ���¼���ռ� */
                        CWMC_WAVE_DELLOCATE( status, pWave );
                        /* ɾ������ */
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
                    /* ��ֹɾ���¹��ϵ�һ��¼������ */
                    if((( pWave->Type & WAVE_TYPE_SOF_TEST ) != 0 )
                        || (( pWave->Type & WAVE_TYPE_START ) != 0 ))
                    {
                        /* �ͷ��ֶ�¼���ռ� */
                        CWMC_WAVE_DELLOCATE( status, pWave );
                        /* ɾ������ */
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
        /* �����ڴ�ɹ� */
        /* ����������� */
        CWave.UserFaultNumber ++;

        /* ���¼����Ȼ����Ƿ���(1~0xFFFF)��Χ�� */
        CWMC_CHECK_DATA_LIMIT( CWave.UserFaultNumber, CWM_FAULT_NUMBER_MIN, CWM_FAULT_NUMBER_MAX );

        /* ������� */
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
        /* ��·Ƶ�� */
        CWMC_SAMPLE_FREQUENCY( pWave -> LineFrequency );
        pWave -> SampleSpace = ( UNSIGNED )iSpace;
        if( pWave -> SampleSpace <= 0 )
        {
            pWave -> SampleSpace = 1;
        }
        /* ������(ÿ���������) */
        CWMC_SAMPLE_RATE( pWave -> SampleRate, pWave -> SampleSpace );
        pWave -> RecordNumber = 0;
        pWave -> SampleNumber = 0;
        /* ����ǰ�����߽��� */
        if( iStartPoint >= CWMC_MAX_SAMPLE_POINT )
            variable = ( UNSIGNED )( CWMC_MAX_SAMPLE_POINT - 1 );
        else
            variable = ( UNSIGNED )iStartPoint;
        pWave -> StartPointer = variable;
        pWave -> TotalPointer = TotalPointNumber;
        /* ���װ��ʱ�� */
        if( pTime == XJ_NULL )
        {
            XJP_TM_Get_Time_Dev( & DevTime );
            pDevTime = & DevTime;
        }
        else
        {
            pDevTime = pTime;
        }
        pWave -> Year = pDevTime -> Year; /* �� */
        pWave -> Month = pDevTime -> Month; /* �� */
        pWave -> Day = pDevTime -> Day; /* �� */
        pWave -> Hour = pDevTime -> Hour; /* ʱ */
        pWave -> Minute = pDevTime -> Minute; /* �� */
        pWave -> Millionsecond = pDevTime -> Millionsecond; /* �루���룩 */
        pWave -> Nanosecond = pDevTime -> Nanosecond; /* �루���룩 */
        pWave -> tQuality = pDevTime -> Quality; /* ʱ��Ʒ������ */
        if( pDeviceTab )
        {
            pWave -> WaveDataType = pDeviceTab -> ParaShowType; /* ¼����������(һ/����) */
        }
        else
        {
            /* ���û��װ�����ݱ�̶�Ϊ2�� */
            pWave -> WaveDataType = PRO_DATA_TYPE_SECONDARY;
        }

        /* ģ������Ϣ */
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

        /* ��������Ϣ */
        pWave -> InputInf = ( CWM_INPUT *)( pWave->AnalogInf + pWave->AnalogNumber );
        p1 = pWaveChannel->pInputNumber;
        p2 = pWaveChannel->pInputMasks;
        for( i = 0; i < InputNumber; i ++)
        {
            ( pWave->InputInf + i ) -> GroupNumber = * p1 ++;
            ( pWave->InputInf + i ) -> MaskBits = * p2 ++;
        }

        /* ��������Ϣ */
        pWave -> OutputInf = ( CWM_OUTPUT *)( pWave->InputInf + pWave->InputNumber );
        p1 = pWaveChannel->pOutputNumber;
        p2 = pWaveChannel->pOutputMasks;
        for( i = 0; i < OutputNumber; i ++)
        {
            ( pWave->OutputInf + i ) -> GroupNumber = * p1 ++;
            ( pWave->OutputInf + i ) -> MaskBits = * p2 ++;
        }

        /* ״̬����Ϣ */
        pWave -> StateInf = ( CWM_STATE *)( pWave->OutputInf + pWave->OutputNumber );
        p1 = pWaveChannel->pStateNumber;
        p2 = pWaveChannel->pStateMasks;
        for( i = 0; i < StateNumber; i ++)
        {
            ( pWave->StateInf + i ) -> GroupNumber = * p1 ++;
            ( pWave->StateInf + i ) -> MaskBits = * p2 ++;
        }

        /* �߼�����Ϣ */
        pWave -> LogicalInf = ( CWM_LOGICAL *)( pWave->StateInf + pWave->StateNumber );
        p1 = pWaveChannel->pLogicalNumber;
        p2 = pWaveChannel->pLogicalMasks;
        for( i = 0; i < LogicalNumber; i ++)
        {
            ( pWave->LogicalInf + i ) -> GroupNumber = * p1 ++;
            ( pWave->LogicalInf + i ) -> MaskBits = * p2 ++;
        }

        /* ���ж� */
        CWMC_LOCK_INTERRUPTS( Number );
        pWave -> WaveMemory = ( UNSIGNED *)( pWave->LogicalInf + pWave->LogicNumber );
        if( ctrlType == CWM_WAVE_CTRL_NEW )
        {
            /* ����¼��״̬�� */
            if( pState )
                CWMC_SetWaveState( pState, iState );
            /* �´���¼����ֹ��ǰ��¼��¼������  */
            if( WavePointer != XJ_NULL )
            {
                /* ����ɱ�־ */
                WavePointer -> EndFlag = 1;
                /* ��ֹ¼�� */
                WavePointer = XJ_NULL;
            }
            WavePointer = pWave;
            WavePointerCB[0] = pWave;
        }
        else
        {
            /* ͬһ�δ����Ķ��¼������¼������¼����Ϣ */
            if( WavePointer == XJ_NULL )
            {
                WavePointer = pWave;
            }
            /* ������ɵ�¼����Ϣ����¼���ƿ��� */
            for( i = 0; i < CWM_MAX_WAVE_POINTER; i ++ )
            {
                if( WavePointerCB[i] == XJ_NULL )
                {
                    WavePointerCB[i] = pWave;
                    break;
                }
            }
        }
        /* ���ж� */
        CWMC_UNLOCK_INTERRUPTS( Number );
    }
    else
    {
        /* �����ڴ�ʧ�� */
        CWMC_ErrInf.waveMalloc ++;
    }

    return( status );
}

/* ����¼��(һ��) */
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

/* ��ȡ¼��ͷ����Ϣ */
INT CWMC_GetWaveHead( CWM_WCB * source, VOID * destination )
{
    UNSIGNED i;
    CWM_WHD * pHead;
    CWM_WCB * pSrc;
    UNSIGNED size;
    UNSIGNED variable;

    pSrc = source;
    pHead = ( CWM_WHD *)destination;
    /* ¼��ͷ���ݴ�С���� */
    size = ( sizeof( CWM_WHD )/ sizeof( UNSIGNED ));
    /* ģ������С���� */
    variable = ( pSrc->RecordNumber + CWMC_ANALOG_BANK_WIDTH()- 1 )/ CWMC_ANALOG_BANK_WIDTH();
    variable += (( sizeof( CWM_VHD )+ sizeof( UNSIGNED ))/ sizeof( UNSIGNED ));
    variable *= pSrc -> AnalogNumber;
    size += variable;
    /* ����������������״̬�����߼�����С���� */
    variable = pSrc->RecordNumber;
    variable += (( sizeof( CWM_VHD )+ sizeof( UNSIGNED ))/ sizeof( UNSIGNED ));
    i = pSrc -> InputNumber;
    i += pSrc -> OutputNumber;
    i += pSrc -> StateNumber;
    i += pSrc -> LogicNumber;
    size += ( variable * i );
    /* ¼�����ݵĴ�С */
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
        /* �ȴ���ʼ����� */
        return( XJ_TRUE );
    }

    /* ���¼�� */
    CWMC_GET_WAVE_CLEAR( Status,& variable );
    if( Status == XJ_SUCCESS )
    {
        /* ¼����ʼ��,���е�����ȫ����ʧ */
        if( CWave.Initialize(& CWave, 1, CWMC_GET_MAX_WAVE_TIME()) == XJ_SUCCESS )
        {
            ClearNumber = 0;
            /* ���¼���ɹ� */
            sprintf( EventName, " %03d-Ok", CPC_RES_EVENT_WAVE_CLR );
            CWMC_FILL_RESERVE_EVENT( EventName );
            CWMC_SET_WAVE_CLEAR();
        }
        else
        {
            /* ���ִ�д��� */
            if((++ ClearNumber ) > 5 )
            {
                ClearNumber = 0;
                /* ���¼������ */
                sprintf( EventName, " %03d-Err", CPC_RES_EVENT_WAVE_CLR );
                CWMC_FILL_RESERVE_EVENT( EventName );
                CWMC_SET_WAVE_CLEAR();
            }
        }

        if( GroupNumber != 0 )
        {
            /* �ر�¼���豸 */
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
    /* ��¼����¼ */
    CWMC_PROTECT_WAVE();
    search_ptr = CWave.WaveList;
    if( search_ptr != XJ_NULL )
    {
        /* ���ҹ��ϲ������� */
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
        /* ��¼¼�����,д¼�� */
        TotalNumber = pRecord->AnalogNumber + pRecord->InputNumber;
        TotalNumber += ( pRecord->OutputNumber + pRecord->StateNumber );
        TotalNumber += pRecord->LogicNumber;

        /* ��¼���豸 */
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
        /* д¼��ͷ������ */
        else if( GroupNumber == 1 )
        {
            /* ��ȡ¼��ͷ����Ϣ */
            CWMC_GetWaveHead( pRecord, CWMC_WaveDataBuf );

            ActualDataNumber = CWMC_WAVE_HEAD_WORD();
            pWaveHead = ( CWM_WHD *)CWMC_WaveDataBuf;

            GroupNumber ++;
            SubNumber = 0;
            if(( pWaveHead->ElecNumber != CWave.ElecNumber ) && ( CWave.ElecNumber ))
            {
                /* ��ɵĹ������ */
                CWave.ReadyFaultNumber = CWave.ElecNumber;
                CWave.ReadySerialNumber = CWave.FaultNumber;
            }
            CWave.FaultNumber1 = pWaveHead->FaultNumber;
            CWave.ElecNumber1 = pWaveHead->ElecNumber;

            SrcWaveAddr = pRecord -> WaveMemory;
        }
        /* дģ�������� */
        else if( GroupNumber == 2 )
        {
            for( i = SubNumber; i < ( pRecord->AnalogNumber ); i ++)
            {
                /* дģ����ͷ����Ϣ */
                pValueHead = ( CWM_VHD *)& CWMC_WaveDataBuf[ActualDataNumber];
                pAnalog = pRecord->AnalogInf;
                pAnalog += i;
                /* ģ����ͷ����Ϣ */
                pValueHead->head_info.analog.Inf = pAnalog->Inf;
                pValueHead->head_info.analog.Primary_rate = pAnalog->PrimaryRate;
                pValueHead->head_info.analog.Secondary_rate = pAnalog->SecondaryRate;
                if(( pRecord->Type & WAVE_TYPE_COEFFECT ) != 0 )
                {
                    /* ��Чֵ¼�� */
                    pValueHead->head_info.analog.Coefficient =
                        pAnalog->CalculateCoefficient / pAnalog->RateCoefficient * pAnalog->WaveCoefficient
                        / pAnalog->WaveConverCoef;
                }
                else
                {
                    /* ˲ʱֵ¼�� */
                    if(( pAnalog->Type & DC_COMPONENT ) != 0 )
                    {
                        pValueHead->head_info.analog.Coefficient =
                            pAnalog->CalculateCoefficient / pAnalog->RateCoefficient * pAnalog->WaveCoefficient
                            / pAnalog->WaveConverCoef;
                    }
                    else
                    {
                        /* ����������ϵ������2�Ŀ��� */
                        pValueHead->head_info.analog.Coefficient =
                            pAnalog->CalculateCoefficient / pAnalog->RateCoefficient *(( float )0.707107 ) * pAnalog->WaveCoefficient
                            / pAnalog->WaveConverCoef;
                    }
                }

                /* ģ�������� */
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
                            /* ���޼�� */
                            variable = 0x7fff;
                        }
                        else if( point_value < CWMC_MIN_ANALOG_VALUE())
                        {
                            /* ���޼�� */
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
                        /* ���޼�� */
                        variable = 0x7fff;
                    }
                    else if( point_value < CWMC_MIN_ANALOG_VALUE())
                    {
                        /* ���޼�� */
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
        /* д���������� */
        else if( GroupNumber == 3 )
        {
            for( i = SubNumber; i < ( pRecord->InputNumber ); i ++)
            {
                pValueHead = ( CWM_VHD *)& CWMC_WaveDataBuf[ActualDataNumber];
                pInput = pRecord->InputInf;
                pInput += i;

                /* ������ͷ����Ϣ */
                pValueHead->head_info.input.GroupNumber = pInput->GroupNumber;
                pValueHead->head_info.input.MaskBits = pInput->MaskBits;

                /* ���������� */
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
        /* д���������� */
        else if( GroupNumber == 4 )
        {
            for( i = SubNumber; i < ( pRecord->OutputNumber ); i ++)
            {
                pValueHead = ( CWM_VHD *)& CWMC_WaveDataBuf[ActualDataNumber];
                pOutput = pRecord->OutputInf;
                pOutput += i;

                /* ������ͷ����Ϣ */
                pValueHead->head_info.output.GroupNumber = pOutput->GroupNumber;
                pValueHead->head_info.output.MaskBits = pOutput->MaskBits;

                /* ���������� */
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
        /* д״̬������ */
        else if( GroupNumber == 5 )
        {
            for( i = SubNumber; i < ( pRecord->StateNumber ); i ++)
            {
                pValueHead = ( CWM_VHD *)& CWMC_WaveDataBuf[ActualDataNumber];
                pState = pRecord->StateInf;
                pState += i;

                /* ״̬��ͷ����Ϣ */
                pValueHead->head_info.state.GroupNumber = pState->GroupNumber;
                pValueHead->head_info.state.MaskBits = pState->MaskBits;

                /* ״̬������ */
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
        /* д�߼������� */
        else if( GroupNumber == 6 )
        {
            for( i = SubNumber; i < ( pRecord->LogicNumber ); i ++)
            {
                pValueHead = ( CWM_VHD *)& CWMC_WaveDataBuf[ActualDataNumber];
                pLogical = pRecord->LogicalInf;
                pLogical += i;

                /* �߼���ͷ����Ϣ */
                pValueHead->head_info.logical.GroupNumber = pLogical->GroupNumber;
                pValueHead->head_info.logical.MaskBits = pLogical->MaskBits;

                /* �߼������� */
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
                    /* ¼���ļ���Ŀ��� */
                    CWMC_WaveFullFileCheckMain();
                }

                GroupNumber = 0;
                SubNumber = 0;

                CWMC_PROTECT_WAVE();
                /* �ͷſռ� */
                CWMC_WAVE_DELLOCATE( Status,&( pRecord->Created ));
                /* ɾ������ */
                CWMC_REMOVE_WAVE_LIST(&( pRecord->Created ));
                CWMC_UNPROTECT_WAVE();

                /* �ر�¼���豸 */
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

/* ��ȡ¼������������� */
INT CWMC_ReadElecFaultNumber( VOID )
{
    INT iElecNumber;

    iElecNumber = ( INT )CWMC_GET_WAVE_ELEC_NUMBER();

    return( iElecNumber );
}

/* ��ȡ¼��������� */
INT CWMC_ReadWaveIndexNumber( VOID )
{
    INT iIndexNumber;

    iIndexNumber = ( INT )CWMC_GET_WAVE_INDEX();

    return( iIndexNumber );
}

/* �Զ�����¼������������� */
INT CWMC_AdjustElecNumber( VOID )
{
    UNSIGNED Variable;
    INT iElecNumber;

    Variable = CWMC_GET_WAVE_ELEC_NUMBER(); /* ��ȡ¼������������� */
    Variable ++;

    /* ���¼��������������Ƿ���(1~0xFFFF)��Χ�� */
    CWMC_CHECK_DATA_LIMIT( Variable, CWM_FAULT_NUMBER_MIN, CWM_FAULT_NUMBER_MAX );

    CWMC_SET_WAVE_ELEC_NUMBER( Variable ); /* ����¼������������� */
    iElecNumber = ( INT )Variable;

    return( iElecNumber );
}

/* ����¼������������� */
INT CWMC_SetElecNumber( INT iElecNumber )
{
    UNSIGNED Variable;

    Variable = ( UNSIGNED )iElecNumber;

    /* ���¼��������������Ƿ���(1~0xFFFF)��Χ�� */
    CWMC_CHECK_DATA_LIMIT( Variable, CWM_FAULT_NUMBER_MIN, CWM_FAULT_NUMBER_MAX );

    CWMC_SET_WAVE_ELEC_NUMBER( Variable );

    return(( INT )Variable );
}

/* ����¼��״̬�� */
INT CWMC_SetWaveState( STATE * pState, INT iState )
{
    if( pState )
    {
        if( iState == CWM_PROTECT_OPERATE )
        {
            /* ״̬������ */
            CWMC_Protect_Status[pState->state_protect_group] |=
                pState->state_sub_protect;
        }
        else
        {
            /* ״̬������ */
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

/* ��ȡ¼���߼�״̬����Ϣ */
VOID * CWMC_ProtectLogicInformation( UNSIGNED * Actual_Size )
{
    * Actual_Size = CWMC_MAX_WAVE_LOGICAL_GROUPS;

    return( CWMC_Protect_Logic_Status );
}

/* ��ȡ¼����� */
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

    /* ��ȡָ��������ŵ����������� */
    CondMasks = CWMC_DATA_MASKS16() << CWMC_DATA_BITS16() ;
    CondValue = FaultNumber << CWMC_DATA_BITS16();
    status = XJP_Index_Read( CWMC_WaveHandle, Dst, pActual_size, Number, CondMasks, CondValue );
    if( status == XJ_SUCCESS )
    {
        /* ����������еĹ�����ţ�������¼�������� */
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

/* ���ҹ������� */
static INT CWMC_FindFaultNumber( CWAVE * this, VOID * pBuffer, INT * Actual_Number,
    INT Type, UNSIGNED uTimeOut )
{
    INT status;                         /* complete status */

    if( this -> InitFlag == 0 )
    {
        /* ��ʼ����� */
        return( XJ_UNAVAILABLE );
    }

    /* ��¼���豸 */
    OPENDRIVER( status, CWMC_GET_WAVE_HANDLE(), uTimeOut );
    if( status == XJ_SUCCESS )
    {
        /* ��ȡ¼����� */
        STATUSDRIVER( status, CWMC_GET_WAVE_HANDLE(), pBuffer,( UNSIGNED *)Actual_Number, 1, uTimeOut );

        /* �ر�¼���豸 */
        CLOSEDRIVER( CWMC_GET_WAVE_HANDLE());
    }

    /* Return the completion status.  */
    return( status );
}

/* ����¼������״̬ */
INT CWMC_SetFaultStatus( INT iFaultStatus )
{
    CWMC_SetWaveStatus(& CWave, iFaultStatus );

    return( XJ_SUCCESS );
}

/* ��ʼ��¼��RCD����ģ�� */
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
    /* ����ʼ��һ�ξͿ��� */
    if( iInitFlag == 1 )
    {
        return( XJ_SUCCESS );
    }

    /* �ó�ʼ����ɱ�־ */
    iInitFlag = 1;
    pWaveHead = & waveHead;
    /* ��ȡWAVE RCD����ģ�� */
    pWaveRcd = ( WAVE_RCD *)XJP_DS_Get_Table_Memory( TAB_REC_ID,& iNumber );
    if( iNumber > 0 )
    {
        /* �������ϵ�ʱ��ȡ����һ�ε��������״�¼����ʱ��,
        ����ʱ��ȡ����һ�ε����������һ��¼����ʱ��,
        ��һ�ε��������ж��¼����ʱ����ʱ���ǲ�ͬ��,Ҫ��ȡ���� */
        if( FaultIndex != ElecFaultIndex )
        {
            /* ��ȡ¼��ͷ����Ϣ */
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
                /* ����ʱ��ת������ */
                XJP_TM_Dev_To_NS_UTC( & tSec1, & tFrac1, & uWaveTime );
                /* �ж�¼�����;���RCDģ����Ʒ�����صĲ���λ״̬ */
                if( pWaveHead -> Sof & WAVE_TYPE_SOF_TEST )
                {
                    Quality1 = ( UNSIGNED )QUALITY_TEST;
                }
                else
                {
                    Quality1 = 0;
                }
            }
            /* ��ȡ¼��ͷ����Ϣ */
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
                /* ����ʱ��ת������ */
                XJP_TM_Dev_To_NS_UTC( & tSec2, & tFrac2, & uWaveTime );
                /* �ж�¼�����;���RCDģ����Ʒ�����صĲ���λ״̬ */
                if( pWaveHead -> Sof & WAVE_TYPE_SOF_TEST )
                {
                    Quality2 = ( UNSIGNED )QUALITY_TEST;
                }
                else
                {
                    Quality2 = 0;
                }
            }
            /* �ж϶�ȡ¼�������Ƿ�ɹ�,�����Ƿ����RCDģ���еĶ�Ӧ�� */
            if(( iStatus1 == XJ_SUCCESS ) && ( iStatus2 == XJ_SUCCESS ))
            {
                for( k = 0; k < iNumber; k ++ )
                {
                    ( pWaveRcd + k ) -> fltNum = FaultIndex;        /* ������� */
                    ( pWaveRcd + k ) -> fltNum_q |= Quality1;       /* �������Ʒ������ */
                    ( pWaveRcd + k ) -> fltNum_TSec = tSec1;        /* �������ʱ������ */
                    ( pWaveRcd + k ) -> fltNum_TFrac = tFrac1;      /* �������ʱ������� */
                    ( pWaveRcd + k ) -> fltNum_TQflag = tQuality1;  /* �������ʱ��Ʒ������ */
                    ( pWaveRcd + k ) -> GriFltNum = ElecNum;        /* ����������� */
                    ( pWaveRcd + k ) -> GriFltNum_q |= Quality2;    /* �����������Ʒ������ */
                    ( pWaveRcd + k ) -> GriFltNum_TSec = tSec2;     /* �����������ʱ������ */
                    ( pWaveRcd + k ) -> GriFltNum_TFrac = tFrac2;   /* �����������ʱ������� */
                    ( pWaveRcd + k ) -> GriFltNum_TQflag = tQuality2; /* �����������ʱ��Ʒ������ */
                }
            }
            else
            {
                if( iStatus1 == XJ_SUCCESS )
                {
                    for( k = 0; k < iNumber; k ++ )
                    {
                        ( pWaveRcd + k ) -> fltNum = FaultIndex;        /* ������� */
                        ( pWaveRcd + k ) -> fltNum_q |= Quality1;       /* �������Ʒ������ */
                        ( pWaveRcd + k ) -> fltNum_TSec = tSec1;        /* �������ʱ������ */
                        ( pWaveRcd + k ) -> fltNum_TFrac = tFrac1;      /* �������ʱ������� */
                        ( pWaveRcd + k ) -> fltNum_TQflag = tQuality1;  /* �������ʱ��Ʒ������ */
                    }
                }
                if( iStatus2 == XJ_SUCCESS )
                {
                    for( k = 0; k < iNumber; k ++ )
                    {
                        ( pWaveRcd + k ) -> GriFltNum = ElecNum;        /* ����������� */
                        ( pWaveRcd + k ) -> GriFltNum_q |= Quality2;    /* �����������Ʒ������ */
                        ( pWaveRcd + k ) -> GriFltNum_TSec = tSec2;     /* �����������ʱ������ */
                        ( pWaveRcd + k ) -> GriFltNum_TFrac = tFrac2;   /* �����������ʱ������� */
                        ( pWaveRcd + k ) -> GriFltNum_TQflag = tQuality2; /* �����������ʱ��Ʒ������ */
                    }
                }
            }
        }
        else
        {
            /* ��ȡ¼��ͷ����Ϣ */
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
                /* ����ʱ��ת������ */
                XJP_TM_Dev_To_NS_UTC( & tSec1, & tFrac1, & uWaveTime );
                /* �ж�¼�����;���RCDģ����Ʒ�����صĲ���λ״̬ */
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
                    ( pWaveRcd + k ) -> fltNum = FaultIndex;        /* ������� */
                    ( pWaveRcd + k ) -> fltNum_q |= Quality1;       /* �������Ʒ������ */
                    ( pWaveRcd + k ) -> fltNum_TSec = tSec1;        /* �������ʱ������ */
                    ( pWaveRcd + k ) -> fltNum_TFrac = tFrac1;      /* �������ʱ������� */
                    ( pWaveRcd + k ) -> fltNum_TQflag = tQuality1;  /* �������ʱ��Ʒ������ */
                    ( pWaveRcd + k ) -> GriFltNum = ElecNum;        /* ����������� */
                    ( pWaveRcd + k ) -> GriFltNum_q |= Quality1;    /* �����������Ʒ������ */
                    ( pWaveRcd + k ) -> GriFltNum_TSec = tSec1;     /* �����������ʱ������ */
                    ( pWaveRcd + k ) -> GriFltNum_TFrac = tFrac1;   /* �����������ʱ������� */
                    ( pWaveRcd + k ) -> GriFltNum_TQflag = tQuality1; /* �����������ʱ��Ʒ������ */
                }
            }
        }
    }

    return( XJ_SUCCESS );
}

/* ����¼�������Ź���� */
INT CWMC_SetWatchDogId( HWDM hWatchDogId )
{
    CWave.hWatchDogId = hWatchDogId;
    return( XJ_SUCCESS );
}
