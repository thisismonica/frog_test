
#include <string.h>
#include "plat/net_inc/lc_extr.h"
#include "plat/net_inc/rw_extr.h"
#include "plat/net_inc/ls_extr.h"
#include "plat/net_inc/vld_extr.h"
#include "plat/net_inc/rcc_defs.h"
#include "plat/inc/apf_extr.H"

/* 以下为OS相关配置 */
#ifdef WIN32
#include <stdio.h>
#include <malloc.h>
/* 全局变量申明 */
struct LCMemMService LCMMS = {malloc, free};
#endif

#ifndef WIN32 /* 非windows缺省定义一些宏以便通过编译 */
#include "plat/inc/xj_cmemc.h"
struct LCMemMService LCMMS = {malloc, free};
#endif

/* 通道缓冲区 */
GW_CHANNEL    LCC_Link_Chl_CB[LC_MAX_CHANNEL];
/* 消息队列"First" */
NU_QUEUE      LCC_Link_Primary_QUE;
/* 消息队列"Secnod" */
NU_QUEUE      LCC_Link_Second_QUE;
/* 消息队列"First"缓冲区 */
UNSIGNED LCC_Link_Primary_Buff[LC_PRIMARY_SIZE];
/* 消息队列"Secnod"缓冲区 */
UNSIGNED LCC_Link_Second_Buff[LC_SECOND_SIZE];

/* 通讯初始化函数 */
GW_CHANNEL * LCC_Link_OpenChannel( UNSIGNED chlID )
{
    INT i;
    GW_CHANNEL * pChl;
    STATUS status;

    /* 清缓冲区 */
    memset(( VOID *)LCC_Link_Primary_Buff, 0, ( LC_PRIMARY_SIZE * sizeof( UNSIGNED ) ));
    memset(( VOID *)LCC_Link_Second_Buff, 0, ( LC_SECOND_SIZE * sizeof( UNSIGNED ) ));

    status = NU_Create_Queue(& LCC_Link_Primary_QUE, "First", LCC_Link_Primary_Buff,
        LC_PRIMARY_SIZE, NU_VARIABLE_SIZE, LC_MAX_MSG_SIZE, NU_FIFO );

    status = NU_Create_Queue(& LCC_Link_Second_QUE, "Secnod", LCC_Link_Second_Buff,
        LC_SECOND_SIZE, NU_VARIABLE_SIZE, LC_MAX_MSG_SIZE, NU_FIFO );

    pChl    = GW_NULL;
    for ( i = 0 ; i < LC_MAX_CHANNEL ; i ++)
    {
        if ( chlID ==  LCC_Link_Chl_CB[i].chl_ID )
        {
            pChl = & LCC_Link_Chl_CB[i];
            pChl->chl_Open = NU_TRUE;
            pChl->chl_rx_ok = NU_TRUE;
            pChl->chl_FCB = I870FT12_CONTROL_DIR;
            pChl->chl_temp_size = 0;
            xjdlist_initialize(& pChl->chl_suspend_List, 255 );
            LCC_pActive_Channel = pChl;
            /* 初始化通讯通道 */
            LCC_Link_ChlInitialize( pChl, 0 );
            LCC_Physical_OpenChannel( pChl );
            break;
        }
    }
    return pChl;
}

/* 读数据 */
INT LCC_Link_Read( GW_CHANNEL * pChl, UNSIGNED_CHAR * data )
{
    INT size;
    size = LCC_Physical_Read( pChl, data );
    if ( size > 0 )
    {
        if (! LCC_MessM_CheckFrame( data, ( UNSIGNED_CHAR )size ))
        {
            LCC_ComErr.CheckFrameErr ++;
            size = 0;
        }
    }
    return size;
}

/* 写数据 */
INT LCC_Link_Write( GW_CHANNEL * pChl, UNSIGNED_CHAR * data, INT size )
{
    return LCC_Physical_Write( pChl, data, size );
}

