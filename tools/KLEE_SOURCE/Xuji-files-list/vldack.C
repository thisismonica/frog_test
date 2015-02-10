
#include "plat/net_inc/vld_extr.h"
#include "plat/net_inc/lc_defs.h"
#include "plat/net_inc/lc_extr.h"
#include "plat/net_inc/ls_extr.h"
#include "plat/net_inc/lc_extr.h"
#include "plat/net_inc/commu.h"
#include "plat/net_inc/ied_defs.h"
#include "plat/net_inc/ied_extr.h"

VOID VLD_RegisterCommand_VLD( GW_CHANNEL * pChl );

XJ_VLD_MENU _lcc_VLD_Menu;
/* 注册上调vld数据所需的函数 */
VOID VLD_RegisterCommand_VLD( GW_CHANNEL * pChl )
{
    /*=======================================================*/
    /* 上载VLD数据帧数和CRC码 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_84, VLD_ProcessVldCFG );
    /* 判断装置的功能选项中是否有背景数据功能 */
    if ( LC_FUN_BACKGROUND )
    {
        /* 上载VLD数据 */
        LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_85, VLD_ProcessVldData );
    }
    /* 上载背景数据 */
    LCC_COMM_RegisterCommand( pChl, IEC103_TI_M_86, VLD_ProcessVldMenu );
}

/* 上载VLD数据帧数和CRC码 */
INT VLD_LoadConfig( UNSIGNED_CHAR * dst, GW_CHANNEL   * pChl, VOID  * pCB )
{
    STATUS      status;
    UNSIGNED    length;
    INT         size;
    INT         data_available;
    GW_VLD_CB  * pVldCB;
    /*=======================================================*/

    pVldCB = ( GW_VLD_CB *)pCB;
    if ( LC_FUN_BACKGROUND )
    {
        /* 读取背景数据 */
        status = LC_READ_BACKGROUND(& _lcc_VLD_Menu, 0,
            (( sizeof( XJ_VLD_MENU ) + sizeof( UNSIGNED ) - 1 )/ sizeof( UNSIGNED )), 100 );
        if ( status == NU_SUCCESS )
        {
            if ( _lcc_VLD_Menu.available == VLD_MENU_AVALABLE )
            {
                /*数据有效*/
                data_available = VLD_DATA_OK;
            }
            else
            {
                /*数据格式不对,需要清零*/
                data_available = VLD_DATA_ERROR;
            }
        }
        else if ( status != NU_TIMEOUT )
        {
            /*数据格式不对,需要清零*/
            data_available = VLD_DATA_ERROR;
        }
        else
        {
            /*超时,下次再读*/
            data_available = VLD_DATA_TIMEOUT;
        }
    }
    else
    {
        /*不支持背景数据,需要清零*/
        data_available = 1;
    }

    size    = 0;
    if ( data_available != VLD_DATA_TIMEOUT )
    {
        /* 若读取的数据错误,清除一些数据信息 */
        if ( data_available == VLD_DATA_ERROR )
        {
            _lcc_VLD_Menu.scrNumber = 0;
            _lcc_VLD_Menu.length = 0;
            _lcc_VLD_Menu.crc = 0;
            _lcc_VLD_Menu.config_CRC = 0;
        }

        *( dst + size ++) = IEC103_TI_M_84;    /* 命令类型 */
        *( dst + size ++) = pVldCB->head.sect; /* cpu号 */
        *( dst + size ++) = ( UNSIGNED_CHAR )pVldCB->Serial_L;  /* 序列号低字节 */
        *( dst + size ++) = ( UNSIGNED_CHAR )pVldCB->Serial_H;  /* 序列号高字节 */

        /* 背景数据存储宽度(字节数) */
        *( dst + size ++)   = VLD_DATA_SIZE_4;
        /* 开关数据存储宽度(字节数) */
        *( dst + size ++)   = VLD_DATA_SIZE_4;
        /* 模拟量数据存储宽度(字节数) */
        *( dst + size ++)   = VLD_DATA_SIZE_4;

        /* VLD数据总窗口页数 */
        *( dst + size ++) = ( UNSIGNED_CHAR )_lcc_VLD_Menu.scrNumber;
        /* VLD背景数据总地址长度 */
        length = ( UNSIGNED )_lcc_VLD_Menu.length;

        /* VLD背景数据总地址长度 */
        XJP_CSC_Word_To_Byte( ( dst + size ),& length, VLD_DATA_SIZE_2 );
        size += VLD_DATA_SIZE_2;

        /* VLD背景数据的CRC码 */
        XJP_CSC_Word_To_Byte( ( dst + size ),&( _lcc_VLD_Menu.crc ), VLD_DATA_SIZE_4 );
        size += VLD_DATA_SIZE_4;

        /* VLD配置数据的CRC码 */
        XJP_CSC_Word_To_Byte( ( dst + size ),&( _lcc_VLD_Menu.config_CRC ), VLD_DATA_SIZE_4 );
        size += VLD_DATA_SIZE_4;

        LCC_CB_freeAckCB( pChl, pCB );
    }
    return size;
}

