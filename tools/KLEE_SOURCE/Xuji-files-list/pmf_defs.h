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
/*      pmf_defs.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      PMF - Platform HMI Function                                      */
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

#ifndef PMF_DEFS_H
#define PMF_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/* 触摸屏中断向量定义 */
#define XJ_PR_IRQ0_INT_VECTOR_ID                4

/* 中断向量触发方式定义 */
#define XJ_TRIG_NOT_SUPPORTED                   0
#define XJ_TRIG_RISING_EDGE                     1
#define XJ_TRIG_FALLING_EDGE                    2
#define XJ_TRIG_LEVEL_LOW                       3
#define XJ_TRIG_LEVEL_HIGH                      4
#define XJ_TRIG_RISING_FALLING_EDGES            5
#define XJ_TRIG_HIGH_LOW_RISING_FALLING_EDGES   6

#ifndef XJ_CWDC_MODULE
#define XJ_CWDC_MODULE
#define CWD_WATCHDOG_NORMAN       0 /* 被监视任务正常 */
#define CWD_WATCHDOG_ERROR        1 /* 被监视任务异常 */
#define CWD_WATCHDOG_ENABLE       1 /* 使能监视任务 */
#define CWD_WATCHDOG_DISABLE      0 /* 禁止监视任务 */

typedef long HWDM; /* 应用程序对象句柄 */
#endif

#ifndef XJ_TIME_MODULE
#define XJ_TIME_MODULE
/* 对时时间类型定义 */
#define CTM_TYPE_GPS_SECOND       (1 << 0) /* 秒脉冲对时 */
#define CTM_TYPE_GPS_MINUTE       (1 << 1) /* 分脉冲对时 */
#define CTM_TYPE_GPS_IRIGB        (1 << 2) /* IRIG-B码对时 */
#define CTM_TYPE_NETWORK          (1 << 3) /* 网络对时 */
#define CTM_TYPE_61850            (1 << 4) /* 61850对时:实际是SNTP对时*/
#define CTM_TYPE_1588             (1 << 5) /* 1588对时 */
#define CTM_TYPE_SNTP             CTM_TYPE_61850 /*SNTP对时*/

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

#ifndef XJ_CNUC_MODULE
#define XJ_CNUC_MODULE
typedef long HPRM;  /* nucleus数据保护对象句柄 */
typedef long HTMM;  /* nucleus定时器管理对象句柄 */
typedef long HEVM;  /* nucleus事件位管理对象句柄 */
typedef long HMGM;  /* nucleus内存池管理对象句柄 */
typedef long HQUEM; /* nucleus消息队列管理对象句柄 */
#endif

/* Core processing functions.  */
/*=======================================================================*/
/* 数据表管理函数 */
typedef VOID *(* _get_table_memory )( INT TabIndex, INT * ActualNum );

/* 获取数据表大小 */
typedef INT (* _get_table_size )( INT TabIndex );

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

/* 计算循环冗余码 */
typedef INT (* _calculate_crc )
    ( const VOID * Src, UNSIGNED Size, INT BytesAddress,
    INT CrcBits, UNSIGNED InitialValue, UNSIGNED * CRC_Code );

/* 从平台获取通讯数据 */
typedef INT     (* _read_command )
    ( VOID * Buffer, UNSIGNED * actual_size, UNSIGNED uTimeOut );

/* 向平台写通讯数据 */
typedef INT     (* _write_command )
    ( VOID * Buffer, UNSIGNED actual_size, UNSIGNED uTimeOut );

/* 通信报文处理 */
typedef VOID    (* _lcc_parase_data )( VOID );

/* 设置时间函数 */
typedef INT (* _set_time )( TIME * pTime, INT iTimeZone );

/* 填充记录函数 */
typedef INT (* _fill_event )( const VOID * pName, INT iEventType );

/* 开入管理函数 */
typedef UNSIGNED (* _read_input )( INT iGroup );

/* 创建看门狗管理对象 */
typedef HWDM (* _create_watchdog_handle )
    ( UNSIGNED SetTime, CHAR * Name, UNSIGNED MonitorCtrlHandle );

