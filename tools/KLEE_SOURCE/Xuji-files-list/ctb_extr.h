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
/*      ctb_extr.h                                      Ver  1.0         */
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
/*      pro_defs.h                          Protect Control constants    */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhideYang.      02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_ctbc.h"

/* Check to see if the file has been included already.  */

#ifndef CTB_EXTR_H
#define CTB_EXTR_H

#define MAX_FLOAT_ERROR         0.0001 /* 浮点数计算误差(此数代表百分比) */
#define MAX_TABLE_NUMBER        50  /* 最大数据表个数 */
#define MAX_FIELD_NUMBER        1500 /* 最大字段个数 */
#define TABLE_CRC_INITIALIZE    0xffffffff /* 表数据CRC码的初值 */
#define TABLE_CRC_BITS          32  /* 表数据CRC码的位数 */
#define TABLE_CRC_MASKS         0xffffffff /* 表数据CRC码的掩码 */
#define FREEZE_TABLE_DATA       1 /* 冻结数据表数据 */

/* 计算类型定义 */
#define CHL_FOURIER1_           0   /*fourier algorthem */
#define CHL_MUL_                1   /*linearity algorthem */
#define CHL_VECTOR_             2   /*vector algorthem */
#define CHL_FREQ_               3   /*frequency algorthem */
#define CHL_INT_                4   /*integer algorthem */
#define CHL_ANG_                5   /*angle algorthem */
#define CHL_CTRL_               6   /*control word algorthem */
#define CHL_DISLOG_             7   /*logical display*/
#define CHL_RATA_               8   /* 额定值类型 */
#define CHL_INT_CTRL_           9   /* 整型控制字类型(0/1) */
#define CHL_ENUM_               10  /*enumerate type */

/* 字段属性定义 */
#define FIELD_WRITE_ENABLE (FIELD_CONFIG_ENABLE+FIELD_SETTING_ENABLE+FIELD_VALUE_ENABLE) /* 允许字段写 */

/* 整定值状态 */
#define SETTING_STATUS_SELECT   1   /* 选择编辑整定值 */
#define SETTING_STATUS_EDIT     2   /* 编辑整定值 */
#define SETTING_STATUS_SAVE     3   /* 保存整定值 */

/* 边界检查选项 */
#define CHECK_ENABLE            1   /* check enable */
#define CHECK_DISABLE           0   /* check disable */

/* 错误信息定义 */
#define TABLE_ERR_LEAD_SG       (1 << 0)    /* 引导整定值出错 */
#define TABLE_ERR_LEAD_CFG      (1 << 1)    /* 引导配置信息出错 */
#define TABLE_ERR_SG_MEM        (1 << 2)    /* 整定值存储空间不够 */
#define TABLE_ERR_CFG_MEM       (1 << 3)    /* 配置数据存储空间不够 */
#define TABLE_ERR_RAM_MEM       (1 << 4)    /* 内存空间不够 */
#define TABLE_ERR_LEAD_SG_CRC   (1 << 5)    /* 引导整定值CRC码出错 */
#define TABLE_ERR_LEAD_SG_READ  (1 << 6)    /* 引导整定值读数据出错 */
#define TABLE_ERR_LEAD_SG_CHECK (1 << 7)    /* 引导整定值数据检查出错 */

