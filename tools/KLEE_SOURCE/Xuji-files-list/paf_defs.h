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
/*      paf_defs.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      PAF - Platform Application Function                              */
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

#define PAF_LOCK_SETTING_NAME "SettingLock" /* ��ֵ���ݱ������� */
#define PAF_LOCK_REPORT_NAME  "ReportLock"  /* �������ݱ������� */
#define PAF_LOCK_WAVE_NAME    "WaveLock"    /* ¼�����ݱ������� */
#define PAF_LOCK_FCK_NAME     "FckLock"     /* ������ݱ������� */

/* Check to see if the file has been included already.  */

#ifndef PAF_DEFS_H
#define PAF_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Core processing functions.  */
/*=======================================================================*/
/* ���ݱ������ */
typedef INT (* _define_table )
    ( CHAR * Name, INT Index, INT MaxNumber, INT Size, INT Type, INT EditFieldSize );

typedef INT (* _define_field )
    ( INT TableIndex, CHAR * Name, INT Index, INT Attribute,
    INT DataType, INT Number, INT Offset );

typedef VOID * (* _create_all_field )
    ( INT TableIndex, VOID * Buffer );

typedef INT (* _define_protect_handle )( INT TableIndex, VOID * pProtectHandle );

typedef INT (* _get_freeze_status )( INT TableIndex );

typedef INT (* _define_SG_range )
    ( INT TableIndex, INT TypeIndex, INT CoefIndex, INT MinIndex, INT MaxIndex );

typedef VOID *(* _get_table_memory )( INT TabIndex, INT * ActualNum );

typedef VOID *(* _get_index_tabmemory )( INT TableIndex, TB_CFB * Handle, INT iIndex );

typedef INT (* _register_pre_treatment_fun )( INT TableIndex, INT FieldIndex,
    INT (* preTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, VOID * valuePtr, INT Originate ) );

typedef INT (* _register_fore_treatment_fun )( INT TableIndex, INT FieldIndex,
    INT (* foreTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, VOID * valuePtr ) );

typedef INT (* _register_after_treatment_fun )( INT TableIndex, INT FieldIndex,
    INT (* afterTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, INT Originate ) );

typedef INT (* _register_end_treatment_fun )( INT TableIndex, INT FieldIndex,
    INT (* endTreatment )( INT TabIndex, INT FieldIndex ) );

typedef INT (* _get_table_status )( INT TabIndex, UNSIGNED_CHAR * pDst );

typedef INT (* _register_fore_lead_treatment_fun )( INT TableIndex,
    INT (* foreLeadTreatment )( INT TabIndex ) );

typedef INT (* _register_after_lead_treatment_fun )( INT TableIndex,
    INT (* afterLeadTreatment )( INT TabIndex ) );
/*=======================================================================*/
/* ���������� */
typedef HIOM (* _create_output_handle )( UNSIGNED GroupNumber );

typedef INT (* _set_start_relay )( HIOM Handle, UNSIGNED Flag );

typedef INT (* _set_output_bits )( HIOM Handle, UNSIGNED GroupNo, UNSIGNED Bits, INT Operation );

typedef INT (* _getdev_start_relay )( UNSIGNED * ActualStartRelay );

typedef INT (* _getdev_output_bits )( UNSIGNED GroupNo, UNSIGNED * ActualBits );

typedef INT (* _set_holdrelay_maskbits )( UNSIGNED GroupNo , UNSIGNED Bits );

typedef INT (* _set_output_quality )( INT GroupNo, UNSIGNED QualityBits );

typedef INT (* _rotor_switch_output )( INT iFlag );

typedef INT (* _set_out_logmaskbits )
    ( INT iGroupNum, UNSIGNED MaskBits );

typedef INT (* _read_soft_output )( INT iGroup, UNSIGNED * pBits, UNSIGNED * pQuality );
/*=======================================================================*/
/* ��������� */
typedef HASH (* _create_app_routine )( INT TaskType, UNSIGNED ProtectId, UNSIGNED SectorId,
    VOID (* application_routine )( UNSIGNED, UNSIGNED, INT ));

typedef INT (* _delete_app_routine )( HASH Handle );

typedef INT (* _change_task_time_slice )(  INT TaskType, UNSIGNED TimeSlice );

typedef INT (* _change_sleep_ticks )(  INT TaskType, UNSIGNED TimeSlice );

