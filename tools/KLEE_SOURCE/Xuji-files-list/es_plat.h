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

typedef long HAPP;                  /* Ӧ�ó���װ�ض����� */
#endif

#ifndef XJ_SCHDULE_MODULE
#define XJ_SCHDULE_MODULE
#define CAS_MAIN_PROTECT        1   /* ���������� */
#define CAS_BACKUP_PROTECT      2   /* �󱸱������� */
typedef long HASH;                  /* ��������������� */
#endif

#ifndef XJ_CDDC_MODULE
#define XJ_CDDC_MODULE
typedef long HDDM;                  /* �豸���������� */
#endif

#ifndef XJ_CDMC_MODULE
#define XJ_CDMC_MODULE
typedef long HDMM;                  /* ��̬�ڴ������ */
#endif

#ifndef XJ_CECC_MODULE
#define XJ_CECC_MODULE
#define CEC_FIXED_SIZE          1   /* �̶����ݳ��� */
#define CEC_VARIABLE_SIZE       2   /* �ɱ����ݳ��� */
typedef long HDCM;                  /* ѭ���¼���������� */
#endif

#ifndef XJ_CFGC_MODULE
#define XJ_CFGC_MODULE
typedef long HFGM;                  /* �������ݶ����� */
#endif

#ifndef XJ_CITC_MODULE
#define XJ_CITC_MODULE
typedef long HITM;                  /* �����Ŷ����� */
#endif

#ifndef XJ_CLEC_MODULE
#define XJ_CLEC_MODULE
#define CLE_LED_EXTINGUISH      0x1 /* װ�����е�Ϩ��״̬ */
#define CLE_LED_CORUSCATE       0x2 /* װ�����е���˸״̬ */
#define CLE_LED_LIGHT           0x4 /* װ�����еƳ���״̬ */

#define CLE_LED_START           0x1 /* ���е�����״̬ */
#define CLE_LED_ABNORMAL        0x2 /* ���е��쳣״̬ */
#define CLE_LED_NORMAL          0x4 /* ���е�����״̬ */

typedef long HLEM;                  /* ���еƶ����� */
#endif

#ifndef XJ_CNDC_MODULE
#define XJ_CNDC_MODULE
typedef long HNDM;                  /* ����ֵ������ */
#endif

#ifndef XJ_CNUC_MODULE
#define XJ_CNUC_MODULE
typedef long HPRM;                  /* ���ݱ��������� */
typedef long HTMM;                  /* ��ʱ����������� */
typedef long HEVM;                  /* �¼�λ��������� */
typedef long HMGM;                  /* �ڴ�ع�������� */
typedef long HQUEM;                 /* ��Ϣ���й�������� */
#endif

#ifndef XJ_CRCC_MODULE
#define XJ_CRCC_MODULE
/* crc���ݱ����Ͷ��� */
#define CRCC_CRC16_TABLE        0   /* 16λ���ݱ� */
#define CRCC_CRC32_TABLE        1   /* 32λ���ݱ� */
#endif

#ifndef XJ_TABLE_MODULE
#define XJ_TABLE_MODULE
#define TYPE_NAME               20  /* �����������Ƴ��� */
#define TABLE_NAME              12  /* �����Ƴ��� */
#define FIELD_NAME              12  /* �ֶ����Ƴ��� */
#define MAX_CONDITION           3   /* �����ֶε���������������� */
#define MAX_HANDLE              4   /* �������ֶθ��� */
/* �ֶ����Զ��� */
#define FIELD_CONFIG_ENABLE     1   /* �ֶ�����ѡ�� */
#define FIELD_SETTING_ENABLE    2   /* �ֶ�����ѡ�� */
#define FIELD_CRC_ENABLE        4   /* �ֶ�CRCѡ�� */
#define FIELD_VALUE_ENABLE      8   /* �ֶ���ֵ(��������ֵ)ѡ�� */
#define FIELD_CHECK_ENABLE      0x10 /* �ֶα߽���ѡ�� */
/* �������Ͷ��� */
#define TYPE_CHAR               1   /* char */
#define TYPE_UNSIGNED_CHAR      2   /* unsigned char */
#define TYPE_INT                3   /* int */
#define TYPE_SIGNED             4   /* long */
#define TYPE_UNSIGNED           5   /* unsigned long */
#define TYPE_FLOAT              6   /* float */
/* �޸�����ֵѡ�� */
#define SETTING_ALL_ENABLE      1   /* ȫ������ֵ */
#define SETTING_ONE_ENABLE      2   /* ��������ֵ */
/* �������� */
#define COND_TYPE_AND           1   /* ����� */
#define COND_TYPE_OR            2   /* ����� */
#define COND_TYPE_EQU           3   /* ���ڲ��� */
/* ���ݱ����� */
#define TABLE_TYPE_RELATION     (1 << 0) /* ˫CPU���ݱ����ݹ��� */

/* �����ֶβ�����ṹ���� */
typedef struct TB_CFB_STRUCT
{
    /* �����ֶ����� */
    INT HandleIndex;
    /* �����ֶθ��� */
    INT CondNumber;
    /* �����ֶ����� */
    INT CondIndex[MAX_CONDITION];
    /* �����ֶ�ֵ */
    INT CondValue[MAX_CONDITION];
    /* �������� */
    INT CondType[MAX_CONDITION];
}TB_CFB;

/* �����ֶβ�����ṹ���� */
typedef struct TB_CHB_STRUCT
{
    /* �����ֶθ��� */
    INT HandleNumber;
    /* �����ֶ����� */
    INT HandleIndex[MAX_HANDLE];
    /* �����ֶθ��� */
    INT CondNumber;
    /* �����ֶ����� */
    INT CondIndex[MAX_CONDITION];
    /* �����ֶ�ֵ */
    INT CondValue[MAX_CONDITION];
    /* �������� */
    INT CondType[MAX_CONDITION];
}TB_CHB;

typedef long HTAB;                  /* Ӧ�ó�������� */
#endif

