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
*       rl_extr.h
*
*   COMPONENT
*
*       RL - Release Information
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
*       nucleus.h                           Nucleus PLUS constants
*
***********************************************************************/

#include        "plus/nucleus.h"            /* Include Nucleus constants */


/* Check to see if the file has been included already.  */

#ifndef RL_DEFS
#define RL_DEFS

#ifdef          __cplusplus

/* C declarations in C++     */
extern          "C" {

#endif

/* Release information functions  */

VOID    RLC_Initialize(VOID);
VOID    RLC_Get_Version(UINT* major, UINT* minor);

#ifdef          __cplusplus

/* End of C declarations */
}

#endif  /* __cplusplus */

#endif  /* RL_DEFS */
