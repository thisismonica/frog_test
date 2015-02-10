/*
* Copyright (c) 2006,许继集团有限公司
* All rights reserved.
*
* 文件名称：xjdlist.c(XJ double list core)
* 摘    要：双向链表数据结构的实现
*
* 版本号：0.1   作者：陈玉峰  完成日期：2006年03月15日
*/
#include "xjdlist.h"
#define XJ_NULL 0

#define XJ_TRUE                             (0==0)
#define XJ_FALSE                            (0==1)
/*********************************************************************
【函数名称】       xjdlist_initialize
【函数功能】       初始化链表。
【参数】           pList,要初始化的链表指针
【全局变量】
【返回值】
【备注】
【开发者及日期】   陈玉峰   2005-06-16
【版本】
【更改记录】       2005-06-16  创建
【最后修改】
【修改日期】
【更改原因概要】
【版本】
【使用情况】      创建链表时调用。
**********************************************************************/
XJ_VOID xjdlist_initialize( XJ_DLIST * pList, XJ_ULONG entry_max )
{
    pList->size     = 0;          /*节点为空*/
    pList->max      = entry_max;  /*设置链表最大数，用户可根据实际进行设置*/
    pList->pFirst   = XJ_NULL;    /*链表第一个节点为空*/
    pList->pLast    = XJ_NULL;    /*链表第一个节点为空*/
}

/*********************************************************************
【函数名称】       xjdlist_destroy
【函数功能】       释放链表所有节点。
【参数】           pList,要释放的链表
pFreeFunc,释放链表节点的函数指针。
【全局变量】
【返回值】
【备注】
【开发者及日期】   陈玉峰   2005-06-16
【版本】
【更改记录】       2005-06-16  创建
【最后修改】
【修改日期】
【更改原因概要】
【版本】
【使用情况】      释放链表所有节点时调用。
**********************************************************************/
XJ_VOID xjdlist_destroy( XJ_DLIST * pList,
    XJ_MEM_FREE_FUNC pFreeFunc )
{
    XJ_NODE * pEntry;
    pEntry = xjdlist_getFirst( pList );
    /* 循环删除第一个链表结点，直到链表为空，就完成了链表节点的销毁 */
    while( pEntry != XJ_NULL )
    {
        if ( xjdlist_removeEntry( pList, pEntry ))
        {
            pFreeFunc(( XJ_BYTE *)pEntry );
        }
        pEntry = xjdlist_getFirst( pList );
    }
}

/*********************************************************************
【函数名称】       xjdlist_addEntry
【函数功能】       在链表末尾增加节点。
【参数】           pList,要操作的链表
                   pEntry,要增加的节点。
【全局变量】
【返回值】
【备注】
【开发者及日期】   陈玉峰   2005-06-16
【版本】
【更改记录】       2005-06-16  创建
【最后修改】
【修改日期】
【更改原因概要】
【版本】
【使用情况】
**********************************************************************/
XJ_VOID xjdlist_addEntry( XJ_DLIST * pList,
    XJ_NODE * pEntry )
{
    if( pList->pFirst == XJ_NULL )
    {
        /*如果新节点是唯一一个节点*/
        pEntry->pNext = XJ_NULL;   /*它的前无驱*/
        pEntry->pPrev = XJ_NULL;   /*它的前无继*/
        pList->pFirst = pList->pLast = pEntry;
    }
    else
    {
        /*否则在链表最后加入*/
        pList->pLast->pNext = pEntry;        /*新节点为原最后为的后继*/
        pEntry->pNext       = XJ_NULL;       /*新节点无后续*/
        pEntry->pPrev       = pList->pLast;  /*原最后为新节点的前驱*/
        pList->pLast        = pEntry;        /*放在链表最后*/
    }
    pList->size += 1;
    return;
}

