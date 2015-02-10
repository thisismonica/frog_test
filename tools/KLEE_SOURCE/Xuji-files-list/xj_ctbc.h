/*************************************************************************/
/*                                                                       */
/*        Copyright (c) 1999-2002 XJ ELECTRIC CO.LTD.                    */
/*                                                                       */
/* PROPRIETARY RIGHTS of Accelerated Technology are involved in the      */
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
/*      xj_ctbc.h                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CTB - Common Table Management                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains function prototypes of all functions          */
/*      accessible to other components.                                  */
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
/*      None                                                             */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhideYang.      04-12-2007      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"

/* Check to see if the file has been included already.  */

#ifndef XJ_CTBC_H
#define XJ_CTBC_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef XJ_TABLE_MODULE
#define XJ_TABLE_MODULE
#define TYPE_NAME               20  /* 数据类型名称长度 */
#define TABLE_NAME              12  /* 表名称长度 */
#define FIELD_NAME              12  /* 字段名称长度 */
#define MAX_CONDITION           3   /* 操作字段的最大限制条件个数 */
#define MAX_HANDLE              4   /* 最大操作字段个数 */
/* 字段属性定义 */
#define FIELD_CONFIG_ENABLE     1   /* 字段配置选项 */
#define FIELD_SETTING_ENABLE    2   /* 字段整定选项 */
#define FIELD_CRC_ENABLE        4   /* 字段CRC选项 */
#define FIELD_VALUE_ENABLE      8   /* 字段量值(包括整定值)选项 */
#define FIELD_CHECK_ENABLE      0x10 /* 字段边界检查选项 */
/* 数据类型定义 */
#define TYPE_CHAR               1   /* char */
#define TYPE_UNSIGNED_CHAR      2   /* unsigned char */
#define TYPE_INT                3   /* int */
#define TYPE_SIGNED             4   /* long */
#define TYPE_UNSIGNED           5   /* unsigned long */
#define TYPE_FLOAT              6   /* float */
/* 修改整定值选项 */
#define SETTING_ALL_ENABLE      1   /* 全部整定值 */
#define SETTING_ONE_ENABLE      2   /* 单个整定值 */
/* 条件类型 */
#define COND_TYPE_AND           1   /* 与操作 */
#define COND_TYPE_OR            2   /* 或操作 */
#define COND_TYPE_EQU           3   /* 等于操作 */
/* 数据表类型 */
#define TABLE_TYPE_RELATION     (1 << 0) /* 双CPU数据表数据关联 */

/* 条件字段操作块结构定义 */
typedef struct TB_CFB_STRUCT
{
    /* 操作字段索引 */
    INT HandleIndex;
    /* 条件字段个数 */
    INT CondNumber;
    /* 条件字段索引 */
    INT CondIndex[MAX_CONDITION];
    /* 条件字段值 */
    INT CondValue[MAX_CONDITION];
    /* 条件类型 */
    INT CondType[MAX_CONDITION];
}TB_CFB;

/* 条件字段操作块结构定义 */
typedef struct TB_CHB_STRUCT
{
    /* 操作字段个数 */
    INT HandleNumber;
    /* 操作字段索引 */
    INT HandleIndex[MAX_HANDLE];
    /* 条件字段个数 */
    INT CondNumber;
    /* 条件字段索引 */
    INT CondIndex[MAX_CONDITION];
    /* 条件字段值 */
    INT CondValue[MAX_CONDITION];
    /* 条件类型 */
    INT CondType[MAX_CONDITION];
}TB_CHB;

typedef long HTAB;                /* 应用程序对象句柄 */

#endif

/*=======================================================================*/
/* 对象处理                                                              */
/*=======================================================================*/
/* 创建对象 */

/*=======================================================================*/
/* 数据表管理                                                            */
/*=======================================================================*/
/* 定义数据表 */
INT CTBC_DefTable( CHAR * Name, INT Index, INT MaxNumber, INT Size, INT Type, 
    INT EditFieldSize );
/* 定义字段 */
INT CTBC_DefField( INT TableIndex, CHAR * Name, INT Index, INT Attribute,
    INT DataType, INT Number, INT Offset );
/* 生成表数据 */
VOID * CTBC_CreateAllField( INT TableIndex, VOID * Buffer );
/* 获取单个字段的值 */
INT CTBC_GetOneFieldValue( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * Buffer, INT * ActualNumber );
/* 读多个字段值 */
INT CTBC_GetBatchFieldValue( INT TableIndex, INT Offset, INT Number,
    TB_CHB * Handle, VOID * Buffer, INT * ActualNumber );
