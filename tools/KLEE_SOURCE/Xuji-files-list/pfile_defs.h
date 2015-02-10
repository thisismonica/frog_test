
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
/*      pfile_defs.h                                    Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      FILE - file syatem                                               */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains system constants common to both the           */
/*      application and the actual Nucleus PLUS components.  This file   */
/*      also contains data structure definitions that hide internal      */
/*      information from the application.                                */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* DATA STRUCTURES                                                       */
/*                                                                       */
/*                                                                       */
/* FUNCTIONS                                                             */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       01-04-2009      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include <stdarg.h>
#include <stdio.h>

/* Check to see if this file has been included already.  */

#ifndef PFILE_DEFS_H
#define PFILE_DEFS_H

#define PFILE_FUN_LIST_NAME "PlatFileList" /* 文件系统函数表名称 */

#define XJ_FILE_NAME_LEN_MAX            255

struct FileInformation
{
    char Name[XJ_FILE_NAME_LEN_MAX + 1];
    unsigned int Attribute;
    unsigned long Time;
    long Size;
};

typedef FILE * (* _fopen )( const char * , const char * );

typedef int (* _fclose )( FILE * );

typedef size_t (* _fread )( void * , size_t, size_t, FILE * );

typedef size_t (* _fwrite )( const void * , size_t, size_t, FILE * );

typedef int (* _vfscanf )( FILE *, const char *, va_list );

typedef int (* _fgetc ) ( FILE *);

typedef char * (* _fgets )( char * , int, FILE * );

typedef int (* _fputc )( int, FILE *);

typedef int (* _fputs )( const char * , FILE * );

typedef int (* _fgetpos )( FILE * , fpos_t * );

typedef long (* _p_ftell )( FILE *);

typedef int (* _p_fsetpos )( FILE *, const fpos_t *);

typedef int (* _p_fseek )( FILE *, long, int );

typedef int (* _vfprintf )( FILE *, const char *, va_list );

typedef void (* _perror )( const char *);

typedef int (* _fflush )( FILE *);

typedef void (* _rewind )( FILE *);

/*=======================================================================*/
//函数功能：删除文件。
//参    数：path   文件路径。
//返 回 值：操作成功返回0；错误失败返回-1。
typedef int (* _RemoveFile )( const char * path );

//函数功能：创建目录。
//参    数：path   目录路径。
//返 回 值：操作成功返回0；错误失败返回-1。
typedef int (* _MakeDir )( const char * path );

//函数功能：重命名文件。
//参    数：oldPath 旧的文件路径；newPath 新的文件路径。
//返 回 值：操作成功返回0，错误失败返回-1。
typedef int (* _RenameFile )( const char * oldPath, const char * newPath );

//函数功能：重命名目录。
//参    数：oldPath 旧的目录路径；newPath 新的目录路径。
//返 回 值：操作成功返回0，错误失败返回-1。
typedef int (* _RenameDir )( const char * oldPath, const char * newPath );

//函数功能：删除目录。
//参    数：path 目录路径。
//返 回 值：操作成功返回0，错误失败返回-1。
typedef int (* _RemoveDir )( const char * path );

//函数功能：设置文件信息。
//参    数：path 文件路径；info 文件信息。
//返 回 值：操作成功返回0；错误失败返回-1。
typedef int (* _SetFileInformation )( const char * path, const struct FileInformation * info );

//函数功能：获取文件信息。
//参    数：path 文件路径；info 文件信息。
//返 回 值：操作成功返回0；错误失败返回-1。
typedef int (* _GetFileInformation )( const char * path, struct FileInformation * info );

//函数功能：查询文件是否存在。
//参    数：path 文件路径。
//返 回 值：存在返回1，不存在返回0。
typedef int (* _FileExists )( const char * path );

//函数功能：查询目录是否存在。
//参    数：path 目录路路径。
//返 回 值：存在返回1，不存在返回0。
typedef int (* _DirExists )( const char * path );

//函数功能：查询磁盘分区空闲存储空间大小。
//参    数：root 磁盘分区根路径。
//返 回 值：磁盘分区空闲存储空间字节数。
typedef int (* _GetFreeSpace )( const char * root );

//函数功能：遍历指定目录下的文件，并对每个文件回调指定的函数。
//参    数：dirPath 目录路径； filtercallback 回调函数；filtercallbackcontext回调函数参数。
//返 回 值：如果目录存在，返回目录下文件的个数；否则，返回-1。
typedef int (* _ListFile )( const char * dirPath,
    int (* filtercallback )( struct FileInformation * info, void * context ),
    void * filtercallbackcontext );

//函数功能：读取FLASH分区内存储块的状态。
//参    数：root        FLASH分区根路径。
//          retired 输出坏块数。
//          used        输出使用的块数。
//          empty   输出空闲的块数。
//返 回 值：操作失败返回-1，成功返回0。
typedef int (* _GetFlashPrartitionBlockStatus )( const char * root , int * retired, int * used,
    int * empty, int * page_size, int * pages_per_block, int * free_pages );

/*=======================================================================*/
typedef struct PFILE_FUN_LIST_STRUCT
{
    int size;
    _fopen                              fopen;
    _fclose                             fclose;
    _fread                              fread;
    _fwrite                             fwrite;
    _vfscanf                            vfscanf;
    _fgetc                              fgetc;
    _fgets                              fgets;
    _fputc                              fputc;
    _fputs                              fputs;
    _fgetpos                            fgetpos;
    _p_ftell                            ftell;
    _p_fsetpos                          fsetpos;
    _p_fseek                            fseek;

    _RemoveFile                         RemoveFile;
    _MakeDir                            MakeDir;
    _RenameFile                         RenameFile;
    _RenameDir                          RenameDir;
    _RemoveDir                          RemoveDir;
    _SetFileInformation                 SetFileInformation;
    _GetFileInformation                 GetFileInformation;
    _FileExists                         FileExists;
    _DirExists                          DirExists;
    _GetFreeSpace                       GetFreeSpace;
    _ListFile                           ListFile;

    _vfprintf                           vfprintf;
    _perror                             perror;

    _fflush                             fflush;
    _rewind                             rewind;

    _GetFlashPrartitionBlockStatus      GetFlashPrartitionBlockStatus;
}PFILE_FUN_LIST;

#endif
