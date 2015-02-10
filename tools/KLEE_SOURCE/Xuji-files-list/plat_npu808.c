/***********************************
* Include Files
***********************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "plat/inc/xj_cosc.h"
#include "plat/es_plat.h"
#include "plat/inc/xj_pafc.h"
#include "plat/inc/xj_pmfc.h"
#include "plat/inc/xj_pgfc.h"
#include "plat/inc/xj_pdfc.h"
#include "plat/inc/cin_extr.h"
#include "plat/inc/cout_extr.h"
#include "plat_specific/plat_specific.h"
#include "plat/inc/paf_defs.h"
#include "plat/inc/xj_cmemc.h"
#include "plat/inc/xj_ctb_mg.h"
#include "plat/inc/csmv_extr.h"
#include "plat/inc/cer_extr.h"
#include "plat/inc/cin_extr.h"
#include "plat/inc/pfile_extr.h"
#include "plat/inc/cud_extr.h"
#include "plat/net_inc/lc_extr.h"
#include "plat/net_inc/lc_bcall.h"
#include "plat/net_inc/ls_defs.h"
#include "plat/net_inc/ls_extr.h"
#include "plat/inc/xj_out_check.h"
#include "plat/inc/mpf_extr.h"
#include "plat/inc/sect_crc.h"
#include "plat/inc/gpf_extr.h"
#include "plat/inc/cex_extr.h"
#include "plat/inc/ctd_extr.h"
#include "plat/inc/xj_clog.h"
#include "plat/inc/xj_ccms.h"
#include "plat/inc/xj_ccms.h"
#include "plat/inc/xj_cfdc.h"
#include "plat/inc/xj_file.h"
#include "plat/inc/ccm_extr.h"
#include "yaffs_ramdisk.h"
#include "yaffsfs.h"
#include "plat/inc/devi_extr.h"
#include "plat/inc/rtc_extr.h"
#include "cfg_8313xj.h"
#include "immap_83xx.h"
#include "eth_npi_if.h"
#include "xml/inc/pxml_cfg.h"

/* �궨�� */
#define WATCHDOG_TIME10                 10000           /* ���Ź���λ��ʱʱ��(10��) */
#define WATCHDOG_TIME15                 15000           /* ���Ź���λ��ʱʱ��(15��) */
#define WATCHDOG_TIME60                 60000           /* ���Ź���λ��ʱʱ��(60��) */
#define WATCHDOG_TIME180                180000          /* ���Ź���λ��ʱʱ��(180��) */
#define WATCHDOG_MAX_FUNTION            12              /* ���Ź�������󷵻غ������� */
#define READ_TEMPERATURE_TIME_INTERVAL  1000            /* ��ȡ�¶�ʱ���� */

#define COMM_HARD_CHANNEL_ENABLE        CPC_LINK_HMI_PHY_COM        /* Ӳ��ͨ��ͨ��ʹ�� */
#define WATCHDOG_RESET_FLAG             "npu808WATCHDOGFLAG-"       /* ���Ź���λ��¼��ʶ */
#define SYSTEM_ERR_RESET_FLAG           "npu808SystemErrFlag-"      /* ϵͳ�����ʶ */
#define SYSTEM_ERR_START_ADDR           256                         /* ��¼ϵͳ����ʱ��ʼ��ַ */

/* ����ΪOS������� */
#ifdef WIN32
#include "yaffs_fileem.h"
VOID TASK_Test( INT  iTaskID );
#endif

#ifndef WIN32 /* ��windowsȱʡ����һЩ���Ա�ͨ������ */
#include "yaffs_flashif.h"
void PIT_SystemClockInitialize( void );
/* ע��ϵͳʱ�� */
VOID TCCT_RegisterSystemClock( long long (* get_system_clock )( VOID ) );
#endif

/***********************************
* Global Variables
***********************************/
/* Application Structures */
#if (COMM_HARD_CHANNEL_ENABLE == NU_TRUE)
NU_TASK             Task_0;
#endif
/*========================================================*/
/* ��������������߼��ж���Ϣ */
NU_HISR MainTaskActiveHISR;
#define MAIN_TASTK_HISR_STACK_SIZE 0x800   /* ��ջsize */
#define MAIN_TASTK_HISR_PRIORITY   2       /* �ж����ȼ� */
unsigned char MainTastkHisrStack[MAIN_TASTK_HISR_STACK_SIZE];
/* �߼��ж�ִ�к��� */
void MainTaskActivesHISR_Routine( void );
/* Ӧ��������������������ִ�к��� */
INT MAIN_UserActiveMainTask( VOID );
/* ��ȡϵͳ��ʼ���Ƿ���ɺ���:NU_FALSE-δ���,NU_TRUE-��� */
INT Main_GetSystemInitFlag( VOID );
/* ��ȡ�¶ȴ��������� */
INT Main_GetTemperature ( INT * temperature_ptr );
/* ע��UART�����жϺ��� */
INT MainRegUartIntFun( INT channel, VOID (* UartComIntFun )( int para ));
/* ��ȡUART�����ж�����ID */
UNSIGNED MainGetUartVectorID( INT channel );
/* ����װ�÷��Ͷ�ʱ������� */
VOID ( * SendSynTimeCmd )( VOID );
/*========================================================*/
NU_TASK             Task_1;
NU_TASK             Task_2;
NU_TASK             Task_3;
NU_TIMER            Timer_Run_CB;
NU_TIMER            Timer_Trip_CB;
NU_TIMER            Timer_ResetRelay_CB;
NU_MEMORY_POOL      * PLAT_mem_pool;
NU_MEMORY_POOL      * PLAT_uncached_mem_pool;

NU_EVENT_GROUP      Event_1_CB;
NU_EVENT_GROUP      Event_2_CB;

NU_PROTECT          * pReport_Protect = NU_NULL;
NU_PROTECT          * pWave_Protect = NU_NULL;
NU_PROTECT          * pFck_Protect = NU_NULL;
GW_CHANNEL          * pCommChl = NU_NULL;
UNSIGNED            ad_chl_number = 0;
float               * ad_sample_data;
UNSIGNED            AD_FloatStkBuf[100];   /* ��������ջָ�� */
UNSIGNED            FIBER_FloatStkBuf[100];/* ��������ջָ�� */
UNSIGNED            GTIMER_FloatStkBuf[100];/* ��������ջָ�� */
UNSIGNED            PPS_FloatStkBuf[100];/* ��������ջָ�� */
NU_PROTECT          OverCheck_Protect;
NU_PROTECT          DeleteFile_Protect;
unsigned long       PIT_Cpu_NS_Num;
unsigned long long  PIT_Cpu_NS_Interrupt;

/* External variable declarations */
extern  volatile UNSIGNED    TMD_System_Clock;
extern INT (* PMFC_Save_Time )( TIME * pTime );
extern VOID (* Fiber_Receive_Interrupt )( VOID );   /* NPU816�й����жϴ����� */
extern VOID (* Fiber_Transmit_Interrupt )( VOID );  /* NPU816�й��˷������ݴ����� */
extern INT OverTimeTabNumber;
/* ��ʼ������ͬ���������������Ϣ;iSamplePoint:ÿ�ܲ��������� */
extern INT FIBER_GPS_Initiliaze( INT iSamplePoint );
/* GTM��ʱ���жϹ����� */
extern VOID FIBER_Timer_IntManage( VOID );
/* ͬ�������жϹ����� */
VOID FIBER_GPS_IntManage( INT iFlag );

/* External function declarations */
extern  VOID        ERC_System_Error ( INT );
extern  STATUS      ERC_Register_Error_Handler( VOID (* error_handler )( VOID * task_ptr ));
extern  VOID        FSTK_Save_Float_Register( register VOID * stack_ptr );
extern  VOID        FSTK_Restore_Float_Register( register VOID * stack_ptr );
#ifndef WIN32
extern  void cpu_init_f ( void );
#endif

/* ƽ̨װ�����еƹ����� */
extern  HLEM PlatFormRunLedHandle;
extern void DMScan_Init();
extern void DMScan_Exec();
extern void __init_critical_regions( void );
extern void stdio_open_init();
extern INT DEVI_initialize( void );
extern INT TMCT_OS_ClockDisable( VOID );

/* Function Prototypes */
#if (COMM_HARD_CHANNEL_ENABLE == NU_TRUE)
VOID    Task_0_Entry( UNSIGNED argc, VOID * argv );
#endif
VOID    Task_1_Entry( UNSIGNED argc, VOID * argv );
VOID    Task_2_Entry( UNSIGNED argc, VOID * argv );
VOID    Task_3_Entry( UNSIGNED argc, VOID * argv );
VOID    Timer_Run_Entry( UNSIGNED argc );

