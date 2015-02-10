/***********************************
* Include Files
***********************************/
/* ����ͬ��GPS�жϴ��� */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "plat/es_plat.h"
#include "plat/inc/devi_extr.h"
#include "plat/inc/csmv_extr.h"
/*==================================================*/
/* ע:��С��λΪ���� */
/*==================================================*/
#define FIBER_PULSE_WIDTH_COUNT    100      /* ���ݸ��� */
#define FIBER_GPS_PPS_ERROR_VALUE  500000   /* ���������ֵ(500΢��) */
#define FIBER_GPS_PPS_ERROR_VALUE1 50000    /* ���������ֵ(50΢��) */
#define FIBER_GPS_PPS_TIMEOUT      10000    /* �����峬ʱ���ʱ��(10��) */
#define FIBER_GPS_PPS_CYC_COUNT    50       /* ÿ���ӵ�������(һ���ܲ�20ms) */
#define FIBER_GPS_S_TO_NS   (1000 * 1000 * 1000)   /* ��������Ļ��㵥λ */
#define FIBER_GPS_MS_TO_NS  ( 1000 * 1000)         /* ����������Ļ��㵥λ */
#define FIBER_GPS_US_TO_NS         1000            /* ΢��������Ļ��㵥λ */
#define FIBER_GPS_AFFIRM_NUMBER    2               /* ����ȷ�ϴ��� */
#define FIBER_GPS_TOTAL_NUMBER     16              /* N����������ƽ��ֵ */
#define FIBER_GPS_INIT_FLAG        0x5A            /* ��ʼ����ʶ */
#define FIBER_GPS_OFFSET_BASE      96096              /* ƫ�ƻ��� */
#define FIBER_GPS_BASE_COEF        1000               /* ����ϵ�� */
#define FIBER_TIME_STAMP_COEF_US   16                 /* ʱ�����΢�������ϵ�� */
#define FIBER_ADJUST_POSITIVE      1                  /* �������� */
#define FIBER_ADJUST_NEGATIVE      2                  /* ����Ϊ���� */
extern  volatile UNSIGNED TMD_System_Clock;
/*==================================================*/
/* �ⲿ�������� */
extern VOID (* Fiber_Transmit_Interrupt )( VOID );  /* NPU816�й��˷������ݴ����� */
#ifndef WIN32
extern long long Get_system_clock_nanosecond( VOID );
#endif
/*==================================================*/
/* �������� */
/* ��ʼ������ͬ���������������Ϣ;iSamplePoint:ÿ�ܲ��������� */
INT FIBER_GPS_Initiliaze( INT iSamplePoint );
/* ͬ�������жϹ����� */
VOID FIBER_GPS_IntManage( INT iFlag );
/* ��ʱ���жϹ����� */
VOID FIBER_Timer_IntManage( VOID );
/* ���ģ������Ʒ�����ص�������ƫ��λ�� */
VOID FIBER_CheckAnalogQuality( VOID );
/* ��ȡ���˷�������ƫ��λ�� */
UNSIGNED FIBER_GetFiberSendOffset( VOID );
/* ע�����ù���ͨѶ������ź��� */
VOID FIBER_RegSetSendSerial( VOID (* Fiber_SetSendSerial )( UNSIGNED uSendSerial ) );
/* ���÷������ڵ������� */
INT FIBER_SetSendAdjustStep( UNSIGNED uAdjustStepNs );
/* ����PPS��� */
INT FIBER_SetWidth_PPS( short int Value );
/*==================================================*/
/* ���ù���ͨѶ������ź��� */
VOID (* FIBER_Set_Send_Serial )( UNSIGNED uSendSerial );
/* ���˷���ƫ��λ�� */
UNSIGNED FIBER_SendOffset = 0;
/*==================================================*/
/* �����ж���������Ϣ���� */
typedef struct FIBER_GPS_Class_struct
{
    /*private: */
    /* �����־ */
    INT      iPulseFlag;
    /* ������������� */
    UNSIGNED uPulseNumber;
    /* �����������������(������ʧ������) */
    UNSIGNED uTotalPulseNumber;
    /* ���������ֵ */
    UNSIGNED uPPSErrValueNs;
    /* ÿ���Ӷ�ʱ���жϸ��� */
    UNSIGNED uTimerIntTotalCount;
    /* ���峬ʱ���ʱ�� */
    UNSIGNED uPulseCheckTimeUs;
    /* ���ڶ�ʱ���ı�׼ֵ */
    UNSIGNED uBaseValueNs;
    /* ���ڶ�ʱ���ı�׼ֵ��1/2 */
    UNSIGNED uBaseValueNs2_1;
    /* timer0���ڼ������ĵ���ֵ */
    UNSIGNED uAdjustValueNs;
    /* ������־(��ȡ����ֵ:1,����ȡ��׼ֵ:0) */
    UNSIGNED uAdjustFlag;
    /* ��������ʶ */
    UNSIGNED iSerlClearFlag;
    /* PPS��� */
    UNSIGNED uPPSSpaceNs;
    /* �ϴ�PPS��� */
    UNSIGNED uPPSLastSpaceNs;
    /* ʱ�����ȡ����ʵʱ���� */
    UNSIGNED uStampPPSSpaceNs;
    /* ��ʼtick���ֵ */
    UNSIGNED uStartTickMaxNs;
    /* ���ֵʱ��ķ��� */
    UNSIGNED uStartTickMaxSign;
    /* ��ʼtickʵʱֵ */
    UNSIGNED uStartTickRealNs;
    /* ������� */
    UNSIGNED uTransSerial;
    /* ����������ֵ */
    UNSIGNED uTransSerialMax;
    /* ������ʱ,������Ų�Ϊ0,(max-1)������ */
    UNSIGNED uTransSerialErr;
    /* ������ʱ,������Ų�Ϊ0,(max-1)ʱ,�������ֵ */
    UNSIGNED uTransSerialErrSerial;
    /* �������ʵʱֵ */
    UNSIGNED uRealErrValue;
    /* ������ֵ���� */
    UNSIGNED uErrErrCount;
    /* �ﲻ��999500�ĸ��� */
    UNSIGNED uErrErrMinCount;
    /* ppsʱsampleoffset */
    UNSIGNED uSampleOff;
    /* ppsʱFiberSendoff */
    UNSIGNED uFiberSendoff;
    /* ����ƫ��ֵ */
    UNSIGNED uOffBaseValue;
    /* ʵʱƫ��ֵ */
    UNSIGNED uOffRealValue;
    /* PPS����ʱ��ȡ����ʱ���Last */
    UNSIGNED uLastPPSTimeStamp;
    /* PPS����ʱ��ȡ����ʱ���New */
    UNSIGNED uNewPPSTimeStamp;
    /* ����ƫ�����ϴ���ͬ�ĸ��� */
    UNSIGNED uSendOffsetEqualLast;
    /* ����Ƿ����������� */
    UNSIGNED uSerialErr;
    /* Gtime��ʱ���жϼ�� */
    UNSIGNED uGtimeSpaceNs;
    /* Gtime��ʱ���жϼ�����ֵ */
    UNSIGNED uGtimeSpaceMaxNs;
    /* �ϴη������ */
    UNSIGNED uLastSerial;
    /* �������� */
    INT      iAdjustSign;
    /* �������� */
    UNSIGNED uAdjustStep;
    /* ��������(Ӧ��ʹ��) */
    UNSIGNED uAdjustStep1;
    /* ���������� */
    short int iPulseWidth;
    /* �����Ȳ���ȷ�ĸ���(�ض�����ȷ) */
    UNSIGNED  uPulseBackErr;
    /* ����ض������������(0:����ֵ,1:�ض�ֵ) */
    INT       iPulseWidthErrData[2];
    /* �����ϴ�PPS��������� */
    INT       iLastPulseWidthNs;
    /* ʵ�ʻ�ȡ����PPS�������������ֵ��������� */
    UNSIGNED  uPulseRealErr;
    /* ����� */
    UNSIGNED  uPulseWidthErrValueBaseNs;
    /* �������� */
    INT       iPulseWidthErrValueNs;
    /* ������ */
    INT       iPulseWidthBuf[FIBER_PULSE_WIDTH_COUNT];
#ifndef WIN32
    /* PPS�ж�ʱ��ϵͳtick */
    long long  uPPSSystemTick;
#else
    /* PPS�ж�ʱ��ϵͳtick */
    UNSIGNED   uPPSSystemTick;
#endif
    /* ��PPS�Ƴٵ�GTIME�жϸ��� */
    UNSIGNED uPPSDeferGtimeCount;
}FIBER_GPS_INFO;

