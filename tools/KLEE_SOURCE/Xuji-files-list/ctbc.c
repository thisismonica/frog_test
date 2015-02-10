
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

/* 宏定义 */
#define CTBC_CALCULATE_CRC(x,y,z,u,v,w) CRCC_CalculateCRC(x,y,z,u,v,w) //计算CRC码
#define CTBC_GET_TABLE_CRCINI() TABLE_CRC_INITIALIZE //获取表数据CRC码的初值
#define CTBC_GET_TABLE_CRCBITS() TABLE_CRC_BITS //获取表数据CRC码的位数
#define CTBC_GET_TABLE_CRCMASKS() TABLE_CRC_MASKS //获取表数据CRC码的掩码
#define CTBC_DATA_BYTE_NUMBER_ADDRESS  (4/sizeof(UNSIGNED)) // 表数据字节数/每地址

/* 指定地址空间存放数据个数计算:计算地址空间存放指定类型数据的个数,不够1个按1个计算 */
#define CTBC_DATA_SIZE_TO_NUMBER(x,y) (x=(x + y - 1)/y)
/* 数据对齐 */
#define CTBC_DATA_TIDINESS(x,y) (x=((x + y - 1)/y)*y)

/* 以下为OS相关配置 */
#ifdef WIN32
#include <stdio.h>
#include <malloc.h>
/* 全局变量申明 */
struct TBMemMService TBMMS = {malloc, free};
/* Windows 宏定义 */
#endif

#ifndef WIN32 /* 非windows缺省定义一些宏以便通过编译 */
/* 全局变量申明 */
struct TBMemMService TBMMS = {malloc, free};
/* Nucleus宏定义 */
#endif

/* Define external inner-component global data references.  */

/* Define internal function calls.  */
VOID CTBC_Place_On_List( TB_NODE ** head, TB_NODE * new_node );
TB_TYPE * CTBC_GetDataType( INT Index );
INT CTBC_CalTableDataCRC( VOID );
INT CTBC_CalTableConfigCRC( VOID );
INT CTBC_CalTableFieldInfoCRC( VOID );
VOID * CTBC_FieldCpy( VOID * dst, const VOID * src, unsigned number, register INT typeByte );

TB_NODE * TBD_Created_Tables_List = XJ_NULL; /* 创建定义表的表头指针 */

/* 基本数据类型定义 */
#define TOTAL_TYPE_NUMBER   6 /* 总的数据类型个数 */
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

/* 编辑状态在检查边界出错时,是否需要清除 */

/* 初始化数据表环境变量 */
INT CTBC_TableInitialize( INT MaxTabNumber, INT MaxFieldNumber )
{
    CTable.MaxTabNumber = MaxTabNumber; /* 最大数据表个数 */
    CTable.MaxFieldNumber = MaxFieldNumber; /* 最大字段个数 */

    return( XJ_SUCCESS );
}

/* 添加链表 */
VOID CTBC_Place_On_List( TB_NODE ** head, TB_NODE * new_node )
{
    /* 判断链表是否为空  */
    if (* head )
    {
        /* 链表非空,添加节点到链表的尾部  */
        new_node -> Previous = (* head ) -> Previous;
        ( new_node -> Previous ) -> Next = new_node;
        new_node -> Next = (* head );
        ( new_node -> Next ) -> Previous = new_node;
    }
    else
    {
        /* 链表为空,设置表头和新节点  */
        (* head ) =  new_node;
        new_node -> Previous =  new_node;
        new_node -> Next = new_node;
    }
}

/* 拷贝字段数据 */
VOID * CTBC_FieldCpy( VOID * dst, const VOID * src, unsigned number, register INT typeByte )
{
    if( typeByte == 1 ) /* 单字节类型数据 */
    {
        /* 单字节数据 */
        register UNSIGNED_CHAR * pDst;
        register UNSIGNED_CHAR * pSrc;
        register UNSIGNED_CHAR * pEnd;

        pDst = ( UNSIGNED_CHAR *) dst;
        pSrc = ( UNSIGNED_CHAR *) src;
        pEnd = pSrc + number;
        while ( pSrc < pEnd )
        {
            * pDst ++ = (* pSrc ) & 0xff ; /* 按字节拷贝 */
            pSrc ++;
        }
    }
    else
    {
        /* 4字节数据 */
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
                * pDst ++ = ( UNSIGNED_CHAR )(((* pSrc ) >> k * 8 ) & 0xff ); /* 按字节拷贝 */
            }
            pSrc ++;
        }
    }

    return ( dst );
}

/* 根据类型索引号获取类型信息 */
TB_TYPE * CTBC_GetDataType( INT Index )
{
    TB_TYPE * pType;
    TB_TYPE * pSearch_Type;
    INT i;

    pType = CType;
    pSearch_Type = XJ_NULL; /* 初始化查找结果为空 */
    for( i = 0; i < TOTAL_TYPE_NUMBER; i ++)
    {
        if( pType->Index == Index )
        {
            /* 找到类型表,返回找到的类型表首址 */
            pSearch_Type = pType;
            break;
        }
        pType ++;
    }

    return( pSearch_Type );
}

