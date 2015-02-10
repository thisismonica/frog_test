
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
#include "plat/inc/cin_extr.h"
#include "plat/inc/devi_extr.h"
#include "plat/es_plat.h"
#include "xml/inc/pxml_cfg.h"

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

UNSIGNED CINC_SwitchStatusMemory[CIN_SWITCH_STATUS_NUMBER * CPC_MAX_SWITCH_STATUS_POINT]; /* ��������λ������ */
UNSIGNED CINC_SwitchStatusMasks[ CPC_MAX_INPUT_GROUPS ]; /* ��λ���Ŀ��������� */
UNSIGNED CINC_SoftInFullMasks[ CPC_MAX_INPUT_GROUPS ];   /* �������鴫��ʱ���� */
UNSIGNED CINC_SwitchStatusValue[ CPC_MAX_INPUT_GROUPS ]; /* ״̬��ֵ */
UNSIGNED * CINC_SwitchStatusIn = CINC_SwitchStatusMemory;
XJ_DRIVER CINC_InputDriver;
UNSIGNED CINC_FckInputCheckFreq = 1000; /* ��ؿ����λ���Ƶ�ʣ�1/�룩  */
/* �������Ƿ����־ */
static INT CINC_InputCheckFlag = 0;
/* Define internal function calls.  */
VOID CINC_InputDriverEntry( XJ_DRIVER pDriver, XJ_DRIVER_REQUEST * pRequest );

/* ��������������������˵����nu_logical_unit����ȡ���������� */
/* nu_offset����ʼƫ����  nu_request_size�������������� */
/* nu_actual_size��ʵ�ʿ���������  nu_buffer_ptr�����ݻ����� */
VOID CINC_InputDriverEntry( XJ_DRIVER pDriver, XJ_DRIVER_REQUEST * pRequest )
{
    INT k;
    UNSIGNED i;
    UNSIGNED * pDst;
    UNSIGNED Number;
    UNSIGNED Offset;
    UNSIGNED Value;
    UNSIGNED Quality;
    UNSIGNED ActualNumber;
    ESP_CFG_INPUT_PORT * pPort;
    UNSIGNED_CHAR * pPortAddr8;
    unsigned short int * pPortAddr16;
    UNSIGNED * pPortAddr32;
    UNSIGNED * pQuality;

    switch( pRequest -> nu_function )
    {
    case XJ_ASSIGN:
        break;

    case XJ_RELEASE:
        break;

    case XJ_INPUT:
        /* ��ȡ����״̬�� */
        ActualNumber = 0;
        Number = pRequest->nu_request_info.nu_input.nu_request_size;
        Offset = pRequest->nu_request_info.nu_input.nu_offset;
        pDst = pRequest->nu_request_info.nu_input.nu_buffer_ptr;
        if( ( Offset + Number ) <= ( UNSIGNED )CINC_INPUT_GROUP_NUMBER )
        {
            ActualNumber = Number;
            if( pRequest->nu_request_info.nu_input.nu_logical_unit == ( UNSIGNED )CIN_INPUT_TYPE_QUENTY )
            {
                /* ��ȡ��������Ʒ�����أ�������+Ʒ�����أ� */
                pPort = CINC_INPUT_PORT_MEMORY; /* ����˿��׵�ַ */
                pPort += Offset;
                for( i = 0; i < Number; i ++)
                {
                    Value = 0;
                    Quality = 0;
                    switch( pPort->PortBits )
                    {
                    case 8:  /* 8λ�˿����� */
                        for( k = 0; k < pPort->PortNumber; k ++)
                        {
                            /* ��ȡ����˿����� */
                            pPortAddr8 = ( UNSIGNED_CHAR *)( pPort->PortAddress[k] );
                            Value += ((( * pPortAddr8 ) & pPort->PortMasks ) << ( k * pPort->PortBits ));
                        }
                        Value ^= ( pPort->ReverseBits );
                        break;
                    case 16: /* 16λ�˿����� */
                        for( k = 0; k < pPort->PortNumber; k ++)
                        {
                            /* ��ȡ����˿����� */
                            pPortAddr16 = ( unsigned short int *)( pPort->PortAddress[k] );
                            Value += ((( * pPortAddr16 ) & pPort->PortMasks ) << ( k * pPort->PortBits ));
                        }
                        Value ^= ( pPort->ReverseBits );
                        break;
                    case 32: /* 32λ�˿����� */
                        pPortAddr32 = ( UNSIGNED *)( pPort->PortAddress[0] );
                        /* ��ȡ����˿����� */
                        Value += ((* pPortAddr32 ) & pPort->PortMasks );
                        Value ^= ( pPort->ReverseBits );
                        break;
                    default :
                        break;
                    }
                    pQuality = ( UNSIGNED *)( pPort->QualityAddress );
                    /* ��ȡƷ������ */
                    Quality = * pQuality;
                    * pDst ++ = Value;
                    * pDst ++ = Quality;
                }
                pRequest->nu_status = XJ_SUCCESS;
            }
            else if( pRequest->nu_request_info.nu_input.nu_logical_unit == ( UNSIGNED )CIN_INPUT_TYPE_VALUE )
            {
                /* ��ȡ������ֵ */
                pPort = CINC_INPUT_PORT_MEMORY; /* ����˿��׵�ַ */
                pPort += Offset;
                for( i = 0; i < Number; i ++)
                {
                    Value = 0;
                    switch( pPort->PortBits )
                    {
                    case 8:  /* 8λ�˿����� */
                        for( k = 0; k < pPort->PortNumber; k ++)
                        {
                            /* ��ȡ����˿����� */
                            pPortAddr8 = ( UNSIGNED_CHAR *)( pPort->PortAddress[k] );
                            Value += ((( * pPortAddr8 ) & pPort->PortMasks ) << ( k * pPort->PortBits ));
                        }
                        Value ^= ( pPort->ReverseBits );
                        break;
                    case 16: /* 16λ�˿����� */
                        for( k = 0; k < pPort->PortNumber; k ++)
                        {
                            /* ��ȡ����˿����� */
                            pPortAddr16 = ( unsigned short int *)( pPort->PortAddress[k] );
                            Value += ((( * pPortAddr16 ) & pPort->PortMasks ) << ( k * pPort->PortBits ));
                        }
                        Value ^= ( pPort->ReverseBits );
                        break;
                    case 32: /* 32λ�˿����� */
                        pPortAddr32 = ( UNSIGNED *)( pPort->PortAddress[0] );
                        /* ��ȡ����˿����� */
                        Value += ((* pPortAddr32 ) & pPort->PortMasks );
                        Value ^= ( pPort->ReverseBits );
                        break;
                    default :
                        break;
                    }
                    * pDst ++ = Value;
                }
                pRequest->nu_status = XJ_SUCCESS;
            }
            else
            {
                pRequest->nu_status = XJ_INVALID_ENTRY;
            }
        }
        else
        {
            pRequest->nu_status = XJ_INVALID_ENTRY;
        }
        break;

    default:
        pRequest -> nu_status = XJ_INVALID_ENTRY;
        break;
    }
}

