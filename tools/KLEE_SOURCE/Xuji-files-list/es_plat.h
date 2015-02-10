/*************************************************************************/
/*                                                                       */
/*        Copyright (c) 1999-2002 XJ ELECTRIC CO.LTD.                    */
/*                                                                       */
/* PROPRIETARY RIGHTS of Accelerated Technology are involved in the      */
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
/*      es_plat.h                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      ES - Embed Soft Platform                                         */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains function prototypes of all functions          */
/*      accessible to other components.                                  */
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
/*      None                                                             */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhideYang.      02-20-2008      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
/* Include operate system configuration settings */
#include "plat/inc/xj_cosc.h"

/* Check to see if the file has been included already.  */

#ifndef ES_PLAT_H
#define ES_PLAT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include platform configuration settings */
#include "plat/plat_cfg.h"

/*=======================================================================*/
/* Define application data types to actual internal data structures */
#ifndef XJ_LOAD_MODULE
#define XJ_LOAD_MODULE
typedef struct APP_ENTRY_STRUCT
{
    UNSIGNED    Key1;
    INT         (* LinkApplication )( VOID * PlatFuncList );
    INT         (* IniApplication )( VOID * mem_pool, VOID * uncached_mem_pool );
    INT         (* LeadApplication )( VOID );
    UNSIGNED    Key2;
    UNSIGNED    CRC;
}APP_ENTRY;

typedef long HAPP;                  /* 应用程序装载对象句柄 */
#endif

#ifndef XJ_SCHDULE_MODULE
#define XJ_SCHDULE_MODULE
#define CAS_MAIN_PROTECT        1   /* 主保护任务 */
#define CAS_BACKUP_PROTECT      2   /* 后备保护任务 */
typedef long HASH;                  /* 任务管理函数对象句柄 */
#endif

#ifndef XJ_CDDC_MODULE
#define XJ_CDDC_MODULE
typedef long HDDM;                  /* 设备驱动对象句柄 */
#endif

#ifndef XJ_CDMC_MODULE
#define XJ_CDMC_MODULE
typedef long HDMM;                  /* 动态内存对象句柄 */
#endif

#ifndef XJ_CECC_MODULE
#define XJ_CECC_MODULE
#define CEC_FIXED_SIZE          1   /* 固定数据长度 */
#define CEC_VARIABLE_SIZE       2   /* 可变数据长度 */
typedef long HDCM;                  /* 循环事件管理对象句柄 */
#endif

#ifndef XJ_CFGC_MODULE
#define XJ_CFGC_MODULE
typedef long HFGM;                  /* 配置数据对象句柄 */
#endif

#ifndef XJ_CITC_MODULE
#define XJ_CITC_MODULE
typedef long HITM;                  /* 索引号对象句柄 */
#endif

#ifndef XJ_CLEC_MODULE
#define XJ_CLEC_MODULE
#define CLE_LED_EXTINGUISH      0x1 /* 装置运行灯熄灭状态 */
#define CLE_LED_CORUSCATE       0x2 /* 装置运行灯闪烁状态 */
#define CLE_LED_LIGHT           0x4 /* 装置运行灯常亮状态 */

#define CLE_LED_START           0x1 /* 运行灯启动状态 */
#define CLE_LED_ABNORMAL        0x2 /* 运行灯异常状态 */
#define CLE_LED_NORMAL          0x4 /* 运行灯正常状态 */

typedef long HLEM;                  /* 运行灯对象句柄 */
#endif

#ifndef XJ_CNDC_MODULE
#define XJ_CNDC_MODULE
typedef long HNDM;                  /* 整定值对象句柄 */
#endif

#ifndef XJ_CNUC_MODULE
#define XJ_CNUC_MODULE
typedef long HPRM;                  /* 数据保护对象句柄 */
typedef long HTMM;                  /* 定时器管理对象句柄 */
typedef long HEVM;                  /* 事件位管理对象句柄 */
typedef long HMGM;                  /* 内存池管理对象句柄 */
typedef long HQUEM;                 /* 消息队列管理对象句柄 */
#endif

#ifndef XJ_CRCC_MODULE
#define XJ_CRCC_MODULE
/* crc数据表类型定义 */
#define CRCC_CRC16_TABLE        0   /* 16位数据表 */
#define CRCC_CRC32_TABLE        1   /* 32位数据表 */
#endif

#ifndef XJ_TABLE_MODULE
#define XJ_TABLE_MODULE
#define TYPE_NAME               20  /* 数据类型名称长度 */
#define TABLE_NAME              12  /* 表名称长度 */
#define FIELD_NAME              12  /* 字段名称长度 */
#define MAX_CONDITION           3   /* 操作字段的最大限制条件个数 */
#define MAX_HANDLE              4   /* 最大操作字段个数 */
/* 字段属性定义 */
#define FIELD_CONFIG_ENABLE     1   /* 字段配置选项 */
#define FIELD_SETTING_ENABLE    2   /* 字段整定选项 */
#define FIELD_CRC_ENABLE        4   /* 字段CRC选项 */
#define FIELD_VALUE_ENABLE      8   /* 字段量值(包括整定值)选项 */
#define FIELD_CHECK_ENABLE      0x10 /* 字段边界检查选项 */
/* 数据类型定义 */
#define TYPE_CHAR               1   /* char */
#define TYPE_UNSIGNED_CHAR      2   /* unsigned char */
#define TYPE_INT                3   /* int */
#define TYPE_SIGNED             4   /* long */
#define TYPE_UNSIGNED           5   /* unsigned long */
#define TYPE_FLOAT              6   /* float */
/* 修改整定值选项 */
#define SETTING_ALL_ENABLE      1   /* 全部整定值 */
#define SETTING_ONE_ENABLE      2   /* 单个整定值 */
/* 条件类型 */
#define COND_TYPE_AND           1   /* 与操作 */
#define COND_TYPE_OR            2   /* 或操作 */
#define COND_TYPE_EQU           3   /* 等于操作 */
/* 数据表类型 */
#define TABLE_TYPE_RELATION     (1 << 0) /* 双CPU数据表数据关联 */

/* 条件字段操作块结构定义 */
typedef struct TB_CFB_STRUCT
{
    /* 操作字段索引 */
    INT HandleIndex;
    /* 条件字段个数 */
    INT CondNumber;
    /* 条件字段索引 */
    INT CondIndex[MAX_CONDITION];
    /* 条件字段值 */
    INT CondValue[MAX_CONDITION];
    /* 条件类型 */
    INT CondType[MAX_CONDITION];
}TB_CFB;

/* 条件字段操作块结构定义 */
typedef struct TB_CHB_STRUCT
{
    /* 操作字段个数 */
    INT HandleNumber;
    /* 操作字段索引 */
    INT HandleIndex[MAX_HANDLE];
    /* 条件字段个数 */
    INT CondNumber;
    /* 条件字段索引 */
    INT CondIndex[MAX_CONDITION];
    /* 条件字段值 */
    INT CondValue[MAX_CONDITION];
    /* 条件类型 */
    INT CondType[MAX_CONDITION];
}TB_CHB;

typedef long HTAB;                  /* 应用程序对象句柄 */
#endif

#ifndef XJ_TIME_MODULE
#define XJ_TIME_MODULE
/* 对时时间类型定义 */
#define CTM_TYPE_GPS_SECOND       (1 << 0) /* 秒脉冲对时 */
#define CTM_TYPE_GPS_MINUTE       (1 << 1) /* 分脉冲对时 */
#define CTM_TYPE_GPS_IRIGB        (1 << 2) /* IRIG-B码对时 */
#define CTM_TYPE_NETWORK          (1 << 3) /* 网络对时 */
#define CTM_TYPE_61850            (1 << 4) /* 61850对时 */
#define CTM_TYPE_1588             (1 << 5) /* 1588对时 */

/* 时间品质定义 */
#define CTM_TIME_ACCURACY         0        /* 装置时间准确度选择 */
#define CTM_Q_LEAP_SECOND         (1 << 7) /* 闰秒 */
#define CTM_Q_CLOCK_FAILURE       (1 << 6) /* 时钟故障 */
#define CTM_Q_CLOCK_NO_SYN        (1 << 5) /* 时钟未同步 */
#define CTM_Q_TIME_ACCURACY(x)    (x << CTM_TIME_ACCURACY) /* 时间准确度 */

