
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
/*      pmfc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      PMF - Platform HMI Function                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the platform            */
/*      functions to HMI.                                                */
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
#include "plat/es_plat.h"
#include "plat/inc/pro_defs.h"
#include "plat/inc/pmf_defs.h"
#include "plat/inc/mpf_extr.h"
#include "plat/net_inc/lc_extr.h"
#include "plat/inc/xj_pmfc.h"
#include "plat/inc/cin_extr.h"
#include "plat_specific/plat_specific.h"
#include "plat/inc/xj_pafc.h"
#include "plat/inc/apf_extr.h"
#include "plat/inc/devi_extr.h"
#include "plat/inc/xj_clog.h"
#include "plat/inc/ccm_extr.h"

#define PMFC_HMI_TEXT_CODE1  0xef00ef00  /* the first code for searching protect */
#define PMFC_HMI_TEXT_CODE2  0x00fe00fe  /* the second code for searching protect */

/* Define external inner-component global data references.  */

MTRANSFER  * PlatHMIFunctionList;
MTRANSFER  PlatHMIFunction;
APP_ENTRY * HMIEntryFunctionList = XJ_NULL;

extern volatile UNSIGNED TMD_System_Clock;
extern GW_CHANNEL * pCommChl;
extern UNSIGNED CCMC_hmiCRC;
INT (* PMFC_Save_Time )( TIME * pTime );

/* Define internal function calls. */
UNSIGNED PMFC_Get_System_Clock( VOID );
VOID PMFC_MessM_ParseData( VOID );
/* 填充事件函数 */
INT PMFC_FillEvent( const VOID * pName, INT iEventType );
/* 获取用户数据根目录路径名称函数 */
const CHAR * PMFC_GetUserRootName( VOID );
/* 获取接口程序的CRC */
INT PMFC_GetMmiCrc( UNSIGNED * pBuff );
/* 注册UART串口中断函数 */
extern INT MainRegUartIntFun( INT channel, VOID (* UartComIntFun )( int para ));
/* 中断向量设置 */
INT PMFC_GE_INT_Enable( INT vector_id, INT trigger_type, INT priority );
/* 禁止中断 */
INT PMFC_GE_INT_Disable( INT vector_id );

