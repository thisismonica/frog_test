
/*************************************************************************/
/*                                                                       */
/*        Copyright (c) 1999-2002 XJ ELECTRIC CO.LTD.                    */
/*                                                                       */
/* PROPRIETARY RIGHTS of XJ ELECTRIC CO.LTD are involved in the          */
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
/*      gpf_defs.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      GP - GWS to PlatForm                                             */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains system constants common to both the           */
/*      application and the actual Nucleus PLUS components.  This file   */
/*      also contains data structure definitions that hide internal      */
/*      information from the application.                                */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* DATA STRUCTURES                                                       */
/*                                                                       */
/*                                                                       */
/* FUNCTIONS                                                             */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       01-04-2008      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

/* Check to see if this file has been included already.  */

#ifndef GPF_DEFS_H
#define GPF_DEFS_H

#define GP_FUN_LIST_NAME "GwsPlatFunList" /* 网关函数表名称(平台使用) */

/* Core processing functions.  */
/*=======================================================================*/
typedef INT (* _get_gws_ver )
    ( UNSIGNED * verL, UNSIGNED  * verH );

typedef int (* _gws_printf )
    ( const char * szFormat, ... );
/*=======================================================================*/
typedef struct GP_FUN_LIST_STRUCT
{
    int size;
    _get_gws_ver                        get_gws_ver;
    _gws_printf                         gws_printf;
}GP_FUN_LIST;

#endif
