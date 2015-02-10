/*
* Copyright (c) 2006,许继集团有限公司
* All rights reserved.
*
* 文件名称：rcc.c(report copy core)
* 摘    要：平台内部通信程序
            复制处理报告内核程序
*
* 版本号：
*/
#include <string.h>
#include <math.h>
#include "plat/net_inc/rcc_defs.h"
#include "plat/net_inc/ls_defs.h"
#include "plat/net_inc/lsc_b6.h"
#include "plat/net_inc/lc_bcall.h"
#include "plat/net_inc/ied_extr.h"
#include "plat/net_inc/vld_extr.h"
#include "plat/net_inc/lc_defs.h"
#include "plat/net_inc/lc_extr.h"
#include "plat/inc/mpf_extr.h"
#include "plat/inc/apf_extr.h"
#include "plat/es_plat.h"
/*=============================================*/
/* 内部函数 */
/* 向接口上送突变遥测量值函数 */
static INT RCC_YC_TB_Message( VOID * Dst, GW_SOE * pSoe_temp );
STATUS RCC_Get_Number( GW_RC_CB * pCB, GW_CHANNEL * pChl );
INT RCC_AckReportCopy( UNSIGNED_CHAR * dst, GW_CHANNEL * pChl, VOID  * pCB );
INT RCC_Pack_ASDU_Report( UNSIGNED_CHAR * dst, INT iFaultNumber, UNSIGNED serial,
    UNSIGNED_CHAR cot, UNSIGNED_CHAR SIN );
/*=============================================*/
/* 通讯中事件管理句柄 */
extern HDCM CommEventHandle;
/* 通讯中故障返回管理句柄 */
extern HDCM CommFltRetHandle;
extern NU_PROTECT * pReport_Protect;

/* 填写突变事件(外部提供时间)函数 */
INT RCC_Fill_CheckInfo( INT iTableID, INT CondValue, INT iType,
    INT iSecInf, INT iFunInf, INT iStatus, UNSIGNED q )
{
    INT ret;
    TIME * pTime, RunTime;
    RCC_CheckInfo  CheckInfo;
    UNSIGNED tFrac, tSec, tQuality;

    NU_Protect( pReport_Protect );

    pTime = & RunTime;
    /* 获取装置时间(61850) */
    XJP_TM_Get_Time_61850( pTime,& tSec, & tFrac, & tQuality );
    CheckInfo.TableID = iTableID;
    CheckInfo.CondValue = CondValue;
    CheckInfo.Status = iStatus;
    CheckInfo.FunInf = iFunInf;
    CheckInfo.Sector = iSecInf;
    CheckInfo.Type = iType;
    CheckInfo.Year = pTime->Year;
    CheckInfo.Month = pTime->Month;
    CheckInfo.Day = pTime->Day;
    CheckInfo.Hour = pTime->Hour;
    CheckInfo.Minute = pTime->Minute;
    CheckInfo.Millionsecond = pTime->Millionsecond;
    CheckInfo.q = q;               /* 数据品质因素 */
    CheckInfo.t_sec = tSec;        /* t.sec */
    CheckInfo.t_frac = tFrac;      /* t.frac */
    CheckInfo.t_qflag = tQuality;  /* t.qflag */

    ret = XJP_Queue_Write( CommEventHandle , & CheckInfo );
    NU_Unprotect();

    return ret;
}

/* 填写突变事件(自带时间)函数 */
INT RCC_Fill_CheckInfo_Full( INT iTableID, INT CondValue, INT iType,
    INT iSecInf, INT iFunInf, INT iStatus, TIME * pTime, UNSIGNED q,
    UNSIGNED tSec, UNSIGNED tFrac, UNSIGNED tQuality )
{
    INT ret;
    RCC_CheckInfo  CheckInfo;

    CheckInfo.TableID = iTableID;
    CheckInfo.CondValue = CondValue;
    CheckInfo.Status = iStatus;
    CheckInfo.FunInf = iFunInf;
    CheckInfo.Sector = iSecInf;
    CheckInfo.Type = iType;
    CheckInfo.Year = pTime->Year;
    CheckInfo.Month = pTime->Month;
    CheckInfo.Day = pTime->Day;
    CheckInfo.Hour = pTime->Hour;
    CheckInfo.Minute = pTime->Minute;
    CheckInfo.Millionsecond = pTime->Millionsecond;
    CheckInfo.q = q;               /* 数据品质因素 */
    CheckInfo.t_sec = tSec;        /* t.sec */
    CheckInfo.t_frac = tFrac;      /* t.frac */
    CheckInfo.t_qflag = tQuality;  /* t.qflag */

    ret = XJP_Queue_Write( CommEventHandle , & CheckInfo );

    return ret;
}

