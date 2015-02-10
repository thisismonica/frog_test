
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

/* 以下为OS相关配置 */
#ifdef WIN32
/* 全局变量申明 */
/* Windows 宏定义 */
#endif

#ifndef WIN32 /* 非windows缺省定义一些宏以便通过编译 */
/* 全局变量申明 */
/* Nucleus宏定义 */
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

UNSIGNED DEVI_period_time = 1000; /* 周期间隔定时器的运行周期（微秒）*/

/* 初始化设备 */
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

    /* 获取驱动函数表指针 */
    number = DPFC_DriverPointer( & pointer_list );
    /* 初始化设备驱动环境 */
    XJP_DRV_Initialize( number, (( void **)pointer_list ));

    /* 获取硬件标识 */
    XJP_DRV_mdBindDevice( CDEV_NAME_IDC, 0 );         /* 绑定硬件标识设备 */
    DEVI_hIDC = XJP_DRV_gioCreate( CDEV_NAME_IDC_A, XJP_DRV_MODE_INPUT, & status, XJ_NULL ); /* 创建硬件标识通道 */
    id_code = DEVI_GetIdentificationCode(); /* 读取硬件标识码 */
    switch( id_code )
    {
    case XJ_IDC_NPU808:
        /* NPU808设备ID选择 */
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
        /* NPU808绑定设备使能 */
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
        /* NPU808设备通道使能 */
        devId_GPS_Pulse_Enable = XJ_TRUE;
        devId_GPS_Irigb_Enable = XJ_FALSE;
        break;
    case XJ_IDC_NPU816:
        /* NPU816设备ID选择 */
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
        /* NPU816绑定设备使能 */
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
        /* NPU816使能设备选择 */
        devId_GPS_Pulse_Enable = XJ_TRUE;
        devId_GPS_Irigb_Enable = XJ_FALSE;
        break;
    case XJ_IDC_NPU823:
        /* NPU823设备ID选择 */
        devId_IO = 3; /* 设备号 */
        devId_StartRelay = 1;
        devId_AD = 2; /* A/D设备号 */
        devId_Key = 1;
        devId_SW = 1;
        devId_CPU = 0;
        devId_Led = 0;
        devId_OPR = 1;
        devId_OCD = 0;
        devId_SOV = 0;
        devId_GPS = 0;
        /* NPU823绑定设备使能 */
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
        /* NPU823使能设备选择 */
        devId_GPS_Pulse_Enable = XJ_TRUE;
        devId_GPS_Irigb_Enable = XJ_FALSE;
        break;
    case XJ_IDC_NPU824:
    case XJ_IDC_NPU8104:
        /* NPU824设备ID选择 */
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
        /* NPU824绑定设备使能 */
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
        /* NPU824设备通道使能 */
        devId_GPS_Pulse_Enable = XJ_FALSE;
        devId_GPS_Irigb_Enable = XJ_FALSE;
        break;
    case XJ_IDC_NPU8507:
        /* NPU824设备ID选择 */
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
        /* NPU8507绑定设备使能 */
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
        /* NPU824设备通道使能 */
        devId_GPS_Pulse_Enable = XJ_TRUE;
        devId_GPS_Irigb_Enable = XJ_FALSE;
        break;
    case XJ_IDC_NPU8509:
        /* NPU8509设备ID选择 */
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
        /* NPU8509绑定设备使能 */
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
        /* NPU8509设备通道使能 */
        devId_GPS_Pulse_Enable = XJ_TRUE;
        devId_GPS_Irigb_Enable = XJ_FALSE;
        break;
    case XJ_IDC_NPU827:
        /* NPU827设备ID选择 */
        devId_IO = 6; /* I/O设备号 */
        devId_StartRelay = 0;
        devId_AD = 5; /* AD设备号 */
        devId_Key = 0;
        devId_SW = 0;
        devId_CPU = 1;
        devId_Led = 2; /* 灯设备号 */
        devId_OPR = 0;
        devId_OCD = 0;
        devId_SOV = 0;
        devId_GPS = 0;
        /* NPU827绑定设备使能 */
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
        /* NPU827设备通道使能 */
        devId_GPS_Pulse_Enable = XJ_FALSE;
        devId_GPS_Irigb_Enable = XJ_FALSE;
        break;
    case XJ_IDC_NPU8100:
        /* NPU8100设备ID选择 */
        devId_IO = 2; /* I/O设备号 */
        devId_StartRelay = 2; /* 启动继电器设备号 */
        devId_AD = 1;
        devId_Key = 2; /* 键盘设备号 */
        devId_SW = 1;
        devId_CPU = 1;
        devId_Led = 1;
        devId_OPR = 2; /* 开出回读设备号 */
        devId_OCD = 1;
        devId_SOV = 0;
        devId_GPS = 0;
        /* NPU8100绑定设备使能 */
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
        /* NPU8100使能设备选择 */
        devId_GPS_Pulse_Enable = XJ_FALSE;
        devId_GPS_Irigb_Enable = XJ_TRUE;
        break;
    case XJ_IDC_NPU8101:
        /* NPU8101设备ID选择 */
        devId_IO = 4; /* I/O设备号 */
        devId_StartRelay = 6;   /* 无启动继电器 */
        devId_AD = 3; /* AD设备号 */
        devId_Key = 2; /* 键盘设备号 */
        devId_SW = 1;
        devId_CPU = 1;
        devId_Led = 1;
        devId_OPR = 6;          /* 无扩展开出插件 */
        devId_OCD = 0;
        devId_SOV = 0;
        devId_GPS = 0;
        /* NPU8101绑定设备使能 */
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
        /* NPU8101使能设备选择 */
        devId_GPS_Pulse_Enable = XJ_FALSE;
        devId_GPS_Irigb_Enable = XJ_TRUE;
        break;
    case XJ_IDC_NPU8103:
        /* NPU8103设备ID选择 */
        devId_IO = 5; /* I/O设备号 */
        devId_StartRelay = 3; /* 启动继电器设备号 */
        devId_AD = 4; /* A/D设备号 */
        devId_Key = 3; /* 键盘设备号 */
        devId_SW = 0;
        devId_CPU = 0;
        devId_Led = 0;
        devId_OPR = 4;
        devId_OCD = 0;
        devId_SOV = 0;
        devId_GPS = 0;
        /* NPU8103绑定设备使能 */
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
        /* NPU8103使能设备选择 */
        devId_GPS_Pulse_Enable = XJ_FALSE;
        devId_GPS_Irigb_Enable = XJ_TRUE;
        break;
    case XJ_IDC_NPI807:
        /* NPI807设备ID选择 */
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
        /* NPI807绑定设备使能 */
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
        /* NPI807使能设备 */
        devId_GPS_Pulse_Enable = XJ_FALSE;
        devId_GPS_Irigb_Enable = XJ_FALSE;
        break;
    case XJ_IDC_DTS568:
        /* DTS568设备ID选择 */
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
        /* DTS568绑定设备使能 */
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
        /* DTS568使能设备 */
        devId_GPS_Pulse_Enable = XJ_FALSE;
        devId_GPS_Irigb_Enable = XJ_FALSE;
        break;
    case XJ_IDC_NPU826:
        /* NPU826设备ID选择 */
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
        /* NPU826绑定设备使能 */
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
        /* NPU826使能设备 */
        devId_GPS_Pulse_Enable = XJ_FALSE;
        devId_GPS_Irigb_Enable = XJ_TRUE;
        break;
    default:
        /* 设备ID选择 */
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
        /* 绑定设备使能 */
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
        /* 使能设备 */
        devId_GPS_Pulse_Enable = XJ_TRUE;
        devId_GPS_Irigb_Enable = XJ_FALSE;
        break;
    }

    /* 绑定设备 */
    if( devId_mdEnable_IO == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_IO, devId_IO );     /* I/O */
    if( devId_mdEnable_WDT == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_WDT, 0 );           /* 看门狗 */
    if( devId_mdEnable_LED == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_LED, devId_Led );   /* 面板灯 */
    if( devId_mdEnable_START_RELAY == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_START_RELAY, devId_StartRelay ); /* 继电器 */
    if( devId_mdEnable_GPS == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_GPS, devId_GPS );   /* GPS */
    if( devId_mdEnable_VER == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_VER, 0 );           /* CPLD版本 */
    if( devId_mdEnable_CPU == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_CPU, devId_CPU );   /* CPU相关操作 */
    if( devId_mdEnable_AD == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_AD, devId_AD );     /* A/D */
    if( devId_mdEnable_KEY == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_KEY, devId_Key );   /* 键值 */
    if( devId_mdEnable_TIME == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_TIME, 0 );          /* 时间芯片 */
    if( devId_mdEnable_TEMPERATURE == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_TEMPERATURE, 0 );   /* 温度传感器芯片 */
    if( devId_mdEnable_SW == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_SW, devId_SW );     /* SW输出 */
    if( devId_mdEnable_OR == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_OR, devId_OPR );    /* 开出（插件）回读 */
    if( devId_mdEnable_OCOD == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_OCODE, devId_OCD ); /* 正交编码 */
    if( devId_mdEnable_UART == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_UART, 0 );          /* UART */
    if( devId_mdEnable_PPS == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_PPS, 0 );           /* PPS */
    if( devId_mdEnable_MOTOR == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_MOTOR, 0 );         /* 步进电机 */
    if( devId_mdEnable_SOV == XJ_TRUE )
        XJP_DRV_mdBindDevice( CDEV_NAME_SOV, 0 );           /* 电磁阀 */

    /* 创建通道设备 */
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
    /* I/O初始化函数 */
    DEVI_IO_Initialize();

    return( XJ_SUCCESS );
}

