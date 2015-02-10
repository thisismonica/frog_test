
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
/*      xj_outc.h                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      OUT - Output Management                                          */
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
/*      ZhideYang.      07-19-2007      Created initial version 1.0      */
/*     YanJuanJiang.    07-24-2007      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"
#include "plat/es_plat.h"

/* Check to see if the file has been included already.  */

#ifndef XJ_OUT_CHECK_H
#define XJ_OUT_CHECK_H

#ifdef __cplusplus
extern "C" {
#endif

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

/* error bits */
#ifndef XJ_ERROR_EVENTBITS
#define XJ_ERROR_EVENTBITS
#define Error_SRAM          (1 << 0)  /* EEPROM error */
#define Error_RAM           (1 << 1)  /* RAM error */
#define Error_Setting       (1 << 2)  /* setting error */
#define Error_Relay         (1 << 3)  /* relay error�����߻������ */
#define Error_5VPower       (1 << 4)  /* 5v power error */
#define Error_CPU_Number    (1 << 5)  /* CPU number error */
#define Error_AD            (1 << 6)  /* A/D error */
#define Error_Commu         (1 << 7)  /* communication error */
#define Error_FLASH         (1 << 8)  /* flash error */
#define Error_Program       (1 << 9)  /* program error */
#define Error_Relay_Close   (1 << 10) /* relay strike through error�������� */
#define Error_Extend_Port   (1 << 11) /* extended port error */
#define Close_Relay_Flag    (1 << 12) /* power error close relay flag */
#define Sample_Data_OK      (1 << 13) /* sample data ok */
#define User_Alarm_Bits     (1 << 14) /* user alarm bits */
#define GWS_InitStatus      (1 << 15)            /* gws��ʼ״̬ */
#endif

#define OUTC_CHECK_ALARM_EVENT_ID     0x5a  /* ���������¼�����(�澯�¼�) */
#define OUTC_CHECK_RESERVE_EVENT_ID   0x5b  /* �ڲ��¼����� */
#define OUTC_CHECK_EVENT_NAME_LENGTH  31    /* �¼����Ƴ��� */

#ifndef XJ_OUTC_MODULE
#define XJ_OUTC_MODULE

#endif

/*=======================================================================*/
/* ����������                                                            */
/*=======================================================================*/
/* ���ڼ���ʼ������ */
VOID OUTC_CheckInitialize( UNSIGNED uHardGroupNum, UNSIGNED uTotalGroupNum, UNSIGNED * pCheckBits,
    UNSIGNED uCheckNum , NU_EVENT_GROUP * pErrEventGroup, INT(* FillEvent )( const VOID * pName, INT iEventType ),
    STATUS (* Set_EventsBits )( NU_EVENT_GROUP * group, UNSIGNED uEvents, OPTION operation ), VOID * Protect ,
    VOID (* SendGooseFun )( VOID ), INT iHardExtendCount );
/* ���ڼ�������� */
VOID OUTC_CheckMain( VOID );
/* �������ִ�к��� */
VOID OUTC_CheckDrive( VOID );
/* ��������̵������غ��� */
VOID OUTC_CheckStartUpReturn( VOID );
/* ���ó��ڼ̵�������λ */
VOID OUTC_Check_SetPickupBits( INT GroupNo, UNSIGNED uBits );
/* �����̵����������� */
VOID OUTC_Check_StartDriver( UNSIGNED uFlag );
/* ������������ */
INT COUTC_RelayDriver( INT GroupNumber, UNSIGNED RelayBits );
/* ����ִ�к��� */
VOID OUTC_DriveFunction( VOID );
/* ���ÿ�����״̬�仯��¼������λ���� */
INT OUTC_CheckSetOutLogMaskBits ( INT iGroupNum, UNSIGNED MaskBits );
/* ���ñ������ڱ�־(iFlag:1�ñ������ڱ�־,iFlag:0��������ڱ�־) */
INT OUT_SetClockRelayFlag( INT iFlag );
/* ��ȡ�������ڱ�־(iFlag:1��������,iFlag:0����������) */
INT OUT_GetClockRelayFlag( INT * iFlag );
/* ��ȡ����λ״̬ */
UNSIGNED OUT_GetOutPickupBits( INT GroupNo );
/* ��ȡ�������� */
INT OUTC_GetOutGroupCount( INT * iTotalCount, INT * iHardCount );
/* �����ִ��λ�仯��麯�� */
INT OUTC_SoftOutExBitsCheck( VOID );

#ifdef __cplusplus
}
#endif

#endif
