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
*       esal_ar_stk_defs.h
*
*   COMPONENT
*
*       ESAL - Embedded Software Abstraction Layer
*
*   DESCRIPTION
*
*       This file contains the architecture specific definitions,
*       structures, assembly macros, etc, related to stacks
*
*   DATA STRUCTURES
*
*       ESAL_AR_STK_MIN                     Minimum stack-frame
*                                           structure for the given
*                                           architecture
*       ESAL_AR_STK                         Stack-frame structure for
*                                           the given architecture
*
*   DEPENDENCIES
*
*       None
*
***********************************************************************/

#ifndef         ESAL_AR_STK_DEFS_H
#define         ESAL_AR_STK_DEFS_H

/* Define if floating point registers will be saved during context switches
   NOTE:  The assembly define must be changed in esal_ar_stk_defs.inc to
          match this define
   NOTE:  This define will dictate what registers are contained
          within solicited (TS) and unsolicited (AR) stack frames. */
#define         ESAL_AR_STK_FPU_SUPPORT         ESAL_TRUE

/* Define stack frame structure for minimum architecture registers required
   to be saved in order to enter a C environment during in interrupt / exception.
   These registers are the "scratch" registers that will not be preserved across
   a function call boundary and any interrupt state registers that must preserved
   to allow interrupt nesting. */
typedef struct
{
    UINT32          ctr;
    UINT32          xer;
    UINT32          cr;
    UINT32          lr;
    UINT32          r0;
    UINT32          r3;
    UINT32          r4;
    UINT32          r5;
    UINT32          r6;
    UINT32          r7;
    UINT32          r8;
    UINT32          r9;
    UINT32          r10;
    UINT32          msr;
    UINT32          rtn_address;
    UINT32          r11;
    UINT32          r12;

} ESAL_AR_STK_MIN;

/* Define stack frame structure for the architecture supported.
   This stack frame contains all registers that must be preserved
   across an (unsolicited) interrupt context switch.
   NOTE:  This stack frame includes the minimum stack frame
          defined above AND all other registers for the given
          architecture. */
typedef struct  ESAL_AR_STK_STRUCT
{
    UINT32          stack_type;
    UINT32          r14;
    UINT32          r15;
    UINT32          r16;
    UINT32          r17;
    UINT32          r18;
    UINT32          r19;
    UINT32          r20;
    UINT32          r21;
    UINT32          r22;
    UINT32          r23;
    UINT32          r24;
    UINT32          r25;
    UINT32          r26;
    UINT32          r27;
    UINT32          r28;
    UINT32          r29;
    UINT32          r30;
    UINT32          r31;

#if (ESAL_AR_STK_FPU_SUPPORT == ESAL_TRUE)

    UINT32        fpscr;
    UINT32        f0[2];
    UINT32        f1[2];
    UINT32        f2[2];
    UINT32        f3[2];
    UINT32        f4[2];
    UINT32        f5[2];
    UINT32        f6[2];
    UINT32        f7[2];
    UINT32        f8[2];
    UINT32        f9[2];
    UINT32        f10[2];
    UINT32        f11[2];
    UINT32        f12[2];
    UINT32        f13[2];
    UINT32        f14[2];
    UINT32        f15[2];
    UINT32        f16[2];
    UINT32        f17[2];
    UINT32        f18[2];
    UINT32        f19[2];
    UINT32        f20[2];
    UINT32        f21[2];
    UINT32        f22[2];
    UINT32        f23[2];
    UINT32        f24[2];
    UINT32        f25[2];
    UINT32        f26[2];
    UINT32        f27[2];
    UINT32        f28[2];
    UINT32        f29[2];
    UINT32        f30[2];
    UINT32        f31[2];

#endif /* (ESAL_AR_STK_FPU_SUPPORT == ESAL_TRUE) */

    ESAL_AR_STK_MIN min_stack;
    UINT32          eabi[4];

} ESAL_AR_STK;

#endif  /* ESAL_AR_STK_DEFS_H */
