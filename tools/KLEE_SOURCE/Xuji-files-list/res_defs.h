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

/* ���� */
#ifndef RES_ENGLISH
#define RES_STANDBY "����"
#else
#define RES_STANDBY "Standby"
#endif

/* '/0' */
#ifndef RES_ENGLISH
#define RES_ZERO ""
#else
#define RES_ZERO ""
#endif

/* װ�ñ������� */
#ifndef RES_ENGLISH
#define RES_DEVICE_ENAME  "ESP-801"                     /* ��̨- 8 */
#define RES_DEVICE_CNAME  "�ţӣУ�����������װ��"      /* �͵���ʾ-36 */
#else
#define RES_DEVICE_ENAME  "ESP-801"                     /* ��̨-8 */
#define RES_DEVICE_CNAME  "ESP-801 DEVICE"              /* �͵���ʾ-36 */
#endif

/* �����ڴ�ռ䲻��-30 */
#ifndef RES_ENGLISH
#define RES_NO_MEMORY "�����ڴ�ռ䲻��"
#else
#define RES_NO_MEMORY " No Memory"
#endif

/* �����ڴ�ռ䲻��-30 */
#ifndef RES_ENGLISH
#define RES_NO_CONFIG_MEMORY "�����ڴ�ռ䲻��"
#else
#define RES_NO_CONFIG_MEMORY " No Config Memory"
#endif

/* �����汾����-30 */
#ifndef RES_ENGLISH
#define RES_VERSION_ERROR "�����汾����"
#else
#define RES_VERSION_ERROR " Version Error"
#endif

/* δ�ҵ�Ӧ�ó������-30 */
#ifndef RES_ENGLISH
#define RES_NO_PROTECT "δ�ҵ���������"
#else
#define RES_NO_PROTECT " No Protect Error"
#endif

/* ͨ��ϵ��Խ��-10 */
#ifndef RES_ENGLISH
#define RES_COEFFICIENT_OVERRANGE "ͨ�� %02d ϵ��Խ��"
#else
#define RES_COEFFICIENT_OVERRANGE " Channel %02d coefficient error"
#endif

/* ͨ����ƯԽ��-10 */
#ifndef RES_ENGLISH
#define RES_EXCURSION_OVERRANGE "ͨ�� %02d ��ƯԽ��"
#else
#define RES_EXCURSION_OVERRANGE " Channel %02d Zero-offset error"
#endif

/* ͨ����λԽ��-10 */
#ifndef RES_ENGLISH
#define RES_PHASE_OVERRANGE "ͨ�� %02d ��λԽ��"
#else
#define RES_PHASE_OVERRANGE " Channel %02d Phase-offset error"
#endif

/* ��ֵ����Խ��-30 */
#ifndef RES_ENGLISH
#define RES_SETTING_ZONE_OVERRANGE "��ֵ����Խ��"
#else
#define RES_SETTING_ZONE_OVERRANGE " Setting block exceed limit"
#endif

/* ��ֵԽ�ޣ�һ������-10 */
#ifndef RES_ENGLISH
#define RES_SETTING_OVERRANGE_ERROR0 "%sԽ��"
#else
#define RES_SETTING_OVERRANGE_ERROR0 " %s SetErr"
#endif

/* ��ֵԽ�ޣ�������-10 */
#ifndef RES_ENGLISH
#define RES_SETTING_OVERRANGE_ERROR "%s %02d ��Խ��"
#else
#define RES_SETTING_OVERRANGE_ERROR " %s %02d SetErr"
#endif

/* ϵͳ��ֵԽ��-30 */
#ifndef RES_ENGLISH
#define RES_SYS_SETTING_ERROR "%sԽ��"
#else
#define RES_SYS_SETTING_ERROR " %s SysSetErr"
#endif

/* NVRAM�Լ����-30 */
#ifndef RES_ENGLISH
#define RES_SRAM_ERROR "�Σ֣ң����Լ����"
#else
#define RES_SRAM_ERROR " NVRAM error"
#endif

/* NVRAM����ָ�-30 */
#ifndef RES_ENGLISH
#define RES_SRAM_ERROR_RECOVERY "�Σ֣ң��ʹ���ָ�"
#else
#define RES_SRAM_ERROR_RECOVERY " NVRAM error recovery"
#endif

/* RAM�Լ����-30 */
#ifndef RES_ENGLISH
#define RES_RAM_ERROR "�ң����Լ����"
#else
#define RES_RAM_ERROR " RAM error"
#endif

/* RAM����ָ�-30 */
#ifndef RES_ENGLISH
#define RES_RAM_ERROR_RECOVERY "�ң��ʹ���ָ�"
#else
#define RES_RAM_ERROR_RECOVERY " RAM error recovery"
#endif

/* ��ֵ�Լ����-30 */
#ifndef RES_ENGLISH
#define RES_SETTING_ERROR "��ֵ�Լ����"
#else
#define RES_SETTING_ERROR " Settings error"
#endif

/* ��ֵ����ָ�-30 */
#ifndef RES_ENGLISH
#define RES_SETTING_ERROR_RECOVERY "��ֵ����ָ�"
#else
#define RES_SETTING_ERROR_RECOVERY " Settings error recovery"
#endif

/* ��Դ�Լ����-30 */
#ifndef RES_ENGLISH
#define RES_POWER_ERROR "��Դ�Լ����"
#else
#define RES_POWER_ERROR " Power error"
#endif

