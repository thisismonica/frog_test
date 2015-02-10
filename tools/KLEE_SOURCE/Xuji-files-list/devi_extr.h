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
/*      devi_extr.h                                     Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      DEVI - Device Interface functions                                */
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
/*      ZhideYang.      11-28-2009      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
/* Check to see if the file has been included already.  */

#ifndef DEVI_EXTR_H
#define DEVI_EXTR_H

/* Ӳ����Ϣ�����ʶ */
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

/* ��ʼ���豸 */
INT DEVI_initialize( VOID );
/* ��ȡӲ����ʶ�� */
UNSIGNED DEVI_GetIdentificationCode ( VOID );
/* I/O��ʼ������ */
INT DEVI_IO_Initialize( VOID );
/* �������� Offset:��ʼ��� Number:��ȡ���� buffer_ptr:��ȡ���ݴ洢������ */
INT DEVI_ReadInput( INT Offset, INT Number, VOID * buffer_ptr );
/* ��������Ʒ������ Offset:��ʼ��� Number:��ȡ���� buffer_ptr:��ȡ���ݴ洢������ */
INT DEVI_ReadInputQuality( INT Offset, INT Number, VOID * buffer_ptr );
/* ��������ֵ��Ʒ������ Offset:��ʼ��� Number:��ȡ���� buffer_ptr:��ȡ���ݴ洢������ */
INT DEVI_ReadInputVQ( INT Offset, INT Number, VOID * buffer_ptr );
/* �������� Offset:��ʼ��� Number:��ȡ���� buffer_ptr:��ȡ���ݴ洢������ */
INT DEVI_RelayDriver( INT Offset, INT Number, VOID * buffer_ptr );
/* ��ʼ�����Ź� */
INT DEVI_InitWatchdog( VOID );
/* ��λ���Ź� */
INT DEVI_ResetWatchdog ( VOID );
/* ��ʼ�����е� */
INT DEVI_InitializeRunLed ( VOID );
/* �������е� ledStatus:����״̬(0:��  1:��) */
INT DEVI_WriteRunLed ( INT ledStatus );
/* ��ʼ���澯�� ledStatus:����״̬(0:��  1:��) */
INT DEVI_InitializeAlarmLed ( VOID );
/* ���ø澯�� */
INT DEVI_WriteAlarmLed ( INT ledStatus );
/* �����û��� */
INT DEVI_WriteUserLed ( INT iGroup, UNSIGNED ledBits );
/* ��ʼ���û��� */
INT DEVI_InitializeUserLed ( VOID );
/* ��ʼ�������̵��� */
INT DEVI_InitializeStartRelay ( VOID );
/* ���������̵��� Offset:��ʼλ�� Number:���� buffer_ptr:��ֵ  */
INT DEVI_WriteStartRelay ( INT Offset, INT Number, VOID * buffer_ptr );
/* ���GPS�����жϱ�־ */
INT DEVI_ClearGPS ( VOID );
/* ��ȡGPS����״̬ */
INT DEVI_GetGPSStatus ( VOID );
/* ���GPS�����豸 */
INT DEVI_CheckGPSPulseDevice ( VOID );
/* ���B���жϱ�־ */
INT DEVI_ClearIRIGB ( VOID );
/* ��ȡB��״̬ */
INT DEVI_GetIRIGBStatus ( UNSIGNED * buffer );
/* ���GPS B���豸 */
INT DEVI_CheckIRIGBDevice ( VOID );
/* ��ʼ��A/DоƬ sample_points:��������ѡ�� chl_number:����ͨ���� ����ֵ:����������ַ */
float * DEVI_InitializeAD ( INT sample_points, UNSIGNED * chl_number );
/* ��A/D�������� Number:ͨ���� buffer_ptr:�������ݴ�Ż����� */
INT DEVI_GetSampleData ( INT Number, VOID * buffer_ptr );
/* ����AD���� */
INT DEVI_SetADGain( UNSIGNED value );
/* ����������� */
INT DEVI_OutputSamplePulse ( VOID * buffer_ptr );
/* ��ʼ���������� */
INT DEVI_InitializeSamplePulse ( INT sample_points, INT pulse_force );
/* ��ȡ���̼�ֵ */
UNSIGNED DEVI_GetKeyValue ( VOID );
/* ��ȡӲ��(CPLD)�汾�� */
UNSIGNED DEVI_GetCPLDVersion ( VOID );
/* CPU������ó�ʼ�� */
INT DEVI_InitializeCPU ( VOID );
/* ��λCPU */
VOID DEVI_ResetCPU ( VOID );
/* �����λ״̬��Ϣ */
INT DEVI_RsrInfCPU ( VOID );
/* ��ʼ�����ڶ�ʱ�� */
INT DEVI_InitializePerTimer ( UNSIGNED frequency );
/* ������ڶ�ʱ����־ */
INT DEVI_ClearPerTimer ( VOID );
/* ��ȡ��ʱ������ */
UNSIGNED DEVI_GetTimerPeriod ( VOID );
/* ��ʼ��ʱ��оƬ */
INT DEVI_InitializeTimeChip ( VOID );
/* ��ʱ��оƬʱ�� */
INT DEVI_GetChipTime ( INT Number, VOID * buffer_ptr );
/* дʱ��оƬʱ�� */
INT DEVI_SaveChipTime ( INT Number, VOID * buffer_ptr );
/* ��ʼ���¶ȴ����� */
INT DEVI_InitializeTemperature ( VOID );
/* ��ȡ�¶ȴ��������� */
INT DEVI_GetTemperature ( INT * temperature_ptr );
/* SW���� */
INT DEVI_SWDriver( INT iFlag );
/* ������·�ض����� */
INT DEVI_OutReadbackDriver( INT Offset, INT Number, VOID * buffer_ptr );
/* ��������ض����� */
INT DEVI_PlugReadbackDriver( INT Offset, INT Number, VOID * buffer_ptr );
/* �����������뷽��ֵ */
INT DEVI_InitializeOrthogonalCode_direction ( unsigned short int dirSetting );
/* �������������ʼֵ */
INT DEVI_InitializeOrthogonalCode_value ( INT iGroup, unsigned short int iniValue );
/* ��ȡ�������뷽�� */
INT DEVI_GetOrthogonalCodeDirection ( UNSIGNED * dirSetting );
/* ��ȡ��������ֵ */
INT DEVI_GetOrthogonalCodeValue ( INT iGroup, UNSIGNED * iniValue );
/* ����PPS��� */
INT DEVI_SetWidth_PPS ( short int Value );
/* ��ȡ����ʱ��ʱ��� */
INT DEVI_GetPPSTimeStamp( UNSIGNED * pBuffer );
/* ����PPS��Ȼض��Ƿ�������� */
INT DEVI_PPSSetWidthReadBack( INT * pBuffer );
/* ��ȡGtime��ʱ��CTֵ */
INT DEVI_ReadGTimeCnr1 ( VOID * buffer_ptr );
/* UART���ڳ�ʼ�� */
INT DEVI_UartComInit( INT channel, INT baud_rate, INT parity );
/* ����UART485ʹ�ܱ�ʶ */
INT DEVI_UART_RS485_SetEnFlag( int channel, int iEnFlag );
/* ��UART����д���� */
INT DEVI_UART_WriteComm( INT channel, UNSIGNED_CHAR * buf, INT len );
/* ��UART���ڶ�ȡ���� */
INT DEVI_UART_ReadComm( INT channel, UNSIGNED_CHAR * buf );
/* ��ȡUART�ж�״̬ */
INT DEVI_UART_GetPortIntFlag( INT channel );
/* �������豸 */
INT DEVI_CheckFiberDevice( VOID );
/* ��ȡ�������״̬��Ϣ */
INT DEVI_GetMotorStatus ( UNSIGNED * motor_status );
/* ���ò�������������� */
INT DEVI_SetMotorCommand( INT channel, UNSIGNED command );
/* ���ò������������ */
INT DEVI_SetMotorPulse( INT channel, UNSIGNED pulse );
/* ���õ�ŷ��������� */
INT DEVI_SetSolenoidValveCommand( INT channel, UNSIGNED command );
/* ��ʱ��оƬram������(Ferroelectric Nonvolatile RAM) */
INT DEVI_GetDataFNRam ( UNSIGNED addrOffset, CHAR * buff , INT len );
/* ��ʱ��оƬRAM��д����(Ferroelectric Nonvolatile RAM) */
INT DEVI_SaveDataFNRam ( UNSIGNED addrOffset, CHAR * buff , INT len );

#endif
