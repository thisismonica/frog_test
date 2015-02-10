/**************************************************************
** Copyright (C) 2002-2007 ��������������
** All right reserved
***************************************************************
**
** ��Ŀ���ƣ�NPI801
** ���ܼ�飺����������������
**
** ԭʼ���ߣ������
** ��֯���ţ���̼��� ��������������
** ��    ע��
** ����ʱ�䣺2007-05-25
** ���ʱ�䣺2007-06-20
** ��    ����1.0
***************************************************************
** ��Ԫ��ʶ��$Id: xjdefs.h,v 1.4 2010/05/06 01:56:20 yzd Exp $
** �汾��ʷ��
** �� �� �ߣ�
** ����޸ģ�
**************************************************************/
#include "plat/inc/xj_cosc.h"

#ifndef    _XJ_DEFS_H
#define    _XJ_DEFS_H

#define     XJ_GLOBAL   /*ȫ��*/
//#define     XJ_NULL     0 /*��ָ��*/
#define MAX_SNTP_COUNT  4

#define SECONDS_DIFF_1900_1970   (XJ_U32)( 25567u * 24u * 3600u )
/* True or False */
//#define XJ_TRUE  (0==0)/*true*/
//#define XJ_FALSE (0==1)/*false*/

#define XJ_DWORD_SIZE               4/*�ֿ��*/
#define SUPPORT_UDP_CALLBACK    1
/* Ranges for various types defined above */

/* Calculate the max or min of two numbers */
#define XJ_MAX(x,y) ((x)>=(y)?(x):(y))/*max*/
#define XJ_MIN(x,y) ((x)<=(y)?(x):(y))/*min*/

#define XJ_STATUS_SUCCESS  0    /*�ɹ�*/

#define XJ_MASK_BITS_8     0xff /*8λ��������*/

#ifndef TAG_PATH
#define TAG_PATH(id)
#endif

#endif
