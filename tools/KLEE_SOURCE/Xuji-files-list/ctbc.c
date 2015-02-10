
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
/*      ctbc.c                                          Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CTB - Common Table Management                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains the core routines for the common table        */
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
/*      ZhiDeYang       01-26-2007      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "plat/inc/xj_crcc.h"
#include "plat/inc/ctb_extr.h"

/* �궨�� */
#define CTBC_CALCULATE_CRC(x,y,z,u,v,w) CRCC_CalculateCRC(x,y,z,u,v,w) //����CRC��
#define CTBC_GET_TABLE_CRCINI() TABLE_CRC_INITIALIZE //��ȡ������CRC��ĳ�ֵ
#define CTBC_GET_TABLE_CRCBITS() TABLE_CRC_BITS //��ȡ������CRC���λ��
#define CTBC_GET_TABLE_CRCMASKS() TABLE_CRC_MASKS //��ȡ������CRC�������
#define CTBC_DATA_BYTE_NUMBER_ADDRESS  (4/sizeof(UNSIGNED)) // �������ֽ���/ÿ��ַ

/* ָ����ַ�ռ������ݸ�������:�����ַ�ռ���ָ���������ݵĸ���,����1����1������ */
#define CTBC_DATA_SIZE_TO_NUMBER(x,y) (x=(x + y - 1)/y)
/* ���ݶ��� */
#define CTBC_DATA_TIDINESS(x,y) (x=((x + y - 1)/y)*y)

/* ����ΪOS������� */
#ifdef WIN32
#include <stdio.h>
#include <malloc.h>
/* ȫ�ֱ������� */
struct TBMemMService TBMMS = {malloc, free};
/* Windows �궨�� */
#endif

#ifndef WIN32 /* ��windowsȱʡ����һЩ���Ա�ͨ������ */
/* ȫ�ֱ������� */
struct TBMemMService TBMMS = {malloc, free};
/* Nucleus�궨�� */
#endif

/* Define external inner-component global data references.  */

/* Define internal function calls.  */
VOID CTBC_Place_On_List( TB_NODE ** head, TB_NODE * new_node );
TB_TYPE * CTBC_GetDataType( INT Index );
INT CTBC_CalTableDataCRC( VOID );
INT CTBC_CalTableConfigCRC( VOID );
INT CTBC_CalTableFieldInfoCRC( VOID );
VOID * CTBC_FieldCpy( VOID * dst, const VOID * src, unsigned number, register INT typeByte );

TB_NODE * TBD_Created_Tables_List = XJ_NULL; /* ���������ı�ͷָ�� */

/* �����������Ͷ��� */
#define TOTAL_TYPE_NUMBER   6 /* �ܵ��������͸��� */
TB_TYPE CType[TOTAL_TYPE_NUMBER] =
{
    TYPE_CHAR,          "char",             sizeof( char ),           1,  /* 1 BYTE */
    TYPE_UNSIGNED_CHAR, "unsigned char",    sizeof( unsigned char ),  1,  /* 1 BYTE */
    TYPE_INT,           "int",              sizeof( int ),            4,  /* 4 BYTE */
    TYPE_SIGNED,        "long",             sizeof( long ),           4,  /* 4 BYTE */
    TYPE_UNSIGNED,      "unsigned long",    sizeof( unsigned long ),  4,  /* 4 BYTE */
    TYPE_FLOAT,         "float",            sizeof( float ),          4,  /* 4 BYTE */
};

CTABLE CTable =
{
    XJ_FALSE,                           /* InitFlag */
    MAX_TABLE_NUMBER,                   /* MaxTabNumber */
    0,                                  /* TabNumber */
    XJ_NULL,                            /* TabMemory */
    MAX_FIELD_NUMBER,                   /* MaxFieldNumber */
    0,                                  /* FieldNumber */
    XJ_NULL,                            /* FieldMemory */
    sizeof( UNSIGNED ),                   /* TotalSGSize */
    0,                                  /* ConfigId */
    0,                                  /* NVDataId */
    0,                                  /* DataCrcCode */
    0,                                  /* ConfigCrcCode */
    0,                                  /* FieldCrcCode */
    sizeof( UNSIGNED ),                   /* TotalCfgSize */
    0,                                  /* Status */
    0,                                  /* NoSGMemory */
    0,                                  /* NoCfgMemory */
    0,                                  /* NoRamMemory */

    CTBC_DefTable,                      /* DefTable */
    CTBC_DefField,                      /* DefField */
    CTBC_CreateAllField,                /* CreateAllField */
    CTBC_GetOneFieldValue,              /* GetOneFieldValue */
    CTBC_GetBatchFieldValue,            /* GetBatchFieldValue */
    CTBC_GetAllFieldValue,              /* GetAllFieldValue */
    CTBC_DefSGRange,                    /* DefSGRange */
    CTBC_SetConfigValues,               /* SetConfigValues */
    CTBC_SelectEditSG,                  /* SelectEditSG */
    CTBC_SetEditSGValues,               /* SetEditSGValues */
    CTBC_ConfirmEditSGValue,            /* ConfirmEditSGValue */
    CTBC_TableInitialize,               /* TableInitialize */
    CTBC_DefProtectHandle,              /* DefProtectHandle */
    CTBC_FreezeTableData,               /* FreezeTableData */
    CTBC_UnFreezeTableData,             /* UnFreezeTableData */
    CTBC_GetFreezeStatus,               /* GetFreezeStatus */
    CTBC_CalTableDataCRC,               /* CalTabDataCRC */
    CTBC_CalTableConfigCRC,             /* CalTabConfigCRC */
    CTBC_CalTableFieldInfoCRC,          /* CalTabFieldInfoCRC */
    CTBC_SaveConfigValue,               /* SaveConfigValue */
    CTBC_LeadConfigValue,               /* LeadConfigValue */
    CTBC_SaveTabConfigValue,            /* SaveTabConfigValue */
    CTBC_SaveSGValue,                   /* SaveSGValue */
    CTBC_LeadSGValue,                   /* LeadSGValue */
    CTBC_SetDataInitialize,             /* SetDataInitialize */
    XJ_NULL,                            /* WriteNVData */
    XJ_NULL,                            /* ReadNVData */
    XJ_NULL,                            /* RepairNVData */
    XJ_NULL,                            /* WriteConfigData */
    XJ_NULL,                            /* ReadConfigData */
    XJ_NULL,                            /* RepairConfigData */
};

/* �༭״̬�ڼ��߽����ʱ,�Ƿ���Ҫ��� */

/* ��ʼ�����ݱ������� */
INT CTBC_TableInitialize( INT MaxTabNumber, INT MaxFieldNumber )
{
    CTable.MaxTabNumber = MaxTabNumber; /* ������ݱ���� */
    CTable.MaxFieldNumber = MaxFieldNumber; /* ����ֶθ��� */

    return( XJ_SUCCESS );
}

