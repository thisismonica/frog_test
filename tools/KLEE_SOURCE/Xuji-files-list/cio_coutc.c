
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
/*      cioc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CIO - Common Input/Output Management                             */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common input/output */
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
/*      cio_extr.h                          Protect Control functions    */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

#include <string.h>
#include <math.h>
#include "plat/inc/xj_cosc.h"
#include "plat/inc/cout_extr.h"
#include "plat/inc/xj_out_check.h"
#include "plat_specific/plat_specific.h"
#include "plat/inc/devi_extr.h"
#include "plat/es_plat.h"

/* 以下为OS相关配置 */
#ifdef WIN32
#include <stdio.h>
#include <malloc.h>
/* Windows 宏定义 */
#endif

#ifndef WIN32 /* 非windows缺省定义一些宏以便通过编译 */
/* 全局变量申明 */
/* Nucleus宏定义 */
#endif

/* Define external inner-component global data references.  */
UNSIGNED COUTC_RelayBits[ CPC_MAX_OUTPUT_GROUPS ];

/* Define internal function calls. */
VOID COUTC_User_RelayDriver( UNSIGNED GroupNo, UNSIGNED uBits );
VOID COUTC_User_StartDriver( UNSIGNED Flag );

/* 调用开出驱动函数 */
INT COUTC_RelayDriver( INT GroupNumber, UNSIGNED RelayBits )
{
    INT k;
    INT status;
    UNSIGNED Variable;
    ESP_CFG_OUTPUT_PORT * pOutputPort;
    UNSIGNED_CHAR * pOutputPortAddress8;
    unsigned short int * pOutputPortAddress16;
    UNSIGNED * pOutputPortAddress32;

    status = XJ_UNAVAILABLE;
    if( GroupNumber < COUTC_H_OUTPUT_GROUP_NUMBER )
    {
        /* 设置硬件开出量状态 */
        Variable = RelayBits;
        status = DEVI_RelayDriver( GroupNumber, 1, & Variable );
    }
    else if( GroupNumber < COUTC_OUTPUT_GROUP_NUMBER )
    {
        pOutputPort = COUTC_OUTPUT_PORT_MEMORY; /* 开出端口首地址 */
        pOutputPort += GroupNumber;
        Variable = RelayBits;
        Variable ^= ( pOutputPort->ReverseBits );
        switch( pOutputPort->PortBits )
        {
        case 8:  /* 8位端口数据 */
            for( k = 0; k < pOutputPort->PortNumber; k ++)
            {
                /* 写开出端口数据 */
                pOutputPortAddress8 = ( UNSIGNED_CHAR *)( pOutputPort->PortAddress[k] );
                * pOutputPortAddress8 =
                    ( UNSIGNED_CHAR )(( Variable >> ( k * pOutputPort->PortBits )) & ( pOutputPort->PortMasks ));
            }
            COUTC_RelayBits[GroupNumber] = RelayBits;
            status = XJ_SUCCESS;
            break;
        case 16: /* 16位端口数据 */
            for( k = 0; k < pOutputPort->PortNumber; k ++)
            {
                /* 写开出端口数据 */
                pOutputPortAddress16 = ( unsigned short int *)( pOutputPort->PortAddress[k] );
                * pOutputPortAddress16 =
                    ( unsigned short int )(( Variable >> ( k * pOutputPort->PortBits )) & ( pOutputPort->PortMasks ));
            }
            COUTC_RelayBits[GroupNumber] = RelayBits;
            status = XJ_SUCCESS;
            break;
        case 32: /* 32位端口数据 */
            pOutputPortAddress32 = ( UNSIGNED *)( pOutputPort->PortAddress[0] );
            /* 写开出端口数据 */
            * pOutputPortAddress32 = Variable & ( pOutputPort->PortMasks );
            COUTC_RelayBits[GroupNumber] = RelayBits;
            status = XJ_SUCCESS;
            break;
        default :
            break;
        }
    }
    return( status );
}

/* 执行开出用户驱动函数 */
VOID COUTC_User_RelayDriver( UNSIGNED GroupNo, UNSIGNED uBits )
{
    /* 如果开出量组号在平台支持的最大组数之外,直接返回 */
    if( GroupNo >= CPC_MAX_OUTPUT_GROUPS )
    {
        return;
    }

    /* 出口检测时需要知道出口状态 */
    /* 设置出口继电器动作位 */
    OUTC_Check_SetPickupBits( GroupNo, uBits );
    /* 出口执行函数,此处不应直接调用COUTC_RelayDriver,因为有闭锁出口的情况 */
    OUTC_DriveFunction();
    /* 软出口执行位变化检查函数 */
    OUTC_SoftOutExBitsCheck( );
}

