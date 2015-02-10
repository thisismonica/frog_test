
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
/*      lc_defs.h                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      LC - Lonworks Communication                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains data structure definitions and constants for  */
/*      the Lonworks Communication component.                            */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      Chenyufeng, XJ ELECTRIC CO.LTD.                                  */
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
/*      YuFengCheng     09-01-2002      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

/**********************************************************************************/
/*                                                                                */
/*      缩写符          中文名称                英文名称                          */
/*      ADSU            应用数据服务单元        Application Service data Unit     */
/*      APCI            应用规约控制信息        Application Protocol control      */
/*                                              Information                       */
/*      COM             命令                    Command                           */
/*      COT             传送原因                Cause Of Transmission             */
/*      CU              通信单元                Communication Unit                */
/*      CP32Time2a      4个8位位组时间          Four Object Binary Time           */
/*      CP56Time2a      7个8位位组时间          Seven Object Binary Time          */
/*      DCO             双命令                  Double Command                    */
/*      DFC             数据流控制              Data flow control                 */
/*      DPI             双点信息                Double-Point Information          */
/*      DR              差错                    Error                             */
/*      FAN             故障序号                Fault Number                      */
/*      F-Code          功能码                  Function code                     */
/*      FCB             帧计数位                Frame Count Bit                   */
/*      FCV             帧计数有效位            Frame Count Bit Valid             */
/*      FT              帧传输格式              Frame Transmission Format         */
/*      FUN             功能类型                FUNCTTION TYPE                    */
/*      GDD             通用分类数据描述        Generic data description          */
/*      GEN             通用分类功能类型        Generic function type             */
/*      GGI             通用分类数据总          Generic interrogation of          */
/*                      查询(总召唤)            generic data                      */
/*      GI              总查询(总召唤)          Generic interrogation             */
/*      IEC             国际电工委员会          International Electrotechnical    */
/*                                              Commission                        */
/*      IED             智能电子装置            Intrlligent Electronic Device     */
/*      INF             信息符号                Information Number                */
/*      IV              无效                    Invalid                           */
/*      LPCI            链路规约控制信息        Link Protocol Control             */
/*                                              Information                       */
/*      LPDU            链路规约数据单元        Link Protocol Data Unit           */
/*      MEA             带品质描述词的          Measurand with quality            */
/*                      被测值                  descriptor                        */
/*      MVAL            被测值的值              Value of measurand                */
/*      NO              数目                    Number                            */
/*      NOC             通道数目                Number of Channels                */
/*      OV              溢出                    Overflow                          */
/*      P               有功功率                Active Power                      */
/*      PRM             启动报文                Primary Message                   */
/*      Q               无功功率                Reactive Power                    */
/*      RES             备用                    Reserved                          */
/*      RET             相对时间                Relative time                     */
/*      RII             返回信息标识符          Return Information Identifier     */
/*      SCN             扫描序号                Scan Number                       */
/*      SIN             附加信息                Supplementary Information         */
/*      SU              夏季标志位              Summer Bit                        */
/*      SQ              同类信息元素            Sequence of equal information     */
/*                      的顺序                  elements                          */
/*      TOO             命令类型                Type Of Order                     */
/*      TYP             类型标识                Type Identification               */
/*                                                                                */
/**********************************************************************************/
#include <string.h>
#include "plat/inc/xj_cosc.h"

/* Check to see if the file has been included already.  */
#ifndef LC_DEFS_H
#define LC_DEFS_H

#include "plat/inc/pro_defs.h"
#include "plat/net_inc/xjdlist.h"
#include "plat/net_inc/ieddb.h"
#include "plat/net_inc/commu.h"
#include "plat/inc/tab_defs.h"
#include "plat/inc/esp_cfg.h"
#include "plat_specific/plat_specific.h"
#include "plat_specific/inc/cpc_defs.h"
#include "plat/es_plat.h"

