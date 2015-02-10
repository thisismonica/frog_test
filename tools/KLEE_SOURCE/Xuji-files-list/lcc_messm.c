
#include "plat/net_inc/lc_extr.h"
#include "plat/net_inc/rw_extr.h"
#include "plat/net_inc/ls_extr.h"
#include "plat/net_inc/rcc_defs.h"
#include "plat/es_plat.h"

extern volatile UNSIGNED TMD_System_Clock;
extern NU_EVENT_GROUP   Event_1_CB;  // event2 block
extern NU_EVENT_GROUP   Event_2_CB;  // event2 block

#define LCC_PROIRITY_BUFFER_SIZE 32 /* ���ȼ���������С */
#ifdef WIN32
/* ���������Ƿ����к��� */
extern VOID TASK_Test( INT  iTaskID );
#endif

extern UNSIGNED Task_0_EntryCount;
/* ͨѶ���������� */
VOID LCC_MessM_Main( GW_CHANNEL * pChl )
{
    HWDM hWatchDogId;

    /* �������Ź������������ */
    hWatchDogId = XJP_Watchdog_Create( LC_WATCHDOG_TIME, "PTask0", ( UNSIGNED )NU_Current_Task_Pointer );
    pChl->chl_disturb_order_start   = 0;
    pChl->chl_rx_ok_tick = 0;
    pChl->chl_rx_reset_tick = 0;
    while ( 1 )
    {
        Task_0_EntryCount ++;
#ifdef WIN32
        /* ���������Ƿ����к��� */
        //       TASK_Test( 0 );
#endif
        LCC_MessM_ParseData( pChl );
        NU_Relinquish();
        /* ��λ�����Ӷ��� */
        XJP_Watchdog_Reset( hWatchDogId );
    }
}

/* ���FCB�Ƿ��λ */
INT LCC_MessM_CheckFCB( GW_CHANNEL * pChl, UNSIGNED_CHAR ucFCB )
{
    if ( pChl->chl_FCB & I870FT12_CONTROL_DIR )
    {
        pChl->chl_FCB = ( UNSIGNED_CHAR )( ucFCB & I870FT12_CONTROL_FCB );
        return NU_TRUE;
    }
    if ( ucFCB & I870FT12_CONTROL_FCV )        //FCB bit validate
    {
        ucFCB &= I870FT12_CONTROL_FCB;
        if ( ucFCB ^ ( pChl->chl_FCB & I870FT12_CONTROL_FCB ))
        {
            pChl->chl_FCB = ucFCB;
            return NU_TRUE;
        }
        else
        {
            return NU_FALSE;
        }
    }
    else
    {
        return NU_TRUE;
    }
}

/* ���֡�Ƿ���ȷ */
INT LCC_MessM_CheckFrame( UNSIGNED_CHAR * data, GW_UCHAR phy_size )
{
    INT i;
    INT size;
    UNSIGNED_CHAR uCh;
    size = *( data + I870FT12_INDEX_VAR_LENGTH );
    if  ((* data == I870FT12_SYNC_OCTET_VARIABLE )
        && (*( data + size + I870FT12_INDEX_VAR_CONTROL + 1 )== I870FT12_SYNC_OCTET_END ))
    {
        if ((*( data + I870FT12_INDEX_VAR_ADDRESS ) == I870FT12_BROADCAST_ADDRESS )
            || (*( data + I870FT12_INDEX_VAR_ADDRESS ) == LC_LINK_ADDRESS ))
        {
            if (*( data + I870FT12_INDEX_VAR_CONTROL ) & I870FT12_CONTROL_PRM )
            {
                uCh = 0;
                for ( i  = 0; i < size ; i ++)
                {
                    uCh = ( UNSIGNED_CHAR )( uCh +*( data + i + I870FT12_INDEX_VAR_CONTROL ));
                }
                uCh &= GW_MASKBIT_8;
                if  ( uCh == *( data + size + I870FT12_INDEX_VAR_CONTROL ))
                {
                    return NU_TRUE;
                }
            }
        }
    }
    return NU_FALSE;
}

