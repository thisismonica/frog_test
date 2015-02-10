
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
/*      pro_defs.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      PRO - protect structure                                          */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains data structure definitions and constants for  */
/*      the protect structure,setting structure, value structure         */
/*      SOE structure....                                                */
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
/*      ZhiDeYang       09-10-2002      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

/* Check to see if this file has been included already.  */
#ifndef PRO_DEFS_H
#define PRO_DEFS_H

/* Define constants for the number of character string in each of the compress
   format */
/* 获取采样数据的起始指针类型定义 */
#define PRO_SAMPLE_ANALONG  1      /* 模拟量通道指针 */
#define PRO_SAMPLE_INPUT    2      /* 开入量指针 */
#define PRO_SAMPLE_OUTPUT   3      /* 开出量指针 */
#define PRO_SAMPLE_PRO_STS  4      /* 保护状态量指针 */
#define PRO_SAMPLE_PRO_LOG  5      /* 保护逻辑状态量指针 */
#define PRO_SAMPLE_PULSE    6      /* 采样脉冲计数指针 */
#define PRO_SOFT_INPUT_QUA_INFO 7  /* 软开入品质因素信息指针(实时数据) */
#define PRO_ANALONG_CHECK_QUALITY 8  /* 模拟量通道品质因素检修位(实时数据) */

/* 平台透传应用命令基本信息定义 */
/* 数据发送方向 */
#define PRO_DATA_SEND_GWS     0          /* 向网关发送 */
#define PRO_DATA_SEND_MMI     1          /* 向接口发送 */
#define PRO_MAX_DATA_LEN      244        /* 最大数据长度 */
/* 数据优先级 */
#define PRO_DATA_PRI          1          /* 一级数据 */
#define PRO_DATA_SEC          2          /* 二级数据 */

/* 通讯规约宏定义 */
#define PRO_103_RULES         (1 << 0)   /* 103通讯规约 */
#define PRO_61850_RULES       (1 << 1)   /* 61850通讯规约 */

/* 对时来源定义 */
#define PRO_SYN_SRC_STATION   (1 << 0)   /* 站控层 */
#define PRO_SYN_SRC_PROCESS   (1 << 1)   /* 过程层 */

/* 一次、二次值类型定义 */
#define PRO_DATA_TYPE_PRIMARY   1    /* 一次值 */
#define PRO_DATA_TYPE_SECONDARY 2    /* 二次值 */

#define RCHARL(LEN)         ((LEN+3)/4)          /* length */
#define RCHAR               UNSIGNED             /* data type */

/* Define constants local to this component.  */

#define DEVICE_CNAME        36                   /* device name length */
#define DEVICE_ENAME        8                    /* device name length */

#define SECTOR_NAME         21                   /* sector name length */

#define CHANNEL_SIZE        29                   /* chennel size */

#define CHL_CONSTANT_1      (float)1.0           /* constant 1 */
#define CHL_CONSTANT_10     (float)10.0          /* constant 10 */
#define CHL_CONSTANT_100    (float)100.0         /* constant 100 */
#define CHL_CONSTANT_1000   (float)1000.0        /* constant 1000 */
#define CONSTANT_PI (float)3.1415926535897932    /* pi */

#define CHL_FOURIER1_       0   /*fourier algorthem */
#define CHL_MUL_            1   /*linearity algorthem */
#define CHL_VECTOR_         2   /*vector algorthem */
#define CHL_FREQ_           3   /*frequency algorthem */
#define CHL_INT_            4   /*integer algorthem */
#define CHL_ANG_            5   /*angle algorthem */
#define CHL_CTRL_           6   /*control word algorthem */
#define CHL_DISLOG_         7   /*logical display*/
#define CHL_RATA_           8   /* 额定值类型 */
#define CHL_INT_CTRL_       9   /* 整型控制字类型(0/1) */
#define CHL_ENUM_           10  /* enumerate type */
#define CHL_HEX_INT_        11  /* 16进制整型数据 */
#define CHL_UINT_           12  /* 无符号整型 */

#define DC_COMPONENT        (1 << 0)             /*direct current */
#define FUNDAMENTAL         (1 << 1)             /*fundamental */
#define HARMONIC_2ND        (1 << 2)             /*2nd hormonic */
#define HARMONIC_3RD        (1 << 3)             /*3rd hormonic */
#define HARMONIC_5TH        (1 << 4)             /*5th hormonic */
#define HFUNDAMENTAL        (1 << 5)             /*fundamental */
#define CHL_WAVE            (1 << 6)             /*channel wave enable */
#define CHL_COEFFICIENT     (1 << 7)             /*channel coefficient enable*/
#define CHL_RATE            (1 << 8)             /*channel rate enable*/
#define CHL_EXCURSION       (1 << 9)             /*channel excursion enable*/
#define CHL_PHASE           (1 << 10)            /*channel phase enable*/
#define CHL_VOLTAGE         (1 << 11)            /*voltage channel */
#define CHL_CURRENT         (1 << 12)            /*current channel */
#define CHL_POLARITY        (1 << 13)            /*channel polarity*/
#define CHL_PEAKVALUE       (1 << 14)            /*channel peak value(日立直流输电)*/
#define HARMONIC_7TH        (1 << 15)            /*7th hormonic(日立直流输电)*/

#define SET_NAME            21                   /* set name length */
#define SET_DIMENSION_NAME  5                    /* set dimension name */
#define SET_ALIAS           9                    /* set alia */
#define SET_DO_NAME         65                   /* set do name */
#define SET_NO_CHANNEL      -1                   /* set no channel */