#define PROFIBUS_MODE             0x1  /* PROFIBUS时间转换成装置时间 */
#define UTC_MODE                  0x2  /* UTC时间转换成装置 */

/* 装置时间结构 */
typedef struct TIME_STRUCT
{
    UNSIGNED    Year;                                       // year
    UNSIGNED    Month;                                      // month
    UNSIGNED    Day;                                        // day
    UNSIGNED    Hour;                                       // hour
    UNSIGNED    Minute;                                     // minute
    UNSIGNED    Millionsecond;                              // millionsecond
    UNSIGNED    Nanosecond;                                 // nanosecond
    UNSIGNED    Quality;                                    // quality
}TIME;
#endif

/* 看门狗模块定义 */
#ifndef XJ_CWDC_MODULE
#define XJ_CWDC_MODULE
#define CWD_WATCHDOG_NORMAN     0   /* 被监视任务正常 */
#define CWD_WATCHDOG_ERROR      1   /* 被监视任务异常 */
#define CWD_WATCHDOG_ENABLE     1   /* 使能监视任务 */
#define CWD_WATCHDOG_DISABLE    0   /* 禁止监视任务 */

typedef long HWDM;                  /* 看门狗监视对象句柄 */
#endif

/* 开出模块定义 */
#ifndef XJ_CIOC_MODULE
#define XJ_CIOC_MODULE
#define CIO_OR                  1 /* 或 */
#define CIO_AND                 2 /* 与 */
#define CIO_EQU                 4 /* 等于 */

typedef long HIOM; /* 应用程序对象句柄 */
#endif

#ifndef XJ_CSFC_MODULE
#define XJ_CSFC_MODULE
/* 链接节点定义 */
typedef struct CSF_NODE_STRUCT
{
    struct CSF_NODE_STRUCT
        * previous; /* previous node */
    struct CSF_NODE_STRUCT
        * next; /* next node */
}CSF_NODE;
#endif

////typedef long HMUM; /* 互斥量对象句柄 */
typedef VOID * HMUM; /* 互斥量对象句柄 */
/*=======================================================================*/
/* Define service completion status constants.  */
#define XJP_LOAD_SUCCESS            0 /* 装载应用程序成功 */
#define XJP_NO_APPLICATION          1 /* 未找到应用程序 */
#define XJP_VERSION_ERROR           2 /* 应用程序版本错误 */
#define XJP_LOAD_ERROR              3 /* 装载应用程序失败 */
#define XJP_ARRAGE_FULL             1 /* 排序数据满 */
#define XJP_ARRAGE_SAME             2 /* 排序数据相同 */
#define XJP_WATCHDOG_NORMAN         0 /* 被监视任务正常 */
#define XJP_WATCHDOG_ERROR          1 /* 被监视任务异常 */

/* Define constants for use in service parameters.  */
#define XJP_MAIN_PROTECT            1 /* 主保护任务 */
#define XJP_BACKUP_PROTECT          2 /* 后备保护任务 */
#define XJP_EVENT_FIXED_SIZE        1 /* 固定数据长度 */
#define XJP_EVENT_VARIABLE_SIZE     2 /* 可变数据长度 */
#define XJP_LED_EXTINGUISH        0x1 /* 装置运行灯熄灭状态 */
#define XJP_LED_CORUSCATE         0x2 /* 装置运行灯闪烁状态 */
#define XJP_LED_LIGHT             0x4 /* 装置运行灯常亮状态 */
#define XJP_LED_START             0x1 /* 运行灯启动状态 */
#define XJP_LED_ABNORMAL          0x2 /* 运行灯异常状态 */
#define XJP_LED_NORMAL            0x4 /* 运行灯正常状态 */
#define XJP_CRC16_TABLE             0 /* 16位数据表 */
#define XJP_CRC32_TABLE             1 /* 32位数据表 */
#define XJP_SETTING_ALL_ENABLE      1 /* 全部整定值 */
#define XJP_SETTING_ONE_ENABLE      2 /* 单个整定值 */
#define XJP_WATCHDOG_ENABLE         1 /* 使能监视任务 */
#define XJP_WATCHDOG_DISABLE        0 /* 禁止监视任务 */

/* Define constants for use in struct.  */
#define XJP_MAX_CONDITION           3 /* 操作字段的最大限制条件个数 */
#define XJP_MAX_HANDLE              4 /* 最大操作字段个数 */
#define XJP_START_RELAY_FLAG  0x55AAAA55 /* 启动继电器动作标志 */

/* Define constants for use in mode parameter of driver */
#define XJP_DRV_MODE_INPUT      (1 << 0)
#define XJP_DRV_MODE_OUTPUT     (1 << 1)
#define XJP_DRV_MODE_INOUT      (XJP_DRV_MODE_INPUT | XJP_DRV_MODE_OUTPUT)

/* Define constants for use in data type parameter of driver */
#define XJP_DRV_DATA_VALUE      (1 << 0)
#define XJP_DRV_DATA_QUALITY    (1 << 1)
#define XJP_DRV_DATA_VQ         (XJP_DRV_DATA_VALUE | XJP_DRV_DATA_QUALITY)

/* These types are used to identify the type of the device driver. */
#define XJP_DRV_TYPE_IO         0
#define XJP_DRV_TYPE_OTHER      1

/* request command type for mdSubmitChan */
#define XJP_DRV_CMD_READ        0
#define XJP_DRV_CMD_WRITE       1
#define XJP_DRV_CMD_ABORT       2
#define XJP_DRV_CMD_FLUSH       3

/* request command type for mdControlChan */
#define XJP_DRV_CTR_INITIALIZE  0
#define XJP_DRV_CTR_RESET       1
#define XJP_DRV_CTR_CLEAR       2

