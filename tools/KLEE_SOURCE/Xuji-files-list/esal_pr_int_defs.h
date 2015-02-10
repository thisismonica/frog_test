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
*       esal_pr_int_defs.h
*
*   COMPONENT
*
*       ESAL - Embedded Software Abstraction Layer
*
*   DESCRIPTION
*
*       This file contains the processor specific definitions,
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

#ifndef         ESAL_PR_INT_DEFS_H
#define         ESAL_PR_INT_DEFS_H

/* Define for interrupt controller base address */
#define     ESAL_PR_INT_IPIC_BASE                   (ESAL_DP_PERIPH_BASE + 0x700)

/* Integrated Programmable Interrupt Controller (IPIC) Register Offsets  */
#define     ESAL_PR_INT_IPIC_SIVCR_OFFSET           0x04
#define     ESAL_PR_INT_IPIC_SIPRR_A_OFFSET         0x10
#if ((ESAL_PR_CPU_TYPE == ESAL_CPU_8377) || (ESAL_PR_CPU_TYPE == ESAL_CPU_8306))
#define     ESAL_PR_INT_IPIC_SIPRR_B_OFFSET         0x14 /*Add 2008-12-08*/
#define     ESAL_PR_INT_IPIC_SIPRR_C_OFFSET         0x18 /*Add 2008-12-08*/
#endif
#define     ESAL_PR_INT_IPIC_SIPRR_D_OFFSET         0x1C
#define     ESAL_PR_INT_IPIC_SIMSR_H_OFFSET         0x20
#define     ESAL_PR_INT_IPIC_SIMSR_L_OFFSET         0x24
#define     ESAL_PR_INT_IPIC_SICNR_OFFSET           0x28
#define     ESAL_PR_INT_IPIC_SEPNR_OFFSET           0x2C
#define     ESAL_PR_INT_IPIC_SMPRR_A_OFFSET         0x30
#define     ESAL_PR_INT_IPIC_SMPRR_B_OFFSET         0x34
#define     ESAL_PR_INT_IPIC_SEMSR_OFFSET           0x38
#define     ESAL_PR_INT_IPIC_SECNR_OFFSET           0x3C
#define     ESAL_PR_INT_IPIC_SCVCR_OFFSET           0x60

/* Defines to Reset Internal / External Interrupts. */
#define     ESAL_PR_INT_SEPNR_DISABLE_ALL           0xFFFFFFFF
#define     ESAL_PR_INT_MASK_ALL                    0x00000000

/* Define for priority offset */
#define     ESAL_PR_INT_PRIORITY_SHIFT              3

/* Interrupt for priority bit mask. */
#define     ESAL_PR_INT_PRI_MASK                    0xE0000000

/* Macro to get the priority bits shifted to "start position". */
/* NOTE: "Start position" for interrupt priority bits is priority 0
         (bits 0 - 2). */
#define     ESAL_PR_INT_GET_PRI_BITS(id, diff, offset)                                      \
            ((id - diff) + offset) << 29

/* This macro returns the priority bits in the position of 
   priority variable passed in. */
#define     ESAL_PR_INT_LOC_PRI_BITS(pri_bits, priority, offset)                            \
            (pri_bits >> ((priority * ESAL_PR_INT_PRIORITY_SHIFT) + offset))

/* This macro returns the priority of the priority bits
   that it is searching for. */
#define     ESAL_PR_INT_GET_PRI(find, from, offset, priority)                               \
{                                                                                           \
    UINT32   temp_clr = ESAL_PR_INT_PRI_MASK;                                               \
    INT      i;                                                                             \
                                                                                            \
                                                                                            \
    /* Loop through the priority register (from) value. */                                  \
    for (i = offset; i <= (offset + 4); i++)                                                \
    {                                                                                       \
        /* Check if priority bits are found. */                                             \
        if (find == (from & temp_clr))                                                      \
        {                                                                                   \
            /* Save priority value */                                                       \
            *priority = i;                                                                  \
            break;                                                                          \
        }                                                                                   \
                                                                                            \
        /* Check next priority. */                                                          \
        find >>= (ESAL_PR_INT_PRIORITY_SHIFT + offset);                                     \
        temp_clr >>= (ESAL_PR_INT_PRIORITY_SHIFT + offset);                                 \
    }                                                                                       \
}

