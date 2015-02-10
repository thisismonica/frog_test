
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
/*      ls_defs.h                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      LS - Lonworks Communication to Supervisor                        */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains data structure definitions and constants for  */
/*      the Lonworks Communication to Supervisor.                        */
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
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

#include "plat/inc/xj_cosc.h"
#include "plat/es_plat.h"
#include "plat/inc/tab_defs.h"
#include "plat/inc/esp_cfg.h"
#include "plat/inc/csmv_extr.h"
#include "plat/net_inc/commu.h"
#include "plat/net_inc/ver_defs.h"
#include "plat_specific/plat_specific.h"

/* Check to see if the file has been included already.  */

#ifndef LS_DEFS_H
#define LS_DEFS_H

/* Define macro to eliminate warnings for unused parameters */
#define LS_UNUSED_PARAM(parameter)      (VOID)parameter

/* �ⲿ���� */
extern ESP_CFG PLAT_cfg;
extern NU_PROTECT * pReport_Protect;
extern NU_PROTECT * pWave_Protect;
extern NU_EVENT_GROUP Event_2_CB;
extern UNSIGNED CCMC_platCRC;
extern UNSIGNED CCMC_devCRC;
extern UNSIGNED CCMC_hmiCRC;
extern UNSIGNED CCMC_gwsCRC;
#define LS_CPU_NUMBER                   (PLAT_cfg.device.CpuNumber)       /* װ��CPU���� */
#define LS_CPU_TYPE                     (PLAT_cfg.device.CpuType)         /* CPU���� */
#define LS_SET_ZONE_COUNT               (PLAT_cfg.device.SetZoneCount)    /* װ�ö�ֵ������ */
#define LS_GET_DEV_CRC                  CCMC_devCRC                       /* װ��CRC�� */
#define LS_GET_PLAT_CRC                 CCMC_platCRC                      /* ƽ̨CRC�� */
#define LS_GET_HMI_CRC                  CCMC_hmiCRC                       /* �˻��ӿ�CRC�� */
#define LS_GET_GWS_CRC                  CCMC_gwsCRC                       /* ����CRC�� */
#define LS_USER_RECORD_EVENT_GROUP      (PLAT_cfg.history.UserEventGroups) /* �û��¼����� */
#define LS_GET_OUTPUT_SIGNAL_LED_MEMORY (PLAT_cfg.output.HardOutputSignalLed) /* Ӳ���źŵƶ���λ�ڴ��ַ */
#define LS_GET_OUTPUT_TRIP_LED_MEMORY   (PLAT_cfg.output.HardOutputTripLed)   /* Ӳ����բ�ƶ���λ�ڴ��ַ */
#define LS_GET_OUTPUT_GROUPS            (PLAT_cfg.output.OutputGroups)        /* ���������� */
#define LS_GET_OUTPUT_RESET_MEMORY      (PLAT_cfg.output.HardOutputReset)     /* ����̵�������λ�ڴ��ַ */

