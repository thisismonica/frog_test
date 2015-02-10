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
*       esal_ar_isr_defs.h
*
*   COMPONENT
*
*       ESAL - Embedded Software Abstraction Layer
*
*   DESCRIPTION
*
*       This file contains the architecture specific definitions,
*       structures, assembly macros, etc related to interrupt servicing
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

#ifndef         ESAL_AR_ISR_DEFS_H
#define         ESAL_AR_ISR_DEFS_H

/* Define default vector table address for PPC */
#define         ESAL_AR_ISR_VECT_TABLE_ADDRESS          0x00000000

/* MSR IP-Exception prefix bit */
#define         ESAL_AR_ISR_MSR_IP_BIT                  ESAL_GE_MEM_32BIT_SET(25)

#endif  /* ESAL_AR_ISR_DEFS_H */
