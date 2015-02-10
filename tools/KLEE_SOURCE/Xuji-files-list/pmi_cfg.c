
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
/*      pmi_cfg.c                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      PMI - Platform Initialize                                        */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the initialization routine for this           */
/*      component.                                                       */
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
/*      ZhiDeYang       02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include <string.h>
#include <math.h>
#include "plat/inc/xj_cosc.h"
#include "plat/inc/esp_cfg.h"
#include "plat_specific/plat_specific.h"
#include "plat/inc/apf_extr.h"
#include "plat/inc/xj_out_check.h"
#include "plat/net_inc/lc_extr.h"
#include "plat/inc/ctb_extr.h"
#include "plat/inc/pgf_defs.h"
#include "plat/inc/csmv_extr.h"

#define MAX_POWER_CHECK_NUMBER          8               /* ����Դ�Լ��ͨ������ */
#define MAX_FREQ_CHANNEL_NUMBER         2               /* ����Ƶ��ͨ������ */
#define BOUNDARY_CHECK(x,y) ((x > y) ? y : x )          /* �߽��� */
#define DEVICE_CPU_TYPE                 0x0B /* CPU������(bit0-����;bit1-��բ;bit2-������·����;bit3-��CPU��) */

/* Ʒ�����ص�ַ���� */
#define SIN_QUA(x)                      (VOID *)(&PLAT_SoftInputQuality[x]) /* ���ֿ�����Ʒ�����ص�ַ */
#define SOUT_QUA(x)                     (VOID *)(&PLAT_SoftOutputQuality[x]) /* ���ֿ�����Ʒ�����ص�ַ */
#define SOUT_CHECKQUA(x)                (VOID *)(&PLAT_SoftOutputCheckQuality[x])  /* ����Ʒ�����ؼ���λ��ַ */
#define SIN_CHECKQUA(x)                 (VOID *)(&PLAT_SoftInputCheckQuality[x])   /* ����Ʒ�����ؼ���λ��ַ */
#define OUTR_QUA(x)                     (VOID *)(&PLAT_OutputRQuality[x])    /* ����Ʒ�����ص�ַ */
#define OUTR_CHECKQUA(x)                (VOID *)(&PLAT_RCheckQuality[x])     /* ����Ʒ�����ؼ���λ��ַ */
/* ����������˿ڶ��� */
#define SIN1_NUM                        1               /* �˿���Ŀ */
#define SIN1_BITS                       32              /* �˿�����λ�� */
#define SIN1_MASKS                      0xFFFFFFFF      /* �˿��������� */
#define SIN1_REVERSE                    0               /* ���ݷ�תλ */
#define SIN1_PORT0(x)                   (VOID *)(&PLAT_SoftInputMemory[x]) /* ���ֿ������˿ڵ�ַ1 */
#define SIN1_PORT1(x)                   0               /* �˿ڵ�ַ2 */
#define SIN1_PORT2(x)                   0               /* �˿ڵ�ַ3 */
#define SIN1_PORT3(x)                   0               /* �˿ڵ�ַ4 */
/* �����������˿ڶ��� */
#define SOUT1_NUM                       1               /* �˿���Ŀ */
#define SOUT1_BITS                      32              /* �˿�����λ�� */
#define SOUT1_MASKS                     0xFFFFFFFF      /* �˿��������� */
#define SOUT1_REVERSE                   0x00000000      /* ���ݷ�תλ */
#define SOUT1_PORT0(x)                  (VOID *)(&PLAT_SoftOutputMemory[x]) /* ���ֿ������˿ڵ�ַ1 */
#define SOUT1_PORT1(x)                  0               /* �˿ڵ�ַ2 */
#define SOUT1_PORT2(x)                  0               /* �˿ڵ�ַ3 */
#define SOUT1_PORT3(x)                  0               /* �˿ڵ�ַ4 */
/* Ӳ�����濪�����˿ڶ��� */
#define SHCOUT1_NUM                     1               /* �˿���Ŀ */
#define SHCOUT1_BITS                    32              /* �˿�����λ�� */
#define SHCOUT1_MASKS                   0xFFFFFFFF      /* �˿��������� */
#define SHCOUT1_REVERSE                 0x00000000      /* ���ݷ�תλ */
#define SHCOUT1_PORT0(x)                (VOID *)(&PLAT_HardOutputCheckPort[x]) /* Ӳ�����濪�����˿�1 */
#define SHCOUT1_PORT1(x)                0               /* �˿ڵ�ַ2 */
#define SHCOUT1_PORT2(x)                0               /* �˿ڵ�ַ3 */
#define SHCOUT1_PORT3(x)                0               /* �˿ڵ�ַ4 */
/* Ӳ��������չ�����˿ڶ��� */
#define SEHOUT1_NUM                     1               /* �˿���Ŀ */
#define SEHOUT1_BITS                    32              /* �˿�����λ�� */
#define SEHOUT1_MASKS                   0xFFFFFFFF      /* �˿��������� */
#define SEHOUT1_REVERSE                 0x00000000      /* ���ݷ�תλ */
#define SEHOUT1_PORT0(x)                (VOID *)(&PLAT_HardExOutPort[x]) /* Ӳ��������չ�����˿�1 */
#define SEHOUT1_PORT1(x)                0               /* �˿ڵ�ַ2 */
#define SEHOUT1_PORT2(x)                0               /* �˿ڵ�ַ3 */
#define SEHOUT1_PORT3(x)                0               /* �˿ڵ�ַ4 */
/* Ӳ��������չ�������˿ڶ��� */
#define SEHCOUT1_NUM                    1               /* �˿���Ŀ */
#define SEHCOUT1_BITS                   32              /* �˿�����λ�� */
#define SEHCOUT1_MASKS                  0xFFFFFFFF      /* �˿��������� */
#define SEHCOUT1_REVERSE                0x00000000      /* ���ݷ�תλ */
#define SEHCOUT1_PORT0(x)               (VOID *)(&PLAT_HardExOutCheckPort[x]) /* Ӳ��������չ�������˿�1 */
#define SEHCOUT1_PORT1(x)               0               /* �˿ڵ�ַ2 */
#define SEHCOUT1_PORT2(x)               0               /* �˿ڵ�ַ3 */
#define SEHCOUT1_PORT3(x)               0               /* �˿ڵ�ַ4 */

