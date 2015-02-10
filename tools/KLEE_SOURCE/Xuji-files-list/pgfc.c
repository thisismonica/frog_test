
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
/* ע��UART�����жϺ��� */
extern INT MainRegUartIntFun( INT channel, VOID (* UartComIntFun )( int para ));
/* ��ȡUART�����ж�����ID */
extern UNSIGNED MainGetUartVectorID( INT channel );

/* Define internal function calls. */
UNSIGNED PGFC_Get_System_Clock( VOID );
/* ��ȡ��ǰʱ�������1970������� */
UNSIGNED PGFC_Get_Time_Second_1970( VOID );
#if WIN32
/* ��ȡ����ʱ��ֵ */
UNSIGNED Get_system_clock_nanosecond( VOID );
#else
/* ��ȡ����ʱ��ֵ */
long long Get_system_clock_nanosecond( VOID );
#endif
/* �����ں�ʱ��ת��Ϊ�������α�׼ʱ�� */
INT PGFC_TIME_TO_GMT( TIME * time );
/* �ж��������� */
INT PGFC_GE_INT_Enable( INT vector_id, INT trigger_type, INT priority );
/* ��ֹ�ж� */
INT PGFC_GE_INT_Disable( INT vector_id );
/* ����61850��ʼ����ɱ�־ */
INT PGFC_Gws_Set_Init_Status( INT iniFlag );
/* ����ϵͳʱ�� */
INT PGFC_Set_Time_Dev_Zone( TIME * pTime , INT iTimeZone );
/* ����61850ʱ�� */
INT PGFC_Set_Time_61850( UNSIGNED UTCSecond, UNSIGNED UTCNanoSecond, TIME * pTime );
/* ���ö�ʱʱ���� */
INT PGFC_SET_Time_Interval( UNSIGNED TimeInterval );
/* ���ö�ʱʱ��Ʒ������ */
INT PGFC_SET_Time_Quality( UNSIGNED Quality, INT CondType );

