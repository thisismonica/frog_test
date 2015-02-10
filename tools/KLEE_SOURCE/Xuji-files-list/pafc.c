
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
/*      pafc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      PAF - Platform Application Function                              */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the platform            */
/*      functions to application.                                        */
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
#include "plat/es_plat.h"
#include "plat/inc/paf_defs.h"
#include "plat/inc/xj_pafc.h"
#include "plat/inc/apf_extr.h"
#include "plat/inc/cin_extr.h"
#include "plat/inc/cex_extr.h"
#include "plat/inc/cer_extr.h"
#include "plat/inc/cud_extr.h"
#include "plat/inc/csmv_extr.h"
#include "plat/inc/ctd_extr.h"
#include "plat/inc/cout_extr.h"
#include "plat/inc/xj_out_check.h"
#include "plat/net_inc/rcc_defs.h"
#include "plat/net_inc/lc_extr.h"
#include "plat/net_inc/ied_extr.h"
#include "plat/inc/out_check.h"
#include "plat/inc/gpf_extr.h"
#include "plat/inc/ccm_extr.h"
#include "plat/inc/devi_extr.h"
#include "plat/inc/xj_clog.h"
#include "plat/inc/xj_ccms.h"
#include "plat_specific/inc/crm_extr.h"
#include "profibus/inc/profidp.h"

/* 注册函数结构 */
typedef struct FT_FRB_STRUCT
{
    /* 类型 */
    INT Type;
    /* 名称 */
    CHAR * Name;
    /* 函数指针 */
    VOID * FunctionPtr;
}FT_FRB;

#define PAFC_PROTECT_TEXT_CODE1  0xff00ff00  /* the first code for searching protect */
#define PAFC_PROTECT_TEXT_CODE2  0x00ff00ff  /* the second code for searching protect */

VOID (* Fiber_Receive_Interrupt )( VOID );   /* NPU816中光纤中断处理函数 */
VOID (* Fiber_Transmit_Interrupt )( VOID );  /* NPU816中光纤发送数据处理函数 */
/* Define external inner-component global data references.  */
PABFUN  * PlatProBaseFunctionList;
PABFUN  PlatProBaseFunction;
APP_ENTRY * ProEntryFunctionList = NU_NULL;

extern NU_PROTECT * pReport_Protect;
extern NU_MEMORY_POOL * PLAT_mem_pool;
extern NU_MEMORY_POOL * PLAT_uncached_mem_pool;
/* Define internal function calls. */
INT CEMC_FillEvent( const VOID * pName, INT iEventType );
INT CEMC_FillEventFull( const VOID * pName, TIME * pTime, INT iEventType );
INT CWMC_WaveStart( INT iType, INT iSpace, INT iStartPoint, INT iTotalPoint,
    INT iElecNumber, WAVE_CHANNEL * WaveChannel, TIME * pTime, STATE * pState, INT iState );
INT PAFC_ReadSystemFrequency( INT Type, float * frequency );
INT PAFC_ReadSampleFrequency( float * frequency );
INT PAFC_SetUserSampleFrequency( float fFrequency );
VOID PAFC_RegisterFiberReceiveInterrupt( VOID (* fiber_receive_interrupt )( VOID ) );
VOID PAFC_RegisterFiberTransmitInterrupt( VOID (* fiber_transmit_interrupt )( VOID ));
VOID PAFC_Set_Syn_Adjust_Pointer( VOID * pSynAdjust );
INT PAFC_SetFaultStatus( INT iFaultStatus );
INT PAFC_MemPoolInformation( VOID ** mem_pool, VOID ** uncached_mem_pool );
VOID PAFC_Meter_Pulse_Start( UNSIGNED pulse_index, UNSIGNED delay_time_us );
/*=======================================================================*/
/* 获取光纤发送数据偏移位置 */
UNSIGNED FIBER_GetFiberSendOffset( VOID );
/* 注册设置光纤通讯发送序号函数 */
extern VOID FIBER_RegSetSendSerial( VOID (* Fiber_SetSendSerial )( UNSIGNED uSendSerial ) );
/* 设置发送周期调整步长 */
extern INT FIBER_SetSendAdjustStep( UNSIGNED uAdjustStepNs );
/* 设置PPS宽度 */
extern INT FIBER_SetWidth_PPS( short int Value );
/*=======================================================================*/
/* 应用启动立即激活主任务执行函数 */
extern INT MAIN_UserActiveMainTask( VOID );
/*=======================================================================*/
/* 读取温度传感器数据 */
extern INT Main_GetTemperature ( INT * temperature_ptr );
/*=======================================================================*/
/* 填充故障报告 */
INT PAFC_FillReport(  UNSIGNED FaultNumber, INT Type,
    UNSIGNED Size, const VOID * Src );
