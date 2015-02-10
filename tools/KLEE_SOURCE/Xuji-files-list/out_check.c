
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
/*      out_check.c                                     Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      OUT_CHECK - Output Check Management                              */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the Output Check        */
/*      Management component.                                            */
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
/*      ZhiDeYang       07-19-2007      Created initial version 1.0      */
/*      YanJuanJiang    07-23-2007      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "plat/inc/out_check.h"
#include "plat/inc/res_defs.h"
#include "plat/inc/devi_extr.h"
#include "plat/inc/xj_clog.h"
#include "plat/inc/cout_extr.h"

#define OUTC_OUT_LOG_NAME           "���ڱ�λ��Ϣ"
#define OUTC_LOG_MAX_LEN            256                         /* ��־������󳤶� */

extern UNSIGNED CSMV_MSG_DevQStatus;       /* װ��״̬ */
/* �����̵�������λ(Ӳ��������������) */
static OUT_RELAY OUT_RelayBits[CPC_MAX_OUTPUT_GROUPS];

/* ���ݱ��� */
#define OUTC_CheckProtect()                  \
  {   \
    if(OUT_Channel.Protect != NU_NULL)   \
        NU_Protect(OUT_Channel.Protect); \
  }

/* ���ݱ��� */
#define OUTC_CheckUnProtect()               \
  {   \
    if(OUT_Channel.Protect != NU_NULL) \
        NU_Unprotect();                \
  }

/* ����ΪOS������� */
#ifdef WIN32
/* ȫ�ֱ������� */
extern volatile UNSIGNED TMD_System_Clock;
/* Windows �궨�� */
/* �궨�� */
#define OUT_Time_Get_Clock()        TMD_System_Clock       /* ��ȡϵͳʱ�� */
#define OUT_CHECK_RELAY_START(x,y,z)                       /* �����̵���ִ�к��� */
#endif

#ifndef WIN32 /* ��windowsȱʡ����һЩ���Ա�ͨ������ */
/* ȫ�ֱ������� */
/* Nucleus�궨�� */
extern volatile UNSIGNED TMD_System_Clock;

/* �궨�� */
#define OUT_Time_Get_Clock()        TMD_System_Clock     /* ��ȡϵͳʱ�� */
#define OUT_CHECK_RELAY_START(x,y,z) DEVI_WriteStartRelay(x,y,z) /* �����̵���ִ�к��� */
#endif
/*================================================================*/
/* �ڲ����� */
/* �̵���������·���߼�⺯�� */
static VOID OUTC_CheckBreak( VOID );
/* ������·�����¼���¼ */
static VOID OUTC_CheckBreakEventFill( VOID );
/* ������·�����¼���¼ */
static VOID OUTC_CheckJCEventFill( VOID );
/* ���ڱ�λ�ڲ��¼���¼ */
static VOID OUTC_CheckBitsChangeFillEvent( VOID );
/* ��ȡ�����Լ�ض�״̬ */
static INT OUT_OutputCheckPortStatus( INT iGroup, UNSIGNED * uStatus );
/* ��չ����Լ캯��(NU_TRUE:�ɹ�/NU_FALSE:ʧ��) */
static INT OUTC_ExportPort_Check( VOID );
/* ��չ��·��⺯�� */
static VOID OUTC_OutEX_Check( VOID );
/* Ӳ������⺯�� */
static VOID OUTC_HardOutCheckMain( VOID );
/* ע��Ӳ������⺯�� */
static VOID OUTC_RegHardOutCheck( VOID (* HardOutCheck )( VOID ));
/* ע����չ������⺯�� */
static VOID OUTC_RegEXOutCheck( VOID (* EXOutCheck )( VOID ));
/*================================================================*/

/* ������λ��ֵ������ִ��λ */
#define OUT_PICKUP_TO_EXECUTE  \
{   \
    INT  i;\
    for(i=0;i<CPC_MAX_OUTPUT_GROUPS;i++)\
    {   \
        OUT_RelayBits[i].ExecuteBits = OUT_RelayBits[i].PickupBits;\
    }\
}

/* ͨ���Լ�λ��ȡ��Ҫ�Լ�Ŀ����ĸ���(ʹ�ô˷������ʱ��������·) */
#define OUT_CHECK_BITS_TO_COUNT(x,y) \
{   \
    INT   iNumber = 0;               \
    while(y>0)                       \
    {   \
        ++iNumber;                   \
        y >>= 4;                     \
    }                                \
    x = iNumber*4;                   \
}

/* ͨ���жϼ��ͨ����(x),���ü�����(y),���õ�ǰ���еļ��ͨ����(z) */
#define OUT_GET_CHECK_GROUP(x,y,z)           \
{   \
    INT  i;                                  \
    INT  iTemp;                              \
    iTemp = 0;                               \
    for(i=0;i<CPC_MAX_OUTPUT_GROUPS;i++)     \
    {   \
        iTemp += OUT_RelayBits[i].CheckNum;  \
        if(x < iTemp)                        \
        {   \
            y = i;                           \
            break;                           \
        }                                    \
        z = x-iTemp;                         \
    }                                        \
}

/* Define internal function calls.  */

OUT_OCB OUT_Channel = 
{
    CPC_MAX_OUTPUT_GROUPS,              /* MaxNumber */
    0,                                  /* HardGroupNumber */
    0,                                  /* iHardExtendCount */
    0,                                  /* iStartUpGroup */
    0,                                  /* TotalGroupNum */
    OUT_MAX_CHECK_RELAY,                /* CheckRelayMaxNum */
    0,                                  /* CheckRelayNum */
    0,                                  /* LockCheck */
    0,                                  /* CloseCheck */
    0,                                  /* CheckReady */
    0,                                  /* CheckExecute */
    0,                                  /* CheckNumber */
    0,                                  /* CheckGroup */
    0,                                  /* GroupCheckNum */
    4,                                  /* ErrorDelay(ms) */
    40,                                 /* InstErrDelay(ms) */
    {   0},                             /* ErrCount */
    0,                                  /* ErrorFlag */
    0,                                  /* InstErrorFlag */
    0,                                  /* ExtEnable */
    0,                                  /* CloseRelay */
    0,                                  /* StartUp */
    0,                                  /* HoldStartup */
    0,                                  /* StartUpRDFlag */
    XJP_START_RELAY_FLAG,               /* StartupConst */
    OUT_RelayBits,                      /* pRelayBits */
    NU_NULL,                            /* pErrEventGroup */
    NU_NULL,                            /* FillEvent */
    NU_NULL,                            /* Set_EventsBits */
    NU_NULL,                            /* Protect */

    & OUTC_CheckInitialize,             /* Initialize */
    & OUTC_CheckBreak,                  /* OutBk_Check */
    & OUTC_CheckBreakEventFill,         /* OutBk_EventFill */
    & OUTC_CheckJCEventFill,            /* OutJC_EventFill */
    & OUTC_CheckMain,                   /* OutCheck_Main */
    & OUTC_CheckBitsChangeFillEvent,    /* BitsChangeFillEvent */
    NU_NULL,                            /* HardOutCheckMain */
    NU_NULL,                            /* EXOutCheckMain */
    NU_NULL,                            /* SendGooseFun */
};

