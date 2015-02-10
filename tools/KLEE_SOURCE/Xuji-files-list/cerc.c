
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
/*      cerc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CER - Common Error Management                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common error        */
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
/*      cem_extr.h                          Protect Control functions    */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       08-13-2008      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "plat/inc/cer_extr.h"
#include "plat/inc/csmv_extr.h"
#include "plat/inc/xj_out_check.h"
#include "plat/inc/ctb_extr.h"
#include "plat_specific/plat_specific.h"

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
extern NU_EVENT_GROUP Event_1_CB;
extern volatile UNSIGNED TMD_System_Clock;
extern INT SMVC_InitializeFlag; /* �������ݳ�ʼ����ɱ�־ */
extern UNSIGNED SMVC_Power_Error_Counter; /* ��Դ�Լ����������������ؼ������� */
extern volatile UNSIGNED SMVC_Power_Error_Resume_Counter;  /* ����װ�õ�Դ�ָ������� */
extern UNSIGNED SMVC_Power_Error_Flag;    /* ��Դ�Լ�����־ */
extern UNSIGNED SMVC_SampleCount; /* ���������� */
extern CTABLE CTable;
/* Define internal function calls.  */

UNSIGNED CER_AD_ErrTime = CER_AD_ERR_TIME;        /* A/D error delay time */
UNSIGNED CER_AD_ResumeTime = CER_AD_RESUME_TIME;  /* A/D error resume time */

/* ��ȡHMI������Ϣ */
UNSIGNED CERC_ReadHMIErrorBits( VOID )
{
    return( 0 );
}

/* ��ȡ������Ϣ */
INT CERC_ReadErrorBits( UNSIGNED * Actual_ErrorBits )
{
#ifndef WIN32
    CHAR group_name[8];  /* buffer size */
    UNSIGNED event_flags;
    UNSIGNED tasks_suspended;
    NU_TASK * first_task;

    NU_Event_Group_Information(& Event_1_CB, group_name,& event_flags,
        & tasks_suspended,& first_task );
    * Actual_ErrorBits = event_flags;
    return( XJ_SUCCESS );
#else
    * Actual_ErrorBits = ( 1 << 13 );
    return( XJ_SUCCESS );
#endif
}

/* �������ݳ�ʼ����ɼ�� */
VOID CERC_SampleIniCheckMain( VOID )
{
    int chl_number;
    static INT SampleIniFlag = 0;

    chl_number = CSMV_GET_PHY_ANALOG_NUMBER;
    if( chl_number != 0 )
    {
        if( SampleIniFlag != SMVC_InitializeFlag )
        {
            /* ��������ʼ����� */
            if( SMVC_InitializeFlag )
            {
                /* ���ò�����ɱ�־ */
                NU_Set_Events(& Event_1_CB, CPC_EVENT1_SAMPLE_OK, XJ_OR );
                SampleIniFlag = SMVC_InitializeFlag;
            }
            else
            {
                /* ���������ɱ�־ */
                NU_Set_Events(& Event_1_CB,( UNSIGNED )(~ CPC_EVENT1_SAMPLE_OK ), XJ_AND );
                SampleIniFlag = SMVC_InitializeFlag;
            }
        }
    }
    else
    {
        if( SampleIniFlag == XJ_FALSE )
        {
            /* ���ò�����ɱ�־ */
            NU_Set_Events(& Event_1_CB, CPC_EVENT1_SAMPLE_OK, XJ_OR );
            SampleIniFlag = XJ_TRUE;
        }
    }
}

