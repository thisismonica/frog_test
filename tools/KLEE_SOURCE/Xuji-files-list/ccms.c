
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

#define CCMS_QUE_MAX_MES_COUNT  50     /* ����ĸ��� */
#define CCMS_FILE_MAIN_SERIAL   01     /* ���ļ���� */
#define CCMS_FILE_BACKUP_SERIAL 02     /* �����ļ���� */
#define CCMS_SEMAPHORE_TIMEOUT  200    /* ��ȡ�ź������ʱ�ȴ�ʱ�� */

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
HDCM CCMS_MessageId;
/* �洢·�� */
CHAR CCMS_DirName[256] = CPC_CMS_DIR_NAME;    /* ͨ�ű���·�� */
CHAR CCMS_WFileName[256] = "ComMsW";          /* ͨ��д�����ļ����� */
CHAR CCMS_RFileName[256] = "ComMsR";          /* ͨ�Ŷ������ļ����� */
extern NU_PROTECT * pReport_Protect;

/*====================================*/
/* Define internal function calls.  */
/*------------------------------------*/
/* �������� */
INT CCMS_File_Save( CHAR * fileName, VOID * buf, UNSIGNED size,
    UNSIGNED counter );
/*------------------------------------*/
/* д�ļ�(���׷��) */
INT CCMS_WriteComMessData( CHAR * fileName, VOID * buf, UNSIGNED size,
    UNSIGNED counter );
/*------------------------------------*/
/* ���ļ� */
INT CCMS_ReadComMessData( CHAR * fileName, VOID * buf, UNSIGNED size,
    UNSIGNED counter, UNSIGNED offset );
/*------------------------------------*/
/* ��ȡ���ĸ��� */
INT CCMS_GetMessCount( CHAR * fileName, INT * iCount );
/*------------------------------------*/
/* ��ȡ���� */
INT CCMS_ReadComMs( CHAR * msg, INT iMaxLen, INT * iActLen, INT iDelFlag );
/*------------------------------------*/
/* ɾ�����ļ� */
VOID CCMS_DelComMsRFile( VOID );
/*------------------------------------*/
/* ���д��ʱ�ļ����ڴ������Ƿ���� */
/* �ڴ����ݴ���˵��д��ʱ�ļ�ʧ��,��ʱ�ļ�����˵��װ�ó����쳣�ϵ� */
VOID  CCMS_CheckFileInfo( VOID );
/*------------------------------------*/
/* �޸�ͨѶ�����ļ� */
INT CCMS_RepairFileData( CHAR * fileName );
/*------------------------------------*/
/* �ڳ�ʼ���ж�ȡ������Ϣ���ܻ�ʧ�� */
INT CCMS_MessCountInit( VOID );
/*====================================*/
/* ȫ��������Ϣ�ṹ */
typedef struct CCMS_TOTAL_INFO_struct{
    INT              iInitFlag;      /* ��ʼ����ʶ */
    INT              iMessInitFlag;  /* �ļ����ĳ�ʼ����ʶ */
    INT              iWMaxCount;     /* ���д���ĸ��� */
    INT              iRMaxCount;     /* �������ĸ��� */
    INT              iMaxMessLen;    /* ����ĳ��� */
    HDCM             WriteMessId;    /* д������Ϣ���� */
    HDCM             ReadMessId;     /* ��������Ϣ���� */
    CHAR             WriteFileName[256];     /* д�ļ����� */
    CHAR             WriteTempFileName[256]; /* д��ʱ�ļ����� */
    CHAR             ReadFileName[256];  /* ���ļ����� */
    INT              iWriteCount;   /* д���ĸ��� */
    INT              iFWriteCount;  /* �ļ���д���ĸ��� */
    INT              iReadCount;    /* �����ĸ��� */
    INT              iFReadCount;   /* �ļ��ж����ĸ��� */
    INT              iUMessBufLen;  /* һ�����Ļ�������UNSIGNED���� */
    CHAR             * pMemBuf;     /* д���ݵ�ʱ��ʹ�� */
    UNSIGNED         uMemSize;      /* �ڴ����ݴ�С */
    NU_SEMAPHORE     Semaphore_CB;   /* ���ļ��Ĳ�����Ҫ��ȡ�ź��� */
}CCMS_TOTAL_INFO;

CCMS_TOTAL_INFO  CCMS_TotalInfo;

