/* ͨ�������Ҫ��ʱ��ִ�еĺ�������ڵ�ַ�Ͳ���ͨ�����д��ݸ����洦������,
���洦������ִ�к�,��ִ�н��ͨ�����д��ݸ�ͨ������,ͨѶ������ִ�н��*/
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "plat/inc/xj_cosc.h"
#include "plat/net_inc/lc_bcall.h"
#include "plat/net_inc/lc_extr.h"

extern   GW_CHANNEL  * LCC_pActive_Channel;
/* ע���̨�ص������������� */
LC_BLOCK_CB LC_BCallFunBuff[LCC_MAXCOMM_TYPE];

/* ��̨������ı�����Ҫ����Ϣ���м������� */
#define LCC_BCALL_BACK_TASK_MESS_BUF_SIZE     300     /* ��̨�����ͨѶ������Ϣ���л������Ĵ�С */
#define LCC_BCALL_BACK_TASK_MAX_LENGTH        60      /* ��̨�����ͨѶ������󳤶�(UNSIGNED����) */
static NU_QUEUE LCC_BCallBackTastQueue;
static UNSIGNED LCC_BCallBackTastQueueBuffer[LCC_BCALL_BACK_TASK_MESS_BUF_SIZE];

/* ע���ִ̨�еľ͵����� */
INT LCC_RegcBackGroundComm( GW_CHANNEL * pChl, UNSIGNED_CHAR iCommType, VOID * CallBack )
{
    INT  Status;
    /*================================================*/
    Status = NU_UNAVAILABLE;
    /* �ж�ͨѶ������Ƿ�������ķ�Χ֮�� */
    if( iCommType <= LCC_MAXCOMM_TYPE )
    {
        LC_BCallFunBuff[iCommType].pCallback = ( GWAPP_ACK_FUNCTION )CallBack;
        /* ��ͨѶ����ע������ */
        LCC_COMM_RegisterCommand( pChl, iCommType, LC_BCallManage );
        Status = NU_SUCCESS;
    }

    return( Status );
}

/* ͨѶ�����й���ע���̨��ͨѶ����Ĵ����� */
INT LC_BCallManage( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT Status;
    UNSIGNED_CHAR * pSrc;
    UNSIGNED_CHAR uCommType;      /* ͨѶ���� */
    UNSIGNED_CHAR   * pTemp;
    /*================================================*/
    pSrc = ( UNSIGNED_CHAR *)Src;

    Status = NU_UNAVAILABLE;
    uCommType = *( pSrc + 1 ); /* ͨѶ���� */
    /* ����ص�����ִ�й������������ʱ�ռ�,ִ�н�����Ҫ�ͷ� */
    LC_BCallFunBuff[uCommType].pSevData = gwmem_alloc( LCC_BACKDATA_MAXLENGTH );

    if ( LC_BCallFunBuff[uCommType].pSevData != NU_NULL )
    {
        pTemp = ( UNSIGNED_CHAR *)( LC_BCallFunBuff[uCommType].pSevData );
        /* �׵�ַ�洢��Ϊ������ */
        * pTemp = code;
        /* ���·���ͨѶ���ݴ��뻺������ */
        memcpy( ( pTemp + 1 ), pSrc, ( LCC_BACKDATA_MAXLENGTH - 1 ) );
        /* ������Ϣ���з������� */
        Status = NU_Send_To_Queue(& pChl->chl_block_tx_que,
            & LC_BCallFunBuff[uCommType], BLOCK_MSG_SIZE, LCC_QUE_SEND_DELAY );
        if ( Status != NU_SUCCESS )
        {
            /* ��������Ϣʧ��,��Ӧ�ͷ�����Ŀռ� */
            gwmem_free( LC_BCallFunBuff[uCommType].pSevData );
        }
    }
    return( COM_DATA_EMPTY );
}

