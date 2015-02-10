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

/* �궨�� */
#define CWM_MAX_DATA_NUMBER     100         /* ¼��������ݸ��� */
#define CWM_WAVE_START          0           /* д¼��ͷ���� */
#define CWM_WAVE_DATA           1           /* д¼������ */
#define CWM_WAVE_END            2           /* ¼������ */
#define CWM_END_FLAG            0x55aa5a5a  /* ¼��������־ */
#define CWM_MAX_WAVE_POINTER    7           /* ���¼��ָ�� */
#define CWM_WAVE_CTRL_NEW       1           /* ������¼�� */

/* ƫ�������� */
#define CWM_SIZE_OFFSET         0           /* ��С */
#define CWM_FAULT_NUMBER_OFFSET 1           /* �������ƫ���� */
#define CWM_ELEC_FAULT_OFFSET   2           /* �����������ƫ���� */
#define CWM_TYPE_OFFSET         3           /* ����ƫ���� */
#define CWM_ANALOG_NUMBER       4           /* ģ�������� */
#define CWM_TOTAL_POINTER       12          /* �ܵ�¼������ */

#define CWM_FAULT_NUMBER_MIN    1           /* ���������Сֵ */
#define CWM_FAULT_NUMBER_MAX    0xFFFF      /* ����������ֵ */

#define CWM_PROTECT_OPERATE     2           /* ¼������״̬������ */
#define CWM_PROTECT_RETURN      1           /* ¼������״̬������ */

/* ģ������Ϣ�ṹ */
typedef struct CWM_ANALOG_INFO_STRUCT
{
    /* ͨ���� */
    UNSIGNED Number;
    /* ��Ϣ��� */
    UNSIGNED Inf;
    /* �������� */
    UNSIGNED Type;
    /* ����ϵ�� */
    float CalculateCoefficient;
    /* �ֵϵ�� */
    float RateCoefficient;
    /* һ�ζֵ */
    float PrimaryRate;
    /* ���ζֵ */
    float SecondaryRate;
    /* ¼��ϵ�� */
    float WaveCoefficient;
    /* ¼��һ����ת��ϵ�� */
    float WaveConverCoef;
}CWM_ANALOG;

/* ��������Ϣ�ṹ */
typedef struct CWM_INPUT_INFO_STRUCT
{
    /* ��� */
    UNSIGNED GroupNumber;
    /* ���� */
    UNSIGNED MaskBits;
}CWM_INPUT;

/* ��������Ϣ�ṹ */
typedef struct CWM_OUTPUT_INFO_STRUCT
{
    /* ��� */
    UNSIGNED GroupNumber;
    /* ���� */
    UNSIGNED MaskBits;
}CWM_OUTPUT;

/* ״̬����Ϣ�ṹ */
typedef struct CWM_STATE_INFO_STRUCT
{
    /* ��� */
    UNSIGNED GroupNumber;
    /* ���� */
    UNSIGNED MaskBits;
}CWM_STATE;

/* �߼�����Ϣ�ṹ */
typedef struct CWM_LOGICAL_INFO_STRUCT
{
    /* ��� */
    UNSIGNED GroupNumber;
    /* ���� */
    UNSIGNED MaskBits;
}CWM_LOGICAL;