/* 上载VLD数据 */
INT VLD_AckData( UNSIGNED_CHAR * dst, GW_SOE * pSoe )
{
    INT size;
    INT i;
    INT number;
    /*=======================================================*/
    /* 本帧地址个数 */
    number = pSoe->num - 2;  /* 前两个数据已经在命令中单独传输 */
    /* 报文长度计数器 */
    size = 0;
    *( dst + size ++) = IEC103_TI_M_85;   /* 命令类型 */
    *( dst + size ++) = LC_LINK_ADDRESS;  /* cpu号 */
    *( dst + size ++) = pSoe->Serial_L;   /* 序列号低字节 */
    *( dst + size ++) = pSoe->Serial_H;   /* 序列号高字节 */
    *( dst + size ++) = ( UNSIGNED_CHAR )( pSoe->data[0].pid & VLD_DATA_MASKS8 );          /* VLD动态数据窗口页序号 */
    *( dst + size ++) = ( UNSIGNED_CHAR )( pSoe->data[0].data.pvUndef & VLD_DATA_MASKS8 ); /* VLD数据类型 */

    /* VLD数据地址总长度 */
    XJP_CSC_Word_To_Byte( ( dst + size ), &( pSoe->data[1].pid ), VLD_DATA_SIZE_2 );
    size += VLD_DATA_SIZE_2;

    /* VLD数据起始地址序号(从0开始) */
    XJP_CSC_Word_To_Byte( ( dst + size ),&( pSoe->data[1].data.pvUndef ), VLD_DATA_SIZE_2 );
    size += VLD_DATA_SIZE_2;

    /* 本帧地址个数 */
    *( dst + size ++)   = ( UNSIGNED_CHAR )number;
    for ( i = 0 ; i < number ; i ++)
    {
        /* 数据 */
        XJP_CSC_Word_To_Byte( ( dst + size ), &( pSoe->data[2 + i].data.pvUndef ), VLD_DATA_SIZE_4 );
        size += VLD_DATA_SIZE_4;
    }
    return size;
}

/* 上载背景数据 */
INT VLD_LoadMenu( UNSIGNED_CHAR * dst, GW_CHANNEL   * pChl, VOID  * pCB )
{
    UNSIGNED_CHAR _lcc_temp[VLD_ONE_MAXDATA_SIZE];
    UNSIGNED_CHAR number;
    UNSIGNED_CHAR offset;
    INT           i;
    INT           data_available;
    STATUS        status;
    INT           size;
    GW_VLD_CB    * pVldCB;
    /*=======================================================*/
    pVldCB = ( GW_VLD_CB *)pCB;
    if ( LC_FUN_BACKGROUND )
    {
        offset = pVldCB->offset;
        number = pVldCB->number;
        /* 读取背景数据 */
        status = LC_READ_BACKGROUND( _lcc_temp, offset + 1, number, 100 );
        if ( status == NU_SUCCESS )
        {
            /*数据有效*/
            data_available = VLD_DATA_OK;
        }
        else if ( status != NU_TIMEOUT )
        {
            /*数据格式不对,需要清零*/
            data_available = VLD_DATA_ERROR;
        }
        else
        {
            /*超时,下次再读*/
            data_available = VLD_DATA_TIMEOUT;
        }
    }
    else
    {
        /*不支持背景数据,需要清零*/
        data_available = VLD_DATA_ERROR;
    }

    size = 0;
    if ( data_available != VLD_DATA_TIMEOUT )
    {
        if ( data_available == VLD_DATA_ERROR )
        {
            number = 0;
        }
        *( dst + size ++) = IEC103_TI_M_86;    /* 命令类型 */
        *( dst + size ++) = pVldCB->head.sect; /* cpu号 */
        *( dst + size ++) = pVldCB->Serial_L;  /* 序列号低字节 */
        *( dst + size ++) = pVldCB->Serial_H;  /* 序列号高字节 */
        /* 背景数据起始序号 */
        XJP_CSC_Word_To_Byte( ( dst + size ), &( pVldCB->offset ), VLD_DATA_SIZE_2 );
        size += VLD_DATA_SIZE_2;
        /* 背景数据长度(字节数) */
        *( dst + size ++) = number ;
        for ( i = 0; i < number ; i ++)
        {
            /* 背景数据 */
            XJP_CSC_Word_To_Byte( ( dst + size ), &( _lcc_temp[i] ), VLD_DATA_SIZE_4 );
            size += VLD_DATA_SIZE_4;
        }
        LCC_CB_freeAckCB( pChl, pCB );
    }
    return size;
}