/* ��̨����ִ�лص����� */
VOID  LC_Block( VOID )
{
    GW_CHANNEL    * pChl = LCC_pActive_Channel;
    INT           status;
    LC_BLOCK_CB   require_data;
    UNSIGNED      actual_size;
    UNSIGNED_CHAR uCode;    /* ����Ϊ�˺����Ĳ���,û��ʹ�� */
    UNSIGNED      uStatus;
    UNSIGNED_CHAR * pTemp1;
    UNSIGNED_CHAR * pTemp2;
    /*================================================*/

    /* �Է�����Ϣ���н���Ѳ��,����Ƿ��к�����Ҫִ�� */
    status = NU_Receive_From_Queue(& pChl->chl_block_tx_que, & require_data,
        BLOCK_MSG_SIZE,& actual_size, NU_NO_SUSPEND );

    if (( status != NU_SUCCESS ) || ( actual_size <= 0 ))
    {
        /* û�����ݻ����ݳ��Ȳ���ֱ�ӷ��� */
        return;
    }

    /* �жϽ������ݻ������Ƿ�Ϊ�� */
    if( require_data.pSevData != NU_NULL )
    {
        /* ���뷢����������Ļ����� */
        require_data.pSendData = gwmem_alloc( LCC_BACKDATA_MAXLENGTH );
    }
    else
    {
        /* �������ݻ�����Ϊ��,ֱ�ӷ��� */
        return;
    }

    /* �ж����ݷ������ݻ������Ƿ�Ϊ�� */
    if( require_data.pSendData != NU_NULL )
    {
        /* �׵�ַ�洢��Ϊ������ */
        pTemp2 = ( UNSIGNED_CHAR *)( require_data.pSevData );
        uCode = * pTemp2;
        pTemp1 = ( UNSIGNED_CHAR *)( require_data.pSendData );
        * pTemp1 = uCode;
        /* ִ�лص����� */
        status = require_data.pCallback( pChl, ( pTemp1 + 1 ), ( pTemp2 + 1 ), uCode );
    }
    else
    {
        /* �ͷ�����Ľ������ݻ����� */
        gwmem_free( require_data.pSevData );
        /* �������ݻ�����Ϊ��,ֱ�ӷ��� */
        return;
    }

    /* �ͷ�����Ľ������ݻ����� */
    gwmem_free( require_data.pSevData );

    /* �жϻص�������ִ�н���Ƿ�ɹ� */
    if( status == COM_DATA_FULL )
    {
        /* �������Ϣ�����з������� */
        uStatus = NU_Send_To_Queue(& pChl->chl_block_rx_que,
            & require_data, BLOCK_MSG_SIZE, LCC_QUE_SEND_DELAY );

        if ( uStatus != NU_SUCCESS )
        {
            /* ��������Ϣʧ��,�ͷ�����ķ������ݻ����� */
            gwmem_free( require_data.pSendData );
        }
    }
    else
    {
        /* �ͷ�����ķ������ݻ����� */
        gwmem_free( require_data.pSendData );
    }
}

/* ͨѶ�����м���Ƿ��лص�����ִ����,�Ƿ���Ҫ�ظ����� */
INT LC_BCall_Ret( GW_CHANNEL * pChl, UNSIGNED_CHAR * dst,
    UNSIGNED_CHAR * dst_port )
{
    INT             status;
    INT             size;
    LC_BLOCK_CB     require_data;
    UNSIGNED        actual_size;
    UNSIGNED_CHAR   * pTemp;

    /* ͨѶ�����жԽ�����Ϣ���н���Ѳ�� */
    status = NU_Receive_From_Queue(& pChl->chl_block_rx_que, & require_data,
        BLOCK_MSG_SIZE,& actual_size, NU_NO_SUSPEND );

    if (( status != NU_SUCCESS ) || ( actual_size <= 0 ))
    {
        /* û�����ݻ����ݳ��Ȳ���ֱ�ӷ��� */
        return 0;
    }
    /* �������ݻ����� */
    pTemp = ( UNSIGNED_CHAR *)require_data.pSendData;

    /* ���ݳ��� */
    size = 0;
    /* �жϷ������ݻ������Ƿ�Ϊ�� */
    if( pTemp != NU_NULL )
    {
        /* ���ÿ����� */
        * dst_port = * pTemp ++;
        /* ���ݳ��� */
        size = * pTemp ++;
        /* �����ݿ�����Ŀ�ĵ�ַ�� */
        memcpy( dst, pTemp, size );
        /* �ͷ�����ķ������ݻ����� */
        gwmem_free( require_data.pSendData );
    }

    return size;
}

