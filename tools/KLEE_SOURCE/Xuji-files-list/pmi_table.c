
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

#define TABLE_SETTING_DATA_WIDTH            4               /* 整定值数据宽度 */
#define TABLE_SETTING_TIMES                 3               /* 整定值保存份数 */
/* 装置数据表缺省值定义 */
#define TABLE_DEVICE_CNAME "ＥＳＰ－８０１软件平台"     /* 装置中文名称 */
#define TABLE_DEVICE_ENAME "ESP-801"                    /* 装置英文名称 */
#define TABLE_DEVICE_SOE_HOLD           0x1             /* 保持信号 */
#define TABLE_DEVICE_PRINT_DELAY        5000            /* 保护报告延时打印时间（5s） */
#define TABLE_DEVICE_INPUT_RESET_BITS   0xff            /* 复归继电器开入位 */
#define TABLE_DEVICE_SET_COPY_ENABLE    0               /* CPU之间定值复制使能选择 */
#define TABLE_DEVICE_OPTICL_ENABLE      0               /* 光纤差动使能选择 */
#define TABLE_DEVICE_VER_HIGH           0x01            /* 装置版本号高位 */
#define TABLE_DEVICE_VER_LOW            0x00            /* 装置版本号低位 */
#define TABLE_DEVICE_CURRENT_SET_ZONE   0               /* 装置定值区号 */

/* Define external inner-component global data references.  */
TAB_DEVICE DeviceTable;
TAB_DEVICE * pDeviceTab = XJ_NULL; /* 装置数据表内存首址 */
STATE * pWaveStateTab = XJ_NULL; /* 录波状态量数据表内存首址 */
INT WaveStateTabNumber = 0; /* 录波状态量数据表的数据个数 */
INPUT * pInputTab = XJ_NULL; /* 开入量数据表内存首址 */
INT InputTabNumber = 0; /* 开入量数据表的数据个数 */
OUTPUT * pOutputTab = XJ_NULL; /* 开出量数据表内存首址 */
INT OutputTabNumber = 0; /* 开出量数据表的数据个数 */
PRO_CHANNEL * pChannelTab = XJ_NULL; /* 模拟量数据表内存首址 */
INT ChannelTabNumber = 0; /* 模拟量数据表的数据个数 */
RJUMPER * pRJumperTab = XJ_NULL; /* 检修压板数据表内存首址 */
INT RJumperTabNumber = 0; /* 检修压板数据表的数据个数 */
INT SJumperTabNumber = 0; /* 软压板数据表的数据个数 */
INT OverTimeTabNumber = 20; /* 超时检查数据个数 */
CHAR PMI_SettingDirName[256] = CPC_SETTING_DATA_DIR_NAME; /* 数据表整定值数据路径名 */
CHAR PMI_ConfigDirName[256] = CPC_CONFIG_DATA_DIR_NAME; /* 数据表配置数据路径名 */
/* 数据表整定值数据存储文件名称 */
CHAR PMI_SettingFileName[256] = "setting";
/* 用户数据存储文件名称 */
CHAR PMI_UserDataFileName[256] = "userdata";
/* 数据表配置数据存储文件名称 */
CHAR PMI_ConfigFileName[256] = "config";
/* 用户数据管理句柄 */
UNSIGNED UserDataHandle;
extern ESP_CFG PLAT_cfg;

/* 以下为OS相关配置 */
#ifdef WIN32
#endif

#ifndef WIN32 /* 非windows缺省定义一些宏以便通过编译 */
#endif

/* Define internal function calls.  */

/* 初始化数据表 */
INT PMI_InitializeTable( VOID )
{
    UNSIGNED CfgHandle;
    UNSIGNED SetHandle;

    CfgHandle = ( UNSIGNED )XJP_Config_Create( PMI_ConfigDirName, PMI_ConfigFileName );
	SetHandle = ( UNSIGNED )XJP_NVData_Create( PMI_SettingDirName, PMI_SettingFileName, 3 ); /* 整定值数据保存三份 */
    UserDataHandle = ( UNSIGNED )XJP_NVData_Create( PMI_SettingDirName, PMI_UserDataFileName, 3 ); /* 用户数据保存三份 */
    XJP_DS_Initialize(( PLAT_cfg.table.TableNumber ), ( PLAT_cfg.table.FieldNumber ));
    /* 注册非易失整定值处理函数 */
    XJP_DS_Register_NVData_Treatment( SetHandle, PMI_SettingDirName, PMI_SettingFileName,
        XJP_DS_NVData_Write, XJP_DS_NVData_Read, XJP_DS_NVData_Repair );
    /* 注册配置信息处理函数 */
    XJP_DS_Register_Config_Treatment( CfgHandle, PMI_ConfigDirName, PMI_ConfigFileName,
        XJP_DS_ConfigData_Write, XJP_DS_ConfigData_Read, XJP_DS_ConfigData_Repair );

    return( XJ_SUCCESS );
}

