/********************************************************************************/
/*  Copyight (C) 2001-2003      保护及自动化事业部基础                          */
/*                                                                              */
/* All right reserved                                                           */
/*                                                                              */
/* FILE NAME                        VERSION                                     */
/*                                                                              */
/*  rwc.c                       PLUS  1.0                                       */
/*                                                                              */
/* COMPONENT                                                                    */
/*                                                                              */
/*      rw - Record Wave                                                        */
/*      c   - core                                                              */
/*                                                                              */
/* DESCRIPTION                                                                  */
/*                                                                              */
/*  This file contains the core routines for Record wave                        */
/*                                                                              */
/*                                                                              */
/* AUTHOR                                                                       */
/*                                                                              */
/*  Chenyufeng      < ChenYuFeng@mail.xjpa.xj>                                  */
/* DATA STRUCTURES                                                              */
/*                                                                              */
/*  None                                                                        */
/*                                                                              */
/* HISTORY                                                                      */
/*                                                                              */
/*                                                                              */
/*      NAME        DATE            REMARKS                                     */
/*                                                                              */
/*      YuFeng.CH   05-15-2003                                                  */
/*      YuFeng.CH   02-15-2005      new model ver 1.0                           */
/********************************************************************************/
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "plat/inc/xj_cosc.h"
#include "plat/inc/tab_defs.h"
#include "plat_specific/plat_specific.h"
#include "plat/net_inc/rw_extr.h"
#include "plat/net_inc/lc_extr.h"

#define RW_TotalWaveState GET_WAVE_STATE_TAB_NUMBER  /* 获取保护状态量个数 */
#define RW_WaveState      GET_WAVE_STATE_TAB_MEMORY  /* 获取保护状态量内存首地址 */
#define RW_TotalInput     GET_INPUT_TAB_NUMBER       /* 获取开入量个数 */
#define RW_TotalOutput    GET_OUTPUT_TAB_NUMBER      /* 获取开出量个数 */
#define RW_Input          GET_INPUT_TAB_MEMORY       /* 获取开入量内存首地址 */
#define RW_Output         GET_OUTPUT_TAB_MEMORY      /* 获取开出量内存首地址 */

extern CWAVE CWave;
extern volatile UNSIGNED TMD_System_Clock;

VOID    gwDIST_AdjustWHD( CWM_WHD * pWaveHead );
STATUS  gwDIST_GetTotalCHNL( GW_RW_CB * pCB );
STATUS  gwDIST_InitDTAG( GW_RW_CB * pCB );
GW_VOID gwDIST_RestoreDTAG( GW_RW_CB * pCB );
GW_VOID gwDIST_SaveDTAG( GW_RW_CB * pCB );
STATUS  gwDIST_GetNextCHNL( GW_RW_CB * pCB );
STATUS  gwDIST_Inf2Offset( GW_RW_CB * pCB );
INT     gwDIST_DTAG_Data( UNSIGNED_CHAR * dst, GW_RW_CB * pCB );
VOID    RWC_ZeroTable( UNSIGNED * pTable );
INT     RWC_Disturb_Ready( UNSIGNED_CHAR * dst, GW_RW_CB * pCB );

/*GW_DISTURB_CB Disturb_CB[GW_RW_MAX_TABLE];*/
#define gwDIST_CHNL_Skip(dst,pCB) RWC_Trans_Result(dst,I3DEF_TOO_END_CHNL_BY_CTRL,1,pCB->rw_chl_inf,pCB)
#define gwDIST_CHNL_End(dst,pCB)  RWC_Trans_Result(dst,I3DEF_TOO_END_CHNL_NO_ABORT,1,pCB->rw_chl_inf,pCB)
#define gwDIST_CHNL_Finish(dst,pCB) RWC_Trans_Result(dst,I3DEF_TOO_END_DATA_NO_ABORT,1,pCB->rw_chl_inf,pCB)
#define RWC_Confirm_Exit(dst,pCB)   RWC_Trans_Result(dst,I3DEF_TOO_END_DATA_BY_CTRL,1,pCB->rw_chl_inf,pCB)

#define gwDIST_DTAG_Skip(dst,pCB)   RWC_Trans_Result(dst,I3DEF_TOO_END_TAGS_BY_CTRL,1,0,pCB)
#define gwDIST_LOGS_Skip(dst,pCB)   RWC_Trans_Result(dst,I3DEF_TOO_END_LOGS_BY_CTRL,1,0,pCB)
#define gwDIST_DTAG_End(dst,pCB)    RWC_Trans_Result(dst,I3DEF_TOO_END_TAGS_NO_ABORT,1,0,pCB)
#define gwDIST_LOGS_End(dst,pCB)    RWC_Trans_Result(dst,I3DEF_TOO_END_LOGS_NO_ABORT,1,0,pCB)

INT gwDIST_DTAG_Ready( UNSIGNED_CHAR * dst, GW_RW_CB * pCB );
INT gwDIST_CHNL_Ready( UNSIGNED_CHAR * dst, GW_RW_CB    * pCB );
INT gwDIST_CHNL_Data( UNSIGNED_CHAR * dst, GW_RW_CB * pCB );
INT RWC_Trans_Result( UNSIGNED_CHAR * dst , UNSIGNED_CHAR  ucCMD, UNSIGNED_CHAR  ucData,
    UNSIGNED_CHAR  ucResult, GW_RW_CB * pCB );
INT RWC_PackReadyHead( UNSIGNED_CHAR * dst, UNSIGNED_CHAR type, UNSIGNED_CHAR  ucCMD, UNSIGNED_CHAR ucData,
    GW_RW_CB * pCB );
INT RWC_PackDataHead( UNSIGNED_CHAR * dst, UNSIGNED_CHAR type, GW_RW_CB * pCB );

