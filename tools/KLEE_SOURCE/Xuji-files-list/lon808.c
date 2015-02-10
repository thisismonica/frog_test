/********************************************************************************/
/*  Copyight (C) 2001-2003      �������Զ�����ҵ������                          */
/*                                                                              */
/* All right reserved                                                           */
/*                                                                              */
/* FILE NAME                        VERSION                                     */
/*                                                                              */
/*  lcc.c                       PLUS  1.0                                       */
/*                                                                              */
/* COMPONENT                                                                    */
/*                                                                              */
/*      lc - lon commuciation                                                   */
/*      c   - core                                                              */
/*                                                                              */
/* DESCRIPTION                                                                  */
/*                                                                              */
/*  This file contains the core routines for Record wave                        */
/*                                                                              */
/*                                                                              */
/* AUTHOR                                                                       */
/*                                                                              */
/*  Chenyufeng      < ChenYuFeng@mail.xjpa.xj>                                  */
/* DATA STRUCTURES                                                              */
/*                                                                              */
/*  None                                                                        */
/*                                                                              */
/* HISTORY                                                                      */
/*                                                                              */
/*                                                                              */
/*      NAME        DATE            REMARKS                                     */
/*                                                                              */
/*      ChenYuFeng      06-20-2003                                              */
/********************************************************************************/
#include "plat/net_inc/lc_extr.h"

#define _LON_ADDR                       0x400010    /* ��ַ�˿� */
#define _LON_DATA                       0x400010    /* ���ݶ˿� */
#define _LON_CONTROL                    0x400011    /* ���ƶ˿� */
#define pLON_ADDR                      ((volatile unsigned short *)_LON_ADDR)
#define pLON_DATA                      ((volatile unsigned short *)_LON_DATA)
#define pLON_CONTROL                   ((volatile unsigned short *)_LON_CONTROL)

#define _HS_MASK                        0x01        /* mask */
#define _CMD_RESYNC                     0x5a        /* RESYNC */
#define _CMD_ACKSYNC                    0x07        /* ACKSYNC */
#define _CMD_NULL                       0x00        /* NULL */
#define _CMD_XFER                       0x01        /* XFER */

#define MAX_WAIT_TIME_HS                2000        /* ���HS�ȴ�ʱ�� */
#define MAX_WAIT_CMD_ACKSYNC            10          /* ���ȴ�CMD_ACKSYNCʱ�� */

/* ����ΪOS������� */
#ifdef WIN32
#include <stdio.h>
#include <malloc.h>
/* ȫ�ֱ������� */
#endif

#ifndef WIN32 /* ��windowsȱʡ����һЩ���Ա�ͨ������ */
#endif

INT LON_Token; /*Boolean Representing Which Side Has The Tokne */
INT LON_DataAvail; /* Slave Pass Data (Token = 0;Data = 1) */
INT LON_WriteReady; /* Master Ready  Data Complete (No Complete=0; Complete=1) */
/* LON���������ݻ����� */
UNSIGNED_CHAR LON_TxdBufferRem[LC_COMMAND_FRAME_SIZE];
/* LON���������ݻ����� */
UNSIGNED_CHAR LON_RxdBufferRem[LC_COMMAND_FRAME_SIZE];
extern volatile UNSIGNED TMD_System_Clock;

VOID LON_Wait_HS( VOID );
VOID LON_Master_Init( VOID );
INT LON_Pio_Read( UNSIGNED_CHAR  * data );
INT LON_Pio_Write( UNSIGNED_CHAR * data, INT size );

VOID LON_Wait_HS( VOID )
{
    UNSIGNED uDelay;

    uDelay = TMD_System_Clock;
    while(* pLON_CONTROL & _HS_MASK )
    {
        if(( TMD_System_Clock - uDelay ) >= MAX_WAIT_TIME_HS )
        {
            break;
        }
    }
}

VOID LON_Master_Init( VOID )
{
    INT k;

    for( k = 0; k < MAX_WAIT_CMD_ACKSYNC; k ++ )
    {
        LON_Wait_HS();
        * pLON_DATA = _CMD_RESYNC;

        LON_Wait_HS();
        * pLON_DATA = 0;

        LON_Wait_HS();
        if((* pLON_DATA & 0xff ) == _CMD_ACKSYNC )
        {
            LON_Wait_HS();
            break;
        }
    }
}

INT LON_Pio_Read( UNSIGNED_CHAR  * data )
{
    INT k;
    INT size;
    UNSIGNED_CHAR * pDst;

    size = 0;
    pDst = data;
    if((* pLON_CONTROL & _HS_MASK ) == 0 )
    {
        if((* pLON_DATA & 0xff ) != 0 )
        {
            LON_Wait_HS();
            size = (* pLON_DATA & 0xff );
            for( k = 0; k < size; k ++ )
            {
                LON_Wait_HS();
                * pDst ++ = ( UNSIGNED_CHAR )(* pLON_DATA & 0xff );
            }
        }
        LON_Wait_HS();
    }
    return( size );
}

INT LON_Pio_Write( UNSIGNED_CHAR * data, INT size )
{
    INT k;
    UNSIGNED_CHAR * pSrc;

    pSrc = data;

    * pLON_DATA = _CMD_XFER;
    LON_Wait_HS();

    * pLON_DATA = size;
    LON_Wait_HS();

    for( k = 0; k < size; k ++)
    {
        * pLON_DATA = * pSrc ++;
        LON_Wait_HS();
    }

    * pLON_DATA = 0;
    LON_Wait_HS();

    return( 0 );
}