/* 读取硬件标识号 */
UNSIGNED DEVI_GetIdentificationCode ( VOID )
{
    INT status;
    UNSIGNED Variable;

    Variable = XJ_IDC_NPU808; /* NPU808标识 */
    status = XJP_DRV_gioSubmit( DEVI_hIDC, XJP_DRV_CMD_READ, & Variable,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( Variable );
}

/* I/O初始化函数 */
INT DEVI_IO_Initialize( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hInput, XJP_DRV_CTR_INITIALIZE, XJ_NULL );

    return( status );
}

/* 读开入状态值 */
INT DEVI_ReadInput( INT Offset, INT Number, VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hInput, XJP_DRV_CMD_READ, buffer_ptr,
        Offset, Number, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* 读开入品质因素 */
INT DEVI_ReadInputQuality( INT Offset, INT Number, VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hInput, XJP_DRV_CMD_READ, buffer_ptr,
        Offset, Number, XJP_DRV_DATA_QUALITY, XJ_NULL );

    return( status );
}

/* 读开入状态值和品质因素 */
INT DEVI_ReadInputVQ( INT Offset, INT Number, VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hInput, XJP_DRV_CMD_READ, buffer_ptr,
        Offset, Number, XJP_DRV_DATA_VQ, XJ_NULL );

    return( status );
}

