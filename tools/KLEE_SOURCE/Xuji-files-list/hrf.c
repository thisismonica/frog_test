
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
/*      hrf.c                                           Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      HRF - History Record File                                        */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the history record file */
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
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhiDeYang       11-05-2011      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hrf.h"

int Hrf_HanldeNumber = 0;
HRF32_HANDLE Hrf_HandleMem[ HRF_MAX_HANDLE ];

/* ��ȡ���ݱ����ֽ��� */
int Hrf_host_byte_order( void );

/* ������������ */
unsigned long Hrf_Create( int cpuNumber, int maxSectNumber, int maxSectSize,
    int version )
{
    unsigned long handle;
    unsigned int MemSize;
    unsigned char * memPtr;
    HRF32_HANDLE * handlePtr;

    handle = 0; /* ���������ֵ */
    if( Hrf_HanldeNumber >= HRF_MAX_HANDLE )
        return( handle ); /* ��������������������ֱ�ӷ���ʧ��״̬ */

    /* �����ڴ�ռ� */
    MemSize = sizeof( HRF32_SHDR ) * maxSectNumber;  /* ����ͷ���� */
    MemSize += maxSectSize;                          /* ������Ϣ */
    MemSize += sizeof( HRF32_RHDR );                 /* ����ͷ�� */
    memPtr = ( unsigned char *)malloc( MemSize );
    if( memPtr )
    {
        handlePtr = Hrf_HandleMem + Hrf_HanldeNumber;
        handlePtr->h_ready = 0;
        handlePtr->h_cpu = cpuNumber;
        handlePtr->h_max_shnum = ( HRF32_HALF )maxSectNumber;
        handlePtr->h_shnum = 0;
        handlePtr->h_max_shsize = maxSectSize;
        handlePtr->h_version = version;
        handlePtr->h_mem = memPtr;
        handlePtr->h_shmem =
            ( HRF32_SHDR *)( handlePtr->h_mem + sizeof( HRF32_RHDR ) + maxSectSize ); /* ����ͷ�� */
        handlePtr->h_soffset = sizeof( HRF32_RHDR );
        Hrf_HanldeNumber ++;
        handle = ( unsigned long )handlePtr;
    }
    return( handle );
}

/* ��¼���������Ϣ */
int Hrf_RecordSectorInf_Report( unsigned long handle, unsigned int sectSize,
    unsigned char * sectInf, int sectType, int cpuNumber )
{
    int status;
    unsigned char buff[HRF_SH_NAME];
    HRF32_HANDLE * handlePtr;

    /* ��ȡ������� */
    handlePtr = ( HRF32_HANDLE *)handle;
    if( handlePtr == 0 ) /* ��Ч���������� */
        return( - 1 ); /* ��Ч��������ֱ�ӷ���ʧ�� */
    sprintf( ( char *)buff, "%s%02d", HRF_REPORT, handlePtr->h_shnum );
    status = Hrf_RecordSectorInf( handle, sectSize, sectInf, sectType, buff, cpuNumber, 0 );

    return( status );
}

/* ��¼���ݱ������Ϣ */
int Hrf_RecordSectorInf_Table( unsigned long handle, unsigned int sectSize,
    unsigned char * sectInf, int tableId, unsigned char * tableName,
    int cpuNumber, unsigned int tab_size )
{
    int status;

    status = Hrf_RecordSectorInf( handle, sectSize, sectInf, tableId, tableName, cpuNumber, tab_size );

    return( status );
}

