
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
/*      pmi_table.c                                     Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      PMI - Platform Initialize                                        */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the initialization routine for this           */
/*      component.                                                       */
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
/*      ZhiDeYang       02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include <string.h>
#include <math.h>
#include "plat/inc/xj_cosc.h"
#include "plat/es_plat.h"
#include "plat/inc/pro_defs.h"
#include "plat/inc/tab_defs.h"
#include "plat/inc/ctb_extr.h"
#include "plat/inc/esp_cfg.h"
#include "plat_specific/plat_specific.h"

#define TABLE_SETTING_DATA_WIDTH            4               /* ����ֵ���ݿ�� */
#define TABLE_SETTING_TIMES                 3               /* ����ֵ������� */
/* װ�����ݱ�ȱʡֵ���� */
#define TABLE_DEVICE_CNAME "�ţӣУ����������ƽ̨"     /* װ���������� */
#define TABLE_DEVICE_ENAME "ESP-801"                    /* װ��Ӣ������ */
#define TABLE_DEVICE_SOE_HOLD           0x1             /* �����ź� */
#define TABLE_DEVICE_PRINT_DELAY        5000            /* ����������ʱ��ӡʱ�䣨5s�� */
#define TABLE_DEVICE_INPUT_RESET_BITS   0xff            /* ����̵�������λ */
#define TABLE_DEVICE_SET_COPY_ENABLE    0               /* CPU֮�䶨ֵ����ʹ��ѡ�� */
#define TABLE_DEVICE_OPTICL_ENABLE      0               /* ���˲ʹ��ѡ�� */
#define TABLE_DEVICE_VER_HIGH           0x01            /* װ�ð汾�Ÿ�λ */
#define TABLE_DEVICE_VER_LOW            0x00            /* װ�ð汾�ŵ�λ */
#define TABLE_DEVICE_CURRENT_SET_ZONE   0               /* װ�ö�ֵ���� */

/* Define external inner-component global data references.  */
TAB_DEVICE DeviceTable;
TAB_DEVICE * pDeviceTab = XJ_NULL; /* װ�����ݱ��ڴ���ַ */
STATE * pWaveStateTab = XJ_NULL; /* ¼��״̬�����ݱ��ڴ���ַ */
INT WaveStateTabNumber = 0; /* ¼��״̬�����ݱ�����ݸ��� */
INPUT * pInputTab = XJ_NULL; /* ���������ݱ��ڴ���ַ */
INT InputTabNumber = 0; /* ���������ݱ�����ݸ��� */
OUTPUT * pOutputTab = XJ_NULL; /* ���������ݱ��ڴ���ַ */
INT OutputTabNumber = 0; /* ���������ݱ�����ݸ��� */
PRO_CHANNEL * pChannelTab = XJ_NULL; /* ģ�������ݱ��ڴ���ַ */
INT ChannelTabNumber = 0; /* ģ�������ݱ�����ݸ��� */
RJUMPER * pRJumperTab = XJ_NULL; /* ����ѹ�����ݱ��ڴ���ַ */
INT RJumperTabNumber = 0; /* ����ѹ�����ݱ�����ݸ��� */
INT SJumperTabNumber = 0; /* ��ѹ�����ݱ�����ݸ��� */
INT OverTimeTabNumber = 20; /* ��ʱ������ݸ��� */
CHAR PMI_SettingDirName[256] = CPC_SETTING_DATA_DIR_NAME; /* ���ݱ�����ֵ����·���� */
CHAR PMI_ConfigDirName[256] = CPC_CONFIG_DATA_DIR_NAME; /* ���ݱ���������·���� */
/* ���ݱ�����ֵ���ݴ洢�ļ����� */
CHAR PMI_SettingFileName[256] = "setting";
/* �û����ݴ洢�ļ����� */
CHAR PMI_UserDataFileName[256] = "userdata";
/* ���ݱ��������ݴ洢�ļ����� */
CHAR PMI_ConfigFileName[256] = "config";
/* �û����ݹ����� */
UNSIGNED UserDataHandle;
extern ESP_CFG PLAT_cfg;

/* ����ΪOS������� */
#ifdef WIN32
#endif

#ifndef WIN32 /* ��windowsȱʡ����һЩ���Ա�ͨ������ */
#endif

/* Define internal function calls.  */

