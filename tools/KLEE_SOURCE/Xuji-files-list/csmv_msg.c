
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
#include "plat/inc/xj_out_check.h"
#include "plat/inc/cout_extr.h"
#include "plat/es_plat.h"
#include "plat/inc/pro_defs.h"
#include "plat/inc/csmv_extr.h"
#include "plat_specific/plat_specific.h"
#include "plat/inc/esp_cfg.h"
#include "plat/net_inc/ls_defs.h"
#include "xml/inc/pxml_cfg.h"
#include "plat/inc/tab_defs.H"
#ifndef  WIN32
#include "eth_npi_if.h"
#endif

#define CSMC_O_COMMAND                  0       /* ��������λ��ƫ�� */
#define CSMC_COMMAND_ID_0x83            0x83    /* ����SNTP��ʱ���� */
#define CSMC_COMMAND_ID_0x88            0x88    /* ����GOOSE���� */
#define CSMC_COMMAND_ID_0x8C            0x8C    /* ����SV���� */
#define CSMC_COMMAND_ID_0x8D            0x8D    /* ���մ�ʱ���GOOSE���� */
#define CSMV_COMMAND_ID_0x8B            0x8B    /* �ǵ������� */
#define CSMV_COMMAND_ID_0x8E            0x8E    /* ����߾��Ȳ������� */
#define CSMV_COMMAND_ID_0x8F            0x8F    /* ����SVͳ�Ʊ��� */
#define CSMV_COMMAND_ID_0x90            0x90    /* ����GOOSEͳ�Ʊ��� */
#define CSMC_COMMAND_ID_0x8A            0x8A    /* ����GOOSE���� */
#define CSMC_ANALOG_COEF_BYTES          4       /* ģ����ϵ���ֽ��� */
#define CSMC_ANALOG_DATA_BYTES          4       /* ģ���������ֽ��� */
#define CSMC_ANALOG_QUA_BYTES           4       /* ģ����Ʒ�������ֽ��� */
/* 9=(֡����:1)+(ģ�������к�:2)+(ģ����ͨ������:1)+(ģ����Ʒ����������:1) + ͨ��ϵ������ */
#define CSMC_ANALOG_MIN_FRAME           9       /* ģ������С֡�� */
#define CSMC_GOOSE_QUA_BYTES            4       /* GOOSEƷ�������ֽ��� */
#define CSMC_GOOSE_DATA_BYTES           4       /* GOOSE�����ֽ��� */
#define CSMC_GOOSE_TEST_BYTES           4       /* GOOSE�����ֽ��� */
#define CSMC_GOOSE_MIN_FRAME            2       /* GOOSE��С֡��:2=(֡����:1)+(����:1) */
#define CSMC_SNTP_FRAME_LENGTH          17      /* SNTP����֡�� */
#define CSMC_DEVICE_START_YEAR          2000    /* װ�õ���ʼ��� */
#define CSMC_SNTP_SECOND_BYTES          4       /* SNTP����ֽ��� */
#define CSMC_SNTP_NSECOND_BYTES         4       /* SNTP���������ݵ��ֽ��� */
#define CSMC_SNTP_QUALITY_BYTES         1       /* ʱ��Ʒ�������ֽ��� */
#define CSMV_DEV_STATUS_BYTES           4       /* װ��״̬�ֽ��� */
#define CSMV_MAX_FRAME_LEN_UN           ((CPC_MAX_LENGTH_FRAME + sizeof(UNSIGNED)-1)/ sizeof(UNSIGNED)) /* ���ĳ���(UNSIGNED) */

/* 8D����ÿ�����ݳ��� */
/* ����λ(2�ֽ�)+״̬(1�ֽ�)+Ʒ��������Ч��Чλ(1�ֽ�)+����λ(1�ֽ�)+ʱ����(4�ֽ�)+ʱ������(4�ֽ�)+ʱ��״̬(1�ֽ�) */
#define CSMV_8D_DATA_LEN                14
#define CSMV_8D_MIN_LEN                 2       /* 8D������С����(֡����(1�ֽ�)+���ݸ���(1�ֽ�)) */

#define CSMV_GROUP_MASK32               5       /* the group mask bit for 32-bits */
#define CSMV_SUB_GROUP_MASK32           31      /* the mask of sub-group 32-bits */

/* 0x89����ƫ�� */
#define CSMC_O_8C_CALC_COEF             1       /* ����ϵ�� */
#define CSMC_O_8C_ANALOG_SERIAL_NUMBER  5       /* ģ�������к� */
#define CSMC_O_8C_CHANNEL_NUMBER        7       /* ģ����ͨ������ */
#define CSMC_O_8C_QUA_GROUP_NUMBER      8       /* ģ����Ʒ���������� */
#define CSMC_O_8C_QUA_VALUE             9       /* ģ����Ʒ������ֵ */
/* 0x82����ƫ�� */
#define CSMC_O_88_GROUP_NUMBER          1       /* ���������� */
#define CSMC_O_88_INPUT_VALUE           2       /* ������ֵ */
/* �汾�����Ͷ��� */
#define CSMC_VERSION_FLOAT              0  /* �汾�ţ������� */
#define CSMC_VERSION_INT                1  /* �汾�ţ������� */

/* GOOSE�������ʹ��������ͼ�� */
#define CSMC_OUT_COUNT_ON_CHANGE    6       /* �б�λʱ��������6�� */
#define CSMC_OUT_SPACE_ON_CHANGE_1  1       /* �б�λʱǰ���η��ͼ��Ϊ1���� */
#define CSMC_OUT_SPACE_ON_CHANGE_2  2       /* �б�λʱ�����η��ͼ��Ϊ2���� */
#define CSMC_OUT_SPACE_ON_CHANGE_4  4       /* �б�λʱ���Ĵη��ͼ��Ϊ4���� */
#define CSMC_OUT_SPACE_NORMAL       500     /* ��λ�����������κ�ת�볣̬��ÿ500�뷢��һ��*/

/* ���Ͷ�ʱ������ʱʱ�� */
#define CSMC_SYNTIME_DELAY_TIME     1000    /* 1���ӷ���һ�ζ�ʱ���� */
#define CSMV_MSG_INPUT_DELAY        3000    /* �����յ���������Ϣ����ʱʱ�� */

/* ��NPI�������ݽӿں��� */
INT (* CSMV_TONpiSendData )( UNSIGNED_CHAR * data, INT length );

extern  volatile UNSIGNED TMD_System_Clock;

/* ����ΪOS������� */
#ifdef WIN32
/* ȫ�ֱ������� */
/* Windows �궨�� */
#endif

#ifndef WIN32 /* ��windowsȱʡ����һЩ���Ա�ͨ������ */
/* ȫ�ֱ������� */
/* Nucleus�궨�� */
#endif