/*=======================================================================*/
/* Map directly to control function */
#define XJP_Create_LoadObj                      CAPC_Create
#define XJP_Link_Application                    CAPC_LinkApplication
#define XJP_Create_AppRoutine                   CASC_CreateAppRoutine
#define XJP_SetLockMainTaskSleepFlag            CASC_SetLockMainTaskSleepFlag
#define XJP_Delete_AppRoutine                   CASC_DeleteAppRoutine
#define XJP_Change_Time_Slice                   CASC_ChangeTaskTimeSlice
#define XJP_Change_Sleep_Ticks                  CASC_ChangeSleepTicks
#define XJP_Execute_App_Routine                 CASC_ExecuteAppRoutine
#define XJP_Get_Main_Run_Time                   CASC_GetMainRunTime
#define XJP_Reset_App_Routine                   CASC_ResetAppRoutine
#define XJP_Register_Backup_Task                CASC_RegisterBackupTask
#define XJP_ResumeMainTask                      CASC_ResumeMainTask
#define XJP_Create_Driver                       CDDC_Create
#define XJP_Driver_Initialize                   CDDC_DriverInitialize
#define XJP_Driver_Read                         CDDC_ReadDriverData
#define XJP_Driver_Write                        CDDC_WriteDriverData
#define XJP_Driver_Assign                       CDDC_AssignSemaphore
#define XJP_Driver_Release                      CDDC_ReleaseSemaphore
#define XJP_Copy_NVdata                         CDDC_CopyNVdata
#define XJP_NVData_Create                       CNDC_Create
#define XJP_NVData_Read                         CNDC_ReadNVData
#define XJP_NVData_Write                        CNDC_WriteNVData
#define XJP_NVData_Repair                       CNDC_RepairNVData
#define XJP_Config_Create                       CFGC_Create
#define XJP_Config_Initialize                   CFGC_ClearConfigData
#define XJP_Config_Read                         CFGC_ReadConfigData
#define XJP_Config_Write                        CFGC_WriteConfigData
#define XJP_Background_Initialize               CBGC_Initialize
#define XJP_Background_Read                     CBGC_ReadBackgroundData
#define XJP_Background_Write                    CBGC_WriteBackgroundData
#define XJP_Output_Hold_Initialize              COHC_Initialize
#define XJP_Output_Hold_Read                    COHC_ReadOutputHoldData
#define XJP_Output_Hold_Write                   COHC_WriteOutputHoldData
#define XJP_Memory_Pool_Create                  CDMC_Create
#define XJP_Memory_Allocate                     CDMC_AllocateMemory
#define XJP_Memory_Deallocate                   CDMC_DeallocateMemory
#define XJP_Queue_Create                        CECC_Create
#define XJP_Queue_Clear                         CECC_ClearData
#define XJP_Queue_Find                          CECC_FindData
#define XJP_Queue_Read                          CECC_ReadData
#define XJP_Queue_Write                         CECC_WriteData
#define XJP_Function_Table_Retrieve             CFTC_GetFunTable
#define XJP_Function_Table_Register             CFTC_RegisterFunctionList
#define XJP_Index_Create                        CITC_Create
#define XJP_Index_Write                         CITC_Write
#define XJP_Index_Delete                        CITC_Delete
#define XJP_Index_Clear                         CITC_Clear
#define XJP_Index_List                          CITC_ListData
#define XJP_Index_ListL                         CITC_ListDataL
#define XJP_Index_ListH                         CITC_ListDataH
#define XJP_Index_Read                          CITC_ReadData
#define XJP_Led_Create                          CLEC_Create
#define XJP_Led_Change_Status                   CLEC_SetLedStatus
#define XJP_Led_Change_Frequency                CLEC_SetDevLedFrequency
#define XJP_Led_Change_Type                     CLEC_SetDevLedType
#define XJP_Led_Timer                           CLEC_Timer
#define XJP_Led_SetDevLedCallStatus             CLEC_SetDevLedCallStatus
#define XJP_Led_DevLedCallTimer                 CLEC_DevLedCallTimer
#define XJP_Dbg_Memory_Create                   CMCM_MemoryCreate
#define XJP_Dbg_Memory_Check                    CMCM_MemoryCheck
#define XJP_Dbg_Time_Testing                    CTMC_TimeTesting
#define XJP_Dbg_Register_FuncList               PDBG_Register_DebugFunctionList
#define XJP_Mutex_Initialize                    CMUC_Initialize
#define XJP_Mutex_Create                        CMUC_Create
#define XJP_Mutex_Obtain                        CMUC_ObtainMutex
#define XJP_Mutex_Release                       CMUC_ReleaseMutex
#define XJP_Overtime_Initialize                 COTC_Initialize
#define XJP_OverTime_Main                       COTC_Main
#define XJP_OverTime_Place_On                   COTC_Place_On_Check
#define XJP_OverTime_Remove                     COTC_Remove_From_Check
#define XJP_CRC_Calculate                       CRCC_CalculateCRC
#define XJP_CRC_Get_Table                       CRCC_GetCrcTable
#define XJP_CSC_Place_On                        CSFC_PlaceOnList
#define XJP_CSC_Remove                          CSFC_RemoveFromList
#define XJP_CSC_Word_To_String                  CSFC_WordToString
#define XJP_CSC_String_To_Word                  CSFC_StringToWord
#define XJP_CSC_Word_To_Byte                    CSFC_WordToByte
#define XJP_CSC_Byte_To_Word                    CSFC_ByteToWord
#define XJP_CSC_Strcmp                          CSFC_strcmp
#define XJP_CSC_Arrage_Insert                   CSFC_ArrageInsert
#define XJP_CSC_Arrage_Insert_Full              CSFC_ArrageInsertFull
#define XJP_CSC_Get_Valid_Number                CSFC_GetValidNumber
#define XJP_DS_Define_Table                     CTBC_DefTable
#define XJP_DS_Define_Field                     CTBC_DefField
#define XJP_DS_Create_All_Field                 CTBC_CreateAllField
#define XJP_DS_Get_Field_Value_One              CTBC_GetOneFieldValue
#define XJP_DS_Get_Field_Value_Batch            CTBC_GetBatchFieldValue
#define XJP_DS_Get_Field_Value_All              CTBC_GetAllFieldValue
#define XJP_DS_Define_SG_Range                  CTBC_DefSGRange
#define XJP_DS_Set_Field_Value_Config           CTBC_SetConfigValues
#define XJP_DS_Select_Edit_Field_SG             CTBC_SelectEditSG
#define XJP_DS_Set_Field_Value_SG               CTBC_SetEditSGValues
#define XJP_DS_Confirm_Field_Value_SG           CTBC_ConfirmEditSGValue
#define XJP_DS_Save_Field_Value_Config          CTBC_SaveConfigValue
#define XJP_DS_Lead_Field_Value_Config          CTBC_LeadConfigValue
#define XJP_DS_Save_Tab_Field_Value_Config      CTBC_SaveTabConfigValue
#define XJP_DS_ConfigData_Repair                CTBC_RepairConfigData
#define XJP_DS_Save_Field_Value_SG              CTBC_SaveSGValue
#define XJP_DS_Lead_Field_Value_SG              CTBC_LeadSGValue
#define XJP_DS_Initialize                       CTBC_TableInitialize
#define XJP_DS_Define_Protect_Handle            CTBC_DefProtectHandle
#define XJP_DS_Freeze_Table                     CTBC_FreezeTableData
#define XJP_DS_UnFreeze_Table                   CTBC_UnFreezeTableData
#define XJP_DS_Freeze_Status                    CTBC_GetFreezeStatus
#define XJP_DS_Freeze_Initialize                CTBC_FreezeTableInitialize
#define XJP_DS_Initialize_SetData               CTBC_SetDataInitialize
#define XJP_DS_NVData_Write                     CTBC_WriteNVData
#define XJP_DS_NVData_Read                      CTBC_ReadNVData
#define XJP_DS_NVData_Repair                    CTBC_RepairNVData
#define XJP_DS_ConfigData_Write                 CTBC_WriteConfigData
#define XJP_DS_ConfigData_Read                  CTBC_ReadConfigData
#define XJP_DS_Register_Pre_Treatment           CTBC_RegisterPreTreatmentFunc
#define XJP_DS_Register_Fore_Treatment          CTBC_RegisterForeTreatmentFunc
#define XJP_DS_Register_After_Treatment         CTBC_RegisterAfterTreatmentFunc
#define XJP_DS_Register_End_Treatment           CTBC_RegisterEndTreatmentFunc
#define XJP_DS_Register_Lead_Fore_Treatment     CTBC_RegisterForeLeadTreatmentFunc
#define XJP_DS_Register_Lead_After_Treatment    CTBC_RegisterAfterLeadTreatmentFunc
#define XJP_DS_Get_Table_Number                 CTBC_GetTableNumber
#define XJP_DS_Get_Table_Memory                 CTBC_GetTableMemory
#define XJP_DS_Get_Table_Size                   CTBC_GetTableSize
#define XJP_DS_Get_Table_Inf                    CTBC_GetTableInf
#define XJP_DS_Get_Field_Inf                    CTBC_GetFieldInf
#define XJP_DS_Get_Table_Status                 CTBC_GetTabStatus
#define XJP_DS_Get_Config_CRC                   CTBC_GetConfigTabCRC
#define XJP_DS_Get_Tab_Field_CRC                CTBC_GetTabFieldCRC
#define XJP_DS_Get_Table_Memory_Index           CTBC_GetIndexTableMemory
#define XJP_DS_Get_Table_Length                 CTBC_GetTabLength
#define XJP_DS_Get_Field_Length                 CTBC_GetFieldLength
#define XJP_DS_Get_Data_Number                  CTBC_ReadDataNumber
#define XJP_DS_Com_Hmi_Modify                   CTBC_COM_HMI_ModifData
#define XJP_DS_Update_SGValue                   CTBC_UpdateSGValue
#define XJP_DS_R_SGValueCRC                     CTBC_R_SGValueCRC
#define XJP_DS_F_SGValueCRC                     CTBC_F_SGValueCRC
#define XJP_DS_SGValueCheck                     CTBC_SGValueCheck
#define XJP_DS_GetSGValueCounter                CTBC_GetSGValueCounter
#define XJP_DS_Register_NVData_Treatment        CTBC_RegisterNVDataTreatmentFunc
#define XJP_DS_Register_Config_Treatment        CTBC_RegisterConfigTreatmentFunc
#define XJP_DS_GetTablePointers                 CTBC_GetTablePointers
#define XJP_TM_Set_Time_Para                    CTMC_SetTimePara
#define XJP_TM_Updata_Time                      CTMC_UpdateRunTime
#define XJP_TM_Get_Time_Dev                     CTMC_GetTime
#define XJP_TM_Set_Time_Dev_Zone                CTMC_SetTime
#define XJP_TM_Set_Time_Dev_Local               CTMC_SetTimeBase
#define XJP_TM_Get_Time_61850                   CTMC_GetTime_61850
#define XJP_TM_Set_Time_61850                   CTMC_SetTime_61850
#define XJP_TM_Set_Time_Type                    CTMC_SetTimeType
#define XJP_TM_Get_Time_Type                    CTMC_GetTimeType
#define XJP_TM_Run_Clock                        CTMC_RunClock
#define XJP_TM_Get_Pulse_Counter                CTMC_GetPulseNum
#define XJP_TM_Get_Init_Flag                    CTMC_GetInitFlag_61850
#define XJP_TM_GPS_Interrupt                    CTMC_GPS_Interrupt
#define XJP_TM_Get_Clock_Precision              CTMC_ReadSystemClock_Microsecond
#define XJP_TM_Get_Clock_Millisecond            CTMC_ReadSystemClock
#define XJP_TM_Save_Time                        CTMC_SaveTime
#define XJP_TM_Dev_To_MS_UTC                    CTMC_TransformTimeFromSysTime
#define XJP_TM_MS_UTC_To_Dev                    CTMC_TransformTimeToSysTime
#define XJP_TM_Dev_To_NS_UTC                    CTMC_Transform_Device_To_61850
#define XJP_TM_NS_UTC_To_Dev                    CTMC_Transform_61850_To_Device
#define XJP_TM_SET_Time_Interval                CTMC_SetTimeInterval
#define XJP_TM_SET_Time_Quality                 CTMC_SetTimeQuailty
#define XJP_TM_Trans_Dev_To_LT                  TPTC_Transform_Time_From_SYS
#define XJP_TM_Trans_LT_To_Dev                  TPTC_Transform_Time_To_SYS
#define XJP_TM_Set_Time_Mode                    TPTC_Set_Time_Mode
#define XJP_TM_Get_Time_Zone                    CTMC_GetTimeZone
#define XJP_TM_Time_Check                       CTMC_Check_Time
#define XJP_TM_Get_Time_Ptr                     CTMC_GetTimePointer
#define XJP_TM_Set_SysInitFlag                  CTMC_SetSysInitFlag
#define XJP_Watchdog_Create                     CWDC_Create
#define XJP_Watchdog_Reset                      CWDC_Reset
#define XJP_Watchdog_Control                    CWDC_Enable
#define XJP_Watchdog_Timer                      CWDC_Timer
#define XJP_ELF_Calculate_CRC16                 calc_elf_file_crc
#define XJP_ELF2_Calculate_CRC16                calc_elf2_file_crc
#define XJP_ELF_Text_CRC16                      calc_elf_text_crc
#define XJP_ELF_Text_Addr                       get_elf_text_addr
#define XJP_ELF_File_CRC16                      TCRC_ElfFileCRC16
#define XJP_NU_Protect_Create                   CNUC_CreateDataProtect
#define XJP_NU_Protect_Find                     CNUC_GetDataProtect
#define XJP_NU_Protect                          CNUC_Protect
#define XJP_NU_UnProtect                        CNUC_UnProtect
#define XJP_NU_Timer_Create                     CNUC_CreateTimer
#define XJP_NU_Timer_Control                    CNUC_ControlTimer
#define XJP_NU_Event_Group_Create               CNUC_CreateEventGroup
#define XJP_NU_Event_Group_Set                  CNUC_SetEvents
#define XJP_NU_Event_Group_Retrieve             CNUC_RetrieveEvents
#define XJP_NU_Pool_Create                      CNUC_Create_Memory_Pool
#define XJP_NU_Pool_Delete                      CNUC_Delete_Memory_Pool
#define XJP_NU_Allocate_Memory                  CNUC_Allocate_Memory
#define XJP_NU_Deallocate_Memory                CNUC_Deallocate_Memory
#define XJP_OS_Get_FuncList                     COSC_GetOsFuncList
#define XJP_IO_Create_Out                       CIOC_Create
#define XJP_IO_Set_Start_Relay                  CIOC_SetStartRelay
#define XJP_IO_Set_Output_Relay                 CIOC_SetOutputBits
#define XJP_IO_Get_Start_Relay                  CIOC_GetDevStartRelay
#define XJP_IO_Get_Output_Relay                 CIOC_GetDevOutputBits
#define XJP_IO_Set_Start_Driver                 CIOC_SetStartDriver
#define XJP_IO_Set_Output_Driver                CIOC_SetRelayDriver
#define XJP_IO_Get_Hold_Relay                   CIOC_GetHoldRelayStatus
#define XJP_IO_Set_Hold_Relay                   CIOC_SetHoldRelayStatus
#define XJP_IO_Clear_Hold_Relay                 CIOC_ClearHoldRelayStatus
#define XJP_IO_Set_Hold_Masks                   CIOC_SetHoldRelayMaskBits
#define XJP_DRV_Initialize                      CDEV_Initialize
#define XJP_DRV_mdBindDevice                    CDEV_mdBindDevice
#define XJP_DRV_gioCreate                       CGIO_create
#define XJP_DRV_gioSubmit                       CGIO_submit
#define XJP_DRV_gioControl                      CGIO_control
#define XJP_File_Save                           CDDC_fileSave
#define XJP_File_Read                           CDDC_fileRead
#define XJP_CalculateFileCRC                    CDDC_CalculateFileCRC
#define XJP_CreateDataQueue                     CNUC_Create_Queue
#define XJP_SendToDataQueue                     CNUC_Send_To_Queue
#define XJP_ReceiveFromDataQueue                CNUC_Receive_From_Queue
#define XJP_DataQueueInformation                CNUC_Queue_Information
#define XJP_DeleteDataQueue                     CNUC_Delete_Queue
/*=======================================================================*/
/* Define control functions.  */
/*=======================================================================*/
/* Define loadding management functions.  */
/* 创建加载对象 */
HAPP XJP_Create_LoadObj( UNSIGNED StartAddr, UNSIGNED Length, UNSIGNED Key1,
    UNSIGNED Key2, VOID * PlatFuntion, UNSIGNED PlatFuncSize,
    CHAR * FileName, CHAR * SectName, CHAR * KeyName, CHAR * EntryName );