/* 外部变量 */
extern ESP_CFG PLAT_cfg;
extern NU_PROTECT * pReport_Protect;  /* 动作报告数据保护 */
#define   DEST_TYPE_REMOTE      0x00
#define LC_CHANNEL_COMMAND      256   /* 一个通讯通道支持的命令个数 */
#define LC_VAR_CONTROL_BITS     0x0F  /* 控制域有效位数 */
#define LC_SETTING_WAIT_TIMEOUT 20000 /* 处理定值的最大等待时间 */
#define LC_MAX_BLOCKIO_CONTEXT  10    /* 节点数据的最大值 */
#define LC_BLOCK_QUE_SIZE       64    /* 需要在后台任务中处理的命令消息队列size */
#define LCC_PRIORITY_MAX        5     /* 数据处理优先方式最大个值 */
#define LC_STAT                 1     /* 支持数据统计编译命令选项 */
#define LC_WATCHDOG_TIME        20000 /* 看门狗复位延时时间(20秒) */
#define LC_COMMAND_FRAME_SIZE   255   /* 单个命令帧的缓冲区大小 */
#define LC_MAX_REPORT_SERIAL    80    /* 上送的最大报告索引号的个数 */
#define LC_MAX_EVENT_SERIAL     80    /* 上送的最大事件索引号的个数 */
#define LC_MAX_WAVE_SERIAL      80    /* 上送的最大录波索引号的个数 */
#define LC_VARIABLE_BASE_LENGTH 6     /* 可变帧报文基本长度 */

#define LC_GLOBAL_FUN           ((GW_UCHAR)((PLAT_cfg.device.DeviceFun) & 0xff))    /* 装置功能类型 */
#define LC_CPU_NUMBER           (PLAT_cfg.device.CpuNumber)                         /* 装置CPU号码 */
#define LC_FUN_SELECT           ((PLAT_cfg.device.FunSelect) & (FUN_PRO | FUN_FCK)) /* 功能选择 */
#define LC_FUN_WAVE             ((PLAT_cfg.device.FunSelect) & FUN_WAVE)            /* 录波功能 */
#define LC_FUN_REPORT           ((PLAT_cfg.device.FunSelect) & FUN_REPORT)          /* 报告功能 */
#define LC_FUN_BACKGROUND       ((PLAT_cfg.device.FunSelect) & FUN_BACKGROUND)      /* 背景数据功能 */
#define LC_FUN_DATAEXCHANGE     ((PLAT_cfg.device.FunSelect) & FUN_DATA_EXCHANGE)   /* 数据交换功能 */
#define LC_LINK_ADDRESS         ((GW_UCHAR)PLAT_cfg.device.CpuNumber)               /* 装置地址 */
#define LC_LINK_HMI_PHY_COM      CPC_LINK_HMI_PHY_COM                               /* 与HMI之间的通信端口设置 */
#define LC_GET_DEV_CRC           0                                                  /* 装置CRC码 */
#define LC_GET_DEVICE_ENAME      ((GET_DEV_TAB_MEMORY)->Device_EName)               /* 获取装置英文名称首址 */
#define LC_GET_RULES_SELECT      ((GET_DEV_TAB_MEMORY)->RulesSelect)                /* 获取规约选择 */
#define LC_REPORT_PROTECT()      NU_Protect( pReport_Protect )                      /* 报告数据保护 */
#define LC_REPORT_UNPROTECT()    NU_Unprotect()                                     /* 释放报告数据保护 */
extern UNSIGNED DEV_Float_To_IEEE( float fTMS );
#define LC_FLOAT_TO_IEEE(x)      DEV_Float_To_IEEE(x)                               /* 装置浮点转换成IEEE格式 */
#define LC_READ_BACKGROUND(buf,off,num,timeout) XJP_Background_Read(buf,off,num,timeout) /* 读背景数据 */
#define GW_LINK_ADDRESS         (PLAT_cfg.device.CpuNumber)                         /* 装置CPU号码 */