float CSMC_Analog[ CPC_MAX_CHL_NUMBER ];
static UNSIGNED CSMC_SampleSerial;

UNSIGNED CSMV_Rxd_Time_SV;    /* SV�������ʱ��(0x89)  */
UNSIGNED CSMV_Rxd_Time_GOOSE; /* GOOSE�������ʱ��(0x82) */
UNSIGNED CSMV_Rxd_Time_SNTP;  /* SNTP�������ʱ��(0x83) */
UNSIGNED CSMV_MessageBuf[CSMV_MAX_FRAME_LEN_UN + 2];  /* ���Ļ����� */
UNSIGNED CSMV_MSG_DevQStatus;       /* װ��״̬ */
extern NU_PROTECT * pReport_Protect;
INT CSMV_MsgStateChlQStartGroup = 0;/* ͨ��Ʒ������¼��״̬����ʼ��� */
INT CSMV_MsgStateChlQGroupCount = 0;/* ͨ��Ʒ������¼��״̬������ */

extern UNSIGNED PLAT_SoftInputMemory[];         /* ���ֿ������˿� */
extern UNSIGNED PLAT_SoftInputQuality[];        /* ���ֿ�����Ʒ��������Ч��Чλ */
extern UNSIGNED PLAT_SoftInputCheckQuality[];   /* ���ֿ�����Ʒ�����ؼ���λ */
#define GET_SOFT_INGROUP()     PLAT_cfg.input.SoftInputGroups  /* ��ȡ�������� */
/*==================================================================*/
extern HDCM NPIUserDataHandle;                  /* ����NPI�û����ݹ����� */
/* Ӧ����NPI�������ݻ����� */
UNSIGNED CSMV_Msg_UserSendDataBuffer[CSMV_MAX_FRAME_LEN_UN + 2] = {0};
/*==================================================================*/
/* �ж�Ʒ�����ؼ�״̬�Ƿ����仯(NU_TRUE:�仯;NU_FALSE:û�б仯) */
static INT CSMV_Msg_OutStatusCheck( UNSIGNED * pStatus, UNSIGNED * pQuality, UNSIGNED * pCheckQua, INT iGroupNum );
/* �������ݻ���������д��������š�61850ʱ��(�롢����)��������״̬��������Ʒ��Ʒ������(���ر��ĳ���) */
static INT CSMC_MSG_FillOutStatus( UNSIGNED_CHAR * pBuffer, UNSIGNED * pOutStatus,
    UNSIGNED * pQuality, UNSIGNED * pCheckQua, INT iGroupNum );
/* �������ݺ��� */
INT CSMV_SendDataFun( UNSIGNED_CHAR * data, INT length );
/* ���տ�������ʱ��⺯�� */
VOID CSMV_MSG_Input_TimeOutCheck( VOID );
/*==================================================================*/
/* ���Ϳ���������ʱʱ������,����λȫ�ֱ����������Բ����Ե�ʱ���޸�ʱ���� */
UNSIGNED CSMV_MG_OutSendDelayTime[CSMC_OUT_COUNT_ON_CHANGE + 1] =
{
    0, CSMC_OUT_SPACE_ON_CHANGE_1, CSMC_OUT_SPACE_ON_CHANGE_1,
    CSMC_OUT_SPACE_ON_CHANGE_2, CSMC_OUT_SPACE_ON_CHANGE_4,
    CSMC_OUT_SPACE_NORMAL, CSMC_OUT_SPACE_NORMAL
};

