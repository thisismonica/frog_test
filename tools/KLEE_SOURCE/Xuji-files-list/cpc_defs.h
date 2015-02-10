/*************************************************************************/
/*                                                                       */
/*        Copyright (c) 1999-2002 XJ ELECTRIC CO.LTD.                    */
/*                                                                       */
/* PROPRIETARY RIGHTS of XJ ELECTRIC CO.LTD are involved in the          */
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
/*      cpc_defs.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      CPC - Common Protect Control                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains data structure definitions and constants for  */
/*      the Common Protect control component.                            */
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
/*      ZhiDeYang       02-20-2003      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"
#include "plat/inc/pro_defs.h"

/* Check to see if the file has been included already.  */

#ifndef CPC_DEFS_H
#define CPC_DEFS_H

/* Define unused parameter */
#define CPC_UNUSED_PARAM(parameter)   ((VOID)parameter)

/* ��ȡ�ṹ��Ա��ַƫ��x:�ṹ����,y:��Ա���� */
#define GETSTRUCTOFFSET(x,y) (int)(&((##x*)0)->##y)

/* ���������ַ�ռ� */
#define CPC_DRV_TEXT_ADDR               PLAT_DRV_TEXT_ADDR                  /* ����������ʼ��ַ */
#define CPC_DRV_TEXT_SIZE               PLAT_DRV_TEXT_SIZE                  /* ������������ַ�ռ� */

/* ���������ַ�ռ� */
#define CPC_PROTECT_TEXT_ADDR           PLAT_PROTECT_TEXT_ADDR              /* ����������ʼ��ַ */
#define CPC_PROTECT_TEXT_SIZE           PLAT_PROTECT_TEXT_SIZE              /* ������������ַ�ռ� */

/* HMI�����ַ�ռ� */
#define CPC_HMI_TEXT_ADDR               PLAT_HMI_TEXT_ADDR                  /* HMI������ʼ��ַ */
#define CPC_HMI_TEXT_SIZE               PLAT_HMI_TEXT_SIZE                  /* HMI��������ַ�ռ� */

/* Gws61850�����ַ�ռ� */
#define CPC_GWS_TEXT_ADDR               PLAT_GWS_TEXT_ADDR                  /* Gws61850������ʼ��ַ */
#define CPC_GWS_TEXT_SIZE               PLAT_GWS_TEXT_SIZE                  /* Gws61850��������ַ�ռ� */

/* NPI�����ַ�ռ� */
#define CPC_NPI_TEXT_ADDR               PLAT_NPI_TEXT_ADDR                  /* NPI������ʼ��ַ */
#define CPC_NPI_TEXT_SIZE               PLAT_NPI_TEXT_SIZE                  /* NPI��������ַ�ռ� */

/* װ�÷�ģ����� */
#define CPC_MAX_MODULE_NUMBER           PLAT_MAX_MODULE_NUMBER              /* װ������ģ����� */

/* ��NPIͨѶ������Ϣ */
#define CPC_MAX_LENGTH_FRAME            PLAT_MAX_LENGTH_FRAME               /* ���֡�� */
#define CPC_MAX_USER_FRAME_NUMBER       PLAT_MAX_USER_FRAME_NUMBER          /* ���������û�����֡���� */

