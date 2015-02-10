
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
/*      commu.h                                         Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      commu - communication Management                                 */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains data structure definitions and constants for  */
/*      the communication Management component.                          */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      XiangAnZhang, XJ ELECTRIC CO.LTD.                                */
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
/*      XiangAnZhang    09-10-2002      Created initial version 1.0      */
/*      ZhiDeYang       03-20-2003      Created Verified version 1.0     */
/*                                                                       */
/*************************************************************************/

#ifndef COMMU_H
#define COMMU_H

//#include "cfgc.h"
/* Define constants local to this component.  */

#define Start_Code1             0x10            /* 103 protocol start code1 */
#define Start_Code2             0x68            /* 103 protocol start code2 */
#define Start_Code3             0x88            /* euronID identify */
#define Start_Code4             0x99            /* between lonworks exchange data */
#define Start_Code5             0xAA            /* betweeen protect cpu exchange data */

#define Reset_FCB               0x00000001      /* reset FCB */
#define Reset_CU                0x00000002      /* reset CU */
#define Reset_Device            0x00000004      /* reset device */
#define Modify_Time             0x00000008      /* modify time */
#define Sets_Parameter          0x00000010      /* Sets_Parameter */
#define Gen_Inspect_Start       0x00000020      /* General Inspect Start */
#define Gen_Inspect_End         0x00000040      /* General Inspect end */

#define Warning_Signals         0x00000080      /* warning singals */
#define Event_Signals           0x00000100      /* event signals */
#define Request_Setting         0x00000200      /* load setting */
#define Send_All_Setting        0x00000400      /* All setting modify prepare send */
#define Return_Version          0x00000800      /* load version */
#define Replay_No_Setting       0x00001000      /* replay no setting */
#define Req_Exe_Setting         0x00002000      /* all settings modify execuate */
#define Sure_Sett_Reply         0x00004000      /* all settings execuate sure */
#define Req_Esc_Setting         0x00008000      /* all settings execuate cancel */
#define Send_Single_Sett        0x00010000      /* single setting modify prepare send */
#define Single_Setting          0x00020000      /* ? */
#define Req_Exe_SinglSett       0x00040000      /* single settings modify execuate */
#define Req_Esc_SinglSett       0x00080000      /* all settings execuate cancel */

#define Req_Fault_Report        0x00100000      /* 调故障报告 */
#define Command_Set             0x00200000      /* 一般命令 */
#define Command_Status          0x00400000      /* 命令后的状态 */
#define Command_Reply           0x00800000      /* 命令的认可 */

#define End_Code                0x16            /* 103 protocol end code */
#define _FCV                    0x10            /* 103 protocol FCV bit */
#define _FCB                    0x20            /* 103 protocol FCB bit */
#define _PRM                    0x40            /* 103 protocol PRM bit */
#define _DFC                    0x10            /* 103 protocol DFC bit */
#define _ACD                    0x20            /* 103 protocol ACD bit */

/*---------------------IEC60870-5-103-----------------------------------*/

#define RESETCU                 0               /* reset CU */
#define SENDCON                 3               /* send confirm frame */
#define SENDNOCON               4               /* send no confirm frame */
#define RESETFCB                7               /* reset FCB */
#define REQUIRELINK             9               /* require link status */
#define REQUIREFIRSTDATA        10              /* require first data */
#define REQUIRESECONDDATA       11              /* require second data */

#define Add_Num_Boardcase       0xff            /* 255 */

#define IEC103_TI_M_87          0x87            // load whole hard jumper status
#define IEC103_TI_M_88          0x88            // load whole trip status
#define IEC103_TI_M_8D          0x8d            // load protect analog ID
#define IEC103_TI_M_8E          0x8e            // load protect analog
#define IEC103_TI_M_8F          0x8f            // change protect analog number
#define IEC103_TI_M_92          0x92            // start program
#define IEC103_TI_M_94          0x94            // save program
#define IEC103_TI_M_99          0x99            // change system setting value
#define IEC103_TI_M_9A          0x9a            // load cpu setting ID
#define IEC103_TI_M_9B          0x9b            // load cpu setting value
#define IEC103_TI_M_9C          0x9c            // load system setting ID
#define IEC103_TI_M_9D          0x9d            // load system setting value
#define IEC103_TI_M_9E          0x9e            // load setting zone
#define IEC103_TI_M_9F          0x9f            // change setting zone

