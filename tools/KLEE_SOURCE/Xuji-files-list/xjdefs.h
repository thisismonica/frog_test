/**************************************************************
** Copyright (C) 2002-2007 基础技术开发部
** All right reserved
***************************************************************
**
** 项目名称：NPI801
** 功能简介：公共基本数据类型
**
** 原始作者：陈玉峰
** 组织部门：许继集团 基础技术开发部
** 备    注：
** 建立时间：2007-05-25
** 完成时间：2007-06-20
** 版    本：1.0
***************************************************************
** 单元标识：$Id: xjdefs.h,v 1.4 2010/05/06 01:56:20 yzd Exp $
** 版本历史：
** 修 改 者：
** 最近修改：
**************************************************************/
#include "plat/inc/xj_cosc.h"

#ifndef    _XJ_DEFS_H
#define    _XJ_DEFS_H

#define     XJ_GLOBAL   /*全局*/
//#define     XJ_NULL     0 /*空指针*/
#define MAX_SNTP_COUNT  4

#define SECONDS_DIFF_1900_1970   (XJ_U32)( 25567u * 24u * 3600u )
/* True or False */
//#define XJ_TRUE  (0==0)/*true*/
//#define XJ_FALSE (0==1)/*false*/

#define XJ_DWORD_SIZE               4/*字宽度*/
#define SUPPORT_UDP_CALLBACK    1
/* Ranges for various types defined above */

/* Calculate the max or min of two numbers */
#define XJ_MAX(x,y) ((x)>=(y)?(x):(y))/*max*/
#define XJ_MIN(x,y) ((x)<=(y)?(x):(y))/*min*/

#define XJ_STATUS_SUCCESS  0    /*成功*/

#define XJ_MASK_BITS_8     0xff /*8位数据掩码*/

#ifndef TAG_PATH
#define TAG_PATH(id)
#endif

#endif
