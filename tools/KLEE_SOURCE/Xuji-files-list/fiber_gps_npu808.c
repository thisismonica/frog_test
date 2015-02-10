/***********************************
* Include Files
***********************************/
/* 光纤同步GPS中断处理 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "plat/es_plat.h"
#include "plat/inc/devi_extr.h"
#include "plat/inc/csmv_extr.h"
/*==================================================*/
/* 注:最小单位为纳秒 */
/*==================================================*/
#define FIBER_PULSE_WIDTH_COUNT    100      /* 数据个数 */
#define FIBER_GPS_PPS_ERROR_VALUE  500000   /* 秒脉冲误差值(500微秒) */
#define FIBER_GPS_PPS_ERROR_VALUE1 50000    /* 秒脉冲误差值(50微秒) */
#define FIBER_GPS_PPS_TIMEOUT      10000    /* 秒脉冲超时检测时间(10秒) */
#define FIBER_GPS_PPS_CYC_COUNT    50       /* 每秒钟的周期数(一个周波20ms) */
#define FIBER_GPS_S_TO_NS   (1000 * 1000 * 1000)   /* 秒与纳秒的换算单位 */
#define FIBER_GPS_MS_TO_NS  ( 1000 * 1000)         /* 豪秒与纳秒的换算单位 */
#define FIBER_GPS_US_TO_NS         1000            /* 微秒与纳秒的换算单位 */
#define FIBER_GPS_AFFIRM_NUMBER    2               /* 脉冲确认次数 */
#define FIBER_GPS_TOTAL_NUMBER     16              /* N次脉冲间隔的平均值 */
#define FIBER_GPS_INIT_FLAG        0x5A            /* 初始化标识 */
#define FIBER_GPS_OFFSET_BASE      96096              /* 偏移基数 */
#define FIBER_GPS_BASE_COEF        1000               /* 基数系数 */
#define FIBER_TIME_STAMP_COEF_US   16                 /* 时间戳与微秒的折算系数 */
#define FIBER_ADJUST_POSITIVE      1                  /* 调整正数 */
#define FIBER_ADJUST_NEGATIVE      2                  /* 调整为负数 */
extern  volatile UNSIGNED TMD_System_Clock;
/*==================================================*/
/* 外部函数声明 */
extern VOID (* Fiber_Transmit_Interrupt )( VOID );  /* NPU816中光纤发送数据处理函数 */
#ifndef WIN32
extern long long Get_system_clock_nanosecond( VOID );
#endif
/*==================================================*/
/* 函数声明 */
/* 初始化光纤同步脉冲基本参数信息;iSamplePoint:每周波采样点数 */
INT FIBER_GPS_Initiliaze( INT iSamplePoint );
/* 同步脉冲中断管理函数 */
VOID FIBER_GPS_IntManage( INT iFlag );
/* 定时器中断管理函数 */
VOID FIBER_Timer_IntManage( VOID );
/* 检查模拟量的品质因素调整发送偏移位置 */
VOID FIBER_CheckAnalogQuality( VOID );
/* 获取光纤发送数据偏移位置 */
UNSIGNED FIBER_GetFiberSendOffset( VOID );
/* 注册设置光纤通讯发送序号函数 */
VOID FIBER_RegSetSendSerial( VOID (* Fiber_SetSendSerial )( UNSIGNED uSendSerial ) );
/* 设置发送周期调整步长 */
INT FIBER_SetSendAdjustStep( UNSIGNED uAdjustStepNs );
/* 设置PPS宽度 */
INT FIBER_SetWidth_PPS( short int Value );
/*==================================================*/
/* 设置光纤通讯发送序号函数 */
VOID (* FIBER_Set_Send_Serial )( UNSIGNED uSendSerial );
/* 光纤发送偏移位置 */
UNSIGNED FIBER_SendOffset = 0;
/*==================================================*/
/* 脉冲中断中所需信息定义 */
typedef struct FIBER_GPS_Class_struct
{
    /*private: */
    /* 脉冲标志 */
    INT      iPulseFlag;
    /* 脉冲次数计数器 */
    UNSIGNED uPulseNumber;
    /* 脉冲次数计数器总数(脉冲消失不清零) */
    UNSIGNED uTotalPulseNumber;
    /* 秒脉冲误差值 */
    UNSIGNED uPPSErrValueNs;
    /* 每秒钟定时器中断个数 */
    UNSIGNED uTimerIntTotalCount;
    /* 脉冲超时检测时间 */
    UNSIGNED uPulseCheckTimeUs;
    /* 周期定时器的标准值 */
    UNSIGNED uBaseValueNs;
    /* 周期定时器的标准值的1/2 */
    UNSIGNED uBaseValueNs2_1;
    /* timer0周期计数器的调整值 */
    UNSIGNED uAdjustValueNs;
    /* 调整标志(是取调整值:1,还是取标准值:0) */
    UNSIGNED uAdjustFlag;
    /* 序号清零标识 */
    UNSIGNED iSerlClearFlag;
    /* PPS间隔 */
    UNSIGNED uPPSSpaceNs;
    /* 上次PPS间隔 */
    UNSIGNED uPPSLastSpaceNs;
    /* 时间戳读取到的实时数据 */
    UNSIGNED uStampPPSSpaceNs;
    /* 起始tick最大值 */
    UNSIGNED uStartTickMaxNs;
    /* 最大值时候的符号 */
    UNSIGNED uStartTickMaxSign;
    /* 起始tick实时值 */
    UNSIGNED uStartTickRealNs;
    /* 发送序号 */
    UNSIGNED uTransSerial;
    /* 发送序号最大值 */
    UNSIGNED uTransSerialMax;
    /* 脉冲来时,发送序号不为0,(max-1)计数器 */
    UNSIGNED uTransSerialErr;
    /* 脉冲来时,发送序号不为0,(max-1)时,发送序号值 */
    UNSIGNED uTransSerialErrSerial;
    /* 脉冲误差实时值 */
    UNSIGNED uRealErrValue;
    /* 脉冲误差超值个数 */
    UNSIGNED uErrErrCount;
    /* 达不到999500的个数 */
    UNSIGNED uErrErrMinCount;
    /* pps时sampleoffset */
    UNSIGNED uSampleOff;
    /* pps时FiberSendoff */
    UNSIGNED uFiberSendoff;
    /* 基础偏移值 */
    UNSIGNED uOffBaseValue;
    /* 实时偏移值 */
    UNSIGNED uOffRealValue;
    /* PPS脉冲时读取到的时间戳Last */
    UNSIGNED uLastPPSTimeStamp;
    /* PPS脉冲时读取到的时间戳New */
    UNSIGNED uNewPPSTimeStamp;
    /* 发送偏移与上次相同的个数 */
    UNSIGNED uSendOffsetEqualLast;
    /* 序号是否连续计数器 */
    UNSIGNED uSerialErr;
    /* Gtime定时器中断间隔 */
    UNSIGNED uGtimeSpaceNs;
    /* Gtime定时器中断间隔最大值 */
    UNSIGNED uGtimeSpaceMaxNs;
    /* 上次发送序号 */
    UNSIGNED uLastSerial;
    /* 调整符号 */
    INT      iAdjustSign;
    /* 调整步长 */
    UNSIGNED uAdjustStep;
    /* 调整步长(应用使用) */
    UNSIGNED uAdjustStep1;
    /* 调整脉冲宽度 */
    short int iPulseWidth;
    /* 脉冲宽度不正确的个数(回读不正确) */
    UNSIGNED  uPulseBackErr;
    /* 脉宽回读错误错误数据(0:设置值,1:回读值) */
    INT       iPulseWidthErrData[2];
    /* 设置上次PPS误差纳秒数 */
    INT       iLastPulseWidthNs;
    /* 实际获取到的PPS宽度误差大于设置值错误计数器 */
    UNSIGNED  uPulseRealErr;
    /* 误差宽度 */
    UNSIGNED  uPulseWidthErrValueBaseNs;
    /* 错误数据 */
    INT       iPulseWidthErrValueNs;
    /* 误差缓冲区 */
    INT       iPulseWidthBuf[FIBER_PULSE_WIDTH_COUNT];
#ifndef WIN32
    /* PPS中断时的系统tick */
    long long  uPPSSystemTick;
#else
    /* PPS中断时的系统tick */
    UNSIGNED   uPPSSystemTick;
#endif
    /* 被PPS推迟的GTIME中断个数 */
    UNSIGNED uPPSDeferGtimeCount;
}FIBER_GPS_INFO;

