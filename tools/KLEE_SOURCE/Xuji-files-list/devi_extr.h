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

/* 初始化设备 */
INT DEVI_initialize( VOID );
/* 读取硬件标识号 */
UNSIGNED DEVI_GetIdentificationCode ( VOID );
/* I/O初始化函数 */
INT DEVI_IO_Initialize( VOID );
/* 读开入量 Offset:起始组号 Number:读取组数 buffer_ptr:读取数据存储缓冲区 */
INT DEVI_ReadInput( INT Offset, INT Number, VOID * buffer_ptr );
/* 读开入量品质因素 Offset:起始组号 Number:读取组数 buffer_ptr:读取数据存储缓冲区 */
INT DEVI_ReadInputQuality( INT Offset, INT Number, VOID * buffer_ptr );
/* 读开入量值和品质因素 Offset:起始组号 Number:读取组数 buffer_ptr:读取数据存储缓冲区 */
INT DEVI_ReadInputVQ( INT Offset, INT Number, VOID * buffer_ptr );
/* 开出驱动 Offset:起始组号 Number:读取组数 buffer_ptr:读取数据存储缓冲区 */
INT DEVI_RelayDriver( INT Offset, INT Number, VOID * buffer_ptr );
/* 初始化看门狗 */
INT DEVI_InitWatchdog( VOID );
/* 复位看门狗 */
INT DEVI_ResetWatchdog ( VOID );
/* 初始化运行灯 */
INT DEVI_InitializeRunLed ( VOID );
/* 设置运行灯 ledStatus:设置状态(0:灭  1:亮) */
INT DEVI_WriteRunLed ( INT ledStatus );
/* 初始化告警灯 ledStatus:设置状态(0:灭  1:亮) */
INT DEVI_InitializeAlarmLed ( VOID );
/* 设置告警灯 */
INT DEVI_WriteAlarmLed ( INT ledStatus );
/* 设置用户灯 */
INT DEVI_WriteUserLed ( INT iGroup, UNSIGNED ledBits );
/* 初始化用户灯 */
INT DEVI_InitializeUserLed ( VOID );
/* 初始化启动继电器 */
INT DEVI_InitializeStartRelay ( VOID );
/* 设置启动继电器 Offset:起始位置 Number:个数 buffer_ptr:量值  */
INT DEVI_WriteStartRelay ( INT Offset, INT Number, VOID * buffer_ptr );
/* 清除GPS脉冲中断标志 */
INT DEVI_ClearGPS ( VOID );
/* 读取GPS脉冲状态 */
INT DEVI_GetGPSStatus ( VOID );
/* 检查GPS脉冲设备 */
INT DEVI_CheckGPSPulseDevice ( VOID );
/* 清除B码中断标志 */
INT DEVI_ClearIRIGB ( VOID );
/* 读取B码状态 */
INT DEVI_GetIRIGBStatus ( UNSIGNED * buffer );
/* 检查GPS B码设备 */
INT DEVI_CheckIRIGBDevice ( VOID );
/* 初始化A/D芯片 sample_points:采样点数选择 chl_number:采样通道数 返回值:采样数据首址 */
float * DEVI_InitializeAD ( INT sample_points, UNSIGNED * chl_number );
/* 读A/D采样数据 Number:通道数 buffer_ptr:采样数据存放缓冲区 */
INT DEVI_GetSampleData ( INT Number, VOID * buffer_ptr );
/* 设置AD增益 */
INT DEVI_SetADGain( UNSIGNED value );
/* 采样脉冲输出 */
INT DEVI_OutputSamplePulse ( VOID * buffer_ptr );
/* 初始化采样脉冲 */
INT DEVI_InitializeSamplePulse ( INT sample_points, INT pulse_force );
/* 读取键盘键值 */
UNSIGNED DEVI_GetKeyValue ( VOID );
/* 读取硬件(CPLD)版本号 */
UNSIGNED DEVI_GetCPLDVersion ( VOID );
/* CPU相关设置初始化 */
INT DEVI_InitializeCPU ( VOID );
/* 复位CPU */
VOID DEVI_ResetCPU ( VOID );
/* 输出复位状态信息 */
INT DEVI_RsrInfCPU ( VOID );
/* 初始化周期定时器 */
INT DEVI_InitializePerTimer ( UNSIGNED frequency );
/* 清除周期定时器标志 */
INT DEVI_ClearPerTimer ( VOID );
/* 获取定时器周期 */
UNSIGNED DEVI_GetTimerPeriod ( VOID );
/* 初始化时钟芯片 */
INT DEVI_InitializeTimeChip ( VOID );
/* 读时钟芯片时间 */
INT DEVI_GetChipTime ( INT Number, VOID * buffer_ptr );
/* 写时钟芯片时间 */
INT DEVI_SaveChipTime ( INT Number, VOID * buffer_ptr );
/* 初始化温度传感器 */
INT DEVI_InitializeTemperature ( VOID );
/* 读取温度传感器数据 */
INT DEVI_GetTemperature ( INT * temperature_ptr );
/* SW驱动 */
INT DEVI_SWDriver( INT iFlag );
/* 开出回路回读驱动 */
INT DEVI_OutReadbackDriver( INT Offset, INT Number, VOID * buffer_ptr );
/* 开出插件回读驱动 */
INT DEVI_PlugReadbackDriver( INT Offset, INT Number, VOID * buffer_ptr );
/* 设置正交编码方向定值 */
INT DEVI_InitializeOrthogonalCode_direction ( unsigned short int dirSetting );
/* 设置正交编码初始值 */
INT DEVI_InitializeOrthogonalCode_value ( INT iGroup, unsigned short int iniValue );
/* 读取正交编码方向 */
INT DEVI_GetOrthogonalCodeDirection ( UNSIGNED * dirSetting );
/* 读取正交编码值 */
INT DEVI_GetOrthogonalCodeValue ( INT iGroup, UNSIGNED * iniValue );
/* 设置PPS宽度 */
INT DEVI_SetWidth_PPS ( short int Value );
/* 获取脉冲时刻时间戳 */
INT DEVI_GetPPSTimeStamp( UNSIGNED * pBuffer );
/* 设置PPS宽度回读是否设置完成 */
INT DEVI_PPSSetWidthReadBack( INT * pBuffer );
/* 读取Gtime定时器CT值 */
INT DEVI_ReadGTimeCnr1 ( VOID * buffer_ptr );
/* UART串口初始化 */
INT DEVI_UartComInit( INT channel, INT baud_rate, INT parity );
/* 设置UART485使能标识 */
INT DEVI_UART_RS485_SetEnFlag( int channel, int iEnFlag );
/* 向UART串口写数据 */
INT DEVI_UART_WriteComm( INT channel, UNSIGNED_CHAR * buf, INT len );
/* 从UART串口读取数据 */
INT DEVI_UART_ReadComm( INT channel, UNSIGNED_CHAR * buf );
/* 获取UART中断状态 */
INT DEVI_UART_GetPortIntFlag( INT channel );
/* 检查光纤设备 */
INT DEVI_CheckFiberDevice( VOID );
/* 读取步进电机状态信息 */
INT DEVI_GetMotorStatus ( UNSIGNED * motor_status );
/* 设置步进电机控制命令 */
INT DEVI_SetMotorCommand( INT channel, UNSIGNED command );
/* 设置步进电机脉冲数 */
INT DEVI_SetMotorPulse( INT channel, UNSIGNED pulse );
/* 设置电磁阀控制命令 */
INT DEVI_SetSolenoidValveCommand( INT channel, UNSIGNED command );
/* 读时钟芯片ram区数据(Ferroelectric Nonvolatile RAM) */
INT DEVI_GetDataFNRam ( UNSIGNED addrOffset, CHAR * buff , INT len );
/* 向时钟芯片RAM区写数据(Ferroelectric Nonvolatile RAM) */
INT DEVI_SaveDataFNRam ( UNSIGNED addrOffset, CHAR * buff , INT len );

#endif