XJ_VOID xjdlist_InsertListAtHead( XJ_DLIST * pDstList,
    XJ_DLIST * pSrcList )
{
    if ( pSrcList->size != 0 )
    {
        if ( pDstList->size != 0 )
        {
            /*被加入表不空*/
            pDstList->pFirst->pPrev = pSrcList->pLast;  /*加入表为被加入表的前驱*/
            pSrcList->pLast->pNext  = pDstList->pFirst; /*被加入表为加入表的后继*/
        }
        else
        {
            /*被加入表空*/
            pDstList->pLast    = pSrcList->pLast;  /*均指向最后一个节点*/
        }
        pDstList->pFirst = pSrcList->pFirst;  /*被加入表的头指向加入表的头*/
        pDstList->size += pSrcList->size;     /*合并表大小*/

        /*清空源数据链表*/
        pSrcList->pFirst    = XJ_NULL;
        pSrcList->pLast     = XJ_NULL;
        pSrcList->size      = 0;
    }
    return;
}

XJ_VOID xjdlist_InsertListAtTail( XJ_DLIST * pDstList,
    XJ_DLIST * pSrcList )
{
    if ( pSrcList->size != 0 )
    {
        if ( pDstList->size != 0 )
        {
            /*被加入表不空*/
            pDstList->pLast->pNext  = pSrcList->pFirst; /*加入表为被加入表的后继*/
            pSrcList->pFirst->pPrev = pDstList->pLast;  /*被加入表为加入表的前驱*/
        }
        else
        {
            /*被加入表空*/
            pDstList->pFirst    = pSrcList->pFirst;  /*均指向第一个节点*/
        }
        pDstList->pLast = pSrcList->pLast; /*被加入表的头指向加入表的尾*/
        pDstList->size += pSrcList->size;  /*合并表大小*/

        /*清空源数据链表*/
        pSrcList->pFirst    = XJ_NULL;
        pSrcList->pLast     = XJ_NULL;
        pSrcList->size      = 0;
    }
    return;
}

/* function: xjdlist_insertEntryAt */
XJ_VOID xjdlist_insertEntryAt( XJ_DLIST * pList,
    XJ_ULONG  index,
    XJ_NODE * pEntry )
{
    XJ_NODE * pMember;
    XJ_U16 i;
    if( index == 0 )
    {
        /*表头加入*/
        pEntry->pPrev = XJ_NULL;
        pEntry->pNext = pList->pFirst;  /*新节点的后继为原表头*/
        if( pList->pLast == XJ_NULL )
        {
            pList->pLast = pEntry;  /*如果原表中无节点，新节点为表尾*/
        }
        else
        {
            pList->pFirst->pPrev = pEntry;  /*否则，新节点为原表头的前驱*/
        }
        pList->pFirst = pEntry;  /*新节点变为新表头*/
        pList->size += 1;
    }
    else if( index >= pList->size )
    {
        /*表尾加入*/
        xjdlist_addEntry( pList, pEntry );  /*如果索引大于表节点数，把新节点插到表尾*/
    }
    else
    {
        /*中间加入*/
        pMember = pList->pFirst;
        for( i = 0; i < ( index - 1 ); i ++)
        {
            pMember = pMember->pNext;
            if( pMember == XJ_NULL )
            {
                break;  /*找到加入位置*/
            }
        }

        /*加入*/
        xjdlist_insertEntryAfter( pList, pMember, pEntry );
    }
}

/* function: xjdlist_insertEntryBefore */
XJ_VOID xjdlist_insertEntryBefore( XJ_DLIST * pList,
    XJ_NODE * pExistingEntry,
    XJ_NODE * pNewEntry )
{
    /*在节点pExistingEntry前加入*/
    pNewEntry->pNext = pExistingEntry;         /*新节点的后继为加入点*/
    pNewEntry->pPrev = pExistingEntry->pPrev;  /*新节点的前驱为加入点的前驱*/
    if( pExistingEntry == pList->pFirst )
    {
        /*加入点为表头，头变为新节点*/
        pList->pFirst = pNewEntry;
    }
    else
    {
        /*否则，新节点变为加入点的前驱的后继*/
        pExistingEntry->pPrev->pNext = pNewEntry;
    }
    pExistingEntry->pPrev = pNewEntry;     /*加入点的前驱为变为新节点*/

    pList->size += 1;
}