/* 执行启动继电器用户驱动函数 */
VOID COUTC_User_StartDriver( UNSIGNED Flag )
{
    /* 出口检测需要知道启动继电器的状态 */
    OUTC_Check_StartDriver( Flag );
    /* 此处不应直接驱动启动继电器,启动继电器应该延时返回 */
}

/* 设置开出量品质因素,不用获取事件位 */
INT COUTC_SetOutputQualityBits( INT GroupNumber, UNSIGNED QualityBits )
{
    INT status;
    UNSIGNED * pQuality;
    ESP_CFG_OUTPUT_PORT * pOutputPort;

    status = XJ_UNAVAILABLE;
    if(( GroupNumber >= COUTC_H_OUTPUT_GROUP_NUMBER )
        && ( GroupNumber < COUTC_OUTPUT_GROUP_NUMBER ))
    {
        /* 有效开出量 */
        pOutputPort = COUTC_OUTPUT_PORT_MEMORY; /* 开出端口首地址 */
        pOutputPort += GroupNumber;
        pQuality = ( UNSIGNED *)pOutputPort->QualityAddress;
        * pQuality = QualityBits;
        status = XJ_SUCCESS;
    }

    return( status );
}

/* 初始化函数 */
VOID COUTC_OutputInitialize( VOID )
{
    /* 注册开出继电器用户驱动函数 */
    XJP_IO_Set_Output_Driver( COUTC_User_RelayDriver );
    /* 注册启动继电器用户驱动函数 */
    XJP_IO_Set_Start_Driver( COUTC_User_StartDriver );
}

/* 读数字开出量状态及品质因素 */
INT COUTC_ReadSoftOutput( INT iGroup, UNSIGNED * pBits, UNSIGNED * pQuality )
{
    UNSIGNED * p;
    ESP_CFG_OUTPUT_PORT * pOutputPort;

    if(( iGroup < COUTC_H_OUTPUT_GROUP_NUMBER ) ||
        ( iGroup >= ( COUTC_H_OUTPUT_GROUP_NUMBER + COUTC_S_OUTPUT_GROUP_NUMBER )))
    {
        /* 非数字开出量，直接返回 */
        * pBits = 0;
        * pQuality = 0;
        return( XJ_UNAVAILABLE );
    }

    pOutputPort = COUTC_OUTPUT_PORT_MEMORY; /* 开出端口首地址 */
    pOutputPort += iGroup;
    p = ( UNSIGNED *)pOutputPort->PortAddress[0];
    * pBits = * p;
    p = ( UNSIGNED *)pOutputPort->QualityAddress;
    * pQuality = * p;

    return( XJ_SUCCESS );
}

/* 设置开出量品质因素检修位,不用获取事件位 */
INT COUTC_SetOutCheckQualityBits( INT GroupNumber, UNSIGNED CheckQuaBits )
{
    INT status;
    UNSIGNED * pQuality;
    ESP_CFG_OUTPUT_PORT * pOutputPort;

    status = XJ_UNAVAILABLE;
    if(( GroupNumber >= COUTC_H_OUTPUT_GROUP_NUMBER )
        && ( GroupNumber < COUTC_OUTPUT_GROUP_NUMBER ))
    {
        /* 有效开出量 */
        pOutputPort = COUTC_OUTPUT_PORT_MEMORY; /* 开出端口首地址 */
        pOutputPort += GroupNumber;
        pQuality = ( UNSIGNED *)pOutputPort->QualityCheckAddr;
        * pQuality = CheckQuaBits;
        status = XJ_SUCCESS;
    }

    return( status );
}

/* 读数字开出量状态,品质因素(有效无效及检修位) */
INT COUTC_ReadSoftOutTotalQua( INT  iGroup, UNSIGNED * pBits, UNSIGNED * pQuality, UNSIGNED * pCheckQuality )
{
    UNSIGNED * p;
    ESP_CFG_OUTPUT_PORT * pOutputPort;

    if(( iGroup < COUTC_H_OUTPUT_GROUP_NUMBER ) ||
        ( iGroup >= ( COUTC_H_OUTPUT_GROUP_NUMBER + COUTC_S_OUTPUT_GROUP_NUMBER )))
    {
        /* 非数字开出量，直接返回 */
        * pBits = 0;
        * pQuality = 0;
        * pCheckQuality = 0;
        return( XJ_UNAVAILABLE );
    }

    pOutputPort = COUTC_OUTPUT_PORT_MEMORY; /* 开出端口首地址 */
    pOutputPort += iGroup;
    p = ( UNSIGNED *)pOutputPort->PortAddress[0];
    * pBits = * p;
    p = ( UNSIGNED *)pOutputPort->QualityAddress;
    * pQuality = * p;
    p = ( UNSIGNED *)pOutputPort->QualityCheckAddr;
    * pCheckQuality = * p;

    return( XJ_SUCCESS );
}
