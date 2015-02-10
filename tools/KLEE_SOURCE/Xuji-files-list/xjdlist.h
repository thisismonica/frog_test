/****************************************************************
* ������ƣ���̵�������������ģ��
* ��Ȩ���У�(C) Copyright 2007 ��̵���������
* �ļ����ƣ�˫������Ľṹ�ͽӿڶ���(double list)
* �ļ����ߣ������
*****************************************************************/
#ifndef _XJ_DLIST_H
#define _XJ_DLIST_H

#include "xjtypes.h"

#define XJ_DLIST_UNLIMIT    0xFFFFFFFF/*����ڵ����û������*/

/*�Ƚ������ڵ�����ȼ�,����ֵ�������ʾҪ����ǰ��*/
typedef XJ_INT  (* XJ_CMP_PRIORITY )( XJ_VOID * pNew1, XJ_VOID * pOld2 );
/*���ڵ������Ƿ���������*/
typedef XJ_BOOL (* XJ_LIMIT_NODE )( XJ_VOID * pNode );

/*�����ͷ�����ڵ�ռ�Ļص���������*/
typedef XJ_VOID (* XJ_MEM_FREE_FUNC )(
    void * pBuf );

/*˫������ڵ����ݽṹ*/
typedef struct STRUCT_XJ_DLIST_NODE
{
    /*��̽ڵ�*/
    struct STRUCT_XJ_DLIST_NODE * pNext;
    /*ǰ���ڵ�*/
    struct STRUCT_XJ_DLIST_NODE * pPrev;
}   XJ_NODE;

/*�������ݽṹ*/
typedef struct STRUCT_XJ_DLIST
{
    XJ_ULONG size;     /*��������*/
    XJ_ULONG max;      /*�����������ɵĽ����*/
    XJ_NODE * pFirst;  /*��һ���ڵ�*/
    XJ_NODE * pLast;   /*���һ���ڵ�*/
}   XJ_DLIST;

/*�������Ľڵ����*/
#define xjdlist_size(pList)             (((XJ_DLIST *)pList)->size)
/*��������ͷָ��*/
#define xjdlist_getFirst(pList)         (((XJ_DLIST *)pList)->pFirst)
/*��������βָ��*/
#define xjdlist_getLast(pList)          (((XJ_DLIST *)pList)->pLast)
/*�������ڵ�ĺ����ڵ�*/
#define xjdlist_getAfter(pList,pEntry)  (((XJ_NODE *)pEntry)->pNext)
/*�������ڵ��ǰ���ڵ�*/
#define xjdlist_getPrev(pList,pEntry)   (((XJ_NODE *)pEntry)->pPrev)
/*�������ڵ�������*/
#define xjdlist_getLimit(pList)         (((XJ_DLIST *)pList)->max)
/*�ж������Ƿ���*/
#define xjdlist_full(pList)             (xjdlist_size(pList)>=xjdlist_getLimit(pList))
/*�ж������Ƿ��*/
#define xjdlist_empty(pList)            (xjdlist_size(pList) == 0)

/*����������ת��������滻*/
/*�������Ľڵ����*/
#define dlist_size                  xjdlist_size
/*��������ͷָ��*/
#define dlist_getFirst              xjdlist_getFirst
/*��������βָ��*/
#define dlist_getLast               xjdlist_getLast
/*�������ڵ�ĺ����ڵ�*/
#define dlist_getAfter              xjdlist_getAfter
/*�������ڵ��ǰ���ڵ�*/
#define dlist_getPrev               xjdlist_getPrev
/*�������ڵ�������*/
#define dlist_getLimit              xjdlist_getLimit
/*�ж������Ƿ���*/
#define dlist_full                  xjdlist_full
/*�ж������Ƿ��*/
#define dlist_empty                 xjdlist_empty
/*˫������ڵ����ݽṹ*/
#define NODE                        XJ_NODE
/*�������ݽṹ*/
#define DLIST                       XJ_DLIST
/*��ʼ������*/
#define dlist_initialize            xjdlist_initialize
/*ɾ���������нڵ㲢�ͷſռ�*/
#define dlist_destroy               xjdlist_destroy
/*������β����ӽڵ�*/
#define dlist_addEntry              xjdlist_addEntry
/*��ָ��λ����ӽڵ�*/
#define dlist_insertEntryAt         xjdlist_insertEntryAt
/*��ָ���ڵ�ǰ��ӽڵ�*/
#define dlist_insertEntryBefore     xjdlist_insertEntryBefore
/*��ָ���ڵ����ӽڵ�*/
#define dlist_insertEntryAfter      xjdlist_insertEntryAfter
/*��ָ��������������*/
#define dlist_InsertListAfter       xjdlist_InsertListAfter
/*��ָ������ǰ�������*/
#define dlist_InsertListBefore      xjdlist_InsertListBefore
/*������ɾ��ָ���ڵ�*/
#define dlist_removeEntry           xjdlist_removeEntry
/*���ָ������λ�õĽڵ�*/
#define dlist_getEntry              xjdlist_getEntry
/*���ָ���ڵ������λ��*/
#define dlist_LocateEntry           xjdlist_LocateEntry
/*�������ȼ����ӽڵ�*/
#define dlist_addPriEntry           xjdlist_addPriEntry