/* ���ó��ڼ̵�������λ */
VOID OUTC_Check_SetPickupBits( INT GroupNo, UNSIGNED uBits )
{
    OUT_OCB         * this;
    OUT_RELAY       * pOUT_Relay;
    INT old_level;
    /*--------------------------------------------------------------------------*/
    this = & OUT_Channel;
    /* ��ȡ�����̵�������ָ�� */
    pOUT_Relay = this->pRelayBits;

    if( ( INT )GroupNo < ( this->TotalGroupNum ) )
    {
        ( pOUT_Relay + GroupNo ) -> PickupBits = uBits;
    }

    /* ���ж��޸ĳ���ִ��λ */
    /* lock interrupt */
    old_level = NU_Local_Control_Interrupts( 0 );
    /* �������ڼ���־ */
    this->LockCheck = 1;
    /* ��ִ�м���־ */
    this->CheckExecute = 0;
    /* ������λ��ֵ������ִ��λ */
    OUT_PICKUP_TO_EXECUTE;
    /* release interrupt */
    NU_Local_Control_Interrupts( old_level );
}

/* �����̵����������� */
VOID OUTC_Check_StartDriver( UNSIGNED uFlag )
{
    OUT_OCB         * this;
    /*--------------------------------------------------------------------------*/
    this = & OUT_Channel;

    /* ��������ִ��,��������ʱʱ�� */
    if( uFlag == XJP_START_RELAY_FLAG )
    {
        this->StartUpRDFlag = 0;
        this->StartUp = XJP_START_RELAY_FLAG;
    }
    else
    {
        this->StartUpRDFlag = 1;
    }
}

/* ���ڼ���ʼ������ */
VOID OUTC_CheckInitialize( UNSIGNED uHardGroupNum, UNSIGNED uTotalGroupNum, UNSIGNED * pCheckBits,
    UNSIGNED uCheckNum , NU_EVENT_GROUP * pErrEventGroup, INT(* FillEvent )( const VOID * pName, INT iEventType ),
    STATUS (* Set_EventsBits )( NU_EVENT_GROUP * group, UNSIGNED uEvents, OPTION operation ), VOID * Protect ,
    VOID (* SendGooseFun )( VOID ), INT iHardExtendCount )
{
    INT             i;
    OUT_RELAY       * pOUT_Relay;
    OUT_OCB         * this;
    UNSIGNED        uCheckBits;
    INT             iCheckCount;
    /*--------------------------------------------------------------------------*/
    /* �����ݻ����� */
    memset( OUT_RelayBits, 0,( sizeof( OUT_RELAY )* CPC_MAX_OUTPUT_GROUPS ));

    this = & OUT_Channel;
    /* ��ȡ�����̵�������ָ�� */
    pOUT_Relay = this->pRelayBits;
    /* Ӳ�������� */
    if( uHardGroupNum > CPC_MAX_OUTPUT_GROUPS )
    {
        this->HardGroupNumber = CPC_MAX_OUTPUT_GROUPS;
    }
    else
    {
        this->HardGroupNumber = ( INT )uHardGroupNum;
    }

    /* �ж��Ƿ���Ӳ���� */
    if( this->HardGroupNumber > 0 )
    {
        /* ע��Ӳ������⺯�� */
        OUTC_RegHardOutCheck(& OUTC_HardOutCheckMain );
    }

    /* �ܿ������� */
    if( uTotalGroupNum > CPC_MAX_OUTPUT_GROUPS )
    {
        this->TotalGroupNum = CPC_MAX_OUTPUT_GROUPS;
    }
    else
    {
        this->TotalGroupNum = ( INT )uTotalGroupNum;
    }

    /* �ж��Ƿ�������,�����Ƿ�ע�ᷢ��GOOSE���� */
    if( this->TotalGroupNum > this->HardGroupNumber )
    {
        this->SendGooseFun = SendGooseFun;
    }
    else
    {
        this->SendGooseFun = NU_NULL;
    }

    /* ��¼��չ������� */
    this->iHardExtendCount = iHardExtendCount;
    if( this->iHardExtendCount > CPC_MAX_OUTPUT_GROUPS )
    {
        this->iHardExtendCount = CPC_MAX_OUTPUT_GROUPS;
    }
    /* �ж��Ƿ�����չ��� */
    if( this->iHardExtendCount )
    {
        this->ExtEnable = 1;
        /* ע����չ������⺯�� */
        OUTC_RegEXOutCheck(& OUTC_OutEX_Check );
    }
    /* ���ݱ��� */
    this->Protect = Protect;
    /* �������λ */
    for( i = 0; i < ( this->HardGroupNumber ); i ++)
    {
        ( pOUT_Relay + i )->CheckBits = *( pCheckBits + i );
        /* ���λ */
        uCheckBits = *( pCheckBits + i );
        /* ͨ�����λ��ȡ�����·�� */
        OUT_CHECK_BITS_TO_COUNT( iCheckCount, uCheckBits );
        /* ���·����ʼ�� */
        ( pOUT_Relay + i )->CheckNum = iCheckCount;
    }
    /* �������·�� */
    if( uCheckNum > OUT_MAX_CHECK_RELAY )
    {
        this->CheckRelayNum = OUT_MAX_CHECK_RELAY;
    }
    else
    {
        this->CheckRelayNum = uCheckNum;
    }

    /* �����¼��� */
    this->pErrEventGroup = pErrEventGroup;
    /* ��д�¼���¼���� */
    this->FillEvent = FillEvent;
    /* �����¼�λ���� */
    this->Set_EventsBits = Set_EventsBits;

    /* ��ȡ�������� */
    this->iStartUpGroup = this->iHardExtendCount;
    /* �ж��Ƿ��л���������· */
    if( OUT_GET_BASE_STARTUP_FLAG())
    {
        /* ���ӻ�������һ· */
        this->iStartUpGroup += 1;
    }
}