#define LS_ONE_REPORT_SIZE              sizeof( CRM_RFD )                 /* һ��������������ռ� */
#define LS_DC_COEFFICIENT               CSMV_GET_DC_COEFFICIENT           /* ֱ������ϵ�� */
#define LS_MAX_CHL_NUMBER               CPC_MAX_CHL_NUMBER                /* ���ģ����ͨ�����ݸ��� */
#define LS_COMMU_VER                    VER_COMMU                         /* ͨ�Ű汾�� */
#define LS_PLATFORM_VER                 VER_PLATFORM                      /* ƽ̨�汾�� */
#define LS_DATA_MASKS8                  CPC_DATA_MASKS8                   /* 8λ�������� */
#define LS_DATA_MASKS16                 CPC_DATA_MASKS16                  /* 16λ�������� */
#define LS_DATA_MASKS32                 CPC_DATA_MASKS32                  /* 32λ�������� */
#define LS_BYTE_BITS                    CPC_BYTE_BITS                     /* �ֽ�λ�� */
#define LS_DISABLE_DISPLAY              CPC_DISABLE_DISPLAY               /* ����״̬ */
#define LS_DATA_WIDTH                   (CPC_RAM_DATA_BITS/CPC_BYTE_BITS) /* �ڴ����ݿ�ȣ��ֽ�����*/
#define LS_ALARM_NAME                   CEM_EVENT_NAME                    /* �澯�¼������ַ������� */
#define LS_EXCHANGE_NUMBER              (PRO_EXCHANGE_NUMBER+2)           /* ���������ܳ��� */
#define LS_MAX_PROTECT_GROUPS           CPC_MAX_PROTECT_GROUPS            /* ��󱣻�״̬������ */
#define LS_MAX_INPUT_GROUPS             CPC_MAX_INPUT_GROUPS              /* ����������� */
#define LS_MAX_OUTPUT_GROUPS            CPC_MAX_OUTPUT_GROUPS             /* ��󿪳������� */
#define LS_MAX_PROTECT_LOG_GROUPS       CPC_MAX_PROTECT_LOG_GROUPS        /* ����߼�״̬������ */
#define LS_MAX_WAVE_DATA_SIZE           CPC_MAX_WAVE_DATA_SIZE            /* ���¼�����ݵ��� */
#define LS_GET_CHANNEL_POINT            CSMV_GET_CHANNEL_POINT            /* ģ����ͨ���Ĳ������� */
#define LS_DATA_BIT16                   16                                /* 16λ���ݵ�λ�� */
//
#define LS_CHANNEL_TABLE_MEMORY         GET_CHANNEL_TAB_MEMORY            /* ģ����ͨ�����ݱ��ڴ���ַ */
#define LS_CHANNEL_TABLE_NUMBER         GET_CHANNEL_TAB_NUMBER            /* ģ����ͨ�����ݱ�����ݸ��� */
#define LS_INPUT_TAB_MEMORY             GET_INPUT_TAB_MEMORY              /* ���������ݱ��ڴ���ַ */
#define LS_INPUT_TAB_NUMBER             GET_INPUT_TAB_NUMBER              /* ���������ݱ�����ݸ��� */
#define LS_OUTPUT_TAB_MEMORY            GET_OUTPUT_TAB_MEMORY             /* ���������ݱ��ڴ���ַ */
#define LS_OUTPUT_TAB_NUMBER            GET_OUTPUT_TAB_NUMBER             /* ���������ݱ�����ݸ��� */
#define LS_WAVE_STATE_TAB_MEMORY        GET_WAVE_STATE_TAB_MEMORY         /* ¼��״̬�����ݱ��ڴ���ַ */
#define LS_WAVE_STATE_TAB_NUMBER        GET_WAVE_STATE_TAB_NUMBER         /* ¼��״̬�����ݱ�����ݸ��� */
//
#define LS_CALCULATE_DC(chl,coef,offset)       CSMV_CalculateDC(chl,coef,offset) /* ����ģ����ͨ����ֱ������ */
#define LS_CALCULATE_FOURIER(chl,para,offset)  CSMV_CalculateFourier(chl,para,offset) /* ����ģ����ͨ����г������ */
#define LS_CALCULATE_PEAK_VALUE(chl,offset)    CSMV_CalculatePeakValue(chl,offset) /* ����ģ����ͨ���ķ�ֵ */
extern float IEEE_To_Dev_Float( UNSIGNED udata );
extern UNSIGNED DEV_Float_To_IEEE( float fvalue );
#define LS_FLOAT_TO_IEEE(x)             DEV_Float_To_IEEE(x)              /* װ�ø���ת����IEEE��ʽ */
#define LS_IEEE_TO_FLOAT(x)             IEEE_To_Dev_Float(x)              /* IEEE��ʽ����ת���ɸ����ʽ */
#define LS_WORD_TO_BYTE(dst,src,len)    XJP_CSC_Word_To_Byte(dst,src,len) /* ��WORD����ת�����ֽ����� */
#define LS_BYTE_TO_WORD(dst,src,len)    XJP_CSC_Byte_To_Word(dst,src,len) /* ���ֽ�����ת����WORD���� */
#define LS_REPORT_PROTECT()             NU_Protect(pReport_Protect)       /* ���汣�� */
#define LS_REPORT_UNPROTECT()           NU_Unprotect()                    /* �ͷű������ݱ��� */
#define LS_WAVE_PROTECT()               NU_Protect(pWave_Protect)         /* ¼������ */
#define LS_WAVE_UNPROTECT()             NU_Unprotect()                    /* �ͷ�¼�����ݱ��� */
#define LS_READ_INPUT(group)            CINC_ReadInputValue(group)        /* ������״̬�� */
#define LS_INPUT_GROUP_NUMBER           CINC_INPUT_GROUP_NUMBER           /* ����˿����� */
#define LS_GET_GPS_PULSE_NUMBER         XJP_TM_Get_Pulse_Counter()        /* ��ȡGPS������� */
#define LS_SET_PROG_READY()  NU_Set_Events(& Event_2_CB, CPC_EVENT2_PROG_READY, NU_OR ) /* �����³���Ԥ��̬ */
#define LS_CLR_PROG_READY()  \
        NU_Set_Events(&Event_2_CB,(UNSIGNED)(~CPC_EVENT2_PROG_READY),NU_AND) /* ����³���Ԥ��̬ */
