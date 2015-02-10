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
/* 修改数据表信息函数,将选择\编辑\确认放在一个函数中 */
INT LSC_Change_TableInf( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * buffer, INT Type, UNSIGNED_CHAR * Dst );

#if SWITCH_IEC103_TI_M_81
/* 装置基本信息 */
INT LSC_Load_Device_Inf( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_82
/* 装置呼叫命令 */
INT LSC_Device_Call( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if ( LC_LINK_HMI_PHY_COM )
#if SWITCH_IEC103_TI_M_90
/* 程序下载与配置通信命令 */
INT LSC_Program_Get_Ready( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_91
/* 进入下传程序状态 */
INT LSC_Program_Make_Ready( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#endif
#if SWITCH_IEC103_TI_M_96
/* 预发固化保护配置 */
INT LSC_Ready_Configure_Program( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_97
/* 执行固化保护配置 */
INT LSC_Execute_Configure_Program( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_98
/* 查询保护配置固化结果 */
INT LSC_Load_Configure_Program_State( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_A0
/* 召唤数据表的总个数 */
INT LSC_ACK_Datatab( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_A1
/* 召唤数据表的信息 */
INT LSC_ACK_Datatab_Info( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_A2
/* 召唤字段信息 */
INT LSC_ACK_Field_Info( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_A3
/* 召唤表中数据的总个数 */
INT LSC_ACK_Data_Number( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_A4
/* 召唤记录的全部字段 */
INT LSC_ACK_All_Field_Value( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_A5
/* 召唤记录的单个字段 */
INT LSC_ACK_Field_Value( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_A6
/* 召唤记录的多个字段 */
INT LSC_ACK_Fields_Value( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_A7
/* 召唤记录的多个字段 */
INT LSC_Set_Config_Values( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_A8
/* 选择编辑字段 */
INT LSC_Slecet_Edit_SG( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_A9
/* 设置编辑定值组的值 */
INT LSC_Set_Edit_SGV( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_AA
/* 确认编辑定值组的值 */
INT LSC_Conform_Edit_SGV( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_AB
/* 读取数据表的状态 */
INT LSC_Get_Tab_Status( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_AC
/* 读取数据表的配置CRC码 */
INT LSC_Get_Config_Tab_CRC( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_AD
/* 冻结数据表的数据 */
INT LSC_Freeze_Table( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_AE
/* 解冻数据表的数据 */
INT LSC_UnFreeze_Table( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_AF
/* 设置多个配置字段的值 */
INT LSC_Set_Batch_Config_Values( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_B0
/* 保存多个配置字段的值 */
INT LSC_Save_Batch_Config_Values( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_C0
/* 召唤事件类型 */
INT LSC_Load_Event_Type( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_C1
/* 召唤事件信息 */
INT LSC_Load_Event_Inf( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_C2
/* 召唤事件索引号 */
INT LSC_Load_Event_Serial( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_C3
/* 召唤事件数据 */
INT LSC_Load_Event_Data( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_C4
/* 召唤事件数据 */
INT LSC_Clear_Alarm_Event( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_C5
/* 召唤报告信息 */
INT LSC_Load_Report_Inf( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_C6
/* 召唤报告故障序号 */
INT LSC_Load_Report_FaultSerl( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_C7
/* 召唤报告索引序号 */
INT LSC_Load_Report_Serial( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_C8
/* 召唤报告数据 */
INT LSC_Load_Report_Data( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_C9
/* 清除故障记录数据 */
INT LSC_Clear_FaultData( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_CA
/* 手动启动录波 */
INT LSC_Start_Wave( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_CB
/* 召唤录波序号 */
INT LSC_Load_Wave_ID( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_CC
/* 召唤录波时间 */
INT LSC_Load_Wave_Time( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_CD
/* 召唤录波信息 */
INT LSC_Load_Wave_Inf( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_CE
/* 召唤录波电网故障序号 */
INT LSC_Load_Wave_FaultSerl( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_E0
/* 预发传动开出 */
INT LSC_Trip_Test_Ready( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_E1
/* 执行传动开出 */
INT LSC_Trip_Test( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_E2
/* 调通道系数及量值 */
INT LSC_Load_Channel_Cofficient ( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_E3
/* 修改通道系数 */
INT LSC_Change_Channel_Cofficient ( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_E4
/* 调通道额定值 */
INT LSC_Load_Channel_Rate ( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_E5
/* 修改通道额定值 */
INT LSC_Change_Channel_Rate ( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_E6
/* 调装置零漂刻度及零漂值 */
INT LSC_Load_Channel_Excursion ( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_E7
/* 修改装置零漂 */
INT LSC_Change_Channel_Excursion ( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_EA
/* 调通道相角 */
INT LSC_Load_Channel_Phase ( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_E9
/* 修改通道相位误差 */
INT LSC_Change_Channel_Phase ( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_F0
/* 信号复归 */
INT LSC_Reset( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_F1
/* 召唤开入量状态及GPS脉冲计数 */
INT LSC_Load_Input_Status( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif
#if SWITCH_IEC103_TI_M_F7
/* 读内存数据 */
INT LSC_Read_Memory( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
#endif

/* 召唤巡检自发上送数据 */
INT  LSC_Report_Event( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
VOID LSC_TripTimerRoutine( UNSIGNED argc );
VOID LSC_RRelayTimerRoutine( UNSIGNED argc );

#endif