/* 填充goose数据函数 */
INT RCC_Fill_Goose( INT iTableID, INT CondValue, INT iType,
    INT iSecInf, INT iFunInf, INT iStatus, TIME * pTime, UNSIGNED q,
    UNSIGNED tSec, UNSIGNED tFrac, UNSIGNED tQuality, UNSIGNED offset )
{
    INT status;
    RCC_CheckInfo CheckInfo;
    RCC_CheckInfo * pCheckInfo;

    pCheckInfo = & CheckInfo;
    pCheckInfo -> TableID = TAB_SOUTPUT_ID;
    pCheckInfo -> CondValue = CondValue;
    pCheckInfo -> Status = iStatus;
    pCheckInfo -> FunInf = iFunInf;
    pCheckInfo -> Sector = iSecInf;
    pCheckInfo -> Type = iType;
    pCheckInfo -> Year = pTime->Year;
    pCheckInfo -> Month = pTime->Month;
    pCheckInfo -> Day = pTime->Day;
    pCheckInfo -> Hour = pTime->Hour;
    pCheckInfo -> Minute = pTime->Minute;
    pCheckInfo -> Millionsecond = pTime->Millionsecond;
    pCheckInfo -> q = q;               /* 数据品质因素 */
    pCheckInfo -> t_sec = tSec;        /* t.sec */
    pCheckInfo -> t_frac = tFrac;      /* t.frac */
    pCheckInfo -> t_qflag = tQuality;  /* t.qflag */
    pCheckInfo -> offset = offset;

    status = MPFC_FillGoose( & CheckInfo );

    return( status );
}

/* 发送goose数据函数 */
INT RCC_Send_Goose( VOID )
{
    INT status;

    status = MPFC_SendGoose();

    return( status );
}

