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
/*      xj_pgfc.h                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      PGF - Platform GWS Function                                      */
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

/* Check to see if the file has been included already.  */

#ifndef XJ_PGFC_H
#define XJ_PGFC_H

#ifdef __cplusplus
extern "C" {
#endif

/*=======================================================================*/
/* ��ʼ��ƽ̨�ṩ��Gws61850ʹ�õķ������� */
VOID PGFC_PlatGwsFunList_Initialize( VOID );
/* ����Gws61850���� */
INT PGFC_LinkGwsApplication(  UNSIGNED StartAddr, UNSIGNED Length, VOID(* WatchDogReset )( VOID ) );
/* ��ʼ�Gws61850���� */
INT PGFC_InitializeGwsApplication( VOID * mem_pool, VOID * uncached_mem_pool );
/* ����Gws61850���� */
INT PGFC_LeadGwsApplication( VOID );

#ifdef __cplusplus
}
#endif

#endif