#define VALUE_NAME          21                   /* value name length */
#define VALUE_DIMENSION     5                    /* value dimension name */
#define VALUE_ALIAS         9                    /* value alia */
#define VALUE_DO_NAME       65                   /* value do name */

#define SOE_NAME            31                   /* soe name length */
#define JUMPER_NAME         21                   /* jumper name length */
#define JUMPER_ALIAS        9                    /* jumper alia */

#define LED_NAME            21                   /* LED reset name length */
#define LED_ALIAS           9                    /* LED reset alia */

#define TRIP_NAME           21                   /* trip name length */
#define TRIP_ALIAS          9                    /* trip alia */
#define SIGNAL_NAME         21                   /* signal name length */
#define SIGNAL_ALIAS        9                    /* signal alia */

#define BREAK_NAME          21                   /* breaker name length */
#define BREAK_ALIAS         9                    /* breaker alia */

/* state point type */
#define POINT_TYPE_SINGLE   1                    /* signal point */
#define POINT_TYPE_DUAL     2                    /* dual point */

#define SINGLE_POINT_OPT    1                    /* 单点动作状态 */
#define SINGLE_POINT_RET    0                    /* 单点返回状态 */
#define DUAL_POINT_OPT      2                    /* 双点动作状态 */
#define DUAL_POINT_RET      1                    /* 双点返回状态 */

#define RSTATE_NAME         21                   /* status name length */
#define RSTATE_ALIAS        9                    /* statuse alia */

#define DO_NAME             65                   /* do name */
#define DZ_NAME             16                   /* dzName */

#define ALARM_EVENT_ID      0x5a                 /* alarm event ID */
#define RESERVE_EVENT_ID    0x5b                 /* reserve event ID */
#define OPERATE_EVENT_ID    0x5c                 /* operate event ID */

#define FCK_INFO_MAX_NAME   21                   /* FCK信息(测控单点,测控双点,测控档位,电度)名称长度 */

#define SOUTPUT_MAX_NAME    21                   /* 数字开出量名称长度 */

#define PROTECT_DISABLE     1                    /* protect disable */
#define PROTECT_ENABLE      2                    /* protect enable */

#define REPORT_SOE_FLAG     (1 << 0)             /* report soe flag(1-fill soe information) */
#define REPORT_STATE_FLAG   (1 << 1)             /* report signal flag(1-fill state information) */
#define REPORT_SCL_FLAG     (1 << 2)             /* short_circuit location */
#define REPORT_START_FLAG   (1 << 3)             /* start report flag */
#define REPORT_LOCAL_FLAG   (1 << 4)             /* local report flag */
#define REPORT_FAULT_FLAG   (1 << 5)             /* 故障标志 */
#define REPORT_LN_STR_FLAG  (1 << 6)             /* 逻辑节点启动（分启动）标志 */
#define REPORT_KEEP_T_FLAG  (1 << 7)             /* 故障持续时间 */

/* cpu type */
#define CPU_TYPE_STARTUP    (1 << 0)             /* cpu type(startup) */
#define CPU_TYPE_TRIP       (1 << 1)             /* cpu type(trip) */
#define CPU_TYPE_SERIAL     (1 << 2)             /* cpu type(serial) */

/* function select */
#define FUN_PRO             (1 << 0)             /* protect select */
#define FUN_FCK             (1 << 1)             /* fck select */
#define FUN_FREQUENCY       (1 << 2)             /* frequency track select */
#define FUN_SETTING_COPY    (1 << 3)             /* setting copy select */
#define FUN_DATA_EXCHANGE   (1 << 4)             /* data exchange select */
#define FUN_REPORT          (1 << 5)             /* report select */
#define FUN_WAVE            (1 << 6)             /* wave select */
#define FUN_BACKGROUND      (1 << 7)             /* BackGround select */
#define FUN_OPTICAL         (1 << 8)             /* optical select */
#define FUN_EXTEND          (1 << 9)             /* extended port select */
#define FUN_INPUT_NKR809    (1 << 10)            /* input select */
#define FUN_IED_DEVICE      (1 << 11)            /* Intelligent Electronic Device  */
#define FUN_IED_FIFO        (1 << 12)            /* FIFO */
#define FUN_BASE_START_UP   (1 << 13)            /* Base start up loop */
#define FUN_PRINT           (1 << 14)            /* 打印功能选择 */

#define PRO_USER_NUMBER     30                   /* the user data size */
#define PRO_EXCHANGE_NUMBER 8                    /* the number of user exchange data */

/* error bits */
#ifndef XJ_ERROR_EVENTBITS
#define XJ_ERROR_EVENTBITS
#define Error_SRAM          (1 << 0)             /* EEPROM error */
#define Error_RAM           (1 << 1)             /* RAM error */
#define Error_Setting       (1 << 2)             /* setting error */
#define Error_Relay         (1 << 3)             /* relay error（断线或击穿） */
#define Error_5VPower       (1 << 4)             /* 5v power error */
#define Error_CPU_Number    (1 << 5)             /* CPU number error */
#define Error_AD            (1 << 6)             /* A/D error */
#define Error_Commu         (1 << 7)             /* communication error */
#define Error_FLASH         (1 << 8)             /* flash error */
#define Error_Program       (1 << 9)             /* program error */
#define Error_Relay_Close   (1 << 10)            /* relay strike through error（击穿） */
#define Error_Extend_Port   (1 << 11)            /* extended port error */
#define Close_Relay_Flag    (1 << 12)            /* power error close relay flag */
#define Sample_Data_OK      (1 << 13)            /* sample data ok */
#define User_Alarm_Bits     (1 << 14)            /* user alarm bits */
#define GWS_InitStatus      (1 << 15)            /* gws初始状态 */
#endif

