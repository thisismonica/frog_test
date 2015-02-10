#ifndef PLAT_NPU808_DEFS_H
#define PLAT_NPU808_DEFS_H

/* 驱动程序地址空间 */
#define PLAT_DRV_TEXT_ADDR              0x00600000      /* 驱动程序起始地址 */
#define PLAT_DRV_TEXT_SIZE              0x00100000      /* 驱动程序最大地址空间 */

/* 保护程序地址空间 */
#define PLAT_PROTECT_TEXT_ADDR          0x00800000      /* 保护程序起始地址 */
#define PLAT_PROTECT_TEXT_SIZE          0x00200000      /* 保护程序最大地址空间 */

/* HMI程序地址空间 */
#define PLAT_HMI_TEXT_ADDR              0x00B00000      /* HMI程序起始地址 */
#define PLAT_HMI_TEXT_SIZE              0x00300000      /* HMI程序最大地址空间 */

/* Gws61850程序地址空间 */
#define PLAT_GWS_TEXT_ADDR              0x01100000      /* GWS程序起始地址 */
#define PLAT_GWS_TEXT_SIZE              0x00600000      /* GWS程序最大地址空间 */

/* NPI程序地址空间 */
#define PLAT_NPI_TEXT_ADDR              0x01800000      /* NPI程序起始地址 */
#define PLAT_NPI_TEXT_SIZE              0x00500000      /* NPI程序最大地址空间 */

/* 装置分模块个数 */
#define PLAT_MAX_MODULE_NUMBER          2               /* 装置最大分模块个数 */

/* 与NPI通讯基本信息 */
#define PLAT_MAX_LENGTH_FRAME           500             /* 最大帧长 */
#define PLAT_MAX_USER_FRAME_NUMBER      50              /* 缓存的最大用户数据帧个数 */

/* 硬件资源设置 */
#define PLAT_MAX_EVENT_RECORD           7               /* 最大事件组数 */
#define PLAT_MAX_AD_NUMBER              3               /* 最大A/D片数 */
#define PLAT_MAX_PHYSICAL_CHL_NUMBER    96              /* 最大的物理通道个数 */
#define PLAT_MAX_VIRTUAL_CHL_NUMBER     48              /* 最大的虚通道个数 */
#define PLAT_MAX_CHL_NUMBER \
(PLAT_MAX_PHYSICAL_CHL_NUMBER+PLAT_MAX_VIRTUAL_CHL_NUMBER) /* 最大模拟量通道个数（物理+虚通道） */
#define PLAT_MAX_SAMPLE_POINT           100             /* 最大采样点数 */
#define PLAT_MAX_CHANNEL_POINT          0x200           /* 每通道最大采样点数 */
#define PLAT_MAX_SAMPLE_COUNTER         0x10000         /* 采样序号的最大值 */
#define PLAT_MAX_SAMPLE_COUNTER_OLT     1200            /* 光纤采样序号的最大值 */
#define PLAT_MAX_SWITCH_STATUS_POINT    200             /* 每组开关量变位的最大记录点数 */
#define PLAT_MAX_WAVE_DATA_SIZE         1200            /* 最大录波数据点数 */
#define PLAT_MAX_RAM_WAVE_DATA_NUMBER   5               /* 最大内存缓存录波数据个数 */
#define PLAT_MAX_PROTECT_GROUPS         10              /* 最大保护状态量组数 */
#define PLAT_MAX_INPUT_GROUPS           40              /* 最大开入量组数 */
#define PLAT_MAX_WAVE_INPUT_GROUPS      20              /* 最大录波开入量组数 */
#define PLAT_MAX_OUTPUT_GROUPS          10              /* 最大开出量组数 */
#define PLAT_MAX_CHECK_RELAY            12              /* 最大出口检测数 */
#define PLAT_MAX_PROTECT_LOG_GROUPS     20              /* 最大逻辑状态量组数 */
#define PLAT_MAX_STATE_GROUPS \
(PLAT_MAX_PROTECT_GROUPS+PLAT_MAX_WAVE_INPUT_GROUPS+PLAT_MAX_OUTPUT_GROUPS+PLAT_MAX_PROTECT_LOG_GROUPS) /* 最大录波状态量数据组数 */
#define PLAT_MAX_DEVICE_SYSTEM_NUM      2               /* 最大装载应用程序数目 */
#define PLAT_MAX_FAULT_REPORT_NUMBER    100             /* 最大访问故障报告个数 */
#define PLAT_MAX_NVRAM_REPORT_NUMBER    PLAT_MAX_FAULT_REPORT_NUMBER /* 最大存储报告数目 */
#define PLAT_MAX_NVRAM_WAVE_NUMBER      100             /* 最大存储录波数目 */
#define PLAT_MAX_NVRAM_EVENT_NUMBER     80              /* 最大存储事件数目 */
#define PLAT_MAX_RAM_REPORT_NUMBER      70              /* 最大内存缓存报告数目 */
#define PLAT_MAX_RAM_EVENT_NUMBER       60              /* 最大内存缓存事件数目 */
#define PLAT_MAX_ONE_FAULT_REPORT_NUMBER 70             /* 一次故障最大报告个数 */
#define PLAT_FCK_SOE_MAX_COUNT           500            /* 支持的最大测控SOE的个数 */
#define PLAT_READ_COMMAND_MAX_COUNT      200            /* 支持的最大测控读命令的个数 */

/* 与HMI通信端口配置 */
#define PLAT_LINK_HMI_PHY_COM           NU_FALSE        /* 与HMI之间的通信端口设置(NU_TRUE：物理通信口连接，NU_FALSE：虚拟连接) */

/* 看门狗使能选择 */
#define PLAT_WATCHDOG_ENABLE            NU_TRUE         /* 看门狗使能 */

/* 测试版本选择 */
#define PLAT_DEBUG_ENABLE               NU_FALSE          /* 测试版本选择 */

/* 定义事件位句柄 */
extern NU_EVENT_GROUP Event_1_CB;
#define PLAT_ERROR_EVENT_CB             (& Event_1_CB)  /* 错误事件位句柄 */

/* 复归继电器经启动闭锁标志 */
#define PLAT_RESET_START_FLAG           0               /* 复归继电器经过启动继电器启动标志 */

/* Define task stacks size */
#define TASK_STACK_SIZE         (5000*sizeof(unsigned long))
#define TASK_H_STACK_SIZE       (10000*sizeof(unsigned long))

/* Define each task's priority */
#define TASK0_PRIORITY          16
#define TASK1_PRIORITY          8
#define TASK2_PRIORITY          16
#define TASK3_PRIORITY          16

/* Define each task's time slice */
#define TASK0_TIMESLICE         2
#define TASK1_TIMESLICE         5
#define TASK2_TIMESLICE         5
#define TASK3_TIMESLICE         2

/* Define each timer's initial time slice */
#define TIMER_RUN_INI_TIMESLICE 2

/* Define each timer's time slice */
#ifdef WIN32
#define TIMER_RUN_TIMESLICE     1 //10
#else
#define TIMER_RUN_TIMESLICE     1 //1
#endif

#endif  /* PLAT_NPU810_DEFS_H */