/* 填充故障报告 */
INT PAFC_FillReportTime(  UNSIGNED FaultNumber, INT Type,
    UNSIGNED Size, const VOID * Src, TIME * pTime );
/* 读取报告故障序号 */
INT PAFC_ReadFaultNumber(  );
/* 自动调整报告故障序号 */
INT PAFC_AdjustFaultNumber(  );
/* 设置报告故障序号 */
INT PAFC_SetFaultNumber(  INT iFaultNumber );
/*=======================================================================*/
extern UNSIGNED DEV_Float_To_IEEE( float fvalue );
extern float IEEE_To_Dev_Float( UNSIGNED udata );

FT_FRB PAFC_FunctionPointer[] = 
{
    /* 数据表管理函数 */
    {   0, "define_table",                         XJP_DS_Define_Table, },
    {   0, "define_field",                         XJP_DS_Define_Field, },
    {   0, "create_all_field",                     XJP_DS_Create_All_Field, },
    {   0, "define_protect_handle",                XJP_DS_Define_Protect_Handle, },
    {   0, "get_freeze_status",                    XJP_DS_Freeze_Status, },
    {   0, "define_SG_range",                      XJP_DS_Define_SG_Range, },
    {   0, "get_table_memory",                     XJP_DS_Get_Table_Memory, },
    {   0, "get_index_tabmemory",                  XJP_DS_Get_Table_Memory_Index, },
    {   0, "register_pre_treatment_fun",           XJP_DS_Register_Pre_Treatment, },
    {   0, "register_fore_treatment_fun",          XJP_DS_Register_Fore_Treatment, },
    {   0, "register_after_treatment_fun",         XJP_DS_Register_After_Treatment, },
    {   0, "register_end_treatment_fun",           XJP_DS_Register_End_Treatment, },
    {   0, "get_table_status",                     XJP_DS_Get_Table_Status, },
    {   0, "register_fore_lead_treatment_fun",     XJP_DS_Register_Lead_Fore_Treatment, },
    {   0, "register_after_lead_treatment_fun",    XJP_DS_Register_Lead_After_Treatment, },
    /* 开出管理函数 */
    {   0, "create_output_handle",                 XJP_IO_Create_Out, },
    {   0, "set_start_relay",                      XJP_IO_Set_Start_Relay, },
    {   0, "set_output_bits",                      XJP_IO_Set_Output_Relay, },
    {   0, "getdev_start_relay",                   XJP_IO_Get_Start_Relay, },
    {   0, "getdev_output_bits",                   XJP_IO_Get_Output_Relay, },
    {   0, "set_holdrelay_maskbits",               XJP_IO_Set_Hold_Masks, },
    {   0, "set_output_quality",                   COUTC_SetOutputQualityBits, },
    {   0, "rotor_switch_output",                  DEVI_SWDriver, },
    {   0, "set_out_logmaskbits",                  OUTC_CheckSetOutLogMaskBits, },
    {   0, "read_soft_output",                     COUTC_ReadSoftOutput, },
    /* 任务管理函数 */
    {   0, "create_app_routine",                   XJP_Create_AppRoutine, },
    {   0, "delete_app_routine",                   XJP_Delete_AppRoutine, },
    {   0, "change_task_time_slice",               XJP_Change_Time_Slice, },
    {   0, "change_sleep_ticks",                   XJP_Change_Sleep_Ticks, },
    {   0, "main_unsleep",                         CTDC_Main_UnSleep, },
    {   0, "user_active_main_task",                MAIN_UserActiveMainTask, },
    /* 时间管理函数 */
    {   0, "read_time",                            XJP_TM_Get_Time_Dev, },
    {   0, "read_61850_time",                      XJP_TM_Get_Time_61850, },
    {   0, "read_system_clock",                    XJP_TM_Get_Clock_Millisecond, },
    {   0, "read_timer_clock",                     XJP_TM_Get_Clock_Precision, },
    {   0, "get_pulse_number",                     XJP_TM_Get_Pulse_Counter, },
    {   0, "get_sync_time_type",                   XJP_TM_Get_Time_Type, },
    {   0, "time_test",                            XJP_Dbg_Time_Testing, },
    {   0, "main_run_time",                        XJP_Get_Main_Run_Time, },
    {   0, "transform_time_from_systime",          XJP_TM_Dev_To_MS_UTC, },
    {   0, "transform_time_to_systime",            XJP_TM_MS_UTC_To_Dev, },
    {   0, "transform_ns_utc_to_dev",              XJP_TM_NS_UTC_To_Dev, },
    {   0, "transform_dev_to_ns_utc",              XJP_TM_Dev_To_NS_UTC, },
    {   0, "place_on_overtime_check",              XJP_OverTime_Place_On, },
    {   0, "remove_from_overtime_check",           XJP_OverTime_Remove, },
    {   0, "set_PPS_width",                        FIBER_SetWidth_PPS, },
    {   0, "pps_set_width_readback",               DEVI_PPSSetWidthReadBack, },
    {   0, "read_gtime_cnr1",                      DEVI_ReadGTimeCnr1, },
    /* 保留的函数 */
    {   0, "word_to_string",                       XJP_CSC_Word_To_String, },
    {   0, "string_to_word",                       XJP_CSC_String_To_Word, },
    {   0, "word_to_byte",                         XJP_CSC_Word_To_Byte, },
    {   0, "byte_to_word",                         XJP_CSC_Byte_To_Word, },
    {   0, "calculate_crc",                        XJP_CRC_Calculate, },
    {   0, "register_function_list",               XJP_Function_Table_Register, },
    {   0, "get_function_list",                    XJP_Function_Table_Retrieve, },
    {   0, "read_user_data",                       CUDC_GetUserData, },
    {   0, "save_user_data",                       CUDC_SaveUserData, },
    {   0, "read_exchange_data",                   CEXC_ReadUserExchangeData, },
    {   0, "send_exchange_data",                   CEXC_SendUserExchangeData, },
    {   0, "get_crc_table",                        XJP_CRC_Get_Table, },
    {   0, "read_background_data",                 XJP_Background_Read, },
    {   0, "device_float_to_ieee",                 DEV_Float_To_IEEE, },
    {   0, "ieee_to_device_float",                 IEEE_To_Dev_Float, },
    {   0, "get_function_pointer",                 PAFC_GetFunctionPointer, },
    /* 开入管理 */
    {   0, "read_input",                           CINC_ReadInputValue, },
    {   0, "read_input_quality",                   CINC_ReadInputQuality, },
    {   0, "retrieve_Fck_Input_information",       CINC_SwitchStatusInformation, },
    {   0, "retrieve_Fck_Input_pointer",           CINC_SwitchStatusPointer, },
    {   0, "set_fckin_maskbits",                   CINC_SetSwitchStatusMasksBits, },
    {   0, "setup_check_input_time",               CINC_SetupFckInput, },
    {   0, "set_soft_input_status",                CINC_SetSoftInputStatus, },
    {   0, "set_soft_input_quality",               CINC_SetSoftInputQuality, },
    {   0, "set_softin_full_masks",                CINC_SetSoftInFullMasks, },
    {   0, "get_softin_full_masks",                CINC_GetSoftInFullMasks, },
    /* 模拟量管理函数 */
    {   0, "read_hard_frequency",                  PAFC_ReadSystemFrequency, },
    {   0, "read_sample_frequency",                PAFC_ReadSampleFrequency, },
    {   0, "adjust_frequency",                     PAFC_SetUserSampleFrequency, },
    {   0, "retrieve_sample_pointer",              CSMV_SamplePointer, },
    {   0, "retrieve_sample_information",          CSMV_SampleInformation, },
    {   0, "set_syn_adjust_pointer",               PAFC_Set_Syn_Adjust_Pointer, },
    {   0, "get_sample_count",                     SMVC_GetSampleCount, },
    {   0, "data_start_pointer",                   CSMV_GetDataStartPointer, },
    {   0, "get_sample_offset",                    CSMV_GetSampleOffset, },
    {   0, "set_ad_gain",                          DEVI_SetADGain, },
    /* nucles数据保护/定时器/事件位 */
    {   0, "create_lock_data",                     XJP_NU_Protect_Create, },
    {   0, "get_lock_data",                        XJP_NU_Protect_Find, },
    {   0, "lock_data",                            XJP_NU_Protect, },
    {   0, "unlock_data",                          XJP_NU_UnProtect, },
    {   0, "create_timer",                         XJP_NU_Timer_Create, },
    {   0, "control_timer",                        XJP_NU_Timer_Control, },
    {   0, "create_event_group",                   XJP_NU_Event_Group_Create, },
    {   0, "set_event",                            XJP_NU_Event_Group_Set, },
    {   0, "retrieve_event",                       XJP_NU_Event_Group_Retrieve, },
    /* 历史记录管理函数 */
    {   0, "fill_event",                           CEMC_FillEvent, },
    {   0, "fill_event_full",                      CEMC_FillEventFull, },
    {   0, "fill_report",                          PAFC_FillReport, },
    {   0, "fill_report_time",                     PAFC_FillReportTime, },
    {   0, "start_wave",                           CWMC_WaveStart, },
    {   0, "start_wave_mul",                       CWMC_WaveStartMul, },
    {   0, "read_wave_index_number",               CWMC_ReadWaveIndexNumber, },
    {   0, "read_elec_number",                     CWMC_ReadElecFaultNumber, },
    {   0, "adjust_elec_number",                   CWMC_AdjustElecNumber, },
    {   0, "set_elec_number",                      CWMC_SetElecNumber, },
    {   0, "read_fault_number",                    PAFC_ReadFaultNumber, },
    {   0, "adjust_fault_number",                  PAFC_AdjustFaultNumber, },
    {   0, "set_fault_number",                     PAFC_SetFaultNumber, },
    {   0, "set_fault_status",                     PAFC_SetFaultStatus, },
    {   0, "get_exchange_fault_number",            CEXC_GetExchangeFaultNumber, },
    {   0, "set_wave_state",                       CWMC_SetWaveState, },
    {   0, "retrieve_logic_information",           CWMC_ProtectLogicInformation, },
    {   0, "fault_return",                         RCC_Fault_Return, },
    {   0, "fault_message",                        RCC_Fault_Message, },
    {   0, "fill_check_info",                      RCC_Fill_CheckInfo, },
    {   0, "fill_check_info_full",                 RCC_Fill_CheckInfo_Full, },
    {   0, "fill_check_goose",                     RCC_Fill_Goose, },
    {   0, "send_check_goose",                     RCC_Send_Goose, },
    {   0, "set_chlq_state_info",                  CSMV_MSG_SetChlQStateInfo, },
    /* 错误管理函数 */
    {   0, "read_error_bits",                      CERC_ReadErrorBits, },
    {   0, "mmi_error_bits",                       CERC_ReadHMIErrorBits, },
    /* 运行指示灯管理函数 */
    {   0, "create_led_handle",                    XJP_Led_Create, },
    {   0, "set_led_status",                       XJP_Led_Change_Status, },
    {   0, "set_dev_led_frequency",                XJP_Led_Change_Frequency, },
    {   0, "set_user_event_bits",                  CERC_SetUserEventBits, },
    {   0, "set_plat_alarmled_bits",               PMI_SetPlatAlarmLedBits, },
    {   0, "set_plat_runledabnor_bits",            PMI_SetPlatRunLedAbnorBits, },
    {   0, "led_reset",                            XJP_Reset_App_Routine, },
    {   0, "set_user_led_bits",                    DEVI_WriteUserLed, },
    {   0, "set_dev_led_call_status",              XJP_Led_SetDevLedCallStatus, },
    {   0, "dev_led_call_timer",                   XJP_Led_DevLedCallTimer, },
    /* 中断管理函数 */
    {   0, "register_ad_interrupt",                SMVC_Register_User_AD_Interrupt, },
    {   0, "register_sample_conversion",           SMVC_Register_Sample_Conversion, },
    {   0, "register_fiber_receive_interrupt",     PAFC_RegisterFiberReceiveInterrupt, },
    {   0, "register_fiber_transmit_interrupt",    PAFC_RegisterFiberTransmitInterrupt, },
    /* 通讯管理函数 */
    {   0, "register_command",                     LCC_COMM_RegisterCommand, },
    {   0, "write_fck_soe",                        iedDB_WriteSOE, },
    {   0, "read_fck_command",                     iedDB_ReadCommand, },
    {   0, "user_send_com_data",                   LCC_UserSendComData, },
    /* 内存管理函数 */
    {   0, "create_memory_pool",                   XJP_NU_Pool_Create, },
    {   0, "delete_memory_pool",                   XJP_NU_Pool_Delete, },
    {   0, "allocate_memory",                      XJP_NU_Allocate_Memory, },
    {   0, "deallocate_memory",                    XJP_NU_Deallocate_Memory, },
    {   0, "mem_pool_information",                 PAFC_MemPoolInformation, },
    /* 状态监测管理函数 */
    {   0, "getCPUTemperature",                    Main_GetTemperature, },
    {   0, "getFlashPrartitionBlockStatus",        CERC_GetFlashPrartitionBlockStatus, },
    /* 设置开出量检修位函数 */
    {   0, "set_out_checkquality_bits",            COUTC_SetOutCheckQualityBits, },
    {   0, "set_dev_status",                       CSMV_MSG_SetDevStatus, },
    /* 光纤通讯所需函数 */
    {   0, "get_fiber_send_offset",                FIBER_GetFiberSendOffset, },
    {   0, "fiber_reg_set_send_serial",            FIBER_RegSetSendSerial, },
    {   0, "fiber_set_send_adjust_step",           FIBER_SetSendAdjustStep, },
    /* 正交编码管理函数 */
    {   0, "setOrthogonalCodeDirection",           DEVI_InitializeOrthogonalCode_direction, },
    {   0, "setOrthogonalCodeValue",               DEVI_InitializeOrthogonalCode_value, },
    {   0, "getOrthogonalCodeDirection",           DEVI_GetOrthogonalCodeDirection, },
    {   0, "getOrthogonalCodeValue",               DEVI_GetOrthogonalCodeValue, },
    /* 应用与NPI交换数据函数 */
    {   0, "user_send_data_to_npi",                CSMV_MSG_UserSendDataTONpi, },
    {   0, "get_data_from_npi",                    CSMV_MSG_GetDataFromNpi, },
    /* 日志记录 */
    {   0, "write_log",                            CLOG_Log, },
    /* 用户数据队列管理函数 */
    {   0, "create_data_queue",                    XJP_CreateDataQueue, },
    {   0, "send_to_data_queue",                   XJP_SendToDataQueue, },
    {   0, "receive_from_data_queue",              XJP_ReceiveFromDataQueue, },
    /* 通信报文管理函数 */
    {   0, "com_message_initialize",               CCMS_Initialize, },
    {   0, "write_com_message",                    CCMS_WriteComMs, },
    {   0, "read_com_message",                     CCMS_UserReadComMs, },
    {   0, "del_com_message",                      CCMS_DelComMs, },
    /* 硬件信息管理函数 */
    {   0, "get_identification_code",              DEVI_GetIdentificationCode, },

    #if PLAT_SWITCH_DTS568
    /* 数字电表专用函数 */
    {   0, "meter_pulse_start",                    meter_pulse_start, },
    #else
    {   0, "meter_pulse_start",                    PAFC_Meter_Pulse_Start, },
    #endif
    /* 状态监测管理函数 */
    {   0, "get_motor_status",                     DEVI_GetMotorStatus, },
    {   0, "set_motor_command",                    DEVI_SetMotorCommand, },
    {   0, "set_motor_pulse",                      DEVI_SetMotorPulse, },
    {   0, "set_solenoid_valve_command",           DEVI_SetSolenoidValveCommand, },
    /* 设置时间函数 */
    {   0, "set_time_ex",                          XJP_TM_Set_Time_Dev_Zone, },
    {   0, "set_time",                             XJP_TM_Set_Time_Dev_Local, },

    /* PROFIBUS 专用函数 */
    {   0, "profidp_init",                         profidp_init, },
    {   0, "profidp_tx_input",                     profidp_tx_input, },
    {   0, "profidp_tx_diag",                      profidp_tx_diag, },
    {   0, "profidp_calculate_io_len",             profidp_calculate_io_len, },

    /* 获得各个模块的CRC信息 */
    {   0, "get_module_crc_inf",                   CCMC_GetModuleCrcInf, },
};