/* 获取全部字段的值 */
INT CTBC_GetAllFieldValue( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * Buffer, INT * ActualNumber );
/* 定义整定值范围 */
INT CTBC_DefSGRange( INT TableIndex, INT TypeIndex, INT CoefIndex,
    INT MinIndex, INT MaxIndex );
/* 设置配置字段值 */
INT CTBC_SetConfigValues( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * buffer, INT Originate );
/* 选择编辑定值组 */
INT CTBC_SelectEditSG( INT TableIndex, INT FieldIndex );
/* 设置定值组值 */
INT CTBC_SetEditSGValues( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * buffer );
/* 确认编辑定值组值 */
INT CTBC_ConfirmEditSGValue( INT TableIndex, INT Type, UNSIGNED Offset,
    TB_CFB * pHandle, UNSIGNED TimeOut, INT ( _AckCall )( INT, UNSIGNED_CHAR *),
    UNSIGNED_CHAR * Dst );
/* 初始化数据表环境变量 */
INT CTBC_TableInitialize( INT MaxTabNumber, INT MaxFieldNumber );
/* 定义数据保护句柄 */
INT CTBC_DefProtectHandle( INT TableIndex, VOID * pProtectHandle );
/* 冻结数据表的数据 */
INT CTBC_FreezeTableData( INT TableIndex );
/* 解冻数据表的数据 */
INT CTBC_UnFreezeTableData( INT TableIndex );
/* 初始化数据表的数据的冻结状态 */
INT CTBC_FreezeTableInitialize( INT TableIndex );
/* 获取数据表的冻结状态 */
INT CTBC_GetFreezeStatus( INT TableIndex );
/* 保存配置字段值 */
INT CTBC_SaveConfigValue( HTAB * pTabList, UNSIGNED TimeOut );
/* 保存表的配置字段值 */
INT CTBC_SaveTabConfigValue( INT TableIndex, UNSIGNED TimeOut );
/* 引导全部配置字段值 */
INT CTBC_LeadConfigValue( UNSIGNED TimeOut );
/* 保存全部定值组值 */
INT CTBC_SaveSGValue( UNSIGNED TimeOut );
/* 引导全部定值组值 */
INT CTBC_LeadSGValue( UNSIGNED TimeOut );
/* 数据表基本数据初始化 */
INT CTBC_SetDataInitialize( INT iType, UNSIGNED uTimeOut );
/* 固化非易失整定数据 */
INT CTBC_WriteNVData( VOID * Buffer, UNSIGNED uOffset, UNSIGNED uSize, 
    UNSIGNED uTimeOut );
/* 读取非易失整定数据 */
INT CTBC_ReadNVData( VOID * Buffer, UNSIGNED uOffset, UNSIGNED uSize, 
    UNSIGNED uTimeOut );
/* 修复非易失整定数据 */
INT CTBC_RepairNVData( UNSIGNED TimeOut );
/* 固化配置信息 */
INT CTBC_WriteConfigData( VOID * Buffer, UNSIGNED uOffset, 
    UNSIGNED uSize, UNSIGNED uTimeOut );
    /* 读取配置信息 */
INT CTBC_ReadConfigData( VOID * Buffer, UNSIGNED uOffset, 
    UNSIGNED uSize, UNSIGNED uTimeOut );
/* 修复配置数据 */
INT CTBC_RepairConfigData( UNSIGNED TimeOut );    
/* 注册字段预处理函数 */
INT CTBC_RegisterPreTreatmentFunc( INT TableIndex, INT FieldIndex,
    INT (* preTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, VOID * valuePtr, INT Originate ) );
/* 注册字段前处理函数 */
INT CTBC_RegisterForeTreatmentFunc( INT TableIndex, INT FieldIndex,
    INT (* foreTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, VOID * valuePtr ) );
/* 注册字段后处理函数 */
INT CTBC_RegisterAfterTreatmentFunc( INT TableIndex, INT FieldIndex,
    INT (* afterTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, INT Originate ) );
/* 注册字段结束处理函数 */
INT CTBC_RegisterEndTreatmentFunc( INT TableIndex, INT FieldIndex,
    INT (* endTreatment )( INT TabIndex, INT FieldIndex ) );    
/* 注册引导整定值前处理函数 */
INT CTBC_RegisterForeLeadTreatmentFunc( INT TableIndex,
    INT (* foreLeadTreatment )( INT TabIndex ) );
/* 注册引导整定值后处理函数 */
INT CTBC_RegisterAfterLeadTreatmentFunc( INT TableIndex,
    INT (* afterLeadTreatment )( INT TabIndex ) );
