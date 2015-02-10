
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
/*      mpf_defs.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      MP - HMI to PlatForm                                             */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains system constants common to both the           */
/*      application and the actual Nucleus PLUS components.  This file   */
/*      also contains data structure definitions that hide internal      */
/*      information from the application.                                */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* DATA STRUCTURES                                                       */
/*                                                                       */
/*                                                                       */
/* FUNCTIONS                                                             */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       01-04-2008      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

/* Check to see if this file has been included already.  */

#ifndef MPF_DEFS_H
#define MPF_DEFS_H

#define MP_FUN_LIST_NAME "HmiPlatFunList" /* ��������������(ƽ̨ʹ��) */

typedef struct  MP_CHECKINFO_STRUCT
{
    INT         TableID;         /* ���ݱ��(2�ֽ�) */
    INT         CondValue;       /* ������ֵ(4�ֽ�) */
    INT         Type;            /* �������� */
    UNSIGNED    Year;            /* �� */
    UNSIGNED    Month;           /* �� */
    UNSIGNED    Day;             /* �� */
    UNSIGNED    Hour;            /* ʱ */
    UNSIGNED    Minute;          /* �� */
    UNSIGNED    Millionsecond;   /* ����(2�ֽ�) */
    INT         Sector;          /* ���� */
    INT         FunInf;          /* ��Ϣ���(2�ֽ�) */
    INT         Status;          /* ״̬ */
    UNSIGNED    q;               /* ����Ʒ������ */
    UNSIGNED    t_sec;           /* t.sec */
    UNSIGNED    t_frac;          /* t.frac */
    UNSIGNED    t_qflag;         /* t.qflag */
    UNSIGNED    offset;          /* λ��ƫ�� */
}MP_CHECKINFO;

/* Core processing functions.  */
/*=======================================================================*/
typedef UNSIGNED (* _cal_hmitext_CRC )
    ( UNSIGNED InitialValue );

typedef INT (* _get_hmi_ver )
    ( UNSIGNED * verL, UNSIGNED  * verH, UNSIGNED * toolCRC );

typedef INT (* _fill_goose )( VOID * pInfo );

typedef INT (* _send_goose )( VOID );

/*=======================================================================*/
typedef struct MP_FUN_LIST_STRUCT
{
    int size;
    _get_hmi_ver                        get_hmi_ver;
    _fill_goose                         fill_goose;
    _send_goose                         send_goose;
}MP_FUN_LIST;

#endif