/* 加载应用程序 */
INT XJP_Link_Application( HAPP Handle, VOID(* WatchDogReset )( VOID ),
    INT(* CalCRCCode )( const VOID *, UNSIGNED, INT, INT, UNSIGNED, UNSIGNED *),
    APP_ENTRY ** AppFunctionList );

/* Define task management functions.  */
/* 创建应用程序 */
HASH XJP_Create_AppRoutine( INT TaskType, UNSIGNED ProtectId, UNSIGNED SectorId,
    VOID (* application_routine )( UNSIGNED, UNSIGNED, INT ));
/* 设置主保护任务是否暂停休眠函数 */
INT XJP_SetLockMainTaskSleepFlag( INT iFlag );
/* 删除应用程序,只能在运行函数外执行 */
INT XJP_Delete_AppRoutine( HASH Handle );
/* 设置任务时间片 */
INT XJP_Change_Time_Slice( INT TaskType, UNSIGNED TimeSlice );
/* 修改应用任务休眠时间片 */
INT XJP_Change_Sleep_Ticks(  INT TaskType, UNSIGNED TimeSlice );
/* 调度应用任务 */
VOID XJP_Execute_App_Routine( INT TaskType );
/* 获取主任务执行时间 */
INT XJP_Get_Main_Run_Time( UNSIGNED * RunTime_ptr, UNSIGNED * SleepTime_ptr );
/* 复归应用任务 */
VOID XJP_Reset_App_Routine( VOID );
/* 注册后备任务句柄 */
VOID XJP_Register_Backup_Task( VOID * Task );
/* 恢复主任务 */
INT XJP_ResumeMainTask( UNSIGNED Time );

/* Define driver management functions.  */
/* 创建对象 */
HDDM XJP_Create_Driver( VOID (* driver_entry )( XJ_DRIVER , XJ_DRIVER_REQUEST *) );
/* 初始化设备驱动函数 */
INT XJP_Driver_Initialize( HDDM Handle, UNSIGNED uId, UNSIGNED uTimeOut );
/* 读驱动数据 */
INT XJP_Driver_Read( HDDM Handle, UNSIGNED uId, VOID * Buffer,
    UNSIGNED uOffset, UNSIGNED uSize, UNSIGNED uTimeOut );