VOID    LoadApplicationData( VOID ); /* ����Ӧ������ */
int     AD_InterruptHandler( int params ); /* A/D�жϴ����� */
void    FIBER_InterruptHandler( int params ); /* �����жϹ����� */
void    GTM1_InterruptHandler( int params );  /* GTM1�жϹ����� */
/* ����PPS������ */
void    FIBER_PPSInterruptHandler( int params );
VOID    DeviceEnvironmentInitialize( VOID ); /* ��ʼ��װ�����л��� */
VOID    MainTaskCommRoutine( UNSIGNED ProtectId, UNSIGNED SectorId, INT iResetFlag ); /* ���������� */
#ifndef WIN32
void    PIT_InterruptHandler( int params ); /* ��������λ���ר�ö�ʱ���жϺ��� */
#endif
INT GetDeviceSystemClock_Precision( UNSIGNED * millisecond_ptr, UNSIGNED * Nanosecond_ptr );
/* ��ȡϵͳʱ��(����1�֡�����1��) */
INT GetDeviceSystemClock_64Precision( UNSIGNED * millisecond_ptr, UNSIGNED * Nanosecond_ptr );
VOID OverCheck_Lock( VOID );
VOID OverCheck_Unlock( VOID );
VOID DeleteFile_Lock( VOID );
VOID DeleteFile_Unlock( VOID );
VOID OvertimeCheckRoutine( UNSIGNED ProtectId, UNSIGNED SectorId, INT iResetFlag );
VOID Error_Handler_Test( VOID * task_ptr );

/* ��������/��ʱ��/�ж��Ƿ����еļ����� */
UNSIGNED AD_InterruptHandlerCount = 0;
UNSIGNED PIT_InterruptHandlerCount = 0;
UNSIGNED FIBER_InterruptHandlerCount = 0;
UNSIGNED GTM1_InterruptHandlerCount = 0;
UNSIGNED FIBER_PPSInterruptHandlerCount = 0;
UNSIGNED Task_0_EntryCount = 0;
UNSIGNED Task_1_EntryCount = 0;
UNSIGNED Task_2_EntryCount = 0;
UNSIGNED Task_3_EntryCount = 0;
UNSIGNED Timer_Run_EntryCount = 0;
//
UNSIGNED Task_0_MemHandle = 0;
UNSIGNED Task_1_MemHandle = 0;
UNSIGNED Task_2_MemHandle = 0;
UNSIGNED Task_3_MemHandle = 0;
INT TAST_SaveTime_Flag = 0;
INT MainTemperatureData[2] = {NU_UNAVAILABLE, 0};  /* ��¼����ִ�н��(0)����ȡ����������Ϣ(1) */

INT RunTime_Enable = XJ_FALSE;

#ifndef WIN32
long long get_cpu_tick( void );
long long Get_system_clock_nanosecond( VOID );

asm long long get_cpu_tick( void )
{
    1:  mftbu   r3
        mftb    r4
        mftbu   r5
        cmp 0, r3, r5
        bne 1b
        blr
}

#define GET_HW_TIME_PER_TICKS   ((long long)ESAL_GE_TMR_OS_CLOCK_PRESCALE * 1000000000 * 1000 /   \
                                 ESAL_GE_TMR_OS_CLOCK_RATE)
#define GET_CPU_NANOSECOND()  (get_cpu_tick() * GET_HW_TIME_PER_TICKS / 1000 )
long long Get_system_clock_nanosecond( VOID )
{
    long long current_time;

    current_time = GET_CPU_NANOSECOND();

    return( current_time );
}
#endif

/* Define the Application_Initialize routine that determines the initial
   Nucleus PLUS application environment.  */
VOID    Application_Initialize( NU_MEMORY_POOL * mem_pool, NU_MEMORY_POOL * uncached_mem_pool )
{
    VOID * mem_ptr;
    VOID * pointer;
    STATUS status;
    UNSIGNED TaskEigenvalue;

    PLAT_mem_pool = mem_pool;
    PLAT_uncached_mem_pool = uncached_mem_pool;
    TaskEigenvalue = 0x55aa5000; /* 0x55aa+'P'+0 */

    XJP_OS_Get_FuncList();

    /* memory initialize */
    MemoryInit( mem_pool );

#if (COMM_HARD_CHANNEL_ENABLE == NU_TRUE)
    /* Allocate memory for task 0 */
    status = NU_Allocate_Memory( mem_pool, & pointer,
        TASK_STACK_SIZE, NU_NO_SUSPEND );

    /* Check to see if previous operation successful */
    if ( status == NU_SUCCESS )
    {
        /* Create task 0.  */
        status = NU_Create_Task(& Task_0, "PTASK 0", Task_0_Entry, 0, NU_NULL, pointer,
            TASK_STACK_SIZE, TASK0_PRIORITY, TASK0_TIMESLICE,
            NU_PREEMPT, NU_START );
    }
#else
    status = NU_SUCCESS;
#endif

    /* Check to see if previous operation successful */
    if ( status == NU_SUCCESS )
    {
        /* Allocate memory for task 1 */
        status = NU_Allocate_Memory( mem_pool, & mem_ptr,
            ( TASK_STACK_SIZE +( sizeof( UNSIGNED )* 2 )), NU_NO_SUSPEND );
    }

    /* Check to see if previous operation successful */
    if ( status == NU_SUCCESS )
    {
#if (PLAT_DEBUG_ENABLE == NU_TRUE)
        Task_1_MemHandle = XJP_Dbg_Memory_Create( mem_ptr, ( TASK_STACK_SIZE +( sizeof( UNSIGNED )* 2 )),
            "PTask1", ( TaskEigenvalue + 1 ));
#endif
        pointer = ( VOID *)((( UNSIGNED )mem_ptr ) + sizeof( UNSIGNED ));
        /* Create task 1.  */
        status = NU_Create_Task(& Task_1, "PTASK 1", Task_1_Entry, 0, NU_NULL, pointer,
            TASK_STACK_SIZE, TASK1_PRIORITY, TASK1_TIMESLICE,
            NU_PREEMPT, NU_START );
    }

    /* Check to see if previous operation successful */
    if ( status == NU_SUCCESS )
    {
        /* Allocate memory for task 2 */
        status = NU_Allocate_Memory( mem_pool, & mem_ptr,
            ( TASK_STACK_SIZE +( sizeof( UNSIGNED )* 2 )), NU_NO_SUSPEND );
    }

    /* Check to see if previous operation successful */
    if ( status == NU_SUCCESS )
    {
#if (PLAT_DEBUG_ENABLE == NU_TRUE)
        Task_2_MemHandle = XJP_Dbg_Memory_Create( mem_ptr, ( TASK_STACK_SIZE +( sizeof( UNSIGNED )* 2 )),
            "PTask2", ( TaskEigenvalue + 2 ));
#endif
        pointer = ( VOID *)((( UNSIGNED )mem_ptr ) + sizeof( UNSIGNED ));
        /* Create task 2 */
        status = NU_Create_Task(& Task_2, "PTASK 2", Task_2_Entry, 0, NU_NULL, pointer,
            TASK_STACK_SIZE, TASK2_PRIORITY, TASK2_TIMESLICE,
            NU_PREEMPT, NU_START );
    }

    /* Check to see if previous operation successful */
    if ( status == NU_SUCCESS )
    {
        /* Allocate memory for task 3 */
        status = NU_Allocate_Memory( mem_pool, & mem_ptr,
            ( TASK_H_STACK_SIZE +( sizeof( UNSIGNED )* 2 )), NU_NO_SUSPEND );
    }

    /* Check to see if previous operation successful */
    if ( status == NU_SUCCESS )
    {
#if (PLAT_DEBUG_ENABLE == NU_TRUE)
        Task_3_MemHandle = XJP_Dbg_Memory_Create( mem_ptr, ( TASK_H_STACK_SIZE +( sizeof( UNSIGNED )* 2 )),
            "PTask3", ( TaskEigenvalue + 3 ));
#endif
        pointer = ( VOID *)((( UNSIGNED )mem_ptr ) + sizeof( UNSIGNED ));
        /* Create task 3. */
        status = NU_Create_Task(& Task_3, "PTASK 3", Task_3_Entry, 0, NU_NULL, pointer,
            TASK_H_STACK_SIZE, TASK3_PRIORITY, TASK3_TIMESLICE,
            NU_PREEMPT, NU_START );
    }

    /* Check to see if previous operation successful */
    if ( status == NU_SUCCESS )
    {
        /* Create run timer. */
        status = NU_Create_Timer(& Timer_Run_CB, "PT0RUN", Timer_Run_Entry, 0,
            TIMER_RUN_INI_TIMESLICE, TIMER_RUN_TIMESLICE, NU_DISABLE_TIMER );
    }

    if ( status == NU_SUCCESS )
    {
        /* initial time:500. reschedule time:500.*/
        status = NU_Create_Timer(& Timer_Trip_CB, "PT1TIP", LSC_TripTimerRoutine, 0, 2000, 2000,
            NU_DISABLE_TIMER );
    }

    /* ����̵�����ʱ���ض�ʱ�� */
    if ( status == NU_SUCCESS )
    {
        /* initial time:500. reschedule time:500.*/
        status = NU_Create_Timer(& Timer_ResetRelay_CB, "PT3RRL", LSC_RRelayTimerRoutine, 0, 500, 500,
            NU_DISABLE_TIMER );
    }

    /* create event */
    NU_Create_Event_Group(& Event_1_CB, "EVENT 1" );
    NU_Create_Event_Group(& Event_2_CB, "EVENT 2" );

    /* �����ָ�������ĸ߼��ж� */
    NU_Create_HISR(& MainTaskActiveHISR, "", MainTaskActivesHISR_Routine, MAIN_TASTK_HISR_PRIORITY,
        MainTastkHisrStack, MAIN_TASTK_HISR_STACK_SIZE );

    /* Check to see if previous operation successful */
    if ( status != NU_SUCCESS )
    {
        /* Call error handling routine */
        ERC_System_Error( status );
    }
}

