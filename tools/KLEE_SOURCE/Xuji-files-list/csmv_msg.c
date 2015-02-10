
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

#define CSMC_O_COMMAND                  0       /* 命令类型位置偏移 */
#define CSMC_COMMAND_ID_0x83            0x83    /* 接收SNTP对时命令 */
#define CSMC_COMMAND_ID_0x88            0x88    /* 接收GOOSE命令 */
#define CSMC_COMMAND_ID_0x8C            0x8C    /* 接收SV命令 */
#define CSMC_COMMAND_ID_0x8D            0x8D    /* 接收带时间的GOOSE命令 */
#define CSMV_COMMAND_ID_0x8B            0x8B    /* 非电量报文 */
#define CSMV_COMMAND_ID_0x8E            0x8E    /* 传输高精度采样报文 */
#define CSMV_COMMAND_ID_0x8F            0x8F    /* 传输SV统计报文 */
#define CSMV_COMMAND_ID_0x90            0x90    /* 传输GOOSE统计报文 */
#define CSMC_COMMAND_ID_0x8A            0x8A    /* 发送GOOSE命令 */
#define CSMC_ANALOG_COEF_BYTES          4       /* 模拟量系数字节数 */
#define CSMC_ANALOG_DATA_BYTES          4       /* 模拟量数据字节数 */
#define CSMC_ANALOG_QUA_BYTES           4       /* 模拟量品质因素字节数 */
/* 9=(帧类型:1)+(模拟量序列号:2)+(模拟量通道个数:1)+(模拟量品质因素组数:1) + 通道系数计数 */
#define CSMC_ANALOG_MIN_FRAME           9       /* 模拟量最小帧长 */
#define CSMC_GOOSE_QUA_BYTES            4       /* GOOSE品质因素字节数 */
#define CSMC_GOOSE_DATA_BYTES           4       /* GOOSE数据字节数 */
#define CSMC_GOOSE_TEST_BYTES           4       /* GOOSE检修字节数 */
#define CSMC_GOOSE_MIN_FRAME            2       /* GOOSE最小帧长:2=(帧类型:1)+(组数:1) */
#define CSMC_SNTP_FRAME_LENGTH          17      /* SNTP报文帧长 */
#define CSMC_DEVICE_START_YEAR          2000    /* 装置的起始年份 */
#define CSMC_SNTP_SECOND_BYTES          4       /* SNTP秒的字节数 */
#define CSMC_SNTP_NSECOND_BYTES         4       /* SNTP秒以下数据的字节数 */
#define CSMC_SNTP_QUALITY_BYTES         1       /* 时间品质因素字节数 */
#define CSMV_DEV_STATUS_BYTES           4       /* 装置状态字节数 */
#define CSMV_MAX_FRAME_LEN_UN           ((CPC_MAX_LENGTH_FRAME + sizeof(UNSIGNED)-1)/ sizeof(UNSIGNED)) /* 报文长度(UNSIGNED) */

/* 8D命令每个数据长度 */
/* 开入位(2字节)+状态(1字节)+品质因素有效无效位(1字节)+检修位(1字节)+时间秒(4字节)+时间纳秒(4字节)+时间状态(1字节) */
#define CSMV_8D_DATA_LEN                14
#define CSMV_8D_MIN_LEN                 2       /* 8D命令最小长度(帧类型(1字节)+数据个数(1字节)) */

#define CSMV_GROUP_MASK32               5       /* the group mask bit for 32-bits */
#define CSMV_SUB_GROUP_MASK32           31      /* the mask of sub-group 32-bits */

/* 0x89命令偏移 */
#define CSMC_O_8C_CALC_COEF             1       /* 数据系数 */
#define CSMC_O_8C_ANALOG_SERIAL_NUMBER  5       /* 模拟量序列号 */
#define CSMC_O_8C_CHANNEL_NUMBER        7       /* 模拟量通道个数 */
#define CSMC_O_8C_QUA_GROUP_NUMBER      8       /* 模拟量品质因素组数 */
#define CSMC_O_8C_QUA_VALUE             9       /* 模拟量品质因素值 */
/* 0x82命令偏移 */
#define CSMC_O_88_GROUP_NUMBER          1       /* 开入量组数 */
#define CSMC_O_88_INPUT_VALUE           2       /* 开入量值 */
/* 版本号类型定义 */
#define CSMC_VERSION_FLOAT              0  /* 版本号：浮点数 */
#define CSMC_VERSION_INT                1  /* 版本号：整形数 */

/* GOOSE开出发送次数及发送间隔 */
#define CSMC_OUT_COUNT_ON_CHANGE    6       /* 有变位时连续发送6次 */
#define CSMC_OUT_SPACE_ON_CHANGE_1  1       /* 有变位时前两次发送间隔为1毫秒 */
#define CSMC_OUT_SPACE_ON_CHANGE_2  2       /* 有变位时第三次发送间隔为2毫秒 */
#define CSMC_OUT_SPACE_ON_CHANGE_4  4       /* 有变位时第四次发送间隔为4毫秒 */
#define CSMC_OUT_SPACE_NORMAL       500     /* 变位连续发送三次后，转入常态，每500秒发送一次*/

/* 发送对时命令延时时间 */
#define CSMC_SYNTIME_DELAY_TIME     1000    /* 1秒钟发送一次对时命令 */
#define CSMV_MSG_INPUT_DELAY        3000    /* 检测接收到开入量信息的延时时间 */

/* 向NPI发送数据接口函数 */
INT (* CSMV_TONpiSendData )( UNSIGNED_CHAR * data, INT length );

extern  volatile UNSIGNED TMD_System_Clock;

/* 以下为OS相关配置 */
#ifdef WIN32
/* 全局变量申明 */
/* Windows 宏定义 */
#endif

#ifndef WIN32 /* 非windows缺省定义一些宏以便通过编译 */
/* 全局变量申明 */
/* Nucleus宏定义 */
#endif