/* �˿����ݶ��� */
#define PORT_DATA_COMM(MNAME,QNAME,CHECKQNAME,NUMBER) \
    MNAME##_NUM, \
    MNAME##_BITS, \
    MNAME##_MASKS, \
    MNAME##_PORT0(NUMBER), \
    MNAME##_PORT1(NUMBER), \
    MNAME##_PORT2(NUMBER), \
    MNAME##_PORT3(NUMBER), \
    MNAME##_REVERSE, \
    QNAME##_QUA(NUMBER), \
    CHECKQNAME##_CHECKQUA(NUMBER),

/* Define external inner-component global data references.  */
/* ƽ̨���ڹ����� */
HIOM PlatHIOM;
ESP_CFG PLAT_cfg;
UNSIGNED PLAT_SoftInputMemory[CPC_MAX_INPUT_GROUPS];    /* ���ֿ������˿� */
UNSIGNED PLAT_SoftInputQuality[CPC_MAX_INPUT_GROUPS];   /* ���ֿ�����Ʒ������ */
UNSIGNED PLAT_SoftInputCheckQuality[CPC_MAX_INPUT_GROUPS];   /* ���ֿ�����Ʒ������ */
UNSIGNED PLAT_SoftOutputMemory[CPC_MAX_OUTPUT_GROUPS];  /* ���ֿ������˿� */
UNSIGNED PLAT_SoftOutputQuality[CPC_MAX_OUTPUT_GROUPS]; /* ���ֿ�����Ʒ������ */
UNSIGNED PLAT_SoftOutputCheckQuality[CPC_MAX_OUTPUT_GROUPS]; /* ���ֿ�����Ʒ�����ؼ���λ */
UNSIGNED PLAT_HardOutputCheckPort[CPC_MAX_OUTPUT_GROUPS]; /* Ӳ������������˿� */
UNSIGNED PLAT_HardExOutPort[CPC_MAX_OUTPUT_GROUPS];       /* Ӳ����չ��������˿� */
UNSIGNED PLAT_HardExOutCheckPort[CPC_MAX_OUTPUT_GROUPS];  /* Ӳ����չ����������˿� */
/* �˻�����������Щ��Ҫ���ö˿ڵ��ǲ���ҪƷ�����صĶ˿�ʹ�� */
UNSIGNED PLAT_OutputRQuality[CPC_MAX_OUTPUT_GROUPS];
/* �˻�����������Щ��Ҫ���ö˿ڵ��ǲ���ҪƷ�����ؼ���λ�Ķ˿�ʹ�� */
UNSIGNED PLAT_RCheckQuality[CPC_MAX_OUTPUT_GROUPS];

