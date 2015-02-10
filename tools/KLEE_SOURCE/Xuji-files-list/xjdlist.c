/*
* Copyright (c) 2006,��̼������޹�˾
* All rights reserved.
*
* �ļ����ƣ�xjdlist.c(XJ double list core)
* ժ    Ҫ��˫���������ݽṹ��ʵ��
*
* �汾�ţ�0.1   ���ߣ������  ������ڣ�2006��03��15��
*/
#include "xjdlist.h"
#define XJ_NULL 0

#define XJ_TRUE                             (0==0)
#define XJ_FALSE                            (0==1)
/*********************************************************************
���������ơ�       xjdlist_initialize
���������ܡ�       ��ʼ������
��������           pList,Ҫ��ʼ��������ָ��
��ȫ�ֱ�����
������ֵ��
����ע��
�������߼����ڡ�   �����   2005-06-16
���汾��
�����ļ�¼��       2005-06-16  ����
������޸ġ�
���޸����ڡ�
������ԭ���Ҫ��
���汾��
��ʹ�������      ��������ʱ���á�
**********************************************************************/
XJ_VOID xjdlist_initialize( XJ_DLIST * pList, XJ_ULONG entry_max )
{
    pList->size     = 0;          /*�ڵ�Ϊ��*/
    pList->max      = entry_max;  /*����������������û��ɸ���ʵ�ʽ�������*/
    pList->pFirst   = XJ_NULL;    /*�����һ���ڵ�Ϊ��*/
    pList->pLast    = XJ_NULL;    /*�����һ���ڵ�Ϊ��*/
}

/*********************************************************************
���������ơ�       xjdlist_destroy
���������ܡ�       �ͷ��������нڵ㡣
��������           pList,Ҫ�ͷŵ�����
pFreeFunc,�ͷ�����ڵ�ĺ���ָ�롣
��ȫ�ֱ�����
������ֵ��
����ע��
�������߼����ڡ�   �����   2005-06-16
���汾��
�����ļ�¼��       2005-06-16  ����
������޸ġ�
���޸����ڡ�
������ԭ���Ҫ��
���汾��
��ʹ�������      �ͷ��������нڵ�ʱ���á�
**********************************************************************/
XJ_VOID xjdlist_destroy( XJ_DLIST * pList,
    XJ_MEM_FREE_FUNC pFreeFunc )
{
    XJ_NODE * pEntry;
    pEntry = xjdlist_getFirst( pList );
    /* ѭ��ɾ����һ�������㣬ֱ������Ϊ�գ������������ڵ������ */
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
���������ơ�       xjdlist_addEntry
���������ܡ�       ������ĩβ���ӽڵ㡣
��������           pList,Ҫ����������
                   pEntry,Ҫ���ӵĽڵ㡣
��ȫ�ֱ�����
������ֵ��
����ע��
�������߼����ڡ�   �����   2005-06-16
���汾��
�����ļ�¼��       2005-06-16  ����
������޸ġ�
���޸����ڡ�
������ԭ���Ҫ��
���汾��
��ʹ�������
**********************************************************************/
XJ_VOID xjdlist_addEntry( XJ_DLIST * pList,
    XJ_NODE * pEntry )
{
    if( pList->pFirst == XJ_NULL )
    {
        /*����½ڵ���Ψһһ���ڵ�*/
        pEntry->pNext = XJ_NULL;   /*����ǰ����*/
        pEntry->pPrev = XJ_NULL;   /*����ǰ�޼�*/
        pList->pFirst = pList->pLast = pEntry;
    }
    else
    {
        /*����������������*/
        pList->pLast->pNext = pEntry;        /*�½ڵ�Ϊԭ���Ϊ�ĺ��*/
        pEntry->pNext       = XJ_NULL;       /*�½ڵ��޺���*/
        pEntry->pPrev       = pList->pLast;  /*ԭ���Ϊ�½ڵ��ǰ��*/
        pList->pLast        = pEntry;        /*�����������*/
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
            /*���������*/
            pDstList->pFirst->pPrev = pSrcList->pLast;  /*�����Ϊ��������ǰ��*/
            pSrcList->pLast->pNext  = pDstList->pFirst; /*�������Ϊ�����ĺ��*/
        }
        else
        {
            /*��������*/
            pDstList->pLast    = pSrcList->pLast;  /*��ָ�����һ���ڵ�*/
        }
        pDstList->pFirst = pSrcList->pFirst;  /*��������ͷָ�������ͷ*/
        pDstList->size += pSrcList->size;     /*�ϲ����С*/

        /*���Դ��������*/
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
            /*���������*/
            pDstList->pLast->pNext  = pSrcList->pFirst; /*�����Ϊ�������ĺ��*/
            pSrcList->pFirst->pPrev = pDstList->pLast;  /*�������Ϊ������ǰ��*/
        }
        else
        {
            /*��������*/
            pDstList->pFirst    = pSrcList->pFirst;  /*��ָ���һ���ڵ�*/
        }
        pDstList->pLast = pSrcList->pLast; /*��������ͷָ�������β*/
        pDstList->size += pSrcList->size;  /*�ϲ����С*/

        /*���Դ��������*/
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
        /*��ͷ����*/
        pEntry->pPrev = XJ_NULL;
        pEntry->pNext = pList->pFirst;  /*�½ڵ�ĺ��Ϊԭ��ͷ*/
        if( pList->pLast == XJ_NULL )
        {
            pList->pLast = pEntry;  /*���ԭ�����޽ڵ㣬�½ڵ�Ϊ��β*/
        }
        else
        {
            pList->pFirst->pPrev = pEntry;  /*�����½ڵ�Ϊԭ��ͷ��ǰ��*/
        }
        pList->pFirst = pEntry;  /*�½ڵ��Ϊ�±�ͷ*/
        pList->size += 1;
    }
    else if( index >= pList->size )
    {
        /*��β����*/
        xjdlist_addEntry( pList, pEntry );  /*����������ڱ�ڵ��������½ڵ�嵽��β*/
    }
    else
    {
        /*�м����*/
        pMember = pList->pFirst;
        for( i = 0; i < ( index - 1 ); i ++)
        {
            pMember = pMember->pNext;
            if( pMember == XJ_NULL )
            {
                break;  /*�ҵ�����λ��*/
            }
        }

        /*����*/
        xjdlist_insertEntryAfter( pList, pMember, pEntry );
    }
}