VOID PAFC_Meter_Pulse_Start( UNSIGNED pulse_index, UNSIGNED delay_time_us )
{
}

INT PAFC_ReadSystemFrequency( INT Type, float * frequency )
{
    * frequency = 50.0; /* 确省值 */
    return( NU_SUCCESS );
}

INT PAFC_ReadSampleFrequency( float * frequency )
{
    * frequency = 50.0; /* 确省值 */
    return( NU_SUCCESS );
}

INT PAFC_SetUserSampleFrequency( float fFrequency )
{
    return( NU_SUCCESS );
}

VOID PAFC_RegisterFiberReceiveInterrupt( VOID (* fiber_receive_interrupt )( VOID ) )
{
    Fiber_Receive_Interrupt = fiber_receive_interrupt;
}

VOID PAFC_RegisterFiberTransmitInterrupt( VOID (* fiber_transmit_interrupt )( VOID ))
{
    Fiber_Transmit_Interrupt = fiber_transmit_interrupt;
}

VOID PAFC_Set_Syn_Adjust_Pointer( VOID * pSynAdjust )
{
}

/* 设置故障状态 */
INT PAFC_SetFaultStatus( INT iFaultStatus )
{
    CRMC_SetFaultStatus( 1, iFaultStatus );
    CWMC_SetFaultStatus( iFaultStatus );

    return( NU_SUCCESS );
}

