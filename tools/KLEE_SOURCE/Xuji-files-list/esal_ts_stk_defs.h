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
*       esal_ts_stk_defs.h
*
*   COMPONENT
*
*       ESAL - Embedded Software Abstraction Layer
*
*   DESCRIPTION
*
*       This file contains the toolset specific definitions, structures,
*       macros, etc, related to stacks
*
*   DATA STRUCTURES
*
*       ESAL_TS_STK_STRUCT                  Stack-frame structure for the
*                                           given toolset / architecture
*
*   DEPENDENCIES
*
*       None
*
***********************************************************************/

#ifndef         ESAL_TS_STK_DEFS_H
#define         ESAL_TS_STK_DEFS_H

/* Define stack frame structure for the toolset / architecture supported. */
typedef struct  ESAL_TS_STK_STRUCT
{
    UINT        stack_type;
    UINT        r14;
    UINT        r15;
    UINT        r16;
    UINT        r17;
    UINT        r18;
    UINT        r19;
    UINT        r20;
    UINT        r21;
    UINT        r22;
    UINT        r23;
    UINT        r24;
    UINT        r25;
    UINT        r26;
    UINT        r27;
    UINT        r28;
    UINT        r29;
    UINT        r30;
    UINT        r31;
    UINT        cr;
    UINT        rtn_address;

#if (ESAL_AR_STK_FPU_SUPPORT == ESAL_TRUE)

    UINT32      f14[2];
    UINT32      f15[2];
    UINT32      f16[2];
    UINT32      f17[2];
    UINT32      f18[2];
    UINT32      f19[2];
    UINT32      f20[2];
    UINT32      f21[2];
    UINT32      f22[2];
    UINT32      f23[2];
    UINT32      f24[2];
    UINT32      f25[2];
    UINT32      f26[2];
    UINT32      f27[2];
    UINT32      f28[2];
    UINT32      f29[2];
    UINT32      f30[2];
    UINT32      f31[2];

#endif /* (ESAL_AR_STK_FPU_SUPPORT == ESAL_TRUE) */

    UINT        eabi[4];

} ESAL_TS_STK;


#endif  /* ESAL_TS_STK_DEFS_H */
