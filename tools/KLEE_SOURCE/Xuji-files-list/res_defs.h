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
/*      res_defs.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      RES - Resource constant                                          */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the constants define for recource             */
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
/*      ZhiDeYang       02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

/* Check to see if the file has been included already.  */

#ifndef RES_DEFS
#define RES_DEFS

/* 备用 */
#ifndef RES_ENGLISH
#define RES_STANDBY "备用"
#else
#define RES_STANDBY "Standby"
#endif

/* '/0' */
#ifndef RES_ENGLISH
#define RES_ZERO ""
#else
#define RES_ZERO ""
#endif

/* 装置标题名称 */
#ifndef RES_ENGLISH
#define RES_DEVICE_ENAME  "ESP-801"                     /* 后台- 8 */
#define RES_DEVICE_CNAME  "ＥＳＰ－８０１保护装置"      /* 就地显示-36 */
#else
#define RES_DEVICE_ENAME  "ESP-801"                     /* 后台-8 */
#define RES_DEVICE_CNAME  "ESP-801 DEVICE"              /* 就地显示-36 */
#endif

/* 保护内存空间不够-30 */
#ifndef RES_ENGLISH
#define RES_NO_MEMORY "保护内存空间不够"
#else
#define RES_NO_MEMORY " No Memory"
#endif

/* 配置内存空间不够-30 */
#ifndef RES_ENGLISH
#define RES_NO_CONFIG_MEMORY "配置内存空间不够"
#else
#define RES_NO_CONFIG_MEMORY " No Config Memory"
#endif

/* 保护版本错误-30 */
#ifndef RES_ENGLISH
#define RES_VERSION_ERROR "保护版本错误"
#else
#define RES_VERSION_ERROR " Version Error"
#endif

/* 未找到应用程序错误-30 */
#ifndef RES_ENGLISH
#define RES_NO_PROTECT "未找到保护程序"
#else
#define RES_NO_PROTECT " No Protect Error"
#endif

/* 通道系数越限-10 */
#ifndef RES_ENGLISH
#define RES_COEFFICIENT_OVERRANGE "通道 %02d 系数越限"
#else
#define RES_COEFFICIENT_OVERRANGE " Channel %02d coefficient error"
#endif

/* 通道零漂越限-10 */
#ifndef RES_ENGLISH
#define RES_EXCURSION_OVERRANGE "通道 %02d 零漂越限"
#else
#define RES_EXCURSION_OVERRANGE " Channel %02d Zero-offset error"
#endif

/* 通道相位越限-10 */
#ifndef RES_ENGLISH
#define RES_PHASE_OVERRANGE "通道 %02d 相位越限"
#else
#define RES_PHASE_OVERRANGE " Channel %02d Phase-offset error"
#endif

/* 定值区号越限-30 */
#ifndef RES_ENGLISH
#define RES_SETTING_ZONE_OVERRANGE "定值区号越限"
#else
#define RES_SETTING_ZONE_OVERRANGE " Setting block exceed limit"
#endif

/* 定值越限（一个区）-10 */
#ifndef RES_ENGLISH
#define RES_SETTING_OVERRANGE_ERROR0 "%s越限"
#else
#define RES_SETTING_OVERRANGE_ERROR0 " %s SetErr"
#endif

/* 定值越限（多区）-10 */
#ifndef RES_ENGLISH
#define RES_SETTING_OVERRANGE_ERROR "%s %02d 区越限"
#else
#define RES_SETTING_OVERRANGE_ERROR " %s %02d SetErr"
#endif

/* 系统定值越限-30 */
#ifndef RES_ENGLISH
#define RES_SYS_SETTING_ERROR "%s越限"
#else
#define RES_SYS_SETTING_ERROR " %s SysSetErr"
#endif

/* NVRAM自检出错-30 */
#ifndef RES_ENGLISH
#define RES_SRAM_ERROR "ＮＶＲＡＭ自检出错"
#else
#define RES_SRAM_ERROR " NVRAM error"
#endif

/* NVRAM错误恢复-30 */
#ifndef RES_ENGLISH
#define RES_SRAM_ERROR_RECOVERY "ＮＶＲＡＭ错误恢复"
#else
#define RES_SRAM_ERROR_RECOVERY " NVRAM error recovery"
#endif

/* RAM自检出错-30 */
#ifndef RES_ENGLISH
#define RES_RAM_ERROR "ＲＡＭ自检出错"
#else
#define RES_RAM_ERROR " RAM error"
#endif

/* RAM错误恢复-30 */
#ifndef RES_ENGLISH
#define RES_RAM_ERROR_RECOVERY "ＲＡＭ错误恢复"
#else
#define RES_RAM_ERROR_RECOVERY " RAM error recovery"
#endif

/* 定值自检出错-30 */
#ifndef RES_ENGLISH
#define RES_SETTING_ERROR "定值自检出错"
#else
#define RES_SETTING_ERROR " Settings error"
#endif

/* 定值错误恢复-30 */
#ifndef RES_ENGLISH
#define RES_SETTING_ERROR_RECOVERY "定值错误恢复"
#else
#define RES_SETTING_ERROR_RECOVERY " Settings error recovery"
#endif

/* 电源自检出错-30 */
#ifndef RES_ENGLISH
#define RES_POWER_ERROR "电源自检出错"
#else
#define RES_POWER_ERROR " Power error"
#endif

