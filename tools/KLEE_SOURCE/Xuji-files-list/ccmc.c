
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
/*      ccmc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CCM- Common Config Management                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common config       */
/*      management component.                                            */
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
/*      ZhiDeYang       11-15-2007      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

#include <string.h>
#include <stdio.h>
#include "plat/inc/xj_cosc.h"
#include "plat_specific/plat_specific.h"
#include "plat/inc/esp_cfg.h"
#include "plat/net_inc/ls_extr.h"
#include "plat/inc/cud_extr.h"
#include "plat/inc/out_check.h"
#include "plat/inc/ctb_extr.h"
#include "plat/inc/ccm_extr.h"
#include "plat/inc/apf_extr.h"
#include "plat/inc/mpf_extr.h"
#include "plat/inc/gpf_extr.h"
#include "plat/es_plat.h"
#include "plat/inc/hrf.h"
#include "plat/inc/xj_file.h"
#include "plat/inc/xj_cmemc.h"

#define CCMC_GETSTRUCTOFFSET(x,y) (int)(&((x*)0)->y)
#define CCMC_TABLE_CRC_DIR_NAME  "/user/pdevTemplateInf" /* 装置模板信息路径 */
#define CCMC_IDENT_TABLE         ".dataTable" /* 数据表标识名称 */

#define CCMC_GET_USER_EVENT_GROUPS  (PLAT_cfg.history.UserEventGroups)        /* 每通道的采样值点数 */
#define CSMV_GET_CHANNEL_POINT      (PLAT_cfg.smvc.ChannelPoint)              /* 每通道的采样值点数 */

/* 设备监视信息结构 */
typedef struct DEV_MONITOR_STRUCT
{
    /* SV监视信息 */
    /* 接收SV报文帧数 */
    UNSIGNED * SV_rxd_frames_number;
    /* 接收SV通道个数 */
    UNSIGNED * SV_rxd_channel_number;
    /* 接收SV序号错误计数 */
    UNSIGNED * SV_rxd_serial_error_count;
    /* 接收SV报文时间 */
    UNSIGNED * SV_rxd_time;
    /* 接收SV原始数据 */
    float * SV_rxd_data;
    /* GOOSE监视信息 */
    /* 接收GOOSE报文帧数 */
    UNSIGNED * GOOSE_rxd_frames_number;
    /* 接收GOOSE组数 */
    UNSIGNED * GOOSE_rxd_group_number;
    /* 接收GOOSE状态 */
    UNSIGNED * GOOSE_rxd_status;
    /* 接收GOOSE品质 */
    UNSIGNED * GOOSE_rxd_quality;
    /* 接收GOOSE检修 */
    UNSIGNED * GOOSE_rxd_repair;
    /* 接收GOOSE报文时间 */
    UNSIGNED * GOOSE_rxd_time;
    /* 接收GOOSE整组掩码位 */
    UNSIGNED * GOOSE_rxd_masks_group;
    /* SNTP对时监视信息 */
    /* 接收SNTP报文帧数 */
    UNSIGNED * SNTP_rxd_frames_number;
    /* 接收SNTP报文长度错误计数 */
    UNSIGNED * SNTP_rxd_length_error;
    /* 接收SNTP报文年份出错 */
    UNSIGNED * SNTP_rxd_year_error;
    /* 带时标GOOSE监视信息 */
    /* 接收带时标GOOSE报文帧数 */
    UNSIGNED * T_GOOSE_rxd_frames_number;
    /* 接收带时标GOOSE长度错误 */
    UNSIGNED * T_GOOSE_rxd_length_error;
}DEV_MONITOR;

