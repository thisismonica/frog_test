
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
/*      smvc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      SMV - Sample Management                                          */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common sample       */
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
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       11-15-2007      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "plat/inc/xj_cosc.h"
#include "plat/inc/pro_defs.h"
#include "plat/inc/csmv_extr.h"
#include "plat_specific/plat_specific.h"

#define CSMV_MAX_ANALOGNUMBER       (CPC_MAX_PHYSICAL_CHL_NUMBER+CPC_MAX_VIRTUAL_CHL_NUMBER)
/* ƽ̨֧�ֵ����ģ����ͨ������ */

/* ����ΪOS������� */
#ifdef WIN32
/* ȫ�ֱ������� */
/* Windows �궨�� */
#endif

#ifndef WIN32 /* ��windowsȱʡ����һЩ���Ա�ͨ������ */
/* ȫ�ֱ������� */
/* Nucleus�궨�� */
#endif

/* Define external inner-component global data references.  */
extern ESP_CFG PLAT_cfg;
extern SMVC_SAMPLE_RECORD SMVC_SampleRecord;
extern SMVC_ERR SMVC_Error;

float DC_Coefficient;
float FOURIER01_R[ CPC_MAX_SAMPLE_POINT ];
float FOURIER01_I[ CPC_MAX_SAMPLE_POINT ];
float FOURIER02_R[ CPC_MAX_SAMPLE_POINT ];
float FOURIER02_I[ CPC_MAX_SAMPLE_POINT ];
float FOURIER03_R[ CPC_MAX_SAMPLE_POINT ];
float FOURIER03_I[ CPC_MAX_SAMPLE_POINT ];
float FOURIER05_R[ CPC_MAX_SAMPLE_POINT ];
float FOURIER05_I[ CPC_MAX_SAMPLE_POINT ];

UNSIGNED SamplePulseMemory[ CPC_MAX_CHANNEL_POINT ]; /* ������������������� */
float AnalogSampleMemory[( CSMV_MAX_ANALOGNUMBER * CPC_MAX_CHANNEL_POINT )]; /* ģ��������ֵ������ */
UNSIGNED AnalogQualityMemory[(( CSMV_MAX_ANALOGNUMBER + 31 )/ 32 )* CPC_MAX_CHANNEL_POINT];
/* ģ����Ʒ�����ػ�����(��λ���) */
UNSIGNED InputSampleMemory[CPC_MAX_INPUT_GROUPS * CPC_MAX_CHANNEL_POINT]; /* ������������ */
UNSIGNED InputQualityMemory[CPC_MAX_INPUT_GROUPS * CPC_MAX_CHANNEL_POINT]; /* ������Ʒ�����ػ����� */
UNSIGNED OutputSampleMemory[CPC_MAX_OUTPUT_GROUPS * CPC_MAX_CHANNEL_POINT]; /* ������������ */
UNSIGNED OutputQualityMemory[CPC_MAX_OUTPUT_GROUPS * CPC_MAX_CHANNEL_POINT]; /* ������Ʒ�����ػ����� */
UNSIGNED ProStateSampleMemory[CPC_MAX_PROTECT_GROUPS * CPC_MAX_CHANNEL_POINT]; /* ����״̬�������� */
UNSIGNED ProStateQualityMemory[CPC_MAX_PROTECT_GROUPS * CPC_MAX_CHANNEL_POINT]; /* ����״̬��Ʒ�����ػ����� */
UNSIGNED LogStateSampleMemory[CPC_MAX_PROTECT_LOG_GROUPS * CPC_MAX_CHANNEL_POINT]; /* �߼�״̬�������� */
UNSIGNED LogStateQualityMemory[CPC_MAX_PROTECT_LOG_GROUPS * CPC_MAX_CHANNEL_POINT]; /* �߼�״̬��Ʒ�����ػ����� */

/* Define internal function calls.  */

