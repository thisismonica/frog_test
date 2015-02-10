/**************************************************************
** Copyright (C) 2002-2007 ��������������
** All right reserved
***************************************************************
**
** ��Ŀ���ƣ�NPI801
** ���ܼ�飺ȫ�����ô���ģ��
**
** ԭʼ���ߣ��ܾ���
** ��֯���ţ���̼��� ��������������
** ��    ע��
** ����ʱ�䣺2007-05-25
** ���ʱ�䣺2007-06-20
** ��    ����1.0
***************************************************************
** ��Ԫ��ʶ��$Id: pxml_cfg.h,v 1.3 2012/03/29 01:20:42 jyj Exp $
** �汾��ʷ��
** �� �� �ߣ�
** ����޸ģ�
**************************************************************/

#ifndef  PXML_CFG_INC
#define  PXML_CFG_INC

/* ��ʹ�õı���ͨ���� */
#define UNUSED_PRO_ID                  0xFFFF

#define PXML_CFG_DATA_COUNT            "count"         /* ���ݸ����ڵ����� */
#define PXML_CFG_CPU_ID_NODE_NAME      "cpuid"         /* cpuid �ڵ����� */

/* SV���ݽ��� */
#define PXML_CFG_PRO_CHL_ID_NODE_NAME  "pro_chl_id"    /* pro_chl_id �ڵ����� */
#define PXML_CFG_ABS_CHL_ID_NODE_NAME  "abs_chl_id"    /* abs_chl_id �ڵ����� */

/* GOOSE���ݽ��� */
#define PXML_CFG_GR_GROUP_ID_NODE_NAME    "groupid"      /* groupid �ڵ����� */
#define PXML_CFG_GR_MASK_BITS_NODE_NAME   "MaskBits"     /* MaskBits �ڵ����� */
#define PXML_CFG_GR_T_MASK_BITS_NODE_NAME "MaskBitsT"    /* MaskBitsT �ڵ����� */

/* goose����SV���ݽڵ㶨�� */
#define PXML_CFG_PRO_GSV_ID_NODE_NAME   "pro_gsv_id"   /* pro_gsv_id �ڵ����� */
#define PXML_CFG_ABS_GSV_ID_NODE_NAME   "abs_gsv_id"   /* abs_gsv_id �ڵ����� */

/* ��ʼ��ƽ̨�����ļ� */
int PXML_CFG_InitConfig( CHAR * fileName );
/* ��ȡͨ����Ϣ,����ͨ������ */
INT PXML_CFG_GetChlInfo( INT iCpuID, INT * pProChlId, INT * pAbsChlId, INT iMaxCount );
/* ��ȡ�������ݵ�CPU��Ϣ;����ֵΪCPU���� */
INT PXML_CFG_GetCpuInfo( INT * pCpuId, INT iMaxCount );
/* ��ȡgoose����������Ϣ */
INT PXML_CFG_GetGooseRecvInfo( INT GroupNo, UNSIGNED  * pMaskBits );
/* ��ȡGOOSE���������ʼ��״̬ */
INT PXML_CFG_GetGRecvInitStatus( VOID );
/* ��ȡ��ʱ���goose����������Ϣ */
INT PXML_CFG_GetTimeGooseRecvInfo( INT GroupNo, UNSIGNED  * pMaskBitsT );

#endif
