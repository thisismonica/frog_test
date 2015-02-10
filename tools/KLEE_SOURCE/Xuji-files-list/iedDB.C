/* 测控数据信息处理 */
#include <string.h>
#include <math.h>
#include "plat/net_inc/ieddb.h"
#include "plat/net_inc/ied_defs.h"
#include "plat/net_inc/ied_extr.h"
#include "plat/es_plat.h"

/* SOE巡检优先级 */
static INT IED_SOEProirityTable[IED_SOE_PROIRITY_BUFF_SIZE];
/* SOE巡检优先级 */
static INT IED_CommandProirityTable[IED_COMMAND_PROIRITY_BUFF_SIZE];

extern HDCM FckPrimarySOEHandle; /* 测控SOE管理句柄(一级数据) */
extern HDCM FckSecondSOEHandle;  /* 测控SOE管理句柄(二级数据) */
extern HDCM FckPrimaryCommandHandle; /* 测控用户命令管理句柄(一级数据) */
extern HDCM FckSecondCommandHandle;  /* 测控用户命令管理句柄(二级数据) */
/* 测控数据保护句柄 */
extern NU_PROTECT * pFck_Protect;
#define IEDDB_FCK_PROTECT()   NU_Protect(pFck_Protect) /* 测控数据保护 */
#define IEDDB_FCK_UNPROTECT() NU_Unprotect()           /* 释放测控数据保护 */
/* FckSoe数据句柄 */
HDCM IEDDB_FckSoeHandle[IED_FCK_SOE_MAX_HANDLE] =
{
    ( HDCM )0,
    ( HDCM )0,
};

/* FckCommand句柄 */
HDCM IEDDB_FckCommandHandle[IED_FCK_COMMAND_MAX_HANDLE] =
{
    ( HDCM )0,
    ( HDCM )0,
};

/* 写测控SOE数据函数 */
INT iedDB_WriteSOE( VOID * pBuffer, UNSIGNED timeout )
{
    INT       ret;
    UNSIGNED  actual_size;
    GW_USHORT num;
    UNSIGNED  Temp[IED_FCK_SOE_BUFFER_LENGTH];  /* 临时缓冲区 */
    GW_SOE    * pFckSoe;
    INT       iStatus;
    /*================================================*/
    pFckSoe = ( GW_SOE *)pBuffer;
    if( pFckSoe->num > GW_SOE_MAX_DATA )
    {
        return( GW_STATUS_WRITE_TIMEOUT );
    }
    num = pFckSoe->num;
    actual_size  = (( UNSIGNED_CHAR *)pFckSoe->data )-(( UNSIGNED_CHAR *)pFckSoe );
    actual_size += num * sizeof( GW_POINT_VALUE );

    if( actual_size > IED_FCK_SOE_SIZE )
    {
        /* 如果报文长度超过允许的最大报文长度,直接返回失败 */
        return( GW_STATUS_WRITE_TIMEOUT );
    }
    /* 首地址存储的为报文大小 */
    Temp[0] = ((( actual_size + sizeof( UNSIGNED )- 1 )/ sizeof( UNSIGNED )) + IED_SOE_DATA_OFFSET );
    /* 数据实际长度 */
    Temp[1] = actual_size;
    if( Temp[0] >  IED_FCK_SOE_BUFFER_LENGTH )
    {
        /* 数据长度超过管理缓冲区的报文长度,返回失败 */
        return( GW_STATUS_WRITE_TIMEOUT );
    }
    memcpy(&( Temp[IED_SOE_DATA_OFFSET] ), pFckSoe, actual_size );
    ret = GW_STATUS_WRITE_TIMEOUT;
    switch( pFckSoe->type )
    {
    case GW_SOE_TYPE_VLD_DATA:   /* VLD数据 */
    case GW_SOE_TYPE_LOCK_DATA:  /* 互锁数据 */
        IEDDB_FCK_PROTECT();  /* 测控数据保护 */
        iStatus = XJP_Queue_Write( FckSecondSOEHandle, Temp );
        IEDDB_FCK_UNPROTECT();/* 释放测控数据保护 */
        if( iStatus == NU_SUCCESS )
        {
            ret = GW_STATUS_SUCCESS;
        }
        break;
    case GW_SOE_TYPE_MEASURE:
        ret = GW_STATUS_WRITE_TIMEOUT;
        if ( pFckSoe->cot == GW_COT_M_OUTBURST )  /* 遥测突变上送模式 */
        {
            IEDDB_FCK_PROTECT();  /* 测控数据保护 */
            iStatus = XJP_Queue_Write( FckPrimarySOEHandle, Temp );
            IEDDB_FCK_UNPROTECT();/* 释放测控数据保护 */
            if( iStatus == NU_SUCCESS )
            {
                ret = GW_STATUS_SUCCESS;
            }
        }
        else
        {
            IEDDB_FCK_PROTECT();  /* 测控数据保护 */
            iStatus = XJP_Queue_Write( FckSecondSOEHandle, Temp );
            IEDDB_FCK_UNPROTECT();/* 释放测控数据保护 */
            if( iStatus == NU_SUCCESS )
            {
                ret = GW_STATUS_SUCCESS;
            }
        }
        break;
    default:
        break;
    }
    return ret;
}

