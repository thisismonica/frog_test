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
/*      ls_extr.h                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      LS - Lonworks Communication to Supervisor                        */
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
/*      ls_defs.h                  Lonworks Communication to Supervisor  */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhideYang.      02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

#include "plat/net_inc/ls_defs.h"
#include "plat/net_inc/lc_defs.h"
#include "plat/es_plat.h"

/* Check to see if the file has been included already.  */

#ifndef LS_EXTR_H
#define LS_EXTR_H

/*  Initialization functions.  */
VOID LSI_Initialize( VOID );
/* Error checking for supplemental functions.  */

/* Core processing functions.  */
VOID LSC_RegisterCommand_Local( GW_CHANNEL * pChl );
/* �޸����ݱ���Ϣ����,��ѡ��\�༭\ȷ�Ϸ���һ�������� */
INT LSC_Change_TableInf( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * buffer, INT Type, UNSIGNED_CHAR * Dst );

#if SWITCH_IEC103_TI_M_81
/* װ�û�����Ϣ */
INT LSC_Load_Device_Inf( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_82
/* װ�ú������� */
INT LSC_Device_Call( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if ( LC_LINK_HMI_PHY_COM )
#if SWITCH_IEC103_TI_M_90
/* ��������������ͨ������ */
INT LSC_Program_Get_Ready( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_91
/* �����´�����״̬ */
INT LSC_Program_Make_Ready( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#endif
#if SWITCH_IEC103_TI_M_96
/* Ԥ���̻��������� */
INT LSC_Ready_Configure_Program( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_97
/* ִ�й̻��������� */
INT LSC_Execute_Configure_Program( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_98
/* ��ѯ�������ù̻���� */
INT LSC_Load_Configure_Program_State( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_A0
/* �ٻ����ݱ���ܸ��� */
INT LSC_ACK_Datatab( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_A1
/* �ٻ����ݱ����Ϣ */
INT LSC_ACK_Datatab_Info( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_A2
/* �ٻ��ֶ���Ϣ */
INT LSC_ACK_Field_Info( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_A3
/* �ٻ��������ݵ��ܸ��� */
INT LSC_ACK_Data_Number( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_A4
/* �ٻ���¼��ȫ���ֶ� */
INT LSC_ACK_All_Field_Value( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_A5
/* �ٻ���¼�ĵ����ֶ� */
INT LSC_ACK_Field_Value( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_A6
/* �ٻ���¼�Ķ���ֶ� */
INT LSC_ACK_Fields_Value( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_A7
/* �ٻ���¼�Ķ���ֶ� */
INT LSC_Set_Config_Values( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_A8
/* ѡ��༭�ֶ� */
INT LSC_Slecet_Edit_SG( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_A9
/* ���ñ༭��ֵ���ֵ */
INT LSC_Set_Edit_SGV( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_AA
/* ȷ�ϱ༭��ֵ���ֵ */
INT LSC_Conform_Edit_SGV( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_AB
/* ��ȡ���ݱ��״̬ */
INT LSC_Get_Tab_Status( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_AC
/* ��ȡ���ݱ������CRC�� */
INT LSC_Get_Config_Tab_CRC( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_AD
/* �������ݱ������ */
INT LSC_Freeze_Table( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_AE
/* �ⶳ���ݱ������ */
INT LSC_UnFreeze_Table( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_AF
/* ���ö�������ֶε�ֵ */
INT LSC_Set_Batch_Config_Values( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_B0
/* �����������ֶε�ֵ */
INT LSC_Save_Batch_Config_Values( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_C0
/* �ٻ��¼����� */
INT LSC_Load_Event_Type( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_C1
/* �ٻ��¼���Ϣ */
INT LSC_Load_Event_Inf( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_C2
/* �ٻ��¼������� */
INT LSC_Load_Event_Serial( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_C3
/* �ٻ��¼����� */
INT LSC_Load_Event_Data( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_C4
/* �ٻ��¼����� */
INT LSC_Clear_Alarm_Event( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_C5
/* �ٻ�������Ϣ */
INT LSC_Load_Report_Inf( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_C6
/* �ٻ����������� */
INT LSC_Load_Report_FaultSerl( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_C7
/* �ٻ������������ */
INT LSC_Load_Report_Serial( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_C8
/* �ٻ��������� */
INT LSC_Load_Report_Data( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_C9
/* ������ϼ�¼���� */
INT LSC_Clear_FaultData( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_CA
/* �ֶ�����¼�� */
INT LSC_Start_Wave( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_CB
/* �ٻ�¼����� */
INT LSC_Load_Wave_ID( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_CC
/* �ٻ�¼��ʱ�� */
INT LSC_Load_Wave_Time( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_CD
/* �ٻ�¼����Ϣ */
INT LSC_Load_Wave_Inf( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_CE
/* �ٻ�¼������������� */
INT LSC_Load_Wave_FaultSerl( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_E0
/* Ԥ���������� */
INT LSC_Trip_Test_Ready( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_E1
/* ִ�д������� */
INT LSC_Trip_Test( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_E2
/* ��ͨ��ϵ������ֵ */
INT LSC_Load_Channel_Cofficient ( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_E3
/* �޸�ͨ��ϵ�� */
INT LSC_Change_Channel_Cofficient ( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_E4
/* ��ͨ���ֵ */
INT LSC_Load_Channel_Rate ( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_E5
/* �޸�ͨ���ֵ */
INT LSC_Change_Channel_Rate ( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_E6
/* ��װ����Ư�̶ȼ���Ưֵ */
INT LSC_Load_Channel_Excursion ( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_E7
/* �޸�װ����Ư */
INT LSC_Change_Channel_Excursion ( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_EA
/* ��ͨ����� */
INT LSC_Load_Channel_Phase ( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_E9
/* �޸�ͨ����λ��� */
INT LSC_Change_Channel_Phase ( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_F0
/* �źŸ��� */
INT LSC_Reset( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_F1
/* �ٻ�������״̬��GPS������� */
INT LSC_Load_Input_Status( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_F7
/* ���ڴ����� */
INT LSC_Read_Memory( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif

/* �ٻ�Ѳ���Է��������� */
INT  LSC_Report_Event( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
VOID LSC_TripTimerRoutine( UNSIGNED argc );
VOID LSC_RRelayTimerRoutine( UNSIGNED argc );

#endif
