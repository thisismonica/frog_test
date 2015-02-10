
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
/*      ls_defs.h                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      rc - report copy                                                 */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains data structure definitions and constants for  */
/*     report copy                                                       */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ChenYuFeng, XJ ELECTRIC CO.LTD.                                  */
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
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ChenYuFeng       05-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"
#include "plat/inc/pro_defs.h"
#include "plat/net_inc/commu.h"
#include "plat/net_inc/lc_defs.h"
#include "plat/es_plat.h"

#ifndef RCC_DEFS_H
#define RCC_DEFS_H

#define MAX_CHECK_INFO          50   /* �洢�����ͻ�䱨���� */
#define MAX_FAULT_RETURN        50   /* �洢�������Ϸ��ر����� */
#define GW_TARG_OP_TIMEOUT      100  /* ��ʱʱ�䶨�� */
/* ���Ϸ��ض����������Ͷ��� */
#define RCC_Q_TYPE_FAULT_RETURN 1    /* ���Ϸ��ر��� */
#define RCC_Q_TYPE_COMMAND      2    /* ��������� */

typedef struct  RCC_CHECKINFO_STRUCT
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
}RCC_CheckInfo;

/* ���Ϸ������� */
typedef struct  RCC_FAULT_RETURN_STRUCT
{
    INT             TableID;        /* ���ݱ��(2�ֽ�) */
    INT             CondValue;      /* ������ֵ(4�ֽ�) */
    UNSIGNED        FaultNumber;    /* �������(2�ֽ�) */
    UNSIGNED        RelativeTime;   /* ���ʱ��(2�ֽ�) */
    UNSIGNED        Sect;           /* ������(1�ֽ�) */
    UNSIGNED        FunType;        /* ��������(1�ֽ�) */
    UNSIGNED        INF;            /* ��Ϣ���(1�ֽ�) */
    UNSIGNED        DPI;            /* DPI(1�ֽ�) */
    UNSIGNED        Year;           /* ��(1�ֽ�) */
    UNSIGNED        Month;          /* ��(1�ֽ�) */
    UNSIGNED        Day;            /* ��(1�ֽ�) */
    UNSIGNED        Hour;           /* ʱ(1�ֽ�) */
    UNSIGNED        Minute;         /* ��(1�ֽ�) */
    UNSIGNED        Millionsecond;  /* ����(2�ֽ�) */
    UNSIGNED        q;              /* ����Ʒ������ */
    UNSIGNED        t_sec;          /* t.sec */
    UNSIGNED        t_frac;         /* t.frac */
    UNSIGNED        t_qflag;        /* t.qflag */
    UNSIGNED        phase;          /* ������� */
}RCC_FAULT_RETURN;

/* �������ݽṹ */
typedef struct RCC_Q_MESSAGE_STRUCT
{
    UNSIGNED        Size;           /* ���ݴ�С */
    UNSIGNED        Type;           /* �������� */
    /* ������Ϣ */
    union DATA_INFO_UNION
    {
        RCC_FAULT_RETURN fault;
        UNSIGNED_CHAR    buf[256];
    }data;
}RCC_Q_MESSAGE;

typedef enum ENUM_XJ_ACK_STEP
{
    LC_STEP_ACK,
    LC_STEP_READ,
    LC_STEP_END
}XJ_ACK_STEP;

typedef struct  STRUCT_GW_RC_CB
{
    GW_REQUEST_CB    head;
    XJ_ACK_STEP      step;
    UNSIGNED         rc_context[CPC_MAX_ONE_FAULT_REPORT_NUMBER];
    UNSIGNED         rc_serial;
    UNSIGNED_CHAR    fun;
    INT              rc_count;
    UNSIGNED         rc_faultNumber;
    UNSIGNED         rc_index;
}GW_RC_CB;

#define ONE_COMM_ESIZE()     ((sizeof(RCC_CheckInfo)+sizeof(UNSIGNED)-1)/sizeof(UNSIGNED))    /* ÿ��ͻ���¼�ͨѶ���Ĵ�С */
#define ONE_COMM_RSIZE()     ((sizeof(RCC_FAULT_RETURN)+sizeof(UNSIGNED)-1)/sizeof(UNSIGNED)) /* ÿ�����ض�������ͨѶ���Ĵ�С */

/* ��дͻ���¼�(�ⲿ�ṩʱ��)���� */
INT RCC_Fill_CheckInfo( INT iTableID, INT CondValue, INT iType,
    INT iSecInf, INT iFunInf, INT iStatus, UNSIGNED q );
/* ��дͻ���¼�(�Դ�ʱ��)���� */
INT RCC_Fill_CheckInfo_Full( INT iTableID, INT CondValue, INT iType,
    INT iSecInf, INT iFunInf, INT iStatus, TIME * pTime, UNSIGNED q,
    UNSIGNED tSec, UNSIGNED tFrac, UNSIGNED tQuality );
/* ��д���Ϸ������� */
INT RCC_Fault_Return( INT iTableID, INT CondValue, UNSIGNED uFaultNumber,
    UNSIGNED uRelTime, UNSIGNED uSect, UNSIGNED uFunType, UNSIGNED uINF,
    UNSIGNED uDPI, TIME * pTime, UNSIGNED q, UNSIGNED tSec, UNSIGNED tFrac,
    UNSIGNED tQuality, UNSIGNED phase );
/* ��д��������� */
INT RCC_Fault_Message( VOID * buf );
/* ��дasdu2����(����д���ر���,���������ɽӿ�����) */
INT RCC_Pack_ASDU2( UNSIGNED_CHAR * dst );
/* ��ӿ�����ͻ�䱨�ĺ��� */
VOID RCC_Pack_BreakEvent( VOID * Dst );
/* ���Ϸ������� */
INT RCC_Pack_FaultRet( VOID * Dst );
/* ���ϱ����� */
INT RCC_Check_Report( GW_CHANNEL * pChl, UNSIGNED_CHAR * dst, UNSIGNED_CHAR * dst_port );
/* ���goose���ݺ��� */
INT RCC_Fill_Goose( INT iTableID, INT CondValue, INT iType,
    INT iSecInf, INT iFunInf, INT iStatus, TIME * pTime, UNSIGNED q,
    UNSIGNED tSec, UNSIGNED tFrac, UNSIGNED tQuality, UNSIGNED offset );
/* ����goose���ݺ��� */
INT RCC_Send_Goose( VOID );
/* ���ϱ��渴�� */
INT lcApp_ProcessReportCopy( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port );

#endif