/* communication states */
#define COM_DATA_EMPTY      0x00000000           /* 空数据 */
#define COM_DATA_FULL       0x00000001           /* 0-无数据 1-有数据 */
#define COM_DATA_FIRST      0x00000002           /* 0-二级数据 1-一级数据 */
#define COM_DATA_CONTINUE   0x00000004           /* 0-无后续帧 1-有后续帧 */
#define COM_DATA_ERROR      0x00000008           /* 1-找不到指定数据 */
#define COM_DATA_TIMEOUT    0x00000010           /* 超时出错 */

/* 录波状态定义 */
#define WAVE_TYPE_SOF_TP    (1 << 0)             /* SOF:TP */
#define WAVE_TYPE_SOF_TM    (1 << 1)             /* SOF:TM */
#define WAVE_TYPE_SOF_TEST  (1 << 2)             /* SOF:TEST */
#define WAVE_TYPE_SOF_OTEV  (1 << 3)             /* SOF:OTEV */
#define WAVE_TYPE_COEFFECT  (1 << 4)             /* 有效值录波 */
#define WAVE_TYPE_START     (1 << 5)             /* 启动录波 */
#define WAVE_TYPE_START_FAULT (1 << 6)           /* 起始电网故障标志 */

/* 控制模式定义 */
#define CTLMODEL_STATUS_ONLY     0               /* status-only */
#define CTLMODEL_DIRECT_NORMAL   1               /* direct-with-normal-security */
#define CTLMODEL_SBO_NORMAL      2               /* sbo-with-normal-security */
#define CTLMODEL_DIRECT_ENHANCED 3               /* direct-with-enhanced-security */
#define CTLMODEL_SBO_ENHANCED    4               /* sbo-with-enhanced-security */

/* 品质类型定义 */
#define QUALITY_GOOD             0               /* good */
#define QUALITY_INVALID          1               /* invalid */
#define QUALITY_RESERVED         2               /* reserved */
#define QUALITY_QUESTIONABLE     3               /* questionable */
//
#define QUALITY_PROCESS          0               /* process */
#define QUALITY_SUBSTITUTED      1               /* substituted */
//
#define QUALITY_VALID_GOOD           (QUALITY_GOOD         << 14) /* good */
#define QUALITY_VALID_INVALID        (QUALITY_INVALID      << 14) /* invalid */
#define QUALITY_VALID_RESERVED       (QUALITY_RESERVED     << 14) /* reserved */
#define QUALITY_VALID_QUESTIONABLE   (QUALITY_QUESTIONABLE << 14) /* invalid */
#define QUALITY_VALIDITY_MASKS \
 (QUALITY_VALID_GOOD | QUALITY_VALID_INVALID | QUALITY_VALID_RESERVED | QUALITY_VALID_QUESTIONABLE) /* masks */
#define QUALITY_DETAIL_OVERFLOW      (1 << 13)                    /* overflow */
#define QUALITY_DETAIL_OUT_RANGE     (1 << 12)                    /* outOfRange */
#define QUALITY_DETAIL_BAD_REFERENCE (1 << 11)                    /* badReference */
#define QUALITY_DETAIL_OSCILLATORY   (1 << 10)                    /* oscillatory */
#define QUALITY_DETAIL_FAILURE       (1 << 9)                     /* failure */
#define QUALITY_DETAIL_OLD_DATA      (1 << 8)                     /* oldData */
#define QUALITY_DETAIL_INCONSISTENT  (1 << 7)                     /* inconsistent */
#define QUALITY_DETAIL_INACCURATE    (1 << 6)                     /* inaccurate */
#define QUALITY_SOURCE_PROCESS       (QUALITY_PROCESS     << 5)   /* process */
#define QUALITY_SOURCE_SUBSTITUTED   (QUALITY_SUBSTITUTED << 5)   /* invalid */
#define QUALITY_SOURCE_MASKS         (QUALITY_SOURCE_PROCESS | QUALITY_SOURCE_SUBSTITUTED) /* source masks */
#define QUALITY_TEST                 (1 << 4)                     /* test */
#define QUALITY_OPERATOR_BLOCKED     (1 << 3)                     /* operatorBlocked */

/* define data struct ID */
#define PRO_STRUCT_TRANSFER_ID 1                 /* TRANSFER struct */

/* 时间定义 */
#ifndef XJ_TIME_MODULE
#define XJ_TIME_MODULE
/* 对时时间类型定义 */
#define CTM_TYPE_GPS_SECOND       (1 << 0) /* 秒脉冲对时 */
#define CTM_TYPE_GPS_MINUTE       (1 << 1) /* 分脉冲对时 */
#define CTM_TYPE_GPS_IRIGB        (1 << 2) /* IRIG-B码对时 */
#define CTM_TYPE_NETWORK          (1 << 3) /* 网络对时 */
#define CTM_TYPE_61850            (1 << 4) /* 61850对时 */
#define CTM_TYPE_1588             (1 << 5) /* 1588对时 */

/* 时间品质定义 */
#define CTM_TIME_ACCURACY         0        /* 装置时间准确度选择 */
#define CTM_Q_LEAP_SECOND         (1 << 7) /* 闰秒 */
#define CTM_Q_CLOCK_FAILURE       (1 << 6) /* 时钟故障 */
#define CTM_Q_CLOCK_NO_SYN        (1 << 5) /* 时钟未同步 */
#define CTM_Q_TIME_ACCURACY(x)    (x << CTM_TIME_ACCURACY) /* 时间准确度 */

#define PROFIBUS_MODE             0x1  /* PROFIBUS时间转换成装置时间 */
#define UTC_MODE                  0x2  /* UTC时间转换成装置 */