/* ��ʼ��ƽ̨�ṩ��GWSʹ�õķ������� */
VOID PGFC_PlatGwsFunList_Initialize( VOID )
{
    /* Initialize the function list data of platform support.  */
    PlatGwsFunctionList = & PlatGwsFunction;
    PlatGwsFunctionList -> size = sizeof( GTRANSFER );
    /* ע�ắ������ */
    PlatGwsFunctionList -> register_function_list = XJP_Function_Table_Register;
    /* ��ȡ�������� */
    PlatGwsFunctionList -> get_function_list = XJP_Function_Table_Retrieve;
    /* ����ϵͳ������ */
    PlatGwsFunctionList -> get_os_fun_list = XJP_OS_Get_FuncList;
    /* ��ȡװ�õ�ǰʱ�亯�� */
    PlatGwsFunctionList -> retrieve_time = XJP_TM_Get_Time_Dev;
    /* ��ȡϵͳʱ�Ӻ��� */
    PlatGwsFunctionList -> retrieve_system_clock = PGFC_Get_System_Clock;
    /* ��ȡ��ǰʱ�������1970������� */
    PlatGwsFunctionList -> get_time_second_1970 = PGFC_Get_Time_Second_1970;
    /* ��ȡ����ʱ��ֵ */
    PlatGwsFunctionList -> get_time_nanosecond = Get_system_clock_nanosecond;
    /* �����ں�ʱ��ת��Ϊ�������α�׼ʱ�� */
    PlatGwsFunctionList -> time_to_gmt = PGFC_TIME_TO_GMT;
    /* �ж��������� */
    PlatGwsFunctionList -> ge_int_enable = PGFC_GE_INT_Enable;
    /* ��ֹ�ж� */
    PlatGwsFunctionList -> ge_int_disable = PGFC_GE_INT_Disable;
    /* �������Ź�������� */
    PlatGwsFunctionList -> create_watchdog_handle = XJP_Watchdog_Create;
    /* ��λ���Ź����Ӷ��� */
    PlatGwsFunctionList -> reset_watchdog_object = XJP_Watchdog_Reset;
    /* ����ʱ�亯�� */
    PlatGwsFunctionList -> set_time = PGFC_Set_Time_Dev_Zone;
    /* ����61850��ʼ����ɱ�־ */
    PlatGwsFunctionList -> gws_set_init_status = PGFC_Gws_Set_Init_Status;
    /* �������ݱ��� */
    PlatGwsFunctionList -> create_lock_data = XJP_NU_Protect_Create;
    /* ���ݱ��� */
    PlatGwsFunctionList -> lock_data = XJP_NU_Protect;
    /* �ͷ����ݱ��� */
    PlatGwsFunctionList -> unlock_data = XJP_NU_UnProtect;
    /* ��ȡ������ */
    PlatGwsFunctionList -> obtain_mutex = XJP_Mutex_Obtain;
    /* �ͷŻ����� */
    PlatGwsFunctionList -> release_mutex = XJP_Mutex_Release;
    /* ��ȡװ��ʱ�� */
    PlatGwsFunctionList -> get_time_zone = XJP_TM_Get_Time_Zone;
    /* ��λ */
    PlatGwsFunctionList -> reset_device = DEVI_ResetCPU;
    /* ��ȡӲ����ʶ�� */
    PlatGwsFunctionList -> get_identification_code = DEVI_GetIdentificationCode;
    /* ��ȡװ�ù���ѡ�� */
    PlatGwsFunctionList -> get_device_fun_select = PMI_get_device_fun_select;
    /* ����61850ʱ�� */
    PlatGwsFunctionList -> set_time_61850 = PGFC_Set_Time_61850;
    /* ���ö�ʱʱ���� */
    PlatGwsFunctionList -> set_time_interval = PGFC_SET_Time_Interval;
    /* ���ö�ʱʱ��Ʒ������ */
    PlatGwsFunctionList -> set_time_quality = PGFC_SET_Time_Quality;
    /* ��־��¼ */
    PlatGwsFunctionList -> write_log = CLOG_Log;
    /* �������ݶ��й����� */
    PlatGwsFunctionList -> create_data_queue = XJP_CreateDataQueue;
    /* �����ݶ��з������ݺ��� */
    PlatGwsFunctionList -> send_to_data_queue = XJP_SendToDataQueue;
    /* �����ݶ��л�ȡ���ݺ��� */
    PlatGwsFunctionList -> receive_from_data_queue = XJP_ReceiveFromDataQueue;
    /* ע��UART�����жϺ��� */
    PlatGwsFunctionList -> reg_uart_int_fun = MainRegUartIntFun;
    /* UART���ڳ�ʼ�� */
    PlatGwsFunctionList -> uart_com_init = DEVI_UartComInit;
    /* ����UART485ʹ�ܱ�ʶ */
    PlatGwsFunctionList -> uart_rs485_set_enflag = DEVI_UART_RS485_SetEnFlag;
    /* ��UART����д���� */
    PlatGwsFunctionList -> uart_write_comm = DEVI_UART_WriteComm;
    /* ��UART���ڶ�ȡ���� */
    PlatGwsFunctionList -> uart_read_comm = DEVI_UART_ReadComm;
    /* ��ȡUART�ж�״̬ */
    /* �жϲ�ѯ����,������ָ�����Ƿ��ڽӿ��ж�״̬NU_TRUE:�����ж�״̬,NU_FALSE:���ǽ����ж�״̬ */
    PlatGwsFunctionList -> uart_get_port_int_flag = DEVI_UART_GetPortIntFlag;
    /* ��ȡUART�����ж�����ID */
    PlatGwsFunctionList -> get_uart_vector_id = MainGetUartVectorID;
    /* ��ȡ���ݶ�����Ϣ */
    PlatGwsFunctionList -> get_data_queue_information = XJP_DataQueueInformation;
    /* ɾ��ָ�����ݶ��� */
    PlatGwsFunctionList -> delete_data_queue = XJP_DeleteDataQueue;
    /* ��ȡƽ̨������ʶ */
    PlatGwsFunctionList -> get_plat_lead_flag = CCMC_GetPlatLeadFlag;
}

/* ����Gws61850���� */
INT PGFC_LinkGwsApplication(  UNSIGNED StartAddr, UNSIGNED Length, VOID(* WatchDogReset )( VOID ) )
{
    HAPP hGwsHandle;
    char FileName[] = "ProConfig.ini";
    char SectName[] = "PROTECT";
    char KeyName[] = "DllName";
    char EntryName[] = "GetProtectEntry";
    INT status = XJP_LOAD_ERROR;

    /* �������Ӷ��� */
    hGwsHandle = XJP_Create_LoadObj( StartAddr, Length, PGFC_HMI_TEXT_CODE1,
        PGFC_HMI_TEXT_CODE2, PlatGwsFunctionList, sizeof( GTRANSFER ),
        FileName, SectName, KeyName, EntryName );
    if( hGwsHandle != 0 )
    {
        /* ����Ӧ�ö��� */
        status = XJP_Link_Application( hGwsHandle, WatchDogReset,
            XJP_CRC_Calculate,& GwsEntryFunctionList );
    }
    return( status );
}