/* 初始化通讯通道 */
VOID LCC_Link_ChlInitialize( GW_CHANNEL * pChl, UNSIGNED  control_mask )
{
    INT i;
    INT status;

    CPC_UNUSED_PARAM( control_mask );
    pChl->chl_initialize_finish = GW_FALSE;
    pChl->chl_initialize_timeout = LC_STARTDT_SHORT_TIMEOUT;
    pChl->chl_initialize_start  = 0;
    pChl->chl_setting_flag = 0;
    /*RWC_Initialize(pChl);*/
#if LC_STAT
    for ( i = 0 ; i < LCC_PRIORITY_MAX;   i ++)
    {
        pChl->stat_select_last_tick[i] = 0;
        pChl->stat_select_count[i]     = 0;
    }
#endif
    for ( i = 0 ; i < LC_CHANNEL_COMMAND ; i ++)
    {
        pChl->chl_callback_List[i].Callback = NULL;
#if     LC_STAT
        pChl->chl_callback_List[i].call_count = 0;
#endif
    }

    LSC_RegisterCommand_Local( pChl );
    /* 注册应用程序通讯命令 */
    APFC_RegisterCommand( pChl );
    /* 注册上调vld数据所需的函数 */
    VLD_RegisterCommand_VLD( pChl );
    /*disturb record support*/
    if ( LC_FUN_WAVE )
    {
        LCC_COMM_RegisterCommand( pChl, I3DEF_TYPE_DIST_ORDR, lcApp_ProcessDISTORDR );
        LCC_COMM_RegisterCommand( pChl, I3DEF_TYPE_DIST_ACK, lcApp_ProcessDISTACK );
    }
    if ( LC_FUN_REPORT )
    {
        LCC_COMM_RegisterCommand( pChl, I3DEF_TYPE_XJ_TI62, lcApp_ProcessReportCopy );
    }

    /* 对时命令 */
    LCC_COMM_RegisterCommand( pChl, I3DEF_TYPE_TIME_SYNC, LCC_COMM_ProcessTimeSyn );

    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_A7, LCC_CB_ProcessBlockIO );
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_A8, LCC_CB_ProcessBlockIO );
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_A9, LCC_CB_ProcessBlockIO );
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_AA, LCC_CB_ProcessBlockIO );
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_AF, LCC_CB_ProcessBlockIO );
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_B0, LCC_CB_ProcessBlockIO );
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C1, LCC_CB_ProcessBlockIO );
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C2, LCC_CB_ProcessBlockIO );
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C3, LCC_CB_ProcessBlockIO );
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C4, LCC_CB_ProcessBlockIO );
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C5, LCC_CB_ProcessBlockIO );
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C6, LCC_CB_ProcessBlockIO );
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C7, LCC_CB_ProcessBlockIO );
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C8, LCC_CB_ProcessBlockIO );
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_C9, LCC_CB_ProcessBlockIO );
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_CB, LCC_CB_ProcessBlockIO );
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_CC, LCC_CB_ProcessBlockIO );
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_CD, LCC_CB_ProcessBlockIO );
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_CE, LCC_CB_ProcessBlockIO );

    /*
    LCC_COMM_RegisterCommand( pChl, I3DEF_TYPE_STARTDT, LCC_COMM_ProcessStartDT );
    LCC_COMM_RegisterCommand( pChl, I3DEF_TYPE_XJ_TI60, LCC_COMM_ProcessVersion );
    */
    LCC_COMM_RegisterCommand( pChl, I3DEF_TYPE_MMI_LONVER, LCC_COMM_ProcessLonVersion );
    LCC_COMM_RegisterCommand( pChl, I3DEF_TYPE_PRO_MMIVER, LCC_COMM_ProcessLonVersion );

    /* 清缓冲区 */
    memset(( VOID *)pChl->chl_block_rx_buffer, 0, ( LC_BLOCK_QUE_SIZE * sizeof( UNSIGNED ) ));
    memset(( VOID *)pChl->chl_block_tx_buffer, 0, ( LC_BLOCK_QUE_SIZE * sizeof( UNSIGNED ) ));
    /* 长时间处理的通讯命令需要的消息队列"read" */
    status = NU_Create_Queue(& pChl->chl_block_rx_que, "read", pChl->chl_block_rx_buffer,
        LC_BLOCK_QUE_SIZE, NU_FIXED_SIZE, BLOCK_MSG_SIZE, NU_FIFO );

    /* 长时间处理的通讯命令需要的消息队列"Secnod" */
    status = NU_Create_Queue(& pChl->chl_block_tx_que, "Secnod", pChl->chl_block_tx_buffer,
        LC_BLOCK_QUE_SIZE, NU_FIXED_SIZE, BLOCK_MSG_SIZE, NU_FIFO );

    return;
}

/* 通讯内存空间申请 */
VOID LCC_Link_AllocateMemory( VOID )
{
    UNSIGNED    uSize;
    VOID        * pMemory;

    uSize  = ( sizeof( GW_COMMAND_CALLBACK_NODE )+ GW_DM_HEADER_SIZE )* LC_DM_MAX_COMMAND;
    uSize  += ( sizeof( GW_BLOCK_IO_CB )+ GW_DM_HEADER_SIZE )* LC_DM_MAX_IO_CB;
    uSize  += ( sizeof( GW_RC_CB )+ GW_DM_HEADER_SIZE )* LC_DM_MAX_IO_CB;
    uSize  += ( sizeof( GW_RW_CB )+ GW_DM_HEADER_SIZE )* LC_DM_MAX_IO_CB;
    uSize  += ( sizeof( GW_VLD_CB )+ GW_DM_HEADER_SIZE )* LC_DM_MAX_IO_CB;

    /* 增加1k空间的余度 */
    uSize  += 1024;
    pMemory = ( VOID *)LCMMS.MemAlloc( uSize );

    if( pMemory != NU_NULL )
    {
        gwmem_CreateMemory( uSize, pMemory );
    }
    else
    {
        NU_Set_Events(& Event_2_CB, CPC_EVENT2_NO_MEMORY, NU_OR );
    }
}

