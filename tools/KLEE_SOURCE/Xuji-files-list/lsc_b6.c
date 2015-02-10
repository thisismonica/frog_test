
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "plat/net_inc/rcc_defs.h"
#include "plat/net_inc/lsc_b6.h"

B6_ManagFun LSC_B6_ManagFun = NU_NULL;

/* B6Ѳ�����ȼ� */
static INT LSC_B6_Proirity_Table[LSC_B6_PROIRITY_BUFF_SIZE];
/* Ӧ�ú��� */
static UNSIGNED LSC_B6_FunAddr[LSC_B6_MAX_EVENT] =
{
    ( UNSIGNED )& LSC_B6_CommEvent,
    ( UNSIGNED )& LSC_B6_FaultReport,
    ( UNSIGNED )& LSC_B6_RamEvent,
    ( UNSIGNED )& LSC_B6_FaultRet,
    ( UNSIGNED )& LSC_B6_WaveIndexChange,
    ( UNSIGNED )& LSC_B6_TableFieldSudden,
};

/* ͻ�������¼������������ */
UNSIGNED Dbg_PlatB6SadCounters[3] = {0};
INT LSC_B6_CommEvent( VOID * Dst )
{
    INT      iStatus;
    UNSIGNED Size;
    //--------------------------------------------------------------------------
    /* ��ʼ�����ؽ�� */
    iStatus = COM_DATA_EMPTY;
    /* 0:ͻ������,��̨ң��,��̬���� */
    Dbg_PlatB6SadCounters[0] ++;/*�ܼ�����*/
    if( XJP_Queue_Find( CommEventHandle, & Size ) == NU_TRUE )
    {
        /* ��ӿ�����ͻ�䱨�ĺ��� */
        RCC_Pack_BreakEvent( Dst );
        /* ���ؽ�� */
        Dbg_PlatB6SadCounters[1] ++;/*�ɹ�������*/
        iStatus = COM_DATA_FULL;
    }
    else
    {
        Dbg_PlatB6SadCounters[2] ++;/*ʧ�ܼ�����*/
    }
    /* return */
    return( iStatus );
}

/* ���ϱ��淢���仯,���͹��ϱ������ */
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
    /* ��ʼ�����ؽ�� */
    iStatus = COM_DATA_EMPTY;
    /* �ϵ�ʱ,�Ƚ�������ż����������ų�ʼ��. */
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
        /* ��ȡ������� */
        if( pReportHandle->GetReadyInformation( pReportHandle, & FaultNumber, & Serial, & TimeTemp ) == NU_TRUE )
        {
            /* �жϹ�������Ƿ����仯 */
            if(( iReportSerial != Serial )
                || ( iReportFaultNumber != FaultNumber ))
            {
                iReportSerial = Serial;
                iReportFaultNumber = FaultNumber;

                * pDst ++ = 7;                                        // fill report length.
                * pDst ++ = IEC103_TI_M_B6;                           // fill command type.
                * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );       // fill cpu number.
                * pDst ++ = LSC_B6_TYPE_REPORT;                       // ͻ�䱨������.

                * pDst ++ = ( UNSIGNED_CHAR )( iReportSerial & CPC_DATA_MASKS8 );
                * pDst ++ = ( UNSIGNED_CHAR )(( iReportSerial >> CPC_BYTE_BITS ) & CPC_DATA_MASKS8 );
                * pDst ++ = ( UNSIGNED_CHAR )( iReportFaultNumber & CPC_DATA_MASKS8 );
                * pDst ++ = ( UNSIGNED_CHAR )(( iReportFaultNumber >> CPC_BYTE_BITS ) & CPC_DATA_MASKS8 );
                /* ���ؽ�� */
                iStatus = COM_DATA_FULL;
            }
        }
        else
        {
            /* �����������ʱ��Ҫ�õ����о� */
            iReportSerial = 0;
            iReportFaultNumber = 0;
        }
    }
    else
    {
        /* �����������ʱ��Ҫ�õ����о� */
        iReportSerial = 0;
        iReportFaultNumber = 0;
    }

    /* return */
    return( iStatus );
}