/* 装置时间结构 */
typedef struct TIME_STRUCT
{
    UNSIGNED    Year;                                       // year
    UNSIGNED    Month;                                      // month
    UNSIGNED    Day;                                        // day
    UNSIGNED    Hour;                                       // hour
    UNSIGNED    Minute;                                     // minute
    UNSIGNED    Millionsecond;                              // millionsecond
    UNSIGNED    Nanosecond;                                 // nanosecond
    UNSIGNED    Quality;                                    // quality
}TIME;

#endif

/* Define the adjust data type for synchronization.  */

typedef struct UserAdjustSyn_Struct
{
    INT         iAdjustSynNumFlag;
    INT         iAdjustSynNum;
    INT         iAdjustMicroSynFlag;
    INT         iAdjustMicroSynStep;
    INT         iAdjustMicroSynNum;
    INT         iAdjustMicroSynStatus;
}gUserAdjustSynStruct;

typedef struct UserDataExchange_Struct
{
    UNSIGNED    uReceiveSysTime;        /* 接收数据时间：毫秒 */
    UNSIGNED    uReceiveCrlTime;        /* 接收数据时间：微妙 */
    INT         iReceiveNum;
    UNSIGNED    iRecSampleSerial;       /* 接收的采样数据对应的采样序号 */
    UNSIGNED    uSendSysTime;           /* 发送时间：毫秒 */
    UNSIGNED    uSendCrlTime;           /* 发送时间：微妙 */
    INT         iSendNum;
    UNSIGNED    iSendSampleSerial;      /* 发送的采样数据对应的采样序号 */
    UNSIGNED    uSendTimeLapse;         /* 发送延时时间 */
}gUserDataExchangeStruct;

typedef struct Stru803CommFrame_Struct
{
    INT         iAnalogNumber;
    INT         * pAnalogChannel;
    INT         iSendType;
    INT         iUserDataNumber;
    VOID        * pUserData;
}Stru803CommFrame;

/* 光纤用户数据结构 */
typedef struct OGB_STRUCT
{
    CHAR        cFrameHeadL;      /* 帧头低字节 */
    CHAR        cFrameHeadH;      /* 帧头高字节 */
    CHAR        cFrameEnd;        /* 帧结束字节 */
    VOID *      pUserData;        /* 用户数据首址 */
    VOID *      pRxdArea;         /* 接收缓冲区首址 */
    UNSIGNED    uRxdSize;         /* 接收缓冲区大小 */
    VOID *      pRxdIn;           /* 接收数据指针(IN) */
    VOID *      pRxdOut;          /* 接收数据指针(OUT) */
    UNSIGNED    uTxdSpace1;       /* 发送数据间距1(采样点数) */
    UNSIGNED    uTxdSpace2;       /* 发送数据间距2(采样点数) */
    UNSIGNED    uTxdTime1;        /* 字符发送时间1 */
    UNSIGNED    uTxdTime2;        /* 字符发送时间2 */
    SIGNED      sSampleMin;       /* 采样最小值 */
    SIGNED      sSampleMax;       /* 采样最大值 */
    UNSIGNED    uSampleSize;      /* 通道采样缓冲区大小 */
    float       * pSampleStartAddr; /* 采样模拟量数据起始地址 */
    UNSIGNED    * pSPulseStartAddr; /* 采样序号起始地址 */
}OGB;

/* 录波通道数据结构 */
typedef struct WAVE_CHANNELS_STRUCT
{
    INT         AnalogNumber;                    /* 模拟量通道个数 */
    INT         * pAnalogNumber;                 /* 模拟量通道号码指针 */
    INT         InputNumber;                     /* 开入量组数 */
    INT         * pInputNumber;                  /* 开入量组号指针 */
    INT         * pInputMasks;                   /* 开入量掩码指针 */
    INT         OutputNumber;                    /* 开出量组数 */
    INT         * pOutputNumber;                 /* 开出量组号指针 */
    INT         * pOutputMasks;                  /* 开出量掩码指针 */
    INT         StateNumber;                     /* 状态量组数 */
    INT         * pStateNumber;                  /* 状态量组号指针 */
    INT         * pStateMasks;                   /* 状态量掩码指针 */
    INT         LogicalNumber;                   /* 逻辑状态量组数 */
    INT         * pLogicalNumber;                /* 逻辑状态量组号指针 */
    INT         * pLogicalMasks;                 /* 逻辑状态量掩码指针 */
}WAVE_CHANNEL;

/* 电源通道配置参数. */
typedef struct CFG_POWER_STRUCT
{
    /* 电源通道号 */
    INT ChannelNumber;
    /* 电源上限量值 */
    float MaxValue;
    /* 电源下限量值 */
    float MinValue;
}CFG_POWER;

/* 装置模拟量通道基本数据结构 */
typedef struct ANALOGSET_STRUCT
{
    INT         AD_Number;                       /* 使用AD片数 */
    INT         DSampleSVChlNum;                 /* 双采样SV通道个数 */
    INT         Analog_Number;                   /* 模拟量通道总个数（物理通道+虚通道） */
    INT         Virtual_Analog_Number;           /* 虚拟的模拟量通道数 */
    INT         Sample_Point;                    /* 每周期采样点数 */
    float       LineFrequency;                   /* 线路频率 */
    float       SampleRate;                      /* 采样率(每秒采样点数) */
    INT         Frequency_Num1;                  /* 测频电压通道1(-1 = 无测频) */
    INT         Frequency_Num2;                  /* 测频电压通道2(-1 = 无测频) */
    float       FrequencyDlt;                    /* 跟踪频率最大变化率(HZ/秒) */
    INT         PowerNumber;                     /* 自检电源通道个数 */
    CFG_POWER   * PowerChannel;                  /* 自检电源通道信息 */
}ANALOGSET;