/* Define internal function calls.  */
/*==================================================*/
/* ����GOOSE���� */
#ifndef  WIN32
#define CSMC_MSG_SEND_GOOSE(x,y,z)  x=CSMV_SendDataFun(y,z)
#else
#define CSMC_MSG_SEND_GOOSE(x,y,z)  x=NU_TRUE
#endif
/*==================================================*/
CSMV_MSG_COUNT  CSMV_MsgCount;
/*==================================================*/
/* ���Ľ������� */
VOID CSMV_MessParse( UNSIGNED_CHAR * data, UINT length )
{
    INT i;
    UNSIGNED_CHAR * p;
    UNSIGNED_CHAR * ptr;
    UINT counter;
    INT  iValue;
    UNSIGNED SampleSerial;
    float   fCalcCoef;
    INT     oldLevel;
    static UNSIGNED uLastSerial = 0;
    UNSIGNED uMillisecond;
    UNSIGNED uMicrosecond;

    /* ���㺯��ִ��ʱ��ʹ�� */
    XJP_Dbg_Time_Testing(& uMillisecond, & uMicrosecond );

    /* �жϳ����Ƿ�������Χ֮�� */
    if( length > CPC_MAX_LENGTH_FRAME )
    {
        /* �������ȴ�������� */
        CSMV_MsgCount.iLenErr ++;
    }
    /* ���ݻ����� */
    ptr = data;
    /* �ж��������� */
    switch(* ( ptr + CSMC_O_COMMAND ))
    {
        /* SV���� */
    case CSMC_COMMAND_ID_0x8C:
        {
            UNSIGNED_CHAR uChlNum;
            UNSIGNED_CHAR uQlityGroup;
            INT           iGroup;

            CSMV_MsgCount.iRecSmvCount[0] ++;
            /* ģ����ͨ������ */
            uChlNum = * ( ptr + CSMC_O_8C_CHANNEL_NUMBER );
            /* ��¼���յ���ģ����ͨ������ */
            CSMV_MsgCount.iRecSmvCount[1] = uChlNum;
            /* ģ����ͨ��Ʒ���������� */
            uQlityGroup = * ( ptr + CSMC_O_8C_QUA_GROUP_NUMBER );
            /* ����ͨ���Ļ�����Ϣ������ı��ĳ��� */
            counter = ( uChlNum * CSMC_ANALOG_DATA_BYTES ) + CSMC_ANALOG_MIN_FRAME
                + ( CSMC_ANALOG_QUA_BYTES * uQlityGroup * 2 ); /* 2:Ʒ������+���� */
            /* �ж����ݳ����Ƿ���ȷ
               �ж�ͨ�������Ƿ�������ķ�Χ֮��
               �ж�ģ����ͨ��Ʒ�����������Ƿ�������ķ�Χ֮��
               �ж����ݳ����Ƿ���ȷ */
            if( ( length > CSMC_ANALOG_MIN_FRAME )
                && ( uChlNum <= CPC_MAX_PHYSICAL_CHL_NUMBER )
                && ( uQlityGroup <= CSMV_ANALOG_MAX_GROUPS )
                &&( counter == length ))
            {
                /* �洢ͨ����ֵ����Ϊ֧�ֵ�������ͨ������ */
                /* ���ݳ�����ȷ,�洢ģ�������� */
                /* �洢ģ�������к� */
                XJP_CSC_Byte_To_Word(& SampleSerial,( ptr + CSMC_O_8C_ANALOG_SERIAL_NUMBER ), 2 ); /* 2�ֽ� */
                /* ������ָ�������Ʒ������״̬��λ�� */
                p = ptr + CSMC_O_8C_QUA_VALUE;
                /* �洢Ʒ������ */
                counter = uQlityGroup * CSMC_ANALOG_QUA_BYTES;
                XJP_CSC_Byte_To_Word( CSMV_GET_ORIGINL_ANA_QUA_MEM, p, counter );
                p += counter;

                /* ��Ʒ��������Ч��Чλ״̬ӳ�䵽��Ӧ��״̬���� */
                iGroup = CSMV_MsgStateChlQGroupCount;
                /* �ж�Ʒ�����������Ƿ񳬳���Χ */
                if( iGroup > uQlityGroup )
                {
                    iGroup = uQlityGroup;
                }
                /* �洢���� */
                for( i = 0; i < iGroup; i ++)
                {
                    CSMV_GET_ORIGINL_PRO_STS_MEM[CSMV_MsgStateChlQStartGroup + i]
                        = CSMV_GET_ORIGINL_ANA_QUA_MEM[i];
                }
                /* �洢����״̬ */
                XJP_CSC_Byte_To_Word( CSMV_GET_ORIGINL_ANA_CHECKQUA_MEM, p, counter );
                p += counter;
                /* ��ȡ����ϵ�� */
                XJP_CSC_Byte_To_Word(& iValue, ( ptr + CSMC_O_8C_CALC_COEF ), CSMC_ANALOG_COEF_BYTES );
                fCalcCoef = ( float )iValue;
                /* ����ϵ������0����� */
                if( fCalcCoef < 0.1 )
                {
                    fCalcCoef = 1.0;
                }

                if( CSMV_GET_DSAMPLE_FLAG == NU_TRUE )
                {
                    /* ���ж���Ϊʹ�õ���ͬһ������ */
                    oldLevel = NU_Local_Control_Interrupts( 0 );
                    /* ����������� */
                    if((++ uLastSerial ) >= ( UNSIGNED )CSMV_GET_SAMPLE_COUNTER )
                    {
                        uLastSerial -= CSMV_GET_SAMPLE_COUNTER;
                    }
                    /* �жϲ�������Ƿ����� */
                    if( uLastSerial != SampleSerial )
                    {
                        CSMV_MsgCount.iRecSmvCount[2] ++;
                    }
                    uLastSerial = SampleSerial;
                    memset( CSMC_Analog, 0, sizeof( CSMC_Analog ));
                    /* ������ */
                    for( i = 0; i < uChlNum; i ++)
                    {
                        XJP_CSC_Byte_To_Word(& iValue, p, CSMC_ANALOG_DATA_BYTES );
                        CSMC_Analog[i] = ( float )iValue;
                        CSMC_Analog[i] /= fCalcCoef;
                        p += CSMC_ANALOG_DATA_BYTES;
                    }
                    CSMC_SampleSerial = SampleSerial;
                    NU_Local_Control_Interrupts( oldLevel );
                }
                else
                {
                    memset( CSMC_Analog, 0, sizeof( CSMC_Analog ));
                    /* ������ */
                    for( i = 0; i < uChlNum; i ++)
                    {
                        XJP_CSC_Byte_To_Word(& iValue, p, CSMC_ANALOG_DATA_BYTES );
                        CSMC_Analog[i] = ( float )iValue;
                        CSMC_Analog[i] /= fCalcCoef;
                        p += CSMC_ANALOG_DATA_BYTES;
                    }
                    /* ģ�������ݴ洢���, ��AD�жϺ��� */
                    SMVC_SampleUpdateSoft( CSMC_Analog, SampleSerial );
                }

                /* ����SMV����ʱ�� */
                CSMV_Rxd_Time_SV = XJ_Retrieve_Clock();
                /* ���ò���׼����ɱ�ʶ */
                CSMV_SetSampleInitFlag( NU_TRUE );
            }
        }
        break;
        /* GOOSE���� */
    case CSMC_COMMAND_ID_0x88:
        {
            INT  iGroupNum;
            UNSIGNED uInputStatus;
            UNSIGNED uTempStatus;
            UNSIGNED MaskBits;

            CSMV_MsgCount.iGooseRecCount[0] ++;
            /* ���������� */
            iGroupNum = ( INT )(* ( ptr + CSMC_O_88_GROUP_NUMBER ));
            /* ��¼���յ��Ŀ������� */
            CSMV_MsgCount.iGooseRecCount[1] = iGroupNum;
            /* ����ͨ���Ļ�����Ϣ������ı��ĳ��� */
            counter = ( CSMC_GOOSE_DATA_BYTES + CSMC_GOOSE_QUA_BYTES + CSMC_GOOSE_TEST_BYTES )* iGroupNum
                + CSMC_GOOSE_MIN_FRAME;
            /* �жϳ����Ƿ���ȷ,�����������Ƿ�������ķ�Χ֮�� */
            if(( counter == length )&&( iGroupNum <= ( INT )CPC_MAX_INPUT_GROUPS ))
            {
                /* ������ָ�������������״̬��λ�� */
                p = ptr + CSMC_O_88_INPUT_VALUE;
                for( i = 0; i < iGroupNum; i ++)
                {
                    MaskBits = 0;
                    /* ��ȡ�������鴫��ʱ���� */
                    CINC_GetSoftInFullMasks( i,& MaskBits );
                    /* ������״̬ */
                    XJP_CSC_Byte_To_Word(& uInputStatus, p, CSMC_GOOSE_DATA_BYTES );
                    /*  ��ȡ����λ��״̬ */
                    uInputStatus &= MaskBits;
                    /* ��ԭ״̬��ȥ������λ״̬ */
                    uTempStatus = ( PLAT_SoftInputMemory[i] & ( ~ MaskBits ));
                    /* ��������λ״̬ */
                    uTempStatus |= uInputStatus;
                    /* ��������״̬ */
                    PLAT_SoftInputMemory[i] = uTempStatus;
                    p += CSMC_GOOSE_DATA_BYTES;
                    /* ������Ʒ��������Ч��Чλ */
                    XJP_CSC_Byte_To_Word(& uInputStatus , p, CSMC_GOOSE_QUA_BYTES );
                    /*  ��ȡ����λ��״̬ */
                    uInputStatus &= MaskBits;
                    /* ��ԭ״̬��ȥ������λ״̬ */
                    uTempStatus = ( PLAT_SoftInputQuality[i] & ( ~ MaskBits ));
                    /* ��������λ״̬ */
                    uTempStatus |= uInputStatus;
                    /* ��������״̬ */
                    PLAT_SoftInputQuality[i] = uTempStatus;
                    p += CSMC_GOOSE_QUA_BYTES;
                    /* ������Ʒ�����ؼ���λ */
                    XJP_CSC_Byte_To_Word(& uInputStatus, p, CSMC_GOOSE_TEST_BYTES );
                    /*  ��ȡ����λ��״̬ */
                    uInputStatus &= MaskBits;
                    /* ��ԭ״̬��ȥ������λ״̬ */
                    uTempStatus = ( PLAT_SoftInputCheckQuality[i] & ( ~ MaskBits ));
                    /* ��������λ״̬ */
                    uTempStatus |= uInputStatus;
                    /* ��������״̬ */
                    PLAT_SoftInputCheckQuality[i] = uTempStatus;
                    /* ����������״̬ */
                    p += CSMC_GOOSE_TEST_BYTES;
                }
                /* ���¿���������ʱ�� */
                CSMV_Rxd_Time_GOOSE = XJ_Retrieve_Clock();
            }
        }
        break;
        /* SNTP��ʱ���� */
    case CSMC_COMMAND_ID_0x83:
        {
            UNSIGNED UTCSecond;
            UNSIGNED UTCNanoSecond;
            TIME devTime;
            UNSIGNED uQuality;

            /* �ж϶�ʱ��Դ */
            if( pDeviceTab )
            {
                /* �ж϶�ʱ��Դ����ʱ�������ΪSNTP��ʱ���Ͳ���NPI���ȡ */
                if(( pDeviceTab->SynSrc & PRO_SYN_SRC_PROCESS ) == 0 )
                {
                    return;
                }
            }
            CSMV_MsgCount.iRecSynCount ++;
            /* �жϳ����Ƿ���ȷ */
            if( length == CSMC_SNTP_FRAME_LENGTH )
            {
                p = ptr + 1; /* ָ��ʱ������ */
                /* �� */
                XJP_CSC_Byte_To_Word(&( devTime.Year ), p, 2 ); /* ��:2�ֽ� */
                if( devTime.Year >= ( UNSIGNED )CSMC_DEVICE_START_YEAR )
                {
                    /* ����ݼ�ȥװ�õ���ʼ���(2000,û�п��ǵ���2000������) */
                    devTime.Year -= CSMC_DEVICE_START_YEAR;
                    p += 2; /* ָ������һ���� */
                    /* �� */
                    devTime.Month = ( UNSIGNED )(* p ++);
                    /* �� */
                    devTime.Day = ( UNSIGNED )(* p ++);
                    /* ʱ */
                    devTime.Hour = ( UNSIGNED )(* p ++);
                    /* �� */
                    devTime.Minute = ( UNSIGNED )(* p ++);
                    /* �� */
                    devTime.Millionsecond = ( UNSIGNED )(* p ++);
                    /* UTC�� */
                    XJP_CSC_Byte_To_Word(& UTCSecond, p, CSMC_SNTP_SECOND_BYTES );
                    p += CSMC_SNTP_SECOND_BYTES; /* UTC�����ݿ�ȣ�4�ֽ� */
                    /* UTC���� */
                    XJP_CSC_Byte_To_Word(& UTCNanoSecond, p, CSMC_SNTP_NSECOND_BYTES );
                    p += CSMC_SNTP_NSECOND_BYTES;
                    /* ʱ��Ʒ������ */
                    uQuality = * p ++;
                    /* ����ʱ�� */
                    XJP_TM_Set_Time_61850( UTCSecond, UTCNanoSecond, XJ_NULL );
                    /* ���ö�ʱʱ��Ʒ������ */
                    XJP_TM_SET_Time_Quality( uQuality, COND_TYPE_EQU );
                    CSMV_Rxd_Time_SNTP = XJ_Retrieve_Clock();
                }
                else
                {
                    /* ������ݴ�������� */
                    CSMV_MsgCount.iRecSynYearErrCount[0] ++;
                    /* ��¼������� */
                    CSMV_MsgCount.iRecSynYearErrCount[1] = devTime.Year;
                }
            }
            else
            {
                /* �������ȴ�������� */
                CSMV_MsgCount.iRecSynLenErrCount[0] ++;
                /* ��¼���󳤶� */
                CSMV_MsgCount.iRecSynLenErrCount[1] = length;
            }
        }
        break;

        /* ��ʱ���GOOSE */
    case CSMC_COMMAND_ID_0x8D:
        {
            INT iGOOSECount;  /* goose������� */
            INT iInputBit;    /* �������� */
            INT iStatus;      /* ����״̬ */
            INT iGroupNo;     /* ��� */
            UNSIGNED uBit;    /* ����λ */

            /* ����������������� */
            CSMV_MsgCount.iRecGooseTimeCount ++;
            p = ptr + 1; /* ָ��GOOSE���� */
            /* ��ȡ���������� */
            iGOOSECount = * p ++;
            /* �жϱ��ĳ����Ƿ���ȷ */
            if(( length == ( UINT )( CSMV_8D_MIN_LEN + CSMV_8D_DATA_LEN * iGOOSECount ) )
                && ( length > CSMV_8D_MIN_LEN ))
            {
                /* �洢������״̬�����ݴ��ݸ�Ӧ�ó��� */
                for( i = 0; i < iGOOSECount; i ++)
                {
                    /* ��ȡ����λ */
                    XJP_CSC_Byte_To_Word(& iInputBit, p, 2 );
                    /* �жϿ��������Ƿ���ȷ */
                    iGroupNo = iInputBit >> CSMV_GROUP_MASK32;
                    /* �жϿ��������Ƿ�������Χ֮��(���) */
                    if( iGroupNo < CPC_MAX_INPUT_GROUPS )
                    {
                        /* ����λ */
                        uBit = ( 1 << ( iInputBit & CSMV_SUB_GROUP_MASK32 ));
                        /* ��ȡ����λ״̬ */
                        iStatus = * ( p + 2 );
                        /* �жϿ���״̬ */
                        if( iStatus )
                        {
                            /* Ͷ�� */
                            PLAT_SoftInputMemory[iGroupNo] |= uBit;
                        }
                        else
                        {
                            /* �˳� */
                            PLAT_SoftInputMemory[iGroupNo] &= ~ uBit;
                        }

                        /* ��ȡ��Ч��Чλ״̬ */
                        iStatus = * ( p + 3 );
                        if( iStatus )
                        {
                            /* ��Ч */
                            PLAT_SoftInputQuality[iGroupNo] |= uBit;
                        }
                        else
                        {
                            /* ��Ч */
                            PLAT_SoftInputQuality[iGroupNo] &= ~ uBit;
                        }

                        /* ��ȡ����λ״̬ */
                        iStatus = * ( p + 4 );
                        if( iStatus )
                        {
                            /* ���� */
                            PLAT_SoftInputCheckQuality[iGroupNo] |= uBit;
                        }
                        else
                        {
                            /* �޼��� */
                            PLAT_SoftInputCheckQuality[iGroupNo] &= ~ uBit;
                        }
                        /* NPI���͹����Ŀ���λΪ����Ŀ���λ,��ƽ̨������Ӧ�õ�Ϊװ����ʵ�ʿ���λ */
                        iInputBit += CINC_HARD_INPUT_GROUPS * 32;
                        XJP_CSC_Word_To_Byte( p,& iInputBit, 2 );
                    }
                    else
                    {
                        /* ����������λ������ */
                        CSMV_MsgCount.iRecGooseTimeBitErrCount ++;
                        /* ��¼������λ */
                        CSMV_MsgCount.iRecGooseTimeErrBits = iInputBit;
                    }
                    /* ��������ָ�� */
                    p += CSMV_8D_DATA_LEN;
                }
                NU_Protect( pReport_Protect );
                /* ������д���û����ݻ����� */
                CSMV_MessageBuf[0] = ( length + sizeof( UNSIGNED ) - 1 )/ sizeof( UNSIGNED ) + 2; /* ʵ�ʳ�����ȡ���� */
                CSMV_MessageBuf[1] = length;   /* ���� */
                /* ���ݴӵ�ַ3��ʼ */
                memcpy(& CSMV_MessageBuf[2], ptr, length );
                /* ������д�뻺������ */
                XJP_Queue_Write( NPIUserDataHandle, CSMV_MessageBuf );
                NU_Unprotect();
            }
            else
            {
                /* �������ȴ�������� */
                CSMV_MsgCount.iRecGooseTimeLenErrCount ++;
                /* ��¼���󳤶� */
                CSMV_MsgCount.iRecGooseTimeErrLen = length;
            }
        }
        break;

        /* �������� */
    case CSMV_COMMAND_ID_0x8B:
    case CSMV_COMMAND_ID_0x8E:
    case CSMV_COMMAND_ID_0x8F:
    case CSMV_COMMAND_ID_0x90:
        {
            CSMV_MsgCount.iOtherCount ++;
            NU_Protect( pReport_Protect );
            CSMV_MessageBuf[0] = ( length + sizeof( UNSIGNED ) - 1 )/ sizeof( UNSIGNED ) + 2; /* ʵ�ʳ�����ȡ���� */
            CSMV_MessageBuf[1] = length;   /* ���� */
            /* ���ݴӵ�ַ3��ʼ */
            memcpy(& CSMV_MessageBuf[2], ptr, length );
            /* ������д�뻺������ */
            XJP_Queue_Write( NPIUserDataHandle, CSMV_MessageBuf );
            NU_Unprotect();
        }
        break;
    }
    /* �������ĺ���ִ��ʵʱʱ�� */
    CSMV_MsgCount.uMsgParseRealTime = XJP_Dbg_Time_Testing(& uMillisecond, & uMicrosecond );
    /* �жϽ����������ִ��ʱ�� */
    if( CSMV_MsgCount.uMsgParseRealTime > CSMV_MsgCount.uMsgParseMaxTime )
    {
        /* �������ĺ���ִ�����ʱ�� */
        CSMV_MsgCount.uMsgParseMaxTime = CSMV_MsgCount.uMsgParseRealTime;
    }
}

