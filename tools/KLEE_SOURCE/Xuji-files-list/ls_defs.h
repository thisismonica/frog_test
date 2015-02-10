
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
/*      ls_defs.h                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      LS - Lonworks Communication to Supervisor                        */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains data structure definitions and constants for  */
/*      the Lonworks Communication to Supervisor.                        */
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

#include "plat/inc/xj_cosc.h"
#include "plat/es_plat.h"
#include "plat/inc/tab_defs.h"
#include "plat/inc/esp_cfg.h"
#include "plat/inc/csmv_extr.h"
#include "plat/net_inc/commu.h"
#include "plat/net_inc/ver_defs.h"
#include "plat_specific/plat_specific.h"

/* Check to see if the file has been included already.  */

#ifndef LS_DEFS_H
#define LS_DEFS_H

/* Define macro to eliminate warnings for unused parameters */
#define LS_UNUSED_PARAM(parameter)      (VOID)parameter

/* 外部变量 */
extern ESP_CFG PLAT_cfg;
extern NU_PROTECT * pReport_Protect;
extern NU_PROTECT * pWave_Protect;
extern NU_EVENT_GROUP Event_2_CB;
extern UNSIGNED CCMC_platCRC;
extern UNSIGNED CCMC_devCRC;
extern UNSIGNED CCMC_hmiCRC;
extern UNSIGNED CCMC_gwsCRC;
#define LS_CPU_NUMBER                   (PLAT_cfg.device.CpuNumber)       /* 装置CPU号码 */
#define LS_CPU_TYPE                     (PLAT_cfg.device.CpuType)         /* CPU类型 */
#define LS_SET_ZONE_COUNT               (PLAT_cfg.device.SetZoneCount)    /* 装置定值区个数 */
#define LS_GET_DEV_CRC                  CCMC_devCRC                       /* 装置CRC码 */
#define LS_GET_PLAT_CRC                 CCMC_platCRC                      /* 平台CRC码 */
#define LS_GET_HMI_CRC                  CCMC_hmiCRC                       /* 人机接口CRC码 */
#define LS_GET_GWS_CRC                  CCMC_gwsCRC                       /* 网关CRC码 */
#define LS_USER_RECORD_EVENT_GROUP      (PLAT_cfg.history.UserEventGroups) /* 用户事件组数 */
#define LS_GET_OUTPUT_SIGNAL_LED_MEMORY (PLAT_cfg.output.HardOutputSignalLed) /* 硬件信号灯动作位内存地址 */
#define LS_GET_OUTPUT_TRIP_LED_MEMORY   (PLAT_cfg.output.HardOutputTripLed)   /* 硬件跳闸灯动作位内存地址 */
#define LS_GET_OUTPUT_GROUPS            (PLAT_cfg.output.OutputGroups)        /* 开出量组数 */
#define LS_GET_OUTPUT_RESET_MEMORY      (PLAT_cfg.output.HardOutputReset)     /* 复归继电器出口位内存地址 */

