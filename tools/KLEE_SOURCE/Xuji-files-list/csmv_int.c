
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
/*      smvc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      SMV - Common Device Driver                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common device       */
/*      driver management component.                                     */
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
/*      ZhiDeYang       11-15-2007      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "plat/inc/xj_cosc.h"
#include "plat/es_plat.h"
#include "plat/inc/pro_defs.h"
#include "plat/inc/csmv_extr.h"
#include "plat_specific/plat_specific.h"
#include "plat/inc/esp_cfg.h"
#include "plat/inc/xj_out_check.h"

#define CVMC_MAX_ANALOG_VALUE_NUMBER    CPC_MAX_PHYSICAL_CHL_NUMBER /* ����ģ����ͨ������ */
#define CVMC_MAX_ANALOG_QUALITY_GROUPS  ((CPC_MAX_PHYSICAL_CHL_NUMBER+31)/32) /* ����ģ����Ʒ��������Ŀ */
#define CVMC_POWER_ERROR_NUMBER         2  /* ��Դ�Լ��������������ֵ */
#define CVMC_SAMPLE_INITIALIZE_POINT    960 /* ������ʼ������ */
#define CSMV_GET_MAX_PROTECT_GROUPS     (CPC_MAX_PROTECT_GROUPS) /* ��󱣻�״̬������ */
#define CSMV_GET_MAX_PROTECT_LOG_GROUPS (CPC_MAX_PROTECT_LOG_GROUPS) /* ��󱣻��߼�״̬������ */
#define CVMC_MAX_SAMPLE_COUNTER         1200 /* ���Ĳ������ */

/* ����ΪOS������� */
#ifdef WIN32
/* ȫ�ֱ������� */
/* Windows �궨�� */
#endif

#ifndef WIN32 /* ��windowsȱʡ����һЩ���Ա�ͨ������ */
/* ȫ�ֱ������� */
/* Nucleus�궨�� */
#endif

extern ESP_CFG PLAT_cfg;
extern CWM_WCB * WavePointer;
extern CWM_WCB * WavePointerCB[];

SMVC_SAMPLE_RECORD SMVC_SampleRecord;
INT SMVC_InitializeFlag; /* �������ݳ�ʼ����ɱ�־ */
INT SMVC_SampleInitFlag = NU_FALSE; /* ����׼����ɱ�ʶ */
UNSIGNED SMVC_SampleCount; /* ���������� */
UNSIGNED SMVC_Power_Error_Counter = 0; /* ��Դ�Լ����������������ؼ������� */
UNSIGNED SMVC_Power_Error_Flag = 0; /* ��Դ�Լ�����־ */
volatile UNSIGNED SMVC_Power_Error_Resume_Counter = 0; /* ��Դ�Լ����ָ������� */
static ESP_CFG_SMVC * pSMVC_AnalogConfig = &( PLAT_cfg.smvc ); /* ģ�������� */
static ESP_CFG_INPUT * pSMVC_InputConfig = &( PLAT_cfg.input ); /* ���������� */
static ESP_CFG_OUTPUT * pSMVC_OutputConfig = &( PLAT_cfg.output ); /* ���������� */
float SMVC_AnalogValueBuf[ CVMC_MAX_ANALOG_VALUE_NUMBER ]; /* ԭʼģ��������ֵ */
UNSIGNED SMVC_AnalogQualityBuf[CVMC_MAX_ANALOG_QUALITY_GROUPS]; /* ģ��������ֵ��Ʒ������ */
UNSIGNED SMVC_AnalogCheckQualityBuf[CVMC_MAX_ANALOG_QUALITY_GROUPS]; /* ģ��������ֵ��Ʒ�����ؼ���λ */
VOID (* SMVC_User_AD_Interrupt )( VOID ) = NU_NULL; /* �û�A/D�жϴ����� */
INT (* SMVC_User_Sample_Conversion )( float *, INT ) = NU_NULL; /* �û�����ת������ */

SMVC_ERR SMVC_Error;

/* Define internal function calls.  */
VOID SMVC_SampleSave( float * sample_data, UNSIGNED * pQualityBuf, UNSIGNED pulse_counter );
INT SMVC_LosePointQMan( INT iLoseCount, UNSIGNED StartLoseSerl ); /* ����ֵ��ʧ��Ʒ�����ش����� */
INT SMVC_LoseOneInsertValue( UNSIGNED pulse_counter ); /* ��ʧһ�����ǰһ������ */

/* ע��A/D�ж��û������� */
VOID SMVC_Register_User_AD_Interrupt( VOID (* User_AD_Interrupt )( VOID ))
{
    SMVC_User_AD_Interrupt = User_AD_Interrupt;
}

/* ע��A/D�жϲ�������ת������ */
VOID SMVC_Register_Sample_Conversion( INT (* User_Sample_Conversion )( float *, INT ))
{
    SMVC_User_Sample_Conversion = User_Sample_Conversion;
}