/* 开出驱动 */
INT DEVI_RelayDriver( INT Offset, INT Number, VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hOutput, XJP_DRV_CMD_WRITE, buffer_ptr,
        Offset, Number, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* 初始化看门狗 */
INT DEVI_InitWatchdog( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hWatchdog, XJP_DRV_CTR_INITIALIZE, XJ_NULL );

    return( status );
}

/* 复位看门狗 */
INT DEVI_ResetWatchdog ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hWatchdog, XJP_DRV_CTR_RESET, XJ_NULL );

    return( status );
}

/* 初始化运行灯 */
INT DEVI_InitializeRunLed ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hRunLed, XJP_DRV_CTR_INITIALIZE, XJ_NULL );

    return( status );
}

/* 设置运行灯 */
INT DEVI_WriteRunLed ( INT ledStatus )
{
    INT status;
    INT Variable;

    Variable = ledStatus;
    status = XJP_DRV_gioSubmit( DEVI_hRunLed, XJP_DRV_CMD_WRITE, & Variable,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* 初始化告警灯 */
INT DEVI_InitializeAlarmLed ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hAlarmLed, XJP_DRV_CTR_INITIALIZE, XJ_NULL );

    return( status );
}

/* 设置告警灯 */
INT DEVI_WriteAlarmLed ( INT ledStatus )
{
    INT status;
    INT Variable;

    Variable = ledStatus;
    status = XJP_DRV_gioSubmit( DEVI_hAlarmLed, XJP_DRV_CMD_WRITE, & Variable,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* 初始化用户灯 */
INT DEVI_InitializeUserLed ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hUserLed, XJP_DRV_CTR_INITIALIZE, XJ_NULL );

    return( status );
}

