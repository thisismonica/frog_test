
/* Check to see if this file has been included already.  */
#ifndef SECT_CRC_H
#define SECT_CRC_H

#define SECT_FILE_NAME          20  /* �ļ����Ƴ���(����·��) */
#define SECT_NAME               20  /* ������ */

typedef struct SECT_CRC_STRUCT
{
    char            fileName[ SECT_FILE_NAME ];         /* �ļ����� */
    char            sectName[ SECT_NAME ];              /* ������ */
    unsigned long   startAddress;                       /* ��ʼ��ַ */
    unsigned long   length;                             /* �����ֽ��� */
    unsigned long   crc_code;                           /* CRC �� */
    int             errYear;                            /* ����ʱ�䣺�� */
    int             errMonth;                           /* ����ʱ�䣺�� */
    int             errDay;                             /* ����ʱ�䣺�� */
    int             errHour;                            /* ����ʱ�䣺ʱ */
    int             errMinute;                          /* ����ʱ�䣺�� */
    int             errMillionsecond;                   /* ����ʱ�䣺���� */
}SECT_CRC;

typedef struct SECT_INF_STRUCT
{
    char            fileName[ SECT_FILE_NAME ];         /* �ļ����� */
    int             sectNumber;                         /* �������ܸ��� */
    SECT_CRC *      sectMemory;                         /* ������ */
}SECT_INF;

/* ��ʼ��У�����ݶ���Ϣ */
int SECT_Initialize( void );
/* ��ȡ���ݶ���Ϣ */
int SECT_GetSectInformation( SECT_INF * sectInf );
/* ���ݶμ������� */
int SECT_CrcCheckout( int CheckFlag );

#endif