/* ����У��� */
INT LCC_MessM_CalculateSum( UNSIGNED_CHAR * data )
{
    UNSIGNED_CHAR uCh;
    INT i;
    INT size;
    size = *( data + I870FT12_INDEX_VAR_LENGTH );
    uCh  = 0;
    for ( i = 0; i < size ; i ++)
    {
        uCh += *( data + i + I870FT12_INDEX_VAR_CONTROL );
    }
    uCh &= GW_MASKBIT_8;
    *( data + size + I870FT12_INDEX_VAR_CONTROL ) = uCh;
    *( data + size + I870FT12_INDEX_VAR_CONTROL + 1 ) = I870FT12_SYNC_OCTET_END;
    /* ����ֵΪ�����ܳ��� */
    return ( size + LC_VARIABLE_BASE_LENGTH );
}

/* ����ȷ�� */
INT LCC_MessM_PackConfirm( UNSIGNED_CHAR * dst, UNSIGNED_CHAR port,
    UNSIGNED_CHAR type, UNSIGNED_CHAR code )
{
    UNSIGNED_CHAR * p;
    p   = dst;
    * dst ++= I870FT12_SYNC_OCTET_VARIABLE;
    * dst ++= LC_VARIABLE_BASE_LENGTH;
    * dst ++= LC_VARIABLE_BASE_LENGTH;
    * dst ++= I870FT12_SYNC_OCTET_VARIABLE;
    * dst ++= port;
    * dst ++= ( UNSIGNED_CHAR )LC_LINK_ADDRESS;
    * dst ++= IEC103_TI_M_B7;
    * dst ++= ( UNSIGNED_CHAR )LC_LINK_ADDRESS;
    * dst ++= type;
    * dst ++= code;
    * dst ++= 0;
    * dst ++= I870FT12_SYNC_OCTET_END;
    return LCC_MessM_CalculateSum( p );
}

/* ��֯֡ͷ */
INT LCC_MessM_PackHead( INT actual_size, UNSIGNED_CHAR * dst, UNSIGNED_CHAR dst_port )
{
    UNSIGNED_CHAR * p;
    p   = dst;
    * p ++= I870FT12_SYNC_OCTET_VARIABLE;
    /* 2:������+��ַ�� */
    * p ++= ( UNSIGNED_CHAR )( actual_size + 2 );
    /* 2:������+��ַ�� */
    * p ++= ( UNSIGNED_CHAR )( actual_size + 2 );
    * p ++= I870FT12_SYNC_OCTET_VARIABLE;
    * p ++= dst_port;
    * p ++= ( UNSIGNED_CHAR )LC_LINK_ADDRESS;
    p  += actual_size;
    * p ++ = 0;
    * p ++ = I870FT12_SYNC_OCTET_END;
    return LCC_MessM_CalculateSum( dst );
}