#ifndef XJ_TIME_MODULE
#define XJ_TIME_MODULE
/* ��ʱʱ�����Ͷ��� */
#define CTM_TYPE_GPS_SECOND       (1 << 0) /* �������ʱ */
#define CTM_TYPE_GPS_MINUTE       (1 << 1) /* �������ʱ */
#define CTM_TYPE_GPS_IRIGB        (1 << 2) /* IRIG-B���ʱ */
#define CTM_TYPE_NETWORK          (1 << 3) /* �����ʱ */
#define CTM_TYPE_61850            (1 << 4) /* 61850��ʱ */
#define CTM_TYPE_1588             (1 << 5) /* 1588��ʱ */

/* ʱ��Ʒ�ʶ��� */
#define CTM_TIME_ACCURACY         0        /* װ��ʱ��׼ȷ��ѡ�� */
#define CTM_Q_LEAP_SECOND         (1 << 7) /* ���� */
#define CTM_Q_CLOCK_FAILURE       (1 << 6) /* ʱ�ӹ��� */
#define CTM_Q_CLOCK_NO_SYN        (1 << 5) /* ʱ��δͬ�� */
#define CTM_Q_TIME_ACCURACY(x)    (x << CTM_TIME_ACCURACY) /* ʱ��׼ȷ�� */

#define PROFIBUS_MODE             0x1  /* PROFIBUSʱ��ת����װ��ʱ�� */
#define UTC_MODE                  0x2  /* UTCʱ��ת����װ�� */

/* װ��ʱ��ṹ */
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

/* ���Ź�ģ�鶨�� */
#ifndef XJ_CWDC_MODULE
#define XJ_CWDC_MODULE
#define CWD_WATCHDOG_NORMAN     0   /* �������������� */
#define CWD_WATCHDOG_ERROR      1   /* �����������쳣 */
#define CWD_WATCHDOG_ENABLE     1   /* ʹ�ܼ������� */
#define CWD_WATCHDOG_DISABLE    0   /* ��ֹ�������� */

typedef long HWDM;                  /* ���Ź����Ӷ����� */
#endif

/* ����ģ�鶨�� */
#ifndef XJ_CIOC_MODULE
#define XJ_CIOC_MODULE
#define CIO_OR                  1 /* �� */
#define CIO_AND                 2 /* �� */
#define CIO_EQU                 4 /* ���� */

typedef long HIOM; /* Ӧ�ó�������� */
#endif

#ifndef XJ_CSFC_MODULE
#define XJ_CSFC_MODULE
/* ���ӽڵ㶨�� */
typedef struct CSF_NODE_STRUCT
{
    struct CSF_NODE_STRUCT
        * previous; /* previous node */
    struct CSF_NODE_STRUCT
        * next; /* next node */
}CSF_NODE;
#endif

////typedef long HMUM; /* ������������ */
typedef VOID * HMUM; /* ������������ */
/*=======================================================================*/
/* Define service completion status constants.  */
#define XJP_LOAD_SUCCESS            0 /* װ��Ӧ�ó���ɹ� */
#define XJP_NO_APPLICATION          1 /* δ�ҵ�Ӧ�ó��� */
#define XJP_VERSION_ERROR           2 /* Ӧ�ó���汾���� */
#define XJP_LOAD_ERROR              3 /* װ��Ӧ�ó���ʧ�� */
#define XJP_ARRAGE_FULL             1 /* ���������� */
#define XJP_ARRAGE_SAME             2 /* ����������ͬ */
#define XJP_WATCHDOG_NORMAN         0 /* �������������� */
#define XJP_WATCHDOG_ERROR          1 /* �����������쳣 */

/* Define constants for use in service parameters.  */
#define XJP_MAIN_PROTECT            1 /* ���������� */
#define XJP_BACKUP_PROTECT          2 /* �󱸱������� */
#define XJP_EVENT_FIXED_SIZE        1 /* �̶����ݳ��� */
#define XJP_EVENT_VARIABLE_SIZE     2 /* �ɱ����ݳ��� */
#define XJP_LED_EXTINGUISH        0x1 /* װ�����е�Ϩ��״̬ */
#define XJP_LED_CORUSCATE         0x2 /* װ�����е���˸״̬ */
#define XJP_LED_LIGHT             0x4 /* װ�����еƳ���״̬ */
#define XJP_LED_START             0x1 /* ���е�����״̬ */
#define XJP_LED_ABNORMAL          0x2 /* ���е��쳣״̬ */
#define XJP_LED_NORMAL            0x4 /* ���е�����״̬ */
#define XJP_CRC16_TABLE             0 /* 16λ���ݱ� */
#define XJP_CRC32_TABLE             1 /* 32λ���ݱ� */
#define XJP_SETTING_ALL_ENABLE      1 /* ȫ������ֵ */
#define XJP_SETTING_ONE_ENABLE      2 /* ��������ֵ */
#define XJP_WATCHDOG_ENABLE         1 /* ʹ�ܼ������� */
#define XJP_WATCHDOG_DISABLE        0 /* ��ֹ�������� */

/* Define constants for use in struct.  */
#define XJP_MAX_CONDITION           3 /* �����ֶε���������������� */
#define XJP_MAX_HANDLE              4 /* �������ֶθ��� */
#define XJP_START_RELAY_FLAG  0x55AAAA55 /* �����̵���������־ */

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
/* �������ض��� */
HAPP XJP_Create_LoadObj( UNSIGNED StartAddr, UNSIGNED Length, UNSIGNED Key1,
    UNSIGNED Key2, VOID * PlatFuntion, UNSIGNED PlatFuncSize,
    CHAR * FileName, CHAR * SectName, CHAR * KeyName, CHAR * EntryName );
/* ����Ӧ�ó��� */
INT XJP_Link_Application( HAPP Handle, VOID(* WatchDogReset )( VOID ),
    INT(* CalCRCCode )( const VOID *, UNSIGNED, INT, INT, UNSIGNED, UNSIGNED *),
    APP_ENTRY ** AppFunctionList );