/* 电源错误恢复-30 */
#ifndef RES_ENGLISH
#define RES_POWER_ERROR_RECOVERY "电源错误恢复"
#else
#define RES_POWER_ERROR_RECOVERY " Power error recovery"
#endif

/* Ａ／Ｄ自检出错-30 */
#ifndef RES_ENGLISH
#define RES_AD_ERROR "Ａ／Ｄ自检出错"
#else
#define RES_AD_ERROR " A/D error"
#endif

/* Ａ／Ｄ错误恢复-30 */
#ifndef RES_ENGLISH
#define RES_AD_ERROR_RECOVERY "Ａ／Ｄ错误恢复"
#else
#define RES_AD_ERROR_RECOVERY " A/D error recovery"
#endif

/* 通讯中断-30 */
#ifndef RES_ENGLISH
#define RES_COMM_ERROR "通讯中断"
#else
#define RES_COMM_ERROR " Communication disconnected"
#endif

/* 通讯恢复-30 */
#ifndef RES_ENGLISH
#define RES_COMM_RECOVERY "通讯恢复"
#else
#define RES_COMM_RECOVERY " Communication resume"
#endif

/* ＦＬＡＳＨ自检出错-30 */
#ifndef RES_ENGLISH
#define RES_FLASH_ERROR "ＦＬＡＳＨ自检出错"
#else
#define RES_FLASH_ERROR " FLASH error"
#endif

/* ＦＬＡＳＨ错误恢复-30 */
#ifndef RES_ENGLISH
#define RES_FLASH_ERROR_RECOVERY "ＦＬＡＳＨ错误恢复"
#else
#define RES_FLASH_ERROR_RECOVERY " FLASH error recovery"
#endif

/* 保护程序校验出错-30 */
#ifndef RES_ENGLISH
#define RES_PROGRAM_CHECK_ERROR "保护程序校验出错"
#else
#define RES_PROGRAM_CHECK_ERROR " Program error"
#endif

/* 保护程序错误恢复-30 */
#ifndef RES_ENGLISH
#define RES_PROGRAM_CHECK_RECOVERY "保护程序错误恢复"
#else
#define RES_PROGRAM_CHECK_RECOVERY " Program error recovery"
#endif

/* 模块号自检出错-30 */
#ifndef RES_ENGLISH
#define RES_CPU_NUMBER_ERROR "模块号自检出错"
#else
#define RES_CPU_NUMBER_ERROR " CPU number error"
#endif

/* 模块号错误恢复-30 */
#ifndef RES_ENGLISH
#define RES_CPU_NUMBER_RECOVERY "模块号错误恢复"
#else
#define RES_CPU_NUMBER_RECOVERY " CPU number OK"
#endif

/* 出口自检出错-30 */
#ifndef RES_ENGLISH
#define RES_SWITCH_ERROR "出口自检出错"
#else
#define RES_SWITCH_ERROR " DO error"
#endif

/* 出口错误恢复-30 */
#ifndef RES_ENGLISH
#define RES_SWITCH_RECOVERY "出口错误恢复"
#else
#define RES_SWITCH_RECOVERY " DO error recovery"
#endif

/* 同步串口通讯出错-30 */
#ifndef RES_ENGLISH
#define RES_SYN_SERIAL_ERROR "同步串口通讯出错"
#else
#define RES_SYN_SERIAL_ERROR " Synchro-serial COMM. error"
#endif

/* 同步串口通讯恢复-30 */
#ifndef RES_ENGLISH
#define RES_SYN_SERIAL_OK "同步串口通讯恢复"
#else
#define RES_SYN_SERIAL_OK " Synchro-serial COMM. OK"
#endif

/* 开出回路断线-30 */
#ifndef RES_ENGLISH
#define RES_DOOUT_DX_ERROR "开出回路 %02d 断线"
#else
#define RES_DOOUT_DX_ERROR " DO channel %02d broken"
#endif

/* 开出回路断线恢复-30 */
#ifndef RES_ENGLISH
#define RES_DOOUT_DX_RECOVERY "开出回路 %02d 断线恢复"
#else
#define RES_DOOUT_DX_RECOVERY " DO channel %02d OK"
#endif

/* 开出回路击穿-30 */
#ifndef RES_ENGLISH
#define RES_DOOUT_JC_ERROR "开出回路击穿"
#else
#define RES_DOOUT_JC_ERROR " DO channel breakdown"
#endif

/* 开出回路击穿恢复-30 */
#ifndef RES_ENGLISH
#define RES_DOOUT_JC_RECOVERY "开出回路击穿恢复"
#else
#define RES_DOOUT_JC_RECOVERY " DO channel OK"
#endif

/* 扩展开出插件出错-30 */
#ifndef RES_ENGLISH
#define RES_EXPORT_ERROR "扩展开出插件错误"
#else
#define RES_EXPORT_ERROR " DO export error"
#endif

/* 扩展开出插件出错恢复-30 */
#ifndef RES_ENGLISH
#define RES_EXPORT_RECOVERY "扩展开出插件错误恢复"
#else
#define RES_EXPORT_RECOVERY " DO export error recovery"
#endif

/* 装置硬件标识码错误-30 */
#ifndef RES_ENGLISH
#define RES_DEVICE_IDENERROR "装置硬件标识码错误"
#else
#define RES_DEVICE_IDENERROR "Device Identify Error"
#endif

#endif