/* 端口号定义(与接口保持一致) */
#define LC_PORT_TYPE_REMOTE         4 /* 监控1 */
#define LC_PORT_TYPE_REMOTE2        5 /* 监控2 */
#define LC_PORT_TYPE_REMOTE3        6 /* 监控3 */
#define LC_PORT_TYPE_PC             3 /* PC */
#define LC_PORT_TYPE_MENU           1 /* 菜单 */
#define LC_PORT_TYPE_PRN            2 /* 打印 */

/* 录波报文偏移定义 */
#define LC_APP_INDEX_VSQ         0x01 /* 可变结构限定词偏移 */
#define LC_APP_INDEX_COT         0x02 /* 传输原因偏移 */
#define LC_APP_INDEX_SECT        0x03 /* 单元公共地址偏移 */
#define LC_APP_INDEX_FUN         0x04 /* 功能类型偏移 */
#define LC_APP_INDEX_INF         0x05 /* 信息序号偏移 */
#define LC_APP_INDEX_CONTEXT     0x06 /* 上下文数据偏移 */

/* 数据个数定义 */
#define LC_DM_MAX_COMMAND         180 /* 最大通讯命令个数 */
#define LC_DM_MAX_IO_CB           20  /* 节点最大个数 */

/* 节点优先级定义 */
#define LC_PRIORITY_VLDCFG       0x00 /* VLD配置数据优先级 */
#define LC_PRIORITY_VLDMENU      0x00 /* VLD菜单数据优先级 */
#define LC_PRIORITY_IO           0x02 /* 普通节点数据优先级 */
#define LC_PRIORITY_RC           0x03 /* 报告节点数据优先级 */
#define LC_PRIORITY_RW           0x05 /* 录波节点数据优先级 */

/* 超时时间宏定义 */
#define LC_TIMEOUT_TICK          300   /* 打包数据超时时间 */
#define LC_TIMEOUT_VLDCFG        5000  /* VLD配置数据创建节点超时时间 */
#define LC_TIMEOUT_VLDMENU       5000  /* VLD数据管理创建节点超时时间 */
#define LC_TIMEOUT_BLOCKIO       8000  /* 创建节点超时时间 */
#define LC_TIMEOUT_RW            30000 /* 录波中创建节点超时时间 */
#define LC_TIMEOUT_RC            20000 /* 录波中创建节点超时时间 */
#define LC_IEDDB_TIMEOUT         200   /* 实时库操作超时等待时间 */

#define I870FT12_BROADCAST_ADDRESS 0xff /* 广播装置地址 */
/* frame synchronization octets */
#define I870FT12_SYNC_OCTET_VARIABLE  0x68 /* 可变帧报文同步字 */
#define I870FT12_SYNC_OCTET_END       0x16 /* 报文结束同步字符 */
/* 可变帧报文偏移量定义 */
#define I870FT12_INDEX_VAR_SYNC_FIRST    0 /* 起始同步字偏移 */
#define I870FT12_INDEX_VAR_LENGTH        1 /* 长度偏移 */
#define I870FT12_INDEX_VAR_LENGTH_COPY   2 /* 第二长度偏移 */
#define I870FT12_INDEX_VAR_SYNC_SECOND   3 /* 第二同步字偏移 */
#define I870FT12_INDEX_VAR_CONTROL       4 /* 控制域偏移 */
#define I870FT12_INDEX_VAR_ADDRESS       5 /* 地址域偏移 */

/* bit masks for Data Link Header CONTROL byte */
#define I870FT12_CONTROL_DIR          0x80   /* Direction                   */
#define I870FT12_CONTROL_PRM          0x40   /* Primary Message             */
#define I870FT12_CONTROL_FCB          0x20   /* Frame Count Bit             */
#define I870FT12_CONTROL_FCV          0x10   /* Frame Count bit Valid       */
#define I870FT12_CONTROL_ACD          0x20   /* Access Demand for class 1   */

/* frames using link address sizes of 0, 1, and 2. */
#define I870FT12_INDEX_VAR_ADDR0_ASDU    5 /* ADDR0 */
#define I870FT12_INDEX_VAR_ADDR1_ASDU    6 /* ADDR1 */
#define I870FT12_INDEX_VAR_ADDR2_ASDU    7 /* ADDR2 */

