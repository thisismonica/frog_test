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
/*      cpc_defs.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CPC - Common Protect Control                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains data structure definitions and constants for  */
/*      the Common Protect control component.                            */
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
#include "plat/inc/pro_defs.h"

/* Check to see if the file has been included already.  */

#ifndef CPC_DEFS_H
#define CPC_DEFS_H

/* Define unused parameter */
#define CPC_UNUSED_PARAM(parameter)   ((VOID)parameter)

/* 获取结构成员地址偏移x:结构名称,y:成员名称 */
#define GETSTRUCTOFFSET(x,y) (int)(&((##x*)0)->##y)

/* 驱动程序地址空间 */
#define CPC_DRV_TEXT_ADDR               PLAT_DRV_TEXT_ADDR                  /* 驱动程序起始地址 */
#define CPC_DRV_TEXT_SIZE               PLAT_DRV_TEXT_SIZE                  /* 驱动程序最大地址空间 */

/* 保护程序地址空间 */
#define CPC_PROTECT_TEXT_ADDR           PLAT_PROTECT_TEXT_ADDR              /* 保护程序起始地址 */
#define CPC_PROTECT_TEXT_SIZE           PLAT_PROTECT_TEXT_SIZE              /* 保护程序最大地址空间 */

/* HMI程序地址空间 */
#define CPC_HMI_TEXT_ADDR               PLAT_HMI_TEXT_ADDR                  /* HMI程序起始地址 */
#define CPC_HMI_TEXT_SIZE               PLAT_HMI_TEXT_SIZE                  /* HMI程序最大地址空间 */

/* Gws61850程序地址空间 */
#define CPC_GWS_TEXT_ADDR               PLAT_GWS_TEXT_ADDR                  /* Gws61850程序起始地址 */
#define CPC_GWS_TEXT_SIZE               PLAT_GWS_TEXT_SIZE                  /* Gws61850程序最大地址空间 */

/* NPI程序地址空间 */
#define CPC_NPI_TEXT_ADDR               PLAT_NPI_TEXT_ADDR                  /* NPI程序起始地址 */
#define CPC_NPI_TEXT_SIZE               PLAT_NPI_TEXT_SIZE                  /* NPI程序最大地址空间 */

/* 装置分模块个数 */
#define CPC_MAX_MODULE_NUMBER           PLAT_MAX_MODULE_NUMBER              /* 装置最大分模块个数 */

/* 与NPI通讯基本信息 */
#define CPC_MAX_LENGTH_FRAME            PLAT_MAX_LENGTH_FRAME               /* 最大帧长 */
#define CPC_MAX_USER_FRAME_NUMBER       PLAT_MAX_USER_FRAME_NUMBER          /* 缓存的最大用户数据帧个数 */