/* 装置基本信息 */
#define IEC103_TI_M_80          0x80            // 通信打包数据格式
#define IEC103_TI_M_81          0x81            // 召唤装置基本信息
#define IEC103_TI_M_82          0x82            // 设置对时类型
#define IEC103_TI_M_83          0x83            // 下发装置检测信息
#define IEC103_TI_M_84          0x84            // 上载VLD数据帧数和CRC码
#define IEC103_TI_M_85          0x85            // 上载VLD数据
#define IEC103_TI_M_86          0x86            // 上载背景数据

/* 程序下载与配置通信命令 */
#define IEC103_TI_M_90          0x90            // 进入下传程序预备状态
#define IEC103_TI_M_91          0x91            // 进入下传程序状态
#define IEC103_TI_M_96          0x96            // 预发固化保护配置
#define IEC103_TI_M_97          0x97            // 执行固化保护配置
#define IEC103_TI_M_98          0x98            // 查询保护配置固化结果

/* 数据表内部命令 */
#define IEC103_TI_M_A0          0xa0            // 召唤数据表的总个数
#define IEC103_TI_M_A1          0xa1            // 召唤数据表的信息
#define IEC103_TI_M_A2          0xa2            // 召唤字段信息
#define IEC103_TI_M_A3          0xa3            // 召唤表中数据的总个数
#define IEC103_TI_M_A4          0xa4            // 召唤记录的全部字段
#define IEC103_TI_M_A5          0xa5            // 召唤记录的单个字段
#define IEC103_TI_M_A6          0xa6            // 召唤记录的多个字段
#define IEC103_TI_M_A7          0xa7            // 设置配置字段的值
#define IEC103_TI_M_A8          0xa8            // 选择编辑字段
#define IEC103_TI_M_A9          0xa9            // 设置编辑定值组的值
#define IEC103_TI_M_AA          0xaa            // 确认编辑定值组的值
#define IEC103_TI_M_AB          0xab            // 读取数据表的状态
#define IEC103_TI_M_AC          0xac            // 读取数据表的配置CRC码
#define IEC103_TI_M_AD          0xad            // 冻结数据表的数据
#define IEC103_TI_M_AE          0xae            // 解冻数据表的数据
#define IEC103_TI_M_AF          0xaf            // 设置多个配置字段的值
#define IEC103_TI_M_B0          0xb0            // 保存多个配置字段值
#define IEC103_TI_M_B1          0xb1            // change trip status
#define IEC103_TI_M_B2          0xb2            // change trip name
#define IEC103_TI_M_B3          0xb3            // load input ID
#define IEC103_TI_M_B4          0xb4            // load output ID
#define IEC103_TI_M_B5          0xb5            // load error status
#define IEC103_TI_M_B6          0xb6            // load report event
#define IEC103_TI_M_B7          0xb7            // nonsupport command

/* 故障数据通信命令 */
#define IEC103_TI_M_C0          0xc0            /* 召唤事件类型 */
#define IEC103_TI_M_C1          0xc1            /* 召唤事件信息 */
#define IEC103_TI_M_C2          0xc2            /* 召唤事件索引号 */
#define IEC103_TI_M_C3          0xc3            /* 召唤事件数据 */
#define IEC103_TI_M_C4          0xc4            /* 清除事件数据 */

#define IEC103_TI_M_C5          0xc5            /* 召唤报告信息 */
#define IEC103_TI_M_C6          0xc6            /* 召唤报告故障序号 */
#define IEC103_TI_M_C7          0xc7            /* 召唤报告索引序号 */
#define IEC103_TI_M_C8          0xc8            /* 召唤报告数据 */
#define IEC103_TI_M_C9          0xc9            /* 清除故障记录数据 */
#define IEC103_TI_M_CA          0xca            /* 手动启动录波 */
#define IEC103_TI_M_CB          0xcb            /* 召唤录波序号 */
#define IEC103_TI_M_CC          0xcc            /* 召唤录波时间 */
#define IEC103_TI_M_CD          0xcd            /* 召唤录波信息 */
#define IEC103_TI_M_CE          0xce            /* 召唤录波电网故障序号 */

/* 传动命令 */
#define IEC103_TI_M_E0          0xe0            /* 预发 */
#define IEC103_TI_M_E1          0xe1            /* 执行 */
/* 调通道信息需要的命令 */
#define IEC103_TI_M_E2          0xe2            /* 调通道系数及量值 */
#define IEC103_TI_M_E3          0xe3            /* 修改保护通道系数 */
#define IEC103_TI_M_E4          0xe4            /* 调保护通道额定值 */
#define IEC103_TI_M_E5          0xe5            /* 修改保护通道额定值 */
#define IEC103_TI_M_E6          0xe6            /* 调装置零漂刻度及零漂 */
#define IEC103_TI_M_E7          0xe7            /* 修改装置零漂 */
#define IEC103_TI_M_E8          0xe8            /* 调通道相位 */
#define IEC103_TI_M_E9          0xe9            /* 修改通道相位误差 */
#define IEC103_TI_M_EA          0xea            /* 上调模拟通道相位误差定值 */

