
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
/*      out_extr.h                                      Ver  1.0         */
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
#include "plat/inc/xj_out_check.h"
#include "plat/inc/esp_cfg.h"
#include "plat_specific/plat_specific.h"
#include "plat_specific/inc/cpc_defs.h"

/* Check to see if the file has been included already.  */

#ifndef OUT_CHECK_H
#define OUT_CHECK_H

extern ESP_CFG PLAT_cfg;

/* 判获取基本启动标识 */
#define OUT_GET_BASE_STARTUP_FLAG()   ( PLAT_cfg.device.FunSelect & FUN_BASE_START_UP )

#define OUT_JC_EVENT_DELAY_TIME     100     /* 击穿事件延时时间 */

#define OUT_MAX_CHECK_RELAY    32           /* 最大检测出口继电器路数 */
#define OUT_GROUP_RELAY_BITS   32           /* 每组出口位数 */
#define OUT_CHECK_BREAK_DELAY  61000        /* 出口断线检测延时时间 */
#define OUT_OUTERROR_DELAY     10000        /* 开出回路断线延时时间 */
#define OUT_OUTERROR_NUMBER    10           /* 开出回路断线次数 */
#define OUT_SUB_GROUP_MASK32   31           /* the mask of sub-group 32-bits */
#define OUT_GROUP_MASK32       5            /* the group mask bit for 32-bits */

#define OUT_STARTUP_RDELAY     500          /* 启动继电器返回延时时间 */
#define OUT_START_OPEN_CHECK   1000         /* 启动继电器返回后开放开出自检的延时时间 */
#define OUT_SEND_STARTB_RDELAY 3000         /* 向对侧置本侧启动继电器返回时的延时时间 */
#define OUT_PICKUP_CHECKDELAY  5            /* 出口变位检测时间 */
#define OUT_RESET_HOLD_TIME    500          /* 复归保持时间 */
/* 继电器组标志 */
#define OUT_BASE_RELAY_FLAG   0x1            /* 基本开出继电器组击穿标志 */
#define OUT_EXT_RELAY_FLAG    0x2            /* 扩展开出继电器组击穿标志 */

/* 开出继电器数据结构  */
typedef struct OUT_RELAY_STRUCT
{
    /* 动作位 */
    UNSIGNED PickupBits;
    /* 实际执行位 */
    UNSIGNED ExecuteBits;
    /* 闭锁保持位 */
    UNSIGNED HoldBits;
    /* 出口变化记录位 */
    UNSIGNED ChangeRegBits;
    /* 以下信息仅硬开出有 */
    /* 断线检测位 */
    UNSIGNED CheckBits;
    /* 检测位数 */
    INT CheckNum;
    /* 开出回路断线计数器 */
    UNSIGNED BreakErrorCount[OUT_GROUP_RELAY_BITS];
    /* 断线检测结果 */
    UNSIGNED ErrorBits;
    /* 断线事件记录位 */
    UNSIGNED ErrorEventBits;
}OUT_RELAY;

/* 软开出继数据结构  */
typedef struct SOUT_RELAY_STRUCT
{
    /* 动作位 */
    UNSIGNED PickupBits;
    /* 实际执行位 */
    UNSIGNED ExecuteBits;
    /* 闭锁保持位 */
    UNSIGNED HoldBits;
    /* 出口变化记录位 */
    UNSIGNED ChangeRegBits;
}SOUT_RELAY;

/* 开出控制块数据结构 */
typedef struct OUT_OCB_STRUCT OUT_OCB;
/* 开出控制块数据结构  */
struct OUT_OCB_STRUCT
{
    /* 最大开出组数 */
    INT MaxNumber;
    /* 硬件开出组数 */
    INT HardGroupNumber;
    /* 扩展插件个数 */
    INT iHardExtendCount;
    /* 启动组数 */
    INT iStartUpGroup;
    /* 总开出组数 */
    INT TotalGroupNum;
    /* 出口检测最大路数 */
    UNSIGNED CheckRelayMaxNum;
    /* 出口检测路数 */
    UNSIGNED CheckRelayNum;
    /* 锁出口检测标志 */
    INT LockCheck;
    /* 闭锁出口检测 */
    INT CloseCheck;
    /* 准备检测标志 */
    INT CheckReady;
    /* 执行检测标志 */
    INT CheckExecute;
    /* 检测通道号码 */
    INT CheckNumber;
    /* 当前检测组号 */
    INT CheckGroup;
    /* 在当前检测组中检测通道号 */
    INT GroupCheckNum;
    /* 击穿延时时间 */
    UNSIGNED ErrorDelay;
    /* 瞬时击穿延时时间 */
    UNSIGNED InstErrDelay;
    /* 开出击穿错误计数器 */
    UNSIGNED ErrCount[CPC_MAX_OUTPUT_GROUPS];
    /* 击穿错误标志 */
    INT ErrorFlag;
    /* 瞬时击穿错误标志 */
    INT InstErrorFlag;
    /* 扩展端口允许 */
    INT ExtEnable;
    /* 闭锁开出 */
    INT CloseRelay;
    /* 启动标志 */
    UNSIGNED StartUp;
    /* 闭锁保持启动标志 */
    UNSIGNED HoldStartup;
    /* 启动继电器返回延时计时标志 */
    UNSIGNED StartUpRDFlag;
    /* 启动继电器动作常量 */
    UNSIGNED StartupConst;
    /* 继电器开出位 */
    OUT_RELAY * pRelayBits;
    /* 填写错误事件的事件组 */
    NU_EVENT_GROUP * pErrEventGroup;
    /* 填写事件记录函数 */
    INT (* FillEvent )( const VOID * pName, INT iEventType );
    /* 设置事件位 */
    STATUS (* Set_EventsBits )( NU_EVENT_GROUP * group, UNSIGNED uEvents, OPTION operation );
    /* 数据保护 */
    VOID * Protect;

    /*public: */
    /* 基本信息初始化函数 */
    VOID (* Initialize )( UNSIGNED uHardGroupNum, UNSIGNED uTotalGroupNum, UNSIGNED * pCheckBits,
        UNSIGNED uCheckNum , NU_EVENT_GROUP * pErrEventGroup, INT(* FillEvent )( const VOID * pName, INT iEventType ),
        STATUS (* Set_EventsBits )( NU_EVENT_GROUP * group, UNSIGNED uEvents, OPTION operation ), VOID * Protect ,
        VOID (* SendGooseFun )( VOID ), INT iHardExtendCount );
    /* 继电器开出回路断线检测函数 */
    VOID (* OutBk_Check )( VOID );
    /* 开出回路断线事件记录 */
    VOID (* OutBk_EventFill )( VOID );
    /* 开出回路击穿事件记录 */
    VOID (* OutJC_EventFill )( VOID );
    /* 出口检测主函数 */
    VOID (* OutCheck_Main )( VOID );
    /* 出口变位内部事件记录 */
    VOID (* BitsChangeFillEvent )( VOID );
    /* 硬开出检测函数 */
    VOID (* HardOutCheckMain )( VOID );
    /* 注册扩展开出检测函数 */
    VOID (* EXOutCheckMain )( VOID );
    /* 注册发送GOOSE函数 */
    VOID (* SendGooseFun )( VOID );
};

#endif
