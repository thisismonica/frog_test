
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

/* 以下为OS相关配置 */
#ifdef WIN32
#include <stdio.h>
#include <malloc.h>
/* Windows 宏定义 */
#endif

#ifndef WIN32 /* 非windows缺省定义一些宏以便通过编译 */
/* 全局变量申明 */
/* Nucleus宏定义 */
#endif

/* Define external inner-component global data references.  */

UNSIGNED CINC_SwitchStatusMemory[CIN_SWITCH_STATUS_NUMBER * CPC_MAX_SWITCH_STATUS_POINT]; /* 开入量变位缓冲区 */
UNSIGNED CINC_SwitchStatusMasks[ CPC_MAX_INPUT_GROUPS ]; /* 变位检查的开入量掩码 */
UNSIGNED CINC_SoftInFullMasks[ CPC_MAX_INPUT_GROUPS ];   /* 软开入整组传输时掩码 */
UNSIGNED CINC_SwitchStatusValue[ CPC_MAX_INPUT_GROUPS ]; /* 状态量值 */
UNSIGNED * CINC_SwitchStatusIn = CINC_SwitchStatusMemory;
XJ_DRIVER CINC_InputDriver;
UNSIGNED CINC_FckInputCheckFreq = 1000; /* 测控开入变位检测频率（1/秒）  */
/* 开入量是否检测标志 */
static INT CINC_InputCheckFlag = 0;
/* Define internal function calls.  */
VOID CINC_InputDriverEntry( XJ_DRIVER pDriver, XJ_DRIVER_REQUEST * pRequest );

