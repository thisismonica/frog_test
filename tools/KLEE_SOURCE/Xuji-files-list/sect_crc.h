
/* Check to see if this file has been included already.  */
#ifndef SECT_CRC_H
#define SECT_CRC_H

#define SECT_FILE_NAME          20  /* 文件名称长度(包含路径) */
#define SECT_NAME               20  /* 段名称 */

typedef struct SECT_CRC_STRUCT
{
    char            fileName[ SECT_FILE_NAME ];         /* 文件名称 */
    char            sectName[ SECT_NAME ];              /* 段名称 */
    unsigned long   startAddress;                       /* 起始地址 */
    unsigned long   length;                             /* 数据字节数 */
    unsigned long   crc_code;                           /* CRC 码 */
    int             errYear;                            /* 错误时间：年 */
    int             errMonth;                           /* 错误时间：月 */
    int             errDay;                             /* 错误时间：日 */
    int             errHour;                            /* 错误时间：时 */
    int             errMinute;                          /* 错误时间：分 */
    int             errMillionsecond;                   /* 错误时间：毫秒 */
}SECT_CRC;

typedef struct SECT_INF_STRUCT
{
    char            fileName[ SECT_FILE_NAME ];         /* 文件名称 */
    int             sectNumber;                         /* 段数据总个数 */
    SECT_CRC *      sectMemory;                         /* 段名称 */
}SECT_INF;

/* 初始化校验数据段信息 */
int SECT_Initialize( void );
/* 获取数据段信息 */
int SECT_GetSectInformation( SECT_INF * sectInf );
/* 数据段检验码检查 */
int SECT_CrcCheckout( int CheckFlag );

#endif
