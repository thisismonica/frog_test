
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
/*      pmi_cirmem.c                                    Ver  1.0         */
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
#include "plat_specific/plat_specific.h"
#include "plat/inc/esp_cfg.h"
#include "plat/net_inc/rcc_defs.h"
#include "plat/net_inc/ied_defs.h"
#include "plat/net_inc/ied_extr.h"
#include "plat/es_plat.h"

/* Define external inner-component global data references.  */

extern ESP_CFG PLAT_cfg;

HDCM RamEventHandle;      /* RAM���¼������� */
HDCM NVRamEventHandle;    /* NVRAM���¼������� */
HDCM RamReportHandle;     /* RAM����������ѭ�������������� */
HDCM CommEventHandle;     /* ͨѶ���¼������� */
HDCM CommFltRetHandle;    /* ͨѶ�й��Ϸ��ع����� */
HDCM FckPrimarySOEHandle; /* ���SOE������(һ������) */
HDCM FckSecondSOEHandle;  /* ���SOE������(��������) */
HDCM FckPrimaryCommandHandle; /* ����û����������(һ������) */
HDCM FckSecondCommandHandle;  /* ����û����������(��������) */
HDCM NPIUserDataHandle;       /* ����NPI�û����ݹ����� */

extern HDCM IEDDB_FckSoeHandle[]; /* FckSoe���ݾ�� */
extern HDCM IEDDB_FckCommandHandle[]; /* FckCommand��� */

/* ����ΪOS������� */
#ifdef WIN32
#endif

#ifndef WIN32 /* ��windowsȱʡ����һЩ���Ա�ͨ������ */
#endif

/* Define internal function calls.  */

/* ��ʼ��ѭ�������� */
INT PMI_InitializeCircleMemory( VOID )
{
    UNSIGNED OneDataSize;
    UNSIGNED TotalDataSize;

    /* ��ʼ���¼�ѭ�������� */
    OneDataSize = ( sizeof( CEM_RAD )+ sizeof( UNSIGNED ) - 1 )/ sizeof( UNSIGNED );
    TotalDataSize = ( PLAT_cfg.history.TempEventNumber ) * OneDataSize;
    RamEventHandle = XJP_Queue_Create( TotalDataSize, XJP_EVENT_FIXED_SIZE, OneDataSize );
    NVRamEventHandle = XJP_Queue_Create( TotalDataSize, XJP_EVENT_FIXED_SIZE, OneDataSize );

    /* ��ʼ����������ѭ�������� */
    OneDataSize = sizeof( CRM_RFD );
    TotalDataSize = ( PLAT_cfg.history.TempReportNumber ) * OneDataSize;
    RamReportHandle = XJP_Queue_Create( TotalDataSize, XJP_EVENT_VARIABLE_SIZE, OneDataSize );

    /* ��ʼ����λ�¼�ѭ�������� */
    TotalDataSize = ONE_COMM_ESIZE() * MAX_CHECK_INFO;
    CommEventHandle = XJP_Queue_Create( TotalDataSize, XJP_EVENT_FIXED_SIZE, ONE_COMM_ESIZE());
    TotalDataSize = ONE_COMM_RSIZE() * MAX_FAULT_RETURN;
    CommFltRetHandle = XJP_Queue_Create( TotalDataSize, XJP_EVENT_VARIABLE_SIZE, ONE_COMM_RSIZE());

    /* ��ʼ�����SOEѭ��������(һ������) */
    OneDataSize = IED_FCK_SOE_BUFFER_LENGTH;
    TotalDataSize = IED_FCK_SOE_MAX_COUNT * OneDataSize;
    FckPrimarySOEHandle = XJP_Queue_Create( TotalDataSize, XJP_EVENT_VARIABLE_SIZE, OneDataSize );
    /* ��ʼ�����SOEѭ��������(��������) */
    FckSecondSOEHandle = XJP_Queue_Create( TotalDataSize, XJP_EVENT_VARIABLE_SIZE, OneDataSize );
    IEDDB_FckSoeHandle[0] = FckPrimarySOEHandle; /* ���SOEѭ��������(һ������) */
    IEDDB_FckSoeHandle[1] = FckSecondSOEHandle; /* ���SOEѭ��������(��������) */

    /* ����û�����ѭ��������(һ������) */
    OneDataSize = IED_READ_COMMAND_BUFFER_LENGTH;
    TotalDataSize = IED_READ_COMMAND_MAX_COUNT * OneDataSize;
    FckPrimaryCommandHandle = XJP_Queue_Create( TotalDataSize, XJP_EVENT_VARIABLE_SIZE, OneDataSize );
    /* ����û�����ѭ��������(��������) */
    FckSecondCommandHandle = XJP_Queue_Create( TotalDataSize, XJP_EVENT_VARIABLE_SIZE, OneDataSize );
    IEDDB_FckCommandHandle[0] = FckPrimaryCommandHandle; /* ����û�����ѭ��������(һ������) */
    IEDDB_FckCommandHandle[1] = FckSecondCommandHandle;  /* ����û�����ѭ��������(��������) */
    /* ���ȼ����ʼ������ */
    iedDB_ProirityInit();
    /* ����NPI�û����ݹ����� */
    OneDataSize = CPC_MAX_LENGTH_FRAME;
    TotalDataSize = CPC_MAX_USER_FRAME_NUMBER * OneDataSize;
    NPIUserDataHandle = XJP_Queue_Create( TotalDataSize, XJP_EVENT_VARIABLE_SIZE, OneDataSize );

    return( XJ_SUCCESS );
}