/* 初始化平台提供给HMI使用的服务函数表 */
VOID PMFC_PlatHMIFunList_Initialize( VOID )
{
    /* 初始化保存时间函数 */
    PMFC_Save_Time = XJ_NULL;
    /* Initialize the function list data of platform support.  */
    PlatHMIFunctionList = & PlatHMIFunction;
    PlatHMIFunctionList -> size = sizeof( MTRANSFER );
    /* 数据表管理函数 */
    PlatHMIFunctionList -> get_table_memory = XJP_DS_Get_Table_Memory;
    /* 获取数据表大小 */
    PlatHMIFunctionList -> get_table_size = XJP_DS_Get_Table_Size;
    /* 注册函数表函数 */
    PlatHMIFunctionList -> register_function_list = XJP_Function_Table_Register;
    /* 获取函数表函数 */
    PlatHMIFunctionList -> get_function_list = XJP_Function_Table_Retrieve;
    /* 操作系统管理函数 */
    PlatHMIFunctionList -> get_os_fun_list = XJP_OS_Get_FuncList;
    /* 获取装置当前时间函数 */
    PlatHMIFunctionList -> retrieve_time = XJP_TM_Get_Time_Dev;
    /* 获取系统时钟函数 */
    PlatHMIFunctionList -> retrieve_system_clock = PMFC_Get_System_Clock;
    /* 计算循环冗余码 */
    PlatHMIFunctionList -> calculate_crc = XJP_CRC_Calculate;
#if (! LC_LINK_HMI_PHY_COM )
    /* 从平台获取通讯数据 */
    PlatHMIFunctionList -> read_command = LCC_Read_Command;
    /* 向平台写通讯数据 */
    PlatHMIFunctionList -> write_command = LCC_Write_Command;
    /* 通信报文处理 */
    PlatHMIFunctionList -> lcc_parase_data = PMFC_MessM_ParseData;
#endif
    /* 设置时间函数 */
    PlatHMIFunctionList -> set_time = XJP_TM_Set_Time_Dev_Zone;
    /* 填充操作记录函数 */
    PlatHMIFunctionList -> fill_event = PMFC_FillEvent;
    /* 开入管理 */
    PlatHMIFunctionList -> read_input = CINC_ReadInputValue;
    /* 创建看门狗管理对象 */
    PlatHMIFunctionList -> create_watchdog_handle = XJP_Watchdog_Create;
    /* 复位看门狗监视对象 */
    PlatHMIFunctionList -> reset_watchdog_object = XJP_Watchdog_Reset;
    /* 获取用户数据根目录路径名称 */
    PlatHMIFunctionList -> get_user_dir_name = PMFC_GetUserRootName;
    /* 设置装置分模块初始化成功函数 */
    PlatHMIFunctionList -> set_modinit_success = CCMC_SetModInitSuccess;
    /* 获取装置分模块初始化完成个数 */
    PlatHMIFunctionList -> get_modinit_count = CCMC_GetModInitCount;
    /* 获取接口程序的CRC */
    PlatHMIFunctionList -> get_mmi_crc = PMFC_GetMmiCrc;
    /* 时间转换函数 */
    PlatHMIFunctionList -> transform_time_from_systime = XJP_TM_Dev_To_MS_UTC;
    /* 接口中直接调用数据表修改函数 */
    PlatHMIFunctionList -> hmi_modif_table_data = XJP_DS_Com_Hmi_Modify;
    /* 保存测控数据表数据 */
    PlatHMIFunctionList -> write_fck_table_data = APFC_WriteFckTabData;
    /* 时间合法性检测函数 */
    PlatHMIFunctionList -> time_check = XJP_TM_Time_Check;
    /* 设置要转换的时间模式,1为PROFIBUS时间,2为UTC时间,缺省为UTC时间  */
    PlatHMIFunctionList -> tm_set_time_mode = XJP_TM_Set_Time_Mode;
    /* 装置时间转换成就地时间 */
    PlatHMIFunctionList -> tm_trans_dev_to_lt = XJP_TM_Trans_Dev_To_LT;
    /* 就地时间转换成装置时间 */
    PlatHMIFunctionList -> tm_trans_lt_to_dev = XJP_TM_Trans_LT_To_Dev;
    /* 获取装置时区 */
    PlatHMIFunctionList -> get_time_zone = XJP_TM_Get_Time_Zone;
    /* 读取61850时间 */
    PlatHMIFunctionList -> read_61850_time = XJP_TM_Get_Time_61850;
    /* 读取键盘键值 */
    PlatHMIFunctionList -> get_key_value = DEVI_GetKeyValue;
    /* 读取硬件标识号 */
    PlatHMIFunctionList -> get_identification_code = DEVI_GetIdentificationCode;
    /* 纳秒UTC转换成TIME类型的装置时间 */
    PlatHMIFunctionList -> transform_ns_utc_to_dev = XJP_TM_NS_UTC_To_Dev;
    /* TIME类型的装置时间转换成纳秒UTC */
    PlatHMIFunctionList -> transform_dev_to_ns_utc = XJP_TM_Dev_To_NS_UTC;
    /* 装置时间转换为MS UTC*/
    PlatHMIFunctionList -> transform_dev_to_ms_utc = XJP_TM_Dev_To_MS_UTC;
    /* MS UTC 转换为装置时间 */
    PlatHMIFunctionList -> transform_ms_utc_to_dev = XJP_TM_MS_UTC_To_Dev;
    /* 日志记录 */
    PlatHMIFunctionList -> write_log = CLOG_Log;
    /* 注册UART串口中断函数 */
    PlatHMIFunctionList -> reg_uart_int_fun = MainRegUartIntFun;
    /* UART串口初始化 */
    PlatHMIFunctionList -> uart_com_init = DEVI_UartComInit;
    /* 设置UART485使能标识 */
    PlatHMIFunctionList -> uart_rs485_set_enflag = DEVI_UART_RS485_SetEnFlag;
    /* 向UART串口写数据 */
    PlatHMIFunctionList -> uart_write_comm = DEVI_UART_WriteComm;
    /* 从UART串口读取数据 */
    PlatHMIFunctionList -> uart_read_comm = DEVI_UART_ReadComm;
    /* 创建数据队列管理函数 */
    PlatHMIFunctionList -> create_data_queue = XJP_CreateDataQueue;
    /* 向数据队列发送数据函数 */
    PlatHMIFunctionList -> send_to_data_queue = XJP_SendToDataQueue;
    /* 从数据队列获取数据函数 */
    PlatHMIFunctionList -> receive_from_data_queue = XJP_ReceiveFromDataQueue;
    /* 中断向量设置 */
    PlatHMIFunctionList -> ge_int_enable = PMFC_GE_INT_Enable;
    /* 禁止中断 */
    PlatHMIFunctionList -> ge_int_disable = PMFC_GE_INT_Disable;
    /* 获取平台引导标识 */
    PlatHMIFunctionList -> get_plat_lead_flag = CCMC_GetPlatLeadFlag;
    /* 读取动作报告数据 */
    PlatHMIFunctionList -> user_read_report_data = CRMC_UserReadReportData;
}