/* ��Դ����ָ�-30 */
#ifndef RES_ENGLISH
#define RES_POWER_ERROR_RECOVERY "��Դ����ָ�"
#else
#define RES_POWER_ERROR_RECOVERY " Power error recovery"
#endif

/* �������Լ����-30 */
#ifndef RES_ENGLISH
#define RES_AD_ERROR "�������Լ����"
#else
#define RES_AD_ERROR " A/D error"
#endif

/* �����Ĵ���ָ�-30 */
#ifndef RES_ENGLISH
#define RES_AD_ERROR_RECOVERY "�����Ĵ���ָ�"
#else
#define RES_AD_ERROR_RECOVERY " A/D error recovery"
#endif

/* ͨѶ�ж�-30 */
#ifndef RES_ENGLISH
#define RES_COMM_ERROR "ͨѶ�ж�"
#else
#define RES_COMM_ERROR " Communication disconnected"
#endif

/* ͨѶ�ָ�-30 */
#ifndef RES_ENGLISH
#define RES_COMM_RECOVERY "ͨѶ�ָ�"
#else
#define RES_COMM_RECOVERY " Communication resume"
#endif

/* �ƣ̣��ӣ��Լ����-30 */
#ifndef RES_ENGLISH
#define RES_FLASH_ERROR "�ƣ̣��ӣ��Լ����"
#else
#define RES_FLASH_ERROR " FLASH error"
#endif

/* �ƣ̣��ӣȴ���ָ�-30 */
#ifndef RES_ENGLISH
#define RES_FLASH_ERROR_RECOVERY "�ƣ̣��ӣȴ���ָ�"
#else
#define RES_FLASH_ERROR_RECOVERY " FLASH error recovery"
#endif

/* ��������У�����-30 */
#ifndef RES_ENGLISH
#define RES_PROGRAM_CHECK_ERROR "��������У�����"
#else
#define RES_PROGRAM_CHECK_ERROR " Program error"
#endif

/* �����������ָ�-30 */
#ifndef RES_ENGLISH
#define RES_PROGRAM_CHECK_RECOVERY "�����������ָ�"
#else
#define RES_PROGRAM_CHECK_RECOVERY " Program error recovery"
#endif

/* ģ����Լ����-30 */
#ifndef RES_ENGLISH
#define RES_CPU_NUMBER_ERROR "ģ����Լ����"
#else
#define RES_CPU_NUMBER_ERROR " CPU number error"
#endif

/* ģ��Ŵ���ָ�-30 */
#ifndef RES_ENGLISH
#define RES_CPU_NUMBER_RECOVERY "ģ��Ŵ���ָ�"
#else
#define RES_CPU_NUMBER_RECOVERY " CPU number OK"
#endif

/* �����Լ����-30 */
#ifndef RES_ENGLISH
#define RES_SWITCH_ERROR "�����Լ����"
#else
#define RES_SWITCH_ERROR " DO error"
#endif

/* ���ڴ���ָ�-30 */
#ifndef RES_ENGLISH
#define RES_SWITCH_RECOVERY "���ڴ���ָ�"
#else
#define RES_SWITCH_RECOVERY " DO error recovery"
#endif

/* ͬ������ͨѶ����-30 */
#ifndef RES_ENGLISH
#define RES_SYN_SERIAL_ERROR "ͬ������ͨѶ����"
#else
#define RES_SYN_SERIAL_ERROR " Synchro-serial COMM. error"
#endif

/* ͬ������ͨѶ�ָ�-30 */
#ifndef RES_ENGLISH
#define RES_SYN_SERIAL_OK "ͬ������ͨѶ�ָ�"
#else
#define RES_SYN_SERIAL_OK " Synchro-serial COMM. OK"
#endif

/* ������·����-30 */
#ifndef RES_ENGLISH
#define RES_DOOUT_DX_ERROR "������· %02d ����"
#else
#define RES_DOOUT_DX_ERROR " DO channel %02d broken"
#endif

/* ������·���߻ָ�-30 */
#ifndef RES_ENGLISH
#define RES_DOOUT_DX_RECOVERY "������· %02d ���߻ָ�"
#else
#define RES_DOOUT_DX_RECOVERY " DO channel %02d OK"
#endif

/* ������·����-30 */
#ifndef RES_ENGLISH
#define RES_DOOUT_JC_ERROR "������·����"
#else
#define RES_DOOUT_JC_ERROR " DO channel breakdown"
#endif

/* ������·�����ָ�-30 */
#ifndef RES_ENGLISH
#define RES_DOOUT_JC_RECOVERY "������·�����ָ�"
#else
#define RES_DOOUT_JC_RECOVERY " DO channel OK"
#endif

/* ��չ�����������-30 */
#ifndef RES_ENGLISH
#define RES_EXPORT_ERROR "��չ�����������"
#else
#define RES_EXPORT_ERROR " DO export error"
#endif

/* ��չ�����������ָ�-30 */
#ifndef RES_ENGLISH
#define RES_EXPORT_RECOVERY "��չ�����������ָ�"
#else
#define RES_EXPORT_RECOVERY " DO export error recovery"
#endif

/* װ��Ӳ����ʶ�����-30 */
#ifndef RES_ENGLISH
#define RES_DEVICE_IDENERROR "װ��Ӳ����ʶ�����"
#else
#define RES_DEVICE_IDENERROR "Device Identify Error"
#endif

#endif