/*==========================================*/
/* ͨ�ű���ϵͳ��ʼ�� */
INT CCMS_Initialize( INT iMaxCount, INT iMaxMessLen )
{
    CCMS_TOTAL_INFO  * this;
    INT  iFixLen;
    /*=======================*/
    this = & CCMS_TotalInfo;
    memset( & CCMS_TotalInfo, 0, sizeof( CCMS_TOTAL_INFO ));

    /* ����ͨ�ű���Ŀ¼ */
    if(( xj_DirExists == XJ_NULL ) || ( xj_MakeDir == XJ_NULL ))
    {
        /* �������������ڣ�ֱ�ӷ���ʧ�� */
        return( XJ_UNAVAILABLE );
    }
    if( xj_DirExists( CCMS_DirName ) == XJ_NULL )
    {
        /* Ŀ¼�����ڣ�����Ŀ¼ */
        if( xj_MakeDir( CCMS_DirName ) != XJ_SUCCESS )
        {
            return( XJ_UNAVAILABLE );
        }
    }
    /* ��ʼ���ļ����� */
    sprintf( this->WriteFileName, "%s/%s", CCMS_DirName, CCMS_WFileName );
    sprintf( this->ReadFileName, "%s/%s", CCMS_DirName, CCMS_RFileName );
    sprintf( this->WriteTempFileName, "%s/%s_Temp", CCMS_DirName, CCMS_WFileName );

    /* �����ź��� */
    NU_Create_Semaphore(& this->Semaphore_CB, "ComMs", 1, NU_FIFO );

    /* ����̶����ĳ���(�ַ������ȼ�¼��������+�س���) */
    iFixLen = ( iMaxMessLen + sizeof( UNSIGNED ) + 2 + sizeof( UNSIGNED ) - 1 )/ sizeof( UNSIGNED );
    /* д������Ϣ���� */
    this->WriteMessId = XJP_Queue_Create(( iFixLen * CCMS_QUE_MAX_MES_COUNT ),
        XJP_EVENT_FIXED_SIZE, iFixLen );
    /* ��������Ϣ���� */
    this->ReadMessId = XJP_Queue_Create(( iFixLen * CCMS_QUE_MAX_MES_COUNT ),
        XJP_EVENT_FIXED_SIZE, iFixLen );
    /* ����ĸ��� */
    this->iWMaxCount = iMaxCount;
    /* ���ļ������� */
    this->iRMaxCount = iMaxCount / 2;
    /* ����ĳ��� */
    this->iMaxMessLen = iMaxMessLen;
    /* һ�����Ļ�������UNSIGNED���� */
    this->iUMessBufLen = iFixLen;
    /* ���ĸ�����û�г�ʼ�� */
    this->iInitFlag = XJ_TRUE;
    return( XJ_SUCCESS );
}

/*====================================*/
/* дͨ�ű��� */
INT CCMS_WriteComMs( CHAR * msg, INT iLen, UNSIGNED uTimeOut )
{
    INT    status = XJ_UNAVAILABLE;
    CCMS_TOTAL_INFO  * this;
    UNSIGNED * pMesBuffer;   /* ���ݻ����� */
    CHAR     * pTempBuffer;  /* ���ݻ����� */
    INT      iSize;
    /*=======================*/
    this = & CCMS_TotalInfo;

    if(( iLen > this->iMaxMessLen )
        || ( msg == XJ_NULL )
        || ( this->iMessInitFlag == XJ_FALSE ))
    {
        return( XJ_UNAVAILABLE );
    }

    /* ���ռ䲻�����ѷ��ͱ���ɾ�� */
    /* ֻ���ڶ��ļ������ݵ�ʱ��ɾ���������� */
    if (( this->iWriteCount >= this->iWMaxCount )
        && ( this->iFReadCount ))
    {
        /* ���ļ�������Ҫ��ȡ�ź��� */
        if( NU_Obtain_Semaphore(& this->Semaphore_CB, uTimeOut ) == XJ_SUCCESS )
        {
            /* ɾ�������ļ� */
            CCMS_DelComMsRFile();
            /* �ͷ��ź��� */
            NU_Release_Semaphore(& this->Semaphore_CB );
        }
        else
        {
            return( XJ_UNAVAILABLE );
        }
    }
    /* �ж�δ���ͱ��ĸ����Ƿ�������Χ֮�� */
    if ( this->iWriteCount < this->iWMaxCount )
    {
        iSize = ( this->iUMessBufLen * sizeof( UNSIGNED ));
        /* ��̬�����ڴ�ռ� */
        pMesBuffer = malloc( iSize );
        /* �жϿռ��Ƿ�����ɹ� */
        if( pMesBuffer )
        {
            memset( pMesBuffer, 0, iSize );
            XJ_Protect( pReport_Protect );
            memcpy( ( CHAR *)( pMesBuffer + 1 ), msg, iLen ); /* ���ӽ����������� */
            pTempBuffer = ( CHAR *)pMesBuffer;
            /* ��������������λ�� */
            pTempBuffer[iSize - 2] = '\r';
            pTempBuffer[iSize - 1] = '\n';
            /* ��¼���ĳ��� */
            * pMesBuffer = iLen;
            /* д���� */
            status = XJP_Queue_Write( this->WriteMessId, pMesBuffer );
            if( status == XJ_SUCCESS )
            {
                /* �������� */
                this->iWriteCount += 1;
            }
            XJ_Unprotect();
            free( pMesBuffer );
        }
    }

    return( status );
}

