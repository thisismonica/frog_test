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

/* 自检时间定义 */
#define CER_POWER_ERR_RESUME    100   /* power error resume time */
#define CER_POWER_ERR_ALARME    500   /* power error alarm time */
#define CER_AD_ERR_TIME         4     /* A/D error delay time */
#define CER_AD_RESUME_TIME      500   /* A/D error resume time */
#define CER_AD_ERR_COUNT        5     /* A/D error count */
#define CER_AD_ERR_RESUME_COEF  100   /* A/D error resume coef */

/* 读取HMI错误信息 */
UNSIGNED CERC_ReadHMIErrorBits( VOID );
/* 读取错误信息 */
INT CERC_ReadErrorBits( UNSIGNED * Actual_ErrorBits );
/* 采样数据初始化检测 */
VOID CERC_SampleIniCheckMain( VOID );
/* A/D检测主函数 */
VOID CERC_AD_CheckMain( VOID );
/* 电源自检 */
void CERC_Power_Check( void );
/* 用户设置故障灯函数 */
VOID CERC_SetUserEventBits( UNSIGNED uEventBits, INT iFlag );
/* RAM区程序文件自检信息初始化 */
INT CERC_FileInitialize( VOID );
/* RAM区程序文件自检 */
VOID * CERC_FileCheck(  VOID );
/* RAM区程序文件自检主函数 */
VOID CERC_FileCheck_Main(  VOID );
/* FALSH分区内存块初始化 */
INT CERC_FlashCheckInitialize( VOID );
/* FALSH分区内存块检测主函数 */
INT CERC_FlashCheckMain( VOID );
/* 读取FLASH分区内存块状态 */
INT CERC_GetFlashPrartitionBlockStatus( int index, int * retired, int * used, int * empty );
/* 整定值数据自检 */
VOID CERC_SettingCheck_Main( VOID );

#endif