/* ������� */
VOID CTBC_Place_On_List( TB_NODE ** head, TB_NODE * new_node )
{
    /* �ж������Ƿ�Ϊ��  */
    if (* head )
    {
        /* ����ǿ�,��ӽڵ㵽�����β��  */
        new_node -> Previous = (* head ) -> Previous;
        ( new_node -> Previous ) -> Next = new_node;
        new_node -> Next = (* head );
        ( new_node -> Next ) -> Previous = new_node;
    }
    else
    {
        /* ����Ϊ��,���ñ�ͷ���½ڵ�  */
        (* head ) =  new_node;
        new_node -> Previous =  new_node;
        new_node -> Next = new_node;
    }
}

/* �����ֶ����� */
VOID * CTBC_FieldCpy( VOID * dst, const VOID * src, unsigned number, register INT typeByte )
{
    if( typeByte == 1 ) /* ���ֽ��������� */
    {
        /* ���ֽ����� */
        register UNSIGNED_CHAR * pDst;
        register UNSIGNED_CHAR * pSrc;
        register UNSIGNED_CHAR * pEnd;

        pDst = ( UNSIGNED_CHAR *) dst;
        pSrc = ( UNSIGNED_CHAR *) src;
        pEnd = pSrc + number;
        while ( pSrc < pEnd )
        {
            * pDst ++ = (* pSrc ) & 0xff ; /* ���ֽڿ��� */
            pSrc ++;
        }
    }
    else
    {
        /* 4�ֽ����� */
        register UNSIGNED_CHAR * pDst;
        register UNSIGNED * pSrc;
        register UNSIGNED * pEnd;
        register INT k;

        pDst = ( UNSIGNED_CHAR *) dst;
        pSrc = ( UNSIGNED *) src;
        pEnd = pSrc + number;
        while ( pSrc < pEnd )
        {
            for( k = 0; k < typeByte; k ++)
            {
                * pDst ++ = ( UNSIGNED_CHAR )(((* pSrc ) >> k * 8 ) & 0xff ); /* ���ֽڿ��� */
            }
            pSrc ++;
        }
    }

    return ( dst );
}

/* �������������Ż�ȡ������Ϣ */
TB_TYPE * CTBC_GetDataType( INT Index )
{
    TB_TYPE * pType;
    TB_TYPE * pSearch_Type;
    INT i;

    pType = CType;
    pSearch_Type = XJ_NULL; /* ��ʼ�����ҽ��Ϊ�� */
    for( i = 0; i < TOTAL_TYPE_NUMBER; i ++)
    {
        if( pType->Index == Index )
        {
            /* �ҵ����ͱ�,�����ҵ������ͱ���ַ */
            pSearch_Type = pType;
            break;
        }
        pType ++;
    }

    return( pSearch_Type );
}

/* ���������Ų��Ҷ���� */
TB_TCB * CTBC_FindTable( INT TableIndex )
{
    TB_NODE * pNode;
    TB_TCB * pTable;
    TB_TCB * pSearch_Table;

    pSearch_Table = XJ_NULL; /* ��ʼ�����ҽ��Ϊ�� */
    if( TBD_Created_Tables_List != XJ_NULL )
    {
        /* �����ǿ�,����ָ�������ŵĶ���� */
        pNode = TBD_Created_Tables_List;
        do
        {
            pTable = ( TB_TCB *)pNode;
            if( pTable->Index == TableIndex )
            {
                /* �ҵ������,�����ҵ��Ķ������ַ */
                pSearch_Table = pTable;
                break;
            }
            pNode = pNode -> Next;
        }while( pNode != TBD_Created_Tables_List );
    }

    return( pSearch_Table );
}

/* �����ֶ�������,��ָ�����в��Ҷ����ֶ� */
TB_FCB * CTBC_FindField( TB_TCB * pTable, INT FieldIndex )
{
    TB_FCB * pField;
    TB_FCB * pSearch_Field;
    TB_NODE * pNode,* pNodeHead;

    pSearch_Field = XJ_NULL; /* ��ʼ�����ҽ��Ϊ�� */
    if( pTable->FieldLink != XJ_NULL )
    {
        /* �ֶηǿ�,����ָ�������ŵ��ֶ� */
        pNode = &(( pTable->FieldLink )->Link );
        pNodeHead = &(( pTable->FieldLink )->Link );
        do
        {
            pField = ( TB_FCB *)pNode;
            if( pField->Index == FieldIndex )
            {
                /* �ҵ��ֶ�,�����ֶ���ַ */
                pSearch_Field = pField;
                break;
            }
            pNode = pNode->Next;
        }while( pNode != pNodeHead );
    }

    return( pSearch_Field );
}

