/***********************************************************************
*
*             Copyright 1993-2007 Mentor Graphics Corporation
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
*       esal_pr_tmr_defs.h
*
*   COMPONENT
*
*       ESAL - Embedded Software Abstraction Layer
*
*   DESCRIPTION
*
*       This file contains the processor specific definitions,
*       structures, assembly macros, etc, related to timers
*
*   DATA STRUCTURES
*
*       None
*
*   DEPENDENCIES
*
*       None
*
***********************************************************************/

#ifndef         ESAL_PR_TMR_DEFS_H
#define         ESAL_PR_TMR_DEFS_H

/* Define the System Priority and Configuration Register (SPCR) offset from IMMR,  */
#define         ESAL_PR_TMR_SPCR_OFFSET                 0x0110

/* Define the Time Base Enable bit in the SPCR register. */
#define         ESAL_PR_TMR_CONFIG_TBEN_BIT             ESAL_GE_MEM_32BIT_SET(9)

/* Define the clock rate for the OS timer */
#if (ESAL_PR_CPU_TYPE == ESAL_CPU_8377)
#define         ESAL_PR_TMR_OS_CLOCK_RATE               (ESAL_DP_REF_CLOCK_RATE * 5)
#elif (ESAL_PR_CPU_TYPE == ESAL_CPU_8306)
#define         ESAL_PR_TMR_OS_CLOCK_RATE               (ESAL_DP_REF_CLOCK_RATE * 4)
#else
#define         ESAL_PR_TMR_OS_CLOCK_RATE               ((ESAL_DP_REF_CLOCK_RATE / 2) * 5)
#endif

/* Define the clock prescaler for the OS timer 
   NOTE:  The prescaler is used to adjust the processor clock rate to a lower clock
          rate suitable for the timer */
#define         ESAL_PR_TMR_OS_CLOCK_PRESCALE           4

/* The following definitions / macros / etc are only used if the processor
   is configured (this is only if the architecture is NOT configured to
   contain the OS timer in esal_ar_cfg.h) to use a processor level timer
   for the OS timer. */
#if (ESAL_AR_OS_TIMER_USED == ESAL_FALSE)

/* Define the processor OS timer type (count-down or count-up) */
#define         ESAL_PR_TMR_OS_COUNT_DIR                (VOID)ESAL_NULL

/* Define for the processor OS timer interrupt vector */
#define         ESAL_PR_TMR_OS_VECTOR                   (VOID)ESAL_NULL

/* Define a macro to read the processor OS timer hardware count.  The
   resultant size of the count must be 32-bits, regardless of the actual
   size of the timer used (8-bit, 16-bit, 32-bit, etc). */
#define         ESAL_PR_TMR_OS_COUNT_READ()             (VOID)ESAL_NULL

/* Define the EOI logic for the processor OS timer */
#define         ESAL_PR_TMR_OS_TIMER_EOI(vector)        (VOID)vector

#endif  /* ESAL_PR_OS_TIMER_USED == ESAL_TRUE */

#endif  /* ESAL_PR_TMR_DEFS_H */