float CSMC_Analog[ CPC_MAX_CHL_NUMBER ];
static UNSIGNED CSMC_SampleSerial;

UNSIGNED CSMV_Rxd_Time_SV;    /* SV命令接收时间(0x89)  */
UNSIGNED CSMV_Rxd_Time_GOOSE; /* GOOSE命令接收时间(0x82) */
UNSIGNED CSMV_Rxd_Time_SNTP;  /* SNTP命令接收时间(0x83) */
UNSIGNED CSMV_MessageBuf[CSMV_MAX_FRAME_LEN_UN + 2];  /* 报文缓冲区 */
UNSIGNED CSMV_MSG_DevQStatus;       /* 装置状态 */
extern NU_PROTECT * pReport_Protect;
INT CSMV_MsgStateChlQStartGroup = 0;/* 通道品质因素录波状态量起始组号 */
INT CSMV_MsgStateChlQGroupCount = 0;/* 通道品质因素录波状态量组数 */

extern UNSIGNED PLAT_SoftInputMemory[];         /* 数字开入量端口 */
extern UNSIGNED PLAT_SoftInputQuality[];        /* 数字开入量品质因素有效无效位 */
extern UNSIGNED PLAT_SoftInputCheckQuality[];   /* 数字开入量品质因素检修位 */
#define GET_SOFT_INGROUP()     PLAT_cfg.input.SoftInputGroups  /* 获取软开入组数 */
/*==================================================================*/
extern HDCM NPIUserDataHandle;                  /* 接收NPI用户数据管理句柄 */
/* 应用向NPI发送数据缓冲区 */
UNSIGNED CSMV_Msg_UserSendDataBuffer[CSMV_MAX_FRAME_LEN_UN + 2] = {0};
/*==================================================================*/
/* 判断品质因素及状态是否发生变化(NU_TRUE:变化;NU_FALSE:没有变化) */
static INT CSMV_Msg_OutStatusCheck( UNSIGNED * pStatus, UNSIGNED * pQuality, UNSIGNED * pCheckQua, INT iGroupNum );
/* 向发送数据缓冲区中填写开出量组号、61850时间(秒、毫秒)、开出量状态、开出量品质品质因素(返回报文长度) */
static INT CSMC_MSG_FillOutStatus( UNSIGNED_CHAR * pBuffer, UNSIGNED * pOutStatus,
    UNSIGNED * pQuality, UNSIGNED * pCheckQua, INT iGroupNum );
/* 发送数据函数 */
INT CSMV_SendDataFun( UNSIGNED_CHAR * data, INT length );
/* 接收开入量超时检测函数 */
VOID CSMV_MSG_Input_TimeOutCheck( VOID );
/*==================================================================*/
/* 发送开出量的延时时间数组,设置位全局变量便于中试部测试的时候修改时间间隔 */
UNSIGNED CSMV_MG_OutSendDelayTime[CSMC_OUT_COUNT_ON_CHANGE + 1] =
{
    0, CSMC_OUT_SPACE_ON_CHANGE_1, CSMC_OUT_SPACE_ON_CHANGE_1,
    CSMC_OUT_SPACE_ON_CHANGE_2, CSMC_OUT_SPACE_ON_CHANGE_4,
    CSMC_OUT_SPACE_NORMAL, CSMC_OUT_SPACE_NORMAL
};