#define LS_GET_PROG_READY(ActEventPtr) \
        NU_Retrieve_Events(&Event_2_CB,CPC_EVENT2_PROG_READY,NU_AND,ActEventPtr,NU_NO_SUSPEND) /* ��ȡ�³���Ԥ��̬ */
#define LS_SET_CFG_READY() NU_Set_Events(& Event_2_CB, CPC_EVENT2_CFG_READY, NU_OR ) /* ��������Ԥ��̬ */
#define LS_GET_CFG_READY(ActEventPtr) \
        NU_Retrieve_Events(& Event_2_CB,CPC_EVENT2_CFG_READY,NU_AND,ActEventPtr,NU_NO_SUSPEND) /* ��ȡ����Ԥ��̬ */
#define LS_CLR_CFG_READY() \
        NU_Set_Events(& Event_2_CB,(UNSIGNED)(~CPC_EVENT2_CFG_READY),NU_AND); /* �������Ԥ��̬ */
#define LS_SET_CFG_PROGRAM() NU_Set_Events(& Event_2_CB, CPC_EVENT2_CFG_PROGRAM, NU_OR ) /* �������ù̻�״̬ */
#define LS_GET_CFG_PROGRAM(ActEventPtr) \
        NU_Retrieve_Events(& Event_2_CB,CPC_EVENT2_CFG_PROGRAM,NU_AND,ActEventPtr,NU_NO_SUSPEND) /* ��ȡ���ù̻�״̬ */
#define LS_CLR_CFG_PROGRAM() \
        NU_Set_Events(& Event_2_CB,(UNSIGNED)(~CPC_EVENT2_CFG_PROGRAM),NU_AND); /* ������ù̻�״̬ */
#define LS_GET_CFG_ERROR(ActEventPtr) \
        NU_Retrieve_Events(& Event_2_CB,CPC_EVENT2_CFG_ERROR,NU_AND,ActEventPtr,NU_NO_SUSPEND) /* ��ȡ�̻�����״̬ */
#define LS_SET_CLR_REPORT() CRMC_ClearReport(1)                                    /* �����������״̬ */
#define LS_SET_CLR_WAVE() NU_Set_Events(& Event_2_CB, CPC_EVENT2_WAVE_CLR, NU_OR ) /* �������¼��״̬ */
#define LS_SET_RESET() NU_Set_Events(& Event_2_CB, CPC_EVENT2_RESET, NU_OR ) /* ���ø����ź�״̬ */

#define LS_RAM_START_ADDR               0            /* ramоƬ��ʼ��ַ */
#define LS_RAM_END_ADDR                 0x04000000   /* ramоƬ������ַ */
/* �������� */
#define LS_CURRENT_COEFFICIENT          1.2                               /* У������ͨ���Ķֵ���� */
#define LS_VOLTAGE_NORMVALUE            40.0                              /* У����ѹͨ���ĵ�ѹֵ */
#define LS_MIN_COEFFICIENT              0.94                              /* ͨ����СУ��ϵ�� */
#define LS_MAX_COEFFICIENT              1.06                              /* ͨ�����У��ϵ�� */
#define LS_MIN_EXCURSION                -200.0                            /* ͨ����С��Ưֵ */
#define LS_MAX_EXCURSION                200.0                             /* ͨ�������Ưֵ */
#define LS_MIN_PHASE                    -10.0                             /* ͨ����СУ����λ */
#define LS_MAX_PHASE                    10.0                              /* ͨ�����У����λ */
#define LS_FLOAT_ERROR                  0.001                             /* ��󸡵���� */
#define LS_FEEQ_SELECT                  0x40                              /* Ƶ�ʾ���ѡ��λ */
#define LS_EXCHANGE_START               0                                 /* ����״̬�ڽ��������е�λ��ƫ�� */
#define LS_EXCHANGE_FAULT_NUMBER        1                                 /* ��������ڽ��������е�λ��ƫ�� */
#define LS_RESULT_TRUE                  0x5a                              /* �����ɹ� */
#define LS_RESULT_NULL                  0x00                              /* ����ʧ�� */
#define LS_MAX_CONFIG_BYTE              0xff                              /* ������������ֽ��� */