/* ¼����¼���ݽṹ  */
typedef struct CWM_WCB_STRUCT
{
    /* ���ӽڵ� */
    CSF_NODE Created;
    /* ������־ */
    UNSIGNED StartFlag;
    /* ������־ */
    UNSIGNED EndFlag;
    /* ������� */
    UNSIGNED FaultNumber;
    /* ����������� */
    UNSIGNED ElecNumber;
    /* ���ͱ�־ */
    UNSIGNED Type;
    /* ģ�������� */
    UNSIGNED AnalogNumber;
    /* ���������� */
    UNSIGNED InputNumber;
    /* ���������� */
    UNSIGNED OutputNumber;
    /* ״̬������ */
    UNSIGNED StateNumber;
    /* �߼�״̬������ */
    UNSIGNED LogicNumber;
    /* ÿ���ڲ������� */
    UNSIGNED SamplePointer;
    /* ��·Ƶ�� */
    float LineFrequency;
    /* ������(ÿ���������) */
    float SampleRate;
    /* ������¼��� */
    UNSIGNED SampleSpace;
    /* ��¼¼������ */
    UNSIGNED RecordNumber;
    /* ��������� */
    UNSIGNED SampleNumber;
    /* ����ǰ���� */
    UNSIGNED StartPointer;
    /* �ܵĹ��ϵ��� */
    UNSIGNED TotalPointer;
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
    /* �루���룩 */
    UNSIGNED Millionsecond;
    /* ���� */
    UNSIGNED Nanosecond;
    /* ʱ��Ʒ������ */
    UNSIGNED tQuality;
    /* �������� */
    UNSIGNED WaveDataType;
    /* ģ������Ϣ */
    CWM_ANALOG * AnalogInf;
    /* ��������Ϣ */
    CWM_INPUT * InputInf;
    /* ��������Ϣ */
    CWM_OUTPUT * OutputInf;
    /* ״̬����Ϣ */
    CWM_STATE * StateInf;
    /* �߼�����Ϣ */
    CWM_LOGICAL * LogicalInf;
    /* ¼�������ڴ� */
    UNSIGNED * WaveMemory;
}CWM_WCB;

/* ¼��ͷ���ݽṹ. */
typedef struct WAVE_HEAD_STRUCT
{
    /* ��С */
    UNSIGNED Size;
    /* ������� */
    UNSIGNED FaultNumber;
    /* ����������� */
    UNSIGNED ElecNumber;
    /* SOF */
    UNSIGNED Sof;
    /* ģ�������� */
    UNSIGNED AnalogNumber;
    /* ���������� */
    UNSIGNED InputNumber;
    /* ���������� */
    UNSIGNED OutputNumber;
    /* ״̬������ */
    UNSIGNED ProtectNumber;
    /* �߼�״̬������ */
    UNSIGNED LogicNumber;
    /* ÿ���ڲ������� */
    UNSIGNED SamplePointer;
    /* ��·Ƶ�� */
    float LineFrequency;
    /* ������(ÿ���������) */
    float SampleRate;
    /* ������� */
    UNSIGNED Space;
    /* ����ǰ��¼������ */
    UNSIGNED StartPointer;
    /* �ܵ�¼������ */
    UNSIGNED TotalPointer;
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
    /* �루���룩 */
    UNSIGNED Millionsecond;
    /* ���� */
    UNSIGNED Nanosecond;
    /* ʱ��Ʒ������ */
    UNSIGNED tQuality;
    /* �������� */
    UNSIGNED WaveDataType;
    /* ģ������Ϣ��� */
    UNSIGNED ChannelInf[CPC_MAX_CHL_NUMBER];
    /* ��������� */
    UNSIGNED InputGroups[CPC_MAX_WAVE_INPUT_GROUPS];
    /* ������Ͷ��λ */
    UNSIGNED InputActiveBits[CPC_MAX_WAVE_INPUT_GROUPS];
    /* ��������� */
    UNSIGNED OutputGroups[CPC_MAX_OUTPUT_GROUPS];
    /* ������Ͷ��λ */
    UNSIGNED OutputActiveBits[CPC_MAX_OUTPUT_GROUPS];
    /* ״̬����� */
    UNSIGNED StateGroups[CPC_MAX_PROTECT_GROUPS];
    /* ״̬��Ͷ��λ */
    UNSIGNED ProtectActiveBits[CPC_MAX_PROTECT_GROUPS];
    /* �߼������ */
    UNSIGNED LogicalGroups[CPC_MAX_PROTECT_LOG_GROUPS];
    /* �߼���Ͷ��λ */
    UNSIGNED LogicalActiveBits[CPC_MAX_PROTECT_LOG_GROUPS];
    /* ������־ */
    UNSIGNED EndFlag;
}CWM_WHD;

/* ģ������Ϣͷ. */
struct WAVE_ANALOG_STRUCT
{
    /* ��Ϣ��� */
    UNSIGNED Inf;
    /* ͨ������ϵ�� */
    float Coefficient;
    /* ͨ��һ�ζֵ */
    float Primary_rate;
    /* ͨ�����ζֵ */
    float Secondary_rate;
};