/* ��¼���ݱ������Ϣ */
int Hrf_RecordSectorInf( unsigned long handle, unsigned int sh_size,
    unsigned char * sh_data, int sh_type, unsigned char * sh_name,
    int sh_info, unsigned int sh_entsize )
{
    HRF32_HANDLE * handlePtr;
    HRF32_SHDR * secheader_ptr;

    /* ��ȡ������� */
    handlePtr = ( HRF32_HANDLE *)handle;
    if( handlePtr == 0 ) /* ��Ч���������� */
        return( - 1 ); /* ��Ч��������ֱ�ӷ���ʧ�� */
    /* ������� */
    if(( handlePtr->h_shnum >= handlePtr->h_max_shnum )
        || (( handlePtr->h_soffset + sh_size ) > ( handlePtr->h_max_shsize + sizeof( HRF32_RHDR ) )))
    {
        return( - 1 ); /* �������������ֱ�ӷ���ʧ�� */
    }
    /* ��¼������Ϣ */
    memcpy( ( handlePtr->h_mem + handlePtr->h_soffset ), sh_data, sh_size );
    /* ��¼����ͷ�� */
    secheader_ptr = handlePtr->h_shmem + handlePtr->h_shnum;
    sprintf( ( char *)secheader_ptr->sh_name, ".%s", sh_name );
    secheader_ptr->sh_type = sh_type;
    secheader_ptr->sh_offset = handlePtr->h_soffset;
    secheader_ptr->sh_size = sh_size;
    secheader_ptr->sh_info = sh_info;
    secheader_ptr->sh_entsize = sh_entsize;
    /* ��¼�ļ���Ϣ */
    handlePtr->h_soffset += sh_size;
    handlePtr->h_shnum ++;

    return( 0 ); /* ���سɹ�״̬ */
}

/* �������ͷ����Ϣ */
int Hrf_RecordSectorHead( unsigned long handle, unsigned char * identName )
{
    HRF32_HANDLE * handlePtr;
    HRF32_RHDR * rcdheader_ptr;

    /* ��ȡ������� */
    handlePtr = ( HRF32_HANDLE *)handle;
    if(( handlePtr == 0 ) /* ��Ч���������� */
        || ( handlePtr->h_mem == 0 )) /* ��Ч�ڴ�ռ� */
        return( - 1 ); /* ��Ч��������ֱ�ӷ���ʧ�� */
    /* �������ͷ����Ϣ */
    if( ( handlePtr->h_mem + handlePtr->h_soffset ) != ( unsigned char *)handlePtr->h_shmem )
    {
        memmove(( handlePtr->h_mem + handlePtr->h_soffset ), handlePtr->h_shmem,
            handlePtr->h_shnum * sizeof( HRF32_SHDR ));
    }
    /* �����¼ͷ����Ϣ */
    rcdheader_ptr = ( HRF32_RHDR *)handlePtr->h_mem;
    strncpy( ( char *)( rcdheader_ptr->r_ident ), ( char *)identName, HRF_IDENT );
    rcdheader_ptr->r_type = Hrf_host_byte_order();
    rcdheader_ptr->r_version = handlePtr->h_version;
    //    rcdheader_ptr->r_shoff = handlePtr->h_soffset + ( handlePtr->h_shnum * sizeof( HRF32_SHDR ));
    rcdheader_ptr->r_shoff = handlePtr->h_soffset;
    rcdheader_ptr->r_shnum = handlePtr->h_shnum;
    rcdheader_ptr->r_shentsize = sizeof( HRF32_SHDR );

    handlePtr->h_ready = 1; /* ����׼����� */

    return( 0 ); /* ���سɹ�״̬ */
}

/* �����¼��Ϣ */
int Hrf_ClearRecordInf( unsigned long handle )
{
    HRF32_HANDLE * handlePtr;

    /* ��ȡ������� */
    handlePtr = ( HRF32_HANDLE *)handle;
    if( handlePtr == 0 ) /* ��Ч���������� */
        return( - 1 ); /* ��Ч��������ֱ�ӷ���ʧ�� */

    /* �����¼��Ϣ */
    handlePtr->h_ready = 0;
    handlePtr->h_shnum = 0;
    handlePtr->h_soffset = sizeof( HRF32_RHDR );

    return( 0 ); /* ���سɹ�״̬ */
}

