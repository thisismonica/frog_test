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
*       esal_ar_tmr_defs.h
*
*   COMPONENT
*
*       ESAL - Embedded Software Abstraction Layer
*
*   DESCRIPTION
*
*       This file contains the architecture specific definitions,
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

#ifndef         ESAL_AR_TMR_DEFS_H
#define         ESAL_AR_TMR_DEFS_H

#if (ESAL_AR_OS_TIMER_USED == ESAL_TRUE)

/* Define the architecture OS timer type (count-down or count-up) */
#define         ESAL_AR_TMR_OS_COUNT_DIR                ESAL_COUNT_DOWN

/* Define for the architecture OS timer interrupt vector */
#define         ESAL_AR_TMR_OS_VECTOR                   ESAL_AR_DECREMENTER_INT_VECTOR_ID

/* Define a macro to read the architecture OS timer hardware count.  The
   resultant size of the count must be 32-bits, regardless of the actual
   size of the timer used (8-bit, 16-bit, 32-bit, etc). */
#define         ESAL_AR_TMR_OS_COUNT_READ()             ESAL_TS_RTE_DEC_READ()

/* Define the EOI logic for the architecture OS timer */
#define         ESAL_AR_TMR_OS_TIMER_EOI(vector)                                                \
                {                                                                               \
                    UINT32  timer_count;                                                        \
                                                                                                \
                    /* Get the timer count from the vector ID data structure */                 \
                    timer_count = (UINT32)ESAL_GE_EXCEPT_VECTOR_DATA_GET(vector);               \
                                                                                                \
                    /* Reseting the decrementer count clears the interrupt */                   \
                    ESAL_TS_RTE_SPR_WRITE(ESAL_TS_RTE_DEC, timer_count);                        \
                }

#endif  /* ESAL_AR_OS_TIMER_USED == ESAL_TRUE */

#endif  /* ESAL_AR_TMR_DEFS_H */