/* 硬件资源设置 */
#define CPC_MAX_EVENT_RECORD            PLAT_MAX_EVENT_RECORD               /* 最大事件组数 */
#define CPC_MAX_AD_NUMBER               PLAT_MAX_AD_NUMBER                  /* 最大A/D片数 */
#define CPC_MAX_PHYSICAL_CHL_NUMBER     PLAT_MAX_PHYSICAL_CHL_NUMBER        /* 最大的物理通道个数 */
#define CPC_MAX_VIRTUAL_CHL_NUMBER      PLAT_MAX_VIRTUAL_CHL_NUMBER         /* 最大的虚通道个数 */
#define CPC_MAX_CHL_NUMBER              PLAT_MAX_CHL_NUMBER                 /* 最大模拟量通道个数（物理+虚通道） */
#define CPC_MAX_SAMPLE_POINT            PLAT_MAX_SAMPLE_POINT               /* 最大采样点数 */
#define CPC_MAX_CHANNEL_POINT           PLAT_MAX_CHANNEL_POINT              /* 每通道最大采样点数 */
#define CPC_MAX_SAMPLE_COUNTER          PLAT_MAX_SAMPLE_COUNTER             /* 采样序号的最大值 */
#define CPC_MAX_SAMPLE_COUNTER_OLT      PLAT_MAX_SAMPLE_COUNTER_OLT         /* 光纤采样序号的最大值 */
#define CPC_MAX_SWITCH_STATUS_POINT     PLAT_MAX_SWITCH_STATUS_POINT        /* 每组开关量变位的最大记录点数 */
#define CPC_MAX_PROTECT_GROUPS          PLAT_MAX_PROTECT_GROUPS             /* 最大保护状态量组数 */
#define CPC_MAX_INPUT_GROUPS            PLAT_MAX_INPUT_GROUPS               /* 最大开入量组数 */
#define CPC_MAX_WAVE_INPUT_GROUPS       PLAT_MAX_WAVE_INPUT_GROUPS          /* 最大录波开入量组数 */
#define CPC_MAX_OUTPUT_GROUPS           PLAT_MAX_OUTPUT_GROUPS              /* 最大开出量组数 */
#define CPC_MAX_CHECK_RELAY             PLAT_MAX_CHECK_RELAY                /* 最大出口检测数 */
#define CPC_MAX_PROTECT_LOG_GROUPS      PLAT_MAX_PROTECT_LOG_GROUPS         /* 最大逻辑状态量组数 */
#define CPC_MAX_STATE_GROUPS            PLAT_MAX_STATE_GROUPS               /* 最大录波状态量数据组数 */
#define CPC_MAX_WAVE_DATA_SIZE          PLAT_MAX_WAVE_DATA_SIZE             /* 最大录波数据点数 */
#define CPC_MAX_RAM_WAVE_DATA_NUMBER    PLAT_MAX_RAM_WAVE_DATA_NUMBER       /* 最大内存缓存录波数据个数 */
#define CPC_MAX_DEVICE_SYSTEM_NUM       PLAT_MAX_DEVICE_SYSTEM_NUM          /* 最大装载应用程序数目 */
#define CPC_MAX_NVRAM_REPORT_NUMBER     PLAT_MAX_NVRAM_REPORT_NUMBER        /* 最大存储报告个数 */
#define CPC_MAX_FAULT_REPORT_NUMBER     PLAT_MAX_FAULT_REPORT_NUMBER        /* 最大访问故障报告个数 */
#define CPC_MAX_NVRAM_WAVE_NUMBER       PLAT_MAX_NVRAM_WAVE_NUMBER          /* 最大存储录波数目 */
#define CPC_MAX_NVRAM_EVENT_NUMBER      PLAT_MAX_NVRAM_EVENT_NUMBER         /* 最大存储事件数目 */
#define CPC_MAX_RAM_REPORT_NUMBER       PLAT_MAX_RAM_REPORT_NUMBER          /* 最大内存缓存报告数目 */
#define CPC_MAX_RAM_EVENT_NUMBER        PLAT_MAX_RAM_EVENT_NUMBER           /* 最大内存缓存事件数目 */
#define CPC_MAX_ONE_FAULT_REPORT_NUMBER PLAT_MAX_ONE_FAULT_REPORT_NUMBER    /* 一次故障最大报告个数 */
#define CPC_FCK_SOE_MAX_COUNT           PLAT_FCK_SOE_MAX_COUNT              /* 支持的最大测控SOE的个数 */
#define CPC_READ_COMMAND_MAX_COUNT      PLAT_READ_COMMAND_MAX_COUNT         /* 支持的最大测控读命令的个数 */

/* 与HMI通信端口配置 */
#define CPC_LINK_HMI_PHY_COM            PLAT_LINK_HMI_PHY_COM               /* 与HMI之间的通信端口设置(1:物理口,0:虚拟) */

/* 看门狗使能选择 */
#define CPC_WATCHDOG_ENABLE             PLAT_WATCHDOG_ENABLE                /* 看门狗使能 */

/* 错误事件组句柄 */
#define CPC_ERROR_EVENT_CB              PLAT_ERROR_EVENT_CB                 /* 错误事件位句柄 */