/* 写驱动数据 */
INT XJP_Driver_Write( HDDM Handle, UNSIGNED uId, VOID * Buffer,
    UNSIGNED uOffset, UNSIGNED uSize, UNSIGNED uTimeOut );
/* 获取信号量 */
INT XJP_Driver_Assign( HDDM Handle, UNSIGNED uTimeOut );
/* 释放信号量 */
INT XJP_Driver_Release( HDDM Handle );
/* 复制文件 */
INT XJP_Copy_NVdata( CHAR * fileNameSrc, CHAR * fileNameDst );

/* Define nvdata management functions.  */
/* 创建整定值数据区间对象句柄 */
HNDM XJP_NVData_Create( CHAR * dirName, CHAR * fileName, INT Times );
/* 读整定值数据 */
INT XJP_NVData_Read( HNDM Handle, VOID * Buffer, UNSIGNED uOffset,
    UNSIGNED uSize, UNSIGNED uTimeOut );
/* 写整定值数据 */
INT XJP_NVData_Write( HNDM Handle, VOID * Buffer, UNSIGNED uOffset,
    UNSIGNED uSize, UNSIGNED uTimeOut );
/* 修复整定值数据 */
INT XJP_NVData_Repair( HNDM Handle, UNSIGNED uTimeOut );

/* Define config management functions.  */
/* 创建配置数据对象句柄 */
HFGM XJP_Config_Create( CHAR * dirName, CHAR * fileName );
/* 清除配置数据 */
INT XJP_Config_Initialize( HFGM Handle, UNSIGNED uTimeOut );
/* 读配置数据 */
INT XJP_Config_Read( HFGM Handle, VOID * Buffer, UNSIGNED uOffset,
    UNSIGNED uSize, UNSIGNED uTimeOut );
/* 写配置数据 */
INT XJP_Config_Write( HFGM Handle, VOID * Buffer, UNSIGNED uOffset,
    UNSIGNED uSize, UNSIGNED uTimeOut );

/* Define background management functions.  */
/* 初始化背景数据管理区间 */
INT XJP_Background_Initialize( CHAR * fullFileName );
/* 读背景数据 */
INT XJP_Background_Read( VOID * Buffer, UNSIGNED uOffset,
    UNSIGNED uSize, UNSIGNED uTimeOut );
/* 写背景数据 */
INT XJP_Background_Write( VOID * Buffer, UNSIGNED uOffset,
    UNSIGNED uSize, UNSIGNED uTimeOut );
/* Define output hold management functions.  */
/* 初始化出口保持位数据管理区间 ByteNumber:32位物理数据的有效字节数 */
INT XJP_Output_Hold_Initialize( CHAR * dirName );
/* 读出口保持位数据 */
INT XJP_Output_Hold_Read( VOID * Buffer, UNSIGNED uOffset,
    UNSIGNED uSize, UNSIGNED uTimeOut );
/* 写出口保持位数据 */
INT XJP_Output_Hold_Write( VOID * Buffer, UNSIGNED uOffset,
    UNSIGNED uSize, UNSIGNED uTimeOut );

/* Define dynamic memory pool management functions.  */
/* 创建对象 */
HDMM XJP_Memory_Pool_Create( unsigned int PoolSize );
/* 分配内存 */
INT XJP_Memory_Allocate( HDMM Handle, VOID ** return_pointer, UNSIGNED size );
/* 释放内存*/
INT XJP_Memory_Deallocate( HDMM Handle, VOID * memory );

/* Define queue management functions.  */
/* 创建对象 */
HDCM XJP_Queue_Create( UNSIGNED BufSize, INT MessageType, UNSIGNED MessageSize );
/* 清除数据 */
INT XJP_Queue_Clear( HDCM Handle );
/* 检查数据 */
INT XJP_Queue_Find( HDCM Handle , UNSIGNED * Actual_Size );
/* 读数据 */
INT XJP_Queue_Read( HDCM Handle , VOID * pBuffer );
/* 写数据 */
INT XJP_Queue_Write( HDCM Handle , VOID * pBuffer );

/* Define function table management functions.  */
/* 获取函数表指针,NU_NULL:未找到函数表 */
VOID * XJP_Function_Table_Retrieve( CHAR * Name );
/* 注册函数表 */
INT XJP_Function_Table_Register( CHAR * Name, VOID * FunTablePtr );

/* Define index table management functions.  */
/* 创建对象 */
HITM XJP_Index_Create( INT MaxNumber );
/* 写数据 overData:溢出数据 overFlag:数据溢出(满)标志 */
INT XJP_Index_Write( HITM Handle, UNSIGNED Index, UNSIGNED * overData,
    INT * overFlag );
/* 删除数据 */
INT XJP_Index_Delete( HITM Handle, UNSIGNED Index );
/* 清除数据 */
INT XJP_Index_Clear( HITM Handle );
/* 获取指定数据个数的数据(从最新开始) */
INT XJP_Index_List( HITM Handle, UNSIGNED * Dst, UNSIGNED * Actual_Number,
    UNSIGNED Number );
/* 获取指定数据个数的低16位数据(保留最新数据) */
INT XJP_Index_ListL( HITM Handle, UNSIGNED * Dst, UNSIGNED * Actual_Number,
    UNSIGNED Number );
/* 获取指定数据个数的高16位数据(从最新开始) */
INT XJP_Index_ListH( HITM Handle, UNSIGNED * Dst, UNSIGNED * Actual_Number,
    UNSIGNED Number );
/* 获取满足高16位数据条件的32位数据(从最新开始) */
INT XJP_Index_Read( HITM Handle, UNSIGNED * Dst, UNSIGNED * Actual_Number,
    UNSIGNED Number, UNSIGNED CondMasks, UNSIGNED CondValue );

/* Define run led management functions.  */
/* 创建对象 */
HLEM XJP_Led_Create( VOID );
/* 设置运行灯状态 */
INT XJP_Led_Change_Status( HLEM Handle, UNSIGNED LedStatus );
/* 设置装置运行灯闪烁频率 */
INT XJP_Led_Change_Frequency( UNSIGNED Frequency );
/* 设置装置运行灯类型 */
INT XJP_Led_Change_Type( UNSIGNED NormalType, UNSIGNED StartType, UNSIGNED AbnormalType );
/* 装置运行灯定时器处理 */
UNSIGNED XJP_Led_Timer( UNSIGNED Time );
/* 设置装置呼叫运行灯状态 */
INT XJP_Led_SetDevLedCallStatus( INT AutoCallEnable, UNSIGNED LedCallDelay,
    UNSIGNED LedCallStatus, UNSIGNED Frequency );
/* 装置运行灯呼叫管理(返回值：0-无呼叫，!0-实际呼叫灯运行状态) */
UNSIGNED XJP_Led_DevLedCallTimer( UNSIGNED Time );

/* Define debug management functions.  */
/* 创建监视内存区 */
UNSIGNED XJP_Dbg_Memory_Create( VOID * mem_ptr, UNSIGNED mem_size,
    CHAR * mem_name, UNSIGNED value );
/* 内存监视 */
INT XJP_Dbg_Memory_Check( UNSIGNED MemHandle );
/* 测试执行时间函数 */
UNSIGNED XJP_Dbg_Time_Testing( UNSIGNED * millisecond_ptr, UNSIGNED * microsecond_ptr );
/* 注册调试函数表 */
INT XJP_Dbg_Register_FuncList( VOID );

/* Define mutex management functions.  */
/* 初始化互斥量 */
INT XJP_Mutex_Initialize( VOID );
/* 创建互斥量对象 */
HMUM XJP_Mutex_Create( CHAR * name, UNSIGNED initial_count, OPTION suspend_type );
/* 获取互斥量 */
INT XJP_Mutex_Obtain( HMUM Handle, UNSIGNED Timeout );
/* 释放互斥量 */
INT XJP_Mutex_Release( HMUM Handle );

/* Define overtime management functions.  */
/* 初始化超时检查数据 */
INT XJP_Overtime_Initialize( INT MaxNumber, VOID (* _lock )( VOID ),
    VOID (* _unlock )( VOID ) );
