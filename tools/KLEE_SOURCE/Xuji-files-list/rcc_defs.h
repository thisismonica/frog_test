
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
/*      ls_defs.h                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      rc - report copy                                                 */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains data structure definitions and constants for  */
/*     report copy                                                       */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ChenYuFeng, XJ ELECTRIC CO.LTD.                                  */
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
/*      ChenYuFeng       05-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"
#include "plat/inc/pro_defs.h"
#include "plat/net_inc/commu.h"
#include "plat/net_inc/lc_defs.h"
#include "plat/es_plat.h"

#ifndef RCC_DEFS_H
#define RCC_DEFS_H

#define MAX_CHECK_INFO          50   /* 存储的最大突变报文数 */
#define MAX_FAULT_RETURN        50   /* 存储的最大故障返回报文数 */
#define GW_TARG_OP_TIMEOUT      100  /* 超时时间定义 */
/* 故障返回队列数据类型定义 */
#define RCC_Q_TYPE_FAULT_RETURN 1    /* 故障返回报告 */
#define RCC_Q_TYPE_COMMAND      2    /* 命令报文类型 */

typedef struct  RCC_CHECKINFO_STRUCT
{
    INT         TableID;         /* 数据表号(2字节) */
    INT         CondValue;       /* 条件量值(4字节) */
    INT         Type;            /* 数据类型 */
    UNSIGNED    Year;            /* 年 */
    UNSIGNED    Month;           /* 月 */
    UNSIGNED    Day;             /* 日 */
    UNSIGNED    Hour;            /* 时 */
    UNSIGNED    Minute;          /* 分 */
    UNSIGNED    Millionsecond;   /* 毫秒(2字节) */
    INT         Sector;          /* 扇区 */
    INT         FunInf;          /* 信息序号(2字节) */
    INT         Status;          /* 状态 */
    UNSIGNED    q;               /* 数据品质因素 */
    UNSIGNED    t_sec;           /* t.sec */
    UNSIGNED    t_frac;          /* t.frac */
    UNSIGNED    t_qflag;         /* t.qflag */
    UNSIGNED    offset;          /* 位置偏移 */
}RCC_CheckInfo;

/* 故障返回数据 */
typedef struct  RCC_FAULT_RETURN_STRUCT
{
    INT             TableID;        /* 数据表号(2字节) */
    INT             CondValue;      /* 条件量值(4字节) */
    UNSIGNED        FaultNumber;    /* 故障序号(2字节) */
    UNSIGNED        RelativeTime;   /* 相对时间(2字节) */
    UNSIGNED        Sect;           /* 扇区号(1字节) */
    UNSIGNED        FunType;        /* 功能类型(1字节) */
    UNSIGNED        INF;            /* 信息序号(1字节) */
    UNSIGNED        DPI;            /* DPI(1字节) */
    UNSIGNED        Year;           /* 年(1字节) */
    UNSIGNED        Month;          /* 月(1字节) */
    UNSIGNED        Day;            /* 日(1字节) */
    UNSIGNED        Hour;           /* 时(1字节) */
    UNSIGNED        Minute;         /* 分(1字节) */
    UNSIGNED        Millionsecond;  /* 毫秒(2字节) */
    UNSIGNED        q;              /* 数据品质因素 */
    UNSIGNED        t_sec;          /* t.sec */
    UNSIGNED        t_frac;         /* t.frac */
    UNSIGNED        t_qflag;        /* t.qflag */
    UNSIGNED        phase;          /* 故障相别 */
}RCC_FAULT_RETURN;

/* 队列数据结构 */
typedef struct RCC_Q_MESSAGE_STRUCT
{
    UNSIGNED        Size;           /* 数据大小 */
    UNSIGNED        Type;           /* 数据类型 */
    /* 数据信息 */
    union DATA_INFO_UNION
    {
        RCC_FAULT_RETURN fault;
        UNSIGNED_CHAR    buf[256];
    }data;
}RCC_Q_MESSAGE;

typedef enum ENUM_XJ_ACK_STEP
{
    LC_STEP_ACK,
    LC_STEP_READ,
    LC_STEP_END
}XJ_ACK_STEP;

typedef struct  STRUCT_GW_RC_CB
{
    GW_REQUEST_CB    head;
    XJ_ACK_STEP      step;
    UNSIGNED         rc_context[CPC_MAX_ONE_FAULT_REPORT_NUMBER];
    UNSIGNED         rc_serial;
    UNSIGNED_CHAR    fun;
    INT              rc_count;
    UNSIGNED         rc_faultNumber;
    UNSIGNED         rc_index;
}GW_RC_CB;

#define ONE_COMM_ESIZE()     ((sizeof(RCC_CheckInfo)+sizeof(UNSIGNED)-1)/sizeof(UNSIGNED))    /* 每个突变事件通讯报文大小 */
#define ONE_COMM_RSIZE()     ((sizeof(RCC_FAULT_RETURN)+sizeof(UNSIGNED)-1)/sizeof(UNSIGNED)) /* 每个返回动作报告通讯报文大小 */

/* 填写突变事件(外部提供时间)函数 */
INT RCC_Fill_CheckInfo( INT iTableID, INT CondValue, INT iType,
    INT iSecInf, INT iFunInf, INT iStatus, UNSIGNED q );
/* 填写突变事件(自带时间)函数 */
INT RCC_Fill_CheckInfo_Full( INT iTableID, INT CondValue, INT iType,
    INT iSecInf, INT iFunInf, INT iStatus, TIME * pTime, UNSIGNED q,
    UNSIGNED tSec, UNSIGNED tFrac, UNSIGNED tQuality );
/* 填写故障返回数据 */
INT RCC_Fault_Return( INT iTableID, INT CondValue, UNSIGNED uFaultNumber,
    UNSIGNED uRelTime, UNSIGNED uSect, UNSIGNED uFunType, UNSIGNED uINF,
    UNSIGNED uDPI, TIME * pTime, UNSIGNED q, UNSIGNED tSec, UNSIGNED tFrac,
    UNSIGNED tQuality, UNSIGNED phase );
/* 填写故障命令报文 */
INT RCC_Fault_Message( VOID * buf );
/* 填写asdu2报文(仅填写返回报文,动作报文由接口上送) */
INT RCC_Pack_ASDU2( UNSIGNED_CHAR * dst );
/* 向接口上送突变报文函数 */
VOID RCC_Pack_BreakEvent( VOID * Dst );
/* 故障返回上送 */
INT RCC_Pack_FaultRet( VOID * Dst );
/* 故障报告检测 */
INT RCC_Check_Report( GW_CHANNEL * pChl, UNSIGNED_CHAR * dst, UNSIGNED_CHAR * dst_port );
/* 填充goose数据函数 */
INT RCC_Fill_Goose( INT iTableID, INT CondValue, INT iType,
    INT iSecInf, INT iFunInf, INT iStatus, TIME * pTime, UNSIGNED q,
    UNSIGNED tSec, UNSIGNED tFrac, UNSIGNED tQuality, UNSIGNED offset );
/* 发送goose数据函数 */
INT RCC_Send_Goose( VOID );
/* 故障报告复制 */
INT lcApp_ProcessReportCopy( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port );

#endif
