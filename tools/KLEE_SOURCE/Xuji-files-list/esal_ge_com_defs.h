/***********************************************************************
*
*            Copyright 1993 Mentor Graphics Corporation
*                         All Rights Reserved.
*
* THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS
* THE PROPERTY OF MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS
* SUBJECT TO LICENSE TERMS.
*
************************************************************************

************************************************************************
*
*   FILE NAME
*
*       esal_ge_com_defs.h
*
*   COMPONENT
*
*       ESAL - Embedded Software Abstraction Layer
*
*   DESCRIPTION
*
*       This file contains generic defines, structures, data types, etc
*       common to all Embedded Software Abstraction Layer components
*
*   DATA STRUCTURES
*
*       None
*
*   DEPENDENCIES
*
*       esal_ar_cfg.h                       Embedded Software
*                                           Abstraction Layer architecture
*                                           configuration settings
*       esal_co_cfg.h                       Embedded Software
*                                           Abstraction Layer core
*                                           configuration settings
*       esal_dp_cfg.h                       Embedded Software
*                                           Abstraction Layer dev platform
*                                           configuration settings
*       esal_ge_cfg.h                       Embedded Software
*                                           Abstraction Layer generic
*                                           configuration settings
*       esal_pr_cfg.h                       Embedded Software
*                                           Abstraction Layer processor
*                                           configuration settings
*       esal_ts_cfg.h                       Embedded Software
*                                           Abstraction Layer toolset
*                                           configuration settings
*
***********************************************************************/

#ifndef         ESAL_GE_COM_DEFS_H
#define         ESAL_GE_COM_DEFS_H

/* Define NULL value */
#define         ESAL_NULL                       0

/* Defines for TRUE / FALSE conditions */
#define         ESAL_FALSE                      0
#define         ESAL_TRUE                       1

/* Define standard data types.  These definitions allow software components to
   perform in the same manner on different target platforms.  */
typedef         signed int                      INT;
typedef         unsigned int                    UINT;
typedef         signed char                     INT8;
typedef         unsigned char                   UINT8;
typedef         signed short                    INT16;
typedef         unsigned short                  UINT16;
typedef         signed long                     INT32;
typedef         unsigned long                   UINT32;
#define         VOID                            void

/* Define type used to cast void pointers when doing pointer math */
typedef         UINT8 *                          VOID_CAST;

/* Define register variable definition.  This define
   can be used to specify local variables and parameters
   that could be placed into registers to increase performance
   (speed and size) */
#define         ESAL_REG                        register

/* Define macro to eliminate warnings for unused parameters */
#define         ESAL_UNUSED_PARAM(parameter)    (VOID)parameter

/* Include configuration header files */
#include        "esal/inc/esal_ge_cfg.h"
#include        "esal/inc/esal_ar_cfg.h"
#include        "esal/inc/esal_co_cfg.h"
#include        "esal/inc/esal_pr_cfg.h"
#include        "esal/inc/esal_ts_cfg.h"
#include        "inc/esal_dp_cfg.h"

/* Check if toolset supports 64-bit data types */
#if             (ESAL_TS_64BIT_SUPPORT == ESAL_TRUE)

typedef         unsigned long long               UINT64;
typedef         signed long long                 INT64;

#endif  /* ESAL_TS_64BIT_SUPPORT == ESAL_TRUE */

#endif  /* ESAL_GE_COM_DEFS_H */