/* function: xjdlist_insertEntryAfter */
XJ_VOID xjdlist_insertEntryAfter( XJ_DLIST * pList,
    XJ_NODE * pExistingEntry,
    XJ_NODE * pNewEntry )
{
    /*在节点pExistingEntry后加入*/
    pNewEntry->pPrev = pExistingEntry;        /*新节点的前驱为加入点*/
    pNewEntry->pNext = pExistingEntry->pNext; /*新节点的后继为加入点的后继*/
    if( pExistingEntry == pList->pLast )
    {
        /*加入点为表尾，尾变为新节点*/
        pList->pLast = pNewEntry;
    }
    else
    {
        /*否则，新节点变为加入点的后继的前驱*/
        pExistingEntry->pNext->pPrev = pNewEntry;
    }
    pExistingEntry->pNext = pNewEntry;  /*加入点的后继为变为新节点*/

    pList->size += 1;
}

/* 从链表删除指定的节点 */
XJ_BOOL xjdlist_removeEntry( XJ_DLIST * pList,
    XJ_NODE * pEntry )
{
    if ( pEntry == pList->pFirst )
    {
        /*删点为表头进，新表头变为下一节点*/
        pList->pFirst = pEntry->pNext;
    }
    else
    {
        /*否则，删点的前驱的后继变为删点的后继*/
        pEntry->pPrev->pNext = pEntry->pNext;
    }

    if ( pEntry == pList->pLast )
    {
        /*删点为表头进，新表头变为前一节点*/
        pList->pLast = pEntry->pPrev;
    }
    else
    {
        pEntry->pNext->pPrev = pEntry->pPrev;
    }
    pList->size -= 1;
    return( XJ_TRUE );
}

/* 根据索引获得节点 */
XJ_NODE * xjdlist_getEntry( XJ_DLIST * pList, XJ_U16 index )
{
    XJ_U16 i;
    XJ_NODE * pMember;
    pMember = XJ_NULL;
    if ( index < pList->size )
    {
        pMember = pList->pFirst;
        i = 0;
        while (( pMember != XJ_NULL ) && ( i < index ))
        {
            pMember = pMember->pNext;
            i ++;
        }
    }
    return( pMember );
}

/*********************************************************************
【函数名称】       xjdlist_LocateEntry
【函数功能】       在链表中定位节点位置，
                   用于在调试态定位无效的链表操作。
【参数】           pList,要操作的链表
                   pNode,要操作的节点。
【返回值】         >=0,节点在链表中的索引。
                   -1，节点不是链表成员。
【备注】
【开发者及日期】   陈玉峰   2006-11-29
【版本】
【更改记录】       2006-11-29  创建
【最后修改】
【修改日期】
【更改原因概要】
【版本】
【使用情况】
**********************************************************************/
XJ_INT  xjdlist_LocateEntry( XJ_DLIST * pList,
    XJ_NODE  * pNode )
{
    XJ_NODE * pEntry;
    XJ_INT   index;
    index  = 0;
    /*遍历链表，查找节点*/
    pEntry = ( XJ_NODE *)xjdlist_getFirst( pList );
    while ( pEntry != XJ_NULL )
    {
        if ( pEntry == pNode )
        {
            /*找到节点，返回索引位置*/
            return index;
        }
        index ++;
        pEntry = xjdlist_getAfter( pList, pEntry );
    }
    /*没有找到节点，返回失败*/
    return - 1;
}

