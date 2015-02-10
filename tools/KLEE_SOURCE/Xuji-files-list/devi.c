
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
/*      devi.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      DEVI - Device Interface functions                                */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the device interface    */
/*      functions.                                                       */
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
/*      ZhiDeYang       11-28-2009      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "plat/inc/xj_cosc.h"
#include "plat/inc/cdev_extr.h"
#include "plat/inc/dpf_extr.h"
#include "plat/inc/devi_extr.h"
#include "plat/inc/xj_clog.h"
#include "plat/es_plat.h"

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

static VOID * DEVI_hInput = XJ_NULL;
static VOID * DEVI_hOutput = XJ_NULL;
static VOID * DEVI_hWatchdog = XJ_NULL;
static VOID * DEVI_hRunLed = XJ_NULL;
static VOID * DEVI_hAlarmLed = XJ_NULL;
static VOID * DEVI_hUserLed = XJ_NULL;
static VOID * DEVI_hStartRelay = XJ_NULL;
static VOID * DEVI_hGPS_Pulse = XJ_NULL;
static VOID * DEVI_hGPS_IRIGB = XJ_NULL;
static VOID * DEVI_hVersion = XJ_NULL;
static VOID * DEVI_hCPU = XJ_NULL;
static VOID * DEVI_hPerTimer = XJ_NULL;
static VOID * DEVI_hAD = XJ_NULL;
static VOID * DEVI_hSamplePulse = XJ_NULL;
static VOID * DEVI_hADGain = XJ_NULL;
static VOID * DEVI_hKey = XJ_NULL;
static VOID * DEVI_hTimeChip = XJ_NULL;
static VOID * DEVI_hTemperature = XJ_NULL;
static VOID * DEVI_hSW = XJ_NULL;
static VOID * DEVI_hIDC = XJ_NULL;
static VOID * DEVI_hOutReadback = XJ_NULL;
static VOID * DEVI_hPlugReadback = XJ_NULL;
static VOID * DEVI_hOrtCodeDir = XJ_NULL;
static VOID * DEVI_hOrtCodeValue = XJ_NULL;
static VOID * DEVI_hPPS = XJ_NULL;
static VOID * DEVI_hPPSStamp = XJ_NULL;
static VOID * DEVI_hUARTStamp = XJ_NULL;
static VOID * DEVI_hMotorStatus = XJ_NULL;
static VOID * DEVI_hMotorCmd = XJ_NULL;
static VOID * DEVI_hMotorPulse = XJ_NULL;
static VOID * DEVI_hSolenoidValveCmd = XJ_NULL;
static VOID * DEVI_hFNRam = XJ_NULL;

UNSIGNED DEVI_period_time = 1000; /* ���ڼ����ʱ�����������ڣ�΢�룩*/