/* ��ȡ����״̬��ֵ���ж�ʹ�ã� */
UNSIGNED CINC_ReadInput( INT GroupNumber )
{
    INT k;
    UNSIGNED Variable;
    ESP_CFG_INPUT_PORT * pInputPort;
    UNSIGNED_CHAR * pInputPortAddress8;
    unsigned short int * pInputPortAddress16;
    UNSIGNED * pInputPortAddress32;
    INT iStatus;

    Variable = 0;
    if( GroupNumber < CINC_HARD_INPUT_GROUPS )
    {
        /* ��ȡӲ����״̬ */
        iStatus = DEVI_ReadInput( GroupNumber, 1, & Variable );
    }
    else if( GroupNumber < CINC_INPUT_GROUP_NUMBER )
    {
        pInputPort = CINC_INPUT_PORT_MEMORY; /* ����˿��׵�ַ */
        pInputPort += GroupNumber;
        switch( pInputPort->PortBits )
        {
        case 8:  /* 8λ�˿����� */
            for( k = 0; k < pInputPort->PortNumber; k ++)
            {
                /* ��ȡ����˿����� */
                pInputPortAddress8 = ( UNSIGNED_CHAR *)( pInputPort->PortAddress[k] );
                Variable += ((( * pInputPortAddress8 ) & pInputPort->PortMasks ) << ( k * pInputPort->PortBits ));
            }
            Variable ^= ( pInputPort->ReverseBits );
            break;
        case 16: /* 16λ�˿����� */
            for( k = 0; k < pInputPort->PortNumber; k ++)
            {
                /* ��ȡ����˿����� */
                pInputPortAddress16 = ( unsigned short int *)( pInputPort->PortAddress[k] );
                Variable += ((( * pInputPortAddress16 ) & pInputPort->PortMasks ) << ( k * pInputPort->PortBits ));
            }
            Variable ^= ( pInputPort->ReverseBits );
            break;
        case 32: /* 32λ�˿����� */
            pInputPortAddress32 = ( UNSIGNED *)( pInputPort->PortAddress[0] );
            /* ��ȡ����˿����� */
            Variable += ((* pInputPortAddress32 ) & pInputPort->PortMasks );
            Variable ^= ( pInputPort->ReverseBits );
            break;
        default :
            break;
        }
    }
    else
    {
        /* ��Ч����״̬����ֱ�ӷ���0 */
        Variable = 0;
    }

    return( Variable );
}