/* A/D��������� */
VOID CERC_AD_CheckMain( VOID )
{
    int chl_number;
    static INT IniFlag = XJ_FALSE;
    static INT ErrFlag = XJ_FALSE;
    static UNSIGNED SampleCount;
    static UNSIGNED CheckDelay;
    static UNSIGNED CheckLoseDelay;
    static UNSIGNED ResumeDelay;
    static INT iInitFlag = NU_FALSE;
    float  fTemp;

    if( iInitFlag == NU_FALSE )
    {
        if( CSMV_GET_SAMPLE_RATE > 1 )
        {
            /* �������(��λ����) */
            fTemp = ( float )1000.0 / CSMV_GET_SAMPLE_RATE;
            fTemp *= CER_AD_ERR_COUNT;
            /* У��ϵ�� */
            fTemp += 1;
            /* �ж���ֵ�Ƿ������Сֵ */
            if(( UNSIGNED )fTemp > CER_AD_ErrTime )
            {
                CER_AD_ErrTime = ( UNSIGNED )fTemp;
            }
            /* ���㷵��ʱ�� */
            fTemp *= CER_AD_ERR_RESUME_COEF;
            /* �ж���ֵ�Ƿ������Сֵ */
            if(( UNSIGNED )fTemp > CER_AD_ResumeTime )
            {
                CER_AD_ResumeTime = ( UNSIGNED )fTemp;
            }

            iInitFlag = NU_TRUE;
        }
    }
    chl_number = CSMV_GET_PHY_ANALOG_NUMBER;
    /* �жϲ���׼���Ƿ����,�����ɲŽ���A/D�Լ� */
    if(( chl_number != 0 ) && ( CSMV_GetSampleInitFlag() == NU_SUCCESS ))
    {
        /* ����ģ�����ɼ�ͨ��ʱ���Ž���A/D�Լ� */
        if( IniFlag == XJ_FALSE )
        {
            SampleCount = SMVC_SampleCount;
            CheckDelay = TMD_System_Clock;
            ResumeDelay = TMD_System_Clock;
            CheckLoseDelay = TMD_System_Clock;
            ErrFlag = XJ_FALSE;
            IniFlag = XJ_TRUE;
            return;
        }

        /* �жϼ��ʱ���Ƿ�,���û��ֱ�ӷ��� */
        if(( TMD_System_Clock - CheckLoseDelay ) < CER_AD_ErrTime )
        {
            return;
        }
        /* ���¼��ʱ�� */
        CheckLoseDelay = TMD_System_Clock;
        if( SampleCount != SMVC_SampleCount )
        {
            /* �������ݱ仯 */
            SampleCount = SMVC_SampleCount;
            CheckDelay = TMD_System_Clock;
            if( ErrFlag == XJ_TRUE )
            {
                /* A/D����ָ���� */
                if(( TMD_System_Clock - ResumeDelay ) >= CER_AD_ResumeTime )
                {
                    NU_Set_Events(& Event_1_CB,( UNSIGNED )(~ CPC_EVENT1_ERR_AD ), NU_AND );
                    ErrFlag = XJ_FALSE;
                }
            }
        }
        else
        {
            /* ���������ޱ仯 */
            ResumeDelay = TMD_System_Clock;
            if( ErrFlag == XJ_FALSE )
            {
                if(( TMD_System_Clock - CheckDelay ) > CER_AD_ErrTime )
                {
                    /* ����AD�Լ�����־ */
                    NU_Set_Events(& Event_1_CB, CPC_EVENT1_ERR_AD, XJ_OR );
                    ErrFlag = XJ_TRUE;
                }
            }
        }
    }
}