/* This macro swaps priority bits to its new priority. */
#define     ESAL_PR_INT_PRIORITY_SWAP(pri_bits, curr_pri_reg, offset, priority)             \
{                                                                                           \
    UINT32      temp_cur;                                                                   \
    INT         temp_priority;                                                              \
    UINT32      temp;                                                                       \
                                                                                            \
                                                                                            \
    /* Get the first group of priority bits to be swaped. */                                \
    temp_cur = *curr_pri_reg & ESAL_PR_INT_LOC_PRI_BITS(ESAL_PR_INT_PRI_MASK, priority, 0); \
                                                                                            \
    /* Find out what priority the above bits are. */                                        \
    ESAL_PR_INT_GET_PRI(pri_bits, *curr_pri_reg, offset, &temp_priority);                   \
                                                                                            \
    /* Move the priority bits to new location. */                                           \
    if ((temp_priority - priority) < 0)                                                     \
    {                                                                                       \
        /* Save priority of swap bits. */                                                   \
        temp = priority - temp_priority;                                                    \
    }                                                                                       \
    else                                                                                    \
    {                                                                                       \
        /* Save priority of swap bits. */                                                   \
        temp = temp_priority - priority;                                                    \
    }                                                                                       \
                                                                                            \
    /* Clear out old priority bits. */                                                      \
    *curr_pri_reg &= ~(ESAL_PR_INT_LOC_PRI_BITS(ESAL_PR_INT_PRI_MASK, temp_pri, offset) |   \
                       ESAL_PR_INT_LOC_PRI_BITS(ESAL_PR_INT_PRI_MASK, temp, offset));       \
                                                                                            \
    /* OR in new priority bits. */                                                          \
    *curr_pri_reg |= (ESAL_PR_INT_LOC_PRI_BITS(pri_bits, priority, offset) |                \
                      ESAL_PR_INT_LOC_PRI_BITS(temp_cur, temp, offset));                    \
}

/* Define ESAL interrupt vector IDs for this processor.
   These IDs match up with processor interrupts.
   Values correspond to the index of entries in ESAL_GE_ISR_Interrupt_Handler[].
   Names are of the form ESAL_PR_<Name>_INT_VECTOR_ID, where <Name> comes
   directly from the hardware documentation */
