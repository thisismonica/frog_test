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

/* �������ж��������� */
#define XJ_PR_IRQ0_INT_VECTOR_ID                4

/* �ж�����������ʽ���� */
#define XJ_TRIG_NOT_SUPPORTED                   0
#define XJ_TRIG_RISING_EDGE                     1
#define XJ_TRIG_FALLING_EDGE                    2
#define XJ_TRIG_LEVEL_LOW                       3
#define XJ_TRIG_LEVEL_HIGH                      4
#define XJ_TRIG_RISING_FALLING_EDGES            5
#define XJ_TRIG_HIGH_LOW_RISING_FALLING_EDGES   6

#ifndef XJ_CWDC_MODULE
#define XJ_CWDC_MODULE
#define CWD_WATCHDOG_NORMAN       0 /* �������������� */
#define CWD_WATCHDOG_ERROR        1 /* �����������쳣 */
#define CWD_WATCHDOG_ENABLE       1 /* ʹ�ܼ������� */
#define CWD_WATCHDOG_DISABLE      0 /* ��ֹ�������� */

typedef long HWDM; /* Ӧ�ó�������� */
#endif

#ifndef XJ_TIME_MODULE
#define XJ_TIME_MODULE
/* ��ʱʱ�����Ͷ��� */
#define CTM_TYPE_GPS_SECOND       (1 << 0) /* �������ʱ */
#define CTM_TYPE_GPS_MINUTE       (1 << 1) /* �������ʱ */
#define CTM_TYPE_GPS_IRIGB        (1 << 2) /* IRIG-B���ʱ */
#define CTM_TYPE_NETWORK          (1 << 3) /* �����ʱ */
#define CTM_TYPE_61850            (1 << 4) /* 61850��ʱ:ʵ����SNTP��ʱ*/
#define CTM_TYPE_1588             (1 << 5) /* 1588��ʱ */
#define CTM_TYPE_SNTP             CTM_TYPE_61850 /*SNTP��ʱ*/

/* ʱ��Ʒ�ʶ��� */
#define CTM_TIME_ACCURACY         0        /* װ��ʱ��׼ȷ��ѡ�� */
#define CTM_Q_LEAP_SECOND         (1 << 7) /* ���� */
#define CTM_Q_CLOCK_FAILURE       (1 << 6) /* ʱ�ӹ��� */
#define CTM_Q_CLOCK_NO_SYN        (1 << 5) /* ʱ��δͬ�� */
#define CTM_Q_TIME_ACCURACY(x)    (x << CTM_TIME_ACCURACY) /* ʱ��׼ȷ�� */

#define PROFIBUS_MODE             0x1  /* PROFIBUSʱ��ת����װ��ʱ�� */
#define UTC_MODE                  0x2  /* UTCʱ��ת����װ�� */

/* װ��ʱ��ṹ */
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
typedef long HPRM;  /* nucleus���ݱ��������� */
typedef long HTMM;  /* nucleus��ʱ����������� */
typedef long HEVM;  /* nucleus�¼�λ��������� */
typedef long HMGM;  /* nucleus�ڴ�ع�������� */
typedef long HQUEM; /* nucleus��Ϣ���й�������� */
#endif

/* Core processing functions.  */
/*=======================================================================*/
/* ���ݱ������ */
typedef VOID *(* _get_table_memory )( INT TabIndex, INT * ActualNum );

/* ��ȡ���ݱ��С */
typedef INT (* _get_table_size )( INT TabIndex );

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

/* ����ѭ�������� */
typedef INT (* _calculate_crc )
    ( const VOID * Src, UNSIGNED Size, INT BytesAddress,
    INT CrcBits, UNSIGNED InitialValue, UNSIGNED * CRC_Code );

/* ��ƽ̨��ȡͨѶ���� */
typedef INT     (* _read_command )
    ( VOID * Buffer, UNSIGNED * actual_size, UNSIGNED uTimeOut );

/* ��ƽ̨дͨѶ���� */
typedef INT     (* _write_command )
    ( VOID * Buffer, UNSIGNED actual_size, UNSIGNED uTimeOut );

/* ͨ�ű��Ĵ��� */
typedef VOID    (* _lcc_parase_data )( VOID );

/* ����ʱ�亯�� */
typedef INT (* _set_time )( TIME * pTime, INT iTimeZone );

/* ����¼���� */
typedef INT (* _fill_event )( const VOID * pName, INT iEventType );