#define FS_FORMATED_FILENAME      "fsinited"

static void check_fs_formated( struct yaffscfg * cfgs, int n )
{
    int i, j, info[2], eraseflag;
    char path[YAFFS_MAX_NAME_LENGTH + 1];
    yaffs_Device * dev;
    FILE * fp;
    char * dummy;
    for( i = 0; i < n; i ++, cfgs ++)
    {
        dev = yaffsfs_FindDevice( cfgs->devName, & dummy );
        if(! dev )
            continue;

        strcpy( path, cfgs->devName );
        strcat( path, "/" );
        strcat( path, FS_FORMATED_FILENAME );

        eraseflag = 1;
        fp = fopen( path, "rb" );
        if( fp )
        {
            if( fread( info, 1, sizeof( info ), fp ) == sizeof( info ))
            {
                if( ( info[0] == cfgs->startBlock ) &&  ( info[1] == cfgs->endBlock ) )
                    eraseflag = 0;
            }
            fclose( fp );
        }

        if( eraseflag )
        {
            yaffs_unmount( cfgs->devName );
            for( j = dev->startBlock; j <= dev->endBlock; j ++)
                dev->eraseBlockInNAND( dev, j );
            yaffs_mount( cfgs->devName );
            fp = fopen( path, "wb" );
            if( fp )
            {
                info[0] = cfgs->startBlock;
                info[1] = cfgs->endBlock;
                fwrite( info, sizeof( info ), 1, fp );
                fclose( fp );
            }
        }
    }
}

/* ��ȡϵͳʱ��(����1�֡�΢��1��) */
INT GetDeviceSystemClock_Precision( UNSIGNED * millisecond_ptr, UNSIGNED * Nanosecond_ptr )
{
#ifndef WIN32
    UNSIGNED Millisecond;
    UNSIGNED Nanosecond;
    UNSIGNED Variable;
    unsigned long long cpuNS;

    cpuNS = GET_CPU_NANOSECOND();
    Variable = ( UNSIGNED )( cpuNS - PIT_Cpu_NS_Interrupt );
    Variable += PIT_Cpu_NS_Num;
    Millisecond = ( UNSIGNED )( Variable / 1000000 );
    Millisecond += TMD_System_Clock;
    Nanosecond = ( UNSIGNED )( Variable % 1000000 );
    * millisecond_ptr = Millisecond;
    * Nanosecond_ptr = Nanosecond;
#else
    * millisecond_ptr = TMD_System_Clock;
    * Nanosecond_ptr = 0;
#endif

    return( NU_SUCCESS );
}

/* ��ȡϵͳʱ��(����1�֡�����1��) */
INT GetDeviceSystemClock_64Precision( UNSIGNED * millisecond_ptr, UNSIGNED * Nanosecond_ptr )
{
#ifndef WIN32
    UNSIGNED Millisecond;
    UNSIGNED Nanosecond;
    unsigned long long cpuNS;

    cpuNS = GET_CPU_NANOSECOND();
    Millisecond = ( UNSIGNED )( cpuNS / 1000000 );
    Nanosecond = ( UNSIGNED )( cpuNS % 1000000 );
    * millisecond_ptr = Millisecond;
    * Nanosecond_ptr = Nanosecond;
#else
    * millisecond_ptr = TMD_System_Clock;
    * Nanosecond_ptr = 0;
#endif

    return( NU_SUCCESS );
}

VOID OverCheck_Lock( VOID )
{
    NU_Protect(& OverCheck_Protect );
}

VOID OverCheck_Unlock( VOID )
{
    NU_Unprotect();
}

VOID DeleteFile_Lock( VOID )
{
    NU_Protect(& DeleteFile_Protect );
}

VOID DeleteFile_Unlock( VOID )
{
    NU_Unprotect();
}

