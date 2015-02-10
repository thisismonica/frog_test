
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
/*      lsc.c                                           Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      LS - Lonworks Communication to Supervisor                        */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the Lonworks            */
/*      Communication to Supervisor.                                     */
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
/*      ls_defs.h              Lonworks Communication to Supervisor      */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       02-20-2003      Created initial version 1.0      */
/*      YanJuanJiang    11-23-2004      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "plat/es_plat.h"
#include "plat/net_inc/ls_extr.h"
#include "plat/net_inc/lc_extr.h"
#include "plat_specific/plat_specific.h"
#include "plat/net_inc/lc_bcall.h"
#include "plat/net_inc/lsc_b6.h"
#include "plat/net_inc/rcc_defs.h"
#include "plat/inc/csmv_extr.h"
#include "plat/inc/cin_extr.h"
#include "plat/inc/ctb_extr.h"
#include "plat/inc/tab_defs.h"
#include "plat/inc/xj_out_check.h"
#include "plat/inc/apf_extr.h"
#include "plat/inc/mpf_extr.h"
#include "plat/inc/gpf_extr.h"
#include "plat/inc/devi_extr.h"
#include "plat/inc/xj_clog.h"
#include "plat/inc/xj_cmemc.h"

/* Define external inner-component global data references.  */

extern CEVENT  CAlarmEvent;
extern CEVENT  CReserveEvent;
extern CEVENT  COperateEvent;
extern CEVENT  PRO_EventRecord[];
extern CREPORT CReport;
extern CWAVE   CWave;

extern NU_TIMER Timer_Trip_CB;
extern NU_TIMER Timer_ResetRelay_CB;
/* Define internal function calls.  */

extern VOID Prog_Execute_Debug_Program( UNSIGNED );
extern VOID Fcopy_Lead_Debug_Program( VOID );

/* 采样数据信息 */
extern float FOURIER01_R[ ];
extern float FOURIER01_I[ ];
extern float FOURIER02_R[ ];
extern float FOURIER02_I[ ];
extern float FOURIER03_R[ ];
extern float FOURIER03_I[ ];
extern float FOURIER05_R[ ];
extern float FOURIER05_I[ ];

/* 以下为OS相关配置 */
#ifdef WIN32
#endif

#ifndef WIN32 /* 非windows缺省定义一些宏以便通过编译 */
#endif

/* 平台出口管理句柄 */
extern HIOM PlatHIOM;

CHAR * LSC_CommChl[] =
{
    "",
    "menu",
    "print",
    "pc",
    "gws",
    "comtrade",
    "103",
    "",
};

/* 注册通信命令 */
VOID LSC_RegisterCommand_Local( GW_CHANNEL * pChl )
{
    /* 优先级缓冲区初始化函数 */
    LSC_B6_ProirityInit();
#if SWITCH_IEC103_TI_M_81
    /* 召唤装置基本信息 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_81, LSC_Load_Device_Inf );
#endif
#if SWITCH_IEC103_TI_M_82
    /* 装置呼叫命令 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_82, LSC_Device_Call );
#endif
#if ( LC_LINK_HMI_PHY_COM )
#if SWITCH_IEC103_TI_M_90
    /* 进入下传程序预备状态 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_90, LSC_Program_Get_Ready );
#endif
#if SWITCH_IEC103_TI_M_91
    /* 进入下传程序状态 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_91, LSC_Program_Make_Ready );
#endif
#endif
#if SWITCH_IEC103_TI_M_96
    /* 预发固化保护配置 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_96, LSC_Ready_Configure_Program );
#endif
#if SWITCH_IEC103_TI_M_97
    /* 执行固化保护配置 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_97, LSC_Execute_Configure_Program );
#endif
#if SWITCH_IEC103_TI_M_98
    /* 查询保护配置固化结果 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_98, LSC_Load_Configure_Program_State );
#endif
#if SWITCH_IEC103_TI_M_A0
    /* 召唤数据表的总个数 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_A0, LSC_ACK_Datatab );
#endif
#if SWITCH_IEC103_TI_M_A1
    /* 召唤数据表的信息 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_A1, LSC_ACK_Datatab_Info );
#endif
#if SWITCH_IEC103_TI_M_A2
    /* 召唤字段信息 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_A2, LSC_ACK_Field_Info );
#endif
#if SWITCH_IEC103_TI_M_A3
    /* 召唤表中数据的总个数 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_A3, LSC_ACK_Data_Number );
#endif
#if SWITCH_IEC103_TI_M_A4
    /* 召唤记录的全部字段 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_A4, LSC_ACK_All_Field_Value );
#endif
#if SWITCH_IEC103_TI_M_A5
    /* 召唤记录的单个字段 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_A5, LSC_ACK_Field_Value );
#endif
#if SWITCH_IEC103_TI_M_A6
    /* 召唤记录的多个字段 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_A6, LSC_ACK_Fields_Value );
#endif
#if SWITCH_IEC103_TI_M_A7
#if 0 /* 在后台任务注册 */
    /* 设置配置字段的值 */
    LCC_RegcBackGroundComm( pChl, IEC103_TI_M_A7, LSC_Set_Config_Values );
#endif
#endif
    /* 将选择和设置放在后台任务中的原因为AA放到后台任务中,避免出现多次下发选择和设置命令 */
#if SWITCH_IEC103_TI_M_A8
#if 0 /* 在后台任务注册 */
    /* 选择编辑字段 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_A8, LSC_Slecet_Edit_SG );
#endif
#endif
#if SWITCH_IEC103_TI_M_A9
#if 0 /* 在后台任务注册 */
    /* 设置编辑定值组的值 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_A9, LSC_Set_Edit_SGV );
#endif
#endif
#if SWITCH_IEC103_TI_M_AA
#if 0 /* 在后台任务注册 */
    /* 确认编辑定值组的值 */
    LCC_RegcBackGroundComm( pChl, IEC103_TI_M_AA, LSC_Conform_Edit_SGV );
#endif
#endif
#if SWITCH_IEC103_TI_M_AB
    /* 读取数据表的状态 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_AB, LSC_Get_Tab_Status );
#endif
#if SWITCH_IEC103_TI_M_AC
    /* 读取数据表的配置CRC码 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_AC, LSC_Get_Config_Tab_CRC );
#endif
#if SWITCH_IEC103_TI_M_AD
    /* 冻结数据表的数据 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_AD, LSC_Freeze_Table );
#endif
#if SWITCH_IEC103_TI_M_AE
    /* 解冻数据表的数据 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_AE, LSC_UnFreeze_Table );
#endif
#if SWITCH_IEC103_TI_M_AF
#if 0 /* 在后台任务注册 */
    /* 设置多个配置字段的值 */
    LCC_RegcBackGroundComm( pChl, IEC103_TI_M_AF, LSC_Set_Batch_Config_Values );
#endif
#endif
#if SWITCH_IEC103_TI_M_B0
#if 0 /* 在后台任务注册 */
    /* 保存多个配置字段的值 */
    LCC_RegcBackGroundComm( pChl, IEC103_TI_M_B0, LSC_Save_Batch_Config_Values );
#endif
#endif
#if SWITCH_IEC103_TI_M_C0
    /* 召唤事件类型 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C0, LSC_Load_Event_Type );
#endif
#if SWITCH_IEC103_TI_M_C1
#if 0 /* 在后台任务注册 */
    /* 召唤事件信息 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C1, LSC_Load_Event_Inf, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_C2
#if 0 /* 在后台任务注册 */
    /* 召唤事件索引号 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C2, LSC_Load_Event_Serial, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_C3
#if 0 /* 在后台任务注册 */
    /* 召唤事件数据 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C3, LSC_Load_Event_Data, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_C4
#if 0 /* 在后台任务注册 */
    /* 清除事件数据 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C4, LSC_Clear_Alarm_Event, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_C5
#if 0 /* 在后台任务注册 */
    /* 召唤报告信息 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C5, LSC_Load_Report_Inf, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_C6
#if 0 /* 在后台任务注册 */
    /* 召唤报告故障序号 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C6, LSC_Load_Report_FaultSerl, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_C7
#if 0 /* 在后台任务注册 */
    /* 召唤报告索引序号 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C7, LSC_Load_Report_Serial, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_C8
#if 0 /* 在后台任务注册 */
    /* 召唤报告数据 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C8, LSC_Load_Report_Data, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_C9
#if 0 /* 在后台任务注册 */
    /* 清除故障记录数据 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C9, LSC_Clear_FaultData, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_CA
    /* 手动启动录波 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_CA, LSC_Start_Wave );
#endif
#if SWITCH_IEC103_TI_M_CB
#if 0 /* 在后台任务注册 */
    /* 召唤录波序号 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_CB, LSC_Load_Wave_ID, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_CC
#if 0 /* 在后台任务注册 */
    /* 召唤录波时间 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_CC, LSC_Load_Wave_Time, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_CD
#if 0 /* 在后台任务注册 */
    /* 召唤报告信息 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_CD, LSC_Load_Wave_Inf, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_CE
#if 0 /* 在后台任务注册 */
    /* 召唤报告故障序号 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_CE, LSC_Load_Wave_FaultSerl, 3, 100 );
#endif
#endif
#if SWITCH_IEC103_TI_M_E0
    /* 预发传动开出 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_E0, LSC_Trip_Test_Ready );
#endif
#if SWITCH_IEC103_TI_M_E1
    /* 执行传动开出 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_E1, LSC_Trip_Test );
#endif
#if SWITCH_IEC103_TI_M_E2
    /* 调通道系数及量值 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_E2, LSC_Load_Channel_Cofficient );
#endif
#if SWITCH_IEC103_TI_M_E3
    /* 修改通道系数 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_E3, LSC_Change_Channel_Cofficient );
#endif
#if SWITCH_IEC103_TI_M_E4
    /* 调通道额定值 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_E4, LSC_Load_Channel_Rate );
#endif
#if SWITCH_IEC103_TI_M_E5
    /* 修改通道额定值 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_E5, LSC_Change_Channel_Rate );
#endif
#if SWITCH_IEC103_TI_M_E6
    /* 调装置零漂刻度及零漂值 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_E6, LSC_Load_Channel_Excursion );
#endif
#if SWITCH_IEC103_TI_M_E7
    /* 修改装置零漂 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_E7, LSC_Change_Channel_Excursion );
#endif
#if SWITCH_IEC103_TI_M_EA
    /* 上调模拟通道相位误差定值 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_EA, LSC_Load_Channel_Phase );
#endif
#if SWITCH_IEC103_TI_M_E9
    /* 修改通道相位误差 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_E9, LSC_Change_Channel_Phase );
#endif
#if SWITCH_IEC103_TI_M_F0
    /* 信号复归 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_F0, LSC_Reset );
#endif
#if SWITCH_IEC103_TI_M_F1
    /* 召唤开入量状态及GPS脉冲计数 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_F1, LSC_Load_Input_Status );
#endif
#if SWITCH_IEC103_TI_M_F7
    /* 读内存数据 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_F7, LSC_Read_Memory );
#endif
}

#if SWITCH_IEC103_TI_M_81
/* 召唤装置基本信息 */
INT LSC_Load_Device_Inf( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR * pSrc,* pDst,* p;
    UNSIGNED      uTemp;
    UNSIGNED      verL, verH, toolCRC;
    //--------------------------------------------------------------------------
    LS_UNUSED_PARAM( pChl );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 27;                                      /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_81;                          /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );      /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                           /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                           /* 序列号高字节 */

    /* 装置CRC码 */
    uTemp = LS_GET_DEV_CRC;
    p = ( UNSIGNED_CHAR *)& uTemp;
    LS_WORD_TO_BYTE( pDst, p, 2 );                      /* 装置CRC码转换成2字节 */
    pDst += 2;                                          /* 2字节CRC码 */

    /* 装置版本号 */
    if( APFC_GetDeviceVersion(& verL, & verH ) == NU_SUCCESS )
    {
        * pDst ++ = ( UNSIGNED_CHAR )( verL & LS_DATA_MASKS8 ); /* 装置版本号低字节 */
        * pDst ++ = ( UNSIGNED_CHAR )( verH & LS_DATA_MASKS8 ); /* 装置版本号高字节 */
    }
    else
    {
        * pDst ++ = 0;                                  /* 装置版本号低字节 */
        * pDst ++ = 0;                                  /* 装置版本号高字节 */
    }

    /* 平台CRC码 */
    uTemp = LS_GET_PLAT_CRC;
    p = ( UNSIGNED_CHAR *)& uTemp;
    LS_WORD_TO_BYTE( pDst, p, 2 );                      /* 平台CRC码转换成2字节 */
    pDst += 2;                                          /* 2字节CRC码 */

    /* 平台版本号 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_PLATFORM_VER & LS_DATA_MASKS8 );
    * pDst ++ = ( UNSIGNED_CHAR )(( LS_PLATFORM_VER >> LS_BYTE_BITS ) & LS_DATA_MASKS8 );

    if( MPFC_GetHmiVersion(& verL, & verH, & toolCRC ) == NU_SUCCESS )
    {
        /* 人机接口配置工具CRC码 */
        p = ( UNSIGNED_CHAR *)& toolCRC;
        LS_WORD_TO_BYTE( pDst, p, 2 );                      /* 人机接口CRC码转换成2字节 */
        pDst += 2;                                          /* 人机接口CRC码转换成2字节 */
        /* 人机接口CRC码 */
        uTemp = LS_GET_HMI_CRC;
        p = ( UNSIGNED_CHAR *)& uTemp;
        LS_WORD_TO_BYTE( pDst, p, 2 );                      /* 人机接口CRC码转换成2字节 */
        pDst += 2;                                          /* 2字节CRC码 */
        /* 人机接口版本号 */
        * pDst ++ = ( UNSIGNED_CHAR )( verL & LS_DATA_MASKS8 ); /* 人机接口版本号低字节 */
        * pDst ++ = ( UNSIGNED_CHAR )( verH & LS_DATA_MASKS8 ); /* 人机接口版本号高字节 */
    }
    else
    {
        /* 人机接口配置工具CRC码 */
        * pDst ++ = 0;                                  /* 人机接口CRC码低字节 */
        * pDst ++ = 0;                                  /* 人机接口CRC码高字节 */
        /* 人机接口CRC码 */
        uTemp = LS_GET_HMI_CRC;
        p = ( UNSIGNED_CHAR *)& uTemp;
        LS_WORD_TO_BYTE( pDst, p, 2 );                      /* 人机接口CRC码转换成2字节 */
        pDst += 2;                                      /* 人机接口CRC码转换成2字节 */
        /* 人机接口版本号 */
        * pDst ++ = 0;                                  /* 人机接口版本号低字节 */
        * pDst ++ = 0;                                  /* 人机接口版本号高字节 */
    }

    /* 网关CRC码 */
    uTemp = LS_GET_GWS_CRC;
    p = ( UNSIGNED_CHAR *)& uTemp;
    LS_WORD_TO_BYTE( pDst, p, 2 );                      /* 网关CRC码转换成2字节 */
    pDst += 2;                                          /* 2字节CRC码 */

    /* 网关版本号 */
    if( GPFC_GetGwsVersion(& verL, & verH ) == NU_SUCCESS )
    {
        * pDst ++ = ( UNSIGNED_CHAR )( verL & LS_DATA_MASKS8 ); /* 网关版本号低字节 */
        * pDst ++ = ( UNSIGNED_CHAR )( verH & LS_DATA_MASKS8 ); /* 网关版本号高字节 */
    }
    else
    {
        * pDst ++ = 0;                                  /* 网关版本号低字节 */
        * pDst ++ = 0;                                  /* 网关版本号高字节 */
    }

    /* 通信版本号 */
    * pDst ++  = ( UNSIGNED_CHAR )( LS_COMMU_VER & LS_DATA_MASKS8 );
    * pDst ++  = ( UNSIGNED_CHAR )(( LS_COMMU_VER >> LS_BYTE_BITS ) & LS_DATA_MASKS8 );

    * pDst ++= ( UNSIGNED_CHAR )( LS_SET_ZONE_COUNT );  /* 装置定值区个数 */

    /* CPLD版本号 */
    uTemp = DEVI_GetCPLDVersion();
    p = ( UNSIGNED_CHAR *)& uTemp;
    LS_WORD_TO_BYTE( pDst, p, 2 );                      /* CPLD版本号转换成2字节 */
    pDst += 2;                                          /* 2字节CPLD版本号 */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_82
/* 装置呼叫命令 */
INT LSC_Device_Call( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT           Status;
    UNSIGNED_CHAR * pSrc,* pDst;
    UNSIGNED      LedFrequency, LedStatus, LedDelay;
    //--------------------------------------------------------------------------
    LS_UNUSED_PARAM( pChl );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 9;                                       /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_82;                          /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );      /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                           /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                           /* 序列号高字节 */
    * pDst ++ = *( pSrc + 5 );                           /* 闪烁频率 */
    * pDst ++ = *( pSrc + 6 );                           /* 闪烁时间低字节 */
    * pDst ++ = *( pSrc + 7 );                           /* 闪烁时间高字节 */

    LedFrequency = *( pSrc + 5 ) & LS_DATA_MASKS8;       /* 闪烁频率 */
    /* 闪烁时间 */
    LedDelay = (*( pSrc + 6 ) & LS_DATA_MASKS8 );                    /* 闪烁时间低字节 */
    LedDelay += ((*( pSrc + 7 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* 闪烁时间高字节 */
    if( LedFrequency >= 100 ) /* 常亮 */
        LedStatus = CLE_LED_LIGHT;
    else if( LedFrequency == 0 ) /* 熄灭 */
        LedStatus = CLE_LED_EXTINGUISH;
    else
        LedStatus = CLE_LED_CORUSCATE;
    /* 设置装置呼叫运行灯状态 */
    Status = XJP_Led_SetDevLedCallStatus( NU_TRUE, LedDelay, LedStatus, LedFrequency );
    * pDst ++ = ( Status & LS_DATA_MASKS8 );               /* 设置状态 */

    return( COM_DATA_FULL );
}
#endif

#if ( LC_LINK_HMI_PHY_COM )
#if SWITCH_IEC103_TI_M_90
/* 进入下传程序预备状态 */
INT LSC_Program_Get_Ready( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    //--------------------------------------------------------------------------

    LS_SET_PROG_READY();

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 5;                        /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_90;           /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );            /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );            /* 序列号高字节 */
    * pDst ++ = LS_RESULT_TRUE;           /* 执行结果 */

    return( COM_DATA_FULL );
}
#endif
#endif

#if ( LC_LINK_HMI_PHY_COM )
#if SWITCH_IEC103_TI_M_91
/* 进入下传程序状态 */
INT LSC_Program_Make_Ready( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        ActualEvent;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );

    if( LS_GET_PROG_READY(& ActualEvent ) == NU_SUCCESS )
    {
        LS_CLR_PROG_READY();
        Fcopy_Lead_Debug_Program();
        Prog_Execute_Debug_Program( code );
    }
    else
    {
        pSrc = ( UNSIGNED_CHAR *)Src;
        pDst = ( UNSIGNED_CHAR *)Dst;

        * pDst ++ = 5;                                        /* 链路用户数据长度 */
        * pDst ++ = IEC103_TI_M_91;                           /* 类别标识 */
        * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );       /* CPU号码 */
        * pDst ++ = *( pSrc + 3 );                            /* 序列号低字节 */
        * pDst ++ = *( pSrc + 4 );                            /* 序列号高字节 */
        * pDst ++ = LS_RESULT_NULL;                           /* 执行结果 */
    }

    return( COM_DATA_FULL );
}
#endif
#endif

