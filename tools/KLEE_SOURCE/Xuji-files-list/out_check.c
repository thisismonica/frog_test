
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

#define OUTC_OUT_LOG_NAME           "出口变位信息"
#define OUTC_LOG_MAX_LEN            256                         /* 日志报文最大长度 */

extern UNSIGNED CSMV_MSG_DevQStatus;       /* 装置状态 */
/* 开出继电器数据位(硬开出及软开出均有) */
static OUT_RELAY OUT_RelayBits[CPC_MAX_OUTPUT_GROUPS];

/* 数据保护 */
#define OUTC_CheckProtect()                  \
  {   \
    if(OUT_Channel.Protect != NU_NULL)   \
        NU_Protect(OUT_Channel.Protect); \
  }

/* 数据保护 */
#define OUTC_CheckUnProtect()               \
  {   \
    if(OUT_Channel.Protect != NU_NULL) \
        NU_Unprotect();                \
  }

/* 以下为OS相关配置 */
#ifdef WIN32
/* 全局变量申明 */
extern volatile UNSIGNED TMD_System_Clock;
/* Windows 宏定义 */
/* 宏定义 */
#define OUT_Time_Get_Clock()        TMD_System_Clock       /* 获取系统时钟 */
#define OUT_CHECK_RELAY_START(x,y,z)                       /* 启动继电器执行函数 */
#endif

#ifndef WIN32 /* 非windows缺省定义一些宏以便通过编译 */
/* 全局变量申明 */
/* Nucleus宏定义 */
extern volatile UNSIGNED TMD_System_Clock;

/* 宏定义 */
#define OUT_Time_Get_Clock()        TMD_System_Clock     /* 获取系统时钟 */
#define OUT_CHECK_RELAY_START(x,y,z) DEVI_WriteStartRelay(x,y,z) /* 启动继电器执行函数 */
#endif
/*================================================================*/
/* 内部函数 */
/* 继电器开出回路断线检测函数 */
static VOID OUTC_CheckBreak( VOID );
/* 开出回路断线事件记录 */
static VOID OUTC_CheckBreakEventFill( VOID );
/* 开出回路击穿事件记录 */
static VOID OUTC_CheckJCEventFill( VOID );
/* 出口变位内部事件记录 */
static VOID OUTC_CheckBitsChangeFillEvent( VOID );
/* 获取开出自检回读状态 */
static INT OUT_OutputCheckPortStatus( INT iGroup, UNSIGNED * uStatus );
/* 扩展插件自检函数(NU_TRUE:成功/NU_FALSE:失败) */
static INT OUTC_ExportPort_Check( VOID );
/* 扩展回路检测函数 */
static VOID OUTC_OutEX_Check( VOID );
/* 硬开出检测函数 */
static VOID OUTC_HardOutCheckMain( VOID );
/* 注册硬开出检测函数 */
static VOID OUTC_RegHardOutCheck( VOID (* HardOutCheck )( VOID ));
/* 注册扩展开出检测函数 */
static VOID OUTC_RegEXOutCheck( VOID (* EXOutCheck )( VOID ));
/*================================================================*/

/* 将出口位赋值给出口执行位 */
#define OUT_PICKUP_TO_EXECUTE  \
{   \
    INT  i;\
    for(i=0;i<CPC_MAX_OUTPUT_GROUPS;i++)\
    {   \
        OUT_RelayBits[i].ExecuteBits = OUT_RelayBits[i].PickupBits;\
    }\
}

/* 通过自检位获取需要自检的开出的个数(使用此方法检测时最多多检测三路) */
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

/* 通过判断检测通道数(x),设置检测组号(y),设置当前组中的检测通道号(z) */
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

/* 设置出口继电器动作位 */
VOID OUTC_Check_SetPickupBits( INT GroupNo, UNSIGNED uBits )
{
    OUT_OCB         * this;
    OUT_RELAY       * pOUT_Relay;
    INT old_level;
    /*--------------------------------------------------------------------------*/
    this = & OUT_Channel;
    /* 获取开出继电器数据指针 */
    pOUT_Relay = this->pRelayBits;

    if( ( INT )GroupNo < ( this->TotalGroupNum ) )
    {
        ( pOUT_Relay + GroupNo ) -> PickupBits = uBits;
    }

    /* 锁中断修改出口执行位 */
    /* lock interrupt */
    old_level = NU_Local_Control_Interrupts( 0 );
    /* 置锁出口检测标志 */
    this->LockCheck = 1;
    /* 清执行检测标志 */
    this->CheckExecute = 0;
    /* 将出口位赋值给出口执行位 */
    OUT_PICKUP_TO_EXECUTE;
    /* release interrupt */
    NU_Local_Control_Interrupts( old_level );
}

