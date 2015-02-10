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
*       esal_dp_int_defs.h
*
*   COMPONENT
*
*       ESAL - Embedded Software Abstraction Layer
*
*   DESCRIPTION
*
*       This file contains the development platform specific definitions,
*       structures, assembly macros, etc, related to interrupt control
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

#ifndef         ESAL_DP_INT_DEFS_H
#define         ESAL_DP_INT_DEFS_H



/* Define ESAL interrupt vector IDs for this development platform.
   These IDs match up with development platform interrupts.
   Values correspond to the index of entries in ESAL_GE_ISR_Interrupt_Handler[].
   Names are of the form ESAL_DP_<Name>_INT_VECTOR_ID, where <Name> comes
   directly from the hardware documentation */


/* Define the last ESAL interrupt vector ID for this development platform + 1 */
#define         ESAL_DP_INT_VECTOR_ID_DELIMITER     (ESAL_PR_INT_VECTOR_ID_DELIMITER + 1)

#endif  /* ESAL_DP_INT_DEFS_H */