/* 获取数据表的数据信息 */
INT PMI_GetTableDataInformation( VOID )
{
    TB_TCB  * pTable;
    INT variable;

    /* 初始化装置数据表内存指针 */
    pDeviceTab = XJP_DS_Get_Table_Memory( TAB_DEVICE_ID, NU_NULL ); /* 获取装置数据表内存首址 */
    if( pDeviceTab == XJ_NULL )
    {
        /* 未创建装置数据表，使用平台缺省配置 */
        pDeviceTab = & DeviceTable;
        XJP_CSC_String_To_Word( pDeviceTab -> Device_CName, TABLE_DEVICE_CNAME, DEVICE_CNAME ); /* 装置中文名称 */
        XJP_CSC_String_To_Word( pDeviceTab -> Device_EName, TABLE_DEVICE_ENAME, DEVICE_ENAME ); /* 装置英文名称 */
        pDeviceTab -> soe_mode = TABLE_DEVICE_SOE_HOLD;  /* soe模式 */
        pDeviceTab -> print_delay_time = TABLE_DEVICE_PRINT_DELAY; /* 保护报告延时打印时间 */
        pDeviceTab -> reset_bit = TABLE_DEVICE_INPUT_RESET_BITS; /* 复归继电器开入位 */
        pDeviceTab -> copy_set_logic = TABLE_DEVICE_SET_COPY_ENABLE; /* CPU间复制定值使能选择 */
        pDeviceTab -> otl_diff_logic = TABLE_DEVICE_OPTICL_ENABLE; /* 光纤差动使能选择 */
        pDeviceTab -> Ver_Low = TABLE_DEVICE_VER_LOW; /* 装置版本号低字节 */
        pDeviceTab -> Ver_High = TABLE_DEVICE_VER_HIGH; /* 装置版本号高字节 */
        pDeviceTab -> Cpu_Number = PLAT_cfg.device.CpuNumber; /* CPU号码 */
        pDeviceTab -> Cpu_Type = PLAT_cfg.device.CpuType; /* CPU类型 */
        pDeviceTab -> Cpu_Zone = TABLE_DEVICE_CURRENT_SET_ZONE; /* 装置当前定值区号 */
    }
    /* 初始化录波状态量数据表内存指针 */
    pWaveStateTab = XJP_DS_Get_Table_Memory( TAB_STATE_ID, & WaveStateTabNumber );
    /* 初始化开入量数据表内存指针 */
    pInputTab = XJP_DS_Get_Table_Memory( TAB_INPUT_ID, & InputTabNumber );
    /* 初始化开出量数据表内存指针 */
    pOutputTab = XJP_DS_Get_Table_Memory( TAB_OUTPUT_ID, & OutputTabNumber );
    /* 初始化检修压板数据表内存指针 */
    pRJumperTab = XJP_DS_Get_Table_Memory( TAB_RJUMPER_ID, & RJumperTabNumber );
    /* 初始软压板数据据表的数据个数 */
    XJP_DS_Get_Table_Memory( TAB_SJUMPER_ID, & SJumperTabNumber );
    /* 超时检查数据表数据个数 */
    variable = SJumperTabNumber + RJumperTabNumber + PLAT_cfg.table.TableNumber;
    if( OverTimeTabNumber < variable )
    {
    	OverTimeTabNumber = variable;
    }

    /* 初始化模拟量数据表内存指针 */
    pTable = CTBC_FindTable( TAB_CHANNEL_ID );
    if( pTable != XJ_NULL )
    {
        pTable->DataNumber = pTable->MaxDataNumber;
        ChannelTabNumber = pTable->DataNumber;
        pChannelTab = ( PRO_CHANNEL *)pTable->DataMemory;
    }

    return( XJ_SUCCESS );
}