/* �̵���������·���߼�⺯�� */
static VOID OUTC_CheckBreak( VOID )
{
    INT             i, j;
    UNSIGNED        uSubBits;
    OUT_RELAY       * pOUT_Relay;
    OUT_RELAY       * pTOUT_Relay;
    OUT_OCB         * this;
    static INT      iInitFlag = 0;
    static INT      iRelayCheck = 0;
    static UNSIGNED uErrorDelay = 0;
    static UNSIGNED uCheckDelay = 0;
    /*--------------------------------------------------------------------------*/
    this = & OUT_Channel;
    /* ��ȡ�����̵�������ָ�� */
    pOUT_Relay = this->pRelayBits;

    if( iInitFlag == 0 )
    {
        /* ��ʼ�ϵ� */
        iInitFlag = 1;
        /* ������ʱʱ�� */
        uCheckDelay = OUT_Time_Get_Clock();
        uErrorDelay = OUT_Time_Get_Clock();
    }

    if( iRelayCheck != 0 )
    {
        uCheckDelay = OUT_Time_Get_Clock();
        if( this->CheckReady != 0 )
        {
            uErrorDelay = OUT_Time_Get_Clock();
        }
        else
        {
            /* ������·���߼����� */
            if(( OUT_Time_Get_Clock() - uErrorDelay ) >= OUT_OUTERROR_DELAY )
            {
                iRelayCheck = 0;

                for( i = 0; i < ( this->HardGroupNumber ); i ++)
                {
                    pTOUT_Relay = ( pOUT_Relay + i );
                    /* �ж��Ƿ��д���λ */
                    if( pTOUT_Relay->ErrorBits > 0 )
                    {
                        for( j = 0; j < ( pTOUT_Relay->CheckNum ); j ++)
                        {
                            uSubBits = ( UNSIGNED )( 1 << ( j & OUT_SUB_GROUP_MASK32 ));
                            if(( pTOUT_Relay->ErrorBits & uSubBits ) != 0 )
                            {
                                if(( ( pTOUT_Relay->ErrorEventBits ) & uSubBits ) == 0 )
                                {
                                    /* ������������� */
                                    pTOUT_Relay->BreakErrorCount[j] += 1;
                                    if( pTOUT_Relay->BreakErrorCount[j] >= OUT_OUTERROR_NUMBER )
                                    {
                                        pTOUT_Relay->ErrorEventBits |= uSubBits;
                                    }
                                }
                            }
                            else
                            {
                                /* ���������� */
                                pTOUT_Relay->BreakErrorCount[j] = 0;
                                pTOUT_Relay->ErrorEventBits &= (~ uSubBits );
                            }
                        }
                    }
                    else
                    {
                        /* ���û�д���λ,�����������ȫ������ */
                        memset( pTOUT_Relay->BreakErrorCount, 0,( sizeof( UNSIGNED )* OUT_GROUP_RELAY_BITS ));
                        /* ���û�д���λ,�����¼�λȫ������ */
                        pTOUT_Relay->ErrorEventBits = 0;
                    }
                }
            }
            else
            {
                /* �Լ�����������,���Լ��� */
                if( this->CloseCheck == 1 )
                {
                    iRelayCheck = 0;
                    uErrorDelay = OUT_Time_Get_Clock();
                }
            }
        }
    }
    else
    {
        uErrorDelay = OUT_Time_Get_Clock();
        if(( OUT_Time_Get_Clock() - uCheckDelay ) >= OUT_CHECK_BREAK_DELAY )
        {
            iRelayCheck = 1;
            this->CheckReady = 1;
            uCheckDelay = OUT_Time_Get_Clock();
        }
    }
}

