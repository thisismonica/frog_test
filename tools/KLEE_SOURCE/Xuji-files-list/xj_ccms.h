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
/*      xj_ccms.h                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CCMS - Common cms Management                                     */
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
/*      ZhideYang.      04-13-2007      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"

/* Check to see if the file has been included already.  */

#ifndef XJ_CCMS_H
#define XJ_CCMS_H

#ifdef __cplusplus
extern "C" {
#endif

/* 通信报文系统初始化 */
INT CCMS_Initialize( INT iMaxCount, INT iMaxMessLen );
/* 写通信报文 */
INT CCMS_WriteComMs( CHAR * msg, INT iLen, UNSIGNED uTimeOut );
/* 用户读取报文 */
INT CCMS_UserReadComMs( CHAR * msg, INT iMaxLen, INT * iActLen, UNSIGNED uTimeOut );
/* 删除报文 */
INT CCMS_DelComMs( UNSIGNED uTimeOut );
/* 写通信报文数据主函数 */
INT CCMS_WriteComMsMain( VOID );
/* 修复通讯报文文件 */
INT CCMS_RepairFileData_Main( VOID );
#ifdef __cplusplus
}
#endif

#endif
