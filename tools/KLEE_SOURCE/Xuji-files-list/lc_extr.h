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

/* ��ǰͨ��ָ���ַ */
extern GW_CHANNEL * LCC_pActive_Channel;
/* LON���������ݻ����� */
extern UNSIGNED_CHAR LON_TxdBufferRem[];
/* LON���������ݻ����� */
extern UNSIGNED_CHAR LON_RxdBufferRem[];
/* ϵͳʱ�� */
extern volatile UNSIGNED TMD_System_Clock;
/* ��Ϣ����"First" */
extern NU_QUEUE      LCC_Link_Primary_QUE;
/* ��Ϣ����"Secnod" */
extern NU_QUEUE      LCC_Link_Second_QUE;
/* event1 block */
extern NU_EVENT_GROUP   Event_1_CB;
/* event2 block */
extern NU_EVENT_GROUP   Event_2_CB;
/*============����Ӳ��������============*/
/* ����Ӳ����ʼ������ */
VOID LCC_Physical_OpenChannel( GW_CHANNEL * pChl );
/* ������Ӳ���ж�ȡ���ݺ��� */
INT  LCC_Physical_Read( GW_CHANNEL * pChl, UNSIGNED_CHAR * data );
/* ������Ӳ��д���� */
INT  LCC_Physical_Write( GW_CHANNEL * pChl, UNSIGNED_CHAR * data, INT size );
#if (LC_LINK_HMI_PHY_COM)
/* LON����ʼ������ */
VOID LON_Master_Init( VOID );
/* LON�������ݺ��� */
INT LON_Pio_Read( UNSIGNED_CHAR  * data );
/* LON��д���ݺ��� */
INT LON_Pio_Write( UNSIGNED_CHAR * data, INT size );
#else
/* �е�ѹװ��ע����ӿڵĶ�д���� */
INT LCC_Read_Command( VOID * buffer, UNSIGNED * actual_size, UNSIGNED timeout );
INT LCC_Write_Command( VOID * buffer, UNSIGNED actual_size, UNSIGNED timeout );
#endif
/*============Ӧ�ù�����============*/
/* ͨѶ��ʼ������ */
GW_CHANNEL * LCC_Link_OpenChannel( UNSIGNED chlID );
/* ������ */
INT LCC_Link_Read( GW_CHANNEL * pChl, UNSIGNED_CHAR * data );
/* д���� */
INT LCC_Link_Write( GW_CHANNEL * pChl, UNSIGNED_CHAR * data, INT size );
/* ��ʼ��ͨѶͨ�� */
VOID LCC_Link_ChlInitialize( GW_CHANNEL * pChl, UNSIGNED  control_mask );
/* ͨѶ�ڴ�ռ����� */
VOID LCC_Link_AllocateMemory( VOID );
/* ��ʱ��⺯�� */
VOID LCC_Link_CheckTimer( GW_CHANNEL * pChl );
/*============ͨѶ���ݴ�����============*/
/* ͨѶ���������� */
VOID LCC_MessM_Main( GW_CHANNEL * pChl );
/* ���FCB�Ƿ��λ */
INT LCC_MessM_CheckFCB( GW_CHANNEL * pChl, UNSIGNED_CHAR ucFCB );
/* ���֡�Ƿ���ȷ */
INT LCC_MessM_CheckFrame( UNSIGNED_CHAR * data, GW_UCHAR rx_size );
/* ����У��� */
INT LCC_MessM_CalculateSum( UNSIGNED_CHAR * data );
/* ����ȷ�� */
INT LCC_MessM_PackConfirm( UNSIGNED_CHAR * dst, UNSIGNED_CHAR port, UNSIGNED_CHAR type, UNSIGNED_CHAR code );
/* ��֯֡ͷ */
INT LCC_MessM_PackHead( INT actual_size, UNSIGNED_CHAR * p, UNSIGNED_CHAR dst_port );
/* ���ݽ��� */
VOID LCC_MessM_ParseData( GW_CHANNEL * pChl );
/* ������������ */
INT  LCC_MessM_ParseCommand( GW_CHANNEL * pChl, GW_UCHAR * src, GW_USHORT rx_size );
/* �������ݴ������ */
INT  LCC_MessM_PackData( GW_CHANNEL * pChl, GW_UCHAR * dst );
/* �����������ݴ����� */
INT LCC_MessM_SelectFun( GW_CHANNEL * pChl, GW_UCHAR sequence, UNSIGNED_CHAR * dst );
/* ����Ѳ�캯�� */
INT LCC_MessM_PollingData( GW_CHANNEL * pChl, UNSIGNED_CHAR * dst );
/* ��������⺯��,�����Ӧ�����������Ƿ���ִ�к��� */
GW_COMMAND_CALLBACK_NODE * LCC_MessM_LookupNode( GW_CHANNEL   * pChl, GW_UCHAR type );
/*--------------------------------------------------------*/
/* Ӧ�ó�����ͨѶ����
����˵��: actual_size ���ݳ���(���Ϊ244)
        : pBuffer ���ݻ�����
        : iDataLevel ���ݼ���(1:Ϊһ������,2:Ϊ��������)
        : iSendDire ���ݷ��ͷ���(��pro_defs.hͷ�ļ����ж���,Ӧ����Ҫƽ̨͸��������Ϣ����)
*/
INT LCC_UserSendComData( INT actual_size, UNSIGNED_CHAR * pBuffer, INT iDataLevel, INT iSendDire );
/*============ͨѶ���������============*/
/* ע��ͨѶ����� */
INT LCC_COMM_RegisterCommand( VOID * pChannel, UNSIGNED_CHAR type, void * callback );
/* ��ʱ���� */
INT LCC_COMM_ProcessTimeSyn( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port );
/* ASDU_5�ϵ�װ�ð汾�� */
INT LCC_COMM_ProcessVersion( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port );
/* �ϵ�����ģ����ͣ���� */
INT LCC_COMM_ProcessStartDT( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port );
/* �ϵ�lon�汾�� */
INT LCC_COMM_ProcessLonVersion( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port );
/* ʱ�ӳ�ʼ������ */
VOID LCC_COMM_ClockInitialize( GW_CHANNEL * pChl );
/*============��Ҫ��ʱ�䴦�������ͨ���ڵ�ķ�ʽ������============*/
/* �����ڵ� */
GW_REQUEST_CB * LCC_CB_CreateActiveCB( GW_CHANNEL * pChl , INT size, GW_UCHAR sesn , GW_UCHAR sect,
    GW_UCHAR priority , GW_UCHAR dest_port, GW_CB_ACK ack_callback, GW_ULONG timeout );
