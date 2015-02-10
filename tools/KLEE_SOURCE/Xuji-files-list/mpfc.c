
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
/*      mpfc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      MPF - HMI Platform Function                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the HMI functions to    */
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
#include "plat/inc/mpf_extr.h"
#include "plat/es_plat.h"

/* Define external inner-component global data references.  */

static MP_FUN_LIST * MPF_HMIPlatFunctionList = NU_NULL;

/* Define internal function calls.  */

INT MPFC_VersionCheck( INT StructOffset );

/* 创建应用函数表 */
INT MPFC_Create( VOID * FunList )
{
    INT status;

    status = NU_UNAVAILABLE;
    if( FunList )
    {
        /* 初始化应用函数表指针 */
        MPF_HMIPlatFunctionList = ( MP_FUN_LIST *)FunList;
        status = NU_SUCCESS;
    }

    return( status );
}

/* 应用函数表的版本检查 */
INT MPFC_VersionCheck( INT StructOffset )
{
    INT status = NU_FALSE;

    if( MPF_HMIPlatFunctionList )
    {
        if( MPF_HMIPlatFunctionList->size > StructOffset )
        {
            /* 有效版本,执行函数 */
            status = NU_TRUE;
        }
    }
    else
    {
        /* 获取应用程序提供给平台的函数链表 */
        MPF_HMIPlatFunctionList = XJP_Function_Table_Retrieve( MP_FUN_LIST_NAME );
        if( MPF_HMIPlatFunctionList )
        {
            if( MPF_HMIPlatFunctionList->size > StructOffset )
            {
                /* 有效版本,执行函数 */
                status = NU_TRUE;
            }
        }
    }
    return( status );
}

/* 获取HMI版本号 */
INT MPFC_GetHmiVersion( UNSIGNED * verL, UNSIGNED  * verH, UNSIGNED * toolCRC )
{
    INT status;

    /* 版本检查 */
    if( MPFC_VersionCheck( MPF_GETSTRUCTOFFSET( MP_FUN_LIST, get_hmi_ver )) == NU_FALSE )
    {
        return( NU_UNAVAILABLE );
    }
    /* 执行函数 */
    status = MPF_HMIPlatFunctionList->get_hmi_ver( verL, verH, toolCRC );

    return( status );
}

/* 填充GOOSE报文 */
INT MPFC_FillGoose( VOID * pInfo )
{
    INT status;

    /* 版本检查 */
    if( MPFC_VersionCheck( MPF_GETSTRUCTOFFSET( MP_FUN_LIST, fill_goose )) == NU_FALSE )
    {
        return( NU_UNAVAILABLE );
    }
    /* 执行函数 */
    status = MPF_HMIPlatFunctionList->fill_goose( pInfo );

    return( status );
}

/* 突变信息快速发送到HMI */
INT MPFC_SendGoose( VOID )
{
    INT status;

    /* 版本检查 */
    if( MPFC_VersionCheck( MPF_GETSTRUCTOFFSET( MP_FUN_LIST, send_goose )) == NU_FALSE )
    {
        return( NU_UNAVAILABLE );
    }
    /* 执行函数 */
    status = MPF_HMIPlatFunctionList->send_goose();

    return( status );
}
