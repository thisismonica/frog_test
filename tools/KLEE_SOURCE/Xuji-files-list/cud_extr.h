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
/*      cud_extr.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CUD - Common User data                                           */
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
#include "plat/inc/pro_defs.h"

/* Check to see if the file has been included already.  */

#ifndef CUD_EXTR_H
#define CUD_EXTR_H

#define CUD_USER_DATA_NUMBER (PRO_USER_NUMBER) /* 用户数据的个数 */
#define CUD_NV_TIME_OUT      800               /* 操作非易失芯片的超时时间 */

/* 保存用户数据 */
INT CUDC_SaveUserData( VOID * pBuffer, UNSIGNED uOffset, UNSIGNED uNumber );
/* 读取用户数据 */
INT CUDC_GetUserData( VOID * pBuffer, UNSIGNED uOffset, UNSIGNED uNumber );
/* 用户数据初始化 */
INT CUDC_UserDataInitialize( UNSIGNED uTimeOut );
/* 检测用户数据是否发生变化 */
VOID CUDC_CheckUserDataChange( VOID );

#endif