/* ��ʼ���������� */
INT CINC_CreateInputDriver( VOID )
{
    CINC_InputDriver = XJ_Create_Driver( "input", CINC_InputDriverEntry );

    return( XJ_SUCCESS );
}

/* ��ȡ����״̬��ֵ */
UNSIGNED CINC_ReadInputValue( INT GroupNumber )
{
    XJ_DRIVER_REQUEST request;
    UNSIGNED Buffer[20]; /* ������ */

    Buffer[0] = 0;
    //   Buffer[0]=CINC_ReadInput(GroupNumber);
    //   return( Buffer[0] );

    if( GroupNumber < CINC_HARD_INPUT_GROUPS )
    {
        /* ��ȡӲ����״̬ */
        DEVI_ReadInput( GroupNumber, 1, Buffer );
    }
    else
    {
        /* ��ȡ������״̬ */
        request.nu_function = XJ_INPUT;
        request.nu_request_info.nu_input.nu_buffer_ptr = ( VOID *)Buffer;
        request.nu_request_info.nu_input.nu_request_size = 1;
        request.nu_request_info.nu_input.nu_offset = GroupNumber;
        request.nu_request_info.nu_input.nu_logical_unit = CIN_INPUT_TYPE_VALUE;

        XJ_Request_Driver( CINC_InputDriver,& request );
    }

    return( Buffer[0] );
}

/* ��������Ʒ������ */
INT CINC_ReadInputQuality( INT GroupNumber, UNSIGNED * pBits, UNSIGNED * pQuality )
{
    INT status;
    XJ_DRIVER_REQUEST request;
    UNSIGNED Buffer[20]; /* ������ */

    Buffer[0] = 0;
    Buffer[1] = QUALITY_VALID_INVALID;

    if( GroupNumber < CINC_HARD_INPUT_GROUPS )
    {
        /* ��ȡӲ����Ʒ������ */
        status = DEVI_ReadInputVQ( GroupNumber, 1, Buffer );
    }
    else
    {
        /* ��ȡ������Ʒ������ */
        request.nu_function = XJ_INPUT;
        request.nu_request_info.nu_input.nu_buffer_ptr = ( VOID *)Buffer;
        request.nu_request_info.nu_input.nu_request_size = 1;
        request.nu_request_info.nu_input.nu_offset = GroupNumber;
        request.nu_request_info.nu_input.nu_logical_unit = CIN_INPUT_TYPE_QUENTY;

        XJ_Request_Driver( CINC_InputDriver,& request );
        status = request.nu_status;
    }

    * pBits = Buffer[0]; /* ����״̬��ֵ */
    * pQuality = Buffer[1]; /* ������Ʒ������ */

    return( status );
}