/*==================================================*/
FIBER_GPS_INFO  FiberGpsInfo;
/*==================================================*/
/* ��ʼ������ͬ���������������Ϣ;iSamplePoint:ÿ�ܲ��������� */
INT FIBER_GPS_Initiliaze( INT iSamplePoint )
{
    /*------------------------------------------*/
    /* �����������Ϣ������ */
    memset( ( void *)& FiberGpsInfo, 0, sizeof( FIBER_GPS_INFO ));
    /* ���������ֵ */
    FiberGpsInfo.uPPSErrValueNs = FIBER_GPS_PPS_ERROR_VALUE;
    /* ���峬ʱ���ʱ�� */
    FiberGpsInfo.uPulseCheckTimeUs = FIBER_GPS_PPS_TIMEOUT;
    /* ÿ���Ӷ�ʱ���жϸ��� */
    FiberGpsInfo.uTimerIntTotalCount = FIBER_GPS_PPS_CYC_COUNT * iSamplePoint;
    /* ���ڶ�ʱ���ı�׼ֵ */
    FiberGpsInfo.uBaseValueNs = FIBER_GPS_S_TO_NS / ( FiberGpsInfo.uTimerIntTotalCount );
    /* ���ڶ�ʱ���ı�׼ֵ��1/2 */
    FiberGpsInfo.uBaseValueNs2_1 = ( FiberGpsInfo.uBaseValueNs )/ 2;
    /* ����������ֵ */
    FiberGpsInfo.uTransSerialMax = FIBER_GPS_PPS_CYC_COUNT * iSamplePoint - 1;
    /* ����� */
    FiberGpsInfo.uPulseWidthErrValueBaseNs = FIBER_GPS_PPS_ERROR_VALUE1;
    return( NU_SUCCESS );
}