#define LS_ONE_REPORT_SIZE              sizeof( CRM_RFD )                 /* 一个报告所需的最大空间 */
#define LS_DC_COEFFICIENT               CSMV_GET_DC_COEFFICIENT           /* 直流计算系数 */
#define LS_MAX_CHL_NUMBER               CPC_MAX_CHL_NUMBER                /* 最大模拟量通道数据个数 */
#define LS_COMMU_VER                    VER_COMMU                         /* 通信版本号 */
#define LS_PLATFORM_VER                 VER_PLATFORM                      /* 平台版本号 */
#define LS_DATA_MASKS8                  CPC_DATA_MASKS8                   /* 8位数据掩码 */
#define LS_DATA_MASKS16                 CPC_DATA_MASKS16                  /* 16位数据掩码 */
#define LS_DATA_MASKS32                 CPC_DATA_MASKS32                  /* 32位数据掩码 */
#define LS_BYTE_BITS                    CPC_BYTE_BITS                     /* 字节位数 */
#define LS_DISABLE_DISPLAY              CPC_DISABLE_DISPLAY               /* 屏蔽状态 */
#define LS_DATA_WIDTH                   (CPC_RAM_DATA_BITS/CPC_BYTE_BITS) /* 内存数据宽度（字节数）*/
#define LS_ALARM_NAME                   CEM_EVENT_NAME                    /* 告警事件名称字符串长度 */
#define LS_EXCHANGE_NUMBER              (PRO_EXCHANGE_NUMBER+2)           /* 交换数据总长度 */
#define LS_MAX_PROTECT_GROUPS           CPC_MAX_PROTECT_GROUPS            /* 最大保护状态量组数 */
#define LS_MAX_INPUT_GROUPS             CPC_MAX_INPUT_GROUPS              /* 最大开入量组数 */
#define LS_MAX_OUTPUT_GROUPS            CPC_MAX_OUTPUT_GROUPS             /* 最大开出量组数 */
#define LS_MAX_PROTECT_LOG_GROUPS       CPC_MAX_PROTECT_LOG_GROUPS        /* 最大逻辑状态量组数 */
#define LS_MAX_WAVE_DATA_SIZE           CPC_MAX_WAVE_DATA_SIZE            /* 最大录波数据点数 */
#define LS_GET_CHANNEL_POINT            CSMV_GET_CHANNEL_POINT            /* 模拟量通道的采样点数 */
#define LS_DATA_BIT16                   16                                /* 16位数据的位数 */
//
#define LS_CHANNEL_TABLE_MEMORY         GET_CHANNEL_TAB_MEMORY            /* 模拟量通道数据表内存首址 */
#define LS_CHANNEL_TABLE_NUMBER         GET_CHANNEL_TAB_NUMBER            /* 模拟量通道数据表的数据个数 */
#define LS_INPUT_TAB_MEMORY             GET_INPUT_TAB_MEMORY              /* 开入量数据表内存首址 */
#define LS_INPUT_TAB_NUMBER             GET_INPUT_TAB_NUMBER              /* 开入量数据表的数据个数 */
#define LS_OUTPUT_TAB_MEMORY            GET_OUTPUT_TAB_MEMORY             /* 开出量数据表内存首址 */
#define LS_OUTPUT_TAB_NUMBER            GET_OUTPUT_TAB_NUMBER             /* 开出量数据表的数据个数 */
#define LS_WAVE_STATE_TAB_MEMORY        GET_WAVE_STATE_TAB_MEMORY         /* 录波状态量数据表内存首址 */
#define LS_WAVE_STATE_TAB_NUMBER        GET_WAVE_STATE_TAB_NUMBER         /* 录波状态量数据表的数据个数 */
//
#define LS_CALCULATE_DC(chl,coef,offset)       CSMV_CalculateDC(chl,coef,offset) /* 计算模拟量通道的直流分量 */
#define LS_CALCULATE_FOURIER(chl,para,offset)  CSMV_CalculateFourier(chl,para,offset) /* 计算模拟量通道的谐波分量 */
#define LS_CALCULATE_PEAK_VALUE(chl,offset)    CSMV_CalculatePeakValue(chl,offset) /* 计算模拟量通道的峰值 */
extern float IEEE_To_Dev_Float( UNSIGNED udata );
extern UNSIGNED DEV_Float_To_IEEE( float fvalue );
#define LS_FLOAT_TO_IEEE(x)             DEV_Float_To_IEEE(x)              /* 装置浮点转换成IEEE格式 */
#define LS_IEEE_TO_FLOAT(x)             IEEE_To_Dev_Float(x)              /* IEEE格式数据转换成浮点格式 */
#define LS_WORD_TO_BYTE(dst,src,len)    XJP_CSC_Word_To_Byte(dst,src,len) /* 将WORD数据转换成字节数据 */
#define LS_BYTE_TO_WORD(dst,src,len)    XJP_CSC_Byte_To_Word(dst,src,len) /* 将字节数据转换成WORD数据 */
#define LS_REPORT_PROTECT()             NU_Protect(pReport_Protect)       /* 报告保护 */
#define LS_REPORT_UNPROTECT()           NU_Unprotect()                    /* 释放报告数据保护 */
#define LS_WAVE_PROTECT()               NU_Protect(pWave_Protect)         /* 录波保护 */
#define LS_WAVE_UNPROTECT()             NU_Unprotect()                    /* 释放录波数据保护 */
#define LS_READ_INPUT(group)            CINC_ReadInputValue(group)        /* 读开入状态量 */
#define LS_INPUT_GROUP_NUMBER           CINC_INPUT_GROUP_NUMBER           /* 开入端口组数 */
#define LS_GET_GPS_PULSE_NUMBER         XJP_TM_Get_Pulse_Counter()        /* 获取GPS脉冲计数 */
#define LS_SET_PROG_READY()  NU_Set_Events(& Event_2_CB, CPC_EVENT2_PROG_READY, NU_OR ) /* 设置下程序预备态 */
#define LS_CLR_PROG_READY()  \
        NU_Set_Events(&Event_2_CB,(UNSIGNED)(~CPC_EVENT2_PROG_READY),NU_AND) /* 清除下程序预备态 */
