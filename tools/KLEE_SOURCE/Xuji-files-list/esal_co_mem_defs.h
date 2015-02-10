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
*       esal_co_mem_defs.h
*
*   COMPONENT
*
*       ESAL - Embedded Software Abstraction Layer
*
*   DESCRIPTION
*
*       This file contains core specific defines, structures, data
*       types, etc related to memory
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

#ifndef         ESAL_CO_MEM_DEFS_H
#define         ESAL_CO_MEM_DEFS_H

/* Size of cache block */
#define     ESAL_CO_MEM_BLOCK_SIZE                  32

/* Bit offsets for upper bat register */
#define     ESAL_CO_MEM_UBAT_SIZE_OFFSET            2

/* Cache related defines for lower bat register bits */
#define     ESAL_CO_MEM_LBAT_GUARDED_MEM_ENABLE     ESAL_GE_MEM_32BIT_SET(28)
#define     ESAL_CO_MEM_LBAT_WT_CACHE_ENABLE        ESAL_GE_MEM_32BIT_SET(25)
#define     ESAL_CO_MEM_LBAT_NO_CACHE               ESAL_GE_MEM_32BIT_SET(26)
#define     ESAL_CO_MEM_LBAT_WB_CACHE_ENABLE        (ESAL_CO_MEM_LBAT_WT_CACHE_ENABLE|  \
                                                     ESAL_GE_MEM_32BIT_SET(27) |        \
                                                     ESAL_CO_MEM_LBAT_NO_CACHE)

/* Define initial states for upper and lower bat registers */
#define     ESAL_CO_MEM_UBAT_INIT                   ESAL_GE_MEM_32BIT_SET(30)
#define     ESAL_CO_MEM_LBAT_INIT                   ESAL_GE_MEM_32BIT_SET(30)

/* Define bits for HID0 register */
#define     ESAL_CO_MEM_HID0_ICACHE_ENABLE          ESAL_GE_MEM_32BIT_SET(16)
#define     ESAL_CO_MEM_HID0_DCACHE_ENABLE          ESAL_GE_MEM_32BIT_SET(17)
#define     ESAL_CO_MEM_HID0_ICACHE_INVALIDATE      ESAL_GE_MEM_32BIT_SET(20)
#define     ESAL_CO_MEM_HID0_DCACHE_INVALIDATE      ESAL_GE_MEM_32BIT_SET(21)

/* Define bits for HID0 register */
#define     ESAL_CO_MEM_HID2_HBE_ENABLE             ESAL_GE_MEM_32BIT_SET(13)

/* Define bits in MSR register for data and instruction translation */
#define     ESAL_CO_MEM_MSR_IR_BIT                  ESAL_GE_MEM_32BIT_SET(26)
#define     ESAL_CO_MEM_MSR_DR_BIT                  ESAL_GE_MEM_32BIT_SET(27)

