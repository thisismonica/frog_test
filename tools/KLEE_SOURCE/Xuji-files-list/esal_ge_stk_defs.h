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
*       esal_ge_stk_defs.h
*
*   COMPONENT
*
*       ESAL - Embedded Software Abstraction Layer
*
*   DESCRIPTION
*
*       This file contains generic defines, structures, data types, etc
*       related to stacks
*
*   DATA STRUCTURES
*
*       None
*
*   DEPENDENCIES
*
*       esal_ar_stk_defs.h                  Embedded Software
*                                           Abstraction Layer architecture
*                                           stack related definitions
*       esal_ts_stk_defs.h                  Embedded Software
*                                           Abstraction Layer toolset
*                                           stack related definitions
*
***********************************************************************/

#ifndef         ESAL_GE_STK_DEFS_H
#define         ESAL_GE_STK_DEFS_H

/* Include required header files */
#include        "esal/inc/esal_ar_stk_defs.h"
#include        "esal/inc/esal_ts_stk_defs.h"
#include        "esal/inc/esal_ts_rte_defs.h"

/* Map generic APIs to lower-level component */
#define         ESAL_GE_STK_Unsolicited_Set         ESAL_AR_STK_Unsolicited_Set
#define         ESAL_GE_STK_Unsolicited_Restore     ESAL_AR_STK_Unsolicited_Restore
#define         ESAL_GE_STK_SYSTEM_SIZE             ESAL_AR_SYSTEM_STACK_SIZE
#define         ESAL_GE_STK_Solicited_Set           ESAL_TS_STK_Solicited_Set
#define         ESAL_GE_STK_Solicited_Restore       ESAL_TS_STK_Solicited_Restore
#define         ESAL_GE_STK_Solicited_Switch        ESAL_TS_STK_Solicited_Switch

/* Define all externally accessible, stack related function prototypes */
VOID            ESAL_GE_STK_Initialize(VOID (*unsol_stk_switch_entry)(VOID));
VOID            *ESAL_GE_STK_System_SP_Start_Get(VOID);
VOID            *ESAL_GE_STK_System_SP_End_Get(VOID);
VOID            *ESAL_AR_STK_Unsolicited_Set(VOID *start_addr,
                                             VOID *end_addr,
                                             VOID (*entry_function)(VOID));
VOID            ESAL_AR_STK_Unsolicited_Restore(VOID *stack_ptr);
VOID            ESAL_AR_STK_Unsolicited_Switch(VOID **stack_ptr);
VOID            *ESAL_TS_STK_Solicited_Set(VOID *start_addr,
                                           VOID *end_addr,
                                           VOID (*entry_function)(VOID));
VOID            ESAL_TS_STK_Solicited_Restore(VOID *stack_ptr);
VOID            ESAL_TS_STK_Solicited_Switch(VOID *call_back_param,
                                             VOID (*call_back)(VOID),
                                             VOID **stack_ptr);
VOID            ESAL_AR_STK_Startup_SP_Set(VOID);

/* Define externally accessed data */
extern          VOID    (*ESAL_GE_STK_Unsol_Switch_OS_Entry)(VOID);
extern          VOID    *ESAL_GE_STK_System_SP;
extern          INT     ESAL_GE_STK_Unsol_Switch_Req;

/* Map generic system stack switch to appropriate ESAL function */
#define         ESAL_GE_STK_SYSTEM_SP_SET()         ESAL_TS_RTE_SP_WRITE(ESAL_GE_STK_System_SP)

/* Define the maximum and minimum sizes of a stack frame */
#define         ESAL_GE_STK_MAX_FRAME_SIZE          sizeof(ESAL_AR_STK)
#define         ESAL_GE_STK_MIN_FRAME_SIZE          sizeof(ESAL_TS_STK)

/* Define macros to get offsets of various registers within the stack frame structures */
#define         ESAL_GE_STK_UNSOL_OFFSET(reg_name)  ((INT)&(((ESAL_AR_STK *)ESAL_NULL)->reg_name))
#define         ESAL_GE_STK_SOL_OFFSET(reg_name)    ((INT)&(((ESAL_TS_STK *)ESAL_NULL)->reg_name))

/* Define macro to set / clear flag showing an unsolicited switch is required */
#define         ESAL_GE_STK_UNSOL_SWITCH_ENABLE()   ESAL_GE_STK_Unsol_Switch_Req = ESAL_TRUE
#define         ESAL_GE_STK_UNSOL_SWITCH_DISABLE()  ESAL_GE_STK_Unsol_Switch_Req = ESAL_FALSE

