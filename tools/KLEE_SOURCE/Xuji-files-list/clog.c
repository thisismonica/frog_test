
/*************************************************************************/
/*                                                                       */
/*        Copyright (c) 1999-2002 XJ ELECTRIC CO.LTD.                    */
/*                                                                       */
/* PROPRIETARY RIGHTS of XJ ELECTRIC CO.LTD are involved in the          */
/* subject matter of this material.  All manufacturing, reproduction,    */
/* use, and sales rights pertaining to this subject matter are governed  */
/* by the license agreement.  The recipient of this software implicitly  */
/* accepts the terms of the license.                                     */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*                                                                       */
/* FILE NAME                                            VERSION          */
/*                                                                       */
/*      clog.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CLOG - Common log Management                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common log          */
/*      management component.                                            */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* DATA STRUCTURES                                                       */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* FUNCTIONS                                                             */
/*                                                                       */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*      cem_extr.h                          Protect Control functions    */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       08-13-2008      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "plat/es_plat.h"
#include "plat/inc/xj_file.h"
#include "plat/inc/xj_clog.h"
#include "plat_specific/plat_specific.h"

#define CLOG_MAX_FILE_SIZE      524288 /* 最大日志文件大小 */
#define CLOG_BACKUP_FILE_INDEX  1   /* 备份日志文件序号 */
#define CLOG_MESSAGE_SIZE       256 /* 单个消息最大字节数 */
#define CLOG_LOG_SIZE (CLOG_MESSAGE_SIZE+32) /* 单个日志消息最大字节数 */

/* 以下为OS相关配置 */
#ifdef WIN32
/* 全局变量申明 */
/* Windows 宏定义 */
#endif

#ifndef WIN32 /* 非windows缺省定义一些宏以便通过编译 */
/* 全局变量申明 */
/* Nucleus宏定义 */
#endif

/* Define external inner-component global data references.  */

INT CLOG_IniFlag = XJ_FALSE;
HDCM CLOG_MessageId;
/* 操作事件事件存储路径 */
CHAR CLOG_DirName[256] = CPC_LOG_DIR_NAME; /* 日志路径 */
CHAR CLOG_FileName[256] = "DeviceLog";     /* 日志文件名称 */
CHAR CLOG_LogFileName[256] = "";           /* 日志全文件名称 */
extern NU_PROTECT * pReport_Protect;

/* Define internal function calls.  */

/* 保存日志数据 */
INT CLOG_WriteLogData( CHAR * fileName, VOID * buf, UNSIGNED size,
    UNSIGNED counter );

/* 日志记录 */
INT CLOG_Log( CHAR * msg )
{
    INT len;
    INT status;
    CHAR buffer[ CLOG_LOG_SIZE ];
    TIME RunTime,* pTime;
    UNSIGNED * p;

    len = strlen( msg );
    if(( len >= CLOG_MESSAGE_SIZE ) || ( msg == XJ_NULL ))
    {
        return( XJ_UNAVAILABLE );
    }

    XJ_Protect( pReport_Protect );

    /* 添加时标 */
    XJP_TM_Get_Time_Dev( & RunTime );
    pTime = & RunTime;
    sprintf( & buffer[sizeof( UNSIGNED )], "%04d-%02d-%02d  %02d:%02d:%02d:%03d %s\r\n",
        ( pTime->Year + 2000 ), pTime->Month, pTime->Day, pTime->Hour, /* 年转换成实际时间 */
        pTime->Minute, ( pTime->Millionsecond / 1000 ), ( pTime->Millionsecond % 1000 ), msg ); /* 毫秒转换成秒 */
    /* 添加数据长度 */
    len = strlen( & buffer[sizeof( UNSIGNED )] ) + 1; /* 包括结束符 */
    len = ( len + sizeof( UNSIGNED ) + sizeof( UNSIGNED ) - 1 )/ sizeof( UNSIGNED );
    p = ( UNSIGNED *)buffer;
    * p = len;
    /* 写数据 */
    status = XJP_Queue_Write( CLOG_MessageId, buffer );

    XJ_Unprotect();

    return( status );
}