/* 启动继电器驱动函数 */
VOID OUTC_Check_StartDriver( UNSIGNED uFlag )
{
    OUT_OCB         * this;
    /*--------------------------------------------------------------------------*/
    this = & OUT_Channel;

    /* 启动立即执行,返回有延时时间 */
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

/* 出口检测初始化函数 */
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
    /* 清数据缓冲区 */
    memset( OUT_RelayBits, 0,( sizeof( OUT_RELAY )* CPC_MAX_OUTPUT_GROUPS ));

    this = & OUT_Channel;
    /* 获取开出继电器数据指针 */
    pOUT_Relay = this->pRelayBits;
    /* 硬开出组数 */
    if( uHardGroupNum > CPC_MAX_OUTPUT_GROUPS )
    {
        this->HardGroupNumber = CPC_MAX_OUTPUT_GROUPS;
    }
    else
    {
        this->HardGroupNumber = ( INT )uHardGroupNum;
    }

    /* 判断是否有硬开出 */
    if( this->HardGroupNumber > 0 )
    {
        /* 注册硬开出检测函数 */
        OUTC_RegHardOutCheck(& OUTC_HardOutCheckMain );
    }

    /* 总开出组数 */
    if( uTotalGroupNum > CPC_MAX_OUTPUT_GROUPS )
    {
        this->TotalGroupNum = CPC_MAX_OUTPUT_GROUPS;
    }
    else
    {
        this->TotalGroupNum = ( INT )uTotalGroupNum;
    }

    /* 判断是否有软开出,决定是否注册发送GOOSE函数 */
    if( this->TotalGroupNum > this->HardGroupNumber )
    {
        this->SendGooseFun = SendGooseFun;
    }
    else
    {
        this->SendGooseFun = NU_NULL;
    }

    /* 记录扩展插件个数 */
    this->iHardExtendCount = iHardExtendCount;
    if( this->iHardExtendCount > CPC_MAX_OUTPUT_GROUPS )
    {
        this->iHardExtendCount = CPC_MAX_OUTPUT_GROUPS;
    }
    /* 判断是否有扩展插件 */
    if( this->iHardExtendCount )
    {
        this->ExtEnable = 1;
        /* 注册扩展开出检测函数 */
        OUTC_RegEXOutCheck(& OUTC_OutEX_Check );
    }
    /* 数据保护 */
    this->Protect = Protect;
    /* 开出检测位 */
    for( i = 0; i < ( this->HardGroupNumber ); i ++)
    {
        ( pOUT_Relay + i )->CheckBits = *( pCheckBits + i );
        /* 检测位 */
        uCheckBits = *( pCheckBits + i );
        /* 通过检测位获取最大检测路数 */
        OUT_CHECK_BITS_TO_COUNT( iCheckCount, uCheckBits );
        /* 检测路数初始化 */
        ( pOUT_Relay + i )->CheckNum = iCheckCount;
    }
    /* 开出检测路数 */
    if( uCheckNum > OUT_MAX_CHECK_RELAY )
    {
        this->CheckRelayNum = OUT_MAX_CHECK_RELAY;
    }
    else
    {
        this->CheckRelayNum = uCheckNum;
    }

    /* 错误事件组 */
    this->pErrEventGroup = pErrEventGroup;
    /* 填写事件记录函数 */
    this->FillEvent = FillEvent;
    /* 设置事件位函数 */
    this->Set_EventsBits = Set_EventsBits;

    /* 获取启动组数 */
    this->iStartUpGroup = this->iHardExtendCount;
    /* 判断是否有基本启动回路 */
    if( OUT_GET_BASE_STARTUP_FLAG())
    {
        /* 增加基本启动一路 */
        this->iStartUpGroup += 1;
    }
}

