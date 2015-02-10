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
/*      cdev_extr.h                                     Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CDEV - Common Device Management                                  */
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
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhideYang.      11-28-2009      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
/* Check to see if the file has been included already.  */

#ifndef CDEV_EXTR_H
#define CDEV_EXTR_H

/* These names are used to identify the name of the DEV driver. */
#define CDEV_NAME_IO            "IODRV"         /* I/O */
#define CDEV_NAME_IO_A          "IODRV/A"       /* 开入通道 */
#define CDEV_NAME_IO_B          "IODRV/B"       /* 开出通道 */

#define CDEV_NAME_WDT           "WATCHDOGDRV"   /* 看门狗 */
#define CDEV_NAME_WDT_A         "WATCHDOGDRV/A" /* 看门狗通道 */

#define CDEV_NAME_LED           "LEDDRV"        /* 面板灯 */
#define CDEV_NAME_LED_A         "LEDDRV/A"      /* 运行灯通道 */
#define CDEV_NAME_LED_B         "LEDDRV/B"      /* 告警灯通道 */
#define CDEV_NAME_LED_C         "LEDDRV/C"      /* 用户灯通道 */

#define CDEV_NAME_START_RELAY   "STARTRDRV"     /* 继电器 */
#define CDEV_NAME_START_RELAY_A "STARTRDRV/A"   /* 启动继电器通道 */

#define CDEV_NAME_GPS           "GPSDRV"        /* GPS */
#define CDEV_NAME_GPS_A         "GPSDRV/A"      /* GPS脉冲通道 */
#define CDEV_NAME_GPS_B         "GPSDRV/B"      /* GPS B码通道 */

#define CDEV_NAME_AD            "ADDRV"         /* A/D */
#define CDEV_NAME_AD_A          "ADDRV/A"       /* 采样脉冲 */
#define CDEV_NAME_AD_B          "ADDRV/B"       /* A/D通道 */
#define CDEV_NAME_AD_C          "ADDRV/C"       /* A/D增益 */

#define CDEV_NAME_VER           "VERDRV"        /* CPLD版本 */
#define CDEV_NAME_VER_A         "VERDRV/A"      /* CPLD版本通道 */

#define CDEV_NAME_CPU           "CPUDRV"        /* CPU */
#define CDEV_NAME_CPU_A         "CPUDRV/A"      /* CPU硬件 */
#define CDEV_NAME_CPU_B         "CPUDRV/B"      /* CPU内核周期定时器 */

#define CDEV_NAME_KEY           "KEYDRV"        /* 键值 */
#define CDEV_NAME_KEY_A         "KEYDRV/A"      /* 键值通道 */

#define CDEV_NAME_TIME          "TIMEDRV"       /* 时钟芯片 */
#define CDEV_NAME_TIME_A        "TIMEDRV/A"     /* 芯片时间通道 */
#define CDEV_NAME_TIME_B        "TIMEDRV/B"     /* 芯片数据通道(暂不支持) */

#define CDEV_NAME_TEMPERATURE   "TEMPERATUREDRV"   /* 温度 */
#define CDEV_NAME_TEMPERATURE_A "TEMPERATUREDRV/A" /* 温度通道 */

#define CDEV_NAME_SW            "SWDRV"         /* SW */
#define CDEV_NAME_SW_A          "SWDRV/A"       /* SW通道 */

#define CDEV_NAME_IDC           "IDCDRV"        /* 硬件标识码 */
#define CDEV_NAME_IDC_A         "IDCDRV/A"      /* 硬件标识码通道 */

#define CDEV_NAME_OR            "ORDRV"         /* 开出回读 */
#define CDEV_NAME_OR_A          "ORDRV/A"       /* 开出回读通道 */
#define CDEV_NAME_OR_B          "ORDRV/B"       /* 插件回读通道 */

#define CDEV_NAME_OCODE         "OCODE"         /* 正交编码 */
#define CDEV_NAME_OCODE_A       "OCODE/A"       /* 正交编码方向通道 */
#define CDEV_NAME_OCODE_B       "OCODE/B"       /* 正交编码初值通道 */

#define CDEV_NAME_PPS           "PPSDRV"        /* PPS */
#define CDEV_NAME_PPS_A         "PPSDRV/A"      /* PPS通道(设置PPS误差值) */
#define CDEV_NAME_PPS_B         "PPSDRV/B"      /* PPS通道(读取pps数据信息) */

#define CDEV_NAME_UART          "UARTDRV"       /* UART */
#define CDEV_NAME_UART_A        "UARTDRV/A"     /* UART通道(串口初始化及数据读写) */

#define CDEV_NAME_MOTOR         "MOTORDRV"      /* 步进电机 */
#define CDEV_NAME_MOTOR_A       "MOTORDRV/A"    /* 步进电机状态通道 */
#define CDEV_NAME_MOTOR_B       "MOTORDRV/B"    /* 步进电机控制通道 */
#define CDEV_NAME_MOTOR_C       "MOTORDRV/C"    /* 步进电机脉冲通道 */

#define CDEV_NAME_SOV           "SOLENOIDVALVEDRV" /* 电磁阀 */
#define CDEV_NAME_SOV_A         "SOLENOIDVALVEDRV/A" /* 电磁阀控制通道 */

#endif