static ESP_CFG_SMVC_POWER PLAT_PowerCfg[MAX_POWER_CHECK_NUMBER];
static ESP_CFG_FREQ_CHANNEL PLAT_FreqChlCfg[MAX_FREQ_CHANNEL_NUMBER];
static ESP_CFG_OUTPUT_PORT PLAT_OutputPortCfg[CPC_MAX_OUTPUT_GROUPS];
static UNSIGNED PLAT_OutputCheckMasks[CPC_MAX_OUTPUT_GROUPS];
static UNSIGNED PLAT_OutputHoldMasks[CPC_MAX_OUTPUT_GROUPS];
static UNSIGNED PLAT_OutputSignalLed[CPC_MAX_OUTPUT_GROUPS];
static UNSIGNED PLAT_OutputTripLed[CPC_MAX_OUTPUT_GROUPS];
static UNSIGNED PLAT_OutputAlarm[CPC_MAX_OUTPUT_GROUPS];
static UNSIGNED PLAT_OutputReset[CPC_MAX_OUTPUT_GROUPS];
static ESP_CFG_INPUT_PORT PLAT_InputPortCfg[CPC_MAX_INPUT_GROUPS];
/* �ڲ����� */

/* ����������˿����� */
static ESP_CFG_INPUT_PORT PLAT_SoftInputPortCfg[] =
{
    PORT_DATA_COMM( SIN1, SIN, SIN, 0 )   /* ����1  */
    PORT_DATA_COMM( SIN1, SIN, SIN, 1 )   /* ����2  */
    PORT_DATA_COMM( SIN1, SIN, SIN, 2 )   /* ����3  */
    PORT_DATA_COMM( SIN1, SIN, SIN, 3 )   /* ����4  */
    PORT_DATA_COMM( SIN1, SIN, SIN, 4 )   /* ����5  */
    PORT_DATA_COMM( SIN1, SIN, SIN, 5 )   /* ����6  */
    PORT_DATA_COMM( SIN1, SIN, SIN, 6 )   /* ����7  */
    PORT_DATA_COMM( SIN1, SIN, SIN, 7 )   /* ����8  */
    PORT_DATA_COMM( SIN1, SIN, SIN, 8 )   /* ����9  */
    PORT_DATA_COMM( SIN1, SIN, SIN, 9 )   /* ����10 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 10 )  /* ����11 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 11 )  /* ����12 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 12 )  /* ����13 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 13 )  /* ����14 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 14 )  /* ����15 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 15 )  /* ����16 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 16 )  /* ����17 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 17 )  /* ����18 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 18 )  /* ����19 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 19 )  /* ����20 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 20 )  /* ����21 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 21 )  /* ����22 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 22 )  /* ����23 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 23 )  /* ����24 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 24 )  /* ����25 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 25 )  /* ����26 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 26 )  /* ����27 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 27 )  /* ����28 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 28 )  /* ����29 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 29 )  /* ����30 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 30 )  /* ����31 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 31 )  /* ����32 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 32 )  /* ����33 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 33 )  /* ����34 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 34 )  /* ����35 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 35 )  /* ����36 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 36 )  /* ����37 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 37 )  /* ����38 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 38 )  /* ����39 */
    PORT_DATA_COMM( SIN1, SIN, SIN, 39 )  /* ����40 */
};

/* �����������˿����� */
static ESP_CFG_OUTPUT_PORT PLAT_SoftOutputPortCfg[] =
{
    PORT_DATA_COMM( SOUT1, SOUT, SOUT, 0 )  /* ����1 */
    PORT_DATA_COMM( SOUT1, SOUT, SOUT, 1 )  /* ����2 */
    PORT_DATA_COMM( SOUT1, SOUT, SOUT, 2 )  /* ����3 */
    PORT_DATA_COMM( SOUT1, SOUT, SOUT, 3 )  /* ����4 */
    PORT_DATA_COMM( SOUT1, SOUT, SOUT, 4 )  /* ����5 */
	PORT_DATA_COMM( SOUT1, SOUT, SOUT, 5 )  /* ����6 */
    PORT_DATA_COMM( SOUT1, SOUT, SOUT, 6 )  /* ����7 */
    PORT_DATA_COMM( SOUT1, SOUT, SOUT, 7 )  /* ����8 */
    PORT_DATA_COMM( SOUT1, SOUT, SOUT, 8 )  /* ����9 */
    PORT_DATA_COMM( SOUT1, SOUT, SOUT, 9 )  /* ����10*/
};

/* Define internal function calls.  */

