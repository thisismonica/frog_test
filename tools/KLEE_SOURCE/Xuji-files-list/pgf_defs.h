/*************************************************************************/
/*                                                                       */
/*        Copyright (c) 1999-2002 XJ ELECTRIC CO.LTD.                    */
/*                                                                       */
/* PROPRIETARY RIGHTS of Accelerated Technology are involved in the      */
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
/*      pgf_defs.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      PGF - Platform GWS Function                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains function prototypes of all functions          */
/*      accessible to other components.                                  */
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
/*      ZhideYang.      02-20-2008      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
/* Check to see if the file has been included already.  */

#ifndef PGF_DEFS_H
#define PGF_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

#define REVISION_L_2_0 /* 版本小于2.0 */

/* 以态网中断向量定义 */
#ifdef REVISION_L_2_0
/* 2.0以下版本 */
#define XJ_PR_TSEC1_TX_INT_VECTOR_ID            14
#define XJ_PR_TSEC1_RX_INT_VECTOR_ID            13
#define XJ_PR_TSEC1_ERR_INT_VECTOR_ID           12
#define XJ_PR_TSEC2_TX_INT_VECTOR_ID            11
#define XJ_PR_TSEC2_RX_INT_VECTOR_ID            10
#define XJ_PR_TSEC2_ERR_INT_VECTOR_ID           9
#else
/* 2.0及以上版本 */
#define XJ_PR_TSEC1_TX_INT_VECTOR_ID            9
#define XJ_PR_TSEC1_RX_INT_VECTOR_ID            10
#define XJ_PR_TSEC1_ERR_INT_VECTOR_ID           11
#define XJ_PR_TSEC2_TX_INT_VECTOR_ID            12
#define XJ_PR_TSEC2_RX_INT_VECTOR_ID            13
#define XJ_PR_TSEC2_ERR_INT_VECTOR_ID           14
#endif

#define XJ_PR_GPIO_INT_VECTOR_ID                51
#define XJ_PR_USB_DR_INT_VECTOR_ID              15

#define XJ_PR_FlexCANx_INT_VECTOR_ID            34

/* 中断向量触发方式定义 */
#define XJ_TRIG_NOT_SUPPORTED                   0
#define XJ_TRIG_RISING_EDGE                     1
#define XJ_TRIG_FALLING_EDGE                    2
#define XJ_TRIG_LEVEL_LOW                       3
#define XJ_TRIG_LEVEL_HIGH                      4
#define XJ_TRIG_RISING_FALLING_EDGES            5
#define XJ_TRIG_HIGH_LOW_RISING_FALLING_EDGES   6

/* NPI接口类型定义 */
#define XJ_NPI_INTERFACE_TYPE_ETH               1
#define XJ_NPI_INTERFACE_TYPE_FIFO              2

#ifndef XJ_CWDC_MODULE
#define XJ_CWDC_MODULE
#define CWD_WATCHDOG_NORMAN       0 /* 被监视任务正常 */
#define CWD_WATCHDOG_ERROR        1 /* 被监视任务异常 */
#define CWD_WATCHDOG_ENABLE       1 /* 使能监视任务 */
#define CWD_WATCHDOG_DISABLE      0 /* 禁止监视任务 */

typedef long HWDM; /* 应用程序对象句柄 */
#endif

#ifndef XJ_CNUC_MODULE
#define XJ_CNUC_MODULE
typedef long HPRM;  /* nucleus数据保护对象句柄 */
typedef long HTMM;  /* nucleus定时器管理对象句柄 */
typedef long HEVM;  /* nucleus事件位管理对象句柄 */
typedef long HMGM;  /* nucleus内存池管理对象句柄 */
typedef long HQUEM; /* nucleus消息队列管理对象句柄 */
#endif

/* 专用数据保护名称定义 */
#define PGF_LOCK_REPORT_NAME  "ReportLock"  /* 报告数据保护名称 */

/* Core processing functions.  */
/*=======================================================================*/
/* 注册函数表函数 */
typedef INT (* _register_function_list )( CHAR * Name, VOID * FunTablePtr );

/* 获取函数表函数 */
typedef VOID * (* _get_function_list )( CHAR * Name );

/* 操作系统管理函数 */
typedef OS_FUN *(* _get_os_fun_list )( VOID );

/* 获取装置当前时间函数 */
typedef INT (* _retrieve_time )( TIME * pTime );

/* 获取系统时钟函数 */
typedef UNSIGNED (* _retrieve_system_clock )( VOID );

/* 获取当前时间相对于1970年的秒数 */
typedef UNSIGNED (* _get_time_second_1970 )( VOID );