/* function: xjdlist_insertEntryBefore */
XJ_VOID xjdlist_insertEntryBefore( XJ_DLIST * pList,
    XJ_NODE * pExistingEntry,
    XJ_NODE * pNewEntry )
{
    /*�ڽڵ�pExistingEntryǰ����*/
    pNewEntry->pNext = pExistingEntry;         /*�½ڵ�ĺ��Ϊ�����*/
    pNewEntry->pPrev = pExistingEntry->pPrev;  /*�½ڵ��ǰ��Ϊ������ǰ��*/
    if( pExistingEntry == pList->pFirst )
    {
        /*�����Ϊ��ͷ��ͷ��Ϊ�½ڵ�*/
        pList->pFirst = pNewEntry;
    }
    else
    {
        /*�����½ڵ��Ϊ������ǰ���ĺ��*/
        pExistingEntry->pPrev->pNext = pNewEntry;
    }
    pExistingEntry->pPrev = pNewEntry;     /*������ǰ��Ϊ��Ϊ�½ڵ�*/

    pList->size += 1;
}

/* function: xjdlist_insertEntryAfter */
XJ_VOID xjdlist_insertEntryAfter( XJ_DLIST * pList,
    XJ_NODE * pExistingEntry,
    XJ_NODE * pNewEntry )
{
    /*�ڽڵ�pExistingEntry�����*/
    pNewEntry->pPrev = pExistingEntry;        /*�½ڵ��ǰ��Ϊ�����*/
    pNewEntry->pNext = pExistingEntry->pNext; /*�½ڵ�ĺ��Ϊ�����ĺ��*/
    if( pExistingEntry == pList->pLast )
    {
        /*�����Ϊ��β��β��Ϊ�½ڵ�*/
        pList->pLast = pNewEntry;
    }
    else
    {
        /*�����½ڵ��Ϊ�����ĺ�̵�ǰ��*/
        pExistingEntry->pNext->pPrev = pNewEntry;
    }
    pExistingEntry->pNext = pNewEntry;  /*�����ĺ��Ϊ��Ϊ�½ڵ�*/

    pList->size += 1;
}