/* ����Ӳ���������� */
VOID SMVC_SampleUpdateHard( float * sample_data, UNSIGNED pulse_counter )
{
    INT status;
    register INT k;
    register INT Number;
    register INT ChannelPoint;
    register float * pSrcAnalog;
    register float * pDstAnalog;
    PRO_CHANNEL        * pChannel;      /* ͨ������ģ��ָ��,��Чֵ¼��ʱʹ�� */
    ESP_CFG_SMVC_POWER * pPowerChlInfo; /* �Լ��Դͨ����Ϣָ�� */
    float fValue;
    float fPowChlMinBasValue;
    float fPowChlMaxBasValue;
    int PowerCheckErrorFlag;            /* ��Դ�Լ��־ */
    static UNSIGNED PowerErrNumber = 0; /* ��Դ�Լ������������� */
    float buffer[256];                  /* ��ʱ������ */
    UNSIGNED uTotalSerial;

    if( SMVC_User_Sample_Conversion )
    {
        /* �û���������ת������ */
        Number = CSMV_GET_PHY_ANALOG_NUMBER;
        status = SMVC_User_Sample_Conversion( sample_data, Number );
        if( status == NU_FALSE )
        {
            return;
        }
    }

    /* �ж��Ƿ�Ϊ˫����װ�� */
    if( CSMV_GET_DSAMPLE_FLAG )
    {
        /* ģ����ͨ����Ư��ϵ������ */
        pSrcAnalog = sample_data;
        Number = CSMV_GET_PHY_ANALOG_NUMBER - CSMV_GET_DSAMPLE_SV_CHL_NUM;
        pDstAnalog = buffer;
        pChannel = CSMV_GET_CHL_TABLE_MEMORY;
        for( k = 0; k < Number; k ++)
        {
            fValue = * pSrcAnalog;
            /* ��Ư���� */
            fValue -= ( pChannel->chl_channel_excursion );
            /* ϵ������ */
            fValue *= ( pChannel->chl_channel_coefficient );
            * pDstAnalog = fValue;
            pSrcAnalog ++;
            pChannel ++;
            pDstAnalog ++;
        }
        /* �洢SV������Ϣ */
        pSrcAnalog = CSMV_MSG_GetSVDataInfo( & uTotalSerial );
        Number = CSMV_GET_DSAMPLE_SV_CHL_NUM;
        /* �������Ҫ��������16λ,�͵�16λΪӲ��������� */
        uTotalSerial <<= CPC_DATA_BITS16;
        uTotalSerial |= pulse_counter;
        for( k = 0; k < Number; k ++)
        {
            fValue = * pSrcAnalog;
            /* ��Ư���� */
            fValue -= ( pChannel->chl_channel_excursion );
            /* ϵ������ */
            fValue *= ( pChannel->chl_channel_coefficient );
            * pDstAnalog = fValue;
            pSrcAnalog ++;
            pChannel ++;
            pDstAnalog ++;
        }
    }
    else
    {
        /* ģ����ͨ����Ư��ϵ������ */
        pSrcAnalog = sample_data;
        Number = CSMV_GET_PHY_ANALOG_NUMBER;
        pDstAnalog = buffer;
        pChannel = CSMV_GET_CHL_TABLE_MEMORY;
        for( k = 0; k < Number; k ++)
        {
            fValue = * pSrcAnalog;
            /* ��Ư���� */
            fValue -= ( pChannel->chl_channel_excursion );
            /* ϵ������ */
            fValue *= ( pChannel->chl_channel_coefficient );
            * pDstAnalog = fValue;
            pSrcAnalog ++;
            pChannel ++;
            pDstAnalog ++;
        }
        uTotalSerial = pulse_counter;
    }

    /* ��ȡͨ������ģ��ָ�� */
    pChannel = CSMV_GET_CHL_TABLE_MEMORY;
    /* �Լ��Դͨ����Ϣָ�� */
    pPowerChlInfo = CSMV_GET_POWER_CHL_INFO_MEMORY;
    /* ���²������ݵ�ַ */
    pSrcAnalog = buffer;
    /* ���Դ�Լ�����־ */
    PowerCheckErrorFlag = NU_FALSE;
    for( k = 0; k < CSMV_GET_POWER_CHECK_NUMBER; k ++)
    {
        /* ͨ����ǰ����ֵ */
        fValue = *( pSrcAnalog + pPowerChlInfo->ChannelNumber );
        /* ͨ���ֵ����ϵ�� */
        fValue *= (( pChannel + pPowerChlInfo->ChannelNumber )->chl_rate_coefficient );
        fValue = ( float )fabs( fValue );

        /* ͨ������׼ֵ */
        /* ����ֵ */
        fPowChlMinBasValue = pPowerChlInfo->MinValue *
            (( pChannel + pPowerChlInfo->ChannelNumber )->chl_calculate_coefficient );
        /* ����ֵ */
        fPowChlMaxBasValue = pPowerChlInfo->MaxValue *
            (( pChannel + pPowerChlInfo->ChannelNumber )->chl_calculate_coefficient );
        /* �ж��Ƿ�������ķ�Χ֮�� */
        if(( fValue < fPowChlMinBasValue ) || ( fValue > fPowChlMaxBasValue ))
        {
            /* �õ�Դ�Լ�����־ */
            PowerCheckErrorFlag = NU_TRUE;
            break;
        }
        /* ������Դ�Լ���Ϣָ�� */
        pPowerChlInfo ++;
    }
    /* �жϵ�Դ�Լ��־ */
    if( PowerCheckErrorFlag == NU_TRUE )
    {
        if( SMVC_Power_Error_Flag == NU_FALSE )
        {
            /* ������Դ�Լ�������������ֵ */
            PowerErrNumber ++;
            /* �жϵ�Դ�Լ��������Ƿ��ѵ���������ֵ */
            if( PowerErrNumber >= CVMC_POWER_ERROR_NUMBER )
            {
                /* ������Դ�Լ��������� */
                SMVC_Power_Error_Counter ++;
                SMVC_Power_Error_Flag = NU_TRUE; /* �����־ */
            }
        }
    }
    else
    {
        /* ���Դ�Լ������������� */
        PowerErrNumber = 0;
        SMVC_Power_Error_Flag = NU_FALSE;
    }

    if(( PowerCheckErrorFlag == NU_TRUE ) && ( SMVC_Power_Error_Flag == NU_FALSE ))
    {
        /* ��Դһ���������ǰ��������浱ǰ�� */
        pSrcAnalog = CSMV_GET_ANALOG_MEMORY + CSMV_GET_SAMPLE_OFFSET;
        pDstAnalog = buffer;
        Number = CSMV_GET_PHY_ANALOG_NUMBER;
        ChannelPoint = CSMV_GET_CHANNEL_POINT;
        for( k = 0; k < Number; k ++)
        {
            * pDstAnalog = * pSrcAnalog;
            pSrcAnalog += ChannelPoint;
            pDstAnalog ++;
        }
    }
    /* ����������� */
    SMVC_SampleSave( buffer, CSMV_GET_ORIGINL_ANA_QUA_MEM, uTotalSerial );
}