/* ������·�����¼���¼ */
static VOID OUTC_CheckBreakEventFill( VOID )
{
    INT             i, j;
    UNSIGNED        uSubBits;
    CHAR            EventName[OUTC_CHECK_EVENT_NAME_LENGTH];
    INT             iStatus = NU_UNAVAILABLE;
    OUT_OCB         * this;
    OUT_RELAY       * pOUT_Relay;
    OUT_RELAY       * pTOUT_Relay;
    static UNSIGNED uErrorEventBits[CPC_MAX_OUTPUT_GROUPS];
    static INT  iInitFlag = 0;
    /*--------------------------------------------------------------------------*/
    if( iInitFlag == 0 )
    {
        iInitFlag = 1;
        /* ��ʼ�������� */
        memset( uErrorEventBits, 0,( sizeof( UNSIGNED )* CPC_MAX_OUTPUT_GROUPS ));
    }

    this = & OUT_Channel;
    /* ��ȡ�����̵�������ָ�� */
    pOUT_Relay = this->pRelayBits;

    /* ������·�����¼���¼ */
    for( i = 0; i < ( this->HardGroupNumber ); i ++)
    {
        pTOUT_Relay = ( pOUT_Relay + i );
        for( j = 0; j < ( pTOUT_Relay->CheckNum ); j ++)
        {
            uSubBits = ( UNSIGNED )( 1 << ( j & OUT_SUB_GROUP_MASK32 ));
            if(( ( pTOUT_Relay->ErrorEventBits ) & uSubBits ) != ( uErrorEventBits[i] & uSubBits ))
            {
                if((( pTOUT_Relay->ErrorEventBits ) & uSubBits ) == 0 )
                {
                    if( this->FillEvent != NU_NULL )
                    {
                        /* fill fault event. */
                        sprintf( EventName, RES_DOOUT_DX_RECOVERY,( i * OUT_GROUP_RELAY_BITS + j + 1 ));
                        iStatus = this->FillEvent( EventName, OUTC_CHECK_ALARM_EVENT_ID );
                        if( iStatus == NU_SUCCESS )
                        {
                            uErrorEventBits[i] &= (~ uSubBits );
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                else
                {
                    if( this->FillEvent != NU_NULL )
                    {
                        /* fill fault event. */
                        sprintf( EventName, RES_DOOUT_DX_ERROR,( i * OUT_GROUP_RELAY_BITS + j + 1 ));
                        iStatus = this->FillEvent( EventName, OUTC_CHECK_ALARM_EVENT_ID );
                        if( iStatus == NU_SUCCESS )
                        {
                            uErrorEventBits[i] |= uSubBits;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
        }
    }
}

/* ������·�����¼���¼ */
static VOID OUTC_CheckJCEventFill( VOID )
{
    INT             iStatus = NU_UNAVAILABLE;
    UNSIGNED        uRelayJCFlag;
    UNSIGNED        uEEventBits;
    INT             i;
    OUT_OCB         * this;
    OUT_RELAY       * pOUT_Relay;
    static UNSIGNED RelayJCFlag = 0;
    static UNSIGNED RelayJCDelay = 0;
    static INT iInitFlag = 0;
    /*--------------------------------------------------------------------------*/
    this = & OUT_Channel;
    if( iInitFlag == 0 )
    {
        iInitFlag = 1;
        RelayJCDelay = OUT_Time_Get_Clock();
    }
    /* ������·�����¼����� */
    uRelayJCFlag = ( UNSIGNED )( this->ErrorFlag );
    if(( uRelayJCFlag != 0 ) && ( RelayJCFlag == 0 ))
    {
        /* ������·��������ʱʱ�� */
        if(( OUT_Time_Get_Clock() - RelayJCDelay ) >= OUT_JC_EVENT_DELAY_TIME )
        {
            if(( this->pErrEventGroup )&&( this->Set_EventsBits ))
            {
                this->Set_EventsBits( this->pErrEventGroup, Error_Relay_Close, NU_OR );
            }
            RelayJCFlag = uRelayJCFlag;
        }
    }
    else
    {
        RelayJCDelay = OUT_Time_Get_Clock();
    }
    /* �ÿ�����·�쳣�¼�λ(����/����) */
    /* ��ȡ�����̵�������ָ�� */
    pOUT_Relay = this->pRelayBits;
    uEEventBits = ( UNSIGNED )this->ErrorFlag;  /* ������·�����¼�λ */
    /* ������·�����¼�λ */
    for( i = 0; i < ( this->HardGroupNumber ); i ++)
    {
        uEEventBits |= ( pOUT_Relay + i )->ErrorEventBits;
    }
    /* ������·���߼������¼�λ */
    if( uEEventBits != 0 )
    {
        if(( this->pErrEventGroup )&&( this->Set_EventsBits ))
        {
            this->Set_EventsBits( this->pErrEventGroup, Error_Relay, NU_OR );
        }
    }
    else
    {
        if(( this->pErrEventGroup )&&( this->Set_EventsBits ))
        {
            this->Set_EventsBits( this->pErrEventGroup,( UNSIGNED )(~ Error_Relay ), NU_AND );
        }
    }
}

/* ���ڼ�������� */
VOID OUTC_CheckMain( VOID )
{
    OUT_OCB         * this;
    /*--------------------------------------------------------------------------*/
    this = & OUT_Channel;

    /* Ӳ������⺯�� */
    if( this->HardOutCheckMain != NU_NULL )
    {
        this->HardOutCheckMain();
    }
    /* ��չ��·��⺯�� */
    if( this->EXOutCheckMain != NU_NULL )
    {
        this->EXOutCheckMain();
    }
}

/* ��������̵������غ��� */
VOID OUTC_CheckStartUpReturn( VOID )
{
    OUT_OCB         * this;
    static UNSIGNED StartDelay;
    static UNSIGNED CheckDelay;
    UNSIGNED        uCheckCloseFlag = 0;
    UNSIGNED        uOutBits;
    INT             i;
    OUT_RELAY       * pOUT_Relay;
    static INT iInitFlag = 0;
    /*--------------------------------------------------------------------------*/
    /* �жϳ�ʼ����־ */
    if( iInitFlag == 0 )
    {
        iInitFlag = 1;
        /* ������ʱʱ�� */
        StartDelay = OUT_Time_Get_Clock();
        CheckDelay = OUT_Time_Get_Clock();
    }

    this = & OUT_Channel;

    /* �����̵��������ж� */
    if(( this->StartUpRDFlag ) == 1 )
    {
        if( ( this->StartUp ) == ( this->StartupConst ) )
        {
            if(( OUT_Time_Get_Clock() - StartDelay )> OUT_STARTUP_RDELAY )
            {
                /* �����̵���������ʱʱ�䵽 */
                this->StartUp = 0;
                StartDelay = OUT_Time_Get_Clock();
            }
        }
        else
        {
            StartDelay = OUT_Time_Get_Clock();
        }
    }
    else
    {
        StartDelay = OUT_Time_Get_Clock();
    }

    /* �����̵������غ���ʱһ��ʱ�俪�ſ����Լ� */
    if( ( this->StartUp ) == ( this->StartupConst ) )
    {
        /* �����̵�������,���������Լ� */
        uCheckCloseFlag |= 1;
    }

    /* ��ȡ�����̵�������ָ�� */
    pOUT_Relay = this->pRelayBits;

    /* ����λΪ�Լ�λ,���������Լ� */
    for( i = 0; i <( this->HardGroupNumber ); i ++)
    {
        uOutBits = ((( pOUT_Relay + i )->PickupBits ) & (( pOUT_Relay + i )->CheckBits ));
        uCheckCloseFlag |= uOutBits;
    }

    if( uCheckCloseFlag != 0 )
    {
        /* ���������Լ� */
        this->CloseCheck = 1;
        CheckDelay = OUT_Time_Get_Clock();
    }
    else
    {
        if( this->CloseCheck != 0 )
        {
            /* �����������Լ�������,��ʱһ��ʱ�俪���Լ� */
            if(( OUT_Time_Get_Clock() - CheckDelay ) > OUT_START_OPEN_CHECK )
            {
                this->CloseCheck = 0;
                CheckDelay = OUT_Time_Get_Clock();
            }
        }
        else
        {
            /* ���ſ����Լ� */
            CheckDelay = OUT_Time_Get_Clock();
        }
    }

    /* ���ڱ�λ�ڲ��¼���¼ */
    this->BitsChangeFillEvent();
}

/* �������ִ�к��� */
VOID OUTC_CheckDrive( VOID )
{
    OUT_OCB     * this;
    OUT_RELAY   * pOUT_Relay;
    UNSIGNED    uCheckStatus; /* ������·���״̬ */
    UNSIGNED    uCheckExecuteBits;   /* ����ִ��λ����ڼ��λ�����Ľ�� */
    UNSIGNED    uTemp1, uTemp2;      /* ��ʱ�м���� */
    INT         i;
    INT         iBreakFlag = 0;      /* ������·���߼���־ */
    UNSIGNED    uErrMaskBits;
    /*----------------------------------------*/
    this = & OUT_Channel;
    /* ��ȡ�����̵�������ָ�� */
    pOUT_Relay = this->pRelayBits;

    /* �ж��Ƿ��б������ڱ�־ */
    if( this->CloseRelay )
    {
        /* ���¿�������λ */
        for( i = 0; i < this->TotalGroupNum; i ++)
        {
            ( pOUT_Relay + i )->HoldBits &=  ( pOUT_Relay + i )->PickupBits;
        }
        /* ���������̵�������״̬ */
        this->HoldStartup &= this->StartUp;
    }
    else
    {
        /* ���¿�������λ */
        for( i = 0; i < this->TotalGroupNum; i ++)
        {
            ( pOUT_Relay + i )->HoldBits = ( pOUT_Relay + i )->PickupBits;
        }
        /* ���������̵�������״̬ */
        this->HoldStartup = this->StartUp;
    }

    /* ���ͨ�������б� */
    /* �������ڼ���־ */
    if( this->CloseCheck == 0 )
    {
        /* ִ�м���־ */
        if( this->CheckExecute == 1 )
        {
            /* ����ɼ��,�������ͨ������ */
            this->CheckNumber += 1;
            /* ������ǰ���еļ��ͨ���� */
            this->GroupCheckNum = ( INT )this->CheckNumber;
            /* ͨ�����ͨ����,���ü�����,���õ�ǰ���еļ��ͨ���� */
            OUT_GET_CHECK_GROUP( this->CheckNumber, this->CheckGroup, this->GroupCheckNum );
            /* ��ִ�м���־ */
            this->CheckExecute = 0;
        }
    }
    else
    {
        /* ��ִ�м���־ */
        this->CheckExecute = 0;
    }

    /* ͨ��������,���������̵�������λ������λ�� */
    pOUT_Relay += this->CheckGroup;

    /* �ж��Ƿ���������� */
    /* �ж������ڼ���־ */
    if( this->LockCheck == 0 )
    {
        /* �������ڼ���־ */
        if( this->CloseCheck == 1 )
        {
            /* ����ִ�б�־���������� */
            /* �忪��������������� */
            memset( this->ErrCount, 0, sizeof( this->ErrCount ));
        }
        else
        {
            /* ʵ��ִ��λ & ���߼��λ */
            uCheckExecuteBits = (( pOUT_Relay->ExecuteBits ) & ( pOUT_Relay->CheckBits ));
            /* �жϻ�·��ⷽʽ(�Ƕ����Լ컹�ǻ����Լ�) */
            if( uCheckExecuteBits )
            {
                /* ������·�����Լ� */
                uCheckStatus = 0;
                OUT_OutputCheckPortStatus( ( this->CheckGroup ),& uCheckStatus );
                /* �жϼ��״̬ */
                if( uCheckStatus == 0 )
                {
                    /* ������·���� */
                    pOUT_Relay->ErrorBits |= uCheckExecuteBits;
                }
                else
                {
                    /* ������·���߼������ */
                    pOUT_Relay->ErrorBits &= (~ uCheckExecuteBits );
                }

                /* ����������·���߼�� */
                iBreakFlag = 1;
            }
            else
            {
                /* ������·������� */
                /* ������·�����Լ� */
                /* ��ȡ�����̵�������ָ�� */
                pOUT_Relay = this->pRelayBits;
                uCheckStatus = 0;
                /* ������������·���߼�� */
                iBreakFlag = 1;
                for( i = 0; i < ( this->HardGroupNumber ); i ++)
                {
                    /* ��Ӧ��Ĵ����ʶ,ͨ��λ������ */
                    uErrMaskBits  = ( 1 << i );
                    /* �г��ڼ��λ�Ž��л������ */
                    if(( pOUT_Relay + i )->CheckBits )
                    {
                        /* ��ȡ�ض��ź�״̬ */
                        OUT_OutputCheckPortStatus( i,& uCheckStatus );
                        /* �ж��Ƿ�Ϊ����״̬ */
                        if( uCheckStatus == 0 )
                        {
                            /* ������·����������� */
                            /* ����Ѿ����ڻ���״̬����������������� */
                            if(( this->ErrorFlag & uErrMaskBits ) == 0 )
                            {
                                /* ������������� */
                                this->ErrCount[i] = 0;
                            }
                        }
                        else
                        {
                            /* ������·���� */
                            /* �жϻ�����ʱʱ���Ƿ� */
                            if( this->ErrCount[i] < this->ErrorDelay )
                            {
                                /* ����������ʱʱ�� */
                                this->ErrCount[i] += 1;
                            }
                            else
                            {
                                /* ���� */
                                /* �û��������־ */
                                this->ErrorFlag |= uErrMaskBits;
                            }
                            /* ����ǰ����鴦�ڻ���״̬,����������߼�� */
                            if( this->CheckGroup == i )
                            {
                                /* ��ǰ����鴦�ڻ���״̬,��������߼�� */
                                iBreakFlag = 0;
                            }
                        }
                    }
                }
            }
        }
    }

    /* �ж��Ƿ�����������·���߼�� */
    if( iBreakFlag == 1 )
    {
        /* ����������·���߼�� */
        /* �ж�׼������־ */
        if( this->CheckReady == 1 )
        {
            /* ͨ��������,���������̵�������λ������λ�� */
            pOUT_Relay = this->pRelayBits;
            pOUT_Relay += this->CheckGroup;
            /* ���ĳ���λ */
            uTemp1 = ( UNSIGNED )( 1 << ( this->GroupCheckNum ));
            /* ���û����ü��λ���� */
            uTemp1 &= pOUT_Relay->CheckBits;
            /* ���ڶ���λ */
            uTemp2 = pOUT_Relay->PickupBits;
            /* ����ִ��λ��ȥ���û����õĳ��ڼ��λ */
            uTemp2 &= (~( pOUT_Relay->CheckBits ));
            /* ʵ��ִ�г���λ��ʵ�ʼ��λ����ʵ��ִ�еĳ���λ */
            uTemp2 |= uTemp1;
            /* ��������ʵ��ִ�еĳ���λ */
            pOUT_Relay->ExecuteBits = uTemp2;
            /* ��ִ�м���־ */
            this->CheckExecute = 1;
        }
        else
        {
            /* �����ڶ���λ����ʵ��ִ��λ */
            OUT_PICKUP_TO_EXECUTE;
        }
    }
    else
    {
        /* �����ڶ���λ����ʵ��ִ��λ */
        OUT_PICKUP_TO_EXECUTE;
    }

    /* �жϿ�����·���߼���Ƿ���� */
    if( this->CheckNumber >= OUT_MAX_CHECK_RELAY )
    {
        /* ������·����һ�μ����� */
        this->CheckNumber = 0; /* ����·�������� */
        this->CheckReady = 0;  /* ��׼������־ */
        this->CheckExecute = 0;/* ��ִ�м���־ */
    }

    /* �������ڼ���־ */
    this->LockCheck = 0;

    /* ���ó���ִ�к��� */
    OUTC_DriveFunction();
}

/* ����ִ�к��� */
VOID OUTC_DriveFunction( VOID )
{
    INT       i;
    OUT_RELAY * pOUT_Relay;
    OUT_OCB   * this;
    UNSIGNED  uBits;
    UNSIGNED  StartFlag = 0;
    UNSIGNED  buffer[ CPC_MAX_OUTPUT_GROUPS ];
    /*===========================*/
    this = & OUT_Channel;
    /* ��ȡ�����̵�������ָ�� */
    pOUT_Relay = this->pRelayBits;
    /* �ж��Ƿ�������� */
    if( this->ErrorFlag )
    {
        /* ������·����,����ȫ�����λ */
        for( i = 0; i < this->TotalGroupNum; i ++)
        {
            /* ����ʵ��ִ��λ */
            ( pOUT_Relay + i )->ExecuteBits &= (~( pOUT_Relay + i )->CheckBits );
        }
    }
    else
    {
        /* �ж��Ƿ��б������ڱ�־ */
        if( this->CloseRelay )
        {
            for( i = 0; i < this->TotalGroupNum; i ++)
            {
                /* ��Դ����,�����Ѿ������ĳ��� */
                uBits = (( pOUT_Relay + i )->CheckBits & ( pOUT_Relay + i )->HoldBits );
                /* ����ʵ��ִ��λ */
                ( pOUT_Relay + i )->ExecuteBits &= (~(( pOUT_Relay + i )->CheckBits & (~ uBits )));
            }
            if( this->HoldStartup == this->StartupConst )
            {
                /* �����̵������� */
                StartFlag = this->StartupConst;
            }
        }
        else
        {
            if( this->StartUp == this->StartupConst )
            {
                /* �����̵������� */
                StartFlag = this->StartupConst;
            }
        }
    }

    if( StartFlag == this->StartupConst )
    {
        /* �����̵������� */
        for( i = 0; i < this->TotalGroupNum; i ++ )
        {
            buffer[i] = this->StartupConst;
        }
        OUT_CHECK_RELAY_START( 0, this->iStartUpGroup, buffer );
    }
    else
    {
        /* �����̵������� */
        for( i = 0; i < this->TotalGroupNum; i ++ )
        {
            buffer[i] = 0;
        }
        OUT_CHECK_RELAY_START( 0, this->iStartUpGroup, buffer );
    }

    /* �̵������� */
    for( i = 0; i < this->TotalGroupNum; i ++)
    {
        COUTC_RelayDriver( i,(( pOUT_Relay + i )->ExecuteBits ));
    }

    /* ����GOOSE���� */
    if( this->SendGooseFun != NU_NULL )
    {
        this->SendGooseFun();
    }
}

/* ���ڱ�λ�ڲ��¼���¼ */
static VOID OUTC_CheckBitsChangeFillEvent( VOID )
{
    OUT_OCB         * this;
    static UNSIGNED RelayBit[CPC_MAX_OUTPUT_GROUPS];
    static UNSIGNED StartFlag = 0;
    UNSIGNED        uTempRelayBit[CPC_MAX_OUTPUT_GROUPS];
    UNSIGNED        uTempStartUp;
    UNSIGNED        uStartFlag;
    OUT_RELAY       * pOUT_Relay;
    CHAR            EventName[OUTC_CHECK_EVENT_NAME_LENGTH];
    UNSIGNED        PickUpCheckDelay = 0;
    static INT      iInitFlag = 0;
    INT             i;
    INT             iChangeFlag = 0;
    /*--------------------------------------------------------------------------*/
    this = & OUT_Channel;

    /* ��ʼ�����ݻ����� */
    if( iInitFlag == 0 )
    {
        iInitFlag = 1;
        /* ������ݻ����� */
        memset( RelayBit, 0,( sizeof( UNSIGNED )* ( CPC_MAX_OUTPUT_GROUPS ) ));
    }
    /* ������ݻ����� */
    memset( uTempRelayBit, 0,( sizeof( UNSIGNED )* CPC_MAX_OUTPUT_GROUPS ));

    /* ��ȡ�����̵�������ָ�� */
    pOUT_Relay = this->pRelayBits;

    if(( OUT_Time_Get_Clock() - PickUpCheckDelay )> OUT_PICKUP_CHECKDELAY )
    {
        /* ���³��ڱ�λ��ʱʱ�� */
        PickUpCheckDelay = OUT_Time_Get_Clock();

        OUTC_CheckProtect();
        for( i = 0; i < ( this->TotalGroupNum ); i ++)
        {
            uTempRelayBit[i] = ( pOUT_Relay + i )->PickupBits;
        }
        uTempStartUp = this->StartUp;                    /* �����̵���״̬ */
        OUTC_CheckUnProtect();

        if( uTempStartUp != StartFlag )
        {
            iChangeFlag = 1;
        }
        else
        {
            /* �жϳ���λ�Ƿ����仯 */
            for( i = 0; i < ( this->TotalGroupNum ); i ++)
            {
                if(( uTempRelayBit[i] & (( pOUT_Relay + i )->ChangeRegBits )) != RelayBit[i] )
                {
                    iChangeFlag = 1;
                    break;
                }
            }
        }
        /* ���ڱ�λ��Ϣ��¼ */
        if( iChangeFlag == 1 )
        {
            StartFlag = uTempStartUp;
            for( i = 0; i < ( this->TotalGroupNum ); i ++)
            {
                RelayBit[i] = ( uTempRelayBit[i] & (( pOUT_Relay + i )->ChangeRegBits ));
            }
            if( uTempStartUp == this->StartupConst )
            {
                /* �����̵������� */
                uStartFlag = 1;
            }
            else
            {
                /* �����̵������� */
                uStartFlag = 0;
            }
            if( this->FillEvent != NU_NULL )
            {
                sprintf( EventName, " %03d:%08lX-%08lX-%04X-%01d",
                    LS_RES_EVENT_RELAY, uTempRelayBit[0], uTempRelayBit[1], uTempRelayBit[2], uStartFlag );/* ��д���ڱ仯��¼ */
                this->FillEvent( EventName, OUTC_CHECK_RESERVE_EVENT_ID );
            }
        }
    }
}

/* ���ÿ�����״̬�仯��¼������λ���� */
INT OUTC_CheckSetOutLogMaskBits( INT iGroupNum, UNSIGNED MaskBits )
{
    OUT_OCB         * this;
    OUT_RELAY       * pOUT_Relay;
    INT             iStatus = NU_UNAVAILABLE;
    /*--------------------------------------------------------------------------*/
    this = & OUT_Channel;
    /* ��ȡ�����̵�������ָ�� */
    pOUT_Relay = this->pRelayBits;
    if( iGroupNum < ( this->TotalGroupNum ) )
    {
        ( pOUT_Relay + iGroupNum ) -> ChangeRegBits = MaskBits;
        iStatus = NU_SUCCESS;
    }
    return( iStatus );
}

/* ��չ��·��⺯�� */
static VOID OUTC_OutEX_Check( VOID )
{
    static INT      iExpErrFlag = 0;
    OUT_OCB         * this;
    /*--------------------------------------------------------------------------*/
    this = & OUT_Channel;
    /* ʹ����չ����,�Լ���չ���������Ƿ����� */
    if( this->ExtEnable == 1 )
    {
        if( OUTC_ExportPort_Check() == NU_TRUE )
        {
            if( iExpErrFlag != 0 )
            {
                if(( this->pErrEventGroup )&&( this->Set_EventsBits ))
                {
                    this->Set_EventsBits( this->pErrEventGroup,( UNSIGNED )(~ ( Error_Extend_Port ) ), NU_AND );
                }
            }
            iExpErrFlag = 0;
        }
        else
        {
            if( iExpErrFlag == 0 )
            {
                if(( this->pErrEventGroup )&&( this->Set_EventsBits ))
                {
                    this->Set_EventsBits( this->pErrEventGroup,  Error_Extend_Port, NU_OR );
                }
            }
            iExpErrFlag = 1;
        }
    }
}

/* ��չ����Լ캯��(NU_TRUE:�ɹ�/NU_FALSE:ʧ��) */
static INT OUTC_ExportPort_Check( VOID )
{
    UNSIGNED uStatus[CPC_MAX_OUTPUT_GROUPS];
    INT      i;
    OUT_OCB  * this;
    INT      iStatus = NU_TRUE;
    /*==================================*/
    this = & OUT_Channel;
    memset( uStatus, 0, sizeof( uStatus ));
    /* ��������ض����� */
    if( DEVI_PlugReadbackDriver( 0, this->iHardExtendCount, uStatus ) == XJ_SUCCESS )
    {
        /* �жϻض�״̬�Ƿ���ȷ */
        for( i = 0; i < this->iHardExtendCount; i ++)
        {
            if( uStatus[i] != NU_TRUE )
            {
                iStatus = NU_FALSE;
                break;
            }
        }
    }
    else
    {
        iStatus = NU_FALSE;
    }

    return( iStatus );
}

/* ��ȡ�����Լ�ض�״̬ */
static INT OUT_OutputCheckPortStatus( INT iGroup, UNSIGNED * uStatus )
{
    /*==================================*/
    /* ������·�ض����� */
    DEVI_OutReadbackDriver( iGroup, 1, uStatus );

    return( NU_SUCCESS );
}

/* ���ñ������ڱ�־(iFlag:1�ñ������ڱ�־,iFlag:0��������ڱ�־) */
INT OUT_SetClockRelayFlag( INT iFlag )
{
    if( iFlag == 1 )
    {
        OUT_Channel.CloseRelay = 1;
    }
    else
    {
        OUT_Channel.CloseRelay = 0;
    }
    return( NU_SUCCESS );
}

/* ��ȡ�������ڱ�־(iFlag:1��������,iFlag:0����������) */
INT OUT_GetClockRelayFlag( INT * iFlag )
{
    * iFlag = OUT_Channel.CloseRelay;
    return( NU_SUCCESS );
}

/* ��ȡ����λ״̬ */
UNSIGNED OUT_GetOutPickupBits( INT GroupNo )
{
    OUT_OCB         * this;
    OUT_RELAY       * pOUT_Relay;
    INT             iStatus = NU_UNAVAILABLE;
    UNSIGNED        uStatus = 0;
    /*--------------------------------------------------------------------------*/
    this = & OUT_Channel;
    /* ��ȡ�����̵�������ָ�� */
    pOUT_Relay = this->pRelayBits;
    if( GroupNo < ( this->TotalGroupNum ) )
    {
        uStatus = ( pOUT_Relay + GroupNo ) -> PickupBits;
    }
    return( uStatus );
}

/* Ӳ������⺯�� */
static VOID OUTC_HardOutCheckMain( VOID )
{
    OUT_OCB         * this;
    /*--------------------------------------------------------------------------*/
    this = & OUT_Channel;

    /* �̵���������·���߼�⺯�� */
    this->OutBk_Check();
    /* ������·�����¼���¼ */
    this->OutBk_EventFill();
    /* ������·�����¼���¼ */
    this->OutJC_EventFill();
}

/* ע��Ӳ������⺯�� */
static VOID OUTC_RegHardOutCheck( VOID (* HardOutCheck )( VOID ))
{
    OUT_Channel.HardOutCheckMain = HardOutCheck;
}

/* ע����չ������⺯�� */
static VOID OUTC_RegEXOutCheck( VOID (* EXOutCheck )( VOID ))
{
    OUT_Channel.EXOutCheckMain = EXOutCheck;
}

/* ��ȡ�������� */
INT OUTC_GetOutGroupCount( INT * iTotalCount, INT * iHardCount )
{
    if( iHardCount != NU_NULL )
    {
        * iHardCount = ( OUT_Channel.HardGroupNumber );
    }
    if( iTotalCount != NU_NULL )
    {
        * iTotalCount = ( OUT_Channel.TotalGroupNum );
    }

    return( NU_SUCCESS );
}

/*================================================*/
/* �����ִ��λ�仯��麯�� */
INT OUTC_SoftOutExBitsCheck( VOID )
{
    static UNSIGNED LastOutQ[CPC_MAX_OUTPUT_GROUPS];       /* �ϴμ�¼�Ŀ���Ʒ��������Ч��Чλ��״̬ */
    static UNSIGNED LastOutQJ[CPC_MAX_OUTPUT_GROUPS];      /* �ϴμ�¼�Ŀ���Ʒ�����ؼ���λ��״̬ */
    static UNSIGNED LastOutStatus[CPC_MAX_OUTPUT_GROUPS];  /* �ϴγ���λ״̬ */
    static INT iInitFlag = 0;
    UNSIGNED NewOutQ[CPC_MAX_OUTPUT_GROUPS];        /* ���¼�¼�Ŀ���Ʒ��������Ч��Чλ��״̬ */
    UNSIGNED NewOutQJ[CPC_MAX_OUTPUT_GROUPS];       /* ���¼�¼�Ŀ���Ʒ�����ؼ���λ��״̬ */
    UNSIGNED NewOutStatus[CPC_MAX_OUTPUT_GROUPS];   /* ���¼�¼�ĳ���λ״̬ */
    INT    iStatus = NU_FALSE;
    INT    i;
    static INT iSoftCount = 0;
    CHAR   LogName[OUTC_LOG_MAX_LEN];
    CHAR   TempBuf[30];   /* ��ʱ������ */
    static UNSIGNED DevQStatus = 0;             /* װ��״̬,�����NPI��� */
    /*======================================*/
    if( iInitFlag == 0 )
    {
        /* �建���� */
        memset( ( VOID *)LastOutQ, 0, sizeof( LastOutQ ));
        memset( ( VOID *)LastOutQJ, 0, sizeof( LastOutQJ ));
        memset( ( VOID *)LastOutStatus, 0, sizeof( LastOutStatus ));
        /* �ж��Ƿ��������� */
        if( OUT_Channel.TotalGroupNum > OUT_Channel.HardGroupNumber )
        {
            iSoftCount = OUT_Channel.TotalGroupNum - OUT_Channel.HardGroupNumber;
        }

        if( iSoftCount > CPC_MAX_OUTPUT_GROUPS )
        {
            iSoftCount = CPC_MAX_OUTPUT_GROUPS;
        }
        iInitFlag = 1;
    }
    /* ��������ʱ����б�λ��� */
    if( iSoftCount )
    {
        /* ��ȡ����״̬��Ʒ������ */
        for( i = 0; i < iSoftCount; i ++)
        {
            COUTC_ReadSoftOutTotalQua(( OUT_Channel.HardGroupNumber + i ),& NewOutStatus[i],& NewOutQ[i],& NewOutQJ[i] );
        }

        /* �ж�Ʒ�������Ƿ����仯 */
        for( i = 0; i < iSoftCount; i ++)
        {
            /* ��Ч��Чλ */
            if( LastOutQ[i] != NewOutQ[i] )
            {
                iStatus = NU_TRUE;
                break;
            }
            /* ����λ */
            if( LastOutQJ[i] != NewOutQJ[i] )
            {
                iStatus = NU_TRUE;
                break;
            }
            /* ����״̬ */
            if( LastOutStatus[i] != NewOutStatus[i] )
            {
                iStatus = NU_TRUE;
                break;
            }
        }

        if( DevQStatus != CSMV_MSG_DevQStatus )
        {
            DevQStatus = CSMV_MSG_DevQStatus;
            iStatus = NU_TRUE;
        }
        /* �ж�״̬�Ƿ��б仯 */
        if( iStatus == NU_TRUE )
        {
            sprintf( LogName, "%s-DevST:%d", OUTC_OUT_LOG_NAME, DevQStatus );
            for( i = 0; i < iSoftCount; i ++)
            {
                sprintf( TempBuf, " %d-%08lX:%08lX:%08lX", i, NewOutStatus[i], NewOutQ[i], NewOutQJ[i] );
                strcat( LogName, TempBuf );

                /* ÿ5��һ��,дһ����־ */
                if ( i % 5 == 4 )
                {
                    CLOG_NoProtectLog( ( CHAR *)LogName );
                    /*�ַ�������������*/
                    memset( LogName, 0, sizeof( LogName ) );
                    sprintf( LogName, "%s-DevST:%d", OUTC_OUT_LOG_NAME, DevQStatus );
                }
                else if ( i == ( iSoftCount - 1 ) )
                {
                    CLOG_NoProtectLog( ( CHAR *)LogName );
                }
            }
        }

        /* ����Ʒ�����ؼ�״̬ */
        memcpy( LastOutStatus, NewOutStatus, sizeof( UNSIGNED ) * iSoftCount );
        memcpy( LastOutQ, NewOutQ, sizeof( UNSIGNED ) * iSoftCount );
        memcpy( LastOutQJ, NewOutQJ, sizeof( UNSIGNED ) * iSoftCount );
    }

    return( iStatus );
}

/*================================================*/