/* 设置用户灯 */
INT DEVI_WriteUserLed ( INT iGroup, UNSIGNED ledBits )
{
    INT status;
    UNSIGNED buffer[3];   /* 临时缓冲区 */

    buffer[0] = iGroup;
    buffer[1] = ledBits;
    status = XJP_DRV_gioSubmit( DEVI_hUserLed, XJP_DRV_CMD_WRITE, buffer,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* 初始化启动继电器 */
INT DEVI_InitializeStartRelay ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hStartRelay, XJP_DRV_CTR_INITIALIZE, XJ_NULL );

    return( status );
}

/* 设置启动继电器 */
INT DEVI_WriteStartRelay ( INT Offset, INT Number, VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hStartRelay, XJP_DRV_CMD_WRITE, buffer_ptr,
        Offset, Number, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* 清除GPS脉冲中断标志 */
INT DEVI_ClearGPS ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hGPS_Pulse, XJP_DRV_CTR_CLEAR, XJ_NULL );

    return( status );
}

/* 读取GPS脉冲状态 */
INT DEVI_GetGPSStatus ( VOID )
{
    INT status;
    INT Variable;

    Variable = 0;
    status = XJP_DRV_gioSubmit( DEVI_hGPS_Pulse, XJP_DRV_CMD_READ, & Variable,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( Variable );
}

/* 检查GPS脉冲设备 */
INT DEVI_CheckGPSPulseDevice ( VOID )
{
    if( DEVI_hGPS_Pulse )
        return( XJ_TRUE );
    else
        return( XJ_FALSE );
}

/* 清除B码中断标志 */
INT DEVI_ClearIRIGB ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hGPS_IRIGB, XJP_DRV_CTR_CLEAR, XJ_NULL );

    return( status );
}

/* 读取B码状态 */
INT DEVI_GetIRIGBStatus ( UNSIGNED * buffer )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hGPS_IRIGB, XJP_DRV_CMD_READ, buffer,
        0, 5, XJP_DRV_DATA_VALUE, XJ_NULL ); /* 读取M码状态 */

    return( status );
}

/* 检查GPS B码设备 */
INT DEVI_CheckIRIGBDevice ( VOID )
{
    if( DEVI_hGPS_IRIGB )
        return( XJ_TRUE );
    else
        return( XJ_FALSE );
}

/* 初始化A/D芯片 */
float * DEVI_InitializeAD ( INT sample_points, UNSIGNED * chl_number )
{
    INT status;
    float * sample_data;
    UNSIGNED buffer[4]; /* 临时缓冲区 */

    buffer[0] = sample_points; /* 采样点数选择 */
    status = XJP_DRV_gioControl( DEVI_hAD, XJP_DRV_CTR_INITIALIZE, buffer );
    if( status == XJ_SUCCESS )
    {
        * chl_number = buffer[0] & 0xff; /* 采样通道个数 */
        sample_data = ( float *)buffer[1];
    }
    else
    {
        * chl_number = 0;
        sample_data = XJ_NULL;
    }

    return( sample_data );
}

