
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
#define CCMC_TABLE_CRC_DIR_NAME  "/user/pdevTemplateInf" /* װ��ģ����Ϣ·�� */
#define CCMC_IDENT_TABLE         ".dataTable" /* ���ݱ��ʶ���� */

#define CCMC_GET_USER_EVENT_GROUPS  (PLAT_cfg.history.UserEventGroups)        /* ÿͨ���Ĳ���ֵ���� */
#define CSMV_GET_CHANNEL_POINT      (PLAT_cfg.smvc.ChannelPoint)              /* ÿͨ���Ĳ���ֵ���� */

/* �豸������Ϣ�ṹ */
typedef struct DEV_MONITOR_STRUCT
{
    /* SV������Ϣ */
    /* ����SV����֡�� */
    UNSIGNED * SV_rxd_frames_number;
    /* ����SVͨ������ */
    UNSIGNED * SV_rxd_channel_number;
    /* ����SV��Ŵ������ */
    UNSIGNED * SV_rxd_serial_error_count;
    /* ����SV����ʱ�� */
    UNSIGNED * SV_rxd_time;
    /* ����SVԭʼ���� */
    float * SV_rxd_data;
    /* GOOSE������Ϣ */
    /* ����GOOSE����֡�� */
    UNSIGNED * GOOSE_rxd_frames_number;
    /* ����GOOSE���� */
    UNSIGNED * GOOSE_rxd_group_number;
    /* ����GOOSE״̬ */
    UNSIGNED * GOOSE_rxd_status;
    /* ����GOOSEƷ�� */
    UNSIGNED * GOOSE_rxd_quality;
    /* ����GOOSE���� */
    UNSIGNED * GOOSE_rxd_repair;
    /* ����GOOSE����ʱ�� */
    UNSIGNED * GOOSE_rxd_time;
    /* ����GOOSE��������λ */
    UNSIGNED * GOOSE_rxd_masks_group;
    /* SNTP��ʱ������Ϣ */
    /* ����SNTP����֡�� */
    UNSIGNED * SNTP_rxd_frames_number;
    /* ����SNTP���ĳ��ȴ������ */
    UNSIGNED * SNTP_rxd_length_error;
    /* ����SNTP������ݳ��� */
    UNSIGNED * SNTP_rxd_year_error;
    /* ��ʱ��GOOSE������Ϣ */
    /* ���մ�ʱ��GOOSE����֡�� */
    UNSIGNED * T_GOOSE_rxd_frames_number;
    /* ���մ�ʱ��GOOSE���ȴ��� */
    UNSIGNED * T_GOOSE_rxd_length_error;
}DEV_MONITOR;

/* ����ΪOS������� */
#ifdef WIN32
/* ȫ�ֱ������� */
/* Windows �궨�� */
#define CCMC_HMI_TEXT_FILE_NAME  "NPU808HMI.elf" /* HMI����洢·�� */
#define CCMC_GWS_TEXT_FILE_NAME  "NPU808Gws.elf" /* GWS����洢·�� */
#define CCMC_PLAT_TEXT_FILE_NAME "NPU808Plat.elf" /* Plat����洢·�� */
#define CCMC_PRO_TEXT_FILE_NAME  "NPU808Pro.elf"  /* Pro����洢·�� */
#define CCMC_DRV_TEXT_FILE_NAME  "NPU808Drv.elf"  /* ��������洢·�� */
#endif

#ifndef WIN32 /* ��windowsȱʡ����һЩ���Ա�ͨ������ */
/* ȫ�ֱ������� */
/* Nucleus�궨�� */
#define CCMC_HMI_TEXT_FILE_NAME   "/bin/NPU808HMI.elf"  /* HMI����洢·�� */
#define CCMC_GWS_TEXT_FILE_NAME   "/bin/NPU808Gws.elf"  /* GWS����洢·�� */
#define CCMC_PLAT_TEXT_FILE_NAME  "/bin/NPU808Plat.elf" /* Plat����洢·�� */
#define CCMC_PRO_TEXT_FILE_NAME   "/bin/NPU808Pro.elf"  /* Pro����洢·�� */
#define CCMC_DRV_TEXT_FILE_NAME   "/bin/NPU808Drv.elf"  /* ��������洢·�� */
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