/*==================================================*/
FIBER_GPS_INFO  FiberGpsInfo;
/*==================================================*/
/* 初始化光纤同步脉冲基本参数信息;iSamplePoint:每周波采样点数 */
INT FIBER_GPS_Initiliaze( INT iSamplePoint )
{
    /*------------------------------------------*/
    /* 清基本数据信息缓冲区 */
    memset( ( void *)& FiberGpsInfo, 0, sizeof( FIBER_GPS_INFO ));
    /* 秒脉冲误差值 */
    FiberGpsInfo.uPPSErrValueNs = FIBER_GPS_PPS_ERROR_VALUE;
    /* 脉冲超时检测时间 */
    FiberGpsInfo.uPulseCheckTimeUs = FIBER_GPS_PPS_TIMEOUT;
    /* 每秒钟定时器中断个数 */
    FiberGpsInfo.uTimerIntTotalCount = FIBER_GPS_PPS_CYC_COUNT * iSamplePoint;
    /* 周期定时器的标准值 */
    FiberGpsInfo.uBaseValueNs = FIBER_GPS_S_TO_NS / ( FiberGpsInfo.uTimerIntTotalCount );
    /* 周期定时器的标准值的1/2 */
    FiberGpsInfo.uBaseValueNs2_1 = ( FiberGpsInfo.uBaseValueNs )/ 2;
    /* 发送序号最大值 */
    FiberGpsInfo.uTransSerialMax = FIBER_GPS_PPS_CYC_COUNT * iSamplePoint - 1;
    /* 误差宽度 */
    FiberGpsInfo.uPulseWidthErrValueBaseNs = FIBER_GPS_PPS_ERROR_VALUE1;
    return( NU_SUCCESS );
}

