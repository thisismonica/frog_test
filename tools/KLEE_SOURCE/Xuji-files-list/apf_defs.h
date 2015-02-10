
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
/*      apf_defs.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      AP - Application to PlatForm                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains system constants common to both the           */
/*      application and the actual Nucleus PLUS components.  This file   */
/*      also contains data structure definitions that hide internal      */
/*      information from the application.                                */
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
/*      ZhiDeYang       01-04-2008      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

/* Check to see if this file has been included already.  */

#ifndef APF_DEFS_H
#define APF_DEFS_H

#define AP_FUN_LIST_NAME "ProPlatFunList" /* 保护函数表名称(平台使用) */

/* 设置参数ID定义 */
#define DEVICE_PARAM_SET_ID         1   /* 设置装置基本参数 */
#define ANALOG_CHANNEL_SET_ID       2   /* 设置模拟量基本通道 */
#define OUT_BITS_SET_ID             3   /* 设置装置出口位相关参数 */
#define SET_TABLE_PARAMNUM_ID       4   /* 设置数据表参数个数 */

/* Core processing functions.  */
/*=======================================================================*/
typedef INT (* _get_parameter )( INT Id, void * buffer );

typedef UNSIGNED (* _cal_protext_CRC )( UNSIGNED InitialValue );

typedef INT (* _get_device_ver )( UNSIGNED * verL, UNSIGNED  * verH );

typedef INT (* _register_cmd )( VOID * channel_ptr );

typedef INT (* _set_test_wave_param )( INT iSpace, INT iStartPoint, INT iTotalPoint );

typedef INT (* _write_fck_tab_data )( VOID ** pGsPtrs, INT iNumber );

typedef INT (* _pack_asdu_report )
    ( VOID * src, UNSIGNED_CHAR * dst, UNSIGNED_CHAR cot, UNSIGNED_CHAR SIN );

typedef INT (* _pack_xhyc )( UNSIGNED_CHAR * dst, VOID * pGW_SOE );
/*=======================================================================*/
typedef struct AP_FUN_LIST_STRUCT
{
    int size;
    _get_parameter                      get_parameter;
    _cal_protext_CRC                    cal_protext_CRC;
    _get_device_ver                     get_device_ver;
    _register_cmd                       register_cmd;
    _set_test_wave_param                set_test_wave_param;
    _write_fck_tab_data                 write_fck_tab_data;
    _pack_asdu_report                   pack_asdu_report;
    _pack_xhyc                          pack_xhyc;
}AP_FUN_LIST;

#endif
