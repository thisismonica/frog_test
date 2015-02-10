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
*       esal_ge_int_defs.h
*
*   COMPONENT
*
*       ESAL - Embedded Software Abstraction Layer
*
*   DESCRIPTION
*
*       This file contains generic defines, structures, data types, etc
*       related to interrupt control
*
*   DATA STRUCTURES
*
*       None
*
*   DEPENDENCIES
*
*       esal_ar_int_defs.h                  Embedded Software
*                                           Abstraction Layer architecture
*                                           interrupt defines
*       esal_dp_int_defs.h                  Embedded Software
*                                           Abstraction Layer dev platform
*                                           interrupt defines
*       esal_pr_int_defs.h                  Embedded Software
*                                           Abstraction Layer processor
*                                           interrupt defines
*
***********************************************************************/

#ifndef         ESAL_GE_INT_DEFS_H
#define         ESAL_GE_INT_DEFS_H

/* Include all required header files */
#include        "esal/inc/esal_ar_int_defs.h"
#include        "esal/inc/esal_pr_int_defs.h"
#include        "inc/esal_dp_int_defs.h"

/* Map generic APIs to lower-level component */
#define         ESAL_GE_INT_FAST_ALL_ENABLE         ESAL_AR_INT_FAST_ALL_ENABLE
#define         ESAL_GE_INT_FAST_ALL_DISABLE        ESAL_AR_INT_FAST_ALL_DISABLE
#define         ESAL_GE_INT_ALL_DISABLE             ESAL_AR_INT_ALL_DISABLE
#define         ESAL_GE_INT_ALL_RESTORE             ESAL_AR_INT_ALL_RESTORE
#define         ESAL_GE_INT_CONTROL_VARS            ESAL_AR_INT_CONTROL_VARS
#define         ESAL_GE_INT_BITS_SET                ESAL_AR_INT_BITS_SET
#define         ESAL_GE_INT_DISABLE_BITS            ESAL_AR_INTERRUPTS_DISABLE_BITS
#define         ESAL_GE_INT_ENABLE_BITS             ESAL_AR_INTERRUPTS_ENABLE_BITS

/* Define enumerated type for interrupt trigger types */
typedef enum
{
    ESAL_TRIG_NOT_SUPPORTED,
    ESAL_TRIG_RISING_EDGE,
    ESAL_TRIG_FALLING_EDGE,
    ESAL_TRIG_LEVEL_LOW,
    ESAL_TRIG_LEVEL_HIGH,
    ESAL_TRIG_RISING_FALLING_EDGES,
    ESAL_TRIG_HIGH_LOW_RISING_FALLING_EDGES

} ESAL_GE_INT_TRIG_TYPE;

/* Define constant to use if priorities being assigned to interrupt sources
   is not supported */
#define         ESAL_PRI_NOT_SUPPORTED              (INT)0x00000FFF

/* Define macro used to get maximum ESAL interrupt vector ID */
#define         ESAL_GE_INT_MAX_VECTOR_ID_GET()     (ESAL_DP_INT_VECTOR_ID_DELIMITER-1)

/* Check if processor level macro exists to wait for an interrupt */
#ifdef          ESAL_PR_INT_WAIT

/* Set generic macro to use the processor specific macro to wait for an interrupt */
#define         ESAL_GE_INT_WAIT                    ESAL_PR_INT_WAIT

#else

/* Define generic way to wait for an interrupt */
#define         ESAL_GE_INT_WAIT()                  while(1){}

#endif  /* ESAL_PR_INT_WAIT */

/* Define all externally accessible, interrupt related function prototypes */
VOID            ESAL_GE_INT_All_Disable(VOID);
INT             ESAL_GE_INT_Global_Set(INT new_value);
INT             ESAL_GE_INT_Enable(INT vector_id,
                                   ESAL_GE_INT_TRIG_TYPE trigger_type,
                                   INT priority);
INT             ESAL_GE_INT_Disable(INT vector_id);
INT             ESAL_AR_INT_Enable(INT vector_id,
                                   ESAL_GE_INT_TRIG_TYPE trigger_type,
                                   INT priority);
INT             ESAL_AR_INT_Disable(INT vector_id);
VOID            ESAL_PR_INT_All_Disable(VOID);
INT             ESAL_PR_INT_Enable(INT vector_id,
                                   ESAL_GE_INT_TRIG_TYPE trigger_type,
                                   INT priority);
INT             ESAL_PR_INT_Disable(INT vector_id);
VOID            ESAL_DP_INT_All_Disable(VOID);
INT             ESAL_DP_INT_Enable(INT vector_id,
                                   ESAL_GE_INT_TRIG_TYPE trigger_type,
                                   INT priority);
INT             ESAL_DP_INT_Disable(INT vector_id);

#endif  /* ESAL_GE_INT_DEFS_H */