/* ����ṹ������ */
INT CTBC_DefTable( CHAR * Name, INT Index, INT MaxNumber, INT Size, INT Type,
    INT EditFieldSize )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;
    UNSIGNED MemSize;
    INT FieldSize;

    /* �����Դ�ռ� */
    if(( CTable.TabNumber < CTable.MaxTabNumber )
        && ( CTable.FieldNumber < CTable.MaxFieldNumber ))
    {
        /* ���붨����ڴ�ռ� */
        if( CTable.TabMemory == XJ_NULL )
        {
            CTable.TabNumber = 0;
            MemSize = ( CTable.MaxTabNumber ) * sizeof( TB_TCB );
            if( MemSize > 0 )
            {
                CTable.TabMemory = ( TB_TCB *)TBMMS.MemAlloc( MemSize );
                if( CTable.TabMemory == XJ_NULL )
                {
                    /* �ڴ治��,ֱ�ӷ���ʧ�� */
                    CTable.Status |= TABLE_ERR_RAM_MEM;
                    CTable.NoRamMemory += MemSize;
                    status = XJ_NO_MEMORY;
                    return( status );
                }
            }
        }
        /* ���붨���ֶ��ڴ�ռ� */
        if( CTable.FieldMemory == XJ_NULL )
        {
            CTable.FieldNumber = 0;
            MemSize = ( CTable.MaxFieldNumber ) * sizeof( TB_FCB );
            if( MemSize > 0 )
            {
                CTable.FieldMemory = ( TB_FCB *)TBMMS.MemAlloc( MemSize );
                if( CTable.FieldMemory == XJ_NULL )
                {
                    /* �ڴ治��,�ͷ�ǰ�������ڴ�,ֱ�ӷ���ʧ�� */
                    CTable.Status |= TABLE_ERR_RAM_MEM;
                    CTable.NoRamMemory += MemSize;
                    TBMMS.MemFree( CTable.TabMemory );
                    status = XJ_NO_MEMORY;
                    return( status );
                }
            }
        }

        /* ���������ڴ�ռ� */
        pTable = CTable.TabMemory + CTable.TabNumber;
        MemSize = ( UNSIGNED )( MaxNumber * Size );
        if( MemSize > 0 )
        {
            pTable->DataMemory = ( void *)TBMMS.MemAlloc( MemSize );
            if( pTable->DataMemory == XJ_NULL )
            {
                /* �ڴ治��,ֱ�ӷ���ʧ�� */
                CTable.Status |= TABLE_ERR_RAM_MEM;
                CTable.NoRamMemory += MemSize;
                status = XJ_NO_MEMORY;
                return( status );
            }
        }

        /* ����༭�ڴ�ռ� */
        pTable = CTable.TabMemory + CTable.TabNumber;
        if( EditFieldSize == 0 )
        {
            FieldSize = sizeof( UNSIGNED );
        }
        else
        {
            FieldSize = EditFieldSize;
        }
        MemSize = MaxNumber * FieldSize;
        if( MemSize > 0 )
        {
            pTable->EditMemory = ( void *)TBMMS.MemAlloc( MemSize );
            if( pTable->EditMemory == XJ_NULL )
            {
                /* �ڴ治��,�ͷ�ǰ�������ڴ�,ֱ�ӷ���ʧ�� */
                CTable.Status |= TABLE_ERR_RAM_MEM;
                CTable.NoRamMemory += MemSize;
                TBMMS.MemFree( pTable->DataMemory );
                status = XJ_NO_MEMORY;
                return( status );
            }
        }

        /* ������ݱ����� */
        pTable = CTable.TabMemory + CTable.TabNumber;
        CTBC_Place_On_List(& TBD_Created_Tables_List,&( pTable->Link ));

        /* ��ʼ��������Ϣ */
        pTable -> Index = Index;
        pTable -> Type = Type;
        pTable -> MaxDataNumber = MaxNumber;
        pTable -> DataNumber = 0;
        pTable -> EditFlag = 0;
        pTable -> EditIndex = 0;
        pTable -> EditFieldSize = FieldSize;
        pTable -> Size = Size;
        pTable -> CrcCode = 0;
        pTable -> ProtectHandle = XJ_NULL;
        pTable -> FreezeData = 0;
        pTable -> FreezeCounter = 0;
        pTable -> Status = 0;
        pTable -> SetOffset = CTable.TotalSGSize;
        pTable -> FieldNumber = 0;
        pTable -> SetFieldNumber = 0;
        pTable -> FieldLink = XJ_NULL;
        pTable -> TypeField = XJ_NULL;
        pTable -> CoefField = XJ_NULL;
        pTable -> MinField = XJ_NULL;
        pTable -> MaxField = XJ_NULL;
        pTable -> BoundaryCheckFlag = CHECK_DISABLE;
        pTable -> CfgOffset = CTable.TotalCfgSize; /* ���������ݵ���ʼλ��ƫ�� */
        pTable -> CfgSize = 0; /* �������ݵ������ֶγߴ� */
        pTable -> foreLeadTreatment = XJ_NULL;
        pTable -> afterLeadTreatment = XJ_NULL;
        pTable -> SetCounter = 0;
        pTable -> NVData_CRC_IniFlag = XJ_FALSE;
        pTable -> NVData_CRC_Counter = 0;
        pTable -> NVData_CRCCode = 0;
        strncpy( pTable->Name, Name, TABLE_NAME );
        ( CTable.TabNumber )++;

        status = XJ_SUCCESS;
    }

    return( status );
}

/* �����ֶ����� */
INT CTBC_DefField( INT TableIndex, CHAR * Name, INT Index, INT Attribute,
    INT DataType, INT Number, INT Offset )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;
    TB_FCB * pField;
    TB_TYPE * pType;
    INT DataOffset;

    /* ��ȡ����������Ϣ */
    pType = CTBC_GetDataType( DataType );
    if( pType == XJ_NULL )
    {
        /* �޷�ʶ�����������,����ʧ�� */
        status = XJ_UNAVAILABLE;
        return( status );
    }

    /* �������ͱ� */
    pTable = CTBC_FindTable( TableIndex );
    if(( pTable != XJ_NULL ) && ( CTable.FieldNumber < CTable.MaxFieldNumber )
        && ((( pType->Size * Number )+ Offset ) <= pTable->Size ))
    {
        if( Attribute & FIELD_SETTING_ENABLE )
        {
            DataOffset = 0;
            if( pTable->SetOffset == CTable.TotalSGSize )
            {
                /* ���ݱ�ĵ�һ������ֵ�ֶ�:���ֶ��� */
                CTBC_DATA_TIDINESS( CTable.TotalSGSize,( INT )( sizeof( UNSIGNED ) ));
                pTable->SetOffset = CTable.TotalSGSize;
                /* ���ݱ�ĵ�һ���洢�ֶ�֮ǰ���CRC��, */
                DataOffset += sizeof( UNSIGNED ); /* ��1������ֵΪCRC�� */
            }
            /* ������ֵ���ݳߴ�ͳ�� */
            DataOffset += ( pTable->MaxDataNumber * pType->Size * Number );
            CTable.TotalSGSize += DataOffset;
            /* ����ֵ�ֶ���ͳ�� */
            ( pTable -> SetFieldNumber )++;
        }

        if( Attribute & FIELD_CONFIG_ENABLE )
        {
            /* �����ֶ� */
            DataOffset = 0;
            if( pTable->CfgOffset == CTable.TotalCfgSize )
            {
                /* ���ݱ�ĵ�һ�������ֶ�:���ֶ��� */
                CTBC_DATA_TIDINESS( CTable.TotalCfgSize,( INT )( sizeof( UNSIGNED ) ));
                pTable->CfgOffset = CTable.TotalCfgSize;
                /* ���ݱ�ĵ�һ�������ֶ�ǰ��ű����CRC�� */
                DataOffset += sizeof( UNSIGNED ); /* ��1������ΪCRC�� */
            }
            DataOffset += ( pTable->MaxDataNumber * pType->Size * Number );
            CTable.TotalCfgSize += DataOffset;
            /* ���������ݳߴ�ͳ�� */
            pTable -> CfgSize += ( pType->Size * Number );
        }

        /* ������ݱ����� */
        pField = CTable.FieldMemory + CTable.FieldNumber;
        CTBC_Place_On_List(( TB_NODE **)(&( pTable->FieldLink )),&( pField->Link ));

        /* ��ʼ���ֶ���Ϣ */
        pField -> Index = Index;
        pField -> Attribute = Attribute;
        pField -> Type = pType -> Index;
        pField -> TypeByte = pType -> Number;
        pField -> TypeSize = pType -> Size;
        pField -> Width = ( pType -> Number / pType -> Size );
        pField -> Size = ( pField->TypeSize )* Number;
        pField -> MemberNumber = Number;
        pField -> Offset = Offset;
        strncpy( pField->Name, Name, FIELD_NAME );
        pField -> preTreatment = NU_NULL;
        pField -> foreTreatment = XJ_NULL;
        pField -> afterTreatment = XJ_NULL;
        pField -> endTreatment = XJ_NULL;
        ( CTable.FieldNumber )++;
        ( pTable -> FieldNumber )++;
        status = XJ_SUCCESS;
    }

    return( status );
}