/* Define internal function calls.  */
/*==================================================*/
/* 发送GOOSE函数 */
#ifndef  WIN32
#define CSMC_MSG_SEND_GOOSE(x,y,z)  x=CSMV_SendDataFun(y,z)
#else
#define CSMC_MSG_SEND_GOOSE(x,y,z)  x=NU_TRUE
#endif
/*==================================================*/
CSMV_MSG_COUNT  CSMV_MsgCount;
/*==================================================*/
/* 报文解析函数 */
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

    /* 计算函数执行时间使用 */
    XJP_Dbg_Time_Testing(& uMillisecond, & uMicrosecond );

    /* 判断长度是否在允许范围之内 */
    if( length > CPC_MAX_LENGTH_FRAME )
    {
        /* 调整长度错误计数器 */
        CSMV_MsgCount.iLenErr ++;
    }
    /* 数据缓冲区 */
    ptr = data;
    /* 判断数据类型 */
    switch(* ( ptr + CSMC_O_COMMAND ))
    {
        /* SV数据 */
    case CSMC_COMMAND_ID_0x8C:
        {
            UNSIGNED_CHAR uChlNum;
            UNSIGNED_CHAR uQlityGroup;
            INT           iGroup;

            CSMV_MsgCount.iRecSmvCount[0] ++;
            /* 模拟量通道个数 */
            uChlNum = * ( ptr + CSMC_O_8C_CHANNEL_NUMBER );
            /* 记录接收到的模拟量通道个数 */
            CSMV_MsgCount.iRecSmvCount[1] = uChlNum;
            /* 模拟量通道品质因素组数 */
            uQlityGroup = * ( ptr + CSMC_O_8C_QUA_GROUP_NUMBER );
            /* 根据通道的基本信息计算出的报文长度 */
            counter = ( uChlNum * CSMC_ANALOG_DATA_BYTES ) + CSMC_ANALOG_MIN_FRAME
                + ( CSMC_ANALOG_QUA_BYTES * uQlityGroup * 2 ); /* 2:品质因素+检修 */
            /* 判断数据长度是否正确
               判断通道个数是否在允许的范围之内
               判断模拟量通道品质因素组数是否在允许的范围之内
               判断数据长度是否正确 */
            if( ( length > CSMC_ANALOG_MIN_FRAME )
                && ( uChlNum <= CPC_MAX_PHYSICAL_CHL_NUMBER )
                && ( uQlityGroup <= CSMV_ANALOG_MAX_GROUPS )
                &&( counter == length ))
            {
                /* 存储通道量值个数为支持的物理量通道个数 */
                /* 数据长度正确,存储模拟量数据 */
                /* 存储模拟量序列号 */
                XJP_CSC_Byte_To_Word(& SampleSerial,( ptr + CSMC_O_8C_ANALOG_SERIAL_NUMBER ), 2 ); /* 2字节 */
                /* 将数据指针调整到品质因素状态的位置 */
                p = ptr + CSMC_O_8C_QUA_VALUE;
                /* 存储品质因素 */
                counter = uQlityGroup * CSMC_ANALOG_QUA_BYTES;
                XJP_CSC_Byte_To_Word( CSMV_GET_ORIGINL_ANA_QUA_MEM, p, counter );
                p += counter;

                /* 将品质因素有效无效位状态映射到相应的状态量中 */
                iGroup = CSMV_MsgStateChlQGroupCount;
                /* 判断品质因素组数是否超出范围 */
                if( iGroup > uQlityGroup )
                {
                    iGroup = uQlityGroup;
                }
                /* 存储数据 */
                for( i = 0; i < iGroup; i ++)
                {
                    CSMV_GET_ORIGINL_PRO_STS_MEM[CSMV_MsgStateChlQStartGroup + i]
                        = CSMV_GET_ORIGINL_ANA_QUA_MEM[i];
                }
                /* 存储检修状态 */
                XJP_CSC_Byte_To_Word( CSMV_GET_ORIGINL_ANA_CHECKQUA_MEM, p, counter );
                p += counter;
                /* 获取计算系数 */
                XJP_CSC_Byte_To_Word(& iValue, ( ptr + CSMC_O_8C_CALC_COEF ), CSMC_ANALOG_COEF_BYTES );
                fCalcCoef = ( float )iValue;
                /* 避免系数出现0的情况 */
                if( fCalcCoef < 0.1 )
                {
                    fCalcCoef = 1.0;
                }

                if( CSMV_GET_DSAMPLE_FLAG == NU_TRUE )
                {
                    /* 关中断因为使用的是同一个缓冲 */
                    oldLevel = NU_Local_Control_Interrupts( 0 );
                    /* 调整采样序号 */
                    if((++ uLastSerial ) >= ( UNSIGNED )CSMV_GET_SAMPLE_COUNTER )
                    {
                        uLastSerial -= CSMV_GET_SAMPLE_COUNTER;
                    }
                    /* 判断采样序号是否连续 */
                    if( uLastSerial != SampleSerial )
                    {
                        CSMV_MsgCount.iRecSmvCount[2] ++;
                    }
                    uLastSerial = SampleSerial;
                    memset( CSMC_Analog, 0, sizeof( CSMC_Analog ));
                    /* 整形数 */
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
                    /* 整形数 */
                    for( i = 0; i < uChlNum; i ++)
                    {
                        XJP_CSC_Byte_To_Word(& iValue, p, CSMC_ANALOG_DATA_BYTES );
                        CSMC_Analog[i] = ( float )iValue;
                        CSMC_Analog[i] /= fCalcCoef;
                        p += CSMC_ANALOG_DATA_BYTES;
                    }
                    /* 模拟量数据存储完成, 调AD中断函数 */
                    SMVC_SampleUpdateSoft( CSMC_Analog, SampleSerial );
                }

                /* 更新SMV接收时间 */
                CSMV_Rxd_Time_SV = XJ_Retrieve_Clock();
                /* 设置采样准备完成标识 */
                CSMV_SetSampleInitFlag( NU_TRUE );
            }
        }
        break;
        /* GOOSE数据 */
    case CSMC_COMMAND_ID_0x88:
        {
            INT  iGroupNum;
            UNSIGNED uInputStatus;
            UNSIGNED uTempStatus;
            UNSIGNED MaskBits;

            CSMV_MsgCount.iGooseRecCount[0] ++;
            /* 开入量组数 */
            iGroupNum = ( INT )(* ( ptr + CSMC_O_88_GROUP_NUMBER ));
            /* 记录接收到的开入组数 */
            CSMV_MsgCount.iGooseRecCount[1] = iGroupNum;
            /* 根据通道的基本信息计算出的报文长度 */
            counter = ( CSMC_GOOSE_DATA_BYTES + CSMC_GOOSE_QUA_BYTES + CSMC_GOOSE_TEST_BYTES )* iGroupNum
                + CSMC_GOOSE_MIN_FRAME;
            /* 判断长度是否正确,开入量组数是否在允许的范围之内 */
            if(( counter == length )&&( iGroupNum <= ( INT )CPC_MAX_INPUT_GROUPS ))
            {
                /* 将数据指针调整到开入量状态的位置 */
                p = ptr + CSMC_O_88_INPUT_VALUE;
                for( i = 0; i < iGroupNum; i ++)
                {
                    MaskBits = 0;
                    /* 获取软开入整组传输时掩码 */
                    CINC_GetSoftInFullMasks( i,& MaskBits );
                    /* 开入量状态 */
                    XJP_CSC_Byte_To_Word(& uInputStatus, p, CSMC_GOOSE_DATA_BYTES );
                    /*  仅取掩码位的状态 */
                    uInputStatus &= MaskBits;
                    /* 在原状态中去除掩码位状态 */
                    uTempStatus = ( PLAT_SoftInputMemory[i] & ( ~ MaskBits ));
                    /* 更新掩码位状态 */
                    uTempStatus |= uInputStatus;
                    /* 设置最新状态 */
                    PLAT_SoftInputMemory[i] = uTempStatus;
                    p += CSMC_GOOSE_DATA_BYTES;
                    /* 开入量品质因素有效无效位 */
                    XJP_CSC_Byte_To_Word(& uInputStatus , p, CSMC_GOOSE_QUA_BYTES );
                    /*  仅取掩码位的状态 */
                    uInputStatus &= MaskBits;
                    /* 在原状态中去除掩码位状态 */
                    uTempStatus = ( PLAT_SoftInputQuality[i] & ( ~ MaskBits ));
                    /* 更新掩码位状态 */
                    uTempStatus |= uInputStatus;
                    /* 设置最新状态 */
                    PLAT_SoftInputQuality[i] = uTempStatus;
                    p += CSMC_GOOSE_QUA_BYTES;
                    /* 开入量品质因素检修位 */
                    XJP_CSC_Byte_To_Word(& uInputStatus, p, CSMC_GOOSE_TEST_BYTES );
                    /*  仅取掩码位的状态 */
                    uInputStatus &= MaskBits;
                    /* 在原状态中去除掩码位状态 */
                    uTempStatus = ( PLAT_SoftInputCheckQuality[i] & ( ~ MaskBits ));
                    /* 更新掩码位状态 */
                    uTempStatus |= uInputStatus;
                    /* 设置最新状态 */
                    PLAT_SoftInputCheckQuality[i] = uTempStatus;
                    /* 开入量检修状态 */
                    p += CSMC_GOOSE_TEST_BYTES;
                }
                /* 更新开入量接收时间 */
                CSMV_Rxd_Time_GOOSE = XJ_Retrieve_Clock();
            }
        }
        break;
        /* SNTP对时数据 */
    case CSMC_COMMAND_ID_0x83:
        {
            UNSIGNED UTCSecond;
            UNSIGNED UTCNanoSecond;
            TIME devTime;
            UNSIGNED uQuality;

            /* 判断对时来源 */
            if( pDeviceTab )
            {
                /* 判断对时来源及对时类型如果为SNTP对时类型不从NPI侧获取 */
                if(( pDeviceTab->SynSrc & PRO_SYN_SRC_PROCESS ) == 0 )
                {
                    return;
                }
            }
            CSMV_MsgCount.iRecSynCount ++;
            /* 判断长度是否正确 */
            if( length == CSMC_SNTP_FRAME_LENGTH )
            {
                p = ptr + 1; /* 指向时间数据 */
                /* 年 */
                XJP_CSC_Byte_To_Word(&( devTime.Year ), p, 2 ); /* 年:2字节 */
                if( devTime.Year >= ( UNSIGNED )CSMC_DEVICE_START_YEAR )
                {
                    /* 将年份减去装置的起始年份(2000,没有考虑低于2000年的情况) */
                    devTime.Year -= CSMC_DEVICE_START_YEAR;
                    p += 2; /* 指向向下一数据 */
                    /* 月 */
                    devTime.Month = ( UNSIGNED )(* p ++);
                    /* 日 */
                    devTime.Day = ( UNSIGNED )(* p ++);
                    /* 时 */
                    devTime.Hour = ( UNSIGNED )(* p ++);
                    /* 分 */
                    devTime.Minute = ( UNSIGNED )(* p ++);
                    /* 秒 */
                    devTime.Millionsecond = ( UNSIGNED )(* p ++);
                    /* UTC秒 */
                    XJP_CSC_Byte_To_Word(& UTCSecond, p, CSMC_SNTP_SECOND_BYTES );
                    p += CSMC_SNTP_SECOND_BYTES; /* UTC秒数据宽度：4字节 */
                    /* UTC毫秒 */
                    XJP_CSC_Byte_To_Word(& UTCNanoSecond, p, CSMC_SNTP_NSECOND_BYTES );
                    p += CSMC_SNTP_NSECOND_BYTES;
                    /* 时间品质因素 */
                    uQuality = * p ++;
                    /* 设置时间 */
                    XJP_TM_Set_Time_61850( UTCSecond, UTCNanoSecond, XJ_NULL );
                    /* 设置对时时间品质因素 */
                    XJP_TM_SET_Time_Quality( uQuality, COND_TYPE_EQU );
                    CSMV_Rxd_Time_SNTP = XJ_Retrieve_Clock();
                }
                else
                {
                    /* 调整年份错误计数器 */
                    CSMV_MsgCount.iRecSynYearErrCount[0] ++;
                    /* 记录错误年份 */
                    CSMV_MsgCount.iRecSynYearErrCount[1] = devTime.Year;
                }
            }
            else
            {
                /* 调整长度错误计数器 */
                CSMV_MsgCount.iRecSynLenErrCount[0] ++;
                /* 记录错误长度 */
                CSMV_MsgCount.iRecSynLenErrCount[1] = length;
            }
        }
        break;

        /* 带时间的GOOSE */
    case CSMC_COMMAND_ID_0x8D:
        {
            INT iGOOSECount;  /* goose开入个数 */
            INT iInputBit;    /* 开入索引 */
            INT iStatus;      /* 开入状态 */
            INT iGroupNo;     /* 组号 */
            UNSIGNED uBit;    /* 开入位 */

            /* 调整命令个数计数器 */
            CSMV_MsgCount.iRecGooseTimeCount ++;
            p = ptr + 1; /* 指向GOOSE数据 */
            /* 获取开入量个数 */
            iGOOSECount = * p ++;
            /* 判断报文长度是否正确 */
            if(( length == ( UINT )( CSMV_8D_MIN_LEN + CSMV_8D_DATA_LEN * iGOOSECount ) )
                && ( length > CSMV_8D_MIN_LEN ))
            {
                /* 存储开入量状态后将数据传递给应用程序 */
                for( i = 0; i < iGOOSECount; i ++)
                {
                    /* 获取开入位 */
                    XJP_CSC_Byte_To_Word(& iInputBit, p, 2 );
                    /* 判断开入索引是否正确 */
                    iGroupNo = iInputBit >> CSMV_GROUP_MASK32;
                    /* 判断开入索引是否在允许范围之内(组号) */
                    if( iGroupNo < CPC_MAX_INPUT_GROUPS )
                    {
                        /* 开入位 */
                        uBit = ( 1 << ( iInputBit & CSMV_SUB_GROUP_MASK32 ));
                        /* 获取开入位状态 */
                        iStatus = * ( p + 2 );
                        /* 判断开入状态 */
                        if( iStatus )
                        {
                            /* 投入 */
                            PLAT_SoftInputMemory[iGroupNo] |= uBit;
                        }
                        else
                        {
                            /* 退出 */
                            PLAT_SoftInputMemory[iGroupNo] &= ~ uBit;
                        }

                        /* 获取有效无效位状态 */
                        iStatus = * ( p + 3 );
                        if( iStatus )
                        {
                            /* 有效 */
                            PLAT_SoftInputQuality[iGroupNo] |= uBit;
                        }
                        else
                        {
                            /* 无效 */
                            PLAT_SoftInputQuality[iGroupNo] &= ~ uBit;
                        }

                        /* 获取检修位状态 */
                        iStatus = * ( p + 4 );
                        if( iStatus )
                        {
                            /* 检修 */
                            PLAT_SoftInputCheckQuality[iGroupNo] |= uBit;
                        }
                        else
                        {
                            /* 无检修 */
                            PLAT_SoftInputCheckQuality[iGroupNo] &= ~ uBit;
                        }
                        /* NPI发送过来的开入位为软开入的开入位,经平台处理后给应用的为装置中实际开入位 */
                        iInputBit += CINC_HARD_INPUT_GROUPS * 32;
                        XJP_CSC_Word_To_Byte( p,& iInputBit, 2 );
                    }
                    else
                    {
                        /* 调整错误开入位计数器 */
                        CSMV_MsgCount.iRecGooseTimeBitErrCount ++;
                        /* 记录错误开入位 */
                        CSMV_MsgCount.iRecGooseTimeErrBits = iInputBit;
                    }
                    /* 调整数据指针 */
                    p += CSMV_8D_DATA_LEN;
                }
                NU_Protect( pReport_Protect );
                /* 将数据写入用户数据缓冲区 */
                CSMV_MessageBuf[0] = ( length + sizeof( UNSIGNED ) - 1 )/ sizeof( UNSIGNED ) + 2; /* 实际长度提取出来 */
                CSMV_MessageBuf[1] = length;   /* 长度 */
                /* 数据从地址3开始 */
                memcpy(& CSMV_MessageBuf[2], ptr, length );
                /* 将数据写入缓冲区中 */
                XJP_Queue_Write( NPIUserDataHandle, CSMV_MessageBuf );
                NU_Unprotect();
            }
            else
            {
                /* 调整长度错误计数器 */
                CSMV_MsgCount.iRecGooseTimeLenErrCount ++;
                /* 记录错误长度 */
                CSMV_MsgCount.iRecGooseTimeErrLen = length;
            }
        }
        break;

        /* 其它数据 */
    case CSMV_COMMAND_ID_0x8B:
    case CSMV_COMMAND_ID_0x8E:
    case CSMV_COMMAND_ID_0x8F:
    case CSMV_COMMAND_ID_0x90:
        {
            CSMV_MsgCount.iOtherCount ++;
            NU_Protect( pReport_Protect );
            CSMV_MessageBuf[0] = ( length + sizeof( UNSIGNED ) - 1 )/ sizeof( UNSIGNED ) + 2; /* 实际长度提取出来 */
            CSMV_MessageBuf[1] = length;   /* 长度 */
            /* 数据从地址3开始 */
            memcpy(& CSMV_MessageBuf[2], ptr, length );
            /* 将数据写入缓冲区中 */
            XJP_Queue_Write( NPIUserDataHandle, CSMV_MessageBuf );
            NU_Unprotect();
        }
        break;
    }
    /* 解析报文函数执行实时时间 */
    CSMV_MsgCount.uMsgParseRealTime = XJP_Dbg_Time_Testing(& uMillisecond, & uMicrosecond );
    /* 判断解析报文最大执行时间 */
    if( CSMV_MsgCount.uMsgParseRealTime > CSMV_MsgCount.uMsgParseMaxTime )
    {
        /* 解析报文函数执行最大时间 */
        CSMV_MsgCount.uMsgParseMaxTime = CSMV_MsgCount.uMsgParseRealTime;
    }
}