/* ������״̬��λ��� */
INT CINC_SwitchStatusDetection( VOID )
{
    INT k;
    UNSIGNED uVariable;
    UNSIGNED uSwitchStatus;
    UNSIGNED * pSrc;
    UNSIGNED * pDst;
    UNSIGNED * pEnd;
    static INT IniFlag = XJ_FALSE;
    INT ChangeFalg;
    TIME * pTime, ClockTime;
    UNSIGNED Buffer[10]; /* ������ */

    /* �жϿ������Ƿ������� */
    if( CINC_InputCheckFlag == 0 )
    {
        IniFlag = XJ_FALSE;
        return( XJ_SUCCESS );
    }

    ChangeFalg = XJ_FALSE;
    if( IniFlag == XJ_FALSE )
    {
        CINC_SwitchStatusIn = CINC_SwitchStatusMemory;
        for( k = 0; k < CPC_MAX_INPUT_GROUPS; k ++)
        {
            /* ��ȡ����״̬�� */
            uSwitchStatus = CINC_ReadInput( k );
            uSwitchStatus &= CINC_SwitchStatusMasks[k];
            CINC_SwitchStatusValue[k] = uSwitchStatus;
        }
        IniFlag = XJ_TRUE;
        /* ��������λ(��ʼ����ʹû�б�λҲҪ��¼״̬,������λ��,��֤inָ���ǵ�ǰ��������β��) */
        ChangeFalg = XJ_TRUE;
    }

    /* ��ȡ����״̬�� */
    for( k = 0; k < CPC_MAX_INPUT_GROUPS; k ++)
    {
        /* ��ȡ����״̬�� */
        uSwitchStatus = CINC_ReadInput( k );
        uSwitchStatus &= CINC_SwitchStatusMasks[k];
        if( uSwitchStatus != CINC_SwitchStatusValue[k] )
        {
            /* ��������λ */
            ChangeFalg = XJ_TRUE;
        }
        CINC_SwitchStatusValue[k] = uSwitchStatus;
    }
    if( ChangeFalg == XJ_TRUE )
    {
        /* ��������λ������״̬��ʱ�� */
        pDst = CINC_SwitchStatusIn;
        pSrc = CINC_SwitchStatusValue;
        pEnd = CINC_SwitchStatusMemory + ( CIN_SWITCH_STATUS_NUMBER * CPC_MAX_SWITCH_STATUS_POINT );
        /* ����ʱ�� */
        XJP_TM_Get_Time_61850( & ClockTime, XJ_NULL, XJ_NULL, & uVariable );
        pTime = & ClockTime;
        * pDst ++ = pTime -> Year;
        * pDst ++ = pTime -> Month;
        * pDst ++ = pTime -> Day;
        * pDst ++ = pTime -> Hour;
        * pDst ++ = pTime -> Minute;
        * pDst ++ = pTime -> Millionsecond;
        XJP_TM_Get_Clock_Precision( Buffer );
        * pDst ++ = Buffer[1]; /* ΢�� */
        * pDst ++ = Buffer[0]; /* ϵͳʱ�� */
        * pDst ++ = uVariable; /* ʱ��Ʒ������ */
        /* ����״̬��ֵ */
        for( k = 0; k < CPC_MAX_INPUT_GROUPS; k ++)
        {
            * pDst ++ = * pSrc ++;
        }
        if( pDst >= pEnd )
        {
            CINC_SwitchStatusIn = CINC_SwitchStatusMemory;
        }
        else
        {
            CINC_SwitchStatusIn = pDst;
        }
    }

    return( XJ_SUCCESS );
}

/* ��ȡ��λ���״̬����Ϣ */
VOID * CINC_SwitchStatusInformation( UNSIGNED * Actual_Size, INT * Actual_Group )
{
    * Actual_Size = ( CIN_SWITCH_STATUS_NUMBER * CPC_MAX_SWITCH_STATUS_POINT );
    * Actual_Group = CPC_MAX_INPUT_GROUPS;

    return( CINC_SwitchStatusMemory );
}

/* ��ȡ��λ��¼����ָ�� */
VOID * CINC_SwitchStatusPointer( VOID )
{
    return(( VOID *)CINC_SwitchStatusIn );
}

/* ���ò�ؿ�������λ���Ƶ�� */
VOID CINC_SetupFckInput( INT iTimeFlag )
{
    if( iTimeFlag == 0 )
    {
        /* 1�����ж� */
        CINC_FckInputCheckFreq = 1000000 / 1000; /* 1000΢�� */
    }
    else
    {
        /* 0.5�����ж� */
        CINC_FckInputCheckFreq = 1000000 / 500; /* 500΢�� */
    }
}

/* ��ȡ��ؿ�������λ���Ƶ�� */
UNSIGNED CINC_GetFckInputCheckFreq( VOID )
{
    return( CINC_FckInputCheckFreq );
}

/* ���ñ�λ��Ч���λ���� */
INT CINC_SetSwitchStatusMasksBits( UNSIGNED GroupNumber, UNSIGNED MaskBits )
{
    INT status;
    UNSIGNED uMaskBitsT;

    status = XJ_FALSE;
    if( GroupNumber < ( UNSIGNED )CINC_INPUT_GROUP_NUMBER )
    {
        /* ��Ч������ */
        CINC_SwitchStatusMasks[ GroupNumber ] = MaskBits;

        if( ( INT )GroupNumber >= CINC_HARD_INPUT_GROUPS )
        {
            /* ��ȡ��ʱ���goose����������Ϣ */
            if( PXML_CFG_GetTimeGooseRecvInfo( ( GroupNumber - CINC_HARD_INPUT_GROUPS ), & uMaskBitsT ))
            {
                uMaskBitsT ^= CPC_DATA_MASKS32;
                CINC_SwitchStatusMasks[ GroupNumber ] &= uMaskBitsT;
            }
        }

        status = XJ_TRUE;
    }

    return( status );
}

