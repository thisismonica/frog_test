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
/*      cdr_extr.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CDD - Common Device Driver                                       */
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
/*      None                                                             */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhideYang.      02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"
#include "plat/inc/esp_cfg.h"
#include "plat/inc/pro_defs.h"
#include "plat/inc/tab_defs.h"
#include "plat/inc/cin_extr.h"

/* Check to see if the file has been included already.  */

#ifndef CSMV_EXTR_H
#define CSMV_EXTR_H

/* ����ֵ��¼���ݽṹ */
typedef struct SMVC_SAMPLE_RECORD_STRUCT
{
    /* ���²���λ��ƫ�� */
    INT sample_offset;
    /* �������� */
    UNSIGNED sample_counter;
    /* ��������������ڴ�ָ�� */
    UNSIGNED * pSamplePulseMemory;
    //
    /* A/Dģ����ͨ���� */
    INT AdAnalogCounter;
    /* A/Dģ����ͨ��ֵ */
    float * pAdAnalogValue;
    /* ģ���������ڴ�ָ�� */
    float * pAnalogMemory;
    /* ģ����Ʒ���������� */
    INT AnalogQualityGroups;
    /* ģ����Ʒ������ֵ */
    UNSIGNED * pAnalogQualityValue;
    /* ģ����Ʒ�������ڴ�ָ�� */
    UNSIGNED * pAanlogQualityMemory;
    //
    /* ���������� */
    INT InputGroupsNumber;
    /* �����������ڴ�ָ�� */
    UNSIGNED * pInputMemory;
    /* ������Ʒ������ֵ */
    UNSIGNED * pInputQualityValue;
    /* ������Ʒ�������ڴ�ָ�� */
    UNSIGNED * pInputQualityMemory;
    //
    /* ���������� */
    INT OutputGroupsNumber;
    /* ������״ֵ̬ */
    UNSIGNED * pOutputValue;
    /* �����������ڴ�ָ�� */
    UNSIGNED * pOutputMemory;
    /* ������Ʒ������ֵ */
    UNSIGNED * pOutputQualityValue;
    /* ������Ʒ�������ڴ�ָ�� */
    UNSIGNED * pOutputQualityMemory;
    //
    /* ����״̬������ */
    INT ProStatusGroupsNumber;
    /* ����״̬��ֵ */
    UNSIGNED * pProStatusValue;
    /* ����״̬�������ڴ�ָ�� */
    UNSIGNED * pProStatusMemory;
    /* ����״̬��Ʒ������ֵ */
    UNSIGNED * pProStatusQualityValue;
    /* ����״̬��Ʒ������ָ�� */
    UNSIGNED * pProStatusQualityMemory;
    //
    /* �߼�״̬������ */
    INT LogicGroupsNumber;
    /* �߼�״̬��ֵ */
    UNSIGNED * pLogicValue;
    /* �߼�״̬���ڴ�ָ�� */
    UNSIGNED * pLogicMemory;
    /* �߼�״̬��Ʒ������ֵ */
    UNSIGNED * pLogicQualityValue;
    /* �߼�״̬��Ʒ�������ڴ�ָ�� */
    UNSIGNED * pLogicQualityMemory;
}SMVC_SAMPLE_RECORD, * PSMVC_SAMPLE_RECORD;

/* SV������Ϣ�ṹ */
typedef struct SMVC_ERR_STRUCT
{
    /* ������Ų��������� */
    UNSIGNED sampleCounter;
    /* ��ʧһ����� */
    UNSIGNED OneSampleCounter;
}SMVC_ERR;

