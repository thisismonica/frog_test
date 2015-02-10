
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
/*      rtcc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      RTC - Clock Time Management                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the clock management    */
/*      component.                                                       */
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
/*      ZhiDeYang       05-22-2010      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "plat/inc/rtc_extr.h"
#include "plat/inc/devi_extr.h"

/* ����ΪOS������� */
#ifdef WIN32
/* ȫ�ֱ������� */
/* Windows �궨�� */
#include <windows.h>
#include <time.h>
void RTC_Win32_GetInitializeTime( TIME * pTime );
#endif

#ifndef WIN32 /* ��windowsȱʡ����һЩ���Ա�ͨ������ */
/* ȫ�ֱ������� */
/* Nucleus�궨�� */
#endif

/* ʱ�ӳ�ʼ������ */
void RTC_Clock_Initial( void )
{
    TIME TempTime;
    TIME TempTime1;
    INT  iStatus;
    UNSIGNED delayTime;
    /*----------------------*/

    iStatus = XJ_UNAVAILABLE;
    delayTime = XJ_Retrieve_Clock();
    /* ��ȡʱ��оƬʱ�� */
    while( 1 )
    {
        /* ��ȡ��ǰϵͳʱ�� */
        if( RTC_Get_Rtc_Time(& TempTime ) == XJ_SUCCESS )
        {
            /* ���ʱ�����Ч�� */
            if( XJP_TM_Time_Check(& TempTime, 1 ) == XJ_TRUE )
            {
                iStatus = XJ_SUCCESS;
                break;
            }
        }

        if(( XJ_Retrieve_Clock() - delayTime ) >= 2000 ) /* ���ȴ�ʱ�� */
        {
            break;
        }
    }

    /* �ȴ�ʱ��оƬʱ����뷢���仯ʱ����ʱ�����ò��� */
    if( iStatus == XJ_SUCCESS )
    {
        delayTime = XJ_Retrieve_Clock();
        while( 1 )
        {
            /* ��ȡ��ǰϵͳʱ�� */
            if( RTC_Get_Rtc_Time(& TempTime1 ) == XJ_SUCCESS )
            {
                /* ���ʱ�����Ч�� */
                if( XJP_TM_Time_Check(& TempTime1, 1 ) == XJ_TRUE )
                {
                    if( TempTime.Millionsecond != TempTime1.Millionsecond )
                    {
                        TempTime1.Nanosecond = 0;
                        /* ��װ��ʱ�䴫�ݸ�����cpu */
                        XJP_TM_Set_Time_Dev_Local(& TempTime1 );
                        break;
                    }
                }
            }

            if(( XJ_Retrieve_Clock() - delayTime ) >= 2000 ) /* ���ȴ�ʱ�� */
            {
                break;
            }
        }
    }
}

/* ��ȡ��ǰϵͳʱ��;����ֵ:1-�ɹ�,0-ʧ�� */
int RTC_Get_Rtc_Time( TIME * pCLOCK_BUF )
{
#ifndef WIN32
    int status;

    /* ��ʱ��оƬʱ�� */
    status = DEVI_GetChipTime( ( sizeof( TIME )/ sizeof( UNSIGNED )), pCLOCK_BUF );

    return( status );
#else
    RTC_Win32_GetInitializeTime( pCLOCK_BUF );
    return( XJ_SUCCESS );
#endif
}

/* ����ϵͳʱ�亯��;;����ֵ:1-�ɹ�,0-ʧ�� */
int RTC_Set_Rtc_Time( TIME * pCLOCK_BUF )
{
    int status;

    /* ����ʱ��оƬʱ�� */
    status = DEVI_SaveChipTime( ( sizeof( TIME )/ sizeof( UNSIGNED )), pCLOCK_BUF );

    return( status );
}

#ifdef WIN32
void RTC_Win32_GetInitializeTime( TIME * pTime )
{
    SYSTEMTIME CurTime;

    //���ó�ʼʱ��
    GetLocalTime(& CurTime );
    pTime->Year = ( CurTime.wYear - 2000 ) & 0xff ;
    pTime->Month = CurTime.wMonth;
    pTime->Day = CurTime.wDay;
    pTime->Hour = CurTime.wHour;
    pTime->Minute = CurTime.wMinute;
    /* second * 1000 add Milliseconds = Milliseconds */
    pTime->Millionsecond = CurTime.wSecond * 1000 + CurTime.wMilliseconds;
}
#endif
