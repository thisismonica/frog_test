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
*       hi_extr.h
*
*   COMPONENT
*
*       HI - History Management
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
*       hi_defs.h                           History Management constants
*
***********************************************************************/

#include        "plus/inc/hi_defs.h"        /* Include HI constants      */


/* Check to see if the file has been included already.  */

#ifndef HI_EXTR
#define HI_EXTR

#ifdef          __cplusplus

/* C declarations in C++     */
extern          "C" {

#endif

/* Core processing functions.  */

VOID            HIC_Disable_History_Saving(VOID);
VOID            HIC_Enable_History_Saving(VOID);
VOID            HIC_Make_History_Entry_Service(UNSIGNED param1,
                                        UNSIGNED param2, UNSIGNED param3);
VOID            HIC_Make_History_Entry(DATA_ELEMENT id, UNSIGNED param1,
                                        UNSIGNED param2, UNSIGNED param3);
STATUS          HIC_Retrieve_History_Entry(DATA_ELEMENT *id, UNSIGNED *param1,
                                        UNSIGNED *param2, UNSIGNED *param3,
                                        UNSIGNED *time, NU_TASK **task,
                                        NU_HISR **hisr);

/* Macro for HIC_Make_History_Entry */
#if (NU_HISTORY_LOGGING == NU_TRUE)

#define HIC_MAKE_HISTORY_ENTRY(id, param1, param2, param3) HIC_Make_History_Entry((id), (param1), (param2), (param3))

#else

#define HIC_MAKE_HISTORY_ENTRY(id, param1, param2, parram3)

#endif  /* NU_HISTORY_LOGGING == NU_TRUE */

#ifdef          __cplusplus

/* End of C declarations */
}

#endif  /* __cplusplus */

#endif
