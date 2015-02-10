
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
/*      pgfc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      PGF - Platform GWS Function                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the platform            */
/*      functions to GWS61850.                                           */
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
#include "plat/inc/pgf_defs.h"
#include "plat/inc/xj_pgfc.h"
#include "plat/inc/cer_extr.h"
#include "plat/es_plat.h"
#include "plat_specific/plat_specific.h"
#include "plat/inc/devi_extr.h"
#include "plat/inc/xj_clog.h"
#include "plat/inc/ccm_extr.h"
#include "plat/inc/tab_defs.H"

#define PGFC_HMI_TEXT_CODE1  0xde00de00  /* the first code for searching protect */
#define PGFC_HMI_TEXT_CODE2  0x00ed00ed  /* the second code for searching protect */

/* Define external inner-component global data references.  */

GTRANSFER  * PlatGwsFunctionList;
GTRANSFER  PlatGwsFunction;
APP_ENTRY * GwsEntryFunctionList = XJ_NULL;

extern volatile UNSIGNED TMD_System_Clock;
/* 注册UART串口中断函数 */
extern INT MainRegUartIntFun( INT channel, VOID (* UartComIntFun )( int para ));
/* 获取UART串口中断向量ID */
extern UNSIGNED MainGetUartVectorID( INT channel );

/* Define internal function calls. */
UNSIGNED PGFC_Get_System_Clock( VOID );
/* 获取当前时间相对于1970年的秒数 */
UNSIGNED PGFC_Get_Time_Second_1970( VOID );
#if WIN32
/* 获取纳秒时间值 */
UNSIGNED Get_system_clock_nanosecond( VOID );
#else
/* 获取纳秒时间值 */
long long Get_system_clock_nanosecond( VOID );
#endif
/* 把日期和时间转换为格林尼治标准时间 */
INT PGFC_TIME_TO_GMT( TIME * time );
/* 中断向量设置 */
INT PGFC_GE_INT_Enable( INT vector_id, INT trigger_type, INT priority );
/* 禁止中断 */
INT PGFC_GE_INT_Disable( INT vector_id );
/* 设置61850初始化完成标志 */
INT PGFC_Gws_Set_Init_Status( INT iniFlag );
/* 设置系统时间 */
INT PGFC_Set_Time_Dev_Zone( TIME * pTime , INT iTimeZone );
/* 设置61850时间 */
INT PGFC_Set_Time_61850( UNSIGNED UTCSecond, UNSIGNED UTCNanoSecond, TIME * pTime );
/* 设置对时时间间隔 */
INT PGFC_SET_Time_Interval( UNSIGNED TimeInterval );
/* 设置对时时间品质因素 */
INT PGFC_SET_Time_Quality( UNSIGNED Quality, INT CondType );