/* 更新定值组值 */
INT CTBC_UpdateSGValue( INT TableIndex, UNSIGNED TimeOut );
/* 计算RAM整定值字段的CRC码 */
INT CTBC_R_SGValueCRC( INT TableIndex, UNSIGNED * CRC_Code );
/* 计算非易失整定值字段的CRC码 */
INT CTBC_F_SGValueCRC( INT TableIndex, UNSIGNED TimeOut, UNSIGNED * CRC_Code );
/* 非易失整定值CRC码初始化 */
INT CTBC_SGValueCRC_Initialize( UNSIGNED TimeOut );
/* 整定值数据检测 */
INT CTBC_SGValueCheck( INT TableIndex, UNSIGNED TimeOut );
/* 获取整定值修改计数器值 */
INT CTBC_GetSGValueCounter( INT TableIndex, UNSIGNED * Counter );
/* 注册非易失整定值处理函数 */
INT CTBC_RegisterNVDataTreatmentFunc( UNSIGNED SetHandle, CHAR * dirName, CHAR * fileName,
    INT (* writeNVDataTreatment )( VOID *, UNSIGNED, UNSIGNED, UNSIGNED ),
    INT (* readNVDataTreatment )( VOID *, UNSIGNED, UNSIGNED, UNSIGNED ),
    INT (* repairNVDataTreatment )( UNSIGNED ) );
/* 注册配置信息处理函数 */
INT CTBC_RegisterConfigTreatmentFunc( UNSIGNED CfgHandle, CHAR * dirName, CHAR * fileName,
    INT (* writeConfigDataTreatment )( VOID *, UNSIGNED, UNSIGNED, UNSIGNED ),
    INT (* readConfigDataTreatment )( VOID *, UNSIGNED, UNSIGNED, UNSIGNED ),
    INT (* repairConfigDataTreatment )( UNSIGNED ));   
/* 获取指定CPU的数据表指针 */
UNSIGNED CTBC_GetTablePointers(  HTAB ** PointerList, UNSIGNED MaxPointers );    
/*========================================================================================*/
/*通讯专用函数                                                                            */
/*========================================================================================*/
/* 获取数据表的总个数 */
INT CTBC_GetTableNumber( VOID );
/* 获取数据表内存地址 */
VOID * CTBC_GetTableMemory( INT TabIndex, INT * ActualNum );
/* 获取数据表大小 */
INT CTBC_GetTableSize( INT TabIndex );
/* 获取数据表的信息 */
INT CTBC_GetTableInf( UNSIGNED Number, UNSIGNED StartNumber, UNSIGNED_CHAR * pDst, UNSIGNED * Length );
/* 获取字段的信息 */
INT CTBC_GetFieldInf( UNSIGNED Number, UNSIGNED StartNumber, UNSIGNED TabIndex, UNSIGNED_CHAR * pDst, UNSIGNED * Length, UNSIGNED * ActualNumber );
/* 获取数据表的状态 */
INT CTBC_GetTabStatus( INT TabIndex, UNSIGNED_CHAR * pDst );
/* 获取配置表的CRC码 */
UNSIGNED CTBC_GetConfigTabCRC( VOID );
/* 获取数据表字段信息的CRC码 */
UNSIGNED CTBC_GetTabFieldCRC( VOID );
/* 根据表的索引号获取字段中到指定条件数据之间的实际元素个数 */
INT CTBC_ReadDataOffset( INT TabIndex, TB_CFB * Handle, INT Offset, UNSIGNED * Actual_Number );
/* 获取指定数据表/指定条件/指定序号的数据模板地址 */
VOID * CTBC_GetIndexTableMemory( INT TableIndex, TB_CFB * Handle, INT iIndex );
/* 获取数据表的长度 */
INT CTBC_GetTabLength( INT TabIndex, UNSIGNED * Length );
/* 获取特定字段的长度 */
INT CTBC_GetFieldLength( INT TabIndex, INT HandleIndex, UNSIGNED * Length );
/* 根据表的索引号获取字段中到指定条件数据之间的实际元素个数 */
INT CTBC_ReadDataNumber( INT TabIndex, TB_CFB * Handle, UNSIGNED * Number );
/* 接口中直接调用数据表修改函数 */
INT CTBC_COM_HMI_ModifData( INT TableIndex, INT StartNumber, INT number,
    VOID * pCondBuf, INT iHandleNumber, VOID * pDataBuf );
#ifdef __cplusplus
}
#endif

#endif
