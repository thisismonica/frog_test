/* 通信任务把要长时间执行的函数的入口地址和参数通过队列传递给报告处理任务,
报告处理任务执行后,把执行结果通过队列传递给通信任务,通讯任务发送执行结果*/
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "plat/inc/xj_cosc.h"
#include "plat/net_inc/lc_bcall.h"
#include "plat/net_inc/lc_extr.h"

extern   GW_CHANNEL  * LCC_pActive_Channel;
/* 注册后台回调函数的最大个数 */
LC_BLOCK_CB LC_BCallFunBuff[LCC_MAXCOMM_TYPE];

/* 后台任务处理的报文需要的消息队列及缓冲区 */
#define LCC_BCALL_BACK_TASK_MESS_BUF_SIZE     300     /* 后台处理的通讯报文消息队列缓冲区的大小 */
#define LCC_BCALL_BACK_TASK_MAX_LENGTH        60      /* 后台处理的通讯报文最大长度(UNSIGNED个数) */
static NU_QUEUE LCC_BCallBackTastQueue;
static UNSIGNED LCC_BCallBackTastQueueBuffer[LCC_BCALL_BACK_TASK_MESS_BUF_SIZE];

/* 注册后台执行的就地命令 */
INT LCC_RegcBackGroundComm( GW_CHANNEL * pChl, UNSIGNED_CHAR iCommType, VOID * CallBack )
{
    INT  Status;
    /*================================================*/
    Status = NU_UNAVAILABLE;
    /* 判断通讯命令号是否在允许的范围之内 */
    if( iCommType <= LCC_MAXCOMM_TYPE )
    {
        LC_BCallFunBuff[iCommType].pCallback = ( GWAPP_ACK_FUNCTION )CallBack;
        /* 向通讯任务注册命令 */
        LCC_COMM_RegisterCommand( pChl, iCommType, LC_BCallManage );
        Status = NU_SUCCESS;
    }

    return( Status );
}

/* 通讯任务中关于注册后台的通讯命令的处理函数 */
INT LC_BCallManage( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code )
{
    INT Status;
    UNSIGNED_CHAR * pSrc;
    UNSIGNED_CHAR uCommType;      /* 通讯类型 */
    UNSIGNED_CHAR   * pTemp;
    /*================================================*/
    pSrc = ( UNSIGNED_CHAR *)Src;

    Status = NU_UNAVAILABLE;
    uCommType = *( pSrc + 1 ); /* 通讯类型 */
    /* 申请回调函数执行过程中所需的临时空间,执行结束后要释放 */
    LC_BCallFunBuff[uCommType].pSevData = gwmem_alloc( LCC_BACKDATA_MAXLENGTH );

    if ( LC_BCallFunBuff[uCommType].pSevData != NU_NULL )
    {
        pTemp = ( UNSIGNED_CHAR *)( LC_BCallFunBuff[uCommType].pSevData );
        /* 首地址存储的为控制域 */
        * pTemp = code;
        /* 将下发的通讯数据存入缓冲区中 */
        memcpy( ( pTemp + 1 ), pSrc, ( LCC_BACKDATA_MAXLENGTH - 1 ) );
        /* 向发送消息队列发送数据 */
        Status = NU_Send_To_Queue(& pChl->chl_block_tx_que,
            & LC_BCallFunBuff[uCommType], BLOCK_MSG_SIZE, LCC_QUE_SEND_DELAY );
        if ( Status != NU_SUCCESS )
        {
            /* 若发送消息失败,则应释放申请的空间 */
            gwmem_free( LC_BCallFunBuff[uCommType].pSevData );
        }
    }
    return( COM_DATA_EMPTY );
}

/* 后台任务执行回调函数 */
VOID  LC_Block( VOID )
{
    GW_CHANNEL    * pChl = LCC_pActive_Channel;
    INT           status;
    LC_BLOCK_CB   require_data;
    UNSIGNED      actual_size;
    UNSIGNED_CHAR uCode;    /* 仅做为了函数的参数,没有使用 */
    UNSIGNED      uStatus;
    UNSIGNED_CHAR * pTemp1;
    UNSIGNED_CHAR * pTemp2;
    /*================================================*/

    /* 对发送消息队列进行巡检,检测是否有函数需要执行 */
    status = NU_Receive_From_Queue(& pChl->chl_block_tx_que, & require_data,
        BLOCK_MSG_SIZE,& actual_size, NU_NO_SUSPEND );

    if (( status != NU_SUCCESS ) || ( actual_size <= 0 ))
    {
        /* 没有数据或数据长度不对直接返回 */
        return;
    }

    /* 判断接收数据缓冲区是否为空 */
    if( require_data.pSevData != NU_NULL )
    {
        /* 申请发送数据所需的缓冲区 */
        require_data.pSendData = gwmem_alloc( LCC_BACKDATA_MAXLENGTH );
    }
    else
    {
        /* 接收数据缓冲区为空,直接返回 */
        return;
    }

    /* 判断数据发送数据缓冲区是否为空 */
    if( require_data.pSendData != NU_NULL )
    {
        /* 首地址存储的为控制域 */
        pTemp2 = ( UNSIGNED_CHAR *)( require_data.pSevData );
        uCode = * pTemp2;
        pTemp1 = ( UNSIGNED_CHAR *)( require_data.pSendData );
        * pTemp1 = uCode;
        /* 执行回调函数 */
        status = require_data.pCallback( pChl, ( pTemp1 + 1 ), ( pTemp2 + 1 ), uCode );
    }
    else
    {
        /* 释放申请的接收数据缓冲区 */
        gwmem_free( require_data.pSevData );
        /* 发送数据缓冲区为空,直接返回 */
        return;
    }

    /* 释放申请的接收数据缓冲区 */
    gwmem_free( require_data.pSevData );

    /* 判断回调函数的执行结果是否成功 */
    if( status == COM_DATA_FULL )
    {
        /* 向接收消息队列中发送数据 */
        uStatus = NU_Send_To_Queue(& pChl->chl_block_rx_que,
            & require_data, BLOCK_MSG_SIZE, LCC_QUE_SEND_DELAY );

        if ( uStatus != NU_SUCCESS )
        {
            /* 若发送消息失败,释放申请的发送数据缓冲区 */
            gwmem_free( require_data.pSendData );
        }
    }
    else
    {
        /* 释放申请的发送数据缓冲区 */
        gwmem_free( require_data.pSendData );
    }
}