/* ��������� */
typedef UNSIGNED (* _read_input )( INT iGroup );

/* �������Ź�������� */
typedef HWDM (* _create_watchdog_handle )
    ( UNSIGNED SetTime, CHAR * Name, UNSIGNED MonitorCtrlHandle );

/* ��λ���Ź����Ӷ��� */
typedef INT (* _reset_watchdog_object )( HWDM Handle );

/* ��ȡ�û����ݸ�Ŀ¼·�����ƺ��� */
typedef const CHAR * (* _get_user_dir_name )( VOID );

/* ����װ�÷�ģ���ʼ���ɹ����� */
typedef INT (* _set_modinit_success )( VOID );

/* ��ȡװ�÷�ģ���ʼ����ɸ��� */
typedef INT (* _get_modinit_count )( VOID );

/* ��ȡ�ӿڳ����CRC */
typedef INT (* _get_mmi_crc )( UNSIGNED * pBuff );

/* ʱ��ת������ */
typedef INT (* _transform_time_from_systime )( UNSIGNED * UTCSecond,
    UNSIGNED * UTCMillionsecond, TIME * pSrcTime );

/* �ӿ���ֱ�ӵ������ݱ��޸ĺ��� */
typedef INT (* _hmi_modif_table_data )( INT TableIndex, INT StartNumber, INT number,
    VOID * pCondBuf, INT iHandleNumber, VOID * pDataBuf );

/* ���������ݱ����� */
typedef INT (* _write_fck_table_data )( VOID ** pGsPtrs, INT iNumber );

/* ���ʱ�����Ч��,ͨ��iChangeFlag�������Ƿ��޸Ĳ���ȷ��ʱ��(�޸�ʱ����Ϊʱ������ֵ) */
typedef INT (* _time_check )( TIME * pTime , INT iChangeFlag );

/* ����Ҫת����ʱ��ģʽ,1ΪPROFIBUSʱ��,2ΪUTCʱ��,ȱʡΪUTCʱ��  */
typedef INT (* _tm_set_time_mode ) ( UNSIGNED TimeMode );

/* װ��ʱ��ת���ɾ͵�ʱ�� */
typedef INT (* _tm_trans_dev_to_lt )( UNSIGNED * Time1, UNSIGNED * Time2, TIME * pTime );

/* �͵�ʱ��ת����װ��ʱ�� */
typedef INT (* _tm_trans_lt_to_dev )( UNSIGNED * Time1, UNSIGNED * Time2, TIME * pTime );

/* ��ȡװ��ʱ�� */
typedef INT (* _get_time_zone )( INT * pTimeZone );

/* ��ȡ���̼�ֵ */
typedef UNSIGNED (* _get_key_value )( VOID );

/* ��ȡӲ����ʶ�� */
typedef UNSIGNED (* _get_identification_code )( VOID );

/*����UTCʱ��ת��ΪTIMEװ��ʱ��*/
typedef INT (* _transform_ns_utc_to_dev )
    ( UNSIGNED UTCSecond, UNSIGNED UTCNanoSecond, TIME * pDstTime );

/* TIMEװ��ʱ��ת��Ϊ����UTCʱ��*/
typedef INT (* _transform_dev_to_ns_utc )
    ( UNSIGNED * UTCSecond, UNSIGNED * UTCNanoSecond, TIME * pSrcTime );

/*װ��ʱ��ת��ΪmsUTCʱ��*/
typedef INT (* _transform_dev_to_ms_utc )
    ( UNSIGNED * UTCSecond, UNSIGNED * UTCMillionsecond, TIME * pSrcTime );

/*msUTCʱ��ת��Ϊװ��ʱ��*/
typedef INT (* _transform_ms_utc_to_dev )
    ( UNSIGNED * UTCSecond, UNSIGNED * UTCMillionsecond, TIME * pSrcTime );

/* ��־��¼ */
typedef INT (* _write_log )( CHAR * msg );

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
/*=======================================================================*/
/* �ж��������� */
typedef INT (* _ge_int_enable )
    ( INT vector_id, INT trigger_type, INT priority );

/* ��ֹ�ж� */
typedef INT (* _ge_int_disable )
    ( INT vector_id );
/*=======================================================================*/
/* ��ȡ61850ʱ�� */
typedef INT (* _read_61850_time )( TIME * pTime, UNSIGNED * UTCSecond,
    UNSIGNED * UTCMillionsecond, UNSIGNED * Quality );
