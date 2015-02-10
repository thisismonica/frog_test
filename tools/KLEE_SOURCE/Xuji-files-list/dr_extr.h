/*************************************************************************/
/*                                                                       */
/*        Copyright (c) 1999-2002 XJ ELECTRIC CO.LTD.                    */
/*                                                                       */
/* PROPRIETARY RIGHTS of Accelerated Technology are involved in the      */
/* subject matter of this material.  All manufacturing, reproduction,    */
/* use, and sales rights pertaining to this subject matter are governed  */
/* by the license agreement.  The recipient of this software implicitly  */
/* accepts the terms of the license.                                     */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*                                                                       */
/* FILE NAME                                            VERSION          */
/*                                                                       */
/*      dr_extr.h                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      DR - Driver Management                                           */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains function prototypes of all functions          */
/*      accessible to other components.                                  */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* DATA STRUCTURES                                                       */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* FUNCTIONS                                                             */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhideYang.      02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"

/* Check to see if the file has been included already.  */

#ifndef DR_EXTR_H
#define DR_EXTR_H

/* �豸��ṹ */
typedef struct DRIVER_Class_struct CDRIVER;
/* �豸��ṹ */
struct DRIVER_Class_struct{
    /*private: */

    /*public: */
    /* ���豸���� */
    INT (* Open )( VOID * driver, UNSIGNED TimeOut );
    /* �ر��豸���� */
    INT (* Close )( VOID * driver );
    /* ��ʼ���豸���� */
    INT (* Initialize )( VOID * driver, UNSIGNED Id, UNSIGNED TimeOut );
    /* ���豸���� */
    INT (* Read )( VOID * driver, UNSIGNED Id, VOID * Buffer, UNSIGNED Offset,
        UNSIGNED Size, UNSIGNED Index, UNSIGNED TimeOut );
    /* д�豸���� */
    INT (* Write )( VOID * driver, UNSIGNED Id, VOID * Buffer, UNSIGNED Offset,
        UNSIGNED Size, UNSIGNED TimeOut );
    /* �豸״̬ */
    INT (* Status )( VOID * Driver, VOID * Buffer, UNSIGNED * Actual_Number,
        UNSIGNED Type, UNSIGNED TimeOut );
};

/* �������ϰ汾�����Բ��Զ��� */
extern CDRIVER CDriver;
#define OPENDRIVER(x,y,z) x = CDriver.Open(y,z)                     /* ���豸 */
#define CLOSEDRIVER(x) CDriver.Close(x)                             /* �ر��豸 */
#define INITIALIZEDRIVER(x,y,z,w) x = CDriver.Initialize(y,z,w)     /* ��ʼ���豸 */
#define READDRIVER(x,y,z,u,v,w,m,n) x = CDriver.Read(y,z,u,v,w,m,n) /* ���豸 */
#define WRITEDRIVER(x,y,z,u,v,w,m) x = CDriver.Write(y,z,u,v,w,m)   /* д�豸 */
#define STATUSDRIVER(x,y,z,u,v,w) x = CDriver.Status(y,z,u,v,w)     /* �豸״̬ */

/*  Initialization functions.  */

/* Error checking for supplemental functions.  */

/* Core processing functions.  */

#endif
