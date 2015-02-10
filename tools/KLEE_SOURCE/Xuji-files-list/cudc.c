
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
/* ����ΪOS������� */
#ifdef WIN32
/* ȫ�ֱ������� */
/* Windows �궨�� */
#endif

#ifndef WIN32 /* ��windowsȱʡ����һЩ���Ա�ͨ������ */
/* ȫ�ֱ������� */
/* Nucleus�궨�� */
#endif

/* Define external inner-component global data references.  */
NU_PROTECT CUDC_UserDataProtect;
INT CUDC_DataReady = 0; /* �û����ݱ�־ */
UNSIGNED CUDC_UserData[ CUD_USER_DATA_NUMBER ]; /* �û����ݻ����� */
/* �û����ݹ����� */
extern UNSIGNED UserDataHandle;
/* Define internal function calls.  */

/* �궨�� */
#define WRITENVDATA(x,y,z,u,v,w) x=XJP_NVData_Write(y,z,u,v,w) //�̻�����ʧ��������
#define READNVDATA(x,y,z,u,v,w) x=XJP_NVData_Read(y,z,u,v,w) //��ȡ����ʧ��������
#define REPAIRNVDATA(x,y,z) x=XJP_NVData_Repair(y,z) //�޸�����ʧ��������

/* �����û����� */
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

/* ��ȡ�û����� */
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

/* ����û������Ƿ����仯 */
VOID CUDC_CheckUserDataChange( VOID )
{
    static   UNSIGNED UserData[ CUD_USER_DATA_NUMBER ]; /* �û����ݻ����� */
    UNSIGNED TempUserData[ CUD_USER_DATA_NUMBER ]; /* �û����ݻ����� */
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
        /* �����ݻ����� */
        memset( UserData, 0, Size );
        /* �����ݴӷ���ʧоƬ���������� */
        READNVDATA( iStatus, ( HNDM )UserDataHandle, UserData,
            ( UNSIGNED )0, Size, CUD_NV_TIME_OUT );
    }
    NU_Protect( & CUDC_UserDataProtect );
    memcpy( TempUserData, CUDC_UserData, Size );
    NU_Unprotect();
    /* �ж������Ƿ����仯 */
    for( i = 0; i < CUD_USER_DATA_NUMBER; i ++)
    {
        if( TempUserData[i] != UserData[i] )
        {
            iFlag = NU_TRUE;
            break;
        }
    }

    /* �ж��Ƿ���Ҫ�������� */
    if( iFlag == NU_TRUE )
    {
        WRITENVDATA( iStatus, ( HNDM )UserDataHandle, TempUserData,
            ( UNSIGNED )0, Size, CUD_NV_TIME_OUT );
        if( iStatus == NU_SUCCESS )
        {
            /* �������ݻ����� */
            memcpy( UserData, TempUserData, Size );
        }
    }
}

/* �û����ݳ�ʼ�� */
INT CUDC_UserDataInitialize( UNSIGNED uTimeOut )
{
    UNSIGNED Size;
    INT      iStatus;
    UNSIGNED TempUserData[ CUD_USER_DATA_NUMBER ]; /* �û����ݻ����� */
    /*================================*/

    Size = sizeof( UNSIGNED ) * CUD_USER_DATA_NUMBER;
    /* �޸��û����� */
    REPAIRNVDATA( iStatus, ( HNDM )UserDataHandle, uTimeOut );
    /* �����ݴӷ���ʧоƬ���������� */
    READNVDATA( iStatus, ( HNDM )UserDataHandle, TempUserData,
        ( UNSIGNED )0, Size, uTimeOut );
    /* �ж϶�ȡ�����Ƿ�ɹ� */
    if( iStatus == NU_SUCCESS )
    {
        NU_Protect( & CUDC_UserDataProtect );
        memcpy( CUDC_UserData, TempUserData, Size );
        NU_Unprotect();
    }
    return( NU_SUCCESS );
}