/* 读取动态内存池指针 */
INT PAFC_MemPoolInformation( VOID ** mem_pool, VOID ** uncached_mem_pool )
{
    * mem_pool = ( VOID *)PLAT_mem_pool;
    * uncached_mem_pool = ( VOID *)PLAT_uncached_mem_pool;

    return( XJ_SUCCESS );
}

/* 获取函数指针 */
VOID * PAFC_GetFunctionPointer( CHAR * name )
{
    INT k;
    INT Number;
    FT_FRB * p;
    VOID * pFunction;

    pFunction = XJ_NULL;
    p = PAFC_FunctionPointer;
    Number = sizeof( PAFC_FunctionPointer )/ sizeof( FT_FRB );
    for( k = 0; k < Number; k ++ )
    {
        if( strcmp( p->Name, name ) == 0 )
        {
            pFunction = p -> FunctionPtr;
            break;
        }
        p ++;
    }

    return( pFunction );
}

/* 初始化平台提供给保护使用的服务函数表 */
VOID PAFC_PlatProFunList_Initialize( VOID )
{
    /* Initialize the function list data of platform support.  */
    PlatProBaseFunctionList = & PlatProBaseFunction;
    PlatProBaseFunctionList -> size = sizeof( PABFUN );
    PlatProBaseFunctionList -> get_function_pointer = PAFC_GetFunctionPointer;

    Fiber_Receive_Interrupt = NU_NULL;   /* NPU816中光纤中断处理函数 */
    Fiber_Transmit_Interrupt = NU_NULL;  /* NPU816中光纤发送数据处理函数 */
}