#define LS_GET_PROG_READY(ActEventPtr) \
        NU_Retrieve_Events(&Event_2_CB,CPC_EVENT2_PROG_READY,NU_AND,ActEventPtr,NU_NO_SUSPEND) /* 获取下程序预备态 */
#define LS_SET_CFG_READY() NU_Set_Events(& Event_2_CB, CPC_EVENT2_CFG_READY, NU_OR ) /* 设置配置预备态 */
#define LS_GET_CFG_READY(ActEventPtr) \
        NU_Retrieve_Events(& Event_2_CB,CPC_EVENT2_CFG_READY,NU_AND,ActEventPtr,NU_NO_SUSPEND) /* 获取配置预备态 */
#define LS_CLR_CFG_READY() \
        NU_Set_Events(& Event_2_CB,(UNSIGNED)(~CPC_EVENT2_CFG_READY),NU_AND); /* 清除配置预备态 */
#define LS_SET_CFG_PROGRAM() NU_Set_Events(& Event_2_CB, CPC_EVENT2_CFG_PROGRAM, NU_OR ) /* 设置配置固化状态 */
#define LS_GET_CFG_PROGRAM(ActEventPtr) \
        NU_Retrieve_Events(& Event_2_CB,CPC_EVENT2_CFG_PROGRAM,NU_AND,ActEventPtr,NU_NO_SUSPEND) /* 获取配置固化状态 */
#define LS_CLR_CFG_PROGRAM() \
        NU_Set_Events(& Event_2_CB,(UNSIGNED)(~CPC_EVENT2_CFG_PROGRAM),NU_AND); /* 清除配置固化状态 */
#define LS_GET_CFG_ERROR(ActEventPtr) \
        NU_Retrieve_Events(& Event_2_CB,CPC_EVENT2_CFG_ERROR,NU_AND,ActEventPtr,NU_NO_SUSPEND) /* 获取固化错误状态 */
#define LS_SET_CLR_REPORT() CRMC_ClearReport(1)                                    /* 设置清除报告状态 */
#define LS_SET_CLR_WAVE() NU_Set_Events(& Event_2_CB, CPC_EVENT2_WAVE_CLR, NU_OR ) /* 设置清除录波状态 */
#define LS_SET_RESET() NU_Set_Events(& Event_2_CB, CPC_EVENT2_RESET, NU_OR ) /* 设置复归信号状态 */

#define LS_RAM_START_ADDR               0            /* ram芯片起始地址 */
#define LS_RAM_END_ADDR                 0x04000000   /* ram芯片结束地址 */
/* 常量定义 */
#define LS_CURRENT_COEFFICIENT          1.2                               /* 校正电流通道的额定值倍数 */
#define LS_VOLTAGE_NORMVALUE            40.0                              /* 校正电压通道的电压值 */
#define LS_MIN_COEFFICIENT              0.94                              /* 通道最小校正系数 */
#define LS_MAX_COEFFICIENT              1.06                              /* 通道最大校正系数 */
#define LS_MIN_EXCURSION                -200.0                            /* 通道最小零漂值 */
#define LS_MAX_EXCURSION                200.0                             /* 通道最大零漂值 */
#define LS_MIN_PHASE                    -10.0                             /* 通道最小校正相位 */
#define LS_MAX_PHASE                    10.0                              /* 通道最大校正相位 */
#define LS_FLOAT_ERROR                  0.001                             /* 最大浮点误差 */
#define LS_FEEQ_SELECT                  0x40                              /* 频率精度选择位 */
#define LS_EXCHANGE_START               0                                 /* 启动状态在交换数据中的位置偏移 */
#define LS_EXCHANGE_FAULT_NUMBER        1                                 /* 故障序号在交换数据中的位置偏移 */
#define LS_RESULT_TRUE                  0x5a                              /* 操作成功 */
#define LS_RESULT_NULL                  0x00                              /* 操作失败 */
#define LS_MAX_CONFIG_BYTE              0xff                              /* 最大配置数据字节数 */

