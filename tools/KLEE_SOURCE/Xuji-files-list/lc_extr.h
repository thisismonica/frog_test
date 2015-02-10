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
/*      lc_extr.h                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      LC - Lonworks Communication                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains function prototypes of all functions          */
/*      accessible to other components.                                  */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      Chenyufeng, XJ ELECTRIC CO.LTD.                                  */
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
/*      lc_defs.h                    Lonworks Communication constants    */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      YuFengCheng     09-01-2002      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"
#include "plat/net_inc/lc_defs.H"
#include "plat/net_inc/lc_bcall.h"

/* Check to see if the file has been included already.  */
#ifndef LC_EXTR_H
#define LC_EXTR_H

/* 当前通道指针地址 */
extern GW_CHANNEL * LCC_pActive_Channel;
/* LON网发送数据缓冲区 */
extern UNSIGNED_CHAR LON_TxdBufferRem[];
/* LON网接收数据缓冲区 */
extern UNSIGNED_CHAR LON_RxdBufferRem[];
/* 系统时钟 */
extern volatile UNSIGNED TMD_System_Clock;
/* 消息队列"First" */
extern NU_QUEUE      LCC_Link_Primary_QUE;
/* 消息队列"Secnod" */
extern NU_QUEUE      LCC_Link_Second_QUE;
/* event1 block */
extern NU_EVENT_GROUP   Event_1_CB;
/* event2 block */
extern NU_EVENT_GROUP   Event_2_CB;
/*============物理硬件管理函数============*/
/* 物理硬件初始化函数 */
VOID LCC_Physical_OpenChannel( GW_CHANNEL * pChl );
/* 从物理硬件中读取数据函数 */
INT  LCC_Physical_Read( GW_CHANNEL * pChl, UNSIGNED_CHAR * data );
/* 向物理硬件写数据 */
INT  LCC_Physical_Write( GW_CHANNEL * pChl, UNSIGNED_CHAR * data, INT size );
#if (LC_LINK_HMI_PHY_COM)
/* LON网初始化函数 */
VOID LON_Master_Init( VOID );
/* LON网读数据函数 */
INT LON_Pio_Read( UNSIGNED_CHAR  * data );
/* LON网写数据函数 */
INT LON_Pio_Write( UNSIGNED_CHAR * data, INT size );
#else
/* 中低压装置注册给接口的读写函数 */
INT LCC_Read_Command( VOID * buffer, UNSIGNED * actual_size, UNSIGNED timeout );
INT LCC_Write_Command( VOID * buffer, UNSIGNED actual_size, UNSIGNED timeout );
#endif
/*============应用管理函数============*/
/* 通讯初始化函数 */
GW_CHANNEL * LCC_Link_OpenChannel( UNSIGNED chlID );
/* 读数据 */
INT LCC_Link_Read( GW_CHANNEL * pChl, UNSIGNED_CHAR * data );
/* 写数据 */
INT LCC_Link_Write( GW_CHANNEL * pChl, UNSIGNED_CHAR * data, INT size );
/* 初始化通讯通道 */
VOID LCC_Link_ChlInitialize( GW_CHANNEL * pChl, UNSIGNED  control_mask );
/* 通讯内存空间申请 */
VOID LCC_Link_AllocateMemory( VOID );
/* 超时检测函数 */
VOID LCC_Link_CheckTimer( GW_CHANNEL * pChl );
/*============通讯数据处理函数============*/
/* 通讯任务主函数 */
VOID LCC_MessM_Main( GW_CHANNEL * pChl );
/* 检查FCB是否变位 */
INT LCC_MessM_CheckFCB( GW_CHANNEL * pChl, UNSIGNED_CHAR ucFCB );
/* 检查帧是否正确 */
INT LCC_MessM_CheckFrame( UNSIGNED_CHAR * data, GW_UCHAR rx_size );
/* 计算校验和 */
INT LCC_MessM_CalculateSum( UNSIGNED_CHAR * data );
/* 报文确认 */
INT LCC_MessM_PackConfirm( UNSIGNED_CHAR * dst, UNSIGNED_CHAR port, UNSIGNED_CHAR type, UNSIGNED_CHAR code );
/* 组织帧头 */
INT LCC_MessM_PackHead( INT actual_size, UNSIGNED_CHAR * p, UNSIGNED_CHAR dst_port );
/* 数据解析 */
VOID LCC_MessM_ParseData( GW_CHANNEL * pChl );
/* 解析命令类型 */
INT  LCC_MessM_ParseCommand( GW_CHANNEL * pChl, GW_UCHAR * src, GW_USHORT rx_size );
/* 发送数据打包函数 */
INT  LCC_MessM_PackData( GW_CHANNEL * pChl, GW_UCHAR * dst );
/* 主动上送数据处理函数 */
INT LCC_MessM_SelectFun( GW_CHANNEL * pChl, GW_UCHAR sequence, UNSIGNED_CHAR * dst );
/* 数据巡检函数 */
INT LCC_MessM_PollingData( GW_CHANNEL * pChl, UNSIGNED_CHAR * dst );
/* 命令函数表检测函数,看其对应的命令类型是否有执行函数 */
GW_COMMAND_CALLBACK_NODE * LCC_MessM_LookupNode( GW_CHANNEL   * pChl, GW_UCHAR type );
/*--------------------------------------------------------*/
/* 应用程序发送通讯数据
参数说明: actual_size 数据长度(最大为244)
        : pBuffer 数据缓冲区
        : iDataLevel 数据级别(1:为一级数据,2:为二级数据)
        : iSendDire 数据发送方向(在pro_defs.h头文件中有定义,应用需要平台透传命令信息定义)
*/
INT LCC_UserSendComData( INT actual_size, UNSIGNED_CHAR * pBuffer, INT iDataLevel, INT iSendDire );
/*============通讯命令管理函数============*/
/* 注册通讯命令函数 */
INT LCC_COMM_RegisterCommand( VOID * pChannel, UNSIGNED_CHAR type, void * callback );
/* 对时命令 */
INT LCC_COMM_ProcessTimeSyn( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port );
/* ASDU_5上调装置版本号 */
INT LCC_COMM_ProcessVersion( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port );
/* 上调数据模板启停函数 */
INT LCC_COMM_ProcessStartDT( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port );
/* 上调lon版本号 */
INT LCC_COMM_ProcessLonVersion( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port );
/* 时钟初始化函数 */
VOID LCC_COMM_ClockInitialize( GW_CHANNEL * pChl );
/*============需要长时间处理的命令通过节点的方式来处理============*/
/* 创建节点 */
GW_REQUEST_CB * LCC_CB_CreateActiveCB( GW_CHANNEL * pChl , INT size, GW_UCHAR sesn , GW_UCHAR sect,
    GW_UCHAR priority , GW_UCHAR dest_port, GW_CB_ACK ack_callback, GW_ULONG timeout );
