
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
/*      cfdc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CFDC - Common file delete Management                             */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common file delete  */
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
#include "plat/inc/xj_cfdc.h"

#define CFDC_MAX_DEL_NUMBER   3                 /* 最大删除次数 */
#define CFDC_MAX_FILE_NAME    256               /* 文件名称最大字节数 */
#define CFDC_QUE_LEN          0                 /* 长度位置偏移 */
#define CFDC_QUE_NUMBER       sizeof(UNSIGNED)  /* 删除次数位置偏移 */
#define CFDC_QUE_FILE_NAME    (CFDC_QUE_NUMBER+sizeof(UNSIGNED)) /* 文件名称起始偏移 */
#define CFDC_MAX_MESSAGE_SIZE (CFDC_QUE_FILE_NAME+CFDC_MAX_FILE_NAME) /* 单个消息最大字节数 */
/* Macro used to align a data pointer to next address that meets the specified
   required alignment.
   NOTE:  The required alignment must be a power of 2 (2, 4, 8, 16, 32, etc)
   NOTE:  The next aligned address will be returned regardless of whether the
          data pointer is already aligned. */
#define CFDC_GE_MEM_PTR_ALIGN(ptr_addr, req_align)                      \
        (VOID *)(((UINT32)(ptr_addr) & (UINT32)(~((req_align) - 1))) + (UINT32)(req_align))

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
HDCM CFDC_DelQueueId;
INT CFDC_IniFlag = XJ_FALSE;
VOID (* CFDC_Lock )( VOID ) = XJ_NULL;
VOID (* CFDC_Unlock )( VOID ) = XJ_NULL;
INT (* CFDC_RemoveFile )(  const char * ) = XJ_NULL;
INT (* CFDC_FileExists )(  const char * ) = XJ_NULL;

/* Define internal function calls.  */

/* 删除文件管理模块初始化 */
INT CFDC_Initialize( UNSIGNED BufSize, VOID (* _lock )( VOID  ),
    VOID (* _unlock )( VOID ), INT (* _removeFile )( const char *  ),
    INT (* _fileExists )( const char *  ) )
{
    CFDC_Lock = _lock;
    CFDC_Unlock = _unlock;
    CFDC_RemoveFile = _removeFile;
    CFDC_FileExists = _fileExists;
    CFDC_DelQueueId = XJP_Queue_Create( BufSize, XJP_EVENT_VARIABLE_SIZE,
        ( CFDC_MAX_MESSAGE_SIZE / sizeof( UNSIGNED ) ));

    CFDC_IniFlag = XJ_TRUE;

    return( XJ_SUCCESS );
}

/* 增加删除文件列表 */
INT CFDC_Delete_File( CHAR * fileName )
{
    INT len;
    INT status;
    CHAR * pbuf;
    UNSIGNED * p;
    CHAR buffer[ CFDC_MAX_MESSAGE_SIZE + sizeof( UNSIGNED )]; /* 消息缓冲区 */

    if( CFDC_IniFlag == XJ_FALSE )
    {
        return( XJ_UNAVAILABLE );
    }

    len = strlen( fileName );
    if(( len >= CFDC_MAX_FILE_NAME ) || ( fileName == XJ_NULL ))
    {
        return( XJ_UNAVAILABLE );
    }

    /* 获取对齐内存指针 */
    pbuf = CFDC_GE_MEM_PTR_ALIGN( buffer, sizeof( UNSIGNED ) );
    /* 拷贝文件名称 */
    strncpy(( pbuf + CFDC_QUE_FILE_NAME ), fileName, CFDC_MAX_FILE_NAME );
    p = ( UNSIGNED *)pbuf;
    /* 设置数据长度 */
    len = CFDC_QUE_FILE_NAME + strlen( ( pbuf + CFDC_QUE_FILE_NAME ) );
    len = ( len + sizeof( UNSIGNED ) - 1 ) / sizeof( UNSIGNED );
    * p ++ = len;
    /* 设置初始删除次数 */
    * p = 0;

    /* 发送数据到队列 */
    if( CFDC_Lock )
        CFDC_Lock();
    status = XJP_Queue_Write( CFDC_DelQueueId, pbuf );
    if( CFDC_Unlock )
        CFDC_Unlock();

    return( status );
}

/* 后台删除文件主函数 */
INT CFDC_DeleteFileMain( VOID )
{
    INT status;
    UNSIGNED size;
    CHAR * pbuf;
    CHAR * fileName;
    UNSIGNED number;
    CHAR buffer[  CFDC_MAX_MESSAGE_SIZE + sizeof( UNSIGNED ) ];

    if(( CFDC_IniFlag == XJ_FALSE ) || ( CFDC_RemoveFile == XJ_NULL ) || ( CFDC_FileExists == XJ_NULL ))
    {
        return( XJ_UNAVAILABLE );
    }

    status = XJ_SUCCESS;
    if( XJP_Queue_Find( CFDC_DelQueueId, & size ) == XJ_TRUE )
    {
        memset( buffer, 0, sizeof( buffer ));
        /* 获取对齐内存指针 */
        pbuf = CFDC_GE_MEM_PTR_ALIGN( buffer, sizeof( UNSIGNED ) );
        /* 读取删除文件信息 */
        if( CFDC_Lock )
            CFDC_Lock();
        status = XJP_Queue_Read( CFDC_DelQueueId , pbuf );
        if( CFDC_Unlock )
            CFDC_Unlock();

        if( status == XJ_SUCCESS )
        {
            fileName = pbuf + CFDC_QUE_FILE_NAME;
            if( CFDC_RemoveFile( fileName ))
            {
                /* 删除文件失败，检查文件是否存在 */
                if( CFDC_FileExists( fileName ) == NU_TRUE )
                {
                    /* 删除文件存在，检查删除文件次数 */
                    number = *(( UNSIGNED *)( pbuf + CFDC_QUE_NUMBER ));
                    if(++ number < CFDC_MAX_DEL_NUMBER )
                    {
                        /* 删除失败次数未达边界值，写回重新删除 */
                        *(( UNSIGNED *)( pbuf + CFDC_QUE_NUMBER )) = number;

                        /* 发送数据到队列 */
                        if( CFDC_Lock )
                            CFDC_Lock();
                        status = XJP_Queue_Write( CFDC_DelQueueId, pbuf );
                        if( CFDC_Unlock )
                            CFDC_Unlock();
                    }
                }
            }
        }
    }

    return( status );
}