/* ����GOOSE���������� */
VOID CSMV_Msg_SendGooseOut( VOID )
{
    static INT      count = 0;                         /* ��λ����ʱ,���������͵Ĵ��� */
    static UNSIGNED Last_Put_Clock = 0;                /* �ϴη���֡��ʱ�� */
    static INT      CBCount = CSMC_OUT_COUNT_ON_CHANGE;
    static INT      iInitFlag = 0;                     /* ��ʼ����־ */
    UNSIGNED_CHAR   Out_Buffer[ CPC_MAX_LENGTH_FRAME ];/* ���ݷ��ͻ����� */
    INT             i;
    UNSIGNED        uMessLength;
    static INT      iTotalCount, iHardCount, iSoftCount;
    UNSIGNED        NewOutQ[CPC_MAX_OUTPUT_GROUPS];        /* ���¼�¼�Ŀ���Ʒ��������Ч��Чλ��״̬ */
    UNSIGNED        NewOutQJ[CPC_MAX_OUTPUT_GROUPS];       /* ���¼�¼�Ŀ���Ʒ�����ؼ���λ��״̬ */
    UNSIGNED        NewOutStatus[CPC_MAX_OUTPUT_GROUPS];   /* ���¼�¼�ĳ���λ״̬ */
    INT             iStatus;
    UNSIGNED        uSendDelayTime;
    /*--------------------------------------------------------------------------*/
    if( iInitFlag == 0 )
    {
        iInitFlag = 1;
        Last_Put_Clock = XJ_Retrieve_Clock();
        /* ��ȡ�������� */
        OUTC_GetOutGroupCount(& iTotalCount,& iHardCount );
        /* �ж��������� */
        if( iTotalCount <= iHardCount )
        {
            iSoftCount = 0;
        }
        else
        {
            iSoftCount = iTotalCount - iHardCount;
        }
        /* ����������� */
        if( iSoftCount > CPC_MAX_OUTPUT_GROUPS )
        {
            iSoftCount = CPC_MAX_OUTPUT_GROUPS;
        }
    }

    if( iSoftCount <= 0 )
    {
        return;
    }

    /* ��ȡ����״̬��Ʒ������ */
    for( i = 0; i < iSoftCount; i ++)
    {
        COUTC_ReadSoftOutTotalQua(( iHardCount + i ),& NewOutStatus[i],& NewOutQ[i],& NewOutQJ[i] );
    }

    /* ���Ʒ�����ؼ�״̬�Ƿ����仯 */
    iStatus = CSMV_Msg_OutStatusCheck( NewOutStatus, NewOutQ, NewOutQJ, iSoftCount );
    if( iStatus == NU_TRUE )
    {
        /* ��Ʒ�����ط����仯��ʱ��ҲҪ��״̬�仯���������ݵķ��� */
        count = CSMC_OUT_COUNT_ON_CHANGE;
        CBCount = 0;                   /* �б�λʱ,�����ͱ�λ�������� */
    }

    /* ���ο������ķ���ʱ�� */
    uSendDelayTime = CSMV_MG_OutSendDelayTime[CBCount];
    /* �б�λʱ��������CSOP_COUNT_ON_CHANGE��,ÿ�μ��������ʱ������CSOP_SendDelayTime */
    if(( XJ_Retrieve_Clock() - Last_Put_Clock ) >= uSendDelayTime )
    {
        /* �׵�ַ��д�������� */
        Out_Buffer[0] = CSMC_COMMAND_ID_0x8A;
        /* ��д���ļ���ȡ���ĳ��� */
        uMessLength = CSMC_MSG_FillOutStatus( & Out_Buffer[1], NewOutStatus, NewOutQ, NewOutQJ, iSoftCount );
        uMessLength += 1;  /* �������� */
        /* �������ݺ��� */
        CSMC_MSG_SEND_GOOSE( iStatus, Out_Buffer, uMessLength );

        if( iStatus > 0 )
        {
            Last_Put_Clock = XJ_Retrieve_Clock();
            if( count > 0 )  /* ���������ʹ�����Ϊ0 */
            {
                count --;
                CBCount ++;
            }
            /* �������ͳɹ������� */
            CSMV_MsgCount.iSendGooseSucCount ++;
        }
        else
        {
            /* ��������ʧ�ܼ����� */
            CSMV_MsgCount.iSendGooseErrCount ++;
            /* ʧ��ʱ��¼��ǰʱ�� */
            CSMV_MsgCount.uSendGooseErrTime = TMD_System_Clock;
        }
    }
}