#if WIN32
/* 获取纳秒时间值 */
typedef UNSIGNED (* _get_time_nanosecond )( VOID );
#else
/* 获取纳秒时间值 */
typedef long long (* _get_time_nanosecond )( VOID );
#endif

/* 把日期和时间转换为格林尼治标准时间 */
typedef INT (* _time_to_gmt )( TIME * time );

/* 中断向量设置 */
typedef INT (* _ge_int_enable )( INT vector_id, INT trigger_type, INT priority );

/* 禁止中断 */
typedef INT (* _ge_int_disable )( INT vector_id );

/* 创建看门狗管理对象 */
typedef HWDM (* _create_watchdog_handle )
    ( UNSIGNED SetTime, CHAR * Name, UNSIGNED MonitorCtrlHandle );

/* 复位看门狗监视对象 */
typedef INT (* _reset_watchdog_object )( HWDM Handle );

/* 设置时间函数 */
typedef INT (* _set_time )( TIME * pTime, INT iTimeZone );

/* 设置61850初始化完成标志 */
typedef INT (* _gws_set_init_status )( INT  initFlag );

/* 创建数据保护 */
typedef long (* _create_lock_data )( CHAR * Name );

/* 数据保护 */
typedef INT (* _lock_data )( long Handle );

/* 释放数据保护 */
typedef INT (* _unlock_data )( long Handle );

/* 获取互斥量 */
typedef INT (* _obtain_mutex )( VOID * Mutex, UNSIGNED Timeout );

/* 释放互斥量 */
typedef INT (* _release_mutex )( VOID * Mutex );

/* 获取装置时区 */
typedef INT (* _get_time_zone )( INT * pTimeZone );

/* 软复位 */
typedef VOID (* _reset_device )( VOID );

/* 读取硬件标识号 */
typedef UNSIGNED (* _get_identification_code )( VOID );

/* 获取装置功能选择 */
typedef INT (* _get_device_fun_select )( VOID );

/* 设置61850时间 */
/* UTCSecond:UTC秒以上时间(秒),UTCNanoSecond:UTC秒以下时间(纳秒),pTime:返回装置时间 */
typedef INT (* _set_time_61850 )( UNSIGNED UTCSecond, UNSIGNED UTCNanoSecond, TIME * pTime );

/* 设置对时时间间隔 */
/* TimeInterval:对时时间间隔(毫秒),为0时不进行时钟源消失检查 */
typedef INT (* _set_time_interval )( UNSIGNED TimeInterval );

/* 设置对时时间品质因素 */
/* Quality:时间品质因素 CondType:条件操作(1-与,2-或,3-等) */
typedef INT (* _set_time_quality )( UNSIGNED Quality, INT CondType );

/* 日志记录 */
typedef INT (* _write_log )( CHAR * msg );
/*=======================================================================*/
/* 用户数据队列管理函数 */
/* 创建数据队列管理函数 */
typedef HQUEM (* _create_data_queue )
    ( CHAR * name, UNSIGNED uBufSize, UNSIGNED uMaxMesLen, UNSIGNED uDataByte );

/* 向数据队列发送数据函数 */
typedef INT  (* _send_to_data_queue )
    ( CHAR * name, HQUEM Handle, VOID * message, UNSIGNED size,
    UNSIGNED uTimeOut, INT iFrontFlag );

/* 从数据队列获取数据函数 */
typedef INT  (* _receive_from_data_queue )
    ( CHAR * name, HQUEM Handle, VOID * message, UNSIGNED maxsize,
    UNSIGNED * ActSize, UNSIGNED uTimeOut );

/* 获取数据队列信息 */
typedef INT (* _get_data_queue_information )
    ( CHAR * name, HQUEM Handle, UNSIGNED * msg_count,
    UNSIGNED * avail_mem, UNSIGNED * msg_size );

/* 删除指定数据队列 */
typedef INT (* _delete_data_queue )
    ( CHAR * name, HQUEM Handle );
/*=======================================================================*/
/* 注册UART串口中断函数 */
typedef INT (* _reg_uart_int_fun )
    ( INT channel, VOID (* UartComIntFun )( int para ));

/* UART串口初始化 */
typedef INT (* _uart_com_init )
    ( INT channel, INT baud_rate, INT parity );

/* 设置UART485使能标识 */
typedef INT (* _uart_rs485_set_enflag )
    ( int channel, int iEnFlag );

/* 向UART串口写数据 */
typedef INT (* _uart_write_comm )
    ( INT channel, UNSIGNED_CHAR * buf, INT len );

/* 从UART串口读取数据 */
typedef INT (* _uart_read_comm )
    ( INT channel, UNSIGNED_CHAR * buf );

