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

#define MAX_FLOAT_ERROR         0.0001 /* �������������(��������ٷֱ�) */
#define MAX_TABLE_NUMBER        50  /* ������ݱ���� */
#define MAX_FIELD_NUMBER        1500 /* ����ֶθ��� */
#define TABLE_CRC_INITIALIZE    0xffffffff /* ������CRC��ĳ�ֵ */
#define TABLE_CRC_BITS          32  /* ������CRC���λ�� */
#define TABLE_CRC_MASKS         0xffffffff /* ������CRC������� */
#define FREEZE_TABLE_DATA       1 /* �������ݱ����� */

/* �������Ͷ��� */
#define CHL_FOURIER1_           0   /*fourier algorthem */
#define CHL_MUL_                1   /*linearity algorthem */
#define CHL_VECTOR_             2   /*vector algorthem */
#define CHL_FREQ_               3   /*frequency algorthem */
#define CHL_INT_                4   /*integer algorthem */
#define CHL_ANG_                5   /*angle algorthem */
#define CHL_CTRL_               6   /*control word algorthem */
#define CHL_DISLOG_             7   /*logical display*/
#define CHL_RATA_               8   /* �ֵ���� */
#define CHL_INT_CTRL_           9   /* ���Ϳ���������(0/1) */
#define CHL_ENUM_               10  /*enumerate type */

/* �ֶ����Զ��� */
#define FIELD_WRITE_ENABLE (FIELD_CONFIG_ENABLE+FIELD_SETTING_ENABLE+FIELD_VALUE_ENABLE) /* �����ֶ�д */

/* ����ֵ״̬ */
#define SETTING_STATUS_SELECT   1   /* ѡ��༭����ֵ */
#define SETTING_STATUS_EDIT     2   /* �༭����ֵ */
#define SETTING_STATUS_SAVE     3   /* ��������ֵ */

/* �߽���ѡ�� */
#define CHECK_ENABLE            1   /* check enable */
#define CHECK_DISABLE           0   /* check disable */

/* ������Ϣ���� */
#define TABLE_ERR_LEAD_SG       (1 << 0)    /* ��������ֵ���� */
#define TABLE_ERR_LEAD_CFG      (1 << 1)    /* ����������Ϣ���� */
#define TABLE_ERR_SG_MEM        (1 << 2)    /* ����ֵ�洢�ռ䲻�� */
#define TABLE_ERR_CFG_MEM       (1 << 3)    /* �������ݴ洢�ռ䲻�� */
#define TABLE_ERR_RAM_MEM       (1 << 4)    /* �ڴ�ռ䲻�� */
#define TABLE_ERR_LEAD_SG_CRC   (1 << 5)    /* ��������ֵCRC����� */
#define TABLE_ERR_LEAD_SG_READ  (1 << 6)    /* ��������ֵ�����ݳ��� */
#define TABLE_ERR_LEAD_SG_CHECK (1 << 7)    /* ��������ֵ���ݼ����� */