/* ��ʼ���豸 */
INT DEVI_initialize( VOID )
{
    int status;
    int number;
    void * pointer_list;
    UNSIGNED id_code;
    int devId_mdEnable_IO;
    int devId_mdEnable_WDT;
    int devId_mdEnable_LED;
    int devId_mdEnable_START_RELAY;
    int devId_mdEnable_GPS;
    int devId_mdEnable_VER;
    int devId_mdEnable_CPU;
    int devId_mdEnable_AD;
    int devId_mdEnable_KEY;
    int devId_mdEnable_TIME;
    int devId_mdEnable_TEMPERATURE;
    int devId_mdEnable_SW;
    int devId_mdEnable_OR;
    int devId_mdEnable_OCOD;
    int devId_mdEnable_UART;
    int devId_mdEnable_PPS;
    int devId_mdEnable_MOTOR;
    int devId_mdEnable_SOV;
    //
    int devId_GPS_Pulse_Enable;
    int devId_GPS_Irigb_Enable;
    int devId_IO;
    int devId_StartRelay;
    int devId_Key;
    int devId_SW;
    int devId_AD;
    int devId_CPU;
    int devId_Led;
    int devId_OPR;
    int devId_OCD;
    int devId_SOV;
    int devId_GPS;

    /* ��ȡ����������ָ�� */
    number = DPFC_DriverPointer( & pointer_list );
    /* ��ʼ���豸�������� */
    XJP_DRV_Initialize( number, (( void **)pointer_list ));

    /* ��ȡӲ����ʶ */
    XJP_DRV_mdBindDevice( CDEV_NAME_IDC, 0 );         /* ��Ӳ����ʶ�豸 */
    DEVI_hIDC = XJP_DRV_gioCreate( CDEV_NAME_IDC_A, XJP_DRV_MODE_INPUT, & status, XJ_NULL ); /* ����Ӳ����ʶͨ�� */
    id_code = DEVI_GetIdentificationCode(); /* ��ȡӲ����ʶ�� */
    switch( id_code )
    {
    case XJ_IDC_NPU808:
        /* NPU808�豸IDѡ�� */
        devId_IO = 0;
        devId_StartRelay = 0;
        devId_AD = 0;
        devId_Key = 0;
        devId_SW = 0;
        devId_CPU = 0;
        devId_Led = 0;
        devId_OPR = 0;
        devId_OCD = 0;
        devId_SOV = 0;
        devId_GPS = 0;
        /* NPU808���豸ʹ�� */
        devId_mdEnable_IO = XJ_TRUE;
        devId_mdEnable_WDT = XJ_TRUE;
        devId_mdEnable_LED = XJ_TRUE;
        devId_mdEnable_START_RELAY = XJ_TRUE;
        devId_mdEnable_GPS = XJ_TRUE;
        devId_mdEnable_VER = XJ_TRUE;
        devId_mdEnable_CPU = XJ_TRUE;
        devId_mdEnable_AD = XJ_TRUE;
        devId_mdEnable_KEY = XJ_TRUE;
        devId_mdEnable_TIME = XJ_TRUE;
        devId_mdEnable_TEMPERATURE = XJ_TRUE;
        devId_mdEnable_SW = XJ_TRUE;
        devId_mdEnable_OR = XJ_TRUE;
        devId_mdEnable_OCOD = XJ_TRUE;
        devId_mdEnable_UART = XJ_TRUE;
        devId_mdEnable_PPS = XJ_FALSE;
        devId_mdEnable_MOTOR = XJ_FALSE;
        devId_mdEnable_SOV = XJ_FALSE;
        /* NPU808�豸ͨ��ʹ�� */
        devId_GPS_Pulse_Enable = XJ_TRUE;
        devId_GPS_Irigb_Enable = XJ_FALSE;
        break;
    case XJ_IDC_NPU816:
        /* NPU816�豸IDѡ�� */
        devId_IO = 1;
        devId_StartRelay = 1;
        devId_AD = 0;
        devId_Key = 1;
        devId_SW = 1;
        devId_CPU = 0;
        devId_Led = 0;
        devId_OPR = 1;
        devId_OCD = 0;
        devId_SOV = 0;
        devId_GPS = 0;
        /* NPU816���豸ʹ�� */
        devId_mdEnable_IO = XJ_TRUE;
        devId_mdEnable_WDT = XJ_TRUE;
        devId_mdEnable_LED = XJ_TRUE;
        devId_mdEnable_START_RELAY = XJ_TRUE;
        devId_mdEnable_GPS = XJ_TRUE;
        devId_mdEnable_VER = XJ_TRUE;
        devId_mdEnable_CPU = XJ_TRUE;
        devId_mdEnable_AD = XJ_TRUE;
        devId_mdEnable_KEY = XJ_TRUE;
        devId_mdEnable_TIME = XJ_TRUE;
        devId_mdEnable_TEMPERATURE = XJ_TRUE;
        devId_mdEnable_SW = XJ_TRUE;
        devId_mdEnable_OR = XJ_TRUE;
        devId_mdEnable_OCOD = XJ_TRUE;
        devId_mdEnable_UART = XJ_TRUE;
        devId_mdEnable_PPS = XJ_FALSE;
        devId_mdEnable_MOTOR = XJ_FALSE;
        devId_mdEnable_SOV = XJ_FALSE;
        /* NPU816ʹ���豸ѡ�� */
        devId_GPS_Pulse_Enable = XJ_TRUE;
        devId_GPS_Irigb_Enable = XJ_FALSE;
        break;
    case XJ_IDC_NPU823:
        /* NPU823�豸IDѡ�� */
        devId_IO = 3; /* �豸�� */
        devId_StartRelay = 1;
        devId_AD = 2; /* A/D�豸�� */
        devId_Key = 1;
        devId_SW = 1;
        devId_CPU = 0;
        devId_Led = 0;
        devId_OPR = 1;
        devId_OCD = 0;
        devId_SOV = 0;
        devId_GPS = 0;
        /* NPU823���豸ʹ�� */
        devId_mdEnable_IO = XJ_TRUE;
        devId_mdEnable_WDT = XJ_TRUE;
        devId_mdEnable_LED = XJ_TRUE;
        devId_mdEnable_START_RELAY = XJ_TRUE;
        devId_mdEnable_GPS = XJ_TRUE;
        devId_mdEnable_VER = XJ_TRUE;
        devId_mdEnable_CPU = XJ_TRUE;
        devId_mdEnable_AD = XJ_TRUE;
        devId_mdEnable_KEY = XJ_TRUE;
        devId_mdEnable_TIME = XJ_TRUE;
        devId_mdEnable_TEMPERATURE = XJ_TRUE;
        devId_mdEnable_SW = XJ_TRUE;
        devId_mdEnable_OR = XJ_TRUE;
        devId_mdEnable_OCOD = XJ_TRUE;
        devId_mdEnable_UART = XJ_TRUE;
        devId_mdEnable_PPS = XJ_TRUE;
        devId_mdEnable_MOTOR = XJ_FALSE;
        devId_mdEnable_SOV = XJ_FALSE;
        /* NPU823ʹ���豸ѡ�� */
        devId_GPS_Pulse_Enable = XJ_TRUE;
        devId_GPS_Irigb_Enable = XJ_FALSE;
        break;
    case XJ_IDC_NPU824:
    case XJ_IDC_NPU8104:
        /* NPU824�豸IDѡ�� */
        devId_IO = 8;
        devId_StartRelay = 0;
        devId_AD = 0;
        devId_Key = 0;
        devId_SW = 0;
        devId_CPU = 0;
        devId_Led = 0;
        devId_OPR = 0;
        devId_OCD = 0;
        devId_SOV = 0;
        devId_GPS = 0;
        /* NPU824���豸ʹ�� */
        devId_mdEnable_IO = XJ_TRUE;
        devId_mdEnable_WDT = XJ_TRUE;
        devId_mdEnable_LED = XJ_TRUE;
        devId_mdEnable_START_RELAY = XJ_FALSE;
        devId_mdEnable_GPS = XJ_FALSE;
        devId_mdEnable_VER = XJ_FALSE;
        devId_mdEnable_CPU = XJ_TRUE;
        devId_mdEnable_AD = XJ_FALSE;
        devId_mdEnable_KEY = XJ_FALSE;
        devId_mdEnable_TIME = XJ_TRUE;
        devId_mdEnable_TEMPERATURE = XJ_TRUE;
        devId_mdEnable_SW = XJ_FALSE;
        devId_mdEnable_OR = XJ_FALSE;
        devId_mdEnable_OCOD = XJ_FALSE;
        devId_mdEnable_UART = XJ_TRUE;
        devId_mdEnable_PPS = XJ_FALSE;
        devId_mdEnable_MOTOR = XJ_FALSE;
        devId_mdEnable_SOV = XJ_FALSE;
        /* NPU824�豸ͨ��ʹ�� */
        devId_GPS_Pulse_Enable = XJ_FALSE;
        devId_GPS_Irigb_Enable = XJ_FALSE;
        break;
    case XJ_IDC_NPU8507:
        /* NPU824�豸IDѡ�� */
        devId_IO = 9;
        devId_StartRelay = 0;
        devId_AD = 0;
        devId_Key = 0;
        devId_SW = 0;
        devId_CPU = 0;
        devId_Led = 0;
        devId_OPR = 0;
        devId_OCD = 0;
        devId_SOV = 0;
        devId_GPS = 2;
        /* NPU8507���豸ʹ�� */
        devId_mdEnable_IO = XJ_TRUE;
        devId_mdEnable_WDT = XJ_TRUE;
        devId_mdEnable_LED = XJ_TRUE;
        devId_mdEnable_START_RELAY = XJ_FALSE;
        devId_mdEnable_GPS = XJ_TRUE;
        devId_mdEnable_VER = XJ_FALSE;
        devId_mdEnable_CPU = XJ_TRUE;
        devId_mdEnable_AD = XJ_FALSE;
        devId_mdEnable_KEY = XJ_FALSE;
        devId_mdEnable_TIME = XJ_TRUE;
        devId_mdEnable_TEMPERATURE = XJ_TRUE;
        devId_mdEnable_SW = XJ_FALSE;
        devId_mdEnable_OR = XJ_FALSE;
        devId_mdEnable_OCOD = XJ_FALSE;
        devId_mdEnable_UART = XJ_TRUE;
        devId_mdEnable_PPS = XJ_FALSE;
        devId_mdEnable_MOTOR = XJ_FALSE;
        devId_mdEnable_SOV = XJ_FALSE;
        /* NPU824�豸ͨ��ʹ�� */
        devId_GPS_Pulse_Enable = XJ_TRUE;
        devId_GPS_Irigb_Enable = XJ_FALSE;
        break;
    case XJ_IDC_NPU8509:
        /* NPU8509�豸IDѡ�� */
        devId_IO = 10;
        devId_StartRelay = 0;
        devId_AD = 0;
        devId_Key = 0;
        devId_SW = 0;
        devId_CPU = 0;
        devId_Led = 0;
        devId_OPR = 0;
        devId_OCD = 0;
        devId_SOV = 0;
        devId_GPS = 2;
        /* NPU8509���豸ʹ�� */
        devId_mdEnable_IO = XJ_TRUE;
        devId_mdEnable_WDT = XJ_TRUE;
        devId_mdEnable_LED = XJ_TRUE;
        devId_mdEnable_START_RELAY = XJ_FALSE;
        devId_mdEnable_GPS = XJ_TRUE;
        devId_mdEnable_VER = XJ_FALSE;
        devId_mdEnable_CPU = XJ_TRUE;
        devId_mdEnable_AD = XJ_FALSE;
        devId_mdEnable_KEY = XJ_FALSE;
        devId_mdEnable_TIME = XJ_TRUE;
        devId_mdEnable_TEMPERATURE = XJ_TRUE;
        devId_mdEnable_SW = XJ_FALSE;
        devId_mdEnable_OR = XJ_FALSE;
        devId_mdEnable_OCOD = XJ_FALSE;
        devId_mdEnable_UART = XJ_TRUE;
        devId_mdEnable_PPS = XJ_FALSE;
        devId_mdEnable_MOTOR = XJ_FALSE;
        devId_mdEnable_SOV = XJ_FALSE;
        /* NPU8509�豸ͨ��ʹ�� */
        devId_GPS_Pulse_Enable = XJ_TRUE;
        devId_GPS_Irigb_Enable = XJ_FALSE;
        break;
    case XJ_IDC_NPU827:
        /* NPU827�豸IDѡ�� */
        devId_IO = 6; /* I/O�豸�� */
        devId_StartRelay = 0;
        devId_AD = 5; /* AD�豸�� */
        devId_Key = 0;
        devId_SW = 0;
        devId_CPU = 1;
        devId_Led = 2; /* ���豸�� */
        devId_OPR = 0;
        devId_OCD = 0;
        devId_SOV = 0;
        devId_GPS = 0;
        /* NPU827���豸ʹ�� */
        devId_mdEnable_IO = XJ_TRUE;
        devId_mdEnable_WDT = XJ_TRUE;
        devId_mdEnable_LED = XJ_TRUE;
        devId_mdEnable_START_RELAY = XJ_FALSE;
        devId_mdEnable_GPS = XJ_FALSE;
        devId_mdEnable_VER = XJ_TRUE;
        devId_mdEnable_CPU = XJ_TRUE;
        devId_mdEnable_AD = XJ_TRUE;
        devId_mdEnable_KEY = XJ_FALSE;
        devId_mdEnable_TIME = XJ_TRUE;
        devId_mdEnable_TEMPERATURE = XJ_TRUE;
        devId_mdEnable_SW = XJ_FALSE;
        devId_mdEnable_OR = XJ_FALSE;
        devId_mdEnable_OCOD = XJ_FALSE;
        devId_mdEnable_UART = XJ_TRUE;
        devId_mdEnable_PPS = XJ_FALSE;
        devId_mdEnable_MOTOR = XJ_TRUE;
        devId_mdEnable_SOV = XJ_TRUE;
        /* NPU827�豸ͨ��ʹ�� */
        devId_GPS_Pulse_Enable = XJ_FALSE;
        devId_GPS_Irigb_Enable = XJ_FALSE;
        break;
    case XJ_IDC_NPU8100:
        /* NPU8100�豸IDѡ�� */
        devId_IO = 2; /* I/O�豸�� */
        devId_StartRelay = 2; /* �����̵����豸�� */
        devId_AD = 1;
        devId_Key = 2; /* �����豸�� */
        devId_SW = 1;
        devId_CPU = 1;
        devId_Led = 1;
        devId_OPR = 2; /* �����ض��豸�� */
        devId_OCD = 1;
        devId_SOV = 0;
        devId_GPS = 0;
        /* NPU8100���豸ʹ�� */
        devId_mdEnable_IO = XJ_TRUE;
        devId_mdEnable_WDT = XJ_TRUE;
        devId_mdEnable_LED = XJ_TRUE;
        devId_mdEnable_START_RELAY = XJ_TRUE;
        devId_mdEnable_GPS = XJ_TRUE;
        devId_mdEnable_VER = XJ_TRUE;
        devId_mdEnable_CPU = XJ_TRUE;
        devId_mdEnable_AD = XJ_TRUE;
        devId_mdEnable_KEY = XJ_TRUE;
        devId_mdEnable_TIME = XJ_TRUE;
        devId_mdEnable_TEMPERATURE = XJ_TRUE;
        devId_mdEnable_SW = XJ_TRUE;
        devId_mdEnable_OR = XJ_TRUE;
        devId_mdEnable_OCOD = XJ_TRUE;
        devId_mdEnable_UART = XJ_TRUE;
        devId_mdEnable_PPS = XJ_FALSE;
        devId_mdEnable_MOTOR = XJ_FALSE;
        devId_mdEnable_SOV = XJ_FALSE;
        /* NPU8100ʹ���豸ѡ�� */
        devId_GPS_Pulse_Enable = XJ_FALSE;
        devId_GPS_Irigb_Enable = XJ_TRUE;
        break;
    case XJ_IDC_NPU8101:
        /* NPU8101�豸IDѡ�� */
        devId_IO = 4; /* I/O�豸�� */
        devId_StartRelay = 6;   /* �������̵��� */
        devId_AD = 3; /* AD�豸�� */
        devId_Key = 2; /* �����豸�� */
        devId_SW = 1;
        devId_CPU = 1;
        devId_Led = 1;
        devId_OPR = 6;          /* ����չ������� */
        devId_OCD = 0;
        devId_SOV = 0;
        devId_GPS = 0;
        /* NPU8101���豸ʹ�� */
        devId_mdEnable_IO = XJ_TRUE;
        devId_mdEnable_WDT = XJ_TRUE;
        devId_mdEnable_LED = XJ_TRUE;
        devId_mdEnable_START_RELAY = XJ_TRUE;
        devId_mdEnable_GPS = XJ_TRUE;
        devId_mdEnable_VER = XJ_TRUE;
        devId_mdEnable_CPU = XJ_TRUE;
        devId_mdEnable_AD = XJ_TRUE;
        devId_mdEnable_KEY = XJ_TRUE;
        devId_mdEnable_TIME = XJ_TRUE;
        devId_mdEnable_TEMPERATURE = XJ_TRUE;
        devId_mdEnable_SW = XJ_TRUE;
        devId_mdEnable_OR = XJ_TRUE;
        devId_mdEnable_OCOD = XJ_TRUE;
        devId_mdEnable_UART = XJ_TRUE;
        devId_mdEnable_PPS = XJ_FALSE;
        devId_mdEnable_MOTOR = XJ_FALSE;
        devId_mdEnable_SOV = XJ_FALSE;
        /* NPU8101ʹ���豸ѡ�� */
        devId_GPS_Pulse_Enable = XJ_FALSE;
        devId_GPS_Irigb_Enable = XJ_TRUE;
        break;
    case XJ_IDC_NPU8103:
        /* NPU8103�豸IDѡ�� */
        devId_IO = 5; /* I/O�豸�� */
        devId_StartRelay = 3; /* �����̵����豸�� */
        devId_AD = 4; /* A/D�豸�� */
        devId_Key = 3; /* �����豸�� */
        devId_SW = 0;
        devId_CPU = 0;
        devId_Led = 0;
        devId_OPR = 4;
        devId_OCD = 0;
        devId_SOV = 0;
        devId_GPS = 0;
        /* NPU8103���豸ʹ�� */
        devId_mdEnable_IO = XJ_TRUE;
        devId_mdEnable_WDT = XJ_TRUE;
        devId_mdEnable_LED = XJ_TRUE;
        devId_mdEnable_START_RELAY = XJ_TRUE;
        devId_mdEnable_GPS = XJ_TRUE;
        devId_mdEnable_VER = XJ_TRUE;
        devId_mdEnable_CPU = XJ_TRUE;
        devId_mdEnable_AD = XJ_TRUE;
        devId_mdEnable_KEY = XJ_TRUE;
        devId_mdEnable_TIME = XJ_TRUE;
        devId_mdEnable_TEMPERATURE = XJ_TRUE;
        devId_mdEnable_SW = XJ_FALSE;
        devId_mdEnable_OR = XJ_TRUE;
        devId_mdEnable_OCOD = XJ_FALSE;
        devId_mdEnable_UART = XJ_TRUE;
        devId_mdEnable_PPS = XJ_TRUE;
        devId_mdEnable_MOTOR = XJ_FALSE;
        devId_mdEnable_SOV = XJ_FALSE;
        /* NPU8103ʹ���豸ѡ�� */
        devId_GPS_Pulse_Enable = XJ_FALSE;
        devId_GPS_Irigb_Enable = XJ_TRUE;
        break;
    case XJ_IDC_NPI807:
        /* NPI807�豸IDѡ�� */
        devId_IO = 0;
        devId_StartRelay = 0;
        devId_AD = 0;
        devId_Key = 0;
        devId_SW = 0;
        devId_CPU = 0;
        devId_Led = 0;
        devId_OPR = 0;
        devId_OCD = 0;
        devId_SOV = 0;
        devId_GPS = 0;
        /* NPI807���豸ʹ�� */
        devId_mdEnable_IO = XJ_TRUE;
        devId_mdEnable_WDT = XJ_TRUE;
        devId_mdEnable_LED = XJ_TRUE;
        devId_mdEnable_START_RELAY = XJ_TRUE;
        devId_mdEnable_GPS = XJ_TRUE;
        devId_mdEnable_VER = XJ_TRUE;
        devId_mdEnable_CPU = XJ_TRUE;
        devId_mdEnable_AD = XJ_TRUE;
        devId_mdEnable_KEY = XJ_TRUE;
        devId_mdEnable_TIME = XJ_TRUE;
        devId_mdEnable_TEMPERATURE = XJ_TRUE;
        devId_mdEnable_SW = XJ_TRUE;
        devId_mdEnable_OR = XJ_TRUE;
        devId_mdEnable_OCOD = XJ_TRUE;
        devId_mdEnable_UART = XJ_TRUE;
        devId_mdEnable_PPS = XJ_FALSE;
        devId_mdEnable_MOTOR = XJ_FALSE;
        devId_mdEnable_SOV = XJ_FALSE;
        /* NPI807ʹ���豸 */
        devId_GPS_Pulse_Enable = XJ_FALSE;
        devId_GPS_Irigb_Enable = XJ_FALSE;
        break;
    case XJ_IDC_DTS568:
        /* DTS568�豸IDѡ�� */
        devId_IO = 0;
        devId_StartRelay = 0;
        devId_AD = 0;
        devId_Key = 0;
        devId_SW = 0;
        devId_CPU = 0;
        devId_Led = 0;
        devId_OPR = 0;
        devId_OCD = 0;
        devId_SOV = 0;
        devId_GPS = 0;
        /* DTS568���豸ʹ�� */
        devId_mdEnable_IO = XJ_FALSE;
        devId_mdEnable_WDT = XJ_TRUE;
        devId_mdEnable_LED = XJ_FALSE;
        devId_mdEnable_START_RELAY = XJ_FALSE;
        devId_mdEnable_GPS = XJ_FALSE;
        devId_mdEnable_VER = XJ_FALSE;
        devId_mdEnable_CPU = XJ_TRUE;
        devId_mdEnable_AD = XJ_FALSE;
        devId_mdEnable_KEY = XJ_FALSE;
        devId_mdEnable_TIME = XJ_FALSE;
        devId_mdEnable_TEMPERATURE = XJ_TRUE;
        devId_mdEnable_SW = XJ_FALSE;
        devId_mdEnable_OR = XJ_FALSE;
        devId_mdEnable_OCOD = XJ_FALSE;
        devId_mdEnable_UART = XJ_FALSE;
        devId_mdEnable_PPS = XJ_FALSE;
        devId_mdEnable_MOTOR = XJ_FALSE;
        devId_mdEnable_SOV = XJ_FALSE;
        /* DTS568ʹ���豸 */
        devId_GPS_Pulse_Enable = XJ_FALSE;
        devId_GPS_Irigb_Enable = XJ_FALSE;
        break;
    case XJ_IDC_NPU826:
        /* NPU826�豸IDѡ�� */
        devId_IO = 7;
        devId_StartRelay = 4;
        devId_AD = 6;
        devId_Key = 1;
        devId_SW = 1;
        devId_CPU = 0;
        devId_Led = 0;
        devId_OPR = 3;
        devId_OCD = 0;
        devId_SOV = 0;
        devId_GPS = 1;
        /* NPU826���豸ʹ�� */
        devId_mdEnable_IO = XJ_TRUE;
        devId_mdEnable_WDT = XJ_TRUE;
        devId_mdEnable_LED = XJ_TRUE;
        devId_mdEnable_START_RELAY = XJ_TRUE;
        devId_mdEnable_GPS = XJ_TRUE;
        devId_mdEnable_VER = XJ_TRUE;
        devId_mdEnable_CPU = XJ_TRUE;
        devId_mdEnable_AD = XJ_TRUE;
        devId_mdEnable_KEY = XJ_TRUE;
        devId_mdEnable_TIME = XJ_TRUE;
        devId_mdEnable_TEMPERATURE = XJ_FALSE;
        devId_mdEnable_SW = XJ_FALSE;
        devId_mdEnable_OR = XJ_TRUE;
        devId_mdEnable_OCOD = XJ_FALSE;
        devId_mdEnable_UART = XJ_FALSE;
        devId_mdEnable_PPS = XJ_FALSE;
        devId_mdEnable_MOTOR = XJ_FALSE;
        devId_mdEnable_SOV = XJ_FALSE;
        /* NPU826ʹ���豸 */
        devId_GPS_Pulse_Enable = XJ_FALSE;
        devId_GPS_Irigb_Enable = XJ_TRUE;
        break;
    default:
        /* �豸IDѡ�� */
        devId_IO = 0;
        devId_StartRelay = 0;
        devId_AD = 0;
        devId_Key = 0;
        devId_SW = 0;
        devId_CPU = 0;
        devId_Led = 0;
        devId_OPR = 0;
        devId_OCD = 0;
        devId_SOV = 0;
        devId_GPS = 0;
        /* ���豸ʹ�� */
        devId_mdEnable_IO = XJ_TRUE;
        devId_mdEnable_WDT = XJ_TRUE;
        devId_mdEnable_LED = XJ_TRUE;
        devId_mdEnable_START_RELAY = XJ_TRUE;
        devId_mdEnable_GPS = XJ_TRUE;
        devId_mdEnable_VER = XJ_TRUE;
        devId_mdEnable_CPU = XJ_TRUE;
        devId_mdEnable_AD = XJ_TRUE;
        devId_mdEnable_KEY = XJ_TRUE;
        devId_mdEnable_TIME = XJ_TRUE;
        devId_mdEnable_TEMPERATURE = XJ_TRUE;
        devId_mdEnable_SW = XJ_TRUE;
        devId_mdEnable_OR = XJ_TRUE;
        devId_mdEnable_OCOD = XJ_TRUE;
        devId_mdEnable_UART = XJ_TRUE;
        devId_mdEnable_PPS = XJ_FALSE;
        devId_mdEnable_MOTOR = XJ_FALSE;
        devId_mdEnable_SOV = XJ_FALSE;
        /* ʹ���豸 */
        devId_GPS_Pulse_Enable = XJ_TRUE;
        devId_GPS_Irigb_Enable = XJ_FALSE;
        break;
    }

    /* ���豸 */
    if( devId_mdEnable_IO == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_IO, devId_IO );     /* I/O */
    if( devId_mdEnable_WDT == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_WDT, 0 );           /* ���Ź� */
    if( devId_mdEnable_LED == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_LED, devId_Led );   /* ���� */
    if( devId_mdEnable_START_RELAY == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_START_RELAY, devId_StartRelay ); /* �̵��� */
    if( devId_mdEnable_GPS == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_GPS, devId_GPS );   /* GPS */
    if( devId_mdEnable_VER == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_VER, 0 );           /* CPLD�汾 */
    if( devId_mdEnable_CPU == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_CPU, devId_CPU );   /* CPU��ز��� */
    if( devId_mdEnable_AD == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_AD, devId_AD );     /* A/D */
    if( devId_mdEnable_KEY == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_KEY, devId_Key );   /* ��ֵ */
    if( devId_mdEnable_TIME == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_TIME, 0 );          /* ʱ��оƬ */
    if( devId_mdEnable_TEMPERATURE == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_TEMPERATURE, 0 );   /* �¶ȴ�����оƬ */
    if( devId_mdEnable_SW == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_SW, devId_SW );     /* SW��� */
    if( devId_mdEnable_OR == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_OR, devId_OPR );    /* ������������ض� */
    if( devId_mdEnable_OCOD == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_OCODE, devId_OCD ); /* �������� */
    if( devId_mdEnable_UART == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_UART, 0 );          /* UART */
    if( devId_mdEnable_PPS == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_PPS, 0 );           /* PPS */
    if( devId_mdEnable_MOTOR == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_MOTOR, 0 );         /* ������� */
    if( devId_mdEnable_SOV == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_SOV, 0 );           /* ��ŷ� */

    /* ����ͨ���豸 */
    DEVI_hInput = XJP_DRV_gioCreate( CDEV_NAME_IO_A, XJP_DRV_MODE_INPUT, & status, XJ_NULL );
    DEVI_hOutput = XJP_DRV_gioCreate( CDEV_NAME_IO_B, XJP_DRV_MODE_OUTPUT, & status, XJ_NULL );
    DEVI_hWatchdog = XJP_DRV_gioCreate( CDEV_NAME_WDT_A, XJP_DRV_MODE_OUTPUT, & status, XJ_NULL );
    DEVI_hRunLed = XJP_DRV_gioCreate( CDEV_NAME_LED_A, XJP_DRV_MODE_OUTPUT, & status, XJ_NULL );
    DEVI_hAlarmLed = XJP_DRV_gioCreate( CDEV_NAME_LED_B, XJP_DRV_MODE_OUTPUT, & status, XJ_NULL );
    DEVI_hUserLed = XJP_DRV_gioCreate( CDEV_NAME_LED_C, XJP_DRV_MODE_OUTPUT, & status, XJ_NULL );
    DEVI_hStartRelay = XJP_DRV_gioCreate( CDEV_NAME_START_RELAY_A, XJP_DRV_MODE_OUTPUT, & status, XJ_NULL );
    if( devId_GPS_Pulse_Enable == XJ_TRUE )
        DEVI_hGPS_Pulse = XJP_DRV_gioCreate( CDEV_NAME_GPS_A, XJP_DRV_MODE_INPUT, & status, XJ_NULL );
    if( devId_GPS_Irigb_Enable == XJ_TRUE )
        DEVI_hGPS_IRIGB = XJP_DRV_gioCreate( CDEV_NAME_GPS_B, XJP_DRV_MODE_INPUT, & status, XJ_NULL );
    DEVI_hVersion = XJP_DRV_gioCreate( CDEV_NAME_VER_A, XJP_DRV_MODE_INPUT, & status, XJ_NULL );
    DEVI_hCPU = XJP_DRV_gioCreate( CDEV_NAME_CPU_A, XJP_DRV_MODE_OUTPUT, & status, XJ_NULL );
    DEVI_hPerTimer = XJP_DRV_gioCreate( CDEV_NAME_CPU_B, XJP_DRV_MODE_OUTPUT, & status, XJ_NULL );
    DEVI_hSamplePulse = XJP_DRV_gioCreate( CDEV_NAME_AD_A, XJP_DRV_MODE_INOUT, & status, XJ_NULL );
    DEVI_hAD = XJP_DRV_gioCreate( CDEV_NAME_AD_B, XJP_DRV_MODE_INPUT, & status, XJ_NULL );
    DEVI_hADGain = XJP_DRV_gioCreate( CDEV_NAME_AD_C, XJP_DRV_MODE_OUTPUT, & status, XJ_NULL );
    DEVI_hKey = XJP_DRV_gioCreate( CDEV_NAME_KEY_A, XJP_DRV_MODE_INPUT, & status, XJ_NULL );
    DEVI_hTimeChip = XJP_DRV_gioCreate( CDEV_NAME_TIME_A, XJP_DRV_MODE_INOUT, & status, XJ_NULL );
    DEVI_hTemperature = XJP_DRV_gioCreate( CDEV_NAME_TEMPERATURE_A, XJP_DRV_MODE_INPUT, & status, XJ_NULL );
    DEVI_hSW = XJP_DRV_gioCreate( CDEV_NAME_SW_A, XJP_DRV_MODE_OUTPUT, & status, XJ_NULL );
    DEVI_hOutReadback = XJP_DRV_gioCreate( CDEV_NAME_OR_A, XJP_DRV_MODE_INPUT, & status, XJ_NULL );
    DEVI_hPlugReadback = XJP_DRV_gioCreate( CDEV_NAME_OR_B, XJP_DRV_MODE_INPUT, & status, XJ_NULL );
    DEVI_hOrtCodeDir = XJP_DRV_gioCreate( CDEV_NAME_OCODE_A, XJP_DRV_MODE_OUTPUT, & status, XJ_NULL );
    DEVI_hOrtCodeValue = XJP_DRV_gioCreate( CDEV_NAME_OCODE_B, XJP_DRV_MODE_OUTPUT, & status, XJ_NULL );
    DEVI_hPPS = XJP_DRV_gioCreate( CDEV_NAME_PPS_A, XJP_DRV_MODE_OUTPUT, & status, XJ_NULL );
    DEVI_hPPSStamp = XJP_DRV_gioCreate( CDEV_NAME_PPS_B, XJP_DRV_MODE_INPUT, & status, XJ_NULL );
    DEVI_hUARTStamp = XJP_DRV_gioCreate( CDEV_NAME_UART_A, XJP_DRV_MODE_INOUT, & status, XJ_NULL );
    DEVI_hMotorStatus = XJP_DRV_gioCreate( CDEV_NAME_MOTOR_A, XJP_DRV_MODE_INPUT, & status, XJ_NULL );
    DEVI_hMotorCmd = XJP_DRV_gioCreate( CDEV_NAME_MOTOR_B, XJP_DRV_MODE_OUTPUT, & status, XJ_NULL );
    DEVI_hMotorPulse = XJP_DRV_gioCreate( CDEV_NAME_MOTOR_C, XJP_DRV_MODE_OUTPUT, & status, XJ_NULL );
    DEVI_hSolenoidValveCmd = XJP_DRV_gioCreate( CDEV_NAME_SOV_A, XJP_DRV_MODE_OUTPUT, & status, XJ_NULL );
    DEVI_hFNRam = XJP_DRV_gioCreate( CDEV_NAME_TIME_B, XJP_DRV_MODE_INOUT, & status, XJ_NULL );
    /* I/O��ʼ������ */
    DEVI_IO_Initialize();

    return( XJ_SUCCESS );
}

