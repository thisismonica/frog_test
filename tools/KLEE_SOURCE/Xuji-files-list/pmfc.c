
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
/* ����¼����� */
INT PMFC_FillEvent( const VOID * pName, INT iEventType );
/* ��ȡ�û����ݸ�Ŀ¼·�����ƺ��� */
const CHAR * PMFC_GetUserRootName( VOID );
/* ��ȡ�ӿڳ����CRC */
INT PMFC_GetMmiCrc( UNSIGNED * pBuff );
/* ע��UART�����жϺ��� */
extern INT MainRegUartIntFun( INT channel, VOID (* UartComIntFun )( int para ));
/* �ж��������� */
INT PMFC_GE_INT_Enable( INT vector_id, INT trigger_type, INT priority );
/* ��ֹ�ж� */
INT PMFC_GE_INT_Disable( INT vector_id );

/* ��ʼ��ƽ̨�ṩ��HMIʹ�õķ������� */
VOID PMFC_PlatHMIFunList_Initialize( VOID )
{
    /* ��ʼ������ʱ�亯�� */
    PMFC_Save_Time = XJ_NULL;
    /* Initialize the function list data of platform support.  */
    PlatHMIFunctionList = & PlatHMIFunction;
    PlatHMIFunctionList -> size = sizeof( MTRANSFER );
    /* ���ݱ������ */
    PlatHMIFunctionList -> get_table_memory = XJP_DS_Get_Table_Memory;
    /* ��ȡ���ݱ��С */
    PlatHMIFunctionList -> get_table_size = XJP_DS_Get_Table_Size;
    /* ע�ắ������ */
    PlatHMIFunctionList -> register_function_list = XJP_Function_Table_Register;
    /* ��ȡ�������� */
    PlatHMIFunctionList -> get_function_list = XJP_Function_Table_Retrieve;
    /* ����ϵͳ������ */
    PlatHMIFunctionList -> get_os_fun_list = XJP_OS_Get_FuncList;
    /* ��ȡװ�õ�ǰʱ�亯�� */
    PlatHMIFunctionList -> retrieve_time = XJP_TM_Get_Time_Dev;
    /* ��ȡϵͳʱ�Ӻ��� */
    PlatHMIFunctionList -> retrieve_system_clock = PMFC_Get_System_Clock;
    /* ����ѭ�������� */
    PlatHMIFunctionList -> calculate_crc = XJP_CRC_Calculate;
#if (! LC_LINK_HMI_PHY_COM )
    /* ��ƽ̨��ȡͨѶ���� */
    PlatHMIFunctionList -> read_command = LCC_Read_Command;
    /* ��ƽ̨дͨѶ���� */
    PlatHMIFunctionList -> write_command = LCC_Write_Command;
    /* ͨ�ű��Ĵ��� */
    PlatHMIFunctionList -> lcc_parase_data = PMFC_MessM_ParseData;
#endif
    /* ����ʱ�亯�� */
    PlatHMIFunctionList -> set_time = XJP_TM_Set_Time_Dev_Zone;
    /* ��������¼���� */
    PlatHMIFunctionList -> fill_event = PMFC_FillEvent;
    /* ������� */
    PlatHMIFunctionList -> read_input = CINC_ReadInputValue;
    /* �������Ź�������� */
    PlatHMIFunctionList -> create_watchdog_handle = XJP_Watchdog_Create;
    /* ��λ���Ź����Ӷ��� */
    PlatHMIFunctionList -> reset_watchdog_object = XJP_Watchdog_Reset;
    /* ��ȡ�û����ݸ�Ŀ¼·������ */
    PlatHMIFunctionList -> get_user_dir_name = PMFC_GetUserRootName;
    /* ����װ�÷�ģ���ʼ���ɹ����� */
    PlatHMIFunctionList -> set_modinit_success = CCMC_SetModInitSuccess;
    /* ��ȡװ�÷�ģ���ʼ����ɸ��� */
    PlatHMIFunctionList -> get_modinit_count = CCMC_GetModInitCount;
    /* ��ȡ�ӿڳ����CRC */
    PlatHMIFunctionList -> get_mmi_crc = PMFC_GetMmiCrc;
    /* ʱ��ת������ */
    PlatHMIFunctionList -> transform_time_from_systime = XJP_TM_Dev_To_MS_UTC;
    /* �ӿ���ֱ�ӵ������ݱ��޸ĺ��� */
    PlatHMIFunctionList -> hmi_modif_table_data = XJP_DS_Com_Hmi_Modify;
    /* ���������ݱ����� */
    PlatHMIFunctionList -> write_fck_table_data = APFC_WriteFckTabData;
    /* ʱ��Ϸ��Լ�⺯�� */
    PlatHMIFunctionList -> time_check = XJP_TM_Time_Check;
    /* ����Ҫת����ʱ��ģʽ,1ΪPROFIBUSʱ��,2ΪUTCʱ��,ȱʡΪUTCʱ��  */
    PlatHMIFunctionList -> tm_set_time_mode = XJP_TM_Set_Time_Mode;
    /* װ��ʱ��ת���ɾ͵�ʱ�� */
    PlatHMIFunctionList -> tm_trans_dev_to_lt = XJP_TM_Trans_Dev_To_LT;
    /* �͵�ʱ��ת����װ��ʱ�� */
    PlatHMIFunctionList -> tm_trans_lt_to_dev = XJP_TM_Trans_LT_To_Dev;
    /* ��ȡװ��ʱ�� */
    PlatHMIFunctionList -> get_time_zone = XJP_TM_Get_Time_Zone;
    /* ��ȡ61850ʱ�� */
    PlatHMIFunctionList -> read_61850_time = XJP_TM_Get_Time_61850;
    /* ��ȡ���̼�ֵ */
    PlatHMIFunctionList -> get_key_value = DEVI_GetKeyValue;
    /* ��ȡӲ����ʶ�� */
    PlatHMIFunctionList -> get_identification_code = DEVI_GetIdentificationCode;
    /* ����UTCת����TIME���͵�װ��ʱ�� */
    PlatHMIFunctionList -> transform_ns_utc_to_dev = XJP_TM_NS_UTC_To_Dev;
    /* TIME���͵�װ��ʱ��ת��������UTC */
    PlatHMIFunctionList -> transform_dev_to_ns_utc = XJP_TM_Dev_To_NS_UTC;
    /* װ��ʱ��ת��ΪMS UTC*/
    PlatHMIFunctionList -> transform_dev_to_ms_utc = XJP_TM_Dev_To_MS_UTC;
    /* MS UTC ת��Ϊװ��ʱ�� */
    PlatHMIFunctionList -> transform_ms_utc_to_dev = XJP_TM_MS_UTC_To_Dev;
    /* ��־��¼ */
    PlatHMIFunctionList -> write_log = CLOG_Log;
    /* ע��UART�����жϺ��� */
    PlatHMIFunctionList -> reg_uart_int_fun = MainRegUartIntFun;
    /* UART���ڳ�ʼ�� */
    PlatHMIFunctionList -> uart_com_init = DEVI_UartComInit;
    /* ����UART485ʹ�ܱ�ʶ */
    PlatHMIFunctionList -> uart_rs485_set_enflag = DEVI_UART_RS485_SetEnFlag;
    /* ��UART����д���� */
    PlatHMIFunctionList -> uart_write_comm = DEVI_UART_WriteComm;
    /* ��UART���ڶ�ȡ���� */
    PlatHMIFunctionList -> uart_read_comm = DEVI_UART_ReadComm;
    /* �������ݶ��й����� */
    PlatHMIFunctionList -> create_data_queue = XJP_CreateDataQueue;
    /* �����ݶ��з������ݺ��� */
    PlatHMIFunctionList -> send_to_data_queue = XJP_SendToDataQueue;
    /* �����ݶ��л�ȡ���ݺ��� */
    PlatHMIFunctionList -> receive_from_data_queue = XJP_ReceiveFromDataQueue;
    /* �ж��������� */
    PlatHMIFunctionList -> ge_int_enable = PMFC_GE_INT_Enable;
    /* ��ֹ�ж� */
    PlatHMIFunctionList -> ge_int_disable = PMFC_GE_INT_Disable;
    /* ��ȡƽ̨������ʶ */
    PlatHMIFunctionList -> get_plat_lead_flag = CCMC_GetPlatLeadFlag;
    /* ��ȡ������������ */
    PlatHMIFunctionList -> user_read_report_data = CRMC_UserReadReportData;
}