/* 向接口上送突变报文函数 */
VOID RCC_Pack_BreakEvent( VOID * Dst )
{
    UNSIGNED_CHAR * pDst,* p;
    RCC_CheckInfo * pInfo, CheckInfoTemp;
    /*=======================================*/
    pDst = ( UNSIGNED_CHAR *)Dst;

    LC_REPORT_PROTECT();
    XJP_Queue_Read( CommEventHandle , & CheckInfoTemp );
    LC_REPORT_UNPROTECT();

    pInfo = & CheckInfoTemp;

    * pDst ++ = ( 6 * LS_DATA_WIDTH + 2 + 11 ) + 3;    // fill report length.

    * pDst ++ = IEC103_TI_M_B6;                           // fill command type.
    * pDst ++ = ( UNSIGNED_CHAR )( LC_CPU_NUMBER );       // fill cpu number.
    * pDst ++ = LSC_B6_TYPE_DYNAMIC_DATA;                 // 突变上送,后台遥控,动态数据.

    p = ( UNSIGNED_CHAR *)& ( pInfo->TableID );           // 数据表号.
    XJP_CSC_Word_To_Byte( pDst, p, 2 );                   // 数据表号.
    pDst += 2;                                            // 数据表号.

    p = ( UNSIGNED_CHAR *)& ( pInfo->CondValue );         // 条件值.
    XJP_CSC_Word_To_Byte( pDst, p, LS_DATA_WIDTH );
    pDst += LS_DATA_WIDTH;

    * pDst ++ = ( UNSIGNED_CHAR )( pInfo->Type );         // 数据类型
    * pDst ++ = ( UNSIGNED_CHAR )( pInfo->Year );         // 年
    * pDst ++ = ( UNSIGNED_CHAR )( pInfo->Month );        // 月
    * pDst ++ = ( UNSIGNED_CHAR )( pInfo->Day );          // 日
    * pDst ++ = ( UNSIGNED_CHAR )( pInfo->Hour );         // 时
    * pDst ++ = ( UNSIGNED_CHAR )( pInfo->Minute );       // 分

    p = ( UNSIGNED_CHAR *)& ( pInfo->Millionsecond );     // 毫秒.
    XJP_CSC_Word_To_Byte( pDst, p, 2 );                   // 毫秒.
    pDst += 2;                                            // 毫秒.

    * pDst ++ = ( UNSIGNED_CHAR )( pInfo->Sector );       // 扇区

    p = ( UNSIGNED_CHAR *)& ( pInfo->FunInf );            // 信息序号+功能类型.
    XJP_CSC_Word_To_Byte( pDst, p, 2 );                   // 信息序号+功能类型.
    pDst += 2;                                            // 信息序号+功能类型.

    p = ( UNSIGNED_CHAR *)& ( pInfo->Status );            // 状态
    XJP_CSC_Word_To_Byte( pDst, p, LS_DATA_WIDTH );
    pDst += LS_DATA_WIDTH;

    p = ( UNSIGNED_CHAR *)& ( pInfo->q );                 // 数据品质因素.
    XJP_CSC_Word_To_Byte( pDst, p, LS_DATA_WIDTH );
    pDst += LS_DATA_WIDTH;

    p = ( UNSIGNED_CHAR *)& ( pInfo->t_sec );             // 纪元秒.
    XJP_CSC_Word_To_Byte( pDst, p, LS_DATA_WIDTH );
    pDst += LS_DATA_WIDTH;

    p = ( UNSIGNED_CHAR *)& ( pInfo->t_frac );            // 秒的小数.
    XJP_CSC_Word_To_Byte( pDst, p, LS_DATA_WIDTH );
    pDst += LS_DATA_WIDTH;

    p = ( UNSIGNED_CHAR *)& ( pInfo->t_qflag );           // 时间品质因素.
    XJP_CSC_Word_To_Byte( pDst, p, LS_DATA_WIDTH );
    pDst += LS_DATA_WIDTH;
}

/* 填写故障返回数据 */
INT RCC_Fault_Return( INT iTableID, INT CondValue, UNSIGNED uFaultNumber,
    UNSIGNED uRelTime, UNSIGNED uSect, UNSIGNED uFunType, UNSIGNED uINF,
    UNSIGNED uDPI, TIME * pTime, UNSIGNED q, UNSIGNED tSec, UNSIGNED tFrac,
    UNSIGNED tQuality, UNSIGNED phase )
{
    RCC_Q_MESSAGE message;
    RCC_FAULT_RETURN * pRccFaultReturn;
    INT              ret;

    pRccFaultReturn = & message.data.fault;
    pRccFaultReturn->TableID = iTableID;
    pRccFaultReturn->CondValue = CondValue;
    pRccFaultReturn->FaultNumber = uFaultNumber;
    pRccFaultReturn->RelativeTime = uRelTime;
    pRccFaultReturn->Sect = uSect;
    pRccFaultReturn->FunType = uFunType;
    pRccFaultReturn->INF = uINF;
    pRccFaultReturn->DPI = uDPI;
    pRccFaultReturn->Year = pTime->Year;
    pRccFaultReturn->Month = pTime->Month;
    pRccFaultReturn->Day = pTime->Day;
    pRccFaultReturn->Hour = pTime->Hour;
    pRccFaultReturn->Minute = pTime->Minute;
    pRccFaultReturn->Millionsecond = pTime->Millionsecond;
    pRccFaultReturn->q = q;               /* 数据品质因素 */
    pRccFaultReturn->t_sec = tSec;        /* t.sec */
    pRccFaultReturn->t_frac = tFrac;      /* t.frac */
    pRccFaultReturn->t_qflag = tQuality;  /* t.qflag */
    pRccFaultReturn->phase = phase;
    message.Size = ONE_COMM_RSIZE()+ 2;   /* 数据大小、数据类型 */
    message.Type = RCC_Q_TYPE_FAULT_RETURN;

    ret = XJP_Queue_Write( CommFltRetHandle , & message );

    return( ret );
}

