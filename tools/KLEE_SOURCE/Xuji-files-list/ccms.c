
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
/*      ccms.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CCMS - Common com message Management                             */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common cms          */
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
/*      ccms_extr.h                          Protect Control functions   */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      YanJuanJiang    08-03-2011      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "plat/es_plat.h"
#include "plat/inc/xj_file.h"
#include "plat/inc/xj_ccms.h"
#include "plat/inc/xj_cmemc.h"
#include "plat_specific/plat_specific.h"

#define CCMS_QUE_MAX_MES_COUNT  50     /* 最大报文个数 */
#define CCMS_FILE_MAIN_SERIAL   01     /* 主文件序号 */
#define CCMS_FILE_BACKUP_SERIAL 02     /* 备用文件序号 */
#define CCMS_SEMAPHORE_TIMEOUT  200    /* 获取信号量最大超时等待时间 */

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
HDCM CCMS_MessageId;
/* 存储路径 */
CHAR CCMS_DirName[256] = CPC_CMS_DIR_NAME;    /* 通信报文路径 */
CHAR CCMS_WFileName[256] = "ComMsW";          /* 通信写报文文件名称 */
CHAR CCMS_RFileName[256] = "ComMsR";          /* 通信读报文文件名称 */
extern NU_PROTECT * pReport_Protect;

/*====================================*/
/* Define internal function calls.  */
/*------------------------------------*/
/* 保存数据 */
INT CCMS_File_Save( CHAR * fileName, VOID * buf, UNSIGNED size,
    UNSIGNED counter );
/*------------------------------------*/
/* 写文件(向后追加) */
INT CCMS_WriteComMessData( CHAR * fileName, VOID * buf, UNSIGNED size,
    UNSIGNED counter );
/*------------------------------------*/
/* 读文件 */
INT CCMS_ReadComMessData( CHAR * fileName, VOID * buf, UNSIGNED size,
    UNSIGNED counter, UNSIGNED offset );
/*------------------------------------*/
/* 获取报文个数 */
INT CCMS_GetMessCount( CHAR * fileName, INT * iCount );
/*------------------------------------*/
/* 读取报文 */
INT CCMS_ReadComMs( CHAR * msg, INT iMaxLen, INT * iActLen, INT iDelFlag );
/*------------------------------------*/
/* 删除读文件 */
VOID CCMS_DelComMsRFile( VOID );
/*------------------------------------*/
/* 检查写临时文件或内存数据是否存在 */
/* 内存数据存在说明写临时文件失败,临时文件存在说明装置出现异常断电 */
VOID  CCMS_CheckFileInfo( VOID );
/*------------------------------------*/
/* 修复通讯报文文件 */
INT CCMS_RepairFileData( CHAR * fileName );
/*------------------------------------*/
/* 在初始化中读取数据信息可能会失败 */
INT CCMS_MessCountInit( VOID );
/*====================================*/
/* 全局数据信息结构 */
typedef struct CCMS_TOTAL_INFO_struct{
    INT              iInitFlag;      /* 初始化标识 */
    INT              iMessInitFlag;  /* 文件报文初始化标识 */
    INT              iWMaxCount;     /* 最大写报文个数 */
    INT              iRMaxCount;     /* 最大读报文个数 */
    INT              iMaxMessLen;    /* 最大报文长度 */
    HDCM             WriteMessId;    /* 写数据消息队列 */
    HDCM             ReadMessId;     /* 读数据消息队列 */
    CHAR             WriteFileName[256];     /* 写文件名称 */
    CHAR             WriteTempFileName[256]; /* 写临时文件名称 */
    CHAR             ReadFileName[256];  /* 读文件名称 */
    INT              iWriteCount;   /* 写报文个数 */
    INT              iFWriteCount;  /* 文件中写报文个数 */
    INT              iReadCount;    /* 读报文个数 */
    INT              iFReadCount;   /* 文件中读报文个数 */
    INT              iUMessBufLen;  /* 一个报文缓冲区的UNSIGNED个数 */
    CHAR             * pMemBuf;     /* 写数据的时候使用 */
    UNSIGNED         uMemSize;      /* 内存数据大小 */
    NU_SEMAPHORE     Semaphore_CB;   /* 对文件的操作需要获取信号量 */
}CCMS_TOTAL_INFO;

CCMS_TOTAL_INFO  CCMS_TotalInfo;