/* ��ȡӲ����ʶ�� */
UNSIGNED DEVI_GetIdentificationCode ( VOID )
{
    INT status;
    UNSIGNED Variable;

    Variable = XJ_IDC_NPU808; /* NPU808��ʶ */
    status = XJP_DRV_gioSubmit( DEVI_hIDC, XJP_DRV_CMD_READ, & Variable,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( Variable );
}

/* I/O��ʼ������ */
INT DEVI_IO_Initialize( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hInput, XJP_DRV_CTR_INITIALIZE, XJ_NULL );

    return( status );
}

/* ������״ֵ̬ */
INT DEVI_ReadInput( INT Offset, INT Number, VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hInput, XJP_DRV_CMD_READ, buffer_ptr,
        Offset, Number, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* ������Ʒ������ */
INT DEVI_ReadInputQuality( INT Offset, INT Number, VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hInput, XJP_DRV_CMD_READ, buffer_ptr,
        Offset, Number, XJP_DRV_DATA_QUALITY, XJ_NULL );

    return( status );
}

/* ������״ֵ̬��Ʒ������ */
INT DEVI_ReadInputVQ( INT Offset, INT Number, VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hInput, XJP_DRV_CMD_READ, buffer_ptr,
        Offset, Number, XJP_DRV_DATA_VQ, XJ_NULL );

    return( status );
}