/* ��ȡ�ڵ� */
GW_REQUEST_CB * LCC_CB_GetActiveAckCB( GW_CHANNEL * pChl );
/* �ͷŽڵ� */
VOID LCC_CB_freeAckCB( GW_CHANNEL * pChl, VOID * pCB );
/* ����� */
INT  LCC_CB_ProcessBlockIO_CB( GW_CHANNEL * pChl, GW_UCHAR * dst, GW_UCHAR * src, GW_UCHAR dst_port );
/* �ڵ�ִ�к��� */
INT  LCC_CB_AckBlockIO( UNSIGNED_CHAR * dst, GW_CHANNEL   * pChl, VOID  * pCB );
/* ע����Ҫ��ʱ�䴦�������ڵ� */
INT LCC_CB_ProcessBlockIO( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port );
/* �����ϵͳ����Ϣ���������ݳ���ΪUNSIGNED���ݸ���,��ʵ��ʹ�����õ���ͨѶ���ľ�ΪCHAR,�������ڽ������ϵͳ�ĺ�����װ */
/* ������Ϣ���� */
STATUS LCC_Link_Send_To_Queue( NU_QUEUE * queue_ptr, VOID * message, UNSIGNED size, UNSIGNED suspend, UNSIGNED uDataSize );
/* ������Ϣ���� */
STATUS LCC_Link_Receive_From_Queue( NU_QUEUE * queue, VOID * message, UNSIGNED size,
    UNSIGNED * actual_size, UNSIGNED suspend, UNSIGNED uDataSize );

#endif
