/*************************************************************************/
/*                                                                       */
/*        Copyright (c) 1999-2002 XJ ELECTRIC CO.LTD.                    */
/*                                                                       */
/* PROPRIETARY RIGHTS of Accelerated Technology are involved in the      */
/* subject matter of this material.  All manufacturing, reproduction,    */
/* use, and sales rights pertaining to this subject matter are governed  */
/* by the license agreement.  The recipient of this software implicitly  */
/* accepts the terms of the license.                                     */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*                                                                       */
/* FILE NAME                                            VERSION          */
/*                                                                       */
/*      pdf_defs.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      PDF - Platform Driver Function                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains function prototypes of all functions          */
/*      accessible to other components.                                  */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* DATA STRUCTURES                                                       */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* FUNCTIONS                                                             */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhideYang.      02-08-2010      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
/* Check to see if the file has been included already.  */

#ifndef PDF_DEFS_H
#define PDF_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Core processing functions.  */
/*=======================================================================*/
typedef INT (* _register_function_list )( CHAR * Name, VOID * FunTablePtr );

typedef VOID * (* _get_function_list )( CHAR * Name );

typedef UNSIGNED (* _retrieve_system_clock )( VOID );

typedef INT (* _control_interrupts )( INT new_level );
/*=======================================================================*/
/* Define the data structure for use within the core processing functions.  */
typedef struct DTRANSFER_STRUCT
{
    /*  Standard protect struct information.   Changes made
    to this area of the structure can have undesirable side effects. */
    int size;
    _register_function_list             register_function_list;
    _get_function_list                  get_function_list;
    _retrieve_system_clock              retrieve_system_clock;
    _control_interrupts                 control_interrupts;
}DTRANSFER;

/* 模块装载定义 */
#ifndef XJ_LOAD_MODULE
#define XJ_LOAD_MODULE
typedef struct APP_ENTRY_STRUCT
{
    UNSIGNED    Key1;
    INT         (* LinkApplication )( VOID * PlatFuncList );
    INT         (* IniApplication )( VOID * mem_pool, VOID * uncached_mem_pool );
    INT         (* LeadApplication )( VOID );
    UNSIGNED    Key2;
    UNSIGNED    CRC;
}APP_ENTRY;
#endif

#ifdef __cplusplus
}
#endif

#endif
