#ifndef PLAT_NPU808_DEFS_H
#define PLAT_NPU808_DEFS_H

/* ���������ַ�ռ� */
#define PLAT_DRV_TEXT_ADDR              0x00600000      /* ����������ʼ��ַ */
#define PLAT_DRV_TEXT_SIZE              0x00100000      /* ������������ַ�ռ� */

/* ���������ַ�ռ� */
#define PLAT_PROTECT_TEXT_ADDR          0x00800000      /* ����������ʼ��ַ */
#define PLAT_PROTECT_TEXT_SIZE          0x00200000      /* ������������ַ�ռ� */

/* HMI�����ַ�ռ� */
#define PLAT_HMI_TEXT_ADDR              0x00B00000      /* HMI������ʼ��ַ */
#define PLAT_HMI_TEXT_SIZE              0x00300000      /* HMI��������ַ�ռ� */

/* Gws61850�����ַ�ռ� */
#define PLAT_GWS_TEXT_ADDR              0x01100000      /* GWS������ʼ��ַ */
#define PLAT_GWS_TEXT_SIZE              0x00600000      /* GWS��������ַ�ռ� */

/* NPI�����ַ�ռ� */
#define PLAT_NPI_TEXT_ADDR              0x01800000      /* NPI������ʼ��ַ */
#define PLAT_NPI_TEXT_SIZE              0x00500000      /* NPI��������ַ�ռ� */

/* װ�÷�ģ����� */
#define PLAT_MAX_MODULE_NUMBER          2               /* װ������ģ����� */

/* ��NPIͨѶ������Ϣ */
#define PLAT_MAX_LENGTH_FRAME           500             /* ���֡�� */
#define PLAT_MAX_USER_FRAME_NUMBER      50              /* ���������û�����֡���� */

/* Ӳ����Դ���� */
#define PLAT_MAX_EVENT_RECORD           7               /* ����¼����� */
#define PLAT_MAX_AD_NUMBER              3               /* ���A/DƬ�� */
#define PLAT_MAX_PHYSICAL_CHL_NUMBER    96              /* ��������ͨ������ */
#define PLAT_MAX_VIRTUAL_CHL_NUMBER     48              /* ������ͨ������ */
#define PLAT_MAX_CHL_NUMBER \
(PLAT_MAX_PHYSICAL_CHL_NUMBER+PLAT_MAX_VIRTUAL_CHL_NUMBER) /* ���ģ����ͨ������������+��ͨ���� */
#define PLAT_MAX_SAMPLE_POINT           100             /* ���������� */
#define PLAT_MAX_CHANNEL_POINT          0x200           /* ÿͨ������������ */
#define PLAT_MAX_SAMPLE_COUNTER         0x10000         /* ������ŵ����ֵ */
#define PLAT_MAX_SAMPLE_COUNTER_OLT     1200            /* ���˲�����ŵ����ֵ */
#define PLAT_MAX_SWITCH_STATUS_POINT    200             /* ÿ�鿪������λ������¼���� */
#define PLAT_MAX_WAVE_DATA_SIZE         1200            /* ���¼�����ݵ��� */
#define PLAT_MAX_RAM_WAVE_DATA_NUMBER   5               /* ����ڴ滺��¼�����ݸ��� */
#define PLAT_MAX_PROTECT_GROUPS         10              /* ��󱣻�״̬������ */
#define PLAT_MAX_INPUT_GROUPS           40              /* ����������� */
#define PLAT_MAX_WAVE_INPUT_GROUPS      20              /* ���¼������������ */
#define PLAT_MAX_OUTPUT_GROUPS          10              /* ��󿪳������� */
#define PLAT_MAX_CHECK_RELAY            12              /* �����ڼ���� */
#define PLAT_MAX_PROTECT_LOG_GROUPS     20              /* ����߼�״̬������ */
#define PLAT_MAX_STATE_GROUPS \
(PLAT_MAX_PROTECT_GROUPS+PLAT_MAX_WAVE_INPUT_GROUPS+PLAT_MAX_OUTPUT_GROUPS+PLAT_MAX_PROTECT_LOG_GROUPS) /* ���¼��״̬���������� */
#define PLAT_MAX_DEVICE_SYSTEM_NUM      2               /* ���װ��Ӧ�ó�����Ŀ */
#define PLAT_MAX_FAULT_REPORT_NUMBER    100             /* �����ʹ��ϱ������ */
#define PLAT_MAX_NVRAM_REPORT_NUMBER    PLAT_MAX_FAULT_REPORT_NUMBER /* ���洢������Ŀ */
#define PLAT_MAX_NVRAM_WAVE_NUMBER      100             /* ���洢¼����Ŀ */
#define PLAT_MAX_NVRAM_EVENT_NUMBER     80              /* ���洢�¼���Ŀ */
#define PLAT_MAX_RAM_REPORT_NUMBER      70              /* ����ڴ滺�汨����Ŀ */
#define PLAT_MAX_RAM_EVENT_NUMBER       60              /* ����ڴ滺���¼���Ŀ */
#define PLAT_MAX_ONE_FAULT_REPORT_NUMBER 70             /* һ�ι�����󱨸���� */
#define PLAT_FCK_SOE_MAX_COUNT           500            /* ֧�ֵ������SOE�ĸ��� */
#define PLAT_READ_COMMAND_MAX_COUNT      200            /* ֧�ֵ�����ض�����ĸ��� */

/* ��HMIͨ�Ŷ˿����� */
#define PLAT_LINK_HMI_PHY_COM           NU_FALSE        /* ��HMI֮���ͨ�Ŷ˿�����(NU_TRUE������ͨ�ſ����ӣ�NU_FALSE����������) */

/* ���Ź�ʹ��ѡ�� */
#define PLAT_WATCHDOG_ENABLE            NU_TRUE         /* ���Ź�ʹ�� */

/* ���԰汾ѡ�� */
#define PLAT_DEBUG_ENABLE               NU_FALSE          /* ���԰汾ѡ�� */

/* �����¼�λ��� */
extern NU_EVENT_GROUP Event_1_CB;
#define PLAT_ERROR_EVENT_CB             (& Event_1_CB)  /* �����¼�λ��� */

/* ����̵���������������־ */
#define PLAT_RESET_START_FLAG           0               /* ����̵������������̵���������־ */

/* Define task stacks size */
#define TASK_STACK_SIZE         (5000*sizeof(unsigned long))
#define TASK_H_STACK_SIZE       (10000*sizeof(unsigned long))

/* Define each task's priority */
#define TASK0_PRIORITY          16
#define TASK1_PRIORITY          8
#define TASK2_PRIORITY          16
#define TASK3_PRIORITY          16

/* Define each task's time slice */
#define TASK0_TIMESLICE         2
#define TASK1_TIMESLICE         5
#define TASK2_TIMESLICE         5
#define TASK3_TIMESLICE         2

/* Define each timer's initial time slice */
#define TIMER_RUN_INI_TIMESLICE 2

/* Define each timer's time slice */
#ifdef WIN32
#define TIMER_RUN_TIMESLICE     1 //10
#else
#define TIMER_RUN_TIMESLICE     1 //1
#endif

#endif  /* PLAT_NPU810_DEFS_H */
