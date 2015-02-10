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
*       esal_pr_isr_defs.h
*
*   COMPONENT
*
*       ESAL - Embedded Software Abstraction Layer
*
*   DESCRIPTION
*
*       This file contains the processor specific definitions,
*       structures, assembly macros, etc, related to interrupt servicing
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

#ifndef         ESAL_PR_ISR_DEFS_H
#define         ESAL_PR_ISR_DEFS_H

/* Define for Reserved vector id's. */
#define         ESAL_PR_ISR_RESERVED_ID         0xFFFFFFFF

/* Mask value for SIVCR (System Regular Interrupt Vector Register). */
#define         ESAL_PR_ISR_SIVCR_MASK          0x0000007F

/* Mask value for SCVCR (System Critical Interrupt Vector Register). */
#define         ESAL_PR_ISR_SCVCR_MASK          0x0000007F

#endif  /* ESAL_PR_ISR_DEFS_H */