/* ��ʼ��ģ�������� */
INT CSMV_InitializeChannelPara( VOID )
{
    INT k;
    INT PeriodPoint;
    float base_angle;

    PeriodPoint = CSMV_GET_PERIOD_POINT;
    /* ��ʼ�����ϼ���ϵ�� */
    base_angle = ( float )( 2.0 * CONSTANT_PI / PeriodPoint );
    /* �������г���ĸ���ϵ�� */
    for( k = 0; k < PeriodPoint; k ++)
    {
        /* fourier 01 */
        FOURIER01_R[k] = ( float )( 2.0 * cos( base_angle * k )/ PeriodPoint );
        /* fourier 01 */
        FOURIER01_I[k] = ( float )( 2.0 * sin( base_angle * k )/ PeriodPoint );

        /* fourier 02 */
        FOURIER02_R[k] = ( float )( 2.0 * cos( 2.0 * base_angle * k )/ PeriodPoint );
        /* fourier 02 */
        FOURIER02_I[k] = ( float )( 2.0 * sin( 2.0 * base_angle * k )/ PeriodPoint );

        /* fourier 03 */
        FOURIER03_R[k] = ( float )( 2.0 * cos( 3.0 * base_angle * k )/ PeriodPoint );
        /* fourier 03 */
        FOURIER03_I[k] = ( float )( 2.0 * sin( 3.0 * base_angle * k )/ PeriodPoint );

        /* fourier 05 */
        FOURIER05_R[k] = ( float )( 2.0 * cos( 5.0 * base_angle * k )/ PeriodPoint );
        /* fourier 05 */
        FOURIER05_I[k] = ( float )( 2.0 * sin( 5.0 * base_angle * k )/ PeriodPoint );
    }

    /* ����ֱ��ϵ����ֱ���㷨�ã� */
    DC_Coefficient = ( float )( 1.0 / PeriodPoint );

    /* ��ʼ������ֵ��¼��Ϣ */
    SMVC_SampleRecord.sample_offset = 0; /* ���²���λ��ƫ�� */
    SMVC_SampleRecord.sample_counter = 0; /* �������� */
    SMVC_SampleRecord.pSamplePulseMemory = SamplePulseMemory; /* ������������������� */
    SMVC_SampleRecord.AdAnalogCounter = CSMV_GET_PHY_ANALOG_NUMBER; /* A/Dģ����ͨ���� */
    SMVC_SampleRecord.pAnalogMemory = AnalogSampleMemory; /* ģ���������ڴ�ָ�� */
    SMVC_SampleRecord.AnalogQualityGroups = CSMV_GET_PHY_ANALOG_GROUPS; /* ģ����Ʒ���������� */
    SMVC_SampleRecord.pAanlogQualityMemory = AnalogQualityMemory; /* ģ����Ʒ�������ڴ�ָ�� */
    SMVC_SampleRecord.InputGroupsNumber = CSMV_GET_INPUT_GROUPS; /* ���������� */
    SMVC_SampleRecord.pInputMemory = InputSampleMemory; /* �����������ڴ�ָ�� */
    SMVC_SampleRecord.pInputQualityMemory = InputQualityMemory; /* ������Ʒ�������ڴ�ָ�� */
    SMVC_SampleRecord.OutputGroupsNumber = CSMV_GET_OUTPUT_GROUPS; /* ���������� */
    SMVC_SampleRecord.pOutputMemory = OutputSampleMemory; /* �����������ڴ�ָ�� */
    SMVC_SampleRecord.pOutputQualityMemory = OutputQualityMemory; /* ������Ʒ�������ڴ�ָ�� */
    SMVC_SampleRecord.ProStatusGroupsNumber = CPC_MAX_PROTECT_GROUPS; /* ����״̬������ */
    SMVC_SampleRecord.pProStatusMemory = ProStateSampleMemory; /* ����״̬�������ڴ�ָ�� */
    SMVC_SampleRecord.pProStatusQualityMemory = ProStateQualityMemory; /* ����״̬��Ʒ������ָ�� */
    SMVC_SampleRecord.LogicGroupsNumber = CPC_MAX_PROTECT_LOG_GROUPS; /* �߼�״̬������ */
    SMVC_SampleRecord.pLogicMemory = LogStateSampleMemory; /* �߼�״̬���ڴ�ָ�� */
    SMVC_SampleRecord.pLogicQualityMemory = LogStateQualityMemory; /* �߼�״̬��Ʒ�������ڴ�ָ�� */
    /* ��ʼ������������Ϣ */
    memset(& SMVC_Error, 0, sizeof( SMVC_ERR ));

    return( XJ_SUCCESS );
}

/* ��ȡ����������Ϣ */
VOID * CSMV_SampleInformation( UNSIGNED * Actual_Number, UNSIGNED * Actual_Size,
    UNSIGNED * Actual_Point )
{
    VOID * pSampleDataAddr;

    * Actual_Number = ( UNSIGNED )CSMV_GET_PHY_ANALOG_NUMBER;
    * Actual_Size = ( UNSIGNED )CSMV_GET_CHANNEL_POINT ;
    * Actual_Point = ( UNSIGNED )CSMV_GET_PERIOD_POINT;

    pSampleDataAddr = ( VOID *)CSMV_GET_ANALOG_MEMORY;

    return( pSampleDataAddr );
}

