
#include "plat/net_inc/lc_extr.h"
#include "plat/net_inc/ls_extr.h"

/* �����ڵ� */
GW_REQUEST_CB * LCC_CB_CreateActiveCB( GW_CHANNEL * pChl , INT size, GW_UCHAR sesn , GW_UCHAR sect,
    GW_UCHAR priority , GW_UCHAR dest_port, GW_CB_ACK ack_callback, GW_ULONG timeout )
{
    GW_REQUEST_CB * pCB;
    GW_REQUEST_CB * pEntry;

    /* �ռ����� */
    pCB = ( GW_REQUEST_CB *)gwmem_alloc( size );
    if ( pCB )
    {
        pCB->dest_port  = dest_port;
        pCB->priority   = priority;
        pCB->sesn       = sesn;
        pCB->sect       = sect;
        pCB->entry      = 0;
        pCB->timeout    = timeout;
        pCB->ack_callback   = ack_callback;
        pEntry = ( GW_REQUEST_CB *)xjdlist_getFirst(& pChl->chl_suspend_List );
        while ( pEntry )
        {
            if ( pEntry->priority > priority )
            {
                pCB->start_timeout  = TMD_System_Clock;
                xjdlist_insertEntryBefore(& pChl->chl_suspend_List,
                    & pEntry->node,& pCB->node );
                break;
            }
            pEntry = ( GW_REQUEST_CB *)xjdlist_getAfter(
                & pChl->chl_suspend_List,& pEntry->node );
        }
        if ( pEntry == GW_NULL )
        {
            pCB->start_timeout  = TMD_System_Clock;
            xjdlist_addEntry(& pChl->chl_suspend_List,& pCB->node );
        }
    }
    return pCB;
}

/* ��ȡ�ڵ� */
GW_REQUEST_CB * LCC_CB_GetActiveAckCB( GW_CHANNEL * pChl )
{
    return (( GW_REQUEST_CB *)xjdlist_getFirst(& pChl->chl_suspend_List ));
}

/* �ͷŽڵ� */
VOID LCC_CB_freeAckCB( GW_CHANNEL * pChl, VOID * pCB )
{
    GW_REQUEST_CB * pAck;
    pAck = ( GW_REQUEST_CB *)pCB;
    xjdlist_removeEntry(& pChl->chl_suspend_List,& pAck->node );
    gwmem_free( pCB );
    return;
}

/* �ڵ�ִ�к��� */
INT LCC_CB_AckBlockIO( UNSIGNED_CHAR * dst, GW_CHANNEL   * pChl, VOID  * pCB )
{
    INT status;
    INT i;
    INT actual_size;
    GW_BLOCK_IO_CB  * pIO_CB;
    UNSIGNED_CHAR   * src;
    pIO_CB  = ( GW_BLOCK_IO_CB *)pCB;
    src = pIO_CB->data;

    /* ִ������ */
    status = LCC_CB_ProcessBlockIO_CB( pChl, dst, src, pIO_CB->head.dest_port );
    if (!( status & COM_DATA_TIMEOUT ))
    {
        if ( status & COM_DATA_FULL )
        {
            actual_size = * dst;
            for ( i = 0 ; i < actual_size ; i ++)
            {
                /* ����ֵΪ���ĳ���,Ŀ�ĵ�ַ���������ʹ���ʼ��д(�������LCC_MessM_SelectFun) */
                *( dst + i )    = *( dst + i + 1 );
            }
        }
        else
        {
            /* ��������� */
            actual_size = 0;
            *( dst + actual_size ++) = IEC103_TI_M_B7;
            *( dst + actual_size ++) = LC_LINK_ADDRESS;
            *( dst + actual_size ++) = *( src + 1 );  /* �������� */
            *( dst + actual_size ++) = LC_CMD_RET_EMPTY;
        }
        /* �ͷ�����ڵ� */
        LCC_CB_freeAckCB( pChl, pCB );
        return actual_size;
    }
    return 0;
}

UNSIGNED TestCount = 0;
/* ע����Ҫ��ʱ�䴦�������ڵ� */
INT LCC_CB_ProcessBlockIO( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port )
{
    UNSIGNED_CHAR   * dst = ( UNSIGNED_CHAR *)p_dst;
    UNSIGNED_CHAR   * src = ( UNSIGNED_CHAR *)p_src;
    INT status = COM_DATA_EMPTY;

    switch( *( src + 1 ) )
    {
        /* ����¼� */
    case IEC103_TI_M_C4:
    case IEC103_TI_M_C9: /* ������ϼ�¼���� */
    case IEC103_TI_M_C1: /* �ٻ��¼���Ϣ */
    case IEC103_TI_M_C2: /* �ٻ��¼������� */
    case IEC103_TI_M_C3: /* �ٻ��¼����� */
        /* ������ע�ᵽ��̨��������� */
        LCC_Run_RegcBackGroundComm( pChl, dst, src, dst_port,*( src + 1 ), LCC_CB_ProcessBlockIO_CB );
        TestCount ++;
        status = COM_DATA_EMPTY;
        break;
    default :
        /* ִ������ */
        status = LCC_CB_ProcessBlockIO_CB( pChl, dst, src, dst_port );
        /* ������ִ�г�ʱ,������ע�ᵽ�ڵ��� */
        if ( status &  COM_DATA_TIMEOUT )
        {
            /* ������ע�ᵽ��̨��������� */
            LCC_Run_RegcBackGroundComm( pChl, dst, src, dst_port,*( src + 1 ), LCC_CB_ProcessBlockIO_CB );
            TestCount ++;
            status = COM_DATA_EMPTY;
        }
        break;
    }
    return status;
}