/* 超时检查主循环函数 */
INT XJP_OverTime_Main( UNSIGNED ClockTick );
/* 添加超时检查数据函数 */
INT XJP_OverTime_Place_On( VOID * memory, UNSIGNED overTime,
    VOID * callBackArgs, void (* _callBack )( void * arg ));
/* 删除超时检查数据函数 */
INT XJP_OverTime_Remove( VOID * memory );

/* Define CRC management functions.  */
INT XJP_CRC_Calculate( const VOID * Src, UNSIGNED Size, INT ByteNumber,
    INT CrcBits, UNSIGNED InitialValue, UNSIGNED * CRC_Code );
/* 获取crc数据表 */
VOID * XJP_CRC_Get_Table( INT iType );

/* Define common services management functions.  */
/* 添加链表节点 */
VOID XJP_CSC_Place_On( CSF_NODE ** head, CSF_NODE * new_node );
/* 删除链表节点 */
VOID XJP_CSC_Remove( CSF_NODE ** head, CSF_NODE * node );
/* 将WORD数据转换为字符串 */
char * XJP_CSC_Word_To_String( void * s1, const void * s2, register unsigned int n );
/* 将字符串转换为WORD数据 */
char * XJP_CSC_String_To_Word( void * s1, const void * s2, register unsigned int n );
/* 将WORD数据转换成字节数据 */
char * XJP_CSC_Word_To_Byte( void * s1, const void * s2, register unsigned int n );
/* 将字节数据转换成WORD数据 */
char * XJP_CSC_Byte_To_Word( void * s1, const void * s2, register unsigned int n );
/* 字符串比较 */
char * XJP_CSC_Strcmp( register const char * s1, register const char * s2 );
/* 循环数据排序(ascending-1:升序 maxNumber:最大数据个数:>=2 数据位数:16位) */
VOID XJP_CSC_Arrage_Insert( UNSIGNED * buff, UNSIGNED * actNumber, UNSIGNED maxNumber,
    UNSIGNED actData, INT ascending );
/* 循环数据排序并返回超过极限的无效数据(ascending-1:升序 maxNumber:最大数据个数:>=2) */
/* NU_TRUE:数据满, NU_FALSE:数据未满 */
INT XJP_CSC_Arrage_Insert_Full( UNSIGNED * buff, UNSIGNED * actNumber,
    UNSIGNED maxNumber, UNSIGNED actData, UNSIGNED * InvalidData, INT ascending );
/* 获取有效数据个数(ascending-1:升序 ),找不到截止数据时为全有效 */
UNSIGNED XJP_CSC_Get_Valid_Number( VOID * pBuffer, UNSIGNED Number, UNSIGNED endIndex,
    INT ascending, UNSIGNED uMaskBits );

/* Define common table management functions.  */
/* 定义数据表 */
INT XJP_DS_Define_Table( CHAR * Name, INT Index, INT MaxNumber, INT Size, INT Type,
    INT EditFieldSize );
/* 定义字段 */
INT XJP_DS_Define_Field( INT TableIndex, CHAR * Name, INT Index, INT Attribute,
    INT DataType, INT Number, INT Offset );
/* 生成表数据 */
VOID * XJP_DS_Create_All_Field( INT TableIndex, VOID * Buffer );
/* 获取单个字段的值 */
INT XJP_DS_Get_Field_Value_One( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * Buffer, INT * ActualNumber );
/* 读多个字段值 */
INT XJP_DS_Get_Field_Value_Batch( INT TableIndex, INT Offset, INT Number,
    TB_CHB * Handle, VOID * Buffer, INT * ActualNumber );
/* 获取全部字段的值 */
INT XJP_DS_Get_Field_Value_All( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * Buffer, INT * ActualNumber );
/* 定义整定值范围 */
INT XJP_DS_Define_SG_Range( INT TableIndex, INT TypeIndex, INT CoefIndex,
    INT MinIndex, INT MaxIndex );
/* 设置配置字段值 */
INT XJP_DS_Set_Field_Value_Config( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * buffer, INT Originate );
/* 选择编辑定值组 */
INT XJP_DS_Select_Edit_Field_SG( INT TableIndex, INT FieldIndex );
/* 设置定值组值 */
INT XJP_DS_Set_Field_Value_SG( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * buffer );
/* 确认编辑定值组值 */
INT XJP_DS_Confirm_Field_Value_SG( INT TableIndex, INT Type, UNSIGNED Offset,
    TB_CFB * pHandle, UNSIGNED TimeOut, INT ( _AckCall )( INT, UNSIGNED_CHAR *),
    UNSIGNED_CHAR * Dst );
/* 初始化数据表环境变量 */
INT XJP_DS_Initialize( INT MaxTabNumber, INT MaxFieldNumber );
/* 定义数据保护句柄 */
INT XJP_DS_Define_Protect_Handle( INT TableIndex, VOID * pProtectHandle );
/* 冻结数据表的数据 */
INT XJP_DS_Freeze_Table( INT TableIndex );
/* 解冻数据表的数据 */
INT XJP_DS_UnFreeze_Table( INT TableIndex );
/* 获取数据表的冻结状态 */
INT XJP_DS_Freeze_Status( INT TableIndex );
/* 初始化数据表的数据冻结状态 */
INT XJP_DS_Freeze_Initialize( INT TableIndex );
/* 保存配置字段值 */
INT XJP_DS_Save_Field_Value_Config( HTAB * pTabList, UNSIGNED TimeOut );
/* 引导全部配置字段值 */
INT XJP_DS_Lead_Field_Value_Config( UNSIGNED TimeOut );
/* 保存表的配置字段值 */
INT XJP_DS_Save_Tab_Field_Value_Config( INT TableIndex, UNSIGNED TimeOut );
/* 修复配置数据 */
INT XJP_DS_ConfigData_Repair( UNSIGNED TimeOut );
/* 保存全部定值组值 */
INT XJP_DS_Save_Field_Value_SG( UNSIGNED TimeOut );
/* 引导全部定值组值 */
INT XJP_DS_Lead_Field_Value_SG( UNSIGNED TimeOut );
/* 数据表基本数据初始化 */
INT XJP_DS_Initialize_SetData( INT iType, UNSIGNED uTimeOut );
/* 固化非易失整定数据 */
INT XJP_DS_NVData_Write( VOID * Buffer, UNSIGNED uOffset, UNSIGNED uSize,
    UNSIGNED uTimeOut );
/* 读取非易失整定数据 */
INT XJP_DS_NVData_Read( VOID * Buffer, UNSIGNED uOffset, UNSIGNED uSize,
    UNSIGNED uTimeOut );
/* 修复非易失整定数据 */
INT XJP_DS_NVData_Repair( UNSIGNED TimeOut );
/* 固化配置信息 */
INT XJP_DS_ConfigData_Write( VOID * Buffer, UNSIGNED uOffset,
    UNSIGNED uSize, UNSIGNED uTimeOut );
/* 读取配置信息 */
INT XJP_DS_ConfigData_Read( VOID * Buffer, UNSIGNED uOffset,
    UNSIGNED uSize, UNSIGNED uTimeOut );
/* 注册字段预处理函数 */
INT XJP_DS_Register_Pre_Treatment( INT TableIndex, INT FieldIndex,
    INT (* preTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr,
    VOID * valuePtr, INT Originate ) );
/* 注册字段前处理函数 */
INT XJP_DS_Register_Fore_Treatment( INT TableIndex, INT FieldIndex,
    INT (* foreTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr,
    VOID * valuePtr ) );
/* 注册字段后处理函数 */
INT XJP_DS_Register_After_Treatment( INT TableIndex, INT FieldIndex,
    INT (* afterTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, INT Originate ) );
/* 注册字段结束处理函数 */
INT XJP_DS_Register_End_Treatment( INT TableIndex, INT FieldIndex,
    INT (* endTreatment )( INT TabIndex, INT FieldIndex ) );
/* 注册引导整定值前处理函数 */
INT XJP_DS_Register_Lead_Fore_Treatment( INT TableIndex,
    INT (* foreLeadTreatment )( INT TabIndex ) );
/* 注册引导整定值后处理函数 */
INT XJP_DS_Register_Lead_After_Treatment( INT TableIndex,
    INT (* afterLeadTreatment )( INT TabIndex ) );