/* 获取节点 */
GW_REQUEST_CB * LCC_CB_GetActiveAckCB( GW_CHANNEL * pChl );
/* 释放节点 */
VOID LCC_CB_freeAckCB( GW_CHANNEL * pChl, VOID * pCB );
/* 命令处理 */
INT  LCC_CB_ProcessBlockIO_CB( GW_CHANNEL * pChl, GW_UCHAR * dst, GW_UCHAR * src, GW_UCHAR dst_port );
/* 节点执行函数 */
INT  LCC_CB_AckBlockIO( UNSIGNED_CHAR * dst, GW_CHANNEL   * pChl, VOID  * pCB );
/* 注册需要长时间处理的命令节点 */
INT LCC_CB_ProcessBlockIO( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port );
/* 因操作系统的消息队列中数据长度为UNSIGNED数据个数,而实际使用中用到的通讯报文均为CHAR,所以现在将其操作系统的函数封装 */
/* 发送消息队列 */
STATUS LCC_Link_Send_To_Queue( NU_QUEUE * queue_ptr, VOID * message, UNSIGNED size, UNSIGNED suspend, UNSIGNED uDataSize );
/* 接收消息队列 */
STATUS LCC_Link_Receive_From_Queue( NU_QUEUE * queue, VOID * message, UNSIGNED size,
    UNSIGNED * actual_size, UNSIGNED suspend, UNSIGNED uDataSize );

#endif