/* 读测控SOE数据函数 */
INT iedDB_ReadSOE( VOID * pBuffer, UNSIGNED timeout )
{
    static INT offset = 0;
    HDCM       FckSoeHandle[IED_FCK_SOE_MAX_HANDLE];
    INT        ret;
    INT        i;
    UNSIGNED   Temp[IED_FCK_SOE_BUFFER_LENGTH];  /* 临时缓冲区 */
    GW_SOE     * pFckSoe;
    INT        iStatus;
    /*================================================*/
    IEDDB_FCK_PROTECT();  /* 测控数据保护 */
    memset( Temp, 0,( sizeof( UNSIGNED ) * IED_FCK_SOE_BUFFER_LENGTH ));
    pFckSoe = ( GW_SOE *)pBuffer;
    /* 将管理句柄按优先级重新排序 */

    /* 将管理句柄按优先级重新排序 */
    for( i = 0; i < IED_FCK_SOE_MAX_HANDLE; i ++)
    {
        FckSoeHandle[i] = IEDDB_FckSoeHandle[IED_SOEProirityTable[( offset + i ) % IED_SOE_PROIRITY_BUFF_SIZE]];
    }
    /* 调整优先级缓冲区计数器 */
    offset = (( offset + 1 ) % IED_SOE_PROIRITY_BUFF_SIZE );
    ret = GW_STATUS_NO_SOE;
    for ( i = 0 ; i < IED_FCK_SOE_MAX_HANDLE ; i ++)
    {
        /* 从指定句柄读取数据 */
        iStatus = XJP_Queue_Read( FckSoeHandle[i] , Temp );
        if( iStatus == NU_SUCCESS )
        {
            if( Temp[1] <= IED_FCK_SOE_SIZE )
            {
                memcpy( pFckSoe, & Temp[IED_SOE_DATA_OFFSET], Temp[1] );
                ret = GW_STATUS_SUCCESS;
                break;
            }
        }
    }
    IEDDB_FCK_UNPROTECT();/* 释放测控数据保护 */
    return ret;
}