/* 以下为OS相关配置 */
#ifdef WIN32
/* 全局变量申明 */
/* Windows 宏定义 */
#define CCMC_HMI_TEXT_FILE_NAME  "NPU808HMI.elf" /* HMI程序存储路径 */
#define CCMC_GWS_TEXT_FILE_NAME  "NPU808Gws.elf" /* GWS程序存储路径 */
#define CCMC_PLAT_TEXT_FILE_NAME "NPU808Plat.elf" /* Plat程序存储路径 */
#define CCMC_PRO_TEXT_FILE_NAME  "NPU808Pro.elf"  /* Pro程序存储路径 */
#define CCMC_DRV_TEXT_FILE_NAME  "NPU808Drv.elf"  /* 驱动程序存储路径 */
#endif

#ifndef WIN32 /* 非windows缺省定义一些宏以便通过编译 */
/* 全局变量申明 */
/* Nucleus宏定义 */
#define CCMC_HMI_TEXT_FILE_NAME   "/bin/NPU808HMI.elf"  /* HMI程序存储路径 */
#define CCMC_GWS_TEXT_FILE_NAME   "/bin/NPU808Gws.elf"  /* GWS程序存储路径 */
#define CCMC_PLAT_TEXT_FILE_NAME  "/bin/NPU808Plat.elf" /* Plat程序存储路径 */
#define CCMC_PRO_TEXT_FILE_NAME   "/bin/NPU808Pro.elf"  /* Pro程序存储路径 */
#define CCMC_DRV_TEXT_FILE_NAME   "/bin/NPU808Drv.elf"  /* 驱动程序存储路径 */
#endif

extern NU_EVENT_GROUP Event_1_CB;
extern NU_EVENT_GROUP Event_2_CB;
extern ESP_CFG PLAT_cfg;
extern CEVENT PRO_EventRecord[];
extern CEVENT CAlarmEvent;
extern CEVENT CReserveEvent;
extern CEVENT COperateEvent;
extern CTABLE CTable;
extern NU_PROTECT * pReport_Protect;

/* 装置监视信息列表 */
extern CSMV_MSG_COUNT CSMV_MsgCount;
extern UNSIGNED CSMV_Rxd_Time_SV;
extern float CSMC_Analog[];
extern UNSIGNED PLAT_SoftInputMemory[];
extern UNSIGNED PLAT_SoftInputQuality[];
extern UNSIGNED PLAT_SoftInputCheckQuality[];
extern UNSIGNED CSMV_Rxd_Time_GOOSE;
extern UNSIGNED CINC_SoftInFullMasks[];

DEV_MONITOR DeviceMointoring = 
{
    /* SV监视信息 */
    & CSMV_MsgCount.iRecSmvCount[0],                /* SV_rxd_frames_number */
    & CSMV_MsgCount.iRecSmvCount[1],                /* SV_rxd_channel_number */
    & CSMV_MsgCount.iRecSmvCount[2],                /* SV_rxd_serial_error_count */
    & CSMV_Rxd_Time_SV,                             /* SV_rxd_time */
    & CSMC_Analog[0],                               /* SV_rxd_data */
    /* GOOSE监视信息 */
    & CSMV_MsgCount.iGooseRecCount[0],              /* GOOSE_rxd_frames_number */
    & CSMV_MsgCount.iGooseRecCount[1],              /* GOOSE_rxd_group_number */
    PLAT_SoftInputMemory,                           /* GOOSE_rxd_status */
    PLAT_SoftInputQuality,                          /* GOOSE_rxd_quality */
    PLAT_SoftInputCheckQuality,                     /* GOOSE_rxd_repair */
    & CSMV_Rxd_Time_GOOSE,                          /* GOOSE_rxd_time */
    CINC_SoftInFullMasks,                           /* GOOSE_rxd_masks_group */
    /* SNTP对时监视信息 */
    & CSMV_MsgCount.iRecSynCount,                   /* SNTP_rxd_frames_number */
    & CSMV_MsgCount.iRecSynLenErrCount[0],          /* SNTP_rxd_length_error */
    & CSMV_MsgCount.iRecSynYearErrCount[0],         /* SNTP_rxd_year_error */
    /* 带时标GOOSE监视信息 */
    & CSMV_MsgCount.iRecGooseTimeCount,             /* T_GOOSE_rxd_frames_number */
    & CSMV_MsgCount.iRecGooseTimeLenErrCount,       /* T_GOOSE_rxd_length_error */
};