/* ���ݽ��� */
VOID LCC_MessM_ParseData( GW_CHANNEL * pChl )
{
    INT tx_size;
    INT rx_size;
    GW_UCHAR    ucFCB;
    GW_UCHAR    ucADDR;

    /* �����ж��Ƿ��н������� */
    rx_size = LCC_Link_Read( pChl, pChl->chl_rx_buff );
    LCC_Link_CheckTimer( pChl );
    if( rx_size > 0 )
    {
        pChl->chl_rx_ok_tick    = TMD_System_Clock;
        pChl->chl_rx_reset_tick = TMD_System_Clock;
        if (! pChl->chl_rx_ok )
        {
            pChl->chl_rx_ok = GW_TRUE;
            NU_Set_Events(& Event_1_CB,( UNSIGNED )(~ CPC_EVENT1_ERR_COMMU ), NU_AND );
        }
        tx_size = 0;
        ucFCB   = pChl->chl_rx_buff[I870FT12_INDEX_VAR_CONTROL];
        ucADDR  = pChl->chl_rx_buff[I870FT12_INDEX_VAR_ADDRESS];
        /* ���FCB�Ƿ���ȷ */
        if ( LCC_MessM_CheckFCB( pChl, ucFCB ))
        {
            /* ������������(���ջ�����) */
            LCC_MessM_ParseCommand( pChl, pChl->chl_rx_buff, rx_size );
            /* �ж��Ƿ�Ϊ�㲥���� */
            if ( ucADDR != I870FT12_BROADCAST_ADDRESS )
            {
                /* ����Ƿ�������Ҫ���� */
                tx_size = LCC_MessM_PackData( pChl, pChl->chl_tx_buff );
            }
        }
        else
        {
            /* �ж��Ƿ�Ϊ�㲥���� */
            if ( ucADDR != I870FT12_BROADCAST_ADDRESS )
            {
                /* ��ⷢ�ͻ��������Ƿ���������Ҫ����(FCB�Ƿ���ȷ) */
                if ( LCC_MessM_CheckFrame( pChl->chl_tx_buff, 0 ))
                {
                    tx_size = pChl->chl_tx_buff[I870FT12_INDEX_VAR_LENGTH] + LC_VARIABLE_BASE_LENGTH;
                }
                else
                {
                    /* ����Ƿ�������Ҫ���� */
                    tx_size = LCC_MessM_PackData( pChl, pChl->chl_tx_buff );
                }
            }
        }
        /* �ж��Ƿ���������Ҫ���� */
        if ( tx_size > 0 )
        {
            /* ���÷������ݺ��� */
            LCC_Link_Write( pChl, pChl->chl_tx_buff, tx_size );
        }
    }
}

/* �������ݴ������ */
INT LCC_MessM_PackData( GW_CHANNEL * pChl, GW_UCHAR * dst )
{
    INT tx_size = 0;
    UNSIGNED    start_tick;
    INT packge_num = 0;

    start_tick  = TMD_System_Clock;
    /*�Ǵ������,��������*/
    if (( pChl->chl_temp_size > 0 )
        && ( pChl->chl_temp_size <= LC_MAX_FRAME_SIZE ))
    {
        /* ��lon���汾�ŵ������� */
        if (( pChl->chl_temp_buff[I870FT12_INDEX_VAR_ADDR1_ASDU] == I3DEF_TYPE_PRO_MMIVER )
            ||( pChl->chl_temp_buff[I870FT12_INDEX_VAR_ADDR1_ASDU] == I3DEF_TYPE_MMI_LONVER )
            ||( pChl->chl_temp_size >= LC_MAX_FRAME_DATA_SIZE ))
        {
            tx_size = pChl->chl_temp_size;
            memcpy( dst, pChl->chl_temp_buff, tx_size );
            pChl->chl_temp_size = 0;
            return tx_size;
        }
    }

    /* ������������ */
    *( dst + I870FT12_INDEX_VAR_ADDR1_ASDU ) = IEC103_TI_M_80;
    *( dst + I870FT12_INDEX_VAR_ADDR1_ASDU + 1 ) = LC_LINK_ADDRESS;   /* װ��cpu�� */
    *( dst + I870FT12_INDEX_VAR_ADDR1_ASDU + 2 ) = 0;   /* ��������� */
    tx_size = I870FT12_INDEX_VAR_ADDR1_ASDU + 3;        /* ���ĳ��� */
    if (( pChl->chl_temp_size > 0 )
        && ( pChl->chl_temp_size < LC_MAX_FRAME_DATA_SIZE ))
    {
        memcpy( dst + tx_size, pChl->chl_temp_buff, pChl->chl_temp_size );
        tx_size += pChl->chl_temp_size;
        pChl->chl_temp_size = 0;
        packge_num ++;
    }
    while (( tx_size < LC_MAX_FRAME_DATA_SIZE )
        && (( TMD_System_Clock - start_tick )< LC_TIMEOUT_TICK ))
    {
        if ( ( pChl->chl_temp_size = LCC_MessM_PollingData( pChl, pChl->chl_temp_buff )) != 0 )
        {
            if (( pChl->chl_temp_size + tx_size ) > LC_MAX_FRAME_DATA_SIZE )
            {
                break;
            }
            else
            {
                memcpy( dst + tx_size, pChl->chl_temp_buff, pChl->chl_temp_size );
                tx_size += pChl->chl_temp_size;
                packge_num ++;
                pChl->chl_temp_size = 0;
            }
        }
        else
        {
            break;
        }
    }
    *( dst + I870FT12_INDEX_VAR_ADDR1_ASDU + 2 ) = packge_num;  /* ��������� */
    return LCC_MessM_PackHead( tx_size - I870FT12_INDEX_VAR_ADDR1_ASDU, dst, LC_VAR_CONTROL_BITS );
}