/* 存储用户命令函数 */
INT iedDB_WriteCommand( VOID * pBuffer, UNSIGNED timeout )
{
    INT       ret;
    UNSIGNED  actual_size;
    GW_USHORT num;
    UNSIGNED  Temp[IED_READ_COMMAND_BUFFER_LENGTH];  /* 临时缓冲区 */
    GW_COMMAND * pCommand;
    INT        iStatus;
    /*================================================*/
    pCommand = ( GW_COMMAND *)pBuffer;

    if( pCommand->num > GW_COMMAND_MAX_DATA )
    {
        return( GW_STATUS_WRITE_TIMEOUT );
    }
    num = pCommand->num;

    actual_size  = (( UNSIGNED_CHAR *)pCommand->data )-(( UNSIGNED_CHAR *)pCommand );
    actual_size += num * sizeof( GW_POINT_VALUE );

    if( actual_size > IED_READ_COMMAND_SIZE )
    {
        /* 如果报文长度超过允许的最大报文长度,直接返回失败 */
        return( GW_STATUS_WRITE_TIMEOUT );
    }
    /* 首地址存储的为报文大小 */
    Temp[0] = ((( actual_size + sizeof( UNSIGNED )- 1 )/ sizeof( UNSIGNED )) + IED_COMMAND_DATA_OFFSET );
    /* 数据实际长度 */
    Temp[1] = actual_size;
    if( Temp[0] > IED_READ_COMMAND_BUFFER_LENGTH )
    {
        /* 数据长度超过管理缓冲区的报文长度,返回失败 */
        return( GW_STATUS_WRITE_TIMEOUT );
    }
    memcpy(&( Temp[IED_COMMAND_DATA_OFFSET] ), pCommand, actual_size );
    /* 返回值 */
    ret = GW_STATUS_WRITE_TIMEOUT;
    switch( pCommand->type )
    {
    case    GW_COMMAND_TYPE_VLD_DATA:
        IEDDB_FCK_PROTECT();  /* 测控数据保护 */
        iStatus = XJP_Queue_Write( FckSecondCommandHandle, Temp );
        IEDDB_FCK_UNPROTECT();/* 释放测控数据保护 */
        if ( iStatus == NU_SUCCESS )
        {
            ret = GW_STATUS_SUCCESS;
        }
        break;
    default:
        break;
    }

    return ret;
}

/* 读取用户命令函数 */
INT iedDB_ReadCommand( VOID * pBuffer, UNSIGNED timeout )
{
    static INT offset = 0;
    HDCM       FckCommandHandle[IED_FCK_COMMAND_MAX_HANDLE];
    INT        ret;
    INT        i;
    UNSIGNED   Temp[IED_READ_COMMAND_BUFFER_LENGTH];  /* 临时缓冲区 */
    GW_COMMAND * pCommand;
    INT        iStatus;
    /*================================================*/
    IEDDB_FCK_PROTECT();  /* 测控数据保护 */
    /* 清数据缓冲区 */
    memset( Temp, 0,( sizeof( UNSIGNED ) * IED_READ_COMMAND_BUFFER_LENGTH ));
    pCommand = ( GW_COMMAND *)pBuffer;
    /* 将管理句柄按优先级重新排序 */
    for( i = 0; i < IED_FCK_COMMAND_MAX_HANDLE; i ++)
    {
        FckCommandHandle[i] = IEDDB_FckCommandHandle[IED_CommandProirityTable[( offset + i ) % IED_COMMAND_PROIRITY_BUFF_SIZE]];
    }
    /* 调整优先级缓冲区计数器 */
    offset = (( offset + 1 ) % IED_COMMAND_PROIRITY_BUFF_SIZE );
    ret = GW_STATUS_NO_SOE;
    for ( i = 0 ; i < IED_FCK_COMMAND_MAX_HANDLE ; i ++)
    {
        iStatus = XJP_Queue_Read( FckCommandHandle[i] , Temp );
        if ( iStatus == NU_SUCCESS )
        {
            /* 判断报文长度是否在允许的范围之内 */
            if( Temp[1] <= IED_READ_COMMAND_SIZE )
            {
                memcpy( pCommand, & Temp[IED_COMMAND_DATA_OFFSET], Temp[1] );
                ret = GW_STATUS_SUCCESS;
                break;
            }
        }
    }
    IEDDB_FCK_UNPROTECT();/* 释放测控数据保护 */
    return ret;
}

/* 优先级表初始化函数 */
VOID iedDB_ProirityInit( void )
{
    INT i;
    /*================================================*/
    /* FCK SOE 优先级缓冲区 */
    for( i = 0; i < IED_SOE_PROIRITY_BUFF_SIZE; i ++)
    {
        IED_SOEProirityTable[i] = ( i % IED_FCK_SOE_MAX_HANDLE );
    }

    /* 用户命令优先级缓冲区 */
    for( i = 0; i < IED_COMMAND_PROIRITY_BUFF_SIZE; i ++)
    {
        IED_CommandProirityTable[i] = ( i % IED_FCK_COMMAND_MAX_HANDLE );
    }
}
