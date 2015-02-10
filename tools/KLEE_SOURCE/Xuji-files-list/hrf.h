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
/*      hrf.h                                           Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      HRF - History Record File                                        */
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
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhideYang.      11-05-2011      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#ifndef _HRF_H
#define _HRF_H

/*
 * These typedefs need to be handled better.
 */
typedef unsigned int   HRF32_WORD;      /* Unsigned large integer */
typedef unsigned short HRF32_HALF;      /* Unsigned medium integer  */

#define HRFVERSION     0                /* Version */
#define HRF_IDENT      16               /* Size of r_ident[] */
#define HRF_SH_NAME    16               /* Size of sh_name[] */
#define HRF_MAX_HANDLE 32               /* max handle number */

/* 数据编码类型定义r_type */
#define HRF_DATA_NONE  0                /* invalid */
#define HRF_DATA2_LSB  1                /* Little-Endian */
#define HRF_DATA2_MSB  2                /* Big-Endian */

/* HRF Header */
typedef struct HRFHDR{
    unsigned char r_ident[HRF_IDENT];   /* HRF Identification */
    HRF32_WORD  r_type;                 /* history record file type */
    HRF32_WORD  r_version;              /* history record file version */
    HRF32_WORD  r_shoff;                /* section header table offset */
    HRF32_HALF  r_shnum;                /* number of section header entries */
    HRF32_HALF  r_shentsize;            /* section header entry size */
} HRF32_RHDR;

/* Section Header */
typedef struct {
    unsigned char sh_name[HRF_SH_NAME]; /* section header name */
    HRF32_WORD  sh_type;                /* type */
    HRF32_WORD  sh_offset;              /* file offset */
    HRF32_WORD  sh_size;                /* section size */
    HRF32_WORD  sh_entsize;             /* section entry size */
    HRF32_WORD  sh_info;                /* extra information */
} HRF32_SHDR;

/* Handle information */
typedef struct {
    int  h_ready;                      /* ready flag */
    int  h_cpu;                        /* cpu number */
    HRF32_HALF  h_max_shnum;           /* max sector number */
    HRF32_HALF  h_shnum;               /* sector number */
    int  h_max_shsize;                 /* max sector size */
    HRF32_WORD h_version;              /* history record file version */
    unsigned char * h_mem;             /* file memory */
    HRF32_SHDR * h_shmem;              /* section header memory */
    HRF32_WORD h_soffset;              /* section offset */
} HRF32_HANDLE;

/* Section names */
#define HRF_REPORT     "report"         /* report data */
#define HRF_SOE        ".soe"           /* soe data */
#define HRF_R_VALUE    ".rvalue"        /* report value data */

/* 创建操作对象 */
unsigned long Hrf_Create( int cpuNumber, int maxSectNumber, int maxSectSize,
    int version  );
/* 删除操作对象 */
int Hrf_Delete( unsigned long handle );
/* 记录数据表节区信息 */
int Hrf_RecordSectorInf( unsigned long handle, unsigned int sh_size,
    unsigned char * sh_data, int sh_type, unsigned char * sh_name,
    int sh_info, unsigned int sh_entsize );
/* 记录报告节区信息 */
int Hrf_RecordSectorInf_Report( unsigned long handle, unsigned int sectSize,
    unsigned char * sectInf, int sectType, int cpuNumber );
/* 记录数据表节区信息 */
int Hrf_RecordSectorInf_Table( unsigned long handle, unsigned int sectSize,
    unsigned char * sectInf, int tableId, unsigned char * tableName,
    int cpuNumber, unsigned int tab_size );
/* 保存节区头部信息 */
int Hrf_RecordSectorHead( unsigned long handle, unsigned char * identName );
/* 清除记录信息 */
int Hrf_ClearRecordInf( unsigned long handle );
/* 获取记录信息 */
int Hrf_GetRecordInf( unsigned long handle, unsigned char ** buffer,
    unsigned long * size );
/* 读取节区信息，返回读取信息字节数 */
int Hrf_GetSectInf( unsigned char * filebuf, int shIndex, int offset,
    int size, unsigned char * buffer );
/* 获取节区头个数 */
int Hrf_GetSectHeadCount( unsigned char * filebuf );

#endif /* _HRF_H */