/* ����ȫ���ֶ� */
VOID * CTBC_CreateAllField( INT TableIndex, VOID * Buffer )
{
    TB_TCB * pTable;
    UNSIGNED_CHAR * pDst;
    VOID * pResult = XJ_NULL;

    /* �������ͱ� */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        if( pTable->DataNumber < pTable->MaxDataNumber )
        {
            /* �����µ����� */
            /* �������� */
            pDst = ( UNSIGNED_CHAR *)pTable->DataMemory;
            pDst += ( pTable->DataNumber * pTable->Size );
            pResult = ( VOID *)pDst;
            memcpy( pDst, Buffer, ( UNSIGNED )pTable->Size );
            ( pTable->DataNumber )++;
        }
    }

    return( pResult );
}

/* ���ָ�������Ƿ�����Ҫ�� */
INT CTBC_FieldCondCheck( VOID * Memory, INT CondNumber, INT * CondValue,
    TB_FCB ** CondField, INT * CondType )
{
    INT status = XJ_SUCCESS;
    UNSIGNED_CHAR * pSrc;
    INT FieldValue;
    INT * pCondValue;
    INT * pCondType;
    TB_FCB ** pCondField,* pField;
    INT i;

    pCondValue = CondValue; /* ����ֵ */
    pCondField = CondField; /* �����ֶ� */
    pCondType = CondType; /* �������� */
    for( i = 0; i < CondNumber; i ++)
    {
        pField = * pCondField;
        pSrc = ( UNSIGNED_CHAR *)Memory;
        pSrc += ( pField -> Offset );
        FieldValue = 0;
        memcpy(& FieldValue, pSrc, ( UNSIGNED )pField->TypeSize ); /* ��ȡ�ֶ�ֵ */
        if((* pCondType ) == COND_TYPE_EQU )
        {
            /* ��Ȳ��� */
            if( FieldValue != (* pCondValue ))
            {
                /* ����������,����ʧ�� */
                status = XJ_UNAVAILABLE;
                break;
            }
        }
        else if((* pCondType ) == COND_TYPE_OR )
        {
            /* ����� */
            if(( FieldValue & (* pCondValue )) == 0 )
            {
                /* ����������,����ʧ�� */
                status = XJ_UNAVAILABLE;
                break;
            }
        }
        else
        {
            /* ����� */
            if( ( FieldValue & (* pCondValue )) != (* pCondValue ))
            {
                /* ����������,����ʧ�� */
                status = XJ_UNAVAILABLE;
                break;
            }
        }
        pCondField ++;
        pCondValue ++;
        pCondType ++;
    }

    return( status );
}

/* ��ȡ���е�ָ���ֶ�֮���ͬ��������Ԫ�صĴ�С */
TB_FCB * CTBC_GetFieldOffset( TB_TCB * pTable, INT FieldIndex, INT Attribute,
    UNSIGNED * Actual_Size )
{
    UNSIGNED ActualSize;
    TB_FCB * pField;
    TB_FCB * pSearch_Field;
    TB_NODE * pNode,* pNodeHead;

    ActualSize = 0; /* ��ʼ����ʼ���ݴ�С */
    pSearch_Field = XJ_NULL; /* ��ʼ�����ҽ��Ϊ�� */
    if( pTable->FieldLink != XJ_NULL )
    {
        /* �ֶηǿ�,����ָ�������ŵ��ֶ� */
        pNode = &(( pTable->FieldLink )->Link );
        pNodeHead = &(( pTable->FieldLink )->Link );
        do
        {
            pField = ( TB_FCB *)pNode;
            if( pField->Index == FieldIndex )
            {
                /* �ҵ��ֶ�,�������Լ�� */
                if( pField->Attribute & Attribute )
                {
                    /* ָ������,�����ֶ���ַ */
                    pSearch_Field = pField;
                }
                break;
            }

            /* �������ͬ���Ե����ݴ�С */
            if( pField->Attribute & Attribute )
            {
                ActualSize += ( pTable->DataNumber * pField -> TypeSize * pField -> MemberNumber );
            }

            pNode = pNode->Next;
        }while( pNode != pNodeHead );
    }

    /* ���ر��е�ָ���ֶ�֮���ͬ��������Ԫ�صĴ�С */
    * Actual_Size = ActualSize;

    return( pSearch_Field );
}

/* �������ֶ�ֵ */
INT CTBC_GetOneFieldValue( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * Buffer, INT * ActualNumber )
{
    INT status;
    TB_TCB * pTable;
    TB_FCB * pField;
    TB_CFB * pHandle;
    INT i, TotalNumber;
    UNSIGNED_CHAR * pDst;
    UNSIGNED_CHAR * p,* pSrc;
    TB_FCB * CondField[MAX_CONDITION];

    TotalNumber = 0;
    status = XJ_SUCCESS;
    /* �������ͱ� */
    pTable = CTBC_FindTable( TableIndex );
    if(( pTable != XJ_NULL ) && ( Number != 0 ))
    {
        /* ���������ֶ� */
        pHandle = Handle;
        for( i = 0; i < pHandle->CondNumber; i ++)
        {
            CondField[i] = CTBC_FindField( pTable,( pHandle->CondIndex[i] ));
            if( CondField[i] == XJ_NULL )
            {
                if( ActualNumber != XJ_NULL )
                {
                    /* ����ʵ�ʶ�ȡ�����ݸ��� */
                    * ActualNumber = TotalNumber;
                }
                /* ��Ч�����ֶ�,����ʧ�� */
                status = XJ_UNAVAILABLE;
                return( status );
            }
        }

        /* ���Ҳ����ֶ� */
        pField = CTBC_FindField( pTable,( pHandle->HandleIndex ));
        if( pField != XJ_NULL )
        {
            pSrc = ( UNSIGNED_CHAR *)pTable->DataMemory;
            pDst = ( UNSIGNED_CHAR *)Buffer;
            for( i = 0; i < pTable->DataNumber; i ++)
            {
                /* ������� */
                if( CTBC_FieldCondCheck( pSrc, pHandle->CondNumber,
                    pHandle->CondValue, CondField, pHandle->CondType ) == XJ_SUCCESS )
                {
                    /* ��������,�ܸ�����һ */
                    if(( TotalNumber ++) >= Offset )
                    {
                        p = pSrc + ( pField->Offset );
                        /* ��ȡ�ֶ�ֵ */
                        CTBC_FieldCpy( pDst, p, ( UNSIGNED )pField->MemberNumber, pField->TypeByte );
                        pDst += ( pField->Size * pField->Width );

                        if( TotalNumber >= ( Offset + Number ))
                        {
                            /* ��ȡ������� */
                            status = XJ_SUCCESS;
                            break;
                        }
                    }
                }

                /* ָ����һ������ */
                pSrc += ( pTable->Size );
            }
        }
    }

    if( ActualNumber != XJ_NULL )
    {
        /* ����ʵ�ʶ�ȡ�����ݸ��� */
        * ActualNumber = TotalNumber;
    }

    /* ���ض�ȡ���ݵ�״̬ */
    return( status );
}