/* 连接应用程序 */
INT PAFC_LinkApplication(  UNSIGNED StartAddr, UNSIGNED Length, VOID(* WatchDogReset )( VOID ) )
{
    HAPP hProHandle;
    char FileName[] = "ProConfig.ini";
    char SectName[] = "PROTECT";
    char KeyName[] = "DllName";
    char EntryName[] = "GetProtectEntry";
    INT status = XJP_LOAD_ERROR;

    /* 创建链接对象 */
    hProHandle = XJP_Create_LoadObj( StartAddr, Length, PAFC_PROTECT_TEXT_CODE1,
        PAFC_PROTECT_TEXT_CODE2, PlatProBaseFunctionList, sizeof( PABFUN ),
        FileName, SectName, KeyName, EntryName );
    if( hProHandle != 0 )
    {
        /* 链接应用对象 */
        status = XJP_Link_Application( hProHandle, WatchDogReset,
            XJP_CRC_Calculate,& ProEntryFunctionList );
        if( status == XJP_LOAD_SUCCESS )
        {
            /* 应用对象链接成功，获取应用程序提供给平台的函数链表 */
            APFC_Create( XJP_Function_Table_Retrieve( AP_FUN_LIST_NAME ) );
        }
    }
    return( status );
}

/* 初始化应用程序 */
INT PAFC_InitializeApplication( VOID * mem_pool, VOID * uncached_mem_pool )
{
    INT status = NU_UNAVAILABLE;

    if( ProEntryFunctionList != NU_NULL )
    {
        /* 应用程序装载成功 */
        if( ProEntryFunctionList->IniApplication != NU_NULL )
        {
            status = (* ProEntryFunctionList->IniApplication )( mem_pool, uncached_mem_pool );
        }
    }
    return( status );
}