/* SPR Register definitions */
#define     ESAL_CO_MEM_HID0_REG                    1008
#define     ESAL_CO_MEM_HID2_REG                    1011
#define     ESAL_CO_MEM_IBAT0U_REG                  ESAL_TS_RTE_IBAT0U
#define     ESAL_CO_MEM_IBAT0L_REG                  ESAL_TS_RTE_IBAT0L
#define     ESAL_CO_MEM_IBAT1U_REG                  ESAL_TS_RTE_IBAT1U
#define     ESAL_CO_MEM_IBAT1L_REG                  ESAL_TS_RTE_IBAT1L
#define     ESAL_CO_MEM_IBAT2U_REG                  ESAL_TS_RTE_IBAT2U
#define     ESAL_CO_MEM_IBAT2L_REG                  ESAL_TS_RTE_IBAT2L
#define     ESAL_CO_MEM_IBAT3U_REG                  ESAL_TS_RTE_IBAT3U
#define     ESAL_CO_MEM_IBAT3L_REG                  ESAL_TS_RTE_IBAT3L
#define     ESAL_CO_MEM_DBAT0U_REG                  ESAL_TS_RTE_DBAT0U
#define     ESAL_CO_MEM_DBAT0L_REG                  ESAL_TS_RTE_DBAT0L
#define     ESAL_CO_MEM_DBAT1U_REG                  ESAL_TS_RTE_DBAT1U
#define     ESAL_CO_MEM_DBAT1L_REG                  ESAL_TS_RTE_DBAT1L
#define     ESAL_CO_MEM_DBAT2U_REG                  ESAL_TS_RTE_DBAT2U
#define     ESAL_CO_MEM_DBAT2L_REG                  ESAL_TS_RTE_DBAT2L
#define     ESAL_CO_MEM_DBAT3U_REG                  ESAL_TS_RTE_DBAT3U
#define     ESAL_CO_MEM_DBAT3L_REG                  ESAL_TS_RTE_DBAT3L
#define     ESAL_CO_MEM_IBAT4U_REG                  560
#define     ESAL_CO_MEM_IBAT4L_REG                  561
#define     ESAL_CO_MEM_IBAT5U_REG                  562
#define     ESAL_CO_MEM_IBAT5L_REG                  563
#define     ESAL_CO_MEM_IBAT6U_REG                  564
#define     ESAL_CO_MEM_IBAT6L_REG                  565
#define     ESAL_CO_MEM_IBAT7U_REG                  566
#define     ESAL_CO_MEM_IBAT7L_REG                  567
#define     ESAL_CO_MEM_DBAT4U_REG                  568
#define     ESAL_CO_MEM_DBAT4L_REG                  569
#define     ESAL_CO_MEM_DBAT5U_REG                  570
#define     ESAL_CO_MEM_DBAT5L_REG                  571
#define     ESAL_CO_MEM_DBAT6U_REG                  572
#define     ESAL_CO_MEM_DBAT6L_REG                  573
#define     ESAL_CO_MEM_DBAT7U_REG                  574
#define     ESAL_CO_MEM_DBAT7L_REG                  575

#if (ESAL_CO_CACHE_AVAILABLE == ESAL_TRUE)

/* This macro invalidates all of the cache at the core level. */
#define         ESAL_CO_MEM_CACHE_ALL_INVALIDATE()                          \
                {                                                           \
                    INT     hid0_val;                                       \
                    INT     hid0_temp;                                      \
                                                                            \
                                                                            \
                    /* Read HID0 read */                                    \
                    ESAL_TS_RTE_SPR_READ(ESAL_CO_MEM_HID0_REG, &hid0_temp); \
                    hid0_val = hid0_temp;                                   \
                    hid0_temp |= (ESAL_CO_MEM_HID0_ICACHE_INVALIDATE |      \
                                  ESAL_CO_MEM_HID0_DCACHE_INVALIDATE);      \
                                                                            \
                    /* Execute invalidate in HID0 reg. */                   \
                    ESAL_TS_RTE_SPR_WRITE(ESAL_CO_MEM_HID0_REG, hid0_temp); \
                                                                            \
                    /* Set to previous HID0 register. */                    \
                    ESAL_TS_RTE_SPR_WRITE(ESAL_CO_MEM_HID0_REG, hid0_val);  \
                                                                            \
                    /* Execute instruction sync instruction */              \
                    ESAL_TS_RTE_ISYNC_EXECUTE();                            \
                }

/* This macro invalidates all of the instruction cache at the core level. */
#define         ESAL_CO_MEM_ICACHE_ALL_INVALIDATE()                         \
                {                                                           \
                    INT     hid0_val;                                       \
                    INT     hid0_temp;                                      \
                                                                            \
                                                                            \
                    /* Read HID0 read */                                    \
                    ESAL_TS_RTE_SPR_READ(ESAL_CO_MEM_HID0_REG, &hid0_temp); \
                    hid0_val = hid0_temp;                                   \
                    hid0_temp |= ESAL_CO_MEM_HID0_ICACHE_INVALIDATE;        \
                                                                            \
                    /* Execute invalidate in HID0 reg. */                   \
                    ESAL_TS_RTE_SPR_WRITE(ESAL_CO_MEM_HID0_REG, hid0_temp); \
                                                                            \
                    /* Set to previous HID0 register. */                    \
                    ESAL_TS_RTE_SPR_WRITE(ESAL_CO_MEM_HID0_REG, hid0_val);  \
                                                                            \
                    /* Execute instruction sync instruction */              \
                    ESAL_TS_RTE_ISYNC_EXECUTE();                            \
                }