/* Define task management functions.  */
/* ����Ӧ�ó��� */
HASH XJP_Create_AppRoutine( INT TaskType, UNSIGNED ProtectId, UNSIGNED SectorId,
    VOID (* application_routine )( UNSIGNED, UNSIGNED, INT ));
/* ���������������Ƿ���ͣ���ߺ��� */
INT XJP_SetLockMainTaskSleepFlag( INT iFlag );
/* ɾ��Ӧ�ó���,ֻ�������к�����ִ�� */
INT XJP_Delete_AppRoutine( HASH Handle );
/* ��������ʱ��Ƭ */
INT XJP_Change_Time_Slice( INT TaskType, UNSIGNED TimeSlice );
/* �޸�Ӧ����������ʱ��Ƭ */
INT XJP_Change_Sleep_Ticks(  INT TaskType, UNSIGNED TimeSlice );
/* ����Ӧ������ */
VOID XJP_Execute_App_Routine( INT TaskType );
/* ��ȡ������ִ��ʱ�� */
INT XJP_Get_Main_Run_Time( UNSIGNED * RunTime_ptr, UNSIGNED * SleepTime_ptr );
/* ����Ӧ������ */
VOID XJP_Reset_App_Routine( VOID );
/* ע��������� */
VOID XJP_Register_Backup_Task( VOID * Task );
/* �ָ������� */
INT XJP_ResumeMainTask( UNSIGNED Time );

/* Define driver management functions.  */
/* �������� */
HDDM XJP_Create_Driver( VOID (* driver_entry )( XJ_DRIVER , XJ_DRIVER_REQUEST *) );
/* ��ʼ���豸�������� */
INT XJP_Driver_Initialize( HDDM Handle, UNSIGNED uId, UNSIGNED uTimeOut );
/* ���������� */
INT XJP_Driver_Read( HDDM Handle, UNSIGNED uId, VOID * Buffer,
    UNSIGNED uOffset, UNSIGNED uSize, UNSIGNED uTimeOut );
/* д�������� */
INT XJP_Driver_Write( HDDM Handle, UNSIGNED uId, VOID * Buffer,
    UNSIGNED uOffset, UNSIGNED uSize, UNSIGNED uTimeOut );
/* ��ȡ�ź��� */
INT XJP_Driver_Assign( HDDM Handle, UNSIGNED uTimeOut );
/* �ͷ��ź��� */
INT XJP_Driver_Release( HDDM Handle );
/* �����ļ� */
INT XJP_Copy_NVdata( CHAR * fileNameSrc, CHAR * fileNameDst );

/* Define nvdata management functions.  */
/* ��������ֵ������������� */
HNDM XJP_NVData_Create( CHAR * dirName, CHAR * fileName, INT Times );
/* ������ֵ���� */
INT XJP_NVData_Read( HNDM Handle, VOID * Buffer, UNSIGNED uOffset,
    UNSIGNED uSize, UNSIGNED uTimeOut );
/* д����ֵ���� */
INT XJP_NVData_Write( HNDM Handle, VOID * Buffer, UNSIGNED uOffset,
    UNSIGNED uSize, UNSIGNED uTimeOut );
/* �޸�����ֵ���� */
INT XJP_NVData_Repair( HNDM Handle, UNSIGNED uTimeOut );

/* Define config management functions.  */
/* �����������ݶ����� */
HFGM XJP_Config_Create( CHAR * dirName, CHAR * fileName );
/* ����������� */
INT XJP_Config_Initialize( HFGM Handle, UNSIGNED uTimeOut );
/* ���������� */
INT XJP_Config_Read( HFGM Handle, VOID * Buffer, UNSIGNED uOffset,
    UNSIGNED uSize, UNSIGNED uTimeOut );
/* д�������� */
INT XJP_Config_Write( HFGM Handle, VOID * Buffer, UNSIGNED uOffset,
    UNSIGNED uSize, UNSIGNED uTimeOut );

/* Define background management functions.  */
/* ��ʼ���������ݹ������� */
INT XJP_Background_Initialize( CHAR * fullFileName );
/* ���������� */
INT XJP_Background_Read( VOID * Buffer, UNSIGNED uOffset,
    UNSIGNED uSize, UNSIGNED uTimeOut );
/* д�������� */
INT XJP_Background_Write( VOID * Buffer, UNSIGNED uOffset,
    UNSIGNED uSize, UNSIGNED uTimeOut );
/* Define output hold management functions.  */
/* ��ʼ�����ڱ���λ���ݹ������� ByteNumber:32λ�������ݵ���Ч�ֽ��� */
INT XJP_Output_Hold_Initialize( CHAR * dirName );
/* �����ڱ���λ���� */
INT XJP_Output_Hold_Read( VOID * Buffer, UNSIGNED uOffset,
    UNSIGNED uSize, UNSIGNED uTimeOut );
/* д���ڱ���λ���� */
INT XJP_Output_Hold_Write( VOID * Buffer, UNSIGNED uOffset,
    UNSIGNED uSize, UNSIGNED uTimeOut );

/* Define dynamic memory pool management functions.  */
/* �������� */
HDMM XJP_Memory_Pool_Create( unsigned int PoolSize );
/* �����ڴ� */
INT XJP_Memory_Allocate( HDMM Handle, VOID ** return_pointer, UNSIGNED size );
/* �ͷ��ڴ�*/
INT XJP_Memory_Deallocate( HDMM Handle, VOID * memory );

/* Define queue management functions.  */
/* �������� */
HDCM XJP_Queue_Create( UNSIGNED BufSize, INT MessageType, UNSIGNED MessageSize );
/* ������� */
INT XJP_Queue_Clear( HDCM Handle );
/* ������� */
INT XJP_Queue_Find( HDCM Handle , UNSIGNED * Actual_Size );
/* ������ */
INT XJP_Queue_Read( HDCM Handle , VOID * pBuffer );
/* д���� */
INT XJP_Queue_Write( HDCM Handle , VOID * pBuffer );

