/*************************************************************************/
/*                                                                       */
/*        Copyright (c) 1993-1998 Accelerated Technology, Inc.           */
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
/*      apf_extr.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains data structure definitions that hide internal */
/*      from the application.                                            */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* DATA STRUCTURES                                                       */
/*                                                                       */
/*                                                                       */
/* FUNCTIONS                                                             */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       01-07-2007      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"
#include "plat/inc/apf_defs.h"

/* Check to see if this file has been included already.  */

#ifndef APF_EXTR_H
#define APF_EXTR_H

#define AP_FUN_LIST_NAME "ProPlatFunList" /* 保护函数表名称(平台使用) */

/* 获取结构成员地址偏移x:结构名称,y:成员名称 */
#define APF_GETSTRUCTOFFSET(x,y) (int)(&((x*)0)->y)

/* Define external inner-component global data references.  */

/*=======================================================================*/
INT APFC_Create( VOID * FunList );
INT APFC_GetParameter( INT Id, void * buffer );
INT APFC_GetDeviceVersion( UNSIGNED * verL, UNSIGNED  * verH );
UNSIGNED APFC_CalProtextCRC( UNSIGNED InitialValue );
/* 注册应用程序的通讯命令 */
INT APFC_RegisterCommand( VOID * channel_ptr );
/* 设置启动录波信息 */
INT APFC_SetTestWaveParam( INT iSpace, INT iStartPoint, INT iTotalPoint );
/* 保存测控数据表数据 */
INT APFC_WriteFckTabData( VOID ** pGsPtrs, INT iNumber );
/* 生成故障量值报文 返回值：报文长度 */
INT APFC_PackAsduReport( VOID * src, UNSIGNED_CHAR * dst, UNSIGNED_CHAR cot,
    UNSIGNED_CHAR index );
/* 生成循环遥测报文 返回值：报文长度 */
INT APFC_PackXHYC( UNSIGNED_CHAR * dst, VOID * pGW_SOE );
#endif