/* ������������������ */
VOID SMVC_SampleUpdateSoft( float * pSampleData, UNSIGNED pulse_counter )
{
    INT status;
    register INT k;
    register INT Number;
    register INT ChannelPoint;
    register float * pSrcAnalog;
    PRO_CHANNEL * pChannel;      /* ͨ������ģ��ָ��,��Чֵ¼��ʱʹ�� */
    float fValue;
    static UNSIGNED PowerErrNumber = 0; /* ��Դ�Լ������������� */
    register UNSIGNED * pSrcValue;
    static INT iniFlag = XJ_FALSE;
    register UNSIGNED uVariable;
    register UNSIGNED uCounter;
    /*--------------------------------------------------*/

    if( SMVC_User_Sample_Conversion )
    {
        /* �û���������ת������ */
        Number = CSMV_GET_PHY_ANALOG_NUMBER;
        status = SMVC_User_Sample_Conversion( pSampleData, Number );
        if( status == NU_FALSE )
        {
            return;
        }
    }

    if( iniFlag == XJ_TRUE )
    {
        /* ������Ŷ����� */
        pSrcValue = CSMV_GET_SAMPLE_PULSE_MEMORY + CSMV_GET_SAMPLE_OFFSET;
        uVariable = * pSrcValue;
        if((++ uVariable ) >= ( UNSIGNED )CSMV_GET_SAMPLE_COUNTER )
        {
            uVariable -= CSMV_GET_SAMPLE_COUNTER;
        }

        if( pulse_counter == uVariable )
        {
            /* ����������� */
            if( SMVC_Power_Error_Resume_Counter )
            {
                SMVC_Power_Error_Resume_Counter --;
            }
            /* ���Դ�Լ������������� */
            PowerErrNumber = 0;
            SMVC_Power_Error_Flag = NU_FALSE;
        }
        else
        {
            SMVC_Error.sampleCounter ++;
            /* ������Ų�����,�ж϶������ */
            if( pulse_counter >= uVariable )
            {
                uCounter = pulse_counter - uVariable;
            }
            else
            {
                uCounter = pulse_counter + CSMV_GET_SAMPLE_COUNTER - uVariable;
            }
            uCounter &= CPC_DATA_MASKS16;
            if( uCounter == 1 )
            {
                /* ��¼��ʧһ��Ĵ��� */
                SMVC_Error.OneSampleCounter ++;
            }
            if( PowerErrNumber < CVMC_POWER_ERROR_NUMBER )
            {
                PowerErrNumber = uCounter;
                if( PowerErrNumber >= CVMC_POWER_ERROR_NUMBER )
                {
                    /* �õ�Դ�Լ�����ʶ */
                    SMVC_Power_Error_Flag = NU_TRUE; /* �����־ */
                    /* ��Դ�Լ�����õ�Դ�ָ���������ֵΪ��ֵ */
                    SMVC_Power_Error_Resume_Counter = CSMV_GET_CHANNEL_POINT;
                }
                else if( PowerErrNumber == 1 ) /* ��ʧһ���Ƿ���в�ֵ���� */
                {
                    /* ��ʧһ�����ǰһ������ */
                    if( CSMV_IF_LOSE_ONE_SV_INSERT == XJ_TRUE )
                    {
                        SMVC_LoseOneInsertValue(  uVariable );
                    }
                }
            }
            else
            {
                /* �õ�Դ�Լ�����ʶ */
                SMVC_Power_Error_Flag = NU_TRUE; /* �����־ */
                /* ��Դ�Լ�����õ�Դ�ָ���������ֵΪ��ֵ */
                SMVC_Power_Error_Resume_Counter = CSMV_GET_CHANNEL_POINT;
            }

            /* �����Ʒ�����ع����� */
            if( CSMV_IF_LOSE_SV_QUALITY == XJ_TRUE )
            {
                SMVC_LosePointQMan( uCounter, uVariable );
            }
        }
    }
    else
    {
        /* ��ʼ��1���㲻������ż�� */
        PowerErrNumber = 0;
        SMVC_Power_Error_Flag = NU_FALSE;
        iniFlag = XJ_TRUE;
    }

    /* ����ԭʼ�������� */
    /* ����ʵͨ��ģ����ֵ,��ͨ����Ӧ������ */
    pSrcAnalog = pSampleData;
    Number = CSMV_GET_PHY_ANALOG_NUMBER;
    pChannel = CSMV_GET_CHL_TABLE_MEMORY;
    ChannelPoint = CSMV_GET_CHANNEL_POINT;
    for( k = 0; k < Number; k ++)
    {
        fValue = * pSrcAnalog;

        /* ͨ������ֵϵ�� */
        fValue *= pChannel->chl_sample_coefficient;
        /* ��Ư���� */
        fValue -= ( pChannel->chl_channel_excursion );
        /* ϵ������ */
        fValue *= ( pChannel->chl_channel_coefficient );
        SMVC_AnalogValueBuf[k] = fValue;
        pSrcAnalog ++;
        pChannel ++;
    }

    /* ����������� */
    SMVC_SampleSave( SMVC_AnalogValueBuf, CSMV_GET_ORIGINL_ANA_QUA_MEM, pulse_counter );
}