/* �������� */
INT DEVI_RelayDriver( INT Offset, INT Number, VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hOutput, XJP_DRV_CMD_WRITE, buffer_ptr,
        Offset, Number, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* ��ʼ�����Ź� */
INT DEVI_InitWatchdog( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hWatchdog, XJP_DRV_CTR_INITIALIZE, XJ_NULL );

    return( status );
}

/* ��λ���Ź� */
INT DEVI_ResetWatchdog ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hWatchdog, XJP_DRV_CTR_RESET, XJ_NULL );

    return( status );
}

/* ��ʼ�����е� */
INT DEVI_InitializeRunLed ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hRunLed, XJP_DRV_CTR_INITIALIZE, XJ_NULL );

    return( status );
}

/* �������е� */
INT DEVI_WriteRunLed ( INT ledStatus )
{
    INT status;
    INT Variable;

    Variable = ledStatus;
    status = XJP_DRV_gioSubmit( DEVI_hRunLed, XJP_DRV_CMD_WRITE, & Variable,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* ��ʼ���澯�� */
INT DEVI_InitializeAlarmLed ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hAlarmLed, XJP_DRV_CTR_INITIALIZE, XJ_NULL );

    return( status );
}

/* ���ø澯�� */
INT DEVI_WriteAlarmLed ( INT ledStatus )
{
    INT status;
    INT Variable;

    Variable = ledStatus;
    status = XJP_DRV_gioSubmit( DEVI_hAlarmLed, XJP_DRV_CMD_WRITE, & Variable,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* ��ʼ���û��� */
INT DEVI_InitializeUserLed ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hUserLed, XJP_DRV_CTR_INITIALIZE, XJ_NULL );

    return( status );
}