/* In monitor direction */
#define I3DEF_TYPE_LINK_RESET   0 /* used for local link reset request          */
#define I3DEF_TYPE_DPI_TIME     1 /* time-tagged message                        */
#define I3DEF_TYPE_DPI_REL      2 /* time-tagged message with relative time     */
#define I3DEF_TYPE_MSRNDS_I     3 /* measurands I                               */
#define I3DEF_TYPE_MSRNDS_REL   4 /* time-tagged measurands with relative time  */
#define I3DEF_TYPE_IDENT        5 /* identification                             */
#define I3DEF_TYPE_TIME_SYNC    6 /* time synchronization   (control also)      */
#define I3DEF_TYPE_GI_TERM      8 /* general interrogation termination          */
#define I3DEF_TYPE_MSRNDS_II    9 /* measurands II                              */
#define I3DEF_TYPE_GNRC_DATA   10 /* generic data           (control also)      */
#define I3DEF_TYPE_GNRC_ID     11 /* generic identification                     */
#define I3DEF_TYPE_LST_DSTRB   23 /* list of recorded disturbances              */
#define I3DEF_TYPE_RDY_DDATA   26 /* ready for transmission of disturbance data */
#define I3DEF_TYPE_RDY_DCHAN   27 /* ready for transmission of a channel        */
#define I3DEF_TYPE_RDY_DTAGS   28 /* ready for transmission of tags             */
#define I3DEF_TYPE_TRNS_DTAG   29 /* transmission of tags                       */
#define I3DEF_TYPE_TRNS_DVAL   30 /* transmission of disturbance values         */
#define I3DEF_TYPE_END_TRANS   31 /* end of transmission                        */

#define I3DEF_TYPE_MMSRNDS_III 32 /* measurands III                             */
#define I3DEF_TYPE_MMSRNDS_IV  33 /* measurands IV                              */
#define I3DEF_TYPE_MMSRNDS_V   34 /* measurands V                               */
#define I3DEF_TYPE_MMSRNDS_VI  35 /* measurands VI                              */
#define I3DEF_TYPE_MIT_NA      36 /* counter no time-tag                        */
#define I3DEF_TYPE_MIT_TA      37 /* counter with time-tag                      */
#define I3DEF_TYPE_MST_NA      38 /* step no time-tag                           */
#define I3DEF_TYPE_MST_TA      39 /* step with time-tag                         */
#define I3DEF_TYPE_MSP_NA      40 /* single point no time-tag                   */
#define I3DEF_TYPE_MSP_TA      41 /* single point with time-tag                 */
#define I3DEF_TYPE_MDP_NA      42 /* double point no time-tag                   */
#define I3DEF_TYPE_MDP_TA      43 /* double point with time-tag                 */
#define I3DEF_TYPE_MSS_NA      44 /* single point state and change detect no time-tag   */
#define I3DEF_TYPE_MSS_TA      45 /* single point state and change detect with time-tag */
#define I3DEF_TYPE_MDS_NA      46 /* double point state and change detect no time-tag   */
#define I3DEF_TYPE_MDS_TA      47 /* double point state and change detect with time-tag */
#define I3DEF_TYPE_MWL_TA      48 /* water level with time-tag                  */
#define I3DEF_TYPE_MMSRNDS_VII 50 /* measurands VII                             */

/* In control direction */
#define I3DEF_TYPE_GI           7 /* general interrogation                      */
#define I3DEF_TYPE_GNRL_CMD    20 /* general command                            */
#define I3DEF_TYPE_GNRC_CMD    21 /* generic command                            */
#define I3DEF_TYPE_DIST_ORDR   24 /* order for disturbance data transmission    */
#define I3DEF_TYPE_DIST_ACK    25 /* Ack for disturbance data transmission      */

