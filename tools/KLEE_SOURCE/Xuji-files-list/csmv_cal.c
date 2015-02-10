
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
/*      smvc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      SMV - Sample Management                                          */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common sample       */
/*      management component.                                            */
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
/*      ZhiDeYang       11-15-2007      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

#include <string.h>
#include <math.h>
#include <stdio.h>
#include "plat/inc/xj_cosc.h"
#include "plat/inc/csmv_extr.h"

/* 傅氏算法计算基波实(虚)部分量 */
float CSMV_mFOURIER( float * SamplePtr, float * SampleEndPtr, float * ParaPtr,
    INT iPoint, INT iNumber );
/* 傅氏算法计算直流分量 */
float CSMV_mDC( float * SamplePtr, float * SampleEndPtr, float Coefficient,
    INT iPoint, INT iNumber );
/* 计算峰值 */
float CSMV_mPEAKVALUE( float * SamplePtr, float * SampleEndPtr, INT iPoint,
    INT iNumber );

/* 傅氏算法计算基波实(虚)部分量 */
float CSMV_mFOURIER( float * SamplePtr, float * SampleEndPtr, float * ParaPtr, INT iPoint, INT iNumber )
{
    INT k;
    float * pSamplePtr;
    float * pParaPtr;
    float Value;

    Value = 0.0;
    pSamplePtr = SamplePtr;
    pParaPtr = ParaPtr;
    if( iNumber >= iPoint )
    {
        /* 一次直接计算完成 */
        for( k = 0; k < iPoint; k ++)
        {
            Value += ((* pSamplePtr --) * (* pParaPtr ++));
        }
    }
    else
    {
        /* 分两次计算，先计算当前采样到下限之间的数据 */
        for( k = 0; k < iNumber; k ++)
        {
            Value += ((* pSamplePtr --) * (* pParaPtr ++));
        }
        /* 再计算上限开始的剩余数据 */
        pSamplePtr = SampleEndPtr;
        for( k = 0; k < ( iPoint - iNumber ); k ++)
        {
            Value += ((* pSamplePtr --) * (* pParaPtr ++));
        }
    }

    return( Value );
}

/* 傅氏算法计算直流分量 */
float CSMV_mDC( float * SamplePtr, float * SampleEndPtr, float Coefficient, INT iPoint, INT iNumber )
{
    INT k;
    float * pSamplePtr;
    float Value;

    Value = 0.0;
    pSamplePtr = SamplePtr;
    if( iNumber >= iPoint )
    {
        /* 一次直接计算完成 */
        for( k = 0; k < iPoint; k ++)
        {
            Value += (* pSamplePtr --);
        }
    }
    else
    {
        /* 分两次计算，先计算当前采样到下限之间的数据 */
        for( k = 0; k < iNumber; k ++)
        {
            Value += (* pSamplePtr --);
        }
        /* 再计算上限开始的剩余数据 */
        pSamplePtr = SampleEndPtr;
        for( k = 0; k < ( iPoint - iNumber ); k ++)
        {
            Value += (* pSamplePtr --);
        }
    }
    Value *= Coefficient;

    return( Value );
}

/* 计算峰值 */
float CSMV_mPEAKVALUE( float * SamplePtr, float * SampleEndPtr, INT iPoint, INT iNumber )
{
    INT k;
    float * pSamplePtr;
    float Value;
    float Variable;

    Value = 0.0;
    pSamplePtr = SamplePtr;
    if( iNumber >= iPoint )
    {
        /* 一次直接计算完成 */
        for( k = 0; k < iPoint; k ++)
        {
            Variable = ( float )fabs( * pSamplePtr -- );
            Value = ( Value > Variable ) ? Value : Variable;
        }
    }
    else
    {
        /* 分两次计算，先计算当前采样到下限之间的数据 */
        for( k = 0; k < iNumber; k ++)
        {
            Variable = ( float )fabs( * pSamplePtr -- );
            Value = ( Value > Variable ) ? Value : Variable;
        }
        /* 再计算上限开始的剩余数据 */
        pSamplePtr = SampleEndPtr;
        for( k = 0; k < ( iPoint - iNumber ); k ++)
        {
            Variable = ( float )fabs( * pSamplePtr -- );
            Value = ( Value > Variable ) ? Value : Variable;
        }
    }

    return( Value );
}

/* 计算模拟量通道的谐波分量 */
float CSMV_CalculateFourier( INT ChannelNumber, float * ParaPtr, INT SampleOffset )
{
    INT Number;
    INT PeriodPoint;
    float * pSamplePtr;
    float * pSampleStartAddr;
    float * pSampleEndAddr;
    float fValue;

    /* 获取通道采样值首地址 */
    pSampleStartAddr = CSMV_GET_ANALOG_MEMORY + ( ChannelNumber * CSMV_GET_CHANNEL_POINT );
    /* 获取通道采样值结束地址 */
    pSampleEndAddr = pSampleStartAddr + CSMV_GET_CHANNEL_POINT - 1;
    /* 获取当前通道采样指针 */
    pSamplePtr = pSampleStartAddr + SampleOffset;

    Number = SampleOffset + 1; /* 到下限之间的采样点数 */
    PeriodPoint = CSMV_GET_PERIOD_POINT;
    fValue = CSMV_mFOURIER( pSamplePtr, pSampleEndAddr, ParaPtr, PeriodPoint, Number );

    return( fValue );
}

/* 计算模拟量通道的直流分量 */
float CSMV_CalculateDC( INT ChannelNumber, float Coefficient, INT SampleOffset )
{
    INT Number;
    INT PeriodPoint;
    float * pSamplePtr;
    float * pSampleStartAddr;
    float * pSampleEndAddr;
    float fValue;

    /* 获取通道采样值首地址 */
    pSampleStartAddr = CSMV_GET_ANALOG_MEMORY + ( ChannelNumber * CSMV_GET_CHANNEL_POINT );
    /* 获取通道采样值结束地址 */
    pSampleEndAddr = pSampleStartAddr + CSMV_GET_CHANNEL_POINT - 1;
    /* 获取当前通道采样指针 */
    pSamplePtr = pSampleStartAddr + SampleOffset;

    Number = SampleOffset + 1; /* 到下限之间的采样点数 */
    PeriodPoint = CSMV_GET_PERIOD_POINT;
    fValue = CSMV_mDC( pSamplePtr, pSampleEndAddr, Coefficient, PeriodPoint, Number );

    return( fValue );
}

/* 计算模拟量通道的峰值 */
float CSMV_CalculatePeakValue( INT ChannelNumber, INT SampleOffset )
{
    INT Number;
    INT PeriodPoint;
    float * pSamplePtr;
    float * pSampleStartAddr;
    float * pSampleEndAddr;
    float fValue;

    /* 获取通道采样值首地址 */
    pSampleStartAddr = CSMV_GET_ANALOG_MEMORY + ( ChannelNumber * CSMV_GET_CHANNEL_POINT );
    /* 获取通道采样值结束地址 */
    pSampleEndAddr = pSampleStartAddr + CSMV_GET_CHANNEL_POINT - 1;
    /* 获取当前通道采样指针 */
    pSamplePtr = pSampleStartAddr + SampleOffset;

    Number = SampleOffset + 1; /* 到下限之间的采样点数 */
    PeriodPoint = CSMV_GET_PERIOD_POINT;
    fValue = CSMV_mPEAKVALUE( pSamplePtr, pSampleEndAddr, PeriodPoint, Number );

    return( fValue );
}