/* Define function table management functions.  */
/* ��ȡ������ָ��,NU_NULL:δ�ҵ������� */
VOID * XJP_Function_Table_Retrieve( CHAR * Name );
/* ע�ắ���� */
INT XJP_Function_Table_Register( CHAR * Name, VOID * FunTablePtr );

/* Define index table management functions.  */
/* �������� */
HITM XJP_Index_Create( INT MaxNumber );
/* д���� overData:������� overFlag:�������(��)��־ */
INT XJP_Index_Write( HITM Handle, UNSIGNED Index, UNSIGNED * overData,
    INT * overFlag );
/* ɾ������ */
INT XJP_Index_Delete( HITM Handle, UNSIGNED Index );
/* ������� */
INT XJP_Index_Clear( HITM Handle );
/* ��ȡָ�����ݸ���������(�����¿�ʼ) */
INT XJP_Index_List( HITM Handle, UNSIGNED * Dst, UNSIGNED * Actual_Number,
    UNSIGNED Number );
/* ��ȡָ�����ݸ����ĵ�16λ����(������������) */
INT XJP_Index_ListL( HITM Handle, UNSIGNED * Dst, UNSIGNED * Actual_Number,
    UNSIGNED Number );
/* ��ȡָ�����ݸ����ĸ�16λ����(�����¿�ʼ) */
INT XJP_Index_ListH( HITM Handle, UNSIGNED * Dst, UNSIGNED * Actual_Number,
    UNSIGNED Number );
/* ��ȡ�����16λ����������32λ����(�����¿�ʼ) */
INT XJP_Index_Read( HITM Handle, UNSIGNED * Dst, UNSIGNED * Actual_Number,
    UNSIGNED Number, UNSIGNED CondMasks, UNSIGNED CondValue );

/* Define run led management functions.  */
/* �������� */
HLEM XJP_Led_Create( VOID );
/* �������е�״̬ */
INT XJP_Led_Change_Status( HLEM Handle, UNSIGNED LedStatus );
/* ����װ�����е���˸Ƶ�� */
INT XJP_Led_Change_Frequency( UNSIGNED Frequency );
/* ����װ�����е����� */
INT XJP_Led_Change_Type( UNSIGNED NormalType, UNSIGNED StartType, UNSIGNED AbnormalType );
/* װ�����еƶ�ʱ������ */
UNSIGNED XJP_Led_Timer( UNSIGNED Time );
/* ����װ�ú������е�״̬ */
INT XJP_Led_SetDevLedCallStatus( INT AutoCallEnable, UNSIGNED LedCallDelay,
    UNSIGNED LedCallStatus, UNSIGNED Frequency );
/* װ�����еƺ��й���(����ֵ��0-�޺��У�!0-ʵ�ʺ��е�����״̬) */
UNSIGNED XJP_Led_DevLedCallTimer( UNSIGNED Time );

/* Define debug management functions.  */
/* ���������ڴ��� */
UNSIGNED XJP_Dbg_Memory_Create( VOID * mem_ptr, UNSIGNED mem_size,
    CHAR * mem_name, UNSIGNED value );
/* �ڴ���� */
INT XJP_Dbg_Memory_Check( UNSIGNED MemHandle );
/* ����ִ��ʱ�亯�� */
UNSIGNED XJP_Dbg_Time_Testing( UNSIGNED * millisecond_ptr, UNSIGNED * microsecond_ptr );
/* ע����Ժ����� */
INT XJP_Dbg_Register_FuncList( VOID );

/* Define mutex management functions.  */
/* ��ʼ�������� */
INT XJP_Mutex_Initialize( VOID );
/* �������������� */
HMUM XJP_Mutex_Create( CHAR * name, UNSIGNED initial_count, OPTION suspend_type );
/* ��ȡ������ */
INT XJP_Mutex_Obtain( HMUM Handle, UNSIGNED Timeout );
/* �ͷŻ����� */
INT XJP_Mutex_Release( HMUM Handle );

/* Define overtime management functions.  */
/* ��ʼ����ʱ������� */
INT XJP_Overtime_Initialize( INT MaxNumber, VOID (* _lock )( VOID ),
    VOID (* _unlock )( VOID ) );
/* ��ʱ�����ѭ������ */
INT XJP_OverTime_Main( UNSIGNED ClockTick );
/* ��ӳ�ʱ������ݺ��� */
INT XJP_OverTime_Place_On( VOID * memory, UNSIGNED overTime,
    VOID * callBackArgs, void (* _callBack )( void * arg ));
/* ɾ����ʱ������ݺ��� */
INT XJP_OverTime_Remove( VOID * memory );

/* Define CRC management functions.  */
INT XJP_CRC_Calculate( const VOID * Src, UNSIGNED Size, INT ByteNumber,
    INT CrcBits, UNSIGNED InitialValue, UNSIGNED * CRC_Code );
/* ��ȡcrc���ݱ� */
VOID * XJP_CRC_Get_Table( INT iType );

/* Define common services management functions.  */
/* �������ڵ� */
VOID XJP_CSC_Place_On( CSF_NODE ** head, CSF_NODE * new_node );
/* ɾ������ڵ� */
VOID XJP_CSC_Remove( CSF_NODE ** head, CSF_NODE * node );
/* ��WORD����ת��Ϊ�ַ��� */
char * XJP_CSC_Word_To_String( void * s1, const void * s2, register unsigned int n );
/* ���ַ���ת��ΪWORD���� */
char * XJP_CSC_String_To_Word( void * s1, const void * s2, register unsigned int n );
/* ��WORD����ת�����ֽ����� */
char * XJP_CSC_Word_To_Byte( void * s1, const void * s2, register unsigned int n );
/* ���ֽ�����ת����WORD���� */
char * XJP_CSC_Byte_To_Word( void * s1, const void * s2, register unsigned int n );
/* �ַ����Ƚ� */
char * XJP_CSC_Strcmp( register const char * s1, register const char * s2 );
/* ѭ����������(ascending-1:���� maxNumber:������ݸ���:>=2 ����λ��:16λ) */
VOID XJP_CSC_Arrage_Insert( UNSIGNED * buff, UNSIGNED * actNumber, UNSIGNED maxNumber,
    UNSIGNED actData, INT ascending );