/* 发送GOOSE开出管理函数 */
VOID CSMV_Msg_SendGooseOut( VOID )
{
    static INT      count = 0;                         /* 变位发生时,待连续发送的次数 */
    static UNSIGNED Last_Put_Clock = 0;                /* 上次发送帧的时刻 */
    static INT      CBCount = CSMC_OUT_COUNT_ON_CHANGE;
    static INT      iInitFlag = 0;                     /* 初始化标志 */
    UNSIGNED_CHAR   Out_Buffer[ CPC_MAX_LENGTH_FRAME ];/* 数据发送缓冲区 */
    INT             i;
    UNSIGNED        uMessLength;
    static INT      iTotalCount, iHardCount, iSoftCount;
    UNSIGNED        NewOutQ[CPC_MAX_OUTPUT_GROUPS];        /* 最新记录的开出品质因素有效无效位的状态 */
    UNSIGNED        NewOutQJ[CPC_MAX_OUTPUT_GROUPS];       /* 最新记录的开出品质因素检修位的状态 */
    UNSIGNED        NewOutStatus[CPC_MAX_OUTPUT_GROUPS];   /* 最新记录的出口位状态 */
    INT             iStatus;
    UNSIGNED        uSendDelayTime;
    /*--------------------------------------------------------------------------*/
    if( iInitFlag == 0 )
    {
        iInitFlag = 1;
        Last_Put_Clock = XJ_Retrieve_Clock();
        /* 获取开出组数 */
        OUTC_GetOutGroupCount(& iTotalCount,& iHardCount );
        /* 判断软开出组数 */
        if( iTotalCount <= iHardCount )
        {
            iSoftCount = 0;
        }
        else
        {
            iSoftCount = iTotalCount - iHardCount;
        }
        /* 检查软开出组数 */
        if( iSoftCount > CPC_MAX_OUTPUT_GROUPS )
        {
            iSoftCount = CPC_MAX_OUTPUT_GROUPS;
        }
    }

    if( iSoftCount <= 0 )
    {
        return;
    }

    /* 获取软开出状态及品质因素 */
    for( i = 0; i < iSoftCount; i ++)
    {
        COUTC_ReadSoftOutTotalQua(( iHardCount + i ),& NewOutStatus[i],& NewOutQ[i],& NewOutQJ[i] );
    }

    /* 检查品质因素及状态是否发生变化 */
    iStatus = CSMV_Msg_OutStatusCheck( NewOutStatus, NewOutQ, NewOutQJ, iSoftCount );
    if( iStatus == NU_TRUE )
    {
        /* 当品质因素发生变化的时候也要按状态变化来进行数据的发送 */
        count = CSMC_OUT_COUNT_ON_CHANGE;
        CBCount = 0;                   /* 有变位时,将发送变位计数清零 */
    }

    /* 本次开出量的发送时间 */
    uSendDelayTime = CSMV_MG_OutSendDelayTime[CBCount];
    /* 有变位时连续发送CSOP_COUNT_ON_CHANGE次,每次间隔按发送时间数组CSOP_SendDelayTime */
    if(( XJ_Retrieve_Clock() - Last_Put_Clock ) >= uSendDelayTime )
    {
        /* 首地址填写命令类型 */
        Out_Buffer[0] = CSMC_COMMAND_ID_0x8A;
        /* 填写报文及获取报文长度 */
        uMessLength = CSMC_MSG_FillOutStatus( & Out_Buffer[1], NewOutStatus, NewOutQ, NewOutQJ, iSoftCount );
        uMessLength += 1;  /* 命令类型 */
        /* 发送数据函数 */
        CSMC_MSG_SEND_GOOSE( iStatus, Out_Buffer, uMessLength );

        if( iStatus > 0 )
        {
            Last_Put_Clock = XJ_Retrieve_Clock();
            if( count > 0 )  /* 待连续发送次数不为0 */
            {
                count --;
                CBCount ++;
            }
            /* 调整发送成功计数器 */
            CSMV_MsgCount.iSendGooseSucCount ++;
        }
        else
        {
            /* 调整发送失败计数器 */
            CSMV_MsgCount.iSendGooseErrCount ++;
            /* 失败时记录当前时间 */
            CSMV_MsgCount.uSendGooseErrTime = TMD_System_Clock;
        }
    }
}

