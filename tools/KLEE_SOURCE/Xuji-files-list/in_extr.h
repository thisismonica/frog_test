/***********************************************************************
*
*            Copyright 1993 Mentor Graphics Corporation
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
*       in_extr.h
*
*   COMPONENT
*
*       IN -    Initialization
*
*   DESCRIPTION
*
*       This file contains function prototypes of all functions
*       accessible to other components.
*
*   DATA STRUCTURES
*
*       None
*
*   DEPENDENCIES
*
*       in_defs.h                           Initialization definitions
*
***********************************************************************/

#include        "plus/inc/in_defs.h"        /* Initialization defintions */

/* Check to see if the file has been included already.  */

#ifndef IN_EXTR
#define IN_EXTR

#ifdef          __cplusplus

/* C declarations in C++     */
extern          "C" {

#endif

/* Define function prototypes */
VOID            INC_Initialize(VOID *first_available_memory);
VOID            INCT_Sys_Mem_Pools_Initialize(VOID  *avail_mem_ptr);

/* External variable declarations */
extern INT      INC_Initialize_State;

#ifdef          __cplusplus

/* End of C declarations */
}

#endif  /* __cplusplus */

#endif /* IN_EXTR */