/* 复位看门狗监视对象 */
typedef INT (* _reset_watchdog_object )( HWDM Handle );

/* 获取用户数据根目录路径名称函数 */
typedef const CHAR * (* _get_user_dir_name )( VOID );

/* 设置装置分模块初始化成功函数 */
typedef INT (* _set_modinit_success )( VOID );

/* 获取装置分模块初始化完成个数 */
typedef INT (* _get_modinit_count )( VOID );

/* 获取接口程序的CRC */
typedef INT (* _get_mmi_crc )( UNSIGNED * pBuff );

/* 时间转换函数 */
typedef INT (* _transform_time_from_systime )( UNSIGNED * UTCSecond,
    UNSIGNED * UTCMillionsecond, TIME * pSrcTime );

/* 接口中直接调用数据表修改函数 */
typedef INT (* _hmi_modif_table_data )( INT TableIndex, INT StartNumber, INT number,
    VOID * pCondBuf, INT iHandleNumber, VOID * pDataBuf );

/* 保存测控数据表数据 */
typedef INT (* _write_fck_table_data )( VOID ** pGsPtrs, INT iNumber );

/* 检测时间的有效性,通过iChangeFlag来决定是否修改不正确的时间(修改时设置为时间的最大值) */
typedef INT (* _time_check )( TIME * pTime , INT iChangeFlag );

/* 设置要转换的时间模式,1为PROFIBUS时间,2为UTC时间,缺省为UTC时间  */
typedef INT (* _tm_set_time_mode ) ( UNSIGNED TimeMode );

/* 装置时间转换成就地时间 */
typedef INT (* _tm_trans_dev_to_lt )( UNSIGNED * Time1, UNSIGNED * Time2, TIME * pTime );

/* 就地时间转换成装置时间 */
typedef INT (* _tm_trans_lt_to_dev )( UNSIGNED * Time1, UNSIGNED * Time2, TIME * pTime );

/* 获取装置时区 */
typedef INT (* _get_time_zone )( INT * pTimeZone );

/* 读取键盘键值 */
typedef UNSIGNED (* _get_key_value )( VOID );

/* 读取硬件标识号 */
typedef UNSIGNED (* _get_identification_code )( VOID );

/*纳秒UTC时间转换为TIME装置时间*/
typedef INT (* _transform_ns_utc_to_dev )
    ( UNSIGNED UTCSecond, UNSIGNED UTCNanoSecond, TIME * pDstTime );

/* TIME装置时间转换为纳秒UTC时间*/
typedef INT (* _transform_dev_to_ns_utc )
    ( UNSIGNED * UTCSecond, UNSIGNED * UTCNanoSecond, TIME * pSrcTime );

/*装置时间转换为msUTC时间*/
typedef INT (* _transform_dev_to_ms_utc )
    ( UNSIGNED * UTCSecond, UNSIGNED * UTCMillionsecond, TIME * pSrcTime );

/*msUTC时间转换为装置时间*/
typedef INT (* _transform_ms_utc_to_dev )
    ( UNSIGNED * UTCSecond, UNSIGNED * UTCMillionsecond, TIME * pSrcTime );

/* 日志记录 */
typedef INT (* _write_log )( CHAR * msg );

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
/*=======================================================================*/
/* 中断向量设置 */
typedef INT (* _ge_int_enable )
    ( INT vector_id, INT trigger_type, INT priority );

/* 禁止中断 */
typedef INT (* _ge_int_disable )
    ( INT vector_id );
/*=======================================================================*/
/* 读取61850时间 */
typedef INT (* _read_61850_time )( TIME * pTime, UNSIGNED * UTCSecond,
    UNSIGNED * UTCMillionsecond, UNSIGNED * Quality );
/*=======================================================================*/
/* 获取平台引导标识 */
typedef INT  (* _get_plat_lead_flag )
    ( VOID );
/*=======================================================================*/
/* 应用读取动作报告数据函数 */
typedef INT (* _user_read_report_data )
    ( INT CpuNumber , UNSIGNED ElecNumber, INT startIndex,
    INT IndexNumber, UNSIGNED Offset, UNSIGNED Size, VOID * pBuffer,
    UNSIGNED uTimeOut );