/* ������������ */
INT LCC_MessM_ParseCommand( GW_CHANNEL   * pChl, GW_UCHAR * src, GW_USHORT rx_size )
{
    INT ret = 0;
    INT port;
    GW_UCHAR    type;
    GW_UCHAR   * dst;
    GW_COMMAND_CALLBACK_NODE * pNode;

    UNSIGNED    actual_size;
    UNSIGNED_CHAR ucADDR;
    UNSIGNED    uStatus;

    dst     = pChl->chl_tx_buff;
    port    = *( src + I870FT12_INDEX_VAR_CONTROL ) & LC_VAR_CONTROL_BITS;
    ucADDR  = src[I870FT12_INDEX_VAR_ADDRESS];
    type = *( src + I870FT12_INDEX_VAR_ADDR1_ASDU );
    /* 2:������+��ַ��,�õ����ı���ĳ��� */
    *( src + I870FT12_INDEX_VAR_ADDRESS ) = ( UNSIGNED_CHAR )(*( src + I870FT12_INDEX_VAR_LENGTH )- 2 );
    if ( type != IEC103_TI_M_B6 )
    {
        pChl->chl_request_sequence  = LC_REQUEST_SEQUENCE_ANY;
        pNode   = LCC_MessM_LookupNode( pChl, type );
        if ( pNode != GW_NULL )
        {
#if     LC_STAT
            pNode->call_count ++;
#endif
            ret = pNode->Callback( pChl,
                ( VOID *)( dst + I870FT12_INDEX_VAR_ADDRESS ),
                ( VOID *)( src + I870FT12_INDEX_VAR_ADDRESS ),
                ( UNSIGNED_CHAR )port );
            if (( type == I3DEF_TYPE_PRO_MMIVER )
                || ( type == I3DEF_TYPE_MMI_LONVER ))
            {
                actual_size = *( dst + I870FT12_INDEX_VAR_ADDRESS );
                pChl->chl_temp_size = LCC_MessM_PackHead(( INT ) actual_size, dst, ( UNSIGNED_CHAR )port );
                memcpy( pChl->chl_temp_buff, dst, pChl->chl_temp_size );
                return 0;
            }
            if ( ret & COM_DATA_TIMEOUT )
            {
                actual_size = LCC_MessM_PackConfirm( dst, ( UNSIGNED_CHAR )port,
                    ( UNSIGNED_CHAR )(*( src + I870FT12_INDEX_VAR_ADDR1_ASDU )),
                    ( UNSIGNED_CHAR )LC_CMD_RET_BUSY );
                LCC_Link_Send_To_Queue(& LCC_Link_Primary_QUE, dst, actual_size, NU_NO_SUSPEND, sizeof( UNSIGNED_CHAR ));
            }
            else if ( ret & COM_DATA_ERROR )
            {
                actual_size = LCC_MessM_PackConfirm( dst, ( UNSIGNED_CHAR )port,
                    ( UNSIGNED_CHAR )(*( src + I870FT12_INDEX_VAR_ADDR1_ASDU )),
                    ( UNSIGNED_CHAR )LC_CMD_RET_EMPTY );
                LCC_Link_Send_To_Queue(& LCC_Link_Primary_QUE, dst, actual_size, NU_NO_SUSPEND, sizeof( UNSIGNED_CHAR ));
            }
            else if ( ret & COM_DATA_FULL )
            {
                actual_size = *( dst + I870FT12_INDEX_VAR_ADDRESS );
                LCC_MessM_PackHead(( INT )actual_size, dst,
                    ( UNSIGNED_CHAR )(*( src + I870FT12_INDEX_VAR_CONTROL ) & LC_VAR_CONTROL_BITS ));
                if ( ret & COM_DATA_FIRST )
                {
                    uStatus = LCC_Link_Send_To_Queue(& LCC_Link_Primary_QUE, dst,
                        ( actual_size + LC_LINK_SIZE ), NU_NO_SUSPEND, sizeof( UNSIGNED_CHAR ));
                }
                else
                {
                    uStatus = LCC_Link_Send_To_Queue(& LCC_Link_Second_QUE, dst,
                        ( actual_size + LC_LINK_SIZE ), NU_NO_SUSPEND, sizeof( UNSIGNED_CHAR ));
                }
            }
        }
        else
        {
            actual_size = LCC_MessM_PackConfirm( dst,
                ( UNSIGNED_CHAR )port, type,
                ( UNSIGNED_CHAR )LC_CMD_RET_UNDEF );
            LCC_Link_Send_To_Queue(& LCC_Link_Primary_QUE, dst, actual_size, NU_NO_SUSPEND, sizeof( UNSIGNED_CHAR ));
        }
    }
    else
    {
        /* ��¼���ȼ�(b6�����Ѳ�����ȼ�) */
        pChl->chl_request_sequence  = *( src + I870FT12_INDEX_VAR_ADDR1_ASDU + 2 );
    }
    return ret;
}