/* 根据索引号查找定义表 */
TB_TCB * CTBC_FindTable( INT TableIndex )
{
    TB_NODE * pNode;
    TB_TCB * pTable;
    TB_TCB * pSearch_Table;

    pSearch_Table = XJ_NULL; /* 初始化查找结果为空 */
    if( TBD_Created_Tables_List != XJ_NULL )
    {
        /* 定义表非空,查找指定索引号的定义表 */
        pNode = TBD_Created_Tables_List;
        do
        {
            pTable = ( TB_TCB *)pNode;
            if( pTable->Index == TableIndex )
            {
                /* 找到定义表,返回找到的定义表首址 */
                pSearch_Table = pTable;
                break;
            }
            pNode = pNode -> Next;
        }while( pNode != TBD_Created_Tables_List );
    }

    return( pSearch_Table );
}

/* 根据字段索引号,在指定表中查找定义字段 */
TB_FCB * CTBC_FindField( TB_TCB * pTable, INT FieldIndex )
{
    TB_FCB * pField;
    TB_FCB * pSearch_Field;
    TB_NODE * pNode,* pNodeHead;

    pSearch_Field = XJ_NULL; /* 初始化查找结果为空 */
    if( pTable->FieldLink != XJ_NULL )
    {
        /* 字段非空,查找指定索引号的字段 */
        pNode = &(( pTable->FieldLink )->Link );
        pNodeHead = &(( pTable->FieldLink )->Link );
        do
        {
            pField = ( TB_FCB *)pNode;
            if( pField->Index == FieldIndex )
            {
                /* 找到字段,返回字段首址 */
                pSearch_Field = pField;
                break;
            }
            pNode = pNode->Next;
        }while( pNode != pNodeHead );
    }

    return( pSearch_Field );
}

/* 定义结构表属性 */
INT CTBC_DefTable( CHAR * Name, INT Index, INT MaxNumber, INT Size, INT Type,
    INT EditFieldSize )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;
    UNSIGNED MemSize;
    INT FieldSize;

    /* 检查资源空间 */
    if(( CTable.TabNumber < CTable.MaxTabNumber )
        && ( CTable.FieldNumber < CTable.MaxFieldNumber ))
    {
        /* 申请定义表内存空间 */
        if( CTable.TabMemory == XJ_NULL )
        {
            CTable.TabNumber = 0;
            MemSize = ( CTable.MaxTabNumber ) * sizeof( TB_TCB );
            if( MemSize > 0 )
            {
                CTable.TabMemory = ( TB_TCB *)TBMMS.MemAlloc( MemSize );
                if( CTable.TabMemory == XJ_NULL )
                {
                    /* 内存不够,直接返回失败 */
                    CTable.Status |= TABLE_ERR_RAM_MEM;
                    CTable.NoRamMemory += MemSize;
                    status = XJ_NO_MEMORY;
                    return( status );
                }
            }
        }
        /* 申请定义字段内存空间 */
        if( CTable.FieldMemory == XJ_NULL )
        {
            CTable.FieldNumber = 0;
            MemSize = ( CTable.MaxFieldNumber ) * sizeof( TB_FCB );
            if( MemSize > 0 )
            {
                CTable.FieldMemory = ( TB_FCB *)TBMMS.MemAlloc( MemSize );
                if( CTable.FieldMemory == XJ_NULL )
                {
                    /* 内存不够,释放前面分配的内存,直接返回失败 */
                    CTable.Status |= TABLE_ERR_RAM_MEM;
                    CTable.NoRamMemory += MemSize;
                    TBMMS.MemFree( CTable.TabMemory );
                    status = XJ_NO_MEMORY;
                    return( status );
                }
            }
        }

        /* 申请数据内存空间 */
        pTable = CTable.TabMemory + CTable.TabNumber;
        MemSize = ( UNSIGNED )( MaxNumber * Size );
        if( MemSize > 0 )
        {
            pTable->DataMemory = ( void *)TBMMS.MemAlloc( MemSize );
            if( pTable->DataMemory == XJ_NULL )
            {
                /* 内存不够,直接返回失败 */
                CTable.Status |= TABLE_ERR_RAM_MEM;
                CTable.NoRamMemory += MemSize;
                status = XJ_NO_MEMORY;
                return( status );
            }
        }

        /* 申请编辑内存空间 */
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
                /* 内存不够,释放前面分配的内存,直接返回失败 */
                CTable.Status |= TABLE_ERR_RAM_MEM;
                CTable.NoRamMemory += MemSize;
                TBMMS.MemFree( pTable->DataMemory );
                status = XJ_NO_MEMORY;
                return( status );
            }
        }

        /* 添加数据表到链表 */
        pTable = CTable.TabMemory + CTable.TabNumber;
        CTBC_Place_On_List(& TBD_Created_Tables_List,&( pTable->Link ));

        /* 初始化数据信息 */
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
        pTable -> CfgOffset = CTable.TotalCfgSize; /* 表配置数据的起始位置偏移 */
        pTable -> CfgSize = 0; /* 表单个数据的配置字段尺寸 */
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