/* 信号复归命令 */
#define IEC103_TI_M_F0          0xF0            /* 信号复归 */
#define IEC103_TI_M_F1          0xF1            /* 召唤开入量状态及GPS开入量计数 */
/****************************************************************/
#define IEC103_TI_M_D0          0xd0            // load state inf
#define IEC103_TI_M_D1          0xd1            // load state inf name
#define IEC103_TI_M_D2          0xd2            // change protect wave name
#define IEC103_TI_M_D3          0xd3            // start wave
#define IEC103_TI_M_D4          0xd4            // load wave ID
#define IEC103_TI_M_D7          0xd7            // load event ID
#define IEC103_TI_M_D8          0xd8            // load wave time
#define IEC103_TI_M_D9          0xd9            // clear wave
#define IEC103_TI_M_DE          0xde            // drive protect outlet(ready)
#define IEC103_TI_M_DF          0xdf            // load report soe inf

#define IEC103_TI_M_EB          0xeb            // load protect channel template
#define IEC103_TI_M_EC          0xec            // change channel name
#define IEC103_TI_M_ED          0xed            // load channel cofficient
#define IEC103_TI_M_EE          0xee            // load channel phase
#define IEC103_TI_M_EF          0xef            // change channel cofficient
#define IEC103_TI_M_F2          0xf2            // change channel rate
#define IEC103_TI_M_F3          0xf3            // change channel phase
#define IEC103_TI_M_F4          0xf4            // load excursion
#define IEC103_TI_M_F5          0xf5            // change excursion
#define IEC103_TI_M_F6          0xf6            // change whole excursion
#define IEC103_TI_M_F7          0xf7            // read memory
#define IEC103_TI_M_FD          0xfd            // general local read
#define IEC103_TI_M_FE          0xfe            // change optical setting

#define IEC103_INF_ResetFCB             2       /* reset FCB */
#define IEC103_INF_ResetCU              3       /* reset CU */
#define IEC103_INF_ResetSetup           4       /* reset Setup */
#define IEC103_INF_LoadVer              6       /* load version */

#define I3DEF_FCK_INF_ERR_RAM           241     /* ram error(FCK) */
#define I3DEF_FCK_INF_ERR_EEPROM        243     /* EEPROM error(FCK) */
#define I3DEF_FCK_INF_ERR_SETTING       244     /* setting error(FCK) */
#define I3DEF_FCK_INF_ERR_AD            245     /* AD error(FCK) */
#define I3DEF_FCK_INF_ERR_RELAY         249     /* relay error(FCK) */
#define I3DEF_FCK_INF_ERR_5VPOWER       239     /* 5v power error(FCK) */
#define I3DEF_FCK_DPI_OPERATE           1       /* operate(FCK) */
#define I3DEF_FCK_DPI_RETURN            0       /* return(FCK) */

#define I3DEF_PRO_INF_ERR_RAM           193     /* ram error(protect) */
#define I3DEF_PRO_INF_ERR_EEPROM        196     /* EEPROM error(protect) */
#define I3DEF_PRO_INF_ERR_SETTING       222     /* setting error(protect) */
#define I3DEF_PRO_INF_ERR_AD            197     /* AD error(protect) */
#define I3DEF_PRO_INF_ERR_RELAY         201     /* relay error(protect) */
#define I3DEF_PRO_INF_ERR_5VPOWER       191     /* 5v power error(protect) */
#define I3DEF_PRO_DPI_OPERATE           2       /* operate(protect) */
#define I3DEF_PRO_DPI_RETURN            1       /* return(protect) */
/*-------------------LED STATUS --------------------------------------*/
#define LED_STATUS_10                   0x10    /* modify cpu number */
#define LED_STATUS_11                   0x11    /* cpu led slow flash */
#define LED_STATUS_12                   0x12    /* cpu led normal */
/*-------------------SETTING DEAL-------------------------------------*/
#define SET_COEFFICIENT_0               0x0     /* 1 */
#define SET_COEFFICIENT_1               0x1     /* 0.1 */
#define SET_COEFFICIENT_2               0x2     /* 0.01 */
#define SET_COEFFICIENT_3               0x3     /* 0.001 */
#define SET_CONTROL                     0x0     /* control word */
#define SET_DATA                        0x10    /* data */
#define SET_SYMBOL_POSTIVE              0x0     /* symbol bit */
#define SET_SYMBOL_NEGATIVE             0x80    /* symbol bit */
#endif
