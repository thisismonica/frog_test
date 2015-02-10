
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

#define CERC_FILE_NAME          64     /* 文件名称长度 */
#define CERC_MAX_FILE_NUM       5      /* 最大自检文件个数 */
#define CERC_TEXT_INI           0x0000 /* 程序CRC码的初值 */
#define CERC_TEXT_MASKS         0x0000 /* 程序CRC码的掩码 */
#define CERC_FLASH_PRART_NUM    2      /* FLASH分区数目 */
#define CERC_MAX_TEXT_NUM       12     /* 最大程序节区个数 */

/* 程序文件自检信息定义 */
typedef struct CERC_FINF_STRUCT
{
    /* 初始化标志 */
    INT InitFlag;
    /* 错误标志 */
    INT ErrFlag;
    /* CRC码 */
    UNSIGNED CrcCode;
    /* 名称 */
    CHAR Name[CERC_FILE_NAME];
    /* 程序地址数目 */
    INT TextNumber;
    /* 程序地址 */
    UNSIGNED TextAddr[ CERC_MAX_TEXT_NUM ];
    /* 程序大小 */
    UNSIGNED TextSize[ CERC_MAX_TEXT_NUM ];
}CERC_FINF;

/* 程序文件自检数据结构定义 */
typedef struct CERC_FCHK_STRUCT
{
    /* 初始化标志 */
    INT InitFlag;
    /* 最大文件个数 */
    INT MaxNumber;
    /* 实际文件个数 */
    INT Number;
    /* 文件索引号 */
    INT Index;
    /* 文件信息指针 */
    CERC_FINF * Memory;
}CERC_FCHK;

/* FLASH分区块信息定义 */
typedef struct CERC_FPINF_STRUCT
{
    /* 初始化标志 */
    INT InitFlag;
    /* 名称 */
    CHAR * Name;
    /* 坏块数 */
    INT Retried;
    /* 使用块数 */
    INT Used;
    /* 空闲块数 */
    INT Emptyx;
}CERC_FPINF;

/* FLASH分区自检数据结构定义 */
typedef struct CERC_FPCK_STRUCT
{
    /* 初始化标志 */
    INT InitFlag;
    /* 最大分区个数 */
    INT MaxNumber;
    /* 实际分区个数 */
    INT Number;
    /* 分区信息指针 */
    CERC_FPINF * Memory;
}CERC_FPCK;

/* 以下为OS相关配置 */
#ifdef WIN32
/* 全局变量申明 */
/* Windows 宏定义 */
#define CERC_FILE_DIR_NAME      "boot.cfg"      /* 程序存储路径 */
#endif

#ifndef WIN32 /* 非windows缺省定义一些宏以便通过编译 */
/* 全局变量申明 */
/* Nucleus宏定义 */
#define CERC_FILE_DIR_NAME      "/bin/boot.cfg" /* 程序存储路径 */
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

/* 计算程序CRC码 */
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

/* RAM区程序文件自检信息初始化 */
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
    status = - 1; /* 初始状态 */
    /* 打开程序配置文件 */
    fp = fopen( CERC_FILE_DIR_NAME, "r" );
    if( fp )
    {
        /* 获取程序文件信息 */
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
                /* 获取程序地址 */
                result = XJP_ELF_Text_Addr( pInf->Name, CERC_MAX_TEXT_NUM, &( pInf->TextNumber ),
                    pInf->TextAddr, pInf->TextSize );
                if( result != XJ_SUCCESS )
                    pInf->TextNumber = 0;
                /* 计算初始CRC码 */
                crc_code = CERC_Calc_Text_Crc( pInf );
                pInf->CrcCode = crc_code;
                pInf->InitFlag = XJ_TRUE;
                CERC_File.Number ++;
            }
        }

        fclose( fp );

        status = 0; /* 成功状态 */
    }

    CERC_File.InitFlag = XJ_TRUE;

    return( status );
#else
    return( 0 );
#endif
}

/* RAM区程序文件自检 */
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
    /* 计算程序CRC码 */
    crc_code = CERC_Calc_Text_Crc( pInf );
    if( pInf->InitFlag == XJ_FALSE )
    {
        pInf->CrcCode = crc_code;
        pInf->InitFlag = XJ_TRUE;
    }
    else
    {
        /* 检查CRC码 */
        if( pInf->CrcCode != crc_code )
        {
            /* 程序校验出错 */
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

/* FALSH分区内存块初始化 */
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

/* FALSH分区内存块检测主函数 */
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

/* 读取FLASH分区内存块状态 */
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
