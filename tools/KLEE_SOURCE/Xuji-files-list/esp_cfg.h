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
/*      esp_cfg.h                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      ESP - Embed Soft Platform Config                                 */
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

/* Check to see if the file has been included already.  */

#ifndef ESP_CFG_H
#define ESP_CFG_H

#define ESP_MAX_PORT_ADDRESS        4  /* ���˿ڵ�ַ��Ŀ */

/* ��Դͨ�����ò���. */
typedef struct ESP_CFG_SMVC_POWER_struct
{
    /* ��Դͨ���� */
    INT ChannelNumber;
    /* ��Դ������ֵ */
    float MaxValue;
    /* ��Դ������ֵ */
    float MinValue;
}ESP_CFG_SMVC_POWER, * PESP_CFG_SMVC_POWER;

/* ����ֵ���ò���. */
typedef struct ESP_CFG_SMVC_struct
{
    /* ʹ��ADƬ�� */
    INT AdNumber;
    /* �����ģ����ͨ���� */
    INT PhysicalAnalogNumber;
    /* ˫����SVͨ������ */
    INT DSampleSVChlNum;
    /* ˫������ʶ */
    INT DSampleFlag;
    /* �����ģ����ͨ���� */
    INT VirtualAnalogNumber;
    /* ģ����Ʒ��������ͨ������ */
    INT AnalogQualityGroups;
    /* ÿ���ڲ������� */
    INT PeriodPoint;
    /* ��·Ƶ�� */
    float LineFrequency;
    /* ������(ÿ���������) */
    float SampleRate;
    /* ÿ��ͨ���Ĳ���ֵ���� */
    INT ChannelPoint;
    /* ������ŵ����ֵ */
    INT SampleCounter;
    /* ��Դ�Լ��ͨ������ */
    INT PowerCheckNumber;
    /* ��Դ�Լ�������Ϣ */
    ESP_CFG_SMVC_POWER * PowerChannel;
    /* ����ֵ����Ʒ�����ش����־ */
    INT LoseSV_Quality;
    /* ��ʧһ�����ֵ��ֵ�����־ */
    INT LoseOneSV_Insert;
}ESP_CFG_SMVC, * PESP_CFG_SMVC;

/* ��Ƶͨ�����ò���. */
typedef struct ESP_CFG_FREQ_CHANNEL_struct
{
    /* ��Ƶ��ѹͨ���� */
    INT ChannelNumber;
    /* Ƶ�ʱ仯�� */
    float FrequencyDlt;
}ESP_CFG_FREQ_CHANNEL, * PESP_CFG_FREQ_CHANNEL;

/* ��Ƶ���ò���. */
typedef struct ESP_CFG_FREQUENCY_struct
{
    /* ��Ƶͨ������ */
    INT FrequencyNumber;
    /* ��Ƶͨ�����ò���. */
    ESP_CFG_FREQ_CHANNEL * FrequencyChannel;
}ESP_CFG_FREQUENCY, * PESP_CFG_FREQUENCY;

/* ����˿����ò���. */
typedef struct ESP_CFG_INPUT_PORT_struct
{
    /* ����˿���Ŀ */
    INT PortNumber;
    /* ����˿�����λ�� */
    INT PortBits;
    /* ����˿��������� */
    UNSIGNED PortMasks;
    /* ����˿ڵ�ַ */
    VOID * PortAddress[ ESP_MAX_PORT_ADDRESS ];
    /* ��������תλ���� */
    UNSIGNED ReverseBits;
    /* ����Ʒ�����ص�ַ */
    VOID * QualityAddress;
    /* ����Ʒ�����ؼ���λ��ַ */
    VOID * QualityCheckAddr;
}ESP_CFG_INPUT_PORT, * PESP_CFG_INPUT_PORT;

/* ���������ò���. */
typedef struct ESP_CFG_INPUT_struct
{
    /* ���������� */
    INT InputNumbers;
    /* ���������� */
    INT InputGroups;
    /* �������˿����� */
    ESP_CFG_INPUT_PORT * InputPort;
    /* Ӳ������������ */
    INT HardInputGroups;
    /* ������������� */
    INT SoftInputGroups;
    /* ���鿪��λ(��1��ʼ) */
    INT ResetInputBits;
    /* ¼������������ */
    INT WaveInputGroups;
}ESP_CFG_INPUT, * PESP_CFG_INPUT;

