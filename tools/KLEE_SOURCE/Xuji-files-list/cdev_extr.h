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
#define CDEV_NAME_IO_A          "IODRV/A"       /* ����ͨ�� */
#define CDEV_NAME_IO_B          "IODRV/B"       /* ����ͨ�� */

#define CDEV_NAME_WDT           "WATCHDOGDRV"   /* ���Ź� */
#define CDEV_NAME_WDT_A         "WATCHDOGDRV/A" /* ���Ź�ͨ�� */

#define CDEV_NAME_LED           "LEDDRV"        /* ���� */
#define CDEV_NAME_LED_A         "LEDDRV/A"      /* ���е�ͨ�� */
#define CDEV_NAME_LED_B         "LEDDRV/B"      /* �澯��ͨ�� */
#define CDEV_NAME_LED_C         "LEDDRV/C"      /* �û���ͨ�� */

#define CDEV_NAME_START_RELAY   "STARTRDRV"     /* �̵��� */
#define CDEV_NAME_START_RELAY_A "STARTRDRV/A"   /* �����̵���ͨ�� */

#define CDEV_NAME_GPS           "GPSDRV"        /* GPS */
#define CDEV_NAME_GPS_A         "GPSDRV/A"      /* GPS����ͨ�� */
#define CDEV_NAME_GPS_B         "GPSDRV/B"      /* GPS B��ͨ�� */

#define CDEV_NAME_AD            "ADDRV"         /* A/D */
#define CDEV_NAME_AD_A          "ADDRV/A"       /* �������� */
#define CDEV_NAME_AD_B          "ADDRV/B"       /* A/Dͨ�� */
#define CDEV_NAME_AD_C          "ADDRV/C"       /* A/D���� */

#define CDEV_NAME_VER           "VERDRV"        /* CPLD�汾 */
#define CDEV_NAME_VER_A         "VERDRV/A"      /* CPLD�汾ͨ�� */

#define CDEV_NAME_CPU           "CPUDRV"        /* CPU */
#define CDEV_NAME_CPU_A         "CPUDRV/A"      /* CPUӲ�� */
#define CDEV_NAME_CPU_B         "CPUDRV/B"      /* CPU�ں����ڶ�ʱ�� */

#define CDEV_NAME_KEY           "KEYDRV"        /* ��ֵ */
#define CDEV_NAME_KEY_A         "KEYDRV/A"      /* ��ֵͨ�� */

#define CDEV_NAME_TIME          "TIMEDRV"       /* ʱ��оƬ */
#define CDEV_NAME_TIME_A        "TIMEDRV/A"     /* оƬʱ��ͨ�� */
#define CDEV_NAME_TIME_B        "TIMEDRV/B"     /* оƬ����ͨ��(�ݲ�֧��) */

#define CDEV_NAME_TEMPERATURE   "TEMPERATUREDRV"   /* �¶� */
#define CDEV_NAME_TEMPERATURE_A "TEMPERATUREDRV/A" /* �¶�ͨ�� */

#define CDEV_NAME_SW            "SWDRV"         /* SW */
#define CDEV_NAME_SW_A          "SWDRV/A"       /* SWͨ�� */

#define CDEV_NAME_IDC           "IDCDRV"        /* Ӳ����ʶ�� */
#define CDEV_NAME_IDC_A         "IDCDRV/A"      /* Ӳ����ʶ��ͨ�� */

#define CDEV_NAME_OR            "ORDRV"         /* �����ض� */
#define CDEV_NAME_OR_A          "ORDRV/A"       /* �����ض�ͨ�� */
#define CDEV_NAME_OR_B          "ORDRV/B"       /* ����ض�ͨ�� */

#define CDEV_NAME_OCODE         "OCODE"         /* �������� */
#define CDEV_NAME_OCODE_A       "OCODE/A"       /* �������뷽��ͨ�� */
#define CDEV_NAME_OCODE_B       "OCODE/B"       /* ���������ֵͨ�� */

#define CDEV_NAME_PPS           "PPSDRV"        /* PPS */
#define CDEV_NAME_PPS_A         "PPSDRV/A"      /* PPSͨ��(����PPS���ֵ) */
#define CDEV_NAME_PPS_B         "PPSDRV/B"      /* PPSͨ��(��ȡpps������Ϣ) */

#define CDEV_NAME_UART          "UARTDRV"       /* UART */
#define CDEV_NAME_UART_A        "UARTDRV/A"     /* UARTͨ��(���ڳ�ʼ�������ݶ�д) */

#define CDEV_NAME_MOTOR         "MOTORDRV"      /* ������� */
#define CDEV_NAME_MOTOR_A       "MOTORDRV/A"    /* �������״̬ͨ�� */
#define CDEV_NAME_MOTOR_B       "MOTORDRV/B"    /* �����������ͨ�� */
#define CDEV_NAME_MOTOR_C       "MOTORDRV/C"    /* �����������ͨ�� */

#define CDEV_NAME_SOV           "SOLENOIDVALVEDRV" /* ��ŷ� */
#define CDEV_NAME_SOV_A         "SOLENOIDVALVEDRV/A" /* ��ŷ�����ͨ�� */

#endif