/* �����������ݴ����� */
INT LCC_MessM_SelectFun( GW_CHANNEL * pChl, GW_UCHAR sequence, UNSIGNED_CHAR * dst )
{
    UNSIGNED      actual_size;
    GW_REQUEST_CB * pCB;
    UNSIGNED_CHAR dst_port;
    UNSIGNED      uStatus;
    UNSIGNED_CHAR uTemp[LC_MAX_MSG_SIZE * sizeof( UNSIGNED )];

    /* ���ݲ�ͬ�����ȼ�Ѳ�첻ͬ�����ݻ����� */
    switch( sequence )
    {
    case 1:  /* Ѳ�����"First" */
#if     LC_STAT
        pChl->stat_select_count[1] ++;                      /* Ѳ������� */
        pChl->stat_select_last_tick[1] = TMD_System_Clock;  /* Ѳ��ʱ�� */
#endif
        uStatus = LCC_Link_Receive_From_Queue(& LCC_Link_Primary_QUE, uTemp, LC_MAX_MSG_SIZE,
            & actual_size, NU_NO_SUSPEND, sizeof( UNSIGNED_CHAR ) );
        if( uStatus == NU_SUCCESS )
        {
            /* �ӱ����л�ȡʵ�ʱ��ĳ��� */
            actual_size = uTemp[I870FT12_INDEX_VAR_LENGTH] + LC_VARIABLE_BASE_LENGTH;
            /* �жϱ��ĳ����Ƿ�������Χ֮�� */
            if( actual_size > LC_MAX_FRAME_SIZE )
            {
                actual_size = 0;
            }
            /* ��װ��������ʱ,���еĵ�ַ����32λ�� */
            memcpy( dst, uTemp, actual_size );
            return actual_size;
        }
        break;
    case 2:    /* Ѳ��͵��¼�(B6����) */
#if     LC_STAT
        pChl->stat_select_count[2] ++;                      /* Ѳ������� */
        pChl->stat_select_last_tick[2] = TMD_System_Clock;  /* Ѳ��ʱ�� */
#endif
        if (( actual_size = LC_BCallCheckBackTaskMess(( dst + I870FT12_INDEX_VAR_ADDR1_ASDU ),& dst_port )) > 0 )
        {
            return LCC_MessM_PackHead( actual_size, dst, dst_port );
        }
        if ( LSC_Report_Event( pChl,( dst + I870FT12_INDEX_VAR_ADDRESS ),
            GW_NULL, LC_PORT_TYPE_MENU ) == COM_DATA_FULL )
        {
            actual_size = *( dst + I870FT12_INDEX_VAR_ADDRESS );
            return LCC_MessM_PackHead( actual_size, dst, LC_PORT_TYPE_MENU );
        }
        break;
    case 3:  /* Ѳ�����"Secnod" */
#if     LC_STAT
        pChl->stat_select_count[3] ++;                       /* Ѳ������� */
        pChl->stat_select_last_tick[3] = TMD_System_Clock;   /* Ѳ��ʱ�� */
#endif
        uStatus = LCC_Link_Receive_From_Queue(& LCC_Link_Second_QUE, uTemp, LC_MAX_MSG_SIZE,
            & actual_size, NU_NO_SUSPEND, sizeof( UNSIGNED_CHAR ) );
        if  ( uStatus == NU_SUCCESS )
        {
            /* �ӱ����л�ȡʵ�ʱ��ĳ��� */
            actual_size = uTemp[I870FT12_INDEX_VAR_LENGTH] + LC_VARIABLE_BASE_LENGTH;
            /* �жϱ��ĳ����Ƿ�������Χ֮�� */
            if( actual_size > LC_MAX_FRAME_SIZE )
            {
                actual_size = 0;
            }
            memcpy( dst, uTemp, actual_size );
            return actual_size;
        }
        break;
    case 4:   /* Ѳ��ڵ� */
#if     LC_STAT
        pChl->stat_select_count[4] ++;                      /* Ѳ������� */
        pChl->stat_select_last_tick[4] = TMD_System_Clock;  /* Ѳ��ʱ�� */
#endif
        if ( ( pCB = LCC_CB_GetActiveAckCB( pChl )) != NU_NULL )
        {
            if (( actual_size = pCB->ack_callback(
                ( dst + I870FT12_INDEX_VAR_ADDR1_ASDU ), pChl, pCB ))> 0 )
            {
                return LCC_MessM_PackHead( actual_size & GW_MASKBIT_8, dst, pCB->dest_port );
            }
        }
        break;
    default:
#if     LC_STAT
        pChl->stat_select_count[0] ++;                     /* Ѳ������� */
        pChl->stat_select_last_tick[0] = TMD_System_Clock; /* Ѳ��ʱ�� */
#endif

        if (( actual_size = RCC_Check_Report( pChl,
            ( dst + I870FT12_INDEX_VAR_ADDR1_ASDU ),& dst_port ))> 0 )
        {
            return LCC_MessM_PackHead( actual_size, dst, dst_port );
        }
        break;
    }
    return 0;
}