/* �����û��� */
INT DEVI_WriteUserLed ( INT iGroup, UNSIGNED ledBits )
{
    INT status;
    UNSIGNED buffer[3];   /* ��ʱ������ */

    buffer[0] = iGroup;
    buffer[1] = ledBits;
    status = XJP_DRV_gioSubmit( DEVI_hUserLed, XJP_DRV_CMD_WRITE, buffer,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* ��ʼ�������̵��� */
INT DEVI_InitializeStartRelay ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hStartRelay, XJP_DRV_CTR_INITIALIZE, XJ_NULL );

    return( status );
}

/* ���������̵��� */
INT DEVI_WriteStartRelay ( INT Offset, INT Number, VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hStartRelay, XJP_DRV_CMD_WRITE, buffer_ptr,
        Offset, Number, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* ���GPS�����жϱ�־ */
INT DEVI_ClearGPS ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hGPS_Pulse, XJP_DRV_CTR_CLEAR, XJ_NULL );

    return( status );
}

/* ��ȡGPS����״̬ */
INT DEVI_GetGPSStatus ( VOID )
{
    INT status;
    INT Variable;

    Variable = 0;
    status = XJP_DRV_gioSubmit( DEVI_hGPS_Pulse, XJP_DRV_CMD_READ, & Variable,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( Variable );
}

/* ���GPS�����豸 */
INT DEVI_CheckGPSPulseDevice ( VOID )
{
    if( DEVI_hGPS_Pulse )
        return( XJ_TRUE );
    else
        return( XJ_FALSE );
}

/* ���B���жϱ�־ */
INT DEVI_ClearIRIGB ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hGPS_IRIGB, XJP_DRV_CTR_CLEAR, XJ_NULL );

    return( status );
}