/* 向发送数据缓冲区中填写开出量组号、61850时间(秒、毫秒)、开出量状态、开出量品质品质因素 */
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

    /* 装置状态 */
    p = ( UNSIGNED_CHAR *)& CSMV_MSG_DevQStatus;
    XJP_CSC_Word_To_Byte( pTempBuf, p, CSMV_DEV_STATUS_BYTES );
    pTempBuf += CSMV_DEV_STATUS_BYTES;

    /* 开出量组数 */
    * pTempBuf ++ = iGroupNum;

    /* 读取61850时间 */
    XJP_TM_Get_Time_61850( NU_NULL, & Second_61850,
        & MillSec_61850, NU_NULL );

    /* 61850秒 */
    p = ( UNSIGNED_CHAR *)& Second_61850;
    XJP_CSC_Word_To_Byte( pTempBuf, p, CSMC_SNTP_SECOND_BYTES );
    pTempBuf += CSMC_SNTP_SECOND_BYTES;

    /* 61850纳秒 */
    p = ( UNSIGNED_CHAR *)& MillSec_61850;
    XJP_CSC_Word_To_Byte( pTempBuf, p, CSMC_SNTP_NSECOND_BYTES );
    pTempBuf += CSMC_SNTP_NSECOND_BYTES;

    for( i = 0; i < iGroupNum; i ++)
    {
        /* 开出状态 */
        p = ( UNSIGNED_CHAR *)( pOutStatus + i );
        XJP_CSC_Word_To_Byte( pTempBuf, p, CSMC_GOOSE_DATA_BYTES );
        pTempBuf += CSMC_GOOSE_DATA_BYTES;

        /* 开出品质因素(有效无效位) */
        p = ( UNSIGNED_CHAR *)( pQuality + i );
        XJP_CSC_Word_To_Byte( pTempBuf, p, CSMC_GOOSE_QUA_BYTES );
        pTempBuf += CSMC_GOOSE_QUA_BYTES;

        /* 开出品质因素(检修位) */
        p = ( UNSIGNED_CHAR *)( pCheckQua + i );
        XJP_CSC_Word_To_Byte( pTempBuf, p, CSMC_GOOSE_TEST_BYTES );
        pTempBuf += CSMC_GOOSE_TEST_BYTES;
    }

    iSize = ( INT )( pTempBuf - pBuffer );
    return( iSize );
}