/* ����������״̬ */
INT CINC_SetSoftInputStatus( INT iGroup, UNSIGNED uBits, INT iStatus )
{
    ESP_CFG_INPUT_PORT * pInputPort;
    UNSIGNED * p;
    UNSIGNED variable;
    INT status;
    /*=============================*/
    status = XJ_UNAVAILABLE;
    if(( iGroup >= CINC_HARD_INPUT_GROUPS )
        && ( iGroup < CINC_INPUT_GROUP_NUMBER ) )
    {
        /* ���ֿ����������¿�����״̬ */
        pInputPort = CINC_INPUT_PORT_MEMORY;
        pInputPort += iGroup;
        p = ( UNSIGNED *)pInputPort->PortAddress[0]; /* �˿ڵ�ַ */
        variable = * p;
        if( iStatus == 2 ) /* Ͷ */
        {
            variable |= uBits;
        }
        else
        {
            variable &= ( ~( uBits ));
        }
        * p = variable;
        status = XJ_SUCCESS;
    }
    return( status );
}

/* ����������Ʒ������ */
INT CINC_SetSoftInputQuality( INT iGroup, UNSIGNED uBits, UNSIGNED Quality )
{
    ESP_CFG_INPUT_PORT * pInputPort;
    UNSIGNED * p;
    UNSIGNED variable;
    INT status;
    /*=============================*/
    status = XJ_UNAVAILABLE;
    if(( iGroup >= CINC_HARD_INPUT_GROUPS )
        && ( iGroup < CINC_INPUT_GROUP_NUMBER ) )
    {
        /* ���ֿ����������¿�����Ʒ������ */
        pInputPort = CINC_INPUT_PORT_MEMORY;
        pInputPort += iGroup;
        p = ( UNSIGNED *)pInputPort->QualityAddress; /* �˿ڵ�ַ */
        variable = * p;
        if( Quality & QUALITY_VALID_INVALID )
        {
            /* ��ЧƷ������ */
            variable &= ( ~ uBits );
        }
        else
        {
            variable |= uBits;
        }
        * p = variable;

        /* ���¼���λ״̬ */
        p = ( UNSIGNED *)pInputPort->QualityCheckAddr; /* �˿ڵ�ַ */
        variable = * p;
        if( Quality & QUALITY_TEST )
        {
            /* ����λƷ������ */
            variable |= uBits;
        }
        else
        {
            variable &= ( ~ uBits );
        }
        * p = variable;

        status = XJ_SUCCESS;
    }
    return( status );
}

/* ���ÿ������Ƿ����־ */
INT  CINC_SetInputCheckFlag( INT iFlag )
{
    if( iFlag == 0 )
    {
        CINC_InputCheckFlag = 0;
    }
    else
    {
        CINC_InputCheckFlag = 1;
    }
    return( XJ_SUCCESS );
}

/* �����������鴫��ʱ���� */
INT CINC_SetSoftInFullMasks( UNSIGNED GroupNumber, UNSIGNED MaskBits )
{
    INT status;
    UNSIGNED uMaskBits;

    status = XJ_FALSE;
    if( GroupNumber < ( UNSIGNED )CPC_MAX_INPUT_GROUPS )
    {
        /* �������鴫������λ */
        CINC_SoftInFullMasks[ GroupNumber ] = MaskBits;

        /* ��ȡ��ʱ���goose����������Ϣ */
        if( PXML_CFG_GetGooseRecvInfo( GroupNumber , & uMaskBits ))
        {
            CINC_SoftInFullMasks[ GroupNumber ] &= uMaskBits;
        }
        status = XJ_TRUE;
    }

    return( status );
}

/* ��ȡ�������鴫��ʱ���� */
INT CINC_GetSoftInFullMasks( UNSIGNED GroupNumber, UNSIGNED * MaskBits )
{
    INT status;

    status = XJ_FALSE;
    if( GroupNumber < ( UNSIGNED )CPC_MAX_INPUT_GROUPS )
    {
        /* �������鴫������λ */
        if( MaskBits )
        {
            * MaskBits = CINC_SoftInFullMasks[ GroupNumber ];
        }
        status = XJ_TRUE;
    }

    return( status );
}