/* ������ֶ�ֵ */
INT CTBC_GetBatchFieldValue( INT TableIndex, INT Offset, INT Number,
    TB_CHB * Handle, VOID * Buffer, INT * ActualNumber )
{
    INT status;
    TB_TCB * pTable;
    TB_FCB * pField;
    TB_CHB * pHandle;
    INT i, k, TotalNumber;
    UNSIGNED_CHAR * pDst;
    UNSIGNED_CHAR * p,* pSrc;
    TB_FCB * CondField[MAX_CONDITION];
    TB_FCB * HandField[MAX_HANDLE];

    TotalNumber = 0;
    status = XJ_SUCCESS;
    /* �������ͱ� */
    pTable = CTBC_FindTable( TableIndex );
    if(( pTable != XJ_NULL ) && ( Number != 0 ))
    {
        /* ���������ֶ� */
        pHandle = Handle;
        for( i = 0; i < pHandle->CondNumber; i ++)
        {
            CondField[i] = CTBC_FindField( pTable,( pHandle->CondIndex[i] ));
            if( CondField[i] == XJ_NULL )
            {
                if( ActualNumber != XJ_NULL )
                {
                    /* ����ʵ�ʶ�ȡ�����ݸ��� */
                    * ActualNumber = TotalNumber;
                }
                /* ��Ч�����ֶ�,����ʧ�� */
                status = XJ_UNAVAILABLE;
                return( status );
            }
        }
        /* ���Ҳ����ֶ� */
        for( i = 0; i < pHandle->HandleNumber; i ++)
        {
            HandField[i] = CTBC_FindField( pTable,( pHandle->HandleIndex[i] ));
            if( HandField[i] == XJ_NULL )
            {
                if( ActualNumber != XJ_NULL )
                {
                    /* ����ʵ�ʶ�ȡ�����ݸ��� */
                    * ActualNumber = TotalNumber;
                }
                /* ��Ч�����ֶ�,����ʧ�� */
                status = XJ_UNAVAILABLE;
                return( status );
            }
        }
        if( pHandle->HandleNumber != 0 )
        {
            pSrc = ( UNSIGNED_CHAR *)pTable->DataMemory;
            pDst = ( UNSIGNED_CHAR *)Buffer;
            for( i = 0; i < pTable->DataNumber; i ++)
            {
                /* ������� */
                if( CTBC_FieldCondCheck( pSrc, pHandle->CondNumber,
                    pHandle->CondValue, CondField, pHandle->CondType ) == XJ_SUCCESS )
                {
                    /* ��������,�ܸ�����һ */
                    if(( TotalNumber ++) >= Offset )
                    {
                        for( k = 0; k < pHandle->HandleNumber; k ++)
                        {
                            pField = HandField[k];
                            p = pSrc + ( pField->Offset );
                            /* ��ȡ�ֶ�ֵ */
                            CTBC_FieldCpy( pDst, p, ( UNSIGNED )pField->MemberNumber, pField->TypeByte );
                            pDst += ( pField->Size * pField->Width );
                            pField ++;
                        }
                        if( TotalNumber >= ( Offset + Number ))
                        {
                            /* ��ȡ������� */
                            status = XJ_SUCCESS;
                            break;
                        }
                    }
                }

                /* ָ����һ������ */
                pSrc += ( pTable->Size );
            }
        }
    }

    if( ActualNumber != XJ_NULL )
    {
        /* ����ʵ�ʶ�ȡ�����ݸ��� */
        * ActualNumber = TotalNumber;
    }

    /* ���ض�ȡ���ݵ�״̬ */
    return( status );
}

/* ��ȫ���ֶ�ֵ */
INT CTBC_GetAllFieldValue( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * Buffer, INT * ActualNumber )
{
    INT status;
    TB_FCB * pField;
    TB_TCB * pTable;
    TB_CFB * pHandle;
    INT i, TotalNumber;
    UNSIGNED_CHAR * pDst;
    UNSIGNED_CHAR * p,* pSrc;
    TB_FCB * CondField[MAX_CONDITION];
    TB_NODE * pFieldNode,* pFieldNodeHead;

    TotalNumber = 0;
    status = XJ_SUCCESS;
    /* �������ͱ� */
    pTable = CTBC_FindTable( TableIndex );
    if(( pTable != XJ_NULL ) && ( Number != 0 ))
    {
        /* ���������ֶ� */
        pHandle = Handle;
        for( i = 0; i < pHandle->CondNumber; i ++)
        {
            CondField[i] = CTBC_FindField( pTable,( pHandle->CondIndex[i] ));
            if( CondField[i] == XJ_NULL )
            {
                if( ActualNumber != XJ_NULL )
                {
                    /* ����ʵ�ʶ�ȡ�����ݸ��� */
                    * ActualNumber = TotalNumber;
                }
                /* ��Ч�����ֶ�,����ʧ�� */
                status = XJ_UNAVAILABLE;
                return( status );
            }
        }

        pSrc = ( UNSIGNED_CHAR *)pTable->DataMemory;
        pDst = ( UNSIGNED_CHAR *)Buffer;
        for( i = 0; i < pTable->DataNumber; i ++)
        {
            /* ������� */
            if( CTBC_FieldCondCheck( pSrc, pHandle->CondNumber,
                pHandle->CondValue, CondField, pHandle->CondType ) == XJ_SUCCESS )
            {
                /* ��������,�ܸ�����һ */
                if(( TotalNumber ++) >= Offset )
                {
                    pFieldNode = &(( pTable->FieldLink )->Link );
                    pFieldNodeHead = &(( pTable->FieldLink )->Link );
                    do
                    {
                        pField = ( TB_FCB *)pFieldNode;
                        p = pSrc + ( pField->Offset );
                        /* ��ȡ�ֶ�ֵ */
                        CTBC_FieldCpy( pDst, p, ( UNSIGNED )pField->MemberNumber, pField->TypeByte );
                        pDst += ( pField->Size * pField->Width );

                        pFieldNode = pFieldNode->Next;
                    }while( pFieldNode != pFieldNodeHead );

                    if( TotalNumber >= ( Offset + Number ))
                    {
                        /* ��ȡ������� */
                        status = XJ_SUCCESS;
                        break;
                    }
                }
            }

            /* ָ����һ������ */
            pSrc += ( pTable->Size );
        }
    }

    if( ActualNumber != XJ_NULL )
    {
        /* ����ʵ�ʶ�ȡ�����ݸ��� */
        * ActualNumber = TotalNumber;
    }

    /* ���ض�ȡ���ݵ�״̬ */
    return( status );
}

