
#include "plat/inc/xj_cosc.h"
#include "plat/es_plat.h"

#ifndef RTC_EXTR_H
#define RTC_EXTR_H

/* ��ȡʱ�亯�� */
int RTC_Get_Rtc_Time( TIME * pCLOCK_BUF );
/* ����ʱ�亯�� */
int RTC_Set_Rtc_Time( TIME * pCLOCK_BUF );
/* ʱ�ӳ�ʼ������ */
void RTC_Clock_Initial( void );

#endif