/* 读取VLD数据程序 */
INT VLD_LoadData( GW_CHANNEL * pChl, GW_COMMAND * pCmd, UNSIGNED_CHAR * src, INT port )
{
    pCmd->type = GW_COMMAND_TYPE_VLD_DATA;
    pCmd->src_addr = port;
    pCmd->num = 1;
    pCmd->RII = ( UNSIGNED_CHAR )( pChl->chl_vld_rii );
    pCmd->Serial_L = *( src + VLD_LOCAL_OFFSET_SERIAL_L ) & VLD_DATA_MASKS8;/* 序列号低字节 */
    pCmd->Serial_H = *( src + VLD_LOCAL_OFFSET_SERIAL_H ) & VLD_DATA_MASKS8;/* 序列号高字节 */
    /* VLD动态数据窗口页数 */
    pCmd->data[0].pid = *( src + VLD_LOCAL_OFFSET_CONTEXT ) & VLD_DATA_MASKS8;
    /* VLD数据类型 */
    pCmd->data[0].data.pvUndef = *( src + VLD_LOCAL_OFFSET_CONTEXT + 1 ) & VLD_DATA_MASKS8;
    iedDB_WriteCommand( pCmd, LC_IEDDB_TIMEOUT );
    return COM_DATA_EMPTY;
}

/*======================================================================*/
/* 通讯命令函数 */
/* 上载VLD数据帧数和CRC码 */
INT VLD_ProcessVldCFG( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port )
{
    UNSIGNED_CHAR * dst    = ( UNSIGNED_CHAR *)p_dst;
    UNSIGNED_CHAR * src    = ( UNSIGNED_CHAR *)p_src;
    GW_VLD_CB     * pCB;
    /*=======================================================*/
    pCB = ( GW_VLD_CB *)LCC_CB_CreateActiveCB( pChl, sizeof( GW_VLD_CB ),
        0, LC_LINK_ADDRESS, LC_PRIORITY_VLDCFG, dst_port,
        VLD_LoadConfig, LC_TIMEOUT_VLDCFG );
    if ( pCB )
    {
        pCB->Serial_L = *( src + VLD_LOCAL_OFFSET_SERIAL_L ) & VLD_DATA_MASKS8; /* 序列号低字节 */
        pCB->Serial_H = *( src + VLD_LOCAL_OFFSET_SERIAL_H ) & VLD_DATA_MASKS8; /* 序列号高字节 */
    }
    return COM_DATA_EMPTY;
}

/* 上载VLD数据 */
INT VLD_ProcessVldData( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port )
{
    UNSIGNED_CHAR   * dst    = ( UNSIGNED_CHAR *)p_dst;
    UNSIGNED_CHAR   * src    = ( UNSIGNED_CHAR *)p_src;
    /*=======================================================*/
    pChl->chl_vld_rii ++;
    return VLD_LoadData( pChl,( GW_COMMAND *)dst, src, dst_port );
}

/* 上载背景数据 */
INT VLD_ProcessVldMenu( GW_CHANNEL * pChl, VOID * p_dst, VOID * p_src, UNSIGNED_CHAR dst_port )
{
    UNSIGNED_CHAR * dst = ( UNSIGNED_CHAR *)p_dst;
    UNSIGNED_CHAR * src = ( UNSIGNED_CHAR *)p_src;
    GW_VLD_CB     * pCB;
    /*=======================================================*/
    pCB = ( GW_VLD_CB *)LCC_CB_CreateActiveCB( pChl, sizeof( GW_VLD_CB ),
        0, LC_LINK_ADDRESS, LC_PRIORITY_VLDMENU, dst_port,
        VLD_LoadMenu, LC_TIMEOUT_VLDMENU );
    if ( pCB )
    {
        pCB->Serial_L = *( src + VLD_LOCAL_OFFSET_SERIAL_L ) & VLD_DATA_MASKS8;  /* 序列号低字节 */
        pCB->Serial_H = *( src + VLD_LOCAL_OFFSET_SERIAL_H ) & VLD_DATA_MASKS8;  /* 序列号高字节 */
        pCB->offset = BYTE_PTR_TO_16(( src + VLD_LOCAL_OFFSET_CONTEXT ));        /* 背景数据起始序号 */
        pCB->number = *( src + VLD_LOCAL_OFFSET_CONTEXT + 2 ) & VLD_DATA_MASKS8; /* 背景数据长度 */
    }
    return COM_DATA_EMPTY;
}