typedef INT (* _main_unsleep )( VOID );
/*=======================================================================*/
/* ʱ������� */
typedef INT (* _read_time )( TIME * pTime );

typedef INT (* _read_61850_time )( TIME * pTime, UNSIGNED * UTCSecond,
    UNSIGNED * UTCMillionsecond, UNSIGNED * Quality );

typedef UNSIGNED (* _read_system_clock )( VOID );

typedef VOID (* _read_timer_clock )( UNSIGNED * pBuffer );

typedef UNSIGNED (* _get_pulse_number )( VOID );

typedef INT (* _get_sync_time_type )( VOID );

typedef UNSIGNED (* _time_test )( UNSIGNED * millisecond_ptr, UNSIGNED * microsecond_ptr );

typedef INT (* _main_run_time )( UNSIGNED * RunTime_ptr, UNSIGNED * SleepTime_ptr );

typedef INT (* _transform_time_from_systime )
    ( UNSIGNED * UTCSecond, UNSIGNED * UTCMillionsecond, TIME * pSrcTime );

typedef INT (* _transform_time_to_systime )
    ( UNSIGNED * UTCSecond, UNSIGNED * UTCMillionsecond, TIME * pSrcTime );

typedef INT (* _transform_ns_utc_to_dev )
    ( UNSIGNED UTCSecond, UNSIGNED UTCNanoSecond, TIME * pDstTime );

typedef INT (* _transform_dev_to_ns_utc )
    ( UNSIGNED * UTCSecond, UNSIGNED * UTCNanoSecond, TIME * pSrcTime );

typedef INT (* _place_on_overtime_check )( VOID * memory, UNSIGNED overTime, VOID * callBackArgs, void (* _callBack )( void * arg ) );

typedef INT (* _remove_from_overtime_check )( VOID * memory );

typedef INT (* _set_PPS_width )( short int Value );

/* ����PPS��Ȼض��Ƿ�������� */
typedef INT (* _pps_set_width_readback )( INT * pBuffer );

/* ��ȡGtime��ʱ��CTֵ */
typedef INT (* _read_gtime_cnr1 )
    ( UNSIGNED * pBufferNs );
/*=======================================================================*/
/* �����ĺ��� */
typedef char *  (* _word_to_string )
    ( void * s1, const void * s2, register unsigned int n );

typedef char *  (* _string_to_word )
    ( void * s1, const void * s2, register unsigned int n );

/* ��WORD����ת�����ֽ����� */
typedef char * (* _word_to_byte )
    ( void * s1, const void * s2, register unsigned int n );

/* ���ֽ�����ת����WORD���� */
typedef char * (* _byte_to_word )
    ( void * s1, const void * s2, register unsigned int n );

typedef INT (* _calculate_crc )
    ( const VOID * pSource, UNSIGNED uDataLength, INT iDataBits,
    INT iCrcBits, UNSIGNED uInitialValue, UNSIGNED * Actual_Code );

typedef INT (* _register_function_list )( CHAR * Name, VOID * FunTablePtr );

typedef VOID * (* _get_function_list )( CHAR * Name );

typedef INT (* _read_user_data )( VOID * pBuffer, UNSIGNED uOffset, UNSIGNED uNumber );

typedef INT (* _save_user_data )( VOID * pBuffer, UNSIGNED uOffset, UNSIGNED uNumber );

typedef INT (* _read_exchange_data )( UNSIGNED * pBuffer );

typedef INT (* _send_exchange_data )( UNSIGNED * pBuffer );

typedef VOID * (* _get_crc_table )( INT iType );

typedef INT (* _read_background_data )
    ( VOID * pBuffer, UNSIGNED uOffset, UNSIGNED uNumber, UNSIGNED uTimeout );

typedef UNSIGNED (* _device_float_to_ieee )( float fData );

typedef float (* _ieee_to_device_float )( UNSIGNED uData );

typedef VOID * (* _get_function_pointer )( CHAR * name  );
/*=======================================================================*/
/* ��������� */
typedef UNSIGNED (* _read_input )( INT iGroup );

typedef INT (* _read_input_quality )( INT iGroup, UNSIGNED * pBits, UNSIGNED * pQuality );

typedef VOID * (* _retrieve_Fck_Input_information )
    ( UNSIGNED * Actual_Size, INT * Actual_Group );