#if (ESAL_PR_CPU_TYPE == ESAL_CPU_8306)
#define     ESAL_PR_IRQ0_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 0)
#define     ESAL_PR_IRQ1_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 1)
#define     ESAL_PR_IRQ2_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 2)
#define     ESAL_PR_IRQ3_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 3)
#define     ESAL_PR_IRQ4_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 4)
#define     ESAL_PR_QE_High_INT_VECTOR_ID           (ESAL_AR_INT_VECTOR_ID_DELIMITER + 5)
#define     ESAL_PR_QE_Low_INT_VECTOR_ID            (ESAL_AR_INT_VECTOR_ID_DELIMITER + 6)
#define     ESAL_PR_USB_DR_INT_VECTOR_ID            (ESAL_AR_INT_VECTOR_ID_DELIMITER + 11)
#define     ESAL_PR_eSDHC_INT_VECTOR_ID             (ESAL_AR_INT_VECTOR_ID_DELIMITER + 15)  
#define     ESAL_PR_DMA_Engine1_INT_VECTOR_ID       (ESAL_AR_INT_VECTOR_ID_DELIMITER + 23)  
#define     ESAL_PR_UARTx_INT_VECTOR_ID             (ESAL_AR_INT_VECTOR_ID_DELIMITER + 29)
#define     ESAL_PR_FlexCANx_INT_VECTOR_ID          (ESAL_AR_INT_VECTOR_ID_DELIMITER + 30)
#define     ESAL_PR_I2C1_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 34)
#define     ESAL_PR_I2C2_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 35)
#define     ESAL_PR_SPI_INT_VECTOR_ID               (ESAL_AR_INT_VECTOR_ID_DELIMITER + 36)
#define     ESAL_PR_RTC_SEC_INT_VECTOR_ID           (ESAL_AR_INT_VECTOR_ID_DELIMITER + 37)
#define     ESAL_PR_PIT_INT_VECTOR_ID               (ESAL_AR_INT_VECTOR_ID_DELIMITER + 38)
#define     ESAL_PR_RTC_ALR_INT_VECTOR_ID           (ESAL_AR_INT_VECTOR_ID_DELIMITER + 41)
#define     ESAL_PR_SBA_INT_VECTOR_ID               (ESAL_AR_INT_VECTOR_ID_DELIMITER + 43)
#define     ESAL_PR_DMA_Engine2_INT_VECTOR_ID       (ESAL_AR_INT_VECTOR_ID_DELIMITER + 44)
#define     ESAL_PR_GTM4_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 45)
#define     ESAL_PR_QE_Ports_INT_VECTOR_ID          (ESAL_AR_INT_VECTOR_ID_DELIMITER + 47)
#define     ESAL_PR_GPIOX_INT_VECTOR_ID             (ESAL_AR_INT_VECTOR_ID_DELIMITER + 48) 
#define     ESAL_PR_DDR_INT_VECTOR_ID               (ESAL_AR_INT_VECTOR_ID_DELIMITER + 49)
#define     ESAL_PR_LBC_INT_VECTOR_ID               (ESAL_AR_INT_VECTOR_ID_DELIMITER + 50)
#define     ESAL_PR_GTM2_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 51)
#define     ESAL_PR_PMC_INT_VECTOR_ID               (ESAL_AR_INT_VECTOR_ID_DELIMITER + 53)
#define     ESAL_PR_GTM3_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 57)
#define     ESAL_PR_GTM1_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 63)
#define     ESAL_PR_DMA_Engine1_ERR_INT_VECTOR_ID   (ESAL_AR_INT_VECTOR_ID_DELIMITER + 67)
#define     ESAL_PR_GTM5_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 68)
#else
#if (!(ESAL_PR_CPU_TYPE == ESAL_CPU_8377))   
#define     ESAL_PR_IRQ0_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 0)
#define     ESAL_PR_IRQ1_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 1)
#define     ESAL_PR_IRQ2_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 2)
#define     ESAL_PR_IRQ3_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 3)
#define     ESAL_PR_IRQ4_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 4)
#endif
#define     ESAL_PR_TSEC1_TX_INT_VECTOR_ID          (ESAL_AR_INT_VECTOR_ID_DELIMITER + 5)
#define     ESAL_PR_TSEC1_RX_INT_VECTOR_ID          (ESAL_AR_INT_VECTOR_ID_DELIMITER + 6)
#define     ESAL_PR_TSEC1_ERR_INT_VECTOR_ID         (ESAL_AR_INT_VECTOR_ID_DELIMITER + 7)
#define     ESAL_PR_TSEC2_TX_INT_VECTOR_ID          (ESAL_AR_INT_VECTOR_ID_DELIMITER + 8)
#define     ESAL_PR_TSEC2_RX_INT_VECTOR_ID          (ESAL_AR_INT_VECTOR_ID_DELIMITER + 9)
#define     ESAL_PR_TSEC2_ERR_INT_VECTOR_ID         (ESAL_AR_INT_VECTOR_ID_DELIMITER + 10)
#define     ESAL_PR_USB_DR_INT_VECTOR_ID            (ESAL_AR_INT_VECTOR_ID_DELIMITER + 11)
#if (ESAL_PR_CPU_TYPE == ESAL_CPU_8377)
#define     ESAL_PR_eSDHC_INT_VECTOR_ID             (ESAL_AR_INT_VECTOR_ID_DELIMITER + 15) /*20081208*/
#define     ESAL_PR_SATA1_INT_VECTOR_ID             (ESAL_AR_INT_VECTOR_ID_DELIMITER + 17) /*20081208*/
#define     ESAL_PR_SATA2_INT_VECTOR_ID             (ESAL_AR_INT_VECTOR_ID_DELIMITER + 18) /*20081208*/
#define     ESAL_PR_SATA3_INT_VECTOR_ID             (ESAL_AR_INT_VECTOR_ID_DELIMITER + 19) /*20081208*/
#define     ESAL_PR_SATA4_INT_VECTOR_ID             (ESAL_AR_INT_VECTOR_ID_DELIMITER + 20) /*20081208*/
#define     ESAL_PR_PEX1_CNT_INT_VECTOR_ID          (ESAL_AR_INT_VECTOR_ID_DELIMITER + 21) /*20081208*/
#define     ESAL_PR_PEX2_CNT_INT_VECTOR_ID          (ESAL_AR_INT_VECTOR_ID_DELIMITER + 22) /*20081208*/
#define     ESAL_PR_MSIR1_INT_VECTOR_ID             (ESAL_AR_INT_VECTOR_ID_DELIMITER + 24) /*20081208*/
#endif
#define     ESAL_PR_UART1_INT_VECTOR_ID             (ESAL_AR_INT_VECTOR_ID_DELIMITER + 29)
#define     ESAL_PR_UART2_INT_VECTOR_ID             (ESAL_AR_INT_VECTOR_ID_DELIMITER + 30)
#define     ESAL_PR_SEC_INT_VECTOR_ID               (ESAL_AR_INT_VECTOR_ID_DELIMITER + 31)
#define     ESAL_PR_ETSEC1_1588TMR_INT_VECTOR_ID    (ESAL_AR_INT_VECTOR_ID_DELIMITER + 32)
#define     ESAL_PR_ETSEC2_1588TMR_INT_VECTOR_ID    (ESAL_AR_INT_VECTOR_ID_DELIMITER + 33)
#define     ESAL_PR_I2C1_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 34)
#define     ESAL_PR_I2C2_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 35)
#define     ESAL_PR_SPI_INT_VECTOR_ID               (ESAL_AR_INT_VECTOR_ID_DELIMITER + 36)
#define     ESAL_PR_RTC_SEC_INT_VECTOR_ID           (ESAL_AR_INT_VECTOR_ID_DELIMITER + 37)
#define     ESAL_PR_PIT_INT_VECTOR_ID               (ESAL_AR_INT_VECTOR_ID_DELIMITER + 38)
#define     ESAL_PR_PCI_INT_VECTOR_ID               (ESAL_AR_INT_VECTOR_ID_DELIMITER + 39)
#if (ESAL_PR_CPU_TYPE == ESAL_CPU_8377)
#define     ESAL_PR_MSIR0_INT_VECTOR_ID             (ESAL_AR_INT_VECTOR_ID_DELIMITER + 40)/*20081208*/
#endif
#define     ESAL_PR_RTC_ALR_INT_VECTOR_ID           (ESAL_AR_INT_VECTOR_ID_DELIMITER + 41)
#define     ESAL_PR_MU_INT_VECTOR_ID                (ESAL_AR_INT_VECTOR_ID_DELIMITER + 42)
#define     ESAL_PR_SBA_INT_VECTOR_ID               (ESAL_AR_INT_VECTOR_ID_DELIMITER + 43)
#define     ESAL_PR_DMA_INT_VECTOR_ID               (ESAL_AR_INT_VECTOR_ID_DELIMITER + 44)
#define     ESAL_PR_GTM4_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 45)
#define     ESAL_PR_GTM8_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 46)
#define     ESAL_PR_GPIO_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 47)
#if (ESAL_PR_CPU_TYPE == ESAL_CPU_8377)
#define     ESAL_PR_GPIO2_INT_VECTOR_ID             (ESAL_AR_INT_VECTOR_ID_DELIMITER + 48)/*20081208*/
#endif
#define     ESAL_PR_DDR_INT_VECTOR_ID               (ESAL_AR_INT_VECTOR_ID_DELIMITER + 49)
#define     ESAL_PR_LBC_INT_VECTOR_ID               (ESAL_AR_INT_VECTOR_ID_DELIMITER + 50)
#define     ESAL_PR_GTM2_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 51)
#define     ESAL_PR_GTM6_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 52)
#define     ESAL_PR_PMC_INT_VECTOR_ID               (ESAL_AR_INT_VECTOR_ID_DELIMITER + 53)
#if (ESAL_PR_CPU_TYPE == ESAL_CPU_8377)
#define     ESAL_PR_MSIR2_INT_VECTOR_ID             (ESAL_AR_INT_VECTOR_ID_DELIMITER + 54)/*20081208*/
#define     ESAL_PR_MSIR3_INT_VECTOR_ID             (ESAL_AR_INT_VECTOR_ID_DELIMITER + 55)/*20081208*/
#endif
#define     ESAL_PR_GTM3_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 57)
#define     ESAL_PR_GTM7_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 58)
#if (ESAL_PR_CPU_TYPE == ESAL_CPU_8377)
#define     ESAL_PR_MSIR4_INT_VECTOR_ID             (ESAL_AR_INT_VECTOR_ID_DELIMITER + 59)/*20081208*/
#define     ESAL_PR_MSIR5_INT_VECTOR_ID             (ESAL_AR_INT_VECTOR_ID_DELIMITER + 60)/*20081208*/
#define     ESAL_PR_MSIR6_INT_VECTOR_ID             (ESAL_AR_INT_VECTOR_ID_DELIMITER + 61)/*20081208*/
#define     ESAL_PR_MSIR7_INT_VECTOR_ID             (ESAL_AR_INT_VECTOR_ID_DELIMITER + 62)/*20081208*/
#endif
#define     ESAL_PR_GTM1_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 63)
#define     ESAL_PR_GTM5_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 64)
#if (ESAL_PR_CPU_TYPE == ESAL_CPU_8377)
#define     ESAL_PR_IRQ0_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 70)
#define     ESAL_PR_IRQ1_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 71)
#define     ESAL_PR_IRQ2_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 72)
#define     ESAL_PR_IRQ3_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 73)
#define     ESAL_PR_IRQ4_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 74)
#define     ESAL_PR_IRQ5_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 75)/*20081208*/
#define     ESAL_PR_IRQ6_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 76)/*20081208*/
#define     ESAL_PR_IRQ7_INT_VECTOR_ID              (ESAL_AR_INT_VECTOR_ID_DELIMITER + 77)/*20081208*/
#endif
#endif
/* Define the last ESAL interrupt vector ID for this processor + 1 */
#if (ESAL_PR_CPU_TYPE == ESAL_CPU_8377)
#define         ESAL_PR_INT_VECTOR_ID_DELIMITER    (ESAL_PR_IRQ7_INT_VECTOR_ID + 1)
#else
#define         ESAL_PR_INT_VECTOR_ID_DELIMITER     (ESAL_PR_GTM5_INT_VECTOR_ID + 1)
#endif

#endif  /* ESAL_PR_INT_DEFS_H */
