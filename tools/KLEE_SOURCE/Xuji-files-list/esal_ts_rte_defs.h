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
*       esal_ts_rte_defs.h
*
*   COMPONENT
*
*       ESAL - Embedded Software Abstraction Layer
*
*   DESCRIPTION
*
*       This file contains the toolset specific definitions, structures,
*       macros, etc, related to run-time environment items
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

#ifndef         ESAL_TS_RTE_DEFS_H
#define         ESAL_TS_RTE_DEFS_H

/* This define is used to add quotes to anything passed in */
#define         ESAL_TS_RTE_QUOTES(x)                   #x

extern UINT32  ESAL_TS_RTE_R3_R4_Reg_Move(UINT32 temp1, UINT32 temp2);

/* Define PPC Architecture SPR register numbers */
#define         ESAL_TS_RTE_DEC                         22
#define         ESAL_TS_RTE_TBL_READ                    268
#define         ESAL_TS_RTE_TBU_READ                    269
#define         ESAL_TS_RTE_SPRG0                       272
#define         ESAL_TS_RTE_SPRG1                       273
#define         ESAL_TS_RTE_SPRG2                       274
#define         ESAL_TS_RTE_SPRG3                       275
#define         ESAL_TS_RTE_TBL_WRITE                   284
#define         ESAL_TS_RTE_TBU_WRITE                   285
#define         ESAL_TS_RTE_IBAT0U                      528
#define         ESAL_TS_RTE_IBAT0L                      529
#define         ESAL_TS_RTE_IBAT1U                      530
#define         ESAL_TS_RTE_IBAT1L                      531
#define         ESAL_TS_RTE_IBAT2U                      532
#define         ESAL_TS_RTE_IBAT2L                      533
#define         ESAL_TS_RTE_IBAT3U                      534
#define         ESAL_TS_RTE_IBAT3L                      535
#define         ESAL_TS_RTE_DBAT0U                      536
#define         ESAL_TS_RTE_DBAT0L                      537
#define         ESAL_TS_RTE_DBAT1U                      538
#define         ESAL_TS_RTE_DBAT1L                      539
#define         ESAL_TS_RTE_DBAT2U                      540
#define         ESAL_TS_RTE_DBAT2L                      541
#define         ESAL_TS_RTE_DBAT3U                      542
#define         ESAL_TS_RTE_DBAT3L                      543

/* This macro writes the stack pointer. */
#define         ESAL_TS_RTE_SP_WRITE(stack_ptr)                           \
{   register unsigned long *tmp =  stack_ptr;                             \
    asm( mr r1, tmp);                                                   \
}

/* This macro gets the stack pointer. */
#define         ESAL_TS_RTE_SP_READ()                                     \
    asm(  mr r3, r1)

/* Read Decremeter */
#define         ESAL_TS_RTE_DEC_READ()                                     \
    asm(  mfspr r3, 22)

/* Read Decremeter */
#define         ESAL_TS_RTE_DEC_READ_VAL(dec_val)                         \
{   register unsigned long *tmp = &dec_val,xyz;                           \
    asm(mfspr r3, 22);	                                                  \
    asm(mr xyz,r3 );                                                      \
    *tmp = xyz;                                                           \
}

/* Read the FPSCR register */
#define         ESAL_TS_RTE_FPSCR_READ(fpscr_val_ptr)                     \
{   register unsigned long *tmp =  fpscr_val_ptr,xyz;                     \
    asm(mffs fp1);                                                        \
    asm(fctiwz fp1,fp1);                                                  \
    asm(stfd fp1,xyz);                                                    \
    *tmp = xyz;                                                           \
}

/* Read the MSR register */
#define         ESAL_TS_RTE_MSR_READ(msr_val_ptr)                         \
{   register unsigned long *tmp = msr_val_ptr,xyz;                          \
    asm (mfmsr r3 );                                                    \
    asm (mr xyz,r3 );                                                     \
    *tmp = xyz;                                                             \
}

/* Write the MSR register */
#define         ESAL_TS_RTE_MSR_WRITE(msr_val)                              \
{                                                                           \
    ESAL_TS_RTE_R3_R4_Reg_Move(msr_val, 0);                                 \
    asm( mtmsr r3);                                                       \
}