typedef VOID * (* _retrieve_Fck_Input_pointer )( VOID );

typedef INT (* _set_fckin_maskbits )( UNSIGNED GroupNumber, UNSIGNED Bits );

typedef VOID (* _setup_check_input_time )( INT iTimeFlag );

/* ����������״̬ */
typedef INT (* _set_soft_input_status )
    ( INT iGroup, UNSIGNED uBits, INT iStatus );

/* ����������Ʒ������ */
typedef INT (* _set_soft_input_quality )
    ( INT iGroup, UNSIGNED uBits, UNSIGNED Quality );

/* �����������鴫��ʱ���� */
typedef INT (* _set_softin_full_masks )
    ( UNSIGNED GroupNumber, UNSIGNED MaskBits );

/* ��ȡ�������鴫��ʱ���� */
typedef INT (* _get_softin_full_masks )
    ( UNSIGNED GroupNumber, UNSIGNED * MaskBits );
/*=======================================================================*/
/* ģ���������� */
typedef INT (* _read_hard_frequency )(  INT Type, float * frequency );

typedef INT (* _read_sample_frequency )( float * fFrequency );

typedef INT (* _adjust_frequency )( float fFrequency );

typedef VOID * (* _retrieve_sample_pointer )( VOID );

typedef VOID * (* _retrieve_sample_information )
    ( UNSIGNED * Actual_Number, UNSIGNED * Actual_Size,
    UNSIGNED * Actual_Point );

typedef VOID    (* _set_syn_adjust_pointer )( VOID * pSynAdjust );

typedef INT(* _get_sample_count )
    ( UNSIGNED * pSampleCount );

typedef INT (* _data_start_pointer )
    ( INT  iType, VOID ** valueMem, VOID ** qualityMemr, INT * number );

typedef UNSIGNED (* _get_sample_offset )( VOID );

/* ����AD���� */
typedef INT (* _set_ad_gain )( UNSIGNED value );
/*=======================================================================*/
/* nucles���ݱ���/��ʱ��/�¼�λ */
typedef HPRM (* _create_lock_data )( CHAR * Name );

typedef HPRM (* _get_lock_data )( CHAR * Name );

typedef INT (* _lock_data )( HPRM Handle );

typedef INT (* _unlock_data )( HPRM Handle );

typedef HTMM (* _create_timer )( CHAR * name, VOID (* expiration_routine )( UNSIGNED ),
    UNSIGNED initial_time, UNSIGNED reschedule_time, OPTION enable );

typedef INT (* _control_timer )( HTMM Handle, OPTION enable );

typedef HEVM (* _create_event_group )( VOID );

typedef INT (* _set_event )( HEVM Handle, UNSIGNED events, OPTION operation );

typedef INT (* _retrieve_event )( HEVM Handle, UNSIGNED requested_flags,
    OPTION operation, UNSIGNED * retrieved_flags, UNSIGNED suspend );
/*=======================================================================*/
/* ��ʷ��¼������ */
typedef INT (* _fill_event )
    ( const VOID * pName, INT iEventType );

typedef INT (* _fill_event_full )
    ( const VOID * pName, TIME * pTime, INT iEventType );

typedef INT (* _fill_report )
    ( UNSIGNED FaultNumber, INT Type, UNSIGNED Size, const VOID * Src );

typedef INT (* _fill_report_time )
    ( UNSIGNED FaultNumber, INT Type, UNSIGNED Size, const VOID * Src, TIME * pTime );

typedef INT (* _start_wave )
    ( INT iType, INT iSpace, INT iStartPoint, INT iTotalPoint,
    INT iElecNumber, WAVE_CHANNEL * WaveChannel, TIME * pTime, STATE * pState, INT iState );

typedef INT (* _start_wave_mul )
    ( INT waveSegments, INT iType, INT * spacePtr, INT iStartPoint,
    INT * totalPointPtr, INT iElecNumber, WAVE_CHANNEL * WaveChannel, TIME * pTime,
    STATE * pState, INT iState );

typedef INT (* _read_wave_index_number )( VOID );

typedef INT (* _read_elec_number )( VOID );

typedef INT (* _adjust_elec_number )( VOID );

typedef INT (* _set_elec_number )( INT iElecNumber );

typedef INT (* _read_fault_number )( VOID );

typedef INT (* _adjust_fault_number )( VOID );

typedef INT (* _set_fault_number )( INT iFaultNumber );