/* 装置基本参数设置 */
typedef struct SET_DEVPARAM_STRUCT
{
    UNSIGNED    Cpu_Number;                      /* CPU号 */
    UNSIGNED    Cpu_Type;                        /* CPU板类型 */
    UNSIGNED    Fun_Select;                      /* 功能选择 */
    UNSIGNED    Device_Fun;                      /* 装置功能类型 */
    UNSIGNED    MinSetZone;                      /* 装置起始定值区号 */
    UNSIGNED    SetZoneCount;                    /* 装置定值区个数 */
    UNSIGNED    Max_EventRecordGroup;            /* 最大的记录事件组数 */
    UNSIGNED    Max_Event_Size;                  /* 最大的事件缓冲区大小 */
    UNSIGNED    Max_Report_Size;                 /* 最大的报告缓冲区大小 */
    UNSIGNED    StringType;                      /* 保护提供的字符串类型 */
    INT         Max_Input;                       /* 最大开入量个数 */
    INT         Max_Output;                      /* 最大开出量个数 */
    INT         Max_State;                       /* 最大保护录波状态量个数 */
    UNSIGNED    Hard_Input_Group;                /* 硬件开入量组数 */
    UNSIGNED    Soft_Input_Group;                /* 软件开入量组数 */
    UNSIGNED    Led_Frequency;                   /* 装置运行灯闪烁频率 */
    UNSIGNED    Led_NormalStatus;                /* 装置运行灯正常状态 */
    UNSIGNED    Led_StartStatus;                 /* 装置运行灯启动状态 */
    UNSIGNED    Led_AbnorStatus;                 /* 装置运行灯异常状态 */
}SETDEVPARAM;

/* 装置出口位设置结构体 */
typedef struct SET_OUTBITS_STRUCT
{
    UNSIGNED    Hard_Output_Group;/* 硬件开出量组数 */
    UNSIGNED    Soft_Output_Group;/* 软件开出量组数 */
    UNSIGNED    Out_Check_Bits;   /* 装置出口检测位(0-31) */
    UNSIGNED    Out_Check_Bits1;  /* 装置出口检测位(32-63) */
    UNSIGNED    Out_Check_Bits2;  /* 装置出口检测位(64-95) */
    UNSIGNED    Chl_Trip_Bits;    /* 按通道传动出口附加动作位 */
    UNSIGNED    Run_Led_Bits;     /* 运行灯动作位 */
    UNSIGNED    Alarm_Led_Bits;   /* 告警灯动作位 */
    UNSIGNED    Signal_Led_Bits;  /* 信号灯动作位 */
    UNSIGNED    Trip_Led_Bits;    /* 跳闸灯动作位 */
    UNSIGNED    Alarm_Relay_Bits; /* 告警继电器动作位 */
    UNSIGNED    Reset_Relay_Bits; /* 复归继电器动作位 */
    UNSIGNED    Reset_Input_Bits; /* 复归继电器开入位 */
    INT         Hard_Extend_Count; /* 扩展插件个数 */
}SETOUTBITS;

/* 数据表参数个数 */
typedef struct SET_TABNUMBER_STRUCT
{
    INT    Max_Table_Number;  /* 最大数据表个数 */
    INT    Max_Field_Number;  /* 最大字段个数 */
    INT    Max_Sg_Number;     /* 最大整定值个数 */
}SETTABNUMBER;

/* 模块装载定义 */
#ifndef XJ_LOAD_MODULE
#define XJ_LOAD_MODULE
typedef struct APP_ENTRY_STRUCT
{
    UNSIGNED    Key1;
    INT         (* LinkApplication )( VOID * PlatFuncList );
    INT         (* IniApplication )( VOID * mem_pool, VOID * uncached_mem_pool );
    INT         (* LeadApplication )( VOID );
    UNSIGNED    Key2;
    UNSIGNED    CRC;
}APP_ENTRY;

typedef long HAPP; /* 应用程序对象句柄 */
#endif

#ifndef XJ_CLEC_MODULE
#define XJ_CLEC_MODULE
#define CLE_LED_EXTINGUISH        0x1 /* 装置运行灯熄灭状态 */
#define CLE_LED_CORUSCATE         0x2 /* 装置运行灯闪烁状态 */
#define CLE_LED_LIGHT             0x4 /* 装置运行灯常亮状态 */

#define CLE_LED_START             0x1 /* 运行灯启动状态 */
#define CLE_LED_ABNORMAL          0x2 /* 运行灯异常状态 */
#define CLE_LED_NORMAL            0x4 /* 运行灯正常状态 */

typedef long HLEM;                /* 应用程序对象句柄 */
#endif

/* 开出管理模块 */
#ifndef XJ_CIOC_MODULE
#define XJ_CIOC_MODULE
#define CIO_OR                  1 /* 或 */
#define CIO_AND                 2 /* 与 */
#define CIO_EQU                 4 /* 等于 */

typedef long HIOM; /* 应用程序对象句柄 */
#endif

/* 保护类型定义 */
#ifndef XJ_SCHDULE_MODULE
#define XJ_SCHDULE_MODULE
#define CAS_MAIN_PROTECT        1  /* 主保护任务 */
#define CAS_BACKUP_PROTECT      2  /* 后备保护任务 */

typedef long HASH; /* 应用程序对象句柄 */
#endif

#define TASK_BACKUP_PROTECT_ID  CAS_BACKUP_PROTECT

#ifndef XJ_CHANNEL_MODULE
#define XJ_CHANNEL_MODULE