/* 判断品质因素及状态是否发生变化(NU_TRUE:变化;NU_FALSE:没有变化) */
static INT CSMV_Msg_OutStatusCheck( UNSIGNED * pStatus, UNSIGNED * pQuality, UNSIGNED * pCheckQua, INT iGroupNum )
{
    static UNSIGNED LastOutQ[CPC_MAX_OUTPUT_GROUPS];       /* 上次记录的开出品质因素有效无效位的状态 */
    static UNSIGNED LastOutQJ[CPC_MAX_OUTPUT_GROUPS];      /* 上次记录的开出品质因素检修位的状态 */
    static UNSIGNED LastOutStatus[CPC_MAX_OUTPUT_GROUPS];  /* 上次出口位状态 */
    static INT iInitFlag = 0;
    UNSIGNED NewOutQ[CPC_MAX_OUTPUT_GROUPS];        /* 最新记录的开出品质因素有效无效位的状态 */
    UNSIGNED NewOutQJ[CPC_MAX_OUTPUT_GROUPS];       /* 最新记录的开出品质因素检修位的状态 */
    UNSIGNED NewOutStatus[CPC_MAX_OUTPUT_GROUPS];   /* 最新记录的出口位状态 */
    INT    iStatus = NU_FALSE;
    INT    i, iCount;
    static UNSIGNED DevQStatus = 0;             /* 装置状态,传输给NPI插件 */
    /*======================================*/
    if( iInitFlag == 0 )
    {
        /* 清缓冲区 */
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
    /* 记录最新状态及品质因素 */
    memcpy( NewOutStatus, pStatus, sizeof( UNSIGNED ) * iCount );
    memcpy( NewOutQ, pQuality, sizeof( UNSIGNED ) * iCount );
    memcpy( NewOutQJ, pCheckQua, sizeof( UNSIGNED ) * iCount );

    /* 判断品质因素是否发生变化 */
    for( i = 0; i < iCount; i ++)
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
        iStatus = NU_TRUE;
    }

    /* 更新品质因素及状态 */
    memcpy( LastOutStatus, NewOutStatus, sizeof( UNSIGNED ) * iCount );
    memcpy( LastOutQ, NewOutQ, sizeof( UNSIGNED ) * iCount );
    memcpy( LastOutQJ, NewOutQJ, sizeof( UNSIGNED ) * iCount );
    /* 装置状态 */
    DevQStatus = CSMV_MSG_DevQStatus;
    return( iStatus );
}