/* 获取数据表的总个数 */
INT XJP_DS_Get_Table_Number( VOID );
/* 获取数据表内存地址 */
VOID * XJP_DS_Get_Table_Memory( INT TabIndex, INT * ActualNum );
/* 获取数据表大小 */
INT XJP_DS_Get_Table_Size( INT TabIndex );
/* 获取数据表的信息 */
INT XJP_DS_Get_Table_Inf( UNSIGNED Number, UNSIGNED StartNumber,
    UNSIGNED_CHAR * pDst, UNSIGNED * Length );
/* 获取字段的信息 */
INT XJP_DS_Get_Field_Inf( UNSIGNED Number, UNSIGNED StartNumber,
    UNSIGNED TabIndex, UNSIGNED_CHAR * pDst, UNSIGNED * Length,
    UNSIGNED * ActualNumber );
/* 获取数据表的状态 */
INT XJP_DS_Get_Table_Status( INT TabIndex, UNSIGNED_CHAR * pDst );
/* 获取配置表的CRC码 */
UNSIGNED XJP_DS_Get_Config_CRC( VOID );
/* 获取数据表字段信息的CRC码 */
UNSIGNED XJP_DS_Get_Tab_Field_CRC( VOID );
/* 获取指定数据表/指定条件/指定序号的数据模板地址 */
VOID * XJP_DS_Get_Table_Memory_Index( INT TableIndex, TB_CFB * Handle, INT iIndex );
/* 获取数据表的长度 */
INT XJP_DS_Get_Table_Length( INT TabIndex, UNSIGNED * Length );
/* 获取特定字段的长度 */
INT XJP_DS_Get_Field_Length( INT TabIndex, INT HandleIndex, UNSIGNED * Length );
/* 根据表的索引号获取字段中到指定条件数据之间的实际元素个数 */
INT XJP_DS_Get_Data_Number( INT TabIndex, TB_CFB * Handle, UNSIGNED * Number );
/* 接口中直接调用数据表修改函数 */
INT XJP_DS_Com_Hmi_Modify( INT TableIndex, INT StartNumber, INT number,
    VOID * pCondBuf, INT iHandleNumber, VOID * pDataBuf );
/* 更新定值组值 */
INT XJP_DS_Update_SGValue( INT TableIndex, UNSIGNED TimeOut );
/* 计算RAM整定值字段的CRC码 */
INT XJP_DS_R_SGValueCRC( INT TableIndex, UNSIGNED * CRC_Code );
/* 计算非易失整定值字段的CRC码 */
INT XJP_DS_F_SGValueCRC( INT TableIndex, UNSIGNED TimeOut, UNSIGNED * CRC_Code );
/* 整定值数据检测 */
INT XJP_DS_SGValueCheck( INT TableIndex, UNSIGNED TimeOut );
/* 获取整定值修改计数器值 */
INT XJP_DS_GetSGValueCounter( INT TableIndex, UNSIGNED * Counter );
/* 注册非易失整定值处理函数 */
INT XJP_DS_Register_NVData_Treatment( UNSIGNED SetHandle, CHAR * dirName, CHAR * fileName,
    INT (* writeNVDataTreatment )( VOID *, UNSIGNED, UNSIGNED, UNSIGNED ),
    INT (* readNVDataTreatment )( VOID *, UNSIGNED, UNSIGNED, UNSIGNED ),
    INT (* repairNVDataTreatment )( UNSIGNED ) );
/* 注册配置信息处理函数 */
INT XJP_DS_Register_Config_Treatment( UNSIGNED CfgHandle, CHAR * dirName, CHAR * fileName,
    INT (* writeConfigDataTreatment )( VOID *, UNSIGNED, UNSIGNED, UNSIGNED ),
    INT (* readConfigDataTreatment )( VOID *, UNSIGNED, UNSIGNED, UNSIGNED ),
    INT (* repairConfigDataTreatment )( UNSIGNED ));
/* 获取数据表指针 */
UNSIGNED XJP_DS_GetTablePointers( HTAB ** PointerList, UNSIGNED MaxPointers );

/* Define time management functions.  */
/* 设置对时选项 */
INT XJP_TM_Set_Time_Para( INT INT_Level, INT(* GetGPSPulse )( VOID ),
    INT(* SaveTime )( TIME * pTime ),
    VOID( * regPlusInterrupt )( VOID (* new_lisr )( INT ), INT type ),
    INT(* GetClockPrecision )( UNSIGNED * millisecond, UNSIGNED * nanosecond ),
    INT(* GetClock64Precision )( UNSIGNED * millisecond, UNSIGNED * Microsecond ),
    INT(* GetIRIGBStatus )( UNSIGNED * ) );
/* 更新运行时间 */
INT XJP_TM_Updata_Time( UNSIGNED ClockTick );
/* 读取系统时间 */
INT XJP_TM_Get_Time_Dev( TIME * pTime );
/* 设置系统时间 */
INT XJP_TM_Set_Time_Dev_Zone( TIME * pTime , INT iTimeZone );
/* 设置时间的基本函数,不带时区 */
INT XJP_TM_Set_Time_Dev_Local( TIME * pTime );
/* 运行系统时钟 */
INT XJP_TM_Run_Clock( UNSIGNED ClockTick );
/* 设置时间类型 */
INT XJP_TM_Set_Time_Type( UNSIGNED Type );
/* 获取时间类型 */
INT XJP_TM_Get_Time_Type( VOID );
/* 读取61850时间 */
INT XJP_TM_Get_Time_61850( TIME * pTime, UNSIGNED * UTCSecond,
    UNSIGNED * UTCNanoSecond, UNSIGNED * Quality );
/* 设置61850时间 */
INT XJP_TM_Set_Time_61850( UNSIGNED UTCSecond, UNSIGNED UTCNanoSecond, TIME * pTime );
/* 获取GPS开入量计数 */
UNSIGNED XJP_TM_Get_Pulse_Counter( VOID );
/* 获取61850时钟初始化标志 */
INT XJP_TM_Get_Init_Flag( VOID );
/* Gps脉冲中断 */
VOID XJP_TM_GPS_Interrupt( INT Handle );
/* 读取系统时间(毫秒1字、微秒1字) */
VOID XJP_TM_Get_Clock_Precision( UNSIGNED * Buffer );
/* 获取系统时钟 */
UNSIGNED XJP_TM_Get_Clock_Millisecond( VOID );
/* 保存时间函数 */
VOID XJP_TM_Save_Time( VOID );
/* 装置时间转换成UTC(格林尼治)时间函数 */
/* UTCSecond:秒 UTCMillionsecond:毫秒 pTime:装置时间 */
INT XJP_TM_Dev_To_MS_UTC( UNSIGNED * UTCSecond, UNSIGNED * UTCMillionsecond,
    TIME * pSrcTime );
/* UTC(格林尼治)时间转换成装置时间函数 */
INT XJP_TM_MS_UTC_To_Dev( UNSIGNED * UTCSecond, UNSIGNED * UTCMillionsecond,
    TIME * pDstTime );
/* 装置时间转换成UTC(格林尼治)时间函数 */
/* UTCSecond:秒 UTCNanoSecond:纳秒 pTime:装置时间 */
INT XJP_TM_Dev_To_NS_UTC( UNSIGNED * UTCSecond, UNSIGNED * UTCNanoSecond, TIME * pTime );
/* UTC(格林尼治)时间转换成装置时间函数 */
/* UTCSecond:秒 UTCNanoSecond:纳秒 pTime:装置时间 */
INT XJP_TM_NS_UTC_To_Dev( UNSIGNED UTCSecond, UNSIGNED UTCNanoSecond, TIME * pTime );
/* 装置时间转换成就地时间 */
INT XJP_TM_Trans_Dev_To_LT( UNSIGNED * Time1, UNSIGNED * Time2, TIME * pTime );
/* 就地时间转换成装置时间 */
INT XJP_TM_Trans_LT_To_Dev( UNSIGNED * Time1, UNSIGNED * Time2, TIME * pTime );
/* 设置要转换的时间模式,1为PROFIBUS时间,2为UTC时间,缺省为UTC时间  */
INT XJP_TM_Set_Time_Mode ( UNSIGNED TimeMode );
/* 获取装置时区 */
INT XJP_TM_Get_Time_Zone( INT * pTimeZone );
/* 检测时间的有效性,通过iChangeFlag来决定是否修改不正确的时间(修改时设置为时间的最大值) */
INT XJP_TM_Time_Check( TIME * pTime , INT iChangeFlag );
/* 读取系统时间指针 */
TIME * XJP_TM_Get_Time_Ptr( VOID );
/* 设置对时时间间隔 */
INT XJP_TM_SET_Time_Interval( UNSIGNED TimeInterval );
/* 设置对时时间品质因素 */
INT XJP_TM_SET_Time_Quality( UNSIGNED Quality, INT CondType );
/* 设置系统初始化标识 */
INT XJP_TM_Set_SysInitFlag( INT iSysInitFlag );

