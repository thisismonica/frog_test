
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "plat/net_inc/rcc_defs.h"
#include "plat/net_inc/lsc_b6.h"

B6_ManagFun LSC_B6_ManagFun = NU_NULL;

/* B6巡检优先级 */
static INT LSC_B6_Proirity_Table[LSC_B6_PROIRITY_BUFF_SIZE];
/* 应用函数 */
static UNSIGNED LSC_B6_FunAddr[LSC_B6_MAX_EVENT] =
{
    ( UNSIGNED )& LSC_B6_CommEvent,
    ( UNSIGNED )& LSC_B6_FaultReport,
    ( UNSIGNED )& LSC_B6_RamEvent,
    ( UNSIGNED )& LSC_B6_FaultRet,
    ( UNSIGNED )& LSC_B6_WaveIndexChange,
    ( UNSIGNED )& LSC_B6_TableFieldSudden,
};

/* 突变上送事件管理与计数器 */
UNSIGNED Dbg_PlatB6SadCounters[3] = {0};
INT LSC_B6_CommEvent( VOID * Dst )
{
    INT      iStatus;
    UNSIGNED Size;
    //--------------------------------------------------------------------------
    /* 初始化返回结果 */
    iStatus = COM_DATA_EMPTY;
    /* 0:突变上送,后台遥控,动态数据 */
    Dbg_PlatB6SadCounters[0] ++;/*总计数器*/
    if( XJP_Queue_Find( CommEventHandle, & Size ) == NU_TRUE )
    {
        /* 向接口上送突变报文函数 */
        RCC_Pack_BreakEvent( Dst );
        /* 返回结果 */
        Dbg_PlatB6SadCounters[1] ++;/*成功计数器*/
        iStatus = COM_DATA_FULL;
    }
    else
    {
        Dbg_PlatB6SadCounters[2] ++;/*失败计数器*/
    }
    /* return */
    return( iStatus );
}

/* 故障报告发生变化,上送故障报告序号 */
INT LSC_B6_FaultReport( VOID * Dst )
{
    INT             iStatus;
    UNSIGNED        Serial, FaultNumber;
    static UNSIGNED iReportSerial = 0, iReportFaultNumber = 0, iFlag = 0;
    UNSIGNED_CHAR * pDst;
    CREPORT * pReportHandle;
    INT     CpuNumber = 1;
    TIME    TimeTemp;
    //--------------------------------------------------------------------------
    pDst = ( UNSIGNED_CHAR *)Dst;
    /* 初始化返回结果 */
    iStatus = COM_DATA_EMPTY;
    /* 上电时,先将报告序号及报告故障序号初始化. */
    if(! iFlag )
    {
        iFlag = 1;
        pReportHandle = CRMC_GetReportHandle( CpuNumber );
        if( pReportHandle )
        {
            if( pReportHandle->GetReadyInformation( pReportHandle, & FaultNumber, & Serial, & TimeTemp ) == NU_TRUE )
            {
                iReportSerial = Serial;
                iReportFaultNumber = FaultNumber;
            }
        }
    }

    pReportHandle = CRMC_GetReportHandle( CpuNumber );
    if( pReportHandle )
    {
        /* 获取故障序号 */
        if( pReportHandle->GetReadyInformation( pReportHandle, & FaultNumber, & Serial, & TimeTemp ) == NU_TRUE )
        {
            /* 判断故障序号是否发生变化 */
            if(( iReportSerial != Serial )
                || ( iReportFaultNumber != FaultNumber ))
            {
                iReportSerial = Serial;
                iReportFaultNumber = FaultNumber;

                * pDst ++ = 7;                                        // fill report length.
                * pDst ++ = IEC103_TI_M_B6;                           // fill command type.
                * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );       // fill cpu number.
                * pDst ++ = LSC_B6_TYPE_REPORT;                       // 突变报告上送.

                * pDst ++ = ( UNSIGNED_CHAR )( iReportSerial & CPC_DATA_MASKS8 );
                * pDst ++ = ( UNSIGNED_CHAR )(( iReportSerial >> CPC_BYTE_BITS ) & CPC_DATA_MASKS8 );
                * pDst ++ = ( UNSIGNED_CHAR )( iReportFaultNumber & CPC_DATA_MASKS8 );
                * pDst ++ = ( UNSIGNED_CHAR )(( iReportFaultNumber >> CPC_BYTE_BITS ) & CPC_DATA_MASKS8 );
                /* 返回结果 */
                iStatus = COM_DATA_FULL;
            }
        }
        else
        {
            /* 清除动作报告时需要用到此判据 */
            iReportSerial = 0;
            iReportFaultNumber = 0;
        }
    }
    else
    {
        /* 清除动作报告时需要用到此判据 */
        iReportSerial = 0;
        iReportFaultNumber = 0;
    }

    /* return */
    return( iStatus );
}