/* ��ȡB��״̬ */
INT DEVI_GetIRIGBStatus ( UNSIGNED * buffer )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hGPS_IRIGB, XJP_DRV_CMD_READ, buffer,
        0, 5, XJP_DRV_DATA_VALUE, XJ_NULL ); /* ��ȡM��״̬ */

    return( status );
}

/* ���GPS B���豸 */
INT DEVI_CheckIRIGBDevice ( VOID )
{
    if( DEVI_hGPS_IRIGB )
        return( XJ_TRUE );
    else
        return( XJ_FALSE );
}

/* ��ʼ��A/DоƬ */
float * DEVI_InitializeAD ( INT sample_points, UNSIGNED * chl_number )
{
    INT status;
    float * sample_data;
    UNSIGNED buffer[4]; /* ��ʱ������ */

    buffer[0] = sample_points; /* ��������ѡ�� */
    status = XJP_DRV_gioControl( DEVI_hAD, XJP_DRV_CTR_INITIALIZE, buffer );
    if( status == XJ_SUCCESS )
    {
        * chl_number = buffer[0] & 0xff; /* ����ͨ������ */
        sample_data = ( float *)buffer[1];
    }
    else
    {
        * chl_number = 0;
        sample_data = XJ_NULL;
    }

    return( sample_data );
}

