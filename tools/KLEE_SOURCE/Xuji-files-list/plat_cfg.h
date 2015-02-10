
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
/*      plat_cfg.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      PLAT - Platform Settings                                         */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains platform configuration settings, compile-time */
/*      options, etc                                                     */
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
/*      ZhideYang.      02-05-2010      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

/* Check to see if this file has been included already.  */

#ifndef PLAT_CFG_H
#define PLAT_CFG_H

#ifdef __cplusplus
/* C declarations in C++     */
extern "C" {
#endif

#define XJ_PLAT_NPU808_INCLUDED         XJ_TRUE
#define XJ_PLAT_WINDOWS_INCLUDED        XJ_FALSE



#if XJ_PLAT_NPU808_INCLUDED
//#include "plat_specific/plat_npu808/plat_npu808_defs.h"
//#include "plat_specific/plat_npu808/inc/cpld1.h"
//#include "plat_specific/inc/cpc_defs.h"
//#include "plat_specific/inc/pmi_extr.h"
//#include "plat_specific/inc/cem_extr.h"
//#include "plat_specific/inc/crm_extr.h"
//#include "plat_specific/inc/cwm_extr.h"
#endif

#if XJ_PLAT_WINDOWS_INCLUDED
//#include "plat_specific/plat_win32/plat_win32_defs.h"
//#include "plat_specific/plat_win32/inc/cpld1.h"
//#include "plat_specific/inc/cpc_defs.h"
//#include "plat_specific/inc/pmi_extr.h"
//#include "plat_specific/inc/cem_extr.h"
//#include "plat_specific/inc/crm_extr.h"
//#include "plat_specific/inc/cwm_extr.h"
#endif

#if 0
/* Define constants for use in service parameters.  */
#define XJ_AND                              NU_AND
#define XJ_AND_CONSUME                      NU_AND_CONSUME
#define XJ_DISABLE_TIMER                    NU_DISABLE_TIMER
#define XJ_ENABLE_TIMER                     NU_ENABLE_TIMER
#define XJ_FIFO                             NU_FIFO
#define XJ_FIXED_SIZE                       NU_FIXED_SIZE
#define XJ_NO_PREEMPT                       NU_NO_PREEMPT
#define XJ_NO_START                         NU_NO_START
#define XJ_NO_SUSPEND                       NU_NO_SUSPEND
#define XJ_NULL                             NU_NULL
#define XJ_OR                               NU_OR
#define XJ_OR_CONSUME                       NU_OR_CONSUME
#define XJ_PREEMPT                          NU_PREEMPT
#define XJ_PRIORITY                         NU_PRIORITY
#define XJ_START                            NU_START
#define XJ_SUSPEND                          NU_SUSPEND
#define XJ_VARIABLE_SIZE                    NU_VARIABLE_SIZE
#endif

#ifdef __cplusplus
/* End of C declarations */
}
#endif  /* __cplusplus */

#endif  /* !PLAT_CFG */