/* ѭ���������򲢷��س������޵���Ч����(ascending-1:���� maxNumber:������ݸ���:>=2) */
/* NU_TRUE:������, NU_FALSE:����δ�� */
INT XJP_CSC_Arrage_Insert_Full( UNSIGNED * buff, UNSIGNED * actNumber,
    UNSIGNED maxNumber, UNSIGNED actData, UNSIGNED * InvalidData, INT ascending );
/* ��ȡ��Ч���ݸ���(ascending-1:���� ),�Ҳ�����ֹ����ʱΪȫ��Ч */
UNSIGNED XJP_CSC_Get_Valid_Number( VOID * pBuffer, UNSIGNED Number, UNSIGNED endIndex,
    INT ascending, UNSIGNED uMaskBits );

/* Define common table management functions.  */
/* �������ݱ� */
INT XJP_DS_Define_Table( CHAR * Name, INT Index, INT MaxNumber, INT Size, INT Type,
    INT EditFieldSize );
/* �����ֶ� */
INT XJP_DS_Define_Field( INT TableIndex, CHAR * Name, INT Index, INT Attribute,
    INT DataType, INT Number, INT Offset );
/* ���ɱ����� */
VOID * XJP_DS_Create_All_Field( INT TableIndex, VOID * Buffer );
/* ��ȡ�����ֶε�ֵ */
INT XJP_DS_Get_Field_Value_One( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * Buffer, INT * ActualNumber );
/* ������ֶ�ֵ */
INT XJP_DS_Get_Field_Value_Batch( INT TableIndex, INT Offset, INT Number,
    TB_CHB * Handle, VOID * Buffer, INT * ActualNumber );
/* ��ȡȫ���ֶε�ֵ */
INT XJP_DS_Get_Field_Value_All( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * Buffer, INT * ActualNumber );
/* ��������ֵ��Χ */
INT XJP_DS_Define_SG_Range( INT TableIndex, INT TypeIndex, INT CoefIndex,
    INT MinIndex, INT MaxIndex );
/* ���������ֶ�ֵ */
INT XJP_DS_Set_Field_Value_Config( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * buffer, INT Originate );
/* ѡ��༭��ֵ�� */
INT XJP_DS_Select_Edit_Field_SG( INT TableIndex, INT FieldIndex );
/* ���ö�ֵ��ֵ */
INT XJP_DS_Set_Field_Value_SG( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * buffer );
/* ȷ�ϱ༭��ֵ��ֵ */
INT XJP_DS_Confirm_Field_Value_SG( INT TableIndex, INT Type, UNSIGNED Offset,
    TB_CFB * pHandle, UNSIGNED TimeOut, INT ( _AckCall )( INT, UNSIGNED_CHAR *),
    UNSIGNED_CHAR * Dst );
/* ��ʼ�����ݱ������� */
INT XJP_DS_Initialize( INT MaxTabNumber, INT MaxFieldNumber );
/* �������ݱ������ */
INT XJP_DS_Define_Protect_Handle( INT TableIndex, VOID * pProtectHandle );
/* �������ݱ������ */
INT XJP_DS_Freeze_Table( INT TableIndex );
/* �ⶳ���ݱ������ */
INT XJP_DS_UnFreeze_Table( INT TableIndex );
/* ��ȡ���ݱ�Ķ���״̬ */
INT XJP_DS_Freeze_Status( INT TableIndex );
/* ��ʼ�����ݱ�����ݶ���״̬ */
INT XJP_DS_Freeze_Initialize( INT TableIndex );
/* ���������ֶ�ֵ */
INT XJP_DS_Save_Field_Value_Config( HTAB * pTabList, UNSIGNED TimeOut );
/* ����ȫ�������ֶ�ֵ */
INT XJP_DS_Lead_Field_Value_Config( UNSIGNED TimeOut );
/* �����������ֶ�ֵ */
INT XJP_DS_Save_Tab_Field_Value_Config( INT TableIndex, UNSIGNED TimeOut );
/* �޸��������� */
INT XJP_DS_ConfigData_Repair( UNSIGNED TimeOut );
/* ����ȫ����ֵ��ֵ */
INT XJP_DS_Save_Field_Value_SG( UNSIGNED TimeOut );
/* ����ȫ����ֵ��ֵ */
INT XJP_DS_Lead_Field_Value_SG( UNSIGNED TimeOut );
/* ���ݱ�������ݳ�ʼ�� */
INT XJP_DS_Initialize_SetData( INT iType, UNSIGNED uTimeOut );
/* �̻�����ʧ�������� */
INT XJP_DS_NVData_Write( VOID * Buffer, UNSIGNED uOffset, UNSIGNED uSize,
    UNSIGNED uTimeOut );
/* ��ȡ����ʧ�������� */
INT XJP_DS_NVData_Read( VOID * Buffer, UNSIGNED uOffset, UNSIGNED uSize,
    UNSIGNED uTimeOut );
/* �޸�����ʧ�������� */
INT XJP_DS_NVData_Repair( UNSIGNED TimeOut );
/* �̻�������Ϣ */
INT XJP_DS_ConfigData_Write( VOID * Buffer, UNSIGNED uOffset,
    UNSIGNED uSize, UNSIGNED uTimeOut );
/* ��ȡ������Ϣ */
INT XJP_DS_ConfigData_Read( VOID * Buffer, UNSIGNED uOffset,
    UNSIGNED uSize, UNSIGNED uTimeOut );
/* ע���ֶ�Ԥ������ */
INT XJP_DS_Register_Pre_Treatment( INT TableIndex, INT FieldIndex,
    INT (* preTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr,
    VOID * valuePtr, INT Originate ) );
/* ע���ֶ�ǰ������ */
INT XJP_DS_Register_Fore_Treatment( INT TableIndex, INT FieldIndex,
    INT (* foreTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr,
    VOID * valuePtr ) );