/*==========================================*/
/* �û���ȡ���� */
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
    /* ���ļ�������Ҫ��ȡ�ź��� */
    if( NU_Obtain_Semaphore(& this->Semaphore_CB, uTimeOut ) == XJ_SUCCESS )
    {
        /* ��ȡ���� */
        iStatus = CCMS_ReadComMs( msg, iMaxLen, iActLen, 0 );
        /* �ͷ��ź��� */
        NU_Release_Semaphore(& this->Semaphore_CB );
    }

    return( iStatus );
}

/*==========================================*/
/* ɾ������ */
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

    /* ���ļ�������Ҫ��ȡ�ź��� */
    if( NU_Obtain_Semaphore(& this->Semaphore_CB, uTimeOut ) == XJ_SUCCESS )
    {
        /* ɾ������ */
        iStatus = CCMS_ReadComMs( XJ_NULL, 0, XJ_NULL, 1 );
        /* �ͷ��ź��� */
        NU_Release_Semaphore(& this->Semaphore_CB );
    }
    return( iStatus );
}

/*==========================================*/
/* дͨ�ű������������� */
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
    /* �������л�ȡ�ź��������еȴ� */
    if( NU_Obtain_Semaphore(& this->Semaphore_CB, CCMS_SEMAPHORE_TIMEOUT ) == XJ_SUCCESS )
    {
        /* �ڳ�ʼ���ж�ȡ������Ϣ���ܻ�ʧ�� */
        CCMS_MessCountInit();
        /* ���д��ʱ�ļ����ڴ������Ƿ���� */
        /* �ڴ����ݴ���˵��д��ʱ�ļ�ʧ��,��ʱ�ļ�����˵��װ�ó����쳣�ϵ� */
        CCMS_CheckFileInfo();
        status = XJ_SUCCESS;
        /* ��̬�����ڴ�ռ� */
        pMesBuffer = malloc( this->iUMessBufLen * sizeof( UNSIGNED ));
        /* �жϿռ��Ƿ�����ɹ� */
        if( pMesBuffer )
        {
            memset( pMesBuffer, 0,( this->iUMessBufLen * sizeof( UNSIGNED )));
            /* ���д���� */
            if( XJP_Queue_Find( this->WriteMessId, & size ) == XJ_TRUE )
            {
                XJ_Protect( pReport_Protect );
                status = XJP_Queue_Read( this->WriteMessId , pMesBuffer );
                XJ_Unprotect();
                if( status == XJ_SUCCESS )
                {
                    /* ������д���ļ�ϵͳ�� */
                    size = this->iUMessBufLen * sizeof( UNSIGNED );
                    /* д�ļ� */
                    status = CCMS_WriteComMessData( this->WriteFileName, pMesBuffer, size, sizeof( CHAR ));
                    /* �ж�д�����Ƿ�ɹ� */
                    if( status == XJ_SUCCESS )
                    {
                        /* �������� */
                        this->iFWriteCount += 1;
                    }
                    else
                    {
                        /* ���д�ļ�ʧ��,��������д�����ݶ����� */
                        XJ_Protect( pReport_Protect );
                        status = XJP_Queue_Write( this->WriteMessId , pMesBuffer );
                        XJ_Unprotect();
                    }
                }
            }

            /* �������� */
            if( XJP_Queue_Find( this->ReadMessId, & size ) == XJ_TRUE )
            {
                memset( pMesBuffer, 0,( this->iUMessBufLen * sizeof( UNSIGNED )));
                XJ_Protect( pReport_Protect );
                status = XJP_Queue_Read( this->ReadMessId , pMesBuffer );
                XJ_Unprotect();
                if( status == XJ_SUCCESS )
                {
                    /* ������д���ļ�ϵͳ�� */
                    size = this->iUMessBufLen * sizeof( UNSIGNED );
                    /* д�ļ� */
                    status = CCMS_WriteComMessData( this->ReadFileName, pMesBuffer, size, sizeof( CHAR ));
                    /* �ж�д�����Ƿ�ɹ� */
                    if( status == XJ_SUCCESS )
                    {
                        /* �������� */
                        this->iFReadCount += 1;
                    }
                    else
                    {
                        /* ���д�ļ�ʧ��,��������д�����ݶ����� */
                        XJ_Protect( pReport_Protect );
                        status = XJP_Queue_Write( this->ReadMessId , pMesBuffer );
                        XJ_Unprotect();
                    }
                }
            }
            free( pMesBuffer );
        }
        /* �ж��ļ��Ƿ�ﵽһ����С */
        if( this->iFReadCount >= this->iRMaxCount )
        {
            /* ִ��ɾ�����ļ����� */
            CCMS_DelComMsRFile();
        }
        /* �ͷ��ź��� */
        NU_Release_Semaphore(& this->Semaphore_CB );
    }

    return( XJ_SUCCESS );
}

