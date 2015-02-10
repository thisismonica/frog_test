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
/*      pmi_extr.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      PMI - Platform Initialize                                        */
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
/*      ZhideYang.      04-12-2007      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"

/* Check to see if the file has been included already.  */

#ifndef PMI_EXTR_H
#define PMI_EXTR_H

#ifdef __cplusplus
extern "C" {
#endif

/*=======================================================================*/
/* 设置应用程序配置参数 */
INT PMI_SetApplicationCfg( VOID );
/* 平台配置参数的初始化 */
INT PMI_InitializePlatCfg( VOID );
/* 获取装置功能选择 */
INT PMI_get_device_fun_select( VOID );
/* 初始化模拟量参数 */
INT PMI_InitializeChannelPara( VOID );
/* 初始化应用程序 */
INT PMI_InitializeApplication( VOID );
/* 初始化循环缓冲区 */
INT PMI_InitializeCircleMemory( VOID );
/* 数据表的初始化 */
INT PMI_InitializeTable( VOID );
/* 获取数据表的数据信息 */
INT PMI_GetTableDataInformation( VOID );
/* 初始化装置运行灯 */
INT PMI_InitializeDeviceLed( VOID );
/* 设置告警灯与运行灯状态函数 */
INT PMI_SetAlarmANDRunLedStatus( VOID );
/* 设置平台点告警灯事件位 */
INT  PMI_SetPlatAlarmLedBits( UNSIGNED uPlatAlarmBits );
/* 设置平台运行灯异常事件位函数 */
INT PMI_SetPlatRunLedAbnorBits( UNSIGNED uRunAbnorEventBits );

#ifdef __cplusplus
}
#endif

#endif