/* 超时时间 */
#define LS_REPORT_TIMEOUT               100                               /* 报告操作超时时间 */
#define LS_WAVE_TIMEOUT                 100                               /* 录波超作超时时间 */
#define LS_EVENT_TIMEOUT                100                               /* 事件操作超时时间 */
#define LS_TIME_OUT                     800                               /* 数据修改超时时间 */

/* 清除记录数据类型定义.  */
#define LS_TYPE_CLEAR_REPORT            0x1                               /* 清除报告 */
#define LS_TYPE_CLEAR_WAVE              0x2                               /* 清除录波 */

/* TB_CFB结构中每个条件的基本信息字节数(条件字段索引(2)+条件字段值(4)+条件类型(1)) */
#define TB_CFB_COND_BYTE_NUMBER         7

/* 传动出口类型定义 */
#define LS_START_RELAY_TEST             0x01                              /* 传动启动继电器 */
#define LS_TRIP_RELAY_TEST              0x02                              /* 传动跳闸矩阵 */
#define LS_OUT_RELAY_TEST               0x04                              /* 传动出口位 */
/* 内部事件类型定义 */
#ifndef XJ_INTERIOR_EVENTBITS
#define XJ_INTERIOR_EVENTBITS
#define LS_RES_EVENT_START              1                                 /* 传动启动继电器 */
#define LS_RES_EVENT_TRIP               2                                 /* 传动跳闸矩阵 */
#define LS_RES_EVENT_OUT                3                                 /* 传动出口 */
#define LS_RES_EVENT_REPAIR             4                                 /* 数据自检出错 */
#define LS_RES_EVENT_EEPROM             5                                 /* 写EEPROM出错 */
#define LS_RES_EVENT_FLASH              6                                 /* 写FLASH出错 */
#define LS_RES_EVENT_ERASE              7                                 /* FLASH扇区擦除出错 */
#define LS_RES_EVENT_SIZE               8                                 /* 数据大小出错 */
#define LS_RES_EVENT_RESTART            9                                 /* 装置重新上电 */
#define LS_RES_EVENT_RELAY              10                                /* 继电器动作 */
#define LS_RES_EVENT_CLOSE              11                                /* 继电器瞬时击穿 */
#define LS_RES_EVENT_CLOSE_EXT          12                                /* 扩展回路瞬时击穿 */
#define LS_RES_EVENT_REPORT_CLR         13                                /* 清除报告 */
#define LS_RES_EVENT_WAVE_CLR           14                                /* 清除录波 */
#define LS_RES_EVENT_ZONE_CHANGE        15                                /* 定值区变化 */
#define LS_RES_EVENT_CONFIG_CHANGE      16                                /* 配置变化 */
#define LS_RES_EVENT_GETIN_OVERTIME     17                                /* 接收开入量超时 */
#endif
/*=======================================================================*/
/* 命令投退                                                              */
/*=======================================================================*/
#define ON                              1                                 /* 投入 */
#define OFF                             0                                 /* 退出 */
#define SWITCH_IEC103_TI_M_81           ON                                /* 召唤装置基本信息 */
#define SWITCH_IEC103_TI_M_82           ON                                /* 装置呼叫命令 */
#define SWITCH_IEC103_TI_M_90           ON                                /* 进入下传程序预备状态 */
#define SWITCH_IEC103_TI_M_91           OFF                               /* 进入下传程序状态 */
#define SWITCH_IEC103_TI_M_96           ON                                /* 预发固化保护配置 */
#define SWITCH_IEC103_TI_M_97           ON                                /* 执行固化保护配置 */
#define SWITCH_IEC103_TI_M_98           ON                                /* 查询保护配置固化结果 */
#define SWITCH_IEC103_TI_M_A0           ON                                /* 召唤数据表的总个数 */
#define SWITCH_IEC103_TI_M_A1           ON                                /* 召唤数据表的信息 */
#define SWITCH_IEC103_TI_M_A2           ON                                /* 召唤字段信息 */
#define SWITCH_IEC103_TI_M_A3           ON                                /* 召唤表中数据的总个数 */
#define SWITCH_IEC103_TI_M_A4           ON                                /* 召唤记录的全部字段 */
#define SWITCH_IEC103_TI_M_A5           ON                                /* 召唤记录的单个字段 */
#define SWITCH_IEC103_TI_M_A6           ON                                /* 召唤记录的多个字段 */
#define SWITCH_IEC103_TI_M_A7           ON                                /* 设置配置字段的值 */
#define SWITCH_IEC103_TI_M_A8           ON                                /* 选择编辑字段 */
#define SWITCH_IEC103_TI_M_A9           ON                                /* 设置编辑定值组的值 */
#define SWITCH_IEC103_TI_M_AA           ON                                /* 确认编辑定值组的值 */
#define SWITCH_IEC103_TI_M_AB           ON                                /* 读取数据表的状态 */
#define SWITCH_IEC103_TI_M_AC           ON                                /* 读取数据表的配置CRC码 */
#define SWITCH_IEC103_TI_M_AD           ON                                /* 冻结数据表的数据 */
#define SWITCH_IEC103_TI_M_AE           ON                                /* 解冻数据表的数据 */
#define SWITCH_IEC103_TI_M_AF           ON                                /* 解冻数据表的数据 */
#define SWITCH_IEC103_TI_M_B0           ON                                /* 保存多个配置字段值 */
#define SWITCH_IEC103_TI_M_C0           ON                                /* 召唤事件类型 */
#define SWITCH_IEC103_TI_M_C1           ON                                /* 召唤事件信息 */
#define SWITCH_IEC103_TI_M_C2           ON                                /* 召唤事件索引号 */
#define SWITCH_IEC103_TI_M_C3           ON                                /* 召唤事件数据 */
#define SWITCH_IEC103_TI_M_C4           ON                                /* 清除事件数据 */
#define SWITCH_IEC103_TI_M_C5           ON                                /* 召唤报告信息 */
#define SWITCH_IEC103_TI_M_C6           ON                                /* 召唤报告故障序号 */
#define SWITCH_IEC103_TI_M_C7           ON                                /* 召唤报告索引序号 */
#define SWITCH_IEC103_TI_M_C8           ON                                /* 召唤报告数据 */
#define SWITCH_IEC103_TI_M_C9           ON                                /* 清除故障记录数据 */
#define SWITCH_IEC103_TI_M_CA           ON                                /* 手动启动录波 */
#define SWITCH_IEC103_TI_M_CB           ON                                /* 召唤录波序号 */
#define SWITCH_IEC103_TI_M_CC           ON                                /* 召唤录波时间 */
#define SWITCH_IEC103_TI_M_CD           ON                                /* 召唤录波信息 */
#define SWITCH_IEC103_TI_M_CE           ON                                /* 召唤录波电网故障序号 */
#define SWITCH_IEC103_TI_M_E0           ON                                /* 预发传动开出 */
#define SWITCH_IEC103_TI_M_E1           ON                                /* 执行传动开出 */
#define SWITCH_IEC103_TI_M_E2           ON                                /* 调通道系数及量值 */
#define SWITCH_IEC103_TI_M_E3           ON                                /* 修改通道系数 */
#define SWITCH_IEC103_TI_M_E4           ON                                /* 调通道额定值 */
#define SWITCH_IEC103_TI_M_E5           ON                                /* 修改通道额定值 */
#define SWITCH_IEC103_TI_M_E6           ON                                /* 调装置零漂刻度及零漂值 */
#define SWITCH_IEC103_TI_M_E7           ON                                /* 修改装置零漂 */
#define SWITCH_IEC103_TI_M_E8           OFF                               /* 上调通道的当前相位误差（应用程序注册）*/
#define SWITCH_IEC103_TI_M_E9           ON                                /* 修改通道相位误差 */
#define SWITCH_IEC103_TI_M_EA           ON                                /* 上调模拟通道相位误差定值 */
#define SWITCH_IEC103_TI_M_F0           ON                                /* 信号复归 */
#define SWITCH_IEC103_TI_M_F1           ON                                /* 召唤开入量状态及GPS脉冲计数 */
#define SWITCH_IEC103_TI_M_F7           ON                                /* 读内存数据 */

#endif