/*==================================================*/
/* 同步脉冲中断管理函数 */
VOID FIBER_GPS_IntManage( INT iFlag )
{
#ifndef WIN32
    FIBER_GPS_INFO   * this;
    UNSIGNED         uSpaceNs;
    UNSIGNED         uErrNs;
    INT              old_level;
    float            fTimeStampSpace;
    short int        iPulseWidth;
    INT              iPulseWidthNs;
    INT              iBackPulseWidth;
    /*------------------------------------------*/
    /* 锁中断 */
    old_level = NU_Local_Control_Interrupts( 0 );
    this = & FiberGpsInfo;
    /* 读取Gtime定时器CT值 */
    DEVI_ReadGTimeCnr1(& this->uStartTickRealNs );/* 获取脉冲时刻时间 */
    DEVI_GetPPSTimeStamp( & this->uNewPPSTimeStamp );
    /* 获取时间戳间隔 */
    fTimeStampSpace = ( this->uNewPPSTimeStamp - this->uLastPPSTimeStamp )/ FIBER_TIME_STAMP_COEF_US * FIBER_GPS_US_TO_NS;
    /* 获取调整脉冲宽度 */
    iPulseWidth = FiberGpsInfo.iPulseWidth;
    iPulseWidthNs = iPulseWidth;
    this->iPulseWidthBuf[this->uPulseNumber % FIBER_PULSE_WIDTH_COUNT] = iPulseWidthNs;
    iPulseWidthNs *= FIBER_GPS_BASE_COEF;
    uSpaceNs = FIBER_GPS_S_TO_NS + iPulseWidthNs;
    /* 上次PPS间隔 */
    this-> uPPSLastSpaceNs = this-> uPPSSpaceNs;
    /* 记录PPS间隔 */
    this-> uPPSSpaceNs = uSpaceNs;
    /* 记录当前系统tick */
    this-> uPPSSystemTick = Get_system_clock_nanosecond();
    /* 记录从时间戳获取到PPS间隔 */
    this->uStampPPSSpaceNs = ( UNSIGNED )fTimeStampSpace;
    /* 判断脉冲间隔是否在允许范围之内 */
    if ( this->uStampPPSSpaceNs >= ( FIBER_GPS_S_TO_NS - this->uPPSErrValueNs ))
    {
        /* 更新时间 */
        this->uLastPPSTimeStamp = this->uNewPPSTimeStamp;
        /* 获取误差值 */
        uErrNs = abs( FIBER_GPS_S_TO_NS - uSpaceNs );
        this->uRealErrValue = uErrNs;
        /* 判断误差是否在允许范围之内 */
        if( uErrNs <= this->uPPSErrValueNs )
        {
            /* PPS中断时记录采样脉冲偏移位置 */
            this->uSampleOff = ( UNSIGNED )CSMV_GET_SAMPLE_OFFSET;
            /* PPS中断时管线发送偏移位置 */
            this->uFiberSendoff = FIBER_SendOffset;
            /* 脉冲次数计数器 */
            this->uPulseNumber ++;
            /* 脉冲次数计数器总数(脉冲消失不清零) */
            this->uTotalPulseNumber ++;
            /* 判断确认脉冲次数是否够 */
            if( this->uPulseNumber > FIBER_GPS_AFFIRM_NUMBER )
            {
                /* 已到确认脉冲次数,置有脉冲标识 */
                this->iPulseFlag = FIBER_GPS_INIT_FLAG;
            }

            /* 判断数据信息是否可用 */
            if( this->iPulseFlag == FIBER_GPS_INIT_FLAG )
            {
                /* 置调整标识 */
                this->uAdjustFlag = 1;
                /* 判断脉冲时刻的发送序号是否正确 */
                if(( this->uTransSerial != this->uTransSerialMax )
                    &&( this->uTransSerial != ( this->uTransSerialMax - 1 )))
                {
                    /* 脉冲来时,发送序号不为max,(max-1)计数器 */
                    this->uTransSerialErr ++;
                    /* 脉冲来时,发送序号不为max,(max-1)时,发送序号值 */
                    this->uTransSerialErrSerial = this->uTransSerial;
                }

                /* 判断初次调整值(与周期的一半进行比较) */
                if( this->uStartTickRealNs > this->uBaseValueNs2_1 )
                {
                    /* 定时器的计数器值大于周期的一半，则先调整发送计数,再清零 */
                    this->iSerlClearFlag = 1;
                    if( this->uStartTickRealNs < this->uBaseValueNs )
                    {
                        this->uStartTickRealNs = this->uBaseValueNs - this->uStartTickRealNs;
                        /* 置调整符号为负数 */
                        this->iAdjustSign = FIBER_ADJUST_NEGATIVE;
                    }
                    else
                    {
                        this->uStartTickRealNs -= this->uBaseValueNs;
                        /* 置调整符号为正数 */
                        this->iAdjustSign = FIBER_ADJUST_POSITIVE;
                    }
                }
                else
                {
                    /* 定时器的计数器值小于周期的一半,则在随后的定时器中断中,先清零后调整发送计数 */
                    this->iSerlClearFlag = 0;
                    /* 置调整符号为正数 */
                    this->iAdjustSign = FIBER_ADJUST_POSITIVE;
                }
                /* 记录最大起始tick */
                if( this->uStartTickRealNs > this->uStartTickMaxNs )
                {
                    /* 记录最大起始tick */
                    this->uStartTickMaxNs = this->uStartTickRealNs;
                    this->uStartTickMaxSign = this->iAdjustSign;
                }
                /* 判断步长值是否在允许范围之内 */
                if(( this->uAdjustStep1 == 0 ) || ( this->uAdjustStep1 > this->uStartTickRealNs ))
                {
                    this->uAdjustStep = this->uStartTickRealNs;
                }
                else
                {
                    /* 置调整步长 */
                    this->uAdjustStep = this->uAdjustStep1;
                }
                /* 清调整值 */
                this->uAdjustValueNs = 0;
                /* 计算周期值 */
                this->uBaseValueNs = uSpaceNs / this->uTimerIntTotalCount;
                /* 偏移基值 */
                this->uOffBaseValue = 0;
                /* 偏移实时值 */
                this->uOffRealValue = 0;
                /* 获取脉冲宽度 */
                uSpaceNs = ( UNSIGNED )fTimeStampSpace;
                /* 判断脉冲宽度与设置的误差值偏差是否在允许范围之内 */
                if( abs( uSpaceNs - ( FIBER_GPS_S_TO_NS + this->iLastPulseWidthNs )) > this->uPulseWidthErrValueBaseNs )
                {
                    /* 调整错误计数器 */
                    this->uPulseRealErr ++;
                    /* 错误数据 */
                    this->iPulseWidthErrValueNs = uSpaceNs - ( FIBER_GPS_S_TO_NS + this->iLastPulseWidthNs );
                }
            }
            /* 设置脉冲宽度 */
            DEVI_SetWidth_PPS( iPulseWidth );
            /* 回读脉冲宽度 */
            DEVI_PPSSetWidthReadBack(& iBackPulseWidth );
            /* 判断脉宽是否设置成功 */
            if( iPulseWidth != iBackPulseWidth )
            {
                /* 实际设置值 */
                this->iPulseWidthErrData[0] = iPulseWidth;
                /* 回读值 */
                this->iPulseWidthErrData[1] = iBackPulseWidth;
                /* 调整脉冲宽度错误计数 */
                this->uPulseBackErr ++;
            }
            /* 记录上次误差的NS数 */
            this->iLastPulseWidthNs = iPulseWidthNs;
        }
        else
        {
            /* 脉冲个数清零 */
            this->uPulseNumber = 0;
            /* 脉冲标识清零 */
            this->iPulseFlag = 0;
            /* 偏移基值 */
            this->uOffBaseValue = 0;
            /* 偏移实时值 */
            this->uOffRealValue = 0;
            /* 调整错误计数器 */
            this->uErrErrCount ++;
        }
    }
    else
    {
        /* 调整达不到999500的个数 */
        this->uErrErrMinCount ++;
    }
    /* 解锁中断 */
    NU_Local_Control_Interrupts( old_level );
#endif
}