/* ��ʼ�����ݱ� */
INT PMI_InitializeTable( VOID )
{
    UNSIGNED CfgHandle;
    UNSIGNED SetHandle;

    CfgHandle = ( UNSIGNED )XJP_Config_Create( PMI_ConfigDirName, PMI_ConfigFileName );
	SetHandle = ( UNSIGNED )XJP_NVData_Create( PMI_SettingDirName, PMI_SettingFileName, 3 ); /* ����ֵ���ݱ������� */
    UserDataHandle = ( UNSIGNED )XJP_NVData_Create( PMI_SettingDirName, PMI_UserDataFileName, 3 ); /* �û����ݱ������� */
    XJP_DS_Initialize(( PLAT_cfg.table.TableNumber ), ( PLAT_cfg.table.FieldNumber ));
    /* ע�����ʧ����ֵ������ */
    XJP_DS_Register_NVData_Treatment( SetHandle, PMI_SettingDirName, PMI_SettingFileName,
        XJP_DS_NVData_Write, XJP_DS_NVData_Read, XJP_DS_NVData_Repair );
    /* ע��������Ϣ������ */
    XJP_DS_Register_Config_Treatment( CfgHandle, PMI_ConfigDirName, PMI_ConfigFileName,
        XJP_DS_ConfigData_Write, XJP_DS_ConfigData_Read, XJP_DS_ConfigData_Repair );

    return( XJ_SUCCESS );
}

/* ��ȡ���ݱ��������Ϣ */
INT PMI_GetTableDataInformation( VOID )
{
    TB_TCB  * pTable;
    INT variable;

    /* ��ʼ��װ�����ݱ��ڴ�ָ�� */
    pDeviceTab = XJP_DS_Get_Table_Memory( TAB_DEVICE_ID, NU_NULL ); /* ��ȡװ�����ݱ��ڴ���ַ */
    if( pDeviceTab == XJ_NULL )
    {
        /* δ����װ�����ݱ�ʹ��ƽ̨ȱʡ���� */
        pDeviceTab = & DeviceTable;
        XJP_CSC_String_To_Word( pDeviceTab -> Device_CName, TABLE_DEVICE_CNAME, DEVICE_CNAME ); /* װ���������� */
        XJP_CSC_String_To_Word( pDeviceTab -> Device_EName, TABLE_DEVICE_ENAME, DEVICE_ENAME ); /* װ��Ӣ������ */
        pDeviceTab -> soe_mode = TABLE_DEVICE_SOE_HOLD;  /* soeģʽ */
        pDeviceTab -> print_delay_time = TABLE_DEVICE_PRINT_DELAY; /* ����������ʱ��ӡʱ�� */
        pDeviceTab -> reset_bit = TABLE_DEVICE_INPUT_RESET_BITS; /* ����̵�������λ */
        pDeviceTab -> copy_set_logic = TABLE_DEVICE_SET_COPY_ENABLE; /* CPU�临�ƶ�ֵʹ��ѡ�� */
        pDeviceTab -> otl_diff_logic = TABLE_DEVICE_OPTICL_ENABLE; /* ���˲ʹ��ѡ�� */
        pDeviceTab -> Ver_Low = TABLE_DEVICE_VER_LOW; /* װ�ð汾�ŵ��ֽ� */
        pDeviceTab -> Ver_High = TABLE_DEVICE_VER_HIGH; /* װ�ð汾�Ÿ��ֽ� */
        pDeviceTab -> Cpu_Number = PLAT_cfg.device.CpuNumber; /* CPU���� */
        pDeviceTab -> Cpu_Type = PLAT_cfg.device.CpuType; /* CPU���� */
        pDeviceTab -> Cpu_Zone = TABLE_DEVICE_CURRENT_SET_ZONE; /* װ�õ�ǰ��ֵ���� */
    }
    /* ��ʼ��¼��״̬�����ݱ��ڴ�ָ�� */
    pWaveStateTab = XJP_DS_Get_Table_Memory( TAB_STATE_ID, & WaveStateTabNumber );
    /* ��ʼ�����������ݱ��ڴ�ָ�� */
    pInputTab = XJP_DS_Get_Table_Memory( TAB_INPUT_ID, & InputTabNumber );
    /* ��ʼ�����������ݱ��ڴ�ָ�� */
    pOutputTab = XJP_DS_Get_Table_Memory( TAB_OUTPUT_ID, & OutputTabNumber );
    /* ��ʼ������ѹ�����ݱ��ڴ�ָ�� */
    pRJumperTab = XJP_DS_Get_Table_Memory( TAB_RJUMPER_ID, & RJumperTabNumber );
    /* ��ʼ��ѹ�����ݾݱ�����ݸ��� */
    XJP_DS_Get_Table_Memory( TAB_SJUMPER_ID, & SJumperTabNumber );
    /* ��ʱ������ݱ����ݸ��� */
    variable = SJumperTabNumber + RJumperTabNumber + PLAT_cfg.table.TableNumber;
    if( OverTimeTabNumber < variable )
    {
    	OverTimeTabNumber = variable;
    }

    /* ��ʼ��ģ�������ݱ��ڴ�ָ�� */
    pTable = CTBC_FindTable( TAB_CHANNEL_ID );
    if( pTable != XJ_NULL )
    {
        pTable->DataNumber = pTable->MaxDataNumber;
        ChannelTabNumber = pTable->DataNumber;
        pChannelTab = ( PRO_CHANNEL *)pTable->DataMemory;
    }

    return( XJ_SUCCESS );
}
