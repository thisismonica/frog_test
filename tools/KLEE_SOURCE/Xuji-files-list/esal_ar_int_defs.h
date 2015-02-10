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
*       esal_ar_int_defs.h
*
*   COMPONENT
*
*       ESAL - Embedded Software Abstraction Layer
*
*   DESCRIPTION
*
*       This file contains the architecture specific definitions,
*       structures, assembly macros, etc related to interrupts
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

#ifndef         ESAL_AR_INT_DEFS_H
#define         ESAL_AR_INT_DEFS_H

/* Defines used to specify an interrupt source type as a normal External Interrupt 
   or Critical Interrupt (for PPC variants that support Critical Interrupts).
   This value can be bitwise OR'd with the priority field when enabling an interrupt
   source via the ESAL_GE_INT_Enable API to specify if an interrupt will be routed
   as an External or Critical Interrupt.  The default for all interrupt sources is
   External routed (interrupt will be External routed if not specified when enabling 
   an interrupt source via ESAL_GE_INT_Enable).
   NOTE:  This operation is not supported by all PPC processor implementations - some 
          processors allow any interrupt source to be either External or Critical while other
          do not allow this functionality.  Additionally, some PPC processors do not
          support Critical interrupts at all. */
#define         ESAL_AR_INT_EXTERNAL_ROUTED                 (INT)0x00001000
#define         ESAL_AR_INT_CRITICAL_ROUTED                 (INT)0x00002000

/* Defines the enable bits for the interrupts enable through the MSR */
#define         ESAL_AR_INT_MACHINE_CHECK_BIT               ESAL_GE_MEM_32BIT_SET(19)
#define         ESAL_AR_INT_EXTERNAL_DECREMENTER_BIT        ESAL_GE_MEM_32BIT_SET(16)
#define         ESAL_AR_INT_CRITICAL_BIT                    ESAL_GE_MEM_32BIT_SET(24)

/* Define ESAL interrupt vector IDs for this architecture.
   These IDs match up with architecture interrupts.
   Values correspond to the index of entries in ESAL_GE_ISR_Interrupt_Handler[].
   Names are of the form ESAL_AR_<Name>_INT_VECTOR_ID, where <Name> comes
   directly from the hardware documentation */
#define         ESAL_AR_MACHINE_CHECK_INT_VECTOR_ID         0
#define         ESAL_AR_EXTERNAL_INT_VECTOR_ID              1
#define         ESAL_AR_DECREMENTER_INT_VECTOR_ID           2
#define         ESAL_AR_CRITICAL_INT_VECTOR_ID              3

/* Define the last ESAL interrupt vector ID for this architecture + 1 */
#define         ESAL_AR_INT_VECTOR_ID_DELIMITER             (ESAL_AR_CRITICAL_INT_VECTOR_ID + 1)

/* Define ESAL exception vector IDs for this architecture.
   These IDs match up with architecture exceptions.
   Values correspond to the index of entries in ESAL_GE_ISR_Exception_Handler[].
   Names are of the form ESAL_AR_<Name>_EXCEPT_VECTOR_ID, where <Name> comes
   directly from the hardware documentation */
#define         ESAL_AR_DSI_EXCEPT_VECTOR_ID                0
#define         ESAL_AR_ISI_EXCEPT_VECTOR_ID                1
#define         ESAL_AR_ALIGNMENT_EXCEPT_VECTOR_ID          2
#define         ESAL_AR_PROGRAM_EXCEPT_VECTOR_ID            3
#define         ESAL_AR_FLTPT_UNAVAIL_EXCEPT_VECTOR_ID      4
#define         ESAL_AR_RSRVD_0B00_EXCEPT_VECTOR_ID         5
#define         ESAL_AR_SYSCALL_EXCEPT_VECTOR_ID            6
#define         ESAL_AR_TRACE_EXCEPT_VECTOR_ID              7
#define         ESAL_AR_FLTPT_ASSIST_EXCEPT_VECTOR_ID       8
#define         ESAL_AR_RSRVD_0F00_EXCEPT_VECTOR_ID         9
#define         ESAL_AR_RSRVD_1000_EXCEPT_VECTOR_ID         10
#define         ESAL_AR_RSRVD_1100_EXCEPT_VECTOR_ID         11
#define         ESAL_AR_RSRVD_1200_EXCEPT_VECTOR_ID         12
#define         ESAL_AR_RSRVD_1300_EXCEPT_VECTOR_ID         13
#define         ESAL_AR_RSRVD_1400_EXCEPT_VECTOR_ID         14
#define         ESAL_AR_RSRVD_1500_EXCEPT_VECTOR_ID         15
#define         ESAL_AR_RSRVD_1600_EXCEPT_VECTOR_ID         16
#define         ESAL_AR_RSRVD_1700_EXCEPT_VECTOR_ID         17
#define         ESAL_AR_RSRVD_1800_EXCEPT_VECTOR_ID         18
#define         ESAL_AR_RSRVD_1900_EXCEPT_VECTOR_ID         19
#define         ESAL_AR_RSRVD_1A00_EXCEPT_VECTOR_ID         20
#define         ESAL_AR_RSRVD_1B00_EXCEPT_VECTOR_ID         21
#define         ESAL_AR_RSRVD_1C00_EXCEPT_VECTOR_ID         22
#define         ESAL_AR_RSRVD_1D00_EXCEPT_VECTOR_ID         23
#define         ESAL_AR_RSRVD_1E00_EXCEPT_VECTOR_ID         24
#define         ESAL_AR_RSRVD_1F00_EXCEPT_VECTOR_ID         25

