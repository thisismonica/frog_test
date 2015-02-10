
#include "plat/inc/xj_cosc.h"
#include "plat/es_plat.h"

#ifndef RTC_EXTR_H
#define RTC_EXTR_H

/* 获取时间函数 */
int RTC_Get_Rtc_Time( TIME * pCLOCK_BUF );
/* 设置时间函数 */
int RTC_Set_Rtc_Time( TIME * pCLOCK_BUF );
/* 时钟初始化函数 */
void RTC_Clock_Initial( void );

#endif