#define CHL_DEVICE_NAME     11              /* channel device name length */
#define CHL_MAX_NAME        21              /* channel name length */
#define CHL_DIMENSION       5               /* channel dimension length */
#define CHMC_RCHARL(LEN)    ((LEN+3)/4)     /* length */
#define CHMC_RCHAR          UNSIGNED        /* data type */

/* 平台需要的通道结构 */
typedef struct PRO_CHANNEL_STRUCT
{
    UNSIGNED    chl_Num;                                          /* channel number */
    UNSIGNED    chl_inf;                                          /* channel information number */
    CHMC_RCHAR  chl_device_name[CHMC_RCHARL( CHL_DEVICE_NAME )];  /* save channel device name */
    CHMC_RCHAR  chl_name[CHMC_RCHARL( CHL_MAX_NAME )];            /* save channel name */
    UNSIGNED    chl_type;                                         /* channel type */
    INT         chla_calculate_style;                             /* channel value calculate style (main protect) */
    INT         chlb_calculate_style;                             /* channel value calculate style (backup protect) */
    CHMC_RCHAR  chl_primary_dimension[CHMC_RCHARL( CHL_DIMENSION )];   /* primary value dimension */
    CHMC_RCHAR  chl_secondary_dimension[CHMC_RCHARL( CHL_DIMENSION )]; /* secondary value dimension */
    float       chl_calculate_coefficient;                        /* channel value calculate coefficient */
    float       chl_sample_coefficient;                           /* channel sample coefficient */
    float       chl_channel_coefficient;                          /* channel coefficient */
    float       chl_channel_excursion;                            /* channel excursion */
    INT         chl_correlation_channel;                          /* correlation channel high 16 bits B phase */
    UNSIGNED    chl_primary_id;                                   /* primary rate ID */
    UNSIGNED    chl_secondary_id;                                 /* secondary rate ID */
    float       chl_primary_rate;                                 /* primary rate value */
    float       chl_secondary_rate;                               /* secondary rate value */
    float       chl_calculate_rate;                               /* secondary calculate rate value */
    float       chl_rate_coefficient;                             /* channel rate coefficient */
    float       chl_channel_phase;                                /* channel phase */
    float       chl_virtual_value;                                /* channel virtual value */
    float       chl_phase_factor3;                                /* phase coefficient3 */
    CHAR        dzName[DZ_NAME];                                  /* dzName */
    float       chl_wave_coefficient;                             /* wave coefficient */
    UNSIGNED    desc_id;                                          /* 描述定值ID */
    float       chl_wave_conver_coef;                             /* 录波一/二次转换系数 */
    INT         vldTag;

    float       chla_DC_component;                                // save D.C component value
    float       chla_fundamental_real;                            // save fundamental real
    float       chla_fundamental_imaginary;                       // save fundamental imaginary
    float       chla_2nd_harmonic_real;                           // save second harmonic real
    float       chla_2nd_harmonic_imaginary;                      // save second harmonic imaginary
    float       chla_3rd_harmonic_real;                           // save third harmonic real
    float       chla_3rd_harmonic_imaginary;                      // save third harmonic imaginary
    float       chla_5th_harmonic_real;                           // save fifth harmonic real
    float       chla_5th_harmonic_imaginary;                      // save fifth harmonic imaginary
    float       chla_hfundamental_real;                           // save fundamental real
    float       chla_hfundamental_imaginary;                      // save fundamental imaginary

    float       chlb_DC_component;                                // save D.C component value
    float       chlb_fundamental_real;                            // save fundamental real
    float       chlb_fundamental_imaginary;                       // save fundamental imaginary
    float       chlb_2nd_harmonic_real;                           // save second harmonic real
    float       chlb_2nd_harmonic_imaginary;                      // save second harmonic imaginary
    float       chlb_3rd_harmonic_real;                           // save third harmonic real
    float       chlb_3rd_harmonic_imaginary;                      // save third harmonic imaginary
    float       chlb_5th_harmonic_real;                           // save fifth harmonic real
    float       chlb_5th_harmonic_imaginary;                      // save fifth harmonic imaginary
    float       chlb_hfundamental_real;                           // save fundamental real
    float       chlb_hfundamental_imaginary;                      // save fundamental imaginary
}PRO_CHANNEL;
#endif

#ifndef XJ_CNUC_MODULE
#define XJ_CNUC_MODULE
typedef long HPRM;  /* nucleus数据保护对象句柄 */
typedef long HTMM;  /* nucleus定时器管理对象句柄 */
typedef long HEVM;  /* nucleus事件位管理对象句柄 */
typedef long HMGM;  /* nucleus内存池管理对象句柄 */
typedef long HQUEM; /* nucleus消息队列管理对象句柄 */

/* 消息队列名称定义 */
#define PRO_TAB_DATA_SUDDEN    "ProTabDataSudden"    /* 应用程序数据表突变数据 */
#endif