/* ��Դ�Լ� */
void CERC_Power_Check( void )
{
    static UNSIGNED PowerErrorCounter = 0;
    static UNSIGNED uErrorDelay1 = 0, uErrorDelay2 = 0;
    static INT iIniFlag = 0;
    INT    iFlag;
    //--------------------------------------------------------------------------
    if( iIniFlag == 0 )
    {
        iIniFlag = 1;
        uErrorDelay1 = TMD_System_Clock;
        uErrorDelay2 = TMD_System_Clock;
    }

    /* �жϵ�Դ�Ƿ���� */
    if(( SMVC_Power_Error_Flag == 1 )
        || ( PowerErrorCounter != SMVC_Power_Error_Counter ))
    {
        /* �ñ������ڱ�־ */
        OUT_SetClockRelayFlag( 1 );
        NU_Set_Events(& Event_1_CB, CPC_EVENT1_CLOSE_RELAY, XJ_OR );
        /* ���±������ڵ���ʱʱ�� */
        uErrorDelay1 = TMD_System_Clock;
        /* �ж��õ�Դ�Լ�����־����ʱʱ�� */
        if(( TMD_System_Clock - uErrorDelay2 ) > CER_POWER_ERR_ALARME )
        {
            NU_Set_Events(& Event_1_CB, CPC_EVENT1_ERR_5VPOWER, XJ_OR );
        }
        /* ���µ�Դ�Լ���������� */
        PowerErrorCounter = SMVC_Power_Error_Counter;
    }
    else
    {
        /* ���µ�Դ�Լ�����־����ʱʱ�� */
        uErrorDelay2 = TMD_System_Clock;
        /* �жϱ������ڱ�־ */
        OUT_GetClockRelayFlag(& iFlag );
        if( iFlag == 1 )
        {
            /* �жϱ������ڵ���ʱʱ�� */
            if(( TMD_System_Clock - uErrorDelay1 ) > CER_POWER_ERR_RESUME )
            {
                /* ��������ڱ�־ */
                OUT_SetClockRelayFlag( 0 );
                NU_Set_Events(& Event_1_CB, ( UNSIGNED )(~ CPC_EVENT1_CLOSE_RELAY ), XJ_AND );
            }
        }
        else
        {
            /* ���±������ڵ���ʱʱ�� */
            uErrorDelay1 = TMD_System_Clock;
        }
        /* ����װ�õ�Դ�Լ�Ҫ�лָ����� */
        if( ( SMVC_Power_Error_Resume_Counter == 0 ) && ( PLAT_cfg.device.FunSelect & FUN_IED_DEVICE ))
        {
            /* ��Դ�Լ����ָ� */
            NU_Set_Events(& Event_1_CB,( UNSIGNED )(~ CPC_EVENT1_ERR_5VPOWER ), NU_AND );
        }
    }
}

/* �����û��澯λ */
VOID CERC_SetUserEventBits( UNSIGNED uEventBits, INT iFlag )
{
    UNSIGNED uActBits;

    /* �޳�ƽ̨�¼�λ,ƽ̨�¼�λ״̬����Ӧ�ó���Ŀ��� */
    uActBits = ( uEventBits & (~ CPC_EVENT1_PLAT_BITS ));

    if( iFlag != 0 )
    {
        /* ���ø澯�¼�λ�����������û��¼���������¼�λ */
        NU_Set_Events(& Event_1_CB, uEventBits, XJ_OR );
    }
    else
    {
        /* ����澯�¼�λ��ֻ��������û��¼�λ */
        NU_Set_Events(& Event_1_CB,( UNSIGNED )(~ uActBits ), XJ_AND );
    }
}

/* RAM�������ļ��Լ������� */
VOID CERC_FileCheck_Main( VOID )
{
    static INT Flag = XJ_FALSE;

    if( CERC_FileCheck() != XJ_NULL )
    {
        if( Flag == XJ_FALSE )
        {
            /* RAM����У����� */
            NU_Set_Events(& Event_1_CB, CPC_EVENT1_ERR_PROGRAM, XJ_OR );
            CEMC_FillEvent( "����У�����", ALARM_EVENT_ID );
            Flag = XJ_TRUE;
        }
    }
}

/* ����ֵ�����Լ� */
VOID CERC_SettingCheck_Main( VOID )
{
    INT status;

    /* �޸�����ֵ���� */
    status = XJP_NVData_Repair( ( HNDM )CTable.NVDataId, CPC_DATA_TIMEOUT );
    if( status == XJ_END_OF_LOG )
    {
        /* ��ֵ�Լ���� */
        NU_Set_Events(& Event_1_CB, CPC_EVENT1_ERR_SETTING, XJ_OR );
    }
}