/* ��������Ϣͷ. */
struct WAVE_INPUT_STRUCT
{
    /* ��������� */
    UNSIGNED GroupNumber;
    /* ���� */
    UNSIGNED MaskBits;
};

/* ��������Ϣͷ. */
struct WAVE_OUTPUT_STRUCT
{
    /* ��������� */
    UNSIGNED GroupNumber;
    /* ���� */
    UNSIGNED MaskBits;
};

/* ״̬����Ϣͷ. */
struct WAVE_STATE_STRUCT
{
    /* ״̬����� */
    UNSIGNED GroupNumber;
    /* ���� */
    UNSIGNED MaskBits;
};

/* �߼�״̬����Ϣͷ. */
struct WAVE_LOGICAL_STRUCT
{
    /* �߼�״̬����� */
    UNSIGNED GroupNumber;
    /* ���� */
    UNSIGNED MaskBits;
};

/* ¼����ֵͷ���ṹ. */
typedef struct WAVE_VALUE_HEAD_STRUCT
{
    /* ����ͷ����Ϣ */
    union VALUE_HEAD_INFO_UNION
    {
        struct WAVE_ANALOG_STRUCT  analog;
        struct WAVE_INPUT_STRUCT   input;
        struct WAVE_OUTPUT_STRUCT  output;
        struct WAVE_STATE_STRUCT   state;
        struct WAVE_LOGICAL_STRUCT logical;
    }head_info;
}CWM_VHD;

/* ¼����ṹ */
typedef struct WAVE_Class_struct CWAVE;
/* ¼����ṹ */
struct WAVE_Class_struct{
    /*private: */
    /* ��ʼ����־ */
    UNSIGNED InitFlag;
    /* ������ţ��ѱ��棩  */
    UNSIGNED FaultNumber;
    /* ������ţ�δ����  */
    UNSIGNED FaultNumber1;
    /* ����������ţ��ѱ��棩 */
    UNSIGNED ElecNumber;
    /* ����������ţ�δ���� */
    UNSIGNED ElecNumber1;
    /* ������ţ��û�ʹ�ã� */
    UNSIGNED UserFaultNumber;
    /* ����������ţ��û�ʹ�ã� */
    UNSIGNED UserElecNumber;
    /* ¼����¼ͷָ�� */
    CSF_NODE  * WaveList;
    /* ¼����ID�ţ�¼����д�� */
    UNSIGNED WaveId;
    /* ¼������ָ�루¼����д�� */
    NU_DRIVER * WaveDriver;
    /* ¼����̬�ڴ�ľ���� */
    HDMM DmHandle;
    /* ¼��������Ϣ��ID�ţ�������Ϣ��д�� */
    UNSIGNED CtrlId;
    /* ¼��������Ϣ����ָ�루������Ϣ��д�� */
    NU_DRIVER * CtrlDriver;
    /* NVRAM��ʼ������ */
    UNSIGNED * NVStart;
    /* NVRAM���������� */
    UNSIGNED * NVEnd;
    /* NVRAM������ָ�루IN�� */
    UNSIGNED * NVIn;
    /* NVRAM����ʱ����ָ�루IN1�� */
    UNSIGNED * NVIn1;
    /* NVRAM������ָ�루OUT�� */
    UNSIGNED * NVOut;
    /* ����¼����־ */
    INT WaveBusy;
    /* ׼���õ�����¼����ţ�����������ɣ� */
    UNSIGNED ReadySerialNumber;
    /* ׼���õ����µ���������ţ�����������ɣ� */
    UNSIGNED ReadyFaultNumber;
    /* �ļ�·������ */
    CHAR * dirName;
    /* ¼�������Ź���� */
    HWDM hWatchDogId;