/* ��������ļ����� */
typedef struct CSMV_MSG_Count_struct
{
    /*private: */
    /* ���ȴ�������� */
    UNSIGNED iLenErr;
    /* ����8C���������;0-�������,1-ͨ������ */
    UNSIGNED iRecSmvCount[3];
    /* ����88���������;0-�������,1-�������� */
    UNSIGNED iGooseRecCount[2];
    /* ����83��������� */
    UNSIGNED iRecSynCount;
    /* ����83����ȴ�������� */
    UNSIGNED iRecSynLenErrCount[2];
    /* ����83�������������� */
    UNSIGNED iRecSynYearErrCount[2];
    /* ����0x8D��������� */
    UNSIGNED iRecGooseTimeCount;
    /* ����0x8D����ȴ�������� */
    UNSIGNED iRecGooseTimeLenErrCount;
    /* ���´��󳤶� */
    UNSIGNED iRecGooseTimeErrLen;
    /* ����λ��������� */
    UNSIGNED iRecGooseTimeBitErrCount;
    /* ���´���Ŀ���λ */
    UNSIGNED iRecGooseTimeErrBits;
    /* ���Ͷ�ʱ����ɹ����������� */
    UNSIGNED iSendSynSucCount;
    /* ���Ͷ�ʱ����ʧ�ܸ��������� */
    UNSIGNED iSendSynErrCount;
    /* ����83����ʱ�� */
    UNSIGNED iSendSynErrTime;
    /* ����GOOSE�ɹ����������� */
    UNSIGNED iSendGooseSucCount;
    /* ����GOOSEʧ�ܸ��������� */
    UNSIGNED iSendGooseErrCount;
    /* ���ʹ���ǰʱ�� */
    UNSIGNED uSendGooseErrTime;
    /* �������� */
    UNSIGNED iOtherCount;
    /* �������ĺ���ִ�����ʱ�� */
    UNSIGNED uMsgParseMaxTime;
    /* �������ĺ���ִ��ʵʱʱ�� */
    UNSIGNED uMsgParseRealTime;
}CSMV_MSG_COUNT;

/*  Initialization functions.  */

/* Error checking for supplemental functions.  */

/* Core processing functions.  */

extern float SMVC_AnalogValueBuf[]; /* ԭʼģ��������ֵ������ */
extern UNSIGNED SMVC_AnalogQualityBuf[]; /* ԭʼģ����Ʒ�����ػ����� */
extern UNSIGNED SMVC_AnalogCheckQualityBuf[]; /* ԭʼģ����Ʒ�����ؼ���λ������ */
extern UNSIGNED CWMC_Protect_Status[]; /* ԭʼ����״̬�� */
extern UNSIGNED CWMC_Protect_Logic_Status[]; /* ԭʼ�����߼�״̬�� */