/* ���²������� */
VOID SMVC_SampleSave( float * sample_data, UNSIGNED * pQualityBuf, UNSIGNED pulse_counter )
{
    register INT k;
    register INT Number;
    register INT iSampleOffset;
    register UNSIGNED uVariable;
    register INT ChannelPoint;
    register float * pSrcAnalog;
    register float * pDstAnalog;
    register float fVariable;
    register SIGNED * pWaveAnalog;
    register UNSIGNED * pSrcValue;
    register UNSIGNED * pDstValue;
    register UNSIGNED uOffset;       /* ��ȡ¼������ƫ���� */
    register UNSIGNED uWaveOffset;   /* ¼����¼λ�� */
    register UNSIGNED * pWaveMomery; /* ¼�����ݴ洢������ */
    register int      iRecordFlag;   /* �Ƿ��¼¼�����ݱ�־ */
    PRO_CHANNEL       * pChannel;    /* ͨ������ģ��ָ��,��Чֵ¼��ʱʹ�� */
    TIME * pTime;

    /* ��������λ�� */
    iSampleOffset = CSMV_GET_SAMPLE_OFFSET + 1; /* ��������λ��ƫ�� */
    if( iSampleOffset >= CSMV_GET_CHANNEL_POINT )
    {
        iSampleOffset = 0; /* ����ֵѭ���洢 */
    }

    /* ����ʵͨ��ģ����ֵ,��ͨ����Ӧ������ */
    pSrcAnalog = sample_data;
    Number = CSMV_GET_PHY_ANALOG_NUMBER;
    pDstAnalog = CSMV_GET_ANALOG_MEMORY + iSampleOffset;
    pChannel = CSMV_GET_CHL_TABLE_MEMORY;
    ChannelPoint = CSMV_GET_CHANNEL_POINT;
    for( k = 0; k < Number; k ++)
    {
        * pDstAnalog = * pSrcAnalog;
        pSrcAnalog ++;
        pChannel ++;
        pDstAnalog += ChannelPoint;
    }
    /* ���²������������ */
    pDstValue = CSMV_GET_SAMPLE_PULSE_MEMORY + iSampleOffset;
    * pDstValue = pulse_counter;

    /* ����ģ����ͨ��Ʒ������ */
    Number = CSMV_GET_PHY_ANALOG_GROUPS;
    pSrcValue = pQualityBuf;
    pDstValue = CSMV_GET_ANALOG_QUA_MEMORY + iSampleOffset;
    for( k = 0; k < Number; k ++)
    {
        * pDstValue = * pSrcValue;
        pSrcValue ++;
        pDstValue += ChannelPoint;
    }

    /* ���¿���״̬�� */
    Number = CSMV_GET_INPUT_GROUPS;
    pDstValue = CSMV_GET_INPUT_MEMORY + iSampleOffset;
    for( k = 0; k < Number; k ++)
    {
        /* ��ȡ����״̬�� */
        uVariable = CINC_ReadInput( k );
        * pDstValue = uVariable;
        pDstValue += ChannelPoint;
    }

    /* ���¿���״̬��ֵ */
    Number = CSMV_GET_OUTPUT_GROUPS;
    pDstValue = CSMV_GET_OUTPUT_MEMORY + iSampleOffset;
    for( k = 0; k < Number; k ++)
    {
        uVariable = OUT_GetOutPickupBits( k );
        * pDstValue = uVariable;
        pDstValue += ChannelPoint;
    }

    /* ���±���״̬��ֵ */
    Number = CSMV_GET_MAX_PROTECT_GROUPS;
    pSrcValue = CSMV_GET_ORIGINL_PRO_STS_MEM;
    pDstValue = CSMV_GET_PRO_STS_MEMORY + iSampleOffset;
    for( k = 0; k < Number; k ++)
    {
        * pDstValue = * pSrcValue;
        pSrcValue ++;
        pDstValue += ChannelPoint;
    }

    /* �����߼�״̬��ֵ */
    Number = CSMV_GET_MAX_PROTECT_LOG_GROUPS;
    pSrcValue = CSMV_GET_ORIGINL_PRO_LOG_MEM;
    pDstValue = CSMV_GET_PRO_LOG_MEMORY + iSampleOffset;
    for( k = 0; k < Number; k ++)
    {
        * pDstValue = * pSrcValue;
        pSrcValue ++;
        pDstValue += ChannelPoint;
    }

    /* ���²���ָ��λ�� */
    CSMV_SET_SAMPLE_OFFSET( iSampleOffset );

    SMVC_SampleCount ++;
    if( SMVC_InitializeFlag == 0 )
    {
        if( SMVC_SampleCount >= CVMC_SAMPLE_INITIALIZE_POINT )
        {
            SMVC_InitializeFlag = 1;
        }
        return;
    }

    if( SMVC_User_AD_Interrupt )
    {
        /* �û�A/D�жϴ����� */
        SMVC_User_AD_Interrupt();
    }

    /* ���¼¼�����ݱ�־ */
    iRecordFlag = 0;

    if( WavePointer == NU_NULL )
    {
        /* �޴���¼�����ݼ�¼��ֱ�ӷ��� */
        return;
    }

    /* ¼�����ݼ�¼ */
    if( WavePointer->RecordNumber == 0 )
    {
        /* ¼�����ݵĹ���ʱ���ڴ���¼��ʱ��¼ */
        /* �ü�¼¼�����ݱ�־ */
        iRecordFlag = 1;
    }

    /* �ж��Ƿ������¼¼������(¼������Ƿ�) */
    if(( WavePointer->SampleNumber ) >= ( WavePointer->SampleSpace ))
    {
        /* �ü�¼¼�����ݱ�־ */
        iRecordFlag = 1;
        /* ��¼����������� */
        WavePointer->SampleNumber = 0;
    }

    /* �ж��Ƿ��¼¼������ */
    if( iRecordFlag == 1 )
    {
        /* ���ݹ���ǰ����������ȡ¼������ƫ��λ�� */
        /* �ж�¼������ǰ�����뵱ǰ���²���������ƫ�� */
        if(( WavePointer -> StartPointer )> ( UNSIGNED )CSMV_GET_SAMPLE_OFFSET )
        {
            uOffset = ( CSMV_GET_SAMPLE_OFFSET + CSMV_GET_CHANNEL_POINT ) - ( WavePointer -> StartPointer );
        }
        else
        {
            uOffset = CSMV_GET_SAMPLE_OFFSET - ( WavePointer -> StartPointer );
        }

        /* ¼����¼λ�� */
        uWaveOffset = WavePointer->RecordNumber;
        /* ¼�����ݴ洢������ */
        pWaveMomery = WavePointer->WaveMemory;
        /* ����ͨ��size */
        ChannelPoint = CSMV_GET_CHANNEL_POINT;

        if( WavePointer->AnalogNumber != 0 )
        {
            /* ģ����ͨ������ģ����ʼ��ַ */
            pChannel = CSMV_GET_CHL_TABLE_MEMORY;
            /* ��¼ģ�������� */
            pWaveAnalog = ( SIGNED *)pWaveMomery;
            if(( WavePointer->Type ) & WAVE_TYPE_COEFFECT )
            {
                /* ��Чֵ¼�� */
                for( k = 0; k < (( INT )WavePointer -> AnalogNumber ); k ++)
                {
                    /* ��¼ģ����¼������ */
                    fVariable = (( pChannel +
                        (( WavePointer->AnalogInf )+ k )->Number )->chl_virtual_value );
                    fVariable *= ((( WavePointer->AnalogInf )+ k )->WaveCoefficient );
                    *( pWaveAnalog + uWaveOffset ) = ( SIGNED )fVariable;
                    /* ����¼��λ��ƫ���� */
                    uWaveOffset += WavePointer->TotalPointer;
                }
            }
            else
            {
                /* ģ������������ */
                pDstAnalog = CSMV_GET_ANALOG_MEMORY + uOffset;
                /* ˲ʱֵ¼�� */
                for( k = 0; k < (( INT )WavePointer -> AnalogNumber ); k ++)
                {
                    /* ��¼ģ����¼������ */
                    fVariable = (* ( pDstAnalog + (( WavePointer->AnalogInf )+ k )->Number * ChannelPoint ));
                    fVariable *= ((( WavePointer->AnalogInf )+ k )->WaveCoefficient );
                    *( pWaveAnalog + uWaveOffset ) = ( SIGNED )fVariable;
                    /* ����¼��λ��ƫ���� */
                    uWaveOffset += WavePointer->TotalPointer;
                }
            }
        }

        if( WavePointer->InputNumber != 0 )
        {
            /* ��¼���������� */
            pDstValue = CSMV_GET_INPUT_MEMORY + uOffset;
            for( k = 0; k < (( INT )WavePointer -> InputNumber ); k ++)
            {
                /* ��¼���������� */
                *( pWaveMomery + uWaveOffset ) = * ( pDstValue +
                    (( WavePointer->InputInf )+ k )->GroupNumber * ChannelPoint );
                /* ����¼��λ��ƫ���� */
                uWaveOffset += WavePointer->TotalPointer;
            }
        }

        if( WavePointer->OutputNumber != 0 )
        {
            /* ��¼���������� */
            pDstValue = CSMV_GET_OUTPUT_MEMORY + uOffset;
            for( k = 0; k < (( INT )WavePointer -> OutputNumber ); k ++)
            {
                /* ��¼���������� */
                *( pWaveMomery + uWaveOffset ) = * ( pDstValue +
                    (( WavePointer->OutputInf )+ k )->GroupNumber * ChannelPoint );
                /* ����¼��λ��ƫ���� */
                uWaveOffset += WavePointer->TotalPointer;
            }
        }

        if( WavePointer->StateNumber != 0 )
        {
            /* ��¼����״̬������ */
            pDstValue = CSMV_GET_PRO_STS_MEMORY + uOffset;
            for( k = 0; k < (( INT )WavePointer -> StateNumber ); k ++)
            {
                /* ��¼����״̬������ */
                *( pWaveMomery + uWaveOffset ) = * ( pDstValue +
                    (( WavePointer->StateInf )+ k )->GroupNumber * ChannelPoint );
                /* ����¼��λ��ƫ���� */
                uWaveOffset += WavePointer->TotalPointer;
            }
        }

        if( WavePointer->LogicNumber != 0 )
        {
            /* ��¼�߼�״̬������ */
            pDstValue = CSMV_GET_PRO_LOG_MEMORY + uOffset;
            for( k = 0; k < (( INT )WavePointer -> LogicNumber ); k ++)
            {
                /* ��¼�߼�״̬������ */
                *( pWaveMomery + uWaveOffset ) = * ( pDstValue +
                    (( WavePointer->LogicalInf )+ k )->GroupNumber * ChannelPoint );
                /* ����¼��λ��ƫ���� */
                uWaveOffset += WavePointer->TotalPointer;
            }
        }
        /* ����¼����¼�ĵ��� */
        WavePointer->RecordNumber += 1;
        /* �ж�¼���Ƿ���� */
        if(( WavePointer->RecordNumber ) >= ( WavePointer->TotalPointer ))
        {
            /* ��¼����ɱ�־ */
            WavePointer->EndFlag = 1;
            /* ��¼��һ��¼������ */
            for( k = 0; k < ( CWM_MAX_WAVE_POINTER - 1 ); k ++ )
            {
                WavePointerCB[ k ] = WavePointerCB[( k + 1 )];
            }
            WavePointerCB[( CWM_MAX_WAVE_POINTER - 1 )] = XJ_NULL;
            /* ����¼������ָ�� */
            WavePointer = WavePointerCB[0];
            if( WavePointer )
            {
                pTime = XJP_TM_Get_Time_Ptr();
                WavePointer -> Year = pTime -> Year;
                WavePointer -> Month = pTime -> Month;
                WavePointer -> Day = pTime -> Day;
                WavePointer -> Hour = pTime -> Hour;
                WavePointer -> Minute = pTime -> Minute;
                WavePointer -> Millionsecond = pTime -> Millionsecond;
                WavePointer -> Nanosecond = pTime -> Nanosecond;
            }
            /* ¼������,ֱ�ӷ��� */
            return;
        }
    }

    /* ����¼����������� */
    WavePointer->SampleNumber += 1;
}