/* 创建数据表模块 */
#ifndef XJ_TABLE_MODULE
#define XJ_TABLE_MODULE
#define TYPE_NAME               20  /* 数据类型名称长度 */
#define TABLE_NAME              12  /* 表名称长度 */
#define FIELD_NAME              12  /* 字段名称长度 */
#define MAX_CONDITION           3   /* 操作字段的最大限制条件个数 */
#define MAX_HANDLE              4   /* 最大操作字段个数 */
/* 字段属性定义 */
#define FIELD_CONFIG_ENABLE     1   /* 字段配置选项 */
#define FIELD_SETTING_ENABLE    2   /* 字段整定选项 */
#define FIELD_CRC_ENABLE        4   /* 字段CRC选项 */
#define FIELD_VALUE_ENABLE      8   /* 字段量值(包括整定值)选项 */
#define FIELD_CHECK_ENABLE      0x10 /* 字段边界检查选项 */
/* 数据类型定义 */
#define TYPE_CHAR               1   /* char */
#define TYPE_UNSIGNED_CHAR      2   /* unsigned char */
#define TYPE_INT                3   /* int */
#define TYPE_SIGNED             4   /* long */
#define TYPE_UNSIGNED           5   /* unsigned long */
#define TYPE_FLOAT              6   /* float */
/* 修改整定值选项 */
#define SETTING_ALL_ENABLE      1   /* 全部整定值 */
#define SETTING_ONE_ENABLE      2   /* 单个整定值 */
/* 条件类型 */
#define COND_TYPE_AND           1   /* 与操作 */
#define COND_TYPE_OR            2   /* 或操作 */
#define COND_TYPE_EQU           3   /* 等于操作 */
/* 数据表类型 */
#define TABLE_TYPE_RELATION     (1 << 0) /* 双CPU数据表数据关联 */

/* 条件字段操作块结构定义 */
typedef struct TB_CFB_STRUCT
{
    /* 操作字段索引 */
    INT HandleIndex;
    /* 条件字段个数 */
    INT CondNumber;
    /* 条件字段索引 */
    INT CondIndex[MAX_CONDITION];
    /* 条件字段值 */
    INT CondValue[MAX_CONDITION];
    /* 条件类型 */
    INT CondType[MAX_CONDITION];
}TB_CFB;

/* 条件字段操作块结构定义 */
typedef struct TB_CHB_STRUCT
{
    /* 操作字段个数 */
    INT HandleNumber;
    /* 操作字段索引 */
    INT HandleIndex[MAX_HANDLE];
    /* 条件字段个数 */
    INT CondNumber;
    /* 条件字段索引 */
    INT CondIndex[MAX_CONDITION];
    /* 条件字段值 */
    INT CondValue[MAX_CONDITION];
    /* 条件类型 */
    INT CondType[MAX_CONDITION];
}TB_CHB;

typedef long HTAB;                /* 应用程序对象句柄 */
#endif

/* 硬件信息编码标识 */
#ifndef XJ_IDC_INFO
#define XJ_IDC_INFO
#define XJ_IDC_NPU808        0x0011 /* NPU808 */
#define XJ_IDC_NPU816        0x0800 /* NPU816 */
#define XJ_IDC_NPU823        0x1001 /* NPU823 */
#define XJ_IDC_NPU824        0x100A /* NPU824 */
#define XJ_IDC_NPU8104       0x100B /* NPU8104 */
#define XJ_IDC_NPU8507       0x100C /* NPU8507 */
#define XJ_IDC_NPU8509       0x100D /* NPU8509 */
#define XJ_IDC_NPU827        0x1008 /* NPU827 */
#define XJ_IDC_NPU8100       0x1002 /* NPU8100 */
#define XJ_IDC_NPU8101       0x1004 /* NPU8101 */
#define XJ_IDC_NPU8103       0x1006 /* NPU8103 */
#define XJ_IDC_NPI807        0x2001 /* NPI807 */
#define XJ_IDC_DTS568        0xA001 /* DTS568 */
#define XJ_IDC_NPU826        0x1007 /* NPU826 */
#endif

#ifndef XJ_FCC_MODULE
#define XJ_FCC_MODULE
#define FCC_TYPE_SOFT       (1 << 0)    /* 软件测频 */
#define FCC_TYPE_HARD_BASE  (1 << 1)    /* 基本硬件测频 */
#define FCC_TYPE_HARD_EXT   (1 << 2)    /* 扩展硬件测频 */
#define FCC_TYPE_BOUNDARY   (1 << 3)    /* 频率边界检测 */

/* 硬件频率类型定义 */
#define  FCC_HARD_BASE_FREQ 1           /* 硬件基本频率 */
#define  FCC_HARD_EXT_FREQ  2           /* 硬件扩展频率 */

typedef long HFMM; /* 应用程序对象句柄 */
#endif

/* 开入量管理模块 */
#ifndef XJ_INPUT_MODULE
#define XJ_INPUT_MODULE

#define INPUT_WAVE          1               /* wave record */
#define INPUT_NAME          31              /* input name length */
#define INPUT_ALIAS         9               /* input alia */
#define CIPC_RCHARL(LEN)    ((LEN+3)/4)     /* length */
#define CIPC_RCHAR          UNSIGNED        /* data type */

/* 开入结构 */
typedef struct INPUT_STRUCT
{
    UNSIGNED    input_id;                             /* input ID */
    UNSIGNED    desc_id;                              /* 描述定值ID */
    UNSIGNED    input_available;                      /* available words */
    UNSIGNED    input_protect_group;                  /* protect group mask bit */
    UNSIGNED     input_sub_protect;                   /* Mask of sub-group bit */
    UNSIGNED    input_inf;                            /* input inf */
    UNSIGNED    input_variinf;                        /* input changeinf */
    CIPC_RCHAR  input_name[CIPC_RCHARL( INPUT_NAME )];     /* input name */
    CIPC_RCHAR  input_alias[CIPC_RCHARL( INPUT_ALIAS )];   /* input alias */
    INT         input_type;                           /* input type */
    UNSIGNED    input_bits;                           /* input bits */
    CHAR        dzName[DZ_NAME];                      /* dzName */
    INT         value;                                /* value */
    INT         vldTag;
    INT         group;
}INPUT;
#endif

/* 开出量管理模块 */
#ifndef XJ_OUTPUT_MODULE
#define XJ_OUTPUT_MODULE