/* Define watchdog management functions.  */
/* 创建对象 */
HWDM XJP_Watchdog_Create( UNSIGNED SetTime, CHAR * Name, UNSIGNED MonitorCtrlHandle );
/* 复位监视对象 */
INT XJP_Watchdog_Reset( HWDM Handle );
/* 使能监视对象 */
INT XJP_Watchdog_Control( HWDM Handle, INT Enable );
/* 看门狗定时器处理 */
CHAR * XJP_Watchdog_Timer( UNSIGNED Time, UNSIGNED * pMonitorCtrlHandle );

/* Define elf management functions.  */
/* 计算ELF文件的CRC码 */
int XJP_ELF_Calculate_CRC16( char * filename, unsigned long int ini_crc,
    unsigned long int * crc_code );
/* 计算ELF文件的CRC码 */
int XJP_ELF2_Calculate_CRC16( char * filename, unsigned long int ini_crc,
    unsigned long int * crc_code, unsigned long int ini_crc1,
    unsigned long int * crc_code1 );        
/* 计算ELF文件程序CRC码 */
int XJP_ELF_Text_CRC16( char * filename, unsigned long int ini_crc,
    unsigned long int * crc_code );
/* 获取ELF文件程序地址 */
int XJP_ELF_Text_Addr( char * filename, int maxNumber, int * actualNumber,
    unsigned long int * text_addr, unsigned long int * text_size );
/* 计算程序文件(elf格式)的CRC码 */
INT XJP_ELF_File_CRC16(  char * filename, UNSIGNED * text_crc );

/* Define protect management functions.  */
/* 创建数据保护; 0创建失败,!0:创建的应用句柄 */
HPRM XJP_NU_Protect_Create( CHAR * Name );
/* 获取互斥量句柄,0:未找到互斥量 */
HPRM XJP_NU_Protect_Find( CHAR * Name );
/* 数据保护; 0:成功,0!:失败 */
INT  XJP_NU_Protect( HPRM Handle );
/* 释放数据保护; 0:成功,0!:失败*/
INT  XJP_NU_UnProtect( HPRM Handle );

/* Define timer management functions.  */
/* 创建定时器; 0创建失败,!0:创建的应用句柄*/
HTMM XJP_NU_Timer_Create( CHAR * name, VOID (* expiration_routine )( UNSIGNED ),
    UNSIGNED initial_time, UNSIGNED reschedule_time, OPTION enable );
/* 定时器使能控制; 0:成功,0!:失败 */
INT XJP_NU_Timer_Control( HTMM Handle, OPTION enable );

/* Define event group management functions.  */
/* 创建事件管理句柄; 0创建失败,!0:创建的应用句柄 */
HEVM XJP_NU_Event_Group_Create( VOID );
/* 设置事件位; 0:成功,0!:失败 */
INT XJP_NU_Event_Group_Set( HEVM Handle, UNSIGNED events, OPTION operation );
/* 获取事件位; 0:成功,0!:失败 */
INT XJP_NU_Event_Group_Retrieve( HEVM Handle, UNSIGNED requested_flags,
    OPTION operation, UNSIGNED * retrieved_flags, UNSIGNED suspend );

/* Define memory pool management functions.  */
/* 创建内存池管理句柄 */
HMGM XJP_NU_Pool_Create( CHAR * name, VOID * start_address,
    UNSIGNED pool_size, UNSIGNED min_allocation, OPTION suspend_type );
/* 删除内存池 */
INT  XJP_NU_Pool_Delete( HMGM Handle );
/* 申请内存 */
INT XJP_NU_Allocate_Memory( HMGM Handle, VOID ** return_pointer, UNSIGNED size,
    UNSIGNED suspend );
/* 释放内存 */
INT XJP_NU_Deallocate_Memory( VOID * memory_ptr );

/* 获取操作系统函数表 */
OS_FUN * XJP_OS_Get_FuncList( VOID );

/* 创建对象 */
HIOM XJP_IO_Create_Out( UNSIGNED GroupNumber );
/* 设置启动继电器 */
INT XJP_IO_Set_Start_Relay( HIOM Handle, UNSIGNED Flag );
/* 设置开出位 */
INT XJP_IO_Set_Output_Relay( HIOM Handle, UNSIGNED GroupNo, UNSIGNED Bits,
    INT Operation );
/* 获取装置启动继电器状态 */
INT XJP_IO_Get_Start_Relay( UNSIGNED * ActualStartRelay );
/* 获取装置开出继电器状态 */
INT XJP_IO_Get_Output_Relay( UNSIGNED GroupNo, UNSIGNED * ActualBits );
/* 设置启动继电器用户驱动函数 */
INT XJP_IO_Set_Start_Driver( VOID (* StartDriver )( UNSIGNED Flag ));
/* 设置开出继电器用户驱动函数 */
INT XJP_IO_Set_Output_Driver( VOID (* RelayDriver )( UNSIGNED GroupNo, UNSIGNED Bits ));
/* 获取保持继电器状态位 */
UNSIGNED XJP_IO_Get_Hold_Relay( UNSIGNED GroupNo );
/* 设置保持继电器状态位 */
INT XJP_IO_Set_Hold_Relay( UNSIGNED GroupNo, UNSIGNED Bits );
/* 清除保持继电器状态位 */
INT XJP_IO_Clear_Hold_Relay( VOID );
/* 设置开出继电器保持掩码位 */
INT XJP_IO_Set_Hold_Masks( UNSIGNED GroupNo , UNSIGNED Bits );

/* Define driver management functions. */
typedef void (* _xjp_appCallback )( void * arg, int status, void * bufp, unsigned size );
INT XJP_DRV_Initialize( INT number, VOID ** entry );
INT XJP_DRV_mdBindDevice( CHAR * name, INT devid );
VOID * XJP_DRV_gioCreate( CHAR * name, INT mode, INT * status, VOID * optArgs );
INT XJP_DRV_gioSubmit( VOID * gioChan, unsigned cmd, void * bufp, unsigned offset,
    unsigned size, unsigned logical_unit, _xjp_appCallback * appCallback );
INT XJP_DRV_gioControl( VOID * gioChan, unsigned cmd, void * args );

/* file management functions. */
/* 保存持久化数据 */
INT XJP_File_Save( CHAR * fileName, VOID * buf, UNSIGNED size,
    UNSIGNED counter, UNSIGNED offset );
/* 读持久化数据 */
INT XJP_File_Read( CHAR * fileName, VOID * buf, UNSIGNED size,
    UNSIGNED counter, UNSIGNED offset );
/* 计算文件的CRC码 */
INT XJP_CalculateFileCRC( CHAR * fileName, UNSIGNED * actCRCcode );
/* 创建数据队列管理函数 */
HQUEM XJP_CreateDataQueue( CHAR * name, UNSIGNED uBufSize, UNSIGNED uMaxMesLen, UNSIGNED uDataByte );
/* 向数据队列发送数据函数 */
INT  XJP_SendToDataQueue( CHAR * name, HQUEM Handle, VOID * message, UNSIGNED size,
    UNSIGNED uTimeOut, INT iFrontFlag );
/* 从数据队列获取数据函数 */
INT  XJP_ReceiveFromDataQueue( CHAR * name, HQUEM Handle, VOID * message, UNSIGNED maxsize,
    UNSIGNED * ActSize, UNSIGNED uTimeOut );
/* 获取数据队列信息 */
INT XJP_DataQueueInformation( CHAR * name, HQUEM Handle, UNSIGNED * msg_count,
    UNSIGNED * avail_mem, UNSIGNED * msg_size );
/* 删除指定数据队列 */
INT XJP_DeleteDataQueue( CHAR * name, HQUEM Handle );
/*=======================================================================*/

#ifdef __cplusplus
}
#endif

#endif
