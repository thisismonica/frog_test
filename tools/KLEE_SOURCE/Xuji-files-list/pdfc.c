
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
/*      pdfc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      PDF - Platform Driver Function                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the platform            */
/*      functions to device driver.                                      */
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
#include <stdio.h>
#include <math.h>
#include "plat/inc/xj_cosc.h"
#include "plat/inc/pro_defs.h"
#include "plat/inc/pdf_defs.h"
#include "plat/inc/xj_pdfc.h"
#include "plat/es_plat.h"

#define PDFC_DRV_TEXT_CODE1  0xdf00df00  /* the first code for searching protect */
#define PDFC_DRV_TEXT_CODE2  0x00fd00fd  /* the second code for searching protect */

/* Define external inner-component global data references.  */

DTRANSFER  * PlatDriverFunctionList;
DTRANSFER  PlatDriverFunction;
APP_ENTRY * DriverEntryFunctionList = XJ_NULL;

/* Define internal function calls. */

/* 初始化平台提供给驱动使用的服务函数表 */
VOID PDFC_PlatDriverFunList_Initialize( VOID )
{
    /* Initialize the function list data of platform support.  */
    PlatDriverFunctionList = & PlatDriverFunction;
    PlatDriverFunctionList -> size = sizeof( DTRANSFER );
    /* 注册函数表函数 */
    PlatDriverFunctionList -> register_function_list = XJP_Function_Table_Register;
    /* 获取函数表函数 */
    PlatDriverFunctionList -> get_function_list = XJP_Function_Table_Retrieve;
    /* 获取系统时间函数 */
    PlatDriverFunctionList -> retrieve_system_clock = XJP_TM_Get_Clock_Millisecond;
    /* 控制屏蔽中断函数 */
    PlatDriverFunctionList -> control_interrupts = NU_Control_Interrupts;
}

/* 连接驱动程序 */
INT PDFC_LinkDriverApplication(  UNSIGNED StartAddr, UNSIGNED Length, VOID(* WatchDogReset )( VOID ) )
{
    HAPP hDriverHandle;
    char FileName[] = "ProConfig.ini";
    char SectName[] = "DRIVER";
    char KeyName[] = "DllName";
    char EntryName[] = "GetDriverEntry";
    INT status = XJP_LOAD_ERROR;

    /* 创建链接对象 */
    hDriverHandle = XJP_Create_LoadObj( StartAddr, Length, PDFC_DRV_TEXT_CODE1,
        PDFC_DRV_TEXT_CODE2, PlatDriverFunctionList, sizeof( DTRANSFER ),
        FileName, SectName, KeyName, EntryName );
    if( hDriverHandle != 0 )
    {
        /* 链接应用对象 */
        status = XJP_Link_Application( hDriverHandle, WatchDogReset,
            XJP_CRC_Calculate,& DriverEntryFunctionList );
    }
    return( status );
}

/* 初始化驱动程序 */
INT PDFC_InitializeDriverApplication( VOID * mem_pool, VOID * uncached_mem_pool )
{
    INT status = NU_UNAVAILABLE;

    if( DriverEntryFunctionList != XJ_NULL )
    {
        /* 驱动程序装载成功 */
        if( DriverEntryFunctionList->IniApplication != XJ_NULL )
        {
            status = (* DriverEntryFunctionList->IniApplication )( mem_pool, uncached_mem_pool );
        }
    }
    return( status );
}

/* 引导驱动程序 */
INT PDFC_LeadDriverApplication( VOID )
{
    INT status = XJ_UNAVAILABLE;

    if( DriverEntryFunctionList != XJ_NULL )
    {
        /* 驱动程序装载成功 */
        if( DriverEntryFunctionList->LeadApplication != XJ_NULL )
        {
            status = (* DriverEntryFunctionList->LeadApplication )( );
        }
    }
    return( status );
}