INT lcApp_ProcessDISTACK( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port )
{
    GW_RW_CB    * pCB;
    GW_USHORT   number;
    UNSIGNED_CHAR   * dst    = ( UNSIGNED_CHAR *)p_dst;
    UNSIGNED_CHAR   * src    = ( UNSIGNED_CHAR *)p_src;

    /* 故障序号 */
    number  = BYTE_PTR_TO_16( src + LCC_OFFSET_CONTEXT + 2 );
    if   ((*( src + LCC_OFFSET_CONTEXT ) == I3DEF_TOO_DISTURB_POS_ACK )
        || (*( src + LCC_OFFSET_CONTEXT ) == I3DEF_TOO_DISTURB_NEG_ACK ))
    {
        if(*( src + LCC_OFFSET_CONTEXT ) == I3DEF_TOO_DISTURB_NEG_ACK )
        {
            pChl->chl_faultNumber   = 0;
        }
    }
    else
    {
        pCB = ( GW_RW_CB *)LCC_CB_CreateActiveCB( pChl, sizeof( GW_RW_CB ),
            0,*( src + LCC_OFFSET_SECT ), LC_PRIORITY_RW, dst_port,
            lcApp_AckRW, LC_TIMEOUT_RW );
        if ( pCB )
        {
            pCB->rw_fun         = *( src + LCC_OFFSET_FUN );
            pCB->rw_Fault_Number = number;
            pCB->rw_TOO = *( src + LCC_OFFSET_CONTEXT );
            switch(*( src + LCC_OFFSET_CONTEXT ))
            {
            case I3DEF_TOO_CHNL_POS_ACK:    //data transmission succeseful
                pCB->rw_chl_inf = *( src + LCC_OFFSET_CONTEXT + 4 );
                pCB->rw_Step_M  = GWDIST_M_STEP_ACK_CHNL;
                break;
            case I3DEF_TOO_CHNL_NEG_ACK:    //data transmission fail
                pCB->rw_chl_inf = *( src + LCC_OFFSET_CONTEXT + 4 );
                pCB->rw_Step_M  = GWDIST_M_STEP_INIT_CHNL;
                break;
            case I3DEF_TOO_TAGS_POS_ACK:    //status transmission succeseful,Control Direction Step 10
            case I3DEF_TOO_LOGS_POS_ACK:
                pCB->rw_Step_M  = GWDIST_M_STEP_INIT_CHNL;
                pCB->rw_CHNL_Index = 0;
                break;
            case I3DEF_TOO_TAGS_NEG_ACK:    //staus transmission fail
                pCB->rw_TOO     = I3DEF_TOO_REQUEST_FOR_TAGS;
                pCB->rw_Step_M  = GWDIST_M_STEP_INIT_DTAG;
                break;
            case I3DEF_TOO_LOGS_NEG_ACK:
                pCB->rw_TOO     = I3DEF_TOO_REQUEST_FOR_LOGS;
                pCB->rw_Step_M  = GWDIST_M_STEP_INIT_DTAG;
                break;
            default:
                LCC_CB_freeAckCB( pChl, pCB );
                break;
            }
        }
    }
    pChl->chl_disturb_order_start   = TMD_System_Clock;
    return COM_DATA_EMPTY;
}

INT lcApp_ProcessDISTORDR( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port )
{
    GW_RW_CB    * pCB;
    GW_USHORT   number;
    UNSIGNED_CHAR   * dst    = ( UNSIGNED_CHAR *)p_dst;
    UNSIGNED_CHAR   * src    = ( UNSIGNED_CHAR *)p_src;

    pChl->chl_disturb_order_start   = TMD_System_Clock;
    if (*( src + LCC_OFFSET_CONTEXT ) == I3DEF_TOO_REQ_DISTURB_LIST )
    {
        pChl->chl_faultNumber   = 0;
    }
    else
    {
        /* 故障序号 */
        number  = BYTE_PTR_TO_16( src + LCC_OFFSET_CONTEXT + 2 );
        pCB = ( GW_RW_CB *)LCC_CB_CreateActiveCB( pChl, sizeof( GW_RW_CB ),
            0,*( src + LCC_OFFSET_SECT ), LC_PRIORITY_RW,
            dst_port, lcApp_AckRW, LC_TIMEOUT_RW );

        if ( pCB )
        {
            pCB->rw_fun         = *( src + LCC_OFFSET_FUN );
            pCB->rw_TOO         = *( src + LCC_OFFSET_CONTEXT );
            pCB->rw_Fault_Number = number;
            switch(*( src + LCC_OFFSET_CONTEXT ))
            {
            case I3DEF_TOO_SELECTION_OF_FAULT:      //Active data transmission,Control direction step 2
                pCB->rw_Step_M = GWDIST_M_STEP_READY_DATA;
                break;
            case I3DEF_TOO_REQUEST_OF_DISTURB:      //require data,Control direction step 4
                pCB->rw_Step_M = GWDIST_M_STEP_READY_DTAG;
                break;
            case I3DEF_TOO_ABORT_OF_DISTURB:        //esc
                pCB->rw_Step_M = GWDIST_M_STEP_CONFIRM_EXIT;
                break;
            case I3DEF_TOO_REQUEST_FOR_CHNL:        //send run data
                pCB->rw_Step_M = GWDIST_M_STEP_TRANS_CHNL;
                pCB->rw_chl_inf = *( src + LCC_OFFSET_CONTEXT + 4 ); /* 通道信息序号 */
                pCB->rw_Transmit_Position   = 0;
                break;
            case I3DEF_TOO_ABORT_OF_CHNL:           //exit data transmission
                pCB->rw_Step_M = GWDIST_M_STEP_SKIP_CHNL;
                pCB->rw_chl_inf = *( src + LCC_OFFSET_CONTEXT + 4 ); /* 通道信息序号 */
                break;
            case I3DEF_TOO_REQUEST_FOR_TAGS:        //require send state,Control Step 6
            case I3DEF_TOO_REQUEST_FOR_LOGS:
                pCB->rw_Step_M = GWDIST_M_STEP_INIT_DTAG;
                break;
            case I3DEF_TOO_ABORT_OF_TAGS:           //stop send state
                pCB->rw_Step_M = GWDIST_M_STEP_SKIP_DTAG;
                break;
            case I3DEF_TOO_ABORT_OF_LOGS:
                pCB->rw_Step_M = GWDIST_M_STEP_SKIP_LOGS;
                break;
            default:
                LCC_CB_freeAckCB( pChl, pCB );
                break;
            }
        }
    }
    return COM_DATA_EMPTY;
}

