
#include "plat/net_inc/lc_extr.h"
#include "plat/net_inc/ls_extr.h"
#include "plat/es_plat.h"

/* ע��ͨѶ����� */
INT LCC_COMM_RegisterCommand( VOID * pChannel, UNSIGNED_CHAR type, void * callback )
{
    GW_COMMAND_CALLBACK_NODE * pNode;
    GW_CHANNEL  * pChl;

    pChl = ( GW_CHANNEL *)pChannel;
    pNode   = & pChl->chl_callback_List[type & GW_MASKBIT_8];
    pNode->Callback = ( GWAPP_ACK_FUNCTION )callback;
#if     LC_STAT
    pNode->call_count = 0;
#endif
    return NU_TRUE;
}

/* ��ʱ���� */
INT LCC_COMM_ProcessTimeSyn( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port )
{
    UNSIGNED        i, originate;
    TIME            * pTime;
    TIME    RunClockTime;
    UNSIGNED_CHAR   * dst    = ( UNSIGNED_CHAR *)p_dst;
    UNSIGNED_CHAR   * src    = ( UNSIGNED_CHAR *)p_src;

    pTime = ( TIME *)& RunClockTime;
    /* ���·���������Ϊ���� */
    pTime -> Millionsecond = *( src + LCC_OFFSET_CONTEXT + 1 ) * 0x100 + *( src + LCC_OFFSET_CONTEXT + 0 );
    pTime -> Minute = *( src + LCC_OFFSET_CONTEXT + 2 ); /* �� */
    pTime -> Hour = *( src + LCC_OFFSET_CONTEXT + 3 );   /* ʱ */
    pTime -> Day = (*( src + LCC_OFFSET_CONTEXT + 4 )  & 0x1f );/* �� */
    pTime -> Month = (*( src + LCC_OFFSET_CONTEXT + 5 ));       /* �� */
    pTime -> Year = *( src + LCC_OFFSET_CONTEXT + 6 );          /* �� */
    pTime -> Nanosecond = 0;
    originate = ( INT )(*( src - 1 ) & LC_VAR_CONTROL_BITS );
    /* ���û�н��չ�61850��ʱ����ߴ˶�ʱ��ʽ */
    if( (!( XJP_TM_Get_Init_Flag())) || ( originate == LC_PORT_TYPE_PC ))
    {
        /* ���ñ���ʱ��8ʱ�� */
        XJP_TM_Set_Time_Dev_Local(& RunClockTime );
    }

    /* �ж��Ƿ�Ϊ�㲥����,�㲥�����Ӧ�� */
    if (*( src + LCC_OFFSET_SECT ) != I870FT12_BROADCAST_ADDRESS )
    {
        * dst ++ = LCC_LEN_ASDU6;
        src ++;

        for( i = 0; i < LCC_LEN_ASDU6; i ++)
        {
            * dst ++ = * src ++;
        }
        return  ( COM_DATA_FULL | COM_DATA_FIRST );
    }
    else
    {
        return COM_DATA_EMPTY;
    }
}

/* �ϵ�����ģ����ͣ���� */
INT LCC_COMM_ProcessStartDT( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port )
{
    UNSIGNED_CHAR   * dst    = ( UNSIGNED_CHAR *)p_dst;
    UNSIGNED_CHAR   * src    = ( UNSIGNED_CHAR *)p_src;

    * dst ++  = 3;                            /* ���ĳ��� */
    * dst ++  = *( src + LCC_OFFSET_TI );     /* �������� */
    * dst ++  = LC_LINK_ADDRESS;              /* װ�õ�ַ */
    * dst ++  = *( src + LCC_OFFSET_TI + 2 ); /* �ϵ�����ģ���־(��/ͣ) */
    /* �ж��ϵ�����ģ���־ */
    if (*( src + LCC_OFFSET_TI + 2 ) == LC_START_DT )
    {
        pChl->chl_initialize_finish = NU_FALSE;
        pChl->chl_initialize_timeout = LC_STARTDT_LONG_TIMEOUT;
        pChl->chl_initialize_start = TMD_System_Clock;
    }
    else
    {
        pChl->chl_initialize_finish = NU_TRUE;
    }
    return COM_DATA_FULL;
}

