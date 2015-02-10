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
*       tc_extr.h
*
*   COMPONENT
*
*       TC - Thread Control
*
*   DESCRIPTION
*
*       This file contains function prototypes of all functions
*       accessible to other components.
*
*   DATA STRUCTURES
*
*       None
*
*   DEPENDENCIES
*
*       tc_defs.h                           Thread Control constants
*
***********************************************************************/

#include        "plus/inc/tc_defs.h"        /* Include TC constants      */


/* Check to see if the file has been included already.  */

#ifndef TC_EXTR
#define TC_EXTR


#ifdef          __cplusplus

/* C declarations in C++     */
extern          "C" {

#endif

/* Define externally referenced variables.   */

extern TC_PROTECT           TCD_System_Protect;
extern VOID * volatile      TCD_Current_Thread;
extern TC_HCB               TCD_Init_Thread;
extern TC_TCB * volatile    TCD_Execute_Task;

/* Initialization functions.  */

VOID            TCIT_Initialize(VOID);


/* Core error checking functions.  */

#if (NU_ERROR_CHECKING == NU_TRUE)

BOOLEAN         TCC_Validate_Resume(OPTION resume_type, NU_TASK *task_ptr);

#endif  /* NU_ERROR_CHECKING == NU_TRUE */
STATUS          TCCE_Activate_HISR(NU_HISR *hisr_ptr);
STATUS          TCCE_Create_Task(NU_TASK *task_ptr, CHAR *name,
                                 VOID (*task_entry)(UNSIGNED, VOID *),
                                 UNSIGNED argc,
                                 VOID *argv, VOID *stack_address,
                                 UNSIGNED stack_size,
                                 OPTION priority, UNSIGNED time_slice,
                                 OPTION preempt, OPTION auto_start);
STATUS          TCCE_Reset_Task(NU_TASK *task_ptr, UNSIGNED argc, VOID *argv);
STATUS          TCCE_Create_HISR(NU_HISR *hisr_ptr, CHAR *name,
                                 VOID (*hisr_entry)(VOID), OPTION priority,
                                 VOID *stack_address, UNSIGNED stack_size);
STATUS          TCCE_Terminate_Task(NU_TASK *task_ptr);
STATUS          TCCE_Delete_Task(NU_TASK *task_ptr);
STATUS          TCCE_Delete_HISR(NU_HISR *hisr_ptr);
STATUS          TCCE_Resume_Service(NU_TASK *task_ptr);
STATUS          TCCE_Suspend_Service(NU_TASK *task_ptr);
VOID            TCCE_Relinquish(VOID);
VOID            TCCE_Task_Sleep(UNSIGNED ticks);
STATUS          TCCE_Validate_Resume(OPTION resume_type, NU_TASK *task_ptr);
INT             TCCE_Suspend_Error(VOID);
STATUS          TCCE_Register_LISR(INT vector, VOID (*new_lisr)(INT),
                                   VOID (**old_lisr)(INT));
STATUS          TCFE_Task_Information(NU_TASK *task_ptr, CHAR *name,
                    DATA_ELEMENT *status, UNSIGNED *scheduled_count,
                    DATA_ELEMENT *priority, OPTION *preempt, UNSIGNED *time_slice,
                    VOID **stack_base, UNSIGNED *stack_size,
                    UNSIGNED *minimum_stack);


/* Supplemental error checking functions.  */

OPTION          TCSE_Change_Priority(NU_TASK *task_ptr, OPTION new_priority);
OPTION          TCSE_Change_Preemption(OPTION preempt);
UNSIGNED        TCSE_Change_Time_Slice(NU_TASK *task_ptr, UNSIGNED time_slice);
UNSIGNED        TCSE_Control_Signals(UNSIGNED signal_enable_mask);
UNSIGNED        TCSE_Receive_Signals(VOID);
STATUS          TCSE_Register_Signal_Handler(VOID (*signal_handler)(UNSIGNED));
STATUS          TCSE_Send_Signals(NU_TASK *task_ptr, UNSIGNED signals);


/* Core processing functions.  */

STATUS          TCC_Terminate_Task(NU_TASK *task_ptr);
STATUS          TCC_Delete_Task(NU_TASK *task_ptr);
STATUS          TCC_Delete_HISR(NU_HISR *hisr_ptr);
STATUS          TCC_Resume_Task(NU_TASK *task_ptr, OPTION suspend_type);
STATUS          TCC_Resume_Service(NU_TASK *task_ptr);
VOID            TCC_Suspend_Task(NU_TASK *task_ptr, OPTION suspend_type,
                                 VOID (*cleanup)(VOID *),
                                 VOID *information, UNSIGNED timeout);
STATUS          TCC_Suspend_Service(NU_TASK *task_ptr);
VOID            TCC_Task_Timeout(NU_TASK *task_ptr);
VOID            TCC_Task_Sleep(UNSIGNED ticks);
VOID            TCC_Relinquish(VOID);
VOID            TCC_Time_Slice(NU_TASK *task_ptr);
NU_TASK        *TCC_Current_Task_Pointer(VOID);
NU_HISR        *TCC_Current_HISR_Pointer(VOID);


/* Supplemental functions.  */

OPTION          TCS_Change_Priority(NU_TASK *task_ptr, OPTION new_priority);
OPTION          TCS_Change_Preemption(OPTION preempt);
UNSIGNED        TCS_Change_Time_Slice(NU_TASK *task_ptr, UNSIGNED time_slice);
UNSIGNED        TCS_Control_Signals(UNSIGNED signal_enable_mask);
UNSIGNED        TCS_Receive_Signals(VOID);
STATUS          TCS_Register_Signal_Handler(VOID (*signal_handler)(UNSIGNED));


/* Information retrieval functions.  */

UNSIGNED        TCF_Established_Tasks(VOID);
UNSIGNED        TCF_Established_HISRs(VOID);
STATUS          TCF_Task_Information(NU_TASK *task, CHAR *name,
                                     DATA_ELEMENT *status,
                                     UNSIGNED *scheduled_count,
                                     OPTION *priority, OPTION *preempt,
                                     UNSIGNED *time_slice, VOID **stack_base,
                                     UNSIGNED *stack_size, UNSIGNED *minimum_stack);
STATUS          TCF_HISR_Information(NU_HISR *hisr, CHAR *name,
                                     UNSIGNED *scheduled_count,
                                     DATA_ELEMENT *priority,
                                     VOID **stack_base, UNSIGNED *stack_size,
                                     UNSIGNED *minimum_stack);
UNSIGNED        TCF_Task_Pointers(NU_TASK **pointer_list,
                                  UNSIGNED maximum_pointers);
UNSIGNED        TCF_HISR_Pointers(NU_HISR **pointer_list,
                                  UNSIGNED maximum_pointers);

/* Core functions with target dependencies */

STATUS          TCCT_Create_Task(NU_TASK *task_ptr, CHAR *name,
                                 VOID (*task_entry)(UNSIGNED, VOID *), UNSIGNED argc,
                                 VOID *argv, VOID *stack_address, UNSIGNED stack_size,
                                 OPTION priority, UNSIGNED time_slice,
                                 OPTION preempt, OPTION auto_start);
STATUS          TCCT_Create_HISR(NU_HISR *hisr_ptr, CHAR *name,
                                 VOID (*hisr_entry)(VOID), OPTION priority,
                                 VOID *stack_address, UNSIGNED stack_size);
STATUS          TCCT_Reset_Task(NU_TASK *task_ptr, UNSIGNED argc, VOID *argv);
STATUS          TCCT_Register_LISR(INT vector, VOID (*new_lisr)(INT),
                                   VOID (**old_lisr)(INT));
UNSIGNED        TCCT_Check_Stack(VOID);
VOID            TCCT_Schedule(VOID);
VOID            TCCT_Protect(TC_PROTECT *protect);
VOID            TCCT_Unprotect(VOID);
VOID            TCCT_Unprotect_Specific(TC_PROTECT *protect);
VOID            TCCT_Control_To_System(VOID);
INT             TCCT_Control_Interrupts(INT new_level);
STATUS          TCCT_Activate_HISR(NU_HISR *hisr_ptr);

#if (NU_STACK_CHECKING == NU_TRUE)

#define NU_CHECK_STACK() TCCT_Check_Stack()

#else /* no stack checking */

#define NU_CHECK_STACK() ((VOID)0)

#endif /* NU_STACK_CHECKING == NU_TRUE */

/* Supplementary functions with target dependencies */

STATUS          TCST_Send_Signals(NU_TASK *task, UNSIGNED signals);

/* Determine if pointers are accessible with a single instruction.  If so,
   just reference the pointer directly.  Otherwise, call the target dependent
   service */

#if (ESAL_AR_PTR_ACCESS == 1)

/* Macro definitions for architectures supporting single instruction
   access to pointers */
#define         TCCT_Current_Thread()               TCD_Current_Thread
#define         TCCT_Set_Execute_Task(task)         TCD_Execute_Task =  task
#define         TCCT_Set_Current_Protect(protect)   \
                    ((TC_TCB *)TCD_Current_Thread) -> tc_current_protect = protect
#define         TCCT_Get_Current_Protect()          \
                   ((TC_TCB *) TCD_Current_Thread) -> tc_current_protect

#else

VOID            *TCCT_Current_Thread(VOID);
VOID            TCCT_Set_Execute_Task(TC_TCB *task);
VOID            TCCT_Set_Current_Protect(TC_PROTECT *protect);
TC_PROTECT      *TCCT_Get_Current_Protect(VOID);

#endif /* ESAL_AR_PTR_ACCESS == 1 */

/* Define pseudo function used by internal components of the system.  */

#define         TCCT_System_Protect()                                           \
                   TCCT_Protect(&TCD_System_Protect)
#define         TCCT_System_Unprotect()                                         \
                   TCCT_Unprotect_Specific(&TCD_System_Protect)
#define         TCCT_Set_Suspend_Protect(protect)                               \
                   ((TC_TCB *) TCD_Current_Thread) -> tc_suspend_protect =      \
                   (TC_PROTECT *) protect
#define         TCC_Task_Priority(task)                                         \
                    ((TC_TCB *) (task)) -> tc_priority

/* Define macro, for internal components, to access current HISR pointer */

#define         TCC_CURRENT_HISR_PTR                                            \
                    ((TC_HCB *)TCD_Current_Thread)

/* Define interrupt locking / unlocking macros based on the interrupt locking
   method used */
#if (NU_GLOBAL_INT_LOCKING == NU_TRUE)

/* The following macro sets the interrupt lock-out level to the globally defined
   level (TCD_Interrupt_Level).  This macro is used in the scheduler and within
   the HISR shell routine. */
#define         TCC_INTERRUPTS_GLOBAL_ENABLE()                                  \
                        ESAL_GE_INT_BITS_SET(TCD_Interrupt_Level)

/* The following macro locks out interrupts without saving the state before
   locking-out.  It is used to guard critical sections of code and is always
   paired with the macro TCC_INTERRUPTS_RESTORE defined below. */
#define         TCC_INTERRUPTS_DISABLE()                                        \
                        ESAL_GE_INT_FAST_ALL_DISABLE()

/* The following macro sets the interrupt lock-out level to the globally defined
   level (TCD_Interrupt_Level).  This macro is used at the end of critical
   sections where a context switch may have occurred.  This macro is always
   paired with the macro TCC_INTERRUPTS_ENABLE defined above. */
#define         TCC_INTERRUPTS_RESTORE()                                        \
                        ESAL_GE_INT_BITS_SET(TCD_Interrupt_Level)

#else

/* The following macro is used to enable interrupts globally.  This macro is used
   in the scheduler and within the HISR shell routine. */
#define         TCC_INTERRUPTS_GLOBAL_ENABLE()                                  \
                        ESAL_GE_INT_FAST_ALL_ENABLE()

/* The following macro locks out interrupts and saves the state before
   locking-out.  It is used to guard critical sections of code and is always
   paired with the macro TCC_INTERRUPTS_RESTORE defined below. */
#define         TCC_INTERRUPTS_DISABLE()                                        \
                        ESAL_GE_INT_CONTROL_VARS                                \
                        ESAL_GE_INT_ALL_DISABLE()

/* The following macro sets the interrupt lock-out level to entry level saved
   by TCC_INTERRUPTS_LOCK.  This macro is used at the end of critical
   sections where a context switch may have occurred.  This macro is always
   paired with the macro TCC_INTERRUPTS_LOCK defined above. */
#define         TCC_INTERRUPTS_RESTORE()                                        \
                        ESAL_GE_INT_ALL_RESTORE()

#endif  /* NU_GLOBAL_INT_LOCKING == NU_TRUE */

/* Nucleus PLUS Timing Test 2 macros / external declarations */
#if (NU_TIME_TEST2_ENABLE == NU_TRUE)

/* Include timing test header file */
#include        "plus_timing/test2/test2_defs.h"

#else

/* Define macro to do nothing */
#define         NU_PLUS_TIMING_TEST2()      (VOID)0

#endif  /* (NU_TIME_TEST2_ENABLE == NU_TRUE) */

#ifdef          __cplusplus

/* End of C declarations */
}

#endif  /* __cplusplus */

#endif  /* !TC_EXTR */
