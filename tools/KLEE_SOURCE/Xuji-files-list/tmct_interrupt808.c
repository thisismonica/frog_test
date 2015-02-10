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
*       tmct_common.c
*
*   COMPONENT
*
*       TM - Timer Management
*
*   DESCRIPTION
*
*       This file contains timer management core functions with target
*       dependencies
*
*   DATA STRUCTURES
*
*       None
*
*   FUNCTIONS
*
*       TMCT_Adjust_Timer                   Adjust count-down timer
*       TMCT_Enable_Timer                   Enable count-down timer
*       [TMCT_Set_Clock]                    Set system clock
*       [TMCT_Retrieve_Clock]               Retrieve system clock
*       [TMCT_Increment_Clock]              Increment system clock
*       [TMCT_Read_Timer]                   Read count-down timer
*       [TMCT_Retrieve_TS_Task]             Retrieve time-sliced task
*                                           ptr
*       TMCT_Timer_Interrupt                Timer interrupt handler
*
*   DEPENDENCIES
*
*       cs_extr.h                           Common Service functions
*       tc_extr.h                           Thread Control functions
*       tm_extr.h                           Timer Control functions
*
***********************************************************************/
#define                     NU_SOURCE_FILE

/* Include necessary header files */
#include                    "plus/inc/cs_extr.h"
#include                    "plus/inc/tc_extr.h"
#include                    "plus/inc/tm_extr.h"

/* Define external inner-component global data references */
extern TC_HCB               TMD_HISR;
UNSIGNED                    TMD_Clock_Disable = NU_FALSE;

INT     TMCT_OS_ClockDisable( VOID );
/***********************************************************************
*
*   FUNCTION
*
*       TMCT_Timer_Interrupt
*
*   DESCRIPTION
*
*       This is the interrupt service routine for the Nucleus OS timer
*       interrupt
*
*   CALLED BY
*
*       ESAL OS Timer Interrupt
*
*   CALLS
*
*       TCCT_Activate_HISR
*
*   INPUTS
*
*       vector                              Vector number of OS timer
*
*   OUTPUTS
*
*       None
*
***********************************************************************/
VOID    TMCT_Timer_Interrupt( INT vector )
{
    R1  TC_TCB  * current_thread;

    /* Perform OS timer end-of-interrupt
       NOTE:  This resets the OS timer interrupt as required */
    ESAL_GE_TMR_OS_TIMER_EOI( vector );

    /* Increment system clock */
    if( TMD_Clock_Disable == NU_FALSE )
        TMCT_Increment_Clock();

    /* Check if application timer is enabled */
    if ( TMD_Timer_State == TM_ACTIVE )
    {
        /* Decrement application timer and determine if expired */
        if (-- TMD_Timer == 0 )
        {
            /* Set timer state to expired */
            TMD_Timer_State = TM_EXPIRED;
        }
    }

    /* Get a pointer to the current thread */
    current_thread = ( TC_TCB *)TCCT_Current_Thread();

    /* Check if a thread is currently executing */
    if ( current_thread )
    {
        /* Check if thread has time-slicing enabled */
        if ( current_thread -> tc_cur_time_slice )
        {
            /* Decrement time-slice count */
            current_thread -> tc_cur_time_slice --;

            /* Check if time-slice has expired */
            if (!( current_thread -> tc_cur_time_slice ))
            {
                /* Set time-slice task pointer to the current executing thread */
                TMD_Time_Slice_Task = current_thread;

                /* Give a full time-slice to current thread */
                current_thread -> tc_cur_time_slice = current_thread -> tc_time_slice;
            }
        }
    }

    /* Check if timer HISR needs to be activated */
    if ( ( TMD_Timer_State == TM_EXPIRED ) || ( TMD_Time_Slice_Task ) )
    {
        /* Activate the timer HISR */
        TCCT_Activate_HISR(& TMD_HISR );
    }
}

INT     TMCT_OS_ClockDisable( VOID )
{
    TMD_Clock_Disable = NU_TRUE;

    return( NU_SUCCESS );
}
