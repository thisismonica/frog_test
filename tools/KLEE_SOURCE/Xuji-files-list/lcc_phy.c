/********************************************************************************/
/*  Copyight (C) 2001-2003      �������Զ�����ҵ������                          */
/*                                                                              */
/* All right reserved                                                           */
/*                                                                              */
/* FILE NAME                        VERSION                                     */
/*                                                                              */
/*  lcc.c                       PLUS  1.0                                       */
/*                                                                              */
/* COMPONENT                                                                    */
/*                                                                              */
/*      lc - lon commuciation                                                   */
/*      c   - core                                                              */
/*                                                                              */
/* DESCRIPTION                                                                  */
/*                                                                              */
/*  This file contains the core routines for Record wave                        */
/*                                                                              */
/*                                                                              */
/* AUTHOR                                                                       */
/*                                                                              */
/*  Chenyufeng      < ChenYuFeng@mail.xjpa.xj>                                  */
/* DATA STRUCTURES                                                              */
/*                                                                              */
/*  None                                                                        */
/*                                                                              */
/* HISTORY                                                                      */
/*                                                                              */
/*                                                                              */
/*      NAME        DATE            REMARKS                                     */
/*                                                                              */
/*      ChenYuFeng      06-20-2003                                              */
/********************************************************************************/
#include "plat/net_inc/lc_extr.h"

/* д�����ַ�ĳ�ʱʱ�� */
#define LCC_PHY_WRITE_TIMEOUT    100

/* ����ΪOS������� */
#ifdef WIN32
#include <stdio.h>
#include <malloc.h>
/* ȫ�ֱ������� */
#endif

#ifndef WIN32 /* ��windowsȱʡ����һЩ���Ա�ͨ������ */
#endif

LCC_COM_ERR  LCC_ComErr;
/* ��ǰͨ��ָ���ַ */
GW_CHANNEL * LCC_pActive_Channel = NU_NULL;
/* LON���������ݻ����� */
extern UNSIGNED_CHAR LON_TxdBufferRem[];
/* LON���������ݻ����� */
extern UNSIGNED_CHAR LON_RxdBufferRem[];

/*******************lc physical channel function*****************/
/* ����Ӳ����ʼ������ */
VOID LCC_Physical_OpenChannel( GW_CHANNEL * pChl )
{
#if ( LC_LINK_HMI_PHY_COM )
    CPC_UNUSED_PARAM( pChl );
    LON_Master_Init();
#else
    INT status;
    /* �建���� */
    memset(( VOID *)pChl->chl_mRead_Buff, 0, LC_CHANNEL_QUEUE_SIZE );
    memset(( VOID *)pChl->chl_mWrite_Buff, 0, LC_CHANNEL_QUEUE_SIZE );

    status = NU_Create_Queue(& pChl->chl_mRead_que, "read", pChl->chl_mRead_Buff,
        LC_CHANNEL_QUEUE_SIZE, NU_VARIABLE_SIZE, LC_MAX_MSG_SIZE, NU_FIFO );

    status = NU_Create_Queue(& pChl->chl_mWrite_que, "Secnod", pChl->chl_mWrite_Buff,
        LC_CHANNEL_QUEUE_SIZE, NU_VARIABLE_SIZE, LC_MAX_MSG_SIZE, NU_FIFO );
    /* ��̨�������ͨѶ������Ϣ���г�ʼ������ */
    status = LCC_BackTaskBCallBufferInit();
#endif
    return;
}

/* ������Ӳ���ж�ȡ���ݺ��� */
INT LCC_Physical_Read( GW_CHANNEL * pChl, UNSIGNED_CHAR * data )
{
    INT size = 0;
#if ( LC_LINK_HMI_PHY_COM )
    CPC_UNUSED_PARAM( pChl );
    size = LON_Pio_Read( data );
#else
    UNSIGNED    actual_size;
    if ( LCC_Link_Receive_From_Queue(& pChl->chl_mRead_que, data, LC_MAX_MSG_SIZE,
        & actual_size, NU_NO_SUSPEND, sizeof( UNSIGNED_CHAR ) ) == NU_SUCCESS )
    {
        size = *( data + 1 ) + LC_VARIABLE_BASE_LENGTH;
    }
#endif

    /* return */
    return( size );
}

/* ������Ӳ��д���� */
INT LCC_Physical_Write( GW_CHANNEL * pChl, UNSIGNED_CHAR * data, INT size )
{
    INT iStatus;
#if ( LC_LINK_HMI_PHY_COM )
    CPC_UNUSED_PARAM( pChl );
    LON_Pio_Write( data, size );
    iStatus = NU_SUCCESS;
#else
    iStatus = LCC_Link_Send_To_Queue(& pChl->chl_mWrite_que, data, size,
        LCC_PHY_WRITE_TIMEOUT, sizeof( UNSIGNED_CHAR ) );
    if( iStatus != NU_SUCCESS )
    {
        LCC_ComErr.PlatSendErr ++;
    }
#endif
    return( iStatus );
}

/* �е�ѹװ��ע����ӿڵĶ�д���� */
#if (! LC_LINK_HMI_PHY_COM )
INT LCC_Read_Command( VOID * buffer, UNSIGNED * actual_size, UNSIGNED timeout )
{
    INT iStatus;
    UNSIGNED size = 0;
    UNSIGNED_CHAR * pChar;
    iStatus = LCC_Link_Receive_From_Queue(& LCC_pActive_Channel->chl_mWrite_que,
        buffer, LC_MAX_MSG_SIZE,
        & size, timeout, sizeof( UNSIGNED_CHAR ) );
    * actual_size = 0;
    if( iStatus == NU_SUCCESS )
    {
        pChar = ( UNSIGNED_CHAR *)buffer;
        * actual_size = *( pChar + 1 ) + LC_VARIABLE_BASE_LENGTH;
    }
    return( iStatus );
}

INT LCC_Write_Command( VOID * buffer, UNSIGNED actual_size, UNSIGNED timeout )
{
    INT iStatus;
    iStatus = LCC_Link_Send_To_Queue(& LCC_pActive_Channel->chl_mRead_que, buffer,
        actual_size, timeout, sizeof( UNSIGNED_CHAR ) );
    if( iStatus != NU_SUCCESS )
    {
        LCC_ComErr.HMISendErr ++;
    }
    return( iStatus );
}
#endif
