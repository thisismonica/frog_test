
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

/* 以下为OS相关配置 */
#ifdef WIN32
/* 全局变量申明 */
/* Windows 宏定义 */
#include <windows.h>
#include <time.h>
void RTC_Win32_GetInitializeTime( TIME * pTime );
#endif

#ifndef WIN32 /* 非windows缺省定义一些宏以便通过编译 */
/* 全局变量申明 */
/* Nucleus宏定义 */
#endif

/* 时钟初始化函数 */
void RTC_Clock_Initial( void )
{
    TIME TempTime;
    TIME TempTime1;
    INT  iStatus;
    UNSIGNED delayTime;
    /*----------------------*/

    iStatus = XJ_UNAVAILABLE;
    delayTime = XJ_Retrieve_Clock();
    /* 读取时钟芯片时间 */
    while( 1 )
    {
        /* 获取当前系统时间 */
        if( RTC_Get_Rtc_Time(& TempTime ) == XJ_SUCCESS )
        {
            /* 检测时间的有效性 */
            if( XJP_TM_Time_Check(& TempTime, 1 ) == XJ_TRUE )
            {
                iStatus = XJ_SUCCESS;
                break;
            }
        }

        if(( XJ_Retrieve_Clock() - delayTime ) >= 2000 ) /* 最大等待时间 */
        {
            break;
        }
    }

    /* 等待时钟芯片时间的秒发生变化时进行时间设置操作 */
    if( iStatus == XJ_SUCCESS )
    {
        delayTime = XJ_Retrieve_Clock();
        while( 1 )
        {
            /* 获取当前系统时间 */
            if( RTC_Get_Rtc_Time(& TempTime1 ) == XJ_SUCCESS )
            {
                /* 检测时间的有效性 */
                if( XJP_TM_Time_Check(& TempTime1, 1 ) == XJ_TRUE )
                {
                    if( TempTime.Millionsecond != TempTime1.Millionsecond )
                    {
                        TempTime1.Nanosecond = 0;
                        /* 将装置时间传递给保护cpu */
                        XJP_TM_Set_Time_Dev_Local(& TempTime1 );
                        break;
                    }
                }
            }

            if(( XJ_Retrieve_Clock() - delayTime ) >= 2000 ) /* 最大等待时间 */
            {
                break;
            }
        }
    }
}

/* 获取当前系统时间;返回值:1-成功,0-失败 */
int RTC_Get_Rtc_Time( TIME * pCLOCK_BUF )
{
#ifndef WIN32
    int status;

    /* 读时钟芯片时间 */
    status = DEVI_GetChipTime( ( sizeof( TIME )/ sizeof( UNSIGNED )), pCLOCK_BUF );

    return( status );
#else
    RTC_Win32_GetInitializeTime( pCLOCK_BUF );
    return( XJ_SUCCESS );
#endif
}

/* 设置系统时间函数;;返回值:1-成功,0-失败 */
int RTC_Set_Rtc_Time( TIME * pCLOCK_BUF )
{
    int status;

    /* 保存时钟芯片时间 */
    status = DEVI_SaveChipTime( ( sizeof( TIME )/ sizeof( UNSIGNED )), pCLOCK_BUF );

    return( status );
}

#ifdef WIN32
void RTC_Win32_GetInitializeTime( TIME * pTime )
{
    SYSTEMTIME CurTime;

    //设置初始时间
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