/* ��ȡ�������������� */
INT SMVC_GetSampleCount( UNSIGNED * pSampleCount )
{
    * pSampleCount = SMVC_SampleCount;
    return( NU_SUCCESS );
}

/* ��ʧһ�����ǰһ������ */
static INT SMVC_LoseOneInsertValue( UNSIGNED pulse_counter )
{
    register INT k;
    register INT Number;
    register INT iVariable;
    register INT ChannelPoint;
    register float * pSrcAnalog;
    register UNSIGNED * pDstValue;
    register UNSIGNED * pSrcValue;
    UNSIGNED uQualityBuf[CVMC_MAX_ANALOG_QUALITY_GROUPS];

    /* ��Դһ���������ǰ��������浱ǰ�� */
    iVariable = CSMV_GET_SAMPLE_OFFSET;
    pSrcAnalog = CSMV_GET_ANALOG_MEMORY + iVariable;
    /* ͨ������ */
    Number = CSMV_GET_PHY_ANALOG_NUMBER;
    /* ÿ��ͨ����С */
    ChannelPoint = CSMV_GET_CHANNEL_POINT;
    for( k = 0; k < Number; k ++)
    {
        SMVC_AnalogValueBuf[k] = * pSrcAnalog;
        pSrcAnalog += ChannelPoint;
    }

    /* ��ȡǰһ���Ʒ������ */
    Number = CSMV_GET_PHY_ANALOG_GROUPS;
    pDstValue = ( register UNSIGNED *)& uQualityBuf[0];
    pSrcValue = CSMV_GET_ANALOG_QUA_MEMORY + iVariable;
    for( k = 0; k < Number; k ++)
    {
        * pDstValue = * pSrcValue;
        pDstValue ++;
        pSrcValue += ChannelPoint;
    }

    SMVC_SampleSave( ( float *)& SMVC_AnalogValueBuf[0], uQualityBuf, pulse_counter );
    return( NU_SUCCESS );
}