/* 开入量驱动函数，参数说明：nu_logical_unit：读取开入量类型 */
/* nu_offset：起始偏移量  nu_request_size：请求开入量组数 */
/* nu_actual_size：实际开入量组数  nu_buffer_ptr：数据缓冲区 */
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
        /* 读取开入状态量 */
        ActualNumber = 0;
        Number = pRequest->nu_request_info.nu_input.nu_request_size;
        Offset = pRequest->nu_request_info.nu_input.nu_offset;
        pDst = pRequest->nu_request_info.nu_input.nu_buffer_ptr;
        if( ( Offset + Number ) <= ( UNSIGNED )CINC_INPUT_GROUP_NUMBER )
        {
            ActualNumber = Number;
            if( pRequest->nu_request_info.nu_input.nu_logical_unit == ( UNSIGNED )CIN_INPUT_TYPE_QUENTY )
            {
                /* 读取开入量的品质因素（开入量+品质因素） */
                pPort = CINC_INPUT_PORT_MEMORY; /* 开入端口首地址 */
                pPort += Offset;
                for( i = 0; i < Number; i ++)
                {
                    Value = 0;
                    Quality = 0;
                    switch( pPort->PortBits )
                    {
                    case 8:  /* 8位端口数据 */
                        for( k = 0; k < pPort->PortNumber; k ++)
                        {
                            /* 读取开入端口数据 */
                            pPortAddr8 = ( UNSIGNED_CHAR *)( pPort->PortAddress[k] );
                            Value += ((( * pPortAddr8 ) & pPort->PortMasks ) << ( k * pPort->PortBits ));
                        }
                        Value ^= ( pPort->ReverseBits );
                        break;
                    case 16: /* 16位端口数据 */
                        for( k = 0; k < pPort->PortNumber; k ++)
                        {
                            /* 读取开入端口数据 */
                            pPortAddr16 = ( unsigned short int *)( pPort->PortAddress[k] );
                            Value += ((( * pPortAddr16 ) & pPort->PortMasks ) << ( k * pPort->PortBits ));
                        }
                        Value ^= ( pPort->ReverseBits );
                        break;
                    case 32: /* 32位端口数据 */
                        pPortAddr32 = ( UNSIGNED *)( pPort->PortAddress[0] );
                        /* 读取开入端口数据 */
                        Value += ((* pPortAddr32 ) & pPort->PortMasks );
                        Value ^= ( pPort->ReverseBits );
                        break;
                    default :
                        break;
                    }
                    pQuality = ( UNSIGNED *)( pPort->QualityAddress );
                    /* 读取品质因素 */
                    Quality = * pQuality;
                    * pDst ++ = Value;
                    * pDst ++ = Quality;
                }
                pRequest->nu_status = XJ_SUCCESS;
            }
            else if( pRequest->nu_request_info.nu_input.nu_logical_unit == ( UNSIGNED )CIN_INPUT_TYPE_VALUE )
            {
                /* 读取开入量值 */
                pPort = CINC_INPUT_PORT_MEMORY; /* 开入端口首地址 */
                pPort += Offset;
                for( i = 0; i < Number; i ++)
                {
                    Value = 0;
                    switch( pPort->PortBits )
                    {
                    case 8:  /* 8位端口数据 */
                        for( k = 0; k < pPort->PortNumber; k ++)
                        {
                            /* 读取开入端口数据 */
                            pPortAddr8 = ( UNSIGNED_CHAR *)( pPort->PortAddress[k] );
                            Value += ((( * pPortAddr8 ) & pPort->PortMasks ) << ( k * pPort->PortBits ));
                        }
                        Value ^= ( pPort->ReverseBits );
                        break;
                    case 16: /* 16位端口数据 */
                        for( k = 0; k < pPort->PortNumber; k ++)
                        {
                            /* 读取开入端口数据 */
                            pPortAddr16 = ( unsigned short int *)( pPort->PortAddress[k] );
                            Value += ((( * pPortAddr16 ) & pPort->PortMasks ) << ( k * pPort->PortBits ));
                        }
                        Value ^= ( pPort->ReverseBits );
                        break;
                    case 32: /* 32位端口数据 */
                        pPortAddr32 = ( UNSIGNED *)( pPort->PortAddress[0] );
                        /* 读取开入端口数据 */
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

/* 读取开入状态量值（中断使用） */
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
        /* 读取硬开入状态 */
        iStatus = DEVI_ReadInput( GroupNumber, 1, & Variable );
    }
    else if( GroupNumber < CINC_INPUT_GROUP_NUMBER )
    {
        pInputPort = CINC_INPUT_PORT_MEMORY; /* 开入端口首地址 */
        pInputPort += GroupNumber;
        switch( pInputPort->PortBits )
        {
        case 8:  /* 8位端口数据 */
            for( k = 0; k < pInputPort->PortNumber; k ++)
            {
                /* 读取开入端口数据 */
                pInputPortAddress8 = ( UNSIGNED_CHAR *)( pInputPort->PortAddress[k] );
                Variable += ((( * pInputPortAddress8 ) & pInputPort->PortMasks ) << ( k * pInputPort->PortBits ));
            }
            Variable ^= ( pInputPort->ReverseBits );
            break;
        case 16: /* 16位端口数据 */
            for( k = 0; k < pInputPort->PortNumber; k ++)
            {
                /* 读取开入端口数据 */
                pInputPortAddress16 = ( unsigned short int *)( pInputPort->PortAddress[k] );
                Variable += ((( * pInputPortAddress16 ) & pInputPort->PortMasks ) << ( k * pInputPort->PortBits ));
            }
            Variable ^= ( pInputPort->ReverseBits );
            break;
        case 32: /* 32位端口数据 */
            pInputPortAddress32 = ( UNSIGNED *)( pInputPort->PortAddress[0] );
            /* 读取开入端口数据 */
            Variable += ((* pInputPortAddress32 ) & pInputPort->PortMasks );
            Variable ^= ( pInputPort->ReverseBits );
            break;
        default :
            break;
        }
    }
    else
    {
        /* 无效开入状态量，直接返回0 */
        Variable = 0;
    }

    return( Variable );
}

/* 初始化开入驱动 */
INT CINC_CreateInputDriver( VOID )
{
    CINC_InputDriver = XJ_Create_Driver( "input", CINC_InputDriverEntry );

    return( XJ_SUCCESS );
}

/* 读取开入状态量值 */
UNSIGNED CINC_ReadInputValue( INT GroupNumber )
{
    XJ_DRIVER_REQUEST request;
    UNSIGNED Buffer[20]; /* 缓冲区 */

    Buffer[0] = 0;
    //   Buffer[0]=CINC_ReadInput(GroupNumber);
    //   return( Buffer[0] );

    if( GroupNumber < CINC_HARD_INPUT_GROUPS )
    {
        /* 读取硬开入状态 */
        DEVI_ReadInput( GroupNumber, 1, Buffer );
    }
    else
    {
        /* 读取数字量状态 */
        request.nu_function = XJ_INPUT;
        request.nu_request_info.nu_input.nu_buffer_ptr = ( VOID *)Buffer;
        request.nu_request_info.nu_input.nu_request_size = 1;
        request.nu_request_info.nu_input.nu_offset = GroupNumber;
        request.nu_request_info.nu_input.nu_logical_unit = CIN_INPUT_TYPE_VALUE;

        XJ_Request_Driver( CINC_InputDriver,& request );
    }

    return( Buffer[0] );
}

/* 读开入量品质因素 */
INT CINC_ReadInputQuality( INT GroupNumber, UNSIGNED * pBits, UNSIGNED * pQuality )
{
    INT status;
    XJ_DRIVER_REQUEST request;
    UNSIGNED Buffer[20]; /* 缓冲区 */

    Buffer[0] = 0;
    Buffer[1] = QUALITY_VALID_INVALID;

    if( GroupNumber < CINC_HARD_INPUT_GROUPS )
    {
        /* 读取硬开入品质因数 */
        status = DEVI_ReadInputVQ( GroupNumber, 1, Buffer );
    }
    else
    {
        /* 读取数字量品质因数 */
        request.nu_function = XJ_INPUT;
        request.nu_request_info.nu_input.nu_buffer_ptr = ( VOID *)Buffer;
        request.nu_request_info.nu_input.nu_request_size = 1;
        request.nu_request_info.nu_input.nu_offset = GroupNumber;
        request.nu_request_info.nu_input.nu_logical_unit = CIN_INPUT_TYPE_QUENTY;

        XJ_Request_Driver( CINC_InputDriver,& request );
        status = request.nu_status;
    }

    * pBits = Buffer[0]; /* 开入状态量值 */
    * pQuality = Buffer[1]; /* 开入量品质因素 */

    return( status );
}

/* 开关量状态变位检测 */
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
    UNSIGNED Buffer[10]; /* 缓冲区 */

    /* 判断开入量是否允许检测 */
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
            /* 读取开入状态量 */
            uSwitchStatus = CINC_ReadInput( k );
            uSwitchStatus &= CINC_SwitchStatusMasks[k];
            CINC_SwitchStatusValue[k] = uSwitchStatus;
        }
        IniFlag = XJ_TRUE;
        /* 开关量变位(初始化即使没有变位也要记录状态,及调整位置,保证in指针是当前缓冲区的尾部) */
        ChangeFalg = XJ_TRUE;
    }

    /* 读取开入状态量 */
    for( k = 0; k < CPC_MAX_INPUT_GROUPS; k ++)
    {
        /* 读取开入状态量 */
        uSwitchStatus = CINC_ReadInput( k );
        uSwitchStatus &= CINC_SwitchStatusMasks[k];
        if( uSwitchStatus != CINC_SwitchStatusValue[k] )
        {
            /* 开关量变位 */
            ChangeFalg = XJ_TRUE;
        }
        CINC_SwitchStatusValue[k] = uSwitchStatus;
    }
    if( ChangeFalg == XJ_TRUE )
    {
        /* 开关量变位，保存状态及时间 */
        pDst = CINC_SwitchStatusIn;
        pSrc = CINC_SwitchStatusValue;
        pEnd = CINC_SwitchStatusMemory + ( CIN_SWITCH_STATUS_NUMBER * CPC_MAX_SWITCH_STATUS_POINT );
        /* 保存时间 */
        XJP_TM_Get_Time_61850( & ClockTime, XJ_NULL, XJ_NULL, & uVariable );
        pTime = & ClockTime;
        * pDst ++ = pTime -> Year;
        * pDst ++ = pTime -> Month;
        * pDst ++ = pTime -> Day;
        * pDst ++ = pTime -> Hour;
        * pDst ++ = pTime -> Minute;
        * pDst ++ = pTime -> Millionsecond;
        XJP_TM_Get_Clock_Precision( Buffer );
        * pDst ++ = Buffer[1]; /* 微秒 */
        * pDst ++ = Buffer[0]; /* 系统时钟 */
        * pDst ++ = uVariable; /* 时间品质因素 */
        /* 保存状态量值 */
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

/* 获取变位检查状态量信息 */
VOID * CINC_SwitchStatusInformation( UNSIGNED * Actual_Size, INT * Actual_Group )
{
    * Actual_Size = ( CIN_SWITCH_STATUS_NUMBER * CPC_MAX_SWITCH_STATUS_POINT );
    * Actual_Group = CPC_MAX_INPUT_GROUPS;

    return( CINC_SwitchStatusMemory );
}

/* 获取变位记录数据指针 */
VOID * CINC_SwitchStatusPointer( VOID )
{
    return(( VOID *)CINC_SwitchStatusIn );
}

/* 设置测控开入量变位检测频率 */
VOID CINC_SetupFckInput( INT iTimeFlag )
{
    if( iTimeFlag == 0 )
    {
        /* 1毫秒中断 */
        CINC_FckInputCheckFreq = 1000000 / 1000; /* 1000微妙 */
    }
    else
    {
        /* 0.5毫秒中断 */
        CINC_FckInputCheckFreq = 1000000 / 500; /* 500微妙 */
    }
}

/* 获取测控开入量变位检测频率 */
UNSIGNED CINC_GetFckInputCheckFreq( VOID )
{
    return( CINC_FckInputCheckFreq );
}

/* 设置变位有效检测位掩码 */
INT CINC_SetSwitchStatusMasksBits( UNSIGNED GroupNumber, UNSIGNED MaskBits )
{
    INT status;
    UNSIGNED uMaskBitsT;

    status = XJ_FALSE;
    if( GroupNumber < ( UNSIGNED )CINC_INPUT_GROUP_NUMBER )
    {
        /* 有效开关量 */
        CINC_SwitchStatusMasks[ GroupNumber ] = MaskBits;

        if( ( INT )GroupNumber >= CINC_HARD_INPUT_GROUPS )
        {
            /* 获取带时标的goose接收数据信息 */
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

/* 设置软开入量状态 */
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
        /* 数字开入量，更新开入量状态 */
        pInputPort = CINC_INPUT_PORT_MEMORY;
        pInputPort += iGroup;
        p = ( UNSIGNED *)pInputPort->PortAddress[0]; /* 端口地址 */
        variable = * p;
        if( iStatus == 2 ) /* 投 */
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

/* 设置软开入量品质因素 */
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
        /* 数字开入量，更新开入量品质因素 */
        pInputPort = CINC_INPUT_PORT_MEMORY;
        pInputPort += iGroup;
        p = ( UNSIGNED *)pInputPort->QualityAddress; /* 端口地址 */
        variable = * p;
        if( Quality & QUALITY_VALID_INVALID )
        {
            /* 无效品质因素 */
            variable &= ( ~ uBits );
        }
        else
        {
            variable |= uBits;
        }
        * p = variable;

        /* 更新检修位状态 */
        p = ( UNSIGNED *)pInputPort->QualityCheckAddr; /* 端口地址 */
        variable = * p;
        if( Quality & QUALITY_TEST )
        {
            /* 检修位品质因素 */
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

/* 设置开入量是否检测标志 */
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

/* 设置软开入整组传输时掩码 */
INT CINC_SetSoftInFullMasks( UNSIGNED GroupNumber, UNSIGNED MaskBits )
{
    INT status;
    UNSIGNED uMaskBits;

    status = XJ_FALSE;
    if( GroupNumber < ( UNSIGNED )CPC_MAX_INPUT_GROUPS )
    {
        /* 软开入整组传输掩码位 */
        CINC_SoftInFullMasks[ GroupNumber ] = MaskBits;

        /* 获取带时标的goose接收数据信息 */
        if( PXML_CFG_GetGooseRecvInfo( GroupNumber , & uMaskBits ))
        {
            CINC_SoftInFullMasks[ GroupNumber ] &= uMaskBits;
        }
        status = XJ_TRUE;
    }

    return( status );
}

/* 获取软开入整组传输时掩码 */
INT CINC_GetSoftInFullMasks( UNSIGNED GroupNumber, UNSIGNED * MaskBits )
{
    INT status;

    status = XJ_FALSE;
    if( GroupNumber < ( UNSIGNED )CPC_MAX_INPUT_GROUPS )
    {
        /* 软开入整组传输掩码位 */
        if( MaskBits )
        {
            * MaskBits = CINC_SoftInFullMasks[ GroupNumber ];
        }
        status = XJ_TRUE;
    }

    return( status );
}
