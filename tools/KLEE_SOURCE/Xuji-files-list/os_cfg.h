
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
/*      os_cfg.h                                        Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      OS - Operate System Settings                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains Operate System configuration settings,        */
/*      compile-time options, etc                                        */
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
/*      ZhideYang.      04-13-2007      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

/* Check to see if this file has been included already.  */

#ifndef OS_CFG_H
#define OS_CFG_H

#ifdef __cplusplus
/* C declarations in C++     */
extern "C" {
#endif

#define OS_TRUE         1           /* TRUE */
#define OS_FALSE        0           /* FALSE */

#define XJ_OS_NUCLEUS_INCLUDED      OS_TRUE
#define XJ_OS_WINDOWS_INCLUDED      OS_FALSE
#define XJ_OS_INTEGRATIVE_INCLUDED  OS_FALSE


#if XJ_OS_NUCLEUS_INCLUDED
#include "plus/nucleus.h"
typedef NU_TASK           * XJ_TASK;
typedef NU_MAILBOX        * XJ_MAILBOX;
typedef NU_QUEUE          * XJ_QUEUE;
typedef NU_PIPE           * XJ_PIPE;
typedef NU_SEMAPHORE      * XJ_SEMAPHORE;
typedef NU_EVENT_GROUP    * XJ_EVENT_GROUP;
typedef NU_PARTITION_POOL * XJ_PARTITION_POOL;
typedef NU_MEMORY_POOL    * XJ_MEMORY_POOL;
typedef NU_HISR           * XJ_HISR;
typedef NU_TIMER          * XJ_TIMER;
typedef NU_PROTECT        * XJ_PROTECT;
typedef NU_DRIVER         * XJ_DRIVER;
typedef NU_DRIVER_REQUEST   XJ_DRIVER_REQUEST;
/*--------------------------------------------*/
/* ƒ£øÈ≈‰÷√ */
#define XJ_OS_INTEGRATIVE_INCLUDED  OS_FALSE
/*--------------------------------------------*/
#endif

#if XJ_OS_WINDOWS_INCLUDED
#include "specific/win32/win32_nucleus.h"
typedef long XJ_TASK;
typedef long XJ_MAILBOX;
typedef long XJ_QUEUE;
typedef long XJ_PIPE;
typedef long XJ_SEMAPHORE;
typedef long XJ_EVENT_GROUP;
typedef long XJ_PARTITION_POOL;
typedef long XJ_MEMORY_POOL;
typedef long XJ_HISR;
typedef long XJ_TIMER;
typedef long XJ_PROTECT;
typedef long XJ_DRIVER;
typedef long XJ_DRIVER_REQUEST;
#endif

#if XJ_OS_INTEGRATIVE_INCLUDED
#include "specific/win32/win32_nucleus.h"
typedef long XJ_TASK;
typedef long XJ_MAILBOX;
typedef long XJ_QUEUE;
typedef long XJ_PIPE;
typedef long XJ_SEMAPHORE;
typedef long XJ_EVENT_GROUP;
typedef long XJ_PARTITION_POOL;
typedef long XJ_MEMORY_POOL;
typedef long XJ_HISR;
typedef long XJ_TIMER;
typedef long XJ_PROTECT;
typedef long XJ_DRIVER;
typedef long XJ_DRIVER_REQUEST;
#endif


/* Define TRUE and FALSE values */
#define XJ_TRUE                             NU_TRUE
#define XJ_FALSE                            NU_FALSE

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



/* Define service completion status constants.  */
#define XJ_SUCCESS                          NU_SUCCESS
#define XJ_UNAVAILABLE                      NU_UNAVAILABLE
#define XJ_NO_MEMORY                        NU_NO_MEMORY
#define XJ_INVALID_ENTRY                    NU_INVALID_ENTRY
#define XJ_END_OF_LOG                       NU_END_OF_LOG
#define XJ_TIMEOUT                          NU_TIMEOUT

/* Define I/O driver constants.  */
#define XJ_INITIALIZE                       NU_INITIALIZE
#define XJ_ASSIGN                           NU_ASSIGN
#define XJ_RELEASE                          NU_RELEASE
#define XJ_INPUT                            NU_INPUT
#define XJ_OUTPUT                           NU_OUTPUT
#define XJ_STATUS                           NU_STATUS

#ifdef __cplusplus
/* End of C declarations */
}
#endif  /* __cplusplus */

#endif  /* !PLAT_CFG */