/* ��ȡ�ṹԪ�ص�λ��ƫ��,x:�ṹ����,y:��Ա���� */
#define GETSTRUCTOFFSET(x,y)                    \
    (int)(&((##x*)0)->##y)

/* ��ȡ�����������͵�ֵ,x:�ֶ�����,y:���ݵ�ַ,z:��ȡֵ,w:���� */
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

/* ���û����������͵�ֵ,x:����,y:���ݵ�ַ,z:����ֵ */
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

/* ���ӽڵ㶨�� */
typedef struct TB_NODE_STRUCT
{
    struct TB_NODE_STRUCT
        * Previous; /* previous node */
    struct TB_NODE_STRUCT
        * Next; /* next node */
}TB_NODE;

/* �������ͽṹ���� */
typedef struct TB_TYPE_STRUCT
{
    /* ���� */
    INT Index;
    /* ���� */
    CHAR Name[TYPE_NAME];
    /* ���ݴ�С */
    INT Size;
    /* �ֽ��� */
    INT Number;
}TB_TYPE;

/* �ֶο��ƿ�ṹ���� */
typedef struct TB_FCB_STRUCT
{
    /* ���ӽڵ� */
    TB_NODE Link;
    /* ���� */
    INT Index;
    /* ��������(1:������,2:������) */
    INT Attribute;
    /* �������� */
    INT Type;
    /* �������͵��ֽ��� */
    INT TypeByte;
    /* ����ת����� */
    INT Width;
    /* �������ʹ�С */
    INT TypeSize;
    /* �ֶγ��� */
    INT Size;
    /* �ֶ�����Ԫ�ظ��� */
    INT MemberNumber;
    /* λ��ƫ�� */
    INT Offset;
    /* ���� */
    CHAR Name[FIELD_NAME];
    /* �ֶ�Ԥ������ */
    INT (* preTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, VOID * valuePtr, INT Originate );
    /* �ֶ�ǰ������ */
    INT (* foreTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, VOID * valuePtr );
    /* �ֶκ����� */
    INT (* afterTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, INT Originate );
    /* �ֶν��������� */
    INT (* endTreatment )( INT TabIndex, INT FieldIndex );
}TB_FCB;

/* ���ݱ���ƿ�ṹ���� */
typedef struct TB_TCB_STRUCT
{
    /* ���ӽڵ� */
    TB_NODE Link;
    /* ���� */
    CHAR Name[TABLE_NAME];
    /* ���� */
    INT Index;
    /* �������ݸ��� */
    INT MaxDataNumber;
    /* ���������ݸ��� */
    INT DataNumber;
    /* ���ݱ������ */
    INT Type;
    /* �����ڴ�ָ�� */
    VOID * DataMemory;
    /* ״̬(1:��������ֵ����,2:����������Ϣ����) */
    INT Status;
    /* �༭��־(1:ѡ��༭����ֵ,2:�༭����ֵ,3: ��������ֵ)*/
    INT EditFlag;
    /* �༭����ֵ���ֶ������� */
    INT EditIndex;
    /* �༭������ */
    VOID * EditMemory;
    /* �༭�ֶγ��� */
    INT EditFieldSize;
    /* ����ֵ�洢��ʼλ��ƫ�� */
    INT SetOffset;
    /* ����(�������ݶ���) */
    INT Size;
    /* �ֶθ��� */
    INT FieldNumber;
    /* �ֶ�����ָ�� */
    TB_FCB * FieldLink;
    /* ����ֵ�ֶθ��� */
    INT SetFieldNumber;
    /* CRC�� */
    UNSIGNED CrcCode;
    /* ���ݱ������ */
    VOID * ProtectHandle;
    /* �������ݱ�־ */
    INT FreezeData;
    /* �������ݼ����� */
    UNSIGNED FreezeCounter;
    /* �߽����־(0:�����߽�,1:���߽�) */
    INT BoundaryCheckFlag;
    /* ���������ֶ�(�����߽�ʱ��Ч) */
    TB_FCB * TypeField;
    /* ����ϵ���ֶ�(�����߽�ʱ��Ч) */
    TB_FCB * CoefField;
    /* ��Сֵ�ֶ�(�����߽�ʱ��Ч) */
    TB_FCB * MinField;
    /* ���ֵ�ֶ�(�����߽�ʱ��Ч) */
    TB_FCB * MaxField;
    /* �������ݴ洢����ʼλ��ƫ�� */
    INT CfgOffset;
    /* �������ݵĳߴ� */
    INT CfgSize;
    /* ��������ֵ����ǰ������ */
    INT (* foreLeadTreatment )( INT TabIndex );
    /* ��������ֵ���ݺ����� */
    INT (* afterLeadTreatment )( INT TabIndex );
    /* �޸ļ����� */
    UNSIGNED SetCounter;
    /* ����ʧ����ֵCRC���ʼ����־ */
    UNSIGNED NVData_CRC_IniFlag;
    /* ����ʧ����ֵCRC���޸ļ�����ֵ */
    UNSIGNED NVData_CRC_Counter;
    /* ����ʧ����ֵCRC�� */
    UNSIGNED NVData_CRCCode;    
}TB_TCB;

/*��̬�ڴ��������ӿ����ݽṹ�Ͳ���*/
typedef void * (* TB_MemAlloc )( size_t );
typedef void (* TB_MemFree )( void *);
struct TBMemMService
{
    /* �����ڴ� */
    TB_MemAlloc MemAlloc;
    /* �ͷ��ڴ� */
    TB_MemFree MemFree;
};

/* ���ݱ���ṹ */
typedef struct TABLE_Class_struct CTABLE;
struct TABLE_Class_struct
{
    /*private: */
    /* ��ʼ����־ */
    INT InitFlag;
    /* �������ݱ���� */
    INT MaxTabNumber;
    /* ���������ݱ���� */
    INT TabNumber;
    /* ���ݱ��ڴ�ָ�� */
    TB_TCB * TabMemory;
    /* �����ֶθ��� */
    INT MaxFieldNumber;
    /* �������ֶθ��� */
    INT FieldNumber;
    /* �ֶ��ڴ�ָ�� */
    TB_FCB * FieldMemory;
    /* �ܵ�����ֵ����(ռ����Դ) */
    INT TotalSGSize;
    /* �������ݵ�ID�ţ��������ݶ�д�� */
    UNSIGNED ConfigId;
    /* ����ʧ���ݵ�ID�ţ�����ʧ���ݶ�д�� */
    UNSIGNED NVDataId;
    /* ���ʼ���ݵ�CRC�� */
    UNSIGNED DataCrcCode;
    /* ���������ݵ�CRC�� */
    UNSIGNED ConfigCrcCode;
    /* ���ֶ���Ϣ��CRC�� */
    UNSIGNED FieldCrcCode;
    /* �ܵ��������ݳߴ� */
    INT TotalCfgSize;
    /* ���ݱ�״̬ */
    INT Status;
    /* ���������ݱ�����ֵ�ռ��С */
    INT NoSGMemory;
    /* �������������ݿռ��С */
    INT NoCfgMemory;
    /* �������ڴ�ռ� */
    INT NoRamMemory;

    /*public: */
    /* ����ṹ������ */
    INT (* DefTable )( CHAR * Name, INT Index, INT MaxNumber, INT Size, INT Type, 
        INT EditFieldSize );
    /* �����ֶ����� */
    INT (* DefField )( INT TableIndex, CHAR * Name, INT Index, INT Attribute,
        INT DataType, INT Number, INT Offset );
    /* ����ȫ���ֶ� */
    VOID * (* CreateAllField )( INT TableIndex, VOID * Buffer );
    /* �������ֶ�ֵ */
    INT (* GetOneFieldValue )( INT TableIndex, INT Offset, INT Number,
        TB_CFB * Handle, VOID * Buffer, INT * ActualNumber );
    /* ������ֶ�ֵ */
    INT (* GetBatchFieldValue )( INT TableIndex, INT Offset, INT Number,
        TB_CHB * Handle, VOID * Buffer, INT * ActualNumber );
    /* ��ȫ���ֶ�ֵ */
    INT (* GetAllFieldValue )( INT TableIndex, INT Offset, INT Number,
        TB_CFB * Handle, VOID * Buffer, INT * ActualNumber );
    /* ��������ֵ�߽����� */
    INT (* DefSGRange )( INT TableIndex, INT TypeIndex, INT CoefIndex,
        INT MinIndex, INT MaxIndex );
    /* ���������ֶ�ֵ */
    INT (* SetConfigValues )( INT TableIndex, INT Offset, INT Number,
        TB_CFB * Handle, VOID * buffer, INT Originate );
    /* ѡ��༭��ֵ�� */
    INT (* SelectEditSG )( INT TableIndex, INT FieldIndex );
    /* ���ñ༭��ֵ��ֵ */
    INT (* SetEditSGValues )( INT TableIndex, INT Offset, INT Number,
        TB_CFB * Handle, VOID * buffer );
    /* ȷ�ϱ༭��ֵ��ֵ */
    INT (* ConfirmEditSGValue )( INT TableIndex, INT Type, UNSIGNED Offset,
        TB_CFB * Handle, UNSIGNED TimeOut, INT ( _AckCall )( INT, UNSIGNED_CHAR *),
        UNSIGNED_CHAR * Dst );
    /* ��ʼ�����ݱ������� */
    INT (* TableInitialize )( INT MaxTabNumber, INT MaxFieldNumber );
    /* �������ݱ������ */
    INT (* DefProtectHandle )( INT TableIndex, VOID * pProtectHandle );
    /* �������ݱ������ */
    INT (* FreezeTableData )( INT TableIndex );
    /* �ⶳ���ݱ������ */
    INT (* UnFreezeTableData )( INT TableIndex );
    /* ��ȡ���ݱ�Ķ���״̬ */
    INT (* GetFreezeStatus )( INT TableIndex );
    /* ������ʼ���ݵ�CRC�� */
    INT (* CalTabDataCRC )( VOID );
    /* ������������ݵ�CRC�� */
    INT (* CalTabConfigCRC )( VOID );
    /* ������ֶ���Ϣ��CRC�� */
    INT (* CalTabFieldInfoCRC )( VOID );
    /* ���������ֶ�ֵ */
    INT (* SaveConfigValue )( HTAB * pTabList, UNSIGNED TimeOut );
    /* ���������ֶ�ֵ */
    INT (* LeadConfigValue )( UNSIGNED TimeOut );
    /* �����������ֶ�ֵ */
    INT (* SaveTabConfigValue )( INT TableIndex, UNSIGNED TimeOut );
    /* ��������ֵ */
    INT (* SaveSGValue )( UNSIGNED TimeOut );
    /* ��������ֵ */
    INT (* LeadSGValue )( UNSIGNED TimeOut );
    /* ���ݱ�������ݳ�ʼ�� */
    INT (* SetDataInitialize )( INT iType, UNSIGNED uTimeOut );
    /* �̻�����ʧ�������� */
    INT (* WriteNVData )( VOID * Buffer, UNSIGNED uOffset, UNSIGNED uSize, 
        UNSIGNED uTimeOut );
    /* ��ȡ����ʧ�������� */
    INT (* ReadNVData )( VOID * Buffer, UNSIGNED uOffset, UNSIGNED uSize, 
        UNSIGNED uTimeOut );
    /* �޸�����ʧ�������� */
    INT (* RepairNVData )( UNSIGNED uTimeOut ); 
    /* �̻�������Ϣ */
    INT (* WriteConfigData )( VOID * Buffer, UNSIGNED uOffset, UNSIGNED uSize, 
        UNSIGNED uTimeOut );
    /* ��ȡ������Ϣ */
    INT (* ReadConfigData )( VOID * Buffer, UNSIGNED uOffset, UNSIGNED uSize, 
        UNSIGNED uTimeOut );
    /* �޸��������� */
    INT (* RepairConfigData )( UNSIGNED uTimeOut );
};

//extern CTABLE CTable;
/* �궨�� */
#define GETMAXTABLENUMBER() CTable.MaxTabNumber    /* ��ȡ������ݱ���� */
#define GETMAXFIELDNUMBER() CTable.MaxFieldNumber  /* ��ȡ����ֶθ��� */

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