/* ͨ�ű��Ĵ��� */
VOID PMFC_MessM_ParseData( VOID )
{
    LCC_MessM_ParseData( pCommChl );
}

/* ����HMI���� */
INT PMFC_LinkHMIApplication(  UNSIGNED StartAddr, UNSIGNED Length, VOID(* WatchDogReset )( VOID ) )
{
#ifndef WIN32
    HAPP hHMIHandle;
    char FileName[] = "ProConfig.ini";
    char SectName[] = "MMI";
    char KeyName[] = "DllName";
    char EntryName[] = "GetHmiEntry";
    INT status = XJP_LOAD_ERROR;

    /* �������Ӷ��� */
    hHMIHandle = XJP_Create_LoadObj( StartAddr, Length, PMFC_HMI_TEXT_CODE1,
        PMFC_HMI_TEXT_CODE2, PlatHMIFunctionList, sizeof( MTRANSFER ),
        FileName, SectName, KeyName, EntryName );
    if( hHMIHandle != 0 )
    {
        /* ����Ӧ�ö��� */
        status = XJP_Link_Application( hHMIHandle, WatchDogReset,
            XJP_CRC_Calculate,& HMIEntryFunctionList );
        if( status == XJP_LOAD_SUCCESS )
        {
            /* Ӧ�ö������ӳɹ�����ȡHMI�����ṩ��ƽ̨�ĺ������� */
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
            /* �汾���� */
            status = XJP_VERSION_ERROR;
        }
    }
    return( status );
#endif
}