/* ��ʼ��װ�����л��� */
VOID DeviceEnvironmentInitialize( VOID )
{
    INT status;
    unsigned chl_number;
    HPRM ReportProtectHandle;
    HPRM WaveProtectHandle;
    HPRM FckProtectHandle;
#ifndef WIN32
    VOID (* oldlisr )( int );
    INT (* getGPSStatus )( VOID );
    INT (* getIRIGBStatus )( UNSIGNED *);
#endif
    struct yaffscfg ydiskcfgs[3]; /* �ļ�������ṹ */

    /* ƽ̨���ò����ĳ�ʼ�� */
    PMI_InitializePlatCfg();
    /* ��ʼ��������ʹ�õ�ƽ̨�������� */
    PAFC_PlatProFunList_Initialize();
    /* ��ʼ����HMIʹ�õ�ƽ̨�������� */
    PMFC_PlatHMIFunList_Initialize();
    /* ��ʼ��ƽ̨�ṩ��GWSʹ�õķ������� */
    PGFC_PlatGwsFunList_Initialize();
    /* ��ʼ��ƽ̨�ṩ������ʹ�õķ������� */
    PDFC_PlatDriverFunList_Initialize();
    /*=======================================================================*/
    /* ����������������                                                      */
    /*=======================================================================*/
#if 1
    status = PDFC_LinkDriverApplication( CPC_DRV_TEXT_ADDR, CPC_DRV_TEXT_SIZE, NU_NULL );
    switch( status )
    {
    case XJP_VERSION_ERROR:
        /* ���Ӷ���汾���� */
        NU_Set_Events(& Event_2_CB, CPC_EVENT2_VERSION_ERROR, NU_OR );
        break;
    case XJP_LOAD_SUCCESS:
        /* Ӧ�ö������ӳɹ� */
        NU_Set_Events(& Event_2_CB, CPC_EVENT2_DRV_READY, NU_OR );
        break;
    default:
        break;
    }
#endif
    /*=======================================================================*/
    /* ��ʼ����������                                                        */
    /*=======================================================================*/
    /* ��ʼ���������� */
    PDFC_InitializeDriverApplication( PLAT_mem_pool, PLAT_uncached_mem_pool );
    /* ���豸�������豸ͨ�� */
    DEVI_initialize();
    /* CPU������ó�ʼ�� */
    DEVI_InitializeCPU();
    /* ��ʼ�����е� */
    DEVI_InitializeRunLed();
    /* ��ʼ���澯�� */
    DEVI_InitializeAlarmLed();
    /* ��ʼ���û��� */
    DEVI_InitializeUserLed();
    /* ��ʼ�������̵��� */
    DEVI_InitializeStartRelay();
    /* ��ʼ���¶ȴ����� */
    DEVI_InitializeTemperature();
    /* ��ʼ��ʱ��оƬ */
    DEVI_InitializeTimeChip();
    /*=======================================================================*/
    /* ��ʼ�������� */
    XJP_Mutex_Initialize();
    /* ��ʼ������״̬������ */
    CINC_CreateInputDriver();
    /* ע����Ժ����� */
    XJP_Dbg_Register_FuncList();
    /* ע���ļ�ϵͳ������ */
    PFILE_Register_FileFunctionList();
    /* �ٽ�����ʼ�� */
    __init_critical_regions();
    /* �ļ�open��ʼ�� */
    stdio_open_init();
    /* �ļ�ϵͳ��ʼ�� */
    ydiskcfgs[0].devName = "/user";
    ydiskcfgs[0].nChunksPerBlock = 32;
    ydiskcfgs[0].startBlock = 1152;
    ydiskcfgs[0].endBlock = 4095;
    ydiskcfgs[0].nReservedBlocks = 5;
    ydiskcfgs[0].nShortOpCaches = 32;
#ifndef WIN32
    ydiskcfgs[0].writeChunkToNAND = yflash_WriteChunkToNAND;
    ydiskcfgs[0].readChunkFromNAND = yflash_ReadChunkFromNAND;
    ydiskcfgs[0].eraseBlockInNAND = yflash_EraseBlockInNAND;
    ydiskcfgs[0].initialiseNAND = yflash_InitialiseNAND;
#else
    ydiskcfgs[0].writeChunkToNAND = yfileem_WriteChunkToNAND;
    ydiskcfgs[0].readChunkFromNAND = yfileem_ReadChunkFromNAND;
    ydiskcfgs[0].eraseBlockInNAND = yfileem_EraseBlockInNAND;
    ydiskcfgs[0].initialiseNAND = yfileem_InitialiseNAND;
#endif

    ydiskcfgs[1].devName = "/bin";
    ydiskcfgs[1].nChunksPerBlock = 32;
    ydiskcfgs[1].startBlock = 128;
    ydiskcfgs[1].endBlock = 1151;
    ydiskcfgs[1].nReservedBlocks = 5;
    ydiskcfgs[1].nShortOpCaches = 32;
#ifndef WIN32
    ydiskcfgs[1].writeChunkToNAND = yflash_WriteChunkToNAND;
    ydiskcfgs[1].readChunkFromNAND = yflash_ReadChunkFromNAND;
    ydiskcfgs[1].eraseBlockInNAND = yflash_EraseBlockInNAND;
    ydiskcfgs[1].initialiseNAND = yflash_InitialiseNAND;
#else
    ydiskcfgs[1].writeChunkToNAND = yfileem_WriteChunkToNAND;
    ydiskcfgs[1].readChunkFromNAND = yfileem_ReadChunkFromNAND;
    ydiskcfgs[1].eraseBlockInNAND = yfileem_EraseBlockInNAND;
    ydiskcfgs[1].initialiseNAND = yfileem_InitialiseNAND;
#endif

    memcpy(& ydiskcfgs[2],& ramdiskcfg, sizeof( struct yaffscfg ));
    yaffs_load( ydiskcfgs, 3 );
    check_fs_formated( ydiskcfgs, 3 );
    /*=======================================================================*/
    /* ��HMI֮�佨������                                                     */
    /*=======================================================================*/
#if 1
    status = PMFC_LinkHMIApplication( CPC_HMI_TEXT_ADDR, CPC_HMI_TEXT_SIZE, NU_NULL );
    switch( status )
    {
    case XJP_VERSION_ERROR:
        /* ���Ӷ���汾���� */
        NU_Set_Events(& Event_2_CB, CPC_EVENT2_VERSION_ERROR, NU_OR );
        break;
    case XJP_LOAD_SUCCESS:
        /* Ӧ�ö������ӳɹ� */
        NU_Set_Events(& Event_2_CB, CPC_EVENT2_HMI_READY, NU_OR );
        break;
    default:
        break;
    }
#endif
    /*=======================================================================*/
    /* ����Ӧ�ó�������                                                      */
    /*=======================================================================*/
    status = PAFC_LinkApplication( CPC_PROTECT_TEXT_ADDR, CPC_PROTECT_TEXT_SIZE, NU_NULL );
    switch( status )
    {
    case XJP_VERSION_ERROR:
        /* ���Ӷ���汾���� */
        NU_Set_Events(& Event_2_CB, CPC_EVENT2_VERSION_ERROR, NU_OR );
        break;
    case XJP_LOAD_SUCCESS:
        /* Ӧ�ö������ӳɹ� */
        NU_Set_Events(& Event_2_CB, CPC_EVENT2_PROTECT_READY, NU_OR );
        break;
    default:
        break;
    }
    /*=======================================================================*/
    /* ��Gws61850֮�佨������                                                */
    /*=======================================================================*/
#ifndef WIN32
#if 1
    status = PGFC_LinkGwsApplication( CPC_GWS_TEXT_ADDR, CPC_GWS_TEXT_SIZE, NU_NULL );
    switch( status )
    {
    case XJP_VERSION_ERROR:
        /* ���Ӷ���汾���� */
        NU_Set_Events(& Event_2_CB, CPC_EVENT2_VERSION_ERROR, NU_OR );
        break;
    case XJP_LOAD_SUCCESS:
        /* Ӧ�ö������ӳɹ� */
        NU_Set_Events(& Event_2_CB, CPC_EVENT2_GWS_READY, NU_OR );
        break;
    default:
        break;
    }
#endif
#endif
    /*=======================================================================*/
    /* ��ʼ��HMI����                                                         */
    /*=======================================================================*/
#ifndef WIN32
    /* ϵͳʱ�ӳ�ʼ�� */
    PIT_SystemClockInitialize();
    getGPSStatus = NU_NULL;
    getIRIGBStatus = NU_NULL;
    if( DEVI_CheckGPSPulseDevice() == NU_TRUE )
    {
        getGPSStatus = DEVI_GetGPSStatus;
    }
    if( DEVI_CheckIRIGBDevice() == NU_TRUE )
    {
        getIRIGBStatus = DEVI_GetIRIGBStatus;
    }
    /* ��ֹ����ϵͳʱ�����У�ʹ���Զ���ʱ�� */
    TMCT_OS_ClockDisable();
    /* GPS��ʱ����ѡ�� */
    XJP_TM_Set_Time_Para( 0, getGPSStatus, RTC_Set_Rtc_Time, GPS1_RegisterPluseInterrupt,
        GetDeviceSystemClock_Precision, GetDeviceSystemClock_64Precision, getIRIGBStatus  );
    NU_Register_LISR( ESAL_PR_PIT_INT_VECTOR_ID, ( void *)PIT_InterruptHandler, & oldlisr );
    ESAL_GE_INT_Enable( ESAL_PR_PIT_INT_VECTOR_ID, ESAL_TRIG_FALLING_EDGE  , 1 );
    DEVI_InitializePerTimer( CINC_GetFckInputCheckFreq() );
    RTC_Clock_Initial();
    /* ��ʼ��HMI���� */
    PMFC_InitializeHMIApplication( PLAT_mem_pool, PLAT_uncached_mem_pool );
#endif
    /*=======================================================================*/
    /* ��ʼ��Ӧ�ó���                                                        */
    /*=======================================================================*/
    /* �������汣����� */
    ReportProtectHandle = XJP_NU_Protect_Create( PAF_LOCK_REPORT_NAME );
    pReport_Protect = ( NU_PROTECT *)ReportProtectHandle;
    /* ����¼��������� */
    WaveProtectHandle = XJP_NU_Protect_Create( PAF_LOCK_WAVE_NAME );
    pWave_Protect = ( NU_PROTECT *)WaveProtectHandle;
    /* ����������ݱ������ */
    FckProtectHandle = XJP_NU_Protect_Create( PAF_LOCK_FCK_NAME );
    pFck_Protect = ( NU_PROTECT *)FckProtectHandle;
    /* ��ʼ��ƽ̨�����ļ� */
    PXML_CFG_InitConfig( CPC_CFG_FILE_NAME );
    /* ����Ӧ�ó������ò��� */
    PMI_SetApplicationCfg();
    /* ��ʼ������������Ϣ */
    CEXC_Initialize( PRO_EXCHANGE_NUMBER );
    /* ͨѶ�����ڴ�ռ����� */
    LCC_Link_AllocateMemory();
    /* ���ݱ�ĳ�ʼ�� */
    PMI_InitializeTable();
    /* ��ʼ���������� */
    XJP_Background_Initialize( CPC_BACKGROUND_DATA_FILE_NAME );
    /* ע��������� */
    XJP_Register_Backup_Task(& Task_2 );
    /* ��ʼ��Ӧ�ó��� */
    PAFC_InitializeApplication( PLAT_mem_pool, PLAT_uncached_mem_pool );
    /* ע�����ݱ����� */
    CTB_MG_RegisterTreatmentFunc();
    /* ��ȡ���ݱ��������Ϣ */
    PMI_GetTableDataInformation();
    /* ��ʼ���ڴ�ѭ�������� */
    PMI_InitializeCircleMemory();
    /* ��־ϵͳ��ʼ�� */
    CLOG_Initialize( 0x3000 / sizeof( UNSIGNED ) );
    /* ɾ���ļ�����ģ���ʼ�� */
    CFDC_Initialize( 0x3000, DeleteFile_Lock, DeleteFile_Unlock, xj_RemoveFile, xj_FileExists );
    /* ��ʼ��ģ�������� */
    CSMV_InitializeChannelPara();
    /* ��ʼ��װ�����е� */
    PMI_InitializeDeviceLed();
    /* ��ʼ������������ */
    COUTC_OutputInitialize();
    /* ��ʼ�����ڱ���λ���� */
    XJP_Output_Hold_Initialize( CPC_OUTPUTHOLD_DATA_DIR_NAME );
    /* ��ʼ����ʱ������� */
    XJP_Overtime_Initialize( OverTimeTabNumber, OverCheck_Lock, OverCheck_Unlock );
    /* ��ʼ��FALSH�����ڴ����Ϣ */
    CERC_FlashCheckInitialize();
    /* ��ȡFLASH������ʼ��Ϣ */
    CERC_FlashCheckMain();
    /* RAM�������ļ��Լ���Ϣ��ʼ�� */
    CERC_FileInitialize();
    /* �����λ״̬��Ϣ */
    DEVI_RsrInfCPU();
#if 1
    /* ע�����ϵͳ�����¼��Ϣ���� */
    ERC_Register_Error_Handler( Error_Handler_Test );
#endif
    /*=======================================================================*/
    /* ��ʼ��Gws61850����                                                    */
    /*=======================================================================*/
#ifndef WIN32
    /* ��ʼ��Gws61850���� */
    PGFC_InitializeGwsApplication( PLAT_mem_pool, PLAT_uncached_mem_pool );
#endif

    chl_number = CSMV_GET_PHY_ANALOG_NUMBER;
    if( chl_number != 0 )
    {
        SendSynTimeCmd = NU_NULL;
        if( PLAT_cfg.device.FunSelect & FUN_IED_DEVICE )
        {
            /* ע������װ��A/D�жϴ����� */
            eth_npi_if_initialize( CSMV_MessParse );
            /* ע�ᷢ�Ͷ�ʱ����� */
            SendSynTimeCmd = CSMV_Msg_SynTimeCmd;
#ifndef WIN32
            if( PLAT_cfg.device.FunSelect & FUN_OPTICAL )
            {
                /* Bϵ�й���ǿ�Ʒ��������� */
                DEVI_InitializeSamplePulse( CSMV_GET_PERIOD_POINT, 1 ); /* ǿ�Ʋ������� */
            }
            /* ��NPU8100װ�õ�ʱ���б��Ƿ���AD����ͨ�� */
            if(( CSMV_GET_PHY_ANALOG_NUMBER > CSMV_GET_DSAMPLE_SV_CHL_NUM )
                && ( DEVI_GetIdentificationCode() == XJ_IDC_NPU8100 ))
            {
                /* AD����ͨ������ */
                chl_number = CSMV_GET_PHY_ANALOG_NUMBER - CSMV_GET_DSAMPLE_SV_CHL_NUM;
                /* ��ʼ��A/Dת�� */
                ad_sample_data = DEVI_InitializeAD( CSMV_GET_PERIOD_POINT, & ad_chl_number );
                if( chl_number < ad_chl_number )
                {
                    ad_chl_number = ( UNSIGNED )chl_number;
                }
                if( ad_chl_number > 0 )
                {
                    /* ��ʼ���������� */
                    DEVI_InitializeSamplePulse( CSMV_GET_PERIOD_POINT, 0 );
#ifndef WIN32
                    /* ע��Ӳ��A/D�жϴ����� */
                    NU_Register_LISR( ESAL_PR_IRQ2_INT_VECTOR_ID, ( void *)AD_InterruptHandler, & oldlisr );
                    /* ʹ��A/D�ж� */
                    ESAL_GE_INT_Enable( ESAL_PR_IRQ2_INT_VECTOR_ID, ESAL_TRIG_FALLING_EDGE  , 0 );
#endif
                    /* ���ò���׼����ɱ�ʶ */
                    CSMV_SetSampleInitFlag( NU_TRUE );
                    PLAT_cfg.smvc.DSampleFlag = NU_TRUE;
                }
            }
#endif
        }
        else
        {
            /* ��ʼ��A/Dת�� */
            ad_sample_data = DEVI_InitializeAD( CSMV_GET_PERIOD_POINT, & ad_chl_number );
            if( chl_number < ad_chl_number )
            {
                ad_chl_number = ( UNSIGNED )chl_number;
            }
            if( ad_chl_number > 0 )
            {
                /* ��ʼ���������� */
                DEVI_InitializeSamplePulse( CSMV_GET_PERIOD_POINT, 0 );
#ifndef WIN32
                /* ע��Ӳ��A/D�жϴ����� */
                NU_Register_LISR( ESAL_PR_IRQ2_INT_VECTOR_ID, ( void *)AD_InterruptHandler, & oldlisr );
                /* ʹ��A/D�ж� */
                ESAL_GE_INT_Enable( ESAL_PR_IRQ2_INT_VECTOR_ID, ESAL_TRIG_FALLING_EDGE  , 0 );
#endif
                /* ���ò���׼����ɱ�ʶ */
                CSMV_SetSampleInitFlag( NU_TRUE );
            }
        }
#ifndef WIN32
        /* ѡ���й���ͨѶ����,ͬʱΪNPU816���ʱ��ע����˺�GTM�жϺ��� */
        if(( PLAT_cfg.device.FunSelect & FUN_OPTICAL )
            && ( DEVI_GetIdentificationCode() == XJ_IDC_NPU823 ))
        {
            /* ��ʼ������ͬ���������������Ϣ;iSamplePoint:ÿ�ܲ��������� */
            FIBER_GPS_Initiliaze( CSMV_GET_PERIOD_POINT );

            /* ע������жϴ����� */
            NU_Register_LISR( ESAL_PR_IRQ4_INT_VECTOR_ID, ( void *)FIBER_InterruptHandler, & oldlisr );
            ESAL_GE_INT_Enable( ESAL_PR_IRQ4_INT_VECTOR_ID, ESAL_TRIG_FALLING_EDGE  , 0 );

            /* ����װ�õ�ʱ����Ҫͬ�������GTM��ʱ���ж� */
            if( PLAT_cfg.device.FunSelect & FUN_IED_DEVICE )
            {
                /* ע��GTM�жϴ����� */
                NU_Register_LISR( ESAL_PR_GTM1_INT_VECTOR_ID, ( void *)GTM1_InterruptHandler, & oldlisr );
                ESAL_GE_INT_Enable( ESAL_PR_GTM1_INT_VECTOR_ID, ESAL_TRIG_FALLING_EDGE  , 0 );

                /* ����PPS�жϴ����� */
                NU_Register_LISR( ESAL_PR_IRQ2_INT_VECTOR_ID, ( void *)FIBER_PPSInterruptHandler, & oldlisr );
                /* ʹ�ܹ���PPS�ж� */
                ESAL_GE_INT_Enable( ESAL_PR_IRQ2_INT_VECTOR_ID, ESAL_TRIG_FALLING_EDGE  , 0 );
            }
        }
#endif
    }

#ifndef WIN32
    /* �����û�ѡ�����³�ʼ�����ڶ�ʱ�� */
    DEVI_InitializePerTimer( CINC_GetFckInputCheckFreq() );
#endif
    /* ���ÿ���������־ */
    CINC_SetInputCheckFlag( 1 );
    /* ����װ�÷�ģ���ʼ���ɹ� */
    CCMC_SetModInitSuccess();
}