/* �������ݻ���������д��������š�61850ʱ��(�롢����)��������״̬��������Ʒ��Ʒ������ */
static INT CSMC_MSG_FillOutStatus( UNSIGNED_CHAR * pBuffer, UNSIGNED * pOutStatus,
    UNSIGNED * pQuality, UNSIGNED * pCheckQua, INT iGroupNum )
{
    UNSIGNED_CHAR * pTempBuf;
    UNSIGNED_CHAR * p;
    UNSIGNED      Second_61850;
    UNSIGNED      MillSec_61850;
    INT           i;
    INT           iSize;
    /*--------------------------------------------------------------------------*/
    pTempBuf = pBuffer;

    /* װ��״̬ */
    p = ( UNSIGNED_CHAR *)& CSMV_MSG_DevQStatus;
    XJP_CSC_Word_To_Byte( pTempBuf, p, CSMV_DEV_STATUS_BYTES );
    pTempBuf += CSMV_DEV_STATUS_BYTES;

    /* ���������� */
    * pTempBuf ++ = iGroupNum;

    /* ��ȡ61850ʱ�� */
    XJP_TM_Get_Time_61850( NU_NULL, & Second_61850,
        & MillSec_61850, NU_NULL );

    /* 61850�� */
    p = ( UNSIGNED_CHAR *)& Second_61850;
    XJP_CSC_Word_To_Byte( pTempBuf, p, CSMC_SNTP_SECOND_BYTES );
    pTempBuf += CSMC_SNTP_SECOND_BYTES;

    /* 61850���� */
    p = ( UNSIGNED_CHAR *)& MillSec_61850;
    XJP_CSC_Word_To_Byte( pTempBuf, p, CSMC_SNTP_NSECOND_BYTES );
    pTempBuf += CSMC_SNTP_NSECOND_BYTES;

    for( i = 0; i < iGroupNum; i ++)
    {
        /* ����״̬ */
        p = ( UNSIGNED_CHAR *)( pOutStatus + i );
        XJP_CSC_Word_To_Byte( pTempBuf, p, CSMC_GOOSE_DATA_BYTES );
        pTempBuf += CSMC_GOOSE_DATA_BYTES;

        /* ����Ʒ������(��Ч��Чλ) */
        p = ( UNSIGNED_CHAR *)( pQuality + i );
        XJP_CSC_Word_To_Byte( pTempBuf, p, CSMC_GOOSE_QUA_BYTES );
        pTempBuf += CSMC_GOOSE_QUA_BYTES;

        /* ����Ʒ������(����λ) */
        p = ( UNSIGNED_CHAR *)( pCheckQua + i );
        XJP_CSC_Word_To_Byte( pTempBuf, p, CSMC_GOOSE_TEST_BYTES );
        pTempBuf += CSMC_GOOSE_TEST_BYTES;
    }

    iSize = ( INT )( pTempBuf - pBuffer );
    return( iSize );
}