/* ��ִ������Ĺ�����,���ִ��ʧ��������ע�ᵽ��̨���������ȥִ�� */
INT LCC_Run_RegcBackGroundComm( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code,
    UNSIGNED_CHAR iCommType, VOID * CallBack )
{
    INT  Status;
    /*================================================*/
    Status = NU_UNAVAILABLE;
    /* �ж�ͨѶ������Ƿ�������ķ�Χ֮�� */
    if( iCommType <= LCC_MAXCOMM_TYPE )
    {
        LC_BCallFunBuff[iCommType].pCallback = ( GWAPP_ACK_FUNCTION )CallBack;
        LC_BCallManage( pChl, Dst, Src, code );

        Status = NU_SUCCESS;
    }

    return( Status );
}

/* ����̨����ִ�е�����,������ִ�н�����,������ֱ��������Ϣ������;
����Ϊchar�ĸ���,�����������ݴ洢Ϊ�׵�ַΪ������,�ڶ�������Ϊ���ݳ���(�����������򼰳��ȱ���) */
INT  LCC_BCall_BackTaskSendMessData( INT iNum, UNSIGNED_CHAR * pBuffer )
{
    INT  Status;
    INT  iLen;
    /*====================================================*/
    iLen = ( iNum + sizeof( UNSIGNED ) - 1 )/ sizeof( UNSIGNED );    /* �����ĳ�������ΪUNSIGNED�ĸ��� */

    Status = NU_Send_To_Queue(& LCC_BCallBackTastQueue,
        pBuffer, iLen, LCC_QUE_SEND_DELAY );

    return( Status );
}

/* ��̨�������ͨѶ������Ϣ���г�ʼ������ */
INT LCC_BackTaskBCallBufferInit( VOID )
{
    INT status;
    /*====================================================*/
    status = NU_Create_Queue(& LCC_BCallBackTastQueue, "BTQUE", LCC_BCallBackTastQueueBuffer,
        LCC_BCALL_BACK_TASK_MESS_BUF_SIZE, NU_VARIABLE_SIZE, LCC_BCALL_BACK_TASK_MAX_LENGTH, NU_FIFO );

    return( status );
}

/* ����̨�������Ļ��������Ƿ��б��� */
INT LC_BCallCheckBackTaskMess( UNSIGNED_CHAR * dst, UNSIGNED_CHAR * dst_port )
{
    INT             status;
    INT             size;
    UNSIGNED        actual_size;
    UNSIGNED        TempBuff[LCC_BCALL_BACK_TASK_MAX_LENGTH];
    UNSIGNED_CHAR   * pTemp;

    /* ͨѶ�����жԽ�����Ϣ���н���Ѳ�� */
    status = NU_Receive_From_Queue(& LCC_BCallBackTastQueue, TempBuff,
        LCC_BCALL_BACK_TASK_MAX_LENGTH,& actual_size, NU_NO_SUSPEND );

    if (( status != NU_SUCCESS ) || ( actual_size <= 0 ))
    {
        /* û�����ݻ����ݳ��Ȳ���ֱ�ӷ��� */
        return 0;
    }
    /* �������ݻ����� */
    pTemp = ( UNSIGNED_CHAR *)& TempBuff[0];

    /* ���ݳ��� */
    size = 0;
    /* ���ÿ����� */
    * dst_port = * pTemp ++;
    /* ���ݳ��� */
    size = * pTemp ++;
    /* �����ݿ�����Ŀ�ĵ�ַ�� */
    memcpy( dst, pTemp, size );

    return size;
}