/* 初始化平台提供给GWS使用的服务函数表 */
VOID PGFC_PlatGwsFunList_Initialize( VOID )
{
    /* Initialize the function list data of platform support.  */
    PlatGwsFunctionList = & PlatGwsFunction;
    PlatGwsFunctionList -> size = sizeof( GTRANSFER );
    /* 注册函数表函数 */
    PlatGwsFunctionList -> register_function_list = XJP_Function_Table_Register;
    /* 获取函数表函数 */
    PlatGwsFunctionList -> get_function_list = XJP_Function_Table_Retrieve;
    /* 操作系统管理函数 */
    PlatGwsFunctionList -> get_os_fun_list = XJP_OS_Get_FuncList;
    /* 获取装置当前时间函数 */
    PlatGwsFunctionList -> retrieve_time = XJP_TM_Get_Time_Dev;
    /* 获取系统时钟函数 */
    PlatGwsFunctionList -> retrieve_system_clock = PGFC_Get_System_Clock;
    /* 获取当前时间相对于1970年的秒数 */
    PlatGwsFunctionList -> get_time_second_1970 = PGFC_Get_Time_Second_1970;
    /* 获取纳秒时间值 */
    PlatGwsFunctionList -> get_time_nanosecond = Get_system_clock_nanosecond;
    /* 把日期和时间转换为格林尼治标准时间 */
    PlatGwsFunctionList -> time_to_gmt = PGFC_TIME_TO_GMT;
    /* 中断向量设置 */
    PlatGwsFunctionList -> ge_int_enable = PGFC_GE_INT_Enable;
    /* 禁止中断 */
    PlatGwsFunctionList -> ge_int_disable = PGFC_GE_INT_Disable;
    /* 创建看门狗管理对象 */
    PlatGwsFunctionList -> create_watchdog_handle = XJP_Watchdog_Create;
    /* 复位看门狗监视对象 */
    PlatGwsFunctionList -> reset_watchdog_object = XJP_Watchdog_Reset;
    /* 设置时间函数 */
    PlatGwsFunctionList -> set_time = PGFC_Set_Time_Dev_Zone;
    /* 设置61850初始化完成标志 */
    PlatGwsFunctionList -> gws_set_init_status = PGFC_Gws_Set_Init_Status;
    /* 创建数据保护 */
    PlatGwsFunctionList -> create_lock_data = XJP_NU_Protect_Create;
    /* 数据保护 */
    PlatGwsFunctionList -> lock_data = XJP_NU_Protect;
    /* 释放数据保护 */
    PlatGwsFunctionList -> unlock_data = XJP_NU_UnProtect;
    /* 获取互斥量 */
    PlatGwsFunctionList -> obtain_mutex = XJP_Mutex_Obtain;
    /* 释放互斥量 */
    PlatGwsFunctionList -> release_mutex = XJP_Mutex_Release;
    /* 获取装置时区 */
    PlatGwsFunctionList -> get_time_zone = XJP_TM_Get_Time_Zone;
    /* 软复位 */
    PlatGwsFunctionList -> reset_device = DEVI_ResetCPU;
    /* 读取硬件标识号 */
    PlatGwsFunctionList -> get_identification_code = DEVI_GetIdentificationCode;
    /* 获取装置功能选择 */
    PlatGwsFunctionList -> get_device_fun_select = PMI_get_device_fun_select;
    /* 设置61850时间 */
    PlatGwsFunctionList -> set_time_61850 = PGFC_Set_Time_61850;
    /* 设置对时时间间隔 */
    PlatGwsFunctionList -> set_time_interval = PGFC_SET_Time_Interval;
    /* 设置对时时间品质因素 */
    PlatGwsFunctionList -> set_time_quality = PGFC_SET_Time_Quality;
    /* 日志记录 */
    PlatGwsFunctionList -> write_log = CLOG_Log;
    /* 创建数据队列管理函数 */
    PlatGwsFunctionList -> create_data_queue = XJP_CreateDataQueue;
    /* 向数据队列发送数据函数 */
    PlatGwsFunctionList -> send_to_data_queue = XJP_SendToDataQueue;
    /* 从数据队列获取数据函数 */
    PlatGwsFunctionList -> receive_from_data_queue = XJP_ReceiveFromDataQueue;
    /* 注册UART串口中断函数 */
    PlatGwsFunctionList -> reg_uart_int_fun = MainRegUartIntFun;
    /* UART串口初始化 */
    PlatGwsFunctionList -> uart_com_init = DEVI_UartComInit;
    /* 设置UART485使能标识 */
    PlatGwsFunctionList -> uart_rs485_set_enflag = DEVI_UART_RS485_SetEnFlag;
    /* 向UART串口写数据 */
    PlatGwsFunctionList -> uart_write_comm = DEVI_UART_WriteComm;
    /* 从UART串口读取数据 */
    PlatGwsFunctionList -> uart_read_comm = DEVI_UART_ReadComm;
    /* 获取UART中断状态 */
    /* 中断查询函数,看其所指串口是否处于接口中断状态NU_TRUE:接收中断状态,NU_FALSE:不是接收中断状态 */
    PlatGwsFunctionList -> uart_get_port_int_flag = DEVI_UART_GetPortIntFlag;
    /* 获取UART串口中断向量ID */
    PlatGwsFunctionList -> get_uart_vector_id = MainGetUartVectorID;
    /* 获取数据队列信息 */
    PlatGwsFunctionList -> get_data_queue_information = XJP_DataQueueInformation;
    /* 删除指定数据队列 */
    PlatGwsFunctionList -> delete_data_queue = XJP_DeleteDataQueue;
    /* 获取平台引导标识 */
    PlatGwsFunctionList -> get_plat_lead_flag = CCMC_GetPlatLeadFlag;
}

