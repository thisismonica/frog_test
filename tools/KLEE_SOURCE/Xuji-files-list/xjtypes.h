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
** ��Ԫ��ʶ��$Id: xjtypes.h,v 1.7 2012/02/23 02:34:21 jyj Exp $
** �汾��ʷ��
** �� �� �ߣ�
** ����޸ģ�
**************************************************************/

#ifndef     _XJ_TYPES_H
#define     _XJ_TYPES_H

typedef              void   XJ_VOID;
typedef              char   XJ_CHAR;
typedef     unsigned char   XJ_UCHAR;
typedef     unsigned char   XJ_BYTE;

typedef              int    XJ_INT;
typedef     unsigned int    XJ_UINT;
typedef              short  XJ_SHORT;
typedef     unsigned short  XJ_USHORT;
typedef     unsigned short  XJ_UINT16;
typedef              short  XJ_INT16;
typedef     long            XJ_LONG;
typedef     unsigned long   XJ_ULONG;
typedef     unsigned long   XJ_ULONG32;
typedef     unsigned long   XJ_U32;
typedef     unsigned char   XJ_U8;
typedef     int             XJ_BOOL;
typedef     unsigned short  XJ_U16;

//typedef              int    XJ_STATUS;

typedef              float  XJ_FLOAT;
typedef              double XJ_DOUBLE;

typedef     unsigned long   XJ_DWORD;

typedef XJ_U32 XJ_TICKS;
typedef     unsigned char * XJ_UCHAR_PTR;
typedef     unsigned char * XJ_BYTE_PTR;

#define XJ_STATUS_FAIL     (-1)

#define offset_of(type, memb)   ((XJ_U32)(&((type *)0)->memb))

#define container_of(obj, type, memb) ((type *)(((char *)obj) - offset_of(type, memb)))

#endif