XJ_VOID xjdlist_addPriEntry( XJ_DLIST * pList, XJ_NODE * pEntry,
    XJ_CMP_PRIORITY cmp_pri_fun )
{
    XJ_NODE * pExistEntry;
    if ( XJ_NULL == cmp_pri_fun )
    {
        /*不需要优先级检查*/
        xjdlist_addEntry( pList, pEntry );
    }
    else
    {
        pExistEntry = xjdlist_getFirst( pList );
        while ( XJ_NULL != pExistEntry )
        {
            /*循环检查优先级*/
            if ( cmp_pri_fun( pEntry, pExistEntry ) > 0 )
            {
                /*插在有效位置*/
                xjdlist_insertEntryBefore( pList,
                    pExistEntry, pEntry );
                break;
            }
            pExistEntry = xjdlist_getAfter(
                pList, pExistEntry );
        }
        if ( XJ_NULL == pExistEntry )
        {
            /*优先级最低，插在末尾*/
            xjdlist_addEntry( pList, pEntry );
        }
    }
}

XJ_NODE * xjdlist_getLimitFirst( XJ_DLIST * pList, XJ_LIMIT_NODE limit_func )
{
    XJ_NODE * pNode;
    pNode = xjdlist_getFirst( pList );
    if ( pNode == XJ_NULL )
    {
        return pNode;
    }
    if ( XJ_NULL == limit_func )
    {
        /*没有条件判断*/
        return pNode;
    }
    if ( limit_func( pNode ))
    {
        /*符合条件，返回*/
        return pNode;
    }
    /*不符合条件，返回空*/
    return XJ_NULL;
}

void inith( xnholder_t * holder )
{
    /* Holding queues are doubly-linked and circular */
    holder->last = holder;
    holder->next = holder;
}

void ath( xnholder_t * head, xnholder_t * holder )
{
    /* Inserts the new element right after the heading one  */
    holder->last = head;
    holder->next = head->next;
    holder->next->last = holder;
    head->next = holder;
}

void dth( xnholder_t * holder )
{
    holder->last->next = holder->next;
    holder->next->last = holder->last;
}

void initq( xnqueue_t * qslot )
{
    inith(& qslot->head );
    qslot->elems = 0;
}

void insertq( xnqueue_t * qslot, xnholder_t * head, xnholder_t * holder )
{
    /* Insert the <holder> element before <head> */
    ath( head->last, holder );
    ++ qslot->elems;
}

void removeq( xnqueue_t * qslot, xnholder_t * holder )
{
    dth( holder );
    -- qslot->elems;
}

xnholder_t * getheadq( xnqueue_t * qslot )
{
    xnholder_t * holder = qslot->head.next;
    return holder == & qslot->head ? 0 : holder;
}

xnholder_t * getqueue( xnqueue_t * qslot )
{
    xnholder_t * holder = getheadq( qslot );
    if ( holder )
        removeq( qslot, holder );
    return holder;
}

xnholder_t * nextq( xnqueue_t * qslot, xnholder_t * holder )
{
    xnholder_t * nextholder = holder->next;
    return nextholder == & qslot->head ? 0 : nextholder;
}

xnholder_t * popq( xnqueue_t * qslot, xnholder_t * holder )
{
    xnholder_t * nextholder = nextq( qslot, holder );
    removeq( qslot, holder );
    return nextholder;
}

int countq( xnqueue_t * qslot )
{
    return qslot->elems;
}

int emptyq_p( xnqueue_t * qslot )
{
    return qslot->head.next == & qslot->head;
}

void moveq( xnqueue_t * dstq, xnqueue_t * srcq )
{
    xnholder_t * headsrc = srcq->head.next;
    xnholder_t * tailsrc = srcq->head.last;
    xnholder_t * headdst = & dstq->head;

    if ( emptyq_p( srcq ))
        return;

    headsrc->last->next = tailsrc->next;
    tailsrc->next->last = headsrc->last;
    headsrc->last = headdst;
    tailsrc->next = headdst->next;
    headdst->next->last = tailsrc;
    headdst->next = headsrc;
    dstq->elems += srcq->elems;
    srcq->elems = 0;
}