/* ע���ֶκ����� */
INT XJP_DS_Register_After_Treatment( INT TableIndex, INT FieldIndex,
    INT (* afterTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, INT Originate ) );
/* ע���ֶν��������� */
INT XJP_DS_Register_End_Treatment( INT TableIndex, INT FieldIndex,
    INT (* endTreatment )( INT TabIndex, INT FieldIndex ) );
/* ע����������ֵǰ������ */
INT XJP_DS_Register_Lead_Fore_Treatment( INT TableIndex,
    INT (* foreLeadTreatment )( INT TabIndex ) );
/* ע����������ֵ������ */
INT XJP_DS_Register_Lead_After_Treatment( INT TableIndex,
    INT (* afterLeadTreatment )( INT TabIndex ) );
/* ��ȡ���ݱ���ܸ��� */
INT XJP_DS_Get_Table_Number( VOID );
/* ��ȡ���ݱ��ڴ��ַ */
VOID * XJP_DS_Get_Table_Memory( INT TabIndex, INT * ActualNum );
/* ��ȡ���ݱ��С */
INT XJP_DS_Get_Table_Size( INT TabIndex );
/* ��ȡ���ݱ����Ϣ */
INT XJP_DS_Get_Table_Inf( UNSIGNED Number, UNSIGNED StartNumber,
    UNSIGNED_CHAR * pDst, UNSIGNED * Length );
/* ��ȡ�ֶε���Ϣ */
INT XJP_DS_Get_Field_Inf( UNSIGNED Number, UNSIGNED StartNumber,
    UNSIGNED TabIndex, UNSIGNED_CHAR * pDst, UNSIGNED * Length,
    UNSIGNED * ActualNumber );
/* ��ȡ���ݱ��״̬ */
INT XJP_DS_Get_Table_Status( INT TabIndex, UNSIGNED_CHAR * pDst );
/* ��ȡ���ñ��CRC�� */
UNSIGNED XJP_DS_Get_Config_CRC( VOID );
/* ��ȡ���ݱ��ֶ���Ϣ��CRC�� */
UNSIGNED XJP_DS_Get_Tab_Field_CRC( VOID );
/* ��ȡָ�����ݱ�/ָ������/ָ����ŵ�����ģ���ַ */
VOID * XJP_DS_Get_Table_Memory_Index( INT TableIndex, TB_CFB * Handle, INT iIndex );
/* ��ȡ���ݱ�ĳ��� */
INT XJP_DS_Get_Table_Length( INT TabIndex, UNSIGNED * Length );
/* ��ȡ�ض��ֶεĳ��� */
INT XJP_DS_Get_Field_Length( INT TabIndex, INT HandleIndex, UNSIGNED * Length );
/* ���ݱ�������Ż�ȡ�ֶ��е�ָ����������֮���ʵ��Ԫ�ظ��� */
INT XJP_DS_Get_Data_Number( INT TabIndex, TB_CFB * Handle, UNSIGNED * Number );
/* �ӿ���ֱ�ӵ������ݱ��޸ĺ��� */
INT XJP_DS_Com_Hmi_Modify( INT TableIndex, INT StartNumber, INT number,
    VOID * pCondBuf, INT iHandleNumber, VOID * pDataBuf );
/* ���¶�ֵ��ֵ */
INT XJP_DS_Update_SGValue( INT TableIndex, UNSIGNED TimeOut );
/* ����RAM����ֵ�ֶε�CRC�� */
INT XJP_DS_R_SGValueCRC( INT TableIndex, UNSIGNED * CRC_Code );
/* �������ʧ����ֵ�ֶε�CRC�� */
INT XJP_DS_F_SGValueCRC( INT TableIndex, UNSIGNED TimeOut, UNSIGNED * CRC_Code );
/* ����ֵ���ݼ�� */
INT XJP_DS_SGValueCheck( INT TableIndex, UNSIGNED TimeOut );
/* ��ȡ����ֵ�޸ļ�����ֵ */
INT XJP_DS_GetSGValueCounter( INT TableIndex, UNSIGNED * Counter );
/* ע�����ʧ����ֵ������ */
INT XJP_DS_Register_NVData_Treatment( UNSIGNED SetHandle, CHAR * dirName, CHAR * fileName,
    INT (* writeNVDataTreatment )( VOID *, UNSIGNED, UNSIGNED, UNSIGNED ),
    INT (* readNVDataTreatment )( VOID *, UNSIGNED, UNSIGNED, UNSIGNED ),
    INT (* repairNVDataTreatment )( UNSIGNED ) );
/* ע��������Ϣ������ */
INT XJP_DS_Register_Config_Treatment( UNSIGNED CfgHandle, CHAR * dirName, CHAR * fileName,
    INT (* writeConfigDataTreatment )( VOID *, UNSIGNED, UNSIGNED, UNSIGNED ),
    INT (* readConfigDataTreatment )( VOID *, UNSIGNED, UNSIGNED, UNSIGNED ),
    INT (* repairConfigDataTreatment )( UNSIGNED ));
/* ��ȡ���ݱ�ָ�� */
UNSIGNED XJP_DS_GetTablePointers( HTAB ** PointerList, UNSIGNED MaxPointers );

/* Define time management functions.  */
/* ���ö�ʱѡ�� */
INT XJP_TM_Set_Time_Para( INT INT_Level, INT(* GetGPSPulse )( VOID ),
    INT(* SaveTime )( TIME * pTime ),
    VOID( * regPlusInterrupt )( VOID (* new_lisr )( INT ), INT type ),
    INT(* GetClockPrecision )( UNSIGNED * millisecond, UNSIGNED * nanosecond ),
    INT(* GetClock64Precision )( UNSIGNED * millisecond, UNSIGNED * Microsecond ),
    INT(* GetIRIGBStatus )( UNSIGNED * ) );