/* 突变事件上送 */
INT LSC_B6_RamEvent( VOID * Dst )
{
    INT           iStatus;
    UNSIGNED      Size;
    CEM_RAD       * pEvent, Event;
    UNSIGNED_CHAR * pDst;
    //--------------------------------------------------------------------------
    pDst = ( UNSIGNED_CHAR *)Dst;
    /* 初始化返回结果 */
    iStatus = COM_DATA_EMPTY;
    /* 检测缓冲区中是否有事件 */
    if( XJP_Queue_Find( RamEventHandle, & Size ) == NU_TRUE )
    {
        LC_REPORT_PROTECT();
        XJP_Queue_Read( RamEventHandle , & Event );
        LC_REPORT_UNPROTECT();

        pEvent = & Event;
        * pDst ++ = 13 + LS_ALARM_NAME;                       // fill report length.
        * pDst ++ = IEC103_TI_M_B6;                           // fill command type.
        * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );       // fill cpu number.
        * pDst ++ = ( UNSIGNED_CHAR )LSC_B6_TYPE_EVENT;       // 突变事件上送.
        * pDst ++ = ( UNSIGNED_CHAR )( pEvent->Flag );        // fill report type.
        * pDst ++ = ( UNSIGNED_CHAR )( pEvent->Index & CPC_DATA_MASKS8 ); // 填写事件序号.
        * pDst ++ = ( UNSIGNED_CHAR )(( pEvent->Index >> CPC_BYTE_BITS ) & CPC_DATA_MASKS8 ); // 填写事件序号.
        memcpy( pDst, pEvent -> Name, CEM_EVENT_NAME );
        pDst += CEM_EVENT_NAME;  // fill report name.
        * pDst ++ = ( UNSIGNED_CHAR )( pEvent->Year );   // fill year.
        * pDst ++ = ( UNSIGNED_CHAR )( pEvent->Month );  // fill month.
        * pDst ++ = ( UNSIGNED_CHAR )( pEvent->Day );    // fill day.
        * pDst ++ = ( UNSIGNED_CHAR )( pEvent->Hour );   // fill hour.
        * pDst ++ = ( UNSIGNED_CHAR )( pEvent->Minute ); // fill minute.
        /* fill millionsecondl. */
        * pDst ++ = ( UNSIGNED_CHAR )( pEvent->Millionsecond & CPC_DATA_MASKS8 );
        /* fill millionsecondh. */
        * pDst ++ = ( UNSIGNED_CHAR )(( pEvent->Millionsecond >> CPC_BYTE_BITS ) & CPC_DATA_MASKS8 );
        /* 返回结果 */
        iStatus = COM_DATA_FULL;
    }
    /* return */
    return( iStatus );
}

/* 故障返回 */
INT LSC_B6_FaultRet( VOID * Dst )
{
    INT      iStatus;
    UNSIGNED Size;
    //--------------------------------------------------------------------------
    /* 初始化返回结果 */
    iStatus = COM_DATA_EMPTY;
    if( XJP_Queue_Find( CommFltRetHandle, & Size ) == NU_TRUE )
    {
        RCC_Pack_FaultRet( Dst );
        /* 返回结果 */
        iStatus = COM_DATA_FULL;
    }
    /* return */
    return( iStatus );
}

/* B6命令报文管理函数 */
INT LSC_B6_MessageManagFun( VOID * Dst )
{
    static INT iNumber = 0;  /* 初始化优先级计数器 */
    INT        iStatus;
    INT        i;
    //--------------------------------------------------------------------------
    /* 初始化返回结果 */
    iStatus = COM_DATA_EMPTY;
    /* 获取当前的执行函数 */
    LSC_B6_ManagFun = ( B6_ManagFun )LSC_B6_FunAddr[LSC_B6_Proirity_Table[iNumber % LSC_B6_PROIRITY_BUFF_SIZE]];
    /* 调整优先级计数器 */
    iNumber = (( iNumber + 1 ) % LSC_B6_PROIRITY_BUFF_SIZE );
    /* 执行当前函数 */
    iStatus = LSC_B6_ManagFun( Dst );
    /* 判断是否读取到数据 */
    if( iStatus != COM_DATA_FULL )
    {
        /* 如果处于当前执行位置的函数没有数据,则从头开始查找是否有数据 */
        for( i = 0; i < LSC_B6_MAX_EVENT; i ++)
        {
            /* 获取当前的执行函数 */
            LSC_B6_ManagFun = ( B6_ManagFun )LSC_B6_FunAddr[i];
            /* 执行当前函数 */
            iStatus = LSC_B6_ManagFun( Dst );
            /* 判断是否读取到数据 */
            if( iStatus == COM_DATA_FULL )
            {
                /* 如果读到数据立即返回 */
                break;
            }
        }
    }
    /* return */
    return( iStatus );
}