/* 定义字段属性 */
INT CTBC_DefField( INT TableIndex, CHAR * Name, INT Index, INT Attribute,
    INT DataType, INT Number, INT Offset )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;
    TB_FCB * pField;
    TB_TYPE * pType;
    INT DataOffset;

    /* 获取数据类型信息 */
    pType = CTBC_GetDataType( DataType );
    if( pType == XJ_NULL )
    {
        /* 无法识别的数据类型,返回失败 */
        status = XJ_UNAVAILABLE;
        return( status );
    }

    /* 查找类型表 */
    pTable = CTBC_FindTable( TableIndex );
    if(( pTable != XJ_NULL ) && ( CTable.FieldNumber < CTable.MaxFieldNumber )
        && ((( pType->Size * Number )+ Offset ) <= pTable->Size ))
    {
        if( Attribute & FIELD_SETTING_ENABLE )
        {
            DataOffset = 0;
            if( pTable->SetOffset == CTable.TotalSGSize )
            {
                /* 数据表的第一个整定值字段:按字对齐 */
                CTBC_DATA_TIDINESS( CTable.TotalSGSize,( INT )( sizeof( UNSIGNED ) ));
                pTable->SetOffset = CTable.TotalSGSize;
                /* 数据表的第一个存储字段之前存放CRC码, */
                DataOffset += sizeof( UNSIGNED ); /* 第1个整定值为CRC码 */
            }
            /* 表整定值数据尺寸统计 */
            DataOffset += ( pTable->MaxDataNumber * pType->Size * Number );
            CTable.TotalSGSize += DataOffset;
            /* 整定值字段数统计 */
            ( pTable -> SetFieldNumber )++;
        }

        if( Attribute & FIELD_CONFIG_ENABLE )
        {
            /* 配置字段 */
            DataOffset = 0;
            if( pTable->CfgOffset == CTable.TotalCfgSize )
            {
                /* 数据表的第一个配置字段:按字对齐 */
                CTBC_DATA_TIDINESS( CTable.TotalCfgSize,( INT )( sizeof( UNSIGNED ) ));
                pTable->CfgOffset = CTable.TotalCfgSize;
                /* 数据表的第一个配置字段前存放本表的CRC码 */
                DataOffset += sizeof( UNSIGNED ); /* 第1个数据为CRC码 */
            }
            DataOffset += ( pTable->MaxDataNumber * pType->Size * Number );
            CTable.TotalCfgSize += DataOffset;
            /* 表配置数据尺寸统计 */
            pTable -> CfgSize += ( pType->Size * Number );
        }

        /* 添加数据表到链表 */
        pField = CTable.FieldMemory + CTable.FieldNumber;
        CTBC_Place_On_List(( TB_NODE **)(&( pTable->FieldLink )),&( pField->Link ));

        /* 初始化字段信息 */
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

/* 创建全部字段 */
VOID * CTBC_CreateAllField( INT TableIndex, VOID * Buffer )
{
    TB_TCB * pTable;
    UNSIGNED_CHAR * pDst;
    VOID * pResult = XJ_NULL;

    /* 查找类型表 */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        if( pTable->DataNumber < pTable->MaxDataNumber )
        {
            /* 生成新的数据 */
            /* 拷贝数据 */
            pDst = ( UNSIGNED_CHAR *)pTable->DataMemory;
            pDst += ( pTable->DataNumber * pTable->Size );
            pResult = ( VOID *)pDst;
            memcpy( pDst, Buffer, ( UNSIGNED )pTable->Size );
            ( pTable->DataNumber )++;
        }
    }

    return( pResult );
}