/*==================================================*/
/* ͬ�������жϹ����� */
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
    /* ���ж� */
    old_level = NU_Local_Control_Interrupts( 0 );
    this = & FiberGpsInfo;
    /* ��ȡGtime��ʱ��CTֵ */
    DEVI_ReadGTimeCnr1(& this->uStartTickRealNs );/* ��ȡ����ʱ��ʱ�� */
    DEVI_GetPPSTimeStamp( & this->uNewPPSTimeStamp );
    /* ��ȡʱ������ */
    fTimeStampSpace = ( this->uNewPPSTimeStamp - this->uLastPPSTimeStamp )/ FIBER_TIME_STAMP_COEF_US * FIBER_GPS_US_TO_NS;
    /* ��ȡ���������� */
    iPulseWidth = FiberGpsInfo.iPulseWidth;
    iPulseWidthNs = iPulseWidth;
    this->iPulseWidthBuf[this->uPulseNumber % FIBER_PULSE_WIDTH_COUNT] = iPulseWidthNs;
    iPulseWidthNs *= FIBER_GPS_BASE_COEF;
    uSpaceNs = FIBER_GPS_S_TO_NS + iPulseWidthNs;
    /* �ϴ�PPS��� */
    this-> uPPSLastSpaceNs = this-> uPPSSpaceNs;
    /* ��¼PPS��� */
    this-> uPPSSpaceNs = uSpaceNs;
    /* ��¼��ǰϵͳtick */
    this-> uPPSSystemTick = Get_system_clock_nanosecond();
    /* ��¼��ʱ�����ȡ��PPS��� */
    this->uStampPPSSpaceNs = ( UNSIGNED )fTimeStampSpace;
    /* �ж��������Ƿ�������Χ֮�� */
    if ( this->uStampPPSSpaceNs >= ( FIBER_GPS_S_TO_NS - this->uPPSErrValueNs ))
    {
        /* ����ʱ�� */
        this->uLastPPSTimeStamp = this->uNewPPSTimeStamp;
        /* ��ȡ���ֵ */
        uErrNs = abs( FIBER_GPS_S_TO_NS - uSpaceNs );
        this->uRealErrValue = uErrNs;
        /* �ж�����Ƿ�������Χ֮�� */
        if( uErrNs <= this->uPPSErrValueNs )
        {
            /* PPS�ж�ʱ��¼��������ƫ��λ�� */
            this->uSampleOff = ( UNSIGNED )CSMV_GET_SAMPLE_OFFSET;
            /* PPS�ж�ʱ���߷���ƫ��λ�� */
            this->uFiberSendoff = FIBER_SendOffset;
            /* ������������� */
            this->uPulseNumber ++;
            /* �����������������(������ʧ������) */
            this->uTotalPulseNumber ++;
            /* �ж�ȷ����������Ƿ� */
            if( this->uPulseNumber > FIBER_GPS_AFFIRM_NUMBER )
            {
                /* �ѵ�ȷ���������,���������ʶ */
                this->iPulseFlag = FIBER_GPS_INIT_FLAG;
            }

            /* �ж�������Ϣ�Ƿ���� */
            if( this->iPulseFlag == FIBER_GPS_INIT_FLAG )
            {
                /* �õ�����ʶ */
                this->uAdjustFlag = 1;
                /* �ж�����ʱ�̵ķ�������Ƿ���ȷ */
                if(( this->uTransSerial != this->uTransSerialMax )
                    &&( this->uTransSerial != ( this->uTransSerialMax - 1 )))
                {
                    /* ������ʱ,������Ų�Ϊmax,(max-1)������ */
                    this->uTransSerialErr ++;
                    /* ������ʱ,������Ų�Ϊmax,(max-1)ʱ,�������ֵ */
                    this->uTransSerialErrSerial = this->uTransSerial;
                }

                /* �жϳ��ε���ֵ(�����ڵ�һ����бȽ�) */
                if( this->uStartTickRealNs > this->uBaseValueNs2_1 )
                {
                    /* ��ʱ���ļ�����ֵ�������ڵ�һ�룬���ȵ������ͼ���,������ */
                    this->iSerlClearFlag = 1;
                    if( this->uStartTickRealNs < this->uBaseValueNs )
                    {
                        this->uStartTickRealNs = this->uBaseValueNs - this->uStartTickRealNs;
                        /* �õ�������Ϊ���� */
                        this->iAdjustSign = FIBER_ADJUST_NEGATIVE;
                    }
                    else
                    {
                        this->uStartTickRealNs -= this->uBaseValueNs;
                        /* �õ�������Ϊ���� */
                        this->iAdjustSign = FIBER_ADJUST_POSITIVE;
                    }
                }
                else
                {
                    /* ��ʱ���ļ�����ֵС�����ڵ�һ��,�������Ķ�ʱ���ж���,�������������ͼ��� */
                    this->iSerlClearFlag = 0;
                    /* �õ�������Ϊ���� */
                    this->iAdjustSign = FIBER_ADJUST_POSITIVE;
                }
                /* ��¼�����ʼtick */
                if( this->uStartTickRealNs > this->uStartTickMaxNs )
                {
                    /* ��¼�����ʼtick */
                    this->uStartTickMaxNs = this->uStartTickRealNs;
                    this->uStartTickMaxSign = this->iAdjustSign;
                }
                /* �жϲ���ֵ�Ƿ�������Χ֮�� */
                if(( this->uAdjustStep1 == 0 ) || ( this->uAdjustStep1 > this->uStartTickRealNs ))
                {
                    this->uAdjustStep = this->uStartTickRealNs;
                }
                else
                {
                    /* �õ������� */
                    this->uAdjustStep = this->uAdjustStep1;
                }
                /* �����ֵ */
                this->uAdjustValueNs = 0;
                /* ��������ֵ */
                this->uBaseValueNs = uSpaceNs / this->uTimerIntTotalCount;
                /* ƫ�ƻ�ֵ */
                this->uOffBaseValue = 0;
                /* ƫ��ʵʱֵ */
                this->uOffRealValue = 0;
                /* ��ȡ������ */
                uSpaceNs = ( UNSIGNED )fTimeStampSpace;
                /* �ж������������õ����ֵƫ���Ƿ�������Χ֮�� */
                if( abs( uSpaceNs - ( FIBER_GPS_S_TO_NS + this->iLastPulseWidthNs )) > this->uPulseWidthErrValueBaseNs )
                {
                    /* ������������� */
                    this->uPulseRealErr ++;
                    /* �������� */
                    this->iPulseWidthErrValueNs = uSpaceNs - ( FIBER_GPS_S_TO_NS + this->iLastPulseWidthNs );
                }
            }
            /* ���������� */
            DEVI_SetWidth_PPS( iPulseWidth );
            /* �ض������� */
            DEVI_PPSSetWidthReadBack(& iBackPulseWidth );
            /* �ж������Ƿ����óɹ� */
            if( iPulseWidth != iBackPulseWidth )
            {
                /* ʵ������ֵ */
                this->iPulseWidthErrData[0] = iPulseWidth;
                /* �ض�ֵ */
                this->iPulseWidthErrData[1] = iBackPulseWidth;
                /* ���������ȴ������ */
                this->uPulseBackErr ++;
            }
            /* ��¼�ϴ�����NS�� */
            this->iLastPulseWidthNs = iPulseWidthNs;
        }
        else
        {
            /* ����������� */
            this->uPulseNumber = 0;
            /* �����ʶ���� */
            this->iPulseFlag = 0;
            /* ƫ�ƻ�ֵ */
            this->uOffBaseValue = 0;
            /* ƫ��ʵʱֵ */
            this->uOffRealValue = 0;
            /* ������������� */
            this->uErrErrCount ++;
        }
    }
    else
    {
        /* �����ﲻ��999500�ĸ��� */
        this->uErrErrMinCount ++;
    }
    /* �����ж� */
    NU_Local_Control_Interrupts( old_level );