#ifndef WIN32
/* A/D�жϴ����� */
int  AD_InterruptHandler( int params )
{
    INT status;
    VOID * pStack;
    UNSIGNED pulse_counter;

    AD_InterruptHandlerCount ++;
    pStack = ( VOID *)AD_FloatStkBuf;

    FSTK_Save_Float_Register( pStack );

    status = DEVI_GetSampleData( ad_chl_number, & pulse_counter );
    if( status == NU_SUCCESS )
    {
        /* A/D�������ݴ��� */
        SMVC_SampleUpdateHard( ad_sample_data, pulse_counter );
    }

    FSTK_Restore_Float_Register( pStack );
}

#endif

#ifndef WIN32
void PIT_SystemClockInitialize( void )
{
    /* ��ʼ��ϵͳʱ�� */
    PIT_Cpu_NS_Num = 0;
    PIT_Cpu_NS_Interrupt = GET_CPU_NANOSECOND();
}

/* ��������λ���ר�ö�ʱ���жϺ��� */
void PIT_InterruptHandler( int params )
{
    static UNSIGNED periodRunTime = 0;
    unsigned long variable;
    unsigned long millisecond;
    unsigned long nanosecond;
    unsigned long long cpuNS;
    UNSIGNED intmask;

    PIT_InterruptHandlerCount ++;
    DEVI_ClearPerTimer();

    periodRunTime += DEVI_GetTimerPeriod();
    if( periodRunTime >= 1000 ) /* 1000΢�� */
    {
        /* ��������ʱ�� */
        XJP_TM_Updata_Time( 1 ); /* 1���� */
        periodRunTime = 0;
        /* �ȴ�װ�õ����з�ģ���ʼ����ɺ�,�ٿ��ſ�����·�Լ�,����������� */
        if( CCMC_GetModInitCount() >= CPC_MAX_MODULE_NUMBER )
        {
            /* �������ִ�к��� */
            OUTC_CheckDrive();
        }
    }
    
    intmask = NU_Control_Interrupts( 0 );

    /* Increment system clock */
    cpuNS = GET_CPU_NANOSECOND();
    variable = ( UNSIGNED )( cpuNS - PIT_Cpu_NS_Interrupt );
    variable += PIT_Cpu_NS_Num;
    millisecond = ( UNSIGNED )( variable / 1000000 );
    nanosecond = ( UNSIGNED )( variable % 1000000 );
    TMD_System_Clock += millisecond;
    PIT_Cpu_NS_Num = nanosecond;
    PIT_Cpu_NS_Interrupt = cpuNS;
    
    NU_Control_Interrupts( intmask );

    /* ������״̬��λ��� */
    CINC_SwitchStatusDetection();
}
#endif

#ifndef WIN32
/* �����жϹ����� */
void FIBER_InterruptHandler( int params )
{
    volatile immap_t * im = ( volatile immap_t *)CFG_IMMR;
    volatile ipic83xx_t * p_ipic;
    VOID * pStack;

    p_ipic = & im->ipic;
    p_ipic->sepnr = 0x08000000; //irq4 clear

    pStack = ( VOID *)FIBER_FloatStkBuf;
    FSTK_Save_Float_Register( pStack );

    /* �����жϼ����� */
    FIBER_InterruptHandlerCount ++;
    if( Fiber_Receive_Interrupt )
    {
        (* Fiber_Receive_Interrupt )();
    }

    FSTK_Restore_Float_Register( pStack );
}
#endif

#ifndef WIN32
/* ����PPS������ */
void FIBER_PPSInterruptHandler( int params )
{
    volatile immap_t * im = ( volatile immap_t *)CFG_IMMR;
    volatile ipic83xx_t * p_ipic;
    VOID * pStack;

    p_ipic = & im->ipic;
    p_ipic->sepnr = 0x20000000; //irq2 clear

    pStack = ( VOID *)PPS_FloatStkBuf;
    FSTK_Save_Float_Register( pStack );

    /* ͬ�������жϹ����� */
    FIBER_GPS_IntManage( 0 );
    FSTK_Restore_Float_Register( pStack );

    /* ����PPS��������� */
    FIBER_PPSInterruptHandlerCount ++;
}
#endif