/* 内部事件存储路径 */
CHAR UserEventDirName[7][256] = 
{
    CPC_USER1_DIR_NAME,
    CPC_USER2_DIR_NAME,
    CPC_USER3_DIR_NAME,
    CPC_USER4_DIR_NAME,
    CPC_USER5_DIR_NAME,
    CPC_USER6_DIR_NAME,
    CPC_USER7_DIR_NAME,
};

UNSIGNED CCMC_platCRC;
UNSIGNED CCMC_devCRC;
UNSIGNED CCMC_hmiCRC;
UNSIGNED CCMC_gwsCRC;
volatile INT CCMC_PlatLeadFlag = XJ_FALSE;  /* 平台引导标识 */
volatile UNSIGNED CCMC_ModelInitCount = 0;  /* 装置分模块初始化计数器 */
UNSIGNED RunTextNumber = 0;
RUN_TEXT_CB RunTextInf[ CCMC_MAX_RUN_TEXT + 1 ]; /* 运行程序信息缓冲区 */

INT CCMC_ProgramConfigMain( VOID )
{
    static INT GroupNumber = 0;
    static HTAB RepairTabList;
    UNSIGNED ActualEvent;
    CHAR EventName[LS_ALARM_NAME];
    INT status;

    if( LS_GET_CFG_PROGRAM(& ActualEvent ) == NU_SUCCESS )
    {
        /* clear config data */
        if( GroupNumber == 0 )
        {
            /* 清除配置数据 */
            status = XJP_Config_Initialize(( HFGM )CTable.ConfigId, CPC_CONFIG_TIMEOUT );
            if( status != NU_TIMEOUT )
            {
                RepairTabList = 0;
                GroupNumber ++;
                /* 配置信息变化 */
                sprintf( EventName, " %03d", LS_RES_EVENT_CONFIG_CHANGE );
                CEMC_FillEvent( EventName, RESERVE_EVENT_ID );
            }
        }
        else if( GroupNumber == 1 ) /* save table config data */
        {
            status = XJP_DS_Save_Field_Value_Config( & RepairTabList, CPC_CONFIG_TIMEOUT );
            if( status != NU_TIMEOUT )
            {
                /* 非超时状态，返回固化结果 */
                if( status == NU_SUCCESS )
                {
                    /* 固化正确 */
                    NU_Set_Events(& Event_2_CB, (~ CPC_EVENT2_CFG_ERROR ), NU_AND );
                    CEMC_FillEvent( "配置修改请重启", 0x00 );
                }
                else
                {
                    /* 固化失败 */
                    NU_Set_Events(& Event_2_CB, CPC_EVENT2_CFG_ERROR, NU_OR );
                    CEMC_FillEvent( "配置修改出错请重新配置", 0x00 );
                }
                GroupNumber = 0;
                LS_CLR_CFG_PROGRAM();
            }
        }
        else
        {
            GroupNumber = 0;
            LS_CLR_CFG_PROGRAM();
        }
    }

    /* Return the completion status.  */
    return NU_SUCCESS;
}