/*==========================================*/
/* 通信报文系统初始化 */
INT CCMS_Initialize( INT iMaxCount, INT iMaxMessLen )
{
    CCMS_TOTAL_INFO  * this;
    INT  iFixLen;
    /*=======================*/
    this = & CCMS_TotalInfo;
    memset( & CCMS_TotalInfo, 0, sizeof( CCMS_TOTAL_INFO ));

    /* 创建通信报文目录 */
    if(( xj_DirExists == XJ_NULL ) || ( xj_MakeDir == XJ_NULL ))
    {
        /* 操作函数不存在，直接返回失败 */
        return( XJ_UNAVAILABLE );
    }
    if( xj_DirExists( CCMS_DirName ) == XJ_NULL )
    {
        /* 目录不存在，创建目录 */
        if( xj_MakeDir( CCMS_DirName ) != XJ_SUCCESS )
        {
            return( XJ_UNAVAILABLE );
        }
    }
    /* 初始化文件名称 */
    sprintf( this->WriteFileName, "%s/%s", CCMS_DirName, CCMS_WFileName );
    sprintf( this->ReadFileName, "%s/%s", CCMS_DirName, CCMS_RFileName );
    sprintf( this->WriteTempFileName, "%s/%s_Temp", CCMS_DirName, CCMS_WFileName );

    /* 创建信号量 */
    NU_Create_Semaphore(& this->Semaphore_CB, "ComMs", 1, NU_FIFO );

    /* 计算固定报文长度(字符串长度记录到报文中+回车符) */
    iFixLen = ( iMaxMessLen + sizeof( UNSIGNED ) + 2 + sizeof( UNSIGNED ) - 1 )/ sizeof( UNSIGNED );
    /* 写数据消息队列 */
    this->WriteMessId = XJP_Queue_Create(( iFixLen * CCMS_QUE_MAX_MES_COUNT ),
        XJP_EVENT_FIXED_SIZE, iFixLen );
    /* 读数据消息队列 */
    this->ReadMessId = XJP_Queue_Create(( iFixLen * CCMS_QUE_MAX_MES_COUNT ),
        XJP_EVENT_FIXED_SIZE, iFixLen );
    /* 最大报文个数 */
    this->iWMaxCount = iMaxCount;
    /* 读文件最大个数 */
    this->iRMaxCount = iMaxCount / 2;
    /* 最大报文长度 */
    this->iMaxMessLen = iMaxMessLen;
    /* 一个报文缓冲区的UNSIGNED个数 */
    this->iUMessBufLen = iFixLen;
    /* 报文个数还没有初始化 */
    this->iInitFlag = XJ_TRUE;
    return( XJ_SUCCESS );
}

/*====================================*/
/* 写通信报文 */
INT CCMS_WriteComMs( CHAR * msg, INT iLen, UNSIGNED uTimeOut )
{
    INT    status = XJ_UNAVAILABLE;
    CCMS_TOTAL_INFO  * this;
    UNSIGNED * pMesBuffer;   /* 数据缓冲区 */
    CHAR     * pTempBuffer;  /* 数据缓冲区 */
    INT      iSize;
    /*=======================*/
    this = & CCMS_TotalInfo;

    if(( iLen > this->iMaxMessLen )
        || ( msg == XJ_NULL )
        || ( this->iMessInitFlag == XJ_FALSE ))
    {
        return( XJ_UNAVAILABLE );
    }

    /* 若空间不够将已发送报文删除 */
    /* 只有在读文件有数据的时候删除才有意义 */
    if (( this->iWriteCount >= this->iWMaxCount )
        && ( this->iFReadCount ))
    {
        /* 对文件操作需要获取信号量 */
        if( NU_Obtain_Semaphore(& this->Semaphore_CB, uTimeOut ) == XJ_SUCCESS )
        {
            /* 删除多余文件 */
            CCMS_DelComMsRFile();
            /* 释放信号量 */
            NU_Release_Semaphore(& this->Semaphore_CB );
        }
        else
        {
            return( XJ_UNAVAILABLE );
        }
    }
    /* 判断未发送报文个数是否在允许范围之内 */
    if ( this->iWriteCount < this->iWMaxCount )
    {
        iSize = ( this->iUMessBufLen * sizeof( UNSIGNED ));
        /* 动态申请内存空间 */
        pMesBuffer = malloc( iSize );
        /* 判断空间是否申请成功 */
        if( pMesBuffer )
        {
            memset( pMesBuffer, 0, iSize );
            XJ_Protect( pReport_Protect );
            memcpy( ( CHAR *)( pMesBuffer + 1 ), msg, iLen ); /* 增加结束符及换行 */
            pTempBuffer = ( CHAR *)pMesBuffer;
            /* 调整到结束符的位置 */
            pTempBuffer[iSize - 2] = '\r';
            pTempBuffer[iSize - 1] = '\n';
            /* 记录报文长度 */
            * pMesBuffer = iLen;
            /* 写数据 */
            status = XJP_Queue_Write( this->WriteMessId, pMesBuffer );
            if( status == XJ_SUCCESS )
            {
                /* 调整计数 */
                this->iWriteCount += 1;
            }
            XJ_Unprotect();
            free( pMesBuffer );
        }
    }

    return( status );
}