#define I3DEF_TYPE_DCC_CMD     64 /* breaker command                            */
#define I3DEF_TYPE_RCC_CMD     65 /* remote step command                        */
#define I3DEF_TYPE_SEC_CMD     66 /* set command                                */
#define I3DEF_TYPE_CCC_CMD     67 /* phase and voltage command                  */
#define I3DEF_TYPE_CCI_NA      88 /* counter interrogation                      */

/* xj special message */
#define I3DEF_TYPE_XJ_TI60     60 /* interrogation version information          */
#define I3DEF_TYPE_XJ_TI61     61 /* deal with settings                         */
#define I3DEF_TYPE_XJ_TI62     62 /* find and interrogation fault report        */
#define I3DEF_TYPE_XJ_TI70     70 /* fault report                               */
#define I3DEF_TYPE_XJ_TI73     73 /* protect device self-check & recorder       */
#define I3DEF_TYPE_XJ_TI74     74 /* mea and ctrl device self-check & recorder  */

#define I3DEF_TYPE_PRO_MMIVER  0x8B /* 上调保护cpu的LON网版本号 */
#define I3DEF_TYPE_MMI_LONVER  0x8C /* 上调接口cpu的LON网版本号 */
#define I3DEF_TYPE_STARTDT     0xBD /* 上调数据模型 */
#define I3DEF_TYPE_XJ_LOCK_DATA 127 /* request all data for lock*/

/* Cause Of Transmission in monitor and control direction */
#define I3DEF_COT_NOT_DEFINED           0 /* Not defined                              */
#define I3DEF_COT_SPONTANEOUS           1 /* spontaneous                              */
#define I3DEF_COT_CYCLIC                2 /* periodic, cyclic                         */
#define I3DEF_COT_RESET_FCB             3 /* reset frame count bit (FCB)              */
#define I3DEF_COT_RESET_CU              4 /* reset communication unit (CU)            */
#define I3DEF_COT_RESTART               5 /* start/restart                            */
#define I3DEF_COT_POWERON               6 /* power on                                 */
#define I3DEF_COT_TEST_MODE             7 /* test mode                                */
#define I3DEF_COT_TIME_SYNC             8 /* time synchronization  (control also)     */
#define I3DEF_COT_GEN_INTERROGATION     9 /* general interrogation (control also)     */
#define I3DEF_COT_TERMINATION_GI       10 /* termination of general interrogation     */
#define I3DEF_COT_LOCAL_OPERATION      11 /* local operation                          */
#define I3DEF_COT_REMOTE_OPERATION     12 /* remote operation                         */
#define I3DEF_COT_GENERAL_COMMAND      20 /* general command                          */
#define I3DEF_COT_POS_COMMAND_ACK      20 /* positive acknowledgement of command      */
#define I3DEF_COT_NEG_COMMAND_ACK      21 /* negative acknowledgement of command      */
#define I3DEF_COT_DISTURBANCE_DATA     31 /* transmission of disturbance data         */
#define I3DEF_COT_GENERIC_WRITE        40 /* generic write command or positive ACK    */
#define I3DEF_COT_NEG_GENERIC_ACK      41 /* negative ACK of generic write command    */
#define I3DEF_COT_GENERIC_READ         42 /* generic read command or valid response   */
#define I3DEF_COT_INVALID_GENERIC_RD   43 /* invalid response to generic read command */
#define I3DEF_COT_CONF_GENERIC_WRITE   44 /* generic write confirmation               */
//xj self-defined
#define I3DEF_COT_VERSION_REPORT_INTG  64 /* interrogation version information        */
#define I3DEF_COT_NEG_COUNTER_ACK      66 /* negative ACK of counter interrogation    */