/* Ӳ����Դ���� */
#define CPC_MAX_EVENT_RECORD            PLAT_MAX_EVENT_RECORD               /* ����¼����� */
#define CPC_MAX_AD_NUMBER               PLAT_MAX_AD_NUMBER                  /* ���A/DƬ�� */
#define CPC_MAX_PHYSICAL_CHL_NUMBER     PLAT_MAX_PHYSICAL_CHL_NUMBER        /* ��������ͨ������ */
#define CPC_MAX_VIRTUAL_CHL_NUMBER      PLAT_MAX_VIRTUAL_CHL_NUMBER         /* ������ͨ������ */
#define CPC_MAX_CHL_NUMBER              PLAT_MAX_CHL_NUMBER                 /* ���ģ����ͨ������������+��ͨ���� */
#define CPC_MAX_SAMPLE_POINT            PLAT_MAX_SAMPLE_POINT               /* ���������� */
#define CPC_MAX_CHANNEL_POINT           PLAT_MAX_CHANNEL_POINT              /* ÿͨ������������ */
#define CPC_MAX_SAMPLE_COUNTER          PLAT_MAX_SAMPLE_COUNTER             /* ������ŵ����ֵ */
#define CPC_MAX_SAMPLE_COUNTER_OLT      PLAT_MAX_SAMPLE_COUNTER_OLT         /* ���˲�����ŵ����ֵ */
#define CPC_MAX_SWITCH_STATUS_POINT     PLAT_MAX_SWITCH_STATUS_POINT        /* ÿ�鿪������λ������¼���� */
#define CPC_MAX_PROTECT_GROUPS          PLAT_MAX_PROTECT_GROUPS             /* ��󱣻�״̬������ */
#define CPC_MAX_INPUT_GROUPS            PLAT_MAX_INPUT_GROUPS               /* ����������� */
#define CPC_MAX_WAVE_INPUT_GROUPS       PLAT_MAX_WAVE_INPUT_GROUPS          /* ���¼������������ */
#define CPC_MAX_OUTPUT_GROUPS           PLAT_MAX_OUTPUT_GROUPS              /* ��󿪳������� */
#define CPC_MAX_CHECK_RELAY             PLAT_MAX_CHECK_RELAY                /* �����ڼ���� */
#define CPC_MAX_PROTECT_LOG_GROUPS      PLAT_MAX_PROTECT_LOG_GROUPS         /* ����߼�״̬������ */
#define CPC_MAX_STATE_GROUPS            PLAT_MAX_STATE_GROUPS               /* ���¼��״̬���������� */
#define CPC_MAX_WAVE_DATA_SIZE          PLAT_MAX_WAVE_DATA_SIZE             /* ���¼�����ݵ��� */
#define CPC_MAX_RAM_WAVE_DATA_NUMBER    PLAT_MAX_RAM_WAVE_DATA_NUMBER       /* ����ڴ滺��¼�����ݸ��� */
#define CPC_MAX_DEVICE_SYSTEM_NUM       PLAT_MAX_DEVICE_SYSTEM_NUM          /* ���װ��Ӧ�ó�����Ŀ */
#define CPC_MAX_NVRAM_REPORT_NUMBER     PLAT_MAX_NVRAM_REPORT_NUMBER        /* ���洢������� */
#define CPC_MAX_FAULT_REPORT_NUMBER     PLAT_MAX_FAULT_REPORT_NUMBER        /* �����ʹ��ϱ������ */
#define CPC_MAX_NVRAM_WAVE_NUMBER       PLAT_MAX_NVRAM_WAVE_NUMBER          /* ���洢¼����Ŀ */
#define CPC_MAX_NVRAM_EVENT_NUMBER      PLAT_MAX_NVRAM_EVENT_NUMBER         /* ���洢�¼���Ŀ */
#define CPC_MAX_RAM_REPORT_NUMBER       PLAT_MAX_RAM_REPORT_NUMBER          /* ����ڴ滺�汨����Ŀ */
#define CPC_MAX_RAM_EVENT_NUMBER        PLAT_MAX_RAM_EVENT_NUMBER           /* ����ڴ滺���¼���Ŀ */
#define CPC_MAX_ONE_FAULT_REPORT_NUMBER PLAT_MAX_ONE_FAULT_REPORT_NUMBER    /* һ�ι�����󱨸���� */
#define CPC_FCK_SOE_MAX_COUNT           PLAT_FCK_SOE_MAX_COUNT              /* ֧�ֵ������SOE�ĸ��� */
#define CPC_READ_COMMAND_MAX_COUNT      PLAT_READ_COMMAND_MAX_COUNT         /* ֧�ֵ�����ض�����ĸ��� */

/* ��HMIͨ�Ŷ˿����� */
#define CPC_LINK_HMI_PHY_COM            PLAT_LINK_HMI_PHY_COM               /* ��HMI֮���ͨ�Ŷ˿�����(1:�����,0:����) */

/* ���Ź�ʹ��ѡ�� */
#define CPC_WATCHDOG_ENABLE             PLAT_WATCHDOG_ENABLE                /* ���Ź�ʹ�� */

/* �����¼����� */
#define CPC_ERROR_EVENT_CB              PLAT_ERROR_EVENT_CB                 /* �����¼�λ��� */