/* 获取UART中断状态 */
/* 中断查询函数,看其所指串口是否处于接口中断状态NU_TRUE:接收中断状态,NU_FALSE:不是接收中断状态 */
typedef INT (* _uart_get_port_int_flag )
    ( INT channel );

/* 获取UART串口中断向量ID */
typedef UNSIGNED (* _get_uart_vector_id )
    ( INT channel );
/*=======================================================================*/
/* 获取平台引导标识 */
typedef INT  (* _get_plat_lead_flag )
    ( VOID );
/*=======================================================================*/

/* Define the data structure for use within the core processing functions.  */
typedef struct TRANSFER_STRUCT
{
    /*  Standard protect struct information.   Changes made
    to this area of the structure can have undesirable side effects. */
    int size;
    /* 注册函数表函数 */
    _register_function_list             register_function_list;
    /* 获取函数表函数 */
    _get_function_list                  get_function_list;
    /* 操作系统管理函数 */
    _get_os_fun_list                    get_os_fun_list;
    /* 获取装置当前时间函数 */
    _retrieve_time                      retrieve_time;
    /* 获取系统时钟函数 */
    _retrieve_system_clock              retrieve_system_clock;
    /* 获取当前时间相对于1970年的秒数 */
    _get_time_second_1970               get_time_second_1970;
    /* 获取纳秒时间值 */
    _get_time_nanosecond                get_time_nanosecond;
    /* 把日期和时间转换为格林尼治标准时间 */
    _time_to_gmt                        time_to_gmt;
    /* 中断向量设置 */
    _ge_int_enable                      ge_int_enable;
    /* 禁止中断 */
    _ge_int_disable                     ge_int_disable;
    /* 创建看门狗管理对象 */
    _create_watchdog_handle             create_watchdog_handle;
    /* 复位看门狗监视对象 */
    _reset_watchdog_object              reset_watchdog_object;
    /* 设置时间函数 */
    _set_time                           set_time;
    /* 设置61850初始化完成标志 */
    _gws_set_init_status                gws_set_init_status;
    /* 创建数据保护 */
    _create_lock_data                   create_lock_data;
    /* 数据保护 */
    _lock_data                          lock_data;
    /* 释放数据保护 */
    _unlock_data                        unlock_data;
    /* 获取互斥量 */
    _obtain_mutex                       obtain_mutex;
    /* 释放互斥量 */
    _release_mutex                      release_mutex;
    /* 获取装置时区 */
    _get_time_zone                      get_time_zone;
    /* 软复位 */
    _reset_device                       reset_device;
    /* 读取硬件标识号 */
    _get_identification_code            get_identification_code;
    /* 获取装置功能选择 */
    _get_device_fun_select              get_device_fun_select;
    /* 设置61850时间 */
    _set_time_61850                     set_time_61850;
    /* 设置对时时间间隔 */
    _set_time_interval                  set_time_interval;
    /* 设置对时时间品质因素 */
    _set_time_quality                   set_time_quality;
    /* 日志记录 */
    _write_log                          write_log;
    /* 用户数据队列管理函数 */
    /* 创建数据队列管理函数 */
    _create_data_queue                  create_data_queue;
    /* 向数据队列发送数据函数 */
    _send_to_data_queue                send_to_data_queue;
    /* 从数据队列获取数据函数 */
    _receive_from_data_queue            receive_from_data_queue;
    /* 注册UART串口中断函数 */
    _reg_uart_int_fun                   reg_uart_int_fun;
    /* UART串口初始化 */
    _uart_com_init                      uart_com_init;
    /* 设置UART485使能标识 */
    _uart_rs485_set_enflag              uart_rs485_set_enflag;
    /* 向UART串口写数据 */
    _uart_write_comm                    uart_write_comm;
    /* 从UART串口读取数据 */
    _uart_read_comm                     uart_read_comm;
    /* 获取UART中断状态 */
    /* 中断查询函数,看其所指串口是否处于接口中断状态NU_TRUE:接收中断状态,NU_FALSE:不是接收中断状态 */
    _uart_get_port_int_flag             uart_get_port_int_flag;
    /* 获取UART串口中断向量ID */
    _get_uart_vector_id                 get_uart_vector_id;
    /* 获取数据队列信息 */
    _get_data_queue_information         get_data_queue_information;
    /* 删除指定数据队列 */
    _delete_data_queue                  delete_data_queue;
    /* 获取平台引导标识 */
    _get_plat_lead_flag                 get_plat_lead_flag;
}GTRANSFER;

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
#endif

#ifdef __cplusplus
}
#endif

#endif