/* 录波命令类型宏定义 */
#define I3DEF_TOO_SELECTION_OF_FAULT    1 /* 启动故障录波数据上送命令 */
#define I3DEF_TOO_REQUEST_OF_DISTURB    2 /* 请求发送数据 */
#define I3DEF_TOO_ABORT_OF_DISTURB      3 /* 退出 */
#define I3DEF_TOO_REQUEST_FOR_CHNL      8 /* 发送 */
#define I3DEF_TOO_ABORT_OF_CHNL         9 /* 退出本路数据发送 */
#define I3DEF_TOO_REQUEST_FOR_TAGS     16 /* 请求发送状态量 */
#define I3DEF_TOO_ABORT_OF_TAGS        17 /* 终止发送状态量 */
#define I3DEF_TOO_REQUEST_FOR_LOGS     18 /* 备用,调包括逻辑量的开关量 */
#define I3DEF_TOO_ABORT_OF_LOGS        19 /* 备用,放弃包括逻辑量的开关量 */
#define I3DEF_TOO_REQ_DISTURB_LIST     24 /* 启动故障录波数据上送命令 */
#define I3DEF_TOO_END_DATA_NO_ABORT    32 /* 故障录波数据发送结束报文 */
#define I3DEF_TOO_END_DATA_BY_CTRL     33 /* 故障录波数据发送中途退出认可报文 */
#define I3DEF_TOO_END_DATA_BY_EQPMNT   34 /* 故障录波数据发送中途退出报文 */
#define I3DEF_TOO_END_CHNL_NO_ABORT    35 /* 本路模拟量发送结束 */
#define I3DEF_TOO_END_CHNL_BY_CTRL     36 /* 故障录波数据发送中途退出认可报文 */
#define I3DEF_TOO_END_CHNL_BY_EQPMNT   37 /* 退出本路模拟量发送 */
#define I3DEF_TOO_END_TAGS_NO_ABORT    38 /* 状态量传送结束 */
#define I3DEF_TOO_END_TAGS_BY_CTRL     39 /* 调压开关升降状态变位 */
#define I3DEF_TOO_END_TAGS_BY_EQPMNT   40 /* 退出状态量传送 */
#define I3DEF_TOO_END_LOGS_NO_ABORT    41 /* 备用,包括逻辑量的开关量结束 */
#define I3DEF_TOO_END_LOGS_BY_CTRL     42 /* 备用,包括逻辑量的开关量结束由控制站 */
#define I3DEF_TOO_END_LOGS_BY_EQPMNT   43 /* 备用,包括逻辑量的开关量结束又中途退出 */
#define I3DEF_TOO_DISTURB_POS_ACK      64 /* 录波数据发送成功 */
#define I3DEF_TOO_DISTURB_NEG_ACK      65 /* 录波数据发送失败 */
#define I3DEF_TOO_CHNL_POS_ACK         66 /* 本路模拟量发送成功 */
#define I3DEF_TOO_CHNL_NEG_ACK         67 /* 本路模拟量发送失败 */
#define I3DEF_TOO_TAGS_POS_ACK         68 /* 状态量传送成功 */
#define I3DEF_TOO_TAGS_NEG_ACK         69 /* 状态量传送失败 */
#define I3DEF_TOO_LOGS_POS_ACK         70 /* 备用,肯定确认包括逻辑量的开关量 */
#define I3DEF_TOO_LOGS_NEG_ACK         71 /* 备用,否定确认包括逻辑量的开关量 */

/* 上调数据模板标识 */
#define LC_START_DT              0x55        /* 上调数据模板启动标志 */
#define LC_STOP_DT               0xAA        /* 上调数据模板停止标志 */
#define LC_STARTDT_SHORT_TIMEOUT (15*1000)   /* 上调数据模板初始超时时间 */
#define LC_STARTDT_LONG_TIMEOUT  (5*60*1000) /* 上调数据模板接收后的超时时间 */

/* 0xB7命令回复结果 */
#define LC_CMD_RET_EMPTY        0x00 /* 无数据回答 */
#define LC_CMD_RET_UNDEF        0x5A /* 表示不支持此命令 */
#define LC_CMD_RET_BUSY         0xAA /* 应用程序忙 */

