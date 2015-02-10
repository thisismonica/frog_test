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
/*      cer_extr.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CER - Common Error Management                                    */
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
/*      pro_defs.h                          Protect Control constants    */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhideYang.      08-13-2008      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"

/* Check to see if the file has been included already.  */

#ifndef CER_EXTR_H
#define CER_EXTR_H

/* �Լ�ʱ�䶨�� */
#define CER_POWER_ERR_RESUME    100   /* power error resume time */
#define CER_POWER_ERR_ALARME    500   /* power error alarm time */
#define CER_AD_ERR_TIME         4     /* A/D error delay time */
#define CER_AD_RESUME_TIME      500   /* A/D error resume time */
#define CER_AD_ERR_COUNT        5     /* A/D error count */
#define CER_AD_ERR_RESUME_COEF  100   /* A/D error resume coef */

/* ��ȡHMI������Ϣ */
UNSIGNED CERC_ReadHMIErrorBits( VOID );
/* ��ȡ������Ϣ */
INT CERC_ReadErrorBits( UNSIGNED * Actual_ErrorBits );
/* �������ݳ�ʼ����� */
VOID CERC_SampleIniCheckMain( VOID );
/* A/D��������� */
VOID CERC_AD_CheckMain( VOID );
/* ��Դ�Լ� */
void CERC_Power_Check( void );
/* �û����ù��ϵƺ��� */
VOID CERC_SetUserEventBits( UNSIGNED uEventBits, INT iFlag );
/* RAM�������ļ��Լ���Ϣ��ʼ�� */
INT CERC_FileInitialize( VOID );
/* RAM�������ļ��Լ� */
VOID * CERC_FileCheck(  VOID );
/* RAM�������ļ��Լ������� */
VOID CERC_FileCheck_Main(  VOID );
/* FALSH�����ڴ���ʼ�� */
INT CERC_FlashCheckInitialize( VOID );
/* FALSH�����ڴ���������� */
INT CERC_FlashCheckMain( VOID );
/* ��ȡFLASH�����ڴ��״̬ */
INT CERC_GetFlashPrartitionBlockStatus( int index, int * retired, int * used, int * empty );
/* ����ֵ�����Լ� */
VOID CERC_SettingCheck_Main( VOID );

#endif