/* 设置装置状态函数 */
INT CSMV_MSG_SetDevStatus( UNSIGNED uDevStatus )
{
    CSMV_MSG_DevQStatus = uDevStatus;
    return( NU_SUCCESS );
}

/*================================================*/
/* 用户从NPI侧接收数据 */
INT CSMV_MSG_GetDataFromNpi( UNSIGNED_CHAR * buffer, INT * Length )
{
    INT iStatus = NU_FALSE;
    UNSIGNED Size;
    UNSIGNED MessageBuf[CSMV_MAX_FRAME_LEN_UN + 2];
    /*--------------------------------------*/
    /* 判断是否有用户数据 */
    if( XJP_Queue_Find( NPIUserDataHandle, & Size ) == NU_TRUE )
    {
        NU_Protect( pReport_Protect );
        iStatus = XJP_Queue_Read( NPIUserDataHandle , & MessageBuf );
        NU_Unprotect();
        /* 读取用户数据 */
        if( iStatus == NU_SUCCESS )
        {
            /* 报文长度 */
            if( Length )
            {
                /* 报文长度 */
                * Length = MessageBuf[1];
            }
            /* 报文数据 */
            if( buffer )
            {
                /* 报文数据 */
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
/* 用户向NPI侧发送数据函数 */
INT CSMV_MSG_UserSendDataTONpi( UNSIGNED_CHAR * buffer, INT Length )
{
    static INT iInitFlag = 0;
    INT iStatus = NU_FALSE;
    /*--------------------------------------*/
    /* 判断缓冲区是否初始化 */
    if( iInitFlag == 0 )
    {
        memset( CSMV_Msg_UserSendDataBuffer, 0, sizeof( CSMV_Msg_UserSendDataBuffer ));
        iInitFlag = 1;
    }

    /* 缓冲区的起始地址存储的是报文长度,第二个地址存储到是报文类型 */
    if( CSMV_Msg_UserSendDataBuffer[0] == 0 )
    {
        if(( Length <= CPC_MAX_LENGTH_FRAME ) && ( Length > 0 ))
        {
            /* 拷贝发送数据 */
            memcpy( & CSMV_Msg_UserSendDataBuffer[1], buffer, Length );
            /* 最后设置缓冲区长度 */
            CSMV_Msg_UserSendDataBuffer[0] = Length;
            iStatus = NU_TRUE;
        }
    }

    return( iStatus );
}

/*================================================*/
/* 发送用户数据 */
INT  CSMV_MSG_UserSendData( VOID )
{
    INT           iStatus;
    INT           iStatus1 = NU_UNAVAILABLE;
    UNSIGNED      uMessLength;
    /*--------------------------------------*/
    /* 发送应用数据 */
    if( CSMV_Msg_UserSendDataBuffer[0] != 0 )
    {
        /* 报文长度 */
        uMessLength = CSMV_Msg_UserSendDataBuffer[0];
        /* 发送数据 */
        CSMC_MSG_SEND_GOOSE( iStatus, ( VOID *)& CSMV_Msg_UserSendDataBuffer[1], uMessLength );
        if( iStatus > 0 )
        {
            /* 清报文长度 */
            CSMV_Msg_UserSendDataBuffer[0] = 0;
            /* 函数返回状态值成功状态 */
            iStatus1 = NU_SUCCESS;
        }
    }
    return( iStatus1 );
}

/*================================================*/
/* NPU向NPI发送对时命令 */
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
        /* 置发送对时命令标识 */
        iSendFlag = NU_TRUE;
        /* 初始化延时时间 */
        uDelayTime = TMD_System_Clock;
    }
    else
    {
        /* 判断延时时间是否到 */
        if(( TMD_System_Clock - uDelayTime ) > CSMC_SYNTIME_DELAY_TIME )
        {
            /* 更新发送延时时间 */
            uDelayTime = TMD_System_Clock;
            /* 置发送对时命令标识 */
            iSendFlag = NU_TRUE;
        }
    }
    /* 判断是否发送对时命令 */
    if( iSendFlag == NU_TRUE )
    {
        /* 组织对时报文 */
        iLen = 0;
        /* 清缓冲区 */
        memset( uTestBuf, 0, sizeof( uTestBuf ));
        /* 报文类型 */
        uTestBuf[iLen ++ ] = CSMC_COMMAND_ID_0x83;
        /* 读取时间 */
        XJP_TM_Get_Time_61850( & SynTime, & Second_61850,
            & NandSec_61850, & Quality );
        /* 加上装置的起始时间(2000) */
        SynTime.Year += CSMC_DEVICE_START_YEAR;
        /* 年 */
        p = ( UNSIGNED_CHAR *)&( SynTime.Year );
        /* 年:两个字节 */
        XJP_CSC_Word_To_Byte( & uTestBuf[iLen], p, 2 );
        /* 年:两个字节 */
        iLen += 2;
        /* 月 */
        uTestBuf[iLen ++ ] = ( UNSIGNED_CHAR )SynTime.Month;
        /* 日 */
        uTestBuf[iLen ++ ] = ( UNSIGNED_CHAR )SynTime.Day;
        /* 时 */
        uTestBuf[iLen ++ ] = ( UNSIGNED_CHAR )SynTime.Hour;
        /* 分 */
        uTestBuf[iLen ++ ] = ( UNSIGNED_CHAR )SynTime.Minute;
        /* 秒 */
        uTestBuf[iLen ++ ] = ( UNSIGNED_CHAR )( SynTime.Millionsecond % 1000 );

        /* 61850秒 */
        p = ( UNSIGNED_CHAR *)& Second_61850;
        XJP_CSC_Word_To_Byte( & uTestBuf[iLen], p, CSMC_SNTP_SECOND_BYTES );
        iLen += CSMC_SNTP_SECOND_BYTES;

        /* 61850纳秒 */
        p = ( UNSIGNED_CHAR *)& NandSec_61850;
        XJP_CSC_Word_To_Byte( & uTestBuf[iLen], p, CSMC_SNTP_NSECOND_BYTES );
        iLen += CSMC_SNTP_NSECOND_BYTES;

        /* 时间品质因素 */
        uTestBuf[iLen] = ( UNSIGNED_CHAR )Quality;
        iLen += CSMC_SNTP_QUALITY_BYTES;

        /* 发送数据函数 */
        CSMC_MSG_SEND_GOOSE( iStatus, uTestBuf, iLen );

        /* 判断是否发送成功 */
        if( iStatus > 0 )
        {
            /* 调整发送83命令计数器 */
            CSMV_MsgCount.iSendSynSucCount ++;
        }
        else
        {
            /* 调整发送对时命令失败个数计数器 */
            CSMV_MsgCount.iSendSynErrCount ++;
            /* 记录发送对时错误时的错误时间 */
            CSMV_MsgCount.iSendSynErrTime = TMD_System_Clock;
        }
    }
    /* 接收开入量超时检测函数 */
    //CSMV_MSG_Input_TimeOutCheck();
}

/*================================================*/
/* 注册向NPI发送数据接口函数 */
VOID CSMV_RegTONpiSendData( INT (* RegSendDataFun )( UNSIGNED_CHAR * data, INT length ))
{
    CSMV_TONpiSendData = RegSendDataFun;
}

/*================================================*/
/* 发送数据函数 */
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
/* 接收开入量超时检测函数 */
VOID CSMV_MSG_Input_TimeOutCheck( VOID )
{
    static INT      iFlag = 0; /* 初始化标志 */
    static UNSIGNED CheckTime;
    static INT      iCheckFlag;/* 是否检测标志 */
    INT             i;
    char            EventName[LS_ALARM_NAME];
    /*------------------------------------------------------*/
    /* 判断是否有软开入 */
    if( GET_SOFT_INGROUP())
    {
        /* 初始化基本数据 */
        if( iFlag == 0 )
        {
            iFlag = 1;
            iCheckFlag = 0;
            CheckTime = CSMV_Rxd_Time_GOOSE;
        }
        else
        {
            /* 判断是否需要更新检测时间 */
            if( CheckTime == CSMV_Rxd_Time_GOOSE )
            {
                /* 判断此次时间是否已经检测过 */
                if( iCheckFlag == 0 )
                {
                    /* 判断时间是否超时 */
                    if(( TMD_System_Clock - CheckTime ) >= CSMV_MSG_INPUT_DELAY )
                    {
                        /* 置已检测过标志 */
                        iCheckFlag = 1;
                        /* 将开入量的品质因素设置为无效状态 */
                        for( i = 0; i < CPC_MAX_INPUT_GROUPS; i ++)
                        {
                            PLAT_SoftInputQuality[i] = 0;
                        }
                        /* 设置接收开入量超时内部事件 */
                        sprintf( EventName, " %03d", LS_RES_EVENT_GETIN_OVERTIME );
                        CEMC_FillEvent( EventName, RESERVE_EVENT_ID );
                    }
                }
            }
            else
            {
                /* 置检测标志 */
                iCheckFlag = 0;
                /* 更新检测时间 */
                CheckTime = CSMV_Rxd_Time_GOOSE;
            }
        }
    }
}

/*================================================*/
/* 设置模拟量通道品质因素录波状态量的起始组号及组数 */
INT CSMV_MSG_SetChlQStateInfo( INT iStartGroup, INT iGroupCount )
{
    CSMV_MsgStateChlQStartGroup = iStartGroup;
    CSMV_MsgStateChlQGroupCount = iGroupCount;
    /* 判断起始组号是否在录波状态量的允许范围之内 */
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
/* 获取模拟量通道品质因素录波状态量的起始组号及组数 */
INT CSMV_MSG_GetChlQStateInfo( INT * iStartGroup, INT * iGroupCount )
{
    * iStartGroup = CSMV_MsgStateChlQStartGroup;
    * iGroupCount = CSMV_MsgStateChlQGroupCount;
    return( NU_SUCCESS );
}

/*================================================*/
/* 获取采样缓冲区及采样序号 */
float * CSMV_MSG_GetSVDataInfo( UNSIGNED * SampleSerial )
{
    if( SampleSerial )
    {
        * SampleSerial = CSMC_SampleSerial;
    }

    return( CSMC_Analog );
}

/*================================================*/
