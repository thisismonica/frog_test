
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
/*      lsc.c                                           Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      LS - Lonworks Communication to Supervisor                        */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the Lonworks            */
/*      Communication to Supervisor.                                     */
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
/*      ls_defs.h              Lonworks Communication to Supervisor      */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       02-20-2003      Created initial version 1.0      */
/*      YanJuanJiang    11-23-2004      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "plat/es_plat.h"
#include "plat/net_inc/ls_extr.h"
#include "plat/net_inc/lc_extr.h"
#include "plat_specific/plat_specific.h"
#include "plat/net_inc/lc_bcall.h"
#include "plat/net_inc/lsc_b6.h"
#include "plat/net_inc/rcc_defs.h"
#include "plat/inc/csmv_extr.h"
#include "plat/inc/cin_extr.h"
#include "plat/inc/ctb_extr.h"
#include "plat/inc/tab_defs.h"
#include "plat/inc/xj_out_check.h"
#include "plat/inc/apf_extr.h"
#include "plat/inc/mpf_extr.h"
#include "plat/inc/gpf_extr.h"
#include "plat/inc/devi_extr.h"
#include "plat/inc/xj_clog.h"
#include "plat/inc/xj_cmemc.h"

/* Define external inner-component global data references.  */

extern CEVENT  CAlarmEvent;
extern CEVENT  CReserveEvent;
extern CEVENT  COperateEvent;
extern CEVENT  PRO_EventRecord[];
extern CREPORT CReport;
extern CWAVE   CWave;

extern NU_TIMER Timer_Trip_CB;
extern NU_TIMER Timer_ResetRelay_CB;
/* Define internal function calls.  */

extern VOID Prog_Execute_Debug_Program( UNSIGNED );
extern VOID Fcopy_Lead_Debug_Program( VOID );

/* ����������Ϣ */
extern float FOURIER01_R[ ];
extern float FOURIER01_I[ ];
extern float FOURIER02_R[ ];
extern float FOURIER02_I[ ];
extern float FOURIER03_R[ ];
extern float FOURIER03_I[ ];
extern float FOURIER05_R[ ];
extern float FOURIER05_I[ ];

/* ����ΪOS������� */
#ifdef WIN32
#endif

#ifndef WIN32 /* ��windowsȱʡ����һЩ���Ա�ͨ������ */
#endif

/* ƽ̨���ڹ����� */
extern HIOM PlatHIOM;

CHAR * LSC_CommChl[] =
{
    "",
    "menu",
    "print",
    "pc",
    "gws",
    "comtrade",
    "103",
    "",
};

/* ע��ͨ������ */
VOID LSC_RegisterCommand_Local( GW_CHANNEL * pChl )
{
    /* ���ȼ���������ʼ������ */
    LSC_B6_ProirityInit();
#if SWITCH_IEC103_TI_M_81
    /* �ٻ�װ�û�����Ϣ */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_81, LSC_Load_Device_Inf );
#endif
#if SWITCH_IEC103_TI_M_82
    /* װ�ú������� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_82, LSC_Device_Call );
#endif
#if ( LC_LINK_HMI_PHY_COM )
#if SWITCH_IEC103_TI_M_90
    /* �����´�����Ԥ��״̬ */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_90, LSC_Program_Get_Ready );
#endif
#if SWITCH_IEC103_TI_M_91
    /* �����´�����״̬ */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_91, LSC_Program_Make_Ready );
#endif
#endif
#if SWITCH_IEC103_TI_M_96
    /* Ԥ���̻��������� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_96, LSC_Ready_Configure_Program );
#endif
#if SWITCH_IEC103_TI_M_97
    /* ִ�й̻��������� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_97, LSC_Execute_Configure_Program );
#endif
#if SWITCH_IEC103_TI_M_98
    /* ��ѯ�������ù̻���� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_98, LSC_Load_Configure_Program_State );
#endif
#if SWITCH_IEC103_TI_M_A0
    /* �ٻ����ݱ���ܸ��� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_A0, LSC_ACK_Datatab );
#endif
#if SWITCH_IEC103_TI_M_A1
    /* �ٻ����ݱ����Ϣ */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_A1, LSC_ACK_Datatab_Info );
#endif
#if SWITCH_IEC103_TI_M_A2
    /* �ٻ��ֶ���Ϣ */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_A2, LSC_ACK_Field_Info );
#endif
#if SWITCH_IEC103_TI_M_A3
    /* �ٻ��������ݵ��ܸ��� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_A3, LSC_ACK_Data_Number );
#endif
#if SWITCH_IEC103_TI_M_A4
    /* �ٻ���¼��ȫ���ֶ� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_A4, LSC_ACK_All_Field_Value );
#endif
#if SWITCH_IEC103_TI_M_A5
    /* �ٻ���¼�ĵ����ֶ� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_A5, LSC_ACK_Field_Value );
#endif
#if SWITCH_IEC103_TI_M_A6
    /* �ٻ���¼�Ķ���ֶ� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_A6, LSC_ACK_Fields_Value );
#endif
#if SWITCH_IEC103_TI_M_A7
#if 0 /* �ں�̨����ע�� */
    /* ���������ֶε�ֵ */
    LCC_RegcBackGroundComm( pChl, IEC103_TI_M_A7, LSC_Set_Config_Values );
#endif
#endif
    /* ��ѡ������÷��ں�̨�����е�ԭ��ΪAA�ŵ���̨������,������ֶ���·�ѡ����������� */
#if SWITCH_IEC103_TI_M_A8
#if 0 /* �ں�̨����ע�� */
    /* ѡ��༭�ֶ� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_A8, LSC_Slecet_Edit_SG );
#endif
#endif
#if SWITCH_IEC103_TI_M_A9
#if 0 /* �ں�̨����ע�� */
    /* ���ñ༭��ֵ���ֵ */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_A9, LSC_Set_Edit_SGV );
#endif
#endif
#if SWITCH_IEC103_TI_M_AA
#if 0 /* �ں�̨����ע�� */
    /* ȷ�ϱ༭��ֵ���ֵ */
    LCC_RegcBackGroundComm( pChl, IEC103_TI_M_AA, LSC_Conform_Edit_SGV );
#endif
#endif
#if SWITCH_IEC103_TI_M_AB
    /* ��ȡ���ݱ��״̬ */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_AB, LSC_Get_Tab_Status );
#endif
#if SWITCH_IEC103_TI_M_AC
    /* ��ȡ���ݱ������CRC�� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_AC, LSC_Get_Config_Tab_CRC );
#endif
#if SWITCH_IEC103_TI_M_AD
    /* �������ݱ������ */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_AD, LSC_Freeze_Table );
#endif
#if SWITCH_IEC103_TI_M_AE
    /* �ⶳ���ݱ������ */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_AE, LSC_UnFreeze_Table );
#endif
#if SWITCH_IEC103_TI_M_AF
#if 0 /* �ں�̨����ע�� */
    /* ���ö�������ֶε�ֵ */
    LCC_RegcBackGroundComm( pChl, IEC103_TI_M_AF, LSC_Set_Batch_Config_Values );
#endif
#endif
#if SWITCH_IEC103_TI_M_B0
#if 0 /* �ں�̨����ע�� */
    /* �����������ֶε�ֵ */
    LCC_RegcBackGroundComm( pChl, IEC103_TI_M_B0, LSC_Save_Batch_Config_Values );
#endif
#endif
#if SWITCH_IEC103_TI_M_C0
    /* �ٻ��¼����� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C0, LSC_Load_Event_Type );
#endif
#if SWITCH_IEC103_TI_M_C1
#if 0 /* �ں�̨����ע�� */
    /* �ٻ��¼���Ϣ */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C1, LSC_Load_Event_Inf, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_C2
#if 0 /* �ں�̨����ע�� */
    /* �ٻ��¼������� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C2, LSC_Load_Event_Serial, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_C3
#if 0 /* �ں�̨����ע�� */
    /* �ٻ��¼����� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C3, LSC_Load_Event_Data, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_C4
#if 0 /* �ں�̨����ע�� */
    /* ����¼����� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C4, LSC_Clear_Alarm_Event, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_C5
#if 0 /* �ں�̨����ע�� */
    /* �ٻ�������Ϣ */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C5, LSC_Load_Report_Inf, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_C6
#if 0 /* �ں�̨����ע�� */
    /* �ٻ����������� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C6, LSC_Load_Report_FaultSerl, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_C7
#if 0 /* �ں�̨����ע�� */
    /* �ٻ������������ */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C7, LSC_Load_Report_Serial, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_C8
#if 0 /* �ں�̨����ע�� */
    /* �ٻ��������� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C8, LSC_Load_Report_Data, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_C9
#if 0 /* �ں�̨����ע�� */
    /* ������ϼ�¼���� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C9, LSC_Clear_FaultData, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_CA
    /* �ֶ�����¼�� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_CA, LSC_Start_Wave );
#endif
#if SWITCH_IEC103_TI_M_CB
#if 0 /* �ں�̨����ע�� */
    /* �ٻ�¼����� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_CB, LSC_Load_Wave_ID, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_CC
#if 0 /* �ں�̨����ע�� */
    /* �ٻ�¼��ʱ�� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_CC, LSC_Load_Wave_Time, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_CD
#if 0 /* �ں�̨����ע�� */
    /* �ٻ�������Ϣ */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_CD, LSC_Load_Wave_Inf, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_CE
#if 0 /* �ں�̨����ע�� */
    /* �ٻ����������� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_CE, LSC_Load_Wave_FaultSerl, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_E0
    /* Ԥ���������� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_E0, LSC_Trip_Test_Ready );
#endif
#if SWITCH_IEC103_TI_M_E1
    /* ִ�д������� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_E1, LSC_Trip_Test );
#endif
#if SWITCH_IEC103_TI_M_E2
    /* ��ͨ��ϵ������ֵ */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_E2, LSC_Load_Channel_Cofficient );
#endif
#if SWITCH_IEC103_TI_M_E3
    /* �޸�ͨ��ϵ�� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_E3, LSC_Change_Channel_Cofficient );
#endif
#if SWITCH_IEC103_TI_M_E4
    /* ��ͨ���ֵ */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_E4, LSC_Load_Channel_Rate );
#endif
#if SWITCH_IEC103_TI_M_E5
    /* �޸�ͨ���ֵ */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_E5, LSC_Change_Channel_Rate );
#endif
#if SWITCH_IEC103_TI_M_E6
    /* ��װ����Ư�̶ȼ���Ưֵ */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_E6, LSC_Load_Channel_Excursion );
#endif
#if SWITCH_IEC103_TI_M_E7
    /* �޸�װ����Ư */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_E7, LSC_Change_Channel_Excursion );
#endif
#if SWITCH_IEC103_TI_M_EA
    /* �ϵ�ģ��ͨ����λ��ֵ */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_EA, LSC_Load_Channel_Phase );
#endif
#if SWITCH_IEC103_TI_M_E9
    /* �޸�ͨ����λ��� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_E9, LSC_Change_Channel_Phase );
#endif
#if SWITCH_IEC103_TI_M_F0
    /* �źŸ��� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_F0, LSC_Reset );
#endif
#if SWITCH_IEC103_TI_M_F1
    /* �ٻ�������״̬��GPS������� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_F1, LSC_Load_Input_Status );
#endif
#if SWITCH_IEC103_TI_M_F7
    /* ���ڴ����� */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_F7, LSC_Read_Memory );
#endif
}

#if SWITCH_IEC103_TI_M_81
/* �ٻ�װ�û�����Ϣ */
INT LSC_Load_Device_Inf( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR * pSrc,* pDst,* p;
    UNSIGNED      uTemp;
    UNSIGNED      verL, verH, toolCRC;
    //--------------------------------------------------------------------------
    LS_UNUSED_PARAM( pChl );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 27;                                      /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_81;                          /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );      /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                           /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                           /* ���кŸ��ֽ� */

    /* װ��CRC�� */
    uTemp = LS_GET_DEV_CRC;
    p = ( UNSIGNED_CHAR *)& uTemp;
    LS_WORD_TO_BYTE( pDst, p, 2 );                      /* װ��CRC��ת����2�ֽ� */
    pDst += 2;                                          /* 2�ֽ�CRC�� */

    /* װ�ð汾�� */
    if( APFC_GetDeviceVersion(& verL, & verH ) == NU_SUCCESS )
    {
        * pDst ++ = ( UNSIGNED_CHAR )( verL & LS_DATA_MASKS8 ); /* װ�ð汾�ŵ��ֽ� */
        * pDst ++ = ( UNSIGNED_CHAR )( verH & LS_DATA_MASKS8 ); /* װ�ð汾�Ÿ��ֽ� */
    }
    else
    {
        * pDst ++ = 0;                                  /* װ�ð汾�ŵ��ֽ� */
        * pDst ++ = 0;                                  /* װ�ð汾�Ÿ��ֽ� */
    }

    /* ƽ̨CRC�� */
    uTemp = LS_GET_PLAT_CRC;
    p = ( UNSIGNED_CHAR *)& uTemp;
    LS_WORD_TO_BYTE( pDst, p, 2 );                      /* ƽ̨CRC��ת����2�ֽ� */
    pDst += 2;                                          /* 2�ֽ�CRC�� */

    /* ƽ̨�汾�� */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_PLATFORM_VER & LS_DATA_MASKS8 );
    * pDst ++ = ( UNSIGNED_CHAR )(( LS_PLATFORM_VER >> LS_BYTE_BITS ) & LS_DATA_MASKS8 );

    if( MPFC_GetHmiVersion(& verL, & verH, & toolCRC ) == NU_SUCCESS )
    {
        /* �˻��ӿ����ù���CRC�� */
        p = ( UNSIGNED_CHAR *)& toolCRC;
        LS_WORD_TO_BYTE( pDst, p, 2 );                      /* �˻��ӿ�CRC��ת����2�ֽ� */
        pDst += 2;                                          /* �˻��ӿ�CRC��ת����2�ֽ� */
        /* �˻��ӿ�CRC�� */
        uTemp = LS_GET_HMI_CRC;
        p = ( UNSIGNED_CHAR *)& uTemp;
        LS_WORD_TO_BYTE( pDst, p, 2 );                      /* �˻��ӿ�CRC��ת����2�ֽ� */
        pDst += 2;                                          /* 2�ֽ�CRC�� */
        /* �˻��ӿڰ汾�� */
        * pDst ++ = ( UNSIGNED_CHAR )( verL & LS_DATA_MASKS8 ); /* �˻��ӿڰ汾�ŵ��ֽ� */
        * pDst ++ = ( UNSIGNED_CHAR )( verH & LS_DATA_MASKS8 ); /* �˻��ӿڰ汾�Ÿ��ֽ� */
    }
    else
    {
        /* �˻��ӿ����ù���CRC�� */
        * pDst ++ = 0;                                  /* �˻��ӿ�CRC����ֽ� */
        * pDst ++ = 0;                                  /* �˻��ӿ�CRC����ֽ� */
        /* �˻��ӿ�CRC�� */
        uTemp = LS_GET_HMI_CRC;
        p = ( UNSIGNED_CHAR *)& uTemp;
        LS_WORD_TO_BYTE( pDst, p, 2 );                      /* �˻��ӿ�CRC��ת����2�ֽ� */
        pDst += 2;                                      /* �˻��ӿ�CRC��ת����2�ֽ� */
        /* �˻��ӿڰ汾�� */
        * pDst ++ = 0;                                  /* �˻��ӿڰ汾�ŵ��ֽ� */
        * pDst ++ = 0;                                  /* �˻��ӿڰ汾�Ÿ��ֽ� */
    }

    /* ����CRC�� */
    uTemp = LS_GET_GWS_CRC;
    p = ( UNSIGNED_CHAR *)& uTemp;
    LS_WORD_TO_BYTE( pDst, p, 2 );                      /* ����CRC��ת����2�ֽ� */
    pDst += 2;                                          /* 2�ֽ�CRC�� */

    /* ���ذ汾�� */
    if( GPFC_GetGwsVersion(& verL, & verH ) == NU_SUCCESS )
    {
        * pDst ++ = ( UNSIGNED_CHAR )( verL & LS_DATA_MASKS8 ); /* ���ذ汾�ŵ��ֽ� */
        * pDst ++ = ( UNSIGNED_CHAR )( verH & LS_DATA_MASKS8 ); /* ���ذ汾�Ÿ��ֽ� */
    }
    else
    {
        * pDst ++ = 0;                                  /* ���ذ汾�ŵ��ֽ� */
        * pDst ++ = 0;                                  /* ���ذ汾�Ÿ��ֽ� */
    }

    /* ͨ�Ű汾�� */
    * pDst ++  = ( UNSIGNED_CHAR )( LS_COMMU_VER & LS_DATA_MASKS8 );
    * pDst ++  = ( UNSIGNED_CHAR )(( LS_COMMU_VER >> LS_BYTE_BITS ) & LS_DATA_MASKS8 );

    * pDst ++= ( UNSIGNED_CHAR )( LS_SET_ZONE_COUNT );  /* װ�ö�ֵ������ */

    /* CPLD�汾�� */
    uTemp = DEVI_GetCPLDVersion();
    p = ( UNSIGNED_CHAR *)& uTemp;
    LS_WORD_TO_BYTE( pDst, p, 2 );                      /* CPLD�汾��ת����2�ֽ� */
    pDst += 2;                                          /* 2�ֽ�CPLD�汾�� */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_82
/* װ�ú������� */
INT LSC_Device_Call( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT           Status;
    UNSIGNED_CHAR * pSrc,* pDst;
    UNSIGNED      LedFrequency, LedStatus, LedDelay;
    //--------------------------------------------------------------------------
    LS_UNUSED_PARAM( pChl );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 9;                                       /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_82;                          /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );      /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                           /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                           /* ���кŸ��ֽ� */
    * pDst ++ = *( pSrc + 5 );                           /* ��˸Ƶ�� */
    * pDst ++ = *( pSrc + 6 );                           /* ��˸ʱ����ֽ� */
    * pDst ++ = *( pSrc + 7 );                           /* ��˸ʱ����ֽ� */

    LedFrequency = *( pSrc + 5 ) & LS_DATA_MASKS8;       /* ��˸Ƶ�� */
    /* ��˸ʱ�� */
    LedDelay = (*( pSrc + 6 ) & LS_DATA_MASKS8 );                    /* ��˸ʱ����ֽ� */
    LedDelay += ((*( pSrc + 7 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* ��˸ʱ����ֽ� */
    if( LedFrequency >= 100 ) /* ���� */
        LedStatus = CLE_LED_LIGHT;
    else if( LedFrequency == 0 ) /* Ϩ�� */
        LedStatus = CLE_LED_EXTINGUISH;
    else
        LedStatus = CLE_LED_CORUSCATE;
    /* ����װ�ú������е�״̬ */
    Status = XJP_Led_SetDevLedCallStatus( NU_TRUE, LedDelay, LedStatus, LedFrequency );
    * pDst ++ = ( Status & LS_DATA_MASKS8 );               /* ����״̬ */

    return( COM_DATA_FULL );
}
#endif

#if ( LC_LINK_HMI_PHY_COM )
#if SWITCH_IEC103_TI_M_90
/* �����´�����Ԥ��״̬ */
INT LSC_Program_Get_Ready( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    //--------------------------------------------------------------------------

    LS_SET_PROG_READY();

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 5;                        /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_90;           /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU���� */
    * pDst ++ = *( pSrc + 3 );            /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );            /* ���кŸ��ֽ� */
    * pDst ++ = LS_RESULT_TRUE;           /* ִ�н�� */

    return( COM_DATA_FULL );
}
#endif
#endif

#if ( LC_LINK_HMI_PHY_COM )
#if SWITCH_IEC103_TI_M_91
/* �����´�����״̬ */
INT LSC_Program_Make_Ready( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        ActualEvent;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );

    if( LS_GET_PROG_READY(& ActualEvent ) == NU_SUCCESS )
    {
        LS_CLR_PROG_READY();
        Fcopy_Lead_Debug_Program();
        Prog_Execute_Debug_Program( code );
    }
    else
    {
        pSrc = ( UNSIGNED_CHAR *)Src;
        pDst = ( UNSIGNED_CHAR *)Dst;

        * pDst ++ = 5;                                        /* ��·�û����ݳ��� */
        * pDst ++ = IEC103_TI_M_91;                           /* ����ʶ */
        * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );       /* CPU���� */
        * pDst ++ = *( pSrc + 3 );                            /* ���кŵ��ֽ� */
        * pDst ++ = *( pSrc + 4 );                            /* ���кŸ��ֽ� */
        * pDst ++ = LS_RESULT_NULL;                           /* ִ�н�� */
    }

    return( COM_DATA_FULL );
}
#endif
#endif

