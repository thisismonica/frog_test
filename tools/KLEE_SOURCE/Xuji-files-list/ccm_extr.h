
/*************************************************************************/
/*                                                                       */
/*        Copyright (c) 1999-2002 XJ ELECTRIC CO.LTD.                    */
/*                                                                       */
/* PROPRIETARY RIGHTS of XJ ELECTRIC CO.LTD are involved in the          */
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
/*      ccmc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CCM- Common Config Management                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common config       */
/*      management component.                                            */
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
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       11-15-2007      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
/* Check to see if the file has been included already.  */

#ifndef CCM_EXTR_H
#define CCM_EXTR_H

#define CCMC_MAX_RUN_TEXT           5      /* 网关、接口、保护、驱动、平台，共5段 */
#define CCMC_FILE_NAME_LEN          64     /* 文件名称长度 */

typedef struct  STRUCT_RUN_TEXT_CB
{
    UNSIGNED    CrcCode;
    UNSIGNED    Version;
    CHAR        Name[CCMC_FILE_NAME_LEN];
}RUN_TEXT_CB;

/* Define internal function calls.  */
VOID CCMC_LoadNVdata( VOID );
INT CCMC_ProgramConfigMain( VOID );
/* 保存出口保持位函数 */
VOID CCMC_SaveOutputHoldBits( VOID );
/* 设置平台引导标识 */
INT CCMC_SetPlatLeadFlag( int iFlag );
/* 获取平台引导标识 */
INT CCMC_GetPlatLeadFlag( VOID );
/* 设置装置分模块初始化成功函数 */
INT CCMC_SetModInitSuccess( VOID );
/* 获取装置分模块初始化完成个数 */
INT CCMC_GetModInitCount( VOID );
/* 数据表数据备份 */
INT CCMC_TableDataSave( VOID );
/* 设置装置分模块初始化成功函数 */
INT CCMC_GetModuleCrcInf( INT * pModuleNum, VOID * * pRunTextCb );

#endif
