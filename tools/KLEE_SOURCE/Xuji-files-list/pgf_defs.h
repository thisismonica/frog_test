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

#define REVISION_L_2_0 /* �汾С��2.0 */

/* ��̬���ж��������� */
#ifdef REVISION_L_2_0
/* 2.0���°汾 */
#define XJ_PR_TSEC1_TX_INT_VECTOR_ID            14
#define XJ_PR_TSEC1_RX_INT_VECTOR_ID            13
#define XJ_PR_TSEC1_ERR_INT_VECTOR_ID           12
#define XJ_PR_TSEC2_TX_INT_VECTOR_ID            11
#define XJ_PR_TSEC2_RX_INT_VECTOR_ID            10
#define XJ_PR_TSEC2_ERR_INT_VECTOR_ID           9
#else
/* 2.0�����ϰ汾 */
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

/* �ж�����������ʽ���� */
#define XJ_TRIG_NOT_SUPPORTED                   0
#define XJ_TRIG_RISING_EDGE                     1
#define XJ_TRIG_FALLING_EDGE                    2
#define XJ_TRIG_LEVEL_LOW                       3
#define XJ_TRIG_LEVEL_HIGH                      4
#define XJ_TRIG_RISING_FALLING_EDGES            5
#define XJ_TRIG_HIGH_LOW_RISING_FALLING_EDGES   6

/* NPI�ӿ����Ͷ��� */
#define XJ_NPI_INTERFACE_TYPE_ETH               1
#define XJ_NPI_INTERFACE_TYPE_FIFO              2

#ifndef XJ_CWDC_MODULE
#define XJ_CWDC_MODULE
#define CWD_WATCHDOG_NORMAN       0 /* �������������� */
#define CWD_WATCHDOG_ERROR        1 /* �����������쳣 */
#define CWD_WATCHDOG_ENABLE       1 /* ʹ�ܼ������� */
#define CWD_WATCHDOG_DISABLE      0 /* ��ֹ�������� */

typedef long HWDM; /* Ӧ�ó�������� */
#endif

#ifndef XJ_CNUC_MODULE
#define XJ_CNUC_MODULE
typedef long HPRM;  /* nucleus���ݱ��������� */
typedef long HTMM;  /* nucleus��ʱ����������� */
typedef long HEVM;  /* nucleus�¼�λ��������� */
typedef long HMGM;  /* nucleus�ڴ�ع�������� */
typedef long HQUEM; /* nucleus��Ϣ���й�������� */
#endif

/* ר�����ݱ������ƶ��� */
#define PGF_LOCK_REPORT_NAME  "ReportLock"  /* �������ݱ������� */

/* Core processing functions.  */
/*=======================================================================*/
/* ע�ắ������ */
typedef INT (* _register_function_list )( CHAR * Name, VOID * FunTablePtr );

/* ��ȡ�������� */
typedef VOID * (* _get_function_list )( CHAR * Name );

/* ����ϵͳ������ */
typedef OS_FUN *(* _get_os_fun_list )( VOID );

/* ��ȡװ�õ�ǰʱ�亯�� */
typedef INT (* _retrieve_time )( TIME * pTime );

/* ��ȡϵͳʱ�Ӻ��� */
typedef UNSIGNED (* _retrieve_system_clock )( VOID );

/* ��ȡ��ǰʱ�������1970������� */
typedef UNSIGNED (* _get_time_second_1970 )( VOID );

#if WIN32
/* ��ȡ����ʱ��ֵ */
typedef UNSIGNED (* _get_time_nanosecond )( VOID );
#else
/* ��ȡ����ʱ��ֵ */
typedef long long (* _get_time_nanosecond )( VOID );
#endif

/* �����ں�ʱ��ת��Ϊ�������α�׼ʱ�� */
typedef INT (* _time_to_gmt )( TIME * time );

/* �ж��������� */
typedef INT (* _ge_int_enable )( INT vector_id, INT trigger_type, INT priority );

/* ��ֹ�ж� */
typedef INT (* _ge_int_disable )( INT vector_id );

/* �������Ź�������� */
typedef HWDM (* _create_watchdog_handle )
    ( UNSIGNED SetTime, CHAR * Name, UNSIGNED MonitorCtrlHandle );

/* ��λ���Ź����Ӷ��� */
typedef INT (* _reset_watchdog_object )( HWDM Handle );

/* ����ʱ�亯�� */
typedef INT (* _set_time )( TIME * pTime, INT iTimeZone );