/* 读A/D采样数据 */
INT DEVI_GetSampleData ( INT Number, VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hAD, XJP_DRV_CMD_READ, buffer_ptr,
        0, Number, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* 设置AD增益 */
INT DEVI_SetADGain( UNSIGNED value )
{
    UNSIGNED buffer[5];                 /* 临时缓冲区 */
    INT status;

    buffer[0] = value;                  /* 增益 */
    status = XJP_DRV_gioSubmit( DEVI_hADGain, XJP_DRV_CMD_WRITE, buffer,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* 初始化采样脉冲 */
INT DEVI_InitializeSamplePulse ( INT sample_points, INT pulse_force )
{
    INT status;
    UNSIGNED buffer[4]; /* 临时缓冲区 */

    buffer[0] = sample_points; /* 采样点数选择 */
    buffer[1] = pulse_force;   /* 强制脉冲选择 */
    status = XJP_DRV_gioControl( DEVI_hSamplePulse, XJP_DRV_CTR_INITIALIZE, buffer );

    return( status );
}

/* 采样脉冲输出 */
INT DEVI_OutputSamplePulse ( VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hSamplePulse, XJP_DRV_CMD_WRITE, buffer_ptr,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* 读取硬件(CPLD)版本号 */
UNSIGNED DEVI_GetCPLDVersion ( VOID )
{
    INT status;
    UNSIGNED Variable;

    Variable = 0;
    status = XJP_DRV_gioSubmit( DEVI_hVersion, XJP_DRV_CMD_READ, & Variable,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( Variable );
}

/* CPU相关设置初始化 */
INT DEVI_InitializeCPU ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hCPU, XJP_DRV_CTR_INITIALIZE, XJ_NULL );

    return( status );
}

/* 读取键盘键值 */
UNSIGNED DEVI_GetKeyValue ( VOID )
{
    INT status;
    UNSIGNED Variable;

    Variable = 0;
    status = XJP_DRV_gioSubmit( DEVI_hKey, XJP_DRV_CMD_READ, & Variable,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( Variable );
}

/* 复位CPU */
VOID DEVI_ResetCPU ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hCPU, XJP_DRV_CTR_RESET, XJ_NULL );

    return;
}

/* 输出复位状态信息 */
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

/* 初始化周期定时器 */
INT DEVI_InitializePerTimer ( UNSIGNED frequency )
{
    INT status;
    UNSIGNED buffer[4]; /* 临时缓冲区 */

    buffer[0] = frequency; /* 定时器频率 */
    status = XJP_DRV_gioControl( DEVI_hPerTimer, XJP_DRV_CTR_INITIALIZE, buffer );

    if( status == XJ_SUCCESS )
    {
        DEVI_period_time = 1000000 / frequency; /* 周期（微秒）*/
    }

    return( status );
}

/* 清除周期定时器标志 */
INT DEVI_ClearPerTimer ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hPerTimer, XJP_DRV_CTR_CLEAR, XJ_NULL );

    return( status );
}

/* 获取定时器周期 */
UNSIGNED DEVI_GetTimerPeriod ( VOID )
{
    return( DEVI_period_time );
}

/* 初始化时钟芯片 */
INT DEVI_InitializeTimeChip ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hTimeChip, XJP_DRV_CTR_INITIALIZE, XJ_NULL );

    return( status );
}

