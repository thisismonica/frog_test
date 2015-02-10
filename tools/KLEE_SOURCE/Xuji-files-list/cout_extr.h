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
/*      cout_extr.h                                     Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      COUT - Common Output Management                                  */
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
#include "plat/inc/esp_cfg.h"

/* Check to see if the file has been included already.  */
#ifndef COUT_EXTR_H
#define COUT_EXTR_H

extern ESP_CFG PLAT_cfg;
#define COUTC_OUTPUT_PORT_MEMORY    (PLAT_cfg.output.OutputPort)      /* 开出端口首地址 */
#define COUTC_OUTPUT_GROUP_NUMBER   (PLAT_cfg.output.OutputGroups)    /* 开出端口组数 */
#define COUTC_H_OUTPUT_GROUP_NUMBER (PLAT_cfg.output.HardOutputGroups) /* 硬件开出端口组数 */
#define COUTC_S_OUTPUT_GROUP_NUMBER (PLAT_cfg.output.SoftOutputGroups) /* 数字开出端口组数 */

/* 初始化函数 */
VOID COUTC_OutputInitialize( VOID );
/* 设置开出量品质因素,不用获取事件位 */
INT COUTC_SetOutputQualityBits( INT GroupNumber, UNSIGNED QualityBits );
/* 读数字开出量状态及品质因素 */
INT COUTC_ReadSoftOutput( INT iGroup, UNSIGNED * pBits, UNSIGNED * pQuality );
/* 读数字开出量状态,品质因素(有效无效及检修位) */
INT COUTC_ReadSoftOutTotalQua( INT  iGroup, UNSIGNED * pBits, UNSIGNED * pQuality, UNSIGNED * pCheckQuality );
/* 设置开出量品质因素检修位,不用获取事件位 */
INT COUTC_SetOutCheckQualityBits( INT GroupNumber, UNSIGNED CheckQuaBits );
#endif