/* �޸��ֶε�ֵ */
INT CTBC_SetFieldValue( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * buffer, INT Originate )
{
    INT result;
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;
    TB_FCB * pField;
    TB_CFB * pHandle;
    INT TotalNumber;
    register UNSIGNED_CHAR * pSrc;
    register UNSIGNED_CHAR * pDst;
    register UNSIGNED_CHAR * p;
    register INT i;
    TB_FCB * CondField[MAX_CONDITION];
    INT iStatus1 = 30;  /* �����ֵ */

    /* ������ */
    if( Number == 0 )
    {
        return( XJ_SUCCESS );
    }

    /* �������ͱ� */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        /* ���������ֶ� */
        pHandle = Handle;
        for( i = 0; i < pHandle->CondNumber; i ++)
        {
            CondField[i] = CTBC_FindField( pTable,( pHandle->CondIndex[i] ));
            if( CondField[i] == XJ_NULL )
            {
                /* ��Ч�����ֶ�,����ʧ�� */
                status = XJ_UNAVAILABLE;
                return( iStatus1 + 1 );
            }
        }

        /* ���Ҳ����ֶ� */
        pField = CTBC_FindField( pTable,( pHandle->HandleIndex ));
        if(( pField != XJ_NULL )
            && ( pField->Attribute & FIELD_WRITE_ENABLE ))
        {
            TotalNumber = 0;
            pSrc = ( UNSIGNED_CHAR *) buffer;
            pDst = ( UNSIGNED_CHAR *)pTable->DataMemory;
            for( i = 0; i < pTable->DataNumber; i ++)
            {
                /* ������� */
                if( CTBC_FieldCondCheck( pDst, pHandle->CondNumber,
                    pHandle->CondValue, CondField, pHandle->CondType ) == XJ_SUCCESS )
                {
                    /* ��������,�ܸ�����һ */
                    if(( TotalNumber ++) >= Offset )
                    {
                        /* �ֶ�ǰ������ */
                        if( CTable.InitFlag == XJ_TRUE )
                        {
                            if( pField -> foreTreatment )
                            {
                                result = pField -> foreTreatment( pTable->Index, pField->Index, pDst, pSrc );
                                if( result != XJ_SUCCESS )
                                {
                                    /* ����ʧ��״̬ */
                                    status = ( iStatus1 + 2 );
                                    break;
                                }
                            }
                        }

                        p = pDst + ( pField->Offset );
                        memcpy( p, pSrc, ( UNSIGNED )pField->Size ); /* �޸��ֶ�ֵ */
                        pSrc += ( pField->Size );

                        /* �ֶκ����� */
                        if( CTable.InitFlag == XJ_TRUE )
                        {
                            if( pField -> afterTreatment )
                            {
                                result = pField -> afterTreatment( pTable->Index, pField->Index, pDst, Originate );
                                if( result != XJ_SUCCESS )
                                {
                                    /* ����ʧ��״̬ */
                                    status = ( iStatus1 + 3 );
                                    break;
                                }
                            }
                        }

                        if( TotalNumber >= ( Offset + Number ))
                        {
                            /* �޸�������� */
                            status = XJ_SUCCESS;
                            break;
                        }
                    }
                }

                /* ָ����һ������ */
                pDst += ( pTable->Size );
            }
            if( status == XJ_UNAVAILABLE )
            {
                /* ���ؼ�¼�����ʶ�Ĵ���ֵ */
                status = ( iStatus1 + 5 );
            }
            /* �ֶν��������� */
            if( CTable.InitFlag == XJ_TRUE )
            {
                if( pField -> endTreatment )
                {
                    pField -> endTreatment( pTable->Index, pField->Index );
                }
            }
        }
        else
        {
            /* ���ؼ�¼�����ʶ�Ĵ���ֵ */
            status = ( iStatus1 + 4 );
        }
    }
    else
    {
        /* ���ؼ�¼�����ʶ�Ĵ���ֵ */
        status = ( iStatus1 + 6 );
    }

    return( status );
}

/* �ֶ�Ԥ������ */
INT CTBC_PreFieldValue( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * buffer, INT Originate )
{
    INT result;
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;
    TB_FCB * pField;
    TB_CFB * pHandle;
    INT TotalNumber;
    register UNSIGNED_CHAR * pSrc;
    register UNSIGNED_CHAR * pDst;
    register INT i;
    TB_FCB * CondField[MAX_CONDITION];

    /* ������ */
    if( Number == 0 )
    {
        return( XJ_SUCCESS );
    }

    /* �������ͱ� */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        /* ���������ֶ� */
        pHandle = Handle;
        for( i = 0; i < pHandle->CondNumber; i ++)
        {
            CondField[i] = CTBC_FindField( pTable,( pHandle->CondIndex[i] ));
            if( CondField[i] == XJ_NULL )
            {
                /* ��Ч�����ֶ�,����ʧ�� */
                status = XJ_UNAVAILABLE;
                return( status );
            }
        }

        /* ���Ҳ����ֶ� */
        pField = CTBC_FindField( pTable,( pHandle->HandleIndex ));
        if(( pField != XJ_NULL )
            && ( pField->Attribute & FIELD_WRITE_ENABLE ))
        {
            TotalNumber = 0;
            pSrc = ( UNSIGNED_CHAR *) buffer;
            pDst = ( UNSIGNED_CHAR *)pTable->DataMemory;
            for( i = 0; i < pTable->DataNumber; i ++)
            {
                /* ������� */
                if( CTBC_FieldCondCheck( pDst, pHandle->CondNumber,
                    pHandle->CondValue, CondField, pHandle->CondType ) == XJ_SUCCESS )
                {
                    /* ��������,�ܸ�����һ */
                    if(( TotalNumber ++) >= Offset )
                    {
                        /* �ֶ�ǰ������ */
                        if( CTable.InitFlag == XJ_TRUE )
                        {
                            if( pField -> preTreatment )
                            {
                                result = pField -> preTreatment( pTable->Index,
                                    pField->Index, pDst, pSrc, Originate );
                                if( result != XJ_SUCCESS )
                                {
                                    /* ����ʧ��״̬ */
                                    status = result;
                                    break;
                                }
                            }
                        }

                        if( TotalNumber >= ( Offset + Number ))
                        {
                            /* �޸�������� */
                            status = XJ_SUCCESS;
                            break;
                        }
                        pSrc += ( pField->Size * pField->Width );
                    }
                }

                /* ָ����һ������ */
                pDst += ( pTable->Size );
            }
        }
    }

    return( status );
}