/* 连接Gws61850程序 */
INT PGFC_LinkGwsApplication(  UNSIGNED StartAddr, UNSIGNED Length, VOID(* WatchDogReset )( VOID ) )
{
    HAPP hGwsHandle;
    char FileName[] = "ProConfig.ini";
    char SectName[] = "PROTECT";
    char KeyName[] = "DllName";
    char EntryName[] = "GetProtectEntry";
    INT status = XJP_LOAD_ERROR;

    /* 创建链接对象 */
    hGwsHandle = XJP_Create_LoadObj( StartAddr, Length, PGFC_HMI_TEXT_CODE1,
        PGFC_HMI_TEXT_CODE2, PlatGwsFunctionList, sizeof( GTRANSFER ),
        FileName, SectName, KeyName, EntryName );
    if( hGwsHandle != 0 )
    {
        /* 链接应用对象 */
        status = XJP_Link_Application( hGwsHandle, WatchDogReset,
            XJP_CRC_Calculate,& GwsEntryFunctionList );
    }
    return( status );
}

/* 初始化Gws61850程序 */
INT PGFC_InitializeGwsApplication( VOID * mem_pool, VOID * uncached_mem_pool )
{
    INT status = NU_UNAVAILABLE;

    if( GwsEntryFunctionList != XJ_NULL )
    {
        /* HMI程序装载成功 */
        if( GwsEntryFunctionList->IniApplication != XJ_NULL )
        {
            status = (* GwsEntryFunctionList->IniApplication )( mem_pool, uncached_mem_pool );
        }
    }
    return( status );
}

/* 引导Gws61850程序 */
INT PGFC_LeadGwsApplication( VOID )
{
    INT status = XJ_UNAVAILABLE;

    if( GwsEntryFunctionList != XJ_NULL )
    {
        /* HMI程序装载成功 */
        if( GwsEntryFunctionList->LeadApplication != XJ_NULL )
        {
            status = (* GwsEntryFunctionList->LeadApplication )( );
        }
    }
    return( status );
}

/* 获取系统时钟函数 */
UNSIGNED PGFC_Get_System_Clock( VOID )
{
    return( TMD_System_Clock );
}

/* 获取当前时间相对于1970年的秒数 */
UNSIGNED PGFC_Get_Time_Second_1970( VOID )
{
    TIME  SysTime;
    UNSIGNED uUTCSecond;
    UNSIGNED uUTCMillionsecond;
    /*======================*/
    /* 获取当前系统时间 */
    XJP_TM_Get_Time_Dev(& SysTime );
    /* 将系统时间转换成其它时间 */
    XJP_TM_Trans_Dev_To_LT( & uUTCSecond, & uUTCMillionsecond, & SysTime );
    /* 返回当前时间相对于1970的秒数 */
    return ( uUTCSecond );
}

/* 把日期和时间转换为格林尼治标准时间 */
INT PGFC_TIME_TO_GMT( TIME * time )
{
    UNSIGNED uUTCSecond;
    UNSIGNED uUTCMillionsecond;
    UNSIGNED uUTCNanoSecond;
    /*======================*/
    /* 获取装置当前61850时间 */
    XJP_TM_Get_Time_61850( XJ_NULL, & uUTCSecond, & uUTCNanoSecond, XJ_NULL );
    uUTCMillionsecond = ( uUTCNanoSecond / 1000000 ); /* 纳秒转换成毫秒 */
    /* 转换为格林尼治标准时间 */
    XJP_TM_Trans_LT_To_Dev( & uUTCSecond, & uUTCMillionsecond, time );
    time->Nanosecond = ( uUTCNanoSecond % 1000000 ); /* 获取纳秒 */
    /* return */
    return( XJ_SUCCESS );
}

