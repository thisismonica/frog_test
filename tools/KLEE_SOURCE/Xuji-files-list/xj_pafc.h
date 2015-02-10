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
/*      xj_pafc.h                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      PAF - Platform Application Function                              */
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
/*      ZhideYang.      02-20-2008      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"
#include "plat/es_plat.h"

/* Check to see if the file has been included already.  */

#ifndef XJ_PAFC_H
#define XJ_PAFC_H

#ifdef __cplusplus
extern "C" {
#endif

/*=======================================================================*/
/* ��ʼ��ƽ̨�ṩ������ʹ�õķ������� */
VOID PAFC_PlatProFunList_Initialize( VOID );
/* ����Ӧ�ó��� */
INT PAFC_LinkApplication(  UNSIGNED StartAddr, UNSIGNED Length, VOID(* WatchDogReset )( VOID ) );
/* ��ʼ��Ӧ�ó��� */
INT PAFC_InitializeApplication( VOID * mem_pool, VOID * uncached_mem_pool );
/* ������������ */
INT PAFC_LeadApplication( VOID );
/* ��ȡ����ָ�� */
VOID * PAFC_GetFunctionPointer( CHAR * name );

#ifdef __cplusplus
}
#endif

#endif
