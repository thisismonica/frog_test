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
*       esal_ge_isr_defs.h
*
*   COMPONENT
*
*       ESAL - Embedded Software Abstraction Layer
*
*   DESCRIPTION
*
*       This file contains generic defines, structures, data types, etc
*       related to interrupt servicing
*
*   DATA STRUCTURES
*
*       None
*
*   DEPENDENCIES
*
*       esal_ar_isr_defs.h                  Embedded Software
*                                           Abstraction Layer architecture
*                                           interrupt servicing defines
*       esal_dp_isr_defs.h                  Embedded Software
*                                           Abstraction Layer development
*                                           platform interrupt servicing
*                                           defines
*       esal_pr_isr_defs.h                  Embedded Software
*                                           Abstraction Layer processor
*                                           interrupt servicing defines
*
***********************************************************************/

#ifndef         ESAL_GE_ISR_DEFS_H
#define         ESAL_GE_ISR_DEFS_H

/* Include all required header files */
#include        "esal/inc/esal_ar_isr_defs.h"
#include        "esal/inc/esal_pr_isr_defs.h"
#include        "inc/esal_dp_isr_defs.h"

/* Define all externally accessible, interrupt related function prototypes */
VOID            ESAL_GE_ISR_Initialize(VOID (*default_isr)(INT),
                                       VOID (*default_except)(INT, VOID *),
                                       VOID **(*os_isr_entry)(INT, VOID *),
                                       VOID (*os_nested_isr_entry)(INT));
VOID            ESAL_AR_ISR_Initialize(VOID);
VOID            ESAL_AR_ISR_Vector_Table_Install(VOID);
VOID            ESAL_DP_ISR_Initialize(VOID);
VOID            ESAL_PR_ISR_Initialize(VOID);

/* Externally referenced global data */
extern VOID     **(*ESAL_GE_ISR_OS_Entry)(INT vector, VOID *stack_ptr);
extern VOID     (*ESAL_GE_ISR_OS_Nested_Entry)(INT vector);
extern VOID     (*ESAL_GE_ISR_Interrupt_Handler[])(INT vector);
extern VOID     (*ESAL_GE_ISR_Exception_Handler[])(INT except_num, VOID *frame_ptr);
extern INT      ESAL_GE_ISR_Executing;
extern VOID     *ESAL_GE_ISR_Interrupt_Vector_Data[ESAL_DP_INT_VECTOR_ID_DELIMITER];
extern VOID     *ESAL_GE_ISR_Exception_Vector_Data[ESAL_AR_EXCEPT_VECTOR_ID_DELIMITER];

/* Define macro used to return back to the OS after interrupt handling.  If the OS
   must be returned to instead of returning to the point of interrupt, this macro
   will perform this operation based on the architectural requirements.
   NOTE:  Some architectures require a "return from interrupt" to be execute in
          order for the execution state to be restored when returning to the
          OS.  This is configured in esal_ar_cfg.h */
#if (ESAL_AR_ISR_RTI_MANDATORY == ESAL_FALSE)

/* No special requirements for this architecture - simply expand this macro into
   a function call */
#define         ESAL_GE_ISR_OS_RETURN(os_return_func_ptr)                                   \
                        os_return_func_ptr()

#else

/* Define function prototype for architecture specific return from ISR function */
VOID            ESAL_AR_ISR_Return(VOID (*)(VOID));

/* This architecture must return to the OS using "return from interrupt" or equivalent
   method - expand this macro to call ESAL service to perform this function */
#define         ESAL_GE_ISR_OS_RETURN(os_return_func_ptr)                                   \
                        ESAL_AR_ISR_Return(os_return_func_ptr)

#endif  /* ESAL_AR_ISR_RTI_MANDATORY == ESAL_FALSE */

/* Define macro used to determine if within interrupt service routine */
#ifndef         ESAL_GE_ISR_EXECUTING
#define         ESAL_GE_ISR_EXECUTING()                     ESAL_GE_ISR_Executing
#endif          /* !ESAL_GE_ISR_EXECUTING */

/* Define macros used to identify start / end of interrupt service routine */
#ifndef         ESAL_GE_ISR_START
#define         ESAL_GE_ISR_START()                         ESAL_GE_ISR_Executing = ESAL_TRUE
#endif          /* !ESAL_GE_ISR_START */
#ifndef         ESAL_GE_ISR_END
#define         ESAL_GE_ISR_END()                           ESAL_AR_INT_FAST_ALL_DISABLE();     \
                                                            ESAL_GE_ISR_Executing = ESAL_FALSE
#endif          /* !ESAL_GE_ISR_END */

/* Define macros to get / set / execute interrupt service routine for ESAL interrupt vectors */
#define         ESAL_GE_ISR_HANDLER_GET(num)                (ESAL_GE_ISR_Interrupt_Handler[(num)])
#define         ESAL_GE_ISR_HANDLER_SET(num,ptr)            ESAL_GE_ISR_Interrupt_Handler[(num)]=(ptr)
#define         ESAL_GE_ISR_HANDLER_EXECUTE(num)            ESAL_GE_ISR_Interrupt_Handler[(num)](num)

/* Define macros for get / set exception handlers for ESAL exception vectors */
#define         ESAL_GE_EXCEPT_HANDLER_SET(num,ptr)         ESAL_GE_ISR_Exception_Handler[(num)]=(ptr)
#define         ESAL_GE_EXCEPT_HANDLER_GET(num)             (ESAL_GE_ISR_Exception_Handler[(num)])

/* Define macros to get / set data associated with each interrupt service routine vector */
#define         ESAL_GE_ISR_VECTOR_DATA_GET(vector)         ESAL_GE_ISR_Interrupt_Vector_Data[(vector)]
#define         ESAL_GE_ISR_VECTOR_DATA_SET(vector,data)    ESAL_GE_ISR_Interrupt_Vector_Data[(vector)]=(data)

/* Define macros for get / set data associated with each exception vector */
#define         ESAL_GE_EXCEPT_VECTOR_DATA_GET(vector)      ESAL_GE_ISR_Exception_Vector_Data[(vector)]
#define         ESAL_GE_EXCEPT_VECTOR_DATA_SET(vector,data) ESAL_GE_ISR_Exception_Vector_Data[(vector)]=(data)

#endif  /* ESAL_GE_ISR_DEFS_H */
