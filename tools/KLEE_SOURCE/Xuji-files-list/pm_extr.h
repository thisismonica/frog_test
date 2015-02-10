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
*       pm_extr.h
*
*   COMPONENT
*
*       PM - Partition Memory Management
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
*       pm_defs.h                           Partition Management const.
*
***********************************************************************/

#include        "plus/inc/pm_defs.h"        /* Include PM constants      */


/* Check to see if the file has been included already.  */

#ifndef PM_EXTR
#define PM_EXTR

#ifdef          __cplusplus

/* C declarations in C++     */
extern          "C" {

#endif

/* Core error checking functions.  */

STATUS          PMCE_Create_Partition_Pool(NU_PARTITION_POOL *pool_ptr,
                        CHAR *name, VOID *start_address, UNSIGNED pool_size,
                        UNSIGNED partition_size, OPTION suspend_type);
STATUS          PMCE_Delete_Partition_Pool(NU_PARTITION_POOL *pool_ptr);
STATUS          PMCE_Allocate_Partition(NU_PARTITION_POOL *pool_ptr,
                        VOID **return_pointer, UNSIGNED suspend);
STATUS          PMCE_Deallocate_Partition(VOID *partition);


/* Core processing functions.  */

STATUS          PMC_Create_Partition_Pool(NU_PARTITION_POOL *pool_ptr,
                        CHAR *name, VOID *start_address, UNSIGNED pool_size,
                        UNSIGNED partition_size, OPTION suspend_type);
STATUS          PMC_Delete_Partition_Pool(NU_PARTITION_POOL *pool_ptr);
STATUS          PMC_Allocate_Partition(NU_PARTITION_POOL *pool_ptr,
                        VOID **return_pointer, UNSIGNED suspend);
STATUS          PMC_Deallocate_Partition(VOID *partition);


/* Information retrieval functions.  */

UNSIGNED        PMF_Established_Partition_Pools(VOID);
STATUS          PMF_Partition_Pool_Information(NU_PARTITION_POOL *pool_ptr,
                  CHAR *name, VOID **start_address, UNSIGNED *pool_size,
                  UNSIGNED *partition_size, UNSIGNED *available,
                  UNSIGNED *allocated, OPTION *suspend_type,
                  UNSIGNED *tasks_waiting, NU_TASK **first_task);
UNSIGNED        PMF_Partition_Pool_Pointers(NU_PARTITION_POOL **pointer_list,
                                                UNSIGNED maximum_pointers);

#ifdef          __cplusplus

/* End of C declarations */
}

#endif  /* __cplusplus */

#endif