/*==================================================*/
/* 定时器中断管理函数 */
VOID FIBER_Timer_IntManage( VOID )
{
#ifndef WIN32
    FIBER_GPS_INFO   * this;
    UNSIGNED uTimeClc;
    UNSIGNED uTimeClc1;
    INT      old_level;
    UNSIGNED uAdjustValue;
    static long long last_time;
    long long new_time;
    static INT iInitFlag = NU_FALSE;
    UNSIGNED uCurCnr1;
    UNSIGNED uGPSpace;  /* Gtime与PPS中断的时间差 */
    /*------------------------------------------*/
    /* 锁中断 */
    old_level = NU_Local_Control_Interrupts( 0 );
    this = & FiberGpsInfo;
    /* 调整发送序号同时判断序号是否已到最大值 */
    if(++ this->uTransSerial > this->uTransSerialMax )
    {
        /* 已到最大值,清零 */
        this->uTransSerial = 0;
    }

    new_time = Get_system_clock_nanosecond();
    /* 获取GTIME间隔 */
    this->uGtimeSpaceNs = new_time - last_time;
    last_time = new_time;
    if( iInitFlag != NU_FALSE )
    {
        /* 记录最大时间间隔 */
        if( this->uGtimeSpaceNs > this->uGtimeSpaceMaxNs )
        {
            this->uGtimeSpaceMaxNs = this->uGtimeSpaceNs;
        }
    }
    iInitFlag = NU_TRUE;
    /* 判断是否为秒脉冲后,紧随其后的中断 */
    if( this->uAdjustFlag )
    {
        /* 清调整标识 */
        this->uAdjustFlag = 0;

        /* 判断是否立即调整发送序号 */
        if( this->iSerlClearFlag == 1 )
        {
            /* 发送序号置(最大值(李宝伟要求)) */
            this->uTransSerial = this->uTransSerialMax;
        }
        else
        {
            /* 若为最大值判断当前是否是被PPS推迟的GTIME */
            if( this->uTransSerial == this->uTransSerialMax )
            {
                /* 获取中断时间差 */
                uGPSpace = Get_system_clock_nanosecond() - this->uPPSSystemTick;
                /* 读取当前CN值 */
                DEVI_ReadGTimeCnr1(& uCurCnr1 );
                if( uCurCnr1 < uGPSpace )
                {
                    /* 不是被推迟的Gtime发送序号直接清零 */
                    this->uTransSerial = 0;
                }
                else
                {
                    /* 被推迟的GTIME发送序号置最大值,记录次数 */
                    this->uPPSDeferGtimeCount ++;
                }
            }
            else
            {
                this->uTransSerial = 0;
            }
        }
        this->iSerlClearFlag = 0;

        uTimeClc = this->uBaseValueNs;
        uTimeClc1 = uTimeClc;
        /* 采样脉冲输出(返回值为余数) */
        DEVI_OutputSamplePulse( & uTimeClc );
        /* 仅在PPS中断后的第一个T中断中获取此数值 */
        this->uOffBaseValue = uTimeClc;
    }
    else
    {
        /* 设置定时器周期(调用设置周期函数) */
        this->uOffRealValue += this->uOffBaseValue;
        /* 采样脉冲输出 */
        uTimeClc = this->uBaseValueNs;
        uTimeClc += (( this->uOffRealValue / FIBER_GPS_OFFSET_BASE ) * FIBER_GPS_OFFSET_BASE ) / FIBER_GPS_BASE_COEF;
        this->uOffRealValue %= FIBER_GPS_OFFSET_BASE;

        /* 判断对其量值是否调整完 */
        if( this->uAdjustValueNs < this->uStartTickRealNs )
        {
            /* 判断调整值 */
            if(( this->uAdjustValueNs + this->uAdjustStep ) > this->uStartTickRealNs )
            {
                uAdjustValue = this->uStartTickRealNs - this->uAdjustValueNs;
            }
            else
            {
                uAdjustValue = this->uAdjustStep;
            }
            this->uAdjustValueNs += uAdjustValue;
            /* 判断调整符号 */
            if( this->iAdjustSign == FIBER_ADJUST_POSITIVE )
            {
                uTimeClc += uAdjustValue;
            }
            else
            {
                uTimeClc -= uAdjustValue;
            }
        }

        uTimeClc1 = uTimeClc;
        /* 采样脉冲输出(返回值为余数,仅在PPS中断后第一个T中断的时候存储余数值) */
        DEVI_OutputSamplePulse( & uTimeClc );
    }

    /* 将发送序号设置给应用程序(调用发送序号函数) */
    if( FIBER_Set_Send_Serial )
    {
        (* FIBER_Set_Send_Serial )( this->uTransSerial );
    }
    /* 调整发送指针位置,数据有效才调整发送指针位置,否则不调整 */
    /* 检查模拟量的品质因素调整发送偏移位置 */
    FIBER_CheckAnalogQuality();
    /* 使能光纤发送中断 */
    if( Fiber_Transmit_Interrupt )
    {
        (* Fiber_Transmit_Interrupt )();
    }

    /* 判断序号是否正确 */
    if((( this->uLastSerial + 1 ) % this->uTimerIntTotalCount ) != this->uTransSerial )
    {
        /* 调整序号错误计数器 */
        this->uSerialErr ++;
    }

    /* 更新上次发送序号 */
    this->uLastSerial = this->uTransSerial;
    /* 解锁中断 */
    NU_Local_Control_Interrupts( old_level );
#endif
}