INT lcApp_AckRW( UNSIGNED_CHAR * dst, GW_CHANNEL   * pChl, VOID  * pCB )
{
    INT ret;
    GW_RW_CB    * pRW_CB;
    pRW_CB  = ( GW_RW_CB *)pCB;
    ret = 0;
    switch( pRW_CB->rw_Step_M )
    {
    case GWDIST_M_STEP_READY_DATA:      //send data ready ,Monitor direction Step 3
        ret = RWC_Disturb_Ready( dst, pRW_CB );
        if ( ret != NU_TIMEOUT )
        {
            LCC_CB_freeAckCB( pChl, pCB );
        }
        break;
    case GWDIST_M_STEP_READY_DTAG:      //Send Status Ready,Monitor direction Step 5
        ret = gwDIST_DTAG_Ready( dst, pRW_CB );
        LCC_CB_freeAckCB( pChl, pCB );
        break;
    case GWDIST_M_STEP_SKIP_DTAG:
        ret = gwDIST_DTAG_Skip( dst, pRW_CB );
        pRW_CB->rw_Step_M   = GWDIST_M_STEP_INIT_CHNL;
        pRW_CB->rw_CHNL_Index   = 0;
        break;
    case GWDIST_M_STEP_SKIP_LOGS:
        ret = gwDIST_LOGS_Skip( dst, pRW_CB );
        pRW_CB->rw_Step_M   = GWDIST_M_STEP_INIT_CHNL;
        pRW_CB->rw_CHNL_Index   = 0;
        break;
    case GWDIST_M_STEP_INIT_DTAG:       //Send Status Data, Monitro direction Step 7,8
        ret = gwDIST_InitDTAG( pRW_CB );
        if ( ret == NU_SUCCESS )
        {
            if ( pRW_CB->rw_DTAG_Total > 0 )
            {
                pRW_CB->rw_Step_M       = GWDIST_M_STEP_TRANS_DTAG;
            }
            else
            {
                pRW_CB->rw_Step_M       = GWDIST_M_STEP_END_DTAG;
            }
        }
        else
        {
            if ( ret != NU_TIMEOUT )
            {
                LCC_CB_freeAckCB( pChl, pCB );
            }
        }
        break;
    case GWDIST_M_STEP_TRANS_DTAG:
        ret = gwDIST_DTAG_Data( dst, pRW_CB );
        if (( ret != NU_TIMEOUT ) && ( ret < 0 ))
        {
            LCC_CB_freeAckCB( pChl, pCB );
        }
        break;
    case GWDIST_M_STEP_END_DTAG:
        ret = gwDIST_DTAG_End( dst, pRW_CB );
        LCC_CB_freeAckCB( pChl, pCB );
        break;
    case GWDIST_M_STEP_END_LOGS:
        ret = gwDIST_LOGS_End( dst, pRW_CB );
        LCC_CB_freeAckCB( pChl, pCB );
        break;
    case GWDIST_M_STEP_INIT_CHNL:
        ret = gwDIST_GetTotalCHNL( pRW_CB );
        if ( ret == NU_SUCCESS )
        {
            if ( pRW_CB->rw_CHNL_Total > 0 )
            {
                if ( pRW_CB->rw_CHNL_Index <( INT ) pRW_CB->rw_CHNL_Total )
                {
                    pRW_CB->rw_Step_M = GWDIST_M_STEP_READY_CHNL;
                }
                else
                {
                    pRW_CB->rw_Step_M  = GWDIST_M_STEP_FINISH_TRANS;
                }
            }
            else
            {
                pRW_CB->rw_Step_M  = GWDIST_M_STEP_FINISH_TRANS;
            }
        }
        else
        {
            if ( ret != NU_TIMEOUT )
            {
                LCC_CB_freeAckCB( pChl, pCB );
            }
        }
        break;
    case GWDIST_M_STEP_READY_CHNL:    //Send Analog Ready,Monitor direction Step 11
        ret = gwDIST_CHNL_Ready( dst, pRW_CB );
        if ( ret != NU_TIMEOUT )
        {
            LCC_CB_freeAckCB( pChl, pRW_CB );
        }
        break;
    case GWDIST_M_STEP_TRANS_CHNL:
        ret = gwDIST_CHNL_Data( dst, pRW_CB );
        if (( ret != NU_TIMEOUT ) && ( ret < 0 ))
        {
            LCC_CB_freeAckCB( pChl, pCB );
        }
        break;
    case GWDIST_M_STEP_END_CHNL:
        ret = gwDIST_CHNL_End( dst, pRW_CB );
        LCC_CB_freeAckCB( pChl, pCB );
        break;
    case GWDIST_M_STEP_SKIP_CHNL:
        ret = gwDIST_CHNL_Skip( dst, pRW_CB );
        pRW_CB->rw_Step_M   = GWDIST_M_STEP_ACK_CHNL;
        break;
    case GWDIST_M_STEP_ACK_CHNL:
        ret = gwDIST_GetNextCHNL( pRW_CB );
        if ( ret == NU_SUCCESS )
        {
            if ( pRW_CB->rw_CHNL_Index < ( INT )pRW_CB->rw_CHNL_Total )
            {
                pRW_CB->rw_Step_M = GWDIST_M_STEP_READY_CHNL;
            }
            else
            {
                pRW_CB->rw_Step_M = GWDIST_M_STEP_FINISH_TRANS;
            }
        }
        else
        {
            if ( ret != NU_TIMEOUT )
            {
                LCC_CB_freeAckCB( pChl, pCB );
            }
        }
        break;
    case GWDIST_M_STEP_FINISH_TRANS:
        ret = gwDIST_CHNL_Finish( dst, pRW_CB );
        LCC_CB_freeAckCB( pChl, pCB );
        break;
    case GWDIST_M_STEP_CONFIRM_EXIT:
        ret = RWC_Confirm_Exit( dst, pRW_CB );
        LCC_CB_freeAckCB( pChl, pCB );
        break;
    default:
        LCC_CB_freeAckCB( pChl, pCB );
        break;
    }
    if ( ret < 0 )
    {
        ret = 0;
    }
    return ret;
}

VOID    gwDIST_AdjustWHD( CWM_WHD * pWaveHead )
{
    /*修正模拟量通道数目*/
    if ( pWaveHead->AnalogNumber > CPC_MAX_CHL_NUMBER )
    {
        pWaveHead->AnalogNumber = CPC_MAX_CHL_NUMBER;
    }
    /*修正开入量组数目*/
    if ( pWaveHead->InputNumber > CPC_MAX_INPUT_GROUPS )
    {
        pWaveHead->InputNumber = CPC_MAX_INPUT_GROUPS;
    }
    /*修正开出状态量组数目*/
    if ( pWaveHead->OutputNumber > CPC_MAX_OUTPUT_GROUPS )
    {
        pWaveHead->OutputNumber = CPC_MAX_OUTPUT_GROUPS;
    }
    /*修正保护状态量组数目*/
    if ( pWaveHead->ProtectNumber > CPC_MAX_PROTECT_GROUPS )
    {
        pWaveHead->ProtectNumber = CPC_MAX_PROTECT_GROUPS;
    }
    /*修正逻辑状态量组数目*/
    if ( pWaveHead->LogicNumber > CPC_MAX_PROTECT_LOG_GROUPS )
    {
        pWaveHead->LogicNumber = CPC_MAX_PROTECT_LOG_GROUPS;
    }
    if ( pWaveHead->TotalPointer > CPC_MAX_WAVE_DATA_SIZE )
    {
        pWaveHead->TotalPointer = CPC_MAX_WAVE_DATA_SIZE;
    }
}

