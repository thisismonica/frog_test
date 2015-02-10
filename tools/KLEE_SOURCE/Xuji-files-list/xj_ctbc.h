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
#define TYPE_NAME               20  /* �����������Ƴ��� */
#define TABLE_NAME              12  /* �����Ƴ��� */
#define FIELD_NAME              12  /* �ֶ����Ƴ��� */
#define MAX_CONDITION           3   /* �����ֶε���������������� */
#define MAX_HANDLE              4   /* �������ֶθ��� */
/* �ֶ����Զ��� */
#define FIELD_CONFIG_ENABLE     1   /* �ֶ�����ѡ�� */
#define FIELD_SETTING_ENABLE    2   /* �ֶ�����ѡ�� */
#define FIELD_CRC_ENABLE        4   /* �ֶ�CRCѡ�� */
#define FIELD_VALUE_ENABLE      8   /* �ֶ���ֵ(��������ֵ)ѡ�� */
#define FIELD_CHECK_ENABLE      0x10 /* �ֶα߽���ѡ�� */
/* �������Ͷ��� */
#define TYPE_CHAR               1   /* char */
#define TYPE_UNSIGNED_CHAR      2   /* unsigned char */
#define TYPE_INT                3   /* int */
#define TYPE_SIGNED             4   /* long */
#define TYPE_UNSIGNED           5   /* unsigned long */
#define TYPE_FLOAT              6   /* float */
/* �޸�����ֵѡ�� */
#define SETTING_ALL_ENABLE      1   /* ȫ������ֵ */
#define SETTING_ONE_ENABLE      2   /* ��������ֵ */
/* �������� */
#define COND_TYPE_AND           1   /* ����� */
#define COND_TYPE_OR            2   /* ����� */
#define COND_TYPE_EQU           3   /* ���ڲ��� */
/* ���ݱ����� */
#define TABLE_TYPE_RELATION     (1 << 0) /* ˫CPU���ݱ����ݹ��� */

/* �����ֶβ�����ṹ���� */
typedef struct TB_CFB_STRUCT
{
    /* �����ֶ����� */
    INT HandleIndex;
    /* �����ֶθ��� */
    INT CondNumber;
    /* �����ֶ����� */
    INT CondIndex[MAX_CONDITION];
    /* �����ֶ�ֵ */
    INT CondValue[MAX_CONDITION];
    /* �������� */
    INT CondType[MAX_CONDITION];
}TB_CFB;

/* �����ֶβ�����ṹ���� */
typedef struct TB_CHB_STRUCT
{
    /* �����ֶθ��� */
    INT HandleNumber;
    /* �����ֶ����� */
    INT HandleIndex[MAX_HANDLE];
    /* �����ֶθ��� */
    INT CondNumber;
    /* �����ֶ����� */
    INT CondIndex[MAX_CONDITION];
    /* �����ֶ�ֵ */
    INT CondValue[MAX_CONDITION];
    /* �������� */
    INT CondType[MAX_CONDITION];
}TB_CHB;

typedef long HTAB;                /* Ӧ�ó�������� */

#endif

/*=======================================================================*/
/* ������                                                              */
/*=======================================================================*/
/* �������� */

/*=======================================================================*/
/* ���ݱ����                                                            */
/*=======================================================================*/
/* �������ݱ� */
INT CTBC_DefTable( CHAR * Name, INT Index, INT MaxNumber, INT Size, INT Type, 
    INT EditFieldSize );
/* �����ֶ� */
INT CTBC_DefField( INT TableIndex, CHAR * Name, INT Index, INT Attribute,
    INT DataType, INT Number, INT Offset );
/* ���ɱ����� */
VOID * CTBC_CreateAllField( INT TableIndex, VOID * Buffer );
/* ��ȡ�����ֶε�ֵ */
INT CTBC_GetOneFieldValue( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * Buffer, INT * ActualNumber );
/* ������ֶ�ֵ */
INT CTBC_GetBatchFieldValue( INT TableIndex, INT Offset, INT Number,
    TB_CHB * Handle, VOID * Buffer, INT * ActualNumber );
/* ��ȡȫ���ֶε�ֵ */
INT CTBC_GetAllFieldValue( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * Buffer, INT * ActualNumber );
/* ��������ֵ��Χ */
INT CTBC_DefSGRange( INT TableIndex, INT TypeIndex, INT CoefIndex,
    INT MinIndex, INT MaxIndex );
/* ���������ֶ�ֵ */
INT CTBC_SetConfigValues( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * buffer, INT Originate );
/* ѡ��༭��ֵ�� */
INT CTBC_SelectEditSG( INT TableIndex, INT FieldIndex );
/* ���ö�ֵ��ֵ */
INT CTBC_SetEditSGValues( INT TableIndex, INT Offset, INT Number,
    TB_CFB * Handle, VOID * buffer );
