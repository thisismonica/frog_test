
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
/*      cexc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CEX - Common Exchange Data                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common exchange     */
/*      data component.                                                  */
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
/*      cem_extr.h                          Protect Control functions    */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       08-13-2008      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "plat/inc/cex_extr.h"

/* 以下为OS相关配置 */
#ifdef WIN32
/* 全局变量申明 */
struct EXMemMService EXMMS = {malloc, free};
/* Windows 宏定义 */
#endif

#ifndef WIN32 /* 非windows缺省定义一些宏以便通过编译 */
/* 全局变量申明 */
struct EXMemMService EXMMS = {malloc, free};
/* Nucleus宏定义 */
#endif

/* Define external inner-component global data references.  */

CEXCHANGE CExchange =
{
    0,                                  /* CpuExchangeNumber */
    XJ_NULL,                            /* CpuExchangeRecv */
    XJ_NULL,                            /* CpuExchangeSend */
    0,                                  /* UserExchangeNumber */
    XJ_NULL,                            /* UserExchangeRecv */
    XJ_NULL,                            /* UserExchangeSend */
};

/* Define internal function calls.  */

/* 初始化交换数据信息 */
INT CEXC_Initialize( INT UserExchangeNumber )
{
    unsigned int MemSize;
    VOID * pCpuRecvMemory;
    VOID * pCpuSendMemory;
    VOID * pUserRecvMemory;
    VOID * pUserSendMemory;
    INT status = XJ_UNAVAILABLE; /* complete status */

    MemSize = 0;
    if( UserExchangeNumber != 0 )
    {
        /* 申请用户交换数据的接收缓冲区空间 */
        MemSize += sizeof( UNSIGNED ) * UserExchangeNumber ;
        pUserRecvMemory = ( VOID *)EXMMS.MemAlloc( MemSize );
        if( pUserRecvMemory == XJ_NULL )
        {
            /* 内存不够,直接返回失败 */
            return( XJ_UNAVAILABLE );
        }
        /* 申请用户交换数据的发送缓冲区空间 */
        pUserSendMemory = ( VOID *)EXMMS.MemAlloc( MemSize );
        if( pUserSendMemory == XJ_NULL )
        {
            /* 内存不够,释放前面分配的内存,直接返回失败 */
            EXMMS.MemFree( pUserRecvMemory );
            return( XJ_UNAVAILABLE );
        }
    }

    /* 申请双CPU交换数据的接收缓冲区空间 */
    MemSize += ( sizeof( UNSIGNED ) * CEX_PLAT_EXCHANGE_NUMBER );
    pCpuRecvMemory = ( VOID *)EXMMS.MemAlloc( MemSize );
    if( pCpuRecvMemory == XJ_NULL )
    {
        /* 内存不够,释放前面分配的内存,直接返回失败 */
        if( UserExchangeNumber != 0 )
        {
            EXMMS.MemFree( pUserRecvMemory );
            EXMMS.MemFree( pUserSendMemory );
        }
        return( XJ_UNAVAILABLE );
    }
    /* 申请双CPU交换数据的发送缓冲区空间 */
    pCpuSendMemory = ( VOID *)EXMMS.MemAlloc( MemSize );
    if( pCpuSendMemory == XJ_NULL )
    {
        /* 内存不够,释放前面分配的内存,直接返回失败 */
        EXMMS.MemFree( pCpuRecvMemory );
        if( UserExchangeNumber != 0 )
        {
            EXMMS.MemFree( pUserRecvMemory );
            EXMMS.MemFree( pUserSendMemory );
        }
        return( XJ_UNAVAILABLE );
    }
    CExchange.CpuExchangeNumber = UserExchangeNumber + CEX_PLAT_EXCHANGE_NUMBER;
    CExchange.CpuExchangeRecv = pCpuRecvMemory;
    CExchange.CpuExchangeSend = pCpuSendMemory;
    CExchange.UserExchangeNumber = UserExchangeNumber;
    CExchange.UserExchangeRecv = pUserRecvMemory;
    CExchange.UserExchangeSend = pUserSendMemory;
    return( XJ_SUCCESS );
}

/* 获取交换（对侧CPU）的报告故障序号 */
INT CEXC_GetExchangeFaultNumber( VOID )
{
    UNSIGNED * pSrc;
    INT iFaultNumber = 0;

    pSrc = ( UNSIGNED * )CExchange.CpuExchangeRecv;
    if( pSrc != XJ_NULL )
    {
        iFaultNumber = ( INT )(*( pSrc + CEX_EXCHANGE_FAULT_NUMBER_OFFSET ));
    }

    return( iFaultNumber );
}

/* 读取接收到的用户交换数据 */
INT CEXC_ReadUserExchangeData( UNSIGNED * pBuffer )
{
    register INT k;
    register UNSIGNED * pSrc;
    register UNSIGNED * pDst;
    register INT UserExchangeNumber;
    INT status = XJ_UNAVAILABLE;

    pDst = ( UNSIGNED *)pBuffer;
    pSrc = ( UNSIGNED * )CExchange.UserExchangeRecv;
    UserExchangeNumber = CExchange.UserExchangeNumber;
    if( pSrc != XJ_NULL )
    {
        for( k = 0; k < UserExchangeNumber; k ++)
        {
            * pDst ++ = * pSrc ++;
        }
        status = XJ_SUCCESS;
    }

    return( status );
}

/* 保存需要发送的用户交换数据 */
INT CEXC_SendUserExchangeData( UNSIGNED * pBuffer )
{
    register INT k;
    register UNSIGNED * pSrc;
    register UNSIGNED * pDst;
    register INT UserExchangeNumber;
    INT status = XJ_UNAVAILABLE;

    pSrc = ( UNSIGNED *)pBuffer;
    pDst = ( UNSIGNED * )CExchange.UserExchangeSend;
    UserExchangeNumber = CExchange.UserExchangeNumber;
    if( pDst != XJ_NULL )
    {
        for( k = 0; k < UserExchangeNumber; k ++)
        {
            * pDst ++ = * pSrc ++;
        }
        status = XJ_SUCCESS;
    }

    return( status );
}
