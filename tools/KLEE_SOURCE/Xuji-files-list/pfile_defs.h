
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

#define PFILE_FUN_LIST_NAME "PlatFileList" /* �ļ�ϵͳ���������� */

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
//�������ܣ�ɾ���ļ���
//��    ����path   �ļ�·����
//�� �� ֵ�������ɹ�����0������ʧ�ܷ���-1��
typedef int (* _RemoveFile )( const char * path );

//�������ܣ�����Ŀ¼��
//��    ����path   Ŀ¼·����
//�� �� ֵ�������ɹ�����0������ʧ�ܷ���-1��
typedef int (* _MakeDir )( const char * path );

//�������ܣ��������ļ���
//��    ����oldPath �ɵ��ļ�·����newPath �µ��ļ�·����
//�� �� ֵ�������ɹ�����0������ʧ�ܷ���-1��
typedef int (* _RenameFile )( const char * oldPath, const char * newPath );

//�������ܣ�������Ŀ¼��
//��    ����oldPath �ɵ�Ŀ¼·����newPath �µ�Ŀ¼·����
//�� �� ֵ�������ɹ�����0������ʧ�ܷ���-1��
typedef int (* _RenameDir )( const char * oldPath, const char * newPath );

//�������ܣ�ɾ��Ŀ¼��
//��    ����path Ŀ¼·����
//�� �� ֵ�������ɹ�����0������ʧ�ܷ���-1��
typedef int (* _RemoveDir )( const char * path );

//�������ܣ������ļ���Ϣ��
//��    ����path �ļ�·����info �ļ���Ϣ��
//�� �� ֵ�������ɹ�����0������ʧ�ܷ���-1��
typedef int (* _SetFileInformation )( const char * path, const struct FileInformation * info );

//�������ܣ���ȡ�ļ���Ϣ��
//��    ����path �ļ�·����info �ļ���Ϣ��
//�� �� ֵ�������ɹ�����0������ʧ�ܷ���-1��
typedef int (* _GetFileInformation )( const char * path, struct FileInformation * info );

//�������ܣ���ѯ�ļ��Ƿ���ڡ�
//��    ����path �ļ�·����
//�� �� ֵ�����ڷ���1�������ڷ���0��
typedef int (* _FileExists )( const char * path );

//�������ܣ���ѯĿ¼�Ƿ���ڡ�
//��    ����path Ŀ¼··����
//�� �� ֵ�����ڷ���1�������ڷ���0��
typedef int (* _DirExists )( const char * path );

//�������ܣ���ѯ���̷������д洢�ռ��С��
//��    ����root ���̷�����·����
//�� �� ֵ�����̷������д洢�ռ��ֽ�����
typedef int (* _GetFreeSpace )( const char * root );

//�������ܣ�����ָ��Ŀ¼�µ��ļ�������ÿ���ļ��ص�ָ���ĺ�����
//��    ����dirPath Ŀ¼·���� filtercallback �ص�������filtercallbackcontext�ص�����������
//�� �� ֵ�����Ŀ¼���ڣ�����Ŀ¼���ļ��ĸ��������򣬷���-1��
typedef int (* _ListFile )( const char * dirPath,
    int (* filtercallback )( struct FileInformation * info, void * context ),
    void * filtercallbackcontext );

//�������ܣ���ȡFLASH�����ڴ洢���״̬��
//��    ����root        FLASH������·����
//          retired �����������
//          used        ���ʹ�õĿ�����
//          empty   ������еĿ�����
//�� �� ֵ������ʧ�ܷ���-1���ɹ�����0��
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