#ifndef WIN32
/* GTM1�жϹ����� */
void GTM1_InterruptHandler( int params )
{
    VOID * pStack;

    pStack = ( VOID *)GTIMER_FloatStkBuf;
    FSTK_Save_Float_Register( pStack );
    /* �����жϴ��������� */
    GTM1_InterruptHandlerCount ++;
    FIBER_Timer_IntManage();
    FSTK_Restore_Float_Register( pStack );
}
#endif

/* ����Ӧ������ */
VOID LoadApplicationData( VOID )
{
    NU_Control_Timer(& Timer_Run_CB, NU_ENABLE_TIMER );
    /* ͨ��ͨ���ĳ�ʼ�� */
    pCommChl = LCC_Link_OpenChannel( 0 );
#if (COMM_HARD_CHANNEL_ENABLE == NU_TRUE)
    /* װ��ʱ�ӳ�ʼ�� */
    LCC_COMM_ClockInitialize( pCommChl );
#endif
    /* ��������ʧ���� */
    CCMC_LoadNVdata();
    /* ������������ */
    PAFC_LeadApplication();
    /* ����HMI���� */
    PMFC_LeadHMIApplication();
    /* ����Gws61850���� */
    PGFC_LeadGwsApplication();
    /* ����ϵͳ��ʼ����ʶ */
    XJP_TM_Set_SysInitFlag( 1 );
    /* ����ʱ����ɱ�־ */
    NU_Set_Events(& Event_2_CB, CPC_EVENT2_CLOCK_READY, NU_OR );
    /* ��ʼ��ʱ����Ϣ */
    CTB_MG_InitDevTimeInfo();
}

/* ���������� */
VOID MainTaskCommRoutine( UNSIGNED ProtectId, UNSIGNED SectorId, INT iResetFlag )
{
    CPC_UNUSED_PARAM( ProtectId );
    CPC_UNUSED_PARAM( SectorId );
    CPC_UNUSED_PARAM( iResetFlag );
    /* �������ݳ�ʼ����� */
    CERC_SampleIniCheckMain();
    /* ��Դ��� */
    CERC_Power_Check();
    /* ��������̵������غ��� */
    OUTC_CheckStartUpReturn();
}

/*=======================================================================*/
/* ͨ�������񣨽���ѹʹ�ã�                                              */
/*=======================================================================*/
#if (COMM_HARD_CHANNEL_ENABLE == NU_TRUE)
/* ͨ�������� */
void   Task_0_Entry( UNSIGNED argc, VOID * argv )
{
    OPTION          old_preempt;
    CPC_UNUSED_PARAM( argc );
    CPC_UNUSED_PARAM( argv );

    old_preempt = NU_Change_Preemption( NU_NO_PREEMPT );
    /* ��ʼ��װ�����л��� */
    DeviceEnvironmentInitialize();
    /* װ�ط���ʧ���� */
    LoadApplicationData();
    /* ����ƽ̨������ʶ */
    CCMC_SetPlatLeadFlag( NU_TRUE );
    NU_Change_Preemption( old_preempt );
    /* ͨ����ѭ�� */
    LCC_MessM_Main( pCommChl );

    while( 1 )
    {
        NU_Relinquish();
    }
}
#endif

/*=======================================================================*/
/* Ӧ�ó���������                                                        */
/*=======================================================================*/
void   Task_1_Entry( UNSIGNED argc, VOID * argv )
{
    HWDM hWatchDogId;
#if (COMM_HARD_CHANNEL_ENABLE == NU_TRUE)
    UNSIGNED  ActualEvent;
#endif
    CHAR EventName[256];
    OPTION          old_preempt;
    CHAR LogName[256] = { 0 };  ///
    CHAR * p;
    static INT ClearErr = 0;
    static INT ClearErr1 = 0;
    INT iStatus;

    CPC_UNUSED_PARAM( argc );
    CPC_UNUSED_PARAM( argv );

#if (COMM_HARD_CHANNEL_ENABLE == NU_TRUE)
    /* �ȴ�ʱ�ӳ�ʼ����� */
    NU_Retrieve_Events(& Event_2_CB, CPC_EVENT2_CLOCK_READY,
        NU_AND,& ActualEvent, NU_SUSPEND );
#else
    old_preempt = NU_Change_Preemption( NU_NO_PREEMPT );
    /* ��ʼ��װ�����л��� */
    DeviceEnvironmentInitialize();
    /* װ�ط���ʧ���� */
    LoadApplicationData();
    /* ����ƽ̨������ʶ */
    CCMC_SetPlatLeadFlag( NU_TRUE );
    NU_Change_Preemption( old_preempt );
#endif

    /* �������Ź������������ */
    hWatchDogId = XJP_Watchdog_Create( WATCHDOG_TIME10, "PTask1", ( UNSIGNED )& Task_1 ); /* ���������������Ӧʱ��10�� */
    /* װ���ϵ��¼ */
    sprintf( EventName, " %03d", LS_RES_EVENT_RESTART );
    CEMC_FillEvent( EventName, RESERVE_EVENT_ID );
    /* ��ʱ��оƬram������(Ferroelectric Nonvolatile RAM) */
    DEVI_GetDataFNRam ( 0, LogName, sizeof( LogName ));
    /* �ж��Ƿ�Ϊ���Ź���λ�����װ������ */
    if( strstr( LogName, WATCHDOG_RESET_FLAG ))
    {
        /* ָ������������λ�� */
        p = & LogName[0] + strlen( WATCHDOG_RESET_FLAG );
        sprintf( EventName, "����-%s-���Ź���λ", p );
        CLOG_Log( ( CHAR *)EventName );
        memset( LogName, 0, sizeof( LogName ));
        /* �����λ��¼ */
        iStatus = DEVI_SaveDataFNRam ( 0, LogName , sizeof( LogName ) );
        if( iStatus != 0 )
        {
            ClearErr ++;
        }
    }

    /* �ж��Ƿ���ϵͳ������Ϣ��¼ */
    DEVI_GetDataFNRam ( SYSTEM_ERR_START_ADDR, LogName, sizeof( LogName ));
    /* �ж��Ƿ�Ϊ���Ź���λ�����װ������ */
    if( strstr( LogName, SYSTEM_ERR_RESET_FLAG ))
    {
        /* ָ������������λ�� */
        p = & LogName[0] + strlen( SYSTEM_ERR_RESET_FLAG );
        sprintf( EventName, "%s", p );
        CLOG_Log( ( CHAR *)EventName );
        memset( LogName, 0, sizeof( LogName ));
        /* �����λ��¼ */
        iStatus = DEVI_SaveDataFNRam ( SYSTEM_ERR_START_ADDR, LogName , sizeof( LogName ) );
        if( iStatus != 0 )
        {
            ClearErr1 ++;
        }
    }
#if (CPC_WATCHDOG_ENABLE == NU_TRUE)
    /* ��ʼ�����Ź� */
    DEVI_InitWatchdog();
#endif

    /* �ȴ�ģ�������� */
    while( 1 )
    {
        /* �ȴ�װ�õ����з�ģ���ʼ����� */
        if( CCMC_GetModInitCount() >= CPC_MAX_MODULE_NUMBER )
        {
            /* ����װ�ó�ʼ����ɱ�־ */
            NU_Set_Events(& Event_2_CB, CPC_EVENT2_INITIALIZE, NU_OR );
            break;
        }
        NU_Sleep( 4 );  /* ���߹������ȼ��������� */
        /* ��λ�����Ӷ��� */
        XJP_Watchdog_Reset( hWatchDogId );
    }

    /* �������������� */
    XJP_Create_AppRoutine( XJP_MAIN_PROTECT, 0, 0, MainTaskCommRoutine );

    while( 1 )
    {
        Task_1_EntryCount ++;

#ifdef WIN32
        /* ���������Ƿ����к��� */
        TASK_Test( 1 );
#endif
#if (PLAT_DEBUG_ENABLE == NU_TRUE)
        XJP_Dbg_Memory_Check( Task_1_MemHandle );
#endif
        /* ����Ӧ�ó��� */
        XJP_Execute_App_Routine( XJP_MAIN_PROTECT );
        /* �����û����� */
        CSMV_MSG_UserSendData();
        XJP_Watchdog_Reset( hWatchDogId ); /* ��λ�����Ӷ��� */
    }
}

/*=======================================================================*/
/* Ӧ�ó��������                                                      */
/*=======================================================================*/
/* ���������� */
VOID OvertimeCheckRoutine( UNSIGNED ProtectId, UNSIGNED SectorId, INT iResetFlag )
{
    UNSIGNED tVariable;
    static INT Iniflag = NU_FALSE;
    static UNSIGNED DevSystemClock;

    if( Iniflag == NU_FALSE )
    {
        DevSystemClock = TMD_System_Clock;
        Iniflag = NU_TRUE;
        return;
    }
    tVariable = TMD_System_Clock - DevSystemClock;
    XJP_OverTime_Main( tVariable );
    DevSystemClock = TMD_System_Clock;
}

