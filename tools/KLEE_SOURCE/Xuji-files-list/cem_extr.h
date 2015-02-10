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

/* �궨�� */
#define CEM_EVENT_NAME          31      /* �¼�������Ƴ��� */
#define CEM_END_FLAG            0x5a    /* �¼�������־ */
#define CEM_EVENT_INDEXL        0       /* �¼���λƫ���� */
#define CEM_EVENT_INDEXH        1       /* �¼���λƫ���� */
#define CEM_BYTE_MASKS          0xff    /* �ֽ��������� */
#define CEM_BYTE_BITS           8       /* �ֽ�����λ�� */
#define CEM_SERIAL_NUMBER_MAX   0xFFFF  /* �¼�������ֵ */
#define CEM_SERIAL_NUMBER_MIN   1       /* �¼������Сֵ */

/* RAM���¼����ݽṹ. */
typedef struct EVENT_RAM_STRUCT
{
    /* ��� */
    UNSIGNED Index;
    /* ���ͱ�־ */
    INT Flag;
    /* ���� */
    UNSIGNED_CHAR Name[CEM_EVENT_NAME];
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
}CEM_RAD;

/* NVRAM���¼����ݽṹ. */
typedef struct EVENT_NVRAM_STRUCT
{
    /* ��ŵ��ֽ� */
    UNSIGNED_CHAR IndexL;
    /* ��Ÿ��ֽ� */
    UNSIGNED_CHAR IndexH;
    /* ���ͱ�־ */
    UNSIGNED_CHAR Flag;
    /* ���� */
    UNSIGNED_CHAR Name[CEM_EVENT_NAME];
    /* �� */
    UNSIGNED_CHAR Year;
    /* �� */
    UNSIGNED_CHAR Month;
    /* �� */
    UNSIGNED_CHAR Day;
    /* ʱ */
    UNSIGNED_CHAR Hour;
    /* �� */
    UNSIGNED_CHAR Minute;
    /* ������ֽ� */
    UNSIGNED_CHAR MillionsecondL;
    /* ������ֽ� */
    UNSIGNED_CHAR MillionsecondH;
    /* ������־ */
    UNSIGNED_CHAR EndFlag;
}CEM_NVD;

/* �¼���ṹ */
typedef struct EVENT_Class_struct CEVENT;
/* �¼���ṹ */
struct EVENT_Class_struct{
    /*private: */
    /* ��ʼ����־ */
    UNSIGNED InitFlag;
    /* �����¼����� */
    UNSIGNED MaxNumber;
    /* �ܵ��¼����� */
    UNSIGNED Number;
    /* ���µ��¼���ţ��ѱ��棩  */
    UNSIGNED Serial;
    /* ���µ��¼���ţ�δ���棩  */
    UNSIGNED Serial1;
    /* �¼���ID�ţ��¼���д�� */
    UNSIGNED EventId;
    /* �¼�����ָ�루�¼���д�� */
    NU_DRIVER * EventDriver;
    /* �¼�������Ϣ����ָ�루������Ϣ��д�� */
    NU_DRIVER * CtrlDriver;
    /* NVRAM��ʼ������ */
    UNSIGNED_CHAR * NVStart;
    /* NVRAM���������� */
    UNSIGNED_CHAR * NVEnd;
    /* NVRAM������ָ�루IN�� */
    UNSIGNED_CHAR * NVIn;
    /* NVRAM������ָ�루OUT�� */
    UNSIGNED_CHAR * NVOut;
    /* �ļ�·������ */
    CHAR * dirName;
    /* �¼������������ */
    HITM EventHandle;
    /* �¼������Ź���� */
    HWDM hWatchDogId;

    /*public: */
    /* �¼���ʼ������ */
    INT (* Initialize )( CEVENT * this, INT iType, UNSIGNED TimeOut );
    /* ���¼����� */
    INT (* Read )( CEVENT * this, UNSIGNED EventIndex, VOID * Buffer, UNSIGNED TimeOut );
    /* д�¼����� */
    INT (* Write )( CEVENT * this, VOID * pBuffer, UNSIGNED TimeOut );
    /* ����¼�������Ϣ */
    INT (* Clear )( CEVENT * this, UNSIGNED TimeOut );
    /* ��ȡ�¼�������Ϣ */
    INT (* Information )( CEVENT * this, UNSIGNED * Type, UNSIGNED * Number, UNSIGNED * StartSerial,
        UNSIGNED * EndSerial, UNSIGNED uTimeOut );
    /* �����¼������� */
    INT (* Find )( CEVENT * this, VOID * pBuffer, INT * Actual_Number, UNSIGNED uTimeOut );
};

/* �¼�����Ϣ�ṹ */
typedef struct CEM_ERR_STRUCT
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
    UNSIGNED writeEvent;
    /* �������ļ�ʧ�ܴ��� */
    UNSIGNED renameFile;
    /* ���ȴ��� */
    UNSIGNED runTimes;
    /* ��������ʧ�ܴ��� */
    UNSIGNED createDriver;
    /* ����������ʧ�ܴ��� */
    UNSIGNED createSemaphore;
}CEM_ERR;

/*  Initialization functions.  */

/* Error checking for supplemental functions.  */

/* Core processing functions.  */

INT CEMC_EventMain( VOID );
INT CEMC_FillEvent( const VOID * pName, INT iEventType );
INT CEMC_FillEventFull( const VOID * pName, TIME * pTime, INT iEventType );
/* �����¼������Ź���� */
INT CEMC_SetWatchDogId( HWDM hWatchDogId );

#endif