/* �����ԭʼ���ݵ�CRC�� */
INT CTBC_CalTableDataCRC( VOID )
{
    INT i;
    INT status;
    TB_TCB * pTable;
    TB_FCB * pField;
    TB_NODE * pNode;
    UNSIGNED crc_code1; /* ÿ�����CRC�� */
    UNSIGNED crc_code2; /* ȫ�����CRC�� */
    UNSIGNED_CHAR * pSrc;
    TB_NODE * pFieldNode,* pFieldNodeHead;

    status = XJ_SUCCESS;
    if( TBD_Created_Tables_List == XJ_NULL )
    {
        /* ������,ֱ�ӷ��سɹ� */
        return( status );
    }

    /* ���ñ�����CRC��ĳ�ֵ. */
    crc_code2 = CTBC_GET_TABLE_CRCINI();
    pNode = TBD_Created_Tables_List;
    do
    {
        /* �������ͱ� */
        pTable = ( TB_TCB *)pNode;
        if( pTable->FieldLink != XJ_NULL )
        {
            /* ���ñ�����CRC��ĳ�ֵ. */
            crc_code1 = CTBC_GET_TABLE_CRCINI();
            pFieldNode = &(( pTable->FieldLink )->Link );
            pFieldNodeHead = &(( pTable->FieldLink )->Link );
            do
            {
                /* ����CRC������ֶ� */
                pField = ( TB_FCB *)pFieldNode;
                if(( pField->Attribute & FIELD_CRC_ENABLE ) != 0 )
                {
                    /* �洢����ֵ���CRC�� */
                    pSrc = ( UNSIGNED_CHAR *)pTable->DataMemory;
                    pSrc += pField->Offset; /* CRC���ֶε��ڴ���ַ */
                    for( i = 0; i < pTable->DataNumber; i ++)
                    {
                        /* ����CRC�� */
                        CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )pField->Size, CTBC_DATA_BYTE_NUMBER_ADDRESS,
                            CTBC_GET_TABLE_CRCBITS(), crc_code1,& crc_code1 );
                        CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )pField->Size, CTBC_DATA_BYTE_NUMBER_ADDRESS,
                            CTBC_GET_TABLE_CRCBITS(), crc_code2,& crc_code2 );
                        pSrc += pTable->Size;
                    }
                }
                pFieldNode = pFieldNode->Next;
            }while( pFieldNode != pFieldNodeHead );

            /* CRC������� */
            crc_code1 = ( crc_code1 ^ CTBC_GET_TABLE_CRCMASKS());
            pTable -> CrcCode = crc_code1;
        }
        pNode = pNode -> Next;
    }while( pNode != TBD_Created_Tables_List );

    /* CRC������� */
    crc_code2 = ( crc_code2 ^ CTBC_GET_TABLE_CRCMASKS());
    CTable.DataCrcCode = crc_code2;

    return( status );
}

/* ������������ݵ�CRC�� */
INT CTBC_CalTableConfigCRC( VOID )
{
    INT i;
    INT status;
    TB_TCB * pTable;
    TB_FCB * pField;
    TB_NODE * pNode;
    UNSIGNED crc_code;
    UNSIGNED_CHAR * pSrc;
    TB_NODE * pFieldNode,* pFieldNodeHead;

    status = XJ_SUCCESS;
    if( TBD_Created_Tables_List == XJ_NULL )
    {
        /* ������,ֱ�ӷ��سɹ� */
        return( status );
    }

    /* ���ñ�����CRC��ĳ�ֵ. */
    crc_code = CTBC_GET_TABLE_CRCINI();
    pNode = TBD_Created_Tables_List;
    do
    {
        /* �������ͱ� */
        pTable = ( TB_TCB *)pNode;
        if( pTable->FieldLink != XJ_NULL )
        {
            pFieldNode = &(( pTable->FieldLink )->Link );
            pFieldNodeHead = &(( pTable->FieldLink )->Link );
            do
            {
                /* ����CRC������ֶ� */
                pField = ( TB_FCB *)pFieldNode;
                if(( pField->Attribute & FIELD_CRC_ENABLE ) != 0 )
                {
                    /* �洢����ֵ���CRC�� */
                    pSrc = ( UNSIGNED_CHAR *)pTable->DataMemory;
                    pSrc += pField->Offset; /* CRC���ֶε��ڴ���ַ */
                    for( i = 0; i < pTable->DataNumber; i ++)
                    {
                        /* ����CRC�� */
                        CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )pField->Size, CTBC_DATA_BYTE_NUMBER_ADDRESS,
                            CTBC_GET_TABLE_CRCBITS(), crc_code,& crc_code );
                        pSrc += pTable->Size;
                    }
                }
                pFieldNode = pFieldNode->Next;
            }while( pFieldNode != pFieldNodeHead );
        }
        pNode = pNode -> Next;
    }while( pNode != TBD_Created_Tables_List );

    /* CRC������� */
    crc_code = ( crc_code ^ CTBC_GET_TABLE_CRCMASKS());
    CTable.ConfigCrcCode = crc_code;

    return( status );
}

/* ������ֶ���Ϣ��CRC�� */
INT CTBC_CalTableFieldInfoCRC( VOID )
{
    INT status;
    TB_TCB * pTable;
    TB_FCB * pField;
    TB_NODE * pNode;
    UNSIGNED crc_code;
    UNSIGNED_CHAR * pSrc;
    TB_NODE * pFieldNode,* pFieldNodeHead;

    status = XJ_SUCCESS;
    if( TBD_Created_Tables_List == XJ_NULL )
    {
        /* ������,ֱ�ӷ��سɹ� */
        return( status );
    }

    /* ���ñ�����CRC��ĳ�ֵ. */
    crc_code = CTBC_GET_TABLE_CRCINI();
    pNode = TBD_Created_Tables_List;
    do
    {
        /* �������ͱ� */
        pTable = ( TB_TCB *)pNode;
        if( pTable->FieldLink != XJ_NULL )
        {
            pFieldNode = &(( pTable->FieldLink )->Link );
            pFieldNodeHead = &(( pTable->FieldLink )->Link );
            do
            {
                pField = ( TB_FCB *)pFieldNode;
                /* ����������CRC�� */
                pSrc = ( UNSIGNED_CHAR *)&( pField->Index );
                CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )sizeof( INT ), CTBC_DATA_BYTE_NUMBER_ADDRESS,
                    CTBC_GET_TABLE_CRCBITS(), crc_code,& crc_code );
                /* ������������CRC�� */
                pSrc = ( UNSIGNED_CHAR *)&( pField->Attribute );
                CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )sizeof( INT ), CTBC_DATA_BYTE_NUMBER_ADDRESS,
                    CTBC_GET_TABLE_CRCBITS(), crc_code,& crc_code );
                /* ������������CRC�� */
                pSrc = ( UNSIGNED_CHAR *)&( pField->Type );
                CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )sizeof( INT ), CTBC_DATA_BYTE_NUMBER_ADDRESS,
                    CTBC_GET_TABLE_CRCBITS(), crc_code,& crc_code );
                /* �����������͵��ֽ���CRC�� */
                pSrc = ( UNSIGNED_CHAR *)&( pField->TypeByte );
                CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )sizeof( INT ), CTBC_DATA_BYTE_NUMBER_ADDRESS,
                    CTBC_GET_TABLE_CRCBITS(), crc_code,& crc_code );
                /* ��������ת�����CRC�� */
                pSrc = ( UNSIGNED_CHAR *)&( pField->Width );
                CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )sizeof( INT ), CTBC_DATA_BYTE_NUMBER_ADDRESS,
                    CTBC_GET_TABLE_CRCBITS(), crc_code,& crc_code );
                /* �����������ʹ�СCRC�� */
                pSrc = ( UNSIGNED_CHAR *)&( pField->TypeSize );
                CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )sizeof( INT ), CTBC_DATA_BYTE_NUMBER_ADDRESS,
                    CTBC_GET_TABLE_CRCBITS(), crc_code,& crc_code );
                /* �����ֶγ���CRC�� */
                pSrc = ( UNSIGNED_CHAR *)&( pField->Size );
                CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )sizeof( INT ), CTBC_DATA_BYTE_NUMBER_ADDRESS,
                    CTBC_GET_TABLE_CRCBITS(), crc_code,& crc_code );
                /* �����ֶ�����Ԫ�ظ���CRC�� */
                pSrc = ( UNSIGNED_CHAR *)&( pField->MemberNumber );
                CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )sizeof( INT ), CTBC_DATA_BYTE_NUMBER_ADDRESS,
                    CTBC_GET_TABLE_CRCBITS(), crc_code,& crc_code );
                /* ����λ��ƫ��CRC�� */
                pSrc = ( UNSIGNED_CHAR *)&( pField->Offset );
                CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )sizeof( INT ), CTBC_DATA_BYTE_NUMBER_ADDRESS,
                    CTBC_GET_TABLE_CRCBITS(), crc_code,& crc_code );
                /* ��������CRC�� */
                pSrc = ( UNSIGNED_CHAR *)( pField->Name );
                CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )( sizeof( CHAR ) * FIELD_NAME ),
                    CTBC_DATA_BYTE_NUMBER_ADDRESS, CTBC_GET_TABLE_CRCBITS(), crc_code,& crc_code );

                pFieldNode = pFieldNode->Next;
            }while( pFieldNode != pFieldNodeHead );
        }
        pNode = pNode -> Next;
    }while( pNode != TBD_Created_Tables_List );

    /* CRC������� */
    crc_code = ( crc_code ^ CTBC_GET_TABLE_CRCMASKS());
    CTable.FieldCrcCode = crc_code;

    return( status );
}