/*==========================================*/
/* 用户读取报文 */
INT CCMS_UserReadComMs( CHAR * msg, INT iMaxLen, INT * iActLen, UNSIGNED uTimeOut )
{
    INT iStatus = XJ_UNAVAILABLE;
    CCMS_TOTAL_INFO  * this;
    /*=======================*/
    this = & CCMS_TotalInfo;
    if( this->iMessInitFlag == XJ_FALSE )
    {
        return( XJ_UNAVAILABLE );
    }
    /* 对文件操作需要获取信号量 */
    if( NU_Obtain_Semaphore(& this->Semaphore_CB, uTimeOut ) == XJ_SUCCESS )
    {
        /* 读取报文 */
        iStatus = CCMS_ReadComMs( msg, iMaxLen, iActLen, 0 );
        /* 释放信号量 */
        NU_Release_Semaphore(& this->Semaphore_CB );
    }

    return( iStatus );
}

/*==========================================*/
/* 删除报文 */
INT CCMS_DelComMs( UNSIGNED uTimeOut )
{
    INT iStatus = XJ_UNAVAILABLE;
    CCMS_TOTAL_INFO  * this;
    /*=======================*/
    this = & CCMS_TotalInfo;
    if( this->iMessInitFlag == XJ_FALSE )
    {
        return( XJ_UNAVAILABLE );
    }

    /* 对文件操作需要获取信号量 */
    if( NU_Obtain_Semaphore(& this->Semaphore_CB, uTimeOut ) == XJ_SUCCESS )
    {
        /* 删除报文 */
        iStatus = CCMS_ReadComMs( XJ_NULL, 0, XJ_NULL, 1 );
        /* 释放信号量 */
        NU_Release_Semaphore(& this->Semaphore_CB );
    }
    return( iStatus );
}

/*==========================================*/
/* 写通信报文数据主函数 */
INT CCMS_WriteComMsMain( VOID )
{
    INT      status;
    UNSIGNED size;
    CCMS_TOTAL_INFO  * this;
    UNSIGNED  * pMesBuffer;
    /*=======================*/
    this = & CCMS_TotalInfo;
    if( this->iInitFlag == XJ_FALSE )
    {
        return( XJ_UNAVAILABLE );
    }
    /* 主函数中获取信号量可以有等待 */
    if( NU_Obtain_Semaphore(& this->Semaphore_CB, CCMS_SEMAPHORE_TIMEOUT ) == XJ_SUCCESS )
    {
        /* 在初始化中读取数据信息可能会失败 */
        CCMS_MessCountInit();
        /* 检查写临时文件或内存数据是否存在 */
        /* 内存数据存在说明写临时文件失败,临时文件存在说明装置出现异常断电 */
        CCMS_CheckFileInfo();
        status = XJ_SUCCESS;
        /* 动态申请内存空间 */
        pMesBuffer = malloc( this->iUMessBufLen * sizeof( UNSIGNED ));
        /* 判断空间是否申请成功 */
        if( pMesBuffer )
        {
            memset( pMesBuffer, 0,( this->iUMessBufLen * sizeof( UNSIGNED )));
            /* 检查写报文 */
            if( XJP_Queue_Find( this->WriteMessId, & size ) == XJ_TRUE )
            {
                XJ_Protect( pReport_Protect );
                status = XJP_Queue_Read( this->WriteMessId , pMesBuffer );
                XJ_Unprotect();
                if( status == XJ_SUCCESS )
                {
                    /* 将报文写入文件系统中 */
                    size = this->iUMessBufLen * sizeof( UNSIGNED );
                    /* 写文件 */
                    status = CCMS_WriteComMessData( this->WriteFileName, pMesBuffer, size, sizeof( CHAR ));
                    /* 判断写数据是否成功 */
                    if( status == XJ_SUCCESS )
                    {
                        /* 调整计数 */
                        this->iFWriteCount += 1;
                    }
                    else
                    {
                        /* 如果写文件失败,将数据再写回数据队列中 */
                        XJ_Protect( pReport_Protect );
                        status = XJP_Queue_Write( this->WriteMessId , pMesBuffer );
                        XJ_Unprotect();
                    }
                }
            }

            /* 检查读报文 */
            if( XJP_Queue_Find( this->ReadMessId, & size ) == XJ_TRUE )
            {
                memset( pMesBuffer, 0,( this->iUMessBufLen * sizeof( UNSIGNED )));
                XJ_Protect( pReport_Protect );
                status = XJP_Queue_Read( this->ReadMessId , pMesBuffer );
                XJ_Unprotect();
                if( status == XJ_SUCCESS )
                {
                    /* 将报文写入文件系统中 */
                    size = this->iUMessBufLen * sizeof( UNSIGNED );
                    /* 写文件 */
                    status = CCMS_WriteComMessData( this->ReadFileName, pMesBuffer, size, sizeof( CHAR ));
                    /* 判断写数据是否成功 */
                    if( status == XJ_SUCCESS )
                    {
                        /* 调整计数 */
                        this->iFReadCount += 1;
                    }
                    else
                    {
                        /* 如果写文件失败,将数据再写回数据队列中 */
                        XJ_Protect( pReport_Protect );
                        status = XJP_Queue_Write( this->ReadMessId , pMesBuffer );
                        XJ_Unprotect();
                    }
                }
            }
            free( pMesBuffer );
        }
        /* 判断文件是否达到一定大小 */
        if( this->iFReadCount >= this->iRMaxCount )
        {
            /* 执行删除读文件操作 */
            CCMS_DelComMsRFile();
        }
        /* 释放信号量 */
        NU_Release_Semaphore(& this->Semaphore_CB );
    }

    return( XJ_SUCCESS );
}