/* ��1�¼��飨������Ϣ����λ���� */
#define CPC_EVENT1_ERR_SRAM             Error_SRAM                          /* EEPROM����(1 << 0) */
#define CPC_EVENT1_ERR_RAM              Error_RAM                           /* RAM����(1 << 1) */
#define CPC_EVENT1_ERR_SETTING          Error_Setting                       /* ��ֵ����(1 << 2) */
#define CPC_EVENT1_ERR_RELAY            Error_Relay                         /* �̵�������(1 << 3) */
#define CPC_EVENT1_ERR_5VPOWER          Error_5VPower                       /* ��Դ����(1 << 4) */
#define CPC_EVENT1_ERR_CPU_NUMBER       Error_CPU_Number                    /* CPU��������(1 << 5) */
#define CPC_EVENT1_ERR_AD               Error_AD                            /* AD����(1 << 6) */
#define CPC_EVENT1_ERR_COMMU            Error_Commu                         /* ͨѶ����(1 << 7) */
#define CPC_EVENT1_ERR_FLASH            Error_FLASH                         /* FLASH����(1 << 8) */
#define CPC_EVENT1_ERR_PROGRAM          Error_Program                       /* ����У�����(1 << 9) */
#define CPC_EVENT1_ERR_STRIKE           Error_Relay_Close                   /* ������·����(1 << 10) */
#define CPC_EVENT1_ERR_EXTEND           Error_Extend_Port                   /* ��չ�������(1 << 11) */
#define CPC_EVENT1_CLOSE_RELAY          Close_Relay_Flag                    /* ��Դ�Լ����������ڱ�־(1 << 12) */
#define CPC_EVENT1_SAMPLE_OK            Sample_Data_OK                      /* ��������׼����(1 << 13) */
#define CPC_EVENT1_USER_ALARM           User_Alarm_Bits                     /* �û��澯λ(1 << 14) */
#define CPC_EVENT1_GWS_INIT_STATUS      GWS_InitStatus                      /* ���س�ʼ״̬(1 << 15) */

#define CPC_EVENT1_DEV_ERR (CPC_EVENT1_ERR_SRAM+CPC_EVENT1_ERR_RAM           \
                           +CPC_EVENT1_ERR_SETTING+CPC_EVENT1_ERR_RELAY      \
                           +CPC_EVENT1_ERR_5VPOWER+CPC_EVENT1_ERR_AD         \
                           +CPC_EVENT1_ERR_COMMU+CPC_EVENT1_ERR_FLASH        \
                           +CPC_EVENT1_ERR_EXTEND+CPC_EVENT1_USER_ALARM)

#define CPC_EVENT1_RUNLED_ABNORMAL (CPC_EVENT1_ERR_SRAM+CPC_EVENT1_ERR_RAM   \
                           +CPC_EVENT1_ERR_SETTING+CPC_EVENT1_ERR_RELAY      \
                           +CPC_EVENT1_ERR_5VPOWER+CPC_EVENT1_ERR_AD         \
                           +CPC_EVENT1_ERR_COMMU+CPC_EVENT1_ERR_FLASH        \
                           +CPC_EVENT1_ERR_EXTEND)

/* �¼�1ƽ̨�¼�λ */
#define CPC_EVENT1_PLAT_BITS    (CPC_EVENT1_ERR_SRAM+CPC_EVENT1_ERR_RAM            \
                                +CPC_EVENT1_ERR_SETTING+CPC_EVENT1_ERR_RELAY       \
                                +CPC_EVENT1_ERR_5VPOWER+CPC_EVENT1_ERR_CPU_NUMBER  \
                                +CPC_EVENT1_ERR_AD+CPC_EVENT1_ERR_COMMU            \
                                +CPC_EVENT1_ERR_FLASH+CPC_EVENT1_ERR_PROGRAM       \
                                +CPC_EVENT1_ERR_STRIKE+CPC_EVENT1_ERR_EXTEND       \
                                +CPC_EVENT1_CLOSE_RELAY+CPC_EVENT1_SAMPLE_OK)