/* 通信报文处理 */
VOID PMFC_MessM_ParseData( VOID )
{
    LCC_MessM_ParseData( pCommChl );
}

/* 连接HMI程序 */
INT PMFC_LinkHMIApplication(  UNSIGNED StartAddr, UNSIGNED Length, VOID(* WatchDogReset )( VOID ) )
{
#ifndef WIN32
    HAPP hHMIHandle;
    char FileName[] = "ProConfig.ini";
    char SectName[] = "MMI";
    char KeyName[] = "DllName";
    char EntryName[] = "GetHmiEntry";
    INT status = XJP_LOAD_ERROR;

    /* 创建链接对象 */
    hHMIHandle = XJP_Create_LoadObj( StartAddr, Length, PMFC_HMI_TEXT_CODE1,
        PMFC_HMI_TEXT_CODE2, PlatHMIFunctionList, sizeof( MTRANSFER ),
        FileName, SectName, KeyName, EntryName );
    if( hHMIHandle != 0 )
    {
        /* 链接应用对象 */
        status = XJP_Link_Application( hHMIHandle, WatchDogReset,
            XJP_CRC_Calculate,& HMIEntryFunctionList );
        if( status == XJP_LOAD_SUCCESS )
        {
            /* 应用对象链接成功，获取HMI程序提供给平台的函数链表 */
            MPFC_Create( XJP_Function_Table_Retrieve( MP_FUN_LIST_NAME ) );
        }
    }
    return( status );
#else
    INT status = XJP_LOAD_ERROR;

    if( HMIEntryFunctionList != XJ_NULL )
    {
        if((* HMIEntryFunctionList->LinkApplication )( PlatHMIFunctionList ) == XJ_SUCCESS )
        {
            MPFC_Create( XJP_Function_Table_Retrieve( MP_FUN_LIST_NAME ) );
            status = XJP_LOAD_SUCCESS;
        }
        else
        {
            /* 版本错误 */
            status = XJP_VERSION_ERROR;
        }
    }
    return( status );
#endif
}

/* 初始化HMI程序 */
INT PMFC_InitializeHMIApplication( VOID * mem_pool, VOID * uncached_mem_pool )
{
    INT status = XJ_UNAVAILABLE;

    if( HMIEntryFunctionList != XJ_NULL )
    {
        /* HMI程序装载成功 */
        if( HMIEntryFunctionList->IniApplication != XJ_NULL )
        {
            status = (* HMIEntryFunctionList->IniApplication )( mem_pool, uncached_mem_pool );
        }
    }
    return( status );
}

/* 引导HMI程序 */
INT PMFC_LeadHMIApplication( VOID )
{
    INT status = XJ_UNAVAILABLE;

    if( HMIEntryFunctionList != XJ_NULL )
    {
        /* HMI程序装载成功 */
        if( HMIEntryFunctionList->LeadApplication != XJ_NULL )
        {
            status = (* HMIEntryFunctionList->LeadApplication )( );
        }
    }
    return( status );
}

/* 获取系统时钟函数 */
UNSIGNED PMFC_Get_System_Clock( VOID )
{
    return( TMD_System_Clock );
}

/* 填充事件函数 */
INT PMFC_FillEvent( const VOID * pName, INT iEventType )
{
    INT status;

    status = CEMC_FillEvent( pName, iEventType );

    return( status );
}

/* 获取用户数据根目录路径名称函数 */
const CHAR * PMFC_GetUserRootName( VOID )
{
    return( CPC_USER_ROOT_DIR_NAME );
}

/* 获取接口程序的CRC */
INT PMFC_GetMmiCrc( UNSIGNED * pBuff )
{
    * pBuff = CCMC_hmiCRC;
    return( XJ_SUCCESS );
}

/* 中断向量设置 */
INT PMFC_GE_INT_Enable( INT vector_id, INT trigger_type, INT priority )
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
INT PMFC_GE_INT_Disable( INT vector_id )
{
    INT status;

#ifndef WIN32
    status = ESAL_GE_INT_Disable( vector_id );
#else
    status = XJ_UNAVAILABLE;
#endif
    return( status );
}