/* 检查指定数据是否满足要求 */
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

    pCondValue = CondValue; /* 条件值 */
    pCondField = CondField; /* 条件字段 */
    pCondType = CondType; /* 条件类型 */
    for( i = 0; i < CondNumber; i ++)
    {
        pField = * pCondField;
        pSrc = ( UNSIGNED_CHAR *)Memory;
        pSrc += ( pField -> Offset );
        FieldValue = 0;
        memcpy(& FieldValue, pSrc, ( UNSIGNED )pField->TypeSize ); /* 读取字段值 */
        if((* pCondType ) == COND_TYPE_EQU )
        {
            /* 相等操作 */
            if( FieldValue != (* pCondValue ))
            {
                /* 条件不满足,返回失败 */
                status = XJ_UNAVAILABLE;
                break;
            }
        }
        else if((* pCondType ) == COND_TYPE_OR )
        {
            /* 或操作 */
            if(( FieldValue & (* pCondValue )) == 0 )
            {
                /* 条件不满足,返回失败 */
                status = XJ_UNAVAILABLE;
                break;
            }
        }
        else
        {
            /* 与操作 */
            if( ( FieldValue & (* pCondValue )) != (* pCondValue ))
            {
                /* 条件不满足,返回失败 */
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

/* 获取表中到指定字段之间的同属性数据元素的大小 */
TB_FCB * CTBC_GetFieldOffset( TB_TCB * pTable, INT FieldIndex, INT Attribute,
    UNSIGNED * Actual_Size )
{
    UNSIGNED ActualSize;
    TB_FCB * pField;
    TB_FCB * pSearch_Field;
    TB_NODE * pNode,* pNodeHead;

    ActualSize = 0; /* 初始化起始数据大小 */
    pSearch_Field = XJ_NULL; /* 初始化查找结果为空 */
    if( pTable->FieldLink != XJ_NULL )
    {
        /* 字段非空,查找指定索引号的字段 */
        pNode = &(( pTable->FieldLink )->Link );
        pNodeHead = &(( pTable->FieldLink )->Link );
        do
        {
            pField = ( TB_FCB *)pNode;
            if( pField->Index == FieldIndex )
            {
                /* 找到字段,进行属性检查 */
                if( pField->Attribute & Attribute )
                {
                    /* 指定属性,返回字段首址 */
                    pSearch_Field = pField;
                }
                break;
            }

            /* 计算表中同属性的数据大小 */
            if( pField->Attribute & Attribute )
            {
                ActualSize += ( pTable->DataNumber * pField -> TypeSize * pField -> MemberNumber );
            }

            pNode = pNode->Next;
        }while( pNode != pNodeHead );
    }

    /* 返回表中到指定字段之间的同属性数据元素的大小 */
    * Actual_Size = ActualSize;

    return( pSearch_Field );
}

/* 读单个字段值 */
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
    /* 查找类型表 */
    pTable = CTBC_FindTable( TableIndex );
    if(( pTable != XJ_NULL ) && ( Number != 0 ))
    {
        /* 查找条件字段 */
        pHandle = Handle;
        for( i = 0; i < pHandle->CondNumber; i ++)
        {
            CondField[i] = CTBC_FindField( pTable,( pHandle->CondIndex[i] ));
            if( CondField[i] == XJ_NULL )
            {
                if( ActualNumber != XJ_NULL )
                {
                    /* 返回实际读取的数据个数 */
                    * ActualNumber = TotalNumber;
                }
                /* 无效条件字段,返回失败 */
                status = XJ_UNAVAILABLE;
                return( status );
            }
        }

        /* 查找操作字段 */
        pField = CTBC_FindField( pTable,( pHandle->HandleIndex ));
        if( pField != XJ_NULL )
        {
            pSrc = ( UNSIGNED_CHAR *)pTable->DataMemory;
            pDst = ( UNSIGNED_CHAR *)Buffer;
            for( i = 0; i < pTable->DataNumber; i ++)
            {
                /* 条件检查 */
                if( CTBC_FieldCondCheck( pSrc, pHandle->CondNumber,
                    pHandle->CondValue, CondField, pHandle->CondType ) == XJ_SUCCESS )
                {
                    /* 满足条件,总个数增一 */
                    if(( TotalNumber ++) >= Offset )
                    {
                        p = pSrc + ( pField->Offset );
                        /* 读取字段值 */
                        CTBC_FieldCpy( pDst, p, ( UNSIGNED )pField->MemberNumber, pField->TypeByte );
                        pDst += ( pField->Size * pField->Width );

                        if( TotalNumber >= ( Offset + Number ))
                        {
                            /* 读取数据完成 */
                            status = XJ_SUCCESS;
                            break;
                        }
                    }
                }

                /* 指向下一个数据 */
                pSrc += ( pTable->Size );
            }
        }
    }

    if( ActualNumber != XJ_NULL )
    {
        /* 返回实际读取的数据个数 */
        * ActualNumber = TotalNumber;
    }

    /* 返回读取数据的状态 */
    return( status );
}

/* 读多个字段值 */
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
    /* 查找类型表 */
    pTable = CTBC_FindTable( TableIndex );
    if(( pTable != XJ_NULL ) && ( Number != 0 ))
    {
        /* 查找条件字段 */
        pHandle = Handle;
        for( i = 0; i < pHandle->CondNumber; i ++)
        {
            CondField[i] = CTBC_FindField( pTable,( pHandle->CondIndex[i] ));
            if( CondField[i] == XJ_NULL )
            {
                if( ActualNumber != XJ_NULL )
                {
                    /* 返回实际读取的数据个数 */
                    * ActualNumber = TotalNumber;
                }
                /* 无效条件字段,返回失败 */
                status = XJ_UNAVAILABLE;
                return( status );
            }
        }
        /* 查找操作字段 */
        for( i = 0; i < pHandle->HandleNumber; i ++)
        {
            HandField[i] = CTBC_FindField( pTable,( pHandle->HandleIndex[i] ));
            if( HandField[i] == XJ_NULL )
            {
                if( ActualNumber != XJ_NULL )
                {
                    /* 返回实际读取的数据个数 */
                    * ActualNumber = TotalNumber;
                }
                /* 无效条件字段,返回失败 */
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
                /* 条件检查 */
                if( CTBC_FieldCondCheck( pSrc, pHandle->CondNumber,
                    pHandle->CondValue, CondField, pHandle->CondType ) == XJ_SUCCESS )
                {
                    /* 满足条件,总个数增一 */
                    if(( TotalNumber ++) >= Offset )
                    {
                        for( k = 0; k < pHandle->HandleNumber; k ++)
                        {
                            pField = HandField[k];
                            p = pSrc + ( pField->Offset );
                            /* 读取字段值 */
                            CTBC_FieldCpy( pDst, p, ( UNSIGNED )pField->MemberNumber, pField->TypeByte );
                            pDst += ( pField->Size * pField->Width );
                            pField ++;
                        }
                        if( TotalNumber >= ( Offset + Number ))
                        {
                            /* 读取数据完成 */
                            status = XJ_SUCCESS;
                            break;
                        }
                    }
                }

                /* 指向下一个数据 */
                pSrc += ( pTable->Size );
            }
        }
    }

    if( ActualNumber != XJ_NULL )
    {
        /* 返回实际读取的数据个数 */
        * ActualNumber = TotalNumber;
    }

    /* 返回读取数据的状态 */
    return( status );
}

/* 读全部字段值 */
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
    /* 查找类型表 */
    pTable = CTBC_FindTable( TableIndex );
    if(( pTable != XJ_NULL ) && ( Number != 0 ))
    {
        /* 查找条件字段 */
        pHandle = Handle;
        for( i = 0; i < pHandle->CondNumber; i ++)
        {
            CondField[i] = CTBC_FindField( pTable,( pHandle->CondIndex[i] ));
            if( CondField[i] == XJ_NULL )
            {
                if( ActualNumber != XJ_NULL )
                {
                    /* 返回实际读取的数据个数 */
                    * ActualNumber = TotalNumber;
                }
                /* 无效条件字段,返回失败 */
                status = XJ_UNAVAILABLE;
                return( status );
            }
        }

        pSrc = ( UNSIGNED_CHAR *)pTable->DataMemory;
        pDst = ( UNSIGNED_CHAR *)Buffer;
        for( i = 0; i < pTable->DataNumber; i ++)
        {
            /* 条件检查 */
            if( CTBC_FieldCondCheck( pSrc, pHandle->CondNumber,
                pHandle->CondValue, CondField, pHandle->CondType ) == XJ_SUCCESS )
            {
                /* 满足条件,总个数增一 */
                if(( TotalNumber ++) >= Offset )
                {
                    pFieldNode = &(( pTable->FieldLink )->Link );
                    pFieldNodeHead = &(( pTable->FieldLink )->Link );
                    do
                    {
                        pField = ( TB_FCB *)pFieldNode;
                        p = pSrc + ( pField->Offset );
                        /* 读取字段值 */
                        CTBC_FieldCpy( pDst, p, ( UNSIGNED )pField->MemberNumber, pField->TypeByte );
                        pDst += ( pField->Size * pField->Width );

                        pFieldNode = pFieldNode->Next;
                    }while( pFieldNode != pFieldNodeHead );

                    if( TotalNumber >= ( Offset + Number ))
                    {
                        /* 读取数据完成 */
                        status = XJ_SUCCESS;
                        break;
                    }
                }
            }

            /* 指向下一个数据 */
            pSrc += ( pTable->Size );
        }
    }

    if( ActualNumber != XJ_NULL )
    {
        /* 返回实际读取的数据个数 */
        * ActualNumber = TotalNumber;
    }

    /* 返回读取数据的状态 */
    return( status );
}

