
/*************************************************************************/
/*                                                                       */
/*        Copyright (c) 1999-2002 XJ ELECTRIC CO.LTD.                    */
/*                                                                       */
/* PROPRIETARY RIGHTS of XJ ELECTRIC CO.LTD are involved in the          */
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
/*      cioc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CIO - Common Input/Output Management                             */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common input/output */
/*      management component.                                            */
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
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*      cio_extr.h                          Protect Control functions    */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

#include <string.h>
#include <math.h>
#include "plat/inc/xj_cosc.h"
#include "plat/inc/cout_extr.h"
#include "plat/inc/xj_out_check.h"
#include "plat_specific/plat_specific.h"
#include "plat/inc/devi_extr.h"
#include "plat/es_plat.h"

/* ����ΪOS������� */
#ifdef WIN32
#include <stdio.h>
#include <malloc.h>
/* Windows �궨�� */
#endif

#ifndef WIN32 /* ��windowsȱʡ����һЩ���Ա�ͨ������ */
/* ȫ�ֱ������� */
/* Nucleus�궨�� */
#endif

/* Define external inner-component global data references.  */
UNSIGNED COUTC_RelayBits[ CPC_MAX_OUTPUT_GROUPS ];

/* Define internal function calls. */
VOID COUTC_User_RelayDriver( UNSIGNED GroupNo, UNSIGNED uBits );
VOID COUTC_User_StartDriver( UNSIGNED Flag );

/* ���ÿ����������� */
INT COUTC_RelayDriver( INT GroupNumber, UNSIGNED RelayBits )
{
    INT k;
    INT status;
    UNSIGNED Variable;
    ESP_CFG_OUTPUT_PORT * pOutputPort;
    UNSIGNED_CHAR * pOutputPortAddress8;
    unsigned short int * pOutputPortAddress16;
    UNSIGNED * pOutputPortAddress32;

    status = XJ_UNAVAILABLE;
    if( GroupNumber < COUTC_H_OUTPUT_GROUP_NUMBER )
    {
        /* ����Ӳ��������״̬ */
        Variable = RelayBits;
        status = DEVI_RelayDriver( GroupNumber, 1, & Variable );
    }
    else if( GroupNumber < COUTC_OUTPUT_GROUP_NUMBER )
    {
        pOutputPort = COUTC_OUTPUT_PORT_MEMORY; /* �����˿��׵�ַ */
        pOutputPort += GroupNumber;
        Variable = RelayBits;
        Variable ^= ( pOutputPort->ReverseBits );
        switch( pOutputPort->PortBits )
        {
        case 8:  /* 8λ�˿����� */
            for( k = 0; k < pOutputPort->PortNumber; k ++)
            {
                /* д�����˿����� */
                pOutputPortAddress8 = ( UNSIGNED_CHAR *)( pOutputPort->PortAddress[k] );
                * pOutputPortAddress8 =
                    ( UNSIGNED_CHAR )(( Variable >> ( k * pOutputPort->PortBits )) & ( pOutputPort->PortMasks ));
            }
            COUTC_RelayBits[GroupNumber] = RelayBits;
            status = XJ_SUCCESS;
            break;
        case 16: /* 16λ�˿����� */
            for( k = 0; k < pOutputPort->PortNumber; k ++)
            {
                /* д�����˿����� */
                pOutputPortAddress16 = ( unsigned short int *)( pOutputPort->PortAddress[k] );
                * pOutputPortAddress16 =
                    ( unsigned short int )(( Variable >> ( k * pOutputPort->PortBits )) & ( pOutputPort->PortMasks ));
            }
            COUTC_RelayBits[GroupNumber] = RelayBits;
            status = XJ_SUCCESS;
            break;
        case 32: /* 32λ�˿����� */
            pOutputPortAddress32 = ( UNSIGNED *)( pOutputPort->PortAddress[0] );
            /* д�����˿����� */
            * pOutputPortAddress32 = Variable & ( pOutputPort->PortMasks );
            COUTC_RelayBits[GroupNumber] = RelayBits;
            status = XJ_SUCCESS;
            break;
        default :
            break;
        }
    }
    return( status );
}

/* ִ�п����û��������� */
VOID COUTC_User_RelayDriver( UNSIGNED GroupNo, UNSIGNED uBits )
{
    /* ��������������ƽ̨֧�ֵ��������֮��,ֱ�ӷ��� */
    if( GroupNo >= CPC_MAX_OUTPUT_GROUPS )
    {
        return;
    }

    /* ���ڼ��ʱ��Ҫ֪������״̬ */
    /* ���ó��ڼ̵�������λ */
    OUTC_Check_SetPickupBits( GroupNo, uBits );
    /* ����ִ�к���,�˴���Ӧֱ�ӵ���COUTC_RelayDriver,��Ϊ�б������ڵ���� */
    OUTC_DriveFunction();
    /* �����ִ��λ�仯��麯�� */
    OUTC_SoftOutExBitsCheck( );
}