INT RWC_Disturb_Ready( UNSIGNED_CHAR * dst, GW_RW_CB * pCB )
{
    INT size;
    STATUS          status;
    CWM_WHD         WaveHead,* pWaveHead;
    float           fPeriod;
    UNSIGNED        period;
    size    = 0;

    /* read wave data. */
    status = GW_RW_READ(& CWave, pCB->rw_Fault_Number,& WaveHead, 0, 0,
        ( sizeof( CWM_WHD )/ sizeof( UNSIGNED )), RW_TARG_OP_TIMEOUT );
    if ( status == NU_SUCCESS )
    {
        pWaveHead = & WaveHead;
        gwDIST_AdjustWHD( pWaveHead );
        size    = RWC_PackReadyHead( dst, I3DEF_TYPE_RDY_DDATA,
            0, 1, pCB );

        /*电网故障序号*/
        *( dst + size ++) = ( UNSIGNED_CHAR ) ( pWaveHead->ElecNumber & GW_MASKBIT_8 );
        *( dst + size ++) = ( UNSIGNED_CHAR )(( pWaveHead->ElecNumber >> CPC_BYTE_BITS ) & GW_MASKBIT_8 );

        /*模拟量通道个数*/
        *( dst + size ++) = ( UNSIGNED_CHAR )( pWaveHead->AnalogNumber );

        /*采样点数*/
        *( dst + size ++) = ( UNSIGNED_CHAR )( pWaveHead->TotalPointer & GW_MASKBIT_8 );
        *( dst + size ++) = ( UNSIGNED_CHAR )(( pWaveHead->TotalPointer >> CPC_BYTE_BITS ) & GW_MASKBIT_8 );

        /*采样间隔*/
        fPeriod = ( float )( GW_RW_CIRCLE_TIME / pWaveHead->SamplePointer );
        fPeriod *= pWaveHead->Space;
        fPeriod += 0.5;  /* 浮点数偏移量 */
        period = ( UNSIGNED )fPeriod;
        *( dst + size ++) = ( UNSIGNED_CHAR )( period & GW_MASKBIT_8 );
        *( dst + size ++) = ( UNSIGNED_CHAR )(( period >> CPC_BYTE_BITS ) & GW_MASKBIT_8 );

        /*根据故障前采样点数修正采样开始时间*/
        fPeriod = ( float )( GW_RW_CIRCLE_TIME / pWaveHead->SamplePointer );
        fPeriod *= pWaveHead->Space;
        fPeriod *= pWaveHead->StartPointer;
        fPeriod /= ( float )1000.0; /* 将时间折算为毫秒 */
        fPeriod += ( float )GW_RW_PRECISION;
        period = ( UNSIGNED )fPeriod;
        /* 判断修正值与当前毫秒时间的大小 */
        if ( period > pWaveHead->Millionsecond )
        {
            /* 修正值大于当前毫秒数 */
            pWaveHead->Millionsecond += GW_TIME_MS_MAX;
            if ( pWaveHead->Minute == 0 )
            {
                pWaveHead->Minute = GW_TIME_MINUTE_MAX;
                if ( pWaveHead->Hour == 0 )
                {
                    pWaveHead->Hour = GW_TIME_HOUR_MAX;
                }
                else
                {
                    pWaveHead->Hour -= 1;
                }
            }
            else
            {
                pWaveHead->Minute -= 1;
            }
        }
        pWaveHead->Millionsecond -= period;
        *( dst + size ++)   = ( UNSIGNED_CHAR ) ( pWaveHead->Millionsecond & GW_MASKBIT_8 );
        *( dst + size ++)   = ( UNSIGNED_CHAR )(( pWaveHead->Millionsecond >> CPC_BYTE_BITS ) & GW_MASKBIT_8 );
        *( dst + size ++)   = ( UNSIGNED_CHAR )( pWaveHead->Minute );
        *( dst + size ++)   = ( UNSIGNED_CHAR )( pWaveHead->Hour );
    }
    else
    {
        return status;
    }
    return size;
}

INT gwDIST_DTAG_Ready( UNSIGNED_CHAR * dst, GW_RW_CB    * pCB )
{
    INT size;
    size    = RWC_PackReadyHead( dst, I3DEF_TYPE_RDY_DTAGS,
        0, 0, pCB );
    return size;
}

INT RWC_Trans_Result( UNSIGNED_CHAR * dst   , UNSIGNED_CHAR  ucCMD,
    UNSIGNED_CHAR  ucData, UNSIGNED_CHAR  ucResult,
    GW_RW_CB      * pCB )
{
    INT size;
    size  = 0;
    *( dst + size ++)   = I3DEF_TYPE_END_TRANS;
    *( dst + size ++)   = 0x81;     /* 可变结构限定词 */
    *( dst + size ++)   = I3DEF_COT_DISTURBANCE_DATA;
    *( dst + size ++)   = pCB->head.sect;
    *( dst + size ++)   = pCB->rw_fun;
    *( dst + size ++)   = 0;
    *( dst + size ++)   = ucCMD;     //command type
    *( dst + size ++)   = ucData;    //data type
    //fault number low byte
    *( dst + size ++)   = pCB->rw_Fault_Number & CPC_DATA_MASKS8;
    //fault number high byte
    *( dst + size ++)   = ( pCB->rw_Fault_Number >> CPC_BYTE_BITS ) & CPC_DATA_MASKS8;
    *( dst + size ++)   = ucResult;

    return size;
}

/*获得开入信息序号*/
UNSIGNED gwDIST_GetInputInf( UNSIGNED group, UNSIGNED mask_bit )
{
#if GW_RW_SIM
    /* 开入量信息序号 */
    return ( 0x1000 | ( group << CPC_BYTE_BITS ) | mask_bit );
#else
    INPUT          * pInput;
    UNSIGNED         number;
    INT              i;
    pInput  = RW_Input;
    number  = RW_TotalInput;
    for ( i = 0 ; i < ( INT )number ; i ++, pInput ++)
    {
        /*匹配组号和掩码*/
        if (( pInput->input_protect_group == group )
            && ( pInput->input_sub_protect & mask_bit ))
        {
            return pInput->input_inf;
        }
    }
    return 0;
#endif
}

/*获得开出信息序号*/
UNSIGNED gwDIST_GetOutputInf( UNSIGNED group, UNSIGNED mask_bit )
{
#if     GW_RW_SIM
    /* 开出量信息序号 */
    return ( 0x2000 | ( group << CPC_BYTE_BITS ) | mask_bit );
#else
    OUTPUT          * pOutput;
    UNSIGNED         number;
    INT              i;
    pOutput  = RW_Output;
    number   = RW_TotalOutput;
    for ( i = 0 ; i < ( INT )number ; i ++, pOutput ++)
    {
        /*匹配组号和掩码*/
        if (( pOutput->output_protect_group == group )
            && ( pOutput->output_sub_protect & mask_bit ))
        {
            return pOutput->output_inf;
        }
    }
    return 0;
#endif
}

/*获得保护状态量信息序号*/
UNSIGNED    gwDIST_GetStateInf( UNSIGNED group, UNSIGNED mask_bit )
{
#if     GW_RW_SIM
    /* 保护状态量信息序号 */
    return ( 0x4000 | ( group << CPC_BYTE_BITS ) | mask_bit );
#else
    STATE          * pState;
    UNSIGNED        number;
    INT             i;
    pState  = RW_WaveState;
    number  = RW_TotalWaveState;
    for ( i = 0 ; i < ( INT )number ; i ++, pState ++)
    {
        /*匹配组号和掩码*/
        if (( pState->state_protect_group == group )
            &&( pState->state_sub_protect & mask_bit ))
        {
            return pState->state_inf;
        }
    }
    return 0;
#endif
}

/*获得内部逻辑量信息序号*/
#define GW_DIST_LOG_GROUP_START (100 << CPC_BYTE_BITS)

UNSIGNED gwDIST_GetLogicInf( UNSIGNED group, UNSIGNED mask_bit )
{
#if     GW_RW_SIM
    /* 内部逻辑量信息序号 */
    return ( 0x8000 | ( group << CPC_BYTE_BITS ) | mask_bit );
#else
    UNSIGNED    inf;
    UNSIGNED    i;
    UNSIGNED    actual_bit;
    actual_bit = 1;
    for ( i = 0 ; i < GW_RW_DTAG_WORD_BITS ; i ++)
    {
        if ( actual_bit & mask_bit )
        {
            inf = i;
            break;
        }
        actual_bit = ( actual_bit << 1 );
    }
    inf |= ( GW_DIST_LOG_GROUP_START | ( group << CPC_GROUP_MASK32 ));

    return inf;
#endif
}