/* 填写故障命令报文 */
INT RCC_Fault_Message( VOID * buf )
{
    INT Size;
    INT ret;
    UNSIGNED_CHAR * p;
    RCC_Q_MESSAGE message;

    p = ( UNSIGNED_CHAR *)buf;
    Size = * p;
    Size += 1; /* 数据=长度+报文 */
    memcpy( message.data.buf, p, Size );
    message.Type = RCC_Q_TYPE_COMMAND;
    message.Size = ( Size + sizeof( UNSIGNED ) - 1 )/ sizeof( UNSIGNED );
    message.Size += 2; /* 数据大小、数据类型 */

    ret = XJP_Queue_Write( CommFltRetHandle , & message );
    return( ret );
}

/* 故障返回上送 */
INT RCC_Pack_FaultRet( VOID * Dst )
{
    INT status;
    INT Size;
    RCC_Q_MESSAGE      message;
    RCC_FAULT_RETURN * pRFReturn;
    UNSIGNED_CHAR    * pDst,* p;
    /*=======================================*/
    pDst = ( UNSIGNED_CHAR *)Dst;

    LC_REPORT_PROTECT();
    XJP_Queue_Read( CommFltRetHandle , & message );
    LC_REPORT_UNPROTECT();

    status = NU_UNAVAILABLE;
    if( message.Type == ( UNSIGNED )RCC_Q_TYPE_FAULT_RETURN )   // 队列数据类型
    {
        /* 故障返回报文 */
        pRFReturn = & message.data.fault;                     // 故障返回数据

        * pDst ++ = ( 5 * LS_DATA_WIDTH + 2 * 5 + 9 ) + 3;    // fill report length.
        * pDst ++ = IEC103_TI_M_B6;                           // fill command type.
        * pDst ++ = ( UNSIGNED_CHAR )( LC_CPU_NUMBER );       // fill cpu number.
        * pDst ++ = LSC_B6_TYPE_FAULT_RETURN;                 // 故障返回上送.

        p = ( UNSIGNED_CHAR *)& ( pRFReturn->TableID );       // 数据表号.
        XJP_CSC_Word_To_Byte( pDst, p, 2 );                   // 数据表号.
        pDst += 2;                                            // 数据表号.

        p = ( UNSIGNED_CHAR *)& ( pRFReturn->CondValue );     // 条件值.
        XJP_CSC_Word_To_Byte( pDst, p, LS_DATA_WIDTH );
        pDst += LS_DATA_WIDTH;

        p = ( UNSIGNED_CHAR *)& ( pRFReturn->FaultNumber );   // 故障序号
        XJP_CSC_Word_To_Byte( pDst, p, 2 );                   // 故障序号
        pDst += 2;                                            // 故障序号

        p = ( UNSIGNED_CHAR *)& ( pRFReturn->RelativeTime );  // 相对时间
        XJP_CSC_Word_To_Byte( pDst, p, 2 );                   // 相对时间
        pDst += 2;                                            // 相对时间

        * pDst ++ = ( UNSIGNED_CHAR )( pRFReturn->Sect );     // 扇区号
        * pDst ++ = ( UNSIGNED_CHAR )( pRFReturn->FunType );  // 功能类型(装置功能类型)

        p = ( UNSIGNED_CHAR *)& ( pRFReturn->INF );           // 报告信息序号(功能类型+信息序号)
        XJP_CSC_Word_To_Byte( pDst, p, 2 );                   // 报告信息序号(功能类型+信息序号)
        pDst += 2;                                            // 报告信息序号(功能类型+信息序号)

        * pDst ++ = ( UNSIGNED_CHAR )( pRFReturn->DPI );      // DPI

        * pDst ++ = ( UNSIGNED_CHAR )( pRFReturn->Year );     // 年
        * pDst ++ = ( UNSIGNED_CHAR )( pRFReturn->Month );    // 月
        * pDst ++ = ( UNSIGNED_CHAR )( pRFReturn->Day );      // 日
        * pDst ++ = ( UNSIGNED_CHAR )( pRFReturn->Hour );     // 时
        * pDst ++ = ( UNSIGNED_CHAR )( pRFReturn->Minute );   // 分

        p = ( UNSIGNED_CHAR *)& ( pRFReturn->Millionsecond ); // 毫秒.
        XJP_CSC_Word_To_Byte( pDst, p, 2 );                   // 毫秒.
        pDst += 2;                                            // 毫秒.

        p = ( UNSIGNED_CHAR *)& ( pRFReturn->q );             // 数据品质因素.
        XJP_CSC_Word_To_Byte( pDst, p, LS_DATA_WIDTH );
        pDst += LS_DATA_WIDTH;

        p = ( UNSIGNED_CHAR *)& ( pRFReturn->t_sec );         // 纪元秒.
        XJP_CSC_Word_To_Byte( pDst, p, LS_DATA_WIDTH );
        pDst += LS_DATA_WIDTH;

        p = ( UNSIGNED_CHAR *)& ( pRFReturn->t_frac );        // 秒的小数.
        XJP_CSC_Word_To_Byte( pDst, p, LS_DATA_WIDTH );
        pDst += LS_DATA_WIDTH;

        p = ( UNSIGNED_CHAR *)& ( pRFReturn->t_qflag );       // 时间品质因素.
        XJP_CSC_Word_To_Byte( pDst, p, LS_DATA_WIDTH );
        pDst += LS_DATA_WIDTH;

        * pDst ++ = ( UNSIGNED_CHAR )( pRFReturn->phase & 0xff ); // 故障相别

        status = NU_SUCCESS;
    }
    else if( message.Type == ( UNSIGNED )RCC_Q_TYPE_COMMAND )
    {
        /* 命令报文 */
        p = ( UNSIGNED_CHAR *)(& message.data.buf[0] );
        Size = * p;
        Size += 1; /* 数据=长度+报文 */
        memcpy( pDst, p, Size );

        status = NU_SUCCESS;
    }
    return( status );
}

