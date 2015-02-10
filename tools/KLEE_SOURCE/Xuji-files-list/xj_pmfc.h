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
/*      xj_pmfc.h                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      PMF - Platform HMI Function                                      */
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
/*      ZhideYang.      02-20-2008      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"
//#include "plat/inc/xj_ctbc.h"
//#include "plat/inc/xj_cioc.h"
//#include "plat/inc/xj_ctmc.h"
//#include "plat/inc/xj_csfc.h"
//#include "plat/inc/xj_crcc.h"

/* Check to see if the file has been included already.  */

#ifndef XJ_PMFC_H
#define XJ_PMFC_H

#ifdef __cplusplus
extern "C" {
#endif

/*=======================================================================*/
/* 初始化平台提供给HMI使用的服务函数表 */
VOID PMFC_PlatHMIFunList_Initialize( VOID );
/* 连接HMI程序 */
INT PMFC_LinkHMIApplication(  UNSIGNED StartAddr, UNSIGNED Length, VOID(* WatchDogReset )( VOID ) );
/* 初始化HMI程序 */
INT PMFC_InitializeHMIApplication( VOID * mem_pool, VOID * uncached_mem_pool );
/* 引导HMI程序 */
INT PMFC_LeadHMIApplication( VOID );

#ifdef __cplusplus
}
#endif

#endif
