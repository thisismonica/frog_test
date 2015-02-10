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
*       esal_pr_cfg.h
*
*   COMPONENT
*
*       ESAL - Embedded Software Abstraction Layer
*
*   DESCRIPTION
*
*       This file contains required configuration settings for the
*       given processor.  These configuration settings are used by
*       generic ESAL components and can be used by external components.
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

#ifndef         ESAL_PR_CFG_H
#define         ESAL_PR_CFG_H

#define         ESAL_CPU_8313                   1
#define         ESAL_CPU_8377                   2
#define         ESAL_CPU_8306                   3

/* Define processor execution endianess
   (ESAL_LITTLE_ENDIAN or ESAL_BIG_ENDIAN) */
#define         ESAL_PR_ENDIANESS                       ESAL_BIG_ENDIAN

/* Define processor cache availability
   NOTE:  A differentiation is made in ESAL between cache that
          is contained on a processor and cache that is
          inherent as part of a core (L2 vs L1 cache). */
#define         ESAL_PR_CACHE_AVAILABLE                 ESAL_FALSE

/* Define if an interrupt controller exists on the processor and
   controlling / handling of interrupts from this interrupt controller must
   be accommodated for.  Setting this to ESAL_FALSE means processor level interrupts
   will NOT be controlled or handled.  Setting this to ESAL_TRUE means processor level
   interrupts will be controlled and handled */
#define         ESAL_PR_INTERRUPTS_AVAILABLE            ESAL_TRUE

/* Define processor type */
#ifdef _OS_MPC8377
#define         ESAL_PR_CPU_TYPE                        ESAL_CPU_8377
#else
#ifdef _OS_MPC8306
#define         ESAL_PR_CPU_TYPE                        ESAL_CPU_8306
#else
#define         ESAL_PR_CPU_TYPE                        ESAL_CPU_8313
#endif
#endif

#endif  /* ESAL_PR_CFG_H */