/* ִ�������̵����û��������� */
VOID COUTC_User_StartDriver( UNSIGNED Flag )
{
    /* ���ڼ����Ҫ֪�������̵�����״̬ */
    OUTC_Check_StartDriver( Flag );
    /* �˴���Ӧֱ�����������̵���,�����̵���Ӧ����ʱ���� */
}

/* ���ÿ�����Ʒ������,���û�ȡ�¼�λ */
INT COUTC_SetOutputQualityBits( INT GroupNumber, UNSIGNED QualityBits )
{
    INT status;
    UNSIGNED * pQuality;
    ESP_CFG_OUTPUT_PORT * pOutputPort;

    status = XJ_UNAVAILABLE;
    if(( GroupNumber >= COUTC_H_OUTPUT_GROUP_NUMBER )
        && ( GroupNumber < COUTC_OUTPUT_GROUP_NUMBER ))
    {
        /* ��Ч������ */
        pOutputPort = COUTC_OUTPUT_PORT_MEMORY; /* �����˿��׵�ַ */
        pOutputPort += GroupNumber;
        pQuality = ( UNSIGNED *)pOutputPort->QualityAddress;
        * pQuality = QualityBits;
        status = XJ_SUCCESS;
    }

    return( status );
}

/* ��ʼ������ */
VOID COUTC_OutputInitialize( VOID )
{
    /* ע�Ὺ���̵����û��������� */
    XJP_IO_Set_Output_Driver( COUTC_User_RelayDriver );
    /* ע�������̵����û��������� */
    XJP_IO_Set_Start_Driver( COUTC_User_StartDriver );
}

/* �����ֿ�����״̬��Ʒ������ */
INT COUTC_ReadSoftOutput( INT iGroup, UNSIGNED * pBits, UNSIGNED * pQuality )
{
    UNSIGNED * p;
    ESP_CFG_OUTPUT_PORT * pOutputPort;

    if(( iGroup < COUTC_H_OUTPUT_GROUP_NUMBER ) ||
        ( iGroup >= ( COUTC_H_OUTPUT_GROUP_NUMBER + COUTC_S_OUTPUT_GROUP_NUMBER )))
    {
        /* �����ֿ�������ֱ�ӷ��� */
        * pBits = 0;
        * pQuality = 0;
        return( XJ_UNAVAILABLE );
    }

    pOutputPort = COUTC_OUTPUT_PORT_MEMORY; /* �����˿��׵�ַ */
    pOutputPort += iGroup;
    p = ( UNSIGNED *)pOutputPort->PortAddress[0];
    * pBits = * p;
    p = ( UNSIGNED *)pOutputPort->QualityAddress;
    * pQuality = * p;

    return( XJ_SUCCESS );
}

/* ���ÿ�����Ʒ�����ؼ���λ,���û�ȡ�¼�λ */
INT COUTC_SetOutCheckQualityBits( INT GroupNumber, UNSIGNED CheckQuaBits )
{
    INT status;
    UNSIGNED * pQuality;
    ESP_CFG_OUTPUT_PORT * pOutputPort;

    status = XJ_UNAVAILABLE;
    if(( GroupNumber >= COUTC_H_OUTPUT_GROUP_NUMBER )
        && ( GroupNumber < COUTC_OUTPUT_GROUP_NUMBER ))
    {
        /* ��Ч������ */
        pOutputPort = COUTC_OUTPUT_PORT_MEMORY; /* �����˿��׵�ַ */
        pOutputPort += GroupNumber;
        pQuality = ( UNSIGNED *)pOutputPort->QualityCheckAddr;
        * pQuality = CheckQuaBits;
        status = XJ_SUCCESS;
    }

    return( status );
}

/* �����ֿ�����״̬,Ʒ������(��Ч��Ч������λ) */
INT COUTC_ReadSoftOutTotalQua( INT  iGroup, UNSIGNED * pBits, UNSIGNED * pQuality, UNSIGNED * pCheckQuality )
{
    UNSIGNED * p;
    ESP_CFG_OUTPUT_PORT * pOutputPort;

    if(( iGroup < COUTC_H_OUTPUT_GROUP_NUMBER ) ||
        ( iGroup >= ( COUTC_H_OUTPUT_GROUP_NUMBER + COUTC_S_OUTPUT_GROUP_NUMBER )))
    {
        /* �����ֿ�������ֱ�ӷ��� */
        * pBits = 0;
        * pQuality = 0;
        * pCheckQuality = 0;
        return( XJ_UNAVAILABLE );
    }

    pOutputPort = COUTC_OUTPUT_PORT_MEMORY; /* �����˿��׵�ַ */
    pOutputPort += iGroup;
    p = ( UNSIGNED *)pOutputPort->PortAddress[0];
    * pBits = * p;
    p = ( UNSIGNED *)pOutputPort->QualityAddress;
    * pQuality = * p;
    p = ( UNSIGNED *)pOutputPort->QualityCheckAddr;
    * pCheckQuality = * p;

    return( XJ_SUCCESS );
}
