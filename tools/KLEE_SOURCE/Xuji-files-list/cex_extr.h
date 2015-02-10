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
/*      cex_extr.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CEX - Common Exchange data                                       */
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
/*      pro_defs.h                          Protect Control constants    */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhideYang.      08-13-2008      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"

/* Check to see if the file has been included already.  */

#ifndef CEX_EXTR_H
#define CEX_EXTR_H

#define CEX_PLAT_EXCHANGE_NUMBER            2 /* ˫CPUƽ̨�������ݵĸ��� */

/* ��������λ��ƫ�ƶ��� */
#define CEX_EXCHANGE_START_OFFSET           0                       /* �����̵���״̬λ��ƫ�� */
#define CEX_EXCHANGE_FAULT_NUMBER_OFFSET    1                       /* �������λ��ƫ�� */
#define CEX_EXCHANGE_USER_DATA_OFFSET       2                       /* �û����ݵ�λ��ƫ�� */

/*��̬�ڴ��������ӿ����ݽṹ�Ͳ���*/
typedef void * (* EX_MemAlloc )( size_t );
typedef void (* EX_MemFree )( void *);
struct EXMemMService
{
    /* �����ڴ� */
    EX_MemAlloc MemAlloc;
    /* �ͷ��ڴ� */
    EX_MemFree MemFree;
};

/* ˫CPU����������ṹ */
typedef struct CEXCHANGE_STRUCT CEXCHANGE;
/* ˫CPU����������ṹ */
struct CEXCHANGE_STRUCT
{
    /*private: */
    /* ˫CPU�������ݸ��� */
    INT CpuExchangeNumber;
    /* ˫CPU�������ݵĽ��ջ����� */
    VOID * CpuExchangeRecv;
    /* ˫CPU�������ݵķ��ͻ����� */
    VOID * CpuExchangeSend;
    /* �û��������ݸ��� */
    INT UserExchangeNumber;
    /* �û��������ݵĽ��ջ����� */
    VOID * UserExchangeRecv;
    /* �û��������ݵķ��ͻ����� */
    VOID * UserExchangeSend;
};

/* ��ʼ������������Ϣ */
INT CEXC_Initialize( INT UserExchangeNumber );
/* ��ȡ�������Բ�CPU���ı��������� */
INT CEXC_GetExchangeFaultNumber( VOID );
/* ��ȡ���յ����û��������� */
INT CEXC_ReadUserExchangeData( UNSIGNED * pBuffer );
/* ������Ҫ���͵��û��������� */
INT CEXC_SendUserExchangeData( UNSIGNED * pBuffer );

#endif
