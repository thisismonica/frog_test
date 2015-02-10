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
/*      cex_extr.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CEX - Common Exchange data                                       */
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
/*      pro_defs.h                          Protect Control constants    */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhideYang.      08-13-2008      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"

/* Check to see if the file has been included already.  */

#ifndef CEX_EXTR_H
#define CEX_EXTR_H

#define CEX_PLAT_EXCHANGE_NUMBER            2 /* 双CPU平台交换数据的个数 */

/* 交换数据位置偏移定义 */
#define CEX_EXCHANGE_START_OFFSET           0                       /* 启动继电器状态位置偏移 */
#define CEX_EXCHANGE_FAULT_NUMBER_OFFSET    1                       /* 故障序号位置偏移 */
#define CEX_EXCHANGE_USER_DATA_OFFSET       2                       /* 用户数据的位置偏移 */

/*动态内存管理操作接口数据结构和操作*/
typedef void * (* EX_MemAlloc )( size_t );
typedef void (* EX_MemFree )( void *);
struct EXMemMService
{
    /* 申请内存 */
    EX_MemAlloc MemAlloc;
    /* 释放内存 */
    EX_MemFree MemFree;
};

/* 双CPU交换数据类结构 */
typedef struct CEXCHANGE_STRUCT CEXCHANGE;
/* 双CPU交换数据类结构 */
struct CEXCHANGE_STRUCT
{
    /*private: */
    /* 双CPU交换数据个数 */
    INT CpuExchangeNumber;
    /* 双CPU交换数据的接收缓冲区 */
    VOID * CpuExchangeRecv;
    /* 双CPU交换数据的发送缓冲区 */
    VOID * CpuExchangeSend;
    /* 用户交换数据个数 */
    INT UserExchangeNumber;
    /* 用户交换数据的接收缓冲区 */
    VOID * UserExchangeRecv;
    /* 用户交换数据的发送缓冲区 */
    VOID * UserExchangeSend;
};

/* 初始化交换数据信息 */
INT CEXC_Initialize( INT UserExchangeNumber );
/* 获取交换（对侧CPU）的报告故障序号 */
INT CEXC_GetExchangeFaultNumber( VOID );
/* 读取接收到的用户交换数据 */
INT CEXC_ReadUserExchangeData( UNSIGNED * pBuffer );
/* 保存需要发送的用户交换数据 */
INT CEXC_SendUserExchangeData( UNSIGNED * pBuffer );

#endif