/* ����61850��ʼ����ɱ�־ */
typedef INT (* _gws_set_init_status )( INT  initFlag );

/* �������ݱ��� */
typedef long (* _create_lock_data )( CHAR * Name );

/* ���ݱ��� */
typedef INT (* _lock_data )( long Handle );

/* �ͷ����ݱ��� */
typedef INT (* _unlock_data )( long Handle );

/* ��ȡ������ */
typedef INT (* _obtain_mutex )( VOID * Mutex, UNSIGNED Timeout );

/* �ͷŻ����� */
typedef INT (* _release_mutex )( VOID * Mutex );

/* ��ȡװ��ʱ�� */
typedef INT (* _get_time_zone )( INT * pTimeZone );

/* ��λ */
typedef VOID (* _reset_device )( VOID );

/* ��ȡӲ����ʶ�� */
typedef UNSIGNED (* _get_identification_code )( VOID );

/* ��ȡװ�ù���ѡ�� */
typedef INT (* _get_device_fun_select )( VOID );

/* ����61850ʱ�� */
/* UTCSecond:UTC������ʱ��(��),UTCNanoSecond:UTC������ʱ��(����),pTime:����װ��ʱ�� */
typedef INT (* _set_time_61850 )( UNSIGNED UTCSecond, UNSIGNED UTCNanoSecond, TIME * pTime );

/* ���ö�ʱʱ���� */
/* TimeInterval:��ʱʱ����(����),Ϊ0ʱ������ʱ��Դ��ʧ��� */
typedef INT (* _set_time_interval )( UNSIGNED TimeInterval );

/* ���ö�ʱʱ��Ʒ������ */
/* Quality:ʱ��Ʒ������ CondType:��������(1-��,2-��,3-��) */
typedef INT (* _set_time_quality )( UNSIGNED Quality, INT CondType );

/* ��־��¼ */
typedef INT (* _write_log )( CHAR * msg );
/*=======================================================================*/
/* �û����ݶ��й����� */
/* �������ݶ��й����� */
typedef HQUEM (* _create_data_queue )
    ( CHAR * name, UNSIGNED uBufSize, UNSIGNED uMaxMesLen, UNSIGNED uDataByte );

/* �����ݶ��з������ݺ��� */
typedef INT  (* _send_to_data_queue )
    ( CHAR * name, HQUEM Handle, VOID * message, UNSIGNED size,
    UNSIGNED uTimeOut, INT iFrontFlag );

/* �����ݶ��л�ȡ���ݺ��� */
typedef INT  (* _receive_from_data_queue )
    ( CHAR * name, HQUEM Handle, VOID * message, UNSIGNED maxsize,
    UNSIGNED * ActSize, UNSIGNED uTimeOut );

/* ��ȡ���ݶ�����Ϣ */
typedef INT (* _get_data_queue_information )
    ( CHAR * name, HQUEM Handle, UNSIGNED * msg_count,
    UNSIGNED * avail_mem, UNSIGNED * msg_size );

/* ɾ��ָ�����ݶ��� */
typedef INT (* _delete_data_queue )
    ( CHAR * name, HQUEM Handle );
/*=======================================================================*/
/* ע��UART�����жϺ��� */
typedef INT (* _reg_uart_int_fun )
    ( INT channel, VOID (* UartComIntFun )( int para ));

/* UART���ڳ�ʼ�� */
typedef INT (* _uart_com_init )
    ( INT channel, INT baud_rate, INT parity );

/* ����UART485ʹ�ܱ�ʶ */
typedef INT (* _uart_rs485_set_enflag )
    ( int channel, int iEnFlag );

/* ��UART����д���� */
typedef INT (* _uart_write_comm )
    ( INT channel, UNSIGNED_CHAR * buf, INT len );

/* ��UART���ڶ�ȡ���� */
typedef INT (* _uart_read_comm )
    ( INT channel, UNSIGNED_CHAR * buf );

/* ��ȡUART�ж�״̬ */
/* �жϲ�ѯ����,������ָ�����Ƿ��ڽӿ��ж�״̬NU_TRUE:�����ж�״̬,NU_FALSE:���ǽ����ж�״̬ */
typedef INT (* _uart_get_port_int_flag )
    ( INT channel );

/* ��ȡUART�����ж�����ID */
typedef UNSIGNED (* _get_uart_vector_id )
    ( INT channel );
/*=======================================================================*/
/* ��ȡƽ̨������ʶ */
typedef INT  (* _get_plat_lead_flag )
    ( VOID );