void   Task_2_Entry( UNSIGNED argc, VOID * argv )
{
    HWDM hWatchDogId;
    UNSIGNED ActualEvent;
    UNSIGNED uReadTemperatureTime;

    CPC_UNUSED_PARAM( argc );
    CPC_UNUSED_PARAM( argv );

    /* �ȴ�װ�ó�ʼ����� */
    NU_Retrieve_Events(& Event_2_CB, CPC_EVENT2_INITIALIZE,
        NU_AND,& ActualEvent, NU_SUSPEND );

    /* �������Ź������������ */
    hWatchDogId = XJP_Watchdog_Create( WATCHDOG_TIME15, "PTask2", ( UNSIGNED )& Task_2 ); /* ���������������Ӧʱ��10�� */

    /* ���ú󱸳�ʱ��鴦���� */
    XJP_Create_AppRoutine( XJP_BACKUP_PROTECT, 0, 0, OvertimeCheckRoutine );
    uReadTemperatureTime = TMD_System_Clock;
    while( 1 )
    {
        Task_2_EntryCount ++;
#ifdef WIN32
        /* ���������Ƿ����к��� */
        TASK_Test( 2 );
#endif
#if (PLAT_DEBUG_ENABLE == NU_TRUE)
        XJP_Dbg_Memory_Check( Task_2_MemHandle );
#endif
        /* ����Ӧ�ó��� */
        XJP_Execute_App_Routine( XJP_BACKUP_PROTECT );
        /* ��дʱ���ʶ */
        TAST_SaveTime_Flag = 1;
        /* ���ñ���ʱ�亯�� */
        XJP_TM_Save_Time();
        if(( TMD_System_Clock - uReadTemperatureTime )
            >= READ_TEMPERATURE_TIME_INTERVAL )
        {
            /* ��ȡ�¶ȴ��������� */
            MainTemperatureData[0] = DEVI_GetTemperature ( & MainTemperatureData[1] );
            uReadTemperatureTime = TMD_System_Clock;
        }
        /* ��дʱ���ʶ */
        TAST_SaveTime_Flag = 0;
        /* ��λ�����Ӷ��� */
        XJP_Watchdog_Reset( hWatchDogId );
    }
}

/*=======================================================================*/
/* ��̨��������                                                          */
/*=======================================================================*/
void  Task_3_Entry( UNSIGNED argc, VOID * argv )
{
    HWDM hWatchDogId;
    UNSIGNED ActualEvent;
#if (PLAT_DEBUG_ENABLE == NU_TRUE)
    UNSIGNED uCheckDelay;
    char buf[128]; /* buf */
#endif
    UNSIGNED uTextCheckDelay;
    UNSIGNED uFlashCheckDelay;
    INT FlashCheckIndex;
    INT SGErrorNumber;
    INT RunTimeFlag;
    INT status;

    CPC_UNUSED_PARAM( argc );
    CPC_UNUSED_PARAM( argv );

    /* �ȴ�װ�ó�ʼ����� */
    NU_Retrieve_Events(& Event_2_CB, CPC_EVENT2_INITIALIZE,
        NU_AND,& ActualEvent, NU_SUSPEND );

#if (PLAT_DEBUG_ENABLE == NU_TRUE)
    DMScan_Init();
    SECT_Initialize();
    uCheckDelay = TMD_System_Clock;
#endif
    uTextCheckDelay = TMD_System_Clock;
    uFlashCheckDelay = TMD_System_Clock;
    FlashCheckIndex = 0;
    SGErrorNumber = 0;
    RunTimeFlag = XJ_FALSE;

    /* �������Ź������������ */
    hWatchDogId = XJP_Watchdog_Create( WATCHDOG_TIME180, "PTask3", ( UNSIGNED )& Task_3 ); /* ���������������Ӧʱ��180�� */
    /* �����¼������Ź���� */
    CEMC_SetWatchDogId( hWatchDogId );
    /* ����¼�������Ź���� */
    CWMC_SetWatchDogId( hWatchDogId );
    /* ���ñ��������Ź���� */
    CRMC_SetWatchDogId( hWatchDogId );
    /* Loop to allocate and deallocate the resource.  */
    while( 1 )
    {
        Task_3_EntryCount ++;
#ifdef WIN32
        TASK_Test( 3 ); /* ���������Ƿ����к��� */
#endif

#ifndef WIN32
        if( RunTimeFlag != RunTime_Enable )
        {
            if( RunTime_Enable == NU_TRUE )
            {
                /* ��ʼ������ʱ�� */
                CTDC_InitializeRunTime();
                /* ע��ϵͳʱ�� */
                TCCT_RegisterSystemClock( Get_system_clock_nanosecond );
            }
            else
            {
                /* ע��ϵͳʱ�� */
                TCCT_RegisterSystemClock( XJ_NULL );
            }
            RunTimeFlag = RunTime_Enable;
        }
        if( RunTime_Enable == NU_TRUE )
        {
            CTDC_GetRunTime(); /* ��ȡ����(���񡢸߼��жϡ���ʱ��)����ʱ�� */
        }
#endif
        /* ����û������Ƿ����仯 */
        CUDC_CheckUserDataChange( );
        XJP_Watchdog_Reset( hWatchDogId ); /* ��λ�����Ӷ��� */

        /* д��־���� */
        CLOG_WriteLogMain();
        XJP_Watchdog_Reset( hWatchDogId ); /* ��λ�����Ӷ��� */

        CCMC_SaveOutputHoldBits();/* ������ڱ���λ���� */
        XJP_Watchdog_Reset( hWatchDogId ); /* ��λ�����Ӷ��� */

        LC_Block(); /* ��̨����ִ�лص����� */
        XJP_Watchdog_Reset( hWatchDogId ); /* ��λ�����Ӷ��� */

        CCMC_ProgramConfigMain(); /* �������ݹ̻� */
        XJP_Watchdog_Reset( hWatchDogId ); /* ��λ�����Ӷ��� */

        CEMC_EventMain(); /* �¼����� */
        XJP_Watchdog_Reset( hWatchDogId ); /* ��λ�����Ӷ��� */

        CRMC_ReportMain(); /* �������洦�� */
        XJP_Watchdog_Reset( hWatchDogId ); /* ��λ�����Ӷ��� */

        CWMC_WaveMain(); /* ¼������ */
        XJP_Watchdog_Reset( hWatchDogId ); /* ��λ�����Ӷ��� */

        CFDC_DeleteFileMain(); /* ��̨ɾ���ļ������� */
        XJP_Watchdog_Reset( hWatchDogId ); /* ��λ�����Ӷ��� */

        OUTC_CheckMain();/* ���ڼ�������� */
        XJP_Watchdog_Reset( hWatchDogId ); /* ��λ�����Ӷ��� */

        if( ( TMD_System_Clock - uTextCheckDelay ) > 12000 ) /* 60����һ�Σ�5���ļ� */
        {
            CERC_FileCheck_Main(); /* ���г����Լ������� */
            uTextCheckDelay = TMD_System_Clock;
            XJP_Watchdog_Reset( hWatchDogId ); /* ��λ�����Ӷ��� */
        }

        if( ( TMD_System_Clock - uFlashCheckDelay ) > 30000 ) /* 30����һ�� */
        {
            FlashCheckIndex ++;
            switch( FlashCheckIndex )
            {
            case 1:
                /* FLASH��������� */
                CERC_FlashCheckMain();
                break;
                /* 20110329ȥ�������й����жԶ�ֵ���Լ�(�ֳ�����������"��ֵ�Լ����"����ԭ�������������) */
#if 0
            case 2:
                /* ����ֵ�����Լ������� */
                CERC_SettingCheck_Main();
                break;
#endif
            case 2:
                /* �޸�ͨѶ�����ļ� */
                CCMS_RepairFileData_Main();
                break;
            case 3:
                /* ����ֵ������ֵ��⼰�޸� */
                status = XJP_DS_SGValueCheck( TAB_SETTING_ID, CPC_DATA_TIMEOUT );
                if( status == XJ_FALSE )
                {
                    /* ����ֵ���ݼ����� */
                    SGErrorNumber ++;
                    if( SGErrorNumber > 1 )
                    {
                        XJP_DS_Update_SGValue( TAB_SETTING_ID, CPC_DATA_TIMEOUT );
                        SGErrorNumber = 0;
                    }
                }
                else
                {
                    SGErrorNumber = 0;
                }
                FlashCheckIndex = 0;
                break;
            default:
                FlashCheckIndex = 0;
                break;
            }

            uFlashCheckDelay = TMD_System_Clock;
            XJP_Watchdog_Reset( hWatchDogId ); /* ��λ�����Ӷ��� */
        }

#if (PLAT_DEBUG_ENABLE == NU_TRUE)
        XJP_Dbg_Memory_Check( Task_3_MemHandle );
        if( ( TMD_System_Clock - uCheckDelay ) > 10000 ) /* 10����һ�� */
        {
            DMScan_Exec();
            SECT_CrcCheckout( 1 ); /* ����μ�� */
            uCheckDelay = TMD_System_Clock;
        }
#endif

        NU_Relinquish();
    }
}