/* 超时检测函数 */
VOID LCC_Link_CheckTimer( GW_CHANNEL * pChl )
{
    INT            number;
    UNSIGNED        ActualEvent;
    GW_REQUEST_CB   * pCB;
    number  = 0;

    /* 1000:检查超时时间间隔 */
    if (( TMD_System_Clock - pChl->chl_idle_start ) > 1000 )
    {
        /* 判断数据模板是否上调完成 */
        if (! pChl->chl_initialize_finish )
        {
            /* 若没有完成,等待时间是否到 */
            if (( TMD_System_Clock - pChl->chl_initialize_start )
                > pChl->chl_initialize_timeout )
            {
                /* 等待时间到,置数据模板已经上调完成标志 */
                pChl->chl_initialize_finish = NU_TRUE;
            }
        }

        /* 处理定值等待的最长时间 */
        if (( TMD_System_Clock - pChl->chl_setting_start )
            > LC_SETTING_WAIT_TIMEOUT )
        {
            pChl->chl_setting_flag  = 0;
            pChl->chl_setting_start = TMD_System_Clock;
        }

        if ( pChl->chl_rx_ok )
        {
            /* 判断时钟初始化是否完成 */
            if ( NU_Retrieve_Events(& Event_2_CB, CPC_EVENT2_CLOCK_READY,
                NU_AND,& ActualEvent, NU_NO_SUSPEND ) == NU_SUCCESS )
            {
                /* 检测有多长时间没有接收到数据 */
                if (( TMD_System_Clock - pChl->chl_rx_ok_tick )> 20000 )
                {
                    pChl->chl_rx_ok = GW_FALSE;
                    pChl->chl_rx_ok_tick = TMD_System_Clock;
                    /* 20秒没有接收到数据,置通讯中断标志 */
                    NU_Set_Events(& Event_1_CB, CPC_EVENT1_ERR_COMMU, NU_OR );
                }
            }
        }

        /* 判断接收seret时间是否到 */
        if (( TMD_System_Clock - pChl->chl_rx_reset_tick ) > 3000 )
        {
            LCC_Physical_OpenChannel( pChl );
            pChl->chl_rx_reset_tick = TMD_System_Clock;
        }

        /* 判断节点中是否有长时间没有处理的,若超过处理时间,清除此节点 */
        pCB = ( GW_REQUEST_CB *)xjdlist_getFirst(& pChl->chl_suspend_List );
        while ( pCB != NU_NULL )
        {
            if (( TMD_System_Clock - pCB->start_timeout ) > pCB->timeout )
            {
                xjdlist_removeEntry(& pChl->chl_suspend_List,& pCB->node );
                gwmem_free( pCB );
                pCB = GW_NULL;
            }
            pCB = ( GW_REQUEST_CB *)xjdlist_getAfter(
                & pChl->chl_suspend_List,& pCB->node );
        }
        pChl->chl_idle_start    = TMD_System_Clock;
    }
    return;
}

/* 因操作系统的消息队列中数据长度为UNSIGNED数据个数,而实际使用中用到的通讯报文均为CHAR,所以现在将其操作系统的函数封装 */
/* 发送消息队列 */
STATUS LCC_Link_Send_To_Queue( NU_QUEUE * queue_ptr, VOID * message, UNSIGNED size, UNSIGNED suspend, UNSIGNED uDataSize )
{
    INT      iStatus;
    UNSIGNED uSize;

    /* 将数据个数转换为UNSIGNED的个数 */
    uSize = (( size * uDataSize + sizeof( UNSIGNED ) - 1 )/ sizeof( UNSIGNED ));
    iStatus = NU_Send_To_Queue( queue_ptr, message, uSize, suspend );
    return( iStatus );
}

/* 接收消息队列 */
STATUS LCC_Link_Receive_From_Queue( NU_QUEUE * queue, VOID * message, UNSIGNED size,
    UNSIGNED * actual_size, UNSIGNED suspend, UNSIGNED uDataSize )
{
    INT      iStatus;
    UNSIGNED uSize;
    UNSIGNED_CHAR * pChar;

    iStatus = NU_Receive_From_Queue( queue, message, size,& uSize, suspend );
    * actual_size = 0;
    if( iStatus == NU_SUCCESS )
    {
        pChar = ( UNSIGNED_CHAR *)message;
        /* 指定类型的报文长度 */
        * actual_size = (( uSize * sizeof( UNSIGNED ) + uDataSize - 1 ) / uDataSize );
    }
    return( iStatus );
}
