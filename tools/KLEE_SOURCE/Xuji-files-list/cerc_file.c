
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
/*      cerc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CER - Common Error Management                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common error        */
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
#include <stdlib.h>
#include <math.h>
#include "plat/inc/cer_extr.h"
#include "plat/inc/xj_file.h"
#include "plat/es_plat.h"

#define CERC_FILE_NAME          64     /* �ļ����Ƴ��� */
#define CERC_MAX_FILE_NUM       5      /* ����Լ��ļ����� */
#define CERC_TEXT_INI           0x0000 /* ����CRC��ĳ�ֵ */
#define CERC_TEXT_MASKS         0x0000 /* ����CRC������� */
#define CERC_FLASH_PRART_NUM    2      /* FLASH������Ŀ */
#define CERC_MAX_TEXT_NUM       12     /* ������������� */

/* �����ļ��Լ���Ϣ���� */
typedef struct CERC_FINF_STRUCT
{
    /* ��ʼ����־ */
    INT InitFlag;
    /* �����־ */
    INT ErrFlag;
    /* CRC�� */
    UNSIGNED CrcCode;
    /* ���� */
    CHAR Name[CERC_FILE_NAME];
    /* �����ַ��Ŀ */
    INT TextNumber;
    /* �����ַ */
    UNSIGNED TextAddr[ CERC_MAX_TEXT_NUM ];
    /* �����С */
    UNSIGNED TextSize[ CERC_MAX_TEXT_NUM ];
}CERC_FINF;

/* �����ļ��Լ����ݽṹ���� */
typedef struct CERC_FCHK_STRUCT
{
    /* ��ʼ����־ */
    INT InitFlag;
    /* ����ļ����� */
    INT MaxNumber;
    /* ʵ���ļ����� */
    INT Number;
    /* �ļ������� */
    INT Index;
    /* �ļ���Ϣָ�� */
    CERC_FINF * Memory;
}CERC_FCHK;

/* FLASH��������Ϣ���� */
typedef struct CERC_FPINF_STRUCT
{
    /* ��ʼ����־ */
    INT InitFlag;
    /* ���� */
    CHAR * Name;
    /* ������ */
    INT Retried;
    /* ʹ�ÿ��� */
    INT Used;
    /* ���п��� */
    INT Emptyx;
}CERC_FPINF;

/* FLASH�����Լ����ݽṹ���� */
typedef struct CERC_FPCK_STRUCT
{
    /* ��ʼ����־ */
    INT InitFlag;
    /* ���������� */
    INT MaxNumber;
    /* ʵ�ʷ������� */
    INT Number;
    /* ������Ϣָ�� */
    CERC_FPINF * Memory;
}CERC_FPCK;

/* ����ΪOS������� */
#ifdef WIN32
/* ȫ�ֱ������� */
/* Windows �궨�� */
#define CERC_FILE_DIR_NAME      "boot.cfg"      /* ����洢·�� */
#endif

#ifndef WIN32 /* ��windowsȱʡ����һЩ���Ա�ͨ������ */
/* ȫ�ֱ������� */
/* Nucleus�궨�� */
#define CERC_FILE_DIR_NAME      "/bin/boot.cfg" /* ����洢·�� */
#endif

/* Define external inner-component global data references.  */

XJ_PROTECT CERC_FlashProtect;
CERC_FPINF CERC_FlashBlockInf[ CERC_FLASH_PRART_NUM ];
CHAR * CERC_FlashPrartitionName[] =
{
    "/user",
    "/bin",
};

CERC_FPCK CERC_Flash =
{
    XJ_FALSE,                           /* InitFlag */
    CERC_FLASH_PRART_NUM,               /* MaxNumber */
    0,                                  /* Number */
    CERC_FlashBlockInf,                 /* Memory */
};

CERC_FINF CERC_FileInf[ CERC_MAX_FILE_NUM ];
CERC_FCHK CERC_File =
{
    XJ_FALSE,                           /* InitFlag */
    CERC_MAX_FILE_NUM,                  /* MaxNumber */
    0,                                  /* Number */
    0,                                  /* Index */
    CERC_FileInf,                       /* Memory */
};

/* Define internal function calls.  */
UNSIGNED CERC_Calc_Text_Crc( CERC_FINF * Inf );

/* �������CRC�� */
UNSIGNED CERC_Calc_Text_Crc( CERC_FINF * Inf )
{
    INT k;
    CERC_FINF * pInf;
    UNSIGNED crc_code;

    pInf = Inf;
    crc_code = CERC_TEXT_INI;
    for( k = 0; k < pInf->TextNumber; k ++ )
    {
        XJP_CRC_Calculate( ( void *)( pInf->TextAddr[ k ] ),
            pInf->TextSize[ k ], 1, 16, crc_code, & crc_code );
    }
    crc_code = ( crc_code ^ CERC_TEXT_MASKS );

    return( crc_code );
}

