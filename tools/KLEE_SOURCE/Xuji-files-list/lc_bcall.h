/* ͨ�������Ҫ��ʱ��ִ�еĺ�������ڵ�ַ�Ͳ���ͨ�����д��ݸ����洦������,
���洦������ִ�к�,��ִ�н��ͨ�����д��ݸ�ͨ������,ͨѶ������ִ�н��*/

#include "plat/net_inc/lc_defs.h"
#include "plat/net_inc/gwmem.h"
#include "plat/net_inc/lc_defs.h"

/* Check to see if the file has been included already.  */
#ifndef LC_BCALL_H
#define LC_BCALL_H

#define LCC_BACKDATA_MAXLENGTH   LC_MAX_FRAME_DATA_SIZE   /* ͨѶ������󳤶� */
#define LCC_MAXCOMM_TYPE         0xFF   /* ��̨���ͨѶ�������� */
#define LCC_QUE_SEND_DELAY       200    /* ��Ϣ���з�����ʱʱ�� */

/* �ص��ṹ */
typedef struct STRUCT_LC_BLOCK_CB
{
    GWAPP_ACK_FUNCTION  pCallback;
    VOID                * pSevData;
    VOID                * pSendData;
}LC_BLOCK_CB;

/* ע���̨�ص������������� */
extern LC_BLOCK_CB LC_BCallFunBuff[LCC_MAXCOMM_TYPE];

/* ע���ִ̨�еľ͵����� */
INT LCC_RegcBackGroundComm( GW_CHANNEL * pChl, UNSIGNED_CHAR iCommType, VOID * CallBack );
/* ͨѶ�����й���ע���̨��ͨѶ����Ĵ����� */
INT LC_BCallManage( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
/* ��̨����ִ�лص����� */
VOID  LC_Block( VOID );
/* ͨѶ�����м���Ƿ��лص�����ִ����,�Ƿ���Ҫ�ظ����� */
INT LC_BCall_Ret( GW_CHANNEL * pChl, UNSIGNED_CHAR * dst,
    UNSIGNED_CHAR * dst_port );
/* ��ִ������Ĺ�����,���ִ��ʧ��������ע�ᵽ��̨���������ȥִ�� */
INT LCC_Run_RegcBackGroundComm( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code,
    UNSIGNED_CHAR iCommType, VOID * CallBack );
/* ����̨����ִ�е�����,������ִ�н�����,������ֱ��������Ϣ������;
����Ϊchar�ĸ���,�����������ݴ洢Ϊ�׵�ַΪ������,�ڶ�������Ϊ���ݳ���(�����������򼰳��ȱ���) */
INT  LCC_BCall_BackTaskSendMessData( INT iNum, UNSIGNED_CHAR * pBuffer );
/* ��̨�������ͨѶ������Ϣ���г�ʼ������ */
INT LCC_BackTaskBCallBufferInit( VOID );
/* ����̨�������Ļ��������Ƿ��б��� */
INT LC_BCallCheckBackTaskMess( UNSIGNED_CHAR * dst, UNSIGNED_CHAR * dst_port );
#endif
