
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
/*      pmi_led.c                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      PMI - Platform Initialize                                        */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the initialization routine for this           */
/*      component.                                                       */
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
/*      ZhiDeYang       02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include <string.h>
#include <math.h>
#include "plat/inc/xj_cosc.h"
#include "plat/es_plat.h"
#include "plat_specific/inc/pmi_extr.h"
#include "plat_specific/plat_specific.h"
#include "plat/inc/devi_extr.h"

/* Define external inner-component global data references.  */
/* ƽ̨���еƹ����� */
HLEM PlatFormRunLedHandle = 0;

/* ����ΪOS������� */
#ifdef WIN32
#endif

#ifndef WIN32 /* ��windowsȱʡ����һЩ���Ա�ͨ������ */
#endif

typedef struct PMI_LED_struct
{
    UNSIGNED  PlatAlarmLedBits;      /* ƽ̨��澯���¼�λ */
    UNSIGNED  RunLedAbnorEventBits;  /* ƽ̨���е��쳣�¼�λ */
}PMI_LED;

PMI_LED PmiLed =
{
    CPC_EVENT1_DEV_ERR,         /* ƽ̨��澯���¼�λ */
    CPC_EVENT1_RUNLED_ABNORMAL  /* ƽ̨���е��쳣�¼�λ */
};

/* Define internal function calls.  */

/* ��ʼ��װ�����е� */
INT PMI_InitializeDeviceLed( VOID )
{
    /* ����ƽ̨���еƹ����� */
    PlatFormRunLedHandle = XJP_Led_Create();

    return( XJ_SUCCESS );
}

/* ����ƽ̨��澯���¼�λ */
INT  PMI_SetPlatAlarmLedBits( UNSIGNED uPlatAlarmBits )
{
    PmiLed.PlatAlarmLedBits = uPlatAlarmBits;
    return( XJ_SUCCESS );
}

/* ����ƽ̨���е��쳣�¼�λ���� */
INT PMI_SetPlatRunLedAbnorBits( UNSIGNED uRunAbnorEventBits )
{
    PmiLed.RunLedAbnorEventBits = uRunAbnorEventBits;
    return( XJ_SUCCESS );
}

/* ���ø澯�������е�״̬���� */
INT PMI_SetAlarmANDRunLedStatus( VOID )
{
    UNSIGNED ActualEvent;
    UNSIGNED uEvent;
    /* �����е��쳣�¼�λ���澯�¼�λ��� */
    uEvent = ( PmiLed.PlatAlarmLedBits | PmiLed.RunLedAbnorEventBits );
    /* װ�ù��ϵ�澯�� */
    if( XJ_Retrieve_Events(& Event_1_CB, uEvent , XJ_OR,
        & ActualEvent, XJ_NO_SUSPEND ) == XJ_SUCCESS )
    {
        /* ����װ�����е��쳣״̬ */
        if( ActualEvent & ( PmiLed.RunLedAbnorEventBits ) )
        {
            XJP_Led_Change_Status( PlatFormRunLedHandle, XJP_LED_ABNORMAL );
        }
        else
        {
            XJP_Led_Change_Status( PlatFormRunLedHandle, XJP_LED_NORMAL );
        }
        /* ���ø澯��״̬ */
        if( ActualEvent & ( PmiLed.PlatAlarmLedBits ) )
        {
            DEVI_WriteAlarmLed(0x01); /* �澯���� */
        }
        else
        {
            DEVI_WriteAlarmLed(0x00); /* �澯���� */
        }
    }
    else
    {
        /* ����װ�����е�״̬ */
        XJP_Led_Change_Status( PlatFormRunLedHandle, XJP_LED_NORMAL );
        DEVI_WriteAlarmLed(0x00); /* �澯���� */
    }
    return( XJ_SUCCESS );
}