/* 第1事件组（错误信息）的位定义 */
#define CPC_EVENT1_ERR_SRAM             Error_SRAM                          /* EEPROM出错(1 << 0) */
#define CPC_EVENT1_ERR_RAM              Error_RAM                           /* RAM出错(1 << 1) */
#define CPC_EVENT1_ERR_SETTING          Error_Setting                       /* 定值出错(1 << 2) */
#define CPC_EVENT1_ERR_RELAY            Error_Relay                         /* 继电器出错(1 << 3) */
#define CPC_EVENT1_ERR_5VPOWER          Error_5VPower                       /* 电源出错(1 << 4) */
#define CPC_EVENT1_ERR_CPU_NUMBER       Error_CPU_Number                    /* CPU个数出错(1 << 5) */
#define CPC_EVENT1_ERR_AD               Error_AD                            /* AD出错(1 << 6) */
#define CPC_EVENT1_ERR_COMMU            Error_Commu                         /* 通讯出错(1 << 7) */
#define CPC_EVENT1_ERR_FLASH            Error_FLASH                         /* FLASH出错(1 << 8) */
#define CPC_EVENT1_ERR_PROGRAM          Error_Program                       /* 程序校验出错(1 << 9) */
#define CPC_EVENT1_ERR_STRIKE           Error_Relay_Close                   /* 开出回路击穿(1 << 10) */
#define CPC_EVENT1_ERR_EXTEND           Error_Extend_Port                   /* 扩展插件出错(1 << 11) */
#define CPC_EVENT1_CLOSE_RELAY          Close_Relay_Flag                    /* 电源自检出错闭锁出口标志(1 << 12) */
#define CPC_EVENT1_SAMPLE_OK            Sample_Data_OK                      /* 采样数据准备好(1 << 13) */
#define CPC_EVENT1_USER_ALARM           User_Alarm_Bits                     /* 用户告警位(1 << 14) */
#define CPC_EVENT1_GWS_INIT_STATUS      GWS_InitStatus                      /* 网关初始状态(1 << 15) */

#define CPC_EVENT1_DEV_ERR (CPC_EVENT1_ERR_SRAM+CPC_EVENT1_ERR_RAM           \
                           +CPC_EVENT1_ERR_SETTING+CPC_EVENT1_ERR_RELAY      \
                           +CPC_EVENT1_ERR_5VPOWER+CPC_EVENT1_ERR_AD         \
                           +CPC_EVENT1_ERR_COMMU+CPC_EVENT1_ERR_FLASH        \
                           +CPC_EVENT1_ERR_EXTEND+CPC_EVENT1_USER_ALARM)

#define CPC_EVENT1_RUNLED_ABNORMAL (CPC_EVENT1_ERR_SRAM+CPC_EVENT1_ERR_RAM   \
                           +CPC_EVENT1_ERR_SETTING+CPC_EVENT1_ERR_RELAY      \
                           +CPC_EVENT1_ERR_5VPOWER+CPC_EVENT1_ERR_AD         \
                           +CPC_EVENT1_ERR_COMMU+CPC_EVENT1_ERR_FLASH        \
                           +CPC_EVENT1_ERR_EXTEND)

/* 事件1平台事件位 */
#define CPC_EVENT1_PLAT_BITS    (CPC_EVENT1_ERR_SRAM+CPC_EVENT1_ERR_RAM            \
                                +CPC_EVENT1_ERR_SETTING+CPC_EVENT1_ERR_RELAY       \
                                +CPC_EVENT1_ERR_5VPOWER+CPC_EVENT1_ERR_CPU_NUMBER  \
                                +CPC_EVENT1_ERR_AD+CPC_EVENT1_ERR_COMMU            \
                                +CPC_EVENT1_ERR_FLASH+CPC_EVENT1_ERR_PROGRAM       \
                                +CPC_EVENT1_ERR_STRIKE+CPC_EVENT1_ERR_EXTEND       \
                                +CPC_EVENT1_CLOSE_RELAY+CPC_EVENT1_SAMPLE_OK)