/*==========================================*/
/* 修复通讯报文文件 */
INT CCMS_RepairFileData_Main( VOID )
{
    CCMS_TOTAL_INFO  * this;
    INT  iStatus;
    /*=======================*/
    this = & CCMS_TotalInfo;
    /* 判断数据是否已初始化 */
    if( this->iInitFlag == XJ_TRUE )
    {
        iStatus = NU_Obtain_Semaphore(& this->Semaphore_CB, CCMS_SEMAPHORE_TIMEOUT );
        /* 获取信号量 */
        if( iStatus == XJ_SUCCESS )
        {
            /* 修复通讯报文写文件 */
            iStatus =  CCMS_RepairFileData( this->WriteFileName );
            /* 修复通讯报文读文件 */
            iStatus =  CCMS_RepairFileData( this->ReadFileName );
            /* 释放信号量 */
            NU_Release_Semaphore(& this->Semaphore_CB );
        }
    }
    return( XJ_SUCCESS );
}

/*==========================================*/
/* 内部使用函数 */
/* 获取报文个数 */
INT CCMS_GetMessCount( CHAR * fileName, INT * iCount )
{
    INT status = XJ_UNAVAILABLE;
    CHAR fileName1[256]; /* 完整路径名（包括'/'） */
    CHAR fileName2[256]; /* 完整路径名（包括'/'） */
    CCMS_TOTAL_INFO  * this;
    struct FileInformation fileInfo;
    INT   iStatus = XJ_UNAVAILABLE;
    /*=======================*/
    this = & CCMS_TotalInfo;
    sprintf( fileName1, "%s%d", fileName, CCMS_FILE_MAIN_SERIAL );   /* 数据1文件名称 */
    sprintf( fileName2, "%s%d", fileName, CCMS_FILE_BACKUP_SERIAL ); /* 数据2文件名称 */
    if( xj_FileExists( fileName1 ) == XJ_TRUE )
    {
        if( xj_GetFileInformation( fileName1,& fileInfo ) == XJ_SUCCESS )
        {
            if( iCount )
            {
                * iCount = fileInfo.Size / ( this->iUMessBufLen * sizeof( UNSIGNED ));
            }
            status = XJ_SUCCESS;
        }
    }
    else if( xj_FileExists( fileName2 )== XJ_TRUE )
    {
        if( xj_GetFileInformation( fileName1,& fileInfo ) == XJ_SUCCESS )
        {
            if( iCount )
            {
                * iCount = fileInfo.Size / ( this->iUMessBufLen * sizeof( UNSIGNED ));
            }
            status = XJ_SUCCESS;
        }
    }
    else
    {
        iStatus = XJ_UNAVAILABLE;
    }
    return( iStatus );
}