/* 故障报告检测 */
INT RCC_Check_Report( GW_CHANNEL * pChl, UNSIGNED_CHAR * dst, UNSIGNED_CHAR * dst_port )
{
    INT i;
    INT size;
    INT block_size;
    GW_SOE * pSoe_temp;
    GW_SOE soe_temp;

    size = 0;
    * dst_port = DEST_TYPE_REMOTE;
    pSoe_temp = & soe_temp;
    block_size = LC_BCall_Ret( pChl, dst, dst_port );
    if( block_size )
    {
        size = block_size;
    }
    else if( iedDB_ReadSOE( pSoe_temp, 0 ) == GW_STATUS_SUCCESS )
    {
        switch( pSoe_temp->type )
        {
        case GW_SOE_TYPE_VLD_DATA:
            {
                * dst_port  = pSoe_temp->dest_addr;
                size = VLD_AckData( dst, pSoe_temp );
            }
            break;
        case GW_SOE_TYPE_LOCK_DATA:
            * dst_port  = pSoe_temp->dest_addr;
            *( dst + size ++)   = I3DEF_TYPE_XJ_LOCK_DATA;
            *( dst + size ++)   = 0x81;     /* 可变结构帧限定词 */
            *( dst + size ++)   = I3DEF_COT_GEN_INTERROGATION;
            *( dst + size ++)   = GW_LINK_ADDRESS;
            *( dst + size ++)   = 0;/*fun*/
            *( dst + size ++)   = 0;/*inf*/
            *( dst + size ++)   = ( UNSIGNED_CHAR )( pSoe_temp->data[0].pid & 0xff );/*dest address*/
            *( dst + size ++)   = ( UNSIGNED_CHAR )(( pSoe_temp->data[0].pid >> 8 ) & 0xff );/*dest cpu*/
            *( dst + size ++)   = ( UNSIGNED_CHAR )(( pSoe_temp->data[0].pid >> 16 ) & 0xff );/*source address*/
            *( dst + size ++)   = GW_LINK_ADDRESS;/*source cpu*/

            *( dst + size ++)   = ( UNSIGNED_CHAR )( pSoe_temp->data[1].pid & 0xff );/*type*/
            *( dst + size ++)   = ( UNSIGNED_CHAR )( pSoe_temp->data[2].pid & 0xff );/*number*/
            if( pSoe_temp->num < 4 )   ///
            {
                pSoe_temp->num = 4;     ///
            }
            if( pSoe_temp->num > 24 )   ///
            {
                pSoe_temp->num = 24;     ///
            }
            for ( i = 0 ; i < ( pSoe_temp->num - 4 ) ; i ++)  ///
            {
                /* 获取量值 */
                *( dst + size ++)   = ( UNSIGNED_CHAR )( pSoe_temp->data[3 + i].pid & GW_MASKBIT_8 );
                /* 填写量值 */
                XJP_CSC_Word_To_Byte( ( dst + size ), & pSoe_temp->data[3 + i].data.pvUndef, LS_DATA_WIDTH );
                size += LS_DATA_WIDTH;
            }
            /* 填写报文 */
            XJP_CSC_Word_To_Byte( ( dst + size ), & pSoe_temp->data[pSoe_temp->num - 1].data.pvUndef, 2 );
            size += 2;     ///
            break;
            /* 遥测突变数据通过B6命令上送 */
        case GW_SOE_TYPE_MEASURE:
            {
                if( LC_GET_RULES_SELECT == PRO_103_RULES ) /* 103规约 */
                {
                    if( pSoe_temp->cot == GW_COT_M_OUTBURST )/*突变数据*/
                    {
                        size = RCC_YC_TB_Message( dst, pSoe_temp );
                    }
                    else /*循环数据*/
                    {
                        size = APFC_PackXHYC( dst, pSoe_temp );
                    }
                }
                else /*61850规约*/
                {
                    size = RCC_YC_TB_Message( dst, pSoe_temp );
                }
            }
            break;
        default:
            break;
        }
    }

    return size;
}