VOID gwDIST_CheckDTAG( GW_RW_CB * pCB, UNSIGNED total,
    UNSIGNED * groups, UNSIGNED * mask,
    DIST_GET_INF gwDIST_GetInf )
{
    INT i, j;
    UNSIGNED inf;
    UNSIGNED check_bit;
    UNSIGNED position;
    position = pCB->rw_DTAG_Group_Total;
    for ( i = 0 ; i < ( INT )total ; i ++, position ++)
    {
        /*获得状态掩码*/
        pCB->rw_DTAG_Mask[position] = mask[i];
        pCB->rw_DTAG_C_B[position]  = mask[i];
        check_bit = 1;
        for ( j = 0 ; j < GW_RW_DTAG_WORD_BITS ; j ++)
        {
            /*该位有效*/
            if ( check_bit & mask[i] )
            {
                /*获得信息序号*/
                inf = gwDIST_GetInf( groups[i], check_bit ) & CPC_DATA_MASKS16;
                inf = inf << ( CPC_UNIT_LENGTH * ( j & 1 ));
                /*保存信息序号*/
                pCB->rw_DTAG_Inf_Group[position * CPC_UNIT_LENGTH + ( j >> 1 )] |= inf;
                /*修正状态量个数*/
                pCB->rw_DTAG_Total ++;
            }
            check_bit = check_bit << 1;
        }
    }
    /*修正状态量总组数*/
    pCB->rw_DTAG_Group_Total = position;
    return;
}

STATUS gwDIST_InitDTAG( GW_RW_CB * pCB )
{
    INT             i;
    STATUS          status;
    CWM_WHD         WaveHead;
    CWM_WHD        * pWaveHead;

    pCB->rw_DTAG_Total = 0;
    pCB->rw_DTAG_Bits = 0;
    pCB->rw_DTAG_Group_Total = 0;
    pCB->rw_Transmit_Position = 0;
    pCB->rw_DTAG_Max = GW_RW_MAX_DTAG;
    status = GW_RW_READ(& CWave, pCB->rw_Fault_Number,& WaveHead, 0, 0,
        ( sizeof( CWM_WHD )/ sizeof( UNSIGNED )), RW_TARG_OP_TIMEOUT );
    if ( status == NU_SUCCESS )
    {
        pWaveHead = & WaveHead;
        gwDIST_AdjustWHD( pWaveHead );

        /*清空当前值*/
        for ( i = 0 ; i < GW_RW_MAX_DTAG_GROUP ; i ++)
        {
            pCB->rw_DTAG_Mask[i] = 0;
            pCB->rw_DTAG_Backup[i] = 0;
            pCB->rw_DTAG_C_B[i] = 0;
            pCB->rw_DTAG_D[i] = 0;
        }
        /*清空信息序号*/
        for ( i = 0 ; i < GW_RW_MAX_DTAG_INF ; i ++)
        {
            pCB->rw_DTAG_Inf_Group[i] = 0;
        }

        /*初始化开入量*/
        gwDIST_CheckDTAG( pCB, pWaveHead->InputNumber, pWaveHead->InputGroups,
            pWaveHead->InputActiveBits, gwDIST_GetInputInf );
        /*初始化开出量*/
        gwDIST_CheckDTAG( pCB, pWaveHead->OutputNumber, pWaveHead->OutputGroups,
            pWaveHead->OutputActiveBits, gwDIST_GetOutputInf );
        /*初始化保护状态量*/
        gwDIST_CheckDTAG( pCB, pWaveHead->ProtectNumber, pWaveHead->StateGroups,
            pWaveHead->ProtectActiveBits, gwDIST_GetStateInf );

        /*是否要全部录波状态量*/
        if ( pCB->rw_TOO == I3DEF_TOO_REQUEST_FOR_LOGS )
        {
            /*初始化内部逻辑量*/
            gwDIST_CheckDTAG( pCB, pWaveHead->LogicNumber, pWaveHead->LogicalGroups,
                pWaveHead->LogicalActiveBits, gwDIST_GetLogicInf );
            /*内部规约,多送一些状态量,减少通信次数*/
            pCB->rw_DTAG_Max = GW_RW_MAX_LOGS;
        }
        pCB->rw_CHNL_Total  = pWaveHead->AnalogNumber;
        pCB->rw_Total_Point = pWaveHead->TotalPointer;
    }
    return status;
}

GW_VOID gwDIST_RestoreDTAG( GW_RW_CB * pCB )
{
    memcpy( pCB->rw_DTAG_C_B, pCB->rw_DTAG_Backup,
        pCB->rw_DTAG_Group_Total );
    pCB->rw_Transmit_Position = pCB->rw_Transmit_Backup;
    return;
}

GW_VOID gwDIST_SaveDTAG( GW_RW_CB * pCB )
{
    memcpy( pCB->rw_DTAG_Backup, pCB->rw_DTAG_C_B,
        pCB->rw_DTAG_Group_Total );
    pCB->rw_Transmit_Backup = pCB->rw_Transmit_Position;
    return;
}