/* �ж�Ʒ�����ؼ�״̬�Ƿ����仯(NU_TRUE:�仯;NU_FALSE:û�б仯) */
static INT CSMV_Msg_OutStatusCheck( UNSIGNED * pStatus, UNSIGNED * pQuality, UNSIGNED * pCheckQua, INT iGroupNum )
{
    static UNSIGNED LastOutQ[CPC_MAX_OUTPUT_GROUPS];       /* �ϴμ�¼�Ŀ���Ʒ��������Ч��Чλ��״̬ */
    static UNSIGNED LastOutQJ[CPC_MAX_OUTPUT_GROUPS];      /* �ϴμ�¼�Ŀ���Ʒ�����ؼ���λ��״̬ */
    static UNSIGNED LastOutStatus[CPC_MAX_OUTPUT_GROUPS];  /* �ϴγ���λ״̬ */
    static INT iInitFlag = 0;
    UNSIGNED NewOutQ[CPC_MAX_OUTPUT_GROUPS];        /* ���¼�¼�Ŀ���Ʒ��������Ч��Чλ��״̬ */
    UNSIGNED NewOutQJ[CPC_MAX_OUTPUT_GROUPS];       /* ���¼�¼�Ŀ���Ʒ�����ؼ���λ��״̬ */
    UNSIGNED NewOutStatus[CPC_MAX_OUTPUT_GROUPS];   /* ���¼�¼�ĳ���λ״̬ */
    INT    iStatus = NU_FALSE;
    INT    i, iCount;
    static UNSIGNED DevQStatus = 0;             /* װ��״̬,�����NPI��� */
    /*======================================*/
    if( iInitFlag == 0 )
    {
        /* �建���� */
        memset( ( VOID *)LastOutQ, 0, sizeof( LastOutQ ));
        memset( ( VOID *)LastOutQJ, 0, sizeof( LastOutQJ ));
        memset( ( VOID *)LastOutStatus, 0, sizeof( LastOutStatus ));
        iInitFlag = 1;
    }
    iCount = iGroupNum;
    if( iCount > CPC_MAX_OUTPUT_GROUPS )
    {
        iCount = CPC_MAX_OUTPUT_GROUPS;
    }
    /* ��¼����״̬��Ʒ������ */
    memcpy( NewOutStatus, pStatus, sizeof( UNSIGNED ) * iCount );
    memcpy( NewOutQ, pQuality, sizeof( UNSIGNED ) * iCount );
    memcpy( NewOutQJ, pCheckQua, sizeof( UNSIGNED ) * iCount );

    /* �ж�Ʒ�������Ƿ����仯 */
    for( i = 0; i < iCount; i ++)
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
        iStatus = NU_TRUE;
    }

    /* ����Ʒ�����ؼ�״̬ */
    memcpy( LastOutStatus, NewOutStatus, sizeof( UNSIGNED ) * iCount );
    memcpy( LastOutQ, NewOutQ, sizeof( UNSIGNED ) * iCount );
    memcpy( LastOutQJ, NewOutQJ, sizeof( UNSIGNED ) * iCount );
    /* װ��״̬ */
    DevQStatus = CSMV_MSG_DevQStatus;
    return( iStatus );
}