/*==========================================*/
/* 写文件(向后追加) */
INT CCMS_WriteComMessData( CHAR * fileName, VOID * buf, UNSIGNED size,
    UNSIGNED counter )
{
    INT status = XJ_UNAVAILABLE;
    INT fileStatus1;
    INT fileStatus2;
    CHAR fileName1[256]; /* 完整路径名（包括'/'） */
    CHAR fileName2[256]; /* 完整路径名（包括'/'） */
    INT iStatus = 20;    /* 错误状态起始值 */

    sprintf( fileName1, "%s%d", fileName, CCMS_FILE_MAIN_SERIAL );   /* 数据1文件名称 */
    sprintf( fileName2, "%s%d", fileName, CCMS_FILE_BACKUP_SERIAL ); /* 数据2文件名称 */
    /* 检查临时文件是否存在 */
    if( xj_FileExists( fileName ) == XJ_TRUE )
    {
        /* 临时文件已经存在，删除临时文件 */
        xj_RemoveFile( fileName );
    }

    /* 修复不齐全的主、备文件 */
    fileStatus1 = xj_FileExists( fileName1 );
    fileStatus2 = xj_FileExists( fileName2 );
    if(( fileStatus1 == NU_TRUE ) && ( fileStatus2 == XJ_FALSE ))
    {
        /* 仅有主文件无备份文件，复制主文件到备份文件 */
        XJP_Copy_NVdata( fileName1, fileName2 );
    }
    else if(( fileStatus1 == NU_FALSE ) && ( fileStatus2 == XJ_TRUE ))
    {
        /* 仅有备份文件无主文件，复制备份文件到主文件 */
        XJP_Copy_NVdata( fileName2, fileName1 );
    }

    /* 保存整定值数据，仅保存主备文件属性相同（同时存在或不存在）的整定值数据 */
    fileStatus1 = xj_FileExists( fileName1 );
    fileStatus2 = xj_FileExists( fileName2 );
    if((( fileStatus1 == NU_TRUE ) && ( fileStatus2 == XJ_TRUE ))
        || (( fileStatus1 == NU_FALSE ) && ( fileStatus2 == XJ_FALSE )))
    {
        /* 先保存第1份数据 */
        if( fileStatus1 == XJ_TRUE )
        {
            status = xj_RenameFile( fileName1, fileName );
            if( status != XJ_SUCCESS )
            {
                /* 返回实际错误值,便于查找问题 */
                status = ( iStatus + 1 );
                return( status );
            }
        }
        status = CCMS_File_Save( fileName, buf, size, counter );
        if( status != XJ_SUCCESS )
        {
            /* 返回实际错误值,便于查找问题 */
            status = ( iStatus + 2 );
            return( status );
        }
        status = xj_RenameFile( fileName, fileName1 );
        if( status != XJ_SUCCESS )
        {
            /* 返回实际错误值,便于查找问题 */
            status = ( iStatus + 3 );
            return( status );
        }
        /* 保存第2份数据 */
        if( fileStatus2 == XJ_TRUE )
        {
            status = xj_RenameFile( fileName2, fileName );
            if( status != XJ_SUCCESS )
            {
                /* 返回实际错误值,便于查找问题 */
                status = ( iStatus + 4 );
                return( status );
            }
        }
        status = CCMS_File_Save( fileName, buf, size, counter );
        if( status != XJ_SUCCESS )
        {
            /* 返回实际错误值,便于查找问题 */
            status = ( iStatus + 5 );
            return( status );
        }
        status = xj_RenameFile( fileName, fileName2 );
        if( status != XJ_SUCCESS )
        {
            /* 返回实际错误值,便于查找问题 */
            status = ( iStatus + 6 );
            return( status );
        }
    }
    return( status );
}

/*==========================================*/
/* 读文件 */
INT CCMS_ReadComMessData( CHAR * fileName, VOID * buf, UNSIGNED size,
    UNSIGNED counter, UNSIGNED offset )
{
    INT status = XJ_UNAVAILABLE;
    CHAR fileName1[256]; /* 完整路径名（包括'/'） */
    CHAR fileName2[256]; /* 完整路径名（包括'/'） */
    INT iStatus;

    sprintf( fileName1, "%s%d", fileName, CCMS_FILE_MAIN_SERIAL ); /* 数据1文件名称 */
    sprintf( fileName2, "%s%d", fileName, CCMS_FILE_BACKUP_SERIAL ); /* 数据2文件名称 */
    if( xj_FileExists( fileName1 ) == XJ_TRUE )
    {
        /* 文件已经存在 */
        status = XJP_File_Read( fileName1, buf, size, counter, offset );
        if( status == XJ_SUCCESS )
        {
            iStatus = XJ_SUCCESS;
        }
        else
        {
            iStatus = XJ_UNAVAILABLE;
        }
    }
    else if( xj_FileExists( fileName2 )== XJ_TRUE )
    {
        /* 文件已经存在 */
        status = XJP_File_Read( fileName2, buf, size, counter, offset );
        if( status == XJ_SUCCESS )
        {
            iStatus = XJ_SUCCESS;
        }
        else
        {
            iStatus = XJ_UNAVAILABLE;
        }
    }
    else
    {
        iStatus = XJ_UNAVAILABLE;
    }
    return( iStatus );
}