/* 获取结构元素的位置偏移,x:结构名称,y:成员名称 */
#define GETSTRUCTOFFSET(x,y)                    \
    (int)(&((##x*)0)->##y)

/* 读取基本数据类型的值,x:字段类型,y:数据地址,z:读取值,w:类型 */
#define GETBASETYPEDATA(x,y,z,w)                \
  {                                             \
    switch(x)                                   \
    {                                           \
        case TYPE_FLOAT:                        \
            z = (w)(*((float *)y));             \
            break;                              \
        case TYPE_UNSIGNED:                     \
            z = (w)(*((UNSIGNED *)y));          \
            break;                              \
        case TYPE_INT:                          \
            z = (w)(*((INT *)y));               \
            break;                              \
        case TYPE_SIGNED:                       \
            z = (w)(*((SIGNED *)y));            \
            break;                              \
        case TYPE_UNSIGNED_CHAR:                \
            z = (w)(*((UNSIGNED_CHAR *)y));     \
            break;                              \
        case TYPE_CHAR:                         \
            z = (w)(*((CHAR *)y));              \
            break;                              \
        default:                                \
            z = (w)(*((float *)y));             \
            break;                              \
    }                                           \
  }

/* 设置基本数据类型的值,x:类型,y:数据地址,z:设置值 */
#define SETBASETYPEDATA(x,y,z)                  \
  {                                             \
    switch(x)                                   \
    {                                           \
        case TYPE_FLOAT:                        \
            (*((float *)y)) = (float)(z);       \
            break;                              \
        case TYPE_UNSIGNED:                     \
            (*((UNSIGNED *)y)) = (UNSIGNED)(z); \
            break;                              \
        case TYPE_INT:                          \
            (*((INT *)y)) = (INT)(z);           \
            break;                              \
        case TYPE_SIGNED:                       \
            (*((SIGNED *)y)) = (SIGNED)(z);     \
            break;                              \
        case TYPE_UNSIGNED_CHAR:                \
            (*((UNSIGNED_CHAR *)y)) = (UNSIGNED_CHAR)(z); \
            break;                              \
        case TYPE_CHAR:                         \
            (*((CHAR *)y)) = (CHAR)(z);         \
            break;                              \
        default:                                \
            (*((float *)y)) = (float)(z);       \
            break;                              \
    }                                           \
  }

/* 链接节点定义 */
typedef struct TB_NODE_STRUCT
{
    struct TB_NODE_STRUCT
        * Previous; /* previous node */
    struct TB_NODE_STRUCT
        * Next; /* next node */
}TB_NODE;

/* 数据类型结构定义 */
typedef struct TB_TYPE_STRUCT
{
    /* 索引 */
    INT Index;
    /* 名称 */
    CHAR Name[TYPE_NAME];
    /* 数据大小 */
    INT Size;
    /* 字节数 */
    INT Number;
}TB_TYPE;

/* 字段控制块结构定义 */
typedef struct TB_FCB_STRUCT
{
    /* 链接节点 */
    TB_NODE Link;
    /* 索引 */
    INT Index;
    /* 数据属性(1:可配置,2:可整定) */
    INT Attribute;
    /* 数据类型 */
    INT Type;
    /* 数据类型的字节数 */
    INT TypeByte;
    /* 数据转换宽度 */
    INT Width;
    /* 数据类型大小 */
    INT TypeSize;
    /* 字段长度 */
    INT Size;
    /* 字段数据元素个数 */
    INT MemberNumber;
    /* 位置偏移 */
    INT Offset;
    /* 名称 */
    CHAR Name[FIELD_NAME];
    /* 字段预处理函数 */
    INT (* preTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, VOID * valuePtr, INT Originate );
    /* 字段前处理函数 */
    INT (* foreTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, VOID * valuePtr );
    /* 字段后处理函数 */
    INT (* afterTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, INT Originate );
    /* 字段结束处理函数 */
    INT (* endTreatment )( INT TabIndex, INT FieldIndex );
}TB_FCB;

/* 数据表控制块结构定义 */
typedef struct TB_TCB_STRUCT
{
    /* 链接节点 */
    TB_NODE Link;
    /* 名称 */
    CHAR Name[TABLE_NAME];
    /* 索引 */
    INT Index;
    /* 最大的数据个数 */
    INT MaxDataNumber;
    /* 创建的数据个数 */
    INT DataNumber;
    /* 数据表的类型 */
    INT Type;
    /* 数据内存指针 */
    VOID * DataMemory;
    /* 状态(1:引导整定值出错,2:引导配置信息出错) */
    INT Status;
    /* 编辑标志(1:选择编辑整定值,2:编辑整定值,3: 保存整定值)*/
    INT EditFlag;
    /* 编辑整定值的字段索引号 */
    INT EditIndex;
    /* 编辑缓冲区 */
    VOID * EditMemory;
    /* 编辑字段长度 */
    INT EditFieldSize;
    /* 整定值存储起始位置偏移 */
    INT SetOffset;
    /* 表长度(包括数据对齐) */
    INT Size;
    /* 字段个数 */
    INT FieldNumber;
    /* 字段链表指针 */
    TB_FCB * FieldLink;
    /* 整定值字段个数 */
    INT SetFieldNumber;
    /* CRC码 */
    UNSIGNED CrcCode;
    /* 数据保护句柄 */
    VOID * ProtectHandle;
    /* 冻结数据标志 */
    INT FreezeData;
    /* 冻结数据计数器 */
    UNSIGNED FreezeCounter;
    /* 边界检查标志(0:不检查边界,1:检查边界) */
    INT BoundaryCheckFlag;
    /* 计算类型字段(仅检查边界时有效) */
    TB_FCB * TypeField;
    /* 计算系数字段(仅检查边界时有效) */
    TB_FCB * CoefField;
    /* 最小值字段(仅检查边界时有效) */
    TB_FCB * MinField;
    /* 最大值字段(仅检查边界时有效) */
    TB_FCB * MaxField;
    /* 配置数据存储的起始位置偏移 */
    INT CfgOffset;
    /* 配置数据的尺寸 */
    INT CfgSize;
    /* 引导整定值数据前处理函数 */
    INT (* foreLeadTreatment )( INT TabIndex );
    /* 引导整定值数据后处理函数 */
    INT (* afterLeadTreatment )( INT TabIndex );
    /* 修改计数器 */
    UNSIGNED SetCounter;
    /* 非易失整定值CRC码初始化标志 */
    UNSIGNED NVData_CRC_IniFlag;
    /* 非易失整定值CRC码修改计数器值 */
    UNSIGNED NVData_CRC_Counter;
    /* 非易失整定值CRC码 */
    UNSIGNED NVData_CRCCode;    
}TB_TCB;

/*动态内存管理操作接口数据结构和操作*/
typedef void * (* TB_MemAlloc )( size_t );
typedef void (* TB_MemFree )( void *);
struct TBMemMService
{
    /* 申请内存 */
    TB_MemAlloc MemAlloc;
    /* 释放内存 */
    TB_MemFree MemFree;
};

/* 数据表类结构 */
typedef struct TABLE_Class_struct CTABLE;
struct TABLE_Class_struct
{
    /*private: */
    /* 初始化标志 */
    INT InitFlag;
    /* 最大的数据表个数 */
    INT MaxTabNumber;
    /* 创建的数据表个数 */
    INT TabNumber;
    /* 数据表内存指针 */
    TB_TCB * TabMemory;
    /* 最大的字段个数 */
    INT MaxFieldNumber;
    /* 创建的字段个数 */
    INT FieldNumber;
    /* 字段内存指针 */
    TB_FCB * FieldMemory;
    /* 总的整定值个数(占用资源) */
    INT TotalSGSize;
    /* 配置数据的ID号（配置数据读写） */
    UNSIGNED ConfigId;
    /* 非易失数据的ID号（非易失数据读写） */
    UNSIGNED NVDataId;
    /* 表初始数据的CRC码 */
    UNSIGNED DataCrcCode;
    /* 表配置数据的CRC码 */
    UNSIGNED ConfigCrcCode;
    /* 表字段信息的CRC码 */
    UNSIGNED FieldCrcCode;
    /* 总的配置数据尺寸 */
    INT TotalCfgSize;
    /* 数据表状态 */
    INT Status;
    /* 不够的数据表整定值空间大小 */
    INT NoSGMemory;
    /* 不够的配置数据空间大小 */
    INT NoCfgMemory;
    /* 不够的内存空间 */
    INT NoRamMemory;

    /*public: */
    /* 定义结构表属性 */
    INT (* DefTable )( CHAR * Name, INT Index, INT MaxNumber, INT Size, INT Type, 
        INT EditFieldSize );
    /* 定义字段属性 */
    INT (* DefField )( INT TableIndex, CHAR * Name, INT Index, INT Attribute,
        INT DataType, INT Number, INT Offset );
    /* 创建全部字段 */
    VOID * (* CreateAllField )( INT TableIndex, VOID * Buffer );
    /* 读单个字段值 */
    INT (* GetOneFieldValue )( INT TableIndex, INT Offset, INT Number,
        TB_CFB * Handle, VOID * Buffer, INT * ActualNumber );
    /* 读多个字段值 */
    INT (* GetBatchFieldValue )( INT TableIndex, INT Offset, INT Number,
        TB_CHB * Handle, VOID * Buffer, INT * ActualNumber );
    /* 读全部字段值 */
    INT (* GetAllFieldValue )( INT TableIndex, INT Offset, INT Number,
        TB_CFB * Handle, VOID * Buffer, INT * ActualNumber );
    /* 定义整定值边界属性 */
    INT (* DefSGRange )( INT TableIndex, INT TypeIndex, INT CoefIndex,
        INT MinIndex, INT MaxIndex );
    /* 设置配置字段值 */
    INT (* SetConfigValues )( INT TableIndex, INT Offset, INT Number,
        TB_CFB * Handle, VOID * buffer, INT Originate );
    /* 选择编辑定值组 */
    INT (* SelectEditSG )( INT TableIndex, INT FieldIndex );
    /* 设置编辑定值组值 */
    INT (* SetEditSGValues )( INT TableIndex, INT Offset, INT Number,
        TB_CFB * Handle, VOID * buffer );
    /* 确认编辑定值组值 */
    INT (* ConfirmEditSGValue )( INT TableIndex, INT Type, UNSIGNED Offset,
        TB_CFB * Handle, UNSIGNED TimeOut, INT ( _AckCall )( INT, UNSIGNED_CHAR *),
        UNSIGNED_CHAR * Dst );
    /* 初始化数据表环境变量 */
    INT (* TableInitialize )( INT MaxTabNumber, INT MaxFieldNumber );
    /* 定义数据保护句柄 */
    INT (* DefProtectHandle )( INT TableIndex, VOID * pProtectHandle );
    /* 冻结数据表的数据 */
    INT (* FreezeTableData )( INT TableIndex );
    /* 解冻数据表的数据 */
    INT (* UnFreezeTableData )( INT TableIndex );
    /* 获取数据表的冻结状态 */
    INT (* GetFreezeStatus )( INT TableIndex );
    /* 计算表初始数据的CRC码 */
    INT (* CalTabDataCRC )( VOID );
    /* 计算表配置数据的CRC码 */
    INT (* CalTabConfigCRC )( VOID );
    /* 计算表字段信息的CRC码 */
    INT (* CalTabFieldInfoCRC )( VOID );
    /* 保存配置字段值 */
    INT (* SaveConfigValue )( HTAB * pTabList, UNSIGNED TimeOut );
    /* 引导配置字段值 */
    INT (* LeadConfigValue )( UNSIGNED TimeOut );
    /* 保存表的配置字段值 */
    INT (* SaveTabConfigValue )( INT TableIndex, UNSIGNED TimeOut );
    /* 保存整定值 */
    INT (* SaveSGValue )( UNSIGNED TimeOut );
    /* 引导整定值 */
    INT (* LeadSGValue )( UNSIGNED TimeOut );
    /* 数据表基本数据初始化 */
    INT (* SetDataInitialize )( INT iType, UNSIGNED uTimeOut );
    /* 固化非易失整定数据 */
    INT (* WriteNVData )( VOID * Buffer, UNSIGNED uOffset, UNSIGNED uSize, 
        UNSIGNED uTimeOut );
    /* 读取非易失整定数据 */
    INT (* ReadNVData )( VOID * Buffer, UNSIGNED uOffset, UNSIGNED uSize, 
        UNSIGNED uTimeOut );
    /* 修复非易失整定数据 */
    INT (* RepairNVData )( UNSIGNED uTimeOut ); 
    /* 固化配置信息 */
    INT (* WriteConfigData )( VOID * Buffer, UNSIGNED uOffset, UNSIGNED uSize, 
        UNSIGNED uTimeOut );
    /* 读取配置信息 */
    INT (* ReadConfigData )( VOID * Buffer, UNSIGNED uOffset, UNSIGNED uSize, 
        UNSIGNED uTimeOut );
    /* 修复配置数据 */
    INT (* RepairConfigData )( UNSIGNED uTimeOut );
};

//extern CTABLE CTable;
/* 宏定义 */
#define GETMAXTABLENUMBER() CTable.MaxTabNumber    /* 获取最大数据表个数 */
#define GETMAXFIELDNUMBER() CTable.MaxFieldNumber  /* 获取最大字段个数 */

TB_TCB * CTBC_FindTable( INT TableIndex );
TB_FCB * CTBC_FindField( TB_TCB * pTable, INT FieldIndex );
INT CTBC_FieldCondCheck( VOID * Memory, INT CondNumber, INT * CondValue,
    TB_FCB ** CondField, INT * CondType );
TB_FCB * CTBC_GetFieldOffset( TB_TCB * pTable, INT FieldIndex, INT Attribute,
    UNSIGNED * Actual_Size );
INT CTBC_SetFieldValue( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * buffer, INT Originate );
INT CTBC_PreFieldValue( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * buffer, INT Originate );
 

#endif
