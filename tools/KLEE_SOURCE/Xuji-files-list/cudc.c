
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
/*      cudc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CUD - Common User Data                                           */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common user         */
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
#include <stdio.h>
#include "plat/inc/cud_extr.h"
#include "plat/es_plat.h"
/* 以下为OS相关配置 */
#ifdef WIN32
/* 全局变量申明 */
/* Windows 宏定义 */
#endif

#ifndef WIN32 /* 非windows缺省定义一些宏以便通过编译 */
/* 全局变量申明 */
/* Nucleus宏定义 */
#endif

/* Define external inner-component global data references.  */
NU_PROTECT CUDC_UserDataProtect;
INT CUDC_DataReady = 0; /* 用户数据标志 */
UNSIGNED CUDC_UserData[ CUD_USER_DATA_NUMBER ]; /* 用户数据缓冲区 */
/* 用户数据管理句柄 */
extern UNSIGNED UserDataHandle;
/* Define internal function calls.  */

/* 宏定义 */
#define WRITENVDATA(x,y,z,u,v,w) x=XJP_NVData_Write(y,z,u,v,w) //固化非易失整定数据
#define READNVDATA(x,y,z,u,v,w) x=XJP_NVData_Read(y,z,u,v,w) //读取非易失整定数据
#define REPAIRNVDATA(x,y,z) x=XJP_NVData_Repair(y,z) //修复非易失整定数据

/* 保存用户数据 */
INT CUDC_SaveUserData( VOID * pBuffer, UNSIGNED uOffset, UNSIGNED uNumber )
{
    UNSIGNED k;
    INT status;
    UNSIGNED * pSrc;
    UNSIGNED * pDst;

    if(( uOffset + uNumber ) > CUD_USER_DATA_NUMBER )
    {
        status = NU_UNAVAILABLE;
    }
    else
    {
        NU_Protect( & CUDC_UserDataProtect );

        pSrc = ( UNSIGNED *)pBuffer;
        pDst = CUDC_UserData;
        pDst += uOffset;
        for( k = 0; k < uNumber; k ++)
        {
            * pDst ++ = * pSrc ++;
        }
        CUDC_DataReady = NU_TRUE;

        NU_Unprotect();
        status = NU_SUCCESS;
    }

    return( status );
}

/* 读取用户数据 */
INT CUDC_GetUserData( VOID * pBuffer, UNSIGNED uOffset, UNSIGNED uNumber )
{
    UNSIGNED k;
    INT status;
    UNSIGNED * pSrc;
    UNSIGNED * pDst;

    if(( uOffset + uNumber ) > CUD_USER_DATA_NUMBER )
    {
        status = NU_UNAVAILABLE;
    }
    else
    {
        NU_Protect( & CUDC_UserDataProtect );

        pDst = ( UNSIGNED *)pBuffer;
        pSrc = CUDC_UserData;
        pSrc += uOffset;
        for( k = 0; k < uNumber; k ++)
        {
            * pDst ++ = * pSrc ++;
        }

        NU_Unprotect();
        status = NU_SUCCESS;
    }

    return( status );
}

/* 检测用户数据是否发生变化 */
VOID CUDC_CheckUserDataChange( VOID )
{
    static   UNSIGNED UserData[ CUD_USER_DATA_NUMBER ]; /* 用户数据缓冲区 */
    UNSIGNED TempUserData[ CUD_USER_DATA_NUMBER ]; /* 用户数据缓冲区 */
    static   INT iInitFlag = 0;
    UNSIGNED Size;
    INT      iStatus;
    INT      i;
    INT      iFlag = NU_FALSE;
    /*================================*/

    Size = sizeof( UNSIGNED ) * CUD_USER_DATA_NUMBER;
    if( iInitFlag == 0 )
    {
        iInitFlag = 1;
        /* 清数据缓冲区 */
        memset( UserData, 0, Size );
        /* 将数据从非易失芯片中引导出来 */
        READNVDATA( iStatus, ( HNDM )UserDataHandle, UserData,
            ( UNSIGNED )0, Size, CUD_NV_TIME_OUT );
    }
    NU_Protect( & CUDC_UserDataProtect );
    memcpy( TempUserData, CUDC_UserData, Size );
    NU_Unprotect();
    /* 判断数据是否发生变化 */
    for( i = 0; i < CUD_USER_DATA_NUMBER; i ++)
    {
        if( TempUserData[i] != UserData[i] )
        {
            iFlag = NU_TRUE;
            break;
        }
    }

    /* 判断是否需要保存数据 */
    if( iFlag == NU_TRUE )
    {
        WRITENVDATA( iStatus, ( HNDM )UserDataHandle, TempUserData,
            ( UNSIGNED )0, Size, CUD_NV_TIME_OUT );
        if( iStatus == NU_SUCCESS )
        {
            /* 更新数据缓冲区 */
            memcpy( UserData, TempUserData, Size );
        }
    }
}

/* 用户数据初始化 */
INT CUDC_UserDataInitialize( UNSIGNED uTimeOut )
{
    UNSIGNED Size;
    INT      iStatus;
    UNSIGNED TempUserData[ CUD_USER_DATA_NUMBER ]; /* 用户数据缓冲区 */
    /*================================*/

    Size = sizeof( UNSIGNED ) * CUD_USER_DATA_NUMBER;
    /* 修复用户数据 */
    REPAIRNVDATA( iStatus, ( HNDM )UserDataHandle, uTimeOut );
    /* 将数据从非易失芯片中引导出来 */
    READNVDATA( iStatus, ( HNDM )UserDataHandle, TempUserData,
        ( UNSIGNED )0, Size, uTimeOut );
    /* 判断读取数据是否成功 */
    if( iStatus == NU_SUCCESS )
    {
        NU_Protect( & CUDC_UserDataProtect );
        memcpy( CUDC_UserData, TempUserData, Size );
        NU_Unprotect();
    }
    return( NU_SUCCESS );
}