/* Ѳ�����ȼ� */
INT LCC_Proirity_Table[LCC_PROIRITY_BUFFER_SIZE] =
{
    /* ���ȼ��� */
    0, 1, 0, 2, 0, 3, 0, 4,
    /* ���ȼ��� */
    0, 1, 0, 2, 0, 3, 0, 4,
    /* ���ȼ��� */
    0, 1, 0, 2, 0, 3, 0, 4,
    /* ���ȼ��� */
    0, 1, 0, 2, 0, 3, 0, 4,
};

/* ����Ѳ�캯�� */
INT LCC_MessM_PollingData( GW_CHANNEL * pChl, UNSIGNED_CHAR * dst )
{
    UNSIGNED      actual_size;
    INT i;

    /* ���ȼ������� */
    pChl->chl_acd_times = (( pChl->chl_acd_times + 1 ) % LCC_PROIRITY_BUFFER_SIZE );
    if ( pChl->chl_request_sequence != LC_REQUEST_SEQUENCE_ANY )
    {
        actual_size = LCC_MessM_SelectFun( pChl,
            pChl->chl_request_sequence, dst );
        if ( actual_size > 0 )
        {
            return actual_size;
        }
    }
    if ( pChl->chl_initialize_finish )
    {
        /* 0x1F:���ȼ��������Чλ�� */
        actual_size = LCC_MessM_SelectFun( pChl,
            ( UNSIGNED_CHAR )LCC_Proirity_Table[pChl->chl_acd_times % LCC_PROIRITY_BUFFER_SIZE],
            dst );
        if ( actual_size > 0 )
        {
            return actual_size;
        }
    }

    /* �����ڵ�ǰѲ��λ�õ�û������,���ͷ��ʼ�����Ƿ������� */
    for ( i = 0 ; i < LCC_PRIORITY_MAX ; i ++)
    {
        actual_size = LCC_MessM_SelectFun( pChl, ( UNSIGNED_CHAR )i, dst );
        if ( actual_size > 0 )
        {
            return actual_size;
        }
    }
#if 1
    if( LC_GET_RULES_SELECT == PRO_103_RULES ) /* 103��Լ */
    {
        /* ����Ƿ����Ŷ�������,�����ֱ����д���� */
        if ( LC_FUN_WAVE )
        {
            /* ����Ƿ����Ŷ�������,�����ֱ����д���� */
            actual_size = RWC_Fill_Disturbance( pChl, ( dst + I870FT12_INDEX_VAR_ADDR1_ASDU ));
            if ( actual_size > 0 )
            {
                pChl->chl_disturb_check = NU_TRUE;
                return LCC_MessM_PackHead( actual_size, dst, DEST_TYPE_REMOTE );
            }
        }
    }
#endif
    return 0;
}