/* 继电器开出回路断线检测函数 */
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
    /* 获取开出继电器数据指针 */
    pOUT_Relay = this->pRelayBits;

    if( iInitFlag == 0 )
    {
        /* 初始上电 */
        iInitFlag = 1;
        /* 更新延时时间 */
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
            /* 开出回路断线检测完成 */
            if(( OUT_Time_Get_Clock() - uErrorDelay ) >= OUT_OUTERROR_DELAY )
            {
                iRelayCheck = 0;

                for( i = 0; i < ( this->HardGroupNumber ); i ++)
                {
                    pTOUT_Relay = ( pOUT_Relay + i );
                    /* 判断是否有错误位 */
                    if( pTOUT_Relay->ErrorBits > 0 )
                    {
                        for( j = 0; j < ( pTOUT_Relay->CheckNum ); j ++)
                        {
                            uSubBits = ( UNSIGNED )( 1 << ( j & OUT_SUB_GROUP_MASK32 ));
                            if(( pTOUT_Relay->ErrorBits & uSubBits ) != 0 )
                            {
                                if(( ( pTOUT_Relay->ErrorEventBits ) & uSubBits ) == 0 )
                                {
                                    /* 调整错误计数器 */
                                    pTOUT_Relay->BreakErrorCount[j] += 1;
                                    if( pTOUT_Relay->BreakErrorCount[j] >= OUT_OUTERROR_NUMBER )
                                    {
                                        pTOUT_Relay->ErrorEventBits |= uSubBits;
                                    }
                                }
                            }
                            else
                            {
                                /* 清错误计数器 */
                                pTOUT_Relay->BreakErrorCount[j] = 0;
                                pTOUT_Relay->ErrorEventBits &= (~ uSubBits );
                            }
                        }
                    }
                    else
                    {
                        /* 如果没有错误位,将错误计数器全部清零 */
                        memset( pTOUT_Relay->BreakErrorCount, 0,( sizeof( UNSIGNED )* OUT_GROUP_RELAY_BITS ));
                        /* 如果没有错误位,错误事件位全部清零 */
                        pTOUT_Relay->ErrorEventBits = 0;
                    }
                }
            }
            else
            {
                /* 自检条件不满足,清自检结果 */
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

/* 开出回路断线事件记录 */
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
        /* 初始化缓冲区 */
        memset( uErrorEventBits, 0,( sizeof( UNSIGNED )* CPC_MAX_OUTPUT_GROUPS ));
    }

    this = & OUT_Channel;
    /* 获取开出继电器数据指针 */
    pOUT_Relay = this->pRelayBits;

    /* 开出回路断线事件记录 */
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

/* 开出回路击穿事件记录 */
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
    /* 开出回路击穿事件设置 */
    uRelayJCFlag = ( UNSIGNED )( this->ErrorFlag );
    if(( uRelayJCFlag != 0 ) && ( RelayJCFlag == 0 ))
    {
        /* 开出回路击穿的延时时间 */
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
    /* 置开出回路异常事件位(断线/击穿) */
    /* 获取开出继电器数据指针 */
    pOUT_Relay = this->pRelayBits;
    uEEventBits = ( UNSIGNED )this->ErrorFlag;  /* 开出回路击穿事件位 */
    /* 开出回路断线事件位 */
    for( i = 0; i < ( this->HardGroupNumber ); i ++)
    {
        uEEventBits |= ( pOUT_Relay + i )->ErrorEventBits;
    }
    /* 开出回路断线及击穿事件位 */
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

/* 出口检测主函数 */
VOID OUTC_CheckMain( VOID )
{
    OUT_OCB         * this;
    /*--------------------------------------------------------------------------*/
    this = & OUT_Channel;

    /* 硬开出检测函数 */
    if( this->HardOutCheckMain != NU_NULL )
    {
        this->HardOutCheckMain();
    }
    /* 扩展回路检测函数 */
    if( this->EXOutCheckMain != NU_NULL )
    {
        this->EXOutCheckMain();
    }
}

/* 检测启动继电器返回函数 */
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
    /* 判断初始化标志 */
    if( iInitFlag == 0 )
    {
        iInitFlag = 1;
        /* 更新延时时间 */
        StartDelay = OUT_Time_Get_Clock();
        CheckDelay = OUT_Time_Get_Clock();
    }

    this = & OUT_Channel;

    /* 启动继电器返回判断 */
    if(( this->StartUpRDFlag ) == 1 )
    {
        if( ( this->StartUp ) == ( this->StartupConst ) )
        {
            if(( OUT_Time_Get_Clock() - StartDelay )> OUT_STARTUP_RDELAY )
            {
                /* 启动继电器返回延时时间到 */
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

    /* 启动继电器返回后延时一段时间开放开出自检 */
    if( ( this->StartUp ) == ( this->StartupConst ) )
    {
        /* 启动继电器动作,闭锁开出自检 */
        uCheckCloseFlag |= 1;
    }

    /* 获取开出继电器数据指针 */
    pOUT_Relay = this->pRelayBits;

    /* 出口位为自检位,闭锁开出自检 */
    for( i = 0; i <( this->HardGroupNumber ); i ++)
    {
        uOutBits = ((( pOUT_Relay + i )->PickupBits ) & (( pOUT_Relay + i )->CheckBits ));
        uCheckCloseFlag |= uOutBits;
    }

    if( uCheckCloseFlag != 0 )
    {
        /* 闭锁开出自检 */
        this->CloseCheck = 1;
        CheckDelay = OUT_Time_Get_Clock();
    }
    else
    {
        if( this->CloseCheck != 0 )
        {
            /* 在条件允许自检的情况下,延时一段时间开放自检 */
            if(( OUT_Time_Get_Clock() - CheckDelay ) > OUT_START_OPEN_CHECK )
            {
                this->CloseCheck = 0;
                CheckDelay = OUT_Time_Get_Clock();
            }
        }
        else
        {
            /* 开放开出自检 */
            CheckDelay = OUT_Time_Get_Clock();
        }
    }

    /* 出口变位内部事件记录 */
    this->BitsChangeFillEvent();
}

/* 开出检测执行函数 */
VOID OUTC_CheckDrive( VOID )
{
    OUT_OCB     * this;
    OUT_RELAY   * pOUT_Relay;
    UNSIGNED    uCheckStatus; /* 开出回路检测状态 */
    UNSIGNED    uCheckExecuteBits;   /* 出口执行位与出口检测位相与后的结果 */
    UNSIGNED    uTemp1, uTemp2;      /* 临时中间变量 */
    INT         i;
    INT         iBreakFlag = 0;      /* 开出回路断线检测标志 */
    UNSIGNED    uErrMaskBits;
    /*----------------------------------------*/
    this = & OUT_Channel;
    /* 获取开出继电器数据指针 */
    pOUT_Relay = this->pRelayBits;

    /* 判断是否有闭锁出口标志 */
    if( this->CloseRelay )
    {
        /* 更新开出保持位 */
        for( i = 0; i < this->TotalGroupNum; i ++)
        {
            ( pOUT_Relay + i )->HoldBits &=  ( pOUT_Relay + i )->PickupBits;
        }
        /* 更新启动继电器保持状态 */
        this->HoldStartup &= this->StartUp;
    }
    else
    {
        /* 更新开出保持位 */
        for( i = 0; i < this->TotalGroupNum; i ++)
        {
            ( pOUT_Relay + i )->HoldBits = ( pOUT_Relay + i )->PickupBits;
        }
        /* 更新启动继电器保持状态 */
        this->HoldStartup = this->StartUp;
    }

    /* 检测通道调整判别 */
    /* 闭锁出口检测标志 */
    if( this->CloseCheck == 0 )
    {
        /* 执行检测标志 */
        if( this->CheckExecute == 1 )
        {
            /* 已完成检测,调整检测通道号码 */
            this->CheckNumber += 1;
            /* 调整当前组中的检测通道号 */
            this->GroupCheckNum = ( INT )this->CheckNumber;
            /* 通过检测通道号,设置检测组号,设置当前组中的检测通道号 */
            OUT_GET_CHECK_GROUP( this->CheckNumber, this->CheckGroup, this->GroupCheckNum );
            /* 清执行检测标志 */
            this->CheckExecute = 0;
        }
    }
    else
    {
        /* 清执行检测标志 */
        this->CheckExecute = 0;
    }

    /* 通过检测组号,调整开出继电器数据位缓冲区位置 */
    pOUT_Relay += this->CheckGroup;

    /* 判断是否允许检测出口 */
    /* 判断锁出口检测标志 */
    if( this->LockCheck == 0 )
    {
        /* 闭锁出口检测标志 */
        if( this->CloseCheck == 1 )
        {
            /* 清检测执行标志、击穿次数 */
            /* 清开出击穿错误计数器 */
            memset( this->ErrCount, 0, sizeof( this->ErrCount ));
        }
        else
        {
            /* 实际执行位 & 断线检测位 */
            uCheckExecuteBits = (( pOUT_Relay->ExecuteBits ) & ( pOUT_Relay->CheckBits ));
            /* 判断回路检测方式(是断线自检还是击穿自检) */
            if( uCheckExecuteBits )
            {
                /* 开出回路断线自检 */
                uCheckStatus = 0;
                OUT_OutputCheckPortStatus( ( this->CheckGroup ),& uCheckStatus );
                /* 判断检测状态 */
                if( uCheckStatus == 0 )
                {
                    /* 开出回路断线 */
                    pOUT_Relay->ErrorBits |= uCheckExecuteBits;
                }
                else
                {
                    /* 开出回路断线检测正常 */
                    pOUT_Relay->ErrorBits &= (~ uCheckExecuteBits );
                }

                /* 启动开出回路断线检测 */
                iBreakFlag = 1;
            }
            else
            {
                /* 开出回路击穿检测 */
                /* 开出回路击穿自检 */
                /* 获取开出继电器数据指针 */
                pOUT_Relay = this->pRelayBits;
                uCheckStatus = 0;
                /* 置启动开出回路断线检测 */
                iBreakFlag = 1;
                for( i = 0; i < ( this->HardGroupNumber ); i ++)
                {
                    /* 相应组的错误标识,通过位来定义 */
                    uErrMaskBits  = ( 1 << i );
                    /* 有出口检查位才进行击穿检查 */
                    if(( pOUT_Relay + i )->CheckBits )
                    {
                        /* 获取回读信号状态 */
                        OUT_OutputCheckPortStatus( i,& uCheckStatus );
                        /* 判断是否为击穿状态 */
                        if( uCheckStatus == 0 )
                        {
                            /* 开出回路击穿检测正常 */
                            /* 如果已经处于击穿状态不清击穿次数计数器 */
                            if(( this->ErrorFlag & uErrMaskBits ) == 0 )
                            {
                                /* 清击穿检测计数器 */
                                this->ErrCount[i] = 0;
                            }
                        }
                        else
                        {
                            /* 开出回路击穿 */
                            /* 判断击穿延时时间是否到 */
                            if( this->ErrCount[i] < this->ErrorDelay )
                            {
                                /* 调整击穿延时时间 */
                                this->ErrCount[i] += 1;
                            }
                            else
                            {
                                /* 击穿 */
                                /* 置击穿错误标志 */
                                this->ErrorFlag |= uErrMaskBits;
                            }
                            /* 若当前检查组处于击穿状态,将不允许断线检查 */
                            if( this->CheckGroup == i )
                            {
                                /* 当前检查组处于击穿状态,不允许断线检查 */
                                iBreakFlag = 0;
                            }
                        }
                    }
                }
            }
        }
    }

    /* 判断是否启动开出回路断线检测 */
    if( iBreakFlag == 1 )
    {
        /* 启动开出回路断线检测 */
        /* 判断准备检测标志 */
        if( this->CheckReady == 1 )
        {
            /* 通过检测组号,调整开出继电器数据位缓冲区位置 */
            pOUT_Relay = this->pRelayBits;
            pOUT_Relay += this->CheckGroup;
            /* 检测的出口位 */
            uTemp1 = ( UNSIGNED )( 1 << ( this->GroupCheckNum ));
            /* 与用户设置检测位相与 */
            uTemp1 &= pOUT_Relay->CheckBits;
            /* 出口动作位 */
            uTemp2 = pOUT_Relay->PickupBits;
            /* 出口执行位中去除用户设置的出口检测位 */
            uTemp2 &= (~( pOUT_Relay->CheckBits ));
            /* 实际执行出口位与实际检测位相或得实际执行的出口位 */
            uTemp2 |= uTemp1;
            /* 重新设置实际执行的出口位 */
            pOUT_Relay->ExecuteBits = uTemp2;
            /* 置执行检测标志 */
            this->CheckExecute = 1;
        }
        else
        {
            /* 将出口动作位赋给实际执行位 */
            OUT_PICKUP_TO_EXECUTE;
        }
    }
    else
    {
        /* 将出口动作位赋给实际执行位 */
        OUT_PICKUP_TO_EXECUTE;
    }

    /* 判断开出回路断线检测是否完成 */
    if( this->CheckNumber >= OUT_MAX_CHECK_RELAY )
    {
        /* 开出回路断线一次检测完成 */
        this->CheckNumber = 0; /* 清检测路数计数器 */
        this->CheckReady = 0;  /* 清准备检测标志 */
        this->CheckExecute = 0;/* 清执行检测标志 */
    }

    /* 清锁出口检测标志 */
    this->LockCheck = 0;

    /* 调用出口执行函数 */
    OUTC_DriveFunction();
}

/* 出口执行函数 */
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
    /* 获取开出继电器数据指针 */
    pOUT_Relay = this->pRelayBits;
    /* 判断是否闭锁出口 */
    if( this->ErrorFlag )
    {
        /* 开出回路击穿,闭锁全部检测位 */
        for( i = 0; i < this->TotalGroupNum; i ++)
        {
            /* 更新实际执行位 */
            ( pOUT_Relay + i )->ExecuteBits &= (~( pOUT_Relay + i )->CheckBits );
        }
    }
    else
    {
        /* 判断是否有闭锁出口标志 */
        if( this->CloseRelay )
        {
            for( i = 0; i < this->TotalGroupNum; i ++)
            {
                /* 电源出错,保持已经动作的出口 */
                uBits = (( pOUT_Relay + i )->CheckBits & ( pOUT_Relay + i )->HoldBits );
                /* 更新实际执行位 */
                ( pOUT_Relay + i )->ExecuteBits &= (~(( pOUT_Relay + i )->CheckBits & (~ uBits )));
            }
            if( this->HoldStartup == this->StartupConst )
            {
                /* 启动继电器动作 */
                StartFlag = this->StartupConst;
            }
        }
        else
        {
            if( this->StartUp == this->StartupConst )
            {
                /* 启动继电器动作 */
                StartFlag = this->StartupConst;
            }
        }
    }

    if( StartFlag == this->StartupConst )
    {
        /* 启动继电器动作 */
        for( i = 0; i < this->TotalGroupNum; i ++ )
        {
            buffer[i] = this->StartupConst;
        }
        OUT_CHECK_RELAY_START( 0, this->iStartUpGroup, buffer );
    }
    else
    {
        /* 启动继电器返回 */
        for( i = 0; i < this->TotalGroupNum; i ++ )
        {
            buffer[i] = 0;
        }
        OUT_CHECK_RELAY_START( 0, this->iStartUpGroup, buffer );
    }

    /* 继电器驱动 */
    for( i = 0; i < this->TotalGroupNum; i ++)
    {
        COUTC_RelayDriver( i,(( pOUT_Relay + i )->ExecuteBits ));
    }

    /* 发送GOOSE开出 */
    if( this->SendGooseFun != NU_NULL )
    {
        this->SendGooseFun();
    }
}

/* 出口变位内部事件记录 */
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

    /* 初始化数据缓冲区 */
    if( iInitFlag == 0 )
    {
        iInitFlag = 1;
        /* 清除数据缓冲区 */
        memset( RelayBit, 0,( sizeof( UNSIGNED )* ( CPC_MAX_OUTPUT_GROUPS ) ));
    }
    /* 清除数据缓冲区 */
    memset( uTempRelayBit, 0,( sizeof( UNSIGNED )* CPC_MAX_OUTPUT_GROUPS ));

    /* 获取开出继电器数据指针 */
    pOUT_Relay = this->pRelayBits;

    if(( OUT_Time_Get_Clock() - PickUpCheckDelay )> OUT_PICKUP_CHECKDELAY )
    {
        /* 更新出口变位延时时间 */
        PickUpCheckDelay = OUT_Time_Get_Clock();

        OUTC_CheckProtect();
        for( i = 0; i < ( this->TotalGroupNum ); i ++)
        {
            uTempRelayBit[i] = ( pOUT_Relay + i )->PickupBits;
        }
        uTempStartUp = this->StartUp;                    /* 启动继电器状态 */
        OUTC_CheckUnProtect();

        if( uTempStartUp != StartFlag )
        {
            iChangeFlag = 1;
        }
        else
        {
            /* 判断出口位是否发生变化 */
            for( i = 0; i < ( this->TotalGroupNum ); i ++)
            {
                if(( uTempRelayBit[i] & (( pOUT_Relay + i )->ChangeRegBits )) != RelayBit[i] )
                {
                    iChangeFlag = 1;
                    break;
                }
            }
        }
        /* 出口变位信息记录 */
        if( iChangeFlag == 1 )
        {
            StartFlag = uTempStartUp;
            for( i = 0; i < ( this->TotalGroupNum ); i ++)
            {
                RelayBit[i] = ( uTempRelayBit[i] & (( pOUT_Relay + i )->ChangeRegBits ));
            }
            if( uTempStartUp == this->StartupConst )
            {
                /* 启动继电器动作 */
                uStartFlag = 1;
            }
            else
            {
                /* 启动继电器返回 */
                uStartFlag = 0;
            }
            if( this->FillEvent != NU_NULL )
            {
                sprintf( EventName, " %03d:%08lX-%08lX-%04X-%01d",
                    LS_RES_EVENT_RELAY, uTempRelayBit[0], uTempRelayBit[1], uTempRelayBit[2], uStartFlag );/* 填写出口变化记录 */
                this->FillEvent( EventName, OUTC_CHECK_RESERVE_EVENT_ID );
            }
        }
    }
}

/* 设置开出量状态变化记录的屏蔽位函数 */
INT OUTC_CheckSetOutLogMaskBits( INT iGroupNum, UNSIGNED MaskBits )
{
    OUT_OCB         * this;
    OUT_RELAY       * pOUT_Relay;
    INT             iStatus = NU_UNAVAILABLE;
    /*--------------------------------------------------------------------------*/
    this = & OUT_Channel;
    /* 获取开出继电器数据指针 */
    pOUT_Relay = this->pRelayBits;
    if( iGroupNum < ( this->TotalGroupNum ) )
    {
        ( pOUT_Relay + iGroupNum ) -> ChangeRegBits = MaskBits;
        iStatus = NU_SUCCESS;
    }
    return( iStatus );
}

/* 扩展回路检测函数 */
static VOID OUTC_OutEX_Check( VOID )
{
    static INT      iExpErrFlag = 0;
    OUT_OCB         * this;
    /*--------------------------------------------------------------------------*/
    this = & OUT_Channel;
    /* 使用扩展开出,自检扩展开出工作是否正常 */
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

/* 扩展插件自检函数(NU_TRUE:成功/NU_FALSE:失败) */
static INT OUTC_ExportPort_Check( VOID )
{
    UNSIGNED uStatus[CPC_MAX_OUTPUT_GROUPS];
    INT      i;
    OUT_OCB  * this;
    INT      iStatus = NU_TRUE;
    /*==================================*/
    this = & OUT_Channel;
    memset( uStatus, 0, sizeof( uStatus ));
    /* 开出插件回读驱动 */
    if( DEVI_PlugReadbackDriver( 0, this->iHardExtendCount, uStatus ) == XJ_SUCCESS )
    {
        /* 判断回读状态是否正确 */
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

/* 获取开出自检回读状态 */
static INT OUT_OutputCheckPortStatus( INT iGroup, UNSIGNED * uStatus )
{
    /*==================================*/
    /* 开出回路回读驱动 */
    DEVI_OutReadbackDriver( iGroup, 1, uStatus );

    return( NU_SUCCESS );
}

/* 设置闭锁出口标志(iFlag:1置闭锁出口标志,iFlag:0清闭锁出口标志) */
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

/* 获取闭锁出口标志(iFlag:1闭锁出口,iFlag:0不闭锁出口) */
INT OUT_GetClockRelayFlag( INT * iFlag )
{
    * iFlag = OUT_Channel.CloseRelay;
    return( NU_SUCCESS );
}

/* 获取出口位状态 */
UNSIGNED OUT_GetOutPickupBits( INT GroupNo )
{
    OUT_OCB         * this;
    OUT_RELAY       * pOUT_Relay;
    INT             iStatus = NU_UNAVAILABLE;
    UNSIGNED        uStatus = 0;
    /*--------------------------------------------------------------------------*/
    this = & OUT_Channel;
    /* 获取开出继电器数据指针 */
    pOUT_Relay = this->pRelayBits;
    if( GroupNo < ( this->TotalGroupNum ) )
    {
        uStatus = ( pOUT_Relay + GroupNo ) -> PickupBits;
    }
    return( uStatus );
}

/* 硬开出检测函数 */
static VOID OUTC_HardOutCheckMain( VOID )
{
    OUT_OCB         * this;
    /*--------------------------------------------------------------------------*/
    this = & OUT_Channel;

    /* 继电器开出回路断线检测函数 */
    this->OutBk_Check();
    /* 开出回路断线事件记录 */
    this->OutBk_EventFill();
    /* 开出回路击穿事件记录 */
    this->OutJC_EventFill();
}

/* 注册硬开出检测函数 */
static VOID OUTC_RegHardOutCheck( VOID (* HardOutCheck )( VOID ))
{
    OUT_Channel.HardOutCheckMain = HardOutCheck;
}

/* 注册扩展开出检测函数 */
static VOID OUTC_RegEXOutCheck( VOID (* EXOutCheck )( VOID ))
{
    OUT_Channel.EXOutCheckMain = EXOutCheck;
}

/* 获取开出组数 */
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
/* 软出口执行位变化检查函数 */
INT OUTC_SoftOutExBitsCheck( VOID )
{
    static UNSIGNED LastOutQ[CPC_MAX_OUTPUT_GROUPS];       /* 上次记录的开出品质因素有效无效位的状态 */
    static UNSIGNED LastOutQJ[CPC_MAX_OUTPUT_GROUPS];      /* 上次记录的开出品质因素检修位的状态 */
    static UNSIGNED LastOutStatus[CPC_MAX_OUTPUT_GROUPS];  /* 上次出口位状态 */
    static INT iInitFlag = 0;
    UNSIGNED NewOutQ[CPC_MAX_OUTPUT_GROUPS];        /* 最新记录的开出品质因素有效无效位的状态 */
    UNSIGNED NewOutQJ[CPC_MAX_OUTPUT_GROUPS];       /* 最新记录的开出品质因素检修位的状态 */
    UNSIGNED NewOutStatus[CPC_MAX_OUTPUT_GROUPS];   /* 最新记录的出口位状态 */
    INT    iStatus = NU_FALSE;
    INT    i;
    static INT iSoftCount = 0;
    CHAR   LogName[OUTC_LOG_MAX_LEN];
    CHAR   TempBuf[30];   /* 临时缓冲区 */
    static UNSIGNED DevQStatus = 0;             /* 装置状态,传输给NPI插件 */
    /*======================================*/
    if( iInitFlag == 0 )
    {
        /* 清缓冲区 */
        memset( ( VOID *)LastOutQ, 0, sizeof( LastOutQ ));
        memset( ( VOID *)LastOutQJ, 0, sizeof( LastOutQJ ));
        memset( ( VOID *)LastOutStatus, 0, sizeof( LastOutStatus ));
        /* 判断是否有软开出量 */
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
    /* 有软开出的时间进行变位检查 */
    if( iSoftCount )
    {
        /* 获取软开出状态及品质因素 */
        for( i = 0; i < iSoftCount; i ++)
        {
            COUTC_ReadSoftOutTotalQua(( OUT_Channel.HardGroupNumber + i ),& NewOutStatus[i],& NewOutQ[i],& NewOutQJ[i] );
        }

        /* 判断品质因素是否发生变化 */
        for( i = 0; i < iSoftCount; i ++)
        {
            /* 有效无效位 */
            if( LastOutQ[i] != NewOutQ[i] )
            {
                iStatus = NU_TRUE;
                break;
            }
            /* 检修位 */
            if( LastOutQJ[i] != NewOutQJ[i] )
            {
                iStatus = NU_TRUE;
                break;
            }
            /* 开出状态 */
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
        /* 判断状态是否有变化 */
        if( iStatus == NU_TRUE )
        {
            sprintf( LogName, "%s-DevST:%d", OUTC_OUT_LOG_NAME, DevQStatus );
            for( i = 0; i < iSoftCount; i ++)
            {
                sprintf( TempBuf, " %d-%08lX:%08lX:%08lX", i, NewOutStatus[i], NewOutQ[i], NewOutQJ[i] );
                strcat( LogName, TempBuf );

                /* 每5个一组,写一条日志 */
                if ( i % 5 == 4 )
                {
                    CLOG_NoProtectLog( ( CHAR *)LogName );
                    /*字符串缓冲区清零*/
                    memset( LogName, 0, sizeof( LogName ) );
                    sprintf( LogName, "%s-DevST:%d", OUTC_OUT_LOG_NAME, DevQStatus );
                }
                else if ( i == ( iSoftCount - 1 ) )
                {
                    CLOG_NoProtectLog( ( CHAR *)LogName );
                }
            }
        }

        /* 更新品质因素及状态 */
        memcpy( LastOutStatus, NewOutStatus, sizeof( UNSIGNED ) * iSoftCount );
        memcpy( LastOutQ, NewOutQ, sizeof( UNSIGNED ) * iSoftCount );
        memcpy( LastOutQJ, NewOutQJ, sizeof( UNSIGNED ) * iSoftCount );
    }

    return( iStatus );
}

/*================================================*/