/* 命令报文偏移量(103规约) */
#define LCC_OFFSET_LEN          0    /* 报文长度偏移 */
#define LCC_OFFSET_TI           1    /* 类别标识偏移 */
#define LCC_OFFSET_VSQ          2    /* 可变结构限定词偏移 */
#define LCC_OFFSET_COT          3    /* 传输原因偏移 */
#define LCC_OFFSET_SECT         4    /* 单元公共地址偏移 */
#define LCC_OFFSET_FUN          5    /* 功能类型偏移 */
#define LCC_OFFSET_INF          6    /* 信息序号偏移 */
#define LCC_OFFSET_CONTEXT      7    /* 上下文数据偏移 */

/* 命令报文长度定义 */
#define LCC_LEN_ASDU5           19   /* ASDU_5报文长度 */
#define LCC_LEN_ASDU6           13   /* ASDU_6报文长度 */

/* 数据缓冲区size定义 */
#define LC_LINK_SIZE            8    /* 通讯基本长度(帧头(4=同步字符(2)+长度(2))+校验和+结束符+控制域+地址域) */
#define LC_REQUEST_SEQUENCE_ANY 255  /* 通道优先级标识数 */
#define LC_CHANNEL_QUEUE_SIZE   300  /* 中/低压使用read及write消息队列缓冲区size */
#define LC_MAX_CHANNEL          1    /* 通讯通道最大个数 */
#define LC_MAX_FRAME_SIZE       252  /* 一帧报文的最大长度 */
#define LC_MAX_FRAME_DATA_SIZE  (LC_MAX_FRAME_SIZE - 10)  /* 一帧数据的最大长度 */
#if ( LC_LINK_HMI_PHY_COM )
#define LC_PRIMARY_SIZE         2048 /* First消息队列大小 */
#define LC_SECOND_SIZE          2048 /* Second消息队列大小 */
#else
#define LC_MAX_MSG_SIZE         ((LC_MAX_FRAME_SIZE+sizeof(UNSIGNED)-1)/sizeof(UNSIGNED)) /* 中/低压使用read及write消息队列缓冲区的最大值 */
#define LC_PRIMARY_SIZE         512  /* First消息队列大小 */
#define LC_SECOND_SIZE          512  /* Second消息队列大小 */
#endif

/* 数据拆分为2个字节 */
#define BYTE_PTR_TO_16(p) ((GW_USHORT)(((*((p)+1) & 0xff) << 8) | (*(p) & 0xff)))
/* 数据拆分为3个字节 */
#define BYTE_PTR_TO_24(p) ((GW_ULONG)(((*((p)+2) & 0xff) << 16) | ((*((p)+1) & 0xff) << 8) | (*(p) & 0xff)))
/* 数据拆分为4个字节 */
#define BYTE_PTR_TO_32(p) ((GW_ULONG)(((*((p)+3) & 0xff) << 24) | ((*((p)+2) & 0xff) << 16) \
| ((*(( p )+ 1 ) & 0xff ) << 8 ) | (*( p ) & 0xff ))) /* 数据拆分为4个字节 */

struct STRUCT_CHANNEL_CB;

typedef struct STRUCT_GW_COMMAND_CALLBACK_NODE
{
    GWAPP_ACK_FUNCTION  Callback;
#if     LC_STAT
    UNSIGNED       call_count;
#endif
}GW_COMMAND_CALLBACK_NODE;