/* 录波序号发生变化 */
INT LSC_B6_WaveIndexChange( VOID * Dst )
{
    INT             iStatus;
    UNSIGNED        Serial, FaultNumber;
    static UNSIGNED iSerial = 0, iFaultNumber = 0, iFlag = 0;
    UNSIGNED_CHAR * pDst;
    //--------------------------------------------------------------------------
    pDst = ( UNSIGNED_CHAR *)Dst;
    /* 初始化返回结果 */
    iStatus = COM_DATA_EMPTY;
    /* 上电时,先将故障序号初始化. */
    if(! iFlag )
    {
        iFlag = 1;
        if(( CWave.GetSerial(& CWave,& Serial ) == NU_TRUE )
            && ( CWave.GetFaultNumber(& CWave,& FaultNumber ) == NU_TRUE ))
        {
            iSerial = Serial;
            iFaultNumber = FaultNumber;
        }
    }

    /* 获取故障序号 */
    if( CWave.GetSerial(& CWave,& Serial ) == NU_TRUE )
    {
        /* 判断故障序号是否发生变化 */
        if( iSerial != Serial )
        {
            if( CWave.GetFaultNumber(& CWave,& FaultNumber ) == NU_TRUE )
            {
                iSerial = Serial;
                iFaultNumber = FaultNumber;

                * pDst ++ = 7;                                        // fill report length.
                * pDst ++ = IEC103_TI_M_B6;                           // fill command type.
                * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );       // fill cpu number.
                * pDst ++ = LSC_B6_TYPE_WAVE;                         // 突变录波上送.

                * pDst ++ = ( UNSIGNED_CHAR )( iSerial & CPC_DATA_MASKS8 );
                * pDst ++ = ( UNSIGNED_CHAR )(( iSerial >> CPC_BYTE_BITS ) & CPC_DATA_MASKS8 );
                * pDst ++ = ( UNSIGNED_CHAR )( iFaultNumber & CPC_DATA_MASKS8 );
                * pDst ++ = ( UNSIGNED_CHAR )(( iFaultNumber >> CPC_BYTE_BITS ) & CPC_DATA_MASKS8 );
                /* 返回结果 */
                iStatus = COM_DATA_FULL;
            }
        }
    }
    else
    {
        /* 清除动作报告时需要用到此判据 */
        iSerial = 0;
        iFaultNumber = 0;
    }
    /* return */
    return( iStatus );
}

/* 优先级缓冲区初始化函数 */
VOID LSC_B6_ProirityInit( void )
{
    INT i;
    for( i = 0; i < LSC_B6_PROIRITY_BUFF_SIZE; i ++)
    {
        LSC_B6_Proirity_Table[i] = ( i % LSC_B6_MAX_EVENT );
    }
}

/* 数据表指定字段突变报文 */
INT LSC_B6_TableFieldSudden( VOID * Dst )
{
    UNSIGNED_CHAR uTempBuffer[LC_MAX_FRAME_SIZE];
    INT iStatus;
    INT iStatus1;
    UNSIGNED ActSize;
    UNSIGNED_CHAR * pDst;
    //--------------------------------------------------------------------------
    pDst = ( UNSIGNED_CHAR *)Dst;
    /* 初始化返回结果 */
    iStatus = COM_DATA_EMPTY;
    /* 接收数据 */
    iStatus1 = XJP_ReceiveFromDataQueue( PRO_TAB_DATA_SUDDEN, 0, uTempBuffer, LC_MAX_FRAME_SIZE,
        & ActSize, NU_NO_SUSPEND );
    /* 判断是否有数据及报文是否全 */
    if(( iStatus1 == NU_SUCCESS ) && ( ActSize <= LC_MAX_FRAME_SIZE ))
    {
        * pDst ++ = ( UNSIGNED_CHAR )( 3 + ActSize );           // fill report length.
        * pDst ++ = IEC103_TI_M_B6;                           // fill command type.
        * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );       // fill cpu number.
        * pDst ++ = LSC_B6_TYPE_TABFIELD_SUDDEN;              // 数据表指定字段突变信息.
        memcpy( pDst, uTempBuffer, ActSize );
        /* 返回结果 */
        iStatus = COM_DATA_FULL;
    }
    return( iStatus );
}