/* Defines for stack types */
#define         ESAL_GE_STK_TS_TYPE                 0
#define         ESAL_GE_STK_AR_TYPE                 1

/* Define a generic alignment mask used to obtain a specified toolset required alignment */
#define         ESAL_GE_STK_ALIGN_MASK              (~(ESAL_TS_REQ_STK_ALIGNMENT - 1))

/* Define generic macro (based on architecture integer size) to read the
   stack type from top of stack */
#if             (ESAL_TS_INTEGER_SIZE == 16)

/* Read 16 bit stack type */
#define         ESAL_GE_STK_TYPE_GET(stack_ptr)     ESAL_GE_MEM_READ16(stack_ptr)

#elif           (ESAL_TS_INTEGER_SIZE == 32)

/* Read 32 bit stack type */
#define         ESAL_GE_STK_TYPE_GET(stack_ptr)     ESAL_GE_MEM_READ32(stack_ptr)

#elif           (ESAL_TS_INTEGER_SIZE == 64)

#if             (ESAL_TS_64BIT_SUPPORT == ESAL_TRUE)

/* Read 64 bit stack type */
#define         ESAL_GE_STK_TYPE_GET(stack_ptr)     ESAL_GE_MEM_READ64(stack_ptr)

#else

/* Generate error - tools don't support 64 bit data type */
#error          Toolset does not support 64-bit types (esal_ts_cfg.h)

#endif  /* ESAL_TS_64BIT_SUPPORT == ESAL_TRUE */

#else

/* Generate error - no valid method to retrieve stack type */
#error          No valid method to get stack type (esal_ge_stk_defs.h)

#endif  /* ESAL_TS_INTEGER_SIZE */

/* Define macro used to align stack end address based on data pointer size */
#if (ESAL_TS_DATA_PTR_SIZE <= 16)

/* Generic macro to align stack end address when stack grows down */
#define         ESAL_GE_STK_ALIGN(end_stk_addr)                                 \
                                (VOID *)((UINT16)end_stk_addr & (UINT16)ESAL_GE_STK_ALIGN_MASK)

#elif (ESAL_TS_DATA_PTR_SIZE <= 32)

/* Generic macro to align stack end address when stack grows down */
#define         ESAL_GE_STK_ALIGN(end_stk_addr)                                 \
                                (VOID *)((UINT32)end_stk_addr & (UINT32)ESAL_GE_STK_ALIGN_MASK)

#elif (ESAL_TS_DATA_PTR_SIZE <= 64)

#if (ESAL_TS_64BIT_SUPPORT == ESAL_TRUE)

/* Generic macro to align stack end address when stack grows down */
#define         ESAL_GE_STK_ALIGN(end_stk_addr)                                 \
                                (VOID *)((UINT64)end_stk_addr & (UINT64)ESAL_GE_STK_ALIGN_MASK)

#else

/* Generate error - toolset doesn't support 64-bit operations */
#error          Toolset does not support 64-bit operations (esal_ts_cfg.h)

#endif  /* ESAL_TS_64BIT_SUPPORT == ESAL_TRUE */

#endif  /* ESAL_TS_DATA_PTR_SIZE */

/* Check if toolset has specific macro for preventing return optimizations */
#ifdef          ESAL_TS_STK_NO_RETURN

/* Use toolset specific method for generic macro */
#define         ESAL_GE_STK_NO_RETURN               ESAL_TS_STK_NO_RETURN

#else

/* Define generic method to prevent return optimizations */
#define         ESAL_GE_STK_NO_RETURN()             while(1){}

#endif  /* ESAL_TS_STK_NO_RETURN */

/* Check if toolset solicited stack reset exists */
#ifndef         ESAL_TS_STK_SOLICITED_RESET

/* Define the solicited stack reset to just use the solicited stack set
   routine */
#define         ESAL_GE_STK_SOLICITED_RESET         ESAL_TS_STK_Solicited_Set

#else

/* Define the solicited stack reset to just use the solicited stack reset
   routine */
#define         ESAL_GE_STK_SOLICITED_RESET         ESAL_TS_STK_SOLICITED_RESET

#endif  /* !ESAL_TS_STK_SOLICITED_RESET */

#endif  /* ESAL_GE_STK_DEFS_H */