/* ��2�¼����λ���� */
#define CPC_EVENT2_RESET                (1 << 0)                            /* �����źŵ� */
#define CPC_EVENT2_TRIP                 (1 << 1)                            /* ���ڴ��� */
#define CPC_EVENT2_EXCHANGE             (1 << 2)                            /* CPU���� ,����*/
#define CPC_EVENT2_EXCHANGE_R           (1 << 3)                            /* CPU���� ,����*/
#define CPC_EVENT2_WAVE                 (1 << 4)                            /* ¼����� */
#define CPC_EVENT2_WAVE_CLR             (1 << 5)                            /* ��¼������ */
#define CPC_EVENT2_SRAM_FISH            (1 << 6)                            /* SRAM д��� */
#define CPC_EVENT2_SRAM_ERROR           (1 << 7)                            /* дSRAM ���� */
#define CPC_EVENT2_INITIALIZE           (1 << 8)                            /* ������ʼ����� */
#define CPC_EVENT2_CLOCK_READY          (1 << 9)                            /* ʱ�ӳ�ʼ����� */
#define CPC_EVENT2_TASK_0               (1 << 10)                           /* task 0 */
#define CPC_EVENT2_TASK_1               (1 << 11)                           /* task 1 */
#define CPC_EVENT2_TASK_2               (1 << 12)                           /* task 2 */
#define CPC_EVENT2_TASK_3               (1 << 13)                           /* task 3 */
#define CPC_EVENT2_RESET_MAIN           (1 << 14)                           /* ���������� */
#define CPC_EVENT2_RESET_BACK           (1 << 15)                           /* ����󱸱��� */
#define CPC_EVENT2_SYN_ERR              (1 << 16)                           /* ͬ������ͨѶ���� */
#define CPC_EVENT2_PROG_READY           (1 << 17)                           /* ���س���׼������ */
#define CPC_EVENT2_CFG_READY            (1 << 18)                           /* ��������׼������ */
#define CPC_EVENT2_CFG_PROGRAM          (1 << 19)                           /* �̻��������� */
#define CPC_EVENT2_CFG_ERROR            (1 << 20)                           /* �̻��������ó��� */
#define CPC_EVENT2_CFG_MEMORY_READY     (1 << 21)                           /* ���ÿռ�����ɹ� */
#define CPC_EVENT2_NO_SLEEP             (1 << 22)                           /* �������������� */
#define CPC_EVENT2_NO_MEMORY            (1 << 23)                           /* �ڴ�ռ䲻�� */
#define CPC_EVENT2_VERSION_ERROR        (1 << 24)                           /* �汾���� */
#define CPC_EVENT2_PROTECT_READY        (1 << 25)                           /* װ��Ӧ�ó���ɹ� */
#define CPC_EVENT2_REPORT_CLR           (1 << 26)                           /* �屨������ */
#define CPC_EVENT2_HMI_READY            (1 << 27)                           /* װ��HMI����ɹ� */
#define CPC_EVENT2_GWS_READY            (1 << 28)                           /* װ��GWS����ɹ� */
#define CPC_EVENT2_DRV_READY            (1 << 29)                           /* װ����������ɹ� */

/* Ӳ��оƬ������ʱʱ�䶨�� */
#define CPC_DATA_TIMEOUT                800                                 /* ����ֵ���ݳ�ʱʱ�� */
#define CPC_CONFIG_TIMEOUT              800                                 /* �������ݳ�ʱʱ�� */
#define CPC_EVENT_TIMEOUT               800                                 /* �¼���ʱʱ�� */
#define CPC_REPORT_TIMEOUT              800                                 /* ���泬ʱʱ�� */
#define CPC_WAVE_TIMEOUT                800                                 /* ¼����ʱʱ�� */
#define CPC_OUTHOLD_TIMEOUT             800                                 /* ���ڱ���λ���ݳ�ʱʱ�� */

/* CRC����㳣������ */
#define CPC_CRC32_Initialize            0xffffffff                          /* 32λCRC��ĳ�ֵ */
#define CPC_CRC32_MASKS                 0xffffffff                          /* 32λCRC������� */
#define CPC_CRC32_BITS                  32                                  /* 32λCRC���λ�� */
#define CPC_RAM_DATA_BITS               32                                  /* RAM����λ�� */
#define CPC_CRC16_Initialize            0x0000                              /* 16λCRC��ĳ�ֵ */
#define CPC_CRC16_MASKS                 0x0000                              /* 16λCRC������� */
#define CPC_CRC16_BITS                  16                                  /* 16λCRC���λ�� */