/* ��������⺯��,�����Ӧ�����������Ƿ���ִ�к��� */
GW_COMMAND_CALLBACK_NODE * LCC_MessM_LookupNode( GW_CHANNEL   * pChl, GW_UCHAR type )
{
    GW_COMMAND_CALLBACK_NODE * pNode;
    pNode = & pChl->chl_callback_List[type & GW_MASKBIT_8];
    if ( pNode->Callback == NU_NULL )
    {
        return( NU_NULL );
    }
    return pNode;
}

/* Ӧ�ó�����ͨѶ����
����˵��: actual_size ���ݳ���(���Ϊ244)
        : pBuffer ���ݻ�����
        : iDataLevel ���ݼ���(1:Ϊһ������,2:Ϊ��������)
        : iSendDire ���ݷ��ͷ���(��pro_defs.hͷ�ļ����ж���,Ӧ����Ҫƽ̨͸��������Ϣ����)
*/
INT LCC_UserSendComData( INT actual_size, UNSIGNED_CHAR * pBuffer, INT iDataLevel, INT iSendDire )
{
    INT iStatus = NU_UNAVAILABLE;
    UNSIGNED_CHAR uTemp[LC_MAX_FRAME_SIZE];
    /*==========================================================*/
    if( ( actual_size + LC_LINK_SIZE ) <= LC_MAX_FRAME_SIZE )
    {
        memcpy(& uTemp[I870FT12_INDEX_VAR_ADDRESS + 1], pBuffer, actual_size );
        /* �ж����ݷ��ͷ��� */
        if( iSendDire == PRO_DATA_SEND_GWS )
        {
            LCC_MessM_PackHead( actual_size, uTemp, LC_PORT_TYPE_REMOTE3 );
        }
        else
        {
            LCC_MessM_PackHead( actual_size, uTemp, DEST_TYPE_REMOTE );
        }

        if( iDataLevel == PRO_DATA_PRI )
        {
            iStatus = LCC_Link_Send_To_Queue(& LCC_Link_Primary_QUE, uTemp, ( actual_size + LC_LINK_SIZE ), NU_NO_SUSPEND, sizeof( UNSIGNED_CHAR ) );
        }
        else
        {
            iStatus = LCC_Link_Send_To_Queue(& LCC_Link_Second_QUE, uTemp, ( actual_size + LC_LINK_SIZE ), NU_NO_SUSPEND, sizeof( UNSIGNED_CHAR ) );
        }
    }
    return( iStatus );
}
