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
/*      cin_extr.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CIN - Common Input Management                                    */
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
/*      ZhideYang.      03-16-2007      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"
#include "plat_specific/plat_specific.h"
#include "plat/inc/esp_cfg.h"

/* Check to see if the file has been included already.  */

#ifndef CIN_EXTR_H
#define CIN_EXTR_H

#define CIN_INPUT_TYPE_VALUE        1                                   /* ������ֵ */
#define CIN_INPUT_TYPE_QUENTY       2                                   /* ������+Ʒ������ */
#define CIN_SWITCH_STATUS_NUMBER    (CPC_MAX_INPUT_GROUPS+9)            /* ����״̬����¼�� */

extern ESP_CFG PLAT_cfg;
#define CINC_INPUT_PORT_MEMORY      (PLAT_cfg.input.InputPort)          /* ����˿��׵�ַ */
#define CINC_INPUT_GROUP_NUMBER     (PLAT_cfg.input.InputGroups)        /* ����˿����� */
#define CINC_WAVE_INPUT_GROUP_NUMBER (PLAT_cfg.input.WaveInputGroups)   /* ¼������������ */
#define CINC_HARD_INPUT_GROUPS      (PLAT_cfg.input.HardInputGroups)    /* Ӳ������������ */

/* ��ʼ���������� */
INT CINC_CreateInputDriver( VOID );
/* ��ȡ����״̬��ֵ���ж�ʹ�ã� */
UNSIGNED CINC_ReadInput( INT GroupNumber );
/* ��ȡ����״̬��ֵ */
UNSIGNED CINC_ReadInputValue( INT GroupNumber );
/* ��������Ʒ������ */
INT CINC_ReadInputQuality( INT GroupNumber, UNSIGNED * pBits, UNSIGNED * pQuality );
/* ��ȡ��λ���״̬����Ϣ */
VOID * CINC_SwitchStatusInformation( UNSIGNED * Actual_Size, INT * Actual_Group );
/* ��ȡ��λ��¼����ָ�� */
VOID * CINC_SwitchStatusPointer( VOID );
/* ������״̬��λ��� */
INT CINC_SwitchStatusDetection( VOID );

/* ���ò�ؿ�������λ���Ƶ�� */
VOID CINC_SetupFckInput( INT iTimeFlag );
/* ��ȡ��ؿ�������λ���Ƶ�� */
UNSIGNED CINC_GetFckInputCheckFreq( VOID );
/* ���ñ�λ��Ч���λ���� */
INT CINC_SetSwitchStatusMasksBits( UNSIGNED GroupNumber, UNSIGNED MaskBits );
/* ����������״̬ */
INT CINC_SetSoftInputStatus( INT iGroup, UNSIGNED uBits, INT iStatus );
/* ����������Ʒ������ */
INT CINC_SetSoftInputQuality( INT iGroup, UNSIGNED uBits, UNSIGNED Quality );
/* ���ÿ������Ƿ����־ */
INT  CINC_SetInputCheckFlag( INT iFlag );
/* �����������鴫��ʱ���� */
INT CINC_SetSoftInFullMasks( UNSIGNED GroupNumber, UNSIGNED MaskBits );
/* ��ȡ�������鴫��ʱ���� */
INT CINC_GetSoftInFullMasks( UNSIGNED GroupNumber, UNSIGNED * MaskBits );

#endif
