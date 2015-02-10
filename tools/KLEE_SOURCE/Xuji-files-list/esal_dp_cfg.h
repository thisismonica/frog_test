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
*       esal_dp_cfg.h
*
*   COMPONENT
*
*       ESAL - Embedded Software Abstraction Layer
*
*   DESCRIPTION
*
*       This file contains required configuration settings for the
*       given development platform.  These configuration settings are
*       used by generic ESAL components and may be used by external
*       components.
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

#ifndef         ESAL_DP_CFG_H
#define         ESAL_DP_CFG_H

/* Define the board clock rate (in hertz).
   NOTE: This clock rate is used to calculate the rate of the OS timer.  
         Therefore, if multiple clock sources are fed to the processor,
         this clock rate value must represent the source used
         by the on-chip timer unit. */
#if (ESAL_PR_CPU_TYPE == ESAL_CPU_8377)
#define         ESAL_DP_REF_CLOCK_RATE                  66666000UL
#elif (ESAL_PR_CPU_TYPE == ESAL_CPU_8306)
#define         ESAL_DP_REF_CLOCK_RATE                  33300000UL
#else
#define         ESAL_DP_REF_CLOCK_RATE                  66600000UL
#endif

/* Define number of memory regions contained on the given development platform */
#if (ESAL_PR_CPU_TYPE == ESAL_CPU_8306)
#define         ESAL_DP_MEM_NUM_REGIONS                 5
#else
#define         ESAL_DP_MEM_NUM_REGIONS                 6
#endif

/* Define if an interrupt controller (off processor) exists on the board and
   controlling / handling of interrupts from this interrupt controller must
   be accommodated for.  Setting this to ESAL_FALSE means off-chip interrupts
   will NOT be controlled or handled.  Setting this to ESAL_TRUE means off-chip
   interrupts will be controlled and handled */
#define         ESAL_DP_INTERRUPTS_AVAILABLE            ESAL_FALSE

/* Define the base of address on-chip peripheral registers */
#define         ESAL_DP_PERIPH_BASE                     0xE0000000

#endif  /* ESAL_DP_CFG_H */