/* �������ݱ������ */
INT CTBC_FreezeTableData( INT TableIndex )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;

    /* �������ͱ� */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        pTable -> FreezeData = FREEZE_TABLE_DATA; /* �������� */
        ( pTable -> FreezeCounter )++;
        status = XJ_SUCCESS;
    }

    return( status );
}

/* �ⶳ���ݱ������ */
INT CTBC_UnFreezeTableData( INT TableIndex )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;

    /* �������ͱ� */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        if( pTable -> FreezeCounter > 0 )
        {
            ( pTable -> FreezeCounter )--;
        }
        if( pTable -> FreezeCounter == 0 )
        {
            pTable -> FreezeData = 0; /* �ⶳ���� */
        }
        status = XJ_SUCCESS;
    }

    return( status );
}

/* ��ʼ�����ݱ�����ݵĶ���״̬ */
INT CTBC_FreezeTableInitialize( INT TableIndex )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;

    /* �������ͱ� */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        pTable -> FreezeCounter = 0;
        pTable -> FreezeData = 0;
        status = XJ_SUCCESS;
    }

    return( status );
}

/* ��ȡ���ݱ�Ķ���״̬ */
INT CTBC_GetFreezeStatus( INT TableIndex )
{
    INT status = XJ_FALSE;        /* Completion status */
    TB_TCB * pTable;

    /* �������ͱ� */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        if( pTable -> FreezeData == FREEZE_TABLE_DATA )
        {
            status = XJ_TRUE;
        }
    }

    return( status );
}

/* ע���ֶ�Ԥ������ */
INT CTBC_RegisterPreTreatmentFunc( INT TableIndex, INT FieldIndex,
    INT (* preTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, VOID * valuePtr, INT Originate ) )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;
    TB_FCB * pField;

    /* �������ͱ� */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        /* �����ֶα� */
        pField = CTBC_FindField( pTable, FieldIndex );
        if( pField != XJ_NULL )
        {
            pField -> preTreatment = preTreatment;
            status = XJ_SUCCESS;
        }
    }

    return( status );
}

/* ע���ֶ�ǰ������ */
INT CTBC_RegisterForeTreatmentFunc( INT TableIndex, INT FieldIndex,
    INT (* foreTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, VOID * valuePtr ) )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;
    TB_FCB * pField;

    /* �������ͱ� */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        /* �����ֶα� */
        pField = CTBC_FindField( pTable, FieldIndex );
        if( pField != XJ_NULL )
        {
            pField -> foreTreatment = foreTreatment;
            status = XJ_SUCCESS;
        }
    }

    return( status );
}

/* ע���ֶκ����� */
INT CTBC_RegisterAfterTreatmentFunc( INT TableIndex, INT FieldIndex,
    INT (* afterTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, INT Originate ) )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;
    TB_FCB * pField;

    /* �������ͱ� */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        /* �����ֶα� */
        pField = CTBC_FindField( pTable, FieldIndex );
        if( pField != XJ_NULL )
        {
            pField -> afterTreatment = afterTreatment;
            status = XJ_SUCCESS;
        }
    }

    return( status );
}

/* ע���ֶν��������� */
INT CTBC_RegisterEndTreatmentFunc( INT TableIndex, INT FieldIndex,
    INT (* endTreatment )( INT TabIndex, INT FieldIndex ) )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;
    TB_FCB * pField;

    /* �������ͱ� */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        /* �����ֶα� */
        pField = CTBC_FindField( pTable, FieldIndex );
        if( pField != XJ_NULL )
        {
            pField -> endTreatment = endTreatment;
            status = XJ_SUCCESS;
        }
    }

    return( status );
}

/* ע����������ֵǰ������ */
INT CTBC_RegisterForeLeadTreatmentFunc( INT TableIndex,
    INT (* foreLeadTreatment )( INT TabIndex ) )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;

    /* �������ͱ� */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        pTable -> foreLeadTreatment = foreLeadTreatment;

        status = XJ_SUCCESS;
    }

    return( status );
}

/* ע����������ֵ������ */
INT CTBC_RegisterAfterLeadTreatmentFunc( INT TableIndex,
    INT (* afterLeadTreatment )( INT TabIndex ) )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;

    /* �������ͱ� */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        pTable -> afterLeadTreatment = afterLeadTreatment;

        status = XJ_SUCCESS;
    }

    return( status );
}

/* ��ȡָ��CPU�����ݱ�ָ�� */
UNSIGNED CTBC_GetTablePointers(  HTAB ** PointerList, UNSIGNED MaxPointers )
{
    UNSIGNED pointers;
    TB_NODE * node_ptr;

    /* Initialize the number of pointers returned.  */
    pointers =  0;

    if( TBD_Created_Tables_List )
    {
        /* Loop until all table pointers are in the list or until the maximum
           list size is reached.  */

        node_ptr = TBD_Created_Tables_List;
        while (( node_ptr ) && ( pointers < MaxPointers ))
        {
            /* Place the node into the destination list.  */
            * PointerList ++ =  ( HTAB *) node_ptr;

            /* Increment the pointers variable.  */
            pointers ++;

            /* Position the node pointer to the next node.  */
            node_ptr = node_ptr -> Next;

            /* Determine if the pointer is at the head of the list.  */
            if ( node_ptr == TBD_Created_Tables_List )
            {
                /* The list search is complete.  */
                node_ptr =  NU_NULL;
            }
        }
    }

    /* Return the number of pointers in the list.  */
    return( pointers );
}