/* ɾ���������� */
int Hrf_Delete( unsigned long handle )
{
    HRF32_HANDLE * handlePtr;

    /* ��ȡ������� */
    handlePtr = ( HRF32_HANDLE *)handle;
    if( handlePtr == 0 ) /* ��Ч���������� */
        return( - 1 ); /* ��Ч��������ֱ�ӷ���ʧ�� */
    if( handlePtr->h_mem )
        free( handlePtr->h_mem ); /* �ͷ��ڴ�ռ� */
    handlePtr->h_mem = 0;
    /* �����¼��Ϣ */
    handlePtr->h_ready = 0;
    handlePtr->h_shnum = 0;
    handlePtr->h_soffset = sizeof( HRF32_RHDR );

    return( 0 ); /* ���سɹ�״̬ */
}

/* ��ȡ��¼��Ϣ */
int Hrf_GetRecordInf( unsigned long handle, unsigned char ** buffer,
    unsigned long * size )
{
    unsigned long actSize;
    HRF32_HANDLE * handlePtr;

    /* ��ȡ������� */
    handlePtr = ( HRF32_HANDLE *)handle;
    if( handlePtr == 0 ) /* ��Ч���������� */
        return( - 1 );    /* ��Ч��������ֱ�ӷ���ʧ�� */
    if( handlePtr->h_ready == 0 )
        return( - 1 );    /* ����δ׼���ã�ֱ�ӷ���ʧ�� */

    actSize = handlePtr->h_soffset + ( handlePtr->h_shnum * sizeof( HRF32_SHDR ));

    if( buffer )
        * buffer = handlePtr->h_mem;
    if( size )
        * size = actSize;

    return( 0 ); /* ���سɹ�״̬ */
}

/* ��ȡ������Ϣ�����ض�ȡ��Ϣ�ֽ��� */
int Hrf_GetSectInf( unsigned char * filebuf, int shIndex, int offset,
    int size, unsigned char * buffer )
{
    HRF32_RHDR * rcdheader_ptr;
    HRF32_SHDR * secheader_ptr;
    unsigned char * p;
    int actSize;

    rcdheader_ptr = ( HRF32_RHDR *)filebuf;
    if( shIndex >= ( int )rcdheader_ptr->r_shnum )
        return( 0 ); /* ��������Ч��ֱ�ӷ���ʧ�� */
    secheader_ptr = ( HRF32_SHDR *)( filebuf + rcdheader_ptr->r_shoff );
    secheader_ptr += shIndex;
    if( ( unsigned )offset >= secheader_ptr->sh_size )
        return( 0 ); /* ƫ����Ч��ֱ�ӷ���ʧ�� */
    if( ( unsigned )( offset + size ) >= secheader_ptr->sh_size )
        actSize = secheader_ptr->sh_size - offset;
    else
        actSize = size;
    p = filebuf + secheader_ptr->sh_offset + offset;
    memcpy( buffer, p, actSize );

    return( actSize );
}

/* ��ȡ����ͷ���� */
int Hrf_GetSectHeadCount( unsigned char * filebuf )
{
    HRF32_RHDR * rcdheader_ptr;

    rcdheader_ptr = ( HRF32_RHDR *)filebuf;

    return( rcdheader_ptr->r_shnum );
}

/* ��ȡ���ݱ����ֽ��� */
int Hrf_host_byte_order( void )
{
    int host_byteOorder;
    unsigned short test_short;
    unsigned char * ptr;

    ptr = ( unsigned char *)& test_short;

    test_short = 0xaa55; /* �����ַ� */

    if( ptr[0] == 0x55 && ptr[1] == 0xaa ) /* ������ֽ��ڵ͵�ַ ˵��ΪС��ģʽ */
    {
        host_byteOorder = HRF_DATA2_LSB;
    }
    else if( ptr[0] == 0xaa && ptr[1] == 0x55 ) /* ������ֽ��ڵ͵�ַ ˵��Ϊ���ģʽ */
    {
        host_byteOorder = HRF_DATA2_MSB;
    }
    else
        host_byteOorder = HRF_DATA_NONE;

    return( host_byteOorder );
}