/* 修改字段的值 */
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
    INT iStatus1 = 30;  /* 错误初值 */

    /* 检查参数 */
    if( Number == 0 )
    {
        return( XJ_SUCCESS );
    }

    /* 查找类型表 */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        /* 查找条件字段 */
        pHandle = Handle;
        for( i = 0; i < pHandle->CondNumber; i ++)
        {
            CondField[i] = CTBC_FindField( pTable,( pHandle->CondIndex[i] ));
            if( CondField[i] == XJ_NULL )
            {
                /* 无效条件字段,返回失败 */
                status = XJ_UNAVAILABLE;
                return( iStatus1 + 1 );
            }
        }

        /* 查找操作字段 */
        pField = CTBC_FindField( pTable,( pHandle->HandleIndex ));
        if(( pField != XJ_NULL )
            && ( pField->Attribute & FIELD_WRITE_ENABLE ))
        {
            TotalNumber = 0;
            pSrc = ( UNSIGNED_CHAR *) buffer;
            pDst = ( UNSIGNED_CHAR *)pTable->DataMemory;
            for( i = 0; i < pTable->DataNumber; i ++)
            {
                /* 条件检查 */
                if( CTBC_FieldCondCheck( pDst, pHandle->CondNumber,
                    pHandle->CondValue, CondField, pHandle->CondType ) == XJ_SUCCESS )
                {
                    /* 满足条件,总个数增一 */
                    if(( TotalNumber ++) >= Offset )
                    {
                        /* 字段前处理函数 */
                        if( CTable.InitFlag == XJ_TRUE )
                        {
                            if( pField -> foreTreatment )
                            {
                                result = pField -> foreTreatment( pTable->Index, pField->Index, pDst, pSrc );
                                if( result != XJ_SUCCESS )
                                {
                                    /* 返回失败状态 */
                                    status = ( iStatus1 + 2 );
                                    break;
                                }
                            }
                        }

                        p = pDst + ( pField->Offset );
                        memcpy( p, pSrc, ( UNSIGNED )pField->Size ); /* 修改字段值 */
                        pSrc += ( pField->Size );

                        /* 字段后处理函数 */
                        if( CTable.InitFlag == XJ_TRUE )
                        {
                            if( pField -> afterTreatment )
                            {
                                result = pField -> afterTreatment( pTable->Index, pField->Index, pDst, Originate );
                                if( result != XJ_SUCCESS )
                                {
                                    /* 返回失败状态 */
                                    status = ( iStatus1 + 3 );
                                    break;
                                }
                            }
                        }

                        if( TotalNumber >= ( Offset + Number ))
                        {
                            /* 修改数据完成 */
                            status = XJ_SUCCESS;
                            break;
                        }
                    }
                }

                /* 指向下一个数据 */
                pDst += ( pTable->Size );
            }
            if( status == XJ_UNAVAILABLE )
            {
                /* 返回记录错误标识的错误值 */
                status = ( iStatus1 + 5 );
            }
            /* 字段结束处理函数 */
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
            /* 返回记录错误标识的错误值 */
            status = ( iStatus1 + 4 );
        }
    }
    else
    {
        /* 返回记录错误标识的错误值 */
        status = ( iStatus1 + 6 );
    }

    return( status );
}