/* ��ʱʱ�� */
#define LS_REPORT_TIMEOUT               100                               /* ���������ʱʱ�� */
#define LS_WAVE_TIMEOUT                 100                               /* ¼��������ʱʱ�� */
#define LS_EVENT_TIMEOUT                100                               /* �¼�������ʱʱ�� */
#define LS_TIME_OUT                     800                               /* �����޸ĳ�ʱʱ�� */

/* �����¼�������Ͷ���.  */
#define LS_TYPE_CLEAR_REPORT            0x1                               /* ������� */
#define LS_TYPE_CLEAR_WAVE              0x2                               /* ���¼�� */

/* TB_CFB�ṹ��ÿ�������Ļ�����Ϣ�ֽ���(�����ֶ�����(2)+�����ֶ�ֵ(4)+��������(1)) */
#define TB_CFB_COND_BYTE_NUMBER         7

/* �����������Ͷ��� */
#define LS_START_RELAY_TEST             0x01                              /* ���������̵��� */
#define LS_TRIP_RELAY_TEST              0x02                              /* ������բ���� */
#define LS_OUT_RELAY_TEST               0x04                              /* ��������λ */
/* �ڲ��¼����Ͷ��� */
#ifndef XJ_INTERIOR_EVENTBITS
#define XJ_INTERIOR_EVENTBITS
#define LS_RES_EVENT_START              1                                 /* ���������̵��� */
#define LS_RES_EVENT_TRIP               2                                 /* ������բ���� */
#define LS_RES_EVENT_OUT                3                                 /* �������� */
#define LS_RES_EVENT_REPAIR             4                                 /* �����Լ���� */
#define LS_RES_EVENT_EEPROM             5                                 /* дEEPROM���� */
#define LS_RES_EVENT_FLASH              6                                 /* дFLASH���� */
#define LS_RES_EVENT_ERASE              7                                 /* FLASH������������ */
#define LS_RES_EVENT_SIZE               8                                 /* ���ݴ�С���� */
#define LS_RES_EVENT_RESTART            9                                 /* װ�������ϵ� */
#define LS_RES_EVENT_RELAY              10                                /* �̵������� */
#define LS_RES_EVENT_CLOSE              11                                /* �̵���˲ʱ���� */
#define LS_RES_EVENT_CLOSE_EXT          12                                /* ��չ��·˲ʱ���� */
#define LS_RES_EVENT_REPORT_CLR         13                                /* ������� */
#define LS_RES_EVENT_WAVE_CLR           14                                /* ���¼�� */
#define LS_RES_EVENT_ZONE_CHANGE        15                                /* ��ֵ���仯 */
#define LS_RES_EVENT_CONFIG_CHANGE      16                                /* ���ñ仯 */
#define LS_RES_EVENT_GETIN_OVERTIME     17                                /* ���տ�������ʱ */
#endif
/*=======================================================================*/
/* ����Ͷ��                                                              */
/*=======================================================================*/
#define ON                              1                                 /* Ͷ�� */
#define OFF                             0                                 /* �˳� */
#define SWITCH_IEC103_TI_M_81           ON                                /* �ٻ�װ�û�����Ϣ */
#define SWITCH_IEC103_TI_M_82           ON                                /* װ�ú������� */
#define SWITCH_IEC103_TI_M_90           ON                                /* �����´�����Ԥ��״̬ */
#define SWITCH_IEC103_TI_M_91           OFF                               /* �����´�����״̬ */
#define SWITCH_IEC103_TI_M_96           ON                                /* Ԥ���̻��������� */
#define SWITCH_IEC103_TI_M_97           ON                                /* ִ�й̻��������� */
#define SWITCH_IEC103_TI_M_98           ON                                /* ��ѯ�������ù̻���� */
#define SWITCH_IEC103_TI_M_A0           ON                                /* �ٻ����ݱ���ܸ��� */
#define SWITCH_IEC103_TI_M_A1           ON                                /* �ٻ����ݱ����Ϣ */
#define SWITCH_IEC103_TI_M_A2           ON                                /* �ٻ��ֶ���Ϣ */
#define SWITCH_IEC103_TI_M_A3           ON                                /* �ٻ��������ݵ��ܸ��� */
#define SWITCH_IEC103_TI_M_A4           ON                                /* �ٻ���¼��ȫ���ֶ� */
#define SWITCH_IEC103_TI_M_A5           ON                                /* �ٻ���¼�ĵ����ֶ� */
#define SWITCH_IEC103_TI_M_A6           ON                                /* �ٻ���¼�Ķ���ֶ� */
#define SWITCH_IEC103_TI_M_A7           ON                                /* ���������ֶε�ֵ */
#define SWITCH_IEC103_TI_M_A8           ON                                /* ѡ��༭�ֶ� */
#define SWITCH_IEC103_TI_M_A9           ON                                /* ���ñ༭��ֵ���ֵ */
#define SWITCH_IEC103_TI_M_AA           ON                                /* ȷ�ϱ༭��ֵ���ֵ */
#define SWITCH_IEC103_TI_M_AB           ON                                /* ��ȡ���ݱ��״̬ */
#define SWITCH_IEC103_TI_M_AC           ON                                /* ��ȡ���ݱ������CRC�� */
#define SWITCH_IEC103_TI_M_AD           ON                                /* �������ݱ������ */
#define SWITCH_IEC103_TI_M_AE           ON                                /* �ⶳ���ݱ������ */
#define SWITCH_IEC103_TI_M_AF           ON                                /* �ⶳ���ݱ������ */
#define SWITCH_IEC103_TI_M_B0           ON                                /* �����������ֶ�ֵ */
#define SWITCH_IEC103_TI_M_C0           ON                                /* �ٻ��¼����� */
#define SWITCH_IEC103_TI_M_C1           ON                                /* �ٻ��¼���Ϣ */
#define SWITCH_IEC103_TI_M_C2           ON                                /* �ٻ��¼������� */
#define SWITCH_IEC103_TI_M_C3           ON                                /* �ٻ��¼����� */
#define SWITCH_IEC103_TI_M_C4           ON                                /* ����¼����� */
#define SWITCH_IEC103_TI_M_C5           ON                                /* �ٻ�������Ϣ */
#define SWITCH_IEC103_TI_M_C6           ON                                /* �ٻ����������� */
#define SWITCH_IEC103_TI_M_C7           ON                                /* �ٻ������������ */
#define SWITCH_IEC103_TI_M_C8           ON                                /* �ٻ��������� */
#define SWITCH_IEC103_TI_M_C9           ON                                /* ������ϼ�¼���� */
#define SWITCH_IEC103_TI_M_CA           ON                                /* �ֶ�����¼�� */
#define SWITCH_IEC103_TI_M_CB           ON                                /* �ٻ�¼����� */
#define SWITCH_IEC103_TI_M_CC           ON                                /* �ٻ�¼��ʱ�� */
#define SWITCH_IEC103_TI_M_CD           ON                                /* �ٻ�¼����Ϣ */
#define SWITCH_IEC103_TI_M_CE           ON                                /* �ٻ�¼������������� */
#define SWITCH_IEC103_TI_M_E0           ON                                /* Ԥ���������� */
#define SWITCH_IEC103_TI_M_E1           ON                                /* ִ�д������� */
#define SWITCH_IEC103_TI_M_E2           ON                                /* ��ͨ��ϵ������ֵ */
#define SWITCH_IEC103_TI_M_E3           ON                                /* �޸�ͨ��ϵ�� */
#define SWITCH_IEC103_TI_M_E4           ON                                /* ��ͨ���ֵ */
#define SWITCH_IEC103_TI_M_E5           ON                                /* �޸�ͨ���ֵ */
#define SWITCH_IEC103_TI_M_E6           ON                                /* ��װ����Ư�̶ȼ���Ưֵ */
#define SWITCH_IEC103_TI_M_E7           ON                                /* �޸�װ����Ư */
#define SWITCH_IEC103_TI_M_E8           OFF                               /* �ϵ�ͨ���ĵ�ǰ��λ��Ӧ�ó���ע�ᣩ*/
#define SWITCH_IEC103_TI_M_E9           ON                                /* �޸�ͨ����λ��� */
#define SWITCH_IEC103_TI_M_EA           ON                                /* �ϵ�ģ��ͨ����λ��ֵ */
#define SWITCH_IEC103_TI_M_F0           ON                                /* �źŸ��� */
#define SWITCH_IEC103_TI_M_F1           ON                                /* �ٻ�������״̬��GPS������� */
#define SWITCH_IEC103_TI_M_F7           ON                                /* ���ڴ����� */

#endif
