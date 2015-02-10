
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
/*      gpfc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      GPF - GWS Platform Function                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the GWS functions to    */
/*      platform.                                                        */
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
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       01-07-2008      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "plat/inc/gpf_extr.h"
#include "plat/es_plat.h"

/* Define external inner-component global data references.  */

GP_FUN_LIST * GPF_GwsPlatFunctionList = NU_NULL;

/* Define internal function calls.  */

INT GPFC_VersionCheck( INT StructOffset );

/* 应用函数表的版本检查 */
INT GPFC_VersionCheck( INT StructOffset )
{
    INT status = NU_FALSE;

    if( GPF_GwsPlatFunctionList )
    {
        if( GPF_GwsPlatFunctionList->size > StructOffset )
        {
            /* 有效版本,执行函数 */
            status = NU_TRUE;
        }
    }
    else
    {
        /* 获取网关程序提供给平台的函数链表 */
        GPF_GwsPlatFunctionList = XJP_Function_Table_Retrieve( GP_FUN_LIST_NAME );
        if( GPF_GwsPlatFunctionList )
        {
            if( GPF_GwsPlatFunctionList->size > StructOffset )
            {
                /* 有效版本,执行函数 */
                status = NU_TRUE;
            }
        }
    }
    return( status );
}

/* 获取Gws版本号 */
INT GPFC_GetGwsVersion( UNSIGNED * verL, UNSIGNED  * verH )
{
    INT status;

    /* 版本检查 */
    if( GPFC_VersionCheck( GPF_GETSTRUCTOFFSET( GP_FUN_LIST, get_gws_ver )) == NU_FALSE )
    {
        return( NU_UNAVAILABLE );
    }
    /* 执行函数 */
    status = GPF_GwsPlatFunctionList->get_gws_ver( verL, verH );

    return( status );
}

#if 0
/* 输出串口信息 */
int GPFC_Printf( const char * szFormat, ... )
{
    int status;

    /* 版本检查 */
    if( GPFC_VersionCheck( GPF_GETSTRUCTOFFSET( GP_FUN_LIST, gws_printf )) == NU_FALSE )
    {
        return( NU_UNAVAILABLE );
    }
    /* 执行函数 */
    status = GPF_GwsPlatFunctionList->gws_printf( szFormat );

    return( status );
}
#endif
