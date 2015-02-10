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
*       esal_pr_mem_defs.h
*
*   COMPONENT
*
*       ESAL - Embedded Software Abstraction Layer
*
*   DESCRIPTION
*
*       This file contains processor specific defines, structures, data
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

#ifndef         ESAL_PR_MEM_DEFS_H
#define         ESAL_PR_MEM_DEFS_H

/* Sizes, in bytes, of instruction and data cache for this processor.
   NOTE:  This is the size of the core cache (L1 cache) for this processor. */
#define         ESAL_PR_MEM_ICACHE_SIZE         ESAL_GE_MEM_16K
#define         ESAL_PR_MEM_DCACHE_SIZE         ESAL_GE_MEM_16K



#if (ESAL_PR_CACHE_AVAILABLE == ESAL_TRUE)

/* This macro invalidates all of the cache at the processor level. */
#define         ESAL_PR_MEM_CACHE_ALL_INVALIDATE()                          \
                {                                                           \
                    (VOID)ESAL_NULL                                         \
                }

/* This macro invalidates all of the instruction cache at the processor level. */
#define         ESAL_PR_MEM_ICACHE_ALL_INVALIDATE()                         \
                {                                                           \
                    (VOID)ESAL_NULL                                         \
                }

/* This macro invalidates all of the data cache at the processor level. */
#define         ESAL_PR_MEM_DCACHE_ALL_INVALIDATE()                         \
                {                                                           \
                    (VOID)ESAL_NULL                                         \
                }

/* This macro invalidates all instruction cache for the specified address
   range at the processor level. */
#define         ESAL_PR_MEM_ICACHE_INVALIDATE(addr, size)                   \
                {                                                           \
                    ESAL_UNUSED_PARAM(addr);                                \
                    ESAL_UNUSED_PARAM(size);                                \
                }

/* This macro invalidates all data cache for the specified address
   range at the processor level. */
#define         ESAL_PR_MEM_DCACHE_INVALIDATE(addr, size)                   \
                {                                                           \
                    ESAL_UNUSED_PARAM(addr);                                \
                    ESAL_UNUSED_PARAM(size);                                \
                }

/* This macro flushes all data cache to physical memory (writeback cache)
   and invalidates all data cache entries at the processor level. */
#define         ESAL_PR_MEM_DCACHE_ALL_FLUSH_INVAL()                        \
                {                                                           \
                    (VOID)ESAL_NULL                                         \
                }

/* This macro flushes all data cache to physical memory (writeback cache)
   for the given address range, then invalidates all data cache entries
   at the processor level. */
#define         ESAL_PR_MEM_DCACHE_FLUSH_INVAL(addr, size)                  \
                {                                                           \
                    ESAL_UNUSED_PARAM(addr);                                \
                    ESAL_UNUSED_PARAM(size);                                \
                }

#endif  /* ESAL_PR_CACHE_AVAILABLE == ESAL_TRUE */

#endif  /* ESAL_PR_MEM_DEFS_H */