#define GW_RW_CHL_HEAD_SIZE (sizeof(CWM_VHD)/sizeof(UNSIGNED))
INT     gwDIST_DTAG_Data( UNSIGNED_CHAR * dst, GW_RW_CB * pCB )
{
    STATUS          status;
    UNSIGNED        inf;
    UNSIGNED        InputGroup[GW_RW_MAX_DTAG_GROUP];
    UNSIGNED        temp_DTAG[GW_DIST_MAX_TEMP_DTAG];
    INT             i, j;
    UNSIGNED        pre_offset;
    UNSIGNED        uGroup;
    UNSIGNED        uStatus;
    INT             flag;
    INT             size;

    gwDIST_SaveDTAG( pCB );
    /*状态量从第一位开始传输,获得初始值*/
    if ( pCB->rw_DTAG_Bits == 0 )
    {
        for ( i = 0 ; i < ( INT )pCB->rw_DTAG_Group_Total ; i ++)
        {
            status = GW_RW_READ(& CWave, pCB->rw_Fault_Number,
                & pCB->rw_DTAG_D[i], pCB->rw_Transmit_Position + GW_RW_CHL_HEAD_SIZE,
                1 + pCB->rw_CHNL_Total + i, 1, RW_TARG_OP_TIMEOUT );
            if ( status != NU_SUCCESS )
            {
                /*gwDIST_RestoreDTAG(pCB);*/
                return status;
            }
        }
    }

    size    = RWC_PackDataHead( dst, I3DEF_TYPE_TRNS_DTAG, pCB );
    *( dst + size ++)   = 0;
    //TAP   low byte
    *( dst + size ++)   = ( UNSIGNED_CHAR )( pCB->rw_Transmit_Position & CPC_DATA_MASKS8 );
    //TAP   high byte
    *( dst + size ++)   = ( UNSIGNED_CHAR )(( pCB->rw_Transmit_Position >> CPC_BYTE_BITS ) & CPC_DATA_MASKS8 );

    /* 在状态位范围之内查找实际状态位 */
    for ( i = 0 ; ( pCB->rw_DTAG_Bits < ( pCB->rw_DTAG_Group_Total * 32 ))
        && ( i < ( INT )pCB->rw_DTAG_Max )  ; pCB->rw_DTAG_Bits ++)
    {
        /* 状态量的组位置 */
        uGroup = pCB->rw_DTAG_Bits >> CPC_GROUP_MASK32;
        /* 状态量的位 */
        uStatus  = ( 1 << ( pCB->rw_DTAG_Bits & 0x1F ));
        /* 状态量变位标志 */
        uStatus &= pCB->rw_DTAG_C_B[uGroup];
        if ( uStatus )
        {
            /*清除变位发送标志*/
            pCB->rw_DTAG_C_B[uGroup] ^= uStatus;
            /*信息序号*/
            inf = pCB->rw_DTAG_Inf_Group[pCB->rw_DTAG_Bits >> 1];
            inf = ( inf >> ( CPC_UNIT_LENGTH * ( pCB->rw_DTAG_Bits & 1 ))) & CPC_DATA_MASKS16;
            *( dst + size ++)   = ( UNSIGNED_CHAR )(( inf >> CPC_BYTE_BITS ) & GW_MASKBIT_8 );
            *( dst + size ++)   = ( UNSIGNED_CHAR )( inf & GW_MASKBIT_8 );
            /*当前实际状态*/
            *( dst + size ++) = ( UNSIGNED_CHAR )(( uStatus & pCB->rw_DTAG_D[uGroup] )? 2:1 );
            i ++;
        }
    }
    /*本次发送的模拟量点数*/
    *( dst + LC_APP_INDEX_CONTEXT + 2 ) = i;

    /*检查当前采样点是否有未发送的状态变位*/
    flag    = NU_TRUE;
    /* 判断状态位是否在允许的范围之内 */
    if ( pCB->rw_DTAG_Bits < ( pCB->rw_DTAG_Group_Total * 32 ))
    {
        for ( i = 0 ; i < ( INT )pCB->rw_DTAG_Group_Total ; i ++)
        {
            if ( pCB->rw_DTAG_C_B[i] != 0 )
            {
                flag = NU_FALSE;
                break;
            }
        }
    }

    /*当前采样点所有变位均发送,检索下一个有变位的采样点*/
    if (( pCB->rw_DTAG_Bits >= ( pCB->rw_DTAG_Group_Total * 32 )) || flag )
    {
        pCB->rw_Transmit_Position ++;
        pCB->rw_DTAG_Bits = 0;

        flag    = NU_TRUE;
        pre_offset = 0;
        while (( pCB->rw_Transmit_Position < pCB->rw_Total_Point ) && flag )
        {
            /*获得可以检索的采样点*/
            pre_offset = pCB->rw_Total_Point - pCB->rw_Transmit_Position;
            if ( pre_offset > GW_DIST_MAX_TEMP_DTAG )
            {
                pre_offset = GW_DIST_MAX_TEMP_DTAG;
            }
            for ( i = 0 ; i < ( INT )pCB->rw_DTAG_Group_Total ; i ++)
            {
                /*有变位最近的点的位置*/
                InputGroup[i]   = GW_DIST_MAX_TEMP_DTAG;
                pCB->rw_DTAG_C_B[i] = 0;
                /*跳过通道头部信息*/
                status = GW_RW_READ(& CWave, pCB->rw_Fault_Number, temp_DTAG,
                    pCB->rw_Transmit_Position + GW_RW_CHL_HEAD_SIZE, 1 + pCB->rw_CHNL_Total + i,
                    pre_offset, RW_TARG_OP_TIMEOUT );
                if ( status == NU_SUCCESS )
                {
                    for ( j = 0; j < ( INT )pre_offset ; j ++)
                    {
                        pCB->rw_DTAG_C_B[i]    = temp_DTAG[j] ^ pCB->rw_DTAG_D[i];
                        pCB->rw_DTAG_C_B[i]   &= pCB->rw_DTAG_Mask[i];
                        if ( pCB->rw_DTAG_C_B[i] )
                        {
                            flag    = NU_FALSE;
                            InputGroup[i]   = j;
                            pre_offset  = j + 1;
                        }
                    }
                }
                else
                {
                    gwDIST_RestoreDTAG( pCB );
                    return status;
                }
            }
            if ( flag )
            {
                /*本次查询没有变位*/
                pCB->rw_Transmit_Position += pre_offset;
            }
            else
            {
                /*本次查询有变位*/
                pCB->rw_Transmit_Position += ( pre_offset - 1 );
                /*若变位位置较晚,清除变位标志*/
                for ( i = 0 ; i < ( INT )pCB->rw_DTAG_Group_Total ; i ++)
                {
                    if ( InputGroup[i] >= pre_offset )
                    {
                        pCB->rw_DTAG_C_B[i] = 0;
                    }
                }
                break;
            }
        }
        if ( pCB->rw_Transmit_Position >= pCB->rw_Total_Point )
        {
            if ( pCB->rw_TOO == I3DEF_TOO_REQUEST_FOR_TAGS )
            {
                pCB->rw_Step_M  = GWDIST_M_STEP_END_DTAG;
            }
            else
            {
                pCB->rw_Step_M  = GWDIST_M_STEP_END_LOGS;
            }
        }
    }

    return size;
}

STATUS gwDIST_CHNL_Ready( UNSIGNED_CHAR * dst, GW_RW_CB    * pCB )
{
    UNSIGNED        uRate;
    INT             size;
    STATUS          status;
    CWM_VHD         chl_head;
    status = GW_RW_READ(& CWave, pCB->rw_Fault_Number,& chl_head, 0,
        1 + pCB->rw_CHNL_Index, ( sizeof( CWM_VHD )/ sizeof( UNSIGNED )), RW_TARG_OP_TIMEOUT );
    if ( status == NU_SUCCESS )
    {
        size    = RWC_PackReadyHead( dst, I3DEF_TYPE_RDY_DCHAN,
            0, 1, pCB );
        *( dst + size ++)       = ( UNSIGNED_CHAR )( chl_head.head_info.analog.Inf & CPC_DATA_MASKS8 );/*Get Channel inf*/
        uRate   = LC_FLOAT_TO_IEEE( chl_head.head_info.analog.Primary_rate );
        /* 一次额定值 */
        XJP_CSC_Word_To_Byte( ( dst + size ), & uRate, 4 );
        /* 一次额定值 */
        size += 4;

        uRate   = LC_FLOAT_TO_IEEE( chl_head.head_info.analog.Secondary_rate );
        /* 二次额定值 */
        XJP_CSC_Word_To_Byte( ( dst + size ), & uRate, 4 );
        /* 二次额定值 */
        size += 4;

        uRate   = LC_FLOAT_TO_IEEE( chl_head.head_info.analog.Coefficient );
        /* 计算系数 */
        XJP_CSC_Word_To_Byte( ( dst + size ), & uRate, 4 );
        /* 计算系数 */
        size += 4;

        status = size;
    }
    return status;
}