typedef INT (* _set_fault_status )( INT iFaultStatus );

typedef INT (* _get_exchange_fault_number )( VOID );

typedef INT (* _set_wave_state )( STATE * pState, INT iState );

typedef VOID *  (* _retrieve_logic_information )( UNSIGNED * Actual_Size );

typedef INT (* _fault_return )
    ( INT iTableID, INT CondValue, UNSIGNED uFaultNumber,
    UNSIGNED uRelTime, UNSIGNED uSect, UNSIGNED uFunType, UNSIGNED uINF,
    UNSIGNED uDPI, TIME * pTime, UNSIGNED q, UNSIGNED tSec, UNSIGNED tFrac,
    UNSIGNED tQuality, UNSIGNED phase );

typedef INT (* _fault_message )( VOID * buf );

typedef INT (* _fill_check_info )
    ( INT iTableID, INT CondValue, INT iType,
    INT iSecInf, INT iFunInf, INT iStatus, UNSIGNED q );

typedef INT (* _fill_check_info_full )
    ( INT iTableID, INT CondValue, INT iType,
    INT iSecInf, INT iFunInf, INT iStatus, TIME * pTime, UNSIGNED q,
    UNSIGNED tSec, UNSIGNED tFrac, UNSIGNED tQuality );

/* ���goose���� */
typedef INT (* _fill_check_goose )
    ( INT iTableID, INT CondValue, INT iType, INT iSecInf,
    INT iFunInf, INT iStatus, TIME * pTime, UNSIGNED q,
    UNSIGNED tSec, UNSIGNED tFrac, UNSIGNED tQuality, UNSIGNED offset );

/* ����goose���� */
typedef INT (* _send_check_goose )( VOID );
/*=======================================================================*/
/* ��������� */
typedef INT (* _read_error_bits )( UNSIGNED * Actual_ErrorBits );

typedef UNSIGNED (* _mmi_error_bits )( VOID );
/*=======================================================================*/
/* ����ָʾ�ƹ����� */
typedef HLEM (* _create_led_handle )( VOID );

typedef INT (* _set_led_status )( HLEM Handle, UNSIGNED LedStatus );

typedef INT (* _set_dev_led_frequency )( UNSIGNED Frequency );

typedef VOID (* _set_user_event_bits )( UNSIGNED uEventBits, INT iFlag );

/* ����ƽ̨��澯���¼�λ */
typedef INT  (* _set_plat_alarmled_bits )
    ( UNSIGNED uPlatAlarmBits );

/* ����ƽ̨���е��쳣�¼�λ���� */
typedef INT (* _set_plat_runledabnor_bits )
    ( UNSIGNED uRunAbnorEventBits );

/* ����LED */
typedef VOID  (* _led_reset )( VOID );

/* �����û��� */
typedef INT  (* _set_user_led_bits )
    (  INT iGroup, UNSIGNED ledBits );

/* ����װ�ú������е�״̬ */
typedef INT (* _set_dev_led_call_status )
    ( INT AutoCallEnable, UNSIGNED LedCallDelay, UNSIGNED LedCallStatus, UNSIGNED Frequency );

/* װ�����еƺ��й���(����ֵ��0-�޺��У�!0-ʵ�ʺ��е�����״̬) */
typedef UNSIGNED (* _dev_led_call_timer )( UNSIGNED Time );
/*=======================================================================*/
/* �жϹ����� */
typedef VOID (* _register_ad_interrupt )
    ( VOID (* ad_interrupt )( VOID ));

typedef VOID (* _register_sample_conversion )
    ( INT (* sample_conversion )( float *, INT ));

typedef VOID (* _register_fiber_receive_interrupt )
    ( VOID (* fiber_receive_interrupt )( VOID ));

typedef VOID (* _register_fiber_transmit_interrupt )
    ( VOID (* fiber_transmit_interrupt )( VOID ));
/*=======================================================================*/
/* ͨѶ������ */
typedef INT (* _register_command )
    ( VOID * pChannel, UNSIGNED_CHAR type, void * callback );

typedef INT (* _write_fck_soe )
    ( VOID * pBuffer, UNSIGNED timeout );

typedef INT (* _read_fck_command )
    ( VOID * pBuffer, UNSIGNED timeout );