#if SWITCH_IEC103_TI_M_96
/* 预发固化保护配置 */
INT LSC_Ready_Configure_Program( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    LS_SET_CFG_READY();

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 5;                                  /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_96;                     /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                      /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                      /* 序列号高字节 */
    * pDst ++ = LS_RESULT_TRUE;                     /* 执行结果 */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_97
/* 执行固化保护配置 */
INT LSC_Execute_Configure_Program( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        ActualEvent;
    INT             iStatus = LS_RESULT_NULL;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    if( LS_GET_CFG_READY(& ActualEvent ) == NU_SUCCESS )
    {
        LS_CLR_CFG_READY();       /* 清除配置预备态 */
        LS_SET_CFG_PROGRAM();     /* 设置配置固化状态 */
        iStatus = LS_RESULT_TRUE;
    }

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 5;                                        /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_97;                           /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );       /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                            /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                            /* 序列号高字节 */
    * pDst ++ = ( UNSIGNED_CHAR )iStatus;                 /* 执行结果 */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_98
/* 查询保护配置固化结果 */
INT LSC_Load_Configure_Program_State( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        ActualEvent;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    if( LS_GET_CFG_PROGRAM(& ActualEvent ) == NU_SUCCESS )
    {
        /* 配置固化状态 */
        * pDst ++ = 6;                                        /* 链路用户数据长度 */
        * pDst ++ = IEC103_TI_M_98;                           /* 类别标识 */
        * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );       /* CPU号码 */
        * pDst ++ = *( pSrc + 3 );                            /* 序列号低字节 */
        * pDst ++ = *( pSrc + 4 );                            /* 序列号高字节 */
        * pDst ++ = LS_RESULT_NULL;                           /* 固化状态 */
        * pDst ++ = LS_RESULT_NULL;                           /* 执行结果 */
    }
    else
    {
        /* 配置固化完成 */
        * pDst ++ = 6;                                        /* 链路用户数据长度 */
        * pDst ++ = IEC103_TI_M_98;                           /* 类别标识 */
        * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );       /* CPU号码 */
        * pDst ++ = *( pSrc + 3 );                            /* 序列号低字节 */
        * pDst ++ = *( pSrc + 4 );                            /* 序列号高字节 */
        * pDst ++ = LS_RESULT_TRUE;                           /* 固化状态 */

        if( LS_GET_CFG_ERROR(& ActualEvent ) == NU_SUCCESS )
        {
            * pDst ++ = LS_RESULT_NULL;                       /* 执行结果 */
        }
        else
        {
            * pDst ++ = LS_RESULT_TRUE;                       /* 执行结果 */
        }
    }

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_A0
/* 召唤数据表的总个数 */
INT LSC_ACK_Datatab( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR    * pSrc,* pDst;
    UNSIGNED           TabNumber;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    TabNumber = XJP_DS_Get_Table_Number();

    * pDst ++ = 6;                                  /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_A0;                     /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                      /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                      /* 序列号高字节 */
    * pDst ++ = ( UNSIGNED_CHAR )( TabNumber & LS_DATA_MASKS8 );        /* 数据表个数低字节 */
    * pDst = ( UNSIGNED_CHAR )(( TabNumber >> 8 ) & LS_DATA_MASKS8 );   /* 数据表个数高字节 */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_A1
/* 召唤数据表的信息 */
INT LSC_ACK_Datatab_Info( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED          number, StartNumber;
    UNSIGNED          Length;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 8;                                  /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_A1;                     /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                      /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                      /* 序列号高字节 */
    * pDst ++ = *( pSrc + 5 );                      /* 数据表的个数低字节 */
    * pDst ++ = *( pSrc + 6 );                      /* 数据表的个数高字节 */
    * pDst ++ = *( pSrc + 7 );                      /* 起始表的序号低字节 */
    * pDst ++ = *( pSrc + 8 );                      /* 起始表的序号高字节 */

    /* 数据表个数 */
    number = (*( pSrc + 5 ) & LS_DATA_MASKS8 );        /* 数据表的个数低字节 */
    number += ((*( pSrc + 6 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* 数据表的个数高字节 */

    /* 起始表的序号 */
    StartNumber = (*( pSrc + 7 ) & LS_DATA_MASKS8 );   /* 起始表的序号低字节 */
    StartNumber += ((*( pSrc + 8 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* 起始表的序号高字节 */

    Length = 0;
    XJP_DS_Get_Table_Inf( number, StartNumber, pDst, & Length );

    pDst = ( UNSIGNED_CHAR *)Dst;
    * pDst += ( UNSIGNED_CHAR ) Length;                /* 链路用户数据长度 */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_A2
/* 召唤字段信息 */
INT LSC_ACK_Field_Info( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED          number, StartNumber, TabIndex, ActualNumber;
    UNSIGNED          Length;
    INT               iStatus;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 10;                                 /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_A2;                     /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                      /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                      /* 序列号高字节 */
    * pDst ++ = *( pSrc + 5 );                      /* 表索引号低字节 */
    * pDst ++ = *( pSrc + 6 );                      /* 表索引号高字节 */
    * pDst ++ = *( pSrc + 7 );                      /* 字段个数低字节 */
    * pDst ++ = *( pSrc + 8 );                      /* 字段个数高字节 */
    * pDst ++ = *( pSrc + 9 );                      /* 起始字段的序号低字节 */
    * pDst ++ = *( pSrc + 10 );                     /* 起始字段的序号高字节 */

    /* 表索引号 */
    TabIndex = (*( pSrc + 5 ) & LS_DATA_MASKS8 );                    /* 表索引号低字节 */
    TabIndex += ((*( pSrc + 6 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* 表索引号高字节 */

    /* 字段个数 */
    number = (*( pSrc + 7 ) & LS_DATA_MASKS8 );                    /* 字段个数低字节 */
    number += ((*( pSrc + 8 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* 字段个数高字节 */

    /* 起始字段的序号 */
    StartNumber = (*( pSrc + 9 ) & LS_DATA_MASKS8 );                     /* 起始字段的序号低字节 */
    StartNumber += ((*( pSrc + 10 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* 起始字段的序号高字节 */

    Length = 0;
    iStatus = XJP_DS_Get_Field_Inf( number, StartNumber, TabIndex, pDst, & Length, & ActualNumber );

    pDst = ( UNSIGNED_CHAR *)Dst;
    if( iStatus == NU_SUCCESS )
    {
        * pDst += ( UNSIGNED_CHAR ) Length;                  /* 链路用户数据长度 */
    }
    *( pDst + 7 ) = ( UNSIGNED_CHAR ) ( ActualNumber & LS_DATA_MASKS8 ); /* 实际个数低字节 */
    *( pDst + 8 ) = ( UNSIGNED_CHAR ) (( ActualNumber >> LS_BYTE_BITS ) & LS_DATA_MASKS8 );/* 实际个数高字节 */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_A3
/* 召唤表中数据的总个数 */
INT LSC_ACK_Data_Number( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    INT               i, k;
    INT               TabIndex;
    INT               iStatus;
    TB_CFB            CondField;
    UNSIGNED          Number;
    UNSIGNED          Length;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    Length = 0;
    * pDst ++ = 9;                                  /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_A3;                     /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                      /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                      /* 序列号高字节 */
    * pDst ++ = *( pSrc + 5 );                      /* 表的索引号低字节 */
    * pDst ++ = *( pSrc + 6 );                      /* 表的索引号高字节 */
    * pDst ++ = *( pSrc + 7 );                      /* 条件个数 */

    /* 表的索引号 */
    TabIndex = (*( pSrc + 5 ) & LS_DATA_MASKS8 );                    /* 表的索引号低字节 */
    TabIndex += ((*( pSrc + 6 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* 表的索引号高字节 */

    CondField.CondNumber = (*( pSrc + 7 ) & LS_DATA_MASKS8 );        /* 条件个数 */
    Length = 7 * CondField.CondNumber;                               /* 条件数据字节数 */

    for( i = 0; i < CondField.CondNumber; i ++ )
    {
        /* 条件字段索引号 */
        CondField.CondIndex[ i ] = (*( pSrc + 8 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );/* 低字节 */
        CondField.CondIndex[ i ] += ((*( pSrc + 9 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* 高字节 */

        /* 条件的约束字段值 */
        CondField.CondValue[ i ] = 0;
        for( k = 0; k < LS_DATA_WIDTH; k ++)
        {
            /* 条件值 */
            CondField.CondValue[ i ] += ((*( pSrc + 10 + i * TB_CFB_COND_BYTE_NUMBER + k ) & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
        }

        /* 条件类型 */
        CondField.CondType[ i ] = (*( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );/* 类型值 */

        * pDst ++ = *( pSrc + 8 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件字段索引号低字节 */
        * pDst ++ = *( pSrc + 9 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件字段索引号高字节 */
        * pDst ++ = *( pSrc + 10 + i * TB_CFB_COND_BYTE_NUMBER );    /* 条件约束字段值字节1 */
        * pDst ++ = *( pSrc + 11 + i * TB_CFB_COND_BYTE_NUMBER );    /* 条件约束字段值字节2 */
        * pDst ++ = *( pSrc + 12 + i * TB_CFB_COND_BYTE_NUMBER );    /* 条件约束字段值字节3 */
        * pDst ++ = *( pSrc + 13 + i * TB_CFB_COND_BYTE_NUMBER );    /* 条件约束字段值字节4 */
        * pDst ++ = *( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER );    /* 条件类型 */
    }

    iStatus = XJP_DS_Get_Data_Number( TabIndex, & CondField, & Number );

    /* 满足条件的记录个数 */
    * pDst ++  = ( UNSIGNED_CHAR )( Number & LS_DATA_MASKS8 ); /* 低字节 */
    * pDst  = ( UNSIGNED_CHAR )(( Number >> 8 ) & LS_DATA_MASKS8 ); /* 高字节 */

    pDst = ( UNSIGNED_CHAR *)Dst;
    * pDst += ( UNSIGNED_CHAR )Length;         /* 链路用户数据长度 */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_A4
/* 召唤记录的全部字段 */
INT LSC_ACK_All_Field_Value( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    INT               i, k;
    INT               number, StartNumber, TabIndex;
    INT               iStatus;
    TB_CFB            CondField;
    INT               ActualNumber;
    UNSIGNED          Length;
    INT               iFramLenNum;   /* 一帧报文允许的最大数据个数 */
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    Length = 0;
    * pDst ++ = 10;                                 /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_A4;                     /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                      /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                      /* 序列号高字节 */
    * pDst ++ = *( pSrc + 5 );                      /* 读取数据的个数 */
    * pDst ++ = *( pSrc + 6 );                      /* 起始数据的个数低字节 */
    * pDst ++ = *( pSrc + 7 );                      /* 起始数据的个数高字节 */
    * pDst ++ = *( pSrc + 8 );                      /* 表的索引号低字节 */
    * pDst ++ = *( pSrc + 9 );                      /* 表的索引号高字节 */
    * pDst ++ = *( pSrc + 10 );                     /* 条件个数 */

    number = (*( pSrc + 5 ) & LS_DATA_MASKS8 );     /* 读取数据的个数 */

    /* 起始数据的个数 */
    StartNumber = (*( pSrc + 6 ) & LS_DATA_MASKS8 );                    /* 低字节 */
    StartNumber += ((*( pSrc + 7 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* 高字节 */

    /* 表的索引号 */
    TabIndex = (*( pSrc + 8 ) & LS_DATA_MASKS8 );                       /* 低字节 */
    TabIndex += ((*( pSrc + 9 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );    /* 高字节 */

    CondField.CondNumber = (*( pSrc + 10 ) & LS_DATA_MASKS8 );          /* 条件个数 */

    for( i = 0; i < CondField.CondNumber; i ++ )
    {
        /* 条件字段索引号 */
        CondField.CondIndex[ i ] = (*( pSrc + 11 + i * TB_CFB_COND_BYTE_NUMBER ) & LS_DATA_MASKS8 ); /* 低字节 */
        CondField.CondIndex[ i ] += ((*( pSrc + 12 + i * TB_CFB_COND_BYTE_NUMBER   ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* 高字节 */

        CondField.CondValue[ i ] = 0;
        for( k = 0; k < LS_DATA_WIDTH; k ++)
        {
            /* 条件的约束字段值 */
            CondField.CondValue[ i ] += ((*( pSrc + 13 + i * TB_CFB_COND_BYTE_NUMBER + k ) & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
        }
        CondField.CondType[ i ] = (*( pSrc + 17 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 ); /* 条件类型 */

        * pDst ++ = *( pSrc + 11 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件字段索引号低字节 */
        * pDst ++ = *( pSrc + 12 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件字段索引号高字节 */
        * pDst ++ = *( pSrc + 13 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的约束字段值字节1 */
        * pDst ++ = *( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的约束字段值字节2 */
        * pDst ++ = *( pSrc + 15 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的约束字段值字节3 */
        * pDst ++ = *( pSrc + 16 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的约束字段值字节4 */
        * pDst ++ = *( pSrc + 17 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件类型 */
    }
    XJP_DS_Get_Table_Length( TabIndex, & Length );     /* 获取数据表的长度 */
    /* 计算一帧报文允许的最大数据个数 */
    if( Length > 0 )
    {
        iFramLenNum = (( LC_MAX_FRAME_DATA_SIZE - 20 ) / Length );
        /* 判断最大允许读取的数据个数 */
        if( number > iFramLenNum )
        {
            number = iFramLenNum;
        }
    }
    iStatus = XJP_DS_Get_Field_Value_All( TabIndex, StartNumber, number, & CondField, pDst, & ActualNumber );

    pDst = ( UNSIGNED_CHAR *)Dst;
    if( ActualNumber > StartNumber )
    {
        Length *= ( ActualNumber - StartNumber );    /* 数据长度 */
        *( pDst + 5 ) = ActualNumber - StartNumber;  /* 读取数据的个数 */
    }
    else
    {
        * ( pDst + 5 ) = 0;                          /* 读取数据的个数 */
    }
    Length += 7 * CondField.CondNumber;              /* 条件长度 */

    * pDst += ( UNSIGNED_CHAR ) Length;              /* 链路用户数据长度 */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_A5
/* 召唤记录的单个字段 */
INT LSC_ACK_Field_Value( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    INT               i, k;
    INT               number, StartNumber, TabIndex;
    INT               iStatus;
    TB_CFB            CondField;
    INT               ActualNumber;
    UNSIGNED          Length;
    INT               iFramLenNum;   /* 一帧报文允许的最大数据个数 */
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    Length = 0;
    * pDst ++ = 12;                                 /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_A5;                     /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                      /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                      /* 序列号高字节 */
    * pDst ++ = *( pSrc + 5 );                      /* 读取数据的个数 */
    * pDst ++ = *( pSrc + 6 );                      /* 起始数据的个数低字节 */
    * pDst ++ = *( pSrc + 7 );                      /* 起始数据的个数高字节 */
    * pDst ++ = *( pSrc + 8 );                      /* 表的索引号低字节 */
    * pDst ++ = *( pSrc + 9 );                      /* 表的索引号高字节 */
    * pDst ++ = *( pSrc + 10 );                     /* 操作字段索引号低字节 */
    * pDst ++ = *( pSrc + 11 );                     /* 操作字段索引号高字节 */
    * pDst ++ = *( pSrc + 12 );                     /* 条件个数 */

    number = (*( pSrc + 5 ) & LS_DATA_MASKS8 );     /* 读取数据的个数 */

    /* 起始数据的个数 */
    StartNumber = (*( pSrc + 6 ) & LS_DATA_MASKS8 );                    /* 低字节 */
    StartNumber += ((*( pSrc + 7 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* 高字节 */

    /* 表的索引号 */
    TabIndex = (*( pSrc + 8 ) & LS_DATA_MASKS8 );      /* 低字节 */
    TabIndex += ((*( pSrc + 9 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* 高字节 */

    /* 操作字段索引号 */
    CondField.HandleIndex = (*( pSrc + 10 ) & LS_DATA_MASKS8 ); /* 低字节 */
    CondField.HandleIndex += ((*( pSrc + 11 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* 高字节 */

    CondField.CondNumber = (*( pSrc + 12 ) & LS_DATA_MASKS8 ); /* 条件个数 */

    for( i = 0; i < CondField.CondNumber; i ++ )
    {
        /* 条件的字段索引号 */
        CondField.CondIndex[ i ] = (*( pSrc + 13 + i * TB_CFB_COND_BYTE_NUMBER ) & LS_DATA_MASKS8 );/* 低字节 */
        CondField.CondIndex[ i ] += ((*( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* 高字节 */

        CondField.CondValue[ i ] = 0;
        for( k = 0; k < LS_DATA_WIDTH; k ++)
        {
            /* 条件的约束字段值 */
            CondField.CondValue[ i ] += ((*( pSrc + 15 + i * TB_CFB_COND_BYTE_NUMBER + k ) & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
        }
        CondField.CondType[ i ] = (*( pSrc + 19 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );/* 条件的类型 */

        * pDst ++ = *( pSrc + 13 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的字段索引号低字节 */
        * pDst ++ = *( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的字段索引号高字节 */
        * pDst ++ = *( pSrc + 15 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的约束字段值字节1 */
        * pDst ++ = *( pSrc + 16 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的约束字段值字节2 */
        * pDst ++ = *( pSrc + 17 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的约束字段值字节3 */
        * pDst ++ = *( pSrc + 18 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的约束字段值字节4 */
        * pDst ++ = *( pSrc + 19 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的类型 */
    }
    XJP_DS_Get_Field_Length( TabIndex, ( CondField.HandleIndex ), & Length );/* 获取字段的长度 */
    /* 计算一帧报文允许的最大数据个数 */
    if( Length > 0 )
    {
        iFramLenNum = (( LC_MAX_FRAME_DATA_SIZE - 20 ) / Length );
        /* 判断最大允许读取的数据个数 */
        if( number > iFramLenNum )
        {
            number = iFramLenNum;
        }
    }

    iStatus = XJP_DS_Get_Field_Value_One( TabIndex, StartNumber, number, & CondField, pDst, & ActualNumber );

    pDst = ( UNSIGNED_CHAR *)Dst;
    if( ActualNumber > StartNumber )
    {
        Length *= ( ActualNumber - StartNumber );   /* 数据长度 */
        * ( pDst + 5 ) = ActualNumber - StartNumber;/* 读取数据的个数 */
    }
    else
    {
        * ( pDst + 5 ) = 0;                         /* 读取数据的个数 */
    }
    Length += 7 * CondField.CondNumber;             /* 条件长度 */
    * pDst += ( UNSIGNED_CHAR ) Length;             /* 链路用户数据长度 */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_A6
/* 召唤记录的多个字段 */
INT LSC_ACK_Fields_Value( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    INT               i, k;
    INT               number, StartNumber, TabIndex;
    INT               iStatus;
    TB_CHB            CondField;
    INT               ActualNumber;
    UNSIGNED          Length, FieldLength;
    INT               FreezeStatus = NU_UNAVAILABLE;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    Length = 0;
    * pDst ++ = 11;                                 /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_A6;                     /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                      /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                      /* 序列号高字节 */
    * pDst ++ = *( pSrc + 5 );                      /* 读取数据的个数 */
    * pDst ++ = *( pSrc + 6 );                      /* 起始数据的个数低字节 */
    * pDst ++ = *( pSrc + 7 );                      /* 起始数据的个数高字节 */
    * pDst ++ = *( pSrc + 8 );                      /* 表的索引号低字节 */
    * pDst ++ = *( pSrc + 9 );                      /* 表的索引号高字节 */
    * pDst ++ = *( pSrc + 10 );                     /* 操作字段个数 */

    number = (*( pSrc + 5 ) & LS_DATA_MASKS8 );     /* 读取数据的个数 */

    /* 起始数据的个数 */
    StartNumber = (*( pSrc + 6 ) & LS_DATA_MASKS8 );                   /* 低字节 */
    StartNumber += ((*( pSrc + 7 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* 高字节 */

    /* 表的索引号 */
    TabIndex = (*( pSrc + 8 ) & LS_DATA_MASKS8 );                      /* 低字节 */
    TabIndex += ((*( pSrc + 9 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );   /* 高字节 */

    CondField.HandleNumber = *( pSrc + 10 );                           /* 操作字段个数 */

    for( i = 0; i < CondField.HandleNumber; i ++ )
    {
        /* 操作字段的索引号 */
        CondField.HandleIndex[ i ] = (*( pSrc + 11 + i * 2 ) & LS_DATA_MASKS8 ); /* 低字节 */
        CondField.HandleIndex[ i ] += ((*( pSrc + 12 + i * 2 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* 高字节 */

        * pDst ++ = *( pSrc + 11 + i * 2 ); /* 操作字段的索引号低字节 */
        * pDst ++ = *( pSrc + 12 + i * 2 ); /* 操作字段的索引号高字节 */
    }

    * pDst ++ = *( pSrc + 11 + 2 * CondField.HandleNumber );     /* 条件个数 */

    CondField.CondNumber = (*( pSrc + 11 + 2 * CondField.HandleNumber ) & LS_DATA_MASKS8 );/* 条件个数 */

    for( i = 0; i < CondField.CondNumber; i ++ )
    {
        /* 条件的字段索引号 */
        CondField.CondIndex[ i ]
            = (*( pSrc + 12 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER ) & LS_DATA_MASKS8 );/* 低字节 */
        CondField.CondIndex[ i ]
            += ((*( pSrc + 13 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* 高字节 */

        CondField.CondValue[ i ] = 0;
        for( k = 0; k < LS_DATA_WIDTH; k ++)
        {
            /* 条件的约束字段值 */
            CondField.CondValue[ i ] +=
                ((*( pSrc + 14 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER + k ) & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
        }
        /* 条件的类型 */
        CondField.CondType[ i ] = (*( pSrc + 18 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );

        * pDst ++ = *( pSrc + 12 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的字段索引号低字节 */
        * pDst ++ = *( pSrc + 13 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的字段索引号高字节 */
        * pDst ++ = *( pSrc + 14 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的约束字段值字节1 */
        * pDst ++ = *( pSrc + 15 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的约束字段值字节2 */
        * pDst ++ = *( pSrc + 16 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的约束字段值字节3 */
        * pDst ++ = *( pSrc + 17 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的约束字段值字节4 */
        * pDst ++ = *( pSrc + 18 + 2 * CondField.HandleNumber + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的类型 */
    }
    if( XJP_DS_Freeze_Status( TabIndex ) == NU_FALSE )
    {
        FreezeStatus = XJP_DS_Freeze_Table( TabIndex );
    }
    iStatus = XJP_DS_Get_Field_Value_Batch( TabIndex, StartNumber, number, & CondField, pDst, & ActualNumber );

    if( FreezeStatus == NU_SUCCESS )
    {
        FreezeStatus = XJP_DS_UnFreeze_Table( TabIndex );
    }

    pDst = ( UNSIGNED_CHAR *)Dst;
    Length = 0;
    if( ActualNumber > StartNumber )
    {
        * ( pDst + 5 ) = ActualNumber - StartNumber; /* 读取数据的个数 */
        for( i = 0; i < CondField.HandleNumber; i ++ )
        {
            XJP_DS_Get_Field_Length( TabIndex, ( CondField.HandleIndex[i] ), & FieldLength );/* 字段长度 */
            Length += FieldLength;
        }
        Length *= ( ActualNumber - StartNumber );   /* 数据长度 */
    }
    else
    {
        * ( pDst + 5 ) = 0;  /* 读取数据的个数 */
    }
    Length += 2 * CondField.HandleNumber; /* 操作字段长度 */
    Length += 7 * CondField.CondNumber;   /* 条件长度 */
    * pDst += ( UNSIGNED_CHAR ) Length;   /* 链路用户数据长度 */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_A7
/* 设置配置字段的值 */
INT LSC_Set_Config_Values( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR  * pSrc,* pDst;
    INT              i, k;
    INT              number, StartNumber, TabIndex;
    INT              iStatus;
    TB_CFB           CondField;
    UNSIGNED         Buffer[ LS_MAX_CONFIG_BYTE ];
    UNSIGNED_CHAR    Buffer1[ LS_MAX_CONFIG_BYTE ];
    INT              ValueWidth;
    VOID           * pTemp;
    TB_FCB          * pField;
    TB_TCB          * pTable;
    INT             iNumber;
    INT             originate;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    number = (*( pSrc + 5 ) & LS_DATA_MASKS8 );        /* 设置数据的个数 */

    /* 起始数据的个数 */
    StartNumber = (*( pSrc + 6 ) & LS_DATA_MASKS8 );   /* 低字节 */
    StartNumber += ((*( pSrc + 7 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* 高字节 */

    /* 表的索引号 */
    TabIndex = (*( pSrc + 8 ) & LS_DATA_MASKS8 );      /* 低字节 */
    TabIndex += ((*( pSrc + 9 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* 高字节 */

    /* 操作字段索引号 */
    CondField.HandleIndex = (*( pSrc + 10 ) & LS_DATA_MASKS8 );/* 低字节 */
    CondField.HandleIndex += ((*( pSrc + 11 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* 高字节 */

    ValueWidth = (*( pSrc + 12 ) & LS_DATA_MASKS8 );           /* 单个数据的字节数 */

    CondField.CondNumber = (*( pSrc + 13 ) & LS_DATA_MASKS8 ); /* 条件个数 */

    * pDst ++ = 14 + 7 *( CondField.CondNumber );   /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_A7;                     /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                      /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                      /* 序列号高字节 */
    * pDst ++ = *( pSrc + 5 );                      /* 设置数据的个数 */
    * pDst ++ = *( pSrc + 6 );                      /* 起始数据的个数低字节 */
    * pDst ++ = *( pSrc + 7 );                      /* 起始数据的个数高字节 */
    * pDst ++ = *( pSrc + 8 );                      /* 表的索引号低字节 */
    * pDst ++ = *( pSrc + 9 );                      /* 表的索引号高字节 */
    * pDst ++ = *( pSrc + 10 );                     /* 操作字段索引号低字节 */
    * pDst ++ = *( pSrc + 11 );                     /* 操作字段索引号高字节 */
    * pDst ++ = *( pSrc + 12 );                     /* 单个数据的字节数 */
    * pDst ++ = *( pSrc + 13 );                     /* 条件个数 */

    for( i = 0; i < CondField.CondNumber; i ++ )
    {
        /* 条件的字段索引号 */
        CondField.CondIndex[ i ] = (*( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER ) & LS_DATA_MASKS8 );/* 低字节 */
        CondField.CondIndex[ i ] += ((*( pSrc + 15 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* 高字节 */

        CondField.CondValue[ i ] = 0;
        for( k = 0; k < LS_DATA_WIDTH; k ++)
        {
            /* 条件的约束字段值 */
            CondField.CondValue[ i ] += ((*( pSrc + 16 + i * TB_CFB_COND_BYTE_NUMBER + k ) & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
        }
        CondField.CondType[ i ] = (*( pSrc + 20 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );/* 条件的类型 */

        * pDst ++ = *( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件字段索引号低字节 */
        * pDst ++ = *( pSrc + 15 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件字段索引号高字节 */
        * pDst ++ = *( pSrc + 16 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件约束字段值字节1 */
        * pDst ++ = *( pSrc + 17 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件约束字段值字节2 */
        * pDst ++ = *( pSrc + 18 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件约束字段值字节3 */
        * pDst ++ = *( pSrc + 19 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件约束字段值字节4 */
        * pDst ++ = *( pSrc + 20 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件类型 */
    }

    /* 通过数据表索引号及操作字段索引号,找到相应的字段信息 */
    pTable = CTBC_FindTable( TabIndex );
    pField = CTBC_FindField( pTable, CondField.HandleIndex );
    /* 一个数据的数据个数 */
    iNumber = pField->Size / pField->TypeSize;

    for( i = 0; i < ( number * ( iNumber * pField->TypeByte ) ); i ++ )
    {
        /* 接收数据 */
        Buffer1 [i] = (*( pSrc + 14 + 7 * CondField.CondNumber + i ) & LS_DATA_MASKS8 );
    }

    /* 数据宽度仅有两种1或4,当为1时缓冲区设为UNSIGNED_CHAR,4:UNSIGNED */
    if( ValueWidth == 1 )
    {
        pTemp = ( VOID *)Buffer1;
    }
    else
    {
        for( i = 0; i < ( number * iNumber ); i ++ )
        {
            Buffer[i] = 0;
            for( k = 0; k < ValueWidth; k ++)
            {
                /* 根据数据宽度合成指定格式的数据 */
                Buffer[i] += (( Buffer1[i * ValueWidth + k] & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
            }
        }
        pTemp = ( VOID *)Buffer;
    }

    originate = ( INT )(*( pSrc - 1 ) & LC_VAR_CONTROL_BITS );
    iStatus = XJP_DS_Set_Field_Value_Config( TabIndex, StartNumber, number, & CondField, pTemp, originate );

    * pDst = ( UNSIGNED_CHAR )( iStatus );

    if( iStatus != NU_SUCCESS )
    {
        k = *( pSrc - 1 ) & 0x07;  /* 有效通道 */
        sprintf( ( CHAR *)Buffer1, "装置配置数据设置失败：table：%02d field：%02d org：%s result：%d",
            TabIndex, CondField.HandleIndex, LSC_CommChl[k], iStatus );
        CLOG_Log( ( CHAR *)Buffer1 );
    }

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_A8
/* 选择编辑字段 */
INT LSC_Slecet_Edit_SG( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT               k;
    UNSIGNED_CHAR   * pSrc,* pDst;
    INT               FieldIndex, TabIndex;
    INT               iStatus;
    CHAR              Buffer1[ LS_MAX_CONFIG_BYTE ];
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 9;                                  /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_A8;                     /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                      /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                      /* 序列号高字节 */
    * pDst ++ = *( pSrc + 5 );                      /* 表的索引号低字节 */
    * pDst ++ = *( pSrc + 6 );                      /* 表的索引号高字节 */
    * pDst ++ = *( pSrc + 7 );                      /* 字段索引号低字节 */
    * pDst ++ = *( pSrc + 8 );                      /* 字段索引号高字节 */

    /* 表的索引号 */
    TabIndex = (*( pSrc + 5 ) & LS_DATA_MASKS8 );   /* 低字节 */
    TabIndex += ((*( pSrc + 6 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* 高字节 */

    /* 字段索引号 */
    FieldIndex = (*( pSrc + 7 ) & LS_DATA_MASKS8 ); /* 低字节 */
    FieldIndex += ((*( pSrc + 8 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* 高字节 */

    iStatus = XJP_DS_Select_Edit_Field_SG( TabIndex, FieldIndex );
    * pDst = ( UNSIGNED_CHAR )( iStatus );         /* 状态结果 */
    if( iStatus != NU_SUCCESS )
    {
        k = *( pSrc - 1 ) & 0x07;  /* 有效通道 */
        sprintf( Buffer1, "装置整定值数据选择失败：table：%02d field：%02d org：%s result：%d",
            TabIndex, FieldIndex, LSC_CommChl[k], iStatus );
        CLOG_Log( Buffer1 );
    }

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_A9
/* 设置编辑定值组的值 */
INT LSC_Set_Edit_SGV( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    INT               i, k;
    INT               number, StartNumber, TabIndex;
    INT               iStatus;
    TB_CFB            CondField;
    UNSIGNED          Buffer[ LS_MAX_CONFIG_BYTE ];
    UNSIGNED_CHAR     Buffer1[ LS_MAX_CONFIG_BYTE ];
    INT               ValueWidth;
    VOID            * pTemp;
    TB_FCB          * pField;
    TB_TCB          * pTable;
    INT             iNumber;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    number = (*( pSrc + 5 ) & LS_DATA_MASKS8 );        /* 设置数据的个数 */

    /* 起始数据的个数 */
    StartNumber = (*( pSrc + 6 ) & LS_DATA_MASKS8 );   /* 低字节 */
    StartNumber += ((*( pSrc + 7 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* 高字节 */

    /* 表的索引号 */
    TabIndex = (*( pSrc + 8 ) & LS_DATA_MASKS8 );      /* 低字节 */
    TabIndex += ((*( pSrc + 9 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* 高字节 */

    /* 操作字段索引号 */
    CondField.HandleIndex = (*( pSrc + 10 ) & LS_DATA_MASKS8 );/* 低字节 */
    CondField.HandleIndex += ((*( pSrc + 11 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* 高字节 */

    ValueWidth = (*( pSrc + 12 ) & LS_DATA_MASKS8 );           /* 单个数据的字节数 */

    CondField.CondNumber = (*( pSrc + 13 ) & LS_DATA_MASKS8 ); /* 条件个数 */

    * pDst ++ = 14 + 7 *( CondField.CondNumber );   /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_A9;                     /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                      /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                      /* 序列号高字节 */
    * pDst ++ = *( pSrc + 5 );                      /* 设置数据的个数 */
    * pDst ++ = *( pSrc + 6 );                      /* 起始数据的个数低字节 */
    * pDst ++ = *( pSrc + 7 );                      /* 起始数据的个数高字节 */
    * pDst ++ = *( pSrc + 8 );                      /* 表的索引号低字节 */
    * pDst ++ = *( pSrc + 9 );                      /* 表的索引号高字节 */
    * pDst ++ = *( pSrc + 10 );                     /* 操作字段索引号低字节 */
    * pDst ++ = *( pSrc + 11 );                     /* 操作字段索引号高字节 */
    * pDst ++ = *( pSrc + 12 );                     /* 单个数据的字节数 */
    * pDst ++ = *( pSrc + 13 );                     /* 条件个数 */

    for( i = 0; i < CondField.CondNumber; i ++ )
    {
        /* 条件的字段索引号 */
        CondField.CondIndex[ i ] = (*( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );/* 低字节 */
        CondField.CondIndex[ i ] += ((*( pSrc + 15 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* 高字节 */

        CondField.CondValue[ i ] = 0;
        for( k = 0; k < LS_DATA_WIDTH; k ++)
        {
            /* 条件的约束字段值 */
            CondField.CondValue[ i ] += ((*( pSrc + 16 + i * TB_CFB_COND_BYTE_NUMBER + k ) & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
        }
        CondField.CondType[ i ] = (*( pSrc + 20 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );/* 条件类型 */

        * pDst ++ = *( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的字段索引号低字节 */
        * pDst ++ = *( pSrc + 15 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的字段索引号高字节 */
        * pDst ++ = *( pSrc + 16 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的约束字段值字节1 */
        * pDst ++ = *( pSrc + 17 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的约束字段值字节2 */
        * pDst ++ = *( pSrc + 18 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的约束字段值字节3 */
        * pDst ++ = *( pSrc + 19 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的约束字段值字节4 */
        * pDst ++ = *( pSrc + 20 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件的类型 */
    }

    /* 通过数据表索引号及操作字段索引号,找到相应的字段信息 */
    pTable = CTBC_FindTable( TabIndex );
    pField = CTBC_FindField( pTable, CondField.HandleIndex );
    /* 一个数据的数据个数 */
    iNumber = pField->Size / pField->TypeSize;

    for( i = 0; i < ( number * ( iNumber * pField->TypeByte ) ); i ++ )
    {
        /* 接收数据 */
        Buffer1 [i] = (*( pSrc + 14 + 7 * CondField.CondNumber + i ) & LS_DATA_MASKS8 );
    }

    /* 数据宽度仅有两种1或4,当为1时缓冲区设为UNSIGNED_CHAR,4:UNSIGNED */
    if( ValueWidth == 1 )
    {
        pTemp = ( VOID *)Buffer1;
    }
    else
    {
        for( i = 0; i < ( number * iNumber ); i ++ )
        {
            Buffer[i] = 0;
            for( k = 0; k < ValueWidth; k ++)
            {
                /* 根据数据宽度合成指定格式的数据 */
                Buffer[i] += (( Buffer1[i * ValueWidth + k] & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
            }
        }
        pTemp = ( VOID *)Buffer;
    }

    iStatus = XJP_DS_Set_Field_Value_SG( TabIndex, StartNumber, number, & CondField, pTemp );
    * pDst = ( UNSIGNED_CHAR )( iStatus );
    if( iStatus != NU_SUCCESS )
    {
        k = *( pSrc - 1 ) & 0x07;  /* 有效通道 */
        sprintf( ( CHAR *)Buffer1, "装置整定值数据编辑失败：table：%02d field：%02d org：%s result：%d",
            TabIndex, CondField.HandleIndex, LSC_CommChl[k], iStatus );
        CLOG_Log( ( CHAR *)Buffer1 );
    }

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_AA
/* 确认编辑定值组的值 */
INT LSC_Conform_Edit_SGV( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR  * pSrc,* pDst, * p;
    INT              i, k;
    INT              Type, StartNumber = 0, TabIndex;
    INT              iStatus;
    TB_CFB           CondField;
    INT              TypeOffset = 0;
    CHAR             buffer[128];      ///
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    Type = (*( pSrc + 5 ) & LS_DATA_MASKS8 );     /* 定值类型（1：全部定值，2：单个定值） */

    if( Type == SETTING_ONE_ENABLE )
    {
        TypeOffset = 2; /* start number - 2 bytes */
        /* 起始数据的个数 */
        StartNumber = (*( pSrc + 6 ) & LS_DATA_MASKS8 ); /* 低字节 */
        StartNumber += ((*( pSrc + 7 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* 高字节 */
    }

    /* 表的索引号 */
    TabIndex = (*( pSrc + 6 + TypeOffset ) & LS_DATA_MASKS8 );    /* 低字节 */
    TabIndex += ((*( pSrc + 7 + TypeOffset ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* 高字节 */

    /* 操作字段索引号 */
    CondField.HandleIndex = (*( pSrc + 8 + TypeOffset ) & LS_DATA_MASKS8 ); /* 低字节 */
    CondField.HandleIndex += ((*( pSrc + 9 + TypeOffset ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS ); /* 高字节 */

    CondField.CondNumber = (*( pSrc + 10 + TypeOffset ) & LS_DATA_MASKS8 );  /* 条件个数 */

    * pDst ++ = 11 + TypeOffset + 7 *( CondField.CondNumber ); /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_AA;                                /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );            /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                                 /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                                 /* 序列号高字节 */
    * pDst ++ = *( pSrc + 5 );                                 /* 定值类型 */
    if( Type == SETTING_ONE_ENABLE )
    {
        /* 起始数据的个数 */
        * pDst ++ = *( pSrc + 6 );                             /* 低字节 */
        * pDst ++ = *( pSrc + 7 );                             /* 高字节 */
    }
    * pDst ++ = *( pSrc + 6 + TypeOffset );                    /* 表的索引号低字节 */
    * pDst ++ = *( pSrc + 7 + TypeOffset );                    /* 表的索引号高字节 */
    * pDst ++ = *( pSrc + 8 + TypeOffset );                    /* 操作字段索引号低字节 */
    * pDst ++ = *( pSrc + 9 + TypeOffset );                    /* 操作字段索引号高字节 */
    * pDst ++ = *( pSrc + 10 + TypeOffset );                   /* 条件个数 */

    for( i = 0; i < CondField.CondNumber; i ++ )
    {
        /* 条件的字段索引号 */
        CondField.CondIndex[ i ] = (*( pSrc + 11 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );/* 低字节 */
        CondField.CondIndex[ i ] += ((*( pSrc + 12 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/*高字节*/

        CondField.CondValue[ i ] = 0;
        for( k = 0; k < LS_DATA_WIDTH; k ++)
        {
            /* 条件的约束字段值 */
            CondField.CondValue[ i ]
                += ((*( pSrc + 13 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER + k ) & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
        }
        CondField.CondType[ i ] = (*( pSrc + 17 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );/* 条件的类型 */

        * pDst ++ = *( pSrc + 11 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER );       /* 条件的字段索引号低字节 */
        * pDst ++ = *( pSrc + 12 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER );       /* 条件的字段索引号高字节 */
        * pDst ++ = *( pSrc + 13 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER );       /* 条件的约束字段值字节1 */
        * pDst ++ = *( pSrc + 14 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER );       /* 条件的约束字段值字节2 */
        * pDst ++ = *( pSrc + 15 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER );       /* 条件的约束字段值字节3 */
        * pDst ++ = *( pSrc + 16 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER );       /* 条件的约束字段值字节4 */
        * pDst ++ = *( pSrc + 17 + TypeOffset + i * TB_CFB_COND_BYTE_NUMBER );       /* 条件的类型 */
    }

    p = ( UNSIGNED_CHAR *)Dst;
    p --;
    * p = *( pSrc - 1 ) & LC_VAR_CONTROL_BITS;
    iStatus = XJP_DS_Confirm_Field_Value_SG( TabIndex, Type, StartNumber, & CondField, LS_TIME_OUT,
        LCC_BCall_BackTaskSendMessData, p );
    * pDst = ( UNSIGNED_CHAR )( iStatus );
    if( iStatus != NU_SUCCESS )
    {
        k = *( pSrc - 1 ) & 0x07;  /* 有效通道 */
        sprintf( buffer, "装置整定值数据执行失败：table：%02d field：%02d org：%s result：%d",
            TabIndex, CondField.HandleIndex, LSC_CommChl[k], iStatus );
        CLOG_Log( buffer );
        return( COM_DATA_FULL );
    }
    else
    {
        k = *( pSrc - 1 ) & 0x07; /* 有效通道 */
        sprintf( buffer, "装置整定值数据修改成功：table：%02d field：%02d org：%s",
            TabIndex, CondField.HandleIndex, LSC_CommChl[k] );
        CLOG_Log( buffer );
        return( COM_DATA_EMPTY );
    }
}
#endif

#if SWITCH_IEC103_TI_M_AB
/* 读取数据表的状态 */
INT LSC_Get_Tab_Status( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR    * pSrc,* pDst;
    INT                TableIndex;
    INT                iStatus;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 7;                                  /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_AB;                     /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                      /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                      /* 序列号高字节 */
    * pDst ++ = *( pSrc + 5 );                      /* 表的索引号低字节 */
    * pDst ++ = *( pSrc + 6 );                      /* 表的索引号高字节 */

    /* 表的索引号 */
    TableIndex = (*( pSrc + 5 ) & LS_DATA_MASKS8 );  /* 表的索引号低字节 */
    TableIndex += ((*( pSrc + 6 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* 表的索引号高字节 */

    iStatus = XJP_DS_Get_Table_Status( TableIndex, pDst ); /* 获取数据表的状态 */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_AC
/* 读取数据表的配置CRC码 */
INT LSC_Get_Config_Tab_CRC( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR    * pSrc,* pDst;
    UNSIGNED           CRC_code;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 12;                                  /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_AC;                     /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                      /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                      /* 序列号高字节 */

    CRC_code = XJP_DS_Get_Config_CRC( );
    * pDst ++ = ( UNSIGNED_CHAR )( CRC_code  & LS_DATA_MASKS8 );         /* 配置数据的CRC码字节1 */
    * pDst ++ = ( UNSIGNED_CHAR )(( CRC_code >> 8 ) & LS_DATA_MASKS8 );  /* 配置数据的CRC码字节2 */
    * pDst ++ = ( UNSIGNED_CHAR )(( CRC_code >> 16 ) & LS_DATA_MASKS8 ); /* 配置数据的CRC码字节3 */
    * pDst ++ = ( UNSIGNED_CHAR )(( CRC_code >> 24 ) & LS_DATA_MASKS8 ); /* 配置数据的CRC码字节4 */
    CRC_code = XJP_DS_Get_Tab_Field_CRC( );
    * pDst ++ = ( UNSIGNED_CHAR )( CRC_code  & LS_DATA_MASKS8 );         /* 字段信息的CRC码字节1 */
    * pDst ++ = ( UNSIGNED_CHAR )(( CRC_code >> 8 ) & LS_DATA_MASKS8 );  /* 字段信息的CRC码字节2 */
    * pDst ++ = ( UNSIGNED_CHAR )(( CRC_code >> 16 ) & LS_DATA_MASKS8 ); /* 字段信息的CRC码字节3 */
    * pDst = ( UNSIGNED_CHAR )(( CRC_code >> 24 ) & LS_DATA_MASKS8 );    /* 字段信息的CRC码字节4 */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_AD
INT LSC_FreezeTable;
VOID LSC_FreezeTabOvertimeCall( void * argPtr );
/* 冻结数据表的数据 */
INT  LSC_Freeze_Table( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR    * pSrc,* pDst;
    INT                iStatus;
    INT                TableIndex;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 7;                                  /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_AD;                     /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                      /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                      /* 序列号高字节 */
    * pDst ++ = *( pSrc + 5 );                      /* Table index -1 */
    * pDst ++ = *( pSrc + 6 );                      /* Table index -2 */

    /* 表的索引号 */
    TableIndex = (*( pSrc + 5 ) & LS_DATA_MASKS8 );  /* 低字节 */
    TableIndex += ((*( pSrc + 6 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* 高字节 */

    if( XJP_DS_Freeze_Status( TableIndex ) == NU_FALSE )
    {
        iStatus = XJP_DS_Freeze_Table( TableIndex );
        if( iStatus == XJ_SUCCESS )
        {
            /* 添加超时检查数据函数 */
            LSC_FreezeTable = TableIndex;
            /* 超时时间设置 */
            XJP_OverTime_Place_On( & LSC_FreezeTable, 120000, & LSC_FreezeTable,
                LSC_FreezeTabOvertimeCall );
        }
    }
    else
    {
        iStatus = XJP_DS_Freeze_Table( TableIndex );
    }

    * pDst = ( UNSIGNED_CHAR )( iStatus & LS_DATA_MASKS8 ); /* 执行结果 */

    return( COM_DATA_FULL );
}

/* 冻结数据表超时回调函数 */
VOID LSC_FreezeTabOvertimeCall( void * argPtr )
{
    INT TableIndex;

    if( argPtr != XJ_NULL )
    {
        TableIndex = *( INT *)argPtr;
        XJP_DS_Freeze_Initialize( TableIndex );
    }
}
#endif

#if SWITCH_IEC103_TI_M_AE
/* 解冻数据表的数据 */
INT  LSC_UnFreeze_Table( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR    * pSrc,* pDst;
    INT                iStatus;
    INT                TableIndex;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 7;                                  /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_AE;                     /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                      /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                      /* 序列号高字节 */
    * pDst ++ = *( pSrc + 5 );                      /* 表的索引号低字节 */
    * pDst ++ = *( pSrc + 6 );                      /* 表的索引号高字节 */

    /* 表的索引号 */
    TableIndex = (*( pSrc + 5 ) & LS_DATA_MASKS8 );  /* 低字节 */
    TableIndex += ((*( pSrc + 6 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS );/* 高字节 */

    if( XJP_DS_Freeze_Status( TableIndex ) == NU_TRUE )
    {
        iStatus = XJP_DS_UnFreeze_Table( TableIndex );
        if( XJP_DS_Freeze_Status( TableIndex ) == NU_FALSE )
        {
            /* 撤消超时检查数据函数 */
            XJP_OverTime_Remove( & LSC_FreezeTable );
        }
    }
    else
    {
        iStatus = XJP_DS_UnFreeze_Table( TableIndex );
    }

    * pDst = ( UNSIGNED_CHAR )( iStatus & LS_DATA_MASKS8 ); /* 执行结果 */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_AF
/* 设置多个配置字段的值 */
INT LSC_Set_Batch_Config_Values( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR  * pSrc,* pDst;
    INT              i, j, k;
    INT              number, StartNumber, TabIndex;
    INT              iStatus;
    TB_CFB           CondField;
    UNSIGNED         Buffer[ LS_MAX_CONFIG_BYTE ];
    UNSIGNED_CHAR    Buffer1[ LS_MAX_CONFIG_BYTE ];
    INT              ValueWidth;
    VOID           * pTemp;
    TB_FCB          * pField;
    TB_TCB          * pTable;
    INT             iNumber;
    INT             iHandleNumber;  /* 操作字段个数 */
    INT             iSize;
    INT             iOffset;  /* 地址偏移 */
    INT             originate;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    number = (*( pSrc + 5 ) & LS_DATA_MASKS8 );        /* 设置数据的个数 */

    /* 起始数据的个数 */
    StartNumber = (*( pSrc + 6 ) & LS_DATA_MASKS8 );   /* 低字节 */
    StartNumber += ((*( pSrc + 7 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* 高字节 */

    /* 表的索引号 */
    TabIndex = (*( pSrc + 8 ) & LS_DATA_MASKS8 );      /* 低字节 */
    TabIndex += ((*( pSrc + 9 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* 高字节 */

    CondField.CondNumber = (*( pSrc + 10 ) & LS_DATA_MASKS8 );/* 条件个数 */
    iHandleNumber = *( pSrc + 11 + CondField.CondNumber * TB_CFB_COND_BYTE_NUMBER );/* 操作字段个数 */

    * pDst ++ = 11 + ( 7 * CondField.CondNumber ) + ( ( 2 + 1 ) * iHandleNumber ); /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_AF;                     /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                      /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                      /* 序列号高字节 */
    * pDst ++ = *( pSrc + 5 );                      /* 设置数据的个数 */
    * pDst ++ = *( pSrc + 6 );                      /* 起始数据的个数低字节 */
    * pDst ++ = *( pSrc + 7 );                      /* 起始数据的个数高字节 */
    * pDst ++ = *( pSrc + 8 );                      /* 表的索引号低字节 */
    * pDst ++ = *( pSrc + 9 );                      /* 表的索引号高字节 */
    * pDst ++ = *( pSrc + 10 );                     /* 条件个数 */

    for( i = 0; i < CondField.CondNumber; i ++ )
    {
        /* 条件的字段索引号 */
        CondField.CondIndex[ i ] = (*( pSrc + 11 + i * TB_CFB_COND_BYTE_NUMBER ) & LS_DATA_MASKS8 );/* 低字节 */
        CondField.CondIndex[ i ] += ((*( pSrc + 12 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* 高字节 */

        CondField.CondValue[ i ] = 0;
        for( k = 0; k < LS_DATA_WIDTH; k ++)
        {
            /* 条件的约束字段值 */
            CondField.CondValue[ i ] += ((*( pSrc + 13 + i * TB_CFB_COND_BYTE_NUMBER + k ) & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
        }
        CondField.CondType[ i ] = (*( pSrc + 17 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );/* 条件的类型 */

        * pDst ++ = *( pSrc + 11 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件字段索引号低字节 */
        * pDst ++ = *( pSrc + 12 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件字段索引号高字节 */
        * pDst ++ = *( pSrc + 13 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件约束字段值字节1 */
        * pDst ++ = *( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件约束字段值字节2 */
        * pDst ++ = *( pSrc + 15 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件约束字段值字节3 */
        * pDst ++ = *( pSrc + 16 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件约束字段值字节4 */
        * pDst ++ = *( pSrc + 17 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件类型 */
    }

    * pDst ++ = iHandleNumber;    /* 操作字段个数 */

    iSize = 0;                    /* 操作字段索引号的偏移 */
    for( i = 0; i < iHandleNumber; i ++)
    {
        iOffset = CondField.CondNumber * TB_CFB_COND_BYTE_NUMBER + i * 2 + iSize; /* 获取条件值偏移位置 */

        * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 12 + iOffset ) & LS_DATA_MASKS8 );/* 低字节 */
        * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 13 + iOffset ) & LS_DATA_MASKS8 );/* 高字节 */
        CondField.HandleIndex = (*( pSrc + 12 + iOffset ) & LS_DATA_MASKS8 );/* 低字节 */
        /* 高字节 */
        CondField.HandleIndex += ((*( pSrc + 13 + iOffset ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );

        /* 通过数据表索引号及操作字段索引号,找到相应的字段信息 */
        pTable = CTBC_FindTable( TabIndex );
        pField = CTBC_FindField( pTable, CondField.HandleIndex );
        /* 一个数据的数据个数 */
        iNumber = pField->Size / pField->TypeSize;
        iSize +=  number * ( iNumber * pField->TypeByte ); /* 调整数据长度 */
        for( k = 0; k < ( number * ( iNumber * pField->TypeByte ) ); k ++ )
        {
            /* 接收数据 */
            Buffer1 [k] = (*( pSrc + 14 + iOffset + k ) & LS_DATA_MASKS8 );
        }
        ValueWidth = pField->TypeByte;    /* 字段类型字节数 */
        /* 数据宽度仅有两种1或4,当为1时缓冲区设为UNSIGNED_CHAR,4:UNSIGNED */
        if( ValueWidth == 1 )
        {
            pTemp = ( VOID *)Buffer1;
        }
        else
        {
            for( j = 0; j < ( number * iNumber ); j ++ )
            {
                Buffer[j] = 0;
                for( k = 0; k < ValueWidth; k ++)
                {
                    /* 根据数据宽度合成指定格式的数据 */
                    Buffer[j] += (( Buffer1[j * ValueWidth + k] & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
                }
            }
            pTemp = ( VOID *)Buffer;
        }

        originate = ( INT )(*( pSrc - 1 ) & LC_VAR_CONTROL_BITS );
        iStatus = XJP_DS_Set_Field_Value_Config( TabIndex, StartNumber, number, & CondField, pTemp, originate );
        * pDst ++ = ( UNSIGNED_CHAR )( iStatus );
        if( iStatus != NU_SUCCESS )
        {
            k = *( pSrc - 1 ) & 0x07;  /* 有效通道 */
            sprintf( ( CHAR *)Buffer1, "装置配置数据设置失败：table：%02d field：%02d org：%s result：%d",
                TabIndex, CondField.HandleIndex, LSC_CommChl[k], iStatus );
            CLOG_Log( ( CHAR *)Buffer1 );
        }
    }

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_B0
/* 保存多个配置字段的值 */
INT LSC_Save_Batch_Config_Values( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR  * pSrc,* pDst;
    INT              i, j, k;
    INT              number, StartNumber, TabIndex;
    INT              iStatus;
    TB_CFB           CondField;
    UNSIGNED         Buffer[ LS_MAX_CONFIG_BYTE ];
    UNSIGNED_CHAR    Buffer1[ LS_MAX_CONFIG_BYTE ];
    INT              ValueWidth;
    VOID           * pTemp;
    TB_FCB          * pField;
    TB_TCB          * pTable;
    INT             iNumber;
    INT             iHandleNumber;  /* 操作字段个数 */
    INT             iSize;
    INT             iOffset;  /* 地址偏移 */
    INT             originate;
    INT             SaveFlag;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    number = (*( pSrc + 5 ) & LS_DATA_MASKS8 );        /* 设置数据的个数 */

    /* 起始数据的个数 */
    StartNumber = (*( pSrc + 6 ) & LS_DATA_MASKS8 );   /* 低字节 */
    StartNumber += ((*( pSrc + 7 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* 高字节 */

    /* 表的索引号 */
    TabIndex = (*( pSrc + 8 ) & LS_DATA_MASKS8 );      /* 低字节 */
    TabIndex += ((*( pSrc + 9 ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* 高字节 */

    CondField.CondNumber = (*( pSrc + 10 ) & LS_DATA_MASKS8 );/* 条件个数 */
    iHandleNumber = *( pSrc + 11 + CondField.CondNumber * TB_CFB_COND_BYTE_NUMBER );/* 操作字段个数 */

    * pDst ++ = 12 + ( 7 * CondField.CondNumber ) + ( ( 2 + 1 ) * iHandleNumber ); /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_AF;                     /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER ); /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                      /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                      /* 序列号高字节 */
    * pDst ++ = *( pSrc + 5 );                      /* 设置数据的个数 */
    * pDst ++ = *( pSrc + 6 );                      /* 起始数据的个数低字节 */
    * pDst ++ = *( pSrc + 7 );                      /* 起始数据的个数高字节 */
    * pDst ++ = *( pSrc + 8 );                      /* 表的索引号低字节 */
    * pDst ++ = *( pSrc + 9 );                      /* 表的索引号高字节 */
    * pDst ++ = *( pSrc + 10 );                     /* 条件个数 */

    for( i = 0; i < CondField.CondNumber; i ++ )
    {
        /* 条件的字段索引号 */
        CondField.CondIndex[ i ] = (*( pSrc + 11 + i * TB_CFB_COND_BYTE_NUMBER ) & LS_DATA_MASKS8 );/* 低字节 */
        CondField.CondIndex[ i ] += ((*( pSrc + 12 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );/* 高字节 */

        CondField.CondValue[ i ] = 0;
        for( k = 0; k < LS_DATA_WIDTH; k ++)
        {
            /* 条件的约束字段值 */
            CondField.CondValue[ i ] += ((*( pSrc + 13 + i * TB_CFB_COND_BYTE_NUMBER + k ) & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
        }
        CondField.CondType[ i ] = (*( pSrc + 17 + i * TB_CFB_COND_BYTE_NUMBER  ) & LS_DATA_MASKS8 );/* 条件的类型 */

        * pDst ++ = *( pSrc + 11 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件字段索引号低字节 */
        * pDst ++ = *( pSrc + 12 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件字段索引号高字节 */
        * pDst ++ = *( pSrc + 13 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件约束字段值字节1 */
        * pDst ++ = *( pSrc + 14 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件约束字段值字节2 */
        * pDst ++ = *( pSrc + 15 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件约束字段值字节3 */
        * pDst ++ = *( pSrc + 16 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件约束字段值字节4 */
        * pDst ++ = *( pSrc + 17 + i * TB_CFB_COND_BYTE_NUMBER );     /* 条件类型 */
    }

    * pDst ++ = iHandleNumber;    /* 操作字段个数 */

    SaveFlag = NU_TRUE;
    iSize = 0;                    /* 操作字段索引号的偏移 */
    for( i = 0; i < iHandleNumber; i ++)
    {
        iOffset = CondField.CondNumber * TB_CFB_COND_BYTE_NUMBER + i * 2 + iSize; /* 获取条件值偏移位置 */

        * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 12 + iOffset ) & LS_DATA_MASKS8 );/* 低字节 */
        * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 13 + iOffset ) & LS_DATA_MASKS8 );/* 高字节 */
        CondField.HandleIndex = (*( pSrc + 12 + iOffset ) & LS_DATA_MASKS8 );/* 低字节 */
        /* 高字节 */
        CondField.HandleIndex += ((*( pSrc + 13 + iOffset ) & LS_DATA_MASKS8 )<< LS_BYTE_BITS );

        /* 通过数据表索引号及操作字段索引号,找到相应的字段信息 */
        pTable = CTBC_FindTable( TabIndex );
        pField = CTBC_FindField( pTable, CondField.HandleIndex );
        /* 一个数据的数据个数 */
        iNumber = pField->Size / pField->TypeSize;
        iSize +=  number * ( iNumber * pField->TypeByte ); /* 调整数据长度 */
        for( k = 0; k < ( number * ( iNumber * pField->TypeByte ) ); k ++ )
        {
            /* 接收数据 */
            Buffer1 [k] = (*( pSrc + 14 + iOffset + k ) & LS_DATA_MASKS8 );
        }
        ValueWidth = pField->TypeByte;    /* 字段类型字节数 */
        /* 数据宽度仅有两种1或4,当为1时缓冲区设为UNSIGNED_CHAR,4:UNSIGNED */
        if( ValueWidth == 1 )
        {
            pTemp = ( VOID *)Buffer1;
        }
        else
        {
            for( j = 0; j < ( number * iNumber ); j ++ )
            {
                Buffer[j] = 0;
                for( k = 0; k < ValueWidth; k ++)
                {
                    /* 根据数据宽度合成指定格式的数据 */
                    Buffer[j] += (( Buffer1[j * ValueWidth + k] & LS_DATA_MASKS8 ) << ( k * LS_BYTE_BITS ));
                }
            }
            pTemp = ( VOID *)Buffer;
        }

        originate = ( INT )(*( pSrc - 1 ) & LC_VAR_CONTROL_BITS );
        iStatus = XJP_DS_Set_Field_Value_Config( TabIndex, StartNumber, number, & CondField, pTemp, originate );
        * pDst ++ = ( UNSIGNED_CHAR )( iStatus );
        if( iStatus != NU_SUCCESS )
        {
            SaveFlag = NU_FALSE;
        }
    }
    if( SaveFlag == NU_TRUE )
    {
        iStatus = XJP_DS_Save_Tab_Field_Value_Config( TabIndex, LS_TIME_OUT );
        * pDst ++ = ( UNSIGNED_CHAR )( iStatus );
        if( iStatus == NU_SUCCESS )
        {
            k = *( pSrc - 1 ) & 0x07;  /* 有效通道 */
            sprintf( ( CHAR *)Buffer1, "装置配置数据修改成功：table：%02d org：%s",
                TabIndex, LSC_CommChl[k] );
            CLOG_Log( ( CHAR *)Buffer1 );
        }
        else
        {
            k = *( pSrc - 1 ) & 0x07;  /* 有效通道 */
            sprintf( ( CHAR *)Buffer1, "装置配置数据修改失败：table：%02d org：%s result：%d",
                TabIndex, LSC_CommChl[k], iStatus );
            CLOG_Log( ( CHAR *)Buffer1 );
        }
    }
    else
    {
        k = *( pSrc - 1 ) & 0x07;  /* 有效通道 */
        sprintf( ( CHAR *)Buffer1, "装置配置数据修改失败：table：%02d org：%s result：%d",
            TabIndex, LSC_CommChl[k], iStatus );
        CLOG_Log( ( CHAR *)Buffer1 );
    }

    return( COM_DATA_FULL );
}
#endif

/* 召唤巡检事件 */
INT LSC_Report_Event( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT      iStatus;
    //--------------------------------------------------------------------------
    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );
    LS_UNUSED_PARAM( Src );
    /* B6命令报文管理函数 */
    iStatus = LSC_B6_MessageManagFun( Dst );
    /* return */
    return( iStatus );
}

#if SWITCH_IEC103_TI_M_C0
/* 召唤事件类型 */
INT LSC_Load_Event_Type( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        uSetAvailable;
    UNSIGNED        i;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    uSetAvailable = (*( pSrc + 5 ) & LS_DATA_MASKS8 );   /* 召唤类型 */

    if( uSetAvailable != LS_DISABLE_DISPLAY )
    {
        * pDst ++ = ( UNSIGNED_CHAR )( LS_USER_RECORD_EVENT_GROUP + 7 ); /* 链路用户数据长度 */
    }
    else
    {
        * pDst ++ = ( UNSIGNED_CHAR )( LS_USER_RECORD_EVENT_GROUP + 8 ); /* 链路用户数据长度 */
    }
    * pDst ++ = IEC103_TI_M_C0;                                          /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                      /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                                           /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                                           /* 序列号高字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 );      /* 召唤类型 */

    if( uSetAvailable != LS_DISABLE_DISPLAY )
    {
        * pDst ++ = ( UNSIGNED_CHAR )( LS_USER_RECORD_EVENT_GROUP + 1 ); /* 事件类型个数 */
    }
    else
    {
        * pDst ++ = ( UNSIGNED_CHAR )( LS_USER_RECORD_EVENT_GROUP + 2 ); /* 事件类型个数 */
    }

    * pDst ++ = ALARM_EVENT_ID;                                          /* 告警事件类型 */

    if( uSetAvailable == LS_DISABLE_DISPLAY )
    {
        * pDst ++ = RESERVE_EVENT_ID;                                    /* 内部事件类型 */
    }

    for( i = 0; i < ( UNSIGNED )LS_USER_RECORD_EVENT_GROUP; i ++)
    {
        * pDst ++ = ( UNSIGNED_CHAR )( i + 1 );                          /* 用户事件类型 */
    }

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_C1
/* 召唤事件信息 */
INT LSC_Load_Event_Inf( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             StatusTemp;
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        uEvent_type;
    CEVENT          * pEvent;
    UNSIGNED        Type, Number;
    UNSIGNED        uStrtSerl, uEndSerl;
    INT             Status = COM_DATA_ERROR;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    uEvent_type = (*( pSrc + 5 ) & LS_DATA_MASKS8 );     /* 事件类型 */

    * pDst ++ = 11;                                      /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_C1;                          /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );      /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                           /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                           /* 序列号高字节 */
    * pDst ++ = ( UNSIGNED_CHAR )uEvent_type;            /* 事件类型 */

    pEvent = NU_NULL;
    if( uEvent_type == ALARM_EVENT_ID )
    {
        /* 告警事件 */
        pEvent = & CAlarmEvent;
    }
    else if( uEvent_type == RESERVE_EVENT_ID )
    {
        /* 内部事件 */
        pEvent = & CReserveEvent;
    }
    else if( uEvent_type == OPERATE_EVENT_ID )
    {
        /* 操作事件 */
        pEvent = & COperateEvent;
    }
    else
    {
        /* 用户事件 */
        if( uEvent_type <= ( UNSIGNED )LS_USER_RECORD_EVENT_GROUP )
        {
            pEvent = PRO_EventRecord;
            pEvent += ( uEvent_type - 1 );
        }
    }

    if( pEvent != NU_NULL )
    {
        StatusTemp = pEvent->Information( pEvent,& Type,& Number,& uStrtSerl,
            & uEndSerl, LS_EVENT_TIMEOUT );
        if( StatusTemp == NU_SUCCESS )
        {
            /* 总个数 */
            * pDst ++ = ( UNSIGNED_CHAR )( Number & LS_DATA_MASKS8 ); /* 低字节 */
            * pDst ++ = ( UNSIGNED_CHAR )(( Number >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* 高字节 */
            /* 起始事件序号 */
            * pDst ++ = ( UNSIGNED_CHAR )( uStrtSerl & LS_DATA_MASKS8 ); /* 低字节 */
            * pDst ++ = ( UNSIGNED_CHAR )(( uStrtSerl >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* 高字节 */
            /* 结束事件序号 */
            * pDst ++ = ( UNSIGNED_CHAR )( uEndSerl & LS_DATA_MASKS8 ); /* 低字节 */
            * pDst ++ = ( UNSIGNED_CHAR )(( uEndSerl >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* 高字节 */
            Status = COM_DATA_FULL;
        }
        else if( StatusTemp == NU_TIMEOUT )
        {
            Status = COM_DATA_TIMEOUT;
        }
        else
        {
            Status = COM_DATA_ERROR;
        }
    }
    return( Status );
}
#endif

#if SWITCH_IEC103_TI_M_C2
/* 召唤事件索引号 */
INT LSC_Load_Event_Serial( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             Status, StatusTemp, i;
    INT             Number;
    UNSIGNED        uTotal_Num, uEnd_Seril;
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        uEvent_type;
    CEVENT          * pEvent;
    UNSIGNED        buffer[CPC_MAX_NVRAM_EVENT_NUMBER];
    INT             iNumber;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    uEvent_type = (*( pSrc + 5 ) & LS_DATA_MASKS8 ); /* 事件类型 */

    /* 事件总个数 */
    uTotal_Num = *( pSrc + 6 ) & LS_DATA_MASKS8; /* 低字节 */
    uTotal_Num += ((*( pSrc + 7 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* 高字节 */

    /* 结束事件序号 */
    uEnd_Seril = *( pSrc + 8 ) & LS_DATA_MASKS8; /* 低字节 */
    uEnd_Seril += ((*( pSrc + 9 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* 高字节 */

    pEvent = NU_NULL;
    if( uEvent_type == ALARM_EVENT_ID )
    {
        /* 告警事件 */
        pEvent = & CAlarmEvent;
    }
    else if( uEvent_type == RESERVE_EVENT_ID )
    {
        /* 内部事件 */
        pEvent = & CReserveEvent;
    }
    else if( uEvent_type == OPERATE_EVENT_ID )
    {
        /* 操作事件 */
        pEvent = & COperateEvent;
    }
    else
    {
        /* 用户事件 */
        if( uEvent_type <= ( UNSIGNED )LS_USER_RECORD_EVENT_GROUP )
        {
            pEvent = PRO_EventRecord;
            pEvent += ( uEvent_type - 1 );
        }
    }
    /* 填写具体事件索引号的偏移量 */
    pDst += 8;
    if( pEvent != NU_NULL )
    {
        StatusTemp = pEvent->Find( pEvent, buffer,& Number, LS_EVENT_TIMEOUT );
        if( StatusTemp == NU_SUCCESS )
        {
            iNumber = 0;
            for( i = 0; i < Number; i ++)
            {
                if( buffer[i] == uEnd_Seril )
                {
                    /* 满足条件的实际事件个数 */
                    iNumber = ( i + 1 );
                    break;
                }
            }
            /* 判断个数是否在允许范围之内 */
            if( uTotal_Num > LC_MAX_EVENT_SERIAL )
            {
                uTotal_Num = LC_MAX_EVENT_SERIAL;
            }
            /* 如果召唤个数大于实际个数,按实际个数来上送 */
            if(( INT ) uTotal_Num > iNumber )
            {
                uTotal_Num = ( UNSIGNED ) iNumber;
            }

            /* 填写事件索引号,从end开始 */
            for( i = 0; i < ( INT )uTotal_Num; i ++)
            {
                /* 事件ID */
                * pDst ++ = ( UNSIGNED_CHAR )( buffer[iNumber - 1 - i] & LS_DATA_MASKS8 ); /* 低字节 */
                * pDst ++ = ( UNSIGNED_CHAR )(( buffer[iNumber - 1 - i] >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* 高字节 */
            }
            Status = COM_DATA_FULL;
        }
        else if( StatusTemp == NU_TIMEOUT )
        {
            Status = COM_DATA_TIMEOUT;
        }
        else
        {
            Status = COM_DATA_ERROR;
        }
    }
    else
    {
        Status = COM_DATA_ERROR;
    }

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = ( UNSIGNED_CHAR )( uTotal_Num * 2 + 7 );     /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_C2;                              /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );          /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                               /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                               /* 序列号高字节 */
    * pDst ++ = ( UNSIGNED_CHAR )uEvent_type;                /* 事件类型 */
    /* 实际事件个数 */
    * pDst ++ = ( UNSIGNED_CHAR )( uTotal_Num & LS_DATA_MASKS8 ); /* 低字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(( uTotal_Num >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* 高字节 */

    return( Status );
}
#endif

#if SWITCH_IEC103_TI_M_C3
/* 召唤事件数据 */
INT LSC_Load_Event_Data( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             iIndex;
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        uEvent_type, uLength, uStatus;
    INT             iNumber;
    INT             iActNumber;
    CEVENT          * pEvent;
    INT             i;
    CEM_NVD         EventNVData;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    iNumber = *( pSrc + 5 ) & LS_DATA_MASKS8; /* 事件个数 */
    uLength = sizeof( CEM_NVD );              /* 一个事件报文的长度 */

    pDst += 6; /* 将目的地址指针调整到填写事件数据处 */
    /* 实际事件个数 */
    iActNumber = 0;
    for( i = 0; i < iNumber; i ++)
    {
        uEvent_type = *( pSrc + 6 + ( i * 3 ) ) & LS_DATA_MASKS8;     /* 事件类型 */
        /* 事件序号 */
        iIndex = *( pSrc + 7 + ( i * 3 )) & LS_DATA_MASKS8; /* 低字节 */
        iIndex += ((*( pSrc + 8 + ( i * 3 ) ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* 高字节 */

        pEvent = NU_NULL;
        if( uEvent_type == ALARM_EVENT_ID )
        {
            /* 告警事件 */
            pEvent = & CAlarmEvent;
        }
        else if( uEvent_type == RESERVE_EVENT_ID )
        {
            /* 内部事件 */
            pEvent = & CReserveEvent;
        }
        else if( uEvent_type == OPERATE_EVENT_ID )
        {
            /* 操作事件 */
            pEvent = & COperateEvent;
        }
        else
        {
            /* 用户事件 */
            if( uEvent_type <= ( UNSIGNED )LS_USER_RECORD_EVENT_GROUP )
            {
                pEvent = PRO_EventRecord;
                pEvent += ( uEvent_type - 1 );
            }
        }

        if( pEvent != NU_NULL )
        {
            uStatus = pEvent->Read( pEvent, iIndex, & EventNVData, LS_EVENT_TIMEOUT );
            if( uStatus == NU_SUCCESS )
            {
                /* 事件个数加1 */
                iActNumber ++;
                /* 类型标志 */
                * pDst ++ = ( UNSIGNED_CHAR )EventNVData.Flag;
                /* 序号低字节 */
                * pDst ++ = ( UNSIGNED_CHAR )EventNVData.IndexL;
                /* 序号高字节 */
                * pDst ++ = ( UNSIGNED_CHAR )EventNVData.IndexH;
                /* 名称 */
                strncpy( ( CHAR *)pDst, ( CHAR *)EventNVData.Name, CEM_EVENT_NAME );
                pDst += CEM_EVENT_NAME;
                /* 年 */
                * pDst ++ = ( UNSIGNED_CHAR )EventNVData.Year;
                /* 月 */
                * pDst ++ = ( UNSIGNED_CHAR )EventNVData.Month;
                /* 日 */
                * pDst ++ = ( UNSIGNED_CHAR )EventNVData.Day;
                /* 时 */
                * pDst ++ = ( UNSIGNED_CHAR )EventNVData.Hour;
                /* 分 */
                * pDst ++ = ( UNSIGNED_CHAR )EventNVData.Minute;
                /* 毫秒低字节 */
                * pDst ++ = ( UNSIGNED_CHAR )EventNVData.MillionsecondL;
                /* 毫秒高字节 */
                * pDst ++ = ( UNSIGNED_CHAR )EventNVData.MillionsecondH;
            }
        }
    }

    pDst = ( UNSIGNED_CHAR *)Dst;
    * pDst ++ = ( sizeof( CEM_NVD ) - 1 ) * iActNumber + 5; /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_C3;                             /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );         /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                              /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                              /* 序列号高字节 */
    * pDst ++ = iActNumber;                                 /* 实际事件个数 */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_C4
/* 清除事件数据 */
INT LSC_Clear_Alarm_Event( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             i;
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        uEvent_type;
    INT             iResult = LS_RESULT_NULL;
    CEVENT          * pEvent;
    INT             Status, Status1, StatusTemp;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    Status1 = COM_DATA_EMPTY;

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    uEvent_type = *( pSrc + 5 ) & LS_DATA_MASKS8;  /* 事件类型 */

    if( uEvent_type == LS_DATA_MASKS8 )
    {
        /* 清除全部事件数据 */
        StatusTemp = CAlarmEvent.Clear(& CAlarmEvent, LS_EVENT_TIMEOUT ); /* 清除告警事件 */

        if( StatusTemp == NU_SUCCESS )
        {
            iResult = LS_RESULT_TRUE;
            Status1 |= COM_DATA_FULL;
        }
        else if( StatusTemp == NU_TIMEOUT )
        {
            Status1 |= COM_DATA_TIMEOUT;
        }
        else
        {
            Status1 |= COM_DATA_ERROR;
        }

        pEvent = PRO_EventRecord;
        for( i = 0; i < LS_USER_RECORD_EVENT_GROUP; i ++)
        {
            /* 清除用户事件 */
            StatusTemp = pEvent->Clear( pEvent, LS_EVENT_TIMEOUT );
            if( StatusTemp == NU_SUCCESS )
            {
                Status1 |= COM_DATA_FULL;
            }
            else if( StatusTemp == NU_TIMEOUT )
            {
                iResult = LS_RESULT_NULL;
                Status1 |= COM_DATA_TIMEOUT;
            }
            else
            {
                iResult = LS_RESULT_NULL;
                Status1 |= COM_DATA_ERROR;
            }
            pEvent ++;
        }
    }
    else
    {
        /* 清除指定事件数据 */
        pEvent = NU_NULL;
        if( uEvent_type == ALARM_EVENT_ID )
        {
            /* 告警事件 */
            pEvent = & CAlarmEvent;
        }
        else if( uEvent_type == RESERVE_EVENT_ID )
        {
            /* 内部事件 */
            pEvent = & CReserveEvent;
        }

        else if( uEvent_type == OPERATE_EVENT_ID )
        {
            /* 操作事件 */
            pEvent = & COperateEvent;
        }
        else
        {
            /* 用户事件 */
            if( uEvent_type <= ( UNSIGNED )LS_USER_RECORD_EVENT_GROUP )
            {
                pEvent = PRO_EventRecord;
                pEvent += ( uEvent_type - 1 );
            }
        }

        if( pEvent != NU_NULL )
        {
            StatusTemp = pEvent->Clear( pEvent, LS_EVENT_TIMEOUT );
            if( StatusTemp == NU_SUCCESS )
            {
                iResult = LS_RESULT_TRUE;
                Status1 = COM_DATA_FULL;
            }
            else if( StatusTemp == NU_TIMEOUT )
            {
                Status1 = COM_DATA_TIMEOUT;
            }
            else
            {
                Status1 = COM_DATA_ERROR;
            }
        }
        else
        {
            Status1 = COM_DATA_ERROR;
        }
    }

    if( Status1 == COM_DATA_FULL )
    {
        Status = COM_DATA_FULL;
    }
    else if(( Status1 & COM_DATA_TIMEOUT ) != 0 )
    {
        Status = COM_DATA_TIMEOUT;
    }
    else
    {
        Status = COM_DATA_ERROR;
    }

    * pDst ++ = 6;                                        /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_C4;                           /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );       /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                            /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                            /* 序列号高字节 */
    * pDst ++ = ( UNSIGNED_CHAR )uEvent_type;             /* 事件类型 */
    * pDst ++ = ( UNSIGNED_CHAR )iResult;                 /* 执行结果 */

    return( Status );
}
#endif

#if SWITCH_IEC103_TI_M_C5
/* 召唤报告信息 */
INT LSC_Load_Report_Inf( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        StartFaultNumber, EndFaultNumber, Number, IndexNumber;
    INT             Status, StatusTemp;
    CREPORT         * pReport;
    INT             CpuNumber;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;
    CpuNumber = *( pSrc + 2 );                           /* CPU号码 */
    * pDst ++ = 12;                                      /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_C5;                          /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );      /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                           /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                           /* 序列号高字节 */
    /* 获取报告信息 */
    pReport = CRMC_GetReportHandle( CpuNumber );
    if( pReport != NU_NULL )
    {
        /* 获取报告信息 */
        StatusTemp = pReport->Information( pReport,& Number,& IndexNumber,& StartFaultNumber,
            & EndFaultNumber, LS_REPORT_TIMEOUT );

        if( StatusTemp == NU_SUCCESS )
        {
            /* 故障总个数 */
            * pDst ++ = ( UNSIGNED_CHAR )( Number & LS_DATA_MASKS8 ); /* 低字节 */
            * pDst ++ = ( UNSIGNED_CHAR )(( Number >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* 高字节 */

            /* 起始故障序号 */
            * pDst ++ = ( UNSIGNED_CHAR )( StartFaultNumber & LS_DATA_MASKS8 ); /* 低字节 */
            * pDst ++ = ( UNSIGNED_CHAR )(( StartFaultNumber >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* 高字节 */

            /* 结束故障序号 */
            * pDst ++ = ( UNSIGNED_CHAR )( EndFaultNumber & LS_DATA_MASKS8 ); /* 低字节 */
            * pDst ++ = ( UNSIGNED_CHAR )(( EndFaultNumber >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* 高字节 */

            /* 结束故障索引个数 */
            * pDst ++ = ( UNSIGNED_CHAR )( IndexNumber & LS_DATA_MASKS8 ); /* 低字节 */
            * pDst ++ = ( UNSIGNED_CHAR )(( IndexNumber >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* 高字节 */

            Status = COM_DATA_FULL;
        }
        else if( StatusTemp == NU_TIMEOUT )
        {
            Status = COM_DATA_TIMEOUT;
        }
        else
        {
            Status = COM_DATA_ERROR;
        }
    }
    else
    {
        Status = COM_DATA_ERROR;
    }

    return( Status );
}
#endif

#if SWITCH_IEC103_TI_M_C6
/* 召唤报告故障序号 */
INT LSC_Load_Report_FaultSerl( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    INT             Status, StatusTemp;
    UNSIGNED        Buffer[CPC_MAX_NVRAM_REPORT_NUMBER];
    INT             Type;
    INT             i;
    UNSIGNED        uTotal_Num;
    UNSIGNED        uEnd_FaultNum;
    UNSIGNED        uNumber;
    INT             number;
    CREPORT         * pReport;
    INT             CpuNumber;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;
    CpuNumber = *( pSrc + 2 );                           /* CPU号码 */
    /* 总个数 */
    uTotal_Num = *( pSrc + 5 ) & LS_DATA_MASKS8; /* 低字节 */
    uTotal_Num += ((*( pSrc + 6 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* 高字节 */

    /* 结束故障序号 */
    uEnd_FaultNum = *( pSrc + 7 ) & LS_DATA_MASKS8; /* 低字节 */
    uEnd_FaultNum += ((*( pSrc + 8 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* 高字节 */

    number = 0;
    Type = REPORT_SOE_FLAG + REPORT_STATE_FLAG + REPORT_SCL_FLAG + REPORT_LOCAL_FLAG + REPORT_START_FLAG;

    /* 获取报告信息 */
    pReport = CRMC_GetReportHandle( CpuNumber );
    if( pReport != NU_NULL )
    {
        /* 获取报告信息 */
        StatusTemp = pReport->FindFaultNumber( pReport, Buffer, & number, Type, LS_REPORT_TIMEOUT );

        pDst += 7; /* 调整填写故障报告序号的目的地址 */
        if( StatusTemp == NU_SUCCESS )
        {
            uNumber = 0;
            for( i = 0; i < number; i ++)
            {
                if( Buffer[i] == uEnd_FaultNum )
                {
                    /* 满足条件的实际故障报告序号个数 */
                    uNumber = ( i + 1 );
                    break;
                }
            }

            /* 如果召唤个数大于实际个数取实际个数 */
            if( uTotal_Num > uNumber )
            {
                uTotal_Num = uNumber;
            }
            /* 判断上调的报告个数是否在允许的范围之内(报文长度决定) */
            if( uTotal_Num > LC_MAX_REPORT_SERIAL )
            {
                uTotal_Num = LC_MAX_REPORT_SERIAL;
            }
            for( i = 0; i <( INT ) uTotal_Num; i ++)
            {
                /* 故障序号 */
                * pDst ++ = ( UNSIGNED_CHAR )( Buffer[uNumber - 1 - i] & LS_DATA_MASKS8 ); /* 低字节 */
                * pDst ++ = ( UNSIGNED_CHAR )(( Buffer[uNumber - 1 - i] >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* 高字节 */
            }
            Status = COM_DATA_FULL;
        }
        else if( StatusTemp == NU_TIMEOUT )
        {
            Status = COM_DATA_TIMEOUT;
        }
        else
        {
            Status = COM_DATA_ERROR;
        }
    }
    else
    {
        uTotal_Num = 0;
        Status = COM_DATA_ERROR;
    }

    pDst = ( UNSIGNED_CHAR *)Dst;
    * pDst ++ = ( UNSIGNED_CHAR )( uTotal_Num * 2 + 6 ); /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_C6;                          /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );      /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                           /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                           /* 序列号高字节 */
    /* 实际个数 */
    * pDst ++ = ( UNSIGNED_CHAR )( uTotal_Num & LS_DATA_MASKS8 ); /* 低字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(( uTotal_Num >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* 高字节 */

    return( Status );
}
#endif

#if SWITCH_IEC103_TI_M_C7
/* 召唤报告索引序号 */
INT LSC_Load_Report_Serial( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             Status, StatusTemp;
    INT             Type;
    UNSIGNED        uTemp;
    INT             number;
    UNSIGNED_CHAR   * pSrc,* pDst;
    CREPORT         * pReport;
    INT             CpuNumber;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    CpuNumber = *( pSrc + 2 );                                     /* CPU号码 */
    /* 故障序号 */
    uTemp = *( pSrc + 5 ) & LS_DATA_MASKS8;                        /* 低字节 */
    uTemp += ((*( pSrc + 6 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* 高字节 */

    number = 0;
    Type = REPORT_SOE_FLAG + REPORT_STATE_FLAG + REPORT_SCL_FLAG + REPORT_LOCAL_FLAG + REPORT_START_FLAG;
    pReport = CRMC_GetReportHandle( CpuNumber );
    if( pReport != NU_NULL )
    {
        StatusTemp = pReport->FindIndexNumber( pReport, & number, uTemp, Type, LS_REPORT_TIMEOUT );
        if( StatusTemp == NU_SUCCESS )
        {
            * pDst ++ = 8;                                                  /* 报文长度 */
            * pDst ++ = IEC103_TI_M_C7;                                     /* 类别标识 */
            * pDst ++ = ( UNSIGNED_CHAR )CpuNumber;                         /* CPU号码 */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 )& LS_DATA_MASKS8 );  /* 序列号低字节 */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 )& LS_DATA_MASKS8 );  /* 序列号高字节 */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 ); /* 故障序号低字节 */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 ); /* 故障序号高字节 */
            /* 判断上调的报告个数是否在允许的范围之内(报文长度决定) */
            if( number > LC_MAX_REPORT_SERIAL )
            {
                number = LC_MAX_REPORT_SERIAL;
            }
            /* 实际个数 */
            * pDst ++ = ( UNSIGNED_CHAR )( number & LS_DATA_MASKS8 ); /* 低字节 */
            * pDst ++ = ( UNSIGNED_CHAR )(( number >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* 高字节 */

            Status = COM_DATA_FULL;
        }
        else if( StatusTemp == NU_TIMEOUT )
        {
            Status = COM_DATA_TIMEOUT;
        }
        else
        {
            * pDst ++ = 8;                                                   /* 报文长度 */
            * pDst ++ = IEC103_TI_M_C7;                                     /* 类别标识 */
            * pDst ++ = ( UNSIGNED_CHAR )CpuNumber;                         /* CPU号码 */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 )& LS_DATA_MASKS8 );  /* 序列号低字节 */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 )& LS_DATA_MASKS8 );  /* 序列号高字节 */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 ); /* 故障序号低字节 */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 ); /* 故障序号高字节 */
            * pDst ++ = 0;                                                  /* 实际个数低字节 */
            * pDst ++ = 0;                                                  /* 实际个数高字节 */

            Status = COM_DATA_FULL;
        }
    }
    else
    {
        Status = COM_DATA_ERROR;
    }

    return( Status );
}
#endif

#if SWITCH_IEC103_TI_M_C8
/* 召唤报告数据 */
INT LSC_Load_Report_Data( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             iFaultNumber;
    INT             iStartIndex;
    INT             iIndexNumber;
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        uOffset, uSize;
    INT             StatusTemp;
    INT             Status;
    INT             iActLength;
    INT             i;
    CREPORT         * pReport;
    INT             CpuNumber;
    UNSIGNED        * p;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    CpuNumber = *( pSrc + 2 );                                      /* CPU号码 */
    /* 故障序号 */
    iFaultNumber = *( pSrc + 5 ) & LS_DATA_MASKS8; /* 低字节 */
    iFaultNumber += ((*( pSrc + 6 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* 高字节1 */
    /* 起始索引号 */
    iStartIndex = *( pSrc + 7 ) & LS_DATA_MASKS8; /* 低字节 */
    iStartIndex += ((*( pSrc + 8 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* 高字节 */
    /* 索引个数 */
    iIndexNumber = *( pSrc + 9 ) & LS_DATA_MASKS8; /* 低字节 */
    iIndexNumber += ((*( pSrc + 10 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* 高字节 */
    /* 起始报告数据偏移 */
    uOffset = *( pSrc + 11 ) & LS_DATA_MASKS8; /* 低字节 */
    uOffset += ((*( pSrc + 12 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* 高字节 */
    /* 读取数据大小 */
    uSize = *( pSrc + 13 ) & LS_DATA_MASKS8; /* 低字节 */
    uSize += ((*( pSrc + 14 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* 高字节 */

    pReport = CRMC_GetReportHandle( CpuNumber );
    if( pReport == NU_NULL )
    {
        return( COM_DATA_ERROR );
    }

    /* 填写报文信息 */
    pDst = ( UNSIGNED_CHAR *)Dst;
    * pDst ++ = ( UNSIGNED_CHAR )( iIndexNumber * uSize + 14 ); /* 报文长度 */
    * pDst ++ = IEC103_TI_M_C8;                           /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )CpuNumber;               /* CPU号码 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ));         /* 序列号低字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ));         /* 序列号高字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ));         /* 故障序号低字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ));         /* 故障序号高字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 7 ));         /* 起始索引号低字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 8 ));         /* 起始索引号高字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 9 ));         /* 索引个数低字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 10 ));        /* 索引个数高字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 11 ));        /* 起始数据偏移低字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 12 ));        /* 起始数据偏移高字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 13 ));        /* 读取数据大小低字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 14 ));        /* 读取数据大小高字节 */

    /* 申请内存空间 */
    p = ( UNSIGNED *)malloc(( LC_COMMAND_FRAME_SIZE + LS_DATA_WIDTH ) * 4 );
    if( p == NU_NULL )
    {
        return( COM_DATA_ERROR );
    }
    /* 读取报告数据 */
    StatusTemp = pReport->Read( pReport, iFaultNumber, iStartIndex, iIndexNumber,
        uOffset, uSize, p, LS_REPORT_TIMEOUT );
    if( StatusTemp ==  NU_SUCCESS )
    {
        /* 报告数据格式转换 */
        iActLength = ( iIndexNumber * uSize + sizeof( UNSIGNED )- 1 )/ sizeof( UNSIGNED );
        for( i = 0; i < iActLength; i ++)
        {
            LS_WORD_TO_BYTE( pDst, ( p + i ), LS_DATA_WIDTH ); /* 从数据处开始读取 */
            pDst += LS_DATA_WIDTH;
        }
        Status = COM_DATA_FULL;
    }
    else if( StatusTemp == NU_TIMEOUT )
    {
        Status = COM_DATA_TIMEOUT;
    }
    else
    {
        Status = COM_DATA_ERROR;
    }

    free( p );

    return( Status );
}
#endif

#if SWITCH_IEC103_TI_M_C9
/* 清除故障记录数据 */
INT LSC_Clear_FaultData( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    INT             iType;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;
    iType = *( pSrc + 5 ) & LS_DATA_MASKS8; /* 清除类型 */

    if( iType & LS_TYPE_CLEAR_REPORT )
    {
        LS_SET_CLR_REPORT();     /* 设置清除报告状态 */
    }

    if( iType & LS_TYPE_CLEAR_WAVE )
    {
        LS_SET_CLR_WAVE( );       /* 设置清除录波状态 */
    }

    * pDst ++ = 6;                                        /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_C9;                           /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );       /* CPU号码 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ));         /* 序列号低字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ));         /* 序列号高字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 );/* 清除类型 */
    * pDst ++ = LS_RESULT_TRUE;                           /* 执行结果 */
    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_CA
/* 手动启动录波 */
INT LSC_Start_Wave( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED        uLength, uResult = LS_RESULT_NULL, uStartPoint;
    UNSIGNED_CHAR   * pSrc,* pDst;
    INT             iSpace;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    iSpace = *( pSrc + 5 ) & LS_DATA_MASKS8;       /* 录波间隔点数 */
    uLength = *( pSrc + 6 ) & LS_DATA_MASKS8; /* 录波数据长度低字节 */
    uLength += ((*( pSrc + 7 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* 录波数据长度高字节 */
    if( uLength >= LS_GET_CHANNEL_POINT )
    {
        uStartPoint = LS_GET_CHANNEL_POINT - 1;
    }
    else
    {
        uStartPoint = uLength;
    }

    if(( CSMV_GET_PERIOD_POINT % iSpace ) == 0 )
    {
        /* 设置启动录波信息 */
        APFC_SetTestWaveParam( iSpace, uStartPoint, uLength );
        uResult = LS_RESULT_TRUE;
    }

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;
    * pDst ++ = 8;                                                  /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_CA;                                     /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                 /* CPU号码 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 ); /* 序列号低字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 ); /* 序列号高字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 ); /* 录波间隔点数 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 ); /* 录波数据长度低字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 7 ) & LS_DATA_MASKS8 ); /* 录波数据长度高字节 */
    * pDst ++ = ( UNSIGNED_CHAR )uResult;                           /* 执行结果 */

    return( COM_DATA_FULL );
}
#endif

#define LSC_CB_MAX_WAVE_NUMBER  ((LC_MAX_FRAME_DATA_SIZE - 30)/2)  /* 支持最大上送录波序号个数 */
#if SWITCH_IEC103_TI_M_CB
/* 通过录波序号的缓冲区获取单次电网故障的录波序号及个数OffSet:位置偏移
(返回值源地址缓冲区的当前位置偏移) */
static INT LSC_GetWaveIndexFromElecNumber( UNSIGNED * pSrcBuffer, INT iWaveIndexNumber,
    INT iElecNumber, UNSIGNED * pDstBuffer, INT * iNumber, INT OffSet, INT * NextElecNumber );
/* 召唤录波序号 */
INT LSC_Load_Wave_ID( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        buffer[128];              // define buffer size.
    UNSIGNED        buffer1[128];             // define buffer size.
    INT             k, number, number1 = 0;
    INT             Status, StatusTemp, iType, iTestNum;
    UNSIGNED        uProFaultNum, uProFaultNum1, uProFaultNum2 = 0;
    UNSIGNED        uStartFaultNum;
    INT             iLen;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    iType = *( pSrc + 5 ) & LS_DATA_MASKS8;                         /* 类型 */

    pDst ++;                                                        /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_CB;                                     /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                 /* CPU号码 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 ); /* 序列号低字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 ); /* 序列号高字节 */
    * pDst ++ = ( UNSIGNED_CHAR )iType;                             /* 类型 */
    if( iType == 0 )
    {
        /* 根据召唤类型的不同所产生的不同偏移量 */
        iTestNum = 8;
        * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 ); /* 起始故障序号低字节 */
        * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 7 ) & LS_DATA_MASKS8 ); /* 起始故障序号高字节 */
    }
    else
    {
        /* 根据召唤类型的不同所产生的不同偏移量 */
        iTestNum = 6;
    }
    Status = COM_DATA_FULL;
    StatusTemp = CWave.Find(& CWave, 0, buffer,& number, LS_WAVE_TIMEOUT );
    if( StatusTemp == NU_SUCCESS )
    {
        if( iType == 1 )
        {
            * pDst ++;
            /* 调全部故障录波的电网故障序号. */
            for( k = 0; k < number; k ++)
            {
                uProFaultNum1 = ( buffer[k] >> LS_DATA_BIT16 ) & LS_DATA_MASKS16;
                if( uProFaultNum1 != uProFaultNum2 )
                {
                    uProFaultNum2 = uProFaultNum1;
                    /* fill fault number. */
                    * pDst ++ = ( UNSIGNED_CHAR )( uProFaultNum1 & LS_DATA_MASKS8 );
                    /* fill fault number. */
                    * pDst ++ = ( UNSIGNED_CHAR )(( uProFaultNum1 >> LS_BYTE_BITS ) & LS_DATA_MASKS8 );
                    number1 ++;
                }
            }
            /* 填写报文长度 */
            pDst = ( UNSIGNED_CHAR *)Dst;
            * pDst = ( UNSIGNED_CHAR )( number1 * 2 + iTestNum );   // fill fault number.
            *( pDst + iTestNum ) = ( UNSIGNED_CHAR )number1;        // fill fault number.
        }
        else
        {
            INT  iNumber;
            INT  NextElecNumber;
            INT  iOffset;      /* 起始电网故障序号中的位置偏移 */
            INT  TempOffset;   /* 查询位置偏移 */
            INT  iCount1 = 0;  /* 电网故障个数 */
            INT  iCount = 0;   /* 录波索引号个数 */
            INT  i;
            INT  iWaveNumber;
            INT  iTempCount;

            /* 调电网故障的故障序号. */
            /* 起始故障序号 */
            uStartFaultNum = *( pSrc + 6 ) & LS_DATA_MASKS8; /* 低字节 */
            uStartFaultNum += ((*( pSrc + 7 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* 低字节 */

            /* 需求电网故障次数 */
            iWaveNumber = *( pSrc + 8 ) & LS_DATA_MASKS8;
            /* 起始电网故障序号中的位置偏移 */
            iOffset = *( pSrc + 9 ) & LS_DATA_MASKS8;

            TempOffset = number;   /* 从上往下查找 */
            uProFaultNum = uStartFaultNum;
            pDst ++;  /* 实际电网故障次数 */
            for( k = 0; k < number; k ++)
            {
                TempOffset = LSC_GetWaveIndexFromElecNumber( buffer, number,
                    uProFaultNum, buffer1, & iNumber, TempOffset, & NextElecNumber );
                if( ( iCount + iNumber ) > LSC_CB_MAX_WAVE_NUMBER )
                {
                    if( iCount == 0 )
                    {
                        iNumber = LSC_CB_MAX_WAVE_NUMBER;
                    }
                    else
                    {
                        /* 录波索引号到达允许的最大值返回 */
                        break;
                    }
                }

                iCount += iNumber;
                /* 起始电网故障序号带有偏移 */
                if( uProFaultNum == uStartFaultNum )
                {
                    /* 起始位置偏移 */
                    * pDst ++ = ( UNSIGNED_CHAR )iOffset;
                    /* 起始电网故障序号的总录波个数 */
                    * pDst ++ = ( UNSIGNED_CHAR )iNumber;
                    if( iNumber > iOffset )
                    {
                        iTempCount = iNumber - iOffset;
                    }
                    else
                    {
                        iTempCount = 0;
                    }
                }
                else
                {
                    iOffset = 0;
                    iTempCount = iNumber;
                }
                /* 填写报文 */
                * pDst ++ = ( UNSIGNED_CHAR )( uProFaultNum & LS_DATA_MASKS8 );
                * pDst ++ = ( UNSIGNED_CHAR )( ( uProFaultNum >> LS_BYTE_BITS ) & LS_DATA_MASKS8 );
                * pDst ++ = ( UNSIGNED_CHAR )iTempCount;
                for( i = iOffset; i < iTempCount; i ++)
                {
                    /* fill fault number. */
                    * pDst ++ = ( UNSIGNED_CHAR )( buffer1[i] & LS_DATA_MASKS8 );
                    /* fill fault number. */
                    * pDst ++ = ( UNSIGNED_CHAR )(( buffer1[i] >> LS_BYTE_BITS ) & LS_DATA_MASKS8 );
                }
                if(++ iCount1 >= iWaveNumber )
                {
                    /* 电网故障序号查找完成返回 */
                    break;
                }
                uProFaultNum = NextElecNumber;
            }

            /* 填写报文长度 */
            iLen = ( INT )(( UNSIGNED )pDst - ( UNSIGNED )Dst ) - 1;
            pDst = ( UNSIGNED_CHAR *)Dst;
            * pDst = ( UNSIGNED_CHAR )iLen; // fill fault number.
            *( pDst + iTestNum ) = ( UNSIGNED_CHAR )iCount1;        // 实际电网故障次数.
        }
    }
    else if( StatusTemp == NU_TIMEOUT )
    {
        Status = COM_DATA_TIMEOUT;
    }
    else
    {
        Status = COM_DATA_ERROR;
    }

    return( Status );
}

/* 通过录波序号的缓冲区获取单次电网故障的录波序号及个数OffSet:位置偏移
(返回值源地址缓冲区的当前位置偏移) */
static INT LSC_GetWaveIndexFromElecNumber( UNSIGNED * pSrcBuffer, INT iWaveIndexNumber,
    INT iElecNumber, UNSIGNED * pDstBuffer, INT * iNumber, INT OffSet, INT * NextElecNumber )
{
    UNSIGNED * pTemp;
    INT      i;
    INT      iFlag = 0;
    INT      iFaultNum1;
    INT      iCount = 0;
    INT      iOffset;
    UNSIGNED uStartFaultNum;
    INT      iStartOffset;
    /*==========================*/
    pTemp = ( UNSIGNED *)( pSrcBuffer + OffSet - 1 );
    uStartFaultNum = ( * pTemp >> LS_DATA_BIT16 ) & LS_DATA_MASKS16;
    iOffset = OffSet;
    iStartOffset = OffSet;
    if( iWaveIndexNumber >= OffSet )
    {
        for( i = 0; i < OffSet; i ++)
        {
            iFaultNum1 = ( *( pTemp - i ) >> LS_DATA_BIT16 ) & LS_DATA_MASKS16;
            if( iElecNumber == iFaultNum1 )
            {
                iFlag = 1;
                *( pDstBuffer + iCount ++) = *( pTemp - i );
            }
            else
            {
                /* 判断同一电网故障序号的录波是否搜索完成 */
                if( iFlag == 1 )
                {
                    /* 下一电网故障序号 */
                    * NextElecNumber = iFaultNum1;
                    break;
                }
            }
            iOffset --;
        }
    }
    * iNumber = iCount;

    /* 如果指定故障序号的录波不存在,则下一电网故障序号返回输入缓冲区的第一个电网故障序号 */
    if( iCount == 0 )
    {
        /* 下一电网故障序号 */
        * NextElecNumber = uStartFaultNum;
        /* 偏移位置还原为输入起始位置 */
        iOffset = iStartOffset;
    }

    return( iOffset );
}
#endif

#if SWITCH_IEC103_TI_M_CC
/* 召唤录波时间 */
INT LSC_Load_Wave_Time( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED        FaultNumber;
    INT             Status, StatusTemp;
    UNSIGNED_CHAR   * pSrc,* pDst;
    CWM_WHD         WaveHead,* pWaveHead;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 17;                                                 /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_CC;                                     /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                 /* CPU号码 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 ); /* 序列号低字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 ); /* 序列号高字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 ); /* 录波序号低字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 ); /* 录波序号高字节 */

    /* 录波序号 */
    FaultNumber = *( pSrc + 6 );    /* 高字节 */
    FaultNumber *= 256;             // fault number.
    FaultNumber += *( pSrc + 5 );   /* 低字节 */

    /* 读录波数据 */
    StatusTemp = CWave.Read(& CWave, FaultNumber,& WaveHead, 0, 0, sizeof( CWM_WHD ),
        LS_WAVE_TIMEOUT );

    if( StatusTemp == NU_SUCCESS )
    {
        pWaveHead = & WaveHead;

        * pDst ++ = LS_RESULT_TRUE; /* 状态 0x5A:成功,0x00:错误 */
        * pDst ++ = ( UNSIGNED_CHAR )( pWaveHead->Sof ); /* 录波类型 */

        /* 电网故障序号 */
        * pDst ++ = ( UNSIGNED_CHAR )( pWaveHead->ElecNumber & LS_DATA_MASKS8 ); /* 低字节 */
        * pDst ++ = ( UNSIGNED_CHAR )(( pWaveHead->ElecNumber >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* 高字节 */

        * pDst ++ = ( UNSIGNED_CHAR )( pWaveHead->Year );    /* 年 */
        * pDst ++ = ( UNSIGNED_CHAR )( pWaveHead->Month );   /* 月 */
        * pDst ++ = ( UNSIGNED_CHAR )( pWaveHead->Day );     /* 日 */
        * pDst ++ = ( UNSIGNED_CHAR )( pWaveHead->Hour );    /* 时 */
        * pDst ++ = ( UNSIGNED_CHAR )( pWaveHead->Minute );  /* 分 */

        /* 秒（毫秒） */
        * pDst ++ = ( UNSIGNED_CHAR )( pWaveHead->Millionsecond & LS_DATA_MASKS8 ); /* 低字节 */
        * pDst ++ = ( UNSIGNED_CHAR )(( pWaveHead->Millionsecond >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* 高字节 */

        Status = COM_DATA_FULL;
    }
    else if( StatusTemp == NU_TIMEOUT )
    {
        Status = COM_DATA_TIMEOUT;
    }
    else
    {
        * pDst ++ = LS_RESULT_NULL; /* 状态 0x5A:成功,0x00:错误 */
        Status = COM_DATA_FULL;
    }

    return( Status );
}
#endif

#if SWITCH_IEC103_TI_M_CD
/* 召唤录波信息 */
INT LSC_Load_Wave_Inf( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        StartFaultNumber, EndFaultNumber, Number, Serial;
    INT             Status, StatusTemp;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 12;                                      /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_CD;                          /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );      /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                           /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                           /* 序列号高字节 */

    /* 获取报告信息 */
    StatusTemp = CWave.Information(& CWave,& Number,& Serial,& StartFaultNumber,
        & EndFaultNumber, LS_WAVE_TIMEOUT );

    if( StatusTemp == NU_SUCCESS )
    {
        /* 故障总个数 */
        * pDst ++ = ( UNSIGNED_CHAR )( Number & LS_DATA_MASKS8 ); /* 低字节 */
        * pDst ++ = ( UNSIGNED_CHAR )(( Number >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* 高字节 */

        /* 起始故障序号 */
        * pDst ++ = ( UNSIGNED_CHAR )( StartFaultNumber & LS_DATA_MASKS8 ); /* 低字节 */
        * pDst ++ = ( UNSIGNED_CHAR )(( StartFaultNumber >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* 高字节 */

        /* 结束故障序号 */
        * pDst ++ = ( UNSIGNED_CHAR )( EndFaultNumber & LS_DATA_MASKS8 ); /* 低字节 */
        * pDst ++ = ( UNSIGNED_CHAR )(( EndFaultNumber >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* 高字节 */

        /* 结束报告索引号 */
        * pDst ++ = ( UNSIGNED_CHAR )( Serial & LS_DATA_MASKS8 ); /* 低字节 */
        * pDst ++ = ( UNSIGNED_CHAR )(( Serial >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* 高字节 */

        Status = COM_DATA_FULL;
    }
    else if( StatusTemp == NU_TIMEOUT )
    {
        Status = COM_DATA_TIMEOUT;
    }
    else
    {
        Status = COM_DATA_ERROR;
    }

    return( Status );
}
#endif

#if SWITCH_IEC103_TI_M_CE
/* 召唤录波电网故障序号 */
INT LSC_Load_Wave_FaultSerl( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst;
    INT             Status, StatusTemp;
    UNSIGNED        Buffer[CPC_MAX_NVRAM_WAVE_NUMBER];
    INT             i;
    UNSIGNED        uTotal_Num;
    UNSIGNED        uEnd_FaultNum;
    UNSIGNED        uNumber;
    INT             number;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    /* 总个数 */
    uTotal_Num = *( pSrc + 5 ) & LS_DATA_MASKS8; /* 低字节 */
    uTotal_Num += ((*( pSrc + 6 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* 高字节 */

    /* 结束故障序号 */
    uEnd_FaultNum = *( pSrc + 7 ) & LS_DATA_MASKS8; /* 低字节 */
    uEnd_FaultNum += ((*( pSrc + 8 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* 高字节 */

    number = 0;
    /* 获取报告信息 */
    StatusTemp = CWave.FindFaultNumber( & CWave, Buffer, & number, 1, LS_WAVE_TIMEOUT );

    pDst += 7; /* 调整填写故障报告序号的目的地址 */
    if( StatusTemp == NU_SUCCESS )
    {
        uNumber = 0;
        for( i = 0; i < number; i ++)
        {
            if( Buffer[i] == uEnd_FaultNum )
            {
                /* 满足条件的实际故障报告序号个数 */
                uNumber = ( i + 1 );
                break;
            }
        }

        /* 如果召唤个数大于实际个数取实际个数 */
        if( uTotal_Num > uNumber )
        {
            uTotal_Num = uNumber;
        }

        /* 判断上调的录波个数是否在允许的范围之内(报文长度决定) */
        if( uTotal_Num > LC_MAX_WAVE_SERIAL )
        {
            uTotal_Num = LC_MAX_WAVE_SERIAL;
        }

        for( i = 0; i <( INT ) uTotal_Num; i ++)
        {
            /* 填写事件索引号,从end开始 */
            for( i = 0; i < ( INT )uTotal_Num; i ++)
            {
                /* 故障序号 */
                * pDst ++ = ( UNSIGNED_CHAR )( Buffer[uNumber - 1 - i] & LS_DATA_MASKS8 ); /* 低字节 */
                * pDst ++ = ( UNSIGNED_CHAR )(( Buffer[uNumber - 1 - i] >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* 高字节 */
            }
        }
        Status = COM_DATA_FULL;
    }
    else if( StatusTemp == NU_TIMEOUT )
    {
        Status = COM_DATA_TIMEOUT;
    }
    else
    {
        Status = COM_DATA_ERROR;
    }

    pDst = ( UNSIGNED_CHAR *)Dst;
    * pDst ++ = ( UNSIGNED_CHAR )( uTotal_Num * 2 + 6 ); /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_CE;                          /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );      /* CPU号码 */
    * pDst ++ = *( pSrc + 3 );                           /* 序列号低字节 */
    * pDst ++ = *( pSrc + 4 );                           /* 序列号高字节 */
    /* 实际个数 */
    * pDst ++ = ( UNSIGNED_CHAR )( uTotal_Num & LS_DATA_MASKS8 ); /* 低字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(( uTotal_Num >> LS_BYTE_BITS ) & LS_DATA_MASKS8 ); /* 高字节 */

    return( Status );
}
#endif

#if SWITCH_IEC103_TI_M_E0
/* 预发传动开出 */
INT LSC_Trip_Test_Ready( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             iType, iActualType;
    INT             iResult = LS_RESULT_NULL;
    UNSIGNED_CHAR   * pSrc,* pDst;
    UNSIGNED        uGroupNum;
    UNSIGNED        uGroupID[CPC_MAX_OUTPUT_GROUPS];
    INT             i;
    RJUMPER         * pRJumper;
    INT             jum_status;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;

    iType = (*( pSrc + 5 ) & LS_DATA_MASKS8 );     /* 传动类型 */
    uGroupNum = (*( pSrc + 6 ) & LS_DATA_MASKS8 ); /* 开出组数 */
    iActualType = 0;

    pRJumper = GET_RJUMPER_TAB_MEMORY;
    if(( pRJumper != NU_NULL ) && ( GET_RJUMPER_TAB_NUMBER != 0 ))
    {
        /* 检修压板判别 */
        if( pRJumper-> dataType == POINT_TYPE_SINGLE )
        {
            /* 单点 */
            jum_status = SINGLE_POINT_OPT;
        }
        else
        {
            /* 双点 */
            jum_status = DUAL_POINT_OPT;
        }

        if( pRJumper->jum_status != jum_status )
        {
            /* 检修压板未投入,直接返回失败 */
            pSrc = ( UNSIGNED_CHAR *)Src;
            pDst = ( UNSIGNED_CHAR *)Dst;

            * pDst ++ = ( UNSIGNED_CHAR )( 7 + uGroupNum *( 1 + LS_DATA_WIDTH )); /* 链路用户数据长度 */
            * pDst ++ = IEC103_TI_M_E0;                                           /* 类别标识 */
            * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                       /* CPU号码 */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 );       /* 序列号低字节 */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 );       /* 序列号高字节 */
            * pDst ++ = ( UNSIGNED_CHAR )iActualType;                             /* 传动类型 */
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 );       /* 开出组数 */
            for( i = 0; i < ( INT )( uGroupNum *( 1 + LS_DATA_WIDTH )); i ++)
            {
                * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 7 + i ) & LS_DATA_MASKS8 ); /* 开出位 */
            }
            * pDst ++ = ( UNSIGNED_CHAR )iResult;                                   /* 执行状态 */

            return( COM_DATA_FULL );
        }
    }

    if(( iType & LS_START_RELAY_TEST ) && ( LS_CPU_TYPE & CPU_TYPE_STARTUP ))
    {
        /* test start relay */
        iActualType |= LS_START_RELAY_TEST;
        iResult = LS_RESULT_TRUE;
    }

    if(( iType & LS_OUT_RELAY_TEST ) != 0 )
    {
        /* 如果出口组数大于最大出口组数,返回失败 */
        if( uGroupNum > CPC_MAX_OUTPUT_GROUPS )
        {
            iResult = LS_RESULT_NULL;
        }
        else
        {
            iResult = LS_RESULT_TRUE;
            iActualType |= LS_OUT_RELAY_TEST;
            for( i = 0; i < ( INT )uGroupNum; i ++)
            {
                /* 将下发的通道组号先记录下来,看其是否满足要求 */
                uGroupID[i] = (*( pSrc + 7 + i * LS_DATA_WIDTH ) & LS_DATA_MASKS8 );
            }

            for( i = 0; i < ( INT )uGroupNum; i ++)
            {
                if( uGroupID[i] >= CPC_MAX_OUTPUT_GROUPS )
                {
                    iResult = LS_RESULT_NULL;
                    break;
                }
            }
        }
    }

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = ( UNSIGNED_CHAR )( 7 + uGroupNum *( 1 + LS_DATA_WIDTH )); /* 链路用户数据长度 */
    * pDst ++ = IEC103_TI_M_E0;                                           /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                       /* CPU号码 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 );       /* 序列号低字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 );       /* 序列号高字节 */
    * pDst ++ = ( UNSIGNED_CHAR )iActualType;                             /* 传动类型 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 );       /* 开出组数 */
    for( i = 0; i < ( INT )( uGroupNum *( 1 + LS_DATA_WIDTH )); i ++)
    {
        * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 7 + i ) & LS_DATA_MASKS8 ); /* 开出位 */
    }
    * pDst ++ = ( UNSIGNED_CHAR )iResult;                                   /* 执行状态 */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_E1
/* 执行传动开出 */
INT LSC_Trip_Test( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             iType, iActualType;
    INT             iResult = LS_RESULT_NULL;
    UNSIGNED_CHAR   * pSrc,* pDst;
    CHAR            EventName[LS_ALARM_NAME];
    UNSIGNED        uGroupNum;
    UNSIGNED        uGroupID[CPC_MAX_OUTPUT_GROUPS * 2]; /* buffer size */
    INT             i, j;
    INT             iFlag; /* 按通道传动时,用于判断信号灯和跳闸灯是否已经执行过标志.1:信号;2:跳闸;3:信号+跳闸 */
    UNSIGNED        * pTripLedMemory = LS_GET_OUTPUT_TRIP_LED_MEMORY;
    UNSIGNED        * pSignalLedMemory = LS_GET_OUTPUT_SIGNAL_LED_MEMORY;
    UNSIGNED        uLedBits;
    RJUMPER         * pRJumper;
    INT             jum_status;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;

    iType = (*( pSrc + 5 ) & LS_DATA_MASKS8 );     // 传动类型
    uGroupNum = (*( pSrc + 6 ) & LS_DATA_MASKS8 ); // 出口组数
    iActualType = 0;

    pRJumper = GET_RJUMPER_TAB_MEMORY;
    if(( pRJumper != NU_NULL ) && ( GET_RJUMPER_TAB_NUMBER != 0 ))
    {
        /* 检修压板判别 */
        if( pRJumper-> dataType == POINT_TYPE_SINGLE )
        {
            /* 单点 */
            jum_status = SINGLE_POINT_OPT;
        }
        else
        {
            /* 双点 */
            jum_status = DUAL_POINT_OPT;
        }

        if( pRJumper->jum_status != jum_status )
        {
            /* 检修压板未投入,直接返回失败 */
            pSrc = ( UNSIGNED_CHAR *)Src;
            pDst = ( UNSIGNED_CHAR *)Dst;

            * pDst ++ = ( UNSIGNED_CHAR )( 7 + uGroupNum *( 1 + LS_DATA_WIDTH )); // fill report length.
            * pDst ++ = IEC103_TI_M_E1;                                // fill command type.
            * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );            // fill cpu number.
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 ); // fill serial number: byte 1
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 ); // fill serial number: byte 2
            * pDst ++ = ( UNSIGNED_CHAR )iActualType;                       // fill test type.
            * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 ); // fill output group number.
            for( i = 0; i < ( INT )( uGroupNum *( 1 + LS_DATA_WIDTH )); i ++)
            {
                * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 7 + i ) & LS_DATA_MASKS8 ); // fill output bits.
            }
            * pDst ++ = ( UNSIGNED_CHAR )iResult;                           // fill result.

            return( COM_DATA_FULL );
        }
    }

    if(( iType & LS_START_RELAY_TEST ) && ( LS_CPU_TYPE & CPU_TYPE_STARTUP ))
    {
        /* test start relay */
        NU_Relinquish();

        /* 启动继电器动作 */
        XJP_IO_Set_Start_Relay( PlatHIOM, 1 );
        sprintf( EventName, " %03d", LS_RES_EVENT_START );
        CEMC_FillEvent( EventName, RESERVE_EVENT_ID );
        iActualType |= LS_START_RELAY_TEST;
        iResult = LS_RESULT_TRUE;
        NU_Control_Timer(& Timer_Trip_CB, NU_ENABLE_TIMER );
    }

    if(( iType & ( LS_OUT_RELAY_TEST | LS_TRIP_RELAY_TEST ) ) != 0 )
    {
        /* 如果出口组数大于最大出口组数,返回失败 */
        if( uGroupNum > CPC_MAX_OUTPUT_GROUPS )
        {
            iResult = LS_RESULT_NULL;
        }
        else
        {
            iResult = LS_RESULT_TRUE;

            /* 判断传动类型 */
            if( iType & LS_OUT_RELAY_TEST )
            {
                iActualType |= LS_OUT_RELAY_TEST;
            }
            else
            {
                iActualType |= LS_TRIP_RELAY_TEST;
            }

            for( i = 0; i < ( INT )uGroupNum ; i ++)
            {
                /* 将下发的通道组号先记录下来,看其是否满足要求 */
                uGroupID[i * 2] = (*( pSrc + 7 + i * ( LS_DATA_WIDTH + 1 ) ) & LS_DATA_MASKS8 );
                /* 记录出口位 */
                LS_BYTE_TO_WORD( & uGroupID[2 * i + 1],( pSrc + 8 + i * ( LS_DATA_WIDTH + 1 ) ), LS_DATA_WIDTH );
            }

            for( i = 0; i < ( INT )uGroupNum; i ++)
            {
                /* 判断出口组号是否在允许的范围之内 */
                if( uGroupID[i * 2] >= CPC_MAX_OUTPUT_GROUPS )
                {
                    iResult = LS_RESULT_NULL;
                    break;
                }
            }
        }
        if( iResult == LS_RESULT_TRUE )
        {
            NU_Relinquish();

            /* 如果为跳闸cpu,当启动回路串联时也要点启动继电器 */
            if(( LS_CPU_TYPE & CPU_TYPE_TRIP )
                && ( LS_CPU_TYPE & CPU_TYPE_SERIAL ))
            {
                XJP_IO_Set_Start_Relay( PlatHIOM, 1 );
            }

            iFlag = 0;
            LC_REPORT_PROTECT();
            for( i = 0; i < ( INT )uGroupNum; i ++)
            {
                /* 设置开出位 */
                XJP_IO_Set_Output_Relay( PlatHIOM, uGroupID[i * 2], uGroupID[i * 2 + 1], CIO_EQU );
                if( iType & LS_OUT_RELAY_TEST )
                {
                    for( j = 0; j < LS_GET_OUTPUT_GROUPS; j ++)
                    {
                        uLedBits = *( pSignalLedMemory + j );
                        uLedBits |= *( pTripLedMemory + j );
                        /* 设置装置灯开出 */
                        XJP_IO_Set_Output_Relay( PlatHIOM, j, uLedBits, CIO_OR );
                    }
                }
            }
            LS_REPORT_UNPROTECT();
            sprintf( EventName, " %03d:%02d ", LS_RES_EVENT_OUT, uGroupNum );
            CEMC_FillEvent( EventName, RESERVE_EVENT_ID );
        }
    }

    if( iResult == LS_RESULT_TRUE )
    {
        NU_Control_Timer(& Timer_Trip_CB, NU_ENABLE_TIMER );
    }

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = ( UNSIGNED_CHAR )( 7 + uGroupNum *( 1 + LS_DATA_WIDTH ));                       // fill report length.
    * pDst ++ = IEC103_TI_M_E1;                                      // fill command type.
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );            // fill cpu number.
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 ); // fill serial number: byte 1
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 ); // fill serial number: byte 2
    * pDst ++ = ( UNSIGNED_CHAR )iActualType;                        // fill test type.
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 ); // fill output group number.
    for( i = 0; i < ( INT )( uGroupNum *( 1 + LS_DATA_WIDTH )); i ++)
    {
        * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 7 + i ) & LS_DATA_MASKS8 ); // fill output bits.
    }
    * pDst ++ = ( UNSIGNED_CHAR )iResult;                            // fill result.

    return( COM_DATA_FULL );
}
#endif

//
// trip test timer main function
//
void LSC_TripTimerRoutine( UNSIGNED argc )
{
    INT       j;
    UNSIGNED  uOutBits[CPC_MAX_OUTPUT_GROUPS];
    //--------------------------------------------------------------------------
    LS_UNUSED_PARAM( argc );
    /* 将出口位设置初值0 */
    memset( uOutBits, 0,( sizeof( UNSIGNED )* CPC_MAX_OUTPUT_GROUPS ));
    /* 出口收回时要进行数据保护 */
    LS_REPORT_PROTECT();
    /* 启动继电器收回 */
    XJP_IO_Set_Start_Relay( PlatHIOM, 0 );
    /* 收回传动的出口位 */
    for( j = 0; j < CPC_MAX_OUTPUT_GROUPS; j ++)
    {
        XJP_IO_Set_Output_Relay( PlatHIOM, j, 0, CIO_EQU );
    }
    LS_REPORT_UNPROTECT();

    NU_Control_Timer(& Timer_Trip_CB, NU_DISABLE_TIMER );
}

#if SWITCH_IEC103_TI_M_E2
/* 调通道系数及量值 */
INT LSC_Load_Channel_Cofficient( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             k;
    INT             Length;
    INT             Counter;
    INT             ChlNumber;
    UNSIGNED        uTemp;
    PRO_CHANNEL     * pChannel;
    float           Variable_r, Variable_i, Variable;
    float           ChannelCoefficient;
    UNSIGNED_CHAR   * pSrc,* pDst,* p;
    INT             SampleOffset;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    pDst += 6; /* 通道ID地址 */
    Counter = *( pSrc + 5 ) & LS_DATA_MASKS8;  /* 通道个数 */
    /* 获取采样偏移位置 */
    SampleOffset = CSMV_GET_SAMPLE_OFFSET;
    for( k = 0; k < Counter; k ++)
    {
        ChlNumber = *( pSrc + k + 6 ) & LS_DATA_MASKS8; /* 通道ID */
        pChannel = LS_CHANNEL_TABLE_MEMORY; /* 通道数据表内存首地址 */
        pChannel += ChlNumber;
        if( ChlNumber < LS_CHANNEL_TABLE_NUMBER )
        {
            if(( pChannel -> chl_type & CHL_PEAKVALUE ) != 0 )
            {
                /* 峰值 */
                Variable = LS_CALCULATE_PEAK_VALUE( ChlNumber, SampleOffset );
                Variable = Variable * pChannel -> chl_rate_coefficient;
                Variable = Variable / pChannel -> chl_calculate_coefficient;
                Variable *= ( float ) sqrt( 2 ); /* 折算成有效值 */
            }
            else if(( pChannel -> chl_type & FUNDAMENTAL ) != 0 )
            {
                /* 基波分量 */
                Variable_r = LS_CALCULATE_FOURIER( ChlNumber, FOURIER01_R, SampleOffset );
                Variable_i = LS_CALCULATE_FOURIER( ChlNumber, FOURIER01_I, SampleOffset );
                Variable = Variable_r * Variable_r + Variable_i * Variable_i;
                Variable = ( float )sqrt( Variable );
                Variable = Variable * pChannel -> chl_rate_coefficient;
                Variable = Variable / pChannel -> chl_calculate_coefficient;
            }
            else if(( pChannel -> chl_type & HARMONIC_2ND ) != 0 )
            {
                /* 二次谐波分量 */
                Variable_r = LS_CALCULATE_FOURIER( ChlNumber, FOURIER02_R, SampleOffset );
                Variable_i = LS_CALCULATE_FOURIER( ChlNumber, FOURIER02_I, SampleOffset );
                Variable = Variable_r * Variable_r + Variable_i * Variable_i;
                Variable = ( float )sqrt( Variable );
                Variable = Variable * pChannel -> chl_rate_coefficient;
                Variable = Variable / pChannel -> chl_calculate_coefficient;
            }
            else if(( pChannel -> chl_type & HARMONIC_3RD ) != 0 )
            {
                /* 三次谐波分量 */
                Variable_r = LS_CALCULATE_FOURIER( ChlNumber, FOURIER03_R, SampleOffset );
                Variable_i = LS_CALCULATE_FOURIER( ChlNumber, FOURIER03_I, SampleOffset );
                Variable = Variable_r * Variable_r + Variable_i * Variable_i;
                Variable = ( float )sqrt( Variable );
                Variable = Variable * pChannel -> chl_rate_coefficient;
                Variable = Variable / pChannel -> chl_calculate_coefficient;
            }
            else if(( pChannel -> chl_type & HARMONIC_5TH ) != 0 )
            {
                /* 五次谐波分量 */
                Variable_r = LS_CALCULATE_FOURIER( ChlNumber, FOURIER05_R, SampleOffset );
                Variable_i = LS_CALCULATE_FOURIER( ChlNumber, FOURIER05_I, SampleOffset );
                Variable = Variable_r * Variable_r + Variable_i * Variable_i;
                Variable = ( float )sqrt( Variable );
                Variable = Variable * pChannel -> chl_rate_coefficient;
                Variable = Variable / pChannel -> chl_calculate_coefficient;
            }
            else if(( pChannel -> chl_type & DC_COMPONENT ) != 0 )
            {
                /* 直流分量 */
                Variable = LS_CALCULATE_DC( ChlNumber, LS_DC_COEFFICIENT, SampleOffset );
                Variable = Variable * pChannel -> chl_rate_coefficient;
                Variable = Variable / pChannel -> chl_calculate_coefficient;
            }
            else
            {
                Variable = 0.0;
            }
            ChannelCoefficient = pChannel -> chl_channel_coefficient;
        }
        else
        {
            Variable = 0.0;
            ChannelCoefficient = 0.0;
        }

        /* 填充通道数据 */
        * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + k + 6 ) & LS_DATA_MASKS8 );  /* 通道ID */
        /* 通道系数 */
        uTemp = LS_FLOAT_TO_IEEE( ChannelCoefficient );
        p = ( UNSIGNED_CHAR *)& uTemp;
        LS_WORD_TO_BYTE( pDst, p, LS_DATA_WIDTH );
        pDst += LS_DATA_WIDTH;
        /* 通道量值 */
        uTemp = LS_FLOAT_TO_IEEE( Variable );
        p = ( UNSIGNED_CHAR *)& uTemp;
        LS_WORD_TO_BYTE( pDst, p, LS_DATA_WIDTH );
        pDst += LS_DATA_WIDTH;
    }

    pDst = ( UNSIGNED_CHAR *)Dst;
    Length = 2 * LS_DATA_WIDTH * Counter + Counter;             /* 命令报文长度 */
    * pDst ++ = ( UNSIGNED_CHAR )( Length + 5 );                /* 报文长度 */
    * pDst ++ = IEC103_TI_M_E2;                                 /* 命令类型 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );             /* CPU号码 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 ); /* 序列号低字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 ); /* 序列号高字节 */
    * pDst ++ = ( UNSIGNED_CHAR )Counter;                       /* 通道个数 */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_E3
/* 修改通道系数 */
INT LSC_Change_Channel_Cofficient( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             k;
    INT             iResult;
    INT             Counter;
    INT             ChlNumber;
    INT             iNumber;
    INT             iStartNum;
    INT             iType;
    INT             iStatus;
    TB_CFB          CondField;
    TB_TCB          * pTable;
    UNSIGNED_CHAR   * pSrc,* pDst, * p;
    float           fCVRate;
    float           fCoff[LS_MAX_CHL_NUMBER];
    float           Variable_r, Variable_i, Variable;
    UNSIGNED        uIeeChlValue;
    PRO_CHANNEL     * pChannel;
    INT             SampleOffset;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    iResult = LS_RESULT_NULL;
    pSrc = ( UNSIGNED_CHAR *)Src;

    ChlNumber = *( pSrc + 5 ) & LS_DATA_MASKS8; /* 通道ID */
    /* 获取采样偏移位置 */
    SampleOffset = CSMV_GET_SAMPLE_OFFSET;
    /* 查找数据表便于找到数据表的个数 */
    pTable = CTBC_FindTable( TAB_CHANNEL_ID );
    if( ChlNumber == 0xFF ) /* 自动校正 */
    {
        /* 自动校正通道系数 */
        pChannel = LS_CHANNEL_TABLE_MEMORY; /* 通道数据表内存首地址 */
        Counter = LS_CHANNEL_TABLE_NUMBER;  /* 通道数据表数据个数 */
        for( k = 0; k < Counter; k ++)
        {
            if((( pChannel -> chl_type & FUNDAMENTAL ) != 0 ) && (( pChannel->chl_type & CHL_COEFFICIENT ) != 0 )
                && (( pChannel -> chl_type & CHL_CURRENT ) || ( pChannel -> chl_type & CHL_VOLTAGE )))
            {
                /* 有效电流或电压通道 */
                Variable_r = LS_CALCULATE_FOURIER( ( pChannel->chl_Num ), FOURIER01_R, SampleOffset );
                Variable_i = LS_CALCULATE_FOURIER( ( pChannel->chl_Num ), FOURIER01_I, SampleOffset );
                Variable = Variable_r * Variable_r + Variable_i * Variable_i;
                Variable = ( float )sqrt( Variable );
                Variable = Variable * pChannel -> chl_rate_coefficient;
                Variable = Variable / pChannel -> chl_calculate_coefficient;

                if( pChannel -> chl_type & CHL_CURRENT )
                {
                    /* 电流通道，按1.2倍额定值校正 */
                    fCVRate = ( float ) (( pChannel->chl_secondary_rate )* LS_CURRENT_COEFFICIENT
                        *( pChannel->chl_channel_coefficient ));
                }
                else if( pChannel -> chl_type & CHL_VOLTAGE )
                {
                    /* 电压通道，按40V校正 */
                    fCVRate = ( float )( LS_VOLTAGE_NORMVALUE *( pChannel->chl_channel_coefficient ));
                }
                if(( Variable >= ( fCVRate * LS_MIN_COEFFICIENT )) && ( Variable <= ( fCVRate * LS_MAX_COEFFICIENT )))
                {
                    fCoff[k] = fCVRate / Variable;
                }
                else
                {
                    fCoff[k] = pChannel -> chl_channel_coefficient;
                }
            }
            else
            {
                fCoff[k] = pChannel -> chl_channel_coefficient;
            }
            pChannel ++;
        }
        /* 起始序号 */
        iStartNum = 0;
        /* 修改个数 */
        iNumber = pTable->DataNumber;
        /* 修改类型 */
        iType = SETTING_ALL_ENABLE;
    }
    else
    {
        /* 按通道校正通道系数 */
        if( ChlNumber < LS_CHANNEL_TABLE_NUMBER )
        {
            pChannel = LS_CHANNEL_TABLE_MEMORY; /* 通道数据表内存首地址 */
            pChannel += ChlNumber;

            uIeeChlValue = 0;
            for( k = 0; k < LS_DATA_WIDTH; k ++)
            {
                uIeeChlValue += (*( pSrc + 6 + k ) & LS_DATA_MASKS8 ) << ( k * 8 ); /* 校正系数 */
            }

            /* 将数据转换为TMS格式的浮点数 */
            fCoff[0] = LS_IEEE_TO_FLOAT( uIeeChlValue );
        }
        /* 起始序号 */
        iStartNum = ChlNumber;
        /* 修改个数 */
        iNumber = 1;
        /* 修改类型 */
        iType = SETTING_ONE_ENABLE;
    }
    /* 设置操作字段 */
    CondField.HandleIndex = CHANNELN_CHLCOEFF_INDEX;
    /* 条件个数(修改全部通道) */
    CondField.CondNumber = 0;
    p = ( UNSIGNED_CHAR *)Dst;
    p --;
    * p = *( pSrc - 1 ) & LC_VAR_CONTROL_BITS;

    /* 修改量值并将量值写入ee中 */
    iStatus = LSC_Change_TableInf( TAB_CHANNEL_ID, iStartNum, iNumber,
        & CondField, fCoff, iType, p );
    if( iStatus == NU_SUCCESS )
    {
        /* 整个修改过程成功后更新ram区的通道系数 */
        pChannel = LS_CHANNEL_TABLE_MEMORY; /* 通道数据表内存首地址 */
        /* 自动校正系数模式 */
        if( ChlNumber == 0xFF )
        {
            for( k = 0; k < Counter; k ++)
            {
                ( pChannel + k ) -> chl_channel_coefficient = fCoff[k];
            }
        }
        else   /* 修改单个通道系数 */
        {
            ( pChannel + ChlNumber )-> chl_channel_coefficient = fCoff[0];;
        }

        iResult = LS_RESULT_TRUE;
    }

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 6;                                                   /* 命令报文长度 */
    * pDst ++ = IEC103_TI_M_E3;                                      /* 命令类型 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                  /* CPU号码 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 );  /* 序列号低字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 );  /* 序列号高字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 );  /* 通道ID */
    * pDst ++ = ( UNSIGNED_CHAR )iResult;                            /* 执行结果 */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_E4
/* 调通道额定值 */
INT LSC_Load_Channel_Rate( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             k;
    INT             Counter;
    INT             ChlNumber;
    UNSIGNED        uTemp;
    INT             Length;
    PRO_CHANNEL     * pChannel;
    UNSIGNED_CHAR   * p,* pSrc,* pDst;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    Counter = *( pSrc + 5 ) & LS_DATA_MASKS8;                   /* 通道个数 */
    pDst += 6;                                                  /* 记录通道数据的起始位置(通道id) */
    for( k = 0; k < Counter; k ++)
    {
        ChlNumber = *( pSrc + k + 6 ) & LS_DATA_MASKS8;         /* 通道ID */
        pChannel = LS_CHANNEL_TABLE_MEMORY;                     /* 通道数据表内存首地址 */
        pChannel += ChlNumber;

        * pDst ++ = ( UNSIGNED_CHAR )ChlNumber;
        /* 通道一次额定值 */
        uTemp = LS_FLOAT_TO_IEEE( pChannel -> chl_primary_rate );
        p = ( UNSIGNED_CHAR *)& uTemp;
        LS_WORD_TO_BYTE( pDst, p, LS_DATA_WIDTH );
        pDst += LS_DATA_WIDTH;
        /* 通道二次额定值 */
        uTemp = LS_FLOAT_TO_IEEE( pChannel -> chl_secondary_rate );
        p = ( UNSIGNED_CHAR *)& uTemp;
        LS_WORD_TO_BYTE( pDst, p, LS_DATA_WIDTH );
        pDst += LS_DATA_WIDTH;
    }

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;
    Length = 2 * LS_DATA_WIDTH * Counter + Counter;             /* 命令报文长度 */
    * pDst ++ = ( UNSIGNED_CHAR )( Length + 5 );                /* 报文长度 */
    * pDst ++ = IEC103_TI_M_E4;                                 /* 命令类型 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );             /* CPU号码 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 ); /* 序列号低字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 ); /* 序列号高字节 */
    * pDst ++ = ( UNSIGNED_CHAR )Counter;                       /* 通道个数 */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_E5
/* 修改通道额定值 */
INT LSC_Change_Channel_Rate( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             i;
    INT             k;
    INT             Result;
    INT             iStatus;
    UNSIGNED        uRate[2];  /* 通道额定值 */
    UNSIGNED_CHAR   * pSrc,* pDst, * p;
    INT             ChlGpNum;
    INT             ChlNumber;
    PRO_CHANNEL     * pChannel;
    float           fRate1[LS_MAX_CHL_NUMBER];  /* 一次额定值缓冲区 */
    float           fRate2[LS_MAX_CHL_NUMBER];  /* 二次额定值缓冲区 */
    float           fRate3[2];                  /* 单个通道额定值缓冲区 */
    VOID            * pTempRate;                /* 额定值指针 */
    TB_CFB          CondField;
    TB_TCB          * pTable;
    INT             iNumber;
    INT             iStartNum;
    INT             iType;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    Result = LS_RESULT_NULL;
    iStatus = LS_RESULT_NULL;
    pSrc = ( UNSIGNED_CHAR *)Src;

    ChlGpNum = *( pSrc + 5 ) & LS_DATA_MASKS8;           /* 通道组别低字节 */
    ChlGpNum += ((*( pSrc + 6 ) & LS_DATA_MASKS8 ) << LS_BYTE_BITS ); /* 通道组别高字节 */
    ChlNumber = *( pSrc + 7 ) & LS_DATA_MASKS8;  /* 通道ID */

    uRate[0] = 0;
    for( k = 0; k < LS_DATA_WIDTH; k ++)
    {
        uRate[0] += (*( pSrc + 8 + k ) & LS_DATA_MASKS8 ) << ( k * 8 );  /* 一次额定值 */
    }
    fRate3[0] = LS_IEEE_TO_FLOAT( uRate[0] ); /* IEEEE格式转换成装置浮点格式 */

    uRate[1] = 0;
    for( k = 0; k < LS_DATA_WIDTH; k ++)
    {
        uRate[1] += (*( pSrc + 12 + k ) & LS_DATA_MASKS8 ) << ( k * 8 ); /* 二次额定值 */
    }
    fRate3[1] = LS_IEEE_TO_FLOAT( uRate[1] ); /* IEEEE格式转换成装置浮点格式 */

    /* 查找数据表便于找到数据表的个数 */
    pTable = CTBC_FindTable( TAB_CHANNEL_ID );
    if( ChlGpNum == 0 )
    {
        /* 起始序号 */
        iStartNum = ChlNumber;
        /* 修改个数 */
        iNumber = 1;
        /* 修改类型 */
        iType = SETTING_ONE_ENABLE;
        /* 一次额定值 */
        fRate1[0] = fRate3[0];
        /* 二次额定值 */
        fRate2[0] = fRate3[1];
    }
    else
    {
        pChannel = LS_CHANNEL_TABLE_MEMORY;  /* 通道数据表内存首地址 */
        for( k = 0; k < LS_CHANNEL_TABLE_NUMBER; k ++)
        {
            if(( pChannel -> chl_correlation_channel == ChlGpNum )
                &&( pChannel -> chl_type & CHL_RATE ))
            {
                /* 一次额定值 */
                fRate1[k] = fRate3[0];
                /* 二次额定值 */
                fRate2[k] = fRate3[1];
            }
            else
            {
                /* 一次额定值 */
                fRate1[k] = pChannel -> chl_primary_rate;
                /* 二次额定值 */
                fRate2[k] = pChannel -> chl_secondary_rate;
            }
            pChannel ++;
        }

        /* 起始序号 */
        iStartNum = 0;
        /* 修改个数 */
        iNumber = pTable->DataNumber;
        /* 修改类型 */
        iType = SETTING_ALL_ENABLE;
    }

    /* 修改一、二次额定值 */
    for( i = 0; i < 2; i ++)
    {
        /* 一次额定值 */
        if( i == 0 )
        {
            /* 设置操作字段(一次额定值) */
            CondField.HandleIndex = CHANNELN_PRIMRATE_INDEX;
            pTempRate = fRate1;
        }
        else
        {
            /* 设置操作字段(二次额定值) */
            CondField.HandleIndex = CHANNELN_SECRATE_INDEX;
            pTempRate = fRate2;
        }

        /* 条件个数(因起始序号已经为通道id) */
        CondField.CondNumber = 0;
        p = ( UNSIGNED_CHAR *)Dst;
        p --;
        * p = *( pSrc - 1 ) & LC_VAR_CONTROL_BITS;

        /* 修改量值并将量值写入ee中 */
        iStatus = LSC_Change_TableInf( TAB_CHANNEL_ID, iStartNum, iNumber,
            & CondField, pTempRate, iType, p );
        if( iStatus == NU_SUCCESS )
        {
            /* 整个修改过程成功后更新ram区的通道额定值 */
            if( i == 1 )
            {
                pChannel = LS_CHANNEL_TABLE_MEMORY;  /* 通道数据表内存首地址 */
                if( ChlGpNum == 0 )
                {
                    pChannel += ChlNumber;
                    /* 一次额定值 */
                    pChannel-> chl_primary_rate = fRate1[0];
                    /* 二次额定值 */
                    pChannel-> chl_secondary_rate = fRate2[0];

                    i = FUNDAMENTAL + HARMONIC_2ND + HARMONIC_3RD + HARMONIC_5TH
                        + HFUNDAMENTAL + HARMONIC_7TH + CHL_PEAKVALUE;
                    if(( pChannel -> chl_type & i ) != 0 )
                    {
                        pChannel->chl_rate_coefficient = pChannel->chl_secondary_rate / pChannel->chl_calculate_rate;
                    }
                }
                else
                {
                    for( k = 0; k < LS_CHANNEL_TABLE_NUMBER; k ++)
                    {
                        /* 一次额定值 */
                        pChannel -> chl_primary_rate = fRate1[k];
                        /* 二次额定值 */
                        pChannel -> chl_secondary_rate = fRate2[k];

                        i = FUNDAMENTAL + HARMONIC_2ND + HARMONIC_3RD + HARMONIC_5TH
                            + HFUNDAMENTAL + HARMONIC_7TH + CHL_PEAKVALUE;
                        if(( pChannel -> chl_type & i ) != 0 )
                        {
                            pChannel->chl_rate_coefficient =
                                pChannel->chl_secondary_rate / pChannel->chl_calculate_rate;
                        }

                        pChannel ++;
                    }
                }
                Result = LS_RESULT_TRUE;
            }
        }
        else
        {
            break;
        }
    }

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;
    * pDst ++ = 8;                                                   // fill report length.
    * pDst ++ = IEC103_TI_M_E5;                                      // fill command type.
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                  // fill cpu number.
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 );  // fill serial number: byte 1
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 );  // fill serial number: byte 2
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 );  // fill channel group number.
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 );  // fill channel group number.
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 7 ) & LS_DATA_MASKS8 );  // fill protect channel ID.
    * pDst ++ = ( UNSIGNED_CHAR )Result;                             // fill result.

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_E6
/* 调装置零漂刻度及零漂值 */
INT LSC_Load_Channel_Excursion( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             k;
    UNSIGNED        uTemp;
    UNSIGNED_CHAR   * p,* pSrc,* pDst;
    INT             ChlNumber;
    PRO_CHANNEL     * pChannel;
    float           Variable;
    INT             Counter;
    INT             Length;
    INT             SampleOffset;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    Counter = *( pSrc + 5 ) & LS_DATA_MASKS8;  /* 通道个数 */
    pDst += 6; /* 通道ID */
    /* 获取采样偏移位置 */
    SampleOffset = CSMV_GET_SAMPLE_OFFSET;
    for( k = 0; k < Counter; k ++)
    {
        ChlNumber = *( pSrc + 6 + k ) & LS_DATA_MASKS8; /* 通道ID */
        pChannel = LS_CHANNEL_TABLE_MEMORY;  /* 通道数据表内存首地址 */
        pChannel += ChlNumber;
        if( ChlNumber < LS_CHANNEL_TABLE_NUMBER )
        {
            Variable = LS_CALCULATE_DC( ChlNumber, LS_DC_COEFFICIENT, SampleOffset );
            if(( pChannel -> chl_type & CHL_POLARITY ) != 0 )
            {
                Variable = ( float )( 0.0 - Variable );
            }
        }
        else
        {
            Variable = 0.0;
        }
        * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 + k ) & LS_DATA_MASKS8 ); /* 通道ID */
        /* 零漂刻度 */
        uTemp = LS_FLOAT_TO_IEEE( pChannel -> chl_channel_excursion );
        p = ( UNSIGNED_CHAR *)& uTemp;
        LS_WORD_TO_BYTE( pDst, p, LS_DATA_WIDTH );
        pDst += LS_DATA_WIDTH;
        /* 零漂量值 */
        uTemp = LS_FLOAT_TO_IEEE( Variable );
        p = ( UNSIGNED_CHAR *)& uTemp;
        LS_WORD_TO_BYTE( pDst, p, LS_DATA_WIDTH );
        pDst += LS_DATA_WIDTH;
    }

    pDst = ( UNSIGNED_CHAR *)Dst;
    Length = 2 * LS_DATA_WIDTH * Counter + Counter;             /* 命令报文长度 */
    * pDst ++ = ( UNSIGNED_CHAR )( Length + 5 );                /* 报文长度 */
    * pDst ++ = IEC103_TI_M_E6;                                 /* 命令类型 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );             /* CPU号码 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 ); /* 序列号低字节 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 ); /* 序列号高字节 */
    * pDst ++ = ( UNSIGNED_CHAR )Counter;                       /* 通道个数 */

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_E7
/* 修改装置零漂 */
INT LSC_Change_Channel_Excursion( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             k;
    UNSIGNED_CHAR   * pSrc,* pDst, * p;
    PRO_CHANNEL     * pChannel;
    float           variable;
    INT             iStatus, iResult;
    float           excursion[LS_MAX_CHL_NUMBER];
    TB_CFB          CondField;
    TB_TCB          * pTable;
    INT             iNumber;
    INT             iStartNum;
    INT             iType;
    INT             Chl0;
    UNSIGNED        uExcursion;
    INT             SampleOffset;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    iResult = LS_RESULT_NULL;
    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    /* 通道缓冲区 */
    pChannel = LS_CHANNEL_TABLE_MEMORY;  /* 通道数据表内存首地址 */
    /* 查找数据表便于找到数据表的个数 */
    pTable = CTBC_FindTable( TAB_CHANNEL_ID );
    /* 判断其通道零漂是否在允许整定的范围内的标志 */
    iStatus = NU_TRUE;
    Chl0 = *( pSrc + 5 ) & LS_DATA_MASKS8; /* 通道ID */
    /* 获取采样偏移位置 */
    SampleOffset = CSMV_GET_SAMPLE_OFFSET;
    /* 修改通道零漂 */
    if( Chl0 != 0xFF )
    {
        /* 修改单个通道零漂 */
        if( Chl0 < LS_CHANNEL_TABLE_NUMBER )
        {
            uExcursion = 0;
            for( k = 0; k < LS_DATA_WIDTH; k ++)
            {
                uExcursion += (*( pSrc + 6 + k ) & LS_DATA_MASKS8 ) << ( k * 8 ); // channel excursion value.
            }
            excursion[0] = LS_IEEE_TO_FLOAT( uExcursion );
            /* 判断零漂范围 */
            if(( excursion[0] < LS_MIN_EXCURSION ) || ( excursion[0] > LS_MAX_EXCURSION ))
            {
                /* 零漂不在可以整定的范围内直接返回失败 */
                iStatus = NU_FALSE;
            }
        }
        /* 起始序号 */
        iStartNum = Chl0;
        /* 修改个数 */
        iNumber = 1;
        /* 修改类型 */
        iType = SETTING_ONE_ENABLE;
    }
    else
    {
        /* 自动校正通道零漂 */
        /* 获取通道缓冲区 */
        pChannel = LS_CHANNEL_TABLE_MEMORY;  /* 通道数据表内存首地址 */
        if( LS_CHANNEL_TABLE_NUMBER > 0 )
        {
            iStatus = NU_TRUE;
            for( k = 0; k < LS_CHANNEL_TABLE_NUMBER; k ++)
            {
                if(( pChannel -> chl_type & CHL_EXCURSION ) != 0 )
                {
                    variable = LS_CALCULATE_DC( pChannel->chl_Num, LS_DC_COEFFICIENT, SampleOffset );
                    if(( pChannel -> chl_type & CHL_POLARITY ) != 0 )
                    {
                        variable = ( float )( 0.0 - variable );
                    }
                    variable += pChannel->chl_channel_excursion;
                    if(( variable >= LS_MIN_EXCURSION ) && ( variable <= LS_MAX_EXCURSION ))
                    {
                        excursion[k] = variable;
                    }
                    else
                    {
                        /* 零漂不在可以整定的范围内直接返回失败 */
                        iStatus = NU_FALSE;
                        break;
                    }
                }
                else
                {
                    excursion[k] = 0.0;
                }
                pChannel ++;
            }
        }
        else
        {
            iStatus = NU_FALSE;
        }
        /* 起始序号 */
        iStartNum = 0;
        /* 修改个数 */
        iNumber = pTable->DataNumber;
        /* 修改类型 */
        iType = SETTING_ALL_ENABLE;
    }

    if( iStatus == NU_TRUE )
    {
        /* 设置操作字段 */
        CondField.HandleIndex = CHANNELN_CHLEXCU_INDEX;
        /* 条件个数(修改全部通道) */
        CondField.CondNumber = 0;
        p = ( UNSIGNED_CHAR *)Dst;
        p --;
        * p = *( pSrc - 1 ) & LC_VAR_CONTROL_BITS;

        /* 修改量值并将量值写入ee中 */
        iStatus = LSC_Change_TableInf( TAB_CHANNEL_ID, iStartNum, iNumber,
            & CondField, excursion, iType, p );
        if( iStatus == NU_SUCCESS )
        {
            /* 整个修改过程成功后更新ram区的通道零漂 */
            pChannel = LS_CHANNEL_TABLE_MEMORY;  /* 通道数据表内存首地址 */
            /* 自动校正零漂 */
            if( Chl0 == 0xFF )
            {
                /* 自动校正通道零漂 */
                for( k = 0; k < LS_CHANNEL_TABLE_NUMBER; k ++)
                {
                    ( pChannel + k )->chl_channel_excursion = excursion[k];
                }
            }
            else
            {
                /* 修改单个通道零漂 */
                ( pChannel + Chl0 )->chl_channel_excursion = excursion[0];
            }
            iResult = LS_RESULT_TRUE;
        }
    }
    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = 6;                                                   // fill report length.
    * pDst ++ = IEC103_TI_M_E7;                                      // fill command type.
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                  // fill cpu number.
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 );  // fill serial number: byte 1
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 );  // fill serial number: byte 2
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 );  // fill protect channle ID.
    * pDst ++ = ( UNSIGNED_CHAR )iResult;                            // fill result.

    return( COM_DATA_FULL );
}
#endif
/*=======================================================================================*/
#if SWITCH_IEC103_TI_M_EA
/* 调通道相角 */
INT LSC_Load_Channel_Phase( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED        uTemp;
    UNSIGNED_CHAR   * p,* pSrc,* pDst;
    INT             Chl0;
    PRO_CHANNEL     * pChannel;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    * pDst ++ = ( UNSIGNED_CHAR )( LS_DATA_WIDTH + 5 );         // fill report length.
    * pDst ++ = IEC103_TI_M_EA;                                 // fill command type.
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );             // fill cpu number.
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 ); // fill serial number: byte 1
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 ); // fill serial number: byte 2
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 );  // fill protect channle ID.

    Chl0 = *( pSrc + 5 ) & LS_DATA_MASKS8;  /* channel ID. */
    pChannel = LS_CHANNEL_TABLE_MEMORY;     /* 通道数据表内存首地址 */
    pChannel += Chl0;
    uTemp = LS_FLOAT_TO_IEEE( pChannel->chl_channel_phase );
    p = ( UNSIGNED_CHAR *)& uTemp;
    LS_WORD_TO_BYTE( pDst, p, LS_DATA_WIDTH );
    pDst += LS_DATA_WIDTH;                                  // fill channel angle.

    return( COM_DATA_FULL );
}
#endif
/*=======================================================================================*/
#if SWITCH_IEC103_TI_M_E9
/* 修改通道相位误差 */
INT LSC_Change_Channel_Phase( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             k, iResult;
    UNSIGNED        uPhase;
    UNSIGNED_CHAR   * pSrc,* pDst, * p;
    INT             Chl0;
    PRO_CHANNEL     * pChannel;
    float           Variable;
    TB_CFB          CondField;
    INT             iStatus;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    iResult = LS_RESULT_NULL;
    pSrc = ( UNSIGNED_CHAR *)Src;
    Chl0 = *( pSrc + 5 ) & LS_DATA_MASKS8;  // channel ID.
    if( Chl0 < LS_CHANNEL_TABLE_NUMBER )
    {
        pChannel = LS_CHANNEL_TABLE_MEMORY;  /* 通道数据表内存首地址 */
        pChannel += Chl0;

        uPhase = 0;
        for( k = 0; k < LS_DATA_WIDTH; k ++)
        {
            uPhase += (*( pSrc + 6 + k ) & LS_DATA_MASKS8 ) << ( k * 8 ); // channel phase value.
        }

        Variable = LS_IEEE_TO_FLOAT( uPhase );

        if(( Variable >= LS_MIN_PHASE ) && ( Variable <= LS_MAX_PHASE ))
        {
            /* 设置操作字段 */
            CondField.HandleIndex = CHANNELN_PHASEINDEX;
            /* 条件个数(修改全部通道) */
            CondField.CondNumber = 0;
            p = ( UNSIGNED_CHAR *)Dst;
            p --;
            * p = *( pSrc - 1 ) & LC_VAR_CONTROL_BITS;

            /* 修改量值并将量值写入ee中 */
            iStatus = LSC_Change_TableInf( TAB_CHANNEL_ID, Chl0, 1,
                & CondField, & Variable, SETTING_ONE_ENABLE, p );

            /* 修改成功,更新ram区的相角误差 */
            if( iStatus == NU_SUCCESS )
            {
                pChannel -> chl_channel_phase = Variable;
                iResult = LS_RESULT_TRUE;
            }
        }
    }

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;
    * pDst ++ = 6;                                                   // fill report length.
    * pDst ++ = IEC103_TI_M_E9;                                      // fill command type.
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                  // fill cpu number.
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 );  // fill serial number: byte 1
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 );  // fill serial number: byte 2
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 );  // fill protect channle ID.
    * pDst ++ = ( UNSIGNED_CHAR )iResult;                            // fill result.

    return( COM_DATA_FULL );
}
#endif

/* 修改数据表信息函数,将选择\编辑\确认放在一个函数中 */
INT LSC_Change_TableInf( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * buffer, INT Type, UNSIGNED_CHAR * Dst )
{
    INT iStatus;
    //--------------------------------------------------------------------------
    /* 选择编辑定值组 */
    iStatus = XJP_DS_Select_Edit_Field_SG( TableIndex, Handle->HandleIndex );
    if( iStatus == NU_SUCCESS )
    {
        /* 设置编辑定值组值 */
        iStatus = XJP_DS_Set_Field_Value_SG( TableIndex, Offset, Number, Handle, buffer );
        if( iStatus == NU_SUCCESS )
        {
            /* 确认编辑定值组值(修改全部通道零漂) */
            iStatus = XJP_DS_Confirm_Field_Value_SG( TableIndex, Type, Offset, Handle, LS_TIME_OUT,
                NU_NULL, Dst );
        }
    }
    return( iStatus );
}

#if SWITCH_IEC103_TI_M_F0
/* 信号复归 */
INT LSC_Reset( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT      i;
    UNSIGNED * pMemory;
    //--------------------------------------------------------------------------
    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( Dst );
    LS_UNUSED_PARAM( Src );
    LS_UNUSED_PARAM( code );
    /*==========================================================*/
    /* 复归继电器出口位内存地址 */
    pMemory = LS_GET_OUTPUT_RESET_MEMORY;

    LS_REPORT_PROTECT();
    /* 启动继电器收回 */
    XJP_IO_Set_Start_Relay( PlatHIOM, 0 );
    /* 清除保持继电器状态位 */
    XJP_IO_Clear_Hold_Relay();
    /* 收回出口位/设置复归继电器动作位 */
    for( i = 0; i < CPC_MAX_OUTPUT_GROUPS; i ++)
    {
        XJP_IO_Set_Output_Relay( PlatHIOM, i,*( pMemory + i ), CIO_EQU );
    }
    LS_REPORT_UNPROTECT();

    /* 复归应用程序标志 */
    XJP_Reset_App_Routine();

    LS_SET_RESET(); //修改为只设置标志？
    /* 使能复归继电器的延时定时器 */
    NU_Control_Timer(& Timer_ResetRelay_CB, NU_ENABLE_TIMER );

    return( COM_DATA_EMPTY );
}
#endif

//
// reset relay timer main function
//
VOID LSC_RRelayTimerRoutine( UNSIGNED argc )
{
    INT       j;
    UNSIGNED  uStatus;
    UNSIGNED * pMemory;
    //--------------------------------------------------------------------------
    /* 复归继电器出口位内存地址 */
    pMemory = LS_GET_OUTPUT_RESET_MEMORY;

    LS_UNUSED_PARAM( argc );

    /* 出口收回时要进行数据保护 */
    LS_REPORT_PROTECT();

    /* 收回复归继电器的出口位 */
    for( j = 0; j < CPC_MAX_OUTPUT_GROUPS; j ++)
    {
        uStatus = *( pMemory + j );
        XJP_IO_Set_Output_Relay( PlatHIOM, j,(~ uStatus ), CIO_AND );
    }
    LS_REPORT_UNPROTECT();
    /* 收回复归继电器的延时定时器 */
    NU_Control_Timer(& Timer_ResetRelay_CB, NU_DISABLE_TIMER );
}

#if SWITCH_IEC103_TI_M_F1
/* 召唤开入量状态及GPS脉冲计数 */
INT LSC_Load_Input_Status( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT             k;
    INT             Counter;
    INT             ActualCounter;
    INT             GroupNumber;
    UNSIGNED        uBits, uQuality;
    UNSIGNED_CHAR   * p,* pSrc,* pDst;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;

    Counter = ( INT )(*( pSrc + 5 ) & LS_DATA_MASKS8 ); /* 开入量的组数 */
    if( Counter > LS_INPUT_GROUP_NUMBER )
    {
        /* 召唤的开入量组数大于平台支持的最大开入量组数时,按最大组数上送 */
        Counter = LS_INPUT_GROUP_NUMBER;
    }

    pDst += 6; /* 填写开入量组号的起始偏移 */
    ActualCounter = 0;
    for( k = 0; k < Counter; k ++)
    {
        GroupNumber = ( INT )(*( pSrc + 6 + k ) & LS_DATA_MASKS8 ); /* 开入量组号 */
        if( GroupNumber < LS_INPUT_GROUP_NUMBER )
        {
            /* 只处理有效的开入量组数 */
            ActualCounter ++;
            uBits = 0;
            uQuality = 0;
            * pDst ++ = GroupNumber;
            /* 读取开入量状态及品质因素 */
            CINC_ReadInputQuality( GroupNumber, & uBits, & uQuality );
            /* 开入量状态 */
            p = ( UNSIGNED_CHAR *)& uBits;
            LS_WORD_TO_BYTE( pDst, p, LS_DATA_WIDTH );
            pDst += LS_DATA_WIDTH;
            /* 开入量品质因素 */
            p = ( UNSIGNED_CHAR *)& uQuality;
            LS_WORD_TO_BYTE( pDst, p, LS_DATA_WIDTH );
            pDst += LS_DATA_WIDTH;
        }
    }

    uBits = LS_GET_GPS_PULSE_NUMBER; /* 读取GPS脉冲计数 */
    p = ( UNSIGNED_CHAR *)& uBits;
    LS_WORD_TO_BYTE( pDst, p, LS_DATA_WIDTH );
    pDst += LS_DATA_WIDTH;

    pDst = ( UNSIGNED_CHAR *)Dst;
    /* (1+4):开入量组号+开入量状态;uNumber:开入量组号 */
    * pDst ++ = ( UNSIGNED_CHAR )(( 1 + 4 + 4 )* ActualCounter + 9 ); // fill report length.
    * pDst ++ = IEC103_TI_M_F1;                             // fill command type.
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );         // fill cpu number.
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 ); // fill serial number: byte 1
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 ); // fill serial number: byte 2
    * pDst ++ = ( UNSIGNED_CHAR ) ActualCounter;                    // fill input group number.

    return( COM_DATA_FULL );
}
#endif

#if SWITCH_IEC103_TI_M_F7
/* 读内存数据 */
INT LSC_Read_Memory( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR   * pSrc,* pDst, * pTemp;
    UNSIGNED        uAddress;
    INT             k, width, number, iType;
    INT             iStatus;
    UNSIGNED           * pAddr32;
    unsigned short int * pAddr16, uShorData;
    UNSIGNED_CHAR      * pAddr8;
    //--------------------------------------------------------------------------

    LS_UNUSED_PARAM( pChl );
    LS_UNUSED_PARAM( code );

    pSrc = ( UNSIGNED_CHAR *)Src;
    pDst = ( UNSIGNED_CHAR *)Dst;
    uAddress = 0;
    iStatus = LS_RESULT_NULL;
    for( k = 0; k < LS_DATA_WIDTH; k ++)
    {
        uAddress += (*( pSrc + 4 + k ) & LS_DATA_MASKS8 ) << ( k * 8 );  /* 起始地址 */
    }
    width = (*( pSrc + 3 ) & LS_DATA_MASKS8 );   /* 数据宽度 */
    number = (*( pSrc + 8 ) & LS_DATA_MASKS8 );  /* 数据个数 */
    iType = (*( pSrc + 9 ) & LS_DATA_MASKS8 );   /* 读/写选择 */
    if( iType == 0x02 )  /* 写内存 */
    {
        /* 写内存数据 */
        /* 判断地址空间是否有效,仅RAM区允许写操作 */
        if(( uAddress >= LS_RAM_START_ADDR ) && ( uAddress < LS_RAM_END_ADDR ))
        {
            iStatus = LS_RESULT_TRUE;
            /* 源数据地址 */
            pTemp = ( pSrc + 10 );
            /* 判断数据宽度 */
            switch( width )
            {
                /* 读取数据宽度为8 */
            case 1:
                pAddr8 = ( UNSIGNED_CHAR *)uAddress;
                memcpy( pAddr8, pTemp, number );
                break;
                /* 读取数据宽度为16 */
            case 2:
                pAddr16 = ( unsigned short int *)uAddress;
                for( k = 0; k < number; k ++)
                {
                    uShorData = (* pTemp ++ & CPC_DATA_MASKS8 );
                    uShorData |= ((* pTemp ++ & CPC_DATA_MASKS8 ) << CPC_DATA_BITS8 );
                    * pAddr16 ++ = uShorData;
                }
                break;
                /* 读取数据宽度为32 */
            case 4:
                pAddr32 = ( UNSIGNED *)uAddress;
                for( k = 0; k < number; k ++)
                {
                    LS_BYTE_TO_WORD( pAddr32, pTemp, width );
                    pTemp += width;
                    pAddr32 ++;
                }
                break;
            default:
                /* 数据宽度不在允许的范围之内返回失败 */
                iStatus = LS_RESULT_NULL;
                break;
            }
        }
        * pDst ++ = 10;                                             /* 链路用户数据长度 */
    }
    else
    {
        /* 读内存数据 */
        * pDst ++ = ( UNSIGNED_CHAR )( number * width + 10 );        /* 链路用户数据长度 */
    }

    * pDst ++ = IEC103_TI_M_F7;                                      /* 类别标识 */
    * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );                  /* CPU号码 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 3 ) & LS_DATA_MASKS8 );  /* 数据宽度 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 4 ) & LS_DATA_MASKS8 );  /* 起始地址字节1 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 5 ) & LS_DATA_MASKS8 );  /* 起始地址字节2 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 6 ) & LS_DATA_MASKS8 );  /* 起始地址字节3 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 7 ) & LS_DATA_MASKS8 );  /* 起始地址字节4 */
    * pDst ++ = ( UNSIGNED_CHAR )(*( pSrc + 8 ) & LS_DATA_MASKS8 );  /* 数据个数 */
    if( iType == 0x01 )
    {
        iStatus = LS_RESULT_TRUE;
        /* 存储内存数据的偏移量 */
        pDst += 2;

        switch( width )
        {
            /* 读取数据宽度为8 */
        case 1:
            pAddr8 = ( UNSIGNED_CHAR *)uAddress;
            memcpy( pDst, pAddr8, number );
            break;
            /* 读取数据宽度为16 */
        case 2:
            pAddr16 = ( unsigned short int *)uAddress;
            for( k = 0; k < number; k ++)
            {
                uShorData = * pAddr16 ++;
                * pDst ++ = ( uShorData & CPC_DATA_MASKS8 );
                * pDst ++ = ( ( uShorData >> CPC_DATA_BITS8 ) & CPC_DATA_MASKS8 );
            }
            break;
            /* 读取数据宽度为32 */
        case 4:
            pAddr32 = ( UNSIGNED *)uAddress;
            for( k = 0; k < number; k ++)
            {
                LS_WORD_TO_BYTE( pDst, pAddr32, width );
                pDst += width;
                pAddr32 ++;
            }
            break;
        default:
            /* 数据宽度不在允许的范围之内返回失败 */
            iStatus = LS_RESULT_NULL;
            break;
        }
    }

    pDst = ( UNSIGNED_CHAR *)Dst;
    pDst += 9;                                      /* 执行结果位置偏移 */
    * pDst ++ = iStatus;                            /* 执行正确 */
    * pDst ++ = (*( pSrc + 9 ) & LS_DATA_MASKS8 );  /* 读/写选择 */

    return( COM_DATA_FULL );
}
#endif