/* ͻ���¼����� */
INT LSC_B6_RamEvent( VOID * Dst )
{
    INT           iStatus;
    UNSIGNED      Size;
    CEM_RAD       * pEvent, Event;
    UNSIGNED_CHAR * pDst;
    //--------------------------------------------------------------------------
    pDst = ( UNSIGNED_CHAR *)Dst;
    /* ��ʼ�����ؽ�� */
    iStatus = COM_DATA_EMPTY;
    /* ��⻺�������Ƿ����¼� */
    if( XJP_Queue_Find( RamEventHandle, & Size ) == NU_TRUE )
    {
        LC_REPORT_PROTECT();
        XJP_Queue_Read( RamEventHandle , & Event );
        LC_REPORT_UNPROTECT();

        pEvent = & Event;
        * pDst ++ = 13 + LS_ALARM_NAME;                       // fill report length.
        * pDst ++ = IEC103_TI_M_B6;                           // fill command type.
        * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );       // fill cpu number.
        * pDst ++ = ( UNSIGNED_CHAR )LSC_B6_TYPE_EVENT;       // ͻ���¼�����.
        * pDst ++ = ( UNSIGNED_CHAR )( pEvent->Flag );        // fill report type.
        * pDst ++ = ( UNSIGNED_CHAR )( pEvent->Index & CPC_DATA_MASKS8 ); // ��д�¼����.
        * pDst ++ = ( UNSIGNED_CHAR )(( pEvent->Index >> CPC_BYTE_BITS ) & CPC_DATA_MASKS8 ); // ��д�¼����.
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
        /* ���ؽ�� */
        iStatus = COM_DATA_FULL;
    }
    /* return */
    return( iStatus );
}

/* ���Ϸ��� */
INT LSC_B6_FaultRet( VOID * Dst )
{
    INT      iStatus;
    UNSIGNED Size;
    //--------------------------------------------------------------------------
    /* ��ʼ�����ؽ�� */
    iStatus = COM_DATA_EMPTY;
    if( XJP_Queue_Find( CommFltRetHandle, & Size ) == NU_TRUE )
    {
        RCC_Pack_FaultRet( Dst );
        /* ���ؽ�� */
        iStatus = COM_DATA_FULL;
    }
    /* return */
    return( iStatus );
}

/* B6����Ĺ����� */
INT LSC_B6_MessageManagFun( VOID * Dst )
{
    static INT iNumber = 0;  /* ��ʼ�����ȼ������� */
    INT        iStatus;
    INT        i;
    //--------------------------------------------------------------------------
    /* ��ʼ�����ؽ�� */
    iStatus = COM_DATA_EMPTY;
    /* ��ȡ��ǰ��ִ�к��� */
    LSC_B6_ManagFun = ( B6_ManagFun )LSC_B6_FunAddr[LSC_B6_Proirity_Table[iNumber % LSC_B6_PROIRITY_BUFF_SIZE]];
    /* �������ȼ������� */
    iNumber = (( iNumber + 1 ) % LSC_B6_PROIRITY_BUFF_SIZE );
    /* ִ�е�ǰ���� */
    iStatus = LSC_B6_ManagFun( Dst );
    /* �ж��Ƿ��ȡ������ */
    if( iStatus != COM_DATA_FULL )
    {
        /* ������ڵ�ǰִ��λ�õĺ���û������,���ͷ��ʼ�����Ƿ������� */
        for( i = 0; i < LSC_B6_MAX_EVENT; i ++)
        {
            /* ��ȡ��ǰ��ִ�к��� */
            LSC_B6_ManagFun = ( B6_ManagFun )LSC_B6_FunAddr[i];
            /* ִ�е�ǰ���� */
            iStatus = LSC_B6_ManagFun( Dst );
            /* �ж��Ƿ��ȡ������ */
            if( iStatus == COM_DATA_FULL )
            {
                /* ������������������� */
                break;
            }
        }
    }
    /* return */
    return( iStatus );
}