/*==========================================*/
/* �޸�ͨѶ�����ļ� */
INT CCMS_RepairFileData_Main( VOID )
{
    CCMS_TOTAL_INFO  * this;
    INT  iStatus;
    /*=======================*/
    this = & CCMS_TotalInfo;
    /* �ж������Ƿ��ѳ�ʼ�� */
    if( this->iInitFlag == XJ_TRUE )
    {
        iStatus = NU_Obtain_Semaphore(& this->Semaphore_CB, CCMS_SEMAPHORE_TIMEOUT );
        /* ��ȡ�ź��� */
        if( iStatus == XJ_SUCCESS )
        {
            /* �޸�ͨѶ����д�ļ� */
            iStatus =  CCMS_RepairFileData( this->WriteFileName );
            /* �޸�ͨѶ���Ķ��ļ� */
            iStatus =  CCMS_RepairFileData( this->ReadFileName );
            /* �ͷ��ź��� */
            NU_Release_Semaphore(& this->Semaphore_CB );
        }
    }
    return( XJ_SUCCESS );
}

/*==========================================*/
/* �ڲ�ʹ�ú��� */
/* ��ȡ���ĸ��� */
INT CCMS_GetMessCount( CHAR * fileName, INT * iCount )
{
    INT status = XJ_UNAVAILABLE;
    CHAR fileName1[256]; /* ����·����������'/'�� */
    CHAR fileName2[256]; /* ����·����������'/'�� */
    CCMS_TOTAL_INFO  * this;
    struct FileInformation fileInfo;
    INT   iStatus = XJ_UNAVAILABLE;
    /*=======================*/
    this = & CCMS_TotalInfo;
    sprintf( fileName1, "%s%d", fileName, CCMS_FILE_MAIN_SERIAL );   /* ����1�ļ����� */
    sprintf( fileName2, "%s%d", fileName, CCMS_FILE_BACKUP_SERIAL ); /* ����2�ļ����� */
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
/* д�ļ�(���׷��) */
INT CCMS_WriteComMessData( CHAR * fileName, VOID * buf, UNSIGNED size,
    UNSIGNED counter )
{
    INT status = XJ_UNAVAILABLE;
    INT fileStatus1;
    INT fileStatus2;
    CHAR fileName1[256]; /* ����·����������'/'�� */
    CHAR fileName2[256]; /* ����·����������'/'�� */
    INT iStatus = 20;    /* ����״̬��ʼֵ */

    sprintf( fileName1, "%s%d", fileName, CCMS_FILE_MAIN_SERIAL );   /* ����1�ļ����� */
    sprintf( fileName2, "%s%d", fileName, CCMS_FILE_BACKUP_SERIAL ); /* ����2�ļ����� */
    /* �����ʱ�ļ��Ƿ���� */
    if( xj_FileExists( fileName ) == XJ_TRUE )
    {
        /* ��ʱ�ļ��Ѿ����ڣ�ɾ����ʱ�ļ� */
        xj_RemoveFile( fileName );
    }

    /* �޸�����ȫ���������ļ� */
    fileStatus1 = xj_FileExists( fileName1 );
    fileStatus2 = xj_FileExists( fileName2 );
    if(( fileStatus1 == NU_TRUE ) && ( fileStatus2 == XJ_FALSE ))
    {
        /* �������ļ��ޱ����ļ����������ļ��������ļ� */
        XJP_Copy_NVdata( fileName1, fileName2 );
    }
    else if(( fileStatus1 == NU_FALSE ) && ( fileStatus2 == XJ_TRUE ))
    {
        /* ���б����ļ������ļ������Ʊ����ļ������ļ� */
        XJP_Copy_NVdata( fileName2, fileName1 );
    }

    /* ��������ֵ���ݣ������������ļ�������ͬ��ͬʱ���ڻ򲻴��ڣ�������ֵ���� */
    fileStatus1 = xj_FileExists( fileName1 );
    fileStatus2 = xj_FileExists( fileName2 );
    if((( fileStatus1 == NU_TRUE ) && ( fileStatus2 == XJ_TRUE ))
        || (( fileStatus1 == NU_FALSE ) && ( fileStatus2 == XJ_FALSE )))
    {
        /* �ȱ����1������ */
        if( fileStatus1 == XJ_TRUE )
        {
            status = xj_RenameFile( fileName1, fileName );
            if( status != XJ_SUCCESS )
            {
                /* ����ʵ�ʴ���ֵ,���ڲ������� */
                status = ( iStatus + 1 );
                return( status );
            }
        }
        status = CCMS_File_Save( fileName, buf, size, counter );
        if( status != XJ_SUCCESS )
        {
            /* ����ʵ�ʴ���ֵ,���ڲ������� */
            status = ( iStatus + 2 );
            return( status );
        }
        status = xj_RenameFile( fileName, fileName1 );
        if( status != XJ_SUCCESS )
        {
            /* ����ʵ�ʴ���ֵ,���ڲ������� */
            status = ( iStatus + 3 );
            return( status );
        }
        /* �����2������ */
        if( fileStatus2 == XJ_TRUE )
        {
            status = xj_RenameFile( fileName2, fileName );
            if( status != XJ_SUCCESS )
            {
                /* ����ʵ�ʴ���ֵ,���ڲ������� */
                status = ( iStatus + 4 );
                return( status );
            }
        }
        status = CCMS_File_Save( fileName, buf, size, counter );
        if( status != XJ_SUCCESS )
        {
            /* ����ʵ�ʴ���ֵ,���ڲ������� */
            status = ( iStatus + 5 );
            return( status );
        }
        status = xj_RenameFile( fileName, fileName2 );
        if( status != XJ_SUCCESS )
        {
            /* ����ʵ�ʴ���ֵ,���ڲ������� */
            status = ( iStatus + 6 );
            return( status );
        }
    }
    return( status );
}

/*==========================================*/
/* ���ļ� */
INT CCMS_ReadComMessData( CHAR * fileName, VOID * buf, UNSIGNED size,
    UNSIGNED counter, UNSIGNED offset )
{
    INT status = XJ_UNAVAILABLE;
    CHAR fileName1[256]; /* ����·����������'/'�� */
    CHAR fileName2[256]; /* ����·����������'/'�� */
    INT iStatus;

    sprintf( fileName1, "%s%d", fileName, CCMS_FILE_MAIN_SERIAL ); /* ����1�ļ����� */
    sprintf( fileName2, "%s%d", fileName, CCMS_FILE_BACKUP_SERIAL ); /* ����2�ļ����� */
    if( xj_FileExists( fileName1 ) == XJ_TRUE )
    {
        /* �ļ��Ѿ����� */
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
        /* �ļ��Ѿ����� */
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
/* �������� */
INT CCMS_File_Save( CHAR * fileName, VOID * buf, UNSIGNED size,
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

/*==========================================*/
/* ��ȡ���� */
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
    /* �ж��Ƿ���δ��ȡ�ı��� */
    if( iFWriteCount > iReadCount )
    {
        uSize = this->iUMessBufLen * sizeof( UNSIGNED );
        uOffset = iReadCount * uSize;
        /* ��̬�����ڴ�ռ� */
        pMesBuffer = malloc( uSize );
        /* �жϿռ��Ƿ�����ɹ� */
        if( pMesBuffer )
        {
            memset( pMesBuffer, 0, uSize );
            /* ���ļ� */
            iStatus =  CCMS_ReadComMessData( this->WriteFileName, pMesBuffer,
                uSize, sizeof( CHAR ), uOffset );
            if( iStatus == XJ_SUCCESS )
            {
                if( iDelFlag )
                {
                    XJ_Protect( pReport_Protect );
                    /* д���� */
                    if( XJP_Queue_Write( this->ReadMessId, pMesBuffer ) == XJ_SUCCESS )
                    {
                        /* �������� */
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
        /* ��д���ĸ���С�ڱ��ĸ�����˵���ļ������쳣,��Ҫ���ļ�ɾ�� */
        /* ɾ�����ļ� */
        CCMS_DelComMsRFile();
    }
    return( iStatus );
}

/*==========================================*/
/* ɾ�����ļ� */
VOID CCMS_DelComMsRFile( VOID )
{
    INT status = XJ_UNAVAILABLE;
    CHAR fileName1[256]; /* ����·����������'/'�� */
    CHAR fileName2[256]; /* ����·����������'/'�� */
    CHAR * pFileName = XJ_NULL;
    CCMS_TOTAL_INFO  * this;
    INT    iFlag = XJ_FALSE;
    FILE * fp;
    UNSIGNED readSize;
    UNSIGNED uOffsetSize;
    UNSIGNED uSize;
    INT      iDelTFlag = XJ_FALSE;   /* ɾ�������ļ���ʶ */
    struct FileInformation fileInfo;
    /*=======================*/
    this = & CCMS_TotalInfo;

    /* �ж��Ƿ񻹴���δ���͵ı��� */
    if(( this->iFWriteCount > this->iFReadCount )
        && ( this->iWriteCount >= this->iFWriteCount )
        && ( this->iReadCount >= this->iFReadCount )
        && ( this->iFWriteCount >= this->iReadCount ) )
    {
        sprintf( fileName1, "%s%d", this->WriteFileName, CCMS_FILE_MAIN_SERIAL );   /* ����1�ļ����� */
        sprintf( fileName2, "%s%d", this->WriteFileName, CCMS_FILE_BACKUP_SERIAL ); /* ����2�ļ����� */
        /* �Ƚ�δ���ͱ��Ķ�ȡ���� */
        /* �ж��������ļ��ĸ���Ч */
        if( xj_GetFileInformation( fileName1,& fileInfo ) == XJ_SUCCESS )
        {
            pFileName = fileName1;
        }
        else if( xj_GetFileInformation( fileName2,& fileInfo ) == XJ_SUCCESS )
        {
            pFileName = fileName2;
        }

        /* �Ƿ�����Ч�ļ� */
        if( pFileName )
        {
            /* ��ȡ����ƫ�� */
            uOffsetSize = this->iFReadCount * this->iUMessBufLen * sizeof( UNSIGNED );
            /* �����ļ���С */
            uSize = this->iFWriteCount * this->iUMessBufLen * sizeof( UNSIGNED );
            /* �ж��ļ���С�Ƿ���ȷ */
            if((( UNSIGNED )( fileInfo.Size ) > uOffsetSize )
                && ( ( UNSIGNED )( fileInfo.Size ) == uSize ))
            {
                /* ʣ������size */
                uSize = fileInfo.Size - uOffsetSize;
                /* �����ڴ�ռ� */
                this->pMemBuf = malloc( uSize );
                /* �ж��ڴ��Ƿ�����ɹ� */
                if( this->pMemBuf )
                {
                    /* �����ݱ����� */
                    fp = xj_fopen( pFileName, "rb" );
                    if( fp != XJ_NULL )
                    {
                        readSize = 0;
                        /* �����ļ�λ��ָ�� */
                        if( xj_fseek( fp, uOffsetSize, 0 ) == 0 )
                        {
                            /* ������ */
                            readSize = ( UNSIGNED )xj_fread( this->pMemBuf, uSize, sizeof( CHAR ), fp );
                        }
                        xj_fclose( fp );
                        if( readSize == sizeof( CHAR ) )
                        {
                            /* �����ڴ�����size */
                            this->uMemSize = uSize;
                            /* д����ʱ�ļ��� */
                            /* ���д��ʱ�ļ����ڴ������Ƿ���� */
                            /* �ڴ����ݴ���˵��д��ʱ�ļ�ʧ��,��ʱ�ļ�����˵��װ�ó����쳣�ϵ� */
                            CCMS_CheckFileInfo();
                        }
                        else
                        {
                            /* ��ȡ����ʧ��,�ͷ��ڴ�ռ� */
                            free( this->pMemBuf );
                            this->pMemBuf = XJ_NULL;
                        }
                    }
                    else
                    {
                        /* ���ļ�ʧ��,�ͷ��ڴ�ռ� */
                        free( this->pMemBuf );
                        this->pMemBuf = XJ_NULL;
                    }
                }
            }
            else
            {
                /* �ļ���С����ȷ,ɾ�������ļ� */
                iDelTFlag = XJ_TRUE;
            }
        }
        else
        {
            /* û���ҵ���Ч�ļ�ֱ��ɾ�� */
            iDelTFlag = XJ_TRUE;
        }
    }
    else
    {
        /* ��δ����������ɾ����/д�ļ���ʶ */
        iDelTFlag = XJ_TRUE;
    }

    /* ɾ����д�ļ� */
    if ( iDelTFlag == XJ_TRUE )
    {
        sprintf( fileName1, "%s%d", this->ReadFileName, CCMS_FILE_MAIN_SERIAL );   /* ����1�ļ����� */
        sprintf( fileName2, "%s%d", this->ReadFileName, CCMS_FILE_BACKUP_SERIAL ); /* ����2�ļ����� */
        xj_RemoveFile( fileName1 );
        xj_RemoveFile( fileName2 );
        sprintf( fileName1, "%s%d", this->WriteFileName, CCMS_FILE_MAIN_SERIAL );   /* ����1�ļ����� */
        sprintf( fileName2, "%s%d", this->WriteFileName, CCMS_FILE_BACKUP_SERIAL ); /* ����2�ļ����� */
        xj_RemoveFile( fileName1 );
        xj_RemoveFile( fileName2 );
        /* �������� */
        this->iFWriteCount = 0;
        /* �������� */
        this->iWriteCount = 0;
        /* �������� */
        this->iFReadCount = 0;
        /* �������� */
        this->iReadCount = 0;
    }
}

/*==========================================*/
/* ���д��ʱ�ļ����ڴ������Ƿ���� */
/* �ڴ����ݴ���˵��д��ʱ�ļ�ʧ��,��ʱ�ļ�����˵��װ�ó����쳣�ϵ� */
VOID  CCMS_CheckFileInfo( VOID )
{
    CHAR fileName1[256]; /* ����·����������'/'�� */
    CHAR fileName2[256]; /* ����·����������'/'�� */
    CCMS_TOTAL_INFO  * this;
    INT  status;
    /*=======================*/
    this = & CCMS_TotalInfo;

    sprintf( fileName1, "%s%d", this->WriteFileName, CCMS_FILE_MAIN_SERIAL );   /* ����1�ļ����� */
    sprintf( fileName2, "%s%d", this->WriteFileName, CCMS_FILE_BACKUP_SERIAL ); /* ����2�ļ����� */
    /* �ж��ڴ������Ƿ���� */
    if( this->pMemBuf )
    {
        /* �����ʱ�ļ��Ƿ���� */
        if( xj_FileExists( this->WriteTempFileName ) == XJ_TRUE )
        {
            /* ��ʱ�ļ��Ѿ����ڣ�ɾ����ʱ�ļ� */
            xj_RemoveFile( this->WriteTempFileName );
        }
        /* �������ݵ���ʱ�ļ��� */
        status = CCMS_File_Save( this->WriteTempFileName,
            this->pMemBuf, this->uMemSize, sizeof( CHAR ) );
        if( status == XJ_SUCCESS )
        {
            /* ��ʱ�ļ�д�ɹ���Ϳ����ͷ��ڴ�ռ� */
            free( this->pMemBuf );
            this->pMemBuf = XJ_NULL;
            this->uMemSize = 0;
        }
    }
    /* �ж���ʱ�ļ��Ƿ���� */
    if( xj_FileExists( this->WriteTempFileName ))
    {
        /* �������� */
        XJP_Copy_NVdata( this->WriteTempFileName, fileName1 );
        /* �������� */
        XJP_Copy_NVdata( this->WriteTempFileName, fileName2 );
        /* ɾ�����ļ� */
        sprintf( fileName1, "%s%d", this->ReadFileName, CCMS_FILE_MAIN_SERIAL ); /* ����2�ļ����� */
        sprintf( fileName2, "%s%d", this->ReadFileName, CCMS_FILE_BACKUP_SERIAL ); /* ����1�ļ����� */
        /* �ж��ļ�1�Ƿ���� */
        if( xj_FileExists( fileName1 ))
        {
            xj_RemoveFile( fileName1 );
        }
        /* �ж��ļ�2�Ƿ���� */
        if( xj_FileExists( fileName2 ))
        {
            xj_RemoveFile( fileName2 );
        }
        /* ɾ����ʱ�ļ� */
        xj_RemoveFile( this->WriteTempFileName );
        /* �������� */
        this->iFWriteCount -= this->iFReadCount;
        /* �������� */
        this->iWriteCount -= this->iFReadCount;
        /* �������� */
        this->iFReadCount = 0;
        /* �������� */
        this->iReadCount = 0;
    }
}

/*==========================================*/
/* �޸�ͨѶ�����ļ� */
INT CCMS_RepairFileData( CHAR * fileName )
{
    INT status;
    INT status1;
    INT status2;
    UNSIGNED crc_code1;
    UNSIGNED crc_code2;
    CHAR fileName1[64]; /* ����·����������'/'�� */
    CHAR fileName2[64]; /* ����·����������'/'�� */
    /*=======================*/
    status = XJ_UNAVAILABLE;
    /* ���ļ����� */
    sprintf( fileName1, "%s%d", fileName, CCMS_FILE_MAIN_SERIAL );
    /* ���ļ����� */
    sprintf( fileName2, "%s%d", fileName, CCMS_FILE_BACKUP_SERIAL );
    status1 = XJP_CalculateFileCRC( fileName1, & crc_code1 );
    status2 = XJP_CalculateFileCRC( fileName2, & crc_code2 );
    if(( status1 == XJ_SUCCESS ) && ( status2 == XJ_SUCCESS ))
    {
        /* �������ļ������� */
        if( crc_code1 != crc_code2 )
        {
            /* У������������ļ�Ϊ׼ */
            if( XJP_Copy_NVdata( fileName1, fileName2 )== XJ_SUCCESS )
            {
                status = XJ_SUCCESS;
            }
            else
            {
                /* �޸��ļ����� */
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
            /* ���ļ����ã������ļ����� */
            if( XJP_Copy_NVdata( fileName1, fileName2 )== XJ_SUCCESS )
            {
                status = XJ_SUCCESS;
            }
            else
            {
                /* �޸��ļ����� */
                status = XJ_END_OF_LOG;
            }
        }
        else if( status2 == XJ_SUCCESS )
        {
            /* �����ļ����ã����ļ����� */
            if( XJP_Copy_NVdata( fileName2, fileName1 )== XJ_SUCCESS )
            {
                status = XJ_SUCCESS;
            }
            else
            {
                /* �޸��ļ����� */
                status = XJ_END_OF_LOG;
            }
        }
        else
        {
            /* �޸��ļ����� */
            status = XJ_END_OF_LOG;
        }
    }
    return( status );
}

/*==========================================*/
/* �ڳ�ʼ���ж�ȡ������Ϣ���ܻ�ʧ�� */
INT CCMS_MessCountInit( VOID )
{
    CCMS_TOTAL_INFO  * this;
    static INT iInitFlag = XJ_FALSE;
    CHAR fileName1[64]; /* ����·����������'/'�� */
    CHAR fileName2[64]; /* ����·����������'/'�� */
    INT iStatus = XJ_UNAVAILABLE;
    /*=======================*/
    this = & CCMS_TotalInfo;

    /* ��ʼ����ʶ */
    if(( this->iInitFlag ) && ( this->iMessInitFlag == XJ_FALSE ))
    {
        /* ��ȡ���ĸ��� */
        CCMS_GetMessCount( this->WriteFileName,& this->iWriteCount );
        this->iFWriteCount = this->iWriteCount;
        /* ��ȡ���ĸ��� */
        CCMS_GetMessCount( this->ReadFileName,& this->iReadCount );
        this->iFReadCount = this->iReadCount;

        /* ���ĸ���Ϊ0��������ļ� */
        if( this->iFWriteCount == 0 )
        {
            sprintf( fileName1, "%s%d", this->ReadFileName, CCMS_FILE_MAIN_SERIAL );   /* ����1�ļ����� */
            sprintf( fileName2, "%s%d", this->ReadFileName, CCMS_FILE_BACKUP_SERIAL ); /* ����2�ļ����� */
            xj_RemoveFile( fileName1 );
            xj_RemoveFile( fileName2 );
            sprintf( fileName1, "%s%d", this->WriteFileName, CCMS_FILE_MAIN_SERIAL );   /* ����1�ļ����� */
            sprintf( fileName2, "%s%d", this->WriteFileName, CCMS_FILE_BACKUP_SERIAL ); /* ����2�ļ����� */
            xj_RemoveFile( fileName1 );
            xj_RemoveFile( fileName2 );
            /* �������� */
            this->iFWriteCount = 0;
            /* �������� */
            this->iWriteCount = 0;
            /* �������� */
            this->iFReadCount = 0;
            /* �������� */
            this->iReadCount = 0;
        }

        /* �ó�ʼ����ɱ�ʶ */
        this->iMessInitFlag = XJ_TRUE;
        iStatus = XJ_SUCCESS;
    }
    return iStatus;
}

/*==========================================*/