/* װ�ü�����Ϣ�б� */
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
    /* SV������Ϣ */
    & CSMV_MsgCount.iRecSmvCount[0],                /* SV_rxd_frames_number */
    & CSMV_MsgCount.iRecSmvCount[1],                /* SV_rxd_channel_number */
    & CSMV_MsgCount.iRecSmvCount[2],                /* SV_rxd_serial_error_count */
    & CSMV_Rxd_Time_SV,                             /* SV_rxd_time */
    & CSMC_Analog[0],                               /* SV_rxd_data */
    /* GOOSE������Ϣ */
    & CSMV_MsgCount.iGooseRecCount[0],              /* GOOSE_rxd_frames_number */
    & CSMV_MsgCount.iGooseRecCount[1],              /* GOOSE_rxd_group_number */
    PLAT_SoftInputMemory,                           /* GOOSE_rxd_status */
    PLAT_SoftInputQuality,                          /* GOOSE_rxd_quality */
    PLAT_SoftInputCheckQuality,                     /* GOOSE_rxd_repair */
    & CSMV_Rxd_Time_GOOSE,                          /* GOOSE_rxd_time */
    CINC_SoftInFullMasks,                           /* GOOSE_rxd_masks_group */
    /* SNTP��ʱ������Ϣ */
    & CSMV_MsgCount.iRecSynCount,                   /* SNTP_rxd_frames_number */
    & CSMV_MsgCount.iRecSynLenErrCount[0],          /* SNTP_rxd_length_error */
    & CSMV_MsgCount.iRecSynYearErrCount[0],         /* SNTP_rxd_year_error */
    /* ��ʱ��GOOSE������Ϣ */
    & CSMV_MsgCount.iRecGooseTimeCount,             /* T_GOOSE_rxd_frames_number */
    & CSMV_MsgCount.iRecGooseTimeLenErrCount,       /* T_GOOSE_rxd_length_error */
};