/* ȷ�ϱ༭��ֵ��ֵ */
INT CTBC_ConfirmEditSGValue( INT TableIndex, INT Type, UNSIGNED Offset,
    TB_CFB * pHandle, UNSIGNED TimeOut, INT ( _AckCall )( INT, UNSIGNED_CHAR *),
    UNSIGNED_CHAR * Dst );
/* ��ʼ�����ݱ������� */
INT CTBC_TableInitialize( INT MaxTabNumber, INT MaxFieldNumber );
/* �������ݱ������ */
INT CTBC_DefProtectHandle( INT TableIndex, VOID * pProtectHandle );
/* �������ݱ������ */
INT CTBC_FreezeTableData( INT TableIndex );
/* �ⶳ���ݱ������ */
INT CTBC_UnFreezeTableData( INT TableIndex );
/* ��ʼ�����ݱ�����ݵĶ���״̬ */
INT CTBC_FreezeTableInitialize( INT TableIndex );
/* ��ȡ���ݱ�Ķ���״̬ */
INT CTBC_GetFreezeStatus( INT TableIndex );
/* ���������ֶ�ֵ */
INT CTBC_SaveConfigValue( HTAB * pTabList, UNSIGNED TimeOut );
/* �����������ֶ�ֵ */
INT CTBC_SaveTabConfigValue( INT TableIndex, UNSIGNED TimeOut );
/* ����ȫ�������ֶ�ֵ */
INT CTBC_LeadConfigValue( UNSIGNED TimeOut );
/* ����ȫ����ֵ��ֵ */
INT CTBC_SaveSGValue( UNSIGNED TimeOut );
/* ����ȫ����ֵ��ֵ */
INT CTBC_LeadSGValue( UNSIGNED TimeOut );
/* ���ݱ�������ݳ�ʼ�� */
INT CTBC_SetDataInitialize( INT iType, UNSIGNED uTimeOut );
/* �̻�����ʧ�������� */
INT CTBC_WriteNVData( VOID * Buffer, UNSIGNED uOffset, UNSIGNED uSize, 
    UNSIGNED uTimeOut );
/* ��ȡ����ʧ�������� */
INT CTBC_ReadNVData( VOID * Buffer, UNSIGNED uOffset, UNSIGNED uSize, 
    UNSIGNED uTimeOut );
/* �޸�����ʧ�������� */
INT CTBC_RepairNVData( UNSIGNED TimeOut );
/* �̻�������Ϣ */
INT CTBC_WriteConfigData( VOID * Buffer, UNSIGNED uOffset, 
    UNSIGNED uSize, UNSIGNED uTimeOut );
    /* ��ȡ������Ϣ */
INT CTBC_ReadConfigData( VOID * Buffer, UNSIGNED uOffset, 
    UNSIGNED uSize, UNSIGNED uTimeOut );
/* �޸��������� */
INT CTBC_RepairConfigData( UNSIGNED TimeOut );    
/* ע���ֶ�Ԥ������ */
INT CTBC_RegisterPreTreatmentFunc( INT TableIndex, INT FieldIndex,
    INT (* preTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, VOID * valuePtr, INT Originate ) );
/* ע���ֶ�ǰ������ */
INT CTBC_RegisterForeTreatmentFunc( INT TableIndex, INT FieldIndex,
    INT (* foreTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, VOID * valuePtr ) );
/* ע���ֶκ����� */
INT CTBC_RegisterAfterTreatmentFunc( INT TableIndex, INT FieldIndex,
    INT (* afterTreatment )( INT TabIndex, INT FieldIndex, VOID * dataPtr, INT Originate ) );
/* ע���ֶν��������� */
INT CTBC_RegisterEndTreatmentFunc( INT TableIndex, INT FieldIndex,
    INT (* endTreatment )( INT TabIndex, INT FieldIndex ) );    
/* ע����������ֵǰ������ */
INT CTBC_RegisterForeLeadTreatmentFunc( INT TableIndex,
    INT (* foreLeadTreatment )( INT TabIndex ) );
/* ע����������ֵ������ */
INT CTBC_RegisterAfterLeadTreatmentFunc( INT TableIndex,
    INT (* afterLeadTreatment )( INT TabIndex ) );