/* Define the last ESAL exception vector ID for this architecture + 1 */
#define         ESAL_AR_EXCEPT_VECTOR_ID_DELIMITER          (ESAL_AR_RSRVD_1F00_EXCEPT_VECTOR_ID + 1)

/* Define variable(s) required to save / restore architecture interrupt state.
   These variable(s) are used in conjunction with the ESAL_AR_INT_ALL_DISABLE() and
   ESAL_AR_INT_ALL_RESTORE() macros to hold any data that must be preserved in
   order to allow these macros to function correctly. */
#define         ESAL_AR_INT_CONTROL_VARS                    UINT32 esal_tmp_val;

/* This macro locks out interrupts and saves the current
   architecture status register / state register to the specified
   address.  This function does not attempt to mask any bits in
   the return register value and can be used as a quick method
   to guard a critical section.
   NOTE:  This macro is used in conjunction with ESAL_AR_INT_ALL_RESTORE
          defined below and ESAL_AR_INT_CONTROL_VARS defined above. */
#define         ESAL_AR_INT_ALL_DISABLE()                                           \
                {                                                                   \
                    ESAL_REG UINT32 temp;                                           \
                                                                                    \
                    /* Read-modify-write MSR to disable all interrupts */           \
                    ESAL_TS_RTE_MSR_READ(&esal_tmp_val);                            \
                    temp = esal_tmp_val & ~ESAL_AR_INTERRUPTS_ENABLE_BITS;          \
                    ESAL_TS_RTE_MSR_WRITE(temp);                                    \
                }

/* This macro restores the architecture status / state register
   used to lockout interrupts to the value provided.  The
   intent of this function is to be a fast mechanism to restore the
   interrupt level at the end of a critical section to its
   original level.
   NOTE:  This macro is used in conjunction with ESAL_AR_INT_ALL_DISABLE
          and ESAL_AR_INT_CONTROL_VARS defined above. */
#define         ESAL_AR_INT_ALL_RESTORE()                                           \
                {                                                                   \
                    /* Restore MSR to pre-disable value */                          \
                    ESAL_TS_RTE_MSR_WRITE(esal_tmp_val);                            \
                }

/* This macro locks-out interrupts but doesn't save the status
   register / control register value. */
#define         ESAL_AR_INT_FAST_ALL_DISABLE()                                      \
                {                                                                   \
                    UINT32 temp;                                                    \
                                                                                    \
                    /* Read-modify-write MSR to disable all interrupts */           \
                    ESAL_TS_RTE_MSR_READ(&temp);                                    \
                    temp &= ~ESAL_AR_INTERRUPTS_ENABLE_BITS;                        \
                    ESAL_TS_RTE_MSR_WRITE(temp);                                    \
                }


/* This macro unlocks interrupts but doesn't save the status
   register / control register value. */
#define         ESAL_AR_INT_FAST_ALL_ENABLE()                                       \
                {                                                                   \
                    UINT32   temp;                                                  \
                                                                                    \
                    /* Read-modify-write MSR to enable all interrupts */            \
                    ESAL_TS_RTE_MSR_READ(&temp);                                    \
                    temp |= ESAL_AR_INTERRUPTS_ENABLE_BITS;                         \
                    ESAL_TS_RTE_MSR_WRITE(temp);                                    \
                }

/* This macro sets the interrupt related bits in the status register / control
   register to the specified value. */
#define         ESAL_AR_INT_BITS_SET(set_bits)                                      \
                {                                                                   \
                    UINT32 temp;                                                    \
                                                                                    \
                    /* Read MSR into temp */                                        \
                    ESAL_TS_RTE_MSR_READ(&temp);                                    \
                                                                                    \
                    /* Clear current interrupt bits and set new ones */             \
                    temp &= ~ESAL_AR_INTERRUPTS_ENABLE_BITS;                        \
                    temp |= set_bits;                                               \
                                                                                    \
                    /* Write new MSR */                                             \
                    ESAL_TS_RTE_MSR_WRITE(temp);                                    \
                }

/* This macro gets the interrupt related bits from the status register / control
   register. */
#define         ESAL_AR_INT_BITS_GET(get_bits_ptr)                                  \
                {                                                                   \
                    UINT32 temp;                                                    \
                                                                                    \
                    /* Read MSR into temp */                                        \
                    ESAL_TS_RTE_MSR_READ(&temp);                                    \
                                                                                    \
                    /* Return just interrupt bits */                                \
                    *get_bits_ptr = (temp & (UINT16)ESAL_AR_INTERRUPTS_ENABLE_BITS);\
                }

#endif  /* ESAL_AR_INT_DEFS_H */