/* ��ȡ��ǰ����ָ�� */
VOID * CSMV_SamplePointer( VOID )
{
    float * SamplePointer;

    SamplePointer = CSMV_GET_ANALOG_MEMORY + CSMV_GET_SAMPLE_OFFSET;

    return( ( VOID *)SamplePointer );
}

/* ��ȡ��������������ʼָ�� */
INT CSMV_GetDataStartPointer( INT  iType, VOID ** valueMem, VOID ** qualityMemr, INT * number )
{
    INT status;
    ESP_CFG_INPUT_PORT * pPort;
    /*======================================*/
    status = XJ_UNAVAILABLE;
    switch( iType )
    {
        /* ģ����ͨ��Ʒ������ */
    case PRO_SAMPLE_ANALONG:
        if( valueMem )
            * valueMem = CSMV_GET_ANALOG_MEMORY;
        if( qualityMemr )
            * qualityMemr = CSMV_GET_ANALOG_QUA_MEMORY;
        if( number )
            * number = CSMV_GET_PHY_ANALOG_NUMBER;
        status = XJ_SUCCESS;
        break;
        /* ����״̬�� */
    case PRO_SAMPLE_INPUT:
        if( valueMem )
            * valueMem = CSMV_GET_INPUT_MEMORY;
        if( qualityMemr )
            * qualityMemr = CSMV_GET_INPUT_QUA_MEMORY;
        if( number )
            * number = CSMV_GET_INPUT_GROUPS;
        status = XJ_SUCCESS;
        break;
        /* ����״̬�� */
    case PRO_SAMPLE_OUTPUT:
        if( valueMem )
            * valueMem = CSMV_GET_OUTPUT_MEMORY;
        if( qualityMemr )
            * qualityMemr = CSMV_GET_OUTPUT_QUA_MEMORY;
        if( number )
            * number = CSMV_GET_OUTPUT_GROUPS;
        status = XJ_SUCCESS;
        break;
        /* ����״̬�� */
    case PRO_SAMPLE_PRO_STS:
        if( valueMem )
            * valueMem = CSMV_GET_PRO_STS_MEMORY;
        if( qualityMemr )
            * qualityMemr = CSMV_GET_PRO_STS_QUA_MEMORY;
        if( number )
            * number = CSMV_GET_PRO_STS_GROUPS;
        status = XJ_SUCCESS;
        break;
        /* �߼�״̬�� */
    case PRO_SAMPLE_PRO_LOG:
        if( valueMem )
            * valueMem = CSMV_GET_PRO_LOG_MEMORY;
        if( qualityMemr )
            * qualityMemr = CSMV_GET_PRO_LOG_QUA_MEMORY;
        if( number )
            * number = CSMV_GET_PRO_LOG_GROUPS;
        status = XJ_SUCCESS;
        break;
        /* �����������ָ�� */
    case PRO_SAMPLE_PULSE:
        if( valueMem )
            * valueMem = CSMV_GET_SAMPLE_PULSE_MEMORY;
        if( number )
            * number = 1;
        status = XJ_SUCCESS;
        break;
        /* ����Ʒ��������Ϣָ��(ʵʱ����) */
    case PRO_SOFT_INPUT_QUA_INFO:
        pPort = CINC_INPUT_PORT_MEMORY; /* ����˿��׵�ַ */
        pPort += CINC_HARD_INPUT_GROUPS;
        if( valueMem )
            * valueMem = pPort->QualityAddress;
        if( qualityMemr )
            * qualityMemr = pPort->QualityCheckAddr;
        if( number )
            * number = CSMV_GET_INPUT_GROUPS;
        status = XJ_SUCCESS;
        break;
        /* ģ����ͨ��Ʒ�����ؼ���λ(ʵʱ����) */
    case PRO_ANALONG_CHECK_QUALITY:
        if( valueMem )
            * valueMem = CSMV_GET_ORIGINL_ANA_CHECKQUA_MEM;
        if( number )
            * number = CSMV_GET_PHY_ANALOG_GROUPS;
        status = XJ_SUCCESS;
        break;
    default:
        /* ����֧�ֵ�����ֱ�ӷ���ʧ�� */
        break;
    }
    /* ����ֵ */
    return( status );
}

/* ��ȡ����λ��ƫ�� */
UNSIGNED CSMV_GetSampleOffset( VOID )
{
    return( CSMV_GET_SAMPLE_OFFSET );
}