/* �������� */
#define CPC_MAX_CPU_NUMBER              8                                   /* ���CPU���� */
#define CPC_DATA_MASKS8                 0xff                                /* 8λ�������� */
#define CPC_DATA_MASKS16                0xffff                              /* 16λ�������� */
#define CPC_DATA_MASKS32                0xffffffff                          /* 32λ�������� */
#define CPC_DATA_MASKSH16               0xffff0000                          /* ��16λ�������� */
#define CPC_DATA_BITS8                  8                                   /* 8λ����λ�� */
#define CPC_DATA_BITS16                 16                                  /* 16λ����λ�� */
#define CPC_DATA_BITS32                 32                                  /* 32λ����λ�� */
#define CPC_GROUP_MASK32                5                                   /* 32λ������ŵ�����λ */
#define CPC_SUB_GROUP_MASK32            31                                  /* 32λ����ֵ������λ */
#define CPC_DATA_FLAG                   0x5a                                /* ���ݱ�־ */
#define CPC_BYTE_BITS                   8                                   /* �ֽ�λ�� */
#define CPC_TEXT_DATA_BYTE_NUM          (4/sizeof(UNSIGNED))                /* ���������ַ���ֽ��� */
#define CPC_RAM_DATA_BYTE_NUM           (4/sizeof(UNSIGNED))                /* �������ݵ�ַ���ֽ��� */
#define CPC_UNIT_LENGTH                 16                                  /* the max length of one unit data */
#define CPC_DISABLE_DISPLAY             0                                   /* ��ֹ��ʾ���˳��� */
#define CPC_CHANNEL_NOT_ACTIVE          -1                                  /* δʹ��ͨ����־ */
#define CPC_WORD_BYTES                  4                                   /* WORD���ֽ��� */

/* �ڲ��¼����Ͷ��� */
#define CPC_RES_EVENT_START             1                                   /* ���������̵��� */
#define CPC_RES_EVENT_TRIP              2                                   /* ������բ���� */
#define CPC_RES_EVENT_OUT               3                                   /* �������� */
#define CPC_RES_EVENT_REPAIR            4                                   /* �����Լ���� */
#define CPC_RES_EVENT_EEPROM            5                                   /* дEEPROM���� */
#define CPC_RES_EVENT_FLASH             6                                   /* дFLASH���� */
#define CPC_RES_EVENT_ERASE             7                                   /* FLASH������������ */
#define CPC_RES_EVENT_SIZE              8                                   /* ���ݴ�С���� */
#define CPC_RES_EVENT_RESTART           9                                   /* װ�������ϵ� */
#define CPC_RES_EVENT_RELAY             10                                  /* �̵������� */
#define CPC_RES_EVENT_CLOSE             11                                  /* �̵���˲ʱ���� */
#define CPC_RES_EVENT_CLOSE_EXT         12                                  /* ��չ��·˲ʱ���� */
#define CPC_RES_EVENT_REPORT_CLR        13                                  /* ������� */
#define CPC_RES_EVENT_WAVE_CLR          14                                  /* ���¼�� */
#define CPC_RES_EVENT_ZONE_CHANGE       15                                  /* ��ֵ���仯 */
#define CPC_RES_EVENT_CONFIG_CHANGE     16                                  /* ���ñ仯 */