/* Ӧ�ó�����ͨѶ����
����˵��: actual_size ���ݳ���(���Ϊ244)
        : pBuffer ���ݻ�����
        : iDataLevel ���ݼ���(1:Ϊһ������,2:Ϊ��������)
        : iSendDire ���ݷ��ͷ���(��pro_defs.hͷ�ļ����ж���,Ӧ����Ҫƽ̨͸��������Ϣ����)
*/
typedef INT (* _user_send_com_data )
    ( INT actual_size, UNSIGNED_CHAR * pBuffer, INT iDataLevel, INT iSendDire );
/*=======================================================================*/
/* �ڴ������ */
typedef HMGM (* _create_memory_pool )( CHAR * name,
    VOID * start_address, UNSIGNED pool_size,
    UNSIGNED min_allocation, OPTION suspend_type );

typedef INT  (* _delete_memory_pool )( HMGM Handle );

typedef INT (* _allocate_memory )( HMGM Handle, VOID ** return_pointer,
    UNSIGNED size, UNSIGNED suspend );

typedef INT (* _deallocate_memory )( VOID * memory_ptr );

typedef INT (* _mem_pool_information )( VOID ** mem_pool, VOID ** uncached_mem_pool );
/*=======================================================================*/
/* ״̬�������� */
typedef INT (* _getCPUTemperature )( INT * temperature_ptr );

/* ��ȡFLASH�����ڴ��״̬ */
typedef INT (* _getFlashPrartitionBlockStatus )( int index, int * retired,
    int * used, int * empty );
/*=======================================================================*/
/* ���ÿ�����Ʒ�����ؼ���λ,���û�ȡ�¼�λ */
typedef INT (* _set_out_checkquality_bits )
    ( INT GroupNumber, UNSIGNED CheckQuaBits );

/* ����װ��״̬���� */
typedef INT (* _set_dev_status )
    ( UNSIGNED uDevStatus );
/*=======================================================================*/
/* ����ͨѶ���躯�� */
/* ��ȡ���˷�������ƫ��λ�� */
typedef UNSIGNED (* _get_fiber_send_offset )( VOID );

/* ע�����ù���ͨѶ������ź��� */
typedef VOID (* _fiber_reg_set_send_serial )
    ( VOID (* Fiber_SetSendSerial )( UNSIGNED uSendSerial ) );
/* ���÷������ڵ������� */
typedef INT (* _fiber_set_send_adjust_step )
    ( UNSIGNED uAdjustStepNs );
/*=======================================================================*/
/* ������������� */
/* �����������뷽���ֵ */
/* dirSetting:����ֵ ����ֵ:0-�ɹ� !0-ʧ�� */
typedef INT (* _setOrthogonalCodeDirection )( unsigned short int dirSetting );

/* �������������ʼֵ */
/* iniValue:��ʼֵ ����ֵ:0-�ɹ� !0-ʧ�� */
typedef INT (* _setOrthogonalCodeValue )( INT iGroup, unsigned short int iniValue );

/* ��ȡ�������뷽�� */
/* dirSetting:��ȡ����ֵ ����ֵ:0-�ɹ� !0-ʧ�� */
typedef INT (* _getOrthogonalCodeDirection )( UNSIGNED * dirSetting );

/* ��ȡ��������ֵ */
/* iniValue:��ȡ��������ֵ ����ֵ:0-�ɹ� !0-ʧ�� */
typedef INT (* _getOrthogonalCodeValue )( INT iGroup, UNSIGNED * iniValue );
/*=======================================================================*/

/* Ӧ��������������������ִ�к��� */
typedef INT (* _user_active_main_task )
    ( VOID );
/*=======================================================================*/
/* �û���NPI��������� */
typedef INT (* _get_data_from_npi )
    ( UNSIGNED_CHAR * buffer, INT * Length );

/* �û���NPI�෢�����ݺ��� */
typedef INT (* _user_send_data_to_npi )
    ( UNSIGNED_CHAR * buffer, INT Length );
/*=======================================================================*/
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
/*=======================================================================*/
/* ���ֵ��ר�ú��� */
/*delay_time_us,����С��1����*/
typedef VOID (* _meter_pulse_start )( UNSIGNED pulse_index, UNSIGNED delay_time_us );
/*=======================================================================*/
/* ͨ�ű��Ĺ����� */
/* ͨ�ű���ϵͳ��ʼ�� */
typedef  INT (* _com_message_initialize )
    ( INT iMaxCount, INT iMaxMessLen );