#endif
}

/*==================================================*/
/* ��ʱ���жϹ����� */
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
    UNSIGNED uGPSpace;  /* Gtime��PPS�жϵ�ʱ��� */
    /*------------------------------------------*/
    /* ���ж� */
    old_level = NU_Local_Control_Interrupts( 0 );
    this = & FiberGpsInfo;
    /* �����������ͬʱ�ж�����Ƿ��ѵ����ֵ */
    if(++ this->uTransSerial > this->uTransSerialMax )
    {
        /* �ѵ����ֵ,���� */
        this->uTransSerial = 0;
    }

    new_time = Get_system_clock_nanosecond();
    /* ��ȡGTIME��� */
    this->uGtimeSpaceNs = new_time - last_time;
    last_time = new_time;
    if( iInitFlag != NU_FALSE )
    {
        /* ��¼���ʱ���� */
        if( this->uGtimeSpaceNs > this->uGtimeSpaceMaxNs )
        {
            this->uGtimeSpaceMaxNs = this->uGtimeSpaceNs;
        }
    }
    iInitFlag = NU_TRUE;
    /* �ж��Ƿ�Ϊ�������,���������ж� */
    if( this->uAdjustFlag )
    {
        /* �������ʶ */
        this->uAdjustFlag = 0;

        /* �ж��Ƿ���������������� */
        if( this->iSerlClearFlag == 1 )
        {
            /* ���������(���ֵ(�ΰҪ��)) */
            this->uTransSerial = this->uTransSerialMax;
        }
        else
        {
            /* ��Ϊ���ֵ�жϵ�ǰ�Ƿ��Ǳ�PPS�Ƴٵ�GTIME */
            if( this->uTransSerial == this->uTransSerialMax )
            {
                /* ��ȡ�ж�ʱ��� */
                uGPSpace = Get_system_clock_nanosecond() - this->uPPSSystemTick;
                /* ��ȡ��ǰCNֵ */
                DEVI_ReadGTimeCnr1(& uCurCnr1 );
                if( uCurCnr1 < uGPSpace )
                {
                    /* ���Ǳ��Ƴٵ�Gtime�������ֱ������ */
                    this->uTransSerial = 0;
                }
                else
                {
                    /* ���Ƴٵ�GTIME������������ֵ,��¼���� */
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
        /* �����������(����ֵΪ����) */
        DEVI_OutputSamplePulse( & uTimeClc );
        /* ����PPS�жϺ�ĵ�һ��T�ж��л�ȡ����ֵ */
        this->uOffBaseValue = uTimeClc;
    }
    else
    {
        /* ���ö�ʱ������(�����������ں���) */
        this->uOffRealValue += this->uOffBaseValue;
        /* ����������� */
        uTimeClc = this->uBaseValueNs;
        uTimeClc += (( this->uOffRealValue / FIBER_GPS_OFFSET_BASE ) * FIBER_GPS_OFFSET_BASE ) / FIBER_GPS_BASE_COEF;
        this->uOffRealValue %= FIBER_GPS_OFFSET_BASE;

        /* �ж϶�����ֵ�Ƿ������ */
        if( this->uAdjustValueNs < this->uStartTickRealNs )
        {
            /* �жϵ���ֵ */
            if(( this->uAdjustValueNs + this->uAdjustStep ) > this->uStartTickRealNs )
            {
                uAdjustValue = this->uStartTickRealNs - this->uAdjustValueNs;
            }
            else
            {
                uAdjustValue = this->uAdjustStep;
            }
            this->uAdjustValueNs += uAdjustValue;
            /* �жϵ������� */
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
        /* �����������(����ֵΪ����,����PPS�жϺ��һ��T�жϵ�ʱ��洢����ֵ) */
        DEVI_OutputSamplePulse( & uTimeClc );
    }

    /* ������������ø�Ӧ�ó���(���÷�����ź���) */
    if( FIBER_Set_Send_Serial )
    {
        (* FIBER_Set_Send_Serial )( this->uTransSerial );
    }
    /* ��������ָ��λ��,������Ч�ŵ�������ָ��λ��,���򲻵��� */
    /* ���ģ������Ʒ�����ص�������ƫ��λ�� */
    FIBER_CheckAnalogQuality();
    /* ʹ�ܹ��˷����ж� */
    if( Fiber_Transmit_Interrupt )
    {
        (* Fiber_Transmit_Interrupt )();
    }

    /* �ж�����Ƿ���ȷ */
    if((( this->uLastSerial + 1 ) % this->uTimerIntTotalCount ) != this->uTransSerial )
    {
        /* ������Ŵ�������� */
        this->uSerialErr ++;
    }

    /* �����ϴη������ */
    this->uLastSerial = this->uTransSerial;
    /* �����ж� */
    NU_Local_Control_Interrupts( old_level );
#endif
}

/*==================================================*/
/* ���ģ������Ʒ�����ص�������ƫ��λ�� */
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
    /* �ж��Ƿ����µ�ģ�������� */
    if( uFiberSendDataOffset != uSampleOffset )
    {
        /* �ж�û�з��������Ƿ���ͬһ���� */
        if( uSampleOffset >= uFiberSendDataOffset )
        {
            /* ��ͬһ���� */
            iCount = uSampleOffset - uFiberSendDataOffset;
            /* ��ȡ����λ�� */
            pDstValue = CSMV_GET_ANALOG_QUA_MEMORY + uFiberSendDataOffset;
            /* ����û�з��͵���Ч����λ�� */
            for( i = 0; i < iCount; i ++)
            {
                /* ��������ƫ�� */
                ++ uFiberSendDataOffset;
                /* �ж������Ƿ���Ч */
                if(* ++ pDstValue != 0 )
                {
                    /* �ҵ���Ч����λ��,���� */
                    uEndOffset = uFiberSendDataOffset;
                    break;
                }
            }
        }
        else
        {
            /* ��߽�����μ�飬�ȼ���ѷ������ݵ����޵����� */
            iCount = CSMV_GET_CHANNEL_POINT - uFiberSendDataOffset - 1;
            /* ��ȡ����λ�� */
            pDstValue = CSMV_GET_ANALOG_QUA_MEMORY + uFiberSendDataOffset;
            /* ����û�з��͵���Ч����λ�� */
            for( i = 0; i < iCount; i ++)
            {
                /* ��������ƫ�� */
                ++ uFiberSendDataOffset;
                /* �ж������Ƿ���Ч */
                if(* ++ pDstValue != 0 )
                {
                    /* �ҵ���Ч����λ��,���� */
                    uEndOffset = uFiberSendDataOffset;
                    iStatus = NU_TRUE;
                    break;
                }
            }

            /* �ж����޴��Ƿ���ҵ�,������û���ҵ������޽��в��� */
            if( iStatus == NU_FALSE )
            {
                /* ������޵���ǰ������֮������� */
                pDstValue = CSMV_GET_ANALOG_QUA_MEMORY;
                uFiberSendDataOffset = 0;
                for( i = 0; i <= ( INT )uSampleOffset; i ++)
                {
                    /* �ж������Ƿ���Ч */
                    if(* pDstValue ++ != 0 )
                    {
                        /* �ҵ���Ч����λ��,���� */
                        uEndOffset = uFiberSendDataOffset;
                        break;
                    }
                    /* ��������ƫ�� */
                    uFiberSendDataOffset ++;
                }
            }
        }
    }
    /* ָ����ǰ��������ƫ��λ�� */
    if( FIBER_SendOffset == uEndOffset )
    {
        /* ��¼����ƫ�����ϴ���ͬ�ĸ��� */
        FiberGpsInfo.uSendOffsetEqualLast ++;
    }
    /* �÷���ƫ�� */
    FIBER_SendOffset = uEndOffset;
}

/*==================================================*/
/* ��ȡ���˷�������ƫ��λ�� */
UNSIGNED FIBER_GetFiberSendOffset( VOID )
{
    return( FIBER_SendOffset );
}

/*==================================================*/
/* ע�����ù���ͨѶ������ź��� */
VOID FIBER_RegSetSendSerial( VOID (* Fiber_SetSendSerial )( UNSIGNED uSendSerial ) )
{
    /* ���ù���ͨѶ������ź��� */
    FIBER_Set_Send_Serial = Fiber_SetSendSerial;
}

/*==================================================*/
/* ���÷������ڵ������� */
INT FIBER_SetSendAdjustStep( UNSIGNED uAdjustStepNs )
{
    FiberGpsInfo.uAdjustStep1 = uAdjustStepNs;

    return( NU_SUCCESS );
}

/*==================================================*/
/* ����PPS��� */
INT FIBER_SetWidth_PPS( short int Value )
{
    FiberGpsInfo.iPulseWidth = Value;
    return( NU_SUCCESS );
}

/*==================================================*/