#if (PLAT_WATCHDOG_ENABLE==XJ_FALSE)
CHAR  WatchDogDebugName[20] = {0}; /*��λ���Ź����Ƶ���*/
#endif
/* Define the run timer.  */
void  Timer_Run_Entry( UNSIGNED argc )
{
    UNSIGNED LedStatus; /* ���е�״̬ */
    int CpuLedStatus; /* CPU�����е�״̬ */
    static UNSIGNED ADCheckDelay = 0;
    CHAR * watchDogName;
    static INT ResetFlag = 0;
    CHAR   LogName[256]; /* ��־���� */
    CHAR   FunName[32];  /* �������� */
    NU_TASK  * pMonitorCtrlCB;
    UNSIGNED actual_return_number;
    UNSIGNED function_return_address[ WATCHDOG_MAX_FUNTION ]; /* �������ص�ַ */
    INT      k;
    TIME     uTimeTest;

    Timer_Run_EntryCount ++;
    /* �ָ������� */
    XJP_ResumeMainTask( TIMER_RUN_TIMESLICE );
#ifdef WIN32
    /* ��������ʱ�� */
    XJP_TM_Updata_Time( TIMER_RUN_TIMESLICE );
#endif
    CpuLedStatus = 0;
    /* �������е�״̬ */
    LedStatus = XJP_Led_Timer( TIMER_RUN_TIMESLICE );
    switch( LedStatus )
    {
    case XJP_LED_EXTINGUISH:
        /* Ϩ��״̬. */
        CpuLedStatus = 0; /* CPU�����е�Ϩ�� */
        break;
    case XJP_LED_LIGHT:
        /* ����״̬. */
        CpuLedStatus = 1; /* CPU�����еƳ��� */
        break;
    default:
        break;
    }
    DEVI_WriteRunLed( CpuLedStatus );
#ifdef WIN32
    /* ������״̬��λ��� */
    CINC_SwitchStatusDetection();
#endif
    /* ���ø澯�������е�״̬���� */
    PMI_SetAlarmANDRunLedStatus();
    /* ���Ź����� */
    watchDogName = XJP_Watchdog_Timer( TIMER_RUN_TIMESLICE, ( UNSIGNED *)& pMonitorCtrlCB );
    if( watchDogName == XJ_NULL )
    {
#ifndef WIN32
        /* ��λ���Ź� */
        DEVI_ResetWatchdog();
#endif
    }
    else
    {
        if(( ResetFlag == 0 ) && ( TAST_SaveTime_Flag == 0 ))
        {
            memset( LogName, 0, sizeof( LogName ));
            actual_return_number = 0;
            /* �������ջ��ȡ���е�ַ */
            CTDC_RetrieveAddressFromTaskStack( pMonitorCtrlCB, & function_return_address[0],
                WATCHDOG_MAX_FUNTION, & actual_return_number );
            /* ��¼��λ������Ϣ */
            sprintf( LogName, "%s%s-fun:", WATCHDOG_RESET_FLAG, watchDogName );
            if( actual_return_number > 0 )
            {
                /* ��¼�������ص�ַ */
                for( k = 0; k < ( INT )( actual_return_number - 1 ); k ++ )
                {
                    sprintf( FunName, "0x%x-", function_return_address[k] );
                    strcat( LogName, FunName );
                }
                sprintf( FunName, "0x%x", function_return_address[( actual_return_number - 1 )] );
                strcat( LogName, FunName );
            }
            XJP_TM_Get_Time_Dev(& uTimeTest );
            sprintf( FunName, " %04d-%02d-%02d %02d:%02d:%05d", (( uTimeTest.Year )+ 2000 ),( uTimeTest.Month ),
                ( uTimeTest.Day ), ( uTimeTest.Hour ),( uTimeTest.Minute ),( uTimeTest.Millionsecond ));
            strcat( LogName, FunName );
            /* ��ʱ��оƬRAM��д����(Ferroelectric Nonvolatile RAM) */
            if( DEVI_SaveDataFNRam ( 0, LogName, sizeof( LogName )) == 0 )
            {
                ResetFlag = 1;
            }
        }
#if (PLAT_WATCHDOG_ENABLE==XJ_FALSE)
        strncpy( WatchDogDebugName, watchDogName, sizeof( WatchDogDebugName )- 1 );
#endif
    }

    /* A/D��������� */
    if( ADCheckDelay >= 5000 ) /* �ȴ���ʼ����� */
        CERC_AD_CheckMain();
    else
        ADCheckDelay ++;
    /* ����װ��ʱ��NPI�෢�Ͷ�ʱ���� */
    if( SendSynTimeCmd )
    {
        (* SendSynTimeCmd )();
    }

    /* �ȴ�װ�õ����з�ģ���ʼ����� */
    if( CCMC_GetModInitCount() >= CPC_MAX_MODULE_NUMBER )
    {
        XJ_Protect( pReport_Protect );
        /* �����ִ��λ�仯��麯�� */
        OUTC_SoftOutExBitsCheck();
        XJ_Unprotect();
    }
}

/* Ӧ��������������������ִ�к��� */
INT MAIN_UserActiveMainTask( VOID )
{
    NU_Activate_HISR(& MainTaskActiveHISR );
    return( NU_SUCCESS );
}

/* �߼��ж�ִ�к��� */
void MainTaskActivesHISR_Routine( void )
{
    /* �ָ������� */
    XJP_ResumeMainTask( 0 );
}

/* ע��UART�����жϺ��� */
INT MainRegUartIntFun( INT channel, VOID (* UartComIntFun )( int para ))
{
#ifndef WIN32
    VOID (* old_lisr )( INT );
    if( UartComIntFun )
    {
        if( channel == 0 )
        {
            NU_Register_LISR( ESAL_PR_UART1_INT_VECTOR_ID, ( void *)UartComIntFun, & old_lisr );
            ESAL_GE_INT_Enable( ESAL_PR_UART1_INT_VECTOR_ID, ESAL_TRIG_LEVEL_LOW  , 0 );
        }
        else
        {
            /* ע�ᴮ���жϺ��� */
            NU_Register_LISR( ESAL_PR_UART2_INT_VECTOR_ID, ( void *)UartComIntFun, & old_lisr );
            ESAL_GE_INT_Enable( ESAL_PR_UART2_INT_VECTOR_ID, ESAL_TRIG_LEVEL_LOW  , 0 );
        }
    }
#endif
    return( NU_SUCCESS );
}

/* ��ȡUART�����ж�����ID */
UNSIGNED MainGetUartVectorID( INT channel )
{
    UNSIGNED uVectorID = ESAL_PR_UART1_INT_VECTOR_ID;
    if( channel == 1 )
    {
        uVectorID = ESAL_PR_UART2_INT_VECTOR_ID;
    }
    return( uVectorID );
}

/* ��ȡϵͳ��ʼ���Ƿ���ɺ���:NU_FALSE-δ���,NU_TRUE-��� */
INT Main_GetSystemInitFlag( VOID )
{
    UNSIGNED ActualEvent;
    INT iStatus = NU_FALSE;

    if( NU_Retrieve_Events(& Event_2_CB, CPC_EVENT2_INITIALIZE,
        NU_AND,& ActualEvent, NU_NO_SUSPEND ) == NU_SUCCESS )
    {
        iStatus = NU_TRUE;
    }
    return( iStatus );
}

/* ��ȡ�¶ȴ��������� */
INT Main_GetTemperature ( INT * temperature_ptr )
{
    * temperature_ptr = MainTemperatureData[1];

    return( MainTemperatureData[0] );
}

VOID Error_Handler_Test( VOID * task_ptr )
{
    INT k;
    INT saveNumber;
    CHAR   LogName[256]; /* ��־���� */
    CHAR   FunName[32];  /* �������� */
    UNSIGNED actual_return_number;
    UNSIGNED function_return_address[ WATCHDOG_MAX_FUNTION ]; /* �������ص�ַ */

    saveNumber = 5;

    while( saveNumber )
    {
        memset( LogName, 0, sizeof( LogName ));
        actual_return_number = 0;
        /* �������ջ��ȡ���е�ַ */
        CTDC_RetrieveAddressFromTaskStack( ( NU_TASK *)task_ptr, & function_return_address[0],
            WATCHDOG_MAX_FUNTION, & actual_return_number );
        /* ��¼��λ������Ϣ */
        sprintf( LogName, "%s%s-fun:", SYSTEM_ERR_RESET_FLAG, "SysError" );
        if( actual_return_number > 0 )
        {
            /* ��¼�������ص�ַ */
            for( k = 0; k < ( INT )( actual_return_number - 1 ); k ++ )
            {
                sprintf( FunName, "0x%x-", function_return_address[k] );
                strcat( LogName, FunName );
            }
            sprintf( FunName, "0x%x", function_return_address[( actual_return_number - 1 )] );
            strcat( LogName, FunName );
        }
        /* ��ʱ��оƬRAM��д����(Ferroelectric Nonvolatile RAM) */
        if( DEVI_SaveDataFNRam ( SYSTEM_ERR_START_ADDR, LogName, sizeof( LogName )) == 0 )
        {
            break;
        }
        saveNumber --;
    }
}
