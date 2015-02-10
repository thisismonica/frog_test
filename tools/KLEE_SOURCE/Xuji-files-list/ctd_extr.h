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
/*      ctd_extr.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CTD - Common Thread Management                                   */
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
#include "plat_specific/plat_specific.h"

/* Check to see if the file has been included already.  */

#ifndef CTD_EXTR_H
#define CTD_EXTR_H

/* ��������������״̬ */
INT CTDC_Main_UnSleep( VOID );
/* ��ȡ����(���񡢸߼��жϡ���ʱ��)����ʱ�� */
INT CTDC_GetRunTime( VOID );
/* ��ʼ������(���񡢸߼��жϡ���ʱ��)����ʱ�� */
INT CTDC_InitializeRunTime( VOID );
/* ��ȡ�������е�ַ */
INT CTDC_RetrieveAddressFromTaskStack( NU_TASK * task, UNSIGNED * dst,
    UNSIGNED number, UNSIGNED * actual_number );

#endif