#if SWITCH_IEC103_TI_M_96
/* Ԥ���̻��������� */
INT LSC_Ready_Configure_Program( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    LS_SET_CFG_READY();

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 5;                                  /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_96;                     /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                      /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                      /* ���кŸ��ֽ� */
    * pDst ++ = LS_RESULT_TRUE;                     /* ִ�н�� */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_97
/* ִ�й̻��������� */
INT LSC_Execute_Configure_Program( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        ActualEvent;
    INT             iStatus = LS_RESULT_NULL;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    if( LS_GET_CFG_READY(& ActualEvent ) == NU_SUCCESS )
    {
        LS_CLR_CFG_READY();       /* �������Ԥ��̬ */
        LS_SET_CFG_PROGRAM();     /* �������ù̻�״̬ */
        iStatus = LS_RESULT_TRUE;
    }

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 5;                                        /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_97;                           /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );       /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                            /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                            /* ���кŸ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )iStatus;                 /* ִ�н�� */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_98
/* ��ѯ�������ù̻���� */
INT LSC_Load_Configure_Program_State( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        ActualEvent;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    if( LS_GET_CFG_PROGRAM(& ActualEvent ) == NU_SUCCESS )
    {
        /* ���ù̻�״̬ */
        * pDst ++ = 6;                                        /* ��·�û����ݳ��� */
        * pDst ++ = IEC103_TI_M_98;                           /* ����ʶ */
        * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );       /* CPU���� */
        * pDst ++ = *( pSrc + 3 );                            /* ���кŵ��ֽ� */
        * pDst ++ = *( pSrc + 4 );                            /* ���кŸ��ֽ� */
        * pDst ++ = LS_RESULT_NULL;                           /* �̻�״̬ */
        * pDst ++ = LS_RESULT_NULL;                           /* ִ�н�� */
    }
    else
    {
        /* ���ù̻���� */
        * pDst ++ = 6;                                        /* ��·�û����ݳ��� */
        * pDst ++ = IEC103_TI_M_98;                           /* ����ʶ */
        * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );       /* CPU���� */
        * pDst ++ = *( pSrc + 3 );                            /* ���кŵ��ֽ� */
        * pDst ++ = *( pSrc + 4 );                            /* ���кŸ��ֽ� */
        * pDst ++ = LS_RESULT_TRUE;                           /* �̻�״̬ */

        if( LS_GET_CFG_ERROR(& ActualEvent ) == NU_SUCCESS )
        {
            * pDst ++ = LS_RESULT_NULL;                       /* ִ�н�� */
        }
        else
        {
            * pDst ++ = LS_RESULT_TRUE;                       /* ִ�н�� */
        }
    }

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_A0
/* �ٻ����ݱ���ܸ��� */
INT LSC_ACK_Datatab( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR    * pSrc,* pDst;
    UNSIGNED           TabNumber;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    TabNumber = XJP_DS_Get_Table_Number();

    * pDst ++ = 6;                                  /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_A0;                     /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                      /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                      /* ���кŸ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )( TabNumber & LS_DATA_MASKS8 );        /* ���ݱ�������ֽ� */
    * pDst = ( UNSIGNED_CHAR )(( TabNumber >> 8 ) & LS_DATA_MASKS8 );   /* ���ݱ�������ֽ� */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_A1
/* �ٻ����ݱ����Ϣ */
INT LSC_ACK_Datatab_Info( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED          number, StartNumber;
    UNSIGNED          Length;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 8;                                  /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_A1;                     /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                      /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                      /* ���кŸ��ֽ� */
    * pDst ++ = *( pSrc + 5 );                      /* ���ݱ�ĸ������ֽ� */
    * pDst ++ = *( pSrc + 6 );                      /* ���ݱ�ĸ������ֽ� */
    * pDst ++ = *( pSrc + 7 );                      /* ��ʼ�����ŵ��ֽ� */
    * pDst ++ = *( pSrc + 8 );                      /* ��ʼ�����Ÿ��ֽ� */

    /* ���ݱ���� */
    number = (*( pSrc + 5 ) & LS_DATA_MASKS8 );        /* ���ݱ�ĸ������ֽ� */
    number += ((*( pSrc + 6 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* ���ݱ�ĸ������ֽ� */

    /* ��ʼ������ */
    StartNumber = (*( pSrc + 7 ) & LS_DATA_MASKS8 );   /* ��ʼ�����ŵ��ֽ� */
    StartNumber += ((*( pSrc + 8 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* ��ʼ�����Ÿ��ֽ� */

    Length = 0;
    XJP_DS_Get_Table_Inf( number, StartNumber, pDst, & Length );

    pDst = ( UNSIGNED_CHAR *)Dst;
    * pDst += ( UNSIGNED_CHAR ) Length;                /* ��·�û����ݳ��� */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_A2
/* �ٻ��ֶ���Ϣ */
INT LSC_ACK_Field_Info( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED          number, StartNumber, TabIndex, ActualNumber;
    UNSIGNED          Length;
    INT               iStatus;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 10;                                 /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_A2;                     /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                      /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                      /* ���кŸ��ֽ� */
    * pDst ++ = *( pSrc + 5 );                      /* �������ŵ��ֽ� */
    * pDst ++ = *( pSrc + 6 );                      /* �������Ÿ��ֽ� */
    * pDst ++ = *( pSrc + 7 );                      /* �ֶθ������ֽ� */
    * pDst ++ = *( pSrc + 8 );                      /* �ֶθ������ֽ� */
    * pDst ++ = *( pSrc + 9 );                      /* ��ʼ�ֶε���ŵ��ֽ� */
    * pDst ++ = *( pSrc + 10 );                     /* ��ʼ�ֶε���Ÿ��ֽ� */

    /* �������� */
    TabIndex = (*( pSrc + 5 ) & LS_DATA_MASKS8 );                    /* �������ŵ��ֽ� */
    TabIndex += ((*( pSrc + 6 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* �������Ÿ��ֽ� */

    /* �ֶθ��� */
    number = (*( pSrc + 7 ) & LS_DATA_MASKS8 );                    /* �ֶθ������ֽ� */
    number += ((*( pSrc + 8 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* �ֶθ������ֽ� */

    /* ��ʼ�ֶε���� */
    StartNumber = (*( pSrc + 9 ) & LS_DATA_MASKS8 );                     /* ��ʼ�ֶε���ŵ��ֽ� */
    StartNumber += ((*( pSrc + 10 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* ��ʼ�ֶε���Ÿ��ֽ� */

    Length = 0;
    iStatus = XJP_DS_Get_Field_Inf( number, StartNumber, TabIndex, pDst, & Length, & ActualNumber );

    pDst = ( UNSIGNED_CHAR *)Dst;
    if( iStatus == NU_SUCCESS )
    {
        * pDst += ( UNSIGNED_CHAR ) Length;                  /* ��·�û����ݳ��� */
    }
    *( pDst + 7 ) = ( UNSIGNED_CHAR ) ( ActualNumber & LS_DATA_MASKS8 ); /* ʵ�ʸ������ֽ� */
    *( pDst + 8 ) = ( UNSIGNED_CHAR ) (( ActualNumber >> LS_BYTE_BITS ) & LS_DATA_MASKS8 );/* ʵ�ʸ������ֽ� */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_A3
/* �ٻ��������ݵ��ܸ��� */
INT LSC_ACK_Data_Number( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    INT               i, k;
    INT               TabIndex;
    INT               iStatus;
    TB_CFB            CondField;
    UNSIGNED          Number;
    UNSIGNED          Length;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    Length = 0;
    * pDst ++ = 9;                                  /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_A3;                     /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                      /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                      /* ���кŸ��ֽ� */
    * pDst ++ = *( pSrc + 5 );                      /* ��������ŵ��ֽ� */
    * pDst ++ = *( pSrc + 6 );                      /* ��������Ÿ��ֽ� */
    * pDst ++ = *( pSrc + 7 );                      /* �������� */

    /* ��������� */
    TabIndex = (*( pSrc + 5 ) & LS_DATA_MASKS8 );                    /* ��������ŵ��ֽ� */
    TabIndex += ((*( pSrc + 6 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* ��������Ÿ��ֽ� */

    CondField.CondNumber = (*( pSrc + 7 ) & LS_DATA_MASKS8 );        /* �������� */
    Length = 7 * CondField.CondNumber;                               /* ���������ֽ��� */

    for( i = 0; i < CondField.CondNumber; i ++ )
    {
        /* �����ֶ������� */
        CondField.CondIndex[ i ] = (*( pSrc + 8 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );/* ���ֽ� */
        CondField.CondIndex[ i ] += ((*( pSrc + 9 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* ���ֽ� */

        /* ������Լ���ֶ�ֵ */
        CondField.CondValue[ i ] = 0;
        for( k = 0; k < LS_DATA_WIDTH; k ++)
        {
            /* ����ֵ */
            CondField.CondValue[ i ] += ((*( pSrc + 10 + i * TB_CFB_COND_BYTE_NUMBER + k ) & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
        }

        /* �������� */
        CondField.CondType[ i ] = (*( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );/* ����ֵ */

        * pDst ++ = *( pSrc + 8 + i * TB_CFB_COND_BYTE_NUMBER );     /* �����ֶ������ŵ��ֽ� */
        * pDst ++ = *( pSrc + 9 + i * TB_CFB_COND_BYTE_NUMBER );     /* �����ֶ������Ÿ��ֽ� */
        * pDst ++ = *( pSrc + 10 + i * TB_CFB_COND_BYTE_NUMBER );    /* ����Լ���ֶ�ֵ�ֽ�1 */
        * pDst ++ = *( pSrc + 11 + i * TB_CFB_COND_BYTE_NUMBER );    /* ����Լ���ֶ�ֵ�ֽ�2 */
        * pDst ++ = *( pSrc + 12 + i * TB_CFB_COND_BYTE_NUMBER );    /* ����Լ���ֶ�ֵ�ֽ�3 */
        * pDst ++ = *( pSrc + 13 + i * TB_CFB_COND_BYTE_NUMBER );    /* ����Լ���ֶ�ֵ�ֽ�4 */
        * pDst ++ = *( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER );    /* �������� */
    }

    iStatus = XJP_DS_Get_Data_Number( TabIndex, & CondField, & Number );

    /* ���������ļ�¼���� */
    * pDst ++  = ( UNSIGNED_CHAR )( Number & LS_DATA_MASKS8 ); /* ���ֽ� */
    * pDst  = ( UNSIGNED_CHAR )(( Number >> 8 ) & LS_DATA_MASKS8 ); /* ���ֽ� */

    pDst = ( UNSIGNED_CHAR *)Dst;
    * pDst += ( UNSIGNED_CHAR )Length;         /* ��·�û����ݳ��� */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_A4
/* �ٻ���¼��ȫ���ֶ� */
INT LSC_ACK_All_Field_Value( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    INT               i, k;
    INT               number, StartNumber, TabIndex;
    INT               iStatus;
    TB_CFB            CondField;
    INT               ActualNumber;
    UNSIGNED          Length;
    INT               iFramLenNum;   /* һ֡���������������ݸ��� */
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    Length = 0;
    * pDst ++ = 10;                                 /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_A4;                     /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                      /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                      /* ���кŸ��ֽ� */
    * pDst ++ = *( pSrc + 5 );                      /* ��ȡ���ݵĸ��� */
    * pDst ++ = *( pSrc + 6 );                      /* ��ʼ���ݵĸ������ֽ� */
    * pDst ++ = *( pSrc + 7 );                      /* ��ʼ���ݵĸ������ֽ� */
    * pDst ++ = *( pSrc + 8 );                      /* ��������ŵ��ֽ� */
    * pDst ++ = *( pSrc + 9 );                      /* ��������Ÿ��ֽ� */
    * pDst ++ = *( pSrc + 10 );                     /* �������� */

    number = (*( pSrc + 5 ) & LS_DATA_MASKS8 );     /* ��ȡ���ݵĸ��� */

    /* ��ʼ���ݵĸ��� */
    StartNumber = (*( pSrc + 6 ) & LS_DATA_MASKS8 );                    /* ���ֽ� */
    StartNumber += ((*( pSrc + 7 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* ���ֽ� */

    /* ��������� */
    TabIndex = (*( pSrc + 8 ) & LS_DATA_MASKS8 );                       /* ���ֽ� */
    TabIndex += ((*( pSrc + 9 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );    /* ���ֽ� */

    CondField.CondNumber = (*( pSrc + 10 ) & LS_DATA_MASKS8 );          /* �������� */

    for( i = 0; i < CondField.CondNumber; i ++ )
    {
        /* �����ֶ������� */
        CondField.CondIndex[ i ] = (*( pSrc + 11 + i * TB_CFB_COND_BYTE_NUMBER ) & LS_DATA_MASKS8 ); /* ���ֽ� */
        CondField.CondIndex[ i ] += ((*( pSrc + 12 + i * TB_CFB_COND_BYTE_NUMBER   ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* ���ֽ� */

        CondField.CondValue[ i ] = 0;
        for( k = 0; k < LS_DATA_WIDTH; k ++)
        {
            /* ������Լ���ֶ�ֵ */
            CondField.CondValue[ i ] += ((*( pSrc + 13 + i * TB_CFB_COND_BYTE_NUMBER + k ) & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
        }
        CondField.CondType[ i ] = (*( pSrc + 17 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 ); /* �������� */

        * pDst ++ = *( pSrc + 11 + i * TB_CFB_COND_BYTE_NUMBER );     /* �����ֶ������ŵ��ֽ� */
        * pDst ++ = *( pSrc + 12 + i * TB_CFB_COND_BYTE_NUMBER );     /* �����ֶ������Ÿ��ֽ� */
        * pDst ++ = *( pSrc + 13 + i * TB_CFB_COND_BYTE_NUMBER );     /* ������Լ���ֶ�ֵ�ֽ�1 */
        * pDst ++ = *( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER );     /* ������Լ���ֶ�ֵ�ֽ�2 */
        * pDst ++ = *( pSrc + 15 + i * TB_CFB_COND_BYTE_NUMBER );     /* ������Լ���ֶ�ֵ�ֽ�3 */
        * pDst ++ = *( pSrc + 16 + i * TB_CFB_COND_BYTE_NUMBER );     /* ������Լ���ֶ�ֵ�ֽ�4 */
        * pDst ++ = *( pSrc + 17 + i * TB_CFB_COND_BYTE_NUMBER );     /* �������� */
    }
    XJP_DS_Get_Table_Length( TabIndex, & Length );     /* ��ȡ���ݱ�ĳ��� */
    /* ����һ֡���������������ݸ��� */
    if( Length > 0 )
    {
        iFramLenNum = (( LC_MAX_FRAME_DATA_SIZE - 20 ) / Length );
        /* �ж���������ȡ�����ݸ��� */
        if( number > iFramLenNum )
        {
            number = iFramLenNum;
        }
    }
    iStatus = XJP_DS_Get_Field_Value_All( TabIndex, StartNumber, number, & CondField, pDst, & ActualNumber );

    pDst = ( UNSIGNED_CHAR *)Dst;
    if( ActualNumber > StartNumber )
    {
        Length *= ( ActualNumber - StartNumber );    /* ���ݳ��� */
        *( pDst + 5 ) = ActualNumber - StartNumber;  /* ��ȡ���ݵĸ��� */
    }
    else
    {
        * ( pDst + 5 ) = 0;                          /* ��ȡ���ݵĸ��� */
    }
    Length += 7 * CondField.CondNumber;              /* �������� */

    * pDst += ( UNSIGNED_CHAR ) Length;              /* ��·�û����ݳ��� */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_A5
/* �ٻ���¼�ĵ����ֶ� */
INT LSC_ACK_Field_Value( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    INT               i, k;
    INT               number, StartNumber, TabIndex;
    INT               iStatus;
    TB_CFB            CondField;
    INT               ActualNumber;
    UNSIGNED          Length;
    INT               iFramLenNum;   /* һ֡���������������ݸ��� */
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    Length = 0;
    * pDst ++ = 12;                                 /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_A5;                     /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                      /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                      /* ���кŸ��ֽ� */
    * pDst ++ = *( pSrc + 5 );                      /* ��ȡ���ݵĸ��� */
    * pDst ++ = *( pSrc + 6 );                      /* ��ʼ���ݵĸ������ֽ� */
    * pDst ++ = *( pSrc + 7 );                      /* ��ʼ���ݵĸ������ֽ� */
    * pDst ++ = *( pSrc + 8 );                      /* ��������ŵ��ֽ� */
    * pDst ++ = *( pSrc + 9 );                      /* ��������Ÿ��ֽ� */
    * pDst ++ = *( pSrc + 10 );                     /* �����ֶ������ŵ��ֽ� */
    * pDst ++ = *( pSrc + 11 );                     /* �����ֶ������Ÿ��ֽ� */
    * pDst ++ = *( pSrc + 12 );                     /* �������� */

    number = (*( pSrc + 5 ) & LS_DATA_MASKS8 );     /* ��ȡ���ݵĸ��� */

    /* ��ʼ���ݵĸ��� */
    StartNumber = (*( pSrc + 6 ) & LS_DATA_MASKS8 );                    /* ���ֽ� */
    StartNumber += ((*( pSrc + 7 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* ���ֽ� */

    /* ��������� */
    TabIndex = (*( pSrc + 8 ) & LS_DATA_MASKS8 );      /* ���ֽ� */
    TabIndex += ((*( pSrc + 9 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* ���ֽ� */

    /* �����ֶ������� */
    CondField.HandleIndex = (*( pSrc + 10 ) & LS_DATA_MASKS8 ); /* ���ֽ� */
    CondField.HandleIndex += ((*( pSrc + 11 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* ���ֽ� */

    CondField.CondNumber = (*( pSrc + 12 ) & LS_DATA_MASKS8 ); /* �������� */

    for( i = 0; i < CondField.CondNumber; i ++ )
    {
        /* �������ֶ������� */
        CondField.CondIndex[ i ] = (*( pSrc + 13 + i * TB_CFB_COND_BYTE_NUMBER ) & LS_DATA_MASKS8 );/* ���ֽ� */
        CondField.CondIndex[ i ] += ((*( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* ���ֽ� */

        CondField.CondValue[ i ] = 0;
        for( k = 0; k < LS_DATA_WIDTH; k ++)
        {
            /* ������Լ���ֶ�ֵ */
            CondField.CondValue[ i ] += ((*( pSrc + 15 + i * TB_CFB_COND_BYTE_NUMBER + k ) & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
        }
        CondField.CondType[ i ] = (*( pSrc + 19 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );/* ���������� */

        * pDst ++ = *( pSrc + 13 + i * TB_CFB_COND_BYTE_NUMBER );     /* �������ֶ������ŵ��ֽ� */
        * pDst ++ = *( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER );     /* �������ֶ������Ÿ��ֽ� */
        * pDst ++ = *( pSrc + 15 + i * TB_CFB_COND_BYTE_NUMBER );     /* ������Լ���ֶ�ֵ�ֽ�1 */
        * pDst ++ = *( pSrc + 16 + i * TB_CFB_COND_BYTE_NUMBER );     /* ������Լ���ֶ�ֵ�ֽ�2 */
        * pDst ++ = *( pSrc + 17 + i * TB_CFB_COND_BYTE_NUMBER );     /* ������Լ���ֶ�ֵ�ֽ�3 */
        * pDst ++ = *( pSrc + 18 + i * TB_CFB_COND_BYTE_NUMBER );     /* ������Լ���ֶ�ֵ�ֽ�4 */
        * pDst ++ = *( pSrc + 19 + i * TB_CFB_COND_BYTE_NUMBER );     /* ���������� */
    }
    XJP_DS_Get_Field_Length( TabIndex, ( CondField.HandleIndex ), & Length );/* ��ȡ�ֶεĳ��� */
    /* ����һ֡���������������ݸ��� */
    if( Length > 0 )
    {
        iFramLenNum = (( LC_MAX_FRAME_DATA_SIZE - 20 ) / Length );
        /* �ж���������ȡ�����ݸ��� */
        if( number > iFramLenNum )
        {
            number = iFramLenNum;
        }
    }

    iStatus = XJP_DS_Get_Field_Value_One( TabIndex, StartNumber, number, & CondField, pDst, & ActualNumber );

    pDst = ( UNSIGNED_CHAR *)Dst;
    if( ActualNumber > StartNumber )
    {
        Length *= ( ActualNumber - StartNumber );   /* ���ݳ��� */
        * ( pDst + 5 ) = ActualNumber - StartNumber;/* ��ȡ���ݵĸ��� */
    }
    else
    {
        * ( pDst + 5 ) = 0;                         /* ��ȡ���ݵĸ��� */
    }
    Length += 7 * CondField.CondNumber;             /* �������� */
    * pDst += ( UNSIGNED_CHAR ) Length;             /* ��·�û����ݳ��� */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_A6
/* �ٻ���¼�Ķ���ֶ� */
INT LSC_ACK_Fields_Value( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    INT               i, k;
    INT               number, StartNumber, TabIndex;
    INT               iStatus;
    TB_CHB            CondField;
    INT               ActualNumber;
    UNSIGNED          Length, FieldLength;
    INT               FreezeStatus = NU_UNAVAILABLE;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    Length = 0;
    * pDst ++ = 11;                                 /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_A6;                     /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                      /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                      /* ���кŸ��ֽ� */
    * pDst ++ = *( pSrc + 5 );                      /* ��ȡ���ݵĸ��� */
    * pDst ++ = *( pSrc + 6 );                      /* ��ʼ���ݵĸ������ֽ� */
    * pDst ++ = *( pSrc + 7 );                      /* ��ʼ���ݵĸ������ֽ� */
    * pDst ++ = *( pSrc + 8 );                      /* ��������ŵ��ֽ� */
    * pDst ++ = *( pSrc + 9 );                      /* ��������Ÿ��ֽ� */
    * pDst ++ = *( pSrc + 10 );                     /* �����ֶθ��� */

    number = (*( pSrc + 5 ) & LS_DATA_MASKS8 );     /* ��ȡ���ݵĸ��� */

    /* ��ʼ���ݵĸ��� */
    StartNumber = (*( pSrc + 6 ) & LS_DATA_MASKS8 );                   /* ���ֽ� */
    StartNumber += ((*( pSrc + 7 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* ���ֽ� */

    /* ��������� */
    TabIndex = (*( pSrc + 8 ) & LS_DATA_MASKS8 );                      /* ���ֽ� */
    TabIndex += ((*( pSrc + 9 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );   /* ���ֽ� */

    CondField.HandleNumber = *( pSrc + 10 );                           /* �����ֶθ��� */

    for( i = 0; i < CondField.HandleNumber; i ++ )
    {
        /* �����ֶε������� */
        CondField.HandleIndex[ i ] = (*( pSrc + 11 + i * 2 ) & LS_DATA_MASKS8 ); /* ���ֽ� */
        CondField.HandleIndex[ i ] += ((*( pSrc + 12 + i * 2 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* ���ֽ� */

        * pDst ++ = *( pSrc + 11 + i * 2 ); /* �����ֶε������ŵ��ֽ� */
        * pDst ++ = *( pSrc + 12 + i * 2 ); /* �����ֶε������Ÿ��ֽ� */
    }

    * pDst ++ = *( pSrc + 11 + 2 * CondField.HandleNumber );     /* �������� */

    CondField.CondNumber = (*( pSrc + 11 + 2 * CondField.HandleNumber ) & LS_DATA_MASKS8 );/* �������� */

    for( i = 0; i < CondField.CondNumber; i ++ )
    {
        /* �������ֶ������� */
        CondField.CondIndex[ i ]
            = (*( pSrc + 12 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER ) & LS_DATA_MASKS8 );/* ���ֽ� */
        CondField.CondIndex[ i ]
            += ((*( pSrc + 13 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* ���ֽ� */

        CondField.CondValue[ i ] = 0;
        for( k = 0; k < LS_DATA_WIDTH; k ++)
        {
            /* ������Լ���ֶ�ֵ */
            CondField.CondValue[ i ] +=
                ((*( pSrc + 14 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER + k ) & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
        }
        /* ���������� */
        CondField.CondType[ i ] = (*( pSrc + 18 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );

        * pDst ++ = *( pSrc + 12 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER );     /* �������ֶ������ŵ��ֽ� */
        * pDst ++ = *( pSrc + 13 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER );     /* �������ֶ������Ÿ��ֽ� */
        * pDst ++ = *( pSrc + 14 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER );     /* ������Լ���ֶ�ֵ�ֽ�1 */
        * pDst ++ = *( pSrc + 15 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER );     /* ������Լ���ֶ�ֵ�ֽ�2 */
        * pDst ++ = *( pSrc + 16 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER );     /* ������Լ���ֶ�ֵ�ֽ�3 */
        * pDst ++ = *( pSrc + 17 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER );     /* ������Լ���ֶ�ֵ�ֽ�4 */
        * pDst ++ = *( pSrc + 18 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER );     /* ���������� */
    }
    if( XJP_DS_Freeze_Status( TabIndex ) == NU_FALSE )
    {
        FreezeStatus = XJP_DS_Freeze_Table( TabIndex );
    }
    iStatus = XJP_DS_Get_Field_Value_Batch( TabIndex, StartNumber, number, & CondField, pDst, & ActualNumber );

    if( FreezeStatus == NU_SUCCESS )
    {
        FreezeStatus = XJP_DS_UnFreeze_Table( TabIndex );
    }

    pDst = ( UNSIGNED_CHAR *)Dst;
    Length = 0;
    if( ActualNumber > StartNumber )
    {
        * ( pDst + 5 ) = ActualNumber - StartNumber; /* ��ȡ���ݵĸ��� */
        for( i = 0; i < CondField.HandleNumber; i ++ )
        {
            XJP_DS_Get_Field_Length( TabIndex, ( CondField.HandleIndex[i] ), & FieldLength );/* �ֶγ��� */
            Length += FieldLength;
        }
        Length *= ( ActualNumber - StartNumber );   /* ���ݳ��� */
    }
    else
    {
        * ( pDst + 5 ) = 0;  /* ��ȡ���ݵĸ��� */
    }
    Length += 2 * CondField.HandleNumber; /* �����ֶγ��� */
    Length += 7 * CondField.CondNumber;   /* �������� */
    * pDst += ( UNSIGNED_CHAR ) Length;   /* ��·�û����ݳ��� */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_A7
/* ���������ֶε�ֵ */
INT LSC_Set_Config_Values( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR  * pSrc,* pDst;
    INT              i, k;
    INT              number, StartNumber, TabIndex;
    INT              iStatus;
    TB_CFB           CondField;
    UNSIGNED         Buffer[ LS_MAX_CONFIG_BYTE ];
    UNSIGNED_CHAR    Buffer1[ LS_MAX_CONFIG_BYTE ];
    INT              ValueWidth;
    VOID           * pTemp;
    TB_FCB          * pField;
    TB_TCB          * pTable;
    INT             iNumber;
    INT             originate;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    number = (*( pSrc + 5 ) & LS_DATA_MASKS8 );        /* �������ݵĸ��� */

    /* ��ʼ���ݵĸ��� */
    StartNumber = (*( pSrc + 6 ) & LS_DATA_MASKS8 );   /* ���ֽ� */
    StartNumber += ((*( pSrc + 7 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* ���ֽ� */

    /* ��������� */
    TabIndex = (*( pSrc + 8 ) & LS_DATA_MASKS8 );      /* ���ֽ� */
    TabIndex += ((*( pSrc + 9 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* ���ֽ� */

    /* �����ֶ������� */
    CondField.HandleIndex = (*( pSrc + 10 ) & LS_DATA_MASKS8 );/* ���ֽ� */
    CondField.HandleIndex += ((*( pSrc + 11 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* ���ֽ� */

    ValueWidth = (*( pSrc + 12 ) & LS_DATA_MASKS8 );           /* �������ݵ��ֽ��� */

    CondField.CondNumber = (*( pSrc + 13 ) & LS_DATA_MASKS8 ); /* �������� */

    * pDst ++ = 14 + 7 *( CondField.CondNumber );   /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_A7;                     /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                      /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                      /* ���кŸ��ֽ� */
    * pDst ++ = *( pSrc + 5 );                      /* �������ݵĸ��� */
    * pDst ++ = *( pSrc + 6 );                      /* ��ʼ���ݵĸ������ֽ� */
    * pDst ++ = *( pSrc + 7 );                      /* ��ʼ���ݵĸ������ֽ� */
    * pDst ++ = *( pSrc + 8 );                      /* ��������ŵ��ֽ� */
    * pDst ++ = *( pSrc + 9 );                      /* ��������Ÿ��ֽ� */
    * pDst ++ = *( pSrc + 10 );                     /* �����ֶ������ŵ��ֽ� */
    * pDst ++ = *( pSrc + 11 );                     /* �����ֶ������Ÿ��ֽ� */
    * pDst ++ = *( pSrc + 12 );                     /* �������ݵ��ֽ��� */
    * pDst ++ = *( pSrc + 13 );                     /* �������� */

    for( i = 0; i < CondField.CondNumber; i ++ )
    {
        /* �������ֶ������� */
        CondField.CondIndex[ i ] = (*( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER ) & LS_DATA_MASKS8 );/* ���ֽ� */
        CondField.CondIndex[ i ] += ((*( pSrc + 15 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* ���ֽ� */

        CondField.CondValue[ i ] = 0;
        for( k = 0; k < LS_DATA_WIDTH; k ++)
        {
            /* ������Լ���ֶ�ֵ */
            CondField.CondValue[ i ] += ((*( pSrc + 16 + i * TB_CFB_COND_BYTE_NUMBER + k ) & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
        }
        CondField.CondType[ i ] = (*( pSrc + 20 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );/* ���������� */

        * pDst ++ = *( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER );     /* �����ֶ������ŵ��ֽ� */
        * pDst ++ = *( pSrc + 15 + i * TB_CFB_COND_BYTE_NUMBER );     /* �����ֶ������Ÿ��ֽ� */
        * pDst ++ = *( pSrc + 16 + i * TB_CFB_COND_BYTE_NUMBER );     /* ����Լ���ֶ�ֵ�ֽ�1 */
        * pDst ++ = *( pSrc + 17 + i * TB_CFB_COND_BYTE_NUMBER );     /* ����Լ���ֶ�ֵ�ֽ�2 */
        * pDst ++ = *( pSrc + 18 + i * TB_CFB_COND_BYTE_NUMBER );     /* ����Լ���ֶ�ֵ�ֽ�3 */
        * pDst ++ = *( pSrc + 19 + i * TB_CFB_COND_BYTE_NUMBER );     /* ����Լ���ֶ�ֵ�ֽ�4 */
        * pDst ++ = *( pSrc + 20 + i * TB_CFB_COND_BYTE_NUMBER );     /* �������� */
    }

    /* ͨ�����ݱ������ż������ֶ�������,�ҵ���Ӧ���ֶ���Ϣ */
    pTable = CTBC_FindTable( TabIndex );
    pField = CTBC_FindField( pTable, CondField.HandleIndex );
    /* һ�����ݵ����ݸ��� */
    iNumber = pField->Size / pField->TypeSize;

    for( i = 0; i < ( number * ( iNumber * pField->TypeByte ) ); i ++ )
    {
        /* �������� */
        Buffer1 [i] = (*( pSrc + 14 + 7 * CondField.CondNumber + i ) & LS_DATA_MASKS8 );
    }

    /* ���ݿ�Ƚ�������1��4,��Ϊ1ʱ��������ΪUNSIGNED_CHAR,4:UNSIGNED */
    if( ValueWidth == 1 )
    {
        pTemp = ( VOID *)Buffer1;
    }
    else
    {
        for( i = 0; i < ( number * iNumber ); i ++ )
        {
            Buffer[i] = 0;
            for( k = 0; k < ValueWidth; k ++)
            {
                /* �������ݿ�Ⱥϳ�ָ����ʽ������ */
                Buffer[i] += (( Buffer1[i * ValueWidth + k] & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
            }
        }
        pTemp = ( VOID *)Buffer;
    }

    originate = ( INT )(*( pSrc - 1 ) & LC_VAR_CONTROL_BITS );
    iStatus = XJP_DS_Set_Field_Value_Config( TabIndex, StartNumber, number, & CondField, pTemp, originate );

    * pDst = ( UNSIGNED_CHAR )( iStatus );

    if( iStatus != NU_SUCCESS )
    {
        k = *( pSrc - 1 ) & 0x07;  /* ��Чͨ�� */
        sprintf( ( CHAR *)Buffer1, "װ��������������ʧ�ܣ�table��%02d field��%02d org��%s result��%d",
            TabIndex, CondField.HandleIndex, LSC_CommChl[k], iStatus );
        CLOG_Log( ( CHAR *)Buffer1 );
    }

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_A8
/* ѡ��༭�ֶ� */
INT LSC_Slecet_Edit_SG( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT               k;
    UNSIGNED_CHAR   * pSrc,* pDst;
    INT               FieldIndex, TabIndex;
    INT               iStatus;
    CHAR              Buffer1[ LS_MAX_CONFIG_BYTE ];
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 9;                                  /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_A8;                     /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                      /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                      /* ���кŸ��ֽ� */
    * pDst ++ = *( pSrc + 5 );                      /* ��������ŵ��ֽ� */
    * pDst ++ = *( pSrc + 6 );                      /* ��������Ÿ��ֽ� */
    * pDst ++ = *( pSrc + 7 );                      /* �ֶ������ŵ��ֽ� */
    * pDst ++ = *( pSrc + 8 );                      /* �ֶ������Ÿ��ֽ� */

    /* ��������� */
    TabIndex = (*( pSrc + 5 ) & LS_DATA_MASKS8 );   /* ���ֽ� */
    TabIndex += ((*( pSrc + 6 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* ���ֽ� */

    /* �ֶ������� */
    FieldIndex = (*( pSrc + 7 ) & LS_DATA_MASKS8 ); /* ���ֽ� */
    FieldIndex += ((*( pSrc + 8 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* ���ֽ� */

    iStatus = XJP_DS_Select_Edit_Field_SG( TabIndex, FieldIndex );
    * pDst = ( UNSIGNED_CHAR )( iStatus );         /* ״̬��� */
    if( iStatus != NU_SUCCESS )
    {
        k = *( pSrc - 1 ) & 0x07;  /* ��Чͨ�� */
        sprintf( Buffer1, "װ������ֵ����ѡ��ʧ�ܣ�table��%02d field��%02d org��%s result��%d",
            TabIndex, FieldIndex, LSC_CommChl[k], iStatus );
        CLOG_Log( Buffer1 );
    }

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_A9
/* ���ñ༭��ֵ���ֵ */
INT LSC_Set_Edit_SGV( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    INT               i, k;
    INT               number, StartNumber, TabIndex;
    INT               iStatus;
    TB_CFB            CondField;
    UNSIGNED          Buffer[ LS_MAX_CONFIG_BYTE ];
    UNSIGNED_CHAR     Buffer1[ LS_MAX_CONFIG_BYTE ];
    INT               ValueWidth;
    VOID            * pTemp;
    TB_FCB          * pField;
    TB_TCB          * pTable;
    INT             iNumber;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    number = (*( pSrc + 5 ) & LS_DATA_MASKS8 );        /* �������ݵĸ��� */

    /* ��ʼ���ݵĸ��� */
    StartNumber = (*( pSrc + 6 ) & LS_DATA_MASKS8 );   /* ���ֽ� */
    StartNumber += ((*( pSrc + 7 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* ���ֽ� */

    /* ��������� */
    TabIndex = (*( pSrc + 8 ) & LS_DATA_MASKS8 );      /* ���ֽ� */
    TabIndex += ((*( pSrc + 9 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* ���ֽ� */

    /* �����ֶ������� */
    CondField.HandleIndex = (*( pSrc + 10 ) & LS_DATA_MASKS8 );/* ���ֽ� */
    CondField.HandleIndex += ((*( pSrc + 11 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* ���ֽ� */

    ValueWidth = (*( pSrc + 12 ) & LS_DATA_MASKS8 );           /* �������ݵ��ֽ��� */

    CondField.CondNumber = (*( pSrc + 13 ) & LS_DATA_MASKS8 ); /* �������� */

    * pDst ++ = 14 + 7 *( CondField.CondNumber );   /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_A9;                     /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                      /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                      /* ���кŸ��ֽ� */
    * pDst ++ = *( pSrc + 5 );                      /* �������ݵĸ��� */
    * pDst ++ = *( pSrc + 6 );                      /* ��ʼ���ݵĸ������ֽ� */
    * pDst ++ = *( pSrc + 7 );                      /* ��ʼ���ݵĸ������ֽ� */
    * pDst ++ = *( pSrc + 8 );                      /* ��������ŵ��ֽ� */
    * pDst ++ = *( pSrc + 9 );                      /* ��������Ÿ��ֽ� */
    * pDst ++ = *( pSrc + 10 );                     /* �����ֶ������ŵ��ֽ� */
    * pDst ++ = *( pSrc + 11 );                     /* �����ֶ������Ÿ��ֽ� */
    * pDst ++ = *( pSrc + 12 );                     /* �������ݵ��ֽ��� */
    * pDst ++ = *( pSrc + 13 );                     /* �������� */

    for( i = 0; i < CondField.CondNumber; i ++ )
    {
        /* �������ֶ������� */
        CondField.CondIndex[ i ] = (*( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );/* ���ֽ� */
        CondField.CondIndex[ i ] += ((*( pSrc + 15 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* ���ֽ� */

        CondField.CondValue[ i ] = 0;
        for( k = 0; k < LS_DATA_WIDTH; k ++)
        {
            /* ������Լ���ֶ�ֵ */
            CondField.CondValue[ i ] += ((*( pSrc + 16 + i * TB_CFB_COND_BYTE_NUMBER + k ) & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
        }
        CondField.CondType[ i ] = (*( pSrc + 20 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );/* �������� */

        * pDst ++ = *( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER );     /* �������ֶ������ŵ��ֽ� */
        * pDst ++ = *( pSrc + 15 + i * TB_CFB_COND_BYTE_NUMBER );     /* �������ֶ������Ÿ��ֽ� */
        * pDst ++ = *( pSrc + 16 + i * TB_CFB_COND_BYTE_NUMBER );     /* ������Լ���ֶ�ֵ�ֽ�1 */
        * pDst ++ = *( pSrc + 17 + i * TB_CFB_COND_BYTE_NUMBER );     /* ������Լ���ֶ�ֵ�ֽ�2 */
        * pDst ++ = *( pSrc + 18 + i * TB_CFB_COND_BYTE_NUMBER );     /* ������Լ���ֶ�ֵ�ֽ�3 */
        * pDst ++ = *( pSrc + 19 + i * TB_CFB_COND_BYTE_NUMBER );     /* ������Լ���ֶ�ֵ�ֽ�4 */
        * pDst ++ = *( pSrc + 20 + i * TB_CFB_COND_BYTE_NUMBER );     /* ���������� */
    }

    /* ͨ�����ݱ������ż������ֶ�������,�ҵ���Ӧ���ֶ���Ϣ */
    pTable = CTBC_FindTable( TabIndex );
    pField = CTBC_FindField( pTable, CondField.HandleIndex );
    /* һ�����ݵ����ݸ��� */
    iNumber = pField->Size / pField->TypeSize;

    for( i = 0; i < ( number * ( iNumber * pField->TypeByte ) ); i ++ )
    {
        /* �������� */
        Buffer1 [i] = (*( pSrc + 14 + 7 * CondField.CondNumber + i ) & LS_DATA_MASKS8 );
    }

    /* ���ݿ�Ƚ�������1��4,��Ϊ1ʱ��������ΪUNSIGNED_CHAR,4:UNSIGNED */
    if( ValueWidth == 1 )
    {
        pTemp = ( VOID *)Buffer1;
    }
    else
    {
        for( i = 0; i < ( number * iNumber ); i ++ )
        {
            Buffer[i] = 0;
            for( k = 0; k < ValueWidth; k ++)
            {
                /* �������ݿ�Ⱥϳ�ָ����ʽ������ */
                Buffer[i] += (( Buffer1[i * ValueWidth + k] & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
            }
        }
        pTemp = ( VOID *)Buffer;
    }

    iStatus = XJP_DS_Set_Field_Value_SG( TabIndex, StartNumber, number, & CondField, pTemp );
    * pDst = ( UNSIGNED_CHAR )( iStatus );
    if( iStatus != NU_SUCCESS )
    {
        k = *( pSrc - 1 ) & 0x07;  /* ��Чͨ�� */
        sprintf( ( CHAR *)Buffer1, "װ������ֵ���ݱ༭ʧ�ܣ�table��%02d field��%02d org��%s result��%d",
            TabIndex, CondField.HandleIndex, LSC_CommChl[k], iStatus );
        CLOG_Log( ( CHAR *)Buffer1 );
    }

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_AA
/* ȷ�ϱ༭��ֵ���ֵ */
INT LSC_Conform_Edit_SGV( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR  * pSrc,* pDst, * p;
    INT              i, k;
    INT              Type, StartNumber = 0, TabIndex;
    INT              iStatus;
    TB_CFB           CondField;
    INT              TypeOffset = 0;
    CHAR             buffer[128];      ///
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    Type = (*( pSrc + 5 ) & LS_DATA_MASKS8 );     /* ��ֵ���ͣ�1��ȫ����ֵ��2��������ֵ�� */

    if( Type == SETTING_ONE_ENABLE )
    {
        TypeOffset = 2; /* start number - 2 bytes */
        /* ��ʼ���ݵĸ��� */
        StartNumber = (*( pSrc + 6 ) & LS_DATA_MASKS8 ); /* ���ֽ� */
        StartNumber += ((*( pSrc + 7 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* ���ֽ� */
    }

    /* ��������� */
    TabIndex = (*( pSrc + 6 + TypeOffset ) & LS_DATA_MASKS8 );    /* ���ֽ� */
    TabIndex += ((*( pSrc + 7 + TypeOffset ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* ���ֽ� */

    /* �����ֶ������� */
    CondField.HandleIndex = (*( pSrc + 8 + TypeOffset ) & LS_DATA_MASKS8 ); /* ���ֽ� */
    CondField.HandleIndex += ((*( pSrc + 9 + TypeOffset ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* ���ֽ� */

    CondField.CondNumber = (*( pSrc + 10 + TypeOffset ) & LS_DATA_MASKS8 );  /* �������� */

    * pDst ++ = 11 + TypeOffset + 7 *( CondField.CondNumber ); /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_AA;                                /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );            /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                                 /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                                 /* ���кŸ��ֽ� */
    * pDst ++ = *( pSrc + 5 );                                 /* ��ֵ���� */
    if( Type == SETTING_ONE_ENABLE )
    {
        /* ��ʼ���ݵĸ��� */
        * pDst ++ = *( pSrc + 6 );                             /* ���ֽ� */
        * pDst ++ = *( pSrc + 7 );                             /* ���ֽ� */
    }
    * pDst ++ = *( pSrc + 6 + TypeOffset );                    /* ��������ŵ��ֽ� */
    * pDst ++ = *( pSrc + 7 + TypeOffset );                    /* ��������Ÿ��ֽ� */
    * pDst ++ = *( pSrc + 8 + TypeOffset );                    /* �����ֶ������ŵ��ֽ� */
    * pDst ++ = *( pSrc + 9 + TypeOffset );                    /* �����ֶ������Ÿ��ֽ� */
    * pDst ++ = *( pSrc + 10 + TypeOffset );                   /* �������� */

    for( i = 0; i < CondField.CondNumber; i ++ )
    {
        /* �������ֶ������� */
        CondField.CondIndex[ i ] = (*( pSrc + 11 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );/* ���ֽ� */
        CondField.CondIndex[ i ] += ((*( pSrc + 12 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/*���ֽ�*/

        CondField.CondValue[ i ] = 0;
        for( k = 0; k < LS_DATA_WIDTH; k ++)
        {
            /* ������Լ���ֶ�ֵ */
            CondField.CondValue[ i ]
                += ((*( pSrc + 13 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER + k ) & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
        }
        CondField.CondType[ i ] = (*( pSrc + 17 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );/* ���������� */

        * pDst ++ = *( pSrc + 11 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER );       /* �������ֶ������ŵ��ֽ� */
        * pDst ++ = *( pSrc + 12 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER );       /* �������ֶ������Ÿ��ֽ� */
        * pDst ++ = *( pSrc + 13 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER );       /* ������Լ���ֶ�ֵ�ֽ�1 */
        * pDst ++ = *( pSrc + 14 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER );       /* ������Լ���ֶ�ֵ�ֽ�2 */
        * pDst ++ = *( pSrc + 15 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER );       /* ������Լ���ֶ�ֵ�ֽ�3 */
        * pDst ++ = *( pSrc + 16 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER );       /* ������Լ���ֶ�ֵ�ֽ�4 */
        * pDst ++ = *( pSrc + 17 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER );       /* ���������� */
    }

    p = ( UNSIGNED_CHAR *)Dst;
    p --;
    * p = *( pSrc - 1 ) & LC_VAR_CONTROL_BITS;
    iStatus = XJP_DS_Confirm_Field_Value_SG( TabIndex, Type, StartNumber, & CondField, LS_TIME_OUT,
        LCC_BCall_BackTaskSendMessData, p );
    * pDst = ( UNSIGNED_CHAR )( iStatus );
    if( iStatus != NU_SUCCESS )
    {
        k = *( pSrc - 1 ) & 0x07;  /* ��Чͨ�� */
        sprintf( buffer, "װ������ֵ����ִ��ʧ�ܣ�table��%02d field��%02d org��%s result��%d",
            TabIndex, CondField.HandleIndex, LSC_CommChl[k], iStatus );
        CLOG_Log( buffer );
        return( COM_DATA_FULL );
    }
    else
    {
        k = *( pSrc - 1 ) & 0x07; /* ��Чͨ�� */
        sprintf( buffer, "װ������ֵ�����޸ĳɹ���table��%02d field��%02d org��%s",
            TabIndex, CondField.HandleIndex, LSC_CommChl[k] );
        CLOG_Log( buffer );
        return( COM_DATA_EMPTY );
    }
}
#endif

#if SWITCH_IEC103_TI_M_AB
/* ��ȡ���ݱ��״̬ */
INT LSC_Get_Tab_Status( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR    * pSrc,* pDst;
    INT                TableIndex;
    INT                iStatus;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 7;                                  /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_AB;                     /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                      /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                      /* ���кŸ��ֽ� */
    * pDst ++ = *( pSrc + 5 );                      /* ��������ŵ��ֽ� */
    * pDst ++ = *( pSrc + 6 );                      /* ��������Ÿ��ֽ� */

    /* ��������� */
    TableIndex = (*( pSrc + 5 ) & LS_DATA_MASKS8 );  /* ��������ŵ��ֽ� */
    TableIndex += ((*( pSrc + 6 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* ��������Ÿ��ֽ� */

    iStatus = XJP_DS_Get_Table_Status( TableIndex, pDst ); /* ��ȡ���ݱ��״̬ */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_AC
/* ��ȡ���ݱ������CRC�� */
INT LSC_Get_Config_Tab_CRC( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR    * pSrc,* pDst;
    UNSIGNED           CRC_code;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 12;                                  /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_AC;                     /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                      /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                      /* ���кŸ��ֽ� */

    CRC_code = XJP_DS_Get_Config_CRC( );
    * pDst ++ = ( UNSIGNED_CHAR )( CRC_code  & LS_DATA_MASKS8 );         /* �������ݵ�CRC���ֽ�1 */
    * pDst ++ = ( UNSIGNED_CHAR )(( CRC_code >> 8 ) & LS_DATA_MASKS8 );  /* �������ݵ�CRC���ֽ�2 */
    * pDst ++ = ( UNSIGNED_CHAR )(( CRC_code >> 16 ) & LS_DATA_MASKS8 ); /* �������ݵ�CRC���ֽ�3 */
    * pDst ++ = ( UNSIGNED_CHAR )(( CRC_code >> 24 ) & LS_DATA_MASKS8 ); /* �������ݵ�CRC���ֽ�4 */
    CRC_code = XJP_DS_Get_Tab_Field_CRC( );
    * pDst ++ = ( UNSIGNED_CHAR )( CRC_code  & LS_DATA_MASKS8 );         /* �ֶ���Ϣ��CRC���ֽ�1 */
    * pDst ++ = ( UNSIGNED_CHAR )(( CRC_code >> 8 ) & LS_DATA_MASKS8 );  /* �ֶ���Ϣ��CRC���ֽ�2 */
    * pDst ++ = ( UNSIGNED_CHAR )(( CRC_code >> 16 ) & LS_DATA_MASKS8 ); /* �ֶ���Ϣ��CRC���ֽ�3 */
    * pDst = ( UNSIGNED_CHAR )(( CRC_code >> 24 ) & LS_DATA_MASKS8 );    /* �ֶ���Ϣ��CRC���ֽ�4 */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_AD
INT LSC_FreezeTable;
VOID LSC_FreezeTabOvertimeCall( void * argPtr );
/* �������ݱ������ */
INT  LSC_Freeze_Table( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR    * pSrc,* pDst;
    INT                iStatus;
    INT                TableIndex;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 7;                                  /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_AD;                     /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                      /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                      /* ���кŸ��ֽ� */
    * pDst ++ = *( pSrc + 5 );                      /* Table index -1 */
    * pDst ++ = *( pSrc + 6 );                      /* Table index -2 */

    /* ��������� */
    TableIndex = (*( pSrc + 5 ) & LS_DATA_MASKS8 );  /* ���ֽ� */
    TableIndex += ((*( pSrc + 6 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* ���ֽ� */

    if( XJP_DS_Freeze_Status( TableIndex ) == NU_FALSE )
    {
        iStatus = XJP_DS_Freeze_Table( TableIndex );
        if( iStatus == XJ_SUCCESS )
        {
            /* ��ӳ�ʱ������ݺ��� */
            LSC_FreezeTable = TableIndex;
            /* ��ʱʱ������ */
            XJP_OverTime_Place_On( & LSC_FreezeTable, 120000, & LSC_FreezeTable,
                LSC_FreezeTabOvertimeCall );
        }
    }
    else
    {
        iStatus = XJP_DS_Freeze_Table( TableIndex );
    }

    * pDst = ( UNSIGNED_CHAR )( iStatus & LS_DATA_MASKS8 ); /* ִ�н�� */

    return( COM_DATA_FULL );
}

/* �������ݱ�ʱ�ص����� */
VOID LSC_FreezeTabOvertimeCall( void * argPtr )
{
    INT TableIndex;

    if( argPtr != XJ_NULL )
    {
        TableIndex = *( INT *)argPtr;
        XJP_DS_Freeze_Initialize( TableIndex );
    }
}
#endif

#if SWITCH_IEC103_TI_M_AE
/* �ⶳ���ݱ������ */
INT  LSC_UnFreeze_Table( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR    * pSrc,* pDst;
    INT                iStatus;
    INT                TableIndex;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 7;                                  /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_AE;                     /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                      /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                      /* ���кŸ��ֽ� */
    * pDst ++ = *( pSrc + 5 );                      /* ��������ŵ��ֽ� */
    * pDst ++ = *( pSrc + 6 );                      /* ��������Ÿ��ֽ� */

    /* ��������� */
    TableIndex = (*( pSrc + 5 ) & LS_DATA_MASKS8 );  /* ���ֽ� */
    TableIndex += ((*( pSrc + 6 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS );/* ���ֽ� */

    if( XJP_DS_Freeze_Status( TableIndex ) == NU_TRUE )
    {
        iStatus = XJP_DS_UnFreeze_Table( TableIndex );
        if( XJP_DS_Freeze_Status( TableIndex ) == NU_FALSE )
        {
            /* ������ʱ������ݺ��� */
            XJP_OverTime_Remove( & LSC_FreezeTable );
        }
    }
    else
    {
        iStatus = XJP_DS_UnFreeze_Table( TableIndex );
    }

    * pDst = ( UNSIGNED_CHAR )( iStatus & LS_DATA_MASKS8 ); /* ִ�н�� */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_AF
/* ���ö�������ֶε�ֵ */
INT LSC_Set_Batch_Config_Values( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR  * pSrc,* pDst;
    INT              i, j, k;
    INT              number, StartNumber, TabIndex;
    INT              iStatus;
    TB_CFB           CondField;
    UNSIGNED         Buffer[ LS_MAX_CONFIG_BYTE ];
    UNSIGNED_CHAR    Buffer1[ LS_MAX_CONFIG_BYTE ];
    INT              ValueWidth;
    VOID           * pTemp;
    TB_FCB          * pField;
    TB_TCB          * pTable;
    INT             iNumber;
    INT             iHandleNumber;  /* �����ֶθ��� */
    INT             iSize;
    INT             iOffset;  /* ��ַƫ�� */
    INT             originate;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    number = (*( pSrc + 5 ) & LS_DATA_MASKS8 );        /* �������ݵĸ��� */

    /* ��ʼ���ݵĸ��� */
    StartNumber = (*( pSrc + 6 ) & LS_DATA_MASKS8 );   /* ���ֽ� */
    StartNumber += ((*( pSrc + 7 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* ���ֽ� */

    /* ��������� */
    TabIndex = (*( pSrc + 8 ) & LS_DATA_MASKS8 );      /* ���ֽ� */
    TabIndex += ((*( pSrc + 9 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* ���ֽ� */

    CondField.CondNumber = (*( pSrc + 10 ) & LS_DATA_MASKS8 );/* �������� */
    iHandleNumber = *( pSrc + 11 + CondField.CondNumber * TB_CFB_COND_BYTE_NUMBER );/* �����ֶθ��� */

    * pDst ++ = 11 + ( 7 * CondField.CondNumber ) + ( ( 2 + 1 ) * iHandleNumber ); /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_AF;                     /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                      /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                      /* ���кŸ��ֽ� */
    * pDst ++ = *( pSrc + 5 );                      /* �������ݵĸ��� */
    * pDst ++ = *( pSrc + 6 );                      /* ��ʼ���ݵĸ������ֽ� */
    * pDst ++ = *( pSrc + 7 );                      /* ��ʼ���ݵĸ������ֽ� */
    * pDst ++ = *( pSrc + 8 );                      /* ��������ŵ��ֽ� */
    * pDst ++ = *( pSrc + 9 );                      /* ��������Ÿ��ֽ� */
    * pDst ++ = *( pSrc + 10 );                     /* �������� */

    for( i = 0; i < CondField.CondNumber; i ++ )
    {
        /* �������ֶ������� */
        CondField.CondIndex[ i ] = (*( pSrc + 11 + i * TB_CFB_COND_BYTE_NUMBER ) & LS_DATA_MASKS8 );/* ���ֽ� */
        CondField.CondIndex[ i ] += ((*( pSrc + 12 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* ���ֽ� */

        CondField.CondValue[ i ] = 0;
        for( k = 0; k < LS_DATA_WIDTH; k ++)
        {
            /* ������Լ���ֶ�ֵ */
            CondField.CondValue[ i ] += ((*( pSrc + 13 + i * TB_CFB_COND_BYTE_NUMBER + k ) & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
        }
        CondField.CondType[ i ] = (*( pSrc + 17 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );/* ���������� */

        * pDst ++ = *( pSrc + 11 + i * TB_CFB_COND_BYTE_NUMBER );     /* �����ֶ������ŵ��ֽ� */
        * pDst ++ = *( pSrc + 12 + i * TB_CFB_COND_BYTE_NUMBER );     /* �����ֶ������Ÿ��ֽ� */
        * pDst ++ = *( pSrc + 13 + i * TB_CFB_COND_BYTE_NUMBER );     /* ����Լ���ֶ�ֵ�ֽ�1 */
        * pDst ++ = *( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER );     /* ����Լ���ֶ�ֵ�ֽ�2 */
        * pDst ++ = *( pSrc + 15 + i * TB_CFB_COND_BYTE_NUMBER );     /* ����Լ���ֶ�ֵ�ֽ�3 */
        * pDst ++ = *( pSrc + 16 + i * TB_CFB_COND_BYTE_NUMBER );     /* ����Լ���ֶ�ֵ�ֽ�4 */
        * pDst ++ = *( pSrc + 17 + i * TB_CFB_COND_BYTE_NUMBER );     /* �������� */
    }

    * pDst ++ = iHandleNumber;    /* �����ֶθ��� */

    iSize = 0;                    /* �����ֶ������ŵ�ƫ�� */
    for( i = 0; i < iHandleNumber; i ++)
    {
        iOffset = CondField.CondNumber * TB_CFB_COND_BYTE_NUMBER + i * 2 + iSize; /* ��ȡ����ֵƫ��λ�� */

        * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 12 + iOffset ) & LS_DATA_MASKS8 );/* ���ֽ� */
        * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 13 + iOffset ) & LS_DATA_MASKS8 );/* ���ֽ� */
        CondField.HandleIndex = (*( pSrc + 12 + iOffset ) & LS_DATA_MASKS8 );/* ���ֽ� */
        /* ���ֽ� */
        CondField.HandleIndex += ((*( pSrc + 13 + iOffset ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );

        /* ͨ�����ݱ������ż������ֶ�������,�ҵ���Ӧ���ֶ���Ϣ */
        pTable = CTBC_FindTable( TabIndex );
        pField = CTBC_FindField( pTable, CondField.HandleIndex );
        /* һ�����ݵ����ݸ��� */
        iNumber = pField->Size / pField->TypeSize;
        iSize +=  number * ( iNumber * pField->TypeByte ); /* �������ݳ��� */
        for( k = 0; k < ( number * ( iNumber * pField->TypeByte ) ); k ++ )
        {
            /* �������� */
            Buffer1 [k] = (*( pSrc + 14 + iOffset + k ) & LS_DATA_MASKS8 );
        }
        ValueWidth = pField->TypeByte;    /* �ֶ������ֽ��� */
        /* ���ݿ�Ƚ�������1��4,��Ϊ1ʱ��������ΪUNSIGNED_CHAR,4:UNSIGNED */
        if( ValueWidth == 1 )
        {
            pTemp = ( VOID *)Buffer1;
        }
        else
        {
            for( j = 0; j < ( number * iNumber ); j ++ )
            {
                Buffer[j] = 0;
                for( k = 0; k < ValueWidth; k ++)
                {
                    /* �������ݿ�Ⱥϳ�ָ����ʽ������ */
                    Buffer[j] += (( Buffer1[j * ValueWidth + k] & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
                }
            }
            pTemp = ( VOID *)Buffer;
        }

        originate = ( INT )(*( pSrc - 1 ) & LC_VAR_CONTROL_BITS );
        iStatus = XJP_DS_Set_Field_Value_Config( TabIndex, StartNumber, number, & CondField, pTemp, originate );
        * pDst ++ = ( UNSIGNED_CHAR )( iStatus );
        if( iStatus != NU_SUCCESS )
        {
            k = *( pSrc - 1 ) & 0x07;  /* ��Чͨ�� */
            sprintf( ( CHAR *)Buffer1, "װ��������������ʧ�ܣ�table��%02d field��%02d org��%s result��%d",
                TabIndex, CondField.HandleIndex, LSC_CommChl[k], iStatus );
            CLOG_Log( ( CHAR *)Buffer1 );
        }
    }

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_B0
/* �����������ֶε�ֵ */
INT LSC_Save_Batch_Config_Values( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR  * pSrc,* pDst;
    INT              i, j, k;
    INT              number, StartNumber, TabIndex;
    INT              iStatus;
    TB_CFB           CondField;
    UNSIGNED         Buffer[ LS_MAX_CONFIG_BYTE ];
    UNSIGNED_CHAR    Buffer1[ LS_MAX_CONFIG_BYTE ];
    INT              ValueWidth;
    VOID           * pTemp;
    TB_FCB          * pField;
    TB_TCB          * pTable;
    INT             iNumber;
    INT             iHandleNumber;  /* �����ֶθ��� */
    INT             iSize;
    INT             iOffset;  /* ��ַƫ�� */
    INT             originate;
    INT             SaveFlag;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    number = (*( pSrc + 5 ) & LS_DATA_MASKS8 );        /* �������ݵĸ��� */

    /* ��ʼ���ݵĸ��� */
    StartNumber = (*( pSrc + 6 ) & LS_DATA_MASKS8 );   /* ���ֽ� */
    StartNumber += ((*( pSrc + 7 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* ���ֽ� */

    /* ��������� */
    TabIndex = (*( pSrc + 8 ) & LS_DATA_MASKS8 );      /* ���ֽ� */
    TabIndex += ((*( pSrc + 9 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* ���ֽ� */

    CondField.CondNumber = (*( pSrc + 10 ) & LS_DATA_MASKS8 );/* �������� */
    iHandleNumber = *( pSrc + 11 + CondField.CondNumber * TB_CFB_COND_BYTE_NUMBER );/* �����ֶθ��� */

    * pDst ++ = 12 + ( 7 * CondField.CondNumber ) + ( ( 2 + 1 ) * iHandleNumber ); /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_AF;                     /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                      /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                      /* ���кŸ��ֽ� */
    * pDst ++ = *( pSrc + 5 );                      /* �������ݵĸ��� */
    * pDst ++ = *( pSrc + 6 );                      /* ��ʼ���ݵĸ������ֽ� */
    * pDst ++ = *( pSrc + 7 );                      /* ��ʼ���ݵĸ������ֽ� */
    * pDst ++ = *( pSrc + 8 );                      /* ��������ŵ��ֽ� */
    * pDst ++ = *( pSrc + 9 );                      /* ��������Ÿ��ֽ� */
    * pDst ++ = *( pSrc + 10 );                     /* �������� */

    for( i = 0; i < CondField.CondNumber; i ++ )
    {
        /* �������ֶ������� */
        CondField.CondIndex[ i ] = (*( pSrc + 11 + i * TB_CFB_COND_BYTE_NUMBER ) & LS_DATA_MASKS8 );/* ���ֽ� */
        CondField.CondIndex[ i ] += ((*( pSrc + 12 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* ���ֽ� */

        CondField.CondValue[ i ] = 0;
        for( k = 0; k < LS_DATA_WIDTH; k ++)
        {
            /* ������Լ���ֶ�ֵ */
            CondField.CondValue[ i ] += ((*( pSrc + 13 + i * TB_CFB_COND_BYTE_NUMBER + k ) & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
        }
        CondField.CondType[ i ] = (*( pSrc + 17 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );/* ���������� */

        * pDst ++ = *( pSrc + 11 + i * TB_CFB_COND_BYTE_NUMBER );     /* �����ֶ������ŵ��ֽ� */
        * pDst ++ = *( pSrc + 12 + i * TB_CFB_COND_BYTE_NUMBER );     /* �����ֶ������Ÿ��ֽ� */
        * pDst ++ = *( pSrc + 13 + i * TB_CFB_COND_BYTE_NUMBER );     /* ����Լ���ֶ�ֵ�ֽ�1 */
        * pDst ++ = *( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER );     /* ����Լ���ֶ�ֵ�ֽ�2 */
        * pDst ++ = *( pSrc + 15 + i * TB_CFB_COND_BYTE_NUMBER );     /* ����Լ���ֶ�ֵ�ֽ�3 */
        * pDst ++ = *( pSrc + 16 + i * TB_CFB_COND_BYTE_NUMBER );     /* ����Լ���ֶ�ֵ�ֽ�4 */
        * pDst ++ = *( pSrc + 17 + i * TB_CFB_COND_BYTE_NUMBER );     /* �������� */
    }

    * pDst ++ = iHandleNumber;    /* �����ֶθ��� */

    SaveFlag = NU_TRUE;
    iSize = 0;                    /* �����ֶ������ŵ�ƫ�� */
    for( i = 0; i < iHandleNumber; i ++)
    {
        iOffset = CondField.CondNumber * TB_CFB_COND_BYTE_NUMBER + i * 2 + iSize; /* ��ȡ����ֵƫ��λ�� */

        * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 12 + iOffset ) & LS_DATA_MASKS8 );/* ���ֽ� */
        * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 13 + iOffset ) & LS_DATA_MASKS8 );/* ���ֽ� */
        CondField.HandleIndex = (*( pSrc + 12 + iOffset ) & LS_DATA_MASKS8 );/* ���ֽ� */
        /* ���ֽ� */
        CondField.HandleIndex += ((*( pSrc + 13 + iOffset ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );

        /* ͨ�����ݱ������ż������ֶ�������,�ҵ���Ӧ���ֶ���Ϣ */
        pTable = CTBC_FindTable( TabIndex );
        pField = CTBC_FindField( pTable, CondField.HandleIndex );
        /* һ�����ݵ����ݸ��� */
        iNumber = pField->Size / pField->TypeSize;
        iSize +=  number * ( iNumber * pField->TypeByte ); /* �������ݳ��� */
        for( k = 0; k < ( number * ( iNumber * pField->TypeByte ) ); k ++ )
        {
            /* �������� */
            Buffer1 [k] = (*( pSrc + 14 + iOffset + k ) & LS_DATA_MASKS8 );
        }
        ValueWidth = pField->TypeByte;    /* �ֶ������ֽ��� */
        /* ���ݿ�Ƚ�������1��4,��Ϊ1ʱ��������ΪUNSIGNED_CHAR,4:UNSIGNED */
        if( ValueWidth == 1 )
        {
            pTemp = ( VOID *)Buffer1;
        }
        else
        {
            for( j = 0; j < ( number * iNumber ); j ++ )
            {
                Buffer[j] = 0;
                for( k = 0; k < ValueWidth; k ++)
                {
                    /* �������ݿ�Ⱥϳ�ָ����ʽ������ */
                    Buffer[j] += (( Buffer1[j * ValueWidth + k] & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
                }
            }
            pTemp = ( VOID *)Buffer;
        }

        originate = ( INT )(*( pSrc - 1 ) & LC_VAR_CONTROL_BITS );
        iStatus = XJP_DS_Set_Field_Value_Config( TabIndex, StartNumber, number, & CondField, pTemp, originate );
        * pDst ++ = ( UNSIGNED_CHAR )( iStatus );
        if( iStatus != NU_SUCCESS )
        {
            SaveFlag = NU_FALSE;
        }
    }
    if( SaveFlag == NU_TRUE )
    {
        iStatus = XJP_DS_Save_Tab_Field_Value_Config( TabIndex, LS_TIME_OUT );
        * pDst ++ = ( UNSIGNED_CHAR )( iStatus );
        if( iStatus == NU_SUCCESS )
        {
            k = *( pSrc - 1 ) & 0x07;  /* ��Чͨ�� */
            sprintf( ( CHAR *)Buffer1, "װ�����������޸ĳɹ���table��%02d org��%s",
                TabIndex, LSC_CommChl[k] );
            CLOG_Log( ( CHAR *)Buffer1 );
        }
        else
        {
            k = *( pSrc - 1 ) & 0x07;  /* ��Чͨ�� */
            sprintf( ( CHAR *)Buffer1, "װ�����������޸�ʧ�ܣ�table��%02d org��%s result��%d",
                TabIndex, LSC_CommChl[k], iStatus );
            CLOG_Log( ( CHAR *)Buffer1 );
        }
    }
    else
    {
        k = *( pSrc - 1 ) & 0x07;  /* ��Чͨ�� */
        sprintf( ( CHAR *)Buffer1, "װ�����������޸�ʧ�ܣ�table��%02d org��%s result��%d",
            TabIndex, LSC_CommChl[k], iStatus );
        CLOG_Log( ( CHAR *)Buffer1 );
    }

    return( COM_DATA_FULL );
}
#endif

/* �ٻ�Ѳ���¼� */
INT LSC_Report_Event( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT      iStatus;
    //--------------------------------------------------------------------------
    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );
    LS_UNUSED_PARAM( Src );
    /* B6����Ĺ����� */
    iStatus = LSC_B6_MessageManagFun( Dst );
    /* return */
    return( iStatus );
}

#if SWITCH_IEC103_TI_M_C0
/* �ٻ��¼����� */
INT LSC_Load_Event_Type( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        uSetAvailable;
    UNSIGNED        i;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    uSetAvailable = (*( pSrc + 5 ) & LS_DATA_MASKS8 );   /* �ٻ����� */

    if( uSetAvailable != LS_DISABLE_DISPLAY )
    {
        * pDst ++ = ( UNSIGNED_CHAR )( LS_USER_RECORD_EVENT_GROUP + 7 ); /* ��·�û����ݳ��� */
    }
    else
    {
        * pDst ++ = ( UNSIGNED_CHAR )( LS_USER_RECORD_EVENT_GROUP + 8 ); /* ��·�û����ݳ��� */
    }
    * pDst ++ = IEC103_TI_M_C0;                                          /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                      /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                                           /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                                           /* ���кŸ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 );      /* �ٻ����� */

    if( uSetAvailable != LS_DISABLE_DISPLAY )
    {
        * pDst ++ = ( UNSIGNED_CHAR )( LS_USER_RECORD_EVENT_GROUP + 1 ); /* �¼����͸��� */
    }
    else
    {
        * pDst ++ = ( UNSIGNED_CHAR )( LS_USER_RECORD_EVENT_GROUP + 2 ); /* �¼����͸��� */
    }

    * pDst ++ = ALARM_EVENT_ID;                                          /* �澯�¼����� */

    if( uSetAvailable == LS_DISABLE_DISPLAY )
    {
        * pDst ++ = RESERVE_EVENT_ID;                                    /* �ڲ��¼����� */
    }

    for( i = 0; i < ( UNSIGNED )LS_USER_RECORD_EVENT_GROUP; i ++)
    {
        * pDst ++ = ( UNSIGNED_CHAR )( i + 1 );                          /* �û��¼����� */
    }

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_C1
/* �ٻ��¼���Ϣ */
INT LSC_Load_Event_Inf( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             StatusTemp;
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        uEvent_type;
    CEVENT          * pEvent;
    UNSIGNED        Type, Number;
    UNSIGNED        uStrtSerl, uEndSerl;
    INT             Status = COM_DATA_ERROR;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    uEvent_type = (*( pSrc + 5 ) & LS_DATA_MASKS8 );     /* �¼����� */

    * pDst ++ = 11;                                      /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_C1;                          /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );      /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                           /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                           /* ���кŸ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )uEvent_type;            /* �¼����� */

    pEvent = NU_NULL;
    if( uEvent_type == ALARM_EVENT_ID )
    {
        /* �澯�¼� */
        pEvent = & CAlarmEvent;
    }
    else if( uEvent_type == RESERVE_EVENT_ID )
    {
        /* �ڲ��¼� */
        pEvent = & CReserveEvent;
    }
    else if( uEvent_type == OPERATE_EVENT_ID )
    {
        /* �����¼� */
        pEvent = & COperateEvent;
    }
    else
    {
        /* �û��¼� */
        if( uEvent_type <= ( UNSIGNED )LS_USER_RECORD_EVENT_GROUP )
        {
            pEvent = PRO_EventRecord;
            pEvent += ( uEvent_type - 1 );
        }
    }

    if( pEvent != NU_NULL )
    {
        StatusTemp = pEvent->Information( pEvent,& Type,& Number,& uStrtSerl,
            & uEndSerl, LS_EVENT_TIMEOUT );
        if( StatusTemp == NU_SUCCESS )
        {
            /* �ܸ��� */
            * pDst ++ = ( UNSIGNED_CHAR )( Number & LS_DATA_MASKS8 ); /* ���ֽ� */
            * pDst ++ = ( UNSIGNED_CHAR )(( Number >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* ���ֽ� */
            /* ��ʼ�¼���� */
            * pDst ++ = ( UNSIGNED_CHAR )( uStrtSerl & LS_DATA_MASKS8 ); /* ���ֽ� */
            * pDst ++ = ( UNSIGNED_CHAR )(( uStrtSerl >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* ���ֽ� */
            /* �����¼���� */
            * pDst ++ = ( UNSIGNED_CHAR )( uEndSerl & LS_DATA_MASKS8 ); /* ���ֽ� */
            * pDst ++ = ( UNSIGNED_CHAR )(( uEndSerl >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* ���ֽ� */
            Status = COM_DATA_FULL;
        }
        else if( StatusTemp == NU_TIMEOUT )
        {
            Status = COM_DATA_TIMEOUT;
        }
        else
        {
            Status = COM_DATA_ERROR;
        }
    }
    return( Status );
}
#endif

#if SWITCH_IEC103_TI_M_C2
/* �ٻ��¼������� */
INT LSC_Load_Event_Serial( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             Status, StatusTemp, i;
    INT             Number;
    UNSIGNED        uTotal_Num, uEnd_Seril;
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        uEvent_type;
    CEVENT          * pEvent;
    UNSIGNED        buffer[CPC_MAX_NVRAM_EVENT_NUMBER];
    INT             iNumber;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    uEvent_type = (*( pSrc + 5 ) & LS_DATA_MASKS8 ); /* �¼����� */

    /* �¼��ܸ��� */
    uTotal_Num = *( pSrc + 6 ) & LS_DATA_MASKS8; /* ���ֽ� */
    uTotal_Num += ((*( pSrc + 7 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* ���ֽ� */

    /* �����¼���� */
    uEnd_Seril = *( pSrc + 8 ) & LS_DATA_MASKS8; /* ���ֽ� */
    uEnd_Seril += ((*( pSrc + 9 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* ���ֽ� */

    pEvent = NU_NULL;
    if( uEvent_type == ALARM_EVENT_ID )
    {
        /* �澯�¼� */
        pEvent = & CAlarmEvent;
    }
    else if( uEvent_type == RESERVE_EVENT_ID )
    {
        /* �ڲ��¼� */
        pEvent = & CReserveEvent;
    }
    else if( uEvent_type == OPERATE_EVENT_ID )
    {
        /* �����¼� */
        pEvent = & COperateEvent;
    }
    else
    {
        /* �û��¼� */
        if( uEvent_type <= ( UNSIGNED )LS_USER_RECORD_EVENT_GROUP )
        {
            pEvent = PRO_EventRecord;
            pEvent += ( uEvent_type - 1 );
        }
    }
    /* ��д�����¼������ŵ�ƫ���� */
    pDst += 8;
    if( pEvent != NU_NULL )
    {
        StatusTemp = pEvent->Find( pEvent, buffer,& Number, LS_EVENT_TIMEOUT );
        if( StatusTemp == NU_SUCCESS )
        {
            iNumber = 0;
            for( i = 0; i < Number; i ++)
            {
                if( buffer[i] == uEnd_Seril )
                {
                    /* ����������ʵ���¼����� */
                    iNumber = ( i + 1 );
                    break;
                }
            }
            /* �жϸ����Ƿ�������Χ֮�� */
            if( uTotal_Num > LC_MAX_EVENT_SERIAL )
            {
                uTotal_Num = LC_MAX_EVENT_SERIAL;
            }
            /* ����ٻ���������ʵ�ʸ���,��ʵ�ʸ��������� */
            if(( INT ) uTotal_Num > iNumber )
            {
                uTotal_Num = ( UNSIGNED ) iNumber;
            }

            /* ��д�¼�������,��end��ʼ */
            for( i = 0; i < ( INT )uTotal_Num; i ++)
            {
                /* �¼�ID */
                * pDst ++ = ( UNSIGNED_CHAR )( buffer[iNumber - 1 - i] & LS_DATA_MASKS8 ); /* ���ֽ� */
                * pDst ++ = ( UNSIGNED_CHAR )(( buffer[iNumber - 1 - i] >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* ���ֽ� */
            }
            Status = COM_DATA_FULL;
        }
        else if( StatusTemp == NU_TIMEOUT )
        {
            Status = COM_DATA_TIMEOUT;
        }
        else
        {
            Status = COM_DATA_ERROR;
        }
    }
    else
    {
        Status = COM_DATA_ERROR;
    }

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = ( UNSIGNED_CHAR )( uTotal_Num * 2 + 7 );     /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_C2;                              /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );          /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                               /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                               /* ���кŸ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )uEvent_type;                /* �¼����� */
    /* ʵ���¼����� */
    * pDst ++ = ( UNSIGNED_CHAR )( uTotal_Num & LS_DATA_MASKS8 ); /* ���ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(( uTotal_Num >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* ���ֽ� */

    return( Status );
}
#endif

#if SWITCH_IEC103_TI_M_C3
/* �ٻ��¼����� */
INT LSC_Load_Event_Data( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             iIndex;
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        uEvent_type, uLength, uStatus;
    INT             iNumber;
    INT             iActNumber;
    CEVENT          * pEvent;
    INT             i;
    CEM_NVD         EventNVData;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    iNumber = *( pSrc + 5 ) & LS_DATA_MASKS8; /* �¼����� */
    uLength = sizeof( CEM_NVD );              /* һ���¼����ĵĳ��� */

    pDst += 6; /* ��Ŀ�ĵ�ַָ���������д�¼����ݴ� */
    /* ʵ���¼����� */
    iActNumber = 0;
    for( i = 0; i < iNumber; i ++)
    {
        uEvent_type = *( pSrc + 6 + ( i * 3 ) ) & LS_DATA_MASKS8;     /* �¼����� */
        /* �¼���� */
        iIndex = *( pSrc + 7 + ( i * 3 )) & LS_DATA_MASKS8; /* ���ֽ� */
        iIndex += ((*( pSrc + 8 + ( i * 3 ) ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* ���ֽ� */

        pEvent = NU_NULL;
        if( uEvent_type == ALARM_EVENT_ID )
        {
            /* �澯�¼� */
            pEvent = & CAlarmEvent;
        }
        else if( uEvent_type == RESERVE_EVENT_ID )
        {
            /* �ڲ��¼� */
            pEvent = & CReserveEvent;
        }
        else if( uEvent_type == OPERATE_EVENT_ID )
        {
            /* �����¼� */
            pEvent = & COperateEvent;
        }
        else
        {
            /* �û��¼� */
            if( uEvent_type <= ( UNSIGNED )LS_USER_RECORD_EVENT_GROUP )
            {
                pEvent = PRO_EventRecord;
                pEvent += ( uEvent_type - 1 );
            }
        }

        if( pEvent != NU_NULL )
        {
            uStatus = pEvent->Read( pEvent, iIndex, & EventNVData, LS_EVENT_TIMEOUT );
            if( uStatus == NU_SUCCESS )
            {
                /* �¼�������1 */
                iActNumber ++;
                /* ���ͱ�־ */
                * pDst ++ = ( UNSIGNED_CHAR )EventNVData.Flag;
                /* ��ŵ��ֽ� */
                * pDst ++ = ( UNSIGNED_CHAR )EventNVData.IndexL;
                /* ��Ÿ��ֽ� */
                * pDst ++ = ( UNSIGNED_CHAR )EventNVData.IndexH;
                /* ���� */
                strncpy( ( CHAR *)pDst, ( CHAR *)EventNVData.Name, CEM_EVENT_NAME );
                pDst += CEM_EVENT_NAME;
                /* �� */
                * pDst ++ = ( UNSIGNED_CHAR )EventNVData.Year;
                /* �� */
                * pDst ++ = ( UNSIGNED_CHAR )EventNVData.Month;
                /* �� */
                * pDst ++ = ( UNSIGNED_CHAR )EventNVData.Day;
                /* ʱ */
                * pDst ++ = ( UNSIGNED_CHAR )EventNVData.Hour;
                /* �� */
                * pDst ++ = ( UNSIGNED_CHAR )EventNVData.Minute;
                /* ������ֽ� */
                * pDst ++ = ( UNSIGNED_CHAR )EventNVData.MillionsecondL;
                /* ������ֽ� */
                * pDst ++ = ( UNSIGNED_CHAR )EventNVData.MillionsecondH;
            }
        }
    }

    pDst = ( UNSIGNED_CHAR *)Dst;
    * pDst ++ = ( sizeof( CEM_NVD ) - 1 ) * iActNumber + 5; /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_C3;                             /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );         /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                              /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                              /* ���кŸ��ֽ� */
    * pDst ++ = iActNumber;                                 /* ʵ���¼����� */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_C4
/* ����¼����� */
INT LSC_Clear_Alarm_Event( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             i;
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        uEvent_type;
    INT             iResult = LS_RESULT_NULL;
    CEVENT          * pEvent;
    INT             Status, Status1, StatusTemp;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    Status1 = COM_DATA_EMPTY;

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    uEvent_type = *( pSrc + 5 ) & LS_DATA_MASKS8;  /* �¼����� */

    if( uEvent_type == LS_DATA_MASKS8 )
    {
        /* ���ȫ���¼����� */
        StatusTemp = CAlarmEvent.Clear(& CAlarmEvent, LS_EVENT_TIMEOUT ); /* ����澯�¼� */

        if( StatusTemp == NU_SUCCESS )
        {
            iResult = LS_RESULT_TRUE;
            Status1 |= COM_DATA_FULL;
        }
        else if( StatusTemp == NU_TIMEOUT )
        {
            Status1 |= COM_DATA_TIMEOUT;
        }
        else
        {
            Status1 |= COM_DATA_ERROR;
        }

        pEvent = PRO_EventRecord;
        for( i = 0; i < LS_USER_RECORD_EVENT_GROUP; i ++)
        {
            /* ����û��¼� */
            StatusTemp = pEvent->Clear( pEvent, LS_EVENT_TIMEOUT );
            if( StatusTemp == NU_SUCCESS )
            {
                Status1 |= COM_DATA_FULL;
            }
            else if( StatusTemp == NU_TIMEOUT )
            {
                iResult = LS_RESULT_NULL;
                Status1 |= COM_DATA_TIMEOUT;
            }
            else
            {
                iResult = LS_RESULT_NULL;
                Status1 |= COM_DATA_ERROR;
            }
            pEvent ++;
        }
    }
    else
    {
        /* ���ָ���¼����� */
        pEvent = NU_NULL;
        if( uEvent_type == ALARM_EVENT_ID )
        {
            /* �澯�¼� */
            pEvent = & CAlarmEvent;
        }
        else if( uEvent_type == RESERVE_EVENT_ID )
        {
            /* �ڲ��¼� */
            pEvent = & CReserveEvent;
        }

        else if( uEvent_type == OPERATE_EVENT_ID )
        {
            /* �����¼� */
            pEvent = & COperateEvent;
        }
        else
        {
            /* �û��¼� */
            if( uEvent_type <= ( UNSIGNED )LS_USER_RECORD_EVENT_GROUP )
            {
                pEvent = PRO_EventRecord;
                pEvent += ( uEvent_type - 1 );
            }
        }

        if( pEvent != NU_NULL )
        {
            StatusTemp = pEvent->Clear( pEvent, LS_EVENT_TIMEOUT );
            if( StatusTemp == NU_SUCCESS )
            {
                iResult = LS_RESULT_TRUE;
                Status1 = COM_DATA_FULL;
            }
            else if( StatusTemp == NU_TIMEOUT )
            {
                Status1 = COM_DATA_TIMEOUT;
            }
            else
            {
                Status1 = COM_DATA_ERROR;
            }
        }
        else
        {
            Status1 = COM_DATA_ERROR;
        }
    }

    if( Status1 == COM_DATA_FULL )
    {
        Status = COM_DATA_FULL;
    }
    else if(( Status1 & COM_DATA_TIMEOUT ) != 0 )
    {
        Status = COM_DATA_TIMEOUT;
    }
    else
    {
        Status = COM_DATA_ERROR;
    }

    * pDst ++ = 6;                                        /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_C4;                           /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );       /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                            /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                            /* ���кŸ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )uEvent_type;             /* �¼����� */
    * pDst ++ = ( UNSIGNED_CHAR )iResult;                 /* ִ�н�� */

    return( Status );
}
#endif

#if SWITCH_IEC103_TI_M_C5
/* �ٻ�������Ϣ */
INT LSC_Load_Report_Inf( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        StartFaultNumber, EndFaultNumber, Number, IndexNumber;
    INT             Status, StatusTemp;
    CREPORT         * pReport;
    INT             CpuNumber;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;
    CpuNumber = *( pSrc + 2 );                           /* CPU���� */
    * pDst ++ = 12;                                      /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_C5;                          /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );      /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                           /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                           /* ���кŸ��ֽ� */
    /* ��ȡ������Ϣ */
    pReport = CRMC_GetReportHandle( CpuNumber );
    if( pReport != NU_NULL )
    {
        /* ��ȡ������Ϣ */
        StatusTemp = pReport->Information( pReport,& Number,& IndexNumber,& StartFaultNumber,
            & EndFaultNumber, LS_REPORT_TIMEOUT );

        if( StatusTemp == NU_SUCCESS )
        {
            /* �����ܸ��� */
            * pDst ++ = ( UNSIGNED_CHAR )( Number & LS_DATA_MASKS8 ); /* ���ֽ� */
            * pDst ++ = ( UNSIGNED_CHAR )(( Number >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* ���ֽ� */

            /* ��ʼ������� */
            * pDst ++ = ( UNSIGNED_CHAR )( StartFaultNumber & LS_DATA_MASKS8 ); /* ���ֽ� */
            * pDst ++ = ( UNSIGNED_CHAR )(( StartFaultNumber >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* ���ֽ� */

            /* ����������� */
            * pDst ++ = ( UNSIGNED_CHAR )( EndFaultNumber & LS_DATA_MASKS8 ); /* ���ֽ� */
            * pDst ++ = ( UNSIGNED_CHAR )(( EndFaultNumber >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* ���ֽ� */

            /* ���������������� */
            * pDst ++ = ( UNSIGNED_CHAR )( IndexNumber & LS_DATA_MASKS8 ); /* ���ֽ� */
            * pDst ++ = ( UNSIGNED_CHAR )(( IndexNumber >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* ���ֽ� */

            Status = COM_DATA_FULL;
        }
        else if( StatusTemp == NU_TIMEOUT )
        {
            Status = COM_DATA_TIMEOUT;
        }
        else
        {
            Status = COM_DATA_ERROR;
        }
    }
    else
    {
        Status = COM_DATA_ERROR;
    }

    return( Status );
}
#endif

#if SWITCH_IEC103_TI_M_C6
/* �ٻ����������� */
INT LSC_Load_Report_FaultSerl( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    INT             Status, StatusTemp;
    UNSIGNED        Buffer[CPC_MAX_NVRAM_REPORT_NUMBER];
    INT             Type;
    INT             i;
    UNSIGNED        uTotal_Num;
    UNSIGNED        uEnd_FaultNum;
    UNSIGNED        uNumber;
    INT             number;
    CREPORT         * pReport;
    INT             CpuNumber;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;
    CpuNumber = *( pSrc + 2 );                           /* CPU���� */
    /* �ܸ��� */
    uTotal_Num = *( pSrc + 5 ) & LS_DATA_MASKS8; /* ���ֽ� */
    uTotal_Num += ((*( pSrc + 6 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* ���ֽ� */

    /* ����������� */
    uEnd_FaultNum = *( pSrc + 7 ) & LS_DATA_MASKS8; /* ���ֽ� */
    uEnd_FaultNum += ((*( pSrc + 8 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* ���ֽ� */

    number = 0;
    Type = REPORT_SOE_FLAG + REPORT_STATE_FLAG + REPORT_SCL_FLAG + REPORT_LOCAL_FLAG + REPORT_START_FLAG;

    /* ��ȡ������Ϣ */
    pReport = CRMC_GetReportHandle( CpuNumber );
    if( pReport != NU_NULL )
    {
        /* ��ȡ������Ϣ */
        StatusTemp = pReport->FindFaultNumber( pReport, Buffer, & number, Type, LS_REPORT_TIMEOUT );

        pDst += 7; /* ������д���ϱ�����ŵ�Ŀ�ĵ�ַ */
        if( StatusTemp == NU_SUCCESS )
        {
            uNumber = 0;
            for( i = 0; i < number; i ++)
            {
                if( Buffer[i] == uEnd_FaultNum )
                {
                    /* ����������ʵ�ʹ��ϱ�����Ÿ��� */
                    uNumber = ( i + 1 );
                    break;
                }
            }

            /* ����ٻ���������ʵ�ʸ���ȡʵ�ʸ��� */
            if( uTotal_Num > uNumber )
            {
                uTotal_Num = uNumber;
            }
            /* �ж��ϵ��ı�������Ƿ�������ķ�Χ֮��(���ĳ��Ⱦ���) */
            if( uTotal_Num > LC_MAX_REPORT_SERIAL )
            {
                uTotal_Num = LC_MAX_REPORT_SERIAL;
            }
            for( i = 0; i <( INT ) uTotal_Num; i ++)
            {
                /* ������� */
                * pDst ++ = ( UNSIGNED_CHAR )( Buffer[uNumber - 1 - i] & LS_DATA_MASKS8 ); /* ���ֽ� */
                * pDst ++ = ( UNSIGNED_CHAR )(( Buffer[uNumber - 1 - i] >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* ���ֽ� */
            }
            Status = COM_DATA_FULL;
        }
        else if( StatusTemp == NU_TIMEOUT )
        {
            Status = COM_DATA_TIMEOUT;
        }
        else
        {
            Status = COM_DATA_ERROR;
        }
    }
    else
    {
        uTotal_Num = 0;
        Status = COM_DATA_ERROR;
    }

    pDst = ( UNSIGNED_CHAR *)Dst;
    * pDst ++ = ( UNSIGNED_CHAR )( uTotal_Num * 2 + 6 ); /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_C6;                          /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );      /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                           /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                           /* ���кŸ��ֽ� */
    /* ʵ�ʸ��� */
    * pDst ++ = ( UNSIGNED_CHAR )( uTotal_Num & LS_DATA_MASKS8 ); /* ���ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(( uTotal_Num >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* ���ֽ� */

    return( Status );
}
#endif

#if SWITCH_IEC103_TI_M_C7
/* �ٻ������������ */
INT LSC_Load_Report_Serial( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             Status, StatusTemp;
    INT             Type;
    UNSIGNED        uTemp;
    INT             number;
    UNSIGNED_CHAR   * pSrc,* pDst;
    CREPORT         * pReport;
    INT             CpuNumber;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    CpuNumber = *( pSrc + 2 );                                     /* CPU���� */
    /* ������� */
    uTemp = *( pSrc + 5 ) & LS_DATA_MASKS8;                        /* ���ֽ� */
    uTemp += ((*( pSrc + 6 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* ���ֽ� */

    number = 0;
    Type = REPORT_SOE_FLAG + REPORT_STATE_FLAG + REPORT_SCL_FLAG + REPORT_LOCAL_FLAG + REPORT_START_FLAG;
    pReport = CRMC_GetReportHandle( CpuNumber );
    if( pReport != NU_NULL )
    {
        StatusTemp = pReport->FindIndexNumber( pReport, & number, uTemp, Type, LS_REPORT_TIMEOUT );
        if( StatusTemp == NU_SUCCESS )
        {
            * pDst ++ = 8;                                                  /* ���ĳ��� */
            * pDst ++ = IEC103_TI_M_C7;                                     /* ����ʶ */
            * pDst ++ = ( UNSIGNED_CHAR )CpuNumber;                         /* CPU���� */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 )& LS_DATA_MASKS8 );  /* ���кŵ��ֽ� */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 )& LS_DATA_MASKS8 );  /* ���кŸ��ֽ� */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 ); /* ������ŵ��ֽ� */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 ); /* ������Ÿ��ֽ� */
            /* �ж��ϵ��ı�������Ƿ�������ķ�Χ֮��(���ĳ��Ⱦ���) */
            if( number > LC_MAX_REPORT_SERIAL )
            {
                number = LC_MAX_REPORT_SERIAL;
            }
            /* ʵ�ʸ��� */
            * pDst ++ = ( UNSIGNED_CHAR )( number & LS_DATA_MASKS8 ); /* ���ֽ� */
            * pDst ++ = ( UNSIGNED_CHAR )(( number >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* ���ֽ� */

            Status = COM_DATA_FULL;
        }
        else if( StatusTemp == NU_TIMEOUT )
        {
            Status = COM_DATA_TIMEOUT;
        }
        else
        {
            * pDst ++ = 8;                                                   /* ���ĳ��� */
            * pDst ++ = IEC103_TI_M_C7;                                     /* ����ʶ */
            * pDst ++ = ( UNSIGNED_CHAR )CpuNumber;                         /* CPU���� */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 )& LS_DATA_MASKS8 );  /* ���кŵ��ֽ� */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 )& LS_DATA_MASKS8 );  /* ���кŸ��ֽ� */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 ); /* ������ŵ��ֽ� */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 ); /* ������Ÿ��ֽ� */
            * pDst ++ = 0;                                                  /* ʵ�ʸ������ֽ� */
            * pDst ++ = 0;                                                  /* ʵ�ʸ������ֽ� */

            Status = COM_DATA_FULL;
        }
    }
    else
    {
        Status = COM_DATA_ERROR;
    }

    return( Status );
}
#endif

#if SWITCH_IEC103_TI_M_C8
/* �ٻ��������� */
INT LSC_Load_Report_Data( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             iFaultNumber;
    INT             iStartIndex;
    INT             iIndexNumber;
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        uOffset, uSize;
    INT             StatusTemp;
    INT             Status;
    INT             iActLength;
    INT             i;
    CREPORT         * pReport;
    INT             CpuNumber;
    UNSIGNED        * p;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    CpuNumber = *( pSrc + 2 );                                      /* CPU���� */
    /* ������� */
    iFaultNumber = *( pSrc + 5 ) & LS_DATA_MASKS8; /* ���ֽ� */
    iFaultNumber += ((*( pSrc + 6 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* ���ֽ�1 */
    /* ��ʼ������ */
    iStartIndex = *( pSrc + 7 ) & LS_DATA_MASKS8; /* ���ֽ� */
    iStartIndex += ((*( pSrc + 8 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* ���ֽ� */
    /* �������� */
    iIndexNumber = *( pSrc + 9 ) & LS_DATA_MASKS8; /* ���ֽ� */
    iIndexNumber += ((*( pSrc + 10 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* ���ֽ� */
    /* ��ʼ��������ƫ�� */
    uOffset = *( pSrc + 11 ) & LS_DATA_MASKS8; /* ���ֽ� */
    uOffset += ((*( pSrc + 12 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* ���ֽ� */
    /* ��ȡ���ݴ�С */
    uSize = *( pSrc + 13 ) & LS_DATA_MASKS8; /* ���ֽ� */
    uSize += ((*( pSrc + 14 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* ���ֽ� */

    pReport = CRMC_GetReportHandle( CpuNumber );
    if( pReport == NU_NULL )
    {
        return( COM_DATA_ERROR );
    }

    /* ��д������Ϣ */
    pDst = ( UNSIGNED_CHAR *)Dst;
    * pDst ++ = ( UNSIGNED_CHAR )( iIndexNumber * uSize + 14 ); /* ���ĳ��� */
    * pDst ++ = IEC103_TI_M_C8;                           /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )CpuNumber;               /* CPU���� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ));         /* ���кŵ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ));         /* ���кŸ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ));         /* ������ŵ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ));         /* ������Ÿ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 7 ));         /* ��ʼ�����ŵ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 8 ));         /* ��ʼ�����Ÿ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 9 ));         /* �����������ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 10 ));        /* �����������ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 11 ));        /* ��ʼ����ƫ�Ƶ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 12 ));        /* ��ʼ����ƫ�Ƹ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 13 ));        /* ��ȡ���ݴ�С���ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 14 ));        /* ��ȡ���ݴ�С���ֽ� */

    /* �����ڴ�ռ� */
    p = ( UNSIGNED *)malloc(( LC_COMMAND_FRAME_SIZE + LS_DATA_WIDTH ) * 4 );
    if( p == NU_NULL )
    {
        return( COM_DATA_ERROR );
    }
    /* ��ȡ�������� */
    StatusTemp = pReport->Read( pReport, iFaultNumber, iStartIndex, iIndexNumber,
        uOffset, uSize, p, LS_REPORT_TIMEOUT );
    if( StatusTemp ==  NU_SUCCESS )
    {
        /* �������ݸ�ʽת�� */
        iActLength = ( iIndexNumber * uSize + sizeof( UNSIGNED )- 1 )/ sizeof( UNSIGNED );
        for( i = 0; i < iActLength; i ++)
        {
            LS_WORD_TO_BYTE( pDst, ( p + i ), LS_DATA_WIDTH ); /* �����ݴ���ʼ��ȡ */
            pDst += LS_DATA_WIDTH;
        }
        Status = COM_DATA_FULL;
    }
    else if( StatusTemp == NU_TIMEOUT )
    {
        Status = COM_DATA_TIMEOUT;
    }
    else
    {
        Status = COM_DATA_ERROR;
    }

    free( p );

    return( Status );
}
#endif

#if SWITCH_IEC103_TI_M_C9
/* ������ϼ�¼���� */
INT LSC_Clear_FaultData( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    INT             iType;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;
    iType = *( pSrc + 5 ) & LS_DATA_MASKS8; /* ������� */

    if( iType & LS_TYPE_CLEAR_REPORT )
    {
        LS_SET_CLR_REPORT();     /* �����������״̬ */
    }

    if( iType & LS_TYPE_CLEAR_WAVE )
    {
        LS_SET_CLR_WAVE( );       /* �������¼��״̬ */
    }

    * pDst ++ = 6;                                        /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_C9;                           /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );       /* CPU���� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ));         /* ���кŵ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ));         /* ���кŸ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 );/* ������� */
    * pDst ++ = LS_RESULT_TRUE;                           /* ִ�н�� */
    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_CA
/* �ֶ�����¼�� */
INT LSC_Start_Wave( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED        uLength, uResult = LS_RESULT_NULL, uStartPoint;
    UNSIGNED_CHAR   * pSrc,* pDst;
    INT             iSpace;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    iSpace = *( pSrc + 5 ) & LS_DATA_MASKS8;       /* ¼��������� */
    uLength = *( pSrc + 6 ) & LS_DATA_MASKS8; /* ¼�����ݳ��ȵ��ֽ� */
    uLength += ((*( pSrc + 7 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* ¼�����ݳ��ȸ��ֽ� */
    if( uLength >= LS_GET_CHANNEL_POINT )
    {
        uStartPoint = LS_GET_CHANNEL_POINT - 1;
    }
    else
    {
        uStartPoint = uLength;
    }

    if(( CSMV_GET_PERIOD_POINT % iSpace ) == 0 )
    {
        /* ��������¼����Ϣ */
        APFC_SetTestWaveParam( iSpace, uStartPoint, uLength );
        uResult = LS_RESULT_TRUE;
    }

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;
    * pDst ++ = 8;                                                  /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_CA;                                     /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                 /* CPU���� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 ); /* ���кŵ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 ); /* ���кŸ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 ); /* ¼��������� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 ); /* ¼�����ݳ��ȵ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 7 ) & LS_DATA_MASKS8 ); /* ¼�����ݳ��ȸ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )uResult;                           /* ִ�н�� */

    return( COM_DATA_FULL );
}
#endif

#define LSC_CB_MAX_WAVE_NUMBER  ((LC_MAX_FRAME_DATA_SIZE - 30)/2)  /* ֧���������¼����Ÿ��� */
#if SWITCH_IEC103_TI_M_CB
/* ͨ��¼����ŵĻ�������ȡ���ε������ϵ�¼����ż�����OffSet:λ��ƫ��
(����ֵԴ��ַ�������ĵ�ǰλ��ƫ��) */
static INT LSC_GetWaveIndexFromElecNumber( UNSIGNED * pSrcBuffer, INT iWaveIndexNumber,
    INT iElecNumber, UNSIGNED * pDstBuffer, INT * iNumber, INT OffSet, INT * NextElecNumber );
/* �ٻ�¼����� */
INT LSC_Load_Wave_ID( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        buffer[128];              // define buffer size.
    UNSIGNED        buffer1[128];             // define buffer size.
    INT             k, number, number1 = 0;
    INT             Status, StatusTemp, iType, iTestNum;
    UNSIGNED        uProFaultNum, uProFaultNum1, uProFaultNum2 = 0;
    UNSIGNED        uStartFaultNum;
    INT             iLen;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    iType = *( pSrc + 5 ) & LS_DATA_MASKS8;                         /* ���� */

    pDst ++;                                                        /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_CB;                                     /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                 /* CPU���� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 ); /* ���кŵ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 ); /* ���кŸ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )iType;                             /* ���� */
    if( iType == 0 )
    {
        /* �����ٻ����͵Ĳ�ͬ�������Ĳ�ͬƫ���� */
        iTestNum = 8;
        * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 ); /* ��ʼ������ŵ��ֽ� */
        * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 7 ) & LS_DATA_MASKS8 ); /* ��ʼ������Ÿ��ֽ� */
    }
    else
    {
        /* �����ٻ����͵Ĳ�ͬ�������Ĳ�ͬƫ���� */
        iTestNum = 6;
    }
    Status = COM_DATA_FULL;
    StatusTemp = CWave.Find(& CWave, 0, buffer,& number, LS_WAVE_TIMEOUT );
    if( StatusTemp == NU_SUCCESS )
    {
        if( iType == 1 )
        {
            * pDst ++;
            /* ��ȫ������¼���ĵ����������. */
            for( k = 0; k < number; k ++)
            {
                uProFaultNum1 = ( buffer[k] >> LS_DATA_BIT16 ) & LS_DATA_MASKS16;
                if( uProFaultNum1 != uProFaultNum2 )
                {
                    uProFaultNum2 = uProFaultNum1;
                    /* fill fault number. */
                    * pDst ++ = ( UNSIGNED_CHAR )( uProFaultNum1 & LS_DATA_MASKS8 );
                    /* fill fault number. */
                    * pDst ++ = ( UNSIGNED_CHAR )(( uProFaultNum1 >> LS_BYTE_BITS ) & LS_DATA_MASKS8 );
                    number1 ++;
                }
            }
            /* ��д���ĳ��� */
            pDst = ( UNSIGNED_CHAR *)Dst;
            * pDst = ( UNSIGNED_CHAR )( number1 * 2 + iTestNum );   // fill fault number.
            *( pDst + iTestNum ) = ( UNSIGNED_CHAR )number1;        // fill fault number.
        }
        else
        {
            INT  iNumber;
            INT  NextElecNumber;
            INT  iOffset;      /* ��ʼ������������е�λ��ƫ�� */
            INT  TempOffset;   /* ��ѯλ��ƫ�� */
            INT  iCount1 = 0;  /* �������ϸ��� */
            INT  iCount = 0;   /* ¼�������Ÿ��� */
            INT  i;
            INT  iWaveNumber;
            INT  iTempCount;

            /* ���������ϵĹ������. */
            /* ��ʼ������� */
            uStartFaultNum = *( pSrc + 6 ) & LS_DATA_MASKS8; /* ���ֽ� */
            uStartFaultNum += ((*( pSrc + 7 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* ���ֽ� */

            /* ����������ϴ��� */
            iWaveNumber = *( pSrc + 8 ) & LS_DATA_MASKS8;
            /* ��ʼ������������е�λ��ƫ�� */
            iOffset = *( pSrc + 9 ) & LS_DATA_MASKS8;

            TempOffset = number;   /* �������²��� */
            uProFaultNum = uStartFaultNum;
            pDst ++;  /* ʵ�ʵ������ϴ��� */
            for( k = 0; k < number; k ++)
            {
                TempOffset = LSC_GetWaveIndexFromElecNumber( buffer, number,
                    uProFaultNum, buffer1, & iNumber, TempOffset, & NextElecNumber );
                if( ( iCount + iNumber ) > LSC_CB_MAX_WAVE_NUMBER )
                {
                    if( iCount == 0 )
                    {
                        iNumber = LSC_CB_MAX_WAVE_NUMBER;
                    }
                    else
                    {
                        /* ¼�������ŵ�����������ֵ���� */
                        break;
                    }
                }

                iCount += iNumber;
                /* ��ʼ����������Ŵ���ƫ�� */
                if( uProFaultNum == uStartFaultNum )
                {
                    /* ��ʼλ��ƫ�� */
                    * pDst ++ = ( UNSIGNED_CHAR )iOffset;
                    /* ��ʼ����������ŵ���¼������ */
                    * pDst ++ = ( UNSIGNED_CHAR )iNumber;
                    if( iNumber > iOffset )
                    {
                        iTempCount = iNumber - iOffset;
                    }
                    else
                    {
                        iTempCount = 0;
                    }
                }
                else
                {
                    iOffset = 0;
                    iTempCount = iNumber;
                }
                /* ��д���� */
                * pDst ++ = ( UNSIGNED_CHAR )( uProFaultNum & LS_DATA_MASKS8 );
                * pDst ++ = ( UNSIGNED_CHAR )( ( uProFaultNum >> LS_BYTE_BITS ) & LS_DATA_MASKS8 );
                * pDst ++ = ( UNSIGNED_CHAR )iTempCount;
                for( i = iOffset; i < iTempCount; i ++)
                {
                    /* fill fault number. */
                    * pDst ++ = ( UNSIGNED_CHAR )( buffer1[i] & LS_DATA_MASKS8 );
                    /* fill fault number. */
                    * pDst ++ = ( UNSIGNED_CHAR )(( buffer1[i] >> LS_BYTE_BITS ) & LS_DATA_MASKS8 );
                }
                if(++ iCount1 >= iWaveNumber )
                {
                    /* ����������Ų�����ɷ��� */
                    break;
                }
                uProFaultNum = NextElecNumber;
            }

            /* ��д���ĳ��� */
            iLen = ( INT )(( UNSIGNED )pDst - ( UNSIGNED )Dst ) - 1;
            pDst = ( UNSIGNED_CHAR *)Dst;
            * pDst = ( UNSIGNED_CHAR )iLen; // fill fault number.
            *( pDst + iTestNum ) = ( UNSIGNED_CHAR )iCount1;        // ʵ�ʵ������ϴ���.
        }
    }
    else if( StatusTemp == NU_TIMEOUT )
    {
        Status = COM_DATA_TIMEOUT;
    }
    else
    {
        Status = COM_DATA_ERROR;
    }

    return( Status );
}

/* ͨ��¼����ŵĻ�������ȡ���ε������ϵ�¼����ż�����OffSet:λ��ƫ��
(����ֵԴ��ַ�������ĵ�ǰλ��ƫ��) */
static INT LSC_GetWaveIndexFromElecNumber( UNSIGNED * pSrcBuffer, INT iWaveIndexNumber,
    INT iElecNumber, UNSIGNED * pDstBuffer, INT * iNumber, INT OffSet, INT * NextElecNumber )
{
    UNSIGNED * pTemp;
    INT      i;
    INT      iFlag = 0;
    INT      iFaultNum1;
    INT      iCount = 0;
    INT      iOffset;
    UNSIGNED uStartFaultNum;
    INT      iStartOffset;
    /*==========================*/
    pTemp = ( UNSIGNED *)( pSrcBuffer + OffSet - 1 );
    uStartFaultNum = ( * pTemp >> LS_DATA_BIT16 ) & LS_DATA_MASKS16;
    iOffset = OffSet;
    iStartOffset = OffSet;
    if( iWaveIndexNumber >= OffSet )
    {
        for( i = 0; i < OffSet; i ++)
        {
            iFaultNum1 = ( *( pTemp - i ) >> LS_DATA_BIT16 ) & LS_DATA_MASKS16;
            if( iElecNumber == iFaultNum1 )
            {
                iFlag = 1;
                *( pDstBuffer + iCount ++) = *( pTemp - i );
            }
            else
            {
                /* �ж�ͬһ����������ŵ�¼���Ƿ�������� */
                if( iFlag == 1 )
                {
                    /* ��һ����������� */
                    * NextElecNumber = iFaultNum1;
                    break;
                }
            }
            iOffset --;
        }
    }
    * iNumber = iCount;

    /* ���ָ��������ŵ�¼��������,����һ����������ŷ������뻺�����ĵ�һ������������� */
    if( iCount == 0 )
    {
        /* ��һ����������� */
        * NextElecNumber = uStartFaultNum;
        /* ƫ��λ�û�ԭΪ������ʼλ�� */
        iOffset = iStartOffset;
    }

    return( iOffset );
}
#endif

#if SWITCH_IEC103_TI_M_CC
/* �ٻ�¼��ʱ�� */
INT LSC_Load_Wave_Time( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED        FaultNumber;
    INT             Status, StatusTemp;
    UNSIGNED_CHAR   * pSrc,* pDst;
    CWM_WHD         WaveHead,* pWaveHead;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 17;                                                 /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_CC;                                     /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                 /* CPU���� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 ); /* ���кŵ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 ); /* ���кŸ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 ); /* ¼����ŵ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 ); /* ¼����Ÿ��ֽ� */

    /* ¼����� */
    FaultNumber = *( pSrc + 6 );    /* ���ֽ� */
    FaultNumber *= 256;             // fault number.
    FaultNumber += *( pSrc + 5 );   /* ���ֽ� */

    /* ��¼������ */
    StatusTemp = CWave.Read(& CWave, FaultNumber,& WaveHead, 0, 0, sizeof( CWM_WHD ),
        LS_WAVE_TIMEOUT );

    if( StatusTemp == NU_SUCCESS )
    {
        pWaveHead = & WaveHead;

        * pDst ++ = LS_RESULT_TRUE; /* ״̬ 0x5A:�ɹ�,0x00:���� */
        * pDst ++ = ( UNSIGNED_CHAR )( pWaveHead->Sof ); /* ¼������ */

        /* ����������� */
        * pDst ++ = ( UNSIGNED_CHAR )( pWaveHead->ElecNumber & LS_DATA_MASKS8 ); /* ���ֽ� */
        * pDst ++ = ( UNSIGNED_CHAR )(( pWaveHead->ElecNumber >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* ���ֽ� */

        * pDst ++ = ( UNSIGNED_CHAR )( pWaveHead->Year );    /* �� */
        * pDst ++ = ( UNSIGNED_CHAR )( pWaveHead->Month );   /* �� */
        * pDst ++ = ( UNSIGNED_CHAR )( pWaveHead->Day );     /* �� */
        * pDst ++ = ( UNSIGNED_CHAR )( pWaveHead->Hour );    /* ʱ */
        * pDst ++ = ( UNSIGNED_CHAR )( pWaveHead->Minute );  /* �� */

        /* �루���룩 */
        * pDst ++ = ( UNSIGNED_CHAR )( pWaveHead->Millionsecond & LS_DATA_MASKS8 ); /* ���ֽ� */
        * pDst ++ = ( UNSIGNED_CHAR )(( pWaveHead->Millionsecond >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* ���ֽ� */

        Status = COM_DATA_FULL;
    }
    else if( StatusTemp == NU_TIMEOUT )
    {
        Status = COM_DATA_TIMEOUT;
    }
    else
    {
        * pDst ++ = LS_RESULT_NULL; /* ״̬ 0x5A:�ɹ�,0x00:���� */
        Status = COM_DATA_FULL;
    }

    return( Status );
}
#endif

#if SWITCH_IEC103_TI_M_CD
/* �ٻ�¼����Ϣ */
INT LSC_Load_Wave_Inf( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        StartFaultNumber, EndFaultNumber, Number, Serial;
    INT             Status, StatusTemp;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 12;                                      /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_CD;                          /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );      /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                           /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                           /* ���кŸ��ֽ� */

    /* ��ȡ������Ϣ */
    StatusTemp = CWave.Information(& CWave,& Number,& Serial,& StartFaultNumber,
        & EndFaultNumber, LS_WAVE_TIMEOUT );

    if( StatusTemp == NU_SUCCESS )
    {
        /* �����ܸ��� */
        * pDst ++ = ( UNSIGNED_CHAR )( Number & LS_DATA_MASKS8 ); /* ���ֽ� */
        * pDst ++ = ( UNSIGNED_CHAR )(( Number >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* ���ֽ� */

        /* ��ʼ������� */
        * pDst ++ = ( UNSIGNED_CHAR )( StartFaultNumber & LS_DATA_MASKS8 ); /* ���ֽ� */
        * pDst ++ = ( UNSIGNED_CHAR )(( StartFaultNumber >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* ���ֽ� */

        /* ����������� */
        * pDst ++ = ( UNSIGNED_CHAR )( EndFaultNumber & LS_DATA_MASKS8 ); /* ���ֽ� */
        * pDst ++ = ( UNSIGNED_CHAR )(( EndFaultNumber >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* ���ֽ� */

        /* �������������� */
        * pDst ++ = ( UNSIGNED_CHAR )( Serial & LS_DATA_MASKS8 ); /* ���ֽ� */
        * pDst ++ = ( UNSIGNED_CHAR )(( Serial >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* ���ֽ� */

        Status = COM_DATA_FULL;
    }
    else if( StatusTemp == NU_TIMEOUT )
    {
        Status = COM_DATA_TIMEOUT;
    }
    else
    {
        Status = COM_DATA_ERROR;
    }

    return( Status );
}
#endif

#if SWITCH_IEC103_TI_M_CE
/* �ٻ�¼������������� */
INT LSC_Load_Wave_FaultSerl( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    INT             Status, StatusTemp;
    UNSIGNED        Buffer[CPC_MAX_NVRAM_WAVE_NUMBER];
    INT             i;
    UNSIGNED        uTotal_Num;
    UNSIGNED        uEnd_FaultNum;
    UNSIGNED        uNumber;
    INT             number;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    /* �ܸ��� */
    uTotal_Num = *( pSrc + 5 ) & LS_DATA_MASKS8; /* ���ֽ� */
    uTotal_Num += ((*( pSrc + 6 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* ���ֽ� */

    /* ����������� */
    uEnd_FaultNum = *( pSrc + 7 ) & LS_DATA_MASKS8; /* ���ֽ� */
    uEnd_FaultNum += ((*( pSrc + 8 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* ���ֽ� */

    number = 0;
    /* ��ȡ������Ϣ */
    StatusTemp = CWave.FindFaultNumber( & CWave, Buffer, & number, 1, LS_WAVE_TIMEOUT );

    pDst += 7; /* ������д���ϱ�����ŵ�Ŀ�ĵ�ַ */
    if( StatusTemp == NU_SUCCESS )
    {
        uNumber = 0;
        for( i = 0; i < number; i ++)
        {
            if( Buffer[i] == uEnd_FaultNum )
            {
                /* ����������ʵ�ʹ��ϱ�����Ÿ��� */
                uNumber = ( i + 1 );
                break;
            }
        }

        /* ����ٻ���������ʵ�ʸ���ȡʵ�ʸ��� */
        if( uTotal_Num > uNumber )
        {
            uTotal_Num = uNumber;
        }

        /* �ж��ϵ���¼�������Ƿ�������ķ�Χ֮��(���ĳ��Ⱦ���) */
        if( uTotal_Num > LC_MAX_WAVE_SERIAL )
        {
            uTotal_Num = LC_MAX_WAVE_SERIAL;
        }

        for( i = 0; i <( INT ) uTotal_Num; i ++)
        {
            /* ��д�¼�������,��end��ʼ */
            for( i = 0; i < ( INT )uTotal_Num; i ++)
            {
                /* ������� */
                * pDst ++ = ( UNSIGNED_CHAR )( Buffer[uNumber - 1 - i] & LS_DATA_MASKS8 ); /* ���ֽ� */
                * pDst ++ = ( UNSIGNED_CHAR )(( Buffer[uNumber - 1 - i] >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* ���ֽ� */
            }
        }
        Status = COM_DATA_FULL;
    }
    else if( StatusTemp == NU_TIMEOUT )
    {
        Status = COM_DATA_TIMEOUT;
    }
    else
    {
        Status = COM_DATA_ERROR;
    }

    pDst = ( UNSIGNED_CHAR *)Dst;
    * pDst ++ = ( UNSIGNED_CHAR )( uTotal_Num * 2 + 6 ); /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_CE;                          /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );      /* CPU���� */
    * pDst ++ = *( pSrc + 3 );                           /* ���кŵ��ֽ� */
    * pDst ++ = *( pSrc + 4 );                           /* ���кŸ��ֽ� */
    /* ʵ�ʸ��� */
    * pDst ++ = ( UNSIGNED_CHAR )( uTotal_Num & LS_DATA_MASKS8 ); /* ���ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(( uTotal_Num >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* ���ֽ� */

    return( Status );
}
#endif

#if SWITCH_IEC103_TI_M_E0
/* Ԥ���������� */
INT LSC_Trip_Test_Ready( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             iType, iActualType;
    INT             iResult = LS_RESULT_NULL;
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        uGroupNum;
    UNSIGNED        uGroupID[CPC_MAX_OUTPUT_GROUPS];
    INT             i;
    RJUMPER         * pRJumper;
    INT             jum_status;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;

    iType = (*( pSrc + 5 ) & LS_DATA_MASKS8 );     /* �������� */
    uGroupNum = (*( pSrc + 6 ) & LS_DATA_MASKS8 ); /* �������� */
    iActualType = 0;

    pRJumper = GET_RJUMPER_TAB_MEMORY;
    if(( pRJumper != NU_NULL ) && ( GET_RJUMPER_TAB_NUMBER != 0 ))
    {
        /* ����ѹ���б� */
        if( pRJumper-> dataType == POINT_TYPE_SINGLE )
        {
            /* ���� */
            jum_status = SINGLE_POINT_OPT;
        }
        else
        {
            /* ˫�� */
            jum_status = DUAL_POINT_OPT;
        }

        if( pRJumper->jum_status != jum_status )
        {
            /* ����ѹ��δͶ��,ֱ�ӷ���ʧ�� */
            pSrc = ( UNSIGNED_CHAR *)Src;
            pDst = ( UNSIGNED_CHAR *)Dst;

            * pDst ++ = ( UNSIGNED_CHAR )( 7 + uGroupNum *( 1 + LS_DATA_WIDTH )); /* ��·�û����ݳ��� */
            * pDst ++ = IEC103_TI_M_E0;                                           /* ����ʶ */
            * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                       /* CPU���� */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 );       /* ���кŵ��ֽ� */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 );       /* ���кŸ��ֽ� */
            * pDst ++ = ( UNSIGNED_CHAR )iActualType;                             /* �������� */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 );       /* �������� */
            for( i = 0; i < ( INT )( uGroupNum *( 1 + LS_DATA_WIDTH )); i ++)
            {
                * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 7 + i ) & LS_DATA_MASKS8 ); /* ����λ */
            }
            * pDst ++ = ( UNSIGNED_CHAR )iResult;                                   /* ִ��״̬ */

            return( COM_DATA_FULL );
        }
    }

    if(( iType & LS_START_RELAY_TEST ) && ( LS_CPU_TYPE & CPU_TYPE_STARTUP ))
    {
        /* test start relay */
        iActualType |= LS_START_RELAY_TEST;
        iResult = LS_RESULT_TRUE;
    }

    if(( iType & LS_OUT_RELAY_TEST ) != 0 )
    {
        /* �������������������������,����ʧ�� */
        if( uGroupNum > CPC_MAX_OUTPUT_GROUPS )
        {
            iResult = LS_RESULT_NULL;
        }
        else
        {
            iResult = LS_RESULT_TRUE;
            iActualType |= LS_OUT_RELAY_TEST;
            for( i = 0; i < ( INT )uGroupNum; i ++)
            {
                /* ���·���ͨ������ȼ�¼����,�����Ƿ�����Ҫ�� */
                uGroupID[i] = (*( pSrc + 7 + i * LS_DATA_WIDTH ) & LS_DATA_MASKS8 );
            }

            for( i = 0; i < ( INT )uGroupNum; i ++)
            {
                if( uGroupID[i] >= CPC_MAX_OUTPUT_GROUPS )
                {
                    iResult = LS_RESULT_NULL;
                    break;
                }
            }
        }
    }

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = ( UNSIGNED_CHAR )( 7 + uGroupNum *( 1 + LS_DATA_WIDTH )); /* ��·�û����ݳ��� */
    * pDst ++ = IEC103_TI_M_E0;                                           /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                       /* CPU���� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 );       /* ���кŵ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 );       /* ���кŸ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )iActualType;                             /* �������� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 );       /* �������� */
    for( i = 0; i < ( INT )( uGroupNum *( 1 + LS_DATA_WIDTH )); i ++)
    {
        * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 7 + i ) & LS_DATA_MASKS8 ); /* ����λ */
    }
    * pDst ++ = ( UNSIGNED_CHAR )iResult;                                   /* ִ��״̬ */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_E1
/* ִ�д������� */
INT LSC_Trip_Test( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             iType, iActualType;
    INT             iResult = LS_RESULT_NULL;
    UNSIGNED_CHAR   * pSrc,* pDst;
    CHAR            EventName[LS_ALARM_NAME];
    UNSIGNED        uGroupNum;
    UNSIGNED        uGroupID[CPC_MAX_OUTPUT_GROUPS * 2]; /* buffer size */
    INT             i, j;
    INT             iFlag; /* ��ͨ������ʱ,�����ж��źŵƺ���բ���Ƿ��Ѿ�ִ�й���־.1:�ź�;2:��բ;3:�ź�+��բ */
    UNSIGNED        * pTripLedMemory = LS_GET_OUTPUT_TRIP_LED_MEMORY;
    UNSIGNED        * pSignalLedMemory = LS_GET_OUTPUT_SIGNAL_LED_MEMORY;
    UNSIGNED        uLedBits;
    RJUMPER         * pRJumper;
    INT             jum_status;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;

    iType = (*( pSrc + 5 ) & LS_DATA_MASKS8 );     // ��������
    uGroupNum = (*( pSrc + 6 ) & LS_DATA_MASKS8 ); // ��������
    iActualType = 0;

    pRJumper = GET_RJUMPER_TAB_MEMORY;
    if(( pRJumper != NU_NULL ) && ( GET_RJUMPER_TAB_NUMBER != 0 ))
    {
        /* ����ѹ���б� */
        if( pRJumper-> dataType == POINT_TYPE_SINGLE )
        {
            /* ���� */
            jum_status = SINGLE_POINT_OPT;
        }
        else
        {
            /* ˫�� */
            jum_status = DUAL_POINT_OPT;
        }

        if( pRJumper->jum_status != jum_status )
        {
            /* ����ѹ��δͶ��,ֱ�ӷ���ʧ�� */
            pSrc = ( UNSIGNED_CHAR *)Src;
            pDst = ( UNSIGNED_CHAR *)Dst;

            * pDst ++ = ( UNSIGNED_CHAR )( 7 + uGroupNum *( 1 + LS_DATA_WIDTH )); // fill report length.
            * pDst ++ = IEC103_TI_M_E1;                                // fill command type.
            * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );            // fill cpu number.
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 ); // fill serial number: byte 1
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 ); // fill serial number: byte 2
            * pDst ++ = ( UNSIGNED_CHAR )iActualType;                       // fill test type.
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 ); // fill output group number.
            for( i = 0; i < ( INT )( uGroupNum *( 1 + LS_DATA_WIDTH )); i ++)
            {
                * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 7 + i ) & LS_DATA_MASKS8 ); // fill output bits.
            }
            * pDst ++ = ( UNSIGNED_CHAR )iResult;                           // fill result.

            return( COM_DATA_FULL );
        }
    }

    if(( iType & LS_START_RELAY_TEST ) && ( LS_CPU_TYPE & CPU_TYPE_STARTUP ))
    {
        /* test start relay */
        NU_Relinquish();

        /* �����̵������� */
        XJP_IO_Set_Start_Relay( PlatHIOM, 1 );
        sprintf( EventName, " %03d", LS_RES_EVENT_START );
        CEMC_FillEvent( EventName, RESERVE_EVENT_ID );
        iActualType |= LS_START_RELAY_TEST;
        iResult = LS_RESULT_TRUE;
        NU_Control_Timer(& Timer_Trip_CB, NU_ENABLE_TIMER );
    }

    if(( iType & ( LS_OUT_RELAY_TEST | LS_TRIP_RELAY_TEST ) ) != 0 )
    {
        /* �������������������������,����ʧ�� */
        if( uGroupNum > CPC_MAX_OUTPUT_GROUPS )
        {
            iResult = LS_RESULT_NULL;
        }
        else
        {
            iResult = LS_RESULT_TRUE;

            /* �жϴ������� */
            if( iType & LS_OUT_RELAY_TEST )
            {
                iActualType |= LS_OUT_RELAY_TEST;
            }
            else
            {
                iActualType |= LS_TRIP_RELAY_TEST;
            }

            for( i = 0; i < ( INT )uGroupNum ; i ++)
            {
                /* ���·���ͨ������ȼ�¼����,�����Ƿ�����Ҫ�� */
                uGroupID[i * 2] = (*( pSrc + 7 + i * ( LS_DATA_WIDTH + 1 ) ) & LS_DATA_MASKS8 );
                /* ��¼����λ */
                LS_BYTE_TO_WORD( & uGroupID[2 * i + 1],( pSrc + 8 + i * ( LS_DATA_WIDTH + 1 ) ), LS_DATA_WIDTH );
            }

            for( i = 0; i < ( INT )uGroupNum; i ++)
            {
                /* �жϳ�������Ƿ�������ķ�Χ֮�� */
                if( uGroupID[i * 2] >= CPC_MAX_OUTPUT_GROUPS )
                {
                    iResult = LS_RESULT_NULL;
                    break;
                }
            }
        }
        if( iResult == LS_RESULT_TRUE )
        {
            NU_Relinquish();

            /* ���Ϊ��բcpu,��������·����ʱҲҪ�������̵��� */
            if(( LS_CPU_TYPE & CPU_TYPE_TRIP )
                && ( LS_CPU_TYPE & CPU_TYPE_SERIAL ))
            {
                XJP_IO_Set_Start_Relay( PlatHIOM, 1 );
            }

            iFlag = 0;
            LC_REPORT_PROTECT();
            for( i = 0; i < ( INT )uGroupNum; i ++)
            {
                /* ���ÿ���λ */
                XJP_IO_Set_Output_Relay( PlatHIOM, uGroupID[i * 2], uGroupID[i * 2 + 1], CIO_EQU );
                if( iType & LS_OUT_RELAY_TEST )
                {
                    for( j = 0; j < LS_GET_OUTPUT_GROUPS; j ++)
                    {
                        uLedBits = *( pSignalLedMemory + j );
                        uLedBits |= *( pTripLedMemory + j );
                        /* ����װ�õƿ��� */
                        XJP_IO_Set_Output_Relay( PlatHIOM, j, uLedBits, CIO_OR );
                    }
                }
            }
            LS_REPORT_UNPROTECT();
            sprintf( EventName, " %03d:%02d ", LS_RES_EVENT_OUT, uGroupNum );
            CEMC_FillEvent( EventName, RESERVE_EVENT_ID );
        }
    }

    if( iResult == LS_RESULT_TRUE )
    {
        NU_Control_Timer(& Timer_Trip_CB, NU_ENABLE_TIMER );
    }

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = ( UNSIGNED_CHAR )( 7 + uGroupNum *( 1 + LS_DATA_WIDTH ));                       // fill report length.
    * pDst ++ = IEC103_TI_M_E1;                                      // fill command type.
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );            // fill cpu number.
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 ); // fill serial number: byte 1
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 ); // fill serial number: byte 2
    * pDst ++ = ( UNSIGNED_CHAR )iActualType;                        // fill test type.
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 ); // fill output group number.
    for( i = 0; i < ( INT )( uGroupNum *( 1 + LS_DATA_WIDTH )); i ++)
    {
        * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 7 + i ) & LS_DATA_MASKS8 ); // fill output bits.
    }
    * pDst ++ = ( UNSIGNED_CHAR )iResult;                            // fill result.

    return( COM_DATA_FULL );
}
#endif

//
// trip test timer main function
//
void LSC_TripTimerRoutine( UNSIGNED argc )
{
    INT       j;
    UNSIGNED  uOutBits[CPC_MAX_OUTPUT_GROUPS];
    //--------------------------------------------------------------------------
    LS_UNUSED_PARAM( argc );
    /* ������λ���ó�ֵ0 */
    memset( uOutBits, 0,( sizeof( UNSIGNED )* CPC_MAX_OUTPUT_GROUPS ));
    /* �����ջ�ʱҪ�������ݱ��� */
    LS_REPORT_PROTECT();
    /* �����̵����ջ� */
    XJP_IO_Set_Start_Relay( PlatHIOM, 0 );
    /* �ջش����ĳ���λ */
    for( j = 0; j < CPC_MAX_OUTPUT_GROUPS; j ++)
    {
        XJP_IO_Set_Output_Relay( PlatHIOM, j, 0, CIO_EQU );
    }
    LS_REPORT_UNPROTECT();

    NU_Control_Timer(& Timer_Trip_CB, NU_DISABLE_TIMER );
}

#if SWITCH_IEC103_TI_M_E2
/* ��ͨ��ϵ������ֵ */
INT LSC_Load_Channel_Cofficient( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             k;
    INT             Length;
    INT             Counter;
    INT             ChlNumber;
    UNSIGNED        uTemp;
    PRO_CHANNEL     * pChannel;
    float           Variable_r, Variable_i, Variable;
    float           ChannelCoefficient;
    UNSIGNED_CHAR   * pSrc,* pDst,* p;
    INT             SampleOffset;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    pDst += 6; /* ͨ��ID��ַ */
    Counter = *( pSrc + 5 ) & LS_DATA_MASKS8;  /* ͨ������ */
    /* ��ȡ����ƫ��λ�� */
    SampleOffset = CSMV_GET_SAMPLE_OFFSET;
    for( k = 0; k < Counter; k ++)
    {
        ChlNumber = *( pSrc + k + 6 ) & LS_DATA_MASKS8; /* ͨ��ID */
        pChannel = LS_CHANNEL_TABLE_MEMORY; /* ͨ�����ݱ��ڴ��׵�ַ */
        pChannel += ChlNumber;
        if( ChlNumber < LS_CHANNEL_TABLE_NUMBER )
        {
            if(( pChannel -> chl_type & CHL_PEAKVALUE ) != 0 )
            {
                /* ��ֵ */
                Variable = LS_CALCULATE_PEAK_VALUE( ChlNumber, SampleOffset );
                Variable = Variable * pChannel -> chl_rate_coefficient;
                Variable = Variable / pChannel -> chl_calculate_coefficient;
                Variable *= ( float ) sqrt( 2 ); /* �������Чֵ */
            }
            else if(( pChannel -> chl_type & FUNDAMENTAL ) != 0 )
            {
                /* �������� */
                Variable_r = LS_CALCULATE_FOURIER( ChlNumber, FOURIER01_R, SampleOffset );
                Variable_i = LS_CALCULATE_FOURIER( ChlNumber, FOURIER01_I, SampleOffset );
                Variable = Variable_r * Variable_r + Variable_i * Variable_i;
                Variable = ( float )sqrt( Variable );
                Variable = Variable * pChannel -> chl_rate_coefficient;
                Variable = Variable / pChannel -> chl_calculate_coefficient;
            }
            else if(( pChannel -> chl_type & HARMONIC_2ND ) != 0 )
            {
                /* ����г������ */
                Variable_r = LS_CALCULATE_FOURIER( ChlNumber, FOURIER02_R, SampleOffset );
                Variable_i = LS_CALCULATE_FOURIER( ChlNumber, FOURIER02_I, SampleOffset );
                Variable = Variable_r * Variable_r + Variable_i * Variable_i;
                Variable = ( float )sqrt( Variable );
                Variable = Variable * pChannel -> chl_rate_coefficient;
                Variable = Variable / pChannel -> chl_calculate_coefficient;
            }
            else if(( pChannel -> chl_type & HARMONIC_3RD ) != 0 )
            {
                /* ����г������ */
                Variable_r = LS_CALCULATE_FOURIER( ChlNumber, FOURIER03_R, SampleOffset );
                Variable_i = LS_CALCULATE_FOURIER( ChlNumber, FOURIER03_I, SampleOffset );
                Variable = Variable_r * Variable_r + Variable_i * Variable_i;
                Variable = ( float )sqrt( Variable );
                Variable = Variable * pChannel -> chl_rate_coefficient;
                Variable = Variable / pChannel -> chl_calculate_coefficient;
            }
            else if(( pChannel -> chl_type & HARMONIC_5TH ) != 0 )
            {
                /* ���г������ */
                Variable_r = LS_CALCULATE_FOURIER( ChlNumber, FOURIER05_R, SampleOffset );
                Variable_i = LS_CALCULATE_FOURIER( ChlNumber, FOURIER05_I, SampleOffset );
                Variable = Variable_r * Variable_r + Variable_i * Variable_i;
                Variable = ( float )sqrt( Variable );
                Variable = Variable * pChannel -> chl_rate_coefficient;
                Variable = Variable / pChannel -> chl_calculate_coefficient;
            }
            else if(( pChannel -> chl_type & DC_COMPONENT ) != 0 )
            {
                /* ֱ������ */
                Variable = LS_CALCULATE_DC( ChlNumber, LS_DC_COEFFICIENT, SampleOffset );
                Variable = Variable * pChannel -> chl_rate_coefficient;
                Variable = Variable / pChannel -> chl_calculate_coefficient;
            }
            else
            {
                Variable = 0.0;
            }
            ChannelCoefficient = pChannel -> chl_channel_coefficient;
        }
        else
        {
            Variable = 0.0;
            ChannelCoefficient = 0.0;
        }

        /* ���ͨ������ */
        * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + k + 6 ) & LS_DATA_MASKS8 );  /* ͨ��ID */
        /* ͨ��ϵ�� */
        uTemp = LS_FLOAT_TO_IEEE( ChannelCoefficient );
        p = ( UNSIGNED_CHAR *)& uTemp;
        LS_WORD_TO_BYTE( pDst, p, LS_DATA_WIDTH );
        pDst += LS_DATA_WIDTH;
        /* ͨ����ֵ */
        uTemp = LS_FLOAT_TO_IEEE( Variable );
        p = ( UNSIGNED_CHAR *)& uTemp;
        LS_WORD_TO_BYTE( pDst, p, LS_DATA_WIDTH );
        pDst += LS_DATA_WIDTH;
    }

    pDst = ( UNSIGNED_CHAR *)Dst;
    Length = 2 * LS_DATA_WIDTH * Counter + Counter;             /* ����ĳ��� */
    * pDst ++ = ( UNSIGNED_CHAR )( Length + 5 );                /* ���ĳ��� */
    * pDst ++ = IEC103_TI_M_E2;                                 /* �������� */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );             /* CPU���� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 ); /* ���кŵ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 ); /* ���кŸ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )Counter;                       /* ͨ������ */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_E3
/* �޸�ͨ��ϵ�� */
INT LSC_Change_Channel_Cofficient( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             k;
    INT             iResult;
    INT             Counter;
    INT             ChlNumber;
    INT             iNumber;
    INT             iStartNum;
    INT             iType;
    INT             iStatus;
    TB_CFB          CondField;
    TB_TCB          * pTable;
    UNSIGNED_CHAR   * pSrc,* pDst, * p;
    float           fCVRate;
    float           fCoff[LS_MAX_CHL_NUMBER];
    float           Variable_r, Variable_i, Variable;
    UNSIGNED        uIeeChlValue;
    PRO_CHANNEL     * pChannel;
    INT             SampleOffset;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    iResult = LS_RESULT_NULL;
    pSrc = ( UNSIGNED_CHAR *)Src;

    ChlNumber = *( pSrc + 5 ) & LS_DATA_MASKS8; /* ͨ��ID */
    /* ��ȡ����ƫ��λ�� */
    SampleOffset = CSMV_GET_SAMPLE_OFFSET;
    /* �������ݱ�����ҵ����ݱ�ĸ��� */
    pTable = CTBC_FindTable( TAB_CHANNEL_ID );
    if( ChlNumber == 0xFF ) /* �Զ�У�� */
    {
        /* �Զ�У��ͨ��ϵ�� */
        pChannel = LS_CHANNEL_TABLE_MEMORY; /* ͨ�����ݱ��ڴ��׵�ַ */
        Counter = LS_CHANNEL_TABLE_NUMBER;  /* ͨ�����ݱ����ݸ��� */
        for( k = 0; k < Counter; k ++)
        {
            if((( pChannel -> chl_type & FUNDAMENTAL ) != 0 ) && (( pChannel->chl_type & CHL_COEFFICIENT ) != 0 )
                && (( pChannel -> chl_type & CHL_CURRENT ) || ( pChannel -> chl_type & CHL_VOLTAGE )))
            {
                /* ��Ч�������ѹͨ�� */
                Variable_r = LS_CALCULATE_FOURIER( ( pChannel->chl_Num ), FOURIER01_R, SampleOffset );
                Variable_i = LS_CALCULATE_FOURIER( ( pChannel->chl_Num ), FOURIER01_I, SampleOffset );
                Variable = Variable_r * Variable_r + Variable_i * Variable_i;
                Variable = ( float )sqrt( Variable );
                Variable = Variable * pChannel -> chl_rate_coefficient;
                Variable = Variable / pChannel -> chl_calculate_coefficient;

                if( pChannel -> chl_type & CHL_CURRENT )
                {
                    /* ����ͨ������1.2���ֵУ�� */
                    fCVRate = ( float ) (( pChannel->chl_secondary_rate )* LS_CURRENT_COEFFICIENT
                        *( pChannel->chl_channel_coefficient ));
                }
                else if( pChannel -> chl_type & CHL_VOLTAGE )
                {
                    /* ��ѹͨ������40VУ�� */
                    fCVRate = ( float )( LS_VOLTAGE_NORMVALUE *( pChannel->chl_channel_coefficient ));
                }
                if(( Variable >= ( fCVRate * LS_MIN_COEFFICIENT )) && ( Variable <= ( fCVRate * LS_MAX_COEFFICIENT )))
                {
                    fCoff[k] = fCVRate / Variable;
                }
                else
                {
                    fCoff[k] = pChannel -> chl_channel_coefficient;
                }
            }
            else
            {
                fCoff[k] = pChannel -> chl_channel_coefficient;
            }
            pChannel ++;
        }
        /* ��ʼ��� */
        iStartNum = 0;
        /* �޸ĸ��� */
        iNumber = pTable->DataNumber;
        /* �޸����� */
        iType = SETTING_ALL_ENABLE;
    }
    else
    {
        /* ��ͨ��У��ͨ��ϵ�� */
        if( ChlNumber < LS_CHANNEL_TABLE_NUMBER )
        {
            pChannel = LS_CHANNEL_TABLE_MEMORY; /* ͨ�����ݱ��ڴ��׵�ַ */
            pChannel += ChlNumber;

            uIeeChlValue = 0;
            for( k = 0; k < LS_DATA_WIDTH; k ++)
            {
                uIeeChlValue += (*( pSrc + 6 + k ) & LS_DATA_MASKS8 ) << ( k * 8 ); /* У��ϵ�� */
            }

            /* ������ת��ΪTMS��ʽ�ĸ����� */
            fCoff[0] = LS_IEEE_TO_FLOAT( uIeeChlValue );
        }
        /* ��ʼ��� */
        iStartNum = ChlNumber;
        /* �޸ĸ��� */
        iNumber = 1;
        /* �޸����� */
        iType = SETTING_ONE_ENABLE;
    }
    /* ���ò����ֶ� */
    CondField.HandleIndex = CHANNELN_CHLCOEFF_INDEX;
    /* ��������(�޸�ȫ��ͨ��) */
    CondField.CondNumber = 0;
    p = ( UNSIGNED_CHAR *)Dst;
    p --;
    * p = *( pSrc - 1 ) & LC_VAR_CONTROL_BITS;

    /* �޸���ֵ������ֵд��ee�� */
    iStatus = LSC_Change_TableInf( TAB_CHANNEL_ID, iStartNum, iNumber,
        & CondField, fCoff, iType, p );
    if( iStatus == NU_SUCCESS )
    {
        /* �����޸Ĺ��̳ɹ������ram����ͨ��ϵ�� */
        pChannel = LS_CHANNEL_TABLE_MEMORY; /* ͨ�����ݱ��ڴ��׵�ַ */
        /* �Զ�У��ϵ��ģʽ */
        if( ChlNumber == 0xFF )
        {
            for( k = 0; k < Counter; k ++)
            {
                ( pChannel + k ) -> chl_channel_coefficient = fCoff[k];
            }
        }
        else   /* �޸ĵ���ͨ��ϵ�� */
        {
            ( pChannel + ChlNumber )-> chl_channel_coefficient = fCoff[0];;
        }

        iResult = LS_RESULT_TRUE;
    }

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 6;                                                   /* ����ĳ��� */
    * pDst ++ = IEC103_TI_M_E3;                                      /* �������� */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                  /* CPU���� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 );  /* ���кŵ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 );  /* ���кŸ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 );  /* ͨ��ID */
    * pDst ++ = ( UNSIGNED_CHAR )iResult;                            /* ִ�н�� */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_E4
/* ��ͨ���ֵ */
INT LSC_Load_Channel_Rate( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             k;
    INT             Counter;
    INT             ChlNumber;
    UNSIGNED        uTemp;
    INT             Length;
    PRO_CHANNEL     * pChannel;
    UNSIGNED_CHAR   * p,* pSrc,* pDst;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    Counter = *( pSrc + 5 ) & LS_DATA_MASKS8;                   /* ͨ������ */
    pDst += 6;                                                  /* ��¼ͨ�����ݵ���ʼλ��(ͨ��id) */
    for( k = 0; k < Counter; k ++)
    {
        ChlNumber = *( pSrc + k + 6 ) & LS_DATA_MASKS8;         /* ͨ��ID */
        pChannel = LS_CHANNEL_TABLE_MEMORY;                     /* ͨ�����ݱ��ڴ��׵�ַ */
        pChannel += ChlNumber;

        * pDst ++ = ( UNSIGNED_CHAR )ChlNumber;
        /* ͨ��һ�ζֵ */
        uTemp = LS_FLOAT_TO_IEEE( pChannel -> chl_primary_rate );
        p = ( UNSIGNED_CHAR *)& uTemp;
        LS_WORD_TO_BYTE( pDst, p, LS_DATA_WIDTH );
        pDst += LS_DATA_WIDTH;
        /* ͨ�����ζֵ */
        uTemp = LS_FLOAT_TO_IEEE( pChannel -> chl_secondary_rate );
        p = ( UNSIGNED_CHAR *)& uTemp;
        LS_WORD_TO_BYTE( pDst, p, LS_DATA_WIDTH );
        pDst += LS_DATA_WIDTH;
    }

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;
    Length = 2 * LS_DATA_WIDTH * Counter + Counter;             /* ����ĳ��� */
    * pDst ++ = ( UNSIGNED_CHAR )( Length + 5 );                /* ���ĳ��� */
    * pDst ++ = IEC103_TI_M_E4;                                 /* �������� */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );             /* CPU���� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 ); /* ���кŵ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 ); /* ���кŸ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )Counter;                       /* ͨ������ */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_E5
/* �޸�ͨ���ֵ */
INT LSC_Change_Channel_Rate( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             i;
    INT             k;
    INT             Result;
    INT             iStatus;
    UNSIGNED        uRate[2];  /* ͨ���ֵ */
    UNSIGNED_CHAR   * pSrc,* pDst, * p;
    INT             ChlGpNum;
    INT             ChlNumber;
    PRO_CHANNEL     * pChannel;
    float           fRate1[LS_MAX_CHL_NUMBER];  /* һ�ζֵ������ */
    float           fRate2[LS_MAX_CHL_NUMBER];  /* ���ζֵ������ */
    float           fRate3[2];                  /* ����ͨ���ֵ������ */
    VOID            * pTempRate;                /* �ֵָ�� */
    TB_CFB          CondField;
    TB_TCB          * pTable;
    INT             iNumber;
    INT             iStartNum;
    INT             iType;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    Result = LS_RESULT_NULL;
    iStatus = LS_RESULT_NULL;
    pSrc = ( UNSIGNED_CHAR *)Src;

    ChlGpNum = *( pSrc + 5 ) & LS_DATA_MASKS8;           /* ͨ�������ֽ� */
    ChlGpNum += ((*( pSrc + 6 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* ͨ�������ֽ� */
    ChlNumber = *( pSrc + 7 ) & LS_DATA_MASKS8;  /* ͨ��ID */

    uRate[0] = 0;
    for( k = 0; k < LS_DATA_WIDTH; k ++)
    {
        uRate[0] += (*( pSrc + 8 + k ) & LS_DATA_MASKS8 ) << ( k * 8 );  /* һ�ζֵ */
    }
    fRate3[0] = LS_IEEE_TO_FLOAT( uRate[0] ); /* IEEEE��ʽת����װ�ø����ʽ */

    uRate[1] = 0;
    for( k = 0; k < LS_DATA_WIDTH; k ++)
    {
        uRate[1] += (*( pSrc + 12 + k ) & LS_DATA_MASKS8 ) << ( k * 8 ); /* ���ζֵ */
    }
    fRate3[1] = LS_IEEE_TO_FLOAT( uRate[1] ); /* IEEEE��ʽת����װ�ø����ʽ */

    /* �������ݱ�����ҵ����ݱ�ĸ��� */
    pTable = CTBC_FindTable( TAB_CHANNEL_ID );
    if( ChlGpNum == 0 )
    {
        /* ��ʼ��� */
        iStartNum = ChlNumber;
        /* �޸ĸ��� */
        iNumber = 1;
        /* �޸����� */
        iType = SETTING_ONE_ENABLE;
        /* һ�ζֵ */
        fRate1[0] = fRate3[0];
        /* ���ζֵ */
        fRate2[0] = fRate3[1];
    }
    else
    {
        pChannel = LS_CHANNEL_TABLE_MEMORY;  /* ͨ�����ݱ��ڴ��׵�ַ */
        for( k = 0; k < LS_CHANNEL_TABLE_NUMBER; k ++)
        {
            if(( pChannel -> chl_correlation_channel == ChlGpNum )
                &&( pChannel -> chl_type & CHL_RATE ))
            {
                /* һ�ζֵ */
                fRate1[k] = fRate3[0];
                /* ���ζֵ */
                fRate2[k] = fRate3[1];
            }
            else
            {
                /* һ�ζֵ */
                fRate1[k] = pChannel -> chl_primary_rate;
                /* ���ζֵ */
                fRate2[k] = pChannel -> chl_secondary_rate;
            }
            pChannel ++;
        }

        /* ��ʼ��� */
        iStartNum = 0;
        /* �޸ĸ��� */
        iNumber = pTable->DataNumber;
        /* �޸����� */
        iType = SETTING_ALL_ENABLE;
    }

    /* �޸�һ�����ζֵ */
    for( i = 0; i < 2; i ++)
    {
        /* һ�ζֵ */
        if( i == 0 )
        {
            /* ���ò����ֶ�(һ�ζֵ) */
            CondField.HandleIndex = CHANNELN_PRIMRATE_INDEX;
            pTempRate = fRate1;
        }
        else
        {
            /* ���ò����ֶ�(���ζֵ) */
            CondField.HandleIndex = CHANNELN_SECRATE_INDEX;
            pTempRate = fRate2;
        }

        /* ��������(����ʼ����Ѿ�Ϊͨ��id) */
        CondField.CondNumber = 0;
        p = ( UNSIGNED_CHAR *)Dst;
        p --;
        * p = *( pSrc - 1 ) & LC_VAR_CONTROL_BITS;

        /* �޸���ֵ������ֵд��ee�� */
        iStatus = LSC_Change_TableInf( TAB_CHANNEL_ID, iStartNum, iNumber,
            & CondField, pTempRate, iType, p );
        if( iStatus == NU_SUCCESS )
        {
            /* �����޸Ĺ��̳ɹ������ram����ͨ���ֵ */
            if( i == 1 )
            {
                pChannel = LS_CHANNEL_TABLE_MEMORY;  /* ͨ�����ݱ��ڴ��׵�ַ */
                if( ChlGpNum == 0 )
                {
                    pChannel += ChlNumber;
                    /* һ�ζֵ */
                    pChannel-> chl_primary_rate = fRate1[0];
                    /* ���ζֵ */
                    pChannel-> chl_secondary_rate = fRate2[0];

                    i = FUNDAMENTAL + HARMONIC_2ND + HARMONIC_3RD + HARMONIC_5TH
                        + HFUNDAMENTAL + HARMONIC_7TH + CHL_PEAKVALUE;
                    if(( pChannel -> chl_type & i ) != 0 )
                    {
                        pChannel->chl_rate_coefficient = pChannel->chl_secondary_rate / pChannel->chl_calculate_rate;
                    }
                }
                else
                {
                    for( k = 0; k < LS_CHANNEL_TABLE_NUMBER; k ++)
                    {
                        /* һ�ζֵ */
                        pChannel -> chl_primary_rate = fRate1[k];
                        /* ���ζֵ */
                        pChannel -> chl_secondary_rate = fRate2[k];

                        i = FUNDAMENTAL + HARMONIC_2ND + HARMONIC_3RD + HARMONIC_5TH
                            + HFUNDAMENTAL + HARMONIC_7TH + CHL_PEAKVALUE;
                        if(( pChannel -> chl_type & i ) != 0 )
                        {
                            pChannel->chl_rate_coefficient =
                                pChannel->chl_secondary_rate / pChannel->chl_calculate_rate;
                        }

                        pChannel ++;
                    }
                }
                Result = LS_RESULT_TRUE;
            }
        }
        else
        {
            break;
        }
    }

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;
    * pDst ++ = 8;                                                   // fill report length.
    * pDst ++ = IEC103_TI_M_E5;                                      // fill command type.
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                  // fill cpu number.
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 );  // fill serial number: byte 1
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 );  // fill serial number: byte 2
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 );  // fill channel group number.
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 );  // fill channel group number.
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 7 ) & LS_DATA_MASKS8 );  // fill protect channel ID.
    * pDst ++ = ( UNSIGNED_CHAR )Result;                             // fill result.

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_E6
/* ��װ����Ư�̶ȼ���Ưֵ */
INT LSC_Load_Channel_Excursion( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             k;
    UNSIGNED        uTemp;
    UNSIGNED_CHAR   * p,* pSrc,* pDst;
    INT             ChlNumber;
    PRO_CHANNEL     * pChannel;
    float           Variable;
    INT             Counter;
    INT             Length;
    INT             SampleOffset;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    Counter = *( pSrc + 5 ) & LS_DATA_MASKS8;  /* ͨ������ */
    pDst += 6; /* ͨ��ID */
    /* ��ȡ����ƫ��λ�� */
    SampleOffset = CSMV_GET_SAMPLE_OFFSET;
    for( k = 0; k < Counter; k ++)
    {
        ChlNumber = *( pSrc + 6 + k ) & LS_DATA_MASKS8; /* ͨ��ID */
        pChannel = LS_CHANNEL_TABLE_MEMORY;  /* ͨ�����ݱ��ڴ��׵�ַ */
        pChannel += ChlNumber;
        if( ChlNumber < LS_CHANNEL_TABLE_NUMBER )
        {
            Variable = LS_CALCULATE_DC( ChlNumber, LS_DC_COEFFICIENT, SampleOffset );
            if(( pChannel -> chl_type & CHL_POLARITY ) != 0 )
            {
                Variable = ( float )( 0.0 - Variable );
            }
        }
        else
        {
            Variable = 0.0;
        }
        * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 + k ) & LS_DATA_MASKS8 ); /* ͨ��ID */
        /* ��Ư�̶� */
        uTemp = LS_FLOAT_TO_IEEE( pChannel -> chl_channel_excursion );
        p = ( UNSIGNED_CHAR *)& uTemp;
        LS_WORD_TO_BYTE( pDst, p, LS_DATA_WIDTH );
        pDst += LS_DATA_WIDTH;
        /* ��Ư��ֵ */
        uTemp = LS_FLOAT_TO_IEEE( Variable );
        p = ( UNSIGNED_CHAR *)& uTemp;
        LS_WORD_TO_BYTE( pDst, p, LS_DATA_WIDTH );
        pDst += LS_DATA_WIDTH;
    }

    pDst = ( UNSIGNED_CHAR *)Dst;
    Length = 2 * LS_DATA_WIDTH * Counter + Counter;             /* ����ĳ��� */
    * pDst ++ = ( UNSIGNED_CHAR )( Length + 5 );                /* ���ĳ��� */
    * pDst ++ = IEC103_TI_M_E6;                                 /* �������� */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );             /* CPU���� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 ); /* ���кŵ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 ); /* ���кŸ��ֽ� */
    * pDst ++ = ( UNSIGNED_CHAR )Counter;                       /* ͨ������ */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_E7
/* �޸�װ����Ư */
INT LSC_Change_Channel_Excursion( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             k;
    UNSIGNED_CHAR   * pSrc,* pDst, * p;
    PRO_CHANNEL     * pChannel;
    float           variable;
    INT             iStatus, iResult;
    float           excursion[LS_MAX_CHL_NUMBER];
    TB_CFB          CondField;
    TB_TCB          * pTable;
    INT             iNumber;
    INT             iStartNum;
    INT             iType;
    INT             Chl0;
    UNSIGNED        uExcursion;
    INT             SampleOffset;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    iResult = LS_RESULT_NULL;
    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    /* ͨ�������� */
    pChannel = LS_CHANNEL_TABLE_MEMORY;  /* ͨ�����ݱ��ڴ��׵�ַ */
    /* �������ݱ�����ҵ����ݱ�ĸ��� */
    pTable = CTBC_FindTable( TAB_CHANNEL_ID );
    /* �ж���ͨ����Ư�Ƿ������������ķ�Χ�ڵı�־ */
    iStatus = NU_TRUE;
    Chl0 = *( pSrc + 5 ) & LS_DATA_MASKS8; /* ͨ��ID */
    /* ��ȡ����ƫ��λ�� */
    SampleOffset = CSMV_GET_SAMPLE_OFFSET;
    /* �޸�ͨ����Ư */
    if( Chl0 != 0xFF )
    {
        /* �޸ĵ���ͨ����Ư */
        if( Chl0 < LS_CHANNEL_TABLE_NUMBER )
        {
            uExcursion = 0;
            for( k = 0; k < LS_DATA_WIDTH; k ++)
            {
                uExcursion += (*( pSrc + 6 + k ) & LS_DATA_MASKS8 ) << ( k * 8 ); // channel excursion value.
            }
            excursion[0] = LS_IEEE_TO_FLOAT( uExcursion );
            /* �ж���Ư��Χ */
            if(( excursion[0] < LS_MIN_EXCURSION ) || ( excursion[0] > LS_MAX_EXCURSION ))
            {
                /* ��Ư���ڿ��������ķ�Χ��ֱ�ӷ���ʧ�� */
                iStatus = NU_FALSE;
            }
        }
        /* ��ʼ��� */
        iStartNum = Chl0;
        /* �޸ĸ��� */
        iNumber = 1;
        /* �޸����� */
        iType = SETTING_ONE_ENABLE;
    }
    else
    {
        /* �Զ�У��ͨ����Ư */
        /* ��ȡͨ�������� */
        pChannel = LS_CHANNEL_TABLE_MEMORY;  /* ͨ�����ݱ��ڴ��׵�ַ */
        if( LS_CHANNEL_TABLE_NUMBER > 0 )
        {
            iStatus = NU_TRUE;
            for( k = 0; k < LS_CHANNEL_TABLE_NUMBER; k ++)
            {
                if(( pChannel -> chl_type & CHL_EXCURSION ) != 0 )
                {
                    variable = LS_CALCULATE_DC( pChannel->chl_Num, LS_DC_COEFFICIENT, SampleOffset );
                    if(( pChannel -> chl_type & CHL_POLARITY ) != 0 )
                    {
                        variable = ( float )( 0.0 - variable );
                    }
                    variable += pChannel->chl_channel_excursion;
                    if(( variable >= LS_MIN_EXCURSION ) && ( variable <= LS_MAX_EXCURSION ))
                    {
                        excursion[k] = variable;
                    }
                    else
                    {
                        /* ��Ư���ڿ��������ķ�Χ��ֱ�ӷ���ʧ�� */
                        iStatus = NU_FALSE;
                        break;
                    }
                }
                else
                {
                    excursion[k] = 0.0;
                }
                pChannel ++;
            }
        }
        else
        {
            iStatus = NU_FALSE;
        }
        /* ��ʼ��� */
        iStartNum = 0;
        /* �޸ĸ��� */
        iNumber = pTable->DataNumber;
        /* �޸����� */
        iType = SETTING_ALL_ENABLE;
    }

    if( iStatus == NU_TRUE )
    {
        /* ���ò����ֶ� */
        CondField.HandleIndex = CHANNELN_CHLEXCU_INDEX;
        /* ��������(�޸�ȫ��ͨ��) */
        CondField.CondNumber = 0;
        p = ( UNSIGNED_CHAR *)Dst;
        p --;
        * p = *( pSrc - 1 ) & LC_VAR_CONTROL_BITS;

        /* �޸���ֵ������ֵд��ee�� */
        iStatus = LSC_Change_TableInf( TAB_CHANNEL_ID, iStartNum, iNumber,
            & CondField, excursion, iType, p );
        if( iStatus == NU_SUCCESS )
        {
            /* �����޸Ĺ��̳ɹ������ram����ͨ����Ư */
            pChannel = LS_CHANNEL_TABLE_MEMORY;  /* ͨ�����ݱ��ڴ��׵�ַ */
            /* �Զ�У����Ư */
            if( Chl0 == 0xFF )
            {
                /* �Զ�У��ͨ����Ư */
                for( k = 0; k < LS_CHANNEL_TABLE_NUMBER; k ++)
                {
                    ( pChannel + k )->chl_channel_excursion = excursion[k];
                }
            }
            else
            {
                /* �޸ĵ���ͨ����Ư */
                ( pChannel + Chl0 )->chl_channel_excursion = excursion[0];
            }
            iResult = LS_RESULT_TRUE;
        }
    }
    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 6;                                                   // fill report length.
    * pDst ++ = IEC103_TI_M_E7;                                      // fill command type.
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                  // fill cpu number.
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 );  // fill serial number: byte 1
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 );  // fill serial number: byte 2
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 );  // fill protect channle ID.
    * pDst ++ = ( UNSIGNED_CHAR )iResult;                            // fill result.

    return( COM_DATA_FULL );
}
#endif
/*=======================================================================================*/
#if SWITCH_IEC103_TI_M_EA
/* ��ͨ����� */
INT LSC_Load_Channel_Phase( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED        uTemp;
    UNSIGNED_CHAR   * p,* pSrc,* pDst;
    INT             Chl0;
    PRO_CHANNEL     * pChannel;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = ( UNSIGNED_CHAR )( LS_DATA_WIDTH + 5 );         // fill report length.
    * pDst ++ = IEC103_TI_M_EA;                                 // fill command type.
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );             // fill cpu number.
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 ); // fill serial number: byte 1
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 ); // fill serial number: byte 2
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 );  // fill protect channle ID.

    Chl0 = *( pSrc + 5 ) & LS_DATA_MASKS8;  /* channel ID. */
    pChannel = LS_CHANNEL_TABLE_MEMORY;     /* ͨ�����ݱ��ڴ��׵�ַ */
    pChannel += Chl0;
    uTemp = LS_FLOAT_TO_IEEE( pChannel->chl_channel_phase );
    p = ( UNSIGNED_CHAR *)& uTemp;
    LS_WORD_TO_BYTE( pDst, p, LS_DATA_WIDTH );
    pDst += LS_DATA_WIDTH;                                  // fill channel angle.

    return( COM_DATA_FULL );
}
#endif
/*=======================================================================================*/
#if SWITCH_IEC103_TI_M_E9
/* �޸�ͨ����λ��� */
INT LSC_Change_Channel_Phase( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             k, iResult;
    UNSIGNED        uPhase;
    UNSIGNED_CHAR   * pSrc,* pDst, * p;
    INT             Chl0;
    PRO_CHANNEL     * pChannel;
    float           Variable;
    TB_CFB          CondField;
    INT             iStatus;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    iResult = LS_RESULT_NULL;
    pSrc = ( UNSIGNED_CHAR *)Src;
    Chl0 = *( pSrc + 5 ) & LS_DATA_MASKS8;  // channel ID.
    if( Chl0 < LS_CHANNEL_TABLE_NUMBER )
    {
        pChannel = LS_CHANNEL_TABLE_MEMORY;  /* ͨ�����ݱ��ڴ��׵�ַ */
        pChannel += Chl0;

        uPhase = 0;
        for( k = 0; k < LS_DATA_WIDTH; k ++)
        {
            uPhase += (*( pSrc + 6 + k ) & LS_DATA_MASKS8 ) << ( k * 8 ); // channel phase value.
        }

        Variable = LS_IEEE_TO_FLOAT( uPhase );

        if(( Variable >= LS_MIN_PHASE ) && ( Variable <= LS_MAX_PHASE ))
        {
            /* ���ò����ֶ� */
            CondField.HandleIndex = CHANNELN_PHASEINDEX;
            /* ��������(�޸�ȫ��ͨ��) */
            CondField.CondNumber = 0;
            p = ( UNSIGNED_CHAR *)Dst;
            p --;
            * p = *( pSrc - 1 ) & LC_VAR_CONTROL_BITS;

            /* �޸���ֵ������ֵд��ee�� */
            iStatus = LSC_Change_TableInf( TAB_CHANNEL_ID, Chl0, 1,
                & CondField, & Variable, SETTING_ONE_ENABLE, p );

            /* �޸ĳɹ�,����ram���������� */
            if( iStatus == NU_SUCCESS )
            {
                pChannel -> chl_channel_phase = Variable;
                iResult = LS_RESULT_TRUE;
            }
        }
    }

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;
    * pDst ++ = 6;                                                   // fill report length.
    * pDst ++ = IEC103_TI_M_E9;                                      // fill command type.
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                  // fill cpu number.
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 );  // fill serial number: byte 1
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 );  // fill serial number: byte 2
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 );  // fill protect channle ID.
    * pDst ++ = ( UNSIGNED_CHAR )iResult;                            // fill result.

    return( COM_DATA_FULL );
}
#endif

/* �޸����ݱ���Ϣ����,��ѡ��\�༭\ȷ�Ϸ���һ�������� */
INT LSC_Change_TableInf( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * buffer, INT Type, UNSIGNED_CHAR * Dst )
{
    INT iStatus;
    //--------------------------------------------------------------------------
    /* ѡ��༭��ֵ�� */
    iStatus = XJP_DS_Select_Edit_Field_SG( TableIndex, Handle->HandleIndex );
    if( iStatus == NU_SUCCESS )
    {
        /* ���ñ༭��ֵ��ֵ */
        iStatus = XJP_DS_Set_Field_Value_SG( TableIndex, Offset, Number, Handle, buffer );
        if( iStatus == NU_SUCCESS )
        {
            /* ȷ�ϱ༭��ֵ��ֵ(�޸�ȫ��ͨ����Ư) */
            iStatus = XJP_DS_Confirm_Field_Value_SG( TableIndex, Type, Offset, Handle, LS_TIME_OUT,
                NU_NULL, Dst );
        }
    }
    return( iStatus );
}

#if SWITCH_IEC103_TI_M_F0
/* �źŸ��� */
INT LSC_Reset( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT      i;
    UNSIGNED * pMemory;
    //--------------------------------------------------------------------------
    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( Dst );
    LS_UNUSED_PARAM( Src );
    LS_UNUSED_PARAM( code );
    /*==========================================================*/
    /* ����̵�������λ�ڴ��ַ */
    pMemory = LS_GET_OUTPUT_RESET_MEMORY;

    LS_REPORT_PROTECT();
    /* �����̵����ջ� */
    XJP_IO_Set_Start_Relay( PlatHIOM, 0 );
    /* ������̵ּ���״̬λ */
    XJP_IO_Clear_Hold_Relay();
    /* �ջس���λ/���ø���̵�������λ */
    for( i = 0; i < CPC_MAX_OUTPUT_GROUPS; i ++)
    {
        XJP_IO_Set_Output_Relay( PlatHIOM, i,*( pMemory + i ), CIO_EQU );
    }
    LS_REPORT_UNPROTECT();

    /* ����Ӧ�ó����־ */
    XJP_Reset_App_Routine();

    LS_SET_RESET(); //�޸�Ϊֻ���ñ�־��
    /* ʹ�ܸ���̵�������ʱ��ʱ�� */
    NU_Control_Timer(& Timer_ResetRelay_CB, NU_ENABLE_TIMER );

    return( COM_DATA_EMPTY );
}
#endif

//
// reset relay timer main function
//
VOID LSC_RRelayTimerRoutine( UNSIGNED argc )
{
    INT       j;
    UNSIGNED  uStatus;
    UNSIGNED * pMemory;
    //--------------------------------------------------------------------------
    /* ����̵�������λ�ڴ��ַ */
    pMemory = LS_GET_OUTPUT_RESET_MEMORY;

    LS_UNUSED_PARAM( argc );

    /* �����ջ�ʱҪ�������ݱ��� */
    LS_REPORT_PROTECT();

    /* �ջظ���̵����ĳ���λ */
    for( j = 0; j < CPC_MAX_OUTPUT_GROUPS; j ++)
    {
        uStatus = *( pMemory + j );
        XJP_IO_Set_Output_Relay( PlatHIOM, j,(~ uStatus ), CIO_AND );
    }
    LS_REPORT_UNPROTECT();
    /* �ջظ���̵�������ʱ��ʱ�� */
    NU_Control_Timer(& Timer_ResetRelay_CB, NU_DISABLE_TIMER );
}

#if SWITCH_IEC103_TI_M_F1
/* �ٻ�������״̬��GPS������� */
INT LSC_Load_Input_Status( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             k;
    INT             Counter;
    INT             ActualCounter;
    INT             GroupNumber;
    UNSIGNED        uBits, uQuality;
    UNSIGNED_CHAR   * p,* pSrc,* pDst;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    Counter = ( INT )(*( pSrc + 5 ) & LS_DATA_MASKS8 ); /* ������������ */
    if( Counter > LS_INPUT_GROUP_NUMBER )
    {
        /* �ٻ��Ŀ�������������ƽ̨֧�ֵ������������ʱ,������������� */
        Counter = LS_INPUT_GROUP_NUMBER;
    }

    pDst += 6; /* ��д��������ŵ���ʼƫ�� */
    ActualCounter = 0;
    for( k = 0; k < Counter; k ++)
    {
        GroupNumber = ( INT )(*( pSrc + 6 + k ) & LS_DATA_MASKS8 ); /* ��������� */
        if( GroupNumber < LS_INPUT_GROUP_NUMBER )
        {
            /* ֻ������Ч�Ŀ��������� */
            ActualCounter ++;
            uBits = 0;
            uQuality = 0;
            * pDst ++ = GroupNumber;
            /* ��ȡ������״̬��Ʒ������ */
            CINC_ReadInputQuality( GroupNumber, & uBits, & uQuality );
            /* ������״̬ */
            p = ( UNSIGNED_CHAR *)& uBits;
            LS_WORD_TO_BYTE( pDst, p, LS_DATA_WIDTH );
            pDst += LS_DATA_WIDTH;
            /* ������Ʒ������ */
            p = ( UNSIGNED_CHAR *)& uQuality;
            LS_WORD_TO_BYTE( pDst, p, LS_DATA_WIDTH );
            pDst += LS_DATA_WIDTH;
        }
    }

    uBits = LS_GET_GPS_PULSE_NUMBER; /* ��ȡGPS������� */
    p = ( UNSIGNED_CHAR *)& uBits;
    LS_WORD_TO_BYTE( pDst, p, LS_DATA_WIDTH );
    pDst += LS_DATA_WIDTH;

    pDst = ( UNSIGNED_CHAR *)Dst;
    /* (1+4):���������+������״̬;uNumber:��������� */
    * pDst ++ = ( UNSIGNED_CHAR )(( 1 + 4 + 4 )* ActualCounter + 9 ); // fill report length.
    * pDst ++ = IEC103_TI_M_F1;                             // fill command type.
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );         // fill cpu number.
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 ); // fill serial number: byte 1
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 ); // fill serial number: byte 2
    * pDst ++ = ( UNSIGNED_CHAR ) ActualCounter;                    // fill input group number.

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_F7
/* ���ڴ����� */
INT LSC_Read_Memory( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst, * pTemp;
    UNSIGNED        uAddress;
    INT             k, width, number, iType;
    INT             iStatus;
    UNSIGNED           * pAddr32;
    unsigned short int * pAddr16, uShorData;
    UNSIGNED_CHAR      * pAddr8;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;
    uAddress = 0;
    iStatus = LS_RESULT_NULL;
    for( k = 0; k < LS_DATA_WIDTH; k ++)
    {
        uAddress += (*( pSrc + 4 + k ) & LS_DATA_MASKS8 ) << ( k * 8 );  /* ��ʼ��ַ */
    }
    width = (*( pSrc + 3 ) & LS_DATA_MASKS8 );   /* ���ݿ�� */
    number = (*( pSrc + 8 ) & LS_DATA_MASKS8 );  /* ���ݸ��� */
    iType = (*( pSrc + 9 ) & LS_DATA_MASKS8 );   /* ��/дѡ�� */
    if( iType == 0x02 )  /* д�ڴ� */
    {
        /* д�ڴ����� */
        /* �жϵ�ַ�ռ��Ƿ���Ч,��RAM������д���� */
        if(( uAddress >= LS_RAM_START_ADDR ) && ( uAddress < LS_RAM_END_ADDR ))
        {
            iStatus = LS_RESULT_TRUE;
            /* Դ���ݵ�ַ */
            pTemp = ( pSrc + 10 );
            /* �ж����ݿ�� */
            switch( width )
            {
                /* ��ȡ���ݿ��Ϊ8 */
            case 1:
                pAddr8 = ( UNSIGNED_CHAR *)uAddress;
                memcpy( pAddr8, pTemp, number );
                break;
                /* ��ȡ���ݿ��Ϊ16 */
            case 2:
                pAddr16 = ( unsigned short int *)uAddress;
                for( k = 0; k < number; k ++)
                {
                    uShorData = (* pTemp ++ & CPC_DATA_MASKS8 );
                    uShorData |= ((* pTemp ++ & CPC_DATA_MASKS8 ) << CPC_DATA_BITS8 );
                    * pAddr16 ++ = uShorData;
                }
                break;
                /* ��ȡ���ݿ��Ϊ32 */
            case 4:
                pAddr32 = ( UNSIGNED *)uAddress;
                for( k = 0; k < number; k ++)
                {
                    LS_BYTE_TO_WORD( pAddr32, pTemp, width );
                    pTemp += width;
                    pAddr32 ++;
                }
                break;
            default:
                /* ���ݿ�Ȳ�������ķ�Χ֮�ڷ���ʧ�� */
                iStatus = LS_RESULT_NULL;
                break;
            }
        }
        * pDst ++ = 10;                                             /* ��·�û����ݳ��� */
    }
    else
    {
        /* ���ڴ����� */
        * pDst ++ = ( UNSIGNED_CHAR )( number * width + 10 );        /* ��·�û����ݳ��� */
    }

    * pDst ++ = IEC103_TI_M_F7;                                      /* ����ʶ */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                  /* CPU���� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 );  /* ���ݿ�� */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 );  /* ��ʼ��ַ�ֽ�1 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 );  /* ��ʼ��ַ�ֽ�2 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 );  /* ��ʼ��ַ�ֽ�3 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 7 ) & LS_DATA_MASKS8 );  /* ��ʼ��ַ�ֽ�4 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 8 ) & LS_DATA_MASKS8 );  /* ���ݸ��� */
    if( iType == 0x01 )
    {
        iStatus = LS_RESULT_TRUE;
        /* �洢�ڴ����ݵ�ƫ���� */
        pDst += 2;

        switch( width )
        {
            /* ��ȡ���ݿ��Ϊ8 */
        case 1:
            pAddr8 = ( UNSIGNED_CHAR *)uAddress;
            memcpy( pDst, pAddr8, number );
            break;
            /* ��ȡ���ݿ��Ϊ16 */
        case 2:
            pAddr16 = ( unsigned short int *)uAddress;
            for( k = 0; k < number; k ++)
            {
                uShorData = * pAddr16 ++;
                * pDst ++ = ( uShorData & CPC_DATA_MASKS8 );
                * pDst ++ = ( ( uShorData >> CPC_DATA_BITS8 ) & CPC_DATA_MASKS8 );
            }
            break;
            /* ��ȡ���ݿ��Ϊ32 */
        case 4:
            pAddr32 = ( UNSIGNED *)uAddress;
            for( k = 0; k < number; k ++)
            {
                LS_WORD_TO_BYTE( pDst, pAddr32, width );
                pDst += width;
                pAddr32 ++;
            }
            break;
        default:
            /* ���ݿ�Ȳ�������ķ�Χ֮�ڷ���ʧ�� */
            iStatus = LS_RESULT_NULL;
            break;
        }
    }

    pDst = ( UNSIGNED_CHAR *)Dst;
    pDst += 9;                                      /* ִ�н��λ��ƫ�� */
    * pDst ++ = iStatus;                            /* ִ����ȷ */
    * pDst ++ = (*( pSrc + 9 ) & LS_DATA_MASKS8 );  /* ��/дѡ�� */

    return( COM_DATA_FULL );
}
#endif
