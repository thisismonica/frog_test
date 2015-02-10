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
*       esal_co_cfg.h
*
*   COMPONENT
*
*       ESAL - Embedded Software Abstraction Layer
*
*   DESCRIPTION
*
*       This file contains required configuration settings for the
*       given core.  These configuration settings are used by generic
*       ESAL components and may be used by external components.
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

#ifndef         ESAL_CO_CFG_H
#define         ESAL_CO_CFG_H

/* Define core cache availability
   NOTE:  A differentiation is made in ESAL between cache that
          is contained on a processor and cache that is
          inherent as part of a core (L2 vs L1 cache). */
#define         ESAL_CO_CACHE_AVAILABLE                 ESAL_TRUE

/* Define if critical interrupts are available (ESAL_TRUE) or not (ESAL_FALSE)
   for this core
   NOTE:  An assembler define for this same attribute must also be set in
          esal_co_cfg.inc */
#define         ESAL_CO_CRITICAL_INTS_AVAIL             ESAL_TRUE

#endif  /* ESAL_CO_CFG_H */
