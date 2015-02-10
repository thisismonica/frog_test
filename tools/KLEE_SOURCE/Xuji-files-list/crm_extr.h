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

/* �궨�� */
#define CRM_MAX_REPORT_SIZE     1500       /* ����������ݸ��� */
#define CRM_SIZE_OFFSET         0         /* ��С */
#define CRM_INDEX_OFFSET        1         /* ������ƫ���� */
#define CRM_FAULT_NUMBER_OFFSET 2         /* �������ƫ���� */
#define CRM_TYPE_OFFSET         3         /* ��������ƫ���� */
#define CRM_END_FLAG           0x55aa5a5a /* ���������־ */
#define CRM_FAULT_NUMBER_MAX    0xFFFF    /* ����������ֵ */
#define CRM_FAULT_NUMBER_MIN    1         /* ���������Сֵ */
#define CRM_DIR_NAME            256       /* ���ϱ���·������ */
#define CRM_VERSION             1         /* ���ϱ���汾�� */
#define CRM_IDENT_REPORT        ".report" /* �����ʶ���� */

/* ����ͷ���ݽṹ. */
typedef struct REPORT_HEAD_STRUCT
{
    /* ��С */
    UNSIGNED Size;
    /* CPU���� */
    INT CpuNumber;
    /* ��� */
    UNSIGNED Index;
    /* ������� */
    UNSIGNED FaultNumber;
    /* �������� */
    INT Type;
    /* �� */
    UNSIGNED Year;
    /* �� */
    UNSIGNED Month;
    /* �� */
    UNSIGNED Day;
    /* ʱ */
    UNSIGNED Hour;
    /* �� */
    UNSIGNED Minute;
    /* ���� */
    UNSIGNED Millionsecond;
    /* ���� */
    UNSIGNED Nanosecond;
    /* Ʒ�� */
    UNSIGNED Quality;
}CRM_RHD;

/* �����������ݽṹ. */
typedef struct REPORT_FIXED_STRUCT
{
    /* ����ͷ */
    CRM_RHD Head;
    /* ����ʵ���ֽ��� */
    UNSIGNED DataSize;
    /* ���� */
    UNSIGNED Data[CRM_MAX_REPORT_SIZE];
    /* ������־ */
    UNSIGNED EndFlag;
}CRM_RFD;

/*��̬�ڴ��������ӿ����ݽṹ�Ͳ���*/
typedef void * (* RPT_MemAlloc )( size_t );
typedef void (* RPT_MemFree )( void *);
struct RPTMemMService
{
    /* �����ڴ� */
    RPT_MemAlloc MemAlloc;
    /* �ͷ��ڴ� */
    RPT_MemFree MemFree;
};

/* ������ṹ */
typedef struct REPORT_Class_struct CREPORT;
/* ������ṹ */
struct REPORT_Class_struct{
    /*private: */
    /* ���ӽڵ� */
    CSF_NODE Link;
    /* ��ʼ����־ */
    UNSIGNED InitFlag;
    /* CPU���� */
    INT CpuNumber;
    /* ���ı������ */
    UNSIGNED MaxNumber;
    /* ���µı�����ţ��ѱ��棩 */
    UNSIGNED Serial;
    /* ���µı�����ţ�δ���棩 */
    UNSIGNED Serial1;
    /* ��������¹�����ţ��ѱ��棩 */
    UNSIGNED FaultNumber;
    /* ��������¹�����ţ�δ���棩 */
    UNSIGNED FaultNumber1;
    /* ���µĶ���ʱ�䣨�ѱ��棩 */
    TIME OpTime;
    /* ���µĶ���ʱ�䣨δ���棩 */
    TIME OpTime1;
    /* ������ţ��û�ʹ�ã� */
    UNSIGNED UserFaultNumber;
    /* �����ID�ţ������д�� */
    UNSIGNED ReportId;
    /* ��������ָ�루�����д�� */
    NU_DRIVER * ReportDriver;
    /* ���������Ϣ����ָ�루������Ϣ��д�� */
    NU_DRIVER * CtrlDriver;
    /* NVRAM��ʼ������ */
    UNSIGNED * NVStart;
    /* NVRAM���������� */
    UNSIGNED * NVEnd;
    /* NVRAM������ָ�루IN�� */
    UNSIGNED * NVIn;
    /* NVRAM������ָ�루OUT�� */
    UNSIGNED * NVOut;
    /* ���¶��������־ */
    INT ReportBusy;
    /* ׼���õ����±�����ţ�����������ɣ� */
    UNSIGNED ReadySerialNumber;
    /* ׼���õ����¹�����ţ�����������ɣ� */
    UNSIGNED ReadyFaultNumber;
    /* ׼���õ����µĶ���ʱ�䣨����������ɣ� */
    TIME ReadyOpTime;
    /* ׼���õ�NVRAM������ָ�루IN�� */
    UNSIGNED * ReadyNVIn;
    /* �ļ�·������ */
    CHAR dirName[ CRM_DIR_NAME ];
    /* ��������������� */
    HITM ReportHandle;
    /* ��󱨸��ļ��������� */
    UNSIGNED MaxReportFileIndex;
    /* �����ļ�������������ַ */
    UNSIGNED * ReportFileIndexPtr;
    /* ����������ͳ�� */
    UNSIGNED ClearNumber;
    /* �����¼��� */
    UNSIGNED RecordHandle;
    /* RAM����������ѭ�������������� */
    HDCM RamReportHandle;
    /* �������ݱ�־ */
    INT ReportDataFlag;
    /* �������ݴ����ڴ�ռ� */
    CRM_RFD * ReportDataPtr;
    /* ���̿��������� */
    UNSIGNED GroupNumber;
    /* ���̿��Ƹ����� */
    UNSIGNED SubNumber;
    /* ��ǰ������� */
    UNSIGNED CurFaultNumber;