/* ����װ��״̬���� */
INT CSMV_MSG_SetDevStatus( UNSIGNED uDevStatus )
{
    CSMV_MSG_DevQStatus = uDevStatus;
    return( NU_SUCCESS );
}

/*================================================*/
/* �û���NPI��������� */
INT CSMV_MSG_GetDataFromNpi( UNSIGNED_CHAR * buffer, INT * Length )
{
    INT iStatus = NU_FALSE;
    UNSIGNED Size;
    UNSIGNED MessageBuf[CSMV_MAX_FRAME_LEN_UN + 2];
    /*--------------------------------------*/
    /* �ж��Ƿ����û����� */
    if( XJP_Queue_Find( NPIUserDataHandle, & Size ) == NU_TRUE )
    {
        NU_Protect( pReport_Protect );
        iStatus = XJP_Queue_Read( NPIUserDataHandle , & MessageBuf );
        NU_Unprotect();
        /* ��ȡ�û����� */
        if( iStatus == NU_SUCCESS )
        {
            /* ���ĳ��� */
            if( Length )
            {
                /* ���ĳ��� */
                * Length = MessageBuf[1];
            }
            /* �������� */
            if( buffer )
            {
                /* �������� */
                memcpy( buffer, ( VOID *)& MessageBuf[2], MessageBuf[1] );
            }
            iStatus = NU_TRUE;
        }
        else
        {
            iStatus = NU_FALSE;
        }
    }
    return( iStatus );
}

/*================================================*/
/* �û���NPI�෢�����ݺ��� */
INT CSMV_MSG_UserSendDataTONpi( UNSIGNED_CHAR * buffer, INT Length )
{
    static INT iInitFlag = 0;
    INT iStatus = NU_FALSE;
    /*--------------------------------------*/
    /* �жϻ������Ƿ��ʼ�� */
    if( iInitFlag == 0 )
    {
        memset( CSMV_Msg_UserSendDataBuffer, 0, sizeof( CSMV_Msg_UserSendDataBuffer ));
        iInitFlag = 1;
    }

    /* ����������ʼ��ַ�洢���Ǳ��ĳ���,�ڶ�����ַ�洢���Ǳ������� */
    if( CSMV_Msg_UserSendDataBuffer[0] == 0 )
    {
        if(( Length <= CPC_MAX_LENGTH_FRAME ) && ( Length > 0 ))
        {
            /* ������������ */
            memcpy( & CSMV_Msg_UserSendDataBuffer[1], buffer, Length );
            /* ������û��������� */
            CSMV_Msg_UserSendDataBuffer[0] = Length;
            iStatus = NU_TRUE;
        }
    }

    return( iStatus );
}

/*================================================*/
/* �����û����� */
INT  CSMV_MSG_UserSendData( VOID )
{
    INT           iStatus;
    INT           iStatus1 = NU_UNAVAILABLE;
    UNSIGNED      uMessLength;
    /*--------------------------------------*/
    /* ����Ӧ������ */
    if( CSMV_Msg_UserSendDataBuffer[0] != 0 )
    {
        /* ���ĳ��� */
        uMessLength = CSMV_Msg_UserSendDataBuffer[0];
        /* �������� */
        CSMC_MSG_SEND_GOOSE( iStatus, ( VOID *)& CSMV_Msg_UserSendDataBuffer[1], uMessLength );
        if( iStatus > 0 )
        {
            /* �屨�ĳ��� */
            CSMV_Msg_UserSendDataBuffer[0] = 0;
            /* ��������״ֵ̬�ɹ�״̬ */
            iStatus1 = NU_SUCCESS;
        }
    }
    return( iStatus1 );
}

/*================================================*/
/* NPU��NPI���Ͷ�ʱ���� */
VOID CSMV_Msg_SynTimeCmd( VOID )
{
    UNSIGNED_CHAR uTestBuf[CPC_MAX_LENGTH_FRAME];
    UNSIGNED_CHAR * p;
    INT           iLen;
    INT           iStatus;
    static int iInitFlag = 0;
    static UNSIGNED uDelayTime;
    INT    iSendFlag = NU_FALSE;
    TIME   SynTime;
    UNSIGNED Second_61850;
    UNSIGNED NandSec_61850;
    UNSIGNED Quality;
    /*======================================*/
    if( iInitFlag == 0 )
    {
        iInitFlag = 1;
        /* �÷��Ͷ�ʱ�����ʶ */
        iSendFlag = NU_TRUE;
        /* ��ʼ����ʱʱ�� */
        uDelayTime = TMD_System_Clock;
    }
    else
    {
        /* �ж���ʱʱ���Ƿ� */
        if(( TMD_System_Clock - uDelayTime ) > CSMC_SYNTIME_DELAY_TIME )
        {
            /* ���·�����ʱʱ�� */
            uDelayTime = TMD_System_Clock;
            /* �÷��Ͷ�ʱ�����ʶ */
            iSendFlag = NU_TRUE;
        }
    }
    /* �ж��Ƿ��Ͷ�ʱ���� */
    if( iSendFlag == NU_TRUE )
    {
        /* ��֯��ʱ���� */
        iLen = 0;
        /* �建���� */
        memset( uTestBuf, 0, sizeof( uTestBuf ));
        /* �������� */
        uTestBuf[iLen ++ ] = CSMC_COMMAND_ID_0x83;
        /* ��ȡʱ�� */
        XJP_TM_Get_Time_61850( & SynTime, & Second_61850,
            & NandSec_61850, & Quality );
        /* ����װ�õ���ʼʱ��(2000) */
        SynTime.Year += CSMC_DEVICE_START_YEAR;
        /* �� */
        p = ( UNSIGNED_CHAR *)&( SynTime.Year );
        /* ��:�����ֽ� */
        XJP_CSC_Word_To_Byte( & uTestBuf[iLen], p, 2 );
        /* ��:�����ֽ� */
        iLen += 2;
        /* �� */
        uTestBuf[iLen ++ ] = ( UNSIGNED_CHAR )SynTime.Month;
        /* �� */
        uTestBuf[iLen ++ ] = ( UNSIGNED_CHAR )SynTime.Day;
        /* ʱ */
        uTestBuf[iLen ++ ] = ( UNSIGNED_CHAR )SynTime.Hour;
        /* �� */
        uTestBuf[iLen ++ ] = ( UNSIGNED_CHAR )SynTime.Minute;
        /* �� */
        uTestBuf[iLen ++ ] = ( UNSIGNED_CHAR )( SynTime.Millionsecond % 1000 );

        /* 61850�� */
        p = ( UNSIGNED_CHAR *)& Second_61850;
        XJP_CSC_Word_To_Byte( & uTestBuf[iLen], p, CSMC_SNTP_SECOND_BYTES );
        iLen += CSMC_SNTP_SECOND_BYTES;

        /* 61850���� */
        p = ( UNSIGNED_CHAR *)& NandSec_61850;
        XJP_CSC_Word_To_Byte( & uTestBuf[iLen], p, CSMC_SNTP_NSECOND_BYTES );
        iLen += CSMC_SNTP_NSECOND_BYTES;

        /* ʱ��Ʒ������ */
        uTestBuf[iLen] = ( UNSIGNED_CHAR )Quality;
        iLen += CSMC_SNTP_QUALITY_BYTES;

        /* �������ݺ��� */
        CSMC_MSG_SEND_GOOSE( iStatus, uTestBuf, iLen );

        /* �ж��Ƿ��ͳɹ� */
        if( iStatus > 0 )
        {
            /* ��������83��������� */
            CSMV_MsgCount.iSendSynSucCount ++;
        }
        else
        {
            /* �������Ͷ�ʱ����ʧ�ܸ��������� */
            CSMV_MsgCount.iSendSynErrCount ++;
            /* ��¼���Ͷ�ʱ����ʱ�Ĵ���ʱ�� */
            CSMV_MsgCount.iSendSynErrTime = TMD_System_Clock;
        }
    }
    /* ���տ�������ʱ��⺯�� */
    //CSMV_MSG_Input_TimeOutCheck();
}