/* дͨ�ű��� */
typedef  INT (* _write_com_message )
    ( CHAR * msg, INT iLen, UNSIGNED uTimeOut );
/* ��ȡ���� */
typedef  INT (* _read_com_message )
    ( CHAR * msg, INT iMaxLen, INT * iActLen, UNSIGNED uTimeOut );
/* ɾ������ */
typedef INT (* _del_com_message )
    ( UNSIGNED uTimeOut );
/*=======================================================================*/
/* ����ģ����ͨ��Ʒ������¼��״̬������ʵ��ż����� */
typedef INT (* _set_chlq_state_info )
    ( INT iStartGroup, INT iGroupCount );

/* ��ȡӲ����ʶ�� */
typedef UNSIGNED (* _get_identification_code )
    ( VOID );
/*=======================================================================*/
/* ״̬�������� */
/* ��ȡ�������״̬��Ϣ */
typedef INT (* _get_motor_status )( UNSIGNED * motor_status );

/* ���ò�������������� */
typedef INT (* _set_motor_command )( INT channel, UNSIGNED command );

/* ���ò������������ */
typedef INT (* _set_motor_pulse )( INT channel, UNSIGNED pulse );

/* ���õ�ŷ��������� */
typedef INT (* _set_solenoid_valve_command )( INT channel, UNSIGNED command );

/* ����ʱ�� */
/* ����ʱ�亯�� */
typedef INT (* _set_time_ex )( TIME * pTime, INT iTimeZone );
typedef INT (* _set_time )( TIME * pTime );

/*Profibus��غ���*/
typedef INT     (* _profidp_init )( void * cfgs, int n );

typedef INT     (* _profidp_tx_input )( int dev_index, unsigned char * data );

typedef INT     (* _profidp_tx_diag )( int dev_index, unsigned char * data, int len );

typedef INT     (* _profidp_calculate_io_len )( unsigned char * config, unsigned char len, 
    unsigned char * input_len, unsigned char * output_len );

/* ��ø���ģ���CRC��Ϣ */
typedef INT     ( * _get_module_crc_inf )( INT * pModuleNum, VOID * * pRunTextCb );