/*==========================================*/
/* 保存数据 */
INT CCMS_File_Save( CHAR * fileName, VOID * buf, UNSIGNED size,
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

/*==========================================*/
/* 读取报文 */
INT CCMS_ReadComMs( CHAR * msg, INT iMaxLen, INT * iActLen, INT iDelFlag )
{
    INT iStatus = XJ_UNAVAILABLE;
    CCMS_TOTAL_INFO  * this;
    INT   iFWriteCount;
    INT   iReadCount;
    UNSIGNED uOffset = 0;
    UNSIGNED * pMesBuffer;
    UNSIGNED uSize;
    INT      iMessLen;
    /*=======================*/
    this = & CCMS_TotalInfo;
    XJ_Protect( pReport_Protect );
    iFWriteCount = this->iFWriteCount;
    iReadCount = this->iReadCount;
    XJ_Unprotect();
    /* 判断是否有未读取的报文 */
    if( iFWriteCount > iReadCount )
    {
        uSize = this->iUMessBufLen * sizeof( UNSIGNED );
        uOffset = iReadCount * uSize;
        /* 动态申请内存空间 */
        pMesBuffer = malloc( uSize );
        /* 判断空间是否申请成功 */
        if( pMesBuffer )
        {
            memset( pMesBuffer, 0, uSize );
            /* 读文件 */
            iStatus =  CCMS_ReadComMessData( this->WriteFileName, pMesBuffer,
                uSize, sizeof( CHAR ), uOffset );
            if( iStatus == XJ_SUCCESS )
            {
                if( iDelFlag )
                {
                    XJ_Protect( pReport_Protect );
                    /* 写数据 */
                    if( XJP_Queue_Write( this->ReadMessId, pMesBuffer ) == XJ_SUCCESS )
                    {
                        /* 调整计数 */
                        this->iReadCount = iReadCount + 1;
                    }
                    XJ_Unprotect();
                }
                else
                {
                    iMessLen = * pMesBuffer;
                    if( iMessLen > iMaxLen )
                    {
                        iMessLen = iMaxLen;
                    }
                    if( msg )
                    {
                        memcpy( msg,( pMesBuffer + 1 ), iMessLen );
                    }
                    if( iActLen )
                    {
                        * iActLen = * pMesBuffer;
                    }
                }
            }
        }
    }
    else if( iFWriteCount < iReadCount )
    {
        /* 若写报文个数小于报文个数则说明文件出现异常,需要将文件删除 */
        /* 删除读文件 */
        CCMS_DelComMsRFile();
    }
    return( iStatus );
}

/*==========================================*/
/* 删除读文件 */
VOID CCMS_DelComMsRFile( VOID )
{
    INT status = XJ_UNAVAILABLE;
    CHAR fileName1[256]; /* 完整路径名（包括'/'） */
    CHAR fileName2[256]; /* 完整路径名（包括'/'） */
    CHAR * pFileName = XJ_NULL;
    CCMS_TOTAL_INFO  * this;
    INT    iFlag = XJ_FALSE;
    FILE * fp;
    UNSIGNED readSize;
    UNSIGNED uOffsetSize;
    UNSIGNED uSize;
    INT      iDelTFlag = XJ_FALSE;   /* 删除所有文件标识 */
    struct FileInformation fileInfo;
    /*=======================*/
    this = & CCMS_TotalInfo;

    /* 判断是否还存在未发送的报文 */
    if(( this->iFWriteCount > this->iFReadCount )
        && ( this->iWriteCount >= this->iFWriteCount )
        && ( this->iReadCount >= this->iFReadCount )
        && ( this->iFWriteCount >= this->iReadCount ) )
    {
        sprintf( fileName1, "%s%d", this->WriteFileName, CCMS_FILE_MAIN_SERIAL );   /* 数据1文件名称 */
        sprintf( fileName2, "%s%d", this->WriteFileName, CCMS_FILE_BACKUP_SERIAL ); /* 数据2文件名称 */
        /* 先将未发送报文读取出来 */
        /* 判断主，后备文件哪个有效 */
        if( xj_GetFileInformation( fileName1,& fileInfo ) == XJ_SUCCESS )
        {
            pFileName = fileName1;
        }
        else if( xj_GetFileInformation( fileName2,& fileInfo ) == XJ_SUCCESS )
        {
            pFileName = fileName2;
        }

        /* 是否有有效文件 */
        if( pFileName )
        {
            /* 读取数据偏移 */
            uOffsetSize = this->iFReadCount * this->iUMessBufLen * sizeof( UNSIGNED );
            /* 理论文件大小 */
            uSize = this->iFWriteCount * this->iUMessBufLen * sizeof( UNSIGNED );
            /* 判断文件大小是否正确 */
            if((( UNSIGNED )( fileInfo.Size ) > uOffsetSize )
                && ( ( UNSIGNED )( fileInfo.Size ) == uSize ))
            {
                /* 剩余内容size */
                uSize = fileInfo.Size - uOffsetSize;
                /* 申请内存空间 */
                this->pMemBuf = malloc( uSize );
                /* 判断内存是否申请成功 */
                if( this->pMemBuf )
                {
                    /* 读数据表数据 */
                    fp = xj_fopen( pFileName, "rb" );
                    if( fp != XJ_NULL )
                    {
                        readSize = 0;
                        /* 设置文件位置指针 */
                        if( xj_fseek( fp, uOffsetSize, 0 ) == 0 )
                        {
                            /* 读数据 */
                            readSize = ( UNSIGNED )xj_fread( this->pMemBuf, uSize, sizeof( CHAR ), fp );
                        }
                        xj_fclose( fp );
                        if( readSize == sizeof( CHAR ) )
                        {
                            /* 保存内存数据size */
                            this->uMemSize = uSize;
                            /* 写入临时文件中 */
                            /* 检查写临时文件或内存数据是否存在 */
                            /* 内存数据存在说明写临时文件失败,临时文件存在说明装置出现异常断电 */
                            CCMS_CheckFileInfo();
                        }
                        else
                        {
                            /* 读取数据失败,释放内存空间 */
                            free( this->pMemBuf );
                            this->pMemBuf = XJ_NULL;
                        }
                    }
                    else
                    {
                        /* 打开文件失败,释放内存空间 */
                        free( this->pMemBuf );
                        this->pMemBuf = XJ_NULL;
                    }
                }
            }
            else
            {
                /* 文件大小不正确,删除所有文件 */
                iDelTFlag = XJ_TRUE;
            }
        }
        else
        {
            /* 没有找到有效文件直接删除 */
            iDelTFlag = XJ_TRUE;
        }
    }
    else
    {
        /* 无未发送数据置删除读/写文件标识 */
        iDelTFlag = XJ_TRUE;
    }

    /* 删除读写文件 */
    if ( iDelTFlag == XJ_TRUE )
    {
        sprintf( fileName1, "%s%d", this->ReadFileName, CCMS_FILE_MAIN_SERIAL );   /* 数据1文件名称 */
        sprintf( fileName2, "%s%d", this->ReadFileName, CCMS_FILE_BACKUP_SERIAL ); /* 数据2文件名称 */
        xj_RemoveFile( fileName1 );
        xj_RemoveFile( fileName2 );
        sprintf( fileName1, "%s%d", this->WriteFileName, CCMS_FILE_MAIN_SERIAL );   /* 数据1文件名称 */
        sprintf( fileName2, "%s%d", this->WriteFileName, CCMS_FILE_BACKUP_SERIAL ); /* 数据2文件名称 */
        xj_RemoveFile( fileName1 );
        xj_RemoveFile( fileName2 );
        /* 调整计数 */
        this->iFWriteCount = 0;
        /* 调整计数 */
        this->iWriteCount = 0;
        /* 调整计数 */
        this->iFReadCount = 0;
        /* 调整计数 */
        this->iReadCount = 0;
    }
}

/*==========================================*/
/* 检查写临时文件或内存数据是否存在 */
/* 内存数据存在说明写临时文件失败,临时文件存在说明装置出现异常断电 */
VOID  CCMS_CheckFileInfo( VOID )
{
    CHAR fileName1[256]; /* 完整路径名（包括'/'） */
    CHAR fileName2[256]; /* 完整路径名（包括'/'） */
    CCMS_TOTAL_INFO  * this;
    INT  status;
    /*=======================*/
    this = & CCMS_TotalInfo;

    sprintf( fileName1, "%s%d", this->WriteFileName, CCMS_FILE_MAIN_SERIAL );   /* 数据1文件名称 */
    sprintf( fileName2, "%s%d", this->WriteFileName, CCMS_FILE_BACKUP_SERIAL ); /* 数据2文件名称 */
    /* 判断内存数据是否存在 */
    if( this->pMemBuf )
    {
        /* 检查临时文件是否存在 */
        if( xj_FileExists( this->WriteTempFileName ) == XJ_TRUE )
        {
            /* 临时文件已经存在，删除临时文件 */
            xj_RemoveFile( this->WriteTempFileName );
        }
        /* 保存数据到临时文件中 */
        status = CCMS_File_Save( this->WriteTempFileName,
            this->pMemBuf, this->uMemSize, sizeof( CHAR ) );
        if( status == XJ_SUCCESS )
        {
            /* 临时文件写成功后就可以释放内存空间 */
            free( this->pMemBuf );
            this->pMemBuf = XJ_NULL;
            this->uMemSize = 0;
        }
    }
    /* 判断临时文件是否存在 */
    if( xj_FileExists( this->WriteTempFileName ))
    {
        /* 拷贝数据 */
        XJP_Copy_NVdata( this->WriteTempFileName, fileName1 );
        /* 拷贝数据 */
        XJP_Copy_NVdata( this->WriteTempFileName, fileName2 );
        /* 删除读文件 */
        sprintf( fileName1, "%s%d", this->ReadFileName, CCMS_FILE_MAIN_SERIAL ); /* 数据2文件名称 */
        sprintf( fileName2, "%s%d", this->ReadFileName, CCMS_FILE_BACKUP_SERIAL ); /* 数据1文件名称 */
        /* 判断文件1是否存在 */
        if( xj_FileExists( fileName1 ))
        {
            xj_RemoveFile( fileName1 );
        }
        /* 判断文件2是否存在 */
        if( xj_FileExists( fileName2 ))
        {
            xj_RemoveFile( fileName2 );
        }
        /* 删除临时文件 */
        xj_RemoveFile( this->WriteTempFileName );
        /* 调整计数 */
        this->iFWriteCount -= this->iFReadCount;
        /* 调整计数 */
        this->iWriteCount -= this->iFReadCount;
        /* 调整计数 */
        this->iFReadCount = 0;
        /* 调整计数 */
        this->iReadCount = 0;
    }
}

/*==========================================*/
/* 修复通讯报文文件 */
INT CCMS_RepairFileData( CHAR * fileName )
{
    INT status;
    INT status1;
    INT status2;
    UNSIGNED crc_code1;
    UNSIGNED crc_code2;
    CHAR fileName1[64]; /* 完整路径名（包括'/'） */
    CHAR fileName2[64]; /* 完整路径名（包括'/'） */
    /*=======================*/
    status = XJ_UNAVAILABLE;
    /* 主文件名称 */
    sprintf( fileName1, "%s%d", fileName, CCMS_FILE_MAIN_SERIAL );
    /* 后备文件名称 */
    sprintf( fileName2, "%s%d", fileName, CCMS_FILE_BACKUP_SERIAL );
    status1 = XJP_CalculateFileCRC( fileName1, & crc_code1 );
    status2 = XJP_CalculateFileCRC( fileName2, & crc_code2 );
    if(( status1 == XJ_SUCCESS ) && ( status2 == XJ_SUCCESS ))
    {
        /* 主、备文件都可用 */
        if( crc_code1 != crc_code2 )
        {
            /* 校验码错误，以主文件为准 */
            if( XJP_Copy_NVdata( fileName1, fileName2 )== XJ_SUCCESS )
            {
                status = XJ_SUCCESS;
            }
            else
            {
                /* 修复文件出错 */
                status = XJ_END_OF_LOG;
            }
        }
        else
        {
            status = XJ_SUCCESS;
        }
    }
    else
    {
        if( status1 == XJ_SUCCESS )
        {
            /* 主文件可用，备用文件错误 */
            if( XJP_Copy_NVdata( fileName1, fileName2 )== XJ_SUCCESS )
            {
                status = XJ_SUCCESS;
            }
            else
            {
                /* 修复文件出错 */
                status = XJ_END_OF_LOG;
            }
        }
        else if( status2 == XJ_SUCCESS )
        {
            /* 备用文件可用，主文件错误 */
            if( XJP_Copy_NVdata( fileName2, fileName1 )== XJ_SUCCESS )
            {
                status = XJ_SUCCESS;
            }
            else
            {
                /* 修复文件出错 */
                status = XJ_END_OF_LOG;
            }
        }
        else
        {
            /* 修复文件出错 */
            status = XJ_END_OF_LOG;
        }
    }
    return( status );
}

/*==========================================*/
/* 在初始化中读取数据信息可能会失败 */
INT CCMS_MessCountInit( VOID )
{
    CCMS_TOTAL_INFO  * this;
    static INT iInitFlag = XJ_FALSE;
    CHAR fileName1[64]; /* 完整路径名（包括'/'） */
    CHAR fileName2[64]; /* 完整路径名（包括'/'） */
    INT iStatus = XJ_UNAVAILABLE;
    /*=======================*/
    this = & CCMS_TotalInfo;

    /* 初始化标识 */
    if(( this->iInitFlag ) && ( this->iMessInitFlag == XJ_FALSE ))
    {
        /* 获取报文个数 */
        CCMS_GetMessCount( this->WriteFileName,& this->iWriteCount );
        this->iFWriteCount = this->iWriteCount;
        /* 获取报文个数 */
        CCMS_GetMessCount( this->ReadFileName,& this->iReadCount );
        this->iFReadCount = this->iReadCount;

        /* 报文个数为0清除所有文件 */
        if( this->iFWriteCount == 0 )
        {
            sprintf( fileName1, "%s%d", this->ReadFileName, CCMS_FILE_MAIN_SERIAL );   /* 数据1文件名称 */
            sprintf( fileName2, "%s%d", this->ReadFileName, CCMS_FILE_BACKUP_SERIAL ); /* 数据2文件名称 */
            xj_RemoveFile( fileName1 );
            xj_RemoveFile( fileName2 );
            sprintf( fileName1, "%s%d", this->WriteFileName, CCMS_FILE_MAIN_SERIAL );   /* 数据1文件名称 */
            sprintf( fileName2, "%s%d", this->WriteFileName, CCMS_FILE_BACKUP_SERIAL ); /* 数据2文件名称 */
            xj_RemoveFile( fileName1 );
            xj_RemoveFile( fileName2 );
            /* 调整计数 */
            this->iFWriteCount = 0;
            /* 调整计数 */
            this->iWriteCount = 0;
            /* 调整计数 */
            this->iFReadCount = 0;
            /* 调整计数 */
            this->iReadCount = 0;
        }

        /* 置初始化完成标识 */
        this->iMessInitFlag = XJ_TRUE;
        iStatus = XJ_SUCCESS;
    }
    return iStatus;
}

/*==========================================*/