/* 字段预处理函数 */
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

    /* 检查参数 */
    if( Number == 0 )
    {
        return( XJ_SUCCESS );
    }

    /* 查找类型表 */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        /* 查找条件字段 */
        pHandle = Handle;
        for( i = 0; i < pHandle->CondNumber; i ++)
        {
            CondField[i] = CTBC_FindField( pTable,( pHandle->CondIndex[i] ));
            if( CondField[i] == XJ_NULL )
            {
                /* 无效条件字段,返回失败 */
                status = XJ_UNAVAILABLE;
                return( status );
            }
        }

        /* 查找操作字段 */
        pField = CTBC_FindField( pTable,( pHandle->HandleIndex ));
        if(( pField != XJ_NULL )
            && ( pField->Attribute & FIELD_WRITE_ENABLE ))
        {
            TotalNumber = 0;
            pSrc = ( UNSIGNED_CHAR *) buffer;
            pDst = ( UNSIGNED_CHAR *)pTable->DataMemory;
            for( i = 0; i < pTable->DataNumber; i ++)
            {
                /* 条件检查 */
                if( CTBC_FieldCondCheck( pDst, pHandle->CondNumber,
                    pHandle->CondValue, CondField, pHandle->CondType ) == XJ_SUCCESS )
                {
                    /* 满足条件,总个数增一 */
                    if(( TotalNumber ++) >= Offset )
                    {
                        /* 字段前处理函数 */
                        if( CTable.InitFlag == XJ_TRUE )
                        {
                            if( pField -> preTreatment )
                            {
                                result = pField -> preTreatment( pTable->Index,
                                    pField->Index, pDst, pSrc, Originate );
                                if( result != XJ_SUCCESS )
                                {
                                    /* 返回失败状态 */
                                    status = result;
                                    break;
                                }
                            }
                        }

                        if( TotalNumber >= ( Offset + Number ))
                        {
                            /* 修改数据完成 */
                            status = XJ_SUCCESS;
                            break;
                        }
                        pSrc += ( pField->Size * pField->Width );
                    }
                }

                /* 指向下一个数据 */
                pDst += ( pTable->Size );
            }
        }
    }

    return( status );
}

/* 计算表原始数据的CRC码 */
INT CTBC_CalTableDataCRC( VOID )
{
    INT i;
    INT status;
    TB_TCB * pTable;
    TB_FCB * pField;
    TB_NODE * pNode;
    UNSIGNED crc_code1; /* 每个表的CRC码 */
    UNSIGNED crc_code2; /* 全部表的CRC码 */
    UNSIGNED_CHAR * pSrc;
    TB_NODE * pFieldNode,* pFieldNodeHead;

    status = XJ_SUCCESS;
    if( TBD_Created_Tables_List == XJ_NULL )
    {
        /* 无数据,直接返回成功 */
        return( status );
    }

    /* 设置表数据CRC码的初值. */
    crc_code2 = CTBC_GET_TABLE_CRCINI();
    pNode = TBD_Created_Tables_List;
    do
    {
        /* 查找类型表 */
        pTable = ( TB_TCB *)pNode;
        if( pTable->FieldLink != XJ_NULL )
        {
            /* 设置表数据CRC码的初值. */
            crc_code1 = CTBC_GET_TABLE_CRCINI();
            pFieldNode = &(( pTable->FieldLink )->Link );
            pFieldNodeHead = &(( pTable->FieldLink )->Link );
            do
            {
                /* 查找CRC码计算字段 */
                pField = ( TB_FCB *)pFieldNode;
                if(( pField->Attribute & FIELD_CRC_ENABLE ) != 0 )
                {
                    /* 存储整定值表的CRC码 */
                    pSrc = ( UNSIGNED_CHAR *)pTable->DataMemory;
                    pSrc += pField->Offset; /* CRC码字段的内存首址 */
                    for( i = 0; i < pTable->DataNumber; i ++)
                    {
                        /* 计算CRC码 */
                        CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )pField->Size, CTBC_DATA_BYTE_NUMBER_ADDRESS,
                            CTBC_GET_TABLE_CRCBITS(), crc_code1,& crc_code1 );
                        CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )pField->Size, CTBC_DATA_BYTE_NUMBER_ADDRESS,
                            CTBC_GET_TABLE_CRCBITS(), crc_code2,& crc_code2 );
                        pSrc += pTable->Size;
                    }
                }
                pFieldNode = pFieldNode->Next;
            }while( pFieldNode != pFieldNodeHead );

            /* CRC码的掩码 */
            crc_code1 = ( crc_code1 ^ CTBC_GET_TABLE_CRCMASKS());
            pTable -> CrcCode = crc_code1;
        }
        pNode = pNode -> Next;
    }while( pNode != TBD_Created_Tables_List );

    /* CRC码的掩码 */
    crc_code2 = ( crc_code2 ^ CTBC_GET_TABLE_CRCMASKS());
    CTable.DataCrcCode = crc_code2;

    return( status );
}

