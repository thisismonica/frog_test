
#include "plat/inc/xj_cosc.h"
#include "plat_specific/plat_specific.h"
#include "plat/net_inc/ls_extr.h"
#include "plat/net_inc/commu.h"
#include "plat/es_plat.h"

#ifndef LSC_B6_H
#define LSC_B6_H

#define LSC_B6_MAX_EVENT           6     /* b6�������������¼����� */
#define LSC_B6_PROIRITY_BUFF_SIZE  32    /* ���ȼ���������С */

/* B6������ͻ�䱨���������Ͷ��� */
#define LSC_B6_TYPE_DYNAMIC_DATA    0  /* ͻ������,��̨ң��,��̬���� */
#define LSC_B6_TYPE_REPORT          1  /* ͻ�䱨������ */
#define LSC_B6_TYPE_EVENT           2  /* ͻ���¼����� */
#define LSC_B6_TYPE_FCK_YC          5  /* ͻ��ң���������� */
#define LSC_B6_TYPE_FAULT_RETURN    6  /* ���Ϸ������� */
#define LSC_B6_TYPE_WAVE            7  /* ¼����ŷ����仯 */
#define LSC_B6_TYPE_TABFIELD_SUDDEN 8  /* ���ݱ�ָ���ֶ�ͻ����Ϣ */

/* ȫ�ֱ��� */
extern HDCM       RamEventHandle;
extern HDCM       CommEventHandle;
extern HDCM       CommFltRetHandle;
extern NU_PROTECT LSD_Report_Protect;
extern CREPORT CReport;
/*========ʹ�ú�������========*/
/* ͻ�������¼����� */
INT LSC_B6_CommEvent( VOID * Dst );
/* ���ϱ��淢���仯,���͹��ϱ������ */
INT LSC_B6_FaultReport( VOID * Dst );
/* ͻ���¼����� */
INT LSC_B6_RamEvent( VOID * Dst );
/* ���Ϸ��� */
INT LSC_B6_FaultRet( VOID * Dst );
/* ¼����ŷ����仯 */
INT LSC_B6_WaveIndexChange( VOID * Dst );
/* B6����Ĺ����� */
INT LSC_B6_MessageManagFun( VOID * Dst );
/* ���ݱ�ָ���ֶ�ͻ�䱨�� */
INT LSC_B6_TableFieldSudden( VOID * Dst );
/*============================*/
/* �������Ͷ��� */
typedef INT (* B6_ManagFun )( VOID * Dst );
/*============================*/
/* ���ȼ���������ʼ������ */
VOID LSC_B6_ProirityInit( void );
#endif
