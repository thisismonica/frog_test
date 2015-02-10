#ifndef IED_DB_DEFS_H
#define IED_DB_DEFS_H

#include "plat/net_inc/ieddb.h"
#include "plat_specific/inc/cpc_defs.h"

#define IED_SOE_DATA_OFFSET     2                       /* ��SOE��������������ƫ��λ�� */
#define IED_FCK_SOE_MAX_COUNT   CPC_FCK_SOE_MAX_COUNT   /* ֧�ֵ������SOE�ĸ��� */
#define IED_FCK_SOE_SIZE       (sizeof( GW_SOE )) /* ���SOE��size */
/* ���SOE�Ĺ�����������ݳ���(����UNSIGNED���� + ���ȱ��� + ����ʵ�ʳ���) */
#define IED_FCK_SOE_BUFFER_LENGTH  (((IED_FCK_SOE_SIZE+sizeof(UNSIGNED)-1)/sizeof(UNSIGNED))+IED_SOE_DATA_OFFSET)

#define IED_COMMAND_DATA_OFFSET      2                           /* �ڲ�ض������������������ƫ��λ�� */
#define IED_READ_COMMAND_MAX_COUNT  CPC_READ_COMMAND_MAX_COUNT   /* ֧�ֵ�����ض�����ĸ��� */
#define IED_READ_COMMAND_SIZE  (sizeof( GW_COMMAND ))/* ��ض�����Ļ����� */
/* ��ض�����Ĺ�����������ݳ���(����UNSIGNED���� + ���ȱ��� + ����ʵ�ʳ���) */
#define IED_READ_COMMAND_BUFFER_LENGTH  (((IED_READ_COMMAND_SIZE+sizeof(UNSIGNED)-1)/sizeof(UNSIGNED))+IED_COMMAND_DATA_OFFSET)

#define IED_FCK_SOE_MAX_HANDLE          2   /* ���SOE������������ */
#define IED_FCK_COMMAND_MAX_HANDLE      2   /* ��ض����������������� */
#define IED_SOE_PROIRITY_BUFF_SIZE      32  /* ���ȼ���������С */
#define IED_COMMAND_PROIRITY_BUFF_SIZE  32  /* ���ȼ���������С */

#endif