/*==================================================*/
/* 检查模拟量的品质因素调整发送偏移位置 */
VOID FIBER_CheckAnalogQuality( VOID )
{
    register UNSIGNED * pDstValue;
    register UNSIGNED uFiberSendDataOffset;
    register UNSIGNED uSampleOffset;
    UNSIGNED uEndOffset;
    INT      i;
    INT      iCount = 0;
    INT      iStatus = NU_FALSE;
    /*------------------------------------------*/
    uSampleOffset = ( UNSIGNED )CSMV_GET_SAMPLE_OFFSET;
    uFiberSendDataOffset = FIBER_SendOffset;
    uEndOffset = uFiberSendDataOffset;
    /* 判断是否有新的模拟量数据 */
    if( uFiberSendDataOffset != uSampleOffset )
    {
        /* 判断没有发送数据是否在同一方向 */
        if( uSampleOffset >= uFiberSendDataOffset )
        {
            /* 在同一区域 */
            iCount = uSampleOffset - uFiberSendDataOffset;
            /* 获取发送位置 */
            pDstValue = CSMV_GET_ANALOG_QUA_MEMORY + uFiberSendDataOffset;
            /* 查找没有发送的有效数据位置 */
            for( i = 0; i < iCount; i ++)
            {
                /* 调整发送偏移 */
                ++ uFiberSendDataOffset;
                /* 判断数据是否有效 */
                if(* ++ pDstValue != 0 )
                {
                    /* 找到有效数据位置,返回 */
                    uEndOffset = uFiberSendDataOffset;
                    break;
                }
            }
        }
        else
        {
            /* 跨边界分两段检查，先检查已发送数据到上限的数据 */
            iCount = CSMV_GET_CHANNEL_POINT - uFiberSendDataOffset - 1;
            /* 获取发送位置 */
            pDstValue = CSMV_GET_ANALOG_QUA_MEMORY + uFiberSendDataOffset;
            /* 查找没有发送的有效数据位置 */
            for( i = 0; i < iCount; i ++)
            {
                /* 调整发送偏移 */
                ++ uFiberSendDataOffset;
                /* 判断数据是否有效 */
                if(* ++ pDstValue != 0 )
                {
                    /* 找到有效数据位置,返回 */
                    uEndOffset = uFiberSendDataOffset;
                    iStatus = NU_TRUE;
                    break;
                }
            }

            /* 判断上限处是否查找到,若上限没有找到在下限进行查找 */
            if( iStatus == NU_FALSE )
            {
                /* 检查下限到当前采样点之间的数据 */
                pDstValue = CSMV_GET_ANALOG_QUA_MEMORY;
                uFiberSendDataOffset = 0;
                for( i = 0; i <= ( INT )uSampleOffset; i ++)
                {
                    /* 判断数据是否有效 */
                    if(* pDstValue ++ != 0 )
                    {
                        /* 找到有效数据位置,返回 */
                        uEndOffset = uFiberSendDataOffset;
                        break;
                    }
                    /* 调整发送偏移 */
                    uFiberSendDataOffset ++;
                }
            }
        }
    }
    /* 指定当前发送数据偏移位置 */
    if( FIBER_SendOffset == uEndOffset )
    {
        /* 记录发送偏移与上次相同的个数 */
        FiberGpsInfo.uSendOffsetEqualLast ++;
    }
    /* 置发送偏移 */
    FIBER_SendOffset = uEndOffset;
}

/*==================================================*/
/* 获取光纤发送数据偏移位置 */
UNSIGNED FIBER_GetFiberSendOffset( VOID )
{
    return( FIBER_SendOffset );
}

/*==================================================*/
/* 注册设置光纤通讯发送序号函数 */
VOID FIBER_RegSetSendSerial( VOID (* Fiber_SetSendSerial )( UNSIGNED uSendSerial ) )
{
    /* 设置光纤通讯发送序号函数 */
    FIBER_Set_Send_Serial = Fiber_SetSendSerial;
}

/*==================================================*/
/* 设置发送周期调整步长 */
INT FIBER_SetSendAdjustStep( UNSIGNED uAdjustStepNs )
{
    FiberGpsInfo.uAdjustStep1 = uAdjustStepNs;

    return( NU_SUCCESS );
}

/*==================================================*/
/* 设置PPS宽度 */
INT FIBER_SetWidth_PPS( short int Value )
{
    FiberGpsInfo.iPulseWidth = Value;
    return( NU_SUCCESS );
}

/*==================================================*/
