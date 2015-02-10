
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

#define CFDC_MAX_DEL_NUMBER   3                 /* ���ɾ������ */
#define CFDC_MAX_FILE_NAME    256               /* �ļ���������ֽ��� */
#define CFDC_QUE_LEN          0                 /* ����λ��ƫ�� */
#define CFDC_QUE_NUMBER       sizeof(UNSIGNED)  /* ɾ������λ��ƫ�� */
#define CFDC_QUE_FILE_NAME    (CFDC_QUE_NUMBER+sizeof(UNSIGNED)) /* �ļ�������ʼƫ�� */
#define CFDC_MAX_MESSAGE_SIZE (CFDC_QUE_FILE_NAME+CFDC_MAX_FILE_NAME) /* ������Ϣ����ֽ��� */
/* Macro used to align a data pointer to next address that meets the specified
   required alignment.
   NOTE:  The required alignment must be a power of 2 (2, 4, 8, 16, 32, etc)
   NOTE:  The next aligned address will be returned regardless of whether the
          data pointer is already aligned. */
#define CFDC_GE_MEM_PTR_ALIGN(ptr_addr, req_align)                      \
        (VOID *)(((UINT32)(ptr_addr) & (UINT32)(~((req_align) - 1))) + (UINT32)(req_align))

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
HDCM CFDC_DelQueueId;
INT CFDC_IniFlag = XJ_FALSE;
VOID (* CFDC_Lock )( VOID ) = XJ_NULL;
VOID (* CFDC_Unlock )( VOID ) = XJ_NULL;
INT (* CFDC_RemoveFile )(  const char * ) = XJ_NULL;
INT (* CFDC_FileExists )(  const char * ) = XJ_NULL;

/* Define internal function calls.  */

/* ɾ���ļ�����ģ���ʼ�� */
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

/* ����ɾ���ļ��б� */
INT CFDC_Delete_File( CHAR * fileName )
{
    INT len;
    INT status;
    CHAR * pbuf;
    UNSIGNED * p;
    CHAR buffer[ CFDC_MAX_MESSAGE_SIZE + sizeof( UNSIGNED )]; /* ��Ϣ������ */

    if( CFDC_IniFlag == XJ_FALSE )
    {
        return( XJ_UNAVAILABLE );
    }

    len = strlen( fileName );
    if(( len >= CFDC_MAX_FILE_NAME ) || ( fileName == XJ_NULL ))
    {
        return( XJ_UNAVAILABLE );
    }

    /* ��ȡ�����ڴ�ָ�� */
    pbuf = CFDC_GE_MEM_PTR_ALIGN( buffer, sizeof( UNSIGNED ) );
    /* �����ļ����� */
    strncpy(( pbuf + CFDC_QUE_FILE_NAME ), fileName, CFDC_MAX_FILE_NAME );
    p = ( UNSIGNED *)pbuf;
    /* �������ݳ��� */
    len = CFDC_QUE_FILE_NAME + strlen( ( pbuf + CFDC_QUE_FILE_NAME ) );
    len = ( len + sizeof( UNSIGNED ) - 1 ) / sizeof( UNSIGNED );
    * p ++ = len;
    /* ���ó�ʼɾ������ */
    * p = 0;

    /* �������ݵ����� */
    if( CFDC_Lock )
        CFDC_Lock();
    status = XJP_Queue_Write( CFDC_DelQueueId, pbuf );
    if( CFDC_Unlock )
        CFDC_Unlock();

    return( status );
}

/* ��̨ɾ���ļ������� */
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
        /* ��ȡ�����ڴ�ָ�� */
        pbuf = CFDC_GE_MEM_PTR_ALIGN( buffer, sizeof( UNSIGNED ) );
        /* ��ȡɾ���ļ���Ϣ */
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
                /* ɾ���ļ�ʧ�ܣ�����ļ��Ƿ���� */
                if( CFDC_FileExists( fileName ) == NU_TRUE )
                {
                    /* ɾ���ļ����ڣ����ɾ���ļ����� */
                    number = *(( UNSIGNED *)( pbuf + CFDC_QUE_NUMBER ));
                    if(++ number < CFDC_MAX_DEL_NUMBER )
                    {
                        /* ɾ��ʧ�ܴ���δ��߽�ֵ��д������ɾ�� */
                        *(( UNSIGNED *)( pbuf + CFDC_QUE_NUMBER )) = number;

                        /* �������ݵ����� */
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