INT gwDIST_CHNL_Data( UNSIGNED_CHAR * dst, GW_RW_CB * pCB )
{
    INT i;
    INT size;
    STATUS          status;
    UNSIGNED        uData;
    UNSIGNED        temp[GW_DIST_MAX_TEMP_DVAL];
    GW_USHORT       max_dval;
    UNSIGNED        start_position;
    GW_USHORT       require_dval;

    /*如果此通道未开始传输,初始化通道索引*/
    if ( pCB->rw_Transmit_Position == 0 )
    {
        status  = gwDIST_Inf2Offset( pCB );
        if ( status != NU_SUCCESS )
        {
            return status;
        }
    }

    /*本次可以操作的最大点数*/
    if ( pCB->head.dest_port == LC_PORT_TYPE_PRN )
    {
        max_dval = GW_DIST_MAX_TEMP_DVAL;
    }
    else
    {
        max_dval = GW_RW_MAX_POINT;
    }
    /*若剩余点数不够,操作剩余的点*/
    if (( pCB->rw_Transmit_Position + max_dval ) > pCB->rw_Total_Point )
    {
        max_dval = pCB->rw_Total_Point - pCB->rw_Transmit_Position;
    }
    /*模拟量16位压缩存储,实际操作点数减半*/
    require_dval = ( max_dval + 1 ) >> 1;/**/
    start_position = ( pCB->rw_Transmit_Position >> 1 );

    size    = 0;
    status = GW_RW_READ(& CWave, pCB->rw_Fault_Number,( VOID *)temp,
        ( sizeof( CWM_VHD )/ sizeof( UNSIGNED ))+ start_position, 1 + pCB->rw_CHNL_Index,
        require_dval, RW_TARG_OP_TIMEOUT );
    if ( status == NU_SUCCESS )
    {
        size    = RWC_PackReadyHead( dst, I3DEF_TYPE_TRNS_DVAL,
            0, 1, pCB );
        *( dst + size ++)   = pCB->rw_chl_inf & GW_MASKBIT_8;
        //point number
        *( dst + size ++)   = max_dval;
        //sample position
        *( dst + size ++)   = ( UNSIGNED_CHAR )( pCB->rw_Transmit_Position & GW_MASKBIT_8 );
        *( dst + size ++)   = ( UNSIGNED_CHAR )(( pCB->rw_Transmit_Position >> CPC_BYTE_BITS ) & GW_MASKBIT_8 );

        for ( i = 0 ; i < ( INT )max_dval ; i ++)
        {
            /*取出32位数据*/
            uData = temp[i >> 1];
            /*获得16位信息*/
            uData = ( uData >> (( i & 1 )* CPC_UNIT_LENGTH )) & GW_MASKBIT_16;

            *( dst + size ++)   = ( UNSIGNED_CHAR )( uData & GW_MASKBIT_8 );
            *( dst + size ++)   = ( UNSIGNED_CHAR )(( uData >> CPC_BYTE_BITS ) & GW_MASKBIT_8 );
        }
        pCB->rw_Transmit_Position  += max_dval;
        if ( pCB->rw_Transmit_Position >= pCB->rw_Total_Point )
        {
            pCB->rw_Step_M  = GWDIST_M_STEP_END_CHNL;
        }
    }
    else
    {
        return status;
    }

    return size;
}

INT RWC_PackDataHead( UNSIGNED_CHAR * dst     , UNSIGNED_CHAR type,
    GW_RW_CB     * pCB )
{
    INT size;
    size  = 0;
    *( dst + size ++)   = type;
    *( dst + size ++)   = 0x81; /* 可变结构限定词 */
    *( dst + size ++)   = I3DEF_COT_DISTURBANCE_DATA;
    *( dst + size ++)   = pCB->head.sect;
    *( dst + size ++)   = pCB->rw_fun;
    *( dst + size ++)   = 0;
    //fault number low byte
    *( dst + size ++)   = pCB->rw_Fault_Number & CPC_DATA_MASKS8;
    //fault number high byte
    *( dst + size ++)   = ( pCB->rw_Fault_Number >> CPC_BYTE_BITS ) & CPC_DATA_MASKS8;

    return size;
}

INT RWC_PackReadyHead( UNSIGNED_CHAR * dst     , UNSIGNED_CHAR type,
    UNSIGNED_CHAR  ucCMD   , UNSIGNED_CHAR ucData,
    GW_RW_CB     * pCB )
{
    INT size;
    size  = 0;
    *( dst + size ++)   = type;
    *( dst + size ++)   = 0x81;/* 可变结构限定词 */
    *( dst + size ++)   = I3DEF_COT_DISTURBANCE_DATA;
    *( dst + size ++)   = pCB->head.sect;
    *( dst + size ++)   = pCB->rw_fun;
    *( dst + size ++)   = 0;
    *( dst + size ++)   = ucCMD;     //command type
    *( dst + size ++)   = ucData;            //data type
    //fault number low byte
    *( dst + size ++)   = pCB->rw_Fault_Number & CPC_DATA_MASKS8;
    //fault number high byte
    *( dst + size ++)   = ( pCB->rw_Fault_Number >> CPC_BYTE_BITS ) & CPC_DATA_MASKS8;

    return size;
}

VOID RWC_ZeroTable( UNSIGNED * pTable )
{
    INT i;
    for ( i = 0; i < CPC_MAX_NVRAM_WAVE_NUMBER ; i ++)
    {
        *( pTable + i ) = 0;
    }
}

STATUS  RWC_Initialize( GW_CHANNEL * pChl )
{
    STATUS  status = NU_TRUE;

    pChl->chl_faultNumber   = 0;
    pChl->chl_disturb_transmit = NU_FALSE;
    pChl->chl_disturb_check    = NU_FALSE;
    return status;
}

INT  RWC_Check_Disturbance( GW_CHANNEL * pChl )
{
    UNSIGNED        actual_size = 0;
    UNSIGNED        table[CPC_MAX_NVRAM_WAVE_NUMBER];
    INT             ret;
    static  INT iInitFlag = NU_FALSE;   /* 初始化标识 */
    static  INT iLastWaveCount = 0;

    ret = NU_FALSE;

    RWC_ZeroTable( table );
    if ( GW_RW_FIND(& CWave, 0,( VOID *)table,
        ( INT *)& actual_size, NU_NO_SUSPEND ) == NU_SUCCESS )
    {
        if ( actual_size > 0 )
        {
            /* 记录上次录波个数 */
            iLastWaveCount = actual_size;
            if ( actual_size == pChl->chl_faultNumber )
            {
                if ( memcmp( table, pChl->chl_disturb_table, actual_size ))
                {
                    ret = NU_TRUE;
                }
            }
            else
            {
                ret = NU_TRUE;
            }
        }
        else
        {
            pChl->chl_faultNumber = 0;
            /* 当清除录波时也要上送扰动表 */
            if( iLastWaveCount > 0 )
            {
                iLastWaveCount = 0;
                iInitFlag = NU_FALSE;
            }
        }
    }

    if (( ret ) || ( iInitFlag == NU_FALSE ))
    {
        memcpy( pChl->chl_disturb_table, table, CPC_MAX_NVRAM_WAVE_NUMBER );
        pChl->chl_faultNumber = actual_size;
        iInitFlag = NU_TRUE;
        ret = NU_TRUE;
    }
    return ret;
}