/* 计算表配置数据的CRC码 */
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
        /* 无数据,直接返回成功 */
        return( status );
    }

    /* 设置表数据CRC码的初值. */
    crc_code = CTBC_GET_TABLE_CRCINI();
    pNode = TBD_Created_Tables_List;
    do
    {
        /* 查找类型表 */
        pTable = ( TB_TCB *)pNode;
        if( pTable->FieldLink != XJ_NULL )
        {
            pFieldNode = &(( pTable->FieldLink )->Link );
            pFieldNodeHead = &(( pTable->FieldLink )->Link );
            do
            {
                /* 查找CRC码计算字段 */
                pField = ( TB_FCB *)pFieldNode;
                if(( pField->Attribute & FIELD_CRC_ENABLE ) != 0 )
                {
                    /* 存储整定值表的CRC码 */
                    pSrc = ( UNSIGNED_CHAR *)pTable->DataMemory;
                    pSrc += pField->Offset; /* CRC码字段的内存首址 */
                    for( i = 0; i < pTable->DataNumber; i ++)
                    {
                        /* 计算CRC码 */
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

    /* CRC码的掩码 */
    crc_code = ( crc_code ^ CTBC_GET_TABLE_CRCMASKS());
    CTable.ConfigCrcCode = crc_code;

    return( status );
}

/* 计算表字段信息的CRC码 */
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
        /* 无数据,直接返回成功 */
        return( status );
    }

    /* 设置表数据CRC码的初值. */
    crc_code = CTBC_GET_TABLE_CRCINI();
    pNode = TBD_Created_Tables_List;
    do
    {
        /* 查找类型表 */
        pTable = ( TB_TCB *)pNode;
        if( pTable->FieldLink != XJ_NULL )
        {
            pFieldNode = &(( pTable->FieldLink )->Link );
            pFieldNodeHead = &(( pTable->FieldLink )->Link );
            do
            {
                pField = ( TB_FCB *)pFieldNode;
                /* 计算索引号CRC码 */
                pSrc = ( UNSIGNED_CHAR *)&( pField->Index );
                CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )sizeof( INT ), CTBC_DATA_BYTE_NUMBER_ADDRESS,
                    CTBC_GET_TABLE_CRCBITS(), crc_code,& crc_code );
                /* 计算数据属性CRC码 */
                pSrc = ( UNSIGNED_CHAR *)&( pField->Attribute );
                CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )sizeof( INT ), CTBC_DATA_BYTE_NUMBER_ADDRESS,
                    CTBC_GET_TABLE_CRCBITS(), crc_code,& crc_code );
                /* 计算数据类型CRC码 */
                pSrc = ( UNSIGNED_CHAR *)&( pField->Type );
                CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )sizeof( INT ), CTBC_DATA_BYTE_NUMBER_ADDRESS,
                    CTBC_GET_TABLE_CRCBITS(), crc_code,& crc_code );
                /* 计算数据类型的字节数CRC码 */
                pSrc = ( UNSIGNED_CHAR *)&( pField->TypeByte );
                CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )sizeof( INT ), CTBC_DATA_BYTE_NUMBER_ADDRESS,
                    CTBC_GET_TABLE_CRCBITS(), crc_code,& crc_code );
                /* 计算数据转换宽度CRC码 */
                pSrc = ( UNSIGNED_CHAR *)&( pField->Width );
                CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )sizeof( INT ), CTBC_DATA_BYTE_NUMBER_ADDRESS,
                    CTBC_GET_TABLE_CRCBITS(), crc_code,& crc_code );
                /* 计算数据类型大小CRC码 */
                pSrc = ( UNSIGNED_CHAR *)&( pField->TypeSize );
                CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )sizeof( INT ), CTBC_DATA_BYTE_NUMBER_ADDRESS,
                    CTBC_GET_TABLE_CRCBITS(), crc_code,& crc_code );
                /* 计算字段长度CRC码 */
                pSrc = ( UNSIGNED_CHAR *)&( pField->Size );
                CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )sizeof( INT ), CTBC_DATA_BYTE_NUMBER_ADDRESS,
                    CTBC_GET_TABLE_CRCBITS(), crc_code,& crc_code );
                /* 计算字段数据元素个数CRC码 */
                pSrc = ( UNSIGNED_CHAR *)&( pField->MemberNumber );
                CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )sizeof( INT ), CTBC_DATA_BYTE_NUMBER_ADDRESS,
                    CTBC_GET_TABLE_CRCBITS(), crc_code,& crc_code );
                /* 计算位置偏移CRC码 */
                pSrc = ( UNSIGNED_CHAR *)&( pField->Offset );
                CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )sizeof( INT ), CTBC_DATA_BYTE_NUMBER_ADDRESS,
                    CTBC_GET_TABLE_CRCBITS(), crc_code,& crc_code );
                /* 计算名称CRC码 */
                pSrc = ( UNSIGNED_CHAR *)( pField->Name );
                CTBC_CALCULATE_CRC( pSrc, ( UNSIGNED )( sizeof( CHAR ) * FIELD_NAME ),
                    CTBC_DATA_BYTE_NUMBER_ADDRESS, CTBC_GET_TABLE_CRCBITS(), crc_code,& crc_code );

                pFieldNode = pFieldNode->Next;
            }while( pFieldNode != pFieldNodeHead );
        }
        pNode = pNode -> Next;
    }while( pNode != TBD_Created_Tables_List );

    /* CRC码的掩码 */
    crc_code = ( crc_code ^ CTBC_GET_TABLE_CRCMASKS());
    CTable.FieldCrcCode = crc_code;

    return( status );
}

