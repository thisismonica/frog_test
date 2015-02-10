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
*       tm_extr.h
*
*   COMPONENT
*
*       TM - Timer Management
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
*       tm_defs.h                           Timer Management constants
*
***********************************************************************/

#include        "plus/inc/tm_defs.h"        /* Include TM constants      */


/* Check to see if the file has been included already.  */

#ifndef TM_EXTR
#define TM_EXTR

#ifdef          __cplusplus

/* C declarations in C++     */
extern          "C" {

#endif

/*  Initialization functions.  */

VOID            TMIT_Initialize(VOID);


/* Error checking for supplemental functions.  */

STATUS          TMSE_Create_Timer(NU_TIMER *timer_ptr, CHAR *name,
                        VOID (*expiration_routine)(UNSIGNED), UNSIGNED id,
                        UNSIGNED initial_time, UNSIGNED reschedule_time,
                        OPTION enable);
STATUS          TMSE_Delete_Timer(NU_TIMER *timer_ptr);
STATUS          TMSE_Reset_Timer(NU_TIMER *timer_ptr,
                        VOID (*expiration_routine)(UNSIGNED),
                        UNSIGNED initial_time, UNSIGNED reschedule_time,
                        OPTION enable);
STATUS          TMSE_Control_Timer(NU_TIMER *timer_ptr, OPTION enable);


/* Supplemental processing functions.  */

STATUS          TMS_Create_Timer(NU_TIMER *timer_ptr, CHAR *name,
                        VOID (*expiration_routine)(UNSIGNED), UNSIGNED id,
                        UNSIGNED initial_time, UNSIGNED reschedule_time,
                        OPTION enable);
STATUS          TMS_Delete_Timer(NU_TIMER *timer_ptr);
STATUS          TMS_Reset_Timer(NU_TIMER *timer_ptr,
                        VOID (*expiration_routine)(UNSIGNED),
                        UNSIGNED initial_time, UNSIGNED reschedule_timer,
                        OPTION enable);
STATUS          TMS_Control_Timer(NU_TIMER *timer_ptr, OPTION enable);


/* Core processing functions.  */

VOID            TMC_Init_Task_Timer(TM_TCB *timer, VOID *information);
VOID            TMC_Timer_HISR(VOID);
VOID            TMC_Stop_Timer(TM_TCB *timer);
VOID            TMC_Start_Timer(TM_TCB *timer, UNSIGNED time);

/* Information retrieval functions.  */

UNSIGNED        TMF_Established_Timers(VOID);
STATUS          TMF_Timer_Information(NU_TIMER *timer_ptr, CHAR *name,
                  OPTION *enable, UNSIGNED *expirations, UNSIGNED *id,
                  UNSIGNED *initial_time, UNSIGNED *reschedule_time);
UNSIGNED        TMF_Timer_Pointers(NU_TIMER **pointer_list,
                                                UNSIGNED maximum_pointers);
STATUS          TMF_Get_Remaining_Time(NU_TIMER *timer_ptr, UNSIGNED
                                       *remaining_time);

/* Define macro for commonly used stop task timer functionality */
#define         TMC_Stop_Task_Timer(timer)                          \
                        if (((TM_TCB *)timer) -> tm_next_timer)     \
                        {                                           \
                            TMC_Stop_Timer((TM_TCB *)timer);        \
                        }

/* Define macro for commonly used start task timer functionality */
#define         TMC_Start_Task_Timer        TMC_Start_Timer

/* Target dependent functions.  */

VOID            TMCT_Adjust_Timer(UNSIGNED new_value);
VOID            TMCT_Enable_Timer(UNSIGNED);
VOID            TMCT_Timer_Interrupt(INT vector);

/* Externally reference global data */
extern volatile UNSIGNED    TMD_Timer;
extern volatile UNSIGNED    TMD_System_Clock;
extern volatile INT         TMD_Timer_State;
extern TC_TCB * volatile    TMD_Time_Slice_Task;

/* Determine if pointers / 32-bit values are accessible with a single instruction.
   If so, just reference the pointer / 32-bit value directly.  Otherwise, call
   the target dependent service.  */
#if (ESAL_AR_32BIT_ACCESS == 1)

/* Macro definitions for architectures supporting single instruction
   access to 32-bit values */
#define         TMCT_Set_Clock(new_value)       TMD_System_Clock = new_value
#define         TMCT_Retrieve_Clock()           TMD_System_Clock
#define         TMCT_Increment_Clock()          TMD_System_Clock++
#define         TMCT_Read_Timer()               TMD_Timer

#else

/* Define function prototypes */
VOID            TMCT_Set_Clock(UNSIGNED);
UNSIGNED        TMCT_Retrieve_Clock(VOID);
VOID            TMCT_Increment_Clock(VOID);
UNSIGNED        TMCT_Read_Timer(VOID);

#endif  /* ESAL_AR_32BIT_ACCESS == 1 */

#if (ESAL_AR_PTR_ACCESS == 1)

/* Macro definitions for architectures supporting single instruction
   access to pointers */
#define         TMCT_Retrieve_TS_Task()         TMD_Time_Slice_Task

#else

/* Define function prototypes */
NU_TASK         *TMCT_Retrieve_TS_Task(VOID);

#endif  /* ESAL_AR_PTR_ACCESS == 1 */

#ifdef          __cplusplus

/* End of C declarations */
}

#endif  /* __cplusplus */

#endif  /* !TM_EXTR */