/* 读时钟芯片时间 */
INT DEVI_GetChipTime ( INT Number, VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hTimeChip, XJP_DRV_CMD_READ, buffer_ptr,
        0, Number, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* 写时钟芯片时间 */
INT DEVI_SaveChipTime ( INT Number, VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hTimeChip, XJP_DRV_CMD_WRITE, buffer_ptr,
        0, Number, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* 初始化温度传感器 */
INT DEVI_InitializeTemperature ( VOID )
{
    INT status;

    status = XJP_DRV_gioControl( DEVI_hTemperature, XJP_DRV_CTR_INITIALIZE, XJ_NULL );

    return( status );
}

/* 读取温度传感器数据 */
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

/* SW驱动 */
INT DEVI_SWDriver( INT iFlag )
{
    INT status;
    UNSIGNED variable;

    variable = ( UNSIGNED )iFlag;
    status = XJP_DRV_gioSubmit( DEVI_hSW, XJP_DRV_CMD_WRITE, & variable,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* 开出回路回读驱动 */
INT DEVI_OutReadbackDriver( INT Offset, INT Number, VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hOutReadback, XJP_DRV_CMD_READ, buffer_ptr,
        Offset, Number, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* 开出插件回读驱动 */
INT DEVI_PlugReadbackDriver( INT Offset, INT Number, VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hPlugReadback, XJP_DRV_CMD_READ, buffer_ptr,
        Offset, Number, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* 设置正交编码方向定值 */
INT DEVI_InitializeOrthogonalCode_direction ( unsigned short int dirSetting )
{
    INT status;
    UNSIGNED buffer[4]; /* 临时缓冲区 */

    buffer[0] = ( UNSIGNED )dirSetting; /* 正交编码方向 */
    status = XJP_DRV_gioControl( DEVI_hOrtCodeDir, XJP_DRV_CTR_INITIALIZE, buffer );

    return( status );
}

/* 设置正交编码初始值 */
INT DEVI_InitializeOrthogonalCode_value ( int iGroup, unsigned short int iniValue )
{
    INT status;
    UNSIGNED buffer[4]; /* 临时缓冲区 */

    buffer[0] = iGroup; /* 组号 */
    buffer[1] = iniValue; /* 正交编码初始值 */
    status = XJP_DRV_gioControl( DEVI_hOrtCodeValue, XJP_DRV_CTR_INITIALIZE, buffer );

    return( status );
}

/* 读取正交编码方向 */
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

/* 读取正交编码值 */
INT DEVI_GetOrthogonalCodeValue ( INT iGroup, UNSIGNED * iniValue )
{
    INT status;
    UNSIGNED buffer[4]; /* 临时缓冲区 */

    buffer[0] = iGroup; /* 组号 */
    buffer[1] = 0;

    status = XJP_DRV_gioSubmit( DEVI_hOrtCodeValue, XJP_DRV_CMD_READ, buffer,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );
    * iniValue = buffer[1];

    return( status );
}

/* 设置PPS宽度 */
INT DEVI_SetWidth_PPS ( short int Value )
{
    INT status;
    UNSIGNED buffer[4]; /* 临时缓冲区 */

    buffer[0] = ( UNSIGNED )Value; /* PPS宽度 */
    status = XJP_DRV_gioSubmit( DEVI_hPPS, XJP_DRV_CMD_WRITE, buffer,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* 获取脉冲时刻时间戳 */
INT DEVI_GetPPSTimeStamp( UNSIGNED * pBuffer )
{
    INT status;
    UNSIGNED buffer[4]; /* 临时缓冲区 */

    buffer[0] = 1; /* 读取时间戳标识 */
    status = XJP_DRV_gioSubmit( DEVI_hPPSStamp, XJP_DRV_CMD_READ, buffer,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );
    if( status == NU_SUCCESS )
    {
        * pBuffer = buffer[1];
    }

    return( status );
}

/* 设置PPS宽度回读是否设置完成 */
INT DEVI_PPSSetWidthReadBack( INT * pBuffer )
{
    INT status;
    UNSIGNED buffer[4]; /* 临时缓冲区 */

    buffer[0] = 0; /* 读取设置PPS误差回读标识 */
    status = XJP_DRV_gioSubmit( DEVI_hPPSStamp, XJP_DRV_CMD_READ, buffer,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );
    if( status == NU_SUCCESS )
    {
        * pBuffer = ( INT )buffer[1];
    }

    return( status );

    return( 0 );
}

/* 读取Gtime定时器CT值 */
INT DEVI_ReadGTimeCnr1 ( VOID * buffer_ptr )
{
    INT status;

    status = XJP_DRV_gioSubmit( DEVI_hSamplePulse, XJP_DRV_CMD_READ, buffer_ptr,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* UART串口初始化 */
INT DEVI_UartComInit( INT channel, INT baud_rate, INT parity )
{
    UNSIGNED buffer[5];    /* 临时缓冲区 */
    INT status;

    buffer[0] = channel;   /* 通道号 */
    buffer[1] = baud_rate; /* 波特率 */
    buffer[2] = parity;    /* 奇偶校验位 */
    status = XJP_DRV_gioControl( DEVI_hUARTStamp, XJP_DRV_CTR_INITIALIZE, buffer );

    return( status );
}

/* 设置UART485使能标识 */
INT DEVI_UART_RS485_SetEnFlag( int channel, int iEnFlag )
{
    UNSIGNED buffer[5];    /* 临时缓冲区 */
    INT status;

    buffer[0] = channel;   /* 通道号 */
    buffer[1] = iEnFlag;   /* 使能标识 */
    status = XJP_DRV_gioControl( DEVI_hUARTStamp, XJP_DRV_CTR_CLEAR, buffer );
    return( status );
}

/* 向UART串口写数据 */
INT DEVI_UART_WriteComm( INT channel, UNSIGNED_CHAR * buf, INT len )
{
    UNSIGNED buffer[5];          /* 临时缓冲区 */
    INT status;

    buffer[0] = channel;         /* 通道号 */
    buffer[1] = ( UNSIGNED )buf; /* 缓冲区 */
    buffer[2] = len;             /* 数据长度 */

    status = XJP_DRV_gioSubmit( DEVI_hUARTStamp, XJP_DRV_CMD_WRITE, buffer,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );
    return( status );
}

/* 从UART串口读取数据 */
INT DEVI_UART_ReadComm( INT channel, UNSIGNED_CHAR * buf )
{
    UNSIGNED buffer[5];          /* 临时缓冲区 */
    INT status;
    INT iLen = 0;

    buffer[0] = channel;          /* 通道号 */
    buffer[1] = ( UNSIGNED )& iLen; /* 报文长度 */
    buffer[2] = ( UNSIGNED )buf;  /* 缓冲区 */

    status = XJP_DRV_gioSubmit( DEVI_hUARTStamp, XJP_DRV_CMD_READ, buffer,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );
    if( status != NU_SUCCESS )
    {
        iLen = 0;
    }
    return( iLen );
}

/* 获取UART中断状态 */
INT DEVI_UART_GetPortIntFlag( INT channel )
{
    UNSIGNED buffer[5];          /* 临时缓冲区 */
    INT status;
    INT iFlag = 0;

    buffer[0] = channel;             /* 通道号 */
    buffer[1] = ( UNSIGNED )& iFlag; /* 中断标识 */

    status = XJP_DRV_gioControl( DEVI_hUARTStamp, XJP_DRV_CTR_RESET, buffer );

    return( iFlag );
}

/* 检查光纤设备 */
INT DEVI_CheckFiberDevice ( VOID )
{
    UNSIGNED id_code;
    INT iStatus = XJ_FALSE;

    id_code = DEVI_GetIdentificationCode(); /* 读取硬件标识码 */
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

/* 读取步进电机状态信息 */
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

/* 设置步进电机控制命令 */
INT DEVI_SetMotorCommand( INT channel, UNSIGNED command )
{
    UNSIGNED buffer[5];                 /* 临时缓冲区 */
    INT status;

    buffer[0] = channel;                /* 通道号 */
    buffer[1] = command;                /* 控制命令 */
    status = XJP_DRV_gioSubmit( DEVI_hMotorCmd, XJP_DRV_CMD_WRITE, buffer,
        0, 2, XJP_DRV_DATA_VALUE, XJ_NULL ); /* 设置控制命令 */

    return( status );
}

/* 设置步进电机脉冲数 */
INT DEVI_SetMotorPulse( INT channel, UNSIGNED pulse )
{
    UNSIGNED buffer[5];                 /* 临时缓冲区 */
    INT status;

    buffer[0] = channel;                /* 通道号 */
    buffer[1] = pulse;                  /* 脉冲数 */
    status = XJP_DRV_gioSubmit( DEVI_hMotorPulse, XJP_DRV_CMD_WRITE, buffer,
        0, 2, XJP_DRV_DATA_VALUE, XJ_NULL ); /* 设置脉冲 */

    return( status );
}

/* 设置电磁阀控制命令 */
INT DEVI_SetSolenoidValveCommand( INT channel, UNSIGNED command )
{
    UNSIGNED buffer[5];                 /* 临时缓冲区 */
    INT status;

    buffer[0] = channel;                /* 通道号 */
    buffer[1] = command;                /* 控制命令 */
    status = XJP_DRV_gioSubmit( DEVI_hSolenoidValveCmd, XJP_DRV_CMD_WRITE, buffer,
        0, 2, XJP_DRV_DATA_VALUE, XJ_NULL ); /* 电磁阀控制 */

    return( status );
}

/* 读时钟芯片ram区数据(Ferroelectric Nonvolatile RAM) */
INT DEVI_GetDataFNRam ( UNSIGNED addrOffset, CHAR * buff , INT len )
{
    INT status;
    UNSIGNED buffer[4]; /* 临时缓冲区 */

    buffer[0] = addrOffset;
    buffer[1] = ( UNSIGNED )buff;
    buffer[2] = len;
    status = XJP_DRV_gioSubmit( DEVI_hFNRam, XJP_DRV_CMD_READ, buffer,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}

/* 向时钟芯片RAM区写数据(Ferroelectric Nonvolatile RAM) */
INT DEVI_SaveDataFNRam ( UNSIGNED addrOffset, CHAR * buff , INT len )
{
    INT status;
    UNSIGNED buffer[4]; /* 临时缓冲区 */

    buffer[0] = addrOffset;
    buffer[1] = ( UNSIGNED )buff;
    buffer[2] = len;
    status = XJP_DRV_gioSubmit( DEVI_hFNRam, XJP_DRV_CMD_WRITE, buffer,
        0, 1, XJP_DRV_DATA_VALUE, XJ_NULL );

    return( status );
}