/* 第2事件组的位定义 */
#define CPC_EVENT2_RESET                (1 << 0)                            /* 复归信号灯 */
#define CPC_EVENT2_TRIP                 (1 << 1)                            /* 出口传动 */
#define CPC_EVENT2_EXCHANGE             (1 << 2)                            /* CPU互传 ,发送*/
#define CPC_EVENT2_EXCHANGE_R           (1 << 3)                            /* CPU互传 ,接收*/
#define CPC_EVENT2_WAVE                 (1 << 4)                            /* 录波完成 */
#define CPC_EVENT2_WAVE_CLR             (1 << 5)                            /* 清录波数据 */
#define CPC_EVENT2_SRAM_FISH            (1 << 6)                            /* SRAM 写完成 */
#define CPC_EVENT2_SRAM_ERROR           (1 << 7)                            /* 写SRAM 出错 */
#define CPC_EVENT2_INITIALIZE           (1 << 8)                            /* 保护初始化完成 */
#define CPC_EVENT2_CLOCK_READY          (1 << 9)                            /* 时钟初始化完成 */
#define CPC_EVENT2_TASK_0               (1 << 10)                           /* task 0 */
#define CPC_EVENT2_TASK_1               (1 << 11)                           /* task 1 */
#define CPC_EVENT2_TASK_2               (1 << 12)                           /* task 2 */
#define CPC_EVENT2_TASK_3               (1 << 13)                           /* task 3 */
#define CPC_EVENT2_RESET_MAIN           (1 << 14)                           /* 复归主保护 */
#define CPC_EVENT2_RESET_BACK           (1 << 15)                           /* 复归后备保护 */
#define CPC_EVENT2_SYN_ERR              (1 << 16)                           /* 同步串口通讯出错 */
#define CPC_EVENT2_PROG_READY           (1 << 17)                           /* 下载程序准备就绪 */
#define CPC_EVENT2_CFG_READY            (1 << 18)                           /* 保护配置准备就绪 */
#define CPC_EVENT2_CFG_PROGRAM          (1 << 19)                           /* 固化保护配置 */
#define CPC_EVENT2_CFG_ERROR            (1 << 20)                           /* 固化保护配置出错 */
#define CPC_EVENT2_CFG_MEMORY_READY     (1 << 21)                           /* 配置空间申请成功 */
#define CPC_EVENT2_NO_SLEEP             (1 << 22)                           /* 主保护任务不休眠 */
#define CPC_EVENT2_NO_MEMORY            (1 << 23)                           /* 内存空间不够 */
#define CPC_EVENT2_VERSION_ERROR        (1 << 24)                           /* 版本错误 */
#define CPC_EVENT2_PROTECT_READY        (1 << 25)                           /* 装载应用程序成功 */
#define CPC_EVENT2_REPORT_CLR           (1 << 26)                           /* 清报告数据 */
#define CPC_EVENT2_HMI_READY            (1 << 27)                           /* 装载HMI程序成功 */
#define CPC_EVENT2_GWS_READY            (1 << 28)                           /* 装载GWS程序成功 */
#define CPC_EVENT2_DRV_READY            (1 << 29)                           /* 装载驱动程序成功 */

/* 硬件芯片操作超时时间定义 */
#define CPC_DATA_TIMEOUT                800                                 /* 整定值数据超时时间 */
#define CPC_CONFIG_TIMEOUT              800                                 /* 配置数据超时时间 */
#define CPC_EVENT_TIMEOUT               800                                 /* 事件超时时间 */
#define CPC_REPORT_TIMEOUT              800                                 /* 报告超时时间 */
#define CPC_WAVE_TIMEOUT                800                                 /* 录波超时时间 */
#define CPC_OUTHOLD_TIMEOUT             800                                 /* 出口保持位数据超时时间 */

/* CRC码计算常量定义 */
#define CPC_CRC32_Initialize            0xffffffff                          /* 32位CRC码的初值 */
#define CPC_CRC32_MASKS                 0xffffffff                          /* 32位CRC码的掩码 */
#define CPC_CRC32_BITS                  32                                  /* 32位CRC码的位数 */
#define CPC_RAM_DATA_BITS               32                                  /* RAM数据位数 */
#define CPC_CRC16_Initialize            0x0000                              /* 16位CRC码的初值 */
#define CPC_CRC16_MASKS                 0x0000                              /* 16位CRC码的掩码 */
#define CPC_CRC16_BITS                  16                                  /* 16位CRC码的位数 */

