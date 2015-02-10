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
/*      pmi_extr.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      PMI - Platform Initialize                                        */
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
/*      ZhideYang.      04-12-2007      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"

/* Check to see if the file has been included already.  */

#ifndef PMI_EXTR_H
#define PMI_EXTR_H

#ifdef __cplusplus
extern "C" {
#endif

/*=======================================================================*/
/* ����Ӧ�ó������ò��� */
INT PMI_SetApplicationCfg( VOID );
/* ƽ̨���ò����ĳ�ʼ�� */
INT PMI_InitializePlatCfg( VOID );
/* ��ȡװ�ù���ѡ�� */
INT PMI_get_device_fun_select( VOID );
/* ��ʼ��ģ�������� */
INT PMI_InitializeChannelPara( VOID );
/* ��ʼ��Ӧ�ó��� */
INT PMI_InitializeApplication( VOID );
/* ��ʼ��ѭ�������� */
INT PMI_InitializeCircleMemory( VOID );
/* ���ݱ�ĳ�ʼ�� */
INT PMI_InitializeTable( VOID );
/* ��ȡ���ݱ��������Ϣ */
INT PMI_GetTableDataInformation( VOID );
/* ��ʼ��װ�����е� */
INT PMI_InitializeDeviceLed( VOID );
/* ���ø澯�������е�״̬���� */
INT PMI_SetAlarmANDRunLedStatus( VOID );
/* ����ƽ̨��澯���¼�λ */
INT  PMI_SetPlatAlarmLedBits( UNSIGNED uPlatAlarmBits );
/* ����ƽ̨���е��쳣�¼�λ���� */
INT PMI_SetPlatRunLedAbnorBits( UNSIGNED uRunAbnorEventBits );

#ifdef __cplusplus
}
#endif

#endif