extern float DC_Coefficient;
extern ESP_CFG PLAT_cfg;
extern SMVC_SAMPLE_RECORD SMVC_SampleRecord;
extern INT ChannelTabNumber;
extern PRO_CHANNEL * pChannelTab;
#define CSMV_GET_CHANNEL_POINT      (PLAT_cfg.smvc.ChannelPoint)        /* ÿͨ���Ĳ���ֵ���� */
#define CSMV_GET_ANALOG_MEMORY      (SMVC_SampleRecord.pAnalogMemory)   /* ģ�����ڴ��׵�ַ */
#define CSMV_GET_SAMPLE_OFFSET      (SMVC_SampleRecord.sample_offset)   /* ���²���ֵλ��ƫ�� */
#define CSMV_GET_PERIOD_POINT       (PLAT_cfg.smvc.PeriodPoint)         /* ÿ���ڲ������� */
#define CSMV_GET_SAMPLE_RATE        (PLAT_cfg.smvc.SampleRate)          /* ������(ÿ���������) */
#define CSMV_GET_AD_NUMBER          (PLAT_cfg.smvc.AdNumber)            /* A/DƬ�� */
#define CSMV_GET_SAMPLE_COUNTER     (PLAT_cfg.smvc.SampleCounter)       /* ������ŵ����ֵ */
#define CSMV_GET_PHY_ANALOG_NUMBER  (PLAT_cfg.smvc.PhysicalAnalogNumber) /* �����ģ����ͨ���� */
#define CSMV_GET_DSAMPLE_SV_CHL_NUM (PLAT_cfg.smvc.DSampleSVChlNum)      /* ˫����SVͨ������ */
#define CSMV_GET_DSAMPLE_FLAG       (PLAT_cfg.smvc.DSampleFlag)          /* ˫������ʶ */
#define CSMV_GET_VIRT_ANALOG_NUMBER (PLAT_cfg.smvc.VirtualAnalogNumber) /* ��ͨ������ */
#define CSMV_GET_ANALOG_NUMBER       (CSMV_GET_PHY_ANALOG_NUMBER+CSMV_GET_VIRT_ANALOG_NUMBER)
/* ģ����ͨ���ܸ�������ͨ��+����ͨ����*/
#define CSMV_ANALOG_MAX_GROUPS       ((CPC_MAX_PHYSICAL_CHL_NUMBER + 31)/32) /* ���ģ����ͨ������ */
#define CSMV_GET_PHY_ANALOG_GROUPS   ((CSMV_GET_PHY_ANALOG_NUMBER + 31)/32)  /* �����ģ����ͨ������ */
#define CSMV_GET_ANALOG_GROUPS       (PLAT_cfg.smvc.AnalogQualityGroups)     /* ģ����ͨ������ */
#define CSMV_GET_DC_COEFFICIENT      DC_Coefficient                     /* ֱ������ϵ�� */
#define CSMV_GET_CHL_TABLE_MEMORY    GET_CHANNEL_TAB_MEMORY             /* ģ����ͨ�����ݱ��ڴ���ַ */
#define CSMV_GET_CHL_TABLE_NUMBER    GET_CHANNEL_TAB_NUMBER             /* ģ����ͨ�����ݱ�����ݸ��� */
#define CSMV_GET_ORIGINL_ANA_QUA_MEM SMVC_AnalogQualityBuf              /* ��ȡԭʼģ����Ʒ�����ػ����� */
#define CSMV_GET_ORIGINL_ANA_CHECKQUA_MEM SMVC_AnalogCheckQualityBuf    /* ��ȡԭʼģ����Ʒ�����ؼ���λ������ */
#define CSMV_GET_ANALOG_QUA_MEMORY   (SMVC_SampleRecord.pAanlogQualityMemory) /* ģ����Ʒ�������ڴ���ַ */
#define CSMV_GET_SAMPLE_PULSE_MEMORY (SMVC_SampleRecord.pSamplePulseMemory) /* ��������������ڴ���ַ */
#define CSMV_GET_INPUT_GROUPS        CINC_INPUT_GROUP_NUMBER            /* ����˿����� */
#define CSMV_GET_WAVE_INPUT_GROUPS   CINC_WAVE_INPUT_GROUP_NUMBER       /* ¼������������ */
#define CSMV_GET_INPUT_MEMORY        (SMVC_SampleRecord.pInputMemory)   /* �������ڴ��׵�ַ */
#define CSMV_GET_INPUT_QUA_MEMORY    (SMVC_SampleRecord.pInputQualityMemory)  /* ������Ʒ�������ڴ��׵�ַ */
#define CSMV_GET_INPUT_PORT          CINC_INPUT_PORT_MEMORY             /* ����˿��׵�ַ */
#define CSMV_GET_OUTPUT_GROUPS       (PLAT_cfg.output.OutputGroups)     /* �����˿����� */
#define CSMV_GET_OUTPUT_MEMORY       (SMVC_SampleRecord.pOutputMemory)  /* �������ڴ��׵�ַ */
#define CSMV_GET_OUTPUT_QUA_MEMORY   (SMVC_SampleRecord.pOutputQualityMemory)  /* ������Ʒ�������ڴ��׵�ַ */
#define CSMV_GET_POWER_CHECK_NUMBER  (PLAT_cfg.smvc.PowerCheckNumber)      /* ��ȡ�Լ��Դͨ������ */
#define CSMV_GET_POWER_CHL_INFO_MEMORY (PLAT_cfg.smvc.PowerChannel)        /* ��ȡ�Լ��Դͨ����Ϣ�ڴ���ַ */
#define CSMV_GET_ORIGINL_PRO_STS_MEM CWMC_Protect_Status                   /* ��ȡԭʼ����״̬�������� */
#define CSMV_GET_PRO_STS_MEMORY      (SMVC_SampleRecord.pProStatusMemory)  /* ����״̬���ڴ��׵�ַ */
#define CSMV_GET_PRO_STS_QUA_MEMORY  (SMVC_SampleRecord.pProStatusQualityMemory)  /* ����״̬��Ʒ�������ڴ��׵�ַ */
#define CSMV_GET_PRO_STS_GROUPS      (SMVC_SampleRecord.ProStatusGroupsNumber)  /* ����״̬������ */
#define CSMV_GET_ORIGINL_PRO_LOG_MEM CWMC_Protect_Logic_Status          /* ��ȡԭʼ�����߼�״̬�������� */
#define CSMV_GET_PRO_LOG_MEMORY      (SMVC_SampleRecord.pLogicMemory)   /* �����߼�״̬���ڴ��׵�ַ */
#define CSMV_GET_PRO_LOG_QUA_MEMORY  (SMVC_SampleRecord.pLogicQualityMemory)   /* �����߼�״̬��Ʒ�������ڴ��׵�ַ */
#define CSMV_GET_PRO_LOG_GROUPS      (SMVC_SampleRecord.ProStatusGroupsNumber) /* �����߼�״̬������ */

