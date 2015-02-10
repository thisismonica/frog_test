
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

#define CLOG_MAX_FILE_SIZE      524288 /* �����־�ļ���С */
#define CLOG_BACKUP_FILE_INDEX  1   /* ������־�ļ���� */
#define CLOG_MESSAGE_SIZE       256 /* ������Ϣ����ֽ��� */
#define CLOG_LOG_SIZE (CLOG_MESSAGE_SIZE+32) /* ������־��Ϣ����ֽ��� */

/* ����ΪOS������� */
#ifdef WIN32
/* ȫ�ֱ������� */
/* Windows �궨�� */
#endif

#ifndef WIN32 /* ��windowsȱʡ����һЩ���Ա�ͨ������ */
/* ȫ�ֱ������� */
/* Nucleus�궨�� */
#endif

/* Define external inner-component global data references.  */

INT CLOG_IniFlag = XJ_FALSE;
HDCM CLOG_MessageId;
/* �����¼��¼��洢·�� */
CHAR CLOG_DirName[256] = CPC_LOG_DIR_NAME; /* ��־·�� */
CHAR CLOG_FileName[256] = "DeviceLog";     /* ��־�ļ����� */
CHAR CLOG_LogFileName[256] = "";           /* ��־ȫ�ļ����� */
extern NU_PROTECT * pReport_Protect;

/* Define internal function calls.  */

/* ������־���� */
INT CLOG_WriteLogData( CHAR * fileName, VOID * buf, UNSIGNED size,
    UNSIGNED counter );

/* ��־��¼ */
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

    /* ���ʱ�� */
    XJP_TM_Get_Time_Dev( & RunTime );
    pTime = & RunTime;
    sprintf( & buffer[sizeof( UNSIGNED )], "%04d-%02d-%02d  %02d:%02d:%02d:%03d %s\r\n",
        ( pTime->Year + 2000 ), pTime->Month, pTime->Day, pTime->Hour, /* ��ת����ʵ��ʱ�� */
        pTime->Minute, ( pTime->Millionsecond / 1000 ), ( pTime->Millionsecond % 1000 ), msg ); /* ����ת������ */
    /* ������ݳ��� */
    len = strlen( & buffer[sizeof( UNSIGNED )] ) + 1; /* ���������� */
    len = ( len + sizeof( UNSIGNED ) + sizeof( UNSIGNED ) - 1 )/ sizeof( UNSIGNED );
    p = ( UNSIGNED *)buffer;
    * p = len;
    /* д���� */
    status = XJP_Queue_Write( CLOG_MessageId, buffer );

    XJ_Unprotect();

    return( status );
}

/* д��־���� */
INT CLOG_WriteLogMain( VOID )
{
    INT len;
    INT status;
    UNSIGNED size;
    CHAR buffer[ CLOG_LOG_SIZE ]; /* ��ʱ������ */
    struct FileInformation fileInfo;

    if( CLOG_IniFlag == XJ_FALSE )
    {
        return( XJ_UNAVAILABLE );
    }

    status = XJ_SUCCESS;
    if( XJP_Queue_Find( CLOG_MessageId, & size ) == XJ_TRUE )
    {
        /* ��ȡ��־��Ϣ */
        memset( buffer, 0, sizeof( buffer ));
        XJ_Protect( pReport_Protect );
        status = XJP_Queue_Read( CLOG_MessageId , buffer );
        XJ_Unprotect();
        if( status == XJ_SUCCESS )
        {
            len = strlen( & buffer[sizeof( UNSIGNED )] ); /* ���������� */
            /* ������־���� */
            status = CLOG_WriteLogData( CLOG_LogFileName, & buffer[sizeof( UNSIGNED )], len, sizeof( CHAR ) );
            if( status == XJ_SUCCESS )
            {
                /* �����־�ļ���С */
                if( xj_GetFileInformation( CLOG_LogFileName,& fileInfo ) == XJ_SUCCESS )
                {
                    if( fileInfo.Size >= CLOG_MAX_FILE_SIZE )
                    {
                        /* ����������������汸����־��¼��ʹ�����ļ����½�����־��¼ */
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

/* ������־���� */
INT CLOG_WriteLogData( CHAR * fileName, VOID * buf, UNSIGNED size,
    UNSIGNED counter )
{
    INT status;
    FILE * fp;
    UNSIGNED writeSize;

    status = XJ_UNAVAILABLE;
    /* д���� */
    fp = xj_fopen( fileName, "rb+" );
    if( fp == XJ_NULL )
    {
        fp = xj_fopen( fileName, "wb" );
    }
    if( fp != XJ_NULL )
    {
        writeSize = 0;
        /* �����ļ�λ��ָ�� */
        if( xj_fseek( fp, 0, SEEK_END ) == 0 ) /* �����ɹ� */
        {
            /* д���� */
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

/* ��־ϵͳ��ʼ�� */
INT CLOG_Initialize( UNSIGNED BufSize )
{
    CLOG_IniFlag = XJ_FALSE;
    /* ������־Ŀ¼ */
    if(( xj_DirExists == XJ_NULL ) || ( xj_MakeDir == XJ_NULL ))
    {
        /* �������������ڣ�ֱ�ӷ���ʧ�� */
        return( XJ_UNAVAILABLE );
    }
    if( xj_DirExists( CLOG_DirName ) == XJ_NULL )
    {
        /* Ŀ¼�����ڣ�����Ŀ¼ */
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

/* �����ݱ�����־��¼ */
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

    /* ���ʱ�� */
    XJP_TM_Get_Time_Dev( & RunTime );
    pTime = & RunTime;
    sprintf( & buffer[sizeof( UNSIGNED )], "%04d-%02d-%02d  %02d:%02d:%02d:%03d %s\r\n",
        ( pTime->Year + 2000 ), pTime->Month, pTime->Day, pTime->Hour, /* ��ת����ʵ��ʱ�� */
        pTime->Minute, ( pTime->Millionsecond / 1000 ), ( pTime->Millionsecond % 1000 ), msg ); /* ����ת������ */
    /* ������ݳ��� */
    len = strlen( & buffer[sizeof( UNSIGNED )] ) + 1; /* ���������� */
    len = ( len + sizeof( UNSIGNED ) + sizeof( UNSIGNED ) - 1 )/ sizeof( UNSIGNED );
    p = ( UNSIGNED *)buffer;
    * p = len;
    /* д���� */
    status = XJP_Queue_Write( CLOG_MessageId, buffer );

    return( status );
}