/* 中断向量设置 */
INT PGFC_GE_INT_Enable( INT vector_id, INT trigger_type, INT priority )
{
    INT status;

#ifndef WIN32
    status = ESAL_GE_INT_Enable( vector_id,
        ( ESAL_GE_INT_TRIG_TYPE )( ESAL_TRIG_NOT_SUPPORTED + trigger_type ) ,
        priority );
#else
    status = XJ_UNAVAILABLE;
#endif

    return( status );
}

/* 禁止中断 */
INT PGFC_GE_INT_Disable( INT vector_id )
{
    INT status;

#ifndef WIN32
    status = ESAL_GE_INT_Disable( vector_id );
#else
    status = XJ_UNAVAILABLE;
#endif
    return( status );
}

/* 设置61850初始化完成标志 */
INT PGFC_Gws_Set_Init_Status( INT iniFlag )
{
    if( iniFlag )
    {
        /* 初始化完成 */
        CERC_SetUserEventBits( CPC_EVENT1_GWS_INIT_STATUS, 1 );
    }
    else
    {
        /* 初始化未完成 */
        CERC_SetUserEventBits( CPC_EVENT1_GWS_INIT_STATUS, 0 );
    }
    return( XJ_SUCCESS );
}

/* 网关对时间函数设置操作要判别装置数据表对时来源是否为站控层 */
/* 设置系统时间 */
INT PGFC_Set_Time_Dev_Zone( TIME * pTime , INT iTimeZone )
{
    if( pDeviceTab )
    {
        /* 判断对时来源 */
        if( pDeviceTab->SynSrc & PRO_SYN_SRC_STATION )
        {
            return( XJP_TM_Set_Time_Dev_Zone( pTime, iTimeZone ));
        }
        else
        {
            return( NU_FALSE );
        }
    }
    else
    {
        return( XJP_TM_Set_Time_Dev_Zone( pTime, iTimeZone ));
    }
}

/* 设置61850时间 */
INT PGFC_Set_Time_61850( UNSIGNED UTCSecond, UNSIGNED UTCNanoSecond, TIME * pTime )
{
    if( pDeviceTab )
    {
        /* 判断对时来源 */
        if( pDeviceTab->SynSrc & PRO_SYN_SRC_STATION )
        {
            return( XJP_TM_Set_Time_61850( UTCSecond, UTCNanoSecond,  pTime ));
        }
        else
        {
            return( NU_FALSE );
        }
    }
    else
    {
        return( XJP_TM_Set_Time_61850( UTCSecond, UTCNanoSecond,  pTime ));
    }
}

/* 设置对时时间间隔 */
INT PGFC_SET_Time_Interval( UNSIGNED TimeInterval )
{
    if( pDeviceTab )
    {
        /* 判断对时来源 */
        if( pDeviceTab->SynSrc & PRO_SYN_SRC_STATION )
        {
            return( XJP_TM_SET_Time_Interval( TimeInterval ));
        }
        else
        {
            return( NU_FALSE );
        }
    }
    else
    {
        return( XJP_TM_SET_Time_Interval( TimeInterval ));
    }
}

/* 设置对时时间品质因素 */
INT PGFC_SET_Time_Quality( UNSIGNED Quality, INT CondType )
{
    if( pDeviceTab )
    {
        /* 判断对时来源 */
        if( pDeviceTab->SynSrc & PRO_SYN_SRC_STATION )
        {
            return( XJP_TM_SET_Time_Quality( Quality, CondType ));
        }
        else
        {
            return( NU_FALSE );
        }
    }
    else
    {
        return( XJP_TM_SET_Time_Quality( Quality, CondType ));
    }
}
