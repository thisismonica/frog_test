
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
/*      dpfc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      DPF - Driver Platform Function                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the driver functions to */
/*      platform.                                                        */
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
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       01-07-2008      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "plat/inc/dpf_extr.h"
#include "plat/es_plat.h"

/* Define external inner-component global data references.  */

DP_FUN_LIST * DPF_DriverPlatFunctionList = NU_NULL;

/* Define internal function calls.  */

INT DPFC_VersionCheck( INT StructOffset );

/* Ӧ�ú�����İ汾��� */
INT DPFC_VersionCheck( INT StructOffset )
{
    INT status = NU_FALSE;

    if( DPF_DriverPlatFunctionList )
    {
        if( DPF_DriverPlatFunctionList->size > StructOffset )
        {
            /* ��Ч�汾,ִ�к��� */
            status = NU_TRUE;
        }
    }
    else
    {
        /* ��ȡ���������ṩ��ƽ̨�ĺ������� */
        DPF_DriverPlatFunctionList = XJP_Function_Table_Retrieve( DP_FUN_LIST_NAME );
        if( DPF_DriverPlatFunctionList )
        {
            if( DPF_DriverPlatFunctionList->size > StructOffset )
            {
                /* ��Ч�汾,ִ�к��� */
                status = NU_TRUE;
            }
        }
    }
    return( status );
}

/* ��ȡ�����汾�� */
INT DPFC_GetDriverVersion( UNSIGNED * verL, UNSIGNED  * verH )
{
    INT status;

    /* �汾��� */
    if( DPFC_VersionCheck( DPF_GETSTRUCTOFFSET( DP_FUN_LIST, driver_version )) == NU_FALSE )
    {
        return( NU_UNAVAILABLE );
    }
    /* ִ�к��� */
    status = DPF_DriverPlatFunctionList->driver_version( verL, verH );

    return( status );
}

/* ��ȡ����������ָ�� */
UNSIGNED DPFC_DriverPointer( VOID ** pointer_list )
{
    UNSIGNED Number;

    Number = 0;
    /* �汾��� */
    if( DPFC_VersionCheck( DPF_GETSTRUCTOFFSET( DP_FUN_LIST, driver_pointer )) == NU_FALSE )
    {
        return( Number );
    }
    /* ִ�к��� */
    Number = DPF_DriverPlatFunctionList->driver_pointer( pointer_list );

    return( Number );
}