/* 通讯任务中检测是否有回调函数执行完,是否需要回复报文 */
INT LC_BCall_Ret( GW_CHANNEL * pChl, UNSIGNED_CHAR * dst,
    UNSIGNED_CHAR * dst_port )
{
    INT             status;
    INT             size;
    LC_BLOCK_CB     require_data;
    UNSIGNED        actual_size;
    UNSIGNED_CHAR   * pTemp;

    /* 通讯任务中对接收消息对列进行巡检 */
    status = NU_Receive_From_Queue(& pChl->chl_block_rx_que, & require_data,
        BLOCK_MSG_SIZE,& actual_size, NU_NO_SUSPEND );

    if (( status != NU_SUCCESS ) || ( actual_size <= 0 ))
    {
        /* 没有数据或数据长度不对直接返回 */
        return 0;
    }
    /* 接收数据缓冲区 */
    pTemp = ( UNSIGNED_CHAR *)require_data.pSendData;

    /* 数据长度 */
    size = 0;
    /* 判断发送数据缓冲区是否为空 */
    if( pTemp != NU_NULL )
    {
        /* 设置控制域 */
        * dst_port = * pTemp ++;
        /* 数据长度 */
        size = * pTemp ++;
        /* 将数据拷贝到目的地址中 */
        memcpy( dst, pTemp, size );
        /* 释放申请的发送数据缓冲区 */
        gwmem_free( require_data.pSendData );
    }

    return size;
}

/* 在执行命令的过程中,如果执行失败则将命令注册到后台监控任务中去执行 */
INT LCC_Run_RegcBackGroundComm( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code,
    UNSIGNED_CHAR iCommType, VOID * CallBack )
{
    INT  Status;
    /*================================================*/
    Status = NU_UNAVAILABLE;
    /* 判断通讯命令号是否在允许的范围之内 */
    if( iCommType <= LCC_MAXCOMM_TYPE )
    {
        LC_BCallFunBuff[iCommType].pCallback = ( GWAPP_ACK_FUNCTION )CallBack;
        LC_BCallManage( pChl, Dst, Src, code );

        Status = NU_SUCCESS;
    }

    return( Status );
}

/* 经后台任务执行的命令,将报文执行结束后,将报文直接填入消息队列中;
长度为char的个数,缓冲区的数据存储为首地址为控制域,第二个数据为数据长度(不包括控制域及长度本身) */
INT  LCC_BCall_BackTaskSendMessData( INT iNum, UNSIGNED_CHAR * pBuffer )
{
    INT  Status;
    INT  iLen;
    /*====================================================*/
    iLen = ( iNum + sizeof( UNSIGNED ) - 1 )/ sizeof( UNSIGNED );    /* 将报文长度折算为UNSIGNED的个数 */

    Status = NU_Send_To_Queue(& LCC_BCallBackTastQueue,
        pBuffer, iLen, LCC_QUE_SEND_DELAY );

    return( Status );
}

/* 后台任务处理的通讯报文消息队列初始化函数 */
INT LCC_BackTaskBCallBufferInit( VOID )
{
    INT status;
    /*====================================================*/
    status = NU_Create_Queue(& LCC_BCallBackTastQueue, "BTQUE", LCC_BCallBackTastQueueBuffer,
        LCC_BCALL_BACK_TASK_MESS_BUF_SIZE, NU_VARIABLE_SIZE, LCC_BCALL_BACK_TASK_MAX_LENGTH, NU_FIFO );

    return( status );
}

/* 检测后台命名报文缓冲区中是否有报文 */
INT LC_BCallCheckBackTaskMess( UNSIGNED_CHAR * dst, UNSIGNED_CHAR * dst_port )
{
    INT             status;
    INT             size;
    UNSIGNED        actual_size;
    UNSIGNED        TempBuff[LCC_BCALL_BACK_TASK_MAX_LENGTH];
    UNSIGNED_CHAR   * pTemp;

    /* 通讯任务中对接收消息对列进行巡检 */
    status = NU_Receive_From_Queue(& LCC_BCallBackTastQueue, TempBuff,
        LCC_BCALL_BACK_TASK_MAX_LENGTH,& actual_size, NU_NO_SUSPEND );

    if (( status != NU_SUCCESS ) || ( actual_size <= 0 ))
    {
        /* 没有数据或数据长度不对直接返回 */
        return 0;
    }
    /* 接收数据缓冲区 */
    pTemp = ( UNSIGNED_CHAR *)& TempBuff[0];

    /* 数据长度 */
    size = 0;
    /* 设置控制域 */
    * dst_port = * pTemp ++;
    /* 数据长度 */
    size = * pTemp ++;
    /* 将数据拷贝到目的地址中 */
    memcpy( dst, pTemp, size );

    return size;
}
