#include "plat/inc/xj_cosc.h"
#include "plat/net_inc/ieddb.h"

#ifndef IED_DB_EXTR_H
#define IED_DB_EXTR_H

/* д���SOE���ݺ��� */
INT iedDB_WriteSOE( VOID * pBuffer, UNSIGNED timeout );
/* �����SOE���ݺ��� */
INT iedDB_ReadSOE( VOID * pBuffer, UNSIGNED timeout );
/* �洢�û������ */
INT iedDB_WriteCommand( VOID * pBuffer, UNSIGNED timeout );
/* ��ȡ�û������ */
INT iedDB_ReadCommand( VOID * pBuffer, UNSIGNED timeout );
/* ���ȼ����ʼ������ */
VOID iedDB_ProirityInit( void );
#endif