/* 向接口上送突变遥测量值函数 */
static INT RCC_YC_TB_Message( VOID * Dst, GW_SOE * pSoe_temp )
{
    INT iSize = 0;
    UNSIGNED_CHAR * pDst,* p;
    INT       i;
    /*=======================================*/
    pDst = ( UNSIGNED_CHAR *)Dst;

    * ( pDst + iSize ++) = IEC103_TI_M_B6;                                  // fill command type.
    * ( pDst + iSize ++) = ( UNSIGNED_CHAR )( LC_CPU_NUMBER );              // fill cpu number.
    * ( pDst + iSize ++) = LSC_B6_TYPE_FCK_YC;                              // 突变遥测数据上送.

    p = ( UNSIGNED_CHAR *)& ( pSoe_temp->TableID );                         // 数据表号.
    XJP_CSC_Word_To_Byte( ( pDst + iSize ), p, 2 );                         // 数据表号.
    iSize += 2;                                                             // 数据表号.

    p = ( UNSIGNED_CHAR *)& ( pSoe_temp->CondValue );                       // 条件值.
    XJP_CSC_Word_To_Byte( ( pDst + iSize ), p, LS_DATA_WIDTH );
    iSize += LS_DATA_WIDTH;

    * ( pDst + iSize ++) = ( UNSIGNED_CHAR )( pSoe_temp->CmdType );         // 数据类型
    * ( pDst + iSize ++) = ( UNSIGNED_CHAR )( pSoe_temp->time.year );       // 年
    * ( pDst + iSize ++) = ( UNSIGNED_CHAR )( pSoe_temp->time.month );      // 月
    * ( pDst + iSize ++) = ( UNSIGNED_CHAR )( pSoe_temp->time.day );        // 日
    * ( pDst + iSize ++) = ( UNSIGNED_CHAR )( pSoe_temp->time.hour );       // 时
    * ( pDst + iSize ++) = ( UNSIGNED_CHAR )( pSoe_temp->time.minute );     // 分

    p = ( UNSIGNED_CHAR *)& ( pSoe_temp->time.millionsecond );              // 毫秒.
    XJP_CSC_Word_To_Byte( ( pDst + iSize ), p, 2 );                         // 毫秒.
    iSize += 2;                                                             // 毫秒.

    * ( pDst + iSize ++) = ( UNSIGNED_CHAR )( pSoe_temp->Sector );          // 扇区
    * ( pDst + iSize ++) = ( UNSIGNED_CHAR )( ( pSoe_temp->data[0] ).pid ); // 信息序号
    * ( pDst + iSize ++) = ( UNSIGNED_CHAR )( pSoe_temp->FuncType );        // 功能类型
    * ( pDst + iSize ++) = ( UNSIGNED_CHAR )( pSoe_temp->num );             // 量值个数

    for( i = 0; i <( pSoe_temp->num ); i ++)
    {
        p = ( UNSIGNED_CHAR *)& (( pSoe_temp->data[i] ).data.pvME ); // 量值.
        XJP_CSC_Word_To_Byte( ( pDst + iSize ), p, LS_DATA_WIDTH );
        iSize += LS_DATA_WIDTH;
    }

    p = ( UNSIGNED_CHAR *)& ( pSoe_temp->q );         // 数据品质因素.
    XJP_CSC_Word_To_Byte( ( pDst + iSize ), p, LS_DATA_WIDTH );
    iSize += LS_DATA_WIDTH;

    p = ( UNSIGNED_CHAR *)& ( pSoe_temp->t_sec );     // 纪元秒.
    XJP_CSC_Word_To_Byte( ( pDst + iSize ), p, LS_DATA_WIDTH );
    iSize += LS_DATA_WIDTH;

    p = ( UNSIGNED_CHAR *)& ( pSoe_temp->t_frac );    // 秒的小数.
    XJP_CSC_Word_To_Byte( ( pDst + iSize ), p, LS_DATA_WIDTH );
    iSize += LS_DATA_WIDTH;

    p = ( UNSIGNED_CHAR *)& ( pSoe_temp->t_qflag );   // 时间品质因素.
    XJP_CSC_Word_To_Byte( ( pDst + iSize ), p, LS_DATA_WIDTH );
    iSize += LS_DATA_WIDTH;

    return( iSize );
}

