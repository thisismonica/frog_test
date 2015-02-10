
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
/*      out_extr.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      OUT - Output Management                                          */
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
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhideYang.      07-19-2007      Created initial version 1.0      */
/*     YanJuanJiang.    07-24-2007      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_out_check.h"
#include "plat/inc/esp_cfg.h"
#include "plat_specific/plat_specific.h"
#include "plat_specific/inc/cpc_defs.h"

/* Check to see if the file has been included already.  */

#ifndef OUT_CHECK_H
#define OUT_CHECK_H

extern ESP_CFG PLAT_cfg;

/* �л�ȡ����������ʶ */
#define OUT_GET_BASE_STARTUP_FLAG()   ( PLAT_cfg.device.FunSelect & FUN_BASE_START_UP )

#define OUT_JC_EVENT_DELAY_TIME     100     /* �����¼���ʱʱ�� */

#define OUT_MAX_CHECK_RELAY    32           /* �������ڼ̵���·�� */
#define OUT_GROUP_RELAY_BITS   32           /* ÿ�����λ�� */
#define OUT_CHECK_BREAK_DELAY  61000        /* ���ڶ��߼����ʱʱ�� */
#define OUT_OUTERROR_DELAY     10000        /* ������·������ʱʱ�� */
#define OUT_OUTERROR_NUMBER    10           /* ������·���ߴ��� */
#define OUT_SUB_GROUP_MASK32   31           /* the mask of sub-group 32-bits */
#define OUT_GROUP_MASK32       5            /* the group mask bit for 32-bits */

#define OUT_STARTUP_RDELAY     500          /* �����̵���������ʱʱ�� */
#define OUT_START_OPEN_CHECK   1000         /* �����̵������غ󿪷ſ����Լ����ʱʱ�� */
#define OUT_SEND_STARTB_RDELAY 3000         /* ��Բ��ñ��������̵�������ʱ����ʱʱ�� */
#define OUT_PICKUP_CHECKDELAY  5            /* ���ڱ�λ���ʱ�� */
#define OUT_RESET_HOLD_TIME    500          /* ���鱣��ʱ�� */
/* �̵������־ */
#define OUT_BASE_RELAY_FLAG   0x1            /* ���������̵����������־ */
#define OUT_EXT_RELAY_FLAG    0x2            /* ��չ�����̵����������־ */

/* �����̵������ݽṹ  */
typedef struct OUT_RELAY_STRUCT
{
    /* ����λ */
    UNSIGNED PickupBits;
    /* ʵ��ִ��λ */
    UNSIGNED ExecuteBits;
    /* ��������λ */
    UNSIGNED HoldBits;
    /* ���ڱ仯��¼λ */
    UNSIGNED ChangeRegBits;
    /* ������Ϣ��Ӳ������ */
    /* ���߼��λ */
    UNSIGNED CheckBits;
    /* ���λ�� */
    INT CheckNum;
    /* ������·���߼����� */
    UNSIGNED BreakErrorCount[OUT_GROUP_RELAY_BITS];
    /* ���߼���� */
    UNSIGNED ErrorBits;
    /* �����¼���¼λ */
    UNSIGNED ErrorEventBits;
}OUT_RELAY;

/* ���������ݽṹ  */
typedef struct SOUT_RELAY_STRUCT
{
    /* ����λ */
    UNSIGNED PickupBits;
    /* ʵ��ִ��λ */
    UNSIGNED ExecuteBits;
    /* ��������λ */
    UNSIGNED HoldBits;
    /* ���ڱ仯��¼λ */
    UNSIGNED ChangeRegBits;
}SOUT_RELAY;

/* �������ƿ����ݽṹ */
typedef struct OUT_OCB_STRUCT OUT_OCB;
/* �������ƿ����ݽṹ  */
struct OUT_OCB_STRUCT
{
    /* ��󿪳����� */
    INT MaxNumber;
    /* Ӳ���������� */
    INT HardGroupNumber;
    /* ��չ������� */
    INT iHardExtendCount;
    /* �������� */
    INT iStartUpGroup;
    /* �ܿ������� */
    INT TotalGroupNum;
    /* ���ڼ�����·�� */
    UNSIGNED CheckRelayMaxNum;
    /* ���ڼ��·�� */
    UNSIGNED CheckRelayNum;
    /* �����ڼ���־ */
    INT LockCheck;
    /* �������ڼ�� */
    INT CloseCheck;
    /* ׼������־ */
    INT CheckReady;
    /* ִ�м���־ */
    INT CheckExecute;
    /* ���ͨ������ */
    INT CheckNumber;
    /* ��ǰ������ */
    INT CheckGroup;
    /* �ڵ�ǰ������м��ͨ���� */
    INT GroupCheckNum;
    /* ������ʱʱ�� */
    UNSIGNED ErrorDelay;
    /* ˲ʱ������ʱʱ�� */
    UNSIGNED InstErrDelay;
    /* ����������������� */
    UNSIGNED ErrCount[CPC_MAX_OUTPUT_GROUPS];
    /* ���������־ */
    INT ErrorFlag;
    /* ˲ʱ���������־ */
    INT InstErrorFlag;
    /* ��չ�˿����� */
    INT ExtEnable;
    /* �������� */
    INT CloseRelay;
    /* ������־ */
    UNSIGNED StartUp;
    /* ��������������־ */
    UNSIGNED HoldStartup;
    /* �����̵���������ʱ��ʱ��־ */
    UNSIGNED StartUpRDFlag;
    /* �����̵����������� */
    UNSIGNED StartupConst;
    /* �̵�������λ */
    OUT_RELAY * pRelayBits;
    /* ��д�����¼����¼��� */
    NU_EVENT_GROUP * pErrEventGroup;
    /* ��д�¼���¼���� */
    INT (* FillEvent )( const VOID * pName, INT iEventType );
    /* �����¼�λ */
    STATUS (* Set_EventsBits )( NU_EVENT_GROUP * group, UNSIGNED uEvents, OPTION operation );
    /* ���ݱ��� */
    VOID * Protect;

    /*public: */
    /* ������Ϣ��ʼ������ */
    VOID (* Initialize )( UNSIGNED uHardGroupNum, UNSIGNED uTotalGroupNum, UNSIGNED * pCheckBits,
        UNSIGNED uCheckNum , NU_EVENT_GROUP * pErrEventGroup, INT(* FillEvent )( const VOID * pName, INT iEventType ),
        STATUS (* Set_EventsBits )( NU_EVENT_GROUP * group, UNSIGNED uEvents, OPTION operation ), VOID * Protect ,
        VOID (* SendGooseFun )( VOID ), INT iHardExtendCount );
    /* �̵���������·���߼�⺯�� */
    VOID (* OutBk_Check )( VOID );
    /* ������·�����¼���¼ */
    VOID (* OutBk_EventFill )( VOID );
    /* ������·�����¼���¼ */
    VOID (* OutJC_EventFill )( VOID );
    /* ���ڼ�������� */
    VOID (* OutCheck_Main )( VOID );
    /* ���ڱ�λ�ڲ��¼���¼ */
    VOID (* BitsChangeFillEvent )( VOID );
    /* Ӳ������⺯�� */
    VOID (* HardOutCheckMain )( VOID );
    /* ע����չ������⺯�� */
    VOID (* EXOutCheckMain )( VOID );
    /* ע�ᷢ��GOOSE���� */
    VOID (* SendGooseFun )( VOID );
};

#endif