/* ��A/D�������� */
INT DEVI_GetSampleData ( INT Number, VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hAD, XJP_DRV_CMD_READ, buffer_ptr,
        0, Number, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* ����AD���� */
INT DEVI_SetADGain( UNSIGNED value )
{
    UNSIGNED buffer[5];                 /* ��ʱ������ */
    INT status;

    buffer[0] = value;                  /* ���� */
    status = XJP_DRV_gioSubmit( DEVI_hADGain, XJP_DRV_CMD_WRITE, buffer,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* ��ʼ���������� */
INT DEVI_InitializeSamplePulse ( INT sample_points, INT pulse_force )
{
    INT status;
    UNSIGNED buffer[4]; /* ��ʱ������ */

    buffer[0] = sample_points; /* ��������ѡ�� */
    buffer[1] = pulse_force;   /* ǿ������ѡ�� */
    status = XJP_DRV_gioControl( DEVI_hSamplePulse, XJP_DRV_CTR_INITIALIZE, buffer );

    return( status );
}

/* ����������� */
INT DEVI_OutputSamplePulse ( VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hSamplePulse, XJP_DRV_CMD_WRITE, buffer_ptr,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* ��ȡӲ��(CPLD)�汾�� */
UNSIGNED DEVI_GetCPLDVersion ( VOID )
{
    INT status;
    UNSIGNED Variable;

    Variable = 0;
    status = XJP_DRV_gioSubmit( DEVI_hVersion, XJP_DRV_CMD_READ, & Variable,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( Variable );
}

/* CPU������ó�ʼ�� */
INT DEVI_InitializeCPU ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hCPU, XJP_DRV_CTR_INITIALIZE, XJ_NULL );

    return( status );
}

/* ��ȡ���̼�ֵ */
UNSIGNED DEVI_GetKeyValue ( VOID )
{
    INT status;
    UNSIGNED Variable;

    Variable = 0;
    status = XJP_DRV_gioSubmit( DEVI_hKey, XJP_DRV_CMD_READ, & Variable,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( Variable );
}

/* ��λCPU */
VOID DEVI_ResetCPU ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hCPU, XJP_DRV_CTR_RESET, XJ_NULL );

    return;
}

/* �����λ״̬��Ϣ */
INT DEVI_RsrInfCPU ( VOID )
{
    INT status;
    INT Variable;
    CHAR * p;

    Variable = 0;
    status = XJP_DRV_gioSubmit( DEVI_hCPU, XJP_DRV_CMD_READ, & Variable,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );
    if( status == XJ_SUCCESS )
    {
        p = ( CHAR *)Variable;
        if( p )
            CLOG_Log( p );
    }

    return( status );
}

/* ��ʼ�����ڶ�ʱ�� */
INT DEVI_InitializePerTimer ( UNSIGNED frequency )
{
    INT status;
    UNSIGNED buffer[4]; /* ��ʱ������ */

    buffer[0] = frequency; /* ��ʱ��Ƶ�� */
    status = XJP_DRV_gioControl( DEVI_hPerTimer, XJP_DRV_CTR_INITIALIZE, buffer );

    if( status == XJ_SUCCESS )
    {
        DEVI_period_time = 1000000 / frequency; /* ���ڣ�΢�룩*/
    }

    return( status );
}

/* ������ڶ�ʱ����־ */
INT DEVI_ClearPerTimer ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hPerTimer, XJP_DRV_CTR_CLEAR, XJ_NULL );

    return( status );
}

/* ��ȡ��ʱ������ */
UNSIGNED DEVI_GetTimerPeriod ( VOID )
{
    return( DEVI_period_time );
}

/* ��ʼ��ʱ��оƬ */
INT DEVI_InitializeTimeChip ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hTimeChip, XJP_DRV_CTR_INITIALIZE, XJ_NULL );

    return( status );
}

/* ��ʱ��оƬʱ�� */
INT DEVI_GetChipTime ( INT Number, VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hTimeChip, XJP_DRV_CMD_READ, buffer_ptr,
        0, Number, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* дʱ��оƬʱ�� */
INT DEVI_SaveChipTime ( INT Number, VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hTimeChip, XJP_DRV_CMD_WRITE, buffer_ptr,
        0, Number, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* ��ʼ���¶ȴ����� */
INT DEVI_InitializeTemperature ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hTemperature, XJP_DRV_CTR_INITIALIZE, XJ_NULL );

    return( status );
}

/* ��ȡ�¶ȴ��������� */
INT DEVI_GetTemperature ( INT * temperature_ptr )
{
    INT status;
    INT Variable;

    Variable = 0;
    status = XJP_DRV_gioSubmit( DEVI_hTemperature, XJP_DRV_CMD_READ, & Variable,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );
    * temperature_ptr = Variable;

    return( status );
}