/*=======================================================================*/
/* ��ȡƽ̨������ʶ */
typedef INT  (* _get_plat_lead_flag )
    ( VOID );
/*=======================================================================*/
/* Ӧ�ö�ȡ�����������ݺ��� */
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
    /* ���ݱ������ */
    _get_table_memory                   get_table_memory;
    /* ��ȡ���ݱ��С */
    _get_table_size                     get_table_size;
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
    /* ����ѭ�������� */
    _calculate_crc                      calculate_crc;
    /* ��ƽ̨��ȡͨѶ���� */
    _read_command                       read_command;
    /* ��ƽ̨дͨѶ���� */
    _write_command                      write_command;
    /* ͨ�ű��Ĵ��� */
    _lcc_parase_data                    lcc_parase_data;
    /* ����ʱ�亯�� */
    _set_time                           set_time;
    /* ����¼���¼���� */
    _fill_event                         fill_event;
    /* ��������� */
    _read_input                         read_input;
    /* �������Ź�������� */
    _create_watchdog_handle             create_watchdog_handle;
    /* ��λ���Ź����Ӷ��� */
    _reset_watchdog_object              reset_watchdog_object;
    /* ��ȡ�û����ݸ�Ŀ¼·�����ƺ��� */
    _get_user_dir_name                  get_user_dir_name;
    /* ����װ�÷�ģ���ʼ���ɹ����� */
    _set_modinit_success                set_modinit_success;
    /* ��ȡװ�÷�ģ���ʼ����ɸ��� */
    _get_modinit_count                  get_modinit_count;
    /* ��ȡ�ӿڳ����CRC */
    _get_mmi_crc                        get_mmi_crc;
    /* ʱ��ת������ */
    _transform_time_from_systime        transform_time_from_systime;
    /* �ӿ���ֱ�ӵ������ݱ��޸ĺ��� */
    _hmi_modif_table_data               hmi_modif_table_data;
    /* ���������ݱ����� */
    _write_fck_table_data               write_fck_table_data;
    /* ʱ��Ϸ��Լ�⺯�� */
    _time_check                         time_check;
    /* ����Ҫת����ʱ��ģʽ,1ΪPROFIBUSʱ��,2ΪUTCʱ��,ȱʡΪUTCʱ��  */
    _tm_set_time_mode                   tm_set_time_mode;
    /* װ��ʱ��ת���ɾ͵�ʱ�� */
    _tm_trans_dev_to_lt                 tm_trans_dev_to_lt;
    /* �͵�ʱ��ת����װ��ʱ�� */
    _tm_trans_lt_to_dev                 tm_trans_lt_to_dev;
    /* ��ȡװ��ʱ�� */
    _get_time_zone                      get_time_zone;
    /* ��ȡ���̼�ֵ */
    _get_key_value                      get_key_value;
    /* ��ȡӲ����ʶ�� */
    _get_identification_code            get_identification_code;
    /* ����UTCת����TIME���͵�װ��ʱ�� */
    _transform_ns_utc_to_dev            transform_ns_utc_to_dev;
    /* TIME���͵�װ��ʱ��ת��������UTC */
    _transform_dev_to_ns_utc            transform_dev_to_ns_utc;
    /* װ��ʱ��ת��ΪmsUTC */
    _transform_dev_to_ms_utc            transform_dev_to_ms_utc;
    /* MS UTC ת��Ϊװ��ʱ��*/
    _transform_ms_utc_to_dev            transform_ms_utc_to_dev;
    /* ��־��¼ */
    _write_log                          write_log;
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
    /* �û����ݶ��й����� */
    /* �������ݶ��й����� */
    _create_data_queue                  create_data_queue;
    /* �����ݶ��з������ݺ��� */
    _send_to_data_queue                send_to_data_queue;
    /* �����ݶ��л�ȡ���ݺ��� */
    _receive_from_data_queue            receive_from_data_queue;
    /* �ж��������� */
    _ge_int_enable                      ge_int_enable;
    /* ��ֹ�ж� */
    _ge_int_disable                     ge_int_disable;
    /* ��ȡ61850ʱ�� */
    _read_61850_time                    read_61850_time;
    /* ��ȡƽ̨������ʶ */
    _get_plat_lead_flag                 get_plat_lead_flag;
    /* Ӧ�ö�ȡ�����������ݺ��� */
    _user_read_report_data              user_read_report_data;
}MTRANSFER;

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