/* 常量定义 */
#define CPC_MAX_CPU_NUMBER              8                                   /* 最大CPU个数 */
#define CPC_DATA_MASKS8                 0xff                                /* 8位数据掩码 */
#define CPC_DATA_MASKS16                0xffff                              /* 16位数据掩码 */
#define CPC_DATA_MASKS32                0xffffffff                          /* 32位数据掩码 */
#define CPC_DATA_MASKSH16               0xffff0000                          /* 高16位数据掩码 */
#define CPC_DATA_BITS8                  8                                   /* 8位数据位数 */
#define CPC_DATA_BITS16                 16                                  /* 16位数据位数 */
#define CPC_DATA_BITS32                 32                                  /* 32位数据位数 */
#define CPC_GROUP_MASK32                5                                   /* 32位数据组号的掩码位 */
#define CPC_SUB_GROUP_MASK32            31                                  /* 32位数据值的掩码位 */
#define CPC_DATA_FLAG                   0x5a                                /* 数据标志 */
#define CPC_BYTE_BITS                   8                                   /* 字节位数 */
#define CPC_TEXT_DATA_BYTE_NUM          (4/sizeof(UNSIGNED))                /* 单个程序地址的字节数 */
#define CPC_RAM_DATA_BYTE_NUM           (4/sizeof(UNSIGNED))                /* 单个数据地址的字节数 */
#define CPC_UNIT_LENGTH                 16                                  /* the max length of one unit data */
#define CPC_DISABLE_DISPLAY             0                                   /* 禁止显示（退出） */
#define CPC_CHANNEL_NOT_ACTIVE          -1                                  /* 未使用通道标志 */
#define CPC_WORD_BYTES                  4                                   /* WORD的字节数 */

/* 内部事件类型定义 */
#define CPC_RES_EVENT_START             1                                   /* 传动启动继电器 */
#define CPC_RES_EVENT_TRIP              2                                   /* 传动跳闸矩阵 */
#define CPC_RES_EVENT_OUT               3                                   /* 传动出口 */
#define CPC_RES_EVENT_REPAIR            4                                   /* 数据自检出错 */
#define CPC_RES_EVENT_EEPROM            5                                   /* 写EEPROM出错 */
#define CPC_RES_EVENT_FLASH             6                                   /* 写FLASH出错 */
#define CPC_RES_EVENT_ERASE             7                                   /* FLASH扇区擦除出错 */
#define CPC_RES_EVENT_SIZE              8                                   /* 数据大小出错 */
#define CPC_RES_EVENT_RESTART           9                                   /* 装置重新上电 */
#define CPC_RES_EVENT_RELAY             10                                  /* 继电器动作 */
#define CPC_RES_EVENT_CLOSE             11                                  /* 继电器瞬时击穿 */
#define CPC_RES_EVENT_CLOSE_EXT         12                                  /* 扩展回路瞬时击穿 */
#define CPC_RES_EVENT_REPORT_CLR        13                                  /* 清除报告 */
#define CPC_RES_EVENT_WAVE_CLR          14                                  /* 清除录波 */
#define CPC_RES_EVENT_ZONE_CHANGE       15                                  /* 定值区变化 */
#define CPC_RES_EVENT_CONFIG_CHANGE     16                                  /* 配置变化 */