#define CSMV_IF_LOSE_SV_QUALITY      (PLAT_cfg.smvc.LoseSV_Quality)     /* �������ֵ����Ʒ�����ش����־ */
#define CSMV_IF_LOSE_ONE_SV_INSERT   (PLAT_cfg.smvc.LoseOneSV_Insert)   /* ��һ�����ֵ���ݲ�ֵ�����־ */

/* ���ò���λ��ƫ�� */
#define CSMV_SET_SAMPLE_OFFSET(offset_value)                            \
{   \
    SMVC_SampleRecord.sample_offset = offset_value ;                    \
}

/* ��ʼ��ģ�������� */
INT CSMV_InitializeChannelPara( VOID );
/* ����ģ����ͨ����ֱ������ */
float CSMV_CalculateDC( INT ChannelNumber, float Coefficient, INT SampleOffset );
/* ����ģ����ͨ����г������ */
float CSMV_CalculateFourier( INT ChannelNumber, float * ParaPtr, INT SampleOffset );
/* ����ģ����ͨ���ķ�ֵ */
float CSMV_CalculatePeakValue( INT ChannelNumber, INT SampleOffset );
/* ��ȡ����������Ϣ */
VOID * CSMV_SampleInformation( UNSIGNED * Actual_Number, UNSIGNED * Actual_Size,
    UNSIGNED * Actual_Point );
/* ��ȡ��ǰ����ָ�� */
VOID * CSMV_SamplePointer( VOID );
/* ����Ӳ���������� */
VOID SMVC_SampleUpdateHard( float * sample_data, UNSIGNED pulse_counter );
/* ������������������ */
VOID SMVC_SampleUpdateSoft( float * sample_data, UNSIGNED pulse_counter );
/* ���Ľ������� */
VOID CSMV_MessParse( UNSIGNED_CHAR * data, UINT length );
/* ע��A/D�ж��û������� */
VOID SMVC_Register_User_AD_Interrupt( VOID (* User_AD_Interrupt )( VOID ));
/* ע��A/D�жϲ�������ת������ */
VOID SMVC_Register_Sample_Conversion( INT (* User_Sample_Conversion )( float * , INT ));
/* ��ȡ�������������� */
INT SMVC_GetSampleCount( UNSIGNED * pSampleCount );
/* ��ȡ��������������ʼָ�� */
INT CSMV_GetDataStartPointer( INT  iType, VOID ** valueMem, VOID ** qualityMemr, INT * number );
/* ��ȡ����λ��ƫ�� */
UNSIGNED CSMV_GetSampleOffset( VOID );
/* ����GOOSE���������� */
VOID CSMV_Msg_SendGooseOut( VOID );
/* ����װ��״̬���� */
INT CSMV_MSG_SetDevStatus( UNSIGNED uDevStatus );
/* �û���NPI��������� */
INT CSMV_MSG_GetDataFromNpi( UNSIGNED_CHAR * buffer, INT * Length );
/* �û���NPI�෢�����ݺ��� */
INT CSMV_MSG_UserSendDataTONpi( UNSIGNED_CHAR * buffer, INT Length );
/* �����û����� */
INT  CSMV_MSG_UserSendData( VOID );
/* NPU��NPI���Ͷ�ʱ���� */
VOID CSMV_Msg_SynTimeCmd( VOID );
/* ���ò���׼����ʶ */
INT CSMV_SetSampleInitFlag( INT iFlag );
/* ��ȡ����׼����ʶ */
INT CSMV_GetSampleInitFlag( VOID );
/* ע����NPI�������ݽӿں��� */
VOID CSMV_RegTONpiSendData( INT (* RegSendDataFun )( UNSIGNED_CHAR * data, INT length ));
/* ����ģ����ͨ��Ʒ������¼��״̬������ʵ��ż����� */
INT CSMV_MSG_SetChlQStateInfo( INT iStartGroup, INT iGroupCount );
/* ��ȡģ����ͨ��Ʒ������¼��״̬������ʼ��ż����� */
INT CSMV_MSG_GetChlQStateInfo( INT * iStartGroup, INT * iGroupCount );
/* ��ȡ������������������� */
float * CSMV_MSG_GetSVDataInfo( UNSIGNED * SampleSerial );
#endif
