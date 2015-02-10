
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

/* ����ΪOS������� */
#ifdef WIN32
/* ȫ�ֱ������� */
struct EXMemMService EXMMS = {malloc, free};
/* Windows �궨�� */
#endif

#ifndef WIN32 /* ��windowsȱʡ����һЩ���Ա�ͨ������ */
/* ȫ�ֱ������� */
struct EXMemMService EXMMS = {malloc, free};
/* Nucleus�궨�� */
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

/* ��ʼ������������Ϣ */
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
        /* �����û��������ݵĽ��ջ������ռ� */
        MemSize += sizeof( UNSIGNED ) * UserExchangeNumber ;
        pUserRecvMemory = ( VOID *)EXMMS.MemAlloc( MemSize );
        if( pUserRecvMemory == XJ_NULL )
        {
            /* �ڴ治��,ֱ�ӷ���ʧ�� */
            return( XJ_UNAVAILABLE );
        }
        /* �����û��������ݵķ��ͻ������ռ� */
        pUserSendMemory = ( VOID *)EXMMS.MemAlloc( MemSize );
        if( pUserSendMemory == XJ_NULL )
        {
            /* �ڴ治��,�ͷ�ǰ�������ڴ�,ֱ�ӷ���ʧ�� */
            EXMMS.MemFree( pUserRecvMemory );
            return( XJ_UNAVAILABLE );
        }
    }

    /* ����˫CPU�������ݵĽ��ջ������ռ� */
    MemSize += ( sizeof( UNSIGNED ) * CEX_PLAT_EXCHANGE_NUMBER );
    pCpuRecvMemory = ( VOID *)EXMMS.MemAlloc( MemSize );
    if( pCpuRecvMemory == XJ_NULL )
    {
        /* �ڴ治��,�ͷ�ǰ�������ڴ�,ֱ�ӷ���ʧ�� */
        if( UserExchangeNumber != 0 )
        {
            EXMMS.MemFree( pUserRecvMemory );
            EXMMS.MemFree( pUserSendMemory );
        }
        return( XJ_UNAVAILABLE );
    }
    /* ����˫CPU�������ݵķ��ͻ������ռ� */
    pCpuSendMemory = ( VOID *)EXMMS.MemAlloc( MemSize );
    if( pCpuSendMemory == XJ_NULL )
    {
        /* �ڴ治��,�ͷ�ǰ�������ڴ�,ֱ�ӷ���ʧ�� */
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

/* ��ȡ�������Բ�CPU���ı��������� */
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

/* ��ȡ���յ����û��������� */
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

/* ������Ҫ���͵��û��������� */
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