/* 故障报告复制 */
INT lcApp_ProcessReportCopy( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port )
{
    UNSIGNED_CHAR   * dst    = ( UNSIGNED_CHAR *)p_dst;
    UNSIGNED_CHAR   * src    = ( UNSIGNED_CHAR *)p_src;
    GW_RC_CB  * pRC_CB;

    pRC_CB = ( GW_RC_CB *)LCC_CB_CreateActiveCB( pChl, sizeof( GW_RC_CB ),
        0,*( src + LCC_OFFSET_SECT ), LC_PRIORITY_RC, dst_port,
        RCC_AckReportCopy, LC_TIMEOUT_RC );
    if( pRC_CB )
    {
        pRC_CB->step            = LC_STEP_ACK;
        pRC_CB->rc_index        = *( src + LCC_OFFSET_CONTEXT );
    }
    return COM_DATA_EMPTY;
}

INT RCC_AckReportCopy( UNSIGNED_CHAR * dst, GW_CHANNEL * pChl, VOID  * pCB )
{
    INT        size;
    STATUS      status;
    GW_RC_CB   * pRC_CB = ( GW_RC_CB *)pCB;

    size    = 0;
    if( pRC_CB->step == LC_STEP_ACK )
    {
        status  = RCC_Get_Number( pRC_CB, pChl );

        if( status != NU_TIMEOUT )
        {
            *( dst + size ++)  = I3DEF_TYPE_XJ_TI62;
            *( dst + size ++)  = 0x81;    /* 可变结构限定词 */
            if( status == NU_SUCCESS )
            {
                *( dst + size ++)  = I3DEF_COT_POS_COMMAND_ACK;
            }
            else
            {
                *( dst + size ++)  = I3DEF_COT_NEG_COMMAND_ACK;
            }
            *( dst + size ++)  = pRC_CB->head.sect;
            *( dst + size ++)  = pRC_CB->fun;        /*FUN*/
            *( dst + size ++)  = 0;
            *( dst + size ++)  = ( UNSIGNED_CHAR )pRC_CB->rc_index;
            if( status == NU_SUCCESS )
            {
                pRC_CB->step    = LC_STEP_READ;
                pRC_CB->rc_serial       = 0;
            }
            else
            {
                LCC_CB_freeAckCB( pChl, pCB );
            }
        }
    }
    else if( pRC_CB->step == LC_STEP_READ )
    {
        size    = RCC_Pack_ASDU_Report( dst, pRC_CB->rc_faultNumber,
            pRC_CB->rc_context[pRC_CB->rc_serial],
            I3DEF_COT_VERSION_REPORT_INTG, ( UNSIGNED_CHAR )pRC_CB->rc_index );
        if( size >= 0 )
        {
            pRC_CB->rc_serial ++;
            if( ( INT )pRC_CB->rc_serial >= pRC_CB->rc_count )
            {
                pRC_CB->step    = LC_STEP_END;
            }
        }
        else if( size != NU_TIMEOUT )
        {
            pRC_CB->step    = LC_STEP_END;
        }
    }
    else
    {
        *( dst + size ++)  = I3DEF_TYPE_XJ_TI62;
        *( dst + size ++)  = 0x81;    /* 可变结构限定词 */
        *( dst + size ++)  = I3DEF_COT_TERMINATION_GI;
        *( dst + size ++)  = pRC_CB->head.sect;
        *( dst + size ++)  = pRC_CB->fun;        /*FUN*/
        *( dst + size ++)  = 0;
        *( dst + size ++)  = ( UNSIGNED_CHAR )pRC_CB->rc_index;
        LCC_CB_freeAckCB( pChl, pCB );
    }
    return size;
}