/*=======================================================================*/

/* Define the data structure for use within the core processing functions.  */
typedef struct TRANSFER_STRUCT
{
    /*  Standard protect struct information.   Changes made
    to this area of the structure can have undesirable side effects. */
    int size;
    /* ע�ắ������ */
    _register_function_list             register_function_list;
    /* ��ȡ�������� */
    _get_function_list                  get_function_list;
    /* ����ϵͳ������ */
    _get_os_fun_list                    get_os_fun_list;
    /* ��ȡװ�õ�ǰʱ�亯�� */
    _retrieve_time                      retrieve_time;
    /* ��ȡϵͳʱ�Ӻ��� */
    _retrieve_system_clock              retrieve_system_clock;
    /* ��ȡ��ǰʱ�������1970������� */
    _get_time_second_1970               get_time_second_1970;
    /* ��ȡ����ʱ��ֵ */
    _get_time_nanosecond                get_time_nanosecond;
    /* �����ں�ʱ��ת��Ϊ�������α�׼ʱ�� */
    _time_to_gmt                        time_to_gmt;
    /* �ж��������� */
    _ge_int_enable                      ge_int_enable;
    /* ��ֹ�ж� */
    _ge_int_disable                     ge_int_disable;
    /* �������Ź�������� */
    _create_watchdog_handle             create_watchdog_handle;
    /* ��λ���Ź����Ӷ��� */
    _reset_watchdog_object              reset_watchdog_object;
    /* ����ʱ�亯�� */
    _set_time                           set_time;
    /* ����61850��ʼ����ɱ�־ */
    _gws_set_init_status                gws_set_init_status;
    /* �������ݱ��� */
    _create_lock_data                   create_lock_data;
    /* ���ݱ��� */
    _lock_data                          lock_data;
    /* �ͷ����ݱ��� */
    _unlock_data                        unlock_data;
    /* ��ȡ������ */
    _obtain_mutex                       obtain_mutex;
    /* �ͷŻ����� */
    _release_mutex                      release_mutex;
    /* ��ȡװ��ʱ�� */
    _get_time_zone                      get_time_zone;
    /* ��λ */
    _reset_device                       reset_device;
    /* ��ȡӲ����ʶ�� */
    _get_identification_code            get_identification_code;
    /* ��ȡװ�ù���ѡ�� */
    _get_device_fun_select              get_device_fun_select;
    /* ����61850ʱ�� */
    _set_time_61850                     set_time_61850;
    /* ���ö�ʱʱ���� */
    _set_time_interval                  set_time_interval;
    /* ���ö�ʱʱ��Ʒ������ */
    _set_time_quality                   set_time_quality;
    /* ��־��¼ */
    _write_log                          write_log;
    /* �û����ݶ��й����� */
    /* �������ݶ��й����� */
    _create_data_queue                  create_data_queue;
    /* �����ݶ��з������ݺ��� */
    _send_to_data_queue                send_to_data_queue;
    /* �����ݶ��л�ȡ���ݺ��� */
    _receive_from_data_queue            receive_from_data_queue;
    /* ע��UART�����жϺ��� */
    _reg_uart_int_fun                   reg_uart_int_fun;
    /* UART���ڳ�ʼ�� */
    _uart_com_init                      uart_com_init;
    /* ����UART485ʹ�ܱ�ʶ */
    _uart_rs485_set_enflag              uart_rs485_set_enflag;
    /* ��UART����д���� */
    _uart_write_comm                    uart_write_comm;
    /* ��UART���ڶ�ȡ���� */
    _uart_read_comm                     uart_read_comm;
    /* ��ȡUART�ж�״̬ */
    /* �жϲ�ѯ����,������ָ�����Ƿ��ڽӿ��ж�״̬NU_TRUE:�����ж�״̬,NU_FALSE:���ǽ����ж�״̬ */
    _uart_get_port_int_flag             uart_get_port_int_flag;
    /* ��ȡUART�����ж�����ID */
    _get_uart_vector_id                 get_uart_vector_id;
    /* ��ȡ���ݶ�����Ϣ */
    _get_data_queue_information         get_data_queue_information;
    /* ɾ��ָ�����ݶ��� */
    _delete_data_queue                  delete_data_queue;
    /* ��ȡƽ̨������ʶ */
    _get_plat_lead_flag                 get_plat_lead_flag;
}GTRANSFER;

/* ģ��װ�ض��� */
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