/* �ڲ��¼��洢·�� */
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
volatile INT CCMC_PlatLeadFlag = XJ_FALSE;  /* ƽ̨������ʶ */
volatile UNSIGNED CCMC_ModelInitCount = 0;  /* װ�÷�ģ���ʼ�������� */
UNSIGNED RunTextNumber = 0;
RUN_TEXT_CB RunTextInf[ CCMC_MAX_RUN_TEXT + 1 ]; /* ���г�����Ϣ������ */

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
            /* ����������� */
            status = XJP_Config_Initialize(( HFGM )CTable.ConfigId, CPC_CONFIG_TIMEOUT );
            if( status != NU_TIMEOUT )
            {
                RepairTabList = 0;
                GroupNumber ++;
                /* ������Ϣ�仯 */
                sprintf( EventName, " %03d", LS_RES_EVENT_CONFIG_CHANGE );
                CEMC_FillEvent( EventName, RESERVE_EVENT_ID );
            }
        }
        else if( GroupNumber == 1 ) /* save table config data */
        {
            status = XJP_DS_Save_Field_Value_Config( & RepairTabList, CPC_CONFIG_TIMEOUT );
            if( status != NU_TIMEOUT )
            {
                /* �ǳ�ʱ״̬�����ع̻���� */
                if( status == NU_SUCCESS )
                {
                    /* �̻���ȷ */
                    NU_Set_Events(& Event_2_CB, (~ CPC_EVENT2_CFG_ERROR ), NU_AND );
                    CEMC_FillEvent( "�����޸�������", 0x00 );
                }
                else
                {
                    /* �̻�ʧ�� */
                    NU_Set_Events(& Event_2_CB, CPC_EVENT2_CFG_ERROR, NU_OR );
                    CEMC_FillEvent( "�����޸ĳ�������������", 0x00 );
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

/* �����豸����������� */
VOID CCMC_LoadNVdata( VOID )
{
    INT iStatus;
    CEVENT * pEventGroup;
    INT Number;
    UNSIGNED buf[32];    /* ��ʱ������ */
    INT text_change;
    UNSIGNED verL, verH, toolVer;
    UNSIGNED CrcCode;
    INT i;
    UNSIGNED_CHAR ucStatus;
    CREPORT * pReportHandle;

#if 0
    /* RAM �Լ�.  */
    if( Ram_Check_Flag )
    {
        NU_Set_Events(& Event_1_CB, PRO_EVENT1_ERR_RAM, NU_OR );
    }
    else
    {
        NU_Set_Events(& Event_1_CB,( UNSIGNED )(~ PRO_EVENT1_ERR_RAM ), NU_AND );
    }
#endif
    /* ��ʼ�����г�����Ϣ */
    memset( RunTextInf, 0, sizeof( RunTextInf ));

    /* ������������� */
    CRMC_Create( 1, CPC_MAX_NVRAM_REPORT_NUMBER, CPC_REPORT_DIR_NAME );

    /* �û��¼�ʵ�������� */
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

    /* ����HMI�����CRC�� */
    XJP_ELF_File_CRC16( CCMC_HMI_TEXT_FILE_NAME, & CCMC_hmiCRC );
    /* HMI ���г�����Ϣ */
    RunTextInf[ RunTextNumber ].CrcCode = CCMC_hmiCRC;
    strncpy( RunTextInf[ RunTextNumber ].Name, CCMC_HMI_TEXT_FILE_NAME, CCMC_FILE_NAME_LEN ); /* ��¼�ļ����� */
    if( MPFC_GetHmiVersion( & verL, & verH, & toolVer ) == NU_SUCCESS )
    {
        RunTextInf[ RunTextNumber ].Version = ( verL & CPC_DATA_MASKS8 )
            + (( verH & CPC_DATA_MASKS8 ) << CPC_DATA_BITS8 );
    }
    RunTextNumber ++;

    /* �������س����CRC�� */
    XJP_ELF_File_CRC16( CCMC_GWS_TEXT_FILE_NAME, & CCMC_gwsCRC );
    /* ��¼�������г�����Ϣ */
    RunTextInf[ RunTextNumber ].CrcCode = CCMC_gwsCRC;
    strncpy( RunTextInf[ RunTextNumber ].Name, CCMC_GWS_TEXT_FILE_NAME, CCMC_FILE_NAME_LEN ); /* ��¼�ļ����� */
    if( GPFC_GetGwsVersion( & verL, & verH ) == NU_SUCCESS )
    {
        RunTextInf[ RunTextNumber ].Version = ( verL & CPC_DATA_MASKS8 )
            + (( verH & CPC_DATA_MASKS8 ) << CPC_DATA_BITS8 );
    }
    RunTextNumber ++;

    /* ����ƽ̨�����CRC�� */
    CrcCode = CPC_CRC16_Initialize;
    CCMC_platCRC = CPC_CRC16_Initialize;
    XJP_ELF2_Calculate_CRC16( CCMC_PLAT_TEXT_FILE_NAME, CCMC_platCRC, & CCMC_platCRC,
        CrcCode, & CrcCode );
    CrcCode = ( CrcCode ^ CPC_CRC16_MASKS );
    /* ��¼ƽ̨���г�����Ϣ */
    RunTextInf[ RunTextNumber ].CrcCode = CrcCode;
    strncpy( RunTextInf[ RunTextNumber ].Name, CCMC_PLAT_TEXT_FILE_NAME, CCMC_FILE_NAME_LEN ); /* ��¼�ļ����� */
    RunTextInf[ RunTextNumber ].Version = VER_PLATFORM;
    RunTextNumber ++;

    /* ������������CRC�� */
    CrcCode = CPC_CRC16_Initialize;
    XJP_ELF2_Calculate_CRC16( CCMC_DRV_TEXT_FILE_NAME, CCMC_platCRC, & CCMC_platCRC,
        CrcCode, & CrcCode );
    CrcCode = ( CrcCode ^ CPC_CRC16_MASKS );
    CCMC_devCRC = CCMC_platCRC;
    /* ��¼�������г�����Ϣ */
    RunTextInf[ RunTextNumber ].CrcCode = CrcCode;
    strncpy( RunTextInf[ RunTextNumber ].Name, CCMC_DRV_TEXT_FILE_NAME, CCMC_FILE_NAME_LEN ); /* ��¼�ļ����� */
    RunTextInf[ RunTextNumber ].Version = VER_PLATFORM;
    RunTextNumber ++;

    /* ���㱣������CRC�� */
    CrcCode = CPC_CRC16_Initialize;
    XJP_ELF2_Calculate_CRC16( CCMC_PRO_TEXT_FILE_NAME, CCMC_devCRC, & CCMC_devCRC,
        CrcCode, & CrcCode );
    CrcCode = ( CrcCode ^ CPC_CRC16_MASKS );
    CCMC_platCRC = ( CCMC_platCRC ^ CPC_CRC16_MASKS );
    CCMC_devCRC = ( CCMC_devCRC ^ CPC_CRC16_MASKS );
    /* ��¼����������Ϣ */
    RunTextInf[ RunTextNumber ].CrcCode = CrcCode;
    strncpy( RunTextInf[ RunTextNumber ].Name, CCMC_PRO_TEXT_FILE_NAME, CCMC_FILE_NAME_LEN ); /* ��¼�ļ����� */
    if( APFC_GetDeviceVersion( & verL, & verH ) == NU_SUCCESS )
    {
        RunTextInf[ RunTextNumber ].Version = ( verL & CPC_DATA_MASKS8 )
            + (( verH & CPC_DATA_MASKS8 ) << CPC_DATA_BITS8 );
    }
    RunTextNumber ++;

    text_change = NU_TRUE;
    /* ��ȡƽ̨�����س����CRC�� */
    if( XJP_File_Read( CPC_TEXT_CRC_DIR_NAME, buf, sizeof( UNSIGNED ), 2, 0 ) == XJ_SUCCESS ) /* 2���ֵ�CRC�� */
    {
        if( CCMC_devCRC == buf[0] )
        {
            /* ����δ�����仯 */
            text_change = NU_FALSE;
        }
        if( CCMC_gwsCRC != buf[1] )
        {
            /* ��д�澯�¼� */
            CEMC_FillEvent( "ECMͨѶģ�����У���", ALARM_EVENT_ID );
        }
    }
    else
    {
        /* ��д�澯�¼� */
        CEMC_FillEvent( "ECMͨѶģ�����У���", ALARM_EVENT_ID );
    }

    if(( text_change == XJ_TRUE ) || ( CCMC_devCRC != buf[0] ) || ( CCMC_gwsCRC != buf[1] ))
    {
        /* �³��򣬱������CRC�� */
        buf[0] = CCMC_devCRC;
        buf[1] = CCMC_gwsCRC;
        XJP_File_Save( CPC_TEXT_CRC_DIR_NAME, buf, sizeof( UNSIGNED ), 2, 0 );
    }

    /* �û����ݳ�ʼ��(����CRC�Ƿ����仯�����û����ݴӻ������ж�ȡ����) */
    CUDC_UserDataInitialize( NU_NO_SUSPEND );
    /* ��������� */
    pReportHandle = CRMC_GetReportHandle( 1 );

    /* Determine if the first run.  */
    if( text_change == NU_TRUE )
    {
        /* �����³���,���ó���У������־ */
        NU_Set_Events(& Event_1_CB, CPC_EVENT1_ERR_PROGRAM, NU_OR );
        /* ��д�澯�¼� */
        CEMC_FillEvent( "��������У�����", ALARM_EVENT_ID );

        /* ���ݱ��ʼ�� */
        XJP_DS_Initialize_SetData( 1, NU_NO_SUSPEND );

        /* �澯�¼���ʼ�� */
        CAlarmEvent.Initialize(& CAlarmEvent, 1, NU_NO_SUSPEND );

        /* �ڲ��¼���ʼ�� */
        CReserveEvent.Initialize(& CReserveEvent, 1, NU_NO_SUSPEND );

        /* �����¼���ʼ�� */
        COperateEvent.Initialize(& COperateEvent, 1, NU_NO_SUSPEND );

        /* �û��¼���ʼ�� */
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
        /* �����ʼ�� */
        if( pReportHandle )
            pReportHandle->Initialize( pReportHandle, 1, NU_NO_SUSPEND );

        /* ¼����ʼ�� */
        CWave.Initialize(& CWave, 1, NU_NO_SUSPEND );
    }
    else
    {
        /* ���ݱ��ʼ�� */
        iStatus = XJP_DS_Initialize_SetData( 0, NU_NO_SUSPEND );
        if( iStatus != NU_SUCCESS )
        {
            //            NU_Set_Events(& Event_1_CB, PRO_EVENT1_ERR_SRAM, NU_OR );
        }

        /* �澯�¼���ʼ�� */
        CAlarmEvent.Initialize(& CAlarmEvent, 0, NU_NO_SUSPEND );

        /* �ڲ��¼���ʼ�� */
        CReserveEvent.Initialize(& CReserveEvent, 0, NU_NO_SUSPEND );

        /* �����¼���ʼ�� */
        COperateEvent.Initialize(& COperateEvent, 0, NU_NO_SUSPEND );

        /* �û��¼���ʼ�� */
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

        /* �����ʼ�� */
        if( pReportHandle )
            pReportHandle->Initialize( pReportHandle, 0, NU_NO_SUSPEND );

        /* ¼����ʼ�� */
        CWave.Initialize(& CWave, 0, NU_NO_SUSPEND );
    }

    /* ��鶨ֵ״̬ */
    if( XJP_DS_Get_Table_Status( TAB_SETTING_ID, & ucStatus ) == NU_SUCCESS )
    {
        if( ucStatus & TABLE_ERR_LEAD_SG )
        {
            NU_Set_Events(& Event_1_CB, CPC_EVENT1_ERR_SETTING, NU_OR );
        }
    }
    /* ���ݱ����ݱ��� */
    CCMC_TableDataSave();
    /* �������ڱ���λ���� */
    {
        UNSIGNED uHoldBits[CPC_MAX_OUTPUT_GROUPS];

        /* ��ȡ��������λ״̬ */
        /* �����ڱ���λ���� */
        iStatus = XJP_Output_Hold_Read( uHoldBits, 0,
            CPC_MAX_OUTPUT_GROUPS, NU_NO_SUSPEND );
        /* �жϻ�ȡ�����Ƿ�ɹ� */
        if( iStatus == NU_SUCCESS )
        {
            /* ����ɹ�,�����ݴ�����ڱ���λ������ */
            for( i = 0; i < CPC_MAX_OUTPUT_GROUPS; i ++)
            {
                XJP_IO_Set_Hold_Relay( i, uHoldBits[i] );
                /* ���ó��ڼ̵�������λ */
                OUTC_Check_SetPickupBits( i, uHoldBits[i] );
            }
        }
    }
}

/* ������ڱ���λ���� */
VOID CCMC_SaveOutputHoldBits( VOID )
{
    static   INT InitFlag = 0;
    static   UNSIGNED uOldHoldBits[CPC_MAX_OUTPUT_GROUPS];
    UNSIGNED uNewHoldBits[CPC_MAX_OUTPUT_GROUPS];
    INT      iStatus;
    int      i;
    //--------------------------------------------------------------------------
    /* �����ݻ����� */
    memset( uNewHoldBits, 0,( sizeof( UNSIGNED )* CPC_MAX_OUTPUT_GROUPS ));
    /* װ�ó�ʼ�ϵ�ʱ,�������������ݸ��� */
    if( InitFlag == 0 )
    {
        /* �����ݻ����� */
        memset( uOldHoldBits, 0,( sizeof( UNSIGNED )* CPC_MAX_OUTPUT_GROUPS ));
        /* �����ڱ���λ���� */
        iStatus = XJP_Output_Hold_Read( uOldHoldBits, 0,
            CPC_MAX_OUTPUT_GROUPS, CPC_OUTHOLD_TIMEOUT );
        //if( iStatus == NU_SUCCESS )/* ���ļ������ڵ�ʱ��Ҳ����ʧ�� */
        {
            /* ��ʼ����� */
            InitFlag = 1;
        }
    }
    else
    {
        /* ��ȡ��ǰ���ڱ���λ��״̬ */
        for( i = 0; i < CPC_MAX_OUTPUT_GROUPS; i ++)
        {
            uNewHoldBits[i] = XJP_IO_Get_Hold_Relay( i );
        }
        /* �жϳ��ڱ���λ�Ƿ����仯,�Ա�����Ƿ���Ҫ����ǰ���ݹ̻� */
        for( i = 0; i < CPC_MAX_OUTPUT_GROUPS; i ++)
        {
            if( uOldHoldBits[i] != uNewHoldBits[i] )
            {
                /* �̻����ڱ���λ */
                iStatus = XJP_Output_Hold_Write( uNewHoldBits, 0,
                    CPC_MAX_OUTPUT_GROUPS, CPC_OUTHOLD_TIMEOUT );
                if( iStatus == NU_SUCCESS )
                {
                    /* ����ԭ���ڱ���λ״̬ */
                    memcpy( uOldHoldBits, uNewHoldBits,( sizeof( UNSIGNED )* CPC_MAX_OUTPUT_GROUPS ));
                }
                break;
            }
        }
    }
}

/* ����ƽ̨������ʶ */
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

/* ��ȡƽ̨������ʶ */
INT CCMC_GetPlatLeadFlag( VOID )
{
    return( CCMC_PlatLeadFlag );
}

/* ����װ�÷�ģ���ʼ���ɹ����� */
INT CCMC_SetModInitSuccess( VOID )
{
    NU_Protect( pReport_Protect );
    CCMC_ModelInitCount += 1;
    NU_Unprotect();
    return( NU_SUCCESS );
}

/* ��ȡװ�÷�ģ���ʼ����ɸ��� */
INT CCMC_GetModInitCount( VOID )
{
    return( CCMC_ModelInitCount );
}

/* ���ݱ����ݱ��� */
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

    /* �����������ݵ�CRC�� */
    crc_code = CPC_CRC32_Initialize;

    XJP_CRC_Calculate( &( CTable.ConfigCrcCode ), sizeof( UNSIGNED ),
        ( 4 / sizeof( UNSIGNED )), CPC_CRC32_BITS, crc_code,& crc_code ); /* ����CRC�� */
    crc_code = ( crc_code ^ CPC_CRC32_MASKS );

    /* ���ݱ����ݱ仯��� */
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
        /* ɾ�������ļ� */
        if ( xj_FileExists( CCMC_TABLE_CRC_DIR_NAME ))
        {
            xj_RemoveFile( CCMC_TABLE_CRC_DIR_NAME );
        }
        /* ����ģ�ͱ仯�����±������ݱ����� */
        TableNumber = 0;
        /* ��ȡ���ݱ���� */
        TableNumber = XJP_DS_Get_Table_Number();
        /* �����ڴ�ռ� */
        pTablePointer = ( HTAB **)malloc( TableNumber * sizeof( HTAB *));
        if( pTablePointer )
        {
            /* �������ݱ����ݳߴ� */
            TableSectSize = 0;
            iVariable = XJP_DS_GetTablePointers( pTablePointer, TableNumber );
            for( k = 0; k < iVariable; k ++ )
            {
                pTable = ( TB_TCB *)(*( pTablePointer + k ));
                TableSectSize += ( pTable->DataNumber * pTable->Size );
            }
            /* ��������ģ������ */
            TableSaveHandle = Hrf_Create( 0, TableNumber, TableSectSize, crc_code );
            /* ��¼������Ϣ */
            CpuNumber = 1;
            iVariable = XJP_DS_GetTablePointers( pTablePointer, TableNumber );
            for( k = 0; k < iVariable; k ++ )
            {
                pTable = ( TB_TCB *)(*( pTablePointer + k ));
                Hrf_RecordSectorInf_Table( TableSaveHandle, ( pTable->DataNumber * pTable->Size ),
                    pTable->DataMemory, pTable->Index, ( unsigned char *)pTable->Name, CpuNumber, pTable->Size );
            }
            Hrf_RecordSectorHead( TableSaveHandle, ( unsigned char *)CCMC_IDENT_TABLE );
            /* �̻����ݱ����� */
            iStatus = Hrf_GetRecordInf( TableSaveHandle, & pTabSaveBuf, & TabSaveSize );
            if( iStatus == XJ_SUCCESS )
            {
                XJP_File_Save( CCMC_TABLE_CRC_DIR_NAME, pTabSaveBuf, sizeof( CHAR ), TabSaveSize, 0 );
            }

            /* �ͷ��ڴ�ռ� */
            free( pTablePointer );
            /* ɾ�����ݲ������� */
            Hrf_Delete( TableSaveHandle );
        }
    }

    return( XJ_SUCCESS );
}

/* ��ø���ģ���CRC��Ϣ */
INT CCMC_GetModuleCrcInf( INT * pModuleNum, VOID * * pRunTextCb )
{
    * pModuleNum = RunTextNumber;
    * pRunTextCb = ( VOID * )RunTextInf;
    return( NU_SUCCESS );
}