/* ������ɾ��ָ���Ľڵ� */
XJ_BOOL xjdlist_removeEntry( XJ_DLIST * pList,
    XJ_NODE * pEntry )
{
    if ( pEntry == pList->pFirst )
    {
        /*ɾ��Ϊ��ͷ�����±�ͷ��Ϊ��һ�ڵ�*/
        pList->pFirst = pEntry->pNext;
    }
    else
    {
        /*����ɾ���ǰ���ĺ�̱�Ϊɾ��ĺ��*/
        pEntry->pPrev->pNext = pEntry->pNext;
    }

    if ( pEntry == pList->pLast )
    {
        /*ɾ��Ϊ��ͷ�����±�ͷ��Ϊǰһ�ڵ�*/
        pList->pLast = pEntry->pPrev;
    }
    else
    {
        pEntry->pNext->pPrev = pEntry->pPrev;
    }
    pList->size -= 1;
    return( XJ_TRUE );
}

/* ����������ýڵ� */
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
���������ơ�       xjdlist_LocateEntry
���������ܡ�       �������ж�λ�ڵ�λ�ã�
                   �����ڵ���̬��λ��Ч�����������
��������           pList,Ҫ����������
                   pNode,Ҫ�����Ľڵ㡣
������ֵ��         >=0,�ڵ��������е�������
                   -1���ڵ㲻�������Ա��
����ע��
�������߼����ڡ�   �����   2006-11-29
���汾��
�����ļ�¼��       2006-11-29  ����
������޸ġ�
���޸����ڡ�
������ԭ���Ҫ��
���汾��
��ʹ�������
**********************************************************************/
XJ_INT  xjdlist_LocateEntry( XJ_DLIST * pList,
    XJ_NODE  * pNode )
{
    XJ_NODE * pEntry;
    XJ_INT   index;
    index  = 0;
    /*�����������ҽڵ�*/
    pEntry = ( XJ_NODE *)xjdlist_getFirst( pList );
    while ( pEntry != XJ_NULL )
    {
        if ( pEntry == pNode )
        {
            /*�ҵ��ڵ㣬��������λ��*/
            return index;
        }
        index ++;
        pEntry = xjdlist_getAfter( pList, pEntry );
    }
    /*û���ҵ��ڵ㣬����ʧ��*/
    return - 1;
}

XJ_VOID xjdlist_addPriEntry( XJ_DLIST * pList, XJ_NODE * pEntry,
    XJ_CMP_PRIORITY cmp_pri_fun )
{
    XJ_NODE * pExistEntry;
    if ( XJ_NULL == cmp_pri_fun )
    {
        /*����Ҫ���ȼ����*/
        xjdlist_addEntry( pList, pEntry );
    }
    else
    {
        pExistEntry = xjdlist_getFirst( pList );
        while ( XJ_NULL != pExistEntry )
        {
            /*ѭ��������ȼ�*/
            if ( cmp_pri_fun( pEntry, pExistEntry ) > 0 )
            {
                /*������Чλ��*/
                xjdlist_insertEntryBefore( pList,
                    pExistEntry, pEntry );
                break;
            }
            pExistEntry = xjdlist_getAfter(
                pList, pExistEntry );
        }
        if ( XJ_NULL == pExistEntry )
        {
            /*���ȼ���ͣ�����ĩβ*/
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
        /*û�������ж�*/
        return pNode;
    }
    if ( limit_func( pNode ))
    {
        /*��������������*/
        return pNode;
    }
    /*���������������ؿ�*/
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