/* This macro invalidates all of the data cache at the core level. */
#define         ESAL_CO_MEM_DCACHE_ALL_INVALIDATE()                         \
                {                                                           \
                    INT     hid0_val;                                       \
                    INT     hid0_temp;                                      \
                                                                            \
                                                                            \
                    /* Read HID0 read */                                    \
                    ESAL_TS_RTE_SPR_READ(ESAL_CO_MEM_HID0_REG, &hid0_temp); \
                    hid0_val = hid0_temp;                                   \
                    hid0_temp |= ESAL_CO_MEM_HID0_DCACHE_INVALIDATE;        \
                                                                            \
                    /* Execute invalidate in HID0 reg. */                   \
                    ESAL_TS_RTE_SPR_WRITE(ESAL_CO_MEM_HID0_REG, hid0_temp); \
                                                                            \
                    /* Set to previous HID0 register. */                    \
                    ESAL_TS_RTE_SPR_WRITE(ESAL_CO_MEM_HID0_REG, hid0_val);  \
                                                                            \
                    /* Execute instruction sync instruction */              \
                    ESAL_TS_RTE_ISYNC_EXECUTE();                            \
                }

/* This macro invalidates all instruction cache for the specified address
   range at the core level. */
#define         ESAL_CO_MEM_ICACHE_INVALIDATE(addr, size)                   \
                {                                                           \
                    INT     cache_block = 0;                                \
                                                                            \
                                                                            \
                    do                                                      \
                    {                                                       \
                        /* Execute data block flush invalidate. */          \
                        ESAL_TS_RTE_ICBI_EXECUTE(addr, cache_block);        \
                                                                            \
                        /* Go to next cache block. */                       \
                        cache_block += ESAL_CO_MEM_BLOCK_SIZE;              \
                                                                            \
                        /* Execute instruction sync instruction */          \
                        ESAL_TS_RTE_ISYNC_EXECUTE();                        \
                    } while(cache_block < size);                            \
                }

/* This macro invalidates all data cache for the specified address
   range at the core level. */
#define         ESAL_CO_MEM_DCACHE_INVALIDATE(addr, size)                   \
                {                                                           \
                    ESAL_UNUSED_PARAM(addr);                                \
                    ESAL_UNUSED_PARAM(size);                                \
                    /* There's no method invalidate a range in E300 core */   \
                    /* - invalidating all DCache */                         \
                    ESAL_CO_MEM_DCACHE_ALL_INVALIDATE();                    \
                }

/* This macro flushes all data cache to physical memory (writeback cache)
   and invalidates all data cache entries at the core level. */
#define         ESAL_CO_MEM_DCACHE_ALL_FLUSH_INVAL()                        \
                {                                                           \
                    /* Flush entire data cache */                           \
                    ESAL_CO_MEM_DCACHE_FLUSH_INVAL(0, ESAL_PR_MEM_DCACHE_SIZE);\
                }

/* This macro flushes all data cache to physical memory (writeback cache)
   for the given address range, then invalidates all data cache entries
   at the core level. */
#define         ESAL_CO_MEM_DCACHE_FLUSH_INVAL(addr, size)                  \
                {                                                           \
                    INT     cache_block = 0;                                \
                                                                            \
                                                                            \
                    do                                                      \
                    {                                                       \
                        /* Execute data block flush invalidate. */          \
                        ESAL_TS_RTE_DCBF_EXECUTE(addr, cache_block);        \
                                                                            \
                        /* Go to next cache block. */                       \
                        cache_block += ESAL_CO_MEM_BLOCK_SIZE;              \
                                                                            \
                        /* Execute instruction sync instruction */          \
                        ESAL_TS_RTE_ISYNC_EXECUTE();                        \
                    } while(cache_block < size);                            \
                }

#endif  /* ESAL_CO_CACHE_AVAILABLE == ESAL_TRUE */

#endif  /* ESAL_CO_MEM_DEFS_H */