#define OUTPUT_WAVE         1             /* wave record */
#define OUTPUT_NAME         31            /* output name length */
#define OUTPUT_ALIAS        9             /* output alia */
#define COPC_RCHARL(LEN)    ((LEN+3)/4)   /* length */
#define COPC_RCHAR          UNSIGNED      /* data type */

/* 数据结构 */
typedef struct OUTPUT_STRUCT
{
    UNSIGNED    output_id;                             /* output ID */
    UNSIGNED    desc_id;                               /* 描述定值ID */
    UNSIGNED    output_available;                      /* available words */
    UNSIGNED    output_protect_group;                  /* protect group mask bit */
    UNSIGNED    output_sub_protect;                    /* Mask of sub-group bit */
    UNSIGNED    output_inf;                            /* output inf */
    UNSIGNED    output_variinf;                        /* output changeinf */
    COPC_RCHAR  output_name[COPC_RCHARL( OUTPUT_NAME )];    /* output name */
    COPC_RCHAR  output_alias[COPC_RCHARL( OUTPUT_ALIAS )];  /* output alias */
    INT         output_type;                           /* output type */
    UNSIGNED    output_bits;                           /* output bits */
    UNSIGNED    sec_id;                                /* sec_id */
    CHAR        dzName[DZ_NAME];                       /* dzName */
    INT         vldTag;
}OUTPUT;
#endif

/* 录波保护状态量管理模块 */
#ifndef XJ_STATE_MODULE
#define XJ_STATE_MODULE

#define STATE_NAME          31           /* state alia */
#define STATE_ALIAS         9            /* state name length */
#define CSTC_RCHARL(LEN)    ((LEN+3)/4)  /* length */
#define CSTC_RCHAR          UNSIGNED     /* data type */

/* 数据结构 */
typedef struct STATE_STRUCT
{
    UNSIGNED    pro_id;                                     /* Protect ID */
    UNSIGNED    sec_id;                                     /* Sector ID */
    UNSIGNED    state_id;                                   /* state ID */
    UNSIGNED    desc_id;                                    /* 描述定值ID */
    UNSIGNED    state_available;                            /* available words */
    UNSIGNED    state_protect_group;                        /* state group mask bit */
    UNSIGNED    state_sub_protect;                          /* Mask of sub-group bit */
    UNSIGNED    state_inf;                                  /* state inf */
    CSTC_RCHAR  state_soe_name[CSTC_RCHARL( STATE_NAME )];  /* state soe name */
    CSTC_RCHAR  state_alias[CSTC_RCHARL( STATE_ALIAS )];    /* state alias */
    INT         vldTag;
}STATE;
#endif

/* 检修压板数据结构 */
#ifndef XJ_RJUMPER_MODULE
#define XJ_RJUMPER_MODULE
typedef struct RJUMPER_STRUCT
{
    UNSIGNED    pro_id;                                     /* Protect ID */
    UNSIGNED    sec_id;                                     /* sector ID */
    UNSIGNED    desc_id;                                    /* 描述定值ID */
    UNSIGNED    jum_bits;                                   /* repair jumper bits */
    UNSIGNED    jum_variinf;                                /* jumper changeinf */
    RCHAR       jum_name[RCHARL( JUMPER_NAME )];            /* jumper name */
    RCHAR       jum_alias[RCHARL( JUMPER_ALIAS )];          /* jumper alias */
    INT         jum_status;                                 /* jumper status */
    CHAR        DO[DO_NAME];                                /* do name */
    UNSIGNED    q;                                          /* q */
    UNSIGNED    t_sec;                                      /* t.sec */
    UNSIGNED    t_frac;                                     /* t.tFrac */
    UNSIGNED    t_qflag;                                    /* t.qflag */
    INT         subEna;                                     /* subEna */
    INT         subVal;                                     /* subVal */
    UNSIGNED    subQ;                                       /* subQ */
    CHAR        subID[DO_NAME];                             /* subID */
    INT         dataType;                                   /* data type */
    UNSIGNED    available;                                  /* available */
    INT         vldTag;
}RJUMPER;
#endif

/* 录波表数据结构 */
#ifndef XJ_WAVE_RCD_MODULE
#define XJ_WAVE_RCD_MODULE
typedef struct WAVE_RCD_STRUCT
{
    UNSIGNED    secId;                                      /* 扇区ID */
    UNSIGNED    proId;                                      /* 保护ID */
    INT         rcdMade;                                    /* 录波就绪 */
    UNSIGNED    rcdMade_q;                                  /* 品质因素 */
    UNSIGNED    rcdMade_TSec;                               /* 记录时间秒数 */
    UNSIGNED    rcdMade_TFrac;                              /* 记录时间毫秒数 */
    UNSIGNED    rcdMade_TQflag;                             /* 记录时间品质因素 */
    INT         fltNum;                                     /* 故障序号 */
    UNSIGNED    fltNum_q;                                   /* 故障序号品质因素 */
    UNSIGNED    fltNum_TSec;                                /* 故障序号时间秒数 */
    UNSIGNED    fltNum_TFrac;                               /* 故障序号时间毫秒数 */
    UNSIGNED    fltNum_TQflag;                              /* 故障序号时间品质因素 */
    INT         GriFltNum;                                  /* 电网故障序号 */
    UNSIGNED    GriFltNum_q;                                /* 电网故障序号品质因素 */
    UNSIGNED    GriFltNum_TSec;                             /* 电网故障序号时间秒数 */
    UNSIGNED    GriFltNum_TFrac;                            /* 电网故障序号时间毫秒数 */
    UNSIGNED    GriFltNum_TQflag;                           /* 电网故障序号时间品质因素 */
    UNSIGNED    available;                                  /* available */
    INT         vldTag;
}WAVE_RCD;
#endif

#endif