/* 引导保护程序 */
INT PAFC_LeadApplication( VOID )
{
    INT status = NU_UNAVAILABLE;

    if( ProEntryFunctionList != NU_NULL )
    {
        /* 应用程序装载成功 */
        if( ProEntryFunctionList->LeadApplication != NU_NULL )
        {
            status = (* ProEntryFunctionList->LeadApplication )( );
        }
    }
    return( status );
}

/* 填充故障报告 */
INT PAFC_FillReport(  UNSIGNED FaultNumber, INT Type,
    UNSIGNED Size, const VOID * Src )
{
    INT  iStatus;
    TIME uTempTime;

    XJP_TM_Get_Time_Dev( & uTempTime );

    iStatus = PAFC_FillReportTime( FaultNumber, Type, Size, Src, & uTempTime );
    return( iStatus );
}

/* 填充故障报告 */
INT PAFC_FillReportTime(  UNSIGNED FaultNumber, INT Type,
    UNSIGNED Size, const VOID * Src, TIME * pTime )
{
    INT iStatus;
    iStatus = CRMC_FillReport( 1, FaultNumber, Type, Size, Src, pTime );
    return( iStatus );
}

/* 读取报告故障序号 */
INT PAFC_ReadFaultNumber( VOID )
{
    return( CRMC_ReadFaultNumber( 1 ));
}

/* 自动调整报告故障序号 */
INT PAFC_AdjustFaultNumber( VOID )
{
    return( CRMC_AdjustFaultNumber( 1 ));
}

/* 设置报告故障序号 */
INT PAFC_SetFaultNumber( INT iFaultNumber )
{
    return( CRMC_SetFaultNumber( 1, iFaultNumber ));
}