    /*public: */
    /* ¼����ʼ������ */
    INT (* Initialize )( CWAVE * this, INT iType, UNSIGNED TimeOut );
    /* ��¼������ */
    INT (* Read )( CWAVE * this, UNSIGNED FaultNumber, VOID * pBuffer,
        UNSIGNED Offset, INT DataType, UNSIGNED Number, UNSIGNED TimeOut );
    /* д¼������ */
    INT (* Write )( CWAVE * this, VOID * pBuffer, INT DataType, UNSIGNED Size,
        UNSIGNED TimeOut );
    /* ����¼�� */
    INT (* Find )( CWAVE * this, INT iType, VOID * pBuffer, INT * Actual_Number,
        UNSIGNED uTimeOut );
    /* ��ȡ����¼����� */
    INT (* GetSerial )( CWAVE * this, UNSIGNED * Serial );
    /* ��ȡ���¹������ */
    INT (* GetFaultNumber )( CWAVE * this, UNSIGNED * FaultNumber );
    /* ��ȡ¼��������Ϣ */
    INT (* Information )( CWAVE * this, UNSIGNED * Number, UNSIGNED * Serial,
        UNSIGNED * StartFaultNumber, UNSIGNED * EndFaultNumber, UNSIGNED uTimeOut );
    /* ���ҹ������� */
    INT (* FindFaultNumber )( CWAVE * this, VOID * pBuffer, INT * Actual_Number,
        INT Type, UNSIGNED TimeOut );
    /* ��ȡ׼���õ�¼����Ϣ */
    INT (* Ready )( CWAVE * this, UNSIGNED TimeOut );
    /* ����¼��״̬ */
    INT (* SetWaveStatus )( CWAVE * this, INT iStatus );
};

/* ¼������Ϣ�ṹ */
typedef struct CWM_ERR_STRUCT
{
    /* ����¼��Ŀ¼ʧ�ܴ��� */
    UNSIGNED makeDirCounter;
    /* �����ڴ��ʧ�ܴ��� */
    UNSIGNED craetePool;
    /* ���ļ�������ʧ�ܴ��� */
    UNSIGNED openFileR;
    /* ���ļ�д����ʧ�ܴ��� */
    UNSIGNED openFileW;
    /* �ر��ļ�������ʧ�ܴ��� */
    UNSIGNED closeFileR;
    /* �ر��ļ�д����ʧ�ܴ��� */
    UNSIGNED closeFileW;
    /* �����дʧ�ܴ��� */
    UNSIGNED fseekFile;
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
    /* д¼��ʧ�ܴ��� */
    UNSIGNED writeWave;
    /* �������ļ�ʧ�ܴ��� */
    UNSIGNED renameFile;
    /* �����ڴ�ʧ�� */
    UNSIGNED waveMalloc;
    /* ���д��� */
    UNSIGNED runTimes;
    /* ����¼������ */
    UNSIGNED createDriver;
    /* ����¼�������� */
    UNSIGNED createSemaphore;
}CWM_ERR;

/*  Initialization functions.  */
extern CWAVE CWave;

/* Error checking for supplemental functions.  */

/* Core processing functions.  */

INT CWMC_WaveMain( VOID );
/* ����¼��(����) */
INT CWMC_WaveStart( INT iType, INT iSpace, INT iStartPoint, INT iTotalPoint,
    INT iElecNumber, WAVE_CHANNEL * WaveChannel, TIME * pTime, STATE * pState, INT iState );
/* ����¼��(���) */
INT CWMC_WaveStartMul( INT waveSegments, INT iType, INT * spacePtr, INT iStartPoint,
    INT * totalPointPtr, INT iElecNumber, WAVE_CHANNEL * WaveChannel, TIME * pTime,
    STATE * pState, INT iState );
/* ��ȡ¼��������� */
INT CWMC_ReadWaveIndexNumber( VOID );
/* ��ȡ¼������������� */
INT CWMC_ReadElecFaultNumber( VOID );
/* �Զ�����¼������������� */
INT CWMC_AdjustElecNumber( VOID );
/* ����¼������������� */
INT CWMC_SetElecNumber( INT iElecNumber );
/* ����¼��״̬�� */
INT CWMC_SetWaveState( STATE * pState, INT iState );
/* ��ȡ¼���߼�״̬����Ϣ */
VOID * CWMC_ProtectLogicInformation( UNSIGNED * Actual_Size );
/* ����¼������״̬ */
INT CWMC_SetFaultStatus( INT iFaultStatus );
/* ����¼�������Ź���� */
INT CWMC_SetWatchDogId( HWDM hWatchDogId );

#endif