/* 创建设备驱动句柄函数 */
VOID CCMC_LoadNVdata( VOID )
{
    INT iStatus;
    CEVENT * pEventGroup;
    INT Number;
    UNSIGNED buf[32];    /* 临时缓冲区 */
    INT text_change;
    UNSIGNED verL, verH, toolVer;
    UNSIGNED CrcCode;
    INT i;
    UNSIGNED_CHAR ucStatus;
    CREPORT * pReportHandle;

#if 0
    /* RAM 自检.  */
    if( Ram_Check_Flag )
    {
        NU_Set_Events(& Event_1_CB, PRO_EVENT1_ERR_RAM, NU_OR );
    }
    else
    {
        NU_Set_Events(& Event_1_CB,( UNSIGNED )(~ PRO_EVENT1_ERR_RAM ), NU_AND );
    }
#endif
    /* 初始化运行程序信息 */
    memset( RunTextInf, 0, sizeof( RunTextInf ));

    /* 创建报告对象句柄 */
    CRMC_Create( 1, CPC_MAX_NVRAM_REPORT_NUMBER, CPC_REPORT_DIR_NAME );

    /* 用户事件实例化操作 */
    pEventGroup = PRO_EventRecord;
    Number = CCMC_GET_USER_EVENT_GROUPS;
    for( i = 0; i < Number; i ++)
    {
        /* Setup initialize value */
        pEventGroup -> InitFlag = 0;
        pEventGroup -> MaxNumber = 0;
        pEventGroup -> Number = 0;
        pEventGroup -> Serial = 0;
        pEventGroup -> EventId = ( UNSIGNED )( i + 1 );
        pEventGroup -> EventDriver = CAlarmEvent.EventDriver;
        pEventGroup -> CtrlDriver = CAlarmEvent.CtrlDriver;
        pEventGroup -> NVStart = NU_NULL;
        pEventGroup -> NVEnd = NU_NULL;
        pEventGroup -> NVIn = NU_NULL;
        pEventGroup -> NVOut = NU_NULL;
        pEventGroup -> dirName = UserEventDirName[i];
        pEventGroup -> EventHandle = 0;

        pEventGroup -> Initialize = CAlarmEvent.Initialize;
        pEventGroup -> Read = CAlarmEvent.Read;
        pEventGroup -> Write = CAlarmEvent.Write;
        pEventGroup -> Clear = CAlarmEvent.Clear;
        pEventGroup -> Information = CAlarmEvent.Information;
        pEventGroup -> Find = CAlarmEvent.Find;
        pEventGroup ++;
    }

    /* 计算HMI程序的CRC码 */
    XJP_ELF_File_CRC16( CCMC_HMI_TEXT_FILE_NAME, & CCMC_hmiCRC );
    /* HMI 运行程序信息 */
    RunTextInf[ RunTextNumber ].CrcCode = CCMC_hmiCRC;
    strncpy( RunTextInf[ RunTextNumber ].Name, CCMC_HMI_TEXT_FILE_NAME, CCMC_FILE_NAME_LEN ); /* 记录文件名称 */
    if( MPFC_GetHmiVersion( & verL, & verH, & toolVer ) == NU_SUCCESS )
    {
        RunTextInf[ RunTextNumber ].Version = ( verL & CPC_DATA_MASKS8 )
            + (( verH & CPC_DATA_MASKS8 ) << CPC_DATA_BITS8 );
    }
    RunTextNumber ++;

    /* 计算网关程序的CRC码 */
    XJP_ELF_File_CRC16( CCMC_GWS_TEXT_FILE_NAME, & CCMC_gwsCRC );
    /* 记录网关运行程序信息 */
    RunTextInf[ RunTextNumber ].CrcCode = CCMC_gwsCRC;
    strncpy( RunTextInf[ RunTextNumber ].Name, CCMC_GWS_TEXT_FILE_NAME, CCMC_FILE_NAME_LEN ); /* 记录文件名称 */
    if( GPFC_GetGwsVersion( & verL, & verH ) == NU_SUCCESS )
    {
        RunTextInf[ RunTextNumber ].Version = ( verL & CPC_DATA_MASKS8 )
            + (( verH & CPC_DATA_MASKS8 ) << CPC_DATA_BITS8 );
    }
    RunTextNumber ++;

    /* 计算平台程序的CRC码 */
    CrcCode = CPC_CRC16_Initialize;
    CCMC_platCRC = CPC_CRC16_Initialize;
    XJP_ELF2_Calculate_CRC16( CCMC_PLAT_TEXT_FILE_NAME, CCMC_platCRC, & CCMC_platCRC,
        CrcCode, & CrcCode );
    CrcCode = ( CrcCode ^ CPC_CRC16_MASKS );
    /* 记录平台运行程序信息 */
    RunTextInf[ RunTextNumber ].CrcCode = CrcCode;
    strncpy( RunTextInf[ RunTextNumber ].Name, CCMC_PLAT_TEXT_FILE_NAME, CCMC_FILE_NAME_LEN ); /* 记录文件名称 */
    RunTextInf[ RunTextNumber ].Version = VER_PLATFORM;
    RunTextNumber ++;

    /* 计算驱动程序CRC码 */
    CrcCode = CPC_CRC16_Initialize;
    XJP_ELF2_Calculate_CRC16( CCMC_DRV_TEXT_FILE_NAME, CCMC_platCRC, & CCMC_platCRC,
        CrcCode, & CrcCode );
    CrcCode = ( CrcCode ^ CPC_CRC16_MASKS );
    CCMC_devCRC = CCMC_platCRC;
    /* 记录驱动运行程序信息 */
    RunTextInf[ RunTextNumber ].CrcCode = CrcCode;
    strncpy( RunTextInf[ RunTextNumber ].Name, CCMC_DRV_TEXT_FILE_NAME, CCMC_FILE_NAME_LEN ); /* 记录文件名称 */
    RunTextInf[ RunTextNumber ].Version = VER_PLATFORM;
    RunTextNumber ++;

    /* 计算保护程序CRC码 */
    CrcCode = CPC_CRC16_Initialize;
    XJP_ELF2_Calculate_CRC16( CCMC_PRO_TEXT_FILE_NAME, CCMC_devCRC, & CCMC_devCRC,
        CrcCode, & CrcCode );
    CrcCode = ( CrcCode ^ CPC_CRC16_MASKS );
    CCMC_platCRC = ( CCMC_platCRC ^ CPC_CRC16_MASKS );
    CCMC_devCRC = ( CCMC_devCRC ^ CPC_CRC16_MASKS );
    /* 记录保护程序信息 */
    RunTextInf[ RunTextNumber ].CrcCode = CrcCode;
    strncpy( RunTextInf[ RunTextNumber ].Name, CCMC_PRO_TEXT_FILE_NAME, CCMC_FILE_NAME_LEN ); /* 记录文件名称 */
    if( APFC_GetDeviceVersion( & verL, & verH ) == NU_SUCCESS )
    {
        RunTextInf[ RunTextNumber ].Version = ( verL & CPC_DATA_MASKS8 )
            + (( verH & CPC_DATA_MASKS8 ) << CPC_DATA_BITS8 );
    }
    RunTextNumber ++;

    text_change = NU_TRUE;
    /* 读取平台和网关程序的CRC码 */
    if( XJP_File_Read( CPC_TEXT_CRC_DIR_NAME, buf, sizeof( UNSIGNED ), 2, 0 ) == XJ_SUCCESS ) /* 2个字的CRC码 */
    {
        if( CCMC_devCRC == buf[0] )
        {
            /* 程序未发生变化 */
            text_change = NU_FALSE;
        }
        if( CCMC_gwsCRC != buf[1] )
        {
            /* 填写告警事件 */
            CEMC_FillEvent( "ECM通讯模块程序校验错", ALARM_EVENT_ID );
        }
    }
    else
    {
        /* 填写告警事件 */
        CEMC_FillEvent( "ECM通讯模块程序校验错", ALARM_EVENT_ID );
    }

    if(( text_change == XJ_TRUE ) || ( CCMC_devCRC != buf[0] ) || ( CCMC_gwsCRC != buf[1] ))
    {
        /* 新程序，保存程序CRC码 */
        buf[0] = CCMC_devCRC;
        buf[1] = CCMC_gwsCRC;
        XJP_File_Save( CPC_TEXT_CRC_DIR_NAME, buf, sizeof( UNSIGNED ), 2, 0 );
    }

    /* 用户数据初始化(无论CRC是否发生变化都将用户数据从缓冲区中读取出来) */
    CUDC_UserDataInitialize( NU_NO_SUSPEND );
    /* 报告管理句柄 */
    pReportHandle = CRMC_GetReportHandle( 1 );

    /* Determine if the first run.  */
    if( text_change == NU_TRUE )
    {
        /* 更换新程序,设置程序校验出错标志 */
        NU_Set_Events(& Event_1_CB, CPC_EVENT1_ERR_PROGRAM, NU_OR );
        /* 填写告警事件 */
        CEMC_FillEvent( "保护程序校验出错", ALARM_EVENT_ID );

        /* 数据表初始化 */
        XJP_DS_Initialize_SetData( 1, NU_NO_SUSPEND );

        /* 告警事件初始化 */
        CAlarmEvent.Initialize(& CAlarmEvent, 1, NU_NO_SUSPEND );

        /* 内部事件初始化 */
        CReserveEvent.Initialize(& CReserveEvent, 1, NU_NO_SUSPEND );

        /* 操作事件初始化 */
        COperateEvent.Initialize(& COperateEvent, 1, NU_NO_SUSPEND );

        /* 用户事件初始化 */
        pEventGroup = PRO_EventRecord;
        Number = CCMC_GET_USER_EVENT_GROUPS;
        if( pEventGroup != NU_NULL )
        {
            for( i = 0; i < Number; i ++)
            {
                pEventGroup -> Initialize( pEventGroup, 1, NU_NO_SUSPEND );
                pEventGroup ++;
            }
        }
        /* 报告初始化 */
        if( pReportHandle )
            pReportHandle->Initialize( pReportHandle, 1, NU_NO_SUSPEND );

        /* 录波初始化 */
        CWave.Initialize(& CWave, 1, NU_NO_SUSPEND );
    }
    else
    {
        /* 数据表初始化 */
        iStatus = XJP_DS_Initialize_SetData( 0, NU_NO_SUSPEND );
        if( iStatus != NU_SUCCESS )
        {
            //            NU_Set_Events(& Event_1_CB, PRO_EVENT1_ERR_SRAM, NU_OR );
        }

        /* 告警事件初始化 */
        CAlarmEvent.Initialize(& CAlarmEvent, 0, NU_NO_SUSPEND );

        /* 内部事件初始化 */
        CReserveEvent.Initialize(& CReserveEvent, 0, NU_NO_SUSPEND );

        /* 操作事件初始化 */
        COperateEvent.Initialize(& COperateEvent, 0, NU_NO_SUSPEND );

        /* 用户事件初始化 */
        pEventGroup = PRO_EventRecord;
        Number = CCMC_GET_USER_EVENT_GROUPS;
        if( pEventGroup != NU_NULL )
        {
            for( i = 0; i < Number; i ++)
            {
                pEventGroup -> Initialize( pEventGroup, 0, NU_NO_SUSPEND );
                pEventGroup ++;
            }
        }

        /* 报告初始化 */
        if( pReportHandle )
            pReportHandle->Initialize( pReportHandle, 0, NU_NO_SUSPEND );

        /* 录波初始化 */
        CWave.Initialize(& CWave, 0, NU_NO_SUSPEND );
    }

    /* 检查定值状态 */
    if( XJP_DS_Get_Table_Status( TAB_SETTING_ID, & ucStatus ) == NU_SUCCESS )
    {
        if( ucStatus & TABLE_ERR_LEAD_SG )
        {
            NU_Set_Events(& Event_1_CB, CPC_EVENT1_ERR_SETTING, NU_OR );
        }
    }
    /* 数据表数据备份 */
    CCMC_TableDataSave();
    /* 引导出口保持位数据 */
    {
        UNSIGNED uHoldBits[CPC_MAX_OUTPUT_GROUPS];

        /* 获取开出保持位状态 */
        /* 读出口保持位数据 */
        iStatus = XJP_Output_Hold_Read( uHoldBits, 0,
            CPC_MAX_OUTPUT_GROUPS, NU_NO_SUSPEND );
        /* 判断获取数据是否成功 */
        if( iStatus == NU_SUCCESS )
        {
            /* 如果成功,将数据存入出口保持位缓冲区 */
            for( i = 0; i < CPC_MAX_OUTPUT_GROUPS; i ++)
            {
                XJP_IO_Set_Hold_Relay( i, uHoldBits[i] );
                /* 设置出口继电器动作位 */
                OUTC_Check_SetPickupBits( i, uHoldBits[i] );
            }
        }
    }
}