/* ��������ʱ�� */
INT XJP_TM_Updata_Time( UNSIGNED ClockTick );
/* ��ȡϵͳʱ�� */
INT XJP_TM_Get_Time_Dev( TIME * pTime );
/* ����ϵͳʱ�� */
INT XJP_TM_Set_Time_Dev_Zone( TIME * pTime , INT iTimeZone );
/* ����ʱ��Ļ�������,����ʱ�� */
INT XJP_TM_Set_Time_Dev_Local( TIME * pTime );
/* ����ϵͳʱ�� */
INT XJP_TM_Run_Clock( UNSIGNED ClockTick );
/* ����ʱ������ */
INT XJP_TM_Set_Time_Type( UNSIGNED Type );
/* ��ȡʱ������ */
INT XJP_TM_Get_Time_Type( VOID );
/* ��ȡ61850ʱ�� */
INT XJP_TM_Get_Time_61850( TIME * pTime, UNSIGNED * UTCSecond,
    UNSIGNED * UTCNanoSecond, UNSIGNED * Quality );
/* ����61850ʱ�� */
INT XJP_TM_Set_Time_61850( UNSIGNED UTCSecond, UNSIGNED UTCNanoSecond, TIME * pTime );
/* ��ȡGPS���������� */
UNSIGNED XJP_TM_Get_Pulse_Counter( VOID );
/* ��ȡ61850ʱ�ӳ�ʼ����־ */
INT XJP_TM_Get_Init_Flag( VOID );
/* Gps�����ж� */
VOID XJP_TM_GPS_Interrupt( INT Handle );
/* ��ȡϵͳʱ��(����1�֡�΢��1��) */
VOID XJP_TM_Get_Clock_Precision( UNSIGNED * Buffer );
/* ��ȡϵͳʱ�� */
UNSIGNED XJP_TM_Get_Clock_Millisecond( VOID );
/* ����ʱ�亯�� */
VOID XJP_TM_Save_Time( VOID );
/* װ��ʱ��ת����UTC(��������)ʱ�亯�� */
/* UTCSecond:�� UTCMillionsecond:���� pTime:װ��ʱ�� */
INT XJP_TM_Dev_To_MS_UTC( UNSIGNED * UTCSecond, UNSIGNED * UTCMillionsecond,
    TIME * pSrcTime );
/* UTC(��������)ʱ��ת����װ��ʱ�亯�� */
INT XJP_TM_MS_UTC_To_Dev( UNSIGNED * UTCSecond, UNSIGNED * UTCMillionsecond,
    TIME * pDstTime );
/* װ��ʱ��ת����UTC(��������)ʱ�亯�� */
/* UTCSecond:�� UTCNanoSecond:���� pTime:װ��ʱ�� */
INT XJP_TM_Dev_To_NS_UTC( UNSIGNED * UTCSecond, UNSIGNED * UTCNanoSecond, TIME * pTime );
/* UTC(��������)ʱ��ת����װ��ʱ�亯�� */
/* UTCSecond:�� UTCNanoSecond:���� pTime:װ��ʱ�� */
INT XJP_TM_NS_UTC_To_Dev( UNSIGNED UTCSecond, UNSIGNED UTCNanoSecond, TIME * pTime );
/* װ��ʱ��ת���ɾ͵�ʱ�� */
INT XJP_TM_Trans_Dev_To_LT( UNSIGNED * Time1, UNSIGNED * Time2, TIME * pTime );
/* �͵�ʱ��ת����װ��ʱ�� */
INT XJP_TM_Trans_LT_To_Dev( UNSIGNED * Time1, UNSIGNED * Time2, TIME * pTime );
/* ����Ҫת����ʱ��ģʽ,1ΪPROFIBUSʱ��,2ΪUTCʱ��,ȱʡΪUTCʱ��  */
INT XJP_TM_Set_Time_Mode ( UNSIGNED TimeMode );
/* ��ȡװ��ʱ�� */
INT XJP_TM_Get_Time_Zone( INT * pTimeZone );
/* ���ʱ�����Ч��,ͨ��iChangeFlag�������Ƿ��޸Ĳ���ȷ��ʱ��(�޸�ʱ����Ϊʱ������ֵ) */
INT XJP_TM_Time_Check( TIME * pTime , INT iChangeFlag );
/* ��ȡϵͳʱ��ָ�� */
TIME * XJP_TM_Get_Time_Ptr( VOID );
/* ���ö�ʱʱ���� */
INT XJP_TM_SET_Time_Interval( UNSIGNED TimeInterval );
/* ���ö�ʱʱ��Ʒ������ */
INT XJP_TM_SET_Time_Quality( UNSIGNED Quality, INT CondType );
/* ����ϵͳ��ʼ����ʶ */
INT XJP_TM_Set_SysInitFlag( INT iSysInitFlag );

/* Define watchdog management functions.  */
/* �������� */
HWDM XJP_Watchdog_Create( UNSIGNED SetTime, CHAR * Name, UNSIGNED MonitorCtrlHandle );
/* ��λ���Ӷ��� */
INT XJP_Watchdog_Reset( HWDM Handle );
/* ʹ�ܼ��Ӷ��� */
INT XJP_Watchdog_Control( HWDM Handle, INT Enable );
/* ���Ź���ʱ������ */
CHAR * XJP_Watchdog_Timer( UNSIGNED Time, UNSIGNED * pMonitorCtrlHandle );

/* Define elf management functions.  */
/* ����ELF�ļ���CRC�� */
int XJP_ELF_Calculate_CRC16( char * filename, unsigned long int ini_crc,
    unsigned long int * crc_code );
/* ����ELF�ļ���CRC�� */
int XJP_ELF2_Calculate_CRC16( char * filename, unsigned long int ini_crc,
    unsigned long int * crc_code, unsigned long int ini_crc1,
    unsigned long int * crc_code1 );        
/* ����ELF�ļ�����CRC�� */
int XJP_ELF_Text_CRC16( char * filename, unsigned long int ini_crc,
    unsigned long int * crc_code );
