/***********************************************************************
*
*            Copyright 1993 Mentor Graphics Corporation
*                         All Rights Reserved.
*
* THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS
* THE PROPERTY OF MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS
* SUBJECT TO LICENSE TERMS.
*
************************************************************************

************************************************************************
*
*   FILE NAME
*
*       zc_extr.h
*
*   COMPONENT
*
*       ZC - Zero Copy
*
*   DESCRIPTION
*
*       This file provides the external application interface to the
*       Nucleus Zero Copy components
*
*   DATA STRUCTURES
*
*       None
*
*   DEPENDENCIES
*
*       nucleus.h
*
***********************************************************************/

/* Include required header files */
#include    "plus/nucleus.h"

#ifdef          __cplusplus

/* C declarations in C++     */
extern          "C" {

#endif

#ifndef     ZC_EXTR_H
#define     ZC_EXTR_H

/* Defines for beginning and end of Zero Copy Buffers.  These constants can be used to
   address the first byte (ZC_BUF_BEGIN) or last byte (ZC_BUF_END) within a buffer
   without the necessity of knowing the segment ID (segment ID ignored when these
   constants are used for offsets).  These constants reduce the maximum usable positive
   offsets by 2 */
#define     ZC_BUF_BEGIN            (INT)((UINT32)(-1) >> 1)
#define     ZC_BUF_END              (INT)(ZC_BUF_BEGIN - 1)

/* Define types for zero-copy buffer and segment IDs */
typedef     UINT32                  ZC_BUF_ID;
typedef     UINT32                  ZC_SEG_ID;

/* Define for no segment after removed data (only used
   by NU_ZC_Data_Remove API) */
#define     ZC_SEG_NONE             (ZC_SEG_ID)(0xFFFFFFFFUL)

/* Map NU_ function calls to appropriate zero copy functions
   (error checking or no error checking) */
#if (NU_ERROR_CHECKING == NU_TRUE)

/* Map NU_ functions to error checking functions */
#define     NU_ZC_Initialize                            ZCE_Initialize
#define     NU_ZC_Buf_Create                            ZCE_Buf_Create
#define     NU_ZC_Buf_Delete                            ZCE_Buf_Delete
#define     NU_ZC_Buf_Insert                            ZCE_Buf_Insert
#define     NU_ZC_Buf_Split                             ZCE_Buf_Split
#define     NU_ZC_Buf_Duplicate                         ZCE_Buf_Duplicate
#define     NU_ZC_Data_Insert                           ZCE_Data_Insert
#define     NU_ZC_Data_Copy_Insert                      ZCE_Data_Copy_Insert
#define     NU_ZC_Data_Remove                           ZCE_Data_Remove
#define     NU_ZC_Buf_Len_Get                           ZCE_Buf_Len_Get
#define     NU_ZC_Data_Copy_Get                         ZCE_Data_Copy_Get
#define     NU_ZC_Seg_Optimal_Get                       ZCE_Seg_Optimal_Get
#define     NU_ZC_Seg_Next_Get                          ZCE_Seg_Next_Get
#define     NU_ZC_Seg_Prev_Get                          ZCE_Seg_Prev_Get
#define     NU_ZC_Seg_Data_Get                          ZCE_Seg_Data_Get
#define     NU_ZC_Seg_Len_Get                           ZCE_Seg_Len_Get

#else

/* Map NU_ functions directly to zero copy functions */
#define     NU_ZC_Initialize                            ZC_Initialize
#define     NU_ZC_Buf_Create                            ZC_Buf_Create
#define     NU_ZC_Buf_Delete                            ZC_Buf_Delete
#define     NU_ZC_Buf_Insert                            ZC_Buf_Insert
#define     NU_ZC_Buf_Split                             ZC_Buf_Split
#define     NU_ZC_Buf_Duplicate                         ZC_Buf_Duplicate
#define     NU_ZC_Data_Insert                           ZC_Data_Insert
#define     NU_ZC_Data_Copy_Insert                      ZC_Data_Copy_Insert
#define     NU_ZC_Data_Remove                           ZC_Data_Remove
#define     NU_ZC_Buf_Len_Get                           ZC_Buf_Len_Get
#define     NU_ZC_Data_Copy_Get                         ZC_Data_Copy_Get
#define     NU_ZC_Seg_Optimal_Get                       ZC_Seg_Optimal_Get
#define     NU_ZC_Seg_Next_Get                          ZC_Seg_Next_Get
#define     NU_ZC_Seg_Prev_Get                          ZC_Seg_Prev_Get
#define     NU_ZC_Seg_Data_Get                          ZC_Seg_Data_Get
#define     NU_ZC_Seg_Len_Get                           ZC_Seg_Len_Get

#endif  /* NU_ERROR_CHECKING == NU_TRUE */

/* Define function prototypes for Zero Copy ACTIVE functions.  ACTIVE
   functions are functions that cause the internal data within the
   zero copy software to be modified. */
STATUS      NU_ZC_Initialize        (NU_MEMORY_POOL *   os_pool_ptr,
                                     INT                num_zc_objects);
ZC_BUF_ID   NU_ZC_Buf_Create        (VOID);

STATUS      NU_ZC_Buf_Delete        (ZC_BUF_ID          delete_zc_buf);

ZC_SEG_ID   NU_ZC_Buf_Insert        (ZC_BUF_ID          dst_zc_buf,
                                     ZC_SEG_ID          dst_zc_seg,
                                     INT                dst_byte_offset,
                                     ZC_BUF_ID          src_zc_buf);

ZC_BUF_ID   NU_ZC_Buf_Split         (ZC_BUF_ID          split_zc_buf,
                                     ZC_SEG_ID          split_zc_seg,
                                     INT                split_byte_offset);

ZC_BUF_ID   NU_ZC_Buf_Duplicate     (ZC_BUF_ID          dup_zc_buf,
                                     ZC_SEG_ID          dup_zc_seg,
                                     INT                dup_byte_offset,
                                     INT                dup_len);

ZC_SEG_ID   NU_ZC_Data_Insert       (ZC_BUF_ID          dst_zc_buf,
                                     ZC_SEG_ID          dst_zc_seg,
                                     INT                dst_byte_offset,
                                     CHAR *             src_data_ptr,
                                     INT                src_data_len,
                                     VOID               (*cb_func)(CHAR *, INT),
                                     INT                cb_param);

ZC_SEG_ID   NU_ZC_Data_Copy_Insert  (ZC_BUF_ID          dst_zc_buf,
                                     ZC_SEG_ID          dst_zc_seg,
                                     INT                dst_byte_offset,
                                     CHAR *             src_data_ptr,
                                     INT                src_data_len);

ZC_SEG_ID   NU_ZC_Data_Remove       (ZC_BUF_ID          remove_zc_buf,
                                     ZC_SEG_ID          remove_zc_seg,
                                     INT                remove_byte_offset,
                                     INT                remove_data_len);

/* Define function prototypes for Zero Copy PASSIVE functions.  PASSIVE
   functions are functions that cause no changes to the internal data
   within the zero copy software. */
INT         NU_ZC_Buf_Len_Get       (ZC_BUF_ID          zc_buf);

INT         NU_ZC_Data_Copy_Get     (ZC_BUF_ID          src_zc_buf,
                                     ZC_SEG_ID          src_zc_seg,
                                     INT                src_byte_offset,
                                     CHAR *             dst_data_ptr,
                                     INT                dst_data_len);

ZC_SEG_ID   NU_ZC_Seg_Optimal_Get   (ZC_BUF_ID          zc_buf,
                                     ZC_SEG_ID          zc_seg,
                                     INT *              byte_offset);

ZC_SEG_ID   NU_ZC_Seg_Next_Get      (ZC_BUF_ID          zc_buf,
                                     ZC_SEG_ID          zc_seg);

ZC_SEG_ID   NU_ZC_Seg_Prev_Get      (ZC_BUF_ID          zc_buf,
                                     ZC_SEG_ID          zc_seg);

CHAR *      NU_ZC_Seg_Data_Get      (ZC_BUF_ID          zc_buf,
                                     ZC_SEG_ID          zc_seg);

INT         NU_ZC_Seg_Len_Get       (ZC_BUF_ID          zc_buf,
                                     ZC_SEG_ID          zc_seg);

#ifdef          __cplusplus

/* End of C declarations */
}

#endif  /* __cplusplus */

#endif      /* ZC_EXTR_H */