    /*public: */
    /* �����ʼ������ */
    INT (* Initialize )( CREPORT * this, INT iType, UNSIGNED TimeOut );
    /* ���������� */
    INT (* Read )( CREPORT * this, UNSIGNED ElecNumber, INT startIndex,
        INT IndexNumber, UNSIGNED Offset, UNSIGNED Size, VOID * pBuffer,
        UNSIGNED uTimeOut );
    /* д�������� */
    INT (* Write )( CREPORT * this, VOID * pBuffer, UNSIGNED uSize, UNSIGNED TimeOut );
    /* ��ȡ����������� */
    INT (* GetReadySerial )( CREPORT * this, UNSIGNED * Serial );
    /* ��ȡ����������� */
    INT (* GetReadyFaultNumber )( CREPORT * this, UNSIGNED * FaultNumber );
    /* ��ȡ����������Ϣ */
    INT (* GetReadyInformation )( CREPORT * this, UNSIGNED * Serial,
        UNSIGNED * FaultNumber, TIME * Time );
    /* ��ȡ���������Ϣ */
    INT (* Information )( CREPORT * this, UNSIGNED * ElecCounter, UNSIGNED * EndFaultCounter,
        UNSIGNED * StartFaultIndex, UNSIGNED * EndFaultIndex, UNSIGNED uTimeOut );
    /* ����ָ��������ŵ��������� */
    INT (* FindIndexNumber )( CREPORT * this, INT * Actual_Number, UNSIGNED FaultNumber,
        INT Type, UNSIGNED TimeOut );
    /* ���ҹ������� */
    INT (* FindFaultNumber )( CREPORT * this, VOID * pBuffer, INT * Actual_Number,
        INT Type, UNSIGNED TimeOut );
    /* ��ȡ׼���õı�����Ϣ */
    INT (* Ready )( CREPORT * this, UNSIGNED TimeOut );
    /* ���ñ���״̬ */
    INT (* SetReportStatus )( CREPORT * this, INT iStatus );
};

/* ��������Ϣ�ṹ */
typedef struct CRM_ERR_STRUCT
{
    /* ����¼��Ŀ¼ʧ�ܴ��� */
    UNSIGNED makeDirCounter;
    /* ���ļ�������ʧ�ܴ��� */
    UNSIGNED openFileR;
    /* ���ļ�д����ʧ�ܴ��� */
    UNSIGNED openFileW;
    /* �ر��ļ�������ʧ�ܴ��� */
    UNSIGNED closeFileR;
    /* �ر��ļ�д����ʧ�ܴ��� */
    UNSIGNED closeFileW;
    /* ���ļ�ʧ�ܴ��� */
    UNSIGNED readFile;
    /* д�ļ�ʧ�ܴ��� */
    UNSIGNED writeFile;
    /* ɾ��д�ļ�ʧ�ܴ��� */
    UNSIGNED deleteFileW;
    /* ɾ���б��ļ�ʧ�ܴ��� */
    UNSIGNED deleteFileL;
    /* ɾ��Ŀ¼�ļ�ʧ�ܴ��� */
    UNSIGNED deleteDirFile;
    /* ɾ�������Ч�ļ�ʧ�ܴ��� */
    UNSIGNED deleteCheckFile;
    /* ɾ�������ļ�ʧ�ܴ��� */
    UNSIGNED deleteFullFile;
    /* ��ȡ��Ϣʧ�ܴ��� */
    UNSIGNED getInfoFile;
    /* д����ʧ�ܴ��� */
    UNSIGNED writeReport;
    /* �������ļ�ʧ�ܴ��� */
    UNSIGNED renameFile;
    /* ���ȴ��� */
    UNSIGNED runTimes;
    /* ������������ */
    UNSIGNED createDriver;
    /* �������滥���� */
    UNSIGNED createSemaphore;
    /* ������������¼�ʧ�� */
    UNSIGNED createClrEvent;
}CRM_ERR;

/*  Initialization functions.  */
extern CREPORT CReport;

/* Error checking for supplemental functions.  */

/* Core processing functions.  */

INT CRMC_ReportMain( VOID );
/* �����ϱ��� */
INT CRMC_FillReport( INT CpuNumber, UNSIGNED FaultNumber, INT Type,
    UNSIGNED Size, const VOID * Src, TIME * pTime );
/* ��ȡ���������� */
INT CRMC_ReadFaultNumber( INT CpuNumber );
/* �Զ��������������� */
INT CRMC_AdjustFaultNumber( INT CpuNumber );
/* ���ñ��������� */
INT CRMC_SetFaultNumber( INT CpuNumber, INT iFaultNumber );
/* ���ñ������״̬ */
INT CRMC_SetFaultStatus( INT CpuNumber, INT iFaultStatus );
/* ������ϱ������� */
INT CRMC_ClearReport( INT CpuNumber );
/* ����������������� */
CREPORT * CRMC_Create( INT CpuNumber, INT MaxNumber, CHAR * dirName );
/* ��ȡָ��CPU���������� */
CREPORT * CRMC_GetReportHandle( INT CpuNumber );
/* ���ñ��������Ź���� */
INT CRMC_SetWatchDogId( HWDM hWatchDogId );
/* Ӧ�ö�ȡ�����������ݺ��� */
INT CRMC_UserReadReportData( INT CpuNumber , UNSIGNED ElecNumber, INT startIndex,
    INT IndexNumber, UNSIGNED Offset, UNSIGNED Size, VOID * pBuffer,
    UNSIGNED uTimeOut );

#endif