/* ���¶�ֵ��ֵ */
INT CTBC_UpdateSGValue( INT TableIndex, UNSIGNED TimeOut );
/* ����RAM����ֵ�ֶε�CRC�� */
INT CTBC_R_SGValueCRC( INT TableIndex, UNSIGNED * CRC_Code );
/* �������ʧ����ֵ�ֶε�CRC�� */
INT CTBC_F_SGValueCRC( INT TableIndex, UNSIGNED TimeOut, UNSIGNED * CRC_Code );
/* ����ʧ����ֵCRC���ʼ�� */
INT CTBC_SGValueCRC_Initialize( UNSIGNED TimeOut );
/* ����ֵ���ݼ�� */
INT CTBC_SGValueCheck( INT TableIndex, UNSIGNED TimeOut );
/* ��ȡ����ֵ�޸ļ�����ֵ */
INT CTBC_GetSGValueCounter( INT TableIndex, UNSIGNED * Counter );
/* ע�����ʧ����ֵ������ */
INT CTBC_RegisterNVDataTreatmentFunc( UNSIGNED SetHandle, CHAR * dirName, CHAR * fileName,
    INT (* writeNVDataTreatment )( VOID *, UNSIGNED, UNSIGNED, UNSIGNED ),
    INT (* readNVDataTreatment )( VOID *, UNSIGNED, UNSIGNED, UNSIGNED ),
    INT (* repairNVDataTreatment )( UNSIGNED ) );
/* ע��������Ϣ������ */
INT CTBC_RegisterConfigTreatmentFunc( UNSIGNED CfgHandle, CHAR * dirName, CHAR * fileName,
    INT (* writeConfigDataTreatment )( VOID *, UNSIGNED, UNSIGNED, UNSIGNED ),
    INT (* readConfigDataTreatment )( VOID *, UNSIGNED, UNSIGNED, UNSIGNED ),
    INT (* repairConfigDataTreatment )( UNSIGNED ));   
/* ��ȡָ��CPU�����ݱ�ָ�� */
UNSIGNED CTBC_GetTablePointers(  HTAB ** PointerList, UNSIGNED MaxPointers );    
/*========================================================================================*/
/*ͨѶר�ú���                                                                            */
/*========================================================================================*/
/* ��ȡ���ݱ���ܸ��� */
INT CTBC_GetTableNumber( VOID );
/* ��ȡ���ݱ��ڴ��ַ */
VOID * CTBC_GetTableMemory( INT TabIndex, INT * ActualNum );
/* ��ȡ���ݱ��С */
INT CTBC_GetTableSize( INT TabIndex );
/* ��ȡ���ݱ����Ϣ */
INT CTBC_GetTableInf( UNSIGNED Number, UNSIGNED StartNumber, UNSIGNED_CHAR * pDst, UNSIGNED * Length );
/* ��ȡ�ֶε���Ϣ */
INT CTBC_GetFieldInf( UNSIGNED Number, UNSIGNED StartNumber, UNSIGNED TabIndex, UNSIGNED_CHAR * pDst, UNSIGNED * Length, UNSIGNED * ActualNumber );
/* ��ȡ���ݱ��״̬ */
INT CTBC_GetTabStatus( INT TabIndex, UNSIGNED_CHAR * pDst );
/* ��ȡ���ñ��CRC�� */
UNSIGNED CTBC_GetConfigTabCRC( VOID );
/* ��ȡ���ݱ��ֶ���Ϣ��CRC�� */
UNSIGNED CTBC_GetTabFieldCRC( VOID );
/* ���ݱ�������Ż�ȡ�ֶ��е�ָ����������֮���ʵ��Ԫ�ظ��� */
INT CTBC_ReadDataOffset( INT TabIndex, TB_CFB * Handle, INT Offset, UNSIGNED * Actual_Number );
/* ��ȡָ�����ݱ�/ָ������/ָ����ŵ�����ģ���ַ */
VOID * CTBC_GetIndexTableMemory( INT TableIndex, TB_CFB * Handle, INT iIndex );
/* ��ȡ���ݱ�ĳ��� */
INT CTBC_GetTabLength( INT TabIndex, UNSIGNED * Length );
/* ��ȡ�ض��ֶεĳ��� */
INT CTBC_GetFieldLength( INT TabIndex, INT HandleIndex, UNSIGNED * Length );
/* ���ݱ�������Ż�ȡ�ֶ��е�ָ����������֮���ʵ��Ԫ�ظ��� */
INT CTBC_ReadDataNumber( INT TabIndex, TB_CFB * Handle, UNSIGNED * Number );
/* �ӿ���ֱ�ӵ������ݱ��޸ĺ��� */
INT CTBC_COM_HMI_ModifData( INT TableIndex, INT StartNumber, INT number,
    VOID * pCondBuf, INT iHandleNumber, VOID * pDataBuf );
#ifdef __cplusplus
}
#endif

#endif