/*=======================================================================*/
/* Define the data structure for use within the core processing functions.  */
typedef struct TRANSFER_STRUCT
{
    /*  Standard protect struct information.   Changes made
    to this area of the structure can have undesirable side effects. */
    int size;
    /* ���ݱ������ */
    _define_table                       define_table;
    _define_field                       define_field;
    _create_all_field                   create_all_field;
    _define_protect_handle              define_protect_handle;
    _get_freeze_status                  get_freeze_status;
    _define_SG_range                    define_SG_range;
    _get_table_memory                   get_table_memory;
    _get_index_tabmemory                get_index_tabmemory;
    _register_fore_treatment_fun        register_fore_treatment_fun;
    _register_after_treatment_fun       register_after_treatment_fun;
    _get_table_status                   get_table_status;
    _register_fore_lead_treatment_fun   register_fore_lead_treatment_fun;
    _register_after_lead_treatment_fun  register_after_lead_treatment_fun;
    /* ���������� */
    _create_output_handle               create_output_handle;
    _set_start_relay                    set_start_relay;
    _set_output_bits                    set_output_bits;
    _getdev_start_relay                 getdev_start_relay;
    _getdev_output_bits                 getdev_output_bits;
    _set_holdrelay_maskbits             set_holdrelay_maskbits;
    _set_output_quality                 set_output_quality;
    _rotor_switch_output                rotor_switch_output;
    _set_out_logmaskbits                set_out_logmaskbits;
    _read_soft_output                   read_soft_output;
    /* ��������� */
    _create_app_routine                 create_app_routine;
    _delete_app_routine                 delete_app_routine;
    _change_task_time_slice             change_task_time_slice;
    _change_sleep_ticks                 change_sleep_ticks;
    _main_unsleep                       main_unsleep;
    /* ʱ������� */
    _read_time                          read_time;
    _read_61850_time                    read_61850_time;
    _read_system_clock                  read_system_clock;
    _read_timer_clock                   read_timer_clock;
    _get_pulse_number                   get_pulse_number;
    _get_sync_time_type                 get_sync_time_type;
    _time_test                          time_test;
    _main_run_time                      main_run_time;
    _transform_time_from_systime        transform_time_from_systime;
    /* �����ĺ��� */
    _word_to_string                     word_to_string;
    _string_to_word                     string_to_word;
    _calculate_crc                      calculate_crc;
    _register_function_list             register_function_list;
    _get_function_list                  get_function_list;
    _read_user_data                     read_user_data;
    _save_user_data                     save_user_data;
    _read_exchange_data                 read_exchange_data;
    _send_exchange_data                 send_exchange_data;
    _get_crc_table                      get_crc_table;
    _read_background_data               read_background_data;
    _device_float_to_ieee               device_float_to_ieee;
    _ieee_to_device_float               ieee_to_device_float;
    /* ��������� */
    _read_input                         read_input;
    _read_input_quality                 read_input_quality;
    _retrieve_Fck_Input_information     retrieve_Fck_Input_information;
    _retrieve_Fck_Input_pointer         retrieve_Fck_Input_pointer;
    _set_fckin_maskbits                 set_fckin_maskbits;
    _setup_check_input_time             setup_check_input_time;
    _set_soft_input_status              set_soft_input_status;
    _set_soft_input_quality             set_soft_input_quality;
    /* ģ���������� */
    _read_hard_frequency                read_hard_frequency;
    _read_sample_frequency              read_sample_frequency;
    _adjust_frequency                   adjust_frequency;
    _retrieve_sample_pointer            retrieve_sample_pointer;
    _retrieve_sample_information        retrieve_sample_information;
    _set_syn_adjust_pointer             set_syn_adjust_pointer;
    _get_sample_count                   get_sample_count;
    _data_start_pointer                 data_start_pointer;
    _get_sample_offset                  get_sample_offset;
    /* nucles���ݱ���/��ʱ��/�¼�λ */
    _create_lock_data                   create_lock_data;
    _get_lock_data                      get_lock_data;
    _lock_data                          lock_data;
    _unlock_data                        unlock_data;
    _create_timer                       create_timer;
    _control_timer                      control_timer;
    _create_event_group                 create_event_group;
    _set_event                          set_event;
    _retrieve_event                     retrieve_event;
    /* ��ʷ��¼������ */
    _fill_event                         fill_event;
    _fill_event_full                    fill_event_full;
    _fill_report                        fill_report;
    _start_wave                         start_wave;
    _read_wave_index_number             read_wave_index_number;
    _read_elec_number                   read_elec_number;
    _adjust_elec_number                 adjust_elec_number;
    _set_elec_number                    set_elec_number;
    _read_fault_number                  read_fault_number;
    _adjust_fault_number                adjust_fault_number;
    _set_fault_number                   set_fault_number;
    _set_fault_status                   set_fault_status;
    _get_exchange_fault_number          get_exchange_fault_number;
    _set_wave_state                     set_wave_state;
    _retrieve_logic_information         retrieve_logic_information;
    _fault_return                       fault_return;
    _fill_check_info                    fill_check_info;
    _fill_check_info_full               fill_check_info_full;
    _fill_check_goose                   fill_check_goose;
    _send_check_goose                   send_check_goose;
    /* ��������� */
    _read_error_bits                    read_error_bits;
    _mmi_error_bits                     mmi_error_bits;
    /* ����ָʾ�ƹ����� */
    _create_led_handle                  create_led_handle;
    _set_led_status                     set_led_status;
    _set_dev_led_frequency              set_dev_led_frequency;
    _set_user_event_bits                set_user_event_bits;
    _set_plat_alarmled_bits             set_plat_alarmled_bits;
    _set_plat_runledabnor_bits          set_plat_runledabnor_bits;
    _led_reset                          led_reset;
    _set_dev_led_call_status            set_dev_led_call_status;
    _dev_led_call_timer                 dev_led_call_timer;
    /* �жϹ����� */
    _register_ad_interrupt              register_ad_interrupt;
    _register_sample_conversion         register_sample_conversion;
    _register_fiber_receive_interrupt   register_fiber_receive_interrupt;
    _register_fiber_transmit_interrupt  register_fiber_transmit_interrupt;
    /* ͨѶ������ */
    _register_command                   register_command;
    _write_fck_soe                      write_fck_soe;
    _read_fck_command                   read_fck_command;
    /* �ڴ������ */
    _create_memory_pool                 create_memory_pool;
    _delete_memory_pool                 delete_memory_pool;
    _allocate_memory                    allocate_memory;
    _deallocate_memory                  deallocate_memory;
    /* 1.1�汾���Ӻ��� */
    _place_on_overtime_check            place_on_overtime_check;
    _remove_from_overtime_check         remove_from_overtime_check;
    _transform_time_to_systime          transform_time_to_systime;
    _register_pre_treatment_fun         register_pre_treatment_fun;
    /* 1.2�汾���Ӻ��� */
    _getCPUTemperature                  getCPUTemperature;
    _getFlashPrartitionBlockStatus      getFlashPrartitionBlockStatus;
    _mem_pool_information               mem_pool_information;
    _set_out_checkquality_bits          set_out_checkquality_bits;
    _set_dev_status                     set_dev_status;
    _register_end_treatment_fun         register_end_treatment_fun;
    _transform_ns_utc_to_dev            transform_ns_utc_to_dev;
    _transform_dev_to_ns_utc            transform_dev_to_ns_utc;
    _fault_message                      fault_message;
    /* ����ͨѶ���躯�� */
    _get_fiber_send_offset              get_fiber_send_offset;
    _fiber_reg_set_send_serial          fiber_reg_set_send_serial;
    /* 1.3�汾���Ӻ��� */
    _start_wave_mul                     start_wave_mul;
    /* ������������� */
    _setOrthogonalCodeDirection         setOrthogonalCodeDirection;
    _setOrthogonalCodeValue             setOrthogonalCodeValue;
    _getOrthogonalCodeDirection         getOrthogonalCodeDirection;
    _getOrthogonalCodeValue             getOrthogonalCodeValue;
    _user_active_main_task              user_active_main_task;
    /* ����PPS��� */
    _set_PPS_width                      set_PPS_width;
    _get_data_from_npi                  get_data_from_npi;
    _user_send_data_to_npi              user_send_data_to_npi;
    _pps_set_width_readback             pps_set_width_readback;
    _get_function_pointer               get_function_pointer;
    /* ��־��¼ */
    _write_log                          write_log;
    _fiber_set_send_adjust_step         fiber_set_send_adjust_step;
    _read_gtime_cnr1                    read_gtime_cnr1;
    _set_softin_full_masks              set_softin_full_masks;
    /* �û����ݶ��й����� */
    _create_data_queue                  create_data_queue;
    _send_to_data_queue                 send_to_data_queue;
    _receive_from_data_queue            receive_from_data_queue;
    _word_to_byte                       word_to_byte;
    _byte_to_word                       byte_to_word;
    /* ���ֵ��ר�ú��� */
    _meter_pulse_start                  meter_pulse_start;
    /* ͨ�ű��Ĺ����� */
    _com_message_initialize             com_message_initialize;
    _write_com_message                  write_com_message;
    _read_com_message                   read_com_message;
    _del_com_message                    del_com_message;
    /* 1.5�汾 */
    _set_chlq_state_info                set_chlq_state_info;
    /* Ӳ����Ϣ������ */
    _get_identification_code            get_identification_code;
    _set_user_led_bits                  set_user_led_bits;
    _set_ad_gain                        set_ad_gain;
    /* ״̬�������� */
    _get_motor_status                   get_motor_status;
    _set_motor_command                  set_motor_command;
    _set_motor_pulse                    set_motor_pulse;
    _set_solenoid_valve_command         set_solenoid_valve_command;
    /* ����ʱ�亯�� */
    _set_time_ex                        set_time_ex;
    _set_time                           set_time;
    _fill_report_time                   fill_report_time;
    _get_softin_full_masks              get_softin_full_masks;

    _user_send_com_data                 user_send_com_data;

    /* Profibus ���� */
    _profidp_init                       profidp_init;
    _profidp_tx_input                   profidp_tx_input;
    _profidp_tx_diag                    profidp_tx_diag;
    _profidp_calculate_io_len           profidp_calculate_io_len;

    /* ��ø���ģ���CRC��Ϣ */
    _get_module_crc_inf                 get_module_crc_inf;
}TRANSFER;

/* Define the data structure for use within the core processing functions.  */
typedef struct PABFUN_STRUCT
{
    /*  Standard protect struct information.   Changes made
    to this area of the structure can have undesirable side effects. */
    int size;
    _get_function_pointer               get_function_pointer;
}PABFUN;

#ifdef __cplusplus
}
#endif

#endif