/* ����ֵ��ʧ��Ʒ�����ش����� */
static INT SMVC_LosePointQMan( INT iLoseCount, UNSIGNED StartLoseSerl )
{
    register INT k;
    register INT Number;
    register INT iVariable;
    register INT ChannelPoint;
    register UNSIGNED * pDstValue;
    register INT  uSerial;
    INT  iStartGroup;
    INT  iGroupCount;
    /*--------------------------------------------*/
    /* ��ȡģ����ͨ��Ʒ������¼��״̬������ʼ��ż����� */
    CSMV_MSG_GetChlQStateInfo(& iStartGroup, & iGroupCount );
    /* ��������λ�� */
    iVariable = CSMV_GET_SAMPLE_OFFSET + 1; /* ��������λ��ƫ�� */
    if( iVariable >= CSMV_GET_CHANNEL_POINT )
    {
        iVariable = 0; /* ����ֵѭ���洢 */
    }
    CSMV_SET_SAMPLE_OFFSET( iVariable );

    /* ��ȡǰһ���Ʒ������ */
    Number = CSMV_GET_PHY_ANALOG_GROUPS;
    /* ÿ��ͨ����С */
    ChannelPoint = CSMV_GET_CHANNEL_POINT;
    /* ��ʼ������� */
    uSerial = StartLoseSerl;
    /* �ж϶�������Ƿ��Ѿ��ﵽ���ֵ,����ﵽ���ֵ��������������Ʒ�����ض�����Ϊ��Ч */
    if( iLoseCount >= CSMV_GET_CHANNEL_POINT )
    {
        /* ģ����ͨ��Ʒ��������ʼλ�� */
        pDstValue = CSMV_GET_ANALOG_QUA_MEMORY;
        /* Ʒ�����ص�����������������Ϊ��Ч */
        memset( pDstValue, 0,( sizeof( UNSIGNED )* CSMV_GET_CHANNEL_POINT * Number ));
        /* �洢ͨ��Ʒ�����ص�״̬�� */
        pDstValue = CSMV_GET_PRO_STS_MEMORY + ChannelPoint * iStartGroup;
        /* ��������������Ϊ��Ч */
        memset( pDstValue, 0,( sizeof( UNSIGNED )* CSMV_GET_CHANNEL_POINT * iGroupCount ));
        /* ������ƫ��λ������Ϊ��ֵ */
        iVariable = 0;
    }
    else
    {
        pDstValue = CSMV_GET_ANALOG_QUA_MEMORY + CSMV_GET_SAMPLE_OFFSET;
        /* �жϴ洢λ�� */
        if(( iLoseCount + CSMV_GET_SAMPLE_OFFSET ) <= CSMV_GET_CHANNEL_POINT )
        {
            /* ������� */
            for( k = 0; k < Number; k ++)
            {
                /* ����Ʒ������Ϊ��Ч״̬ */
                memset( pDstValue, 0,( sizeof( UNSIGNED )* iLoseCount ));
                pDstValue += ChannelPoint;
            }

            /* �洢ͨ��Ʒ�����ص�״̬�� */
            pDstValue = CSMV_GET_PRO_STS_MEMORY + ChannelPoint * iStartGroup + CSMV_GET_SAMPLE_OFFSET;
            /* ������� */
            for( k = 0; k < iGroupCount; k ++)
            {
                /* ����Ʒ������Ϊ��Ч״̬ */
                memset( pDstValue, 0,( sizeof( UNSIGNED )* iLoseCount ));
                pDstValue += ChannelPoint;
            }

            /* ������������� */
            pDstValue = CSMV_GET_SAMPLE_PULSE_MEMORY + CSMV_GET_SAMPLE_OFFSET;
            /* ������ŵĵ��� */
            for( k = 0; k < iLoseCount; k ++)
            {
                * pDstValue ++ = ( register UNSIGNED )uSerial;
                if( ++ uSerial >= CSMV_GET_SAMPLE_COUNTER )
                {
                    uSerial = 0;
                }
            }

            /* ����ƫ��λ�õ��� */
            iVariable = iLoseCount + CSMV_GET_SAMPLE_OFFSET;
        }
        else
        {
            iVariable = CSMV_GET_CHANNEL_POINT - CSMV_GET_SAMPLE_OFFSET;
            for( k = 0; k < Number; k ++)
            {
                /* ����Ʒ������Ϊ��Ч״̬ */
                memset( pDstValue, 0,( sizeof( UNSIGNED )* iVariable ));
                pDstValue += ChannelPoint;
            }
            /* �洢ͨ��Ʒ�����ص�״̬�� */
            pDstValue = CSMV_GET_PRO_STS_MEMORY + ChannelPoint * iStartGroup + CSMV_GET_SAMPLE_OFFSET;
            /* ������� */
            for( k = 0; k < iGroupCount; k ++)
            {
                /* ����Ʒ������Ϊ��Ч״̬ */
                memset( pDstValue, 0,( sizeof( UNSIGNED )* iVariable ));
                pDstValue += ChannelPoint;
            }

            /* ������������� */
            pDstValue = CSMV_GET_SAMPLE_PULSE_MEMORY + CSMV_GET_SAMPLE_OFFSET;
            /* ������ŵĵ��� */
            for( k = 0; k < iVariable; k ++)
            {
                * pDstValue ++ = uSerial;
                if( ++ uSerial >= CSMV_GET_SAMPLE_COUNTER )
                {
                    uSerial = 0;
                }
            }
            /* Ʒ��������ʼλ�� */
            pDstValue = CSMV_GET_ANALOG_QUA_MEMORY;
            iVariable = CSMV_GET_SAMPLE_OFFSET + iLoseCount - CSMV_GET_CHANNEL_POINT;
            for( k = 0; k < Number; k ++)
            {
                /* ����Ʒ������Ϊ��Ч״̬ */
                memset( pDstValue, 0,( sizeof( UNSIGNED )* iVariable ));
                pDstValue += ChannelPoint;
            }

            /* �洢ͨ��Ʒ�����ص�״̬�� */
            pDstValue = CSMV_GET_PRO_STS_MEMORY + ChannelPoint * iStartGroup;
            /* ������� */
            for( k = 0; k < iGroupCount; k ++)
            {
                /* ����Ʒ������Ϊ��Ч״̬ */
                memset( pDstValue, 0,( sizeof( UNSIGNED )* iVariable ));
                pDstValue += ChannelPoint;
            }

            /* ������������� */
            pDstValue = CSMV_GET_SAMPLE_PULSE_MEMORY;
            /* ������ŵĵ��� */
            for( k = 0; k < iVariable; k ++)
            {
                * pDstValue ++ = uSerial;
                if( ++ uSerial >= CSMV_GET_SAMPLE_COUNTER )
                {
                    uSerial = 0;
                }
            }
        }
        /* ��������ǰ���λ����,�洢��һ�����ݵ�ʱ���Լ����� */
        iVariable -= 1;
        /* ����ƫ��λ�õ��� */
        if( iVariable >= CSMV_GET_CHANNEL_POINT )
        {
            iVariable = 0;
        }
    }
    /* ���ò���ƫ��λ�� */
    CSMV_SET_SAMPLE_OFFSET( iVariable );
    return( NU_SUCCESS );
}

/* ���ò���׼����ʶ */
INT CSMV_SetSampleInitFlag( INT iFlag )
{
    SMVC_SampleInitFlag = iFlag;
    return( NU_SUCCESS );
}

/* ��ȡ����׼����ʶ */
INT CSMV_GetSampleInitFlag( VOID )
{
    INT iStatus = NU_UNAVAILABLE;
    if( SMVC_SampleInitFlag == NU_TRUE )
    {
        iStatus = NU_SUCCESS;
    }
    return( iStatus );
}