/* ��ʼ��HMI���� */
INT PMFC_InitializeHMIApplication( VOID * mem_pool, VOID * uncached_mem_pool )
{
    INT status = XJ_UNAVAILABLE;

    if( HMIEntryFunctionList != XJ_NULL )
    {
        /* HMI����װ�سɹ� */
        if( HMIEntryFunctionList->IniApplication != XJ_NULL )
        {
            status = (* HMIEntryFunctionList->IniApplication )( mem_pool, uncached_mem_pool );
        }
    }
    return( status );
}

/* ����HMI���� */
INT PMFC_LeadHMIApplication( VOID )
{
    INT status = XJ_UNAVAILABLE;

    if( HMIEntryFunctionList != XJ_NULL )
    {
        /* HMI����װ�سɹ� */
        if( HMIEntryFunctionList->LeadApplication != XJ_NULL )
        {
            status = (* HMIEntryFunctionList->LeadApplication )( );
        }
    }
    return( status );
}

/* ��ȡϵͳʱ�Ӻ��� */
UNSIGNED PMFC_Get_System_Clock( VOID )
{
    return( TMD_System_Clock );
}

/* ����¼����� */
INT PMFC_FillEvent( const VOID * pName, INT iEventType )
{
    INT status;

    status = CEMC_FillEvent( pName, iEventType );

    return( status );
}

/* ��ȡ�û����ݸ�Ŀ¼·�����ƺ��� */
const CHAR * PMFC_GetUserRootName( VOID )
{
    return( CPC_USER_ROOT_DIR_NAME );
}

/* ��ȡ�ӿڳ����CRC */
INT PMFC_GetMmiCrc( UNSIGNED * pBuff )
{
    * pBuff = CCMC_hmiCRC;
    return( XJ_SUCCESS );
}

/* �ж��������� */
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

/* ��ֹ�ж� */
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