/*================================================*/
/* ע����NPI�������ݽӿں��� */
VOID CSMV_RegTONpiSendData( INT (* RegSendDataFun )( UNSIGNED_CHAR * data, INT length ))
{
    CSMV_TONpiSendData = RegSendDataFun;
}

/*================================================*/
/* �������ݺ��� */
INT CSMV_SendDataFun( UNSIGNED_CHAR * data, INT length )
{
    INT iStatus = NU_FALSE;
#ifndef WIN32
    if( PLAT_cfg.device.FunSelect & FUN_IED_FIFO )
    {
        if( CSMV_TONpiSendData )
        {
            iStatus = (* CSMV_TONpiSendData )( data, length );
        }
    }
    else
    {
        iStatus = eth_npi_if_send( data, length );
    }
#endif
    return( iStatus );
}

/*================================================*/
/* ���տ�������ʱ��⺯�� */
VOID CSMV_MSG_Input_TimeOutCheck( VOID )
{
    static INT      iFlag = 0; /* ��ʼ����־ */
    static UNSIGNED CheckTime;
    static INT      iCheckFlag;/* �Ƿ����־ */
    INT             i;
    char            EventName[LS_ALARM_NAME];
    /*------------------------------------------------------*/
    /* �ж��Ƿ������� */
    if( GET_SOFT_INGROUP())
    {
        /* ��ʼ���������� */
        if( iFlag == 0 )
        {
            iFlag = 1;
            iCheckFlag = 0;
            CheckTime = CSMV_Rxd_Time_GOOSE;
        }
        else
        {
            /* �ж��Ƿ���Ҫ���¼��ʱ�� */
            if( CheckTime == CSMV_Rxd_Time_GOOSE )
            {
                /* �жϴ˴�ʱ���Ƿ��Ѿ����� */
                if( iCheckFlag == 0 )
                {
                    /* �ж�ʱ���Ƿ�ʱ */
                    if(( TMD_System_Clock - CheckTime ) >= CSMV_MSG_INPUT_DELAY )
                    {
                        /* ���Ѽ�����־ */
                        iCheckFlag = 1;
                        /* ����������Ʒ����������Ϊ��Ч״̬ */
                        for( i = 0; i < CPC_MAX_INPUT_GROUPS; i ++)
                        {
                            PLAT_SoftInputQuality[i] = 0;
                        }
                        /* ���ý��տ�������ʱ�ڲ��¼� */
                        sprintf( EventName, " %03d", LS_RES_EVENT_GETIN_OVERTIME );
                        CEMC_FillEvent( EventName, RESERVE_EVENT_ID );
                    }
                }
            }
            else
            {
                /* �ü���־ */
                iCheckFlag = 0;
                /* ���¼��ʱ�� */
                CheckTime = CSMV_Rxd_Time_GOOSE;
            }
        }
    }
}

/*================================================*/
/* ����ģ����ͨ��Ʒ������¼��״̬������ʼ��ż����� */
INT CSMV_MSG_SetChlQStateInfo( INT iStartGroup, INT iGroupCount )
{
    CSMV_MsgStateChlQStartGroup = iStartGroup;
    CSMV_MsgStateChlQGroupCount = iGroupCount;
    /* �ж���ʼ����Ƿ���¼��״̬��������Χ֮�� */
    if( CSMV_MsgStateChlQStartGroup >= CPC_MAX_PROTECT_GROUPS )
    {
        CSMV_MsgStateChlQGroupCount = 0;
    }
    else if(( CSMV_MsgStateChlQStartGroup + CSMV_MsgStateChlQGroupCount )
        > CPC_MAX_PROTECT_GROUPS )
    {
        CSMV_MsgStateChlQGroupCount = CPC_MAX_PROTECT_GROUPS - CSMV_MsgStateChlQStartGroup;
    }
    return( NU_SUCCESS );
}

/*================================================*/
/* ��ȡģ����ͨ��Ʒ������¼��״̬������ʼ��ż����� */
INT CSMV_MSG_GetChlQStateInfo( INT * iStartGroup, INT * iGroupCount )
{
    * iStartGroup = CSMV_MsgStateChlQStartGroup;
    * iGroupCount = CSMV_MsgStateChlQGroupCount;
    return( NU_SUCCESS );
}

/*================================================*/
/* ��ȡ������������������� */
float * CSMV_MSG_GetSVDataInfo( UNSIGNED * SampleSerial )
{
    if( SampleSerial )
    {
        * SampleSerial = CSMC_SampleSerial;
    }

    return( CSMC_Analog );
}

/*================================================*/