/* 冻结数据表的数据 */
INT CTBC_FreezeTableData( INT TableIndex )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;

    /* 查找类型表 */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        pTable -> FreezeData = FREEZE_TABLE_DATA; /* 冻结数据 */
        ( pTable -> FreezeCounter )++;
        status = XJ_SUCCESS;
    }

    return( status );
}

/* 解冻数据表的数据 */
INT CTBC_UnFreezeTableData( INT TableIndex )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;

    /* 查找类型表 */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        if( pTable -> FreezeCounter > 0 )
        {
            ( pTable -> FreezeCounter )--;
        }
        if( pTable -> FreezeCounter == 0 )
        {
            pTable -> FreezeData = 0; /* 解冻数据 */
        }
        status = XJ_SUCCESS;
    }

    return( status );
}

/* 初始化数据表的数据的冻结状态 */
INT CTBC_FreezeTableInitialize( INT TableIndex )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;

    /* 查找类型表 */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        pTable -> FreezeCounter = 0;
        pTable -> FreezeData = 0;
        status = XJ_SUCCESS;
    }

    return( status );
}

/* 获取数据表的冻结状态 */
INT CTBC_GetFreezeStatus( INT TableIndex )
{
    INT status = XJ_FALSE;        /* Completion status */
    TB_TCB * pTable;

    /* 查找类型表 */
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

/* 注册字段预处理函数 */
INT CTBC_RegisterPreTreatmentFunc( INT TableIndex, INT FieldIndex,
    INT (* preTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, VOID * valuePtr, INT Originate ) )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;
    TB_FCB * pField;

    /* 查找类型表 */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        /* 查找字段表 */
        pField = CTBC_FindField( pTable, FieldIndex );
        if( pField != XJ_NULL )
        {
            pField -> preTreatment = preTreatment;
            status = XJ_SUCCESS;
        }
    }

    return( status );
}

/* 注册字段前处理函数 */
INT CTBC_RegisterForeTreatmentFunc( INT TableIndex, INT FieldIndex,
    INT (* foreTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, VOID * valuePtr ) )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;
    TB_FCB * pField;

    /* 查找类型表 */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        /* 查找字段表 */
        pField = CTBC_FindField( pTable, FieldIndex );
        if( pField != XJ_NULL )
        {
            pField -> foreTreatment = foreTreatment;
            status = XJ_SUCCESS;
        }
    }

    return( status );
}

/* 注册字段后处理函数 */
INT CTBC_RegisterAfterTreatmentFunc( INT TableIndex, INT FieldIndex,
    INT (* afterTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, INT Originate ) )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;
    TB_FCB * pField;

    /* 查找类型表 */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        /* 查找字段表 */
        pField = CTBC_FindField( pTable, FieldIndex );
        if( pField != XJ_NULL )
        {
            pField -> afterTreatment = afterTreatment;
            status = XJ_SUCCESS;
        }
    }

    return( status );
}

/* 注册字段结束处理函数 */
INT CTBC_RegisterEndTreatmentFunc( INT TableIndex, INT FieldIndex,
    INT (* endTreatment )( INT TabIndex, INT FieldIndex ) )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;
    TB_FCB * pField;

    /* 查找类型表 */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        /* 查找字段表 */
        pField = CTBC_FindField( pTable, FieldIndex );
        if( pField != XJ_NULL )
        {
            pField -> endTreatment = endTreatment;
            status = XJ_SUCCESS;
        }
    }

    return( status );
}

/* 注册引导整定值前处理函数 */
INT CTBC_RegisterForeLeadTreatmentFunc( INT TableIndex,
    INT (* foreLeadTreatment )( INT TabIndex ) )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;

    /* 查找类型表 */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        pTable -> foreLeadTreatment = foreLeadTreatment;

        status = XJ_SUCCESS;
    }

    return( status );
}

/* 注册引导整定值后处理函数 */
INT CTBC_RegisterAfterLeadTreatmentFunc( INT TableIndex,
    INT (* afterLeadTreatment )( INT TabIndex ) )
{
    INT status = XJ_UNAVAILABLE;        /* Completion status */
    TB_TCB * pTable;

    /* 查找类型表 */
    pTable = CTBC_FindTable( TableIndex );
    if( pTable != XJ_NULL )
    {
        pTable -> afterLeadTreatment = afterLeadTreatment;

        status = XJ_SUCCESS;
    }

    return( status );
}

/* 获取指定CPU的数据表指针 */
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