typedef struct STRUCT_CHANNEL_CB
{
#if LC_STAT
    UNSIGNED        stat_select_last_tick[LCC_PRIORITY_MAX];
    UNSIGNED        stat_select_count[LCC_PRIORITY_MAX];
#endif

    UNSIGNED        chl_ID;
    INT             chl_Open;
    GW_BOOL         chl_initialize_finish;
    GW_ULONG        chl_initialize_start;
    GW_ULONG        chl_initialize_timeout;
    UNSIGNED_CHAR   chl_FCB;
    GW_ULONG        chl_acd_times;

    /*调VLD数据的顺序号*/
    GW_ULONG        chl_vld_rii;

    GW_ULONG        chl_setting_start;
    GW_ULONG        chl_setting_flag;
    GW_UCHAR        chl_setting_zone;
    GW_ULONG        chl_rx_reset_tick;
    GW_ULONG        chl_rx_ok_tick;
    GW_ULONG        chl_idle_start;
    GW_BOOL         chl_rx_ok;
    GW_ULONG        chl_reset;
    GW_ULONG        chl_dataexchange_sequence;
    XJ_DLIST        chl_GNRC_GroupList;
    XJ_DLIST        chl_GNRC_WriteSuspendList;
    GW_COMMAND_CALLBACK_NODE chl_callback_List[LC_CHANNEL_COMMAND];
    XJ_DLIST        chl_suspend_List;
    INT             chl_disturb_transmit;
    INT             chl_disturb_check;
    UNSIGNED        chl_disturb_order_start;
    UNSIGNED        chl_faultNumber;
    UNSIGNED        chl_disturb_table[CPC_MAX_NVRAM_WAVE_NUMBER];
    UNSIGNED        chl_Asdu70_Serial;
    /*发送数据区*/
    GW_UCHAR        chl_tx_buff[256];
    /*接收数据区*/
    GW_UCHAR        chl_rx_buff[256];
    GW_USHORT       chl_temp_size;
    /*中间报文区*/
    GW_UCHAR        chl_temp_buff[256];
    GW_UCHAR        chl_request_sequence;
    NU_QUEUE        chl_block_rx_que;
    NU_QUEUE        chl_block_tx_que;
    UNSIGNED        chl_block_rx_buffer[LC_BLOCK_QUE_SIZE];
    UNSIGNED        chl_block_tx_buffer[LC_BLOCK_QUE_SIZE];
#if (! LC_LINK_HMI_PHY_COM)
    NU_QUEUE        chl_mRead_que;
    NU_QUEUE        chl_mWrite_que;
    UNSIGNED        chl_mRead_Buff[LC_CHANNEL_QUEUE_SIZE];
    UNSIGNED        chl_mWrite_Buff[LC_CHANNEL_QUEUE_SIZE];
#endif
}GW_CHANNEL;

typedef INT  (* GW_CB_ACK )( UNSIGNED_CHAR * dst, GW_CHANNEL * pChl, VOID    * pCB );
typedef INT  (* GW_CB_INIT )( VOID * pCB, VOID * temp );
typedef INT  (* GW_CB_FREE )( VOID * pCB );
typedef struct  STRUCT_GW_REQUEST_CB
{
    XJ_NODE             node;
    GW_UCHAR            sesn;
    GW_UCHAR            sect;
    GW_ULONG            timeout;
    GW_ULONG            start_timeout;
    GW_UCHAR            dest_port;
    GW_UCHAR            priority;
    GW_CB_ACK           ack_callback;
    GW_UCHAR            entry;
}GW_REQUEST_CB;

typedef struct  STRUCT_GW_BLOCK_IO_CB
{
    GW_REQUEST_CB       head;
    GW_UCHAR            data[LC_MAX_BLOCKIO_CONTEXT];
}GW_BLOCK_IO_CB;

/* 固定帧长消息队列(chl_block_rx_que/chl_block_tx_que)的大小 */
#define BLOCK_MSG_SIZE   ((sizeof(LC_BLOCK_CB)+sizeof(UNSIGNED)-1)/sizeof(UNSIGNED))

/*动态内存管理操作接口数据结构和操作*/
typedef void * (* LC_MemAlloc )( size_t );
typedef void (* LC_MemFree )( void *);
struct LCMemMService
{
    /* 申请内存 */
    LC_MemAlloc MemAlloc;
    /* 释放内存 */
    LC_MemFree MemFree;
};

/* 通讯错误信息记录 */
typedef struct  STRUCT_LCC_COM_ERR
{
    UNSIGNED HMISendErr;    /* 接口发送报文错误 */
    UNSIGNED PlatSendErr;   /* 平台发送报文错误 */
    UNSIGNED CheckFrameErr; /* 接收报文检测时发现报文不正确 */
}LCC_COM_ERR;

extern LCC_COM_ERR LCC_ComErr;
#endif