/*=======================================================================*/
/* Define the data structure for use within the core processing functions.  */
typedef struct MTRANSFER_STRUCT
{
    /*  Standard protect struct information.   Changes made
    to this area of the structure can have undesirable side effects. */
    int size;
    /* 数据表管理函数 */
    _get_table_memory                   get_table_memory;
    /* 获取数据表大小 */
    _get_table_size                     get_table_size;
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
    /* 计算循环冗余码 */
    _calculate_crc                      calculate_crc;
    /* 从平台获取通讯数据 */
    _read_command                       read_command;
    /* 向平台写通讯数据 */
    _write_command                      write_command;
    /* 通信报文处理 */
    _lcc_parase_data                    lcc_parase_data;
    /* 设置时间函数 */
    _set_time                           set_time;
    /* 填充事件记录函数 */
    _fill_event                         fill_event;
    /* 开入管理函数 */
    _read_input                         read_input;
    /* 创建看门狗管理对象 */
    _create_watchdog_handle             create_watchdog_handle;
    /* 复位看门狗监视对象 */
    _reset_watchdog_object              reset_watchdog_object;
    /* 获取用户数据根目录路径名称函数 */
    _get_user_dir_name                  get_user_dir_name;
    /* 设置装置分模块初始化成功函数 */
    _set_modinit_success                set_modinit_success;
    /* 获取装置分模块初始化完成个数 */
    _get_modinit_count                  get_modinit_count;
    /* 获取接口程序的CRC */
    _get_mmi_crc                        get_mmi_crc;
    /* 时间转换函数 */
    _transform_time_from_systime        transform_time_from_systime;
    /* 接口中直接调用数据表修改函数 */
    _hmi_modif_table_data               hmi_modif_table_data;
    /* 保存测控数据表数据 */
    _write_fck_table_data               write_fck_table_data;
    /* 时间合法性检测函数 */
    _time_check                         time_check;
    /* 设置要转换的时间模式,1为PROFIBUS时间,2为UTC时间,缺省为UTC时间  */
    _tm_set_time_mode                   tm_set_time_mode;
    /* 装置时间转换成就地时间 */
    _tm_trans_dev_to_lt                 tm_trans_dev_to_lt;
    /* 就地时间转换成装置时间 */
    _tm_trans_lt_to_dev                 tm_trans_lt_to_dev;
    /* 获取装置时区 */
    _get_time_zone                      get_time_zone;
    /* 读取键盘键值 */
    _get_key_value                      get_key_value;
    /* 读取硬件标识号 */
    _get_identification_code            get_identification_code;
    /* 纳秒UTC转换成TIME类型的装置时间 */
    _transform_ns_utc_to_dev            transform_ns_utc_to_dev;
    /* TIME类型的装置时间转换成纳秒UTC */
    _transform_dev_to_ns_utc            transform_dev_to_ns_utc;
    /* 装置时间转换为msUTC */
    _transform_dev_to_ms_utc            transform_dev_to_ms_utc;
    /* MS UTC 转换为装置时间*/
    _transform_ms_utc_to_dev            transform_ms_utc_to_dev;
    /* 日志记录 */
    _write_log                          write_log;
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
    /* 用户数据队列管理函数 */
    /* 创建数据队列管理函数 */
    _create_data_queue                  create_data_queue;
    /* 向数据队列发送数据函数 */
    _send_to_data_queue                send_to_data_queue;
    /* 从数据队列获取数据函数 */
    _receive_from_data_queue            receive_from_data_queue;
    /* 中断向量设置 */
    _ge_int_enable                      ge_int_enable;
    /* 禁止中断 */
    _ge_int_disable                     ge_int_disable;
    /* 读取61850时间 */
    _read_61850_time                    read_61850_time;
    /* 获取平台引导标识 */
    _get_plat_lead_flag                 get_plat_lead_flag;
    /* 应用读取动作报告数据函数 */
    _user_read_report_data              user_read_report_data;
}MTRANSFER;

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