/* SW���� */
INT DEVI_SWDriver( INT iFlag )
{
    INT status;
    UNSIGNED variable;

    variable = ( UNSIGNED )iFlag;
    status = XJP_DRV_gioSubmit( DEVI_hSW, XJP_DRV_CMD_WRITE, & variable,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* ������·�ض����� */
INT DEVI_OutReadbackDriver( INT Offset, INT Number, VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hOutReadback, XJP_DRV_CMD_READ, buffer_ptr,
        Offset, Number, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* ��������ض����� */
INT DEVI_PlugReadbackDriver( INT Offset, INT Number, VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hPlugReadback, XJP_DRV_CMD_READ, buffer_ptr,
        Offset, Number, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* �����������뷽��ֵ */
INT DEVI_InitializeOrthogonalCode_direction ( unsigned short int dirSetting )
{
    INT status;
    UNSIGNED buffer[4]; /* ��ʱ������ */

    buffer[0] = ( UNSIGNED )dirSetting; /* �������뷽�� */
    status = XJP_DRV_gioControl( DEVI_hOrtCodeDir, XJP_DRV_CTR_INITIALIZE, buffer );

    return( status );
}

/* �������������ʼֵ */
INT DEVI_InitializeOrthogonalCode_value ( int iGroup, unsigned short int iniValue )
{
    INT status;
    UNSIGNED buffer[4]; /* ��ʱ������ */

    buffer[0] = iGroup; /* ��� */
    buffer[1] = iniValue; /* ���������ʼֵ */
    status = XJP_DRV_gioControl( DEVI_hOrtCodeValue, XJP_DRV_CTR_INITIALIZE, buffer );

    return( status );
}

/* ��ȡ�������뷽�� */
INT DEVI_GetOrthogonalCodeDirection ( UNSIGNED * dirSetting )
{
    INT status;
    UNSIGNED Variable;

    Variable = 0;
    status = XJP_DRV_gioSubmit( DEVI_hOrtCodeDir, XJP_DRV_CMD_READ, & Variable,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );
    * dirSetting = Variable;

    return( status );
}

/* ��ȡ��������ֵ */
INT DEVI_GetOrthogonalCodeValue ( INT iGroup, UNSIGNED * iniValue )
{
    INT status;
    UNSIGNED buffer[4]; /* ��ʱ������ */

    buffer[0] = iGroup; /* ��� */
    buffer[1] = 0;

    status = XJP_DRV_gioSubmit( DEVI_hOrtCodeValue, XJP_DRV_CMD_READ, buffer,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );
    * iniValue = buffer[1];

    return( status );
}

/* ����PPS��� */
INT DEVI_SetWidth_PPS ( short int Value )
{
    INT status;
    UNSIGNED buffer[4]; /* ��ʱ������ */

    buffer[0] = ( UNSIGNED )Value; /* PPS��� */
    status = XJP_DRV_gioSubmit( DEVI_hPPS, XJP_DRV_CMD_WRITE, buffer,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* ��ȡ����ʱ��ʱ��� */
INT DEVI_GetPPSTimeStamp( UNSIGNED * pBuffer )
{
    INT status;
    UNSIGNED buffer[4]; /* ��ʱ������ */

    buffer[0] = 1; /* ��ȡʱ�����ʶ */
    status = XJP_DRV_gioSubmit( DEVI_hPPSStamp, XJP_DRV_CMD_READ, buffer,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );
    if( status == NU_SUCCESS )
    {
        * pBuffer = buffer[1];
    }

    return( status );
}

/* ����PPS��Ȼض��Ƿ�������� */
INT DEVI_PPSSetWidthReadBack( INT * pBuffer )
{
    INT status;
    UNSIGNED buffer[4]; /* ��ʱ������ */

    buffer[0] = 0; /* ��ȡ����PPS���ض���ʶ */
    status = XJP_DRV_gioSubmit( DEVI_hPPSStamp, XJP_DRV_CMD_READ, buffer,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );
    if( status == NU_SUCCESS )
    {
        * pBuffer = ( INT )buffer[1];
    }

    return( status );

    return( 0 );
}

/* ��ȡGtime��ʱ��CTֵ */
INT DEVI_ReadGTimeCnr1 ( VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hSamplePulse, XJP_DRV_CMD_READ, buffer_ptr,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* UART���ڳ�ʼ�� */
INT DEVI_UartComInit( INT channel, INT baud_rate, INT parity )
{
    UNSIGNED buffer[5];    /* ��ʱ������ */
    INT status;

    buffer[0] = channel;   /* ͨ���� */
    buffer[1] = baud_rate; /* ������ */
    buffer[2] = parity;    /* ��żУ��λ */
    status = XJP_DRV_gioControl( DEVI_hUARTStamp, XJP_DRV_CTR_INITIALIZE, buffer );

    return( status );
}

/* ����UART485ʹ�ܱ�ʶ */
INT DEVI_UART_RS485_SetEnFlag( int channel, int iEnFlag )
{
    UNSIGNED buffer[5];    /* ��ʱ������ */
    INT status;

    buffer[0] = channel;   /* ͨ���� */
    buffer[1] = iEnFlag;   /* ʹ�ܱ�ʶ */
    status = XJP_DRV_gioControl( DEVI_hUARTStamp, XJP_DRV_CTR_CLEAR, buffer );
    return( status );
}

/* ��UART����д���� */
INT DEVI_UART_WriteComm( INT channel, UNSIGNED_CHAR * buf, INT len )
{
    UNSIGNED buffer[5];          /* ��ʱ������ */
    INT status;

    buffer[0] = channel;         /* ͨ���� */
    buffer[1] = ( UNSIGNED )buf; /* ������ */
    buffer[2] = len;             /* ���ݳ��� */

    status = XJP_DRV_gioSubmit( DEVI_hUARTStamp, XJP_DRV_CMD_WRITE, buffer,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );
    return( status );
}

/* ��UART���ڶ�ȡ���� */
INT DEVI_UART_ReadComm( INT channel, UNSIGNED_CHAR * buf )
{
    UNSIGNED buffer[5];          /* ��ʱ������ */
    INT status;
    INT iLen = 0;

    buffer[0] = channel;          /* ͨ���� */
    buffer[1] = ( UNSIGNED )& iLen; /* ���ĳ��� */
    buffer[2] = ( UNSIGNED )buf;  /* ������ */

    status = XJP_DRV_gioSubmit( DEVI_hUARTStamp, XJP_DRV_CMD_READ, buffer,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );
    if( status != NU_SUCCESS )
    {
        iLen = 0;
    }
    return( iLen );
}

/* ��ȡUART�ж�״̬ */
INT DEVI_UART_GetPortIntFlag( INT channel )
{
    UNSIGNED buffer[5];          /* ��ʱ������ */
    INT status;
    INT iFlag = 0;

    buffer[0] = channel;             /* ͨ���� */
    buffer[1] = ( UNSIGNED )& iFlag; /* �жϱ�ʶ */

    status = XJP_DRV_gioControl( DEVI_hUARTStamp, XJP_DRV_CTR_RESET, buffer );

    return( iFlag );
}

/* �������豸 */
INT DEVI_CheckFiberDevice ( VOID )
{
    UNSIGNED id_code;
    INT iStatus = XJ_FALSE;

    id_code = DEVI_GetIdentificationCode(); /* ��ȡӲ����ʶ�� */
    switch( id_code )
    {
    case XJ_IDC_NPU823:
    case XJ_IDC_NPU8103:
        iStatus = XJ_TRUE;
        break;
    default:
        iStatus = XJ_FALSE;
        break;
    }
    return( iStatus );
}

/* ��ȡ�������״̬��Ϣ */
INT DEVI_GetMotorStatus ( UNSIGNED * motor_status )
{
    INT status;
    UNSIGNED Variable;

    Variable = 0;
    status = XJP_DRV_gioSubmit( DEVI_hMotorStatus, XJP_DRV_CMD_READ, & Variable,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );
    * motor_status = Variable;

    return( status );
}

/* ���ò�������������� */
INT DEVI_SetMotorCommand( INT channel, UNSIGNED command )
{
    UNSIGNED buffer[5];                 /* ��ʱ������ */
    INT status;

    buffer[0] = channel;                /* ͨ���� */
    buffer[1] = command;                /* �������� */
    status = XJP_DRV_gioSubmit( DEVI_hMotorCmd, XJP_DRV_CMD_WRITE, buffer,
        0, 2, XJP_DRV_DATA_VALUE, XJ_NULL ); /* ���ÿ������� */

    return( status );
}

/* ���ò������������ */
INT DEVI_SetMotorPulse( INT channel, UNSIGNED pulse )
{
    UNSIGNED buffer[5];                 /* ��ʱ������ */
    INT status;

    buffer[0] = channel;                /* ͨ���� */
    buffer[1] = pulse;                  /* ������ */
    status = XJP_DRV_gioSubmit( DEVI_hMotorPulse, XJP_DRV_CMD_WRITE, buffer,
        0, 2, XJP_DRV_DATA_VALUE, XJ_NULL ); /* �������� */

    return( status );
}

/* ���õ�ŷ��������� */
INT DEVI_SetSolenoidValveCommand( INT channel, UNSIGNED command )
{
    UNSIGNED buffer[5];                 /* ��ʱ������ */
    INT status;

    buffer[0] = channel;                /* ͨ���� */
    buffer[1] = command;                /* �������� */
    status = XJP_DRV_gioSubmit( DEVI_hSolenoidValveCmd, XJP_DRV_CMD_WRITE, buffer,
        0, 2, XJP_DRV_DATA_VALUE, XJ_NULL ); /* ��ŷ����� */

    return( status );
}

/* ��ʱ��оƬram������(Ferroelectric Nonvolatile RAM) */
INT DEVI_GetDataFNRam ( UNSIGNED addrOffset, CHAR * buff , INT len )
{
    INT status;
    UNSIGNED buffer[4]; /* ��ʱ������ */

    buffer[0] = addrOffset;
    buffer[1] = ( UNSIGNED )buff;
    buffer[2] = len;
    status = XJP_DRV_gioSubmit( DEVI_hFNRam, XJP_DRV_CMD_READ, buffer,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* ��ʱ��оƬRAM��д����(Ferroelectric Nonvolatile RAM) */
INT DEVI_SaveDataFNRam ( UNSIGNED addrOffset, CHAR * buff , INT len )
{
    INT status;
    UNSIGNED buffer[4]; /* ��ʱ������ */

    buffer[0] = addrOffset;
    buffer[1] = ( UNSIGNED )buff;
    buffer[2] = len;
    status = XJP_DRV_gioSubmit( DEVI_hFNRam, XJP_DRV_CMD_WRITE, buffer,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}