/* ����Ӧ�ó������ò��� */
INT PMI_SetApplicationCfg( VOID )
{
    INT k;
    INT status;
    INT number;
    SETDEVPARAM SetDevParam;
    ANALOGSET AnalogSet;
    SETOUTBITS SetOutBits;
    SETTABNUMBER SetTabNumber;
    INT Groups;
    INT SubGroups;
    INT HardGroups;
    INT SoftGroups;
    ESP_CFG_INPUT_PORT * pInputPort;
    ESP_CFG_OUTPUT_PORT * pOutputPort;
    ESP_CFG_SMVC_POWER * PowerChannel;
    ESP_CFG_FREQ_CHANNEL * FrequencyChannel;

    /* ����װ�û������� */
    status = APFC_GetParameter( DEVICE_PARAM_SET_ID,& SetDevParam );
    if( status == NU_SUCCESS )
    {
        PLAT_cfg.device.CpuNumber = SetDevParam.Cpu_Number; /* CPU���� */
        PLAT_cfg.device.CpuType = SetDevParam.Cpu_Type;     /* CPU������ */
        PLAT_cfg.device.FunSelect = SetDevParam.Fun_Select; /* ����ѡ�� */
        PLAT_cfg.device.DeviceFun = SetDevParam.Device_Fun; /* װ�ù������� */
        PLAT_cfg.device.SetZoneCount = SetDevParam.SetZoneCount; /* װ�ö�ֵ������ */
        if( PLAT_cfg.device.FunSelect & FUN_OPTICAL )
        {
            PLAT_cfg.smvc.SampleCounter = PLAT_MAX_SAMPLE_COUNTER_OLT; /* �������Ĳ������ֵ */
            PLAT_cfg.smvc.LoseSV_Quality = XJ_TRUE; /* �������ֵ����Ʒ�����ش��� */
            PLAT_cfg.smvc.LoseOneSV_Insert = XJ_FALSE; /* ��һ�����ֵ���ݲ���ֵ */
        }
        else
        {
            PLAT_cfg.smvc.SampleCounter = PLAT_MAX_SAMPLE_COUNTER; /* ���Ĳ������ֵ */
            PLAT_cfg.smvc.LoseSV_Quality = XJ_FALSE; /* �������ֵ����Ʒ�����ز����� */
            PLAT_cfg.smvc.LoseOneSV_Insert = XJ_TRUE; /* ��һ�����ֵ���ݲ�ֵ */
        }

        /* ���Ϊ����װ�ý���ʱ�������Ϊ1�� */
        if( PLAT_cfg.device.FunSelect & FUN_IED_DEVICE )
        {
            /* ���ö�ʱʱ���� */
            XJP_TM_SET_Time_Interval( 1000 );
        }
        /* ����װ�����е�״̬ */
        XJP_Led_Change_Type( SetDevParam.Led_NormalStatus,
            SetDevParam.Led_StartStatus, SetDevParam.Led_AbnorStatus );

        PLAT_cfg.history.UserEventGroups =
            BOUNDARY_CHECK(( SetDevParam.Max_EventRecordGroup ), CPC_MAX_EVENT_RECORD ); /* �û���¼�¼����� */
        PLAT_cfg.history.TempEventNumber =
            BOUNDARY_CHECK(( SetDevParam.Max_Event_Size ), CPC_MAX_RAM_EVENT_NUMBER ); /* ��ʱ�����¼����� */
        PLAT_cfg.history.TempReportNumber =
            BOUNDARY_CHECK(( SetDevParam.Max_Report_Size ), CPC_MAX_RAM_REPORT_NUMBER ); /* ��ʱ���汨����� */

        HardGroups = SetDevParam.Hard_Input_Group;
        SoftGroups = SetDevParam.Soft_Input_Group;
        Groups = HardGroups + SoftGroups;
        PLAT_cfg.input.InputNumbers = SetDevParam.Max_Input; /* ���������� */
        PLAT_cfg.input.InputGroups = BOUNDARY_CHECK( Groups, CPC_MAX_INPUT_GROUPS ); /* ���������� */
        PLAT_cfg.input.WaveInputGroups = BOUNDARY_CHECK( CPC_MAX_WAVE_INPUT_GROUPS, PLAT_cfg.input.InputGroups ); /* ¼������������ */
        PLAT_cfg.input.HardInputGroups =
            BOUNDARY_CHECK(( SetDevParam.Hard_Input_Group ),( UNSIGNED )( PLAT_cfg.input.InputGroups ) ); /* Ӳ������������ */
        PLAT_cfg.input.SoftInputGroups =
            ( PLAT_cfg.input.InputGroups ) - ( PLAT_cfg.input.HardInputGroups ); /* ������������� */

        pInputPort = PLAT_cfg.input.InputPort;
        pInputPort += PLAT_cfg.input.HardInputGroups;
        for( k = 0; k < ( PLAT_cfg.input.SoftInputGroups ); k ++)
        {
            /* ����������ӳ�� */
            pInputPort -> PortNumber = PLAT_SoftInputPortCfg[k].PortNumber;
            pInputPort -> PortBits = PLAT_SoftInputPortCfg[k].PortBits;
            pInputPort -> PortMasks = PLAT_SoftInputPortCfg[k].PortMasks;
            pInputPort -> ReverseBits = PLAT_SoftInputPortCfg[k].ReverseBits;
            pInputPort -> PortAddress[0] = PLAT_SoftInputPortCfg[k].PortAddress[0]; /* �˿ڵ�ַ1 */
            pInputPort -> PortAddress[1] = PLAT_SoftInputPortCfg[k].PortAddress[1]; /* �˿ڵ�ַ2 */
            pInputPort -> PortAddress[2] = PLAT_SoftInputPortCfg[k].PortAddress[2]; /* �˿ڵ�ַ3 */
            pInputPort -> PortAddress[3] = PLAT_SoftInputPortCfg[k].PortAddress[3]; /* �˿ڵ�ַ4 */
            pInputPort -> QualityAddress = PLAT_SoftInputPortCfg[k].QualityAddress;
            pInputPort -> QualityCheckAddr = PLAT_SoftInputPortCfg[k].QualityCheckAddr; /* Ʒ�����ؼ���λ��ַ */
            pInputPort ++;
        }
        PLAT_cfg.output.OutputNumber = SetDevParam.Max_Output; /* ���������� */
    }

    /* ����ģ��������ͨ�� */
    if( status == NU_SUCCESS )
    {
        status = APFC_GetParameter( ANALOG_CHANNEL_SET_ID,& AnalogSet );
        if( status == NU_SUCCESS )
        {
            PLAT_cfg.smvc.AdNumber =
                BOUNDARY_CHECK(( AnalogSet.AD_Number ), CPC_MAX_AD_NUMBER ); /* ʹ��ADƬ�� */
            PLAT_cfg.smvc.PhysicalAnalogNumber =
                BOUNDARY_CHECK(( AnalogSet.Analog_Number ), CPC_MAX_PHYSICAL_CHL_NUMBER ); /* �����ģ����ͨ���� */
            /* ˫����SVͨ������ */
            PLAT_cfg.smvc.DSampleSVChlNum = BOUNDARY_CHECK(( AnalogSet.DSampleSVChlNum ), ( AnalogSet.Analog_Number ));
            /* ˫������ʶ����false,���б�����Ƿ�Ϊ˫������ʱ�������� */
            PLAT_cfg.smvc.DSampleFlag = NU_FALSE;
            PLAT_cfg.smvc.VirtualAnalogNumber =
                BOUNDARY_CHECK(( AnalogSet.Virtual_Analog_Number ), CPC_MAX_VIRTUAL_CHL_NUMBER ); /* �����ģ����ͨ���� */
            PLAT_cfg.smvc.AnalogQualityGroups =
                ( PLAT_cfg.smvc.PhysicalAnalogNumber + PLAT_cfg.smvc.VirtualAnalogNumber + 31 )/ 32; /* ģ����Ʒ���������� */

            PLAT_cfg.smvc.PeriodPoint =
                BOUNDARY_CHECK(( AnalogSet.Sample_Point ), CPC_MAX_SAMPLE_POINT ); /* ÿ���ڲ������� */
            PLAT_cfg.smvc.LineFrequency = AnalogSet.LineFrequency;     /* ��·Ƶ�� */
            PLAT_cfg.smvc.SampleRate = AnalogSet.SampleRate;           /* ������(ÿ���������) */
            PLAT_cfg.smvc.PowerCheckNumber = 0;
            PowerChannel = PLAT_cfg.smvc.PowerChannel;
            for( k = 0; k < ( AnalogSet.PowerNumber ); k ++ )
            {
                if((( AnalogSet.PowerChannel + k )->ChannelNumber != - 1 )
                    && ( PLAT_cfg.smvc.PowerCheckNumber < MAX_POWER_CHECK_NUMBER ))
                {
                    PowerChannel -> ChannelNumber = ( AnalogSet.PowerChannel + k )->ChannelNumber;
                    PowerChannel -> MaxValue = ( AnalogSet.PowerChannel + k )->MaxValue;
                    PowerChannel -> MinValue = ( AnalogSet.PowerChannel + k )->MinValue;
                    PLAT_cfg.smvc.PowerCheckNumber ++;
                    PowerChannel ++;
                }
            }

            PLAT_cfg.frequency.FrequencyNumber = 0;
            FrequencyChannel = PLAT_cfg.frequency.FrequencyChannel;
            /* ��Ƶ��ѹͨ��1 */
            number = PLAT_cfg.smvc.PhysicalAnalogNumber;
            if((( AnalogSet.Frequency_Num1 ) < number ) && (( AnalogSet.Frequency_Num1 ) >= 0 ))
            {
                FrequencyChannel -> ChannelNumber = AnalogSet.Frequency_Num1;
                FrequencyChannel -> FrequencyDlt = AnalogSet.FrequencyDlt; /* ����Ƶ�����仯��(HZ/��) */
                ( PLAT_cfg.frequency.FrequencyNumber )++;
                FrequencyChannel ++;
            }

            /* ��Ƶ��ѹͨ��2 */
            if((( AnalogSet.Frequency_Num1 ) < number ) && (( AnalogSet.Frequency_Num2 ) >= 0 ))
            {
                FrequencyChannel -> ChannelNumber = AnalogSet.Frequency_Num2;
                FrequencyChannel -> FrequencyDlt = AnalogSet.FrequencyDlt; /* ����Ƶ�����仯��(HZ/��) */
                ( PLAT_cfg.frequency.FrequencyNumber )++;
                FrequencyChannel ++;
            }
        }
    }

    /* ����װ�ó���λ��ز��� */
    if( status == NU_SUCCESS )
    {
        status = APFC_GetParameter( OUT_BITS_SET_ID,& SetOutBits );
        if( status == NU_SUCCESS )
        {
            HardGroups = SetOutBits.Hard_Output_Group;
            SoftGroups = SetOutBits.Soft_Output_Group;
            Groups = HardGroups + SoftGroups;
            PLAT_cfg.output.OutputGroups =
                BOUNDARY_CHECK( Groups, CPC_MAX_OUTPUT_GROUPS ); /* ���������� */
            PLAT_cfg.output.HardOutputGroups =
                BOUNDARY_CHECK(( SetOutBits.Hard_Output_Group ),( UNSIGNED )PLAT_cfg.output.OutputGroups ); /* Ӳ������������ */
            Groups = SetOutBits.Hard_Extend_Count;
            /* ��չ������� */
            PLAT_cfg.output.HardExtendCount = BOUNDARY_CHECK( Groups, CPC_MAX_OUTPUT_GROUPS );
            PLAT_cfg.output.SoftOutputGroups =
                ( PLAT_cfg.output.OutputGroups ) - ( PLAT_cfg.output.HardOutputGroups ); /* ������������� */
            /* �����˿�ӳ�� */
            pOutputPort = PLAT_cfg.output.OutputPort;
            pOutputPort += PLAT_cfg.output.HardOutputGroups;
            for( k = 0; k < ( PLAT_cfg.output.SoftOutputGroups ); k ++)
            {
                /* ���ֿ����˿� */
                pOutputPort -> PortNumber = PLAT_SoftOutputPortCfg[k].PortNumber;
                pOutputPort -> PortBits = PLAT_SoftOutputPortCfg[k].PortBits;
                pOutputPort -> PortMasks = PLAT_SoftOutputPortCfg[k].PortMasks;
                pOutputPort -> ReverseBits = PLAT_SoftOutputPortCfg[k].ReverseBits;
                pOutputPort -> PortAddress[0] = PLAT_SoftOutputPortCfg[k].PortAddress[0]; /* �˿ڵ�ַ1 */
                pOutputPort -> PortAddress[1] = PLAT_SoftOutputPortCfg[k].PortAddress[1]; /* �˿ڵ�ַ2 */
                pOutputPort -> PortAddress[2] = PLAT_SoftOutputPortCfg[k].PortAddress[2]; /* �˿ڵ�ַ3 */
                pOutputPort -> PortAddress[3] = PLAT_SoftOutputPortCfg[k].PortAddress[3]; /* �˿ڵ�ַ4 */
                pOutputPort -> QualityAddress = PLAT_SoftOutputPortCfg[k].QualityAddress;
                pOutputPort -> QualityCheckAddr = PLAT_SoftOutputPortCfg[k].QualityCheckAddr; /* ����Ʒ�����ؼ���λ��ַ */
                pOutputPort ++;
            }

            *( PLAT_cfg.output.HardOutputCheckMasks + 0 ) = SetOutBits.Out_Check_Bits;  /* װ�ó��ڼ��λ(0-31) */
            *( PLAT_cfg.output.HardOutputCheckMasks + 1 ) = SetOutBits.Out_Check_Bits1; /* װ�ó��ڼ��λ(32-63) */
            *( PLAT_cfg.output.HardOutputCheckMasks + 2 ) = SetOutBits.Out_Check_Bits2; /* װ�ó��ڼ��λ(64-95) */

            if( SetOutBits.Signal_Led_Bits != 0 )
            {
                Groups = ( SetOutBits.Signal_Led_Bits - 1 ) >> CPC_GROUP_MASK32;
                SubGroups = ( 1 << ( ( SetOutBits.Signal_Led_Bits - 1 ) & CPC_SUB_GROUP_MASK32 ));
                if( Groups < PLAT_cfg.output.HardOutputGroups )
                {
                    *( PLAT_cfg.output.HardOutputSignalLed + Groups ) = SubGroups;  /* �źŵƳ���λ */
                }
            }

            if( SetOutBits.Trip_Led_Bits != 0 )
            {
                Groups = ( SetOutBits.Trip_Led_Bits - 1 ) >> CPC_GROUP_MASK32;
                SubGroups = ( 1 << ( ( SetOutBits.Trip_Led_Bits - 1 ) & CPC_SUB_GROUP_MASK32 ));
                if( Groups < PLAT_cfg.output.HardOutputGroups )
                {
                    *( PLAT_cfg.output.HardOutputTripLed + Groups ) = SubGroups;    /* ��բ�Ƴ���λ */
                }
            }

            *( PLAT_cfg.output.HardOutputAlarm + 0 ) = SetOutBits.Alarm_Relay_Bits; /* �澯�̵�������λ */
            *( PLAT_cfg.output.HardOutputReset + 0 ) = SetOutBits.Reset_Relay_Bits; /* ����̵�������λ */
            PLAT_cfg.input.ResetInputBits = SetOutBits.Reset_Input_Bits; /* ����̵�������λ */
            /* ƽ̨���ڹ����� */
            PlatHIOM = XJP_IO_Create_Out( PLAT_cfg.output.OutputGroups );
            /* ���ڼ���ʼ������ */
            OUTC_CheckInitialize( ( PLAT_cfg.output.HardOutputGroups ),( PLAT_cfg.output.OutputGroups ),
                ( PLAT_cfg.output.HardOutputCheckMasks ), CPC_MAX_CHECK_RELAY,
                & Event_1_CB, CEMC_FillEvent, NU_Set_Events, pReport_Protect, CSMV_Msg_SendGooseOut,
                ( PLAT_cfg.output.HardExtendCount ) );
        }
    }

    /* �������ݱ�������� */
    if( status == NU_SUCCESS )
    {
        status = APFC_GetParameter( SET_TABLE_PARAMNUM_ID,& SetTabNumber );
        if( status == NU_SUCCESS )
        {
            PLAT_cfg.table.TableNumber = SetTabNumber.Max_Table_Number;
            PLAT_cfg.table.FieldNumber = SetTabNumber.Max_Field_Number;
        }
    }

    return( status );
}