/* Read an SPR register */
#define         ESAL_TS_RTE_SPR_READ(spr_num, spr_val_ptr)                  \
               {                                                           \
               register unsigned long *tmp=spr_val_ptr, xyz;                 \
               asm ( mfspr r3, spr_num);                                 \
               asm ( mr xyz,r3 );                                          \
               *tmp=xyz; \
                }

/* Write to an SPR register */
#define         ESAL_TS_RTE_SPR_WRITE(spr_num, spr_val)                     \
{                                                                           \
    ESAL_TS_RTE_R3_R4_Reg_Move(spr_val, 0);                                 \
    asm( mtspr spr_num, r3);    \
}

/* Instruction Cache Block Invalidate */
#define         ESAL_TS_RTE_ICBI_EXECUTE(addr, addr_offset)                 \
{                                                                           \
    ESAL_TS_RTE_R3_R4_Reg_Move(addr, addr_offset);                          \
    asm(icbi  3, 4);                                                        \
}

/* Data Cache Block Invalidate */
#define         ESAL_TS_RTE_DCBI_EXECUTE(addr, addr_offset)                 \
{                                                                           \
    ESAL_TS_RTE_R3_R4_Reg_Move(addr, addr_offset);                          \
    asm(dcbi  3, 4);                                                        \
}

/* Data Cache Block Flush */
#define         ESAL_TS_RTE_DCBF_EXECUTE(addr, addr_offset)                 \
{                                                                           \
    ESAL_TS_RTE_R3_R4_Reg_Move(addr, addr_offset);                          \
    asm(dcbf  3, 4);                                                        \
}

/* Sync command to sync memory */
#define         ESAL_TS_RTE_SYNC_EXECUTE()                                  \
{                                                                           \
    asm( sync);                                                             \
}

/* Instruction sync command */
#define         ESAL_TS_RTE_ISYNC_EXECUTE()                                 \
{                                                                           \
    asm( isync);                                                            \
}

/* Translation Lookaside Buffer Load Instruction Entry */
#define         ESAL_TS_RTE_TLBLI_EXECUTE(tlb_val)                          \
{                                                                           \
    ESAL_TS_RTE_R3_R4_Reg_Move(tlb_val, 0);                                 \
    asm(tlbli  3);                                                          \
}

/* Translation Lookaside Buffer Load Data Entry */
#define         ESAL_TS_RTE_TLBLD_EXECUTE(tlb_val)                          \
{                                                                           \
    ESAL_TS_RTE_R3_R4_Reg_Move(tlb_val, 0);                                 \
    asm(tlbld  3);                                                          \
}

/* Translation Lookaside Buffer Synchronize */
#define         ESAL_TS_RTE_TLBSYNC_EXECUTE()                               \
{                                                                           \
    asm( tlbsync);                                                          \
}

/* Translation Lookaside Buffer Invalidate Entry */
#define         ESAL_TS_RTE_TLBIE_EXECUTE(tlb_val)                          \
{                                                                           \
    ESAL_TS_RTE_R3_R4_Reg_Move(tlb_val, 0);                                 \
    asm(tlbie  3);                                                          \
}

/* Translation Lookaside Buffer Invalidate All */
#define         ESAL_TS_RTE_TLBIA_EXECUTE()                                 \
{                                                                           \
    asm( tlbia);                                                            \
}

#if (ESAL_TS_PIC_PID_SUPPORT == ESAL_TRUE)



/* This macro sets the PIC/PID base address register */
#define         ESAL_TS_RTE_PIC_PID_BASE_SET(pic_base, pid_base)            \
                {                                                           \
\
                }

/* This macro gets the PIC/PID base address register */
#define         ESAL_TS_RTE_PIC_PID_BASE_GET(pic_base_ptr, pid_base_ptr)    \
                {                                                           \
\
                }

#endif  /* ESAL_TS_PIC_PID_SUPPORT == ESAL_TRUE */

#endif  /* ESAL_TS_RTE_DEFS_H */