/*��ʼ������*/
XJ_VOID xjdlist_initialize( XJ_DLIST * pList, XJ_ULONG entry_max );

/*ɾ���������нڵ㲢�ͷſռ�*/
XJ_VOID xjdlist_destroy( XJ_DLIST * pList, XJ_MEM_FREE_FUNC pFreeFunc );

/*������β����ӽڵ�*/
XJ_VOID xjdlist_addEntry( XJ_DLIST * pList, XJ_NODE * pEntry );

/*��ָ��λ����ӽڵ�*/
XJ_VOID xjdlist_insertEntryAt( XJ_DLIST * pList, XJ_ULONG  index, XJ_NODE * pEntry );

/*��ָ���ڵ�ǰ��ӽڵ�*/
XJ_VOID xjdlist_insertEntryBefore( XJ_DLIST * pList, XJ_NODE * pExistingEntry, XJ_NODE * pNewEntry );

/*��ָ���ڵ����ӽڵ�*/
XJ_VOID xjdlist_insertEntryAfter( XJ_DLIST * pList, XJ_NODE * pExistingEntry, XJ_NODE * pNewEntry );

/*��ָ��������������*/
XJ_VOID xjdlist_InsertListAtTail( XJ_DLIST * pDstList, XJ_DLIST * pSrcList );

/*��ָ������ǰ�������*/
XJ_VOID xjdlist_InsertListAtHead( XJ_DLIST * pDstList, XJ_DLIST * pSrcList );

/*������ɾ��ָ���ڵ�*/
XJ_BOOL xjdlist_removeEntry( XJ_DLIST * pList, XJ_NODE * pEntry );

/*���ָ������λ�õĽڵ�*/
XJ_NODE * xjdlist_getEntry( XJ_DLIST * pList, XJ_USHORT index );

/*���ָ���ڵ������λ��*/
XJ_INT  xjdlist_LocateEntry( XJ_DLIST * pList, XJ_NODE  * pNode );

/*�������ȼ����ӽڵ�*/
XJ_VOID xjdlist_addPriEntry( XJ_DLIST * pList, XJ_NODE * pEntry,
    XJ_CMP_PRIORITY cmp_pri_fun );

/*�����޶��������ͷ�ڵ�*/
XJ_NODE * xjdlist_getLimitFirst( XJ_DLIST * pList, XJ_LIMIT_NODE limit_func );

/* ����Ԫ������ */
typedef struct xnholder
{
    struct xnholder * next;
    struct xnholder * last;
} xnholder_t;

typedef struct xnqueue
{
    xnholder_t head;
    XJ_U32 elems;
} xnqueue_t;

void inith( xnholder_t * holder );
void ath( xnholder_t * head, xnholder_t * holder );
void dth( xnholder_t * holder );
void initq( xnqueue_t * qslot );
void insertq( xnqueue_t * qslot, xnholder_t * head, xnholder_t * holder );
void removeq( xnqueue_t * qslot, xnholder_t * holder );
xnholder_t * getheadq( xnqueue_t * qslot );
xnholder_t * getqueue( xnqueue_t * qslot );
xnholder_t * nextq( xnqueue_t * qslot, xnholder_t * holder );
xnholder_t * popq( xnqueue_t * qslot, xnholder_t * holder );
int countq( xnqueue_t * qslot );
int emptyq_p( xnqueue_t * qslot );
void moveq( xnqueue_t * dstq, xnqueue_t * srcq );

#endif /* XJDLISTDEFINED */