/* 保存出口保持位函数 */
VOID CCMC_SaveOutputHoldBits( VOID )
{
    static   INT InitFlag = 0;
    static   UNSIGNED uOldHoldBits[CPC_MAX_OUTPUT_GROUPS];
    UNSIGNED uNewHoldBits[CPC_MAX_OUTPUT_GROUPS];
    INT      iStatus;
    int      i;
    //--------------------------------------------------------------------------
    /* 清数据缓冲区 */
    memset( uNewHoldBits, 0,( sizeof( UNSIGNED )* CPC_MAX_OUTPUT_GROUPS ));
    /* 装置初始上电时,将缓冲区的数据更新 */
    if( InitFlag == 0 )
    {
        /* 清数据缓冲区 */
        memset( uOldHoldBits, 0,( sizeof( UNSIGNED )* CPC_MAX_OUTPUT_GROUPS ));
        /* 读出口保持位数据 */
        iStatus = XJP_Output_Hold_Read( uOldHoldBits, 0,
            CPC_MAX_OUTPUT_GROUPS, CPC_OUTHOLD_TIMEOUT );
        //if( iStatus == NU_SUCCESS )/* 当文件不存在的时候也返回失败 */
        {
            /* 初始化完成 */
            InitFlag = 1;
        }
    }
    else
    {
        /* 获取当前出口保持位的状态 */
        for( i = 0; i < CPC_MAX_OUTPUT_GROUPS; i ++)
        {
            uNewHoldBits[i] = XJP_IO_Get_Hold_Relay( i );
        }
        /* 判断出口保持位是否发生变化,以便决定是否需要将当前数据固化 */
        for( i = 0; i < CPC_MAX_OUTPUT_GROUPS; i ++)
        {
            if( uOldHoldBits[i] != uNewHoldBits[i] )
            {
                /* 固化出口保持位 */
                iStatus = XJP_Output_Hold_Write( uNewHoldBits, 0,
                    CPC_MAX_OUTPUT_GROUPS, CPC_OUTHOLD_TIMEOUT );
                if( iStatus == NU_SUCCESS )
                {
                    /* 更新原出口保持位状态 */
                    memcpy( uOldHoldBits, uNewHoldBits,( sizeof( UNSIGNED )* CPC_MAX_OUTPUT_GROUPS ));
                }
                break;
            }
        }
    }
}

