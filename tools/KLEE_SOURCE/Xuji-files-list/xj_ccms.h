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

/* ͨ�ű���ϵͳ��ʼ�� */
INT CCMS_Initialize( INT iMaxCount, INT iMaxMessLen );
/* дͨ�ű��� */
INT CCMS_WriteComMs( CHAR * msg, INT iLen, UNSIGNED uTimeOut );
/* �û���ȡ���� */
INT CCMS_UserReadComMs( CHAR * msg, INT iMaxLen, INT * iActLen, UNSIGNED uTimeOut );
/* ɾ������ */
INT CCMS_DelComMs( UNSIGNED uTimeOut );
/* дͨ�ű������������� */
INT CCMS_WriteComMsMain( VOID );
/* �޸�ͨѶ�����ļ� */
INT CCMS_RepairFileData_Main( VOID );
#ifdef __cplusplus
}
#endif

#endif