/* �����˿����ò���. */
typedef struct ESP_CFG_OUTPUT_PORT_struct
{
    /* �����˿���Ŀ */
    INT PortNumber;
    /* �����˿�����λ�� */
    INT PortBits;
    /* �����˿��������� */
    INT PortMasks;
    /* �����˿ڵ�ַ */
    VOID * PortAddress[ ESP_MAX_PORT_ADDRESS ];
    /* ��������תλ���� */
    UNSIGNED ReverseBits;
    /* ����Ʒ�����ص�ַ */
    VOID * QualityAddress;
    /* ����Ʒ�����ؼ���λ��ַ */
    VOID * QualityCheckAddr;
}ESP_CFG_OUTPUT_PORT, * PESP_CFG_OUTPUT_PORT;

/* ���������ò���. */
typedef struct ESP_CFG_OUTPUT_struct
{
    /* ���������� */
    INT OutputNumber;
    /* ���������� */
    INT OutputGroups;
    /* �������˿����� */
    ESP_CFG_OUTPUT_PORT * OutputPort;
    /* Ӳ������������ */
    INT HardOutputGroups;
    /* Ӳ�����������λ���� */
    UNSIGNED * HardOutputCheckMasks;
    /* Ӳ������������λ���� */
    UNSIGNED * HardOutputHoldMasks;
    /* Ӳ���������źŵ� */
    UNSIGNED * HardOutputSignalLed;
    /* Ӳ����������բ�� */
    UNSIGNED * HardOutputTripLed;
    /* �澯�̵�������λ */
    UNSIGNED * HardOutputAlarm;
    /* ����̵�������λ */
    UNSIGNED * HardOutputReset;
    /* ������������� */
    INT SoftOutputGroups;
    /* ��չ������� */
    INT HardExtendCount;
}ESP_CFG_OUTPUT, * PESP_CFG_OUTPUT;

/* װ�����ò���. */
typedef struct ESP_CFG_DEVICE_struct
{
    /* CPU���� */
    INT CpuNumber;
    /* CPU���� */
    INT CpuType;
    /* ����ѡ�� */
    UNSIGNED FunSelect;
    /* װ�ù������� */
    UNSIGNED DeviceFun;
    /* װ�ö�ֵ������ */
    INT SetZoneCount;
}ESP_CFG_DEVICE, * PESP_CFG_DEVICE;

/* ��ʷ��¼���ò���. */
typedef struct ESP_CFG_HISTORY_struct
{
    /* �û��¼����� */
    INT UserEventGroups;
    /* ��ʱ�����¼����� */
    INT TempEventNumber;
    /* ��ʱ���汨����� */
    INT TempReportNumber;
}ESP_CFG_HISTORY, * PESP_CFG_HISTORY;

/* ���ݱ����ò���. */
typedef struct ESP_CFG_TABLE_struct
{
    /* ���ݱ���� */
    INT TableNumber;
    /* �ֶθ��� */
    INT FieldNumber;
    /* ����ֵ���ݸ��� */
    INT SettingNumber;
}ESP_CFG_TABLE, * PESP_CFG_TABLE;

/* ESP ���ò��� */
typedef struct ESP_CFG_struct
{
    /* װ�����ò���. */
    ESP_CFG_DEVICE device;
    /* ����ֵ���ò���. */
    ESP_CFG_SMVC smvc;
    /* ��Ƶ���ò���. */
    ESP_CFG_FREQUENCY frequency;
    /* ���������ò���. */
    ESP_CFG_INPUT input;
    /* ���������ò���. */
    ESP_CFG_OUTPUT output;
    /* ���ݱ����ò���. */
    ESP_CFG_TABLE table;
    /* ��ʷ��¼���ò���. */
    ESP_CFG_HISTORY history;
}ESP_CFG, * PESP_CFG;

/*  Initialization functions.  */

/* Error checking for supplemental functions.  */

/* Core processing functions.  */

#endif