/* ASDU_5�ϵ�װ�ð汾�� */
INT LCC_COMM_ProcessVersion( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port )
{
    UNSIGNED_CHAR   * dst    = ( UNSIGNED_CHAR *)p_dst;
    UNSIGNED_CHAR   * src    = ( UNSIGNED_CHAR *)p_src;
    UNSIGNED        crccode;

    * dst ++      = LCC_LEN_ASDU5;
    * dst ++      = I3DEF_TYPE_IDENT;
    * dst ++      = 0x81;                      /* �ɱ�ṹ�޶��� */
    * dst ++      = I3DEF_COT_POS_COMMAND_ACK;
    * dst ++      = LC_LINK_ADDRESS;
    * dst ++      = 0;
    * dst ++      = IEC103_INF_LoadVer;
    * dst ++      = 2;                     //COL
    XJP_CSC_Word_To_Byte( dst, LC_GET_DEVICE_ENAME, DEVICE_ENAME );
    dst        += DEVICE_ENAME;

    /* �汾�������ݱ���,����ͨ���ӿ����� */
    * dst ++   = 0;
    * dst ++   = 0;
    crccode  = LC_GET_DEV_CRC;
    * dst ++   = ( UNSIGNED_CHAR )( crccode & GW_MASKBIT_8 );
    /* ��ȡװ��crc��ĸ�8λ */
    * dst ++   = ( UNSIGNED_CHAR )(( crccode >> 8 ) & GW_MASKBIT_8 );

    return COM_DATA_FULL;
}

/* ʱ�ӳ�ʼ������ */
VOID LCC_COMM_ClockInitialize( GW_CHANNEL * pChl )
{
    UNSIGNED_CHAR   * pSrc;
    GW_SHORT         size;
    GW_UCHAR        * dst;
    INT             Status;
    UNSIGNED        uDelay;
    TIME            * pTime;
    INT             read_size;
    extern CHAR     ClockRunAsm[];                                  // timer data

    Status = NU_FALSE;
    uDelay = TMD_System_Clock;
    /* �ϵ��ʼ��,��ʱ�ȴ�ʱ�� */
    while((( TMD_System_Clock - uDelay ) < 5000 )
        && ( Status != NU_TRUE ))
    {
        read_size = LCC_Link_Read( pChl, LON_RxdBufferRem );
        if( read_size > 0 )
        {
            if( LCC_MessM_CheckFrame( LON_RxdBufferRem, 0 ))
            {
                pSrc = ( UNSIGNED_CHAR *)LON_RxdBufferRem;
                /*�ж��������� */
                switch(*( pSrc + 6 ) & 0x0ff )
                {
                case I3DEF_TYPE_TIME_SYNC:
                    pTime = ( TIME *)ClockRunAsm;
                    pTime -> Millionsecond = *( pSrc + 13 )* 0x100 + *( pSrc + 12 ); /* ���� */
                    pTime -> Minute = *( pSrc + 14 );        /* �� */
                    pTime -> Hour = *( pSrc + 15 );          /* ʱ */
                    pTime -> Day = (*( pSrc + 16 ) & 0x1f ); /* �� */
                    pTime -> Month = (*( pSrc + 17 ));       /* �� */
                    pTime -> Year = *( pSrc + 18 );          /* �� */
                    Status = NU_TRUE;
                    break;
                default:
                    if (*( pSrc + I870FT12_INDEX_VAR_ADDRESS ) == LC_LINK_ADDRESS )
                    {
                        dst = LON_TxdBufferRem;
                        LCC_COMM_ProcessVersion( pChl,( dst + I870FT12_INDEX_VAR_ADDRESS ), GW_NULL, 0 );
                        size = *( dst + I870FT12_INDEX_VAR_ADDRESS );
                        *( dst + I870FT12_INDEX_VAR_ADDRESS + LCC_OFFSET_COT ) = I3DEF_COT_RESTART;
                        LCC_MessM_PackHead( size, dst, LC_PORT_TYPE_MENU );
                        size = LCC_MessM_CalculateSum( dst );
                        LCC_Link_Write( pChl, dst, size );
                    }
                    break;
                }
            }
        }
    }
}

/* �ϵ�lon�汾�� */
INT LCC_COMM_ProcessLonVersion( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port )
{
    UNSIGNED_CHAR   * dst    = ( UNSIGNED_CHAR *)p_dst;
    UNSIGNED_CHAR   * src    = ( UNSIGNED_CHAR *)p_src;
    * dst ++  = 4; /* ����� */
    * dst ++  = *( src + LCC_OFFSET_TI );
    * dst ++  = LC_LINK_ADDRESS;
    * dst ++  = 0;
    * dst ++  = 0;
    return COM_DATA_FULL;
}
