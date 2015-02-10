
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
/*      ctb_mg.c                                        Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CTB_MG - Common Table Management                                 */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common table        */
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
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       01-26-2007      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
/* Ӧ�����ݱ������ */
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "plat/inc/xj_ctb_mg.h"
#include "plat/inc/tab_defs.h"
#include "plat/es_plat.h"

#define CTB_MG_WORD_BYTES     4  /* WORD�������������е��ֽ��� */
/*====================================================*/
/* �ڲ����� */
/* װ����Ϣ���ֶκ����� */
INT CTB_MG_DevAfter( INT TabIndex, INT FieldIndex, VOID * dataPtr, INT Originate );
/*====================================================*/

/* װ����Ϣ���ֶκ����� */
INT CTB_MG_DevAfter( INT TabIndex, INT FieldIndex, VOID * dataPtr, INT Originate )
{
    INT      iStatus;
    TB_CFB   CondField;
    UNSIGNED uTimeType;
    INT      iTimeZone;
    INT      ActualNumber;
    UNSIGNED_CHAR Temp[CTB_MG_WORD_BYTES];
    /*=============================*/
    /* ���ò����ֶ� */
    CondField.HandleIndex = FieldIndex;
    /* �������� */
    CondField.CondNumber = 0;
    /* �������ֶ�ֵ */
    iStatus = XJP_DS_Get_Field_Value_One( TabIndex, 0, 1,
        & CondField, Temp, & ActualNumber );
    if( iStatus == XJ_SUCCESS )
    {
        if( FieldIndex == DEV_TIME_TYPE_INDEX )
        {
            XJP_CSC_Byte_To_Word(& uTimeType, Temp, CTB_MG_WORD_BYTES );
            /* ���ö�ʱ���� */
            XJP_TM_Set_Time_Type( uTimeType );
        }
        else if( FieldIndex == DEV_TIME_ZONE_INDEX )
        {
            XJP_CSC_Byte_To_Word(& iTimeZone, Temp, CTB_MG_WORD_BYTES );
            /* ����ϵͳʱ�估ʱ�� */
            XJP_TM_Set_Time_Dev_Zone( NU_NULL , iTimeZone );
        }
    }

    return( iStatus );
}

/* ע�����ݱ����� */
VOID CTB_MG_RegisterTreatmentFunc( VOID )
{
    /* ע��װ����Ϣ���ʱ�����ֶκ����� */
    XJP_DS_Register_After_Treatment( TAB_DEVICE_ID, DEV_TIME_TYPE_INDEX, CTB_MG_DevAfter );
    /* ע��װ����Ϣ���ʱ�����ֶκ����� */
    XJP_DS_Register_After_Treatment( TAB_DEVICE_ID, DEV_TIME_ZONE_INDEX, CTB_MG_DevAfter );
}

/* ��ʼ��ʱ����Ϣ */
INT CTB_MG_InitDevTimeInfo( VOID )
{
    INT      iStatus;
    TB_CFB   CondField;
    UNSIGNED uTimeType;
    INT      iTimeZone;
    INT      ActualNumber;
    UNSIGNED_CHAR Temp[CTB_MG_WORD_BYTES];
    /*=============================*/
    /* ���ò����ֶ� */
    CondField.HandleIndex = DEV_TIME_TYPE_INDEX;
    /* �������� */
    CondField.CondNumber = 0;
    /* �������ֶ�ֵ */
    iStatus = XJP_DS_Get_Field_Value_One( TAB_DEVICE_ID, 0, 1,
        & CondField, Temp, & ActualNumber );
    if( iStatus == XJ_SUCCESS )
    {
        XJP_CSC_Byte_To_Word(& uTimeType, Temp, CTB_MG_WORD_BYTES );
        /* ���ö�ʱ���� */
        XJP_TM_Set_Time_Type( uTimeType );
    }

    /* ���ò����ֶ� */
    CondField.HandleIndex = DEV_TIME_ZONE_INDEX;
    /* �������� */
    CondField.CondNumber = 0;
    /* �������ֶ�ֵ */
    iStatus = XJP_DS_Get_Field_Value_One( TAB_DEVICE_ID, 0, 1,
        & CondField, Temp, & ActualNumber );
    if( iStatus == XJ_SUCCESS )
    {
        XJP_CSC_Byte_To_Word(& iTimeZone, Temp, CTB_MG_WORD_BYTES );
        /* ����ϵͳʱ�估ʱ�� */
        XJP_TM_Set_Time_Dev_Zone( NU_NULL , iTimeZone );
    }

    return( iStatus );
}