/* ��ȡELF�ļ������ַ */
int XJP_ELF_Text_Addr( char * filename, int maxNumber, int * actualNumber,
    unsigned long int * text_addr, unsigned long int * text_size );
/* ��������ļ�(elf��ʽ)��CRC�� */
INT XJP_ELF_File_CRC16(  char * filename, UNSIGNED * text_crc );

/* Define protect management functions.  */
/* �������ݱ���; 0����ʧ��,!0:������Ӧ�þ�� */
HPRM XJP_NU_Protect_Create( CHAR * Name );
/* ��ȡ���������,0:δ�ҵ������� */
HPRM XJP_NU_Protect_Find( CHAR * Name );
/* ���ݱ���; 0:�ɹ�,0!:ʧ�� */
INT  XJP_NU_Protect( HPRM Handle );
/* �ͷ����ݱ���; 0:�ɹ�,0!:ʧ��*/
INT  XJP_NU_UnProtect( HPRM Handle );

/* Define timer management functions.  */
/* ������ʱ��; 0����ʧ��,!0:������Ӧ�þ��*/
HTMM XJP_NU_Timer_Create( CHAR * name, VOID (* expiration_routine )( UNSIGNED ),
    UNSIGNED initial_time, UNSIGNED reschedule_time, OPTION enable );
/* ��ʱ��ʹ�ܿ���; 0:�ɹ�,0!:ʧ�� */
INT XJP_NU_Timer_Control( HTMM Handle, OPTION enable );

/* Define event group management functions.  */
/* �����¼�������; 0����ʧ��,!0:������Ӧ�þ�� */
HEVM XJP_NU_Event_Group_Create( VOID );
/* �����¼�λ; 0:�ɹ�,0!:ʧ�� */
INT XJP_NU_Event_Group_Set( HEVM Handle, UNSIGNED events, OPTION operation );
/* ��ȡ�¼�λ; 0:�ɹ�,0!:ʧ�� */
INT XJP_NU_Event_Group_Retrieve( HEVM Handle, UNSIGNED requested_flags,
    OPTION operation, UNSIGNED * retrieved_flags, UNSIGNED suspend );

/* Define memory pool management functions.  */
/* �����ڴ�ع����� */
HMGM XJP_NU_Pool_Create( CHAR * name, VOID * start_address,
    UNSIGNED pool_size, UNSIGNED min_allocation, OPTION suspend_type );
/* ɾ���ڴ�� */
INT  XJP_NU_Pool_Delete( HMGM Handle );
/* �����ڴ� */
INT XJP_NU_Allocate_Memory( HMGM Handle, VOID ** return_pointer, UNSIGNED size,
    UNSIGNED suspend );
/* �ͷ��ڴ� */
INT XJP_NU_Deallocate_Memory( VOID * memory_ptr );

/* ��ȡ����ϵͳ������ */
OS_FUN * XJP_OS_Get_FuncList( VOID );

/* �������� */
HIOM XJP_IO_Create_Out( UNSIGNED GroupNumber );
/* ���������̵��� */
INT XJP_IO_Set_Start_Relay( HIOM Handle, UNSIGNED Flag );
/* ���ÿ���λ */
INT XJP_IO_Set_Output_Relay( HIOM Handle, UNSIGNED GroupNo, UNSIGNED Bits,
    INT Operation );
/* ��ȡװ�������̵���״̬ */
INT XJP_IO_Get_Start_Relay( UNSIGNED * ActualStartRelay );
/* ��ȡװ�ÿ����̵���״̬ */
INT XJP_IO_Get_Output_Relay( UNSIGNED GroupNo, UNSIGNED * ActualBits );
/* ���������̵����û��������� */
INT XJP_IO_Set_Start_Driver( VOID (* StartDriver )( UNSIGNED Flag ));
/* ���ÿ����̵����û��������� */
INT XJP_IO_Set_Output_Driver( VOID (* RelayDriver )( UNSIGNED GroupNo, UNSIGNED Bits ));
/* ��ȡ���̵ּ���״̬λ */
UNSIGNED XJP_IO_Get_Hold_Relay( UNSIGNED GroupNo );
/* ���ñ��̵ּ���״̬λ */
INT XJP_IO_Set_Hold_Relay( UNSIGNED GroupNo, UNSIGNED Bits );
/* ������̵ּ���״̬λ */
INT XJP_IO_Clear_Hold_Relay( VOID );
/* ���ÿ����̵�����������λ */
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
/* ����־û����� */
INT XJP_File_Save( CHAR * fileName, VOID * buf, UNSIGNED size,
    UNSIGNED counter, UNSIGNED offset );
/* ���־û����� */
INT XJP_File_Read( CHAR * fileName, VOID * buf, UNSIGNED size,
    UNSIGNED counter, UNSIGNED offset );
/* �����ļ���CRC�� */
INT XJP_CalculateFileCRC( CHAR * fileName, UNSIGNED * actCRCcode );
/* �������ݶ��й����� */
HQUEM XJP_CreateDataQueue( CHAR * name, UNSIGNED uBufSize, UNSIGNED uMaxMesLen, UNSIGNED uDataByte );
/* �����ݶ��з������ݺ��� */
INT  XJP_SendToDataQueue( CHAR * name, HQUEM Handle, VOID * message, UNSIGNED size,
    UNSIGNED uTimeOut, INT iFrontFlag );
/* �����ݶ��л�ȡ���ݺ��� */
INT  XJP_ReceiveFromDataQueue( CHAR * name, HQUEM Handle, VOID * message, UNSIGNED maxsize,
    UNSIGNED * ActSize, UNSIGNED uTimeOut );
/* ��ȡ���ݶ�����Ϣ */
INT XJP_DataQueueInformation( CHAR * name, HQUEM Handle, UNSIGNED * msg_count,
    UNSIGNED * avail_mem, UNSIGNED * msg_size );
/* ɾ��ָ�����ݶ��� */
INT XJP_DeleteDataQueue( CHAR * name, HQUEM Handle );
/*=======================================================================*/

#ifdef __cplusplus
}
#endif

#endif
