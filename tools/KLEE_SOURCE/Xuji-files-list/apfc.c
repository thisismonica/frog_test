
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
/*      apfc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      APF - Application Platform Function                              */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the application         */
/*      functions to platform.                                           */
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
#include "plat/inc/apf_extr.h"
#include "plat/es_plat.h"

/* Define external inner-component global data references.  */

static AP_FUN_LIST * APF_ProPlatFunctionList = XJ_NULL;

/* Define internal function calls.  */

INT APFC_VersionCheck( INT StructOffset );

/* 创建应用函数表 */
INT APFC_Create( VOID * FunList )
{
    INT status;

    status = XJ_UNAVAILABLE;
    if( FunList )
    {
        /* 初始化应用函数表指针 */
        APF_ProPlatFunctionList = ( AP_FUN_LIST *)FunList;
        status = XJ_SUCCESS;
    }

    return( status );
}

/* 应用函数表的版本检查 */
INT APFC_VersionCheck( INT StructOffset )
{
    INT status = XJ_FALSE;

    if( APF_ProPlatFunctionList )
    {
        if( APF_ProPlatFunctionList->size > StructOffset )
        {
            /* 有效版本,执行函数 */
            status = XJ_TRUE;
        }
    }
    else
    {
        /* 获取应用程序提供给平台的函数链表 */
        APF_ProPlatFunctionList = XJP_Function_Table_Retrieve( AP_FUN_LIST_NAME );
        if( APF_ProPlatFunctionList )
        {
            if( APF_ProPlatFunctionList->size > StructOffset )
            {
                /* 有效版本,执行函数 */
                status = XJ_TRUE;
            }
        }
    }
    return( status );
}

/* 获取应用参数 */
INT APFC_GetParameter( INT Id, void * buffer )
{
    INT status;

    /* 版本检查 */
    if( APFC_VersionCheck( APF_GETSTRUCTOFFSET( AP_FUN_LIST, get_parameter )) == XJ_FALSE )
    {
        return( XJ_UNAVAILABLE );
    }
    /* 执行函数 */
    status = APF_ProPlatFunctionList->get_parameter( Id, buffer );

    return( status );
}

/* 获取装置版本号 */
INT APFC_GetDeviceVersion( UNSIGNED * verL, UNSIGNED  * verH )
{
    INT status;

    /* 版本检查 */
    if( APFC_VersionCheck( APF_GETSTRUCTOFFSET( AP_FUN_LIST, get_device_ver )) == XJ_FALSE )
    {
        return( XJ_UNAVAILABLE );
    }
    /* 执行函数 */
    status = APF_ProPlatFunctionList->get_device_ver( verL, verH );

    return( status );
}

/* 计算应用程序的CRC码 */
UNSIGNED APFC_CalProtextCRC( UNSIGNED InitialValue )
{
    UNSIGNED crc_code;

    /* 版本检查 */
    if( APFC_VersionCheck( APF_GETSTRUCTOFFSET( AP_FUN_LIST, cal_protext_CRC )) == XJ_FALSE )
    {
        crc_code = InitialValue;
        return( crc_code );
    }
    /* 执行函数 */
    crc_code = APF_ProPlatFunctionList->cal_protext_CRC( InitialValue );

    return( crc_code );
}

/* 注册应用程序的通讯命令 */
INT APFC_RegisterCommand( VOID * channel_ptr )
{
    INT status;
    /* 版本检查 */
    if( APFC_VersionCheck( APF_GETSTRUCTOFFSET( AP_FUN_LIST, register_cmd )) == XJ_FALSE )
    {
        return( XJ_UNAVAILABLE );
    }
    /* 执行函数 */
    status = APF_ProPlatFunctionList->register_cmd( channel_ptr );
    return( status );
}

/* 设置启动录波信息 */
INT APFC_SetTestWaveParam( INT iSpace, INT iStartPoint, INT iTotalPoint )
{
    INT status;
    /* 版本检查 */
    if( APFC_VersionCheck( APF_GETSTRUCTOFFSET( AP_FUN_LIST, set_test_wave_param )) == XJ_FALSE )
    {
        return( XJ_UNAVAILABLE );
    }
    /* 执行函数 */
    status = APF_ProPlatFunctionList->set_test_wave_param( iSpace, iStartPoint, iTotalPoint );
    return( status );
}

/* 保存测控数据表数据 */
INT APFC_WriteFckTabData( VOID ** pGsPtrs, INT iNumber )
{
    INT status;
    /* 版本检查 */
    if( APFC_VersionCheck( APF_GETSTRUCTOFFSET( AP_FUN_LIST, write_fck_tab_data )) == XJ_FALSE )
    {
        return( XJ_UNAVAILABLE );
    }
    /* 执行函数 */
    status = APF_ProPlatFunctionList->write_fck_tab_data( pGsPtrs, iNumber );
    return( status );
}

/* 生成故障量值报文 返回值：报文长度 */
INT APFC_PackAsduReport( VOID * src, UNSIGNED_CHAR * dst, UNSIGNED_CHAR cot, UNSIGNED_CHAR index )
{
    INT Size;
    /* 版本检查 */
    if( APFC_VersionCheck( APF_GETSTRUCTOFFSET( AP_FUN_LIST, pack_asdu_report )) == XJ_FALSE )
    {
        return( 0 );
    }
    /* 执行函数 */
    Size = APF_ProPlatFunctionList->pack_asdu_report( src, dst, cot, index );
    return( Size );
}

/* 生成循环遥测报文 返回值：报文长度 */
INT APFC_PackXHYC( UNSIGNED_CHAR * dst, VOID * pGW_SOE )
{
    INT Size;
    /* 版本检查 */
    if( APFC_VersionCheck( APF_GETSTRUCTOFFSET( AP_FUN_LIST, pack_xhyc )) == XJ_FALSE )
    {
        return( 0 );
    }
    /* 执行函数 */
    Size = APF_ProPlatFunctionList->pack_xhyc( dst, pGW_SOE );
    return( Size );
}
