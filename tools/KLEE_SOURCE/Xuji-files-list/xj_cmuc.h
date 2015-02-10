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
/*      xj_cmuc.h                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CMU - Common Mutex Management                                    */
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

#ifndef XJ_CMUC_H
#define XJ_CMUC_H

#ifdef __cplusplus
extern "C" {
#endif

//typedef long HMUM; /* Ӧ�ó�������� */
typedef VOID * HMUM; /* Ӧ�ó�������� */

/*=======================================================================*/
/* ������                                                              */
/*=======================================================================*/
/* �������� */
/* �������������� */
HMUM CMUC_Create( CHAR *name, UNSIGNED initial_count, OPTION suspend_type );
/*=======================================================================*/
/* ����������                                                            */
/*=======================================================================*/
/* ��ʼ�������� */
INT CMUC_Initialize( VOID );
/* ��ȡ������ */
INT CMUC_ObtainMutex( HMUM Handle, UNSIGNED Timeout );
/* �ͷŻ����� */
INT CMUC_ReleaseMutex( HMUM Handle );

#ifdef __cplusplus
}
#endif

#endif