/* ƽ̨���ò����ĳ�ʼ�� */
INT PMI_InitializePlatCfg( VOID )
{
    INT i;

    /* Initialize the protect option.  */
    PLAT_cfg.device.CpuNumber = 1;                      /* CPU���� */
    PLAT_cfg.device.CpuType = DEVICE_CPU_TYPE;          /* CPU������ */
    PLAT_cfg.device.FunSelect = 0;                      /* ����ѡ�� */
    PLAT_cfg.device.DeviceFun = 0;                      /* װ�ù������� */
    PLAT_cfg.device.SetZoneCount = 1;                   /* װ�ö�ֵ������ */

    PLAT_cfg.smvc.AdNumber = 0;                         /* ʹ��ADƬ�� */
    PLAT_cfg.smvc.PhysicalAnalogNumber = 0;             /* �����ģ����ͨ���� */
    PLAT_cfg.smvc.VirtualAnalogNumber = 0;              /* �����ģ����ͨ���� */
    PLAT_cfg.smvc.AnalogQualityGroups = 0;              /* ģ����Ʒ��������ͨ������ */
    PLAT_cfg.smvc.PeriodPoint = CPC_MAX_SAMPLE_POINT;   /* ÿ���ڲ������� */
    PLAT_cfg.smvc.ChannelPoint = CPC_MAX_CHANNEL_POINT; /* ÿ��ͨ���Ĳ���ֵ���� */
    PLAT_cfg.smvc.SampleCounter = PLAT_MAX_SAMPLE_COUNTER; /* ���Ĳ������ֵ */
    PLAT_cfg.smvc.PowerCheckNumber = 0;                 /* ��Դ�Լ��ͨ������ */
    PLAT_cfg.smvc.PowerChannel = PLAT_PowerCfg;         /* ��Դ�Լ�������Ϣ */
    PLAT_cfg.smvc.LoseSV_Quality = XJ_FALSE;            /* �������ֵ����Ʒ�����ز����� */
    PLAT_cfg.smvc.LoseOneSV_Insert = XJ_TRUE;           /* ��һ�����ֵ���ݲ�ֵ���� */

    PLAT_cfg.frequency.FrequencyNumber = 0;             /* ��Ƶͨ������ */
    PLAT_cfg.frequency.FrequencyChannel = PLAT_FreqChlCfg; /* ��Ƶͨ�����ò���. */

    PLAT_cfg.input.InputNumbers = 0;                    /* ���������� */
    PLAT_cfg.input.InputGroups = 0;                     /* ���������� */
    PLAT_cfg.input.WaveInputGroups = 0;                 /* ¼������������ */
    PLAT_cfg.input.InputPort = PLAT_InputPortCfg;       /* �������˿����� */
    PLAT_cfg.input.HardInputGroups = 0;                 /* Ӳ������������ */
    PLAT_cfg.input.SoftInputGroups = 0;                 /* ������������� */
    PLAT_cfg.input.ResetInputBits = 0;                  /* ���鿪��λ(��1��ʼ) */

    PLAT_cfg.output.OutputNumber = 0;                   /* ���������� */
    PLAT_cfg.output.OutputGroups = 0;                   /* ���������� */
    PLAT_cfg.output.OutputPort = PLAT_OutputPortCfg;    /* �������˿����� */
    PLAT_cfg.output.HardOutputGroups = 0;               /* Ӳ������������ */
    PLAT_cfg.output.HardOutputCheckMasks = PLAT_OutputCheckMasks; /* Ӳ�����������λ���� */
    PLAT_cfg.output.HardOutputHoldMasks = PLAT_OutputHoldMasks; /* Ӳ������������λ���� */
    PLAT_cfg.output.HardOutputSignalLed = PLAT_OutputSignalLed; /* Ӳ���������źŵ� */
    PLAT_cfg.output.HardOutputTripLed = PLAT_OutputTripLed; /* Ӳ����������բ�� */
    PLAT_cfg.output.HardOutputAlarm = PLAT_OutputAlarm; /* �澯�̵�������λ */
    PLAT_cfg.output.HardOutputReset = PLAT_OutputReset; /* ����̵�������λ */
    PLAT_cfg.output.SoftOutputGroups = 0;               /* ������������� */
    for( i = 0; i < CPC_MAX_OUTPUT_GROUPS; i ++)
    {
        PLAT_OutputCheckMasks[i] = 0;
        PLAT_OutputHoldMasks[i] = 0;
        PLAT_OutputSignalLed[i] = 0;
        PLAT_OutputTripLed[i] = 0;
        PLAT_OutputAlarm[i] = 0;
        PLAT_OutputReset[i] = 0;
    }
    /* ��ʼ�����ֿ�����Ʒ������Ϊ��Ч */
    for( i = 0; i < CPC_MAX_INPUT_GROUPS; i ++)
    {
        PLAT_SoftInputQuality[i] = 0; /* ��ЧƷ������ */
        PLAT_SoftInputCheckQuality[i] = 0; /* ����Ʒ������ */
    }
    /* ��ʼ�����ֿ�����Ʒ������Ϊ��Ч */
    for( i = 0; i < CPC_MAX_OUTPUT_GROUPS; i ++)
    {
        PLAT_SoftOutputQuality[i] = 0;       /* ��ЧƷ������ */
        PLAT_SoftOutputCheckQuality[i] = 0;  /* ����Ʒ������ */
    }

    PLAT_cfg.history.UserEventGroups = 0;               /* �û��¼����� */
    PLAT_cfg.history.TempEventNumber = CPC_MAX_RAM_EVENT_NUMBER; /* ��ʱ�����¼����� */
    PLAT_cfg.history.TempReportNumber = CPC_MAX_RAM_REPORT_NUMBER; /* ��ʱ���汨����� */

    PLAT_cfg.table.TableNumber = MAX_TABLE_NUMBER;      /* ���ݱ���� */
    PLAT_cfg.table.FieldNumber = MAX_FIELD_NUMBER;      /* �ֶθ��� */

    return( NU_SUCCESS );
}

/* ��ȡװ�ù���ѡ�� */
INT PMI_get_device_fun_select( VOID )
{
    return PLAT_cfg.device.FunSelect;
}
