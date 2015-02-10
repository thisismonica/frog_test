
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
XJ_MEMORY_POOL Memory_Pool;
UNSIGNED MallocErrorCounter = 0;
UNSIGNED FreeErrorCounter = 0;

/* Define internal function calls.  */

/* �ڴ��ʼ�� */
VOID MemoryInit( VOID * memory_pool )
{
    /* ��ʼ��ʹ�õ��ڴ��ָ�� */
    Memory_Pool = ( XJ_MEMORY_POOL )memory_pool;
}

#ifndef WIN32
/* ���붯̬�ڴ�ռ� */
VOID * malloc( size_t alloc_size )
{
    VOID * temp_ptr;

    if( XJ_Allocate_Memory( Memory_Pool, & temp_ptr, alloc_size, XJ_NO_SUSPEND ) == XJ_SUCCESS )
    {
        /* �����ڴ�ɹ� */
        return( temp_ptr );
    }
    else
    {
        /* �����ڴ�ʧ�� */
        MallocErrorCounter ++;
        return( XJ_NULL );
    }
}

/* �ͷŶ�̬�ڴ�ռ� */
VOID free( VOID * memory_ptr )
{
    if ( XJ_Deallocate_Memory( memory_ptr ) != XJ_SUCCESS )
    {
        /* �ͷ��ڴ���� */
        FreeErrorCounter ++;
    }
}

/* ����һ���ڴ棬�����ڴ����� */
void * calloc( size_t cnt, size_t size )
{
    unsigned long i;
    unsigned long msize;
    void * ptr;
    char * p;

    /* ʵ��Ҫ����Ĵ�С */
    msize = cnt * size;
    ptr = malloc( msize );
    p = ( char *)ptr;
    if ( p != XJ_NULL )
    {
        /* �ڴ����� */
        for ( i = 0; i < msize; i ++)
        {
            * p ++ = 0;
        }
    }

    return ptr;
}
#endif