STATUS RCC_Get_Number( GW_RC_CB * pCB, GW_CHANNEL * pChl )
{
    UNSIGNED    tempbuff[CPC_MAX_NVRAM_REPORT_NUMBER];
    STATUS      status = NU_UNAVAILABLE;
    INT         actual_number;
    CREPORT     * pReport;
    INT         i;

    /* 获取报告信息 */
    pReport = CRMC_GetReportHandle( 1 );
    if( pReport != NU_NULL )
    {
        status = pReport->FindFaultNumber( pReport, tempbuff, & actual_number, REPORT_SOE_FLAG, GW_TARG_OP_TIMEOUT );
    }

    if( status == NU_SUCCESS )
    {
        if( pCB->rc_index < ( UNSIGNED )actual_number )
        {
            pCB->rc_faultNumber = tempbuff[actual_number - pCB->rc_index - 1];
            /* 获取故障报告索引号 */
            status = pReport->FindIndexNumber( pReport, & pCB->rc_count, pCB->rc_faultNumber, REPORT_SOE_FLAG, GW_TARG_OP_TIMEOUT );

            if( status == NU_SUCCESS )
            {
                for( i = 1; i <= pCB->rc_count; i ++)
                {
                    pCB->rc_context[i - 1] = i;
                }
            }
        }
        else
        {
            status = NU_END_OF_LOG;
        }
    }
    return status;
}

INT RCC_Pack_ASDU_Report( UNSIGNED_CHAR * dst, INT iFaultNumber, UNSIGNED serial, UNSIGNED_CHAR cot,
    UNSIGNED_CHAR SIN )
{
    STATUS         status;
    INT            size;
    CRM_RFD        RCC_RFD_Buffer;
    CREPORT     * pReport;
    UNSIGNED      uActSerial;
    size    = 0;

    /* 获取报告信息 */
    pReport = CRMC_GetReportHandle( 1 );
    if( pReport != NU_NULL )
    {
        uActSerial = serial;
        if( uActSerial > 0 )
        {
            uActSerial -= 1;
        }

        status = pReport->Read( pReport, iFaultNumber, uActSerial, 1,
            0, sizeof( RCC_RFD_Buffer ),  & RCC_RFD_Buffer, LS_REPORT_TIMEOUT );

        if( status == NU_SUCCESS )
        {
            size = APFC_PackAsduReport( & RCC_RFD_Buffer, dst, cot, SIN );
        }
        else
        {
            size = 0;
        }
    }

    return size;
}
