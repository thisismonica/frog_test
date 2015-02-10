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

#define CIN_INPUT_TYPE_VALUE        1                                   /* 开入量值 */
#define CIN_INPUT_TYPE_QUENTY       2                                   /* 开入量+品质因素 */
#define CIN_SWITCH_STATUS_NUMBER    (CPC_MAX_INPUT_GROUPS+9)            /* 开关状态量记录数 */

extern ESP_CFG PLAT_cfg;
#define CINC_INPUT_PORT_MEMORY      (PLAT_cfg.input.InputPort)          /* 开入端口首地址 */
#define CINC_INPUT_GROUP_NUMBER     (PLAT_cfg.input.InputGroups)        /* 开入端口组数 */
#define CINC_WAVE_INPUT_GROUP_NUMBER (PLAT_cfg.input.WaveInputGroups)   /* 录波开入量组数 */
#define CINC_HARD_INPUT_GROUPS      (PLAT_cfg.input.HardInputGroups)    /* 硬件开入量组数 */

/* 初始化开入驱动 */
INT CINC_CreateInputDriver( VOID );
/* 读取开入状态量值（中断使用） */
UNSIGNED CINC_ReadInput( INT GroupNumber );
/* 读取开入状态量值 */
UNSIGNED CINC_ReadInputValue( INT GroupNumber );
/* 读开入量品质因素 */
INT CINC_ReadInputQuality( INT GroupNumber, UNSIGNED * pBits, UNSIGNED * pQuality );
/* 获取变位检查状态量信息 */
VOID * CINC_SwitchStatusInformation( UNSIGNED * Actual_Size, INT * Actual_Group );
/* 获取变位记录数据指针 */
VOID * CINC_SwitchStatusPointer( VOID );
/* 开关量状态变位检测 */
INT CINC_SwitchStatusDetection( VOID );

/* 设置测控开入量变位检测频率 */
VOID CINC_SetupFckInput( INT iTimeFlag );
/* 获取测控开入量变位检测频率 */
UNSIGNED CINC_GetFckInputCheckFreq( VOID );
/* 设置变位有效检测位掩码 */
INT CINC_SetSwitchStatusMasksBits( UNSIGNED GroupNumber, UNSIGNED MaskBits );
/* 设置软开入量状态 */
INT CINC_SetSoftInputStatus( INT iGroup, UNSIGNED uBits, INT iStatus );
/* 设置软开入量品质因素 */
INT CINC_SetSoftInputQuality( INT iGroup, UNSIGNED uBits, UNSIGNED Quality );
/* 设置开入量是否检测标志 */
INT  CINC_SetInputCheckFlag( INT iFlag );
/* 设置软开入整组传输时掩码 */
INT CINC_SetSoftInFullMasks( UNSIGNED GroupNumber, UNSIGNED MaskBits );
/* 获取软开入整组传输时掩码 */
INT CINC_GetSoftInFullMasks( UNSIGNED GroupNumber, UNSIGNED * MaskBits );

#endif