/* ִ������ */
INT LCC_CB_ProcessBlockIO_CB( GW_CHANNEL * pChl, GW_UCHAR * dst, GW_UCHAR * src, GW_UCHAR dst_port )
{
    INT status;
    status = COM_DATA_EMPTY;
    /* �ж��������� */
    switch(*( src + 1 ))
    {
    case IEC103_TI_M_A7:
#if SWITCH_IEC103_TI_M_A7
        /* ���������ֶε�ֵ */
        status = LSC_Set_Config_Values( pChl, dst, src, dst_port );
#endif
        break;
    case IEC103_TI_M_A8:
#if SWITCH_IEC103_TI_M_A8
        /* ѡ��༭�ֶ� */
        status = LSC_Slecet_Edit_SG( pChl, dst, src, dst_port );
#endif
        break;
    case IEC103_TI_M_A9:
#if SWITCH_IEC103_TI_M_A9
        /* ���ñ༭��ֵ���ֵ */
        status = LSC_Set_Edit_SGV( pChl, dst, src, dst_port );
#endif
        break;
    case IEC103_TI_M_AA:
#if SWITCH_IEC103_TI_M_AA
        /* ȷ�ϱ༭��ֵ���ֵ */
        status = LSC_Conform_Edit_SGV( pChl, dst, src, dst_port );
#endif
        break;
    case IEC103_TI_M_AF:
#if SWITCH_IEC103_TI_M_AF
        /* ���ö�������ֶε�ֵ */
        status = LSC_Set_Batch_Config_Values( pChl, dst, src, dst_port );
#endif
        break;
    case IEC103_TI_M_B0:
#if SWITCH_IEC103_TI_M_B0
        /* �����������ֶε�ֵ */
        status = LSC_Save_Batch_Config_Values( pChl, dst, src, dst_port );
#endif
        break;
    case IEC103_TI_M_C1:
#if SWITCH_IEC103_TI_M_C1
        /* �ٻ��¼���Ϣ */
        status = LSC_Load_Event_Inf( pChl, dst, src, dst_port );
#endif
        break;
    case IEC103_TI_M_C2:
#if SWITCH_IEC103_TI_M_C2
        /* �ٻ��¼������� */
        status = LSC_Load_Event_Serial( pChl, dst, src, dst_port );
#endif
        break;
    case IEC103_TI_M_C3:
#if SWITCH_IEC103_TI_M_C3
        /* �ٻ��¼����� */
        status = LSC_Load_Event_Data( pChl, dst, src, dst_port );
#endif
        break;
    case IEC103_TI_M_C4:
#if SWITCH_IEC103_TI_M_C4
        /* ����¼����� */
        status = LSC_Clear_Alarm_Event( pChl, dst, src, dst_port );
#endif
        break;
    case IEC103_TI_M_C5:
#if SWITCH_IEC103_TI_M_C5
        /* �ٻ�������Ϣ */
        status = LSC_Load_Report_Inf( pChl, dst, src, dst_port );
#endif
        break;
    case IEC103_TI_M_C6:
#if SWITCH_IEC103_TI_M_C6
        /* �ٻ����������� */
        status = LSC_Load_Report_FaultSerl( pChl, dst, src, dst_port );
#endif
        break;
    case IEC103_TI_M_C7:
#if SWITCH_IEC103_TI_M_C7
        /* �ٻ������������ */
        status = LSC_Load_Report_Serial( pChl, dst, src, dst_port );
#endif
        break;
    case IEC103_TI_M_C8:
#if SWITCH_IEC103_TI_M_C8
        /* �ٻ��������� */
        status = LSC_Load_Report_Data( pChl, dst, src, dst_port );
#endif
        break;
    case IEC103_TI_M_C9:
#if SWITCH_IEC103_TI_M_C9
        /* ������ϼ�¼���� */
        status = LSC_Clear_FaultData( pChl, dst, src, dst_port );
#endif
        break;
    case IEC103_TI_M_CB:
#if SWITCH_IEC103_TI_M_CB
        /* �ٻ�¼����� */
        status = LSC_Load_Wave_ID( pChl, dst, src, dst_port );
#endif
        break;
    case IEC103_TI_M_CC:
#if SWITCH_IEC103_TI_M_CC
        /* �ٻ�¼��ʱ�� */
        status = LSC_Load_Wave_Time( pChl, dst, src, dst_port );
#endif
        break;
    case IEC103_TI_M_CD:
#if SWITCH_IEC103_TI_M_CD
        /* �ٻ�������Ϣ */
        status = LSC_Load_Wave_Inf( pChl, dst, src, dst_port );
#endif
        break;
    case IEC103_TI_M_CE:
#if SWITCH_IEC103_TI_M_CE
        /* �ٻ����������� */
        status = LSC_Load_Wave_FaultSerl( pChl, dst, src, dst_port );
#endif
        break;
    default:
        break;
    }
    return status;
}
