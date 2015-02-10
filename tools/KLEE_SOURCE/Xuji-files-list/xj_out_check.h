
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
/*      xj_outc.h                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      OUT - Output Management                                          */
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
/*      ZhideYang.      07-19-2007      Created initial version 1.0      */
/*     YanJuanJiang.    07-24-2007      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"
#include "plat/es_plat.h"

/* Check to see if the file has been included already.  */

#ifndef XJ_OUT_CHECK_H
#define XJ_OUT_CHECK_H

#ifdef __cplusplus
extern "C" {
#endif

/* 内部事件类型定义 */
#ifndef XJ_INTERIOR_EVENTBITS
#define XJ_INTERIOR_EVENTBITS
#define LS_RES_EVENT_START              1                                 /* 传动启动继电器 */
#define LS_RES_EVENT_TRIP               2                                 /* 传动跳闸矩阵 */
#define LS_RES_EVENT_OUT                3                                 /* 传动出口 */
#define LS_RES_EVENT_REPAIR             4                                 /* 数据自检出错 */
#define LS_RES_EVENT_EEPROM             5                                 /* 写EEPROM出错 */
#define LS_RES_EVENT_FLASH              6                                 /* 写FLASH出错 */
#define LS_RES_EVENT_ERASE              7                                 /* FLASH扇区擦除出错 */
#define LS_RES_EVENT_SIZE               8                                 /* 数据大小出错 */
#define LS_RES_EVENT_RESTART            9                                 /* 装置重新上电 */
#define LS_RES_EVENT_RELAY              10                                /* 继电器动作 */
#define LS_RES_EVENT_CLOSE              11                                /* 继电器瞬时击穿 */
#define LS_RES_EVENT_CLOSE_EXT          12                                /* 扩展回路瞬时击穿 */
#define LS_RES_EVENT_REPORT_CLR         13                                /* 清除报告 */
#define LS_RES_EVENT_WAVE_CLR           14                                /* 清除录波 */
#define LS_RES_EVENT_ZONE_CHANGE        15                                /* 定值区变化 */
#define LS_RES_EVENT_CONFIG_CHANGE      16                                /* 配置变化 */
#define LS_RES_EVENT_GETIN_OVERTIME     17                                /* 接收开入量超时 */
#endif

/* error bits */
#ifndef XJ_ERROR_EVENTBITS
#define XJ_ERROR_EVENTBITS
#define Error_SRAM          (1 << 0)  /* EEPROM error */
#define Error_RAM           (1 << 1)  /* RAM error */
#define Error_Setting       (1 << 2)  /* setting error */
#define Error_Relay         (1 << 3)  /* relay error（断线或击穿） */
#define Error_5VPower       (1 << 4)  /* 5v power error */
#define Error_CPU_Number    (1 << 5)  /* CPU number error */
#define Error_AD            (1 << 6)  /* A/D error */
#define Error_Commu         (1 << 7)  /* communication error */
#define Error_FLASH         (1 << 8)  /* flash error */
#define Error_Program       (1 << 9)  /* program error */
#define Error_Relay_Close   (1 << 10) /* relay strike through error（击穿） */
#define Error_Extend_Port   (1 << 11) /* extended port error */
#define Close_Relay_Flag    (1 << 12) /* power error close relay flag */
#define Sample_Data_OK      (1 << 13) /* sample data ok */
#define User_Alarm_Bits     (1 << 14) /* user alarm bits */
#define GWS_InitStatus      (1 << 15)            /* gws初始状态 */
#endif

#define OUTC_CHECK_ALARM_EVENT_ID     0x5a  /* 开出断线事件类型(告警事件) */
#define OUTC_CHECK_RESERVE_EVENT_ID   0x5b  /* 内部事件类型 */
#define OUTC_CHECK_EVENT_NAME_LENGTH  31    /* 事件名称长度 */

#ifndef XJ_OUTC_MODULE
#define XJ_OUTC_MODULE

#endif

/*=======================================================================*/
/* 开出检测管理                                                            */
/*=======================================================================*/
/* 出口检测初始化函数 */
VOID OUTC_CheckInitialize( UNSIGNED uHardGroupNum, UNSIGNED uTotalGroupNum, UNSIGNED * pCheckBits,
    UNSIGNED uCheckNum , NU_EVENT_GROUP * pErrEventGroup, INT(* FillEvent )( const VOID * pName, INT iEventType ),
    STATUS (* Set_EventsBits )( NU_EVENT_GROUP * group, UNSIGNED uEvents, OPTION operation ), VOID * Protect ,
    VOID (* SendGooseFun )( VOID ), INT iHardExtendCount );
/* 出口检测主函数 */
VOID OUTC_CheckMain( VOID );
/* 开出检测执行函数 */
VOID OUTC_CheckDrive( VOID );
/* 检测启动继电器返回函数 */
VOID OUTC_CheckStartUpReturn( VOID );
/* 设置出口继电器动作位 */
VOID OUTC_Check_SetPickupBits( INT GroupNo, UNSIGNED uBits );
/* 启动继电器驱动函数 */
VOID OUTC_Check_StartDriver( UNSIGNED uFlag );
/* 开出驱动函数 */
INT COUTC_RelayDriver( INT GroupNumber, UNSIGNED RelayBits );
/* 出口执行函数 */
VOID OUTC_DriveFunction( VOID );
/* 设置开出量状态变化记录的屏蔽位函数 */
INT OUTC_CheckSetOutLogMaskBits ( INT iGroupNum, UNSIGNED MaskBits );
/* 设置闭锁出口标志(iFlag:1置闭锁出口标志,iFlag:0清闭锁出口标志) */
INT OUT_SetClockRelayFlag( INT iFlag );
/* 获取闭锁出口标志(iFlag:1闭锁出口,iFlag:0不闭锁出口) */
INT OUT_GetClockRelayFlag( INT * iFlag );
/* 获取出口位状态 */
UNSIGNED OUT_GetOutPickupBits( INT GroupNo );
/* 获取开出组数 */
INT OUTC_GetOutGroupCount( INT * iTotalCount, INT * iHardCount );
/* 软出口执行位变化检查函数 */
INT OUTC_SoftOutExBitsCheck( VOID );

#ifdef __cplusplus
}
#endif

#endif