/* 写日志数据 */
INT CLOG_WriteLogMain( VOID )
{
    INT len;
    INT status;
    UNSIGNED size;
    CHAR buffer[ CLOG_LOG_SIZE ]; /* 临时缓冲区 */
    struct FileInformation fileInfo;

    if( CLOG_IniFlag == XJ_FALSE )
    {
        return( XJ_UNAVAILABLE );
    }

    status = XJ_SUCCESS;
    if( XJP_Queue_Find( CLOG_MessageId, & size ) == XJ_TRUE )
    {
        /* 读取日志信息 */
        memset( buffer, 0, sizeof( buffer ));
        XJ_Protect( pReport_Protect );
        status = XJP_Queue_Read( CLOG_MessageId , buffer );
        XJ_Unprotect();
        if( status == XJ_SUCCESS )
        {
            len = strlen( & buffer[sizeof( UNSIGNED )] ); /* 包括结束符 */
            /* 保存日志数据 */
            status = CLOG_WriteLogData( CLOG_LogFileName, & buffer[sizeof( UNSIGNED )], len, sizeof( CHAR ) );
            if( status == XJ_SUCCESS )
            {
                /* 检查日志文件大小 */
                if( xj_GetFileInformation( CLOG_LogFileName,& fileInfo ) == XJ_SUCCESS )
                {
                    if( fileInfo.Size >= CLOG_MAX_FILE_SIZE )
                    {
                        /* 超过最大容量，保存备份日志记录，使用新文件重新进行日志记录 */
                        sprintf( buffer, "%s%d", CLOG_LogFileName, CLOG_BACKUP_FILE_INDEX );
                        xj_RemoveFile( buffer );
                        xj_RenameFile( CLOG_LogFileName, buffer );
                        status = XJ_SUCCESS;
                    }
                }
            }
        }
    }

    return( status );
}

/* 保存日志数据 */
INT CLOG_WriteLogData( CHAR * fileName, VOID * buf, UNSIGNED size,
    UNSIGNED counter )
{
    INT status;
    FILE * fp;
    UNSIGNED writeSize;

    status = XJ_UNAVAILABLE;
    /* 写数据 */
    fp = xj_fopen( fileName, "rb+" );
    if( fp == XJ_NULL )
    {
        fp = xj_fopen( fileName, "wb" );
    }
    if( fp != XJ_NULL )
    {
        writeSize = 0;
        /* 设置文件位置指针 */
        if( xj_fseek( fp, 0, SEEK_END ) == 0 ) /* 操作成功 */
        {
            /* 写数据 */
            writeSize = ( UNSIGNED )xj_fwrite( buf, size, counter, fp );
        }
        xj_fclose( fp );
        if( writeSize == counter )
        {
            status = XJ_SUCCESS;
        }
    }

    return( status );
}

/* 日志系统初始化 */
INT CLOG_Initialize( UNSIGNED BufSize )
{
    CLOG_IniFlag = XJ_FALSE;
    /* 创建日志目录 */
    if(( xj_DirExists == XJ_NULL ) || ( xj_MakeDir == XJ_NULL ))
    {
        /* 操作函数不存在，直接返回失败 */
        return( XJ_UNAVAILABLE );
    }
    if( xj_DirExists( CLOG_DirName ) == XJ_NULL )
    {
        /* 目录不存在，创建目录 */
        if( xj_MakeDir( CLOG_DirName ) != XJ_SUCCESS )
        {
            return( XJ_UNAVAILABLE );
        }
    }

    CLOG_MessageId = XJP_Queue_Create( BufSize, XJP_EVENT_VARIABLE_SIZE,
        ( CLOG_LOG_SIZE / sizeof( UNSIGNED ) ));
    sprintf( CLOG_LogFileName, "%s/%s", CLOG_DirName, CLOG_FileName );
    CLOG_IniFlag = XJ_TRUE;

    return( XJ_SUCCESS );
}

/* 无数据保护日志记录 */
INT CLOG_NoProtectLog( CHAR * msg )
{
    INT len;
    INT status;
    CHAR buffer[ CLOG_LOG_SIZE ];
    TIME RunTime,* pTime;
    UNSIGNED * p;

    len = strlen( msg );
    if(( len >= CLOG_MESSAGE_SIZE ) || ( msg == XJ_NULL ))
    {
        return( XJ_UNAVAILABLE );
    }

    /* 添加时标 */
    XJP_TM_Get_Time_Dev( & RunTime );
    pTime = & RunTime;
    sprintf( & buffer[sizeof( UNSIGNED )], "%04d-%02d-%02d  %02d:%02d:%02d:%03d %s\r\n",
        ( pTime->Year + 2000 ), pTime->Month, pTime->Day, pTime->Hour, /* 年转换成实际时间 */
        pTime->Minute, ( pTime->Millionsecond / 1000 ), ( pTime->Millionsecond % 1000 ), msg ); /* 毫秒转换成秒 */
    /* 添加数据长度 */
    len = strlen( & buffer[sizeof( UNSIGNED )] ) + 1; /* 包括结束符 */
    len = ( len + sizeof( UNSIGNED ) + sizeof( UNSIGNED ) - 1 )/ sizeof( UNSIGNED );
    p = ( UNSIGNED *)buffer;
    * p = len;
    /* 写数据 */
    status = XJP_Queue_Write( CLOG_MessageId, buffer );

    return( status );
}
