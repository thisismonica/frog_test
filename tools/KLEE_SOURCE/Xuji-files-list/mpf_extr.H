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
/*      mpf_extr.h                                      Ver  1.0         */
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
#include "plat/inc/mpf_defs.h"
#include "plat/es_plat.h"

/* Check to see if this file has been included already.  */

#ifndef MPF_EXTR_H
#define MPF_EXTR_H

/* 获取结构成员地址偏移x:结构名称,y:成员名称 */
#define MPF_GETSTRUCTOFFSET(x,y) (int)(&((x*)0)->y)

/* Define external inner-component global data references.  */

/*=======================================================================*/
INT MPFC_Create( VOID * FunList );
INT MPFC_GetHmiVersion( UNSIGNED * verL, UNSIGNED  * verH, UNSIGNED * toolCRC );
INT MPFC_FillGoose( VOID * pInfo );
INT MPFC_SendGoose( VOID );

#endif