/* 目录路径定义 */
#if 0
#define CPC_USER_ROOT_DIR_NAME          "/ram"                              /* 用户数据根目录路径名称 */
#define CPC_ALARM_DIR_NAME              "/ram/palarm"                       /* 告警事件存储路径 */
#define CPC_REVERSE_DIR_NAME            "/ram/preserve"                     /* 内部事件存储路径 */
#define CPC_USER1_DIR_NAME              "/ram/puser1"                       /* 用户事件1存储路径 */
#define CPC_USER2_DIR_NAME              "/ram/puser2"                       /* 用户事件2存储路径 */
#define CPC_USER3_DIR_NAME              "/ram/puser3"                       /* 用户事件3存储路径 */
#define CPC_USER4_DIR_NAME              "/ram/puser4"                       /* 用户事件4存储路径 */
#define CPC_USER5_DIR_NAME              "/ram/puser5"                       /* 用户事件5存储路径 */
#define CPC_USER6_DIR_NAME              "/ram/puser6"                       /* 用户事件6存储路径 */
#define CPC_USER7_DIR_NAME              "/ram/puser7"                       /* 用户事件7存储路径 */
#define CPC_OPERATE_DIR_NAME            "/ram/poperate"                     /* 操作事件存储路径 */
#define CPC_REPORT_DIR_NAME             "/ram/preport"                      /* 报告存储路径 */
#define CPC_WAVE_DIR_NAME               "/ram/pwave"                        /* 录波存储路径 */
#define CPC_SETTING_DATA_DIR_NAME       "/ram/psetting"                     /* 整定值数据存储路径 */
#define CPC_CONFIG_DATA_DIR_NAME        "/ram/pconfig"                      /* 配置数据存储路径 */
#define CPC_TEXT_CRC_DIR_NAME           "/ram/ptextcrc"                     /* 程序存储路径 */
#define CPC_OUTPUTHOLD_DATA_DIR_NAME    "/ram/poutputhold"                  /* 出口保持位存储路径 */
#define CPC_LOG_DIR_NAME                "/ram/plog"                         /* 日志信息存储路径 */
#define CPC_CMS_DIR_NAME                "/ram/pcommsg"                      /* 通信报文信息存储路径 */
#else
#define CPC_USER_ROOT_DIR_NAME          "/user"                              /* 用户数据根目录路径名称 */
#define CPC_ALARM_DIR_NAME              "/user/palarm"                       /* 告警事件存储路径 */
#define CPC_REVERSE_DIR_NAME            "/user/preserve"                     /* 内部事件存储路径 */
#define CPC_USER1_DIR_NAME              "/user/puser1"                       /* 用户事件1存储路径 */
#define CPC_USER2_DIR_NAME              "/user/puser2"                       /* 用户事件2存储路径 */
#define CPC_USER3_DIR_NAME              "/user/puser3"                       /* 用户事件3存储路径 */
#define CPC_USER4_DIR_NAME              "/user/puser4"                       /* 用户事件4存储路径 */
#define CPC_USER5_DIR_NAME              "/user/puser5"                       /* 用户事件5存储路径 */
#define CPC_USER6_DIR_NAME              "/user/puser6"                       /* 用户事件6存储路径 */
#define CPC_USER7_DIR_NAME              "/user/puser7"                       /* 用户事件7存储路径 */
#define CPC_OPERATE_DIR_NAME            "/user/poperate"                     /* 操作事件存储路径 */
#define CPC_REPORT_DIR_NAME             "/user/preport"                      /* 报告存储路径 */
#define CPC_WAVE_DIR_NAME               "/user/pwave"                        /* 录波存储路径 */
#define CPC_SETTING_DATA_DIR_NAME       "/user/psetting"                     /* 整定值数据存储路径 */
#define CPC_CONFIG_DATA_DIR_NAME        "/user/pconfig"                      /* 配置数据存储路径 */
#define CPC_TEXT_CRC_DIR_NAME           "/user/ptextcrc"                     /* 程序存储路径 */
#define CPC_OUTPUTHOLD_DATA_DIR_NAME    "/user/poutputhold"                  /* 出口保持位存储路径 */
#define CPC_LOG_DIR_NAME                "/user/plog"                         /* 日志信息存储路径 */
#define CPC_CMS_DIR_NAME                "/user/pcommsg"                      /* 通信报文信息存储路径 */
#endif
#ifdef WIN32
#define CPC_CFG_FILE_NAME               "/user/plat_cfg"                      /* 配置文件名称 */
#else
#define CPC_CFG_FILE_NAME               "/user/plat_cfg.xml"                  /* 配置文件名称 */
#endif
#ifdef WIN32
#define CPC_BACKGROUND_DATA_FILE_NAME   "/bin"                               /* 背景数据文件目录 */
#else
#define CPC_BACKGROUND_DATA_FILE_NAME   "/bin/npu808bgd.dat"                 /* 背景数据文件名 */
#endif
/* 检测循环数据,x:被检查数,y:下限,z:上限 */
#define CHECKCYCDATALIMIT(x,y,z)                \
  {   \
    if(x > z) x = y;                            \
    if(x < y) x = z;                            \
  }

/* 复归继电器经启动闭锁标志 */
#define CPC_RESET_START_FLAG            PLAT_RESET_START_FLAG               /* 复归继电器经过启动继电器启动标志 */

/* Define I/O port data structure for operating the port.  */

typedef struct IO_PORT_STRUCT
{
    UNSIGNED        uPortNumber;                /* the number of the port  */
    UNSIGNED        uPortBits;                  /* the bits number of the port */
    UNSIGNED        uMasksBits;                 /* the masks bits */
    VOID            * pPortAddress1;            /* the first address of the port */
    VOID            * pPortAddress2;            /* the second address of the port */
    VOID            * pPortAddress3;            /* the third address of the port */
    VOID            * pPortAddress4;            /* the fourth address of the port */
    UNSIGNED        uReverseBits;               /* the reverse bits */
}IO_PORT;
#endif
