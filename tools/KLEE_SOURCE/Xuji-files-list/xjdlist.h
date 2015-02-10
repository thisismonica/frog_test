/****************************************************************
* 软件名称：许继电气基础部重用模块
* 版权所有：(C) Copyright 2007 许继电气基础部
* 文件名称：双向链表的结构和接口定义(double list)
* 文件作者：陈玉峰
*****************************************************************/
#ifndef _XJ_DLIST_H
#define _XJ_DLIST_H

#include "xjtypes.h"

#define XJ_DLIST_UNLIMIT    0xFFFFFFFF/*链表节点个数没有限制*/

/*比较两个节点的优先级,返回值大于零表示要插在前面*/
typedef XJ_INT  (* XJ_CMP_PRIORITY )( XJ_VOID * pNew1, XJ_VOID * pOld2 );
/*检查节点数据是否满足条件*/
typedef XJ_BOOL (* XJ_LIMIT_NODE )( XJ_VOID * pNode );

/*定义释放链表节点空间的回调函数类型*/
typedef XJ_VOID (* XJ_MEM_FREE_FUNC )(
    void * pBuf );

/*双向链表节点数据结构*/
typedef struct STRUCT_XJ_DLIST_NODE
{
    /*后继节点*/
    struct STRUCT_XJ_DLIST_NODE * pNext;
    /*前驱节点*/
    struct STRUCT_XJ_DLIST_NODE * pPrev;
}   XJ_NODE;

/*链表数据结构*/
typedef struct STRUCT_XJ_DLIST
{
    XJ_ULONG size;     /*链表结点数*/
    XJ_ULONG max;      /*链表最多可容纳的结点数*/
    XJ_NODE * pFirst;  /*第一个节点*/
    XJ_NODE * pLast;   /*最后一个节点*/
}   XJ_DLIST;

/*获得链表的节点个数*/
#define xjdlist_size(pList)             (((XJ_DLIST *)pList)->size)
/*获得链表的头指针*/
#define xjdlist_getFirst(pList)         (((XJ_DLIST *)pList)->pFirst)
/*获得链表的尾指针*/
#define xjdlist_getLast(pList)          (((XJ_DLIST *)pList)->pLast)
/*获得链表节点的后续节点*/
#define xjdlist_getAfter(pList,pEntry)  (((XJ_NODE *)pEntry)->pNext)
/*获得链表节点的前驱节点*/
#define xjdlist_getPrev(pList,pEntry)   (((XJ_NODE *)pEntry)->pPrev)
/*获得链表节点最大个数*/
#define xjdlist_getLimit(pList)         (((XJ_DLIST *)pList)->max)
/*判断链表是否满*/
#define xjdlist_full(pList)             (xjdlist_size(pList)>=xjdlist_getLimit(pList))
/*判断链表是否空*/
#define xjdlist_empty(pList)            (xjdlist_size(pList) == 0)

/*以下用于老转新链表的替换*/
/*获得链表的节点个数*/
#define dlist_size                  xjdlist_size
/*获得链表的头指针*/
#define dlist_getFirst              xjdlist_getFirst
/*获得链表的尾指针*/
#define dlist_getLast               xjdlist_getLast
/*获得链表节点的后续节点*/
#define dlist_getAfter              xjdlist_getAfter
/*获得链表节点的前驱节点*/
#define dlist_getPrev               xjdlist_getPrev
/*获得链表节点最大个数*/
#define dlist_getLimit              xjdlist_getLimit
/*判断链表是否满*/
#define dlist_full                  xjdlist_full
/*判断链表是否空*/
#define dlist_empty                 xjdlist_empty
/*双向链表节点数据结构*/
#define NODE                        XJ_NODE
/*链表数据结构*/
#define DLIST                       XJ_DLIST
/*初始化链表*/
#define dlist_initialize            xjdlist_initialize
/*删除链表所有节点并释放空间*/
#define dlist_destroy               xjdlist_destroy
/*在链表尾部添加节点*/
#define dlist_addEntry              xjdlist_addEntry
/*在指定位置添加节点*/
#define dlist_insertEntryAt         xjdlist_insertEntryAt
/*在指定节点前添加节点*/
#define dlist_insertEntryBefore     xjdlist_insertEntryBefore
/*在指定节点后添加节点*/
#define dlist_insertEntryAfter      xjdlist_insertEntryAfter
/*在指定链表后添加链表*/
#define dlist_InsertListAfter       xjdlist_InsertListAfter
/*在指定链表前添加链表*/
#define dlist_InsertListBefore      xjdlist_InsertListBefore
/*从链表删除指定节点*/
#define dlist_removeEntry           xjdlist_removeEntry
/*获得指定索引位置的节点*/
#define dlist_getEntry              xjdlist_getEntry
/*获得指定节点的索引位置*/
#define dlist_LocateEntry           xjdlist_LocateEntry
/*根据优先级增加节点*/
#define dlist_addPriEntry           xjdlist_addPriEntry

/*初始化链表*/
XJ_VOID xjdlist_initialize( XJ_DLIST * pList, XJ_ULONG entry_max );

/*删除链表所有节点并释放空间*/
XJ_VOID xjdlist_destroy( XJ_DLIST * pList, XJ_MEM_FREE_FUNC pFreeFunc );

/*在链表尾部添加节点*/
XJ_VOID xjdlist_addEntry( XJ_DLIST * pList, XJ_NODE * pEntry );

/*在指定位置添加节点*/
XJ_VOID xjdlist_insertEntryAt( XJ_DLIST * pList, XJ_ULONG  index, XJ_NODE * pEntry );

/*在指定节点前添加节点*/
XJ_VOID xjdlist_insertEntryBefore( XJ_DLIST * pList, XJ_NODE * pExistingEntry, XJ_NODE * pNewEntry );

/*在指定节点后添加节点*/
XJ_VOID xjdlist_insertEntryAfter( XJ_DLIST * pList, XJ_NODE * pExistingEntry, XJ_NODE * pNewEntry );

/*在指定链表后添加链表*/
XJ_VOID xjdlist_InsertListAtTail( XJ_DLIST * pDstList, XJ_DLIST * pSrcList );

/*在指定链表前添加链表*/
XJ_VOID xjdlist_InsertListAtHead( XJ_DLIST * pDstList, XJ_DLIST * pSrcList );

/*从链表删除指定节点*/
XJ_BOOL xjdlist_removeEntry( XJ_DLIST * pList, XJ_NODE * pEntry );

/*获得指定索引位置的节点*/
XJ_NODE * xjdlist_getEntry( XJ_DLIST * pList, XJ_USHORT index );

/*获得指定节点的索引位置*/
XJ_INT  xjdlist_LocateEntry( XJ_DLIST * pList, XJ_NODE  * pNode );

/*根据优先级增加节点*/
XJ_VOID xjdlist_addPriEntry( XJ_DLIST * pList, XJ_NODE * pEntry,
    XJ_CMP_PRIORITY cmp_pri_fun );

/*根据限定条件获得头节点*/
XJ_NODE * xjdlist_getLimitFirst( XJ_DLIST * pList, XJ_LIMIT_NODE limit_func );

/* 基本元素容器 */
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