/* ¼����ŷ����仯 */
INT LSC_B6_WaveIndexChange( VOID * Dst )
{
    INT             iStatus;
    UNSIGNED        Serial, FaultNumber;
    static UNSIGNED iSerial = 0, iFaultNumber = 0, iFlag = 0;
    UNSIGNED_CHAR * pDst;
    //--------------------------------------------------------------------------
    pDst = ( UNSIGNED_CHAR *)Dst;
    /* ��ʼ�����ؽ�� */
    iStatus = COM_DATA_EMPTY;
    /* �ϵ�ʱ,�Ƚ�������ų�ʼ��. */
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

    /* ��ȡ������� */
    if( CWave.GetSerial(& CWave,& Serial ) == NU_TRUE )
    {
        /* �жϹ�������Ƿ����仯 */
        if( iSerial != Serial )
        {
            if( CWave.GetFaultNumber(& CWave,& FaultNumber ) == NU_TRUE )
            {
                iSerial = Serial;
                iFaultNumber = FaultNumber;

                * pDst ++ = 7;                                        // fill report length.
                * pDst ++ = IEC103_TI_M_B6;                           // fill command type.
                * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );       // fill cpu number.
                * pDst ++ = LSC_B6_TYPE_WAVE;                         // ͻ��¼������.

                * pDst ++ = ( UNSIGNED_CHAR )( iSerial & CPC_DATA_MASKS8 );
                * pDst ++ = ( UNSIGNED_CHAR )(( iSerial >> CPC_BYTE_BITS ) & CPC_DATA_MASKS8 );
                * pDst ++ = ( UNSIGNED_CHAR )( iFaultNumber & CPC_DATA_MASKS8 );
                * pDst ++ = ( UNSIGNED_CHAR )(( iFaultNumber >> CPC_BYTE_BITS ) & CPC_DATA_MASKS8 );
                /* ���ؽ�� */
                iStatus = COM_DATA_FULL;
            }
        }
    }
    else
    {
        /* �����������ʱ��Ҫ�õ����о� */
        iSerial = 0;
        iFaultNumber = 0;
    }
    /* return */
    return( iStatus );
}

/* ���ȼ���������ʼ������ */
VOID LSC_B6_ProirityInit( void )
{
    INT i;
    for( i = 0; i < LSC_B6_PROIRITY_BUFF_SIZE; i ++)
    {
        LSC_B6_Proirity_Table[i] = ( i % LSC_B6_MAX_EVENT );
    }
}

/* ���ݱ�ָ���ֶ�ͻ�䱨�� */
INT LSC_B6_TableFieldSudden( VOID * Dst )
{
    UNSIGNED_CHAR uTempBuffer[LC_MAX_FRAME_SIZE];
    INT iStatus;
    INT iStatus1;
    UNSIGNED ActSize;
    UNSIGNED_CHAR * pDst;
    //--------------------------------------------------------------------------
    pDst = ( UNSIGNED_CHAR *)Dst;
    /* ��ʼ�����ؽ�� */
    iStatus = COM_DATA_EMPTY;
    /* �������� */
    iStatus1 = XJP_ReceiveFromDataQueue( PRO_TAB_DATA_SUDDEN, 0, uTempBuffer, LC_MAX_FRAME_SIZE,
        & ActSize, NU_NO_SUSPEND );
    /* �ж��Ƿ������ݼ������Ƿ�ȫ */
    if(( iStatus1 == NU_SUCCESS ) && ( ActSize <= LC_MAX_FRAME_SIZE ))
    {
        * pDst ++ = ( UNSIGNED_CHAR )( 3 + ActSize );           // fill report length.
        * pDst ++ = IEC103_TI_M_B6;                           // fill command type.
        * pDst ++ = ( UNSIGNED_CHAR )( LS_CPU_NUMBER );       // fill cpu number.
        * pDst ++ = LSC_B6_TYPE_TABFIELD_SUDDEN;              // ���ݱ�ָ���ֶ�ͻ����Ϣ.
        memcpy( pDst, uTempBuffer, ActSize );
        /* ���ؽ�� */
        iStatus = COM_DATA_FULL;
    }
    return( iStatus );
}