/* 设置平台引导标识 */
INT CCMC_SetPlatLeadFlag( int iFlag )
{
    if( iFlag )
    {
        CCMC_PlatLeadFlag = XJ_TRUE;
    }
    else
    {
        CCMC_PlatLeadFlag = XJ_FALSE;
    }
    return( XJ_SUCCESS );
}

/* 获取平台引导标识 */
INT CCMC_GetPlatLeadFlag( VOID )
{
    return( CCMC_PlatLeadFlag );
}

/* 设置装置分模块初始化成功函数 */
INT CCMC_SetModInitSuccess( VOID )
{
    NU_Protect( pReport_Protect );
    CCMC_ModelInitCount += 1;
    NU_Unprotect();
    return( NU_SUCCESS );
}

/* 获取装置分模块初始化完成个数 */
INT CCMC_GetModInitCount( VOID )
{
    return( CCMC_ModelInitCount );
}

/* 数据表数据备份 */
INT CCMC_TableDataSave( VOID )
{
    INT i;
    INT k;
    INT iStatus;
    INT CpuNumber;
    INT iVariable;
    UNSIGNED crc_code;
    UNSIGNED uVariable;
    UNSIGNED TableNumber;
    UNSIGNED TableSectSize;
    UNSIGNED TableSaveHandle;
    UNSIGNED TabSaveSize;
    UNSIGNED_CHAR * pTabSaveBuf;
    TB_TCB * pTable;
    HTAB ** pTablePointer;

    /* 计算配置数据的CRC码 */
    crc_code = CPC_CRC32_Initialize;

    XJP_CRC_Calculate( &( CTable.ConfigCrcCode ), sizeof( UNSIGNED ),
        ( 4 / sizeof( UNSIGNED )), CPC_CRC32_BITS, crc_code,& crc_code ); /* 计算CRC码 */
    crc_code = ( crc_code ^ CPC_CRC32_MASKS );

    /* 数据表数据变化检查 */
    iStatus = XJ_TRUE;
    if( XJP_File_Read( CCMC_TABLE_CRC_DIR_NAME, & uVariable, sizeof( UNSIGNED ), 1,
        CCMC_GETSTRUCTOFFSET( HRF32_RHDR, r_version ) ) == XJ_SUCCESS )
    {
        if( crc_code == uVariable )
        {
            iStatus = XJ_FALSE;
        }
    }
    if( iStatus == XJ_TRUE )
    {
        /* 删除备份文件 */
        if ( xj_FileExists( CCMC_TABLE_CRC_DIR_NAME ))
        {
            xj_RemoveFile( CCMC_TABLE_CRC_DIR_NAME );
        }
        /* 数据模型变化，重新备份数据表数据 */
        TableNumber = 0;
        /* 获取数据表个数 */
        TableNumber = XJP_DS_Get_Table_Number();
        /* 申请内存空间 */
        pTablePointer = ( HTAB **)malloc( TableNumber * sizeof( HTAB *));
        if( pTablePointer )
        {
            /* 计算数据表数据尺寸 */
            TableSectSize = 0;
            iVariable = XJP_DS_GetTablePointers( pTablePointer, TableNumber );
            for( k = 0; k < iVariable; k ++ )
            {
                pTable = ( TB_TCB *)(*( pTablePointer + k ));
                TableSectSize += ( pTable->DataNumber * pTable->Size );
            }
            /* 保存数据模型数据 */
            TableSaveHandle = Hrf_Create( 0, TableNumber, TableSectSize, crc_code );
            /* 记录节区信息 */
            CpuNumber = 1;
            iVariable = XJP_DS_GetTablePointers( pTablePointer, TableNumber );
            for( k = 0; k < iVariable; k ++ )
            {
                pTable = ( TB_TCB *)(*( pTablePointer + k ));
                Hrf_RecordSectorInf_Table( TableSaveHandle, ( pTable->DataNumber * pTable->Size ),
                    pTable->DataMemory, pTable->Index, ( unsigned char *)pTable->Name, CpuNumber, pTable->Size );
            }
            Hrf_RecordSectorHead( TableSaveHandle, ( unsigned char *)CCMC_IDENT_TABLE );
            /* 固化数据表数据 */
            iStatus = Hrf_GetRecordInf( TableSaveHandle, & pTabSaveBuf, & TabSaveSize );
            if( iStatus == XJ_SUCCESS )
            {
                XJP_File_Save( CCMC_TABLE_CRC_DIR_NAME, pTabSaveBuf, sizeof( CHAR ), TabSaveSize, 0 );
            }

            /* 释放内存空间 */
            free( pTablePointer );
            /* 删除数据操作对象 */
            Hrf_Delete( TableSaveHandle );
        }
    }

    return( XJ_SUCCESS );
}

/* 获得各个模块的CRC信息 */
INT CCMC_GetModuleCrcInf( INT * pModuleNum, VOID * * pRunTextCb )
{
    * pModuleNum = RunTextNumber;
    * pRunTextCb = ( VOID * )RunTextInf;
    return( NU_SUCCESS );
}