/* Ŀ¼·������ */
#if 0
#define CPC_USER_ROOT_DIR_NAME          "/ram"                              /* �û����ݸ�Ŀ¼·������ */
#define CPC_ALARM_DIR_NAME              "/ram/palarm"                       /* �澯�¼��洢·�� */
#define CPC_REVERSE_DIR_NAME            "/ram/preserve"                     /* �ڲ��¼��洢·�� */
#define CPC_USER1_DIR_NAME              "/ram/puser1"                       /* �û��¼�1�洢·�� */
#define CPC_USER2_DIR_NAME              "/ram/puser2"                       /* �û��¼�2�洢·�� */
#define CPC_USER3_DIR_NAME              "/ram/puser3"                       /* �û��¼�3�洢·�� */
#define CPC_USER4_DIR_NAME              "/ram/puser4"                       /* �û��¼�4�洢·�� */
#define CPC_USER5_DIR_NAME              "/ram/puser5"                       /* �û��¼�5�洢·�� */
#define CPC_USER6_DIR_NAME              "/ram/puser6"                       /* �û��¼�6�洢·�� */
#define CPC_USER7_DIR_NAME              "/ram/puser7"                       /* �û��¼�7�洢·�� */
#define CPC_OPERATE_DIR_NAME            "/ram/poperate"                     /* �����¼��洢·�� */
#define CPC_REPORT_DIR_NAME             "/ram/preport"                      /* ����洢·�� */
#define CPC_WAVE_DIR_NAME               "/ram/pwave"                        /* ¼���洢·�� */
#define CPC_SETTING_DATA_DIR_NAME       "/ram/psetting"                     /* ����ֵ���ݴ洢·�� */
#define CPC_CONFIG_DATA_DIR_NAME        "/ram/pconfig"                      /* �������ݴ洢·�� */
#define CPC_TEXT_CRC_DIR_NAME           "/ram/ptextcrc"                     /* ����洢·�� */
#define CPC_OUTPUTHOLD_DATA_DIR_NAME    "/ram/poutputhold"                  /* ���ڱ���λ�洢·�� */
#define CPC_LOG_DIR_NAME                "/ram/plog"                         /* ��־��Ϣ�洢·�� */
#define CPC_CMS_DIR_NAME                "/ram/pcommsg"                      /* ͨ�ű�����Ϣ�洢·�� */
#else
#define CPC_USER_ROOT_DIR_NAME          "/user"                              /* �û����ݸ�Ŀ¼·������ */
#define CPC_ALARM_DIR_NAME              "/user/palarm"                       /* �澯�¼��洢·�� */
#define CPC_REVERSE_DIR_NAME            "/user/preserve"                     /* �ڲ��¼��洢·�� */
#define CPC_USER1_DIR_NAME              "/user/puser1"                       /* �û��¼�1�洢·�� */
#define CPC_USER2_DIR_NAME              "/user/puser2"                       /* �û��¼�2�洢·�� */
#define CPC_USER3_DIR_NAME              "/user/puser3"                       /* �û��¼�3�洢·�� */
#define CPC_USER4_DIR_NAME              "/user/puser4"                       /* �û��¼�4�洢·�� */
#define CPC_USER5_DIR_NAME              "/user/puser5"                       /* �û��¼�5�洢·�� */
#define CPC_USER6_DIR_NAME              "/user/puser6"                       /* �û��¼�6�洢·�� */
#define CPC_USER7_DIR_NAME              "/user/puser7"                       /* �û��¼�7�洢·�� */
#define CPC_OPERATE_DIR_NAME            "/user/poperate"                     /* �����¼��洢·�� */
#define CPC_REPORT_DIR_NAME             "/user/preport"                      /* ����洢·�� */
#define CPC_WAVE_DIR_NAME               "/user/pwave"                        /* ¼���洢·�� */
#define CPC_SETTING_DATA_DIR_NAME       "/user/psetting"                     /* ����ֵ���ݴ洢·�� */
#define CPC_CONFIG_DATA_DIR_NAME        "/user/pconfig"                      /* �������ݴ洢·�� */
#define CPC_TEXT_CRC_DIR_NAME           "/user/ptextcrc"                     /* ����洢·�� */
#define CPC_OUTPUTHOLD_DATA_DIR_NAME    "/user/poutputhold"                  /* ���ڱ���λ�洢·�� */
#define CPC_LOG_DIR_NAME                "/user/plog"                         /* ��־��Ϣ�洢·�� */
#define CPC_CMS_DIR_NAME                "/user/pcommsg"                      /* ͨ�ű�����Ϣ�洢·�� */
#endif
#ifdef WIN32
#define CPC_CFG_FILE_NAME               "/user/plat_cfg"                      /* �����ļ����� */
#else
#define CPC_CFG_FILE_NAME               "/user/plat_cfg.xml"                  /* �����ļ����� */
#endif
#ifdef WIN32
#define CPC_BACKGROUND_DATA_FILE_NAME   "/bin"                               /* ���������ļ�Ŀ¼ */
#else
#define CPC_BACKGROUND_DATA_FILE_NAME   "/bin/npu808bgd.dat"                 /* ���������ļ��� */
#endif
/* ���ѭ������,x:�������,y:����,z:���� */
#define CHECKCYCDATALIMIT(x,y,z)                \
  {   \
    if(x > z) x = y;                            \
    if(x < y) x = z;                            \
  }

/* ����̵���������������־ */
#define CPC_RESET_START_FLAG            PLAT_RESET_START_FLAG               /* ����̵������������̵���������־ */

/* Define I/O port data structure for operating the port.  */

typedef struct IO_PORT_STRUCT
{
    UNSIGNED        uPortNumber;                /* the number of the port  */
    UNSIGNED        uPortBits;                  /* the bits number of the port */
    UNSIGNED        uMasksBits;                 /* the masks bits */
    VOID            * pPortAddress1;            /* the first address of the port */
    VOID            * pPortAddress2;            /* the second address of the port */
    VOID            * pPortAddress3;            /* the third address of the port */
    VOID            * pPortAddress4;            /* the fourth address of the port */
    UNSIGNED        uReverseBits;               /* the reverse bits */
}IO_PORT;
#endif
