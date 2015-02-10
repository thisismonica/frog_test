
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
/*      cmemc.c                                         Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CMEMC - Common Memory Management                                 */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common memory       */
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
/*      ZhiDeYang       09-12-2008      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "plat/inc/xj_cmemc.h"

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
XJ_MEMORY_POOL Memory_Pool;
UNSIGNED MallocErrorCounter = 0;
UNSIGNED FreeErrorCounter = 0;

/* Define internal function calls.  */

/* 内存初始化 */
VOID MemoryInit( VOID * memory_pool )
{
    /* 初始化使用的内存池指针 */
    Memory_Pool = ( XJ_MEMORY_POOL )memory_pool;
}

#ifndef WIN32
/* 申请动态内存空间 */
VOID * malloc( size_t alloc_size )
{
    VOID * temp_ptr;

    if( XJ_Allocate_Memory( Memory_Pool, & temp_ptr, alloc_size, XJ_NO_SUSPEND ) == XJ_SUCCESS )
    {
        /* 申请内存成功 */
        return( temp_ptr );
    }
    else
    {
        /* 申请内存失败 */
        MallocErrorCounter ++;
        return( XJ_NULL );
    }
}

/* 释放动态内存空间 */
VOID free( VOID * memory_ptr )
{
    if ( XJ_Deallocate_Memory( memory_ptr ) != XJ_SUCCESS )
    {
        /* 释放内存出错 */
        FreeErrorCounter ++;
    }
}

/* 分配一块内存，并将内存清零 */
void * calloc( size_t cnt, size_t size )
{
    unsigned long i;
    unsigned long msize;
    void * ptr;
    char * p;

    /* 实际要分配的大小 */
    msize = cnt * size;
    ptr = malloc( msize );
    p = ( char *)ptr;
    if ( p != XJ_NULL )
    {
        /* 内存清零 */
        for ( i = 0; i < msize; i ++)
        {
            * p ++ = 0;
        }
    }

    return ptr;
}
#endif