INT     RWC_Fill_Disturbance( GW_CHANNEL * pChl, UNSIGNED_CHAR * dst )
{
    INT             size;
    CWM_WHD         WaveHead,* pWaveHead;
    UNSIGNED        actual_size;
    INT             i;
    UNSIGNED        table[CPC_MAX_NVRAM_WAVE_NUMBER];
    UNSIGNED        faultNumber;
    static    INT   iFillMessFlag = NU_FALSE;   /* 是否填写报文标识 */

    /* 检查是否有扰动表数据 */
    if( RWC_Check_Disturbance( pChl ) == NU_TRUE )
    {
        /* 置填写报文标识 */
        iFillMessFlag = NU_TRUE;
    }

    size        = 0;
    actual_size = 0;
    pWaveHead   = & WaveHead;
    if (( GW_RW_FIND(& CWave, 0,( VOID *)table,
        ( INT *)& actual_size, NU_NO_SUSPEND ) == NU_SUCCESS )
        && ( iFillMessFlag == NU_TRUE ))
    {
        *( dst + size ++) = I3DEF_TYPE_LST_DSTRB;
        *( dst + size ++) = 0x0;
        *( dst + size ++) = I3DEF_COT_DISTURBANCE_DATA;
        *( dst + size ++) = LC_LINK_ADDRESS;
        *( dst + size ++) = LC_GLOBAL_FUN;
        *( dst + size ++) = 0;
        /* 判断录波个数是否为0 */
        if( actual_size == 0 )
        {
            /* 清填写报文标识 */
            iFillMessFlag = NU_FALSE;
        }
        for ( i = 0; ( i < RW_MAX_DISTURBANCE ) && ( i < ( INT )actual_size ) ; i ++)
        {
            faultNumber = table[actual_size - i - 1];
            if  ( GW_RW_READ(& CWave, faultNumber,& WaveHead, 0, 0, ( sizeof( CWM_WHD )/ sizeof( UNSIGNED )),
                RW_TARG_OP_TIMEOUT ) == NU_SUCCESS )
            {
                gwDIST_AdjustWHD( pWaveHead );
                *( dst + size ++) = ( UNSIGNED_CHAR )( faultNumber & GW_MASKBIT_8 );
                *( dst + size ++) = ( UNSIGNED_CHAR )(( faultNumber >> CPC_BYTE_BITS ) & GW_MASKBIT_8 );
                *( dst + size ++) = ( UNSIGNED_CHAR )( pWaveHead->Sof & GW_MASKBIT_8 );
                *( dst + size ++) = ( UNSIGNED_CHAR )( pWaveHead->Millionsecond & GW_MASKBIT_8 );
                *( dst + size ++) = ( UNSIGNED_CHAR )(( pWaveHead->Millionsecond >> CPC_BYTE_BITS ) & GW_MASKBIT_8 );
                *( dst + size ++) = ( UNSIGNED_CHAR )( pWaveHead->Minute & GW_MASKBIT_8 );
                *( dst + size ++) = ( UNSIGNED_CHAR )( pWaveHead->Hour & GW_MASKBIT_8 );
                *( dst + size ++) = ( UNSIGNED_CHAR )( pWaveHead->Day & GW_MASKBIT_8 );
                *( dst + size ++) = ( UNSIGNED_CHAR )( pWaveHead->Month & GW_MASKBIT_8 );
                *( dst + size ++) = ( UNSIGNED_CHAR )( pWaveHead->Year & GW_MASKBIT_8 );
                /* 清填写报文标识 */
                iFillMessFlag = NU_FALSE;
            }
            else
            {
                size    = 0;
                break;
            }
        }
        *( dst + 1 )    = ( UNSIGNED_CHAR )i;
    }

    return size;
}

STATUS gwDIST_Inf2Offset( GW_RW_CB * pCB )
{
    INT             i;
    STATUS          status;
    CWM_WHD         WaveHead;
    CWM_WHD        * pWaveHead;
    INT             index;

    status = GW_RW_READ(& CWave, pCB->rw_Fault_Number,& WaveHead, 0, 0,
        ( sizeof( CWM_WHD )/ sizeof( UNSIGNED )), RW_TARG_OP_TIMEOUT );
    if ( status == NU_SUCCESS )
    {
        pWaveHead = & WaveHead;
        gwDIST_AdjustWHD(& WaveHead );
        index = GW_RW_INVALID_CHL_INDEX;
        for ( i = 0; i < ( INT )pWaveHead->AnalogNumber ; i ++)
        {
            if (( pWaveHead->ChannelInf[i] & CPC_DATA_MASKS8 ) == pCB->rw_chl_inf )
            {
                index = i;
            }
        }
        pCB->rw_CHNL_Index = index;
        if ( WaveHead.TotalPointer > CPC_MAX_WAVE_DATA_SIZE )
        {
            pCB->rw_Total_Point = CPC_MAX_WAVE_DATA_SIZE;
        }
        else
        {
            pCB->rw_Total_Point = WaveHead.TotalPointer;
        }

        if ( index == GW_RW_INVALID_CHL_INDEX )
        {
            status = NU_END_OF_LOG;
        }
    }
    return status;
}

STATUS  gwDIST_GetTotalCHNL( GW_RW_CB * pCB )
{
    STATUS          status;
    CWM_WHD         WaveHead;

    pCB->rw_CHNL_Total  = 0;
    status = GW_RW_READ(& CWave, pCB->rw_Fault_Number,& WaveHead, 0, 0,
        ( sizeof( CWM_WHD )/ sizeof( UNSIGNED )), RW_TARG_OP_TIMEOUT );
    if ( status == NU_SUCCESS )
    {
        gwDIST_AdjustWHD(& WaveHead );
        pCB->rw_CHNL_Total = WaveHead.AnalogNumber;
    }
    return status;
}

STATUS gwDIST_GetNextCHNL( GW_RW_CB * pCB )
{
    INT             i;
    STATUS          status;
    CWM_WHD         WaveHead;
    CWM_WHD        * pWaveHead;
    INT             index;

    status = GW_RW_READ(& CWave, pCB->rw_Fault_Number,& WaveHead, 0, 0,
        ( sizeof( CWM_WHD )/ sizeof( UNSIGNED )), RW_TARG_OP_TIMEOUT );
    if ( status == NU_SUCCESS )
    {
        pWaveHead = & WaveHead;
        gwDIST_AdjustWHD( pWaveHead );
        for ( i = 0; i < ( INT )pWaveHead->AnalogNumber ; i ++)
        {
            if (( pWaveHead->ChannelInf[i] & CPC_DATA_MASKS8 ) == pCB->rw_chl_inf )
            {
                index = i;
            }
        }
        pCB->rw_CHNL_Total = pWaveHead->AnalogNumber;
        pCB->rw_CHNL_Index = index + 1;
    }
    return status;
}