/* RAM�������ļ��Լ���Ϣ��ʼ�� */
INT CERC_FileInitialize( VOID )
{
#ifndef WIN32
    FILE * fp;
    INT k;
    INT len;
    INT status;
    CHAR buf[128],* path; /* absolute path */
    CERC_FINF * pInf;
    UNSIGNED crc_code;
    INT result;
    INT actNumber;

    CERC_File.Number = 0;
    CERC_File.Index = 0;
    status = - 1; /* ��ʼ״̬ */
    /* �򿪳��������ļ� */
    fp = fopen( CERC_FILE_DIR_NAME, "r" );
    if( fp )
    {
        /* ��ȡ�����ļ���Ϣ */
        while( 1 )
        {
            /* get path name */
            path = fgets( buf, sizeof( buf ), fp );
            if(! path )
                break;

            while( 1 )
            {
                /* remove "\n\r" at end of a line */
                len = strlen( path ) - 1;
                if( ( path[len] == '\n' ) || ( path[len] == '\r' ) )
                    path[len] = '\0';
                else
                    break;
            }

            if( (! strlen( path )) || ( path[0] == '#' ) )
                continue;

            if( path[0] == '*' )
            {
                path ++;
            }

            if( CERC_File.Number < CERC_File.MaxNumber )
            {
                pInf = CERC_File.Memory;
                pInf += CERC_File.Number;

                pInf -> InitFlag = XJ_FALSE;
                pInf -> CrcCode = CERC_TEXT_INI;
                strncpy( pInf->Name, path, CERC_FILE_NAME );
                pInf -> ErrFlag = XJ_FALSE;
                /* ��ȡ�����ַ */
                result = XJP_ELF_Text_Addr( pInf->Name, CERC_MAX_TEXT_NUM, &( pInf->TextNumber ),
                    pInf->TextAddr, pInf->TextSize );
                if( result != XJ_SUCCESS )
                    pInf->TextNumber = 0;
                /* �����ʼCRC�� */
                crc_code = CERC_Calc_Text_Crc( pInf );
                pInf->CrcCode = crc_code;
                pInf->InitFlag = XJ_TRUE;
                CERC_File.Number ++;
            }
        }

        fclose( fp );

        status = 0; /* �ɹ�״̬ */
    }

    CERC_File.InitFlag = XJ_TRUE;

    return( status );
#else
    return( 0 );
#endif
}

/* RAM�������ļ��Լ� */
VOID * CERC_FileCheck(  VOID )
{
#ifndef WIN32
    UNSIGNED crc_code;
    CERC_FINF * pInf;
    VOID * pName;

    pName = XJ_NULL;
    if(( CERC_File.Number == 0 ) || ( CERC_File.InitFlag == XJ_FALSE ))
    {
        return( XJ_NULL );
    }

    pInf = CERC_File.Memory;
    pInf += CERC_File.Index;
    /* �������CRC�� */
    crc_code = CERC_Calc_Text_Crc( pInf );
    if( pInf->InitFlag == XJ_FALSE )
    {
        pInf->CrcCode = crc_code;
        pInf->InitFlag = XJ_TRUE;
    }
    else
    {
        /* ���CRC�� */
        if( pInf->CrcCode != crc_code )
        {
            /* ����У����� */
            pName = pInf->Name;
            pInf -> ErrFlag = XJ_TRUE;
            pInf->CrcCode = crc_code;
        }
    }

    if( (++ CERC_File.Index ) >= CERC_File.Number )
    {
        CERC_File.Index = 0;
    }

    return( pName );
#else
    return( XJ_NULL );
#endif
}

/* FALSH�����ڴ���ʼ�� */
INT CERC_FlashCheckInitialize( VOID )
{
    INT k;
    INT Number;
    CERC_FPINF * pFlashBlockInf;

    Number = sizeof( CERC_FlashPrartitionName )/ sizeof( CHAR *);
    if( Number <= CERC_Flash.MaxNumber )
    {
        CERC_Flash.Number = Number;
    }
    else
    {
        CERC_Flash.Number = CERC_Flash.MaxNumber;
    }
    CERC_Flash.InitFlag = XJ_TRUE;

    CERC_FlashProtect = XJ_Create_Protect();
    pFlashBlockInf = CERC_Flash.Memory;
    for( k = 0; k < CERC_Flash.Number; k ++ )
    {
        pFlashBlockInf->InitFlag = XJ_FALSE;
        pFlashBlockInf->Name = CERC_FlashPrartitionName[k];
        pFlashBlockInf->Retried = 0;
        pFlashBlockInf->Used = 0;
        pFlashBlockInf->Emptyx = 0;
        pFlashBlockInf ++;
    }

    return( XJ_SUCCESS );
}

/* FALSH�����ڴ���������� */
INT CERC_FlashCheckMain( VOID )
{
    INT k;
    int retried;
    int used;
    int empty;
    int status;
    CERC_FPINF * pFlashBlockInf;

    pFlashBlockInf = CERC_Flash.Memory;
    for( k = 0; k < CERC_Flash.Number; k ++ )
    {
        status = xj_GetFlashPrartitionBlockStatus( pFlashBlockInf->Name,
            & retried, & used, & empty, 0, 0, 0 );
        if( status == XJ_SUCCESS )
        {
            XJ_Protect( CERC_FlashProtect );

            pFlashBlockInf->Retried = retried;
            pFlashBlockInf->Used = used;
            pFlashBlockInf->Emptyx = empty;
            pFlashBlockInf->InitFlag = XJ_TRUE;

            XJ_Unprotect();
        }
        pFlashBlockInf ++;
    }

    return( XJ_SUCCESS );
}

/* ��ȡFLASH�����ڴ��״̬ */
INT CERC_GetFlashPrartitionBlockStatus( int index, int * retired, int * used, int * empty )
{
    INT status;
    CERC_FPINF * pFlashBlockInf;

    status = XJ_UNAVAILABLE;
    if( index < CERC_Flash.Number )
    {
        pFlashBlockInf = CERC_Flash.Memory;
        pFlashBlockInf += index;
        if(( pFlashBlockInf->Used != 0 ) || ( pFlashBlockInf->Emptyx != 0 ))
        {
            XJ_Protect( CERC_FlashProtect );
            * retired = pFlashBlockInf->Retried;
            * used = pFlashBlockInf->Used;
            * empty = pFlashBlockInf->Emptyx;
            XJ_Unprotect();
            status = XJ_SUCCESS;
        }
    }

    return( status );
}