/* ��ʼ��Gws61850���� */
INT PGFC_InitializeGwsApplication( VOID * mem_pool, VOID * uncached_mem_pool )
{
    INT status = NU_UNAVAILABLE;

    if( GwsEntryFunctionList != XJ_NULL )
    {
        /* HMI����װ�سɹ� */
        if( GwsEntryFunctionList->IniApplication != XJ_NULL )
        {
            status = (* GwsEntryFunctionList->IniApplication )( mem_pool, uncached_mem_pool );
        }
    }
    return( status );
}

/* ����Gws61850���� */
INT PGFC_LeadGwsApplication( VOID )
{
    INT status = XJ_UNAVAILABLE;

    if( GwsEntryFunctionList != XJ_NULL )
    {
        /* HMI����װ�سɹ� */
        if( GwsEntryFunctionList->LeadApplication != XJ_NULL )
        {
            status = (* GwsEntryFunctionList->LeadApplication )( );
        }
    }
    return( status );
}

/* ��ȡϵͳʱ�Ӻ��� */
UNSIGNED PGFC_Get_System_Clock( VOID )
{
    return( TMD_System_Clock );
}

/* ��ȡ��ǰʱ�������1970������� */
UNSIGNED PGFC_Get_Time_Second_1970( VOID )
{
    TIME  SysTime;
    UNSIGNED uUTCSecond;
    UNSIGNED uUTCMillionsecond;
    /*======================*/
    /* ��ȡ��ǰϵͳʱ�� */
    XJP_TM_Get_Time_Dev(& SysTime );
    /* ��ϵͳʱ��ת��������ʱ�� */
    XJP_TM_Trans_Dev_To_LT( & uUTCSecond, & uUTCMillionsecond, & SysTime );
    /* ���ص�ǰʱ�������1970������ */
    return ( uUTCSecond );
}

/* �����ں�ʱ��ת��Ϊ�������α�׼ʱ�� */
INT PGFC_TIME_TO_GMT( TIME * time )
{
    UNSIGNED uUTCSecond;
    UNSIGNED uUTCMillionsecond;
    UNSIGNED uUTCNanoSecond;
    /*======================*/
    /* ��ȡװ�õ�ǰ61850ʱ�� */
    XJP_TM_Get_Time_61850( XJ_NULL, & uUTCSecond, & uUTCNanoSecond, XJ_NULL );
    uUTCMillionsecond = ( uUTCNanoSecond / 1000000 ); /* ����ת���ɺ��� */
    /* ת��Ϊ�������α�׼ʱ�� */
    XJP_TM_Trans_LT_To_Dev( & uUTCSecond, & uUTCMillionsecond, time );
    time->Nanosecond = ( uUTCNanoSecond % 1000000 ); /* ��ȡ���� */
    /* return */
    return( XJ_SUCCESS );
}

/* �ж��������� */
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

/* ��ֹ�ж� */
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

/* ����61850��ʼ����ɱ�־ */
INT PGFC_Gws_Set_Init_Status( INT iniFlag )
{
    if( iniFlag )
    {
        /* ��ʼ����� */
        CERC_SetUserEventBits( CPC_EVENT1_GWS_INIT_STATUS, 1 );
    }
    else
    {
        /* ��ʼ��δ��� */
        CERC_SetUserEventBits( CPC_EVENT1_GWS_INIT_STATUS, 0 );
    }
    return( XJ_SUCCESS );
}

/* ���ض�ʱ�亯�����ò���Ҫ�б�װ�����ݱ��ʱ��Դ�Ƿ�Ϊվ�ز� */
/* ����ϵͳʱ�� */
INT PGFC_Set_Time_Dev_Zone( TIME * pTime , INT iTimeZone )
{
    if( pDeviceTab )
    {
        /* �ж϶�ʱ��Դ */
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

/* ����61850ʱ�� */
INT PGFC_Set_Time_61850( UNSIGNED UTCSecond, UNSIGNED UTCNanoSecond, TIME * pTime )
{
    if( pDeviceTab )
    {
        /* �ж϶�ʱ��Դ */
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

/* ���ö�ʱʱ���� */
INT PGFC_SET_Time_Interval( UNSIGNED TimeInterval )
{
    if( pDeviceTab )
    {
        /* �ж϶�ʱ��Դ */
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

/* ���ö�ʱʱ��Ʒ������ */
INT PGFC_SET_Time_Quality( UNSIGNED Quality, INT CondType )
{
    if( pDeviceTab )
    {
        /* �ж϶�ʱ��Դ */
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
