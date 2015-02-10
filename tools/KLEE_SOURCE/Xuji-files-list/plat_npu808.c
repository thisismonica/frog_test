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

/* 宏定义 */
#define WATCHDOG_TIME10                 10000           /* 看门狗复位延时时间(10秒) */
#define WATCHDOG_TIME15                 15000           /* 看门狗复位延时时间(15秒) */
#define WATCHDOG_TIME60                 60000           /* 看门狗复位延时时间(60秒) */
#define WATCHDOG_TIME180                180000          /* 看门狗复位延时时间(180秒) */
#define WATCHDOG_MAX_FUNTION            12              /* 看门狗监视最大返回函数个数 */
#define READ_TEMPERATURE_TIME_INTERVAL  1000            /* 读取温度时间间隔 */

#define COMM_HARD_CHANNEL_ENABLE        CPC_LINK_HMI_PHY_COM        /* 硬件通信通道使能 */
#define WATCHDOG_RESET_FLAG             "npu808WATCHDOGFLAG-"       /* 看门狗复位记录标识 */
#define SYSTEM_ERR_RESET_FLAG           "npu808SystemErrFlag-"      /* 系统错误标识 */
#define SYSTEM_ERR_START_ADDR           256                         /* 记录系统错误时起始地址 */

/* 以下为OS相关配置 */
#ifdef WIN32
#include "yaffs_fileem.h"
VOID TASK_Test( INT  iTaskID );
#endif

#ifndef WIN32 /* 非windows缺省定义一些宏以便通过编译 */
#include "yaffs_flashif.h"
void PIT_SystemClockInitialize( void );
/* 注册系统时钟 */
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
/* 立即激活主任务高级中断信息 */
NU_HISR MainTaskActiveHISR;
#define MAIN_TASTK_HISR_STACK_SIZE 0x800   /* 堆栈size */
#define MAIN_TASTK_HISR_PRIORITY   2       /* 中断优先级 */
unsigned char MainTastkHisrStack[MAIN_TASTK_HISR_STACK_SIZE];
/* 高级中断执行函数 */
void MainTaskActivesHISR_Routine( void );
/* 应用启动立即激活主任务执行函数 */
INT MAIN_UserActiveMainTask( VOID );
/* 获取系统初始化是否完成函数:NU_FALSE-未完成,NU_TRUE-完成 */
INT Main_GetSystemInitFlag( VOID );
/* 读取温度传感器数据 */
INT Main_GetTemperature ( INT * temperature_ptr );
/* 注册UART串口中断函数 */
INT MainRegUartIntFun( INT channel, VOID (* UartComIntFun )( int para ));
/* 获取UART串口中断向量ID */
UNSIGNED MainGetUartVectorID( INT channel );
/* 智能装置发送对时命令处理函数 */
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
UNSIGNED            AD_FloatStkBuf[100];   /* 浮点数堆栈指针 */
UNSIGNED            FIBER_FloatStkBuf[100];/* 浮点数堆栈指针 */
UNSIGNED            GTIMER_FloatStkBuf[100];/* 浮点数堆栈指针 */
UNSIGNED            PPS_FloatStkBuf[100];/* 浮点数堆栈指针 */
NU_PROTECT          OverCheck_Protect;
NU_PROTECT          DeleteFile_Protect;
unsigned long       PIT_Cpu_NS_Num;
unsigned long long  PIT_Cpu_NS_Interrupt;

/* External variable declarations */
extern  volatile UNSIGNED    TMD_System_Clock;
extern INT (* PMFC_Save_Time )( TIME * pTime );
extern VOID (* Fiber_Receive_Interrupt )( VOID );   /* NPU816中光纤中断处理函数 */
extern VOID (* Fiber_Transmit_Interrupt )( VOID );  /* NPU816中光纤发送数据处理函数 */
extern INT OverTimeTabNumber;
/* 初始化光纤同步脉冲基本参数信息;iSamplePoint:每周波采样点数 */
extern INT FIBER_GPS_Initiliaze( INT iSamplePoint );
/* GTM定时器中断管理函数 */
extern VOID FIBER_Timer_IntManage( VOID );
/* 同步脉冲中断管理函数 */
VOID FIBER_GPS_IntManage( INT iFlag );

/* External function declarations */
extern  VOID        ERC_System_Error ( INT );
extern  STATUS      ERC_Register_Error_Handler( VOID (* error_handler )( VOID * task_ptr ));
extern  VOID        FSTK_Save_Float_Register( register VOID * stack_ptr );
extern  VOID        FSTK_Restore_Float_Register( register VOID * stack_ptr );
#ifndef WIN32
extern  void cpu_init_f ( void );
#endif

/* 平台装置运行灯管理句柄 */
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

VOID    LoadApplicationData( VOID ); /* 引导应用数据 */
int     AD_InterruptHandler( int params ); /* A/D中断处理函数 */
void    FIBER_InterruptHandler( int params ); /* 光纤中断管理函数 */
void    GTM1_InterruptHandler( int params );  /* GTM1中断管理函数 */
/* 光纤PPS处理函数 */
void    FIBER_PPSInterruptHandler( int params );
VOID    DeviceEnvironmentInitialize( VOID ); /* 初始化装置运行环境 */
VOID    MainTaskCommRoutine( UNSIGNED ProtectId, UNSIGNED SectorId, INT iResetFlag ); /* 主任务处理函数 */
#ifndef WIN32
void    PIT_InterruptHandler( int params ); /* 开关量变位检测专用定时器中断函数 */
#endif
INT GetDeviceSystemClock_Precision( UNSIGNED * millisecond_ptr, UNSIGNED * Nanosecond_ptr );
/* 获取系统时钟(毫秒1字、纳秒1字) */
INT GetDeviceSystemClock_64Precision( UNSIGNED * millisecond_ptr, UNSIGNED * Nanosecond_ptr );
VOID OverCheck_Lock( VOID );
VOID OverCheck_Unlock( VOID );
VOID DeleteFile_Lock( VOID );
VOID DeleteFile_Unlock( VOID );
VOID OvertimeCheckRoutine( UNSIGNED ProtectId, UNSIGNED SectorId, INT iResetFlag );
VOID Error_Handler_Test( VOID * task_ptr );

/* 监视任务/定时器/中断是否运行的计数器 */
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
INT MainTemperatureData[2] = {NU_UNAVAILABLE, 0};  /* 记录函数执行结果(0)及获取到的数据信息(1) */

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

    /* 复归继电器延时返回定时器 */
    if ( status == NU_SUCCESS )
    {
        /* initial time:500. reschedule time:500.*/
        status = NU_Create_Timer(& Timer_ResetRelay_CB, "PT3RRL", LSC_RRelayTimerRoutine, 0, 500, 500,
            NU_DISABLE_TIMER );
    }

    /* create event */
    NU_Create_Event_Group(& Event_1_CB, "EVENT 1" );
    NU_Create_Event_Group(& Event_2_CB, "EVENT 2" );

    /* 创建恢复主任务的高级中断 */
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

/* 获取系统时钟(毫秒1字、微秒1字) */
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

/* 获取系统时钟(毫秒1字、纳秒1字) */
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

/* 初始化装置运行环境 */
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
    struct yaffscfg ydiskcfgs[3]; /* 文件分区表结构 */

    /* 平台配置参数的初始化 */
    PMI_InitializePlatCfg();
    /* 初始化供保护使用的平台服务函数表 */
    PAFC_PlatProFunList_Initialize();
    /* 初始化供HMI使用的平台服务函数表 */
    PMFC_PlatHMIFunList_Initialize();
    /* 初始化平台提供给GWS使用的服务函数表 */
    PGFC_PlatGwsFunList_Initialize();
    /* 初始化平台提供给驱动使用的服务函数表 */
    PDFC_PlatDriverFunList_Initialize();
    /*=======================================================================*/
    /* 建立驱动程序链接                                                      */
    /*=======================================================================*/
#if 1
    status = PDFC_LinkDriverApplication( CPC_DRV_TEXT_ADDR, CPC_DRV_TEXT_SIZE, NU_NULL );
    switch( status )
    {
    case XJP_VERSION_ERROR:
        /* 链接对象版本错误 */
        NU_Set_Events(& Event_2_CB, CPC_EVENT2_VERSION_ERROR, NU_OR );
        break;
    case XJP_LOAD_SUCCESS:
        /* 应用对象链接成功 */
        NU_Set_Events(& Event_2_CB, CPC_EVENT2_DRV_READY, NU_OR );
        break;
    default:
        break;
    }
#endif
    /*=======================================================================*/
    /* 初始化驱动程序                                                        */
    /*=======================================================================*/
    /* 初始化驱动程序 */
    PDFC_InitializeDriverApplication( PLAT_mem_pool, PLAT_uncached_mem_pool );
    /* 绑定设备、创建设备通道 */
    DEVI_initialize();
    /* CPU相关设置初始化 */
    DEVI_InitializeCPU();
    /* 初始化运行灯 */
    DEVI_InitializeRunLed();
    /* 初始化告警灯 */
    DEVI_InitializeAlarmLed();
    /* 初始化用户灯 */
    DEVI_InitializeUserLed();
    /* 初始化启动继电器 */
    DEVI_InitializeStartRelay();
    /* 初始化温度传感器 */
    DEVI_InitializeTemperature();
    /* 初始化时钟芯片 */
    DEVI_InitializeTimeChip();
    /*=======================================================================*/
    /* 初始化互斥量 */
    XJP_Mutex_Initialize();
    /* 初始化开入状态量驱动 */
    CINC_CreateInputDriver();
    /* 注册调试函数表 */
    XJP_Dbg_Register_FuncList();
    /* 注册文件系统函数表 */
    PFILE_Register_FileFunctionList();
    /* 临界区初始化 */
    __init_critical_regions();
    /* 文件open初始化 */
    stdio_open_init();
    /* 文件系统初始化 */
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
    /* 与HMI之间建立链接                                                     */
    /*=======================================================================*/
#if 1
    status = PMFC_LinkHMIApplication( CPC_HMI_TEXT_ADDR, CPC_HMI_TEXT_SIZE, NU_NULL );
    switch( status )
    {
    case XJP_VERSION_ERROR:
        /* 链接对象版本错误 */
        NU_Set_Events(& Event_2_CB, CPC_EVENT2_VERSION_ERROR, NU_OR );
        break;
    case XJP_LOAD_SUCCESS:
        /* 应用对象链接成功 */
        NU_Set_Events(& Event_2_CB, CPC_EVENT2_HMI_READY, NU_OR );
        break;
    default:
        break;
    }
#endif
    /*=======================================================================*/
    /* 建立应用程序链接                                                      */
    /*=======================================================================*/
    status = PAFC_LinkApplication( CPC_PROTECT_TEXT_ADDR, CPC_PROTECT_TEXT_SIZE, NU_NULL );
    switch( status )
    {
    case XJP_VERSION_ERROR:
        /* 链接对象版本错误 */
        NU_Set_Events(& Event_2_CB, CPC_EVENT2_VERSION_ERROR, NU_OR );
        break;
    case XJP_LOAD_SUCCESS:
        /* 应用对象链接成功 */
        NU_Set_Events(& Event_2_CB, CPC_EVENT2_PROTECT_READY, NU_OR );
        break;
    default:
        break;
    }
    /*=======================================================================*/
    /* 与Gws61850之间建立链接                                                */
    /*=======================================================================*/
#ifndef WIN32
#if 1
    status = PGFC_LinkGwsApplication( CPC_GWS_TEXT_ADDR, CPC_GWS_TEXT_SIZE, NU_NULL );
    switch( status )
    {
    case XJP_VERSION_ERROR:
        /* 链接对象版本错误 */
        NU_Set_Events(& Event_2_CB, CPC_EVENT2_VERSION_ERROR, NU_OR );
        break;
    case XJP_LOAD_SUCCESS:
        /* 应用对象链接成功 */
        NU_Set_Events(& Event_2_CB, CPC_EVENT2_GWS_READY, NU_OR );
        break;
    default:
        break;
    }
#endif
#endif
    /*=======================================================================*/
    /* 初始化HMI程序                                                         */
    /*=======================================================================*/
#ifndef WIN32
    /* 系统时钟初始化 */
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
    /* 禁止操作系统时钟运行，使用自定义时钟 */
    TMCT_OS_ClockDisable();
    /* GPS对时参数选择 */
    XJP_TM_Set_Time_Para( 0, getGPSStatus, RTC_Set_Rtc_Time, GPS1_RegisterPluseInterrupt,
        GetDeviceSystemClock_Precision, GetDeviceSystemClock_64Precision, getIRIGBStatus  );
    NU_Register_LISR( ESAL_PR_PIT_INT_VECTOR_ID, ( void *)PIT_InterruptHandler, & oldlisr );
    ESAL_GE_INT_Enable( ESAL_PR_PIT_INT_VECTOR_ID, ESAL_TRIG_FALLING_EDGE  , 1 );
    DEVI_InitializePerTimer( CINC_GetFckInputCheckFreq() );
    RTC_Clock_Initial();
    /* 初始化HMI程序 */
    PMFC_InitializeHMIApplication( PLAT_mem_pool, PLAT_uncached_mem_pool );
#endif
    /*=======================================================================*/
    /* 初始化应用程序                                                        */
    /*=======================================================================*/
    /* 创建报告保护句柄 */
    ReportProtectHandle = XJP_NU_Protect_Create( PAF_LOCK_REPORT_NAME );
    pReport_Protect = ( NU_PROTECT *)ReportProtectHandle;
    /* 创建录波保护句柄 */
    WaveProtectHandle = XJP_NU_Protect_Create( PAF_LOCK_WAVE_NAME );
    pWave_Protect = ( NU_PROTECT *)WaveProtectHandle;
    /* 创建测控数据保护句柄 */
    FckProtectHandle = XJP_NU_Protect_Create( PAF_LOCK_FCK_NAME );
    pFck_Protect = ( NU_PROTECT *)FckProtectHandle;
    /* 初始化平台配置文件 */
    PXML_CFG_InitConfig( CPC_CFG_FILE_NAME );
    /* 设置应用程序配置参数 */
    PMI_SetApplicationCfg();
    /* 初始化交换数据信息 */
    CEXC_Initialize( PRO_EXCHANGE_NUMBER );
    /* 通讯所需内存空间申请 */
    LCC_Link_AllocateMemory();
    /* 数据表的初始化 */
    PMI_InitializeTable();
    /* 初始化背景数据 */
    XJP_Background_Initialize( CPC_BACKGROUND_DATA_FILE_NAME );
    /* 注册后备任务句柄 */
    XJP_Register_Backup_Task(& Task_2 );
    /* 初始化应用程序 */
    PAFC_InitializeApplication( PLAT_mem_pool, PLAT_uncached_mem_pool );
    /* 注册数据表处理函数 */
    CTB_MG_RegisterTreatmentFunc();
    /* 获取数据表的数据信息 */
    PMI_GetTableDataInformation();
    /* 初始化内存循环缓冲区 */
    PMI_InitializeCircleMemory();
    /* 日志系统初始化 */
    CLOG_Initialize( 0x3000 / sizeof( UNSIGNED ) );
    /* 删除文件管理模块初始化 */
    CFDC_Initialize( 0x3000, DeleteFile_Lock, DeleteFile_Unlock, xj_RemoveFile, xj_FileExists );
    /* 初始化模拟量参数 */
    CSMV_InitializeChannelPara();
    /* 初始化装置运行灯 */
    PMI_InitializeDeviceLed();
    /* 初始化开出量参数 */
    COUTC_OutputInitialize();
    /* 初始化出口保持位数据 */
    XJP_Output_Hold_Initialize( CPC_OUTPUTHOLD_DATA_DIR_NAME );
    /* 初始化超时检查数据 */
    XJP_Overtime_Initialize( OverTimeTabNumber, OverCheck_Lock, OverCheck_Unlock );
    /* 初始化FALSH分区内存块信息 */
    CERC_FlashCheckInitialize();
    /* 获取FLASH扇区初始信息 */
    CERC_FlashCheckMain();
    /* RAM区程序文件自检信息初始化 */
    CERC_FileInitialize();
    /* 输出复位状态信息 */
    DEVI_RsrInfCPU();
#if 1
    /* 注册进行系统错误记录信息函数 */
    ERC_Register_Error_Handler( Error_Handler_Test );
#endif
    /*=======================================================================*/
    /* 初始化Gws61850程序                                                    */
    /*=======================================================================*/
#ifndef WIN32
    /* 初始化Gws61850程序 */
    PGFC_InitializeGwsApplication( PLAT_mem_pool, PLAT_uncached_mem_pool );
#endif

    chl_number = CSMV_GET_PHY_ANALOG_NUMBER;
    if( chl_number != 0 )
    {
        SendSynTimeCmd = NU_NULL;
        if( PLAT_cfg.device.FunSelect & FUN_IED_DEVICE )
        {
            /* 注册智能装置A/D中断处理函数 */
            eth_npi_if_initialize( CSMV_MessParse );
            /* 注册发送对时命令函数 */
            SendSynTimeCmd = CSMV_Msg_SynTimeCmd;
#ifndef WIN32
            if( PLAT_cfg.device.FunSelect & FUN_OPTICAL )
            {
                /* B系列光纤强制发采样脉冲 */
                DEVI_InitializeSamplePulse( CSMV_GET_PERIOD_POINT, 1 ); /* 强制采样脉冲 */
            }
            /* 在NPU8100装置的时候判别是否有AD采样通道 */
            if(( CSMV_GET_PHY_ANALOG_NUMBER > CSMV_GET_DSAMPLE_SV_CHL_NUM )
                && ( DEVI_GetIdentificationCode() == XJ_IDC_NPU8100 ))
            {
                /* AD采样通道个数 */
                chl_number = CSMV_GET_PHY_ANALOG_NUMBER - CSMV_GET_DSAMPLE_SV_CHL_NUM;
                /* 初始化A/D转换 */
                ad_sample_data = DEVI_InitializeAD( CSMV_GET_PERIOD_POINT, & ad_chl_number );
                if( chl_number < ad_chl_number )
                {
                    ad_chl_number = ( UNSIGNED )chl_number;
                }
                if( ad_chl_number > 0 )
                {
                    /* 初始化采样脉冲 */
                    DEVI_InitializeSamplePulse( CSMV_GET_PERIOD_POINT, 0 );
#ifndef WIN32
                    /* 注册硬件A/D中断处理函数 */
                    NU_Register_LISR( ESAL_PR_IRQ2_INT_VECTOR_ID, ( void *)AD_InterruptHandler, & oldlisr );
                    /* 使能A/D中断 */
                    ESAL_GE_INT_Enable( ESAL_PR_IRQ2_INT_VECTOR_ID, ESAL_TRIG_FALLING_EDGE  , 0 );
#endif
                    /* 设置采样准备完成标识 */
                    CSMV_SetSampleInitFlag( NU_TRUE );
                    PLAT_cfg.smvc.DSampleFlag = NU_TRUE;
                }
            }
#endif
        }
        else
        {
            /* 初始化A/D转换 */
            ad_sample_data = DEVI_InitializeAD( CSMV_GET_PERIOD_POINT, & ad_chl_number );
            if( chl_number < ad_chl_number )
            {
                ad_chl_number = ( UNSIGNED )chl_number;
            }
            if( ad_chl_number > 0 )
            {
                /* 初始化采样脉冲 */
                DEVI_InitializeSamplePulse( CSMV_GET_PERIOD_POINT, 0 );
#ifndef WIN32
                /* 注册硬件A/D中断处理函数 */
                NU_Register_LISR( ESAL_PR_IRQ2_INT_VECTOR_ID, ( void *)AD_InterruptHandler, & oldlisr );
                /* 使能A/D中断 */
                ESAL_GE_INT_Enable( ESAL_PR_IRQ2_INT_VECTOR_ID, ESAL_TRIG_FALLING_EDGE  , 0 );
#endif
                /* 设置采样准备完成标识 */
                CSMV_SetSampleInitFlag( NU_TRUE );
            }
        }
#ifndef WIN32
        /* 选择有光纤通讯功能,同时为NPU816插件时才注册光纤和GTM中断函数 */
        if(( PLAT_cfg.device.FunSelect & FUN_OPTICAL )
            && ( DEVI_GetIdentificationCode() == XJ_IDC_NPU823 ))
        {
            /* 初始化光纤同步脉冲基本参数信息;iSamplePoint:每周波采样点数 */
            FIBER_GPS_Initiliaze( CSMV_GET_PERIOD_POINT );

            /* 注册光纤中断处理函数 */
            NU_Register_LISR( ESAL_PR_IRQ4_INT_VECTOR_ID, ( void *)FIBER_InterruptHandler, & oldlisr );
            ESAL_GE_INT_Enable( ESAL_PR_IRQ4_INT_VECTOR_ID, ESAL_TRIG_FALLING_EDGE  , 0 );

            /* 智能装置的时候需要同步脉冲和GTM定时器中断 */
            if( PLAT_cfg.device.FunSelect & FUN_IED_DEVICE )
            {
                /* 注册GTM中断处理函数 */
                NU_Register_LISR( ESAL_PR_GTM1_INT_VECTOR_ID, ( void *)GTM1_InterruptHandler, & oldlisr );
                ESAL_GE_INT_Enable( ESAL_PR_GTM1_INT_VECTOR_ID, ESAL_TRIG_FALLING_EDGE  , 0 );

                /* 光纤PPS中断处理函数 */
                NU_Register_LISR( ESAL_PR_IRQ2_INT_VECTOR_ID, ( void *)FIBER_PPSInterruptHandler, & oldlisr );
                /* 使能光纤PPS中断 */
                ESAL_GE_INT_Enable( ESAL_PR_IRQ2_INT_VECTOR_ID, ESAL_TRIG_FALLING_EDGE  , 0 );
            }
        }
#endif
    }

#ifndef WIN32
    /* 根据用户选择，重新初始化周期定时器 */
    DEVI_InitializePerTimer( CINC_GetFckInputCheckFreq() );
#endif
    /* 设置开入量检测标志 */
    CINC_SetInputCheckFlag( 1 );
    /* 设置装置分模块初始化成功 */
    CCMC_SetModInitSuccess();
}

#ifndef WIN32
/* A/D中断处理函数 */
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
        /* A/D采样数据处理 */
        SMVC_SampleUpdateHard( ad_sample_data, pulse_counter );
    }

    FSTK_Restore_Float_Register( pStack );
}

#endif

#ifndef WIN32
void PIT_SystemClockInitialize( void )
{
    /* 初始化系统时钟 */
    PIT_Cpu_NS_Num = 0;
    PIT_Cpu_NS_Interrupt = GET_CPU_NANOSECOND();
}

/* 开关量变位检测专用定时器中断函数 */
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
    if( periodRunTime >= 1000 ) /* 1000微秒 */
    {
        /* 更新运行时间 */
        XJP_TM_Updata_Time( 1 ); /* 1毫秒 */
        periodRunTime = 0;
        /* 等待装置的所有分模块初始化完成后,再开放开出回路自检,否则击穿反复 */
        if( CCMC_GetModInitCount() >= CPC_MAX_MODULE_NUMBER )
        {
            /* 开出检测执行函数 */
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

    /* 开关量状态变位检测 */
    CINC_SwitchStatusDetection();
}
#endif

#ifndef WIN32
/* 光纤中断管理函数 */
void FIBER_InterruptHandler( int params )
{
    volatile immap_t * im = ( volatile immap_t *)CFG_IMMR;
    volatile ipic83xx_t * p_ipic;
    VOID * pStack;

    p_ipic = & im->ipic;
    p_ipic->sepnr = 0x08000000; //irq4 clear

    pStack = ( VOID *)FIBER_FloatStkBuf;
    FSTK_Save_Float_Register( pStack );

    /* 光纤中断计数器 */
    FIBER_InterruptHandlerCount ++;
    if( Fiber_Receive_Interrupt )
    {
        (* Fiber_Receive_Interrupt )();
    }

    FSTK_Restore_Float_Register( pStack );
}
#endif

#ifndef WIN32
/* 光纤PPS处理函数 */
void FIBER_PPSInterruptHandler( int params )
{
    volatile immap_t * im = ( volatile immap_t *)CFG_IMMR;
    volatile ipic83xx_t * p_ipic;
    VOID * pStack;

    p_ipic = & im->ipic;
    p_ipic->sepnr = 0x20000000; //irq2 clear

    pStack = ( VOID *)PPS_FloatStkBuf;
    FSTK_Save_Float_Register( pStack );

    /* 同步脉冲中断管理函数 */
    FIBER_GPS_IntManage( 0 );
    FSTK_Restore_Float_Register( pStack );

    /* 光纤PPS处理计数器 */
    FIBER_PPSInterruptHandlerCount ++;
}
#endif

#ifndef WIN32
/* GTM1中断管理函数 */
void GTM1_InterruptHandler( int params )
{
    VOID * pStack;

    pStack = ( VOID *)GTIMER_FloatStkBuf;
    FSTK_Save_Float_Register( pStack );
    /* 调整中断次数计数器 */
    GTM1_InterruptHandlerCount ++;
    FIBER_Timer_IntManage();
    FSTK_Restore_Float_Register( pStack );
}
#endif

/* 引导应用数据 */
VOID LoadApplicationData( VOID )
{
    NU_Control_Timer(& Timer_Run_CB, NU_ENABLE_TIMER );
    /* 通信通道的初始化 */
    pCommChl = LCC_Link_OpenChannel( 0 );
#if (COMM_HARD_CHANNEL_ENABLE == NU_TRUE)
    /* 装置时钟初始化 */
    LCC_COMM_ClockInitialize( pCommChl );
#endif
    /* 引导非易失数据 */
    CCMC_LoadNVdata();
    /* 引导保护程序 */
    PAFC_LeadApplication();
    /* 引导HMI程序 */
    PMFC_LeadHMIApplication();
    /* 引导Gws61850程序 */
    PGFC_LeadGwsApplication();
    /* 设置系统初始化标识 */
    XJP_TM_Set_SysInitFlag( 1 );
    /* 设置时钟完成标志 */
    NU_Set_Events(& Event_2_CB, CPC_EVENT2_CLOCK_READY, NU_OR );
    /* 初始化时间信息 */
    CTB_MG_InitDevTimeInfo();
}

/* 主任务处理函数 */
VOID MainTaskCommRoutine( UNSIGNED ProtectId, UNSIGNED SectorId, INT iResetFlag )
{
    CPC_UNUSED_PARAM( ProtectId );
    CPC_UNUSED_PARAM( SectorId );
    CPC_UNUSED_PARAM( iResetFlag );
    /* 采样数据初始化检测 */
    CERC_SampleIniCheckMain();
    /* 电源检测 */
    CERC_Power_Check();
    /* 检测启动继电器返回函数 */
    OUTC_CheckStartUpReturn();
}

/*=======================================================================*/
/* 通信主任务（仅高压使用）                                              */
/*=======================================================================*/
#if (COMM_HARD_CHANNEL_ENABLE == NU_TRUE)
/* 通信主任务 */
void   Task_0_Entry( UNSIGNED argc, VOID * argv )
{
    OPTION          old_preempt;
    CPC_UNUSED_PARAM( argc );
    CPC_UNUSED_PARAM( argv );

    old_preempt = NU_Change_Preemption( NU_NO_PREEMPT );
    /* 初始化装置运行环境 */
    DeviceEnvironmentInitialize();
    /* 装载非易失数据 */
    LoadApplicationData();
    /* 设置平台引导标识 */
    CCMC_SetPlatLeadFlag( NU_TRUE );
    NU_Change_Preemption( old_preempt );
    /* 通信主循环 */
    LCC_MessM_Main( pCommChl );

    while( 1 )
    {
        NU_Relinquish();
    }
}
#endif

/*=======================================================================*/
/* 应用程序主任务                                                        */
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
    /* 等待时钟初始化完成 */
    NU_Retrieve_Events(& Event_2_CB, CPC_EVENT2_CLOCK_READY,
        NU_AND,& ActualEvent, NU_SUSPEND );
#else
    old_preempt = NU_Change_Preemption( NU_NO_PREEMPT );
    /* 初始化装置运行环境 */
    DeviceEnvironmentInitialize();
    /* 装载非易失数据 */
    LoadApplicationData();
    /* 设置平台引导标识 */
    CCMC_SetPlatLeadFlag( NU_TRUE );
    NU_Change_Preemption( old_preempt );
#endif

    /* 创建看门狗监视任务对象 */
    hWatchDogId = XJP_Watchdog_Create( WATCHDOG_TIME10, "PTask1", ( UNSIGNED )& Task_1 ); /* 被监视任务最大响应时间10秒 */
    /* 装置上电记录 */
    sprintf( EventName, " %03d", LS_RES_EVENT_RESTART );
    CEMC_FillEvent( EventName, RESERVE_EVENT_ID );
    /* 读时钟芯片ram区数据(Ferroelectric Nonvolatile RAM) */
    DEVI_GetDataFNRam ( 0, LogName, sizeof( LogName ));
    /* 判断是否为看门狗复位引起的装置重启 */
    if( strstr( LogName, WATCHDOG_RESET_FLAG ))
    {
        /* 指定到任务名称位置 */
        p = & LogName[0] + strlen( WATCHDOG_RESET_FLAG );
        sprintf( EventName, "任务-%s-看门狗复位", p );
        CLOG_Log( ( CHAR *)EventName );
        memset( LogName, 0, sizeof( LogName ));
        /* 清除复位记录 */
        iStatus = DEVI_SaveDataFNRam ( 0, LogName , sizeof( LogName ) );
        if( iStatus != 0 )
        {
            ClearErr ++;
        }
    }

    /* 判断是否有系统错误信息记录 */
    DEVI_GetDataFNRam ( SYSTEM_ERR_START_ADDR, LogName, sizeof( LogName ));
    /* 判断是否为看门狗复位引起的装置重启 */
    if( strstr( LogName, SYSTEM_ERR_RESET_FLAG ))
    {
        /* 指定到任务名称位置 */
        p = & LogName[0] + strlen( SYSTEM_ERR_RESET_FLAG );
        sprintf( EventName, "%s", p );
        CLOG_Log( ( CHAR *)EventName );
        memset( LogName, 0, sizeof( LogName ));
        /* 清除复位记录 */
        iStatus = DEVI_SaveDataFNRam ( SYSTEM_ERR_START_ADDR, LogName , sizeof( LogName ) );
        if( iStatus != 0 )
        {
            ClearErr1 ++;
        }
    }
#if (CPC_WATCHDOG_ENABLE == NU_TRUE)
    /* 初始化看门狗 */
    DEVI_InitWatchdog();
#endif

    /* 等待模块加载完成 */
    while( 1 )
    {
        /* 等待装置的所有分模块初始化完成 */
        if( CCMC_GetModInitCount() >= CPC_MAX_MODULE_NUMBER )
        {
            /* 设置装置初始化完成标志 */
            NU_Set_Events(& Event_2_CB, CPC_EVENT2_INITIALIZE, NU_OR );
            break;
        }
        NU_Sleep( 4 );  /* 休眠供低优先级任务运行 */
        /* 复位被监视对象 */
        XJP_Watchdog_Reset( hWatchDogId );
    }

    /* 设置主任务处理函数 */
    XJP_Create_AppRoutine( XJP_MAIN_PROTECT, 0, 0, MainTaskCommRoutine );

    while( 1 )
    {
        Task_1_EntryCount ++;

#ifdef WIN32
        /* 测试任务是否运行函数 */
        TASK_Test( 1 );
#endif
#if (PLAT_DEBUG_ENABLE == NU_TRUE)
        XJP_Dbg_Memory_Check( Task_1_MemHandle );
#endif
        /* 调度应用程序 */
        XJP_Execute_App_Routine( XJP_MAIN_PROTECT );
        /* 发送用户数据 */
        CSMV_MSG_UserSendData();
        XJP_Watchdog_Reset( hWatchDogId ); /* 复位被监视对象 */
    }
}

/*=======================================================================*/
/* 应用程序后备任务                                                      */
/*=======================================================================*/
/* 主任务处理函数 */
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

    /* 等待装置初始化完成 */
    NU_Retrieve_Events(& Event_2_CB, CPC_EVENT2_INITIALIZE,
        NU_AND,& ActualEvent, NU_SUSPEND );

    /* 创建看门狗监视任务对象 */
    hWatchDogId = XJP_Watchdog_Create( WATCHDOG_TIME15, "PTask2", ( UNSIGNED )& Task_2 ); /* 被监视任务最大响应时间10秒 */

    /* 设置后备超时检查处理函数 */
    XJP_Create_AppRoutine( XJP_BACKUP_PROTECT, 0, 0, OvertimeCheckRoutine );
    uReadTemperatureTime = TMD_System_Clock;
    while( 1 )
    {
        Task_2_EntryCount ++;
#ifdef WIN32
        /* 测试任务是否运行函数 */
        TASK_Test( 2 );
#endif
#if (PLAT_DEBUG_ENABLE == NU_TRUE)
        XJP_Dbg_Memory_Check( Task_2_MemHandle );
#endif
        /* 调度应用程序 */
        XJP_Execute_App_Routine( XJP_BACKUP_PROTECT );
        /* 置写时间标识 */
        TAST_SaveTime_Flag = 1;
        /* 调用保存时间函数 */
        XJP_TM_Save_Time();
        if(( TMD_System_Clock - uReadTemperatureTime )
            >= READ_TEMPERATURE_TIME_INTERVAL )
        {
            /* 读取温度传感器数据 */
            MainTemperatureData[0] = DEVI_GetTemperature ( & MainTemperatureData[1] );
            uReadTemperatureTime = TMD_System_Clock;
        }
        /* 清写时间标识 */
        TAST_SaveTime_Flag = 0;
        /* 复位被监视对象 */
        XJP_Watchdog_Reset( hWatchDogId );
    }
}

/*=======================================================================*/
/* 后台管理任务                                                          */
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

    /* 等待装置初始化完成 */
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

    /* 创建看门狗监视任务对象 */
    hWatchDogId = XJP_Watchdog_Create( WATCHDOG_TIME180, "PTask3", ( UNSIGNED )& Task_3 ); /* 被监视任务最大响应时间180秒 */
    /* 设置事件任务看门狗句柄 */
    CEMC_SetWatchDogId( hWatchDogId );
    /* 设置录波任务看门狗句柄 */
    CWMC_SetWatchDogId( hWatchDogId );
    /* 设置报告任务看门狗句柄 */
    CRMC_SetWatchDogId( hWatchDogId );
    /* Loop to allocate and deallocate the resource.  */
    while( 1 )
    {
        Task_3_EntryCount ++;
#ifdef WIN32
        TASK_Test( 3 ); /* 测试任务是否运行函数 */
#endif

#ifndef WIN32
        if( RunTimeFlag != RunTime_Enable )
        {
            if( RunTime_Enable == NU_TRUE )
            {
                /* 初始化运行时间 */
                CTDC_InitializeRunTime();
                /* 注册系统时钟 */
                TCCT_RegisterSystemClock( Get_system_clock_nanosecond );
            }
            else
            {
                /* 注册系统时钟 */
                TCCT_RegisterSystemClock( XJ_NULL );
            }
            RunTimeFlag = RunTime_Enable;
        }
        if( RunTime_Enable == NU_TRUE )
        {
            CTDC_GetRunTime(); /* 获取对象(任务、高级中断、定时器)运行时间 */
        }
#endif
        /* 检测用户数据是否发生变化 */
        CUDC_CheckUserDataChange( );
        XJP_Watchdog_Reset( hWatchDogId ); /* 复位被监视对象 */

        /* 写日志数据 */
        CLOG_WriteLogMain();
        XJP_Watchdog_Reset( hWatchDogId ); /* 复位被监视对象 */

        CCMC_SaveOutputHoldBits();/* 保存出口保持位函数 */
        XJP_Watchdog_Reset( hWatchDogId ); /* 复位被监视对象 */

        LC_Block(); /* 后台任务执行回调函数 */
        XJP_Watchdog_Reset( hWatchDogId ); /* 复位被监视对象 */

        CCMC_ProgramConfigMain(); /* 配置数据固化 */
        XJP_Watchdog_Reset( hWatchDogId ); /* 复位被监视对象 */

        CEMC_EventMain(); /* 事件处理 */
        XJP_Watchdog_Reset( hWatchDogId ); /* 复位被监视对象 */

        CRMC_ReportMain(); /* 动作报告处理 */
        XJP_Watchdog_Reset( hWatchDogId ); /* 复位被监视对象 */

        CWMC_WaveMain(); /* 录波处理 */
        XJP_Watchdog_Reset( hWatchDogId ); /* 复位被监视对象 */

        CFDC_DeleteFileMain(); /* 后台删除文件主函数 */
        XJP_Watchdog_Reset( hWatchDogId ); /* 复位被监视对象 */

        OUTC_CheckMain();/* 出口检测主函数 */
        XJP_Watchdog_Reset( hWatchDogId ); /* 复位被监视对象 */

        if( ( TMD_System_Clock - uTextCheckDelay ) > 12000 ) /* 60秒检测一次，5个文件 */
        {
            CERC_FileCheck_Main(); /* 运行程序自检主函数 */
            uTextCheckDelay = TMD_System_Clock;
            XJP_Watchdog_Reset( hWatchDogId ); /* 复位被监视对象 */
        }

        if( ( TMD_System_Clock - uFlashCheckDelay ) > 30000 ) /* 30秒检测一次 */
        {
            FlashCheckIndex ++;
            switch( FlashCheckIndex )
            {
            case 1:
                /* FLASH检测主函数 */
                CERC_FlashCheckMain();
                break;
                /* 20110329去掉在运行过程中对定值的自检(现场出现运行中"定值自检出错"问题原因后续继续查找) */
#if 0
            case 2:
                /* 整定值数据自检主函数 */
                CERC_SettingCheck_Main();
                break;
#endif
            case 2:
                /* 修复通讯报文文件 */
                CCMS_RepairFileData_Main();
                break;
            case 3:
                /* 整定值数据量值检测及修复 */
                status = XJP_DS_SGValueCheck( TAB_SETTING_ID, CPC_DATA_TIMEOUT );
                if( status == XJ_FALSE )
                {
                    /* 整定值数据检测出错 */
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
            XJP_Watchdog_Reset( hWatchDogId ); /* 复位被监视对象 */
        }

#if (PLAT_DEBUG_ENABLE == NU_TRUE)
        XJP_Dbg_Memory_Check( Task_3_MemHandle );
        if( ( TMD_System_Clock - uCheckDelay ) > 10000 ) /* 10秒检测一次 */
        {
            DMScan_Exec();
            SECT_CrcCheckout( 1 ); /* 程序段检查 */
            uCheckDelay = TMD_System_Clock;
        }
#endif

        NU_Relinquish();
    }
}

#if (PLAT_WATCHDOG_ENABLE==XJ_FALSE)
CHAR  WatchDogDebugName[20] = {0}; /*复位看门狗名称调试*/
#endif
/* Define the run timer.  */
void  Timer_Run_Entry( UNSIGNED argc )
{
    UNSIGNED LedStatus; /* 运行灯状态 */
    int CpuLedStatus; /* CPU板运行灯状态 */
    static UNSIGNED ADCheckDelay = 0;
    CHAR * watchDogName;
    static INT ResetFlag = 0;
    CHAR   LogName[256]; /* 日志名称 */
    CHAR   FunName[32];  /* 函数名称 */
    NU_TASK  * pMonitorCtrlCB;
    UNSIGNED actual_return_number;
    UNSIGNED function_return_address[ WATCHDOG_MAX_FUNTION ]; /* 函数返回地址 */
    INT      k;
    TIME     uTimeTest;

    Timer_Run_EntryCount ++;
    /* 恢复主任务 */
    XJP_ResumeMainTask( TIMER_RUN_TIMESLICE );
#ifdef WIN32
    /* 更新运行时间 */
    XJP_TM_Updata_Time( TIMER_RUN_TIMESLICE );
#endif
    CpuLedStatus = 0;
    /* 更新运行灯状态 */
    LedStatus = XJP_Led_Timer( TIMER_RUN_TIMESLICE );
    switch( LedStatus )
    {
    case XJP_LED_EXTINGUISH:
        /* 熄灭状态. */
        CpuLedStatus = 0; /* CPU板运行灯熄灭 */
        break;
    case XJP_LED_LIGHT:
        /* 常亮状态. */
        CpuLedStatus = 1; /* CPU板运行灯常亮 */
        break;
    default:
        break;
    }
    DEVI_WriteRunLed( CpuLedStatus );
#ifdef WIN32
    /* 开关量状态变位检测 */
    CINC_SwitchStatusDetection();
#endif
    /* 设置告警灯与运行灯状态函数 */
    PMI_SetAlarmANDRunLedStatus();
    /* 看门狗监视 */
    watchDogName = XJP_Watchdog_Timer( TIMER_RUN_TIMESLICE, ( UNSIGNED *)& pMonitorCtrlCB );
    if( watchDogName == XJ_NULL )
    {
#ifndef WIN32
        /* 复位看门狗 */
        DEVI_ResetWatchdog();
#endif
    }
    else
    {
        if(( ResetFlag == 0 ) && ( TAST_SaveTime_Flag == 0 ))
        {
            memset( LogName, 0, sizeof( LogName ));
            actual_return_number = 0;
            /* 从任务堆栈获取运行地址 */
            CTDC_RetrieveAddressFromTaskStack( pMonitorCtrlCB, & function_return_address[0],
                WATCHDOG_MAX_FUNTION, & actual_return_number );
            /* 记录复位任务信息 */
            sprintf( LogName, "%s%s-fun:", WATCHDOG_RESET_FLAG, watchDogName );
            if( actual_return_number > 0 )
            {
                /* 记录函数返回地址 */
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
            /* 向时钟芯片RAM区写数据(Ferroelectric Nonvolatile RAM) */
            if( DEVI_SaveDataFNRam ( 0, LogName, sizeof( LogName )) == 0 )
            {
                ResetFlag = 1;
            }
        }
#if (PLAT_WATCHDOG_ENABLE==XJ_FALSE)
        strncpy( WatchDogDebugName, watchDogName, sizeof( WatchDogDebugName )- 1 );
#endif
    }

    /* A/D检测主函数 */
    if( ADCheckDelay >= 5000 ) /* 等待初始化完成 */
        CERC_AD_CheckMain();
    else
        ADCheckDelay ++;
    /* 智能装置时向NPI侧发送对时命令 */
    if( SendSynTimeCmd )
    {
        (* SendSynTimeCmd )();
    }

    /* 等待装置的所有分模块初始化完成 */
    if( CCMC_GetModInitCount() >= CPC_MAX_MODULE_NUMBER )
    {
        XJ_Protect( pReport_Protect );
        /* 软出口执行位变化检查函数 */
        OUTC_SoftOutExBitsCheck();
        XJ_Unprotect();
    }
}

/* 应用启动立即激活主任务执行函数 */
INT MAIN_UserActiveMainTask( VOID )
{
    NU_Activate_HISR(& MainTaskActiveHISR );
    return( NU_SUCCESS );
}

/* 高级中断执行函数 */
void MainTaskActivesHISR_Routine( void )
{
    /* 恢复主任务 */
    XJP_ResumeMainTask( 0 );
}

/* 注册UART串口中断函数 */
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
            /* 注册串口中断函数 */
            NU_Register_LISR( ESAL_PR_UART2_INT_VECTOR_ID, ( void *)UartComIntFun, & old_lisr );
            ESAL_GE_INT_Enable( ESAL_PR_UART2_INT_VECTOR_ID, ESAL_TRIG_LEVEL_LOW  , 0 );
        }
    }
#endif
    return( NU_SUCCESS );
}

/* 获取UART串口中断向量ID */
UNSIGNED MainGetUartVectorID( INT channel )
{
    UNSIGNED uVectorID = ESAL_PR_UART1_INT_VECTOR_ID;
    if( channel == 1 )
    {
        uVectorID = ESAL_PR_UART2_INT_VECTOR_ID;
    }
    return( uVectorID );
}

/* 获取系统初始化是否完成函数:NU_FALSE-未完成,NU_TRUE-完成 */
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

/* 读取温度传感器数据 */
INT Main_GetTemperature ( INT * temperature_ptr )
{
    * temperature_ptr = MainTemperatureData[1];

    return( MainTemperatureData[0] );
}

VOID Error_Handler_Test( VOID * task_ptr )
{
    INT k;
    INT saveNumber;
    CHAR   LogName[256]; /* 日志名称 */
    CHAR   FunName[32];  /* 函数名称 */
    UNSIGNED actual_return_number;
    UNSIGNED function_return_address[ WATCHDOG_MAX_FUNTION ]; /* 函数返回地址 */

    saveNumber = 5;

    while( saveNumber )
    {
        memset( LogName, 0, sizeof( LogName ));
        actual_return_number = 0;
        /* 从任务堆栈获取运行地址 */
        CTDC_RetrieveAddressFromTaskStack( ( NU_TASK *)task_ptr, & function_return_address[0],
            WATCHDOG_MAX_FUNTION, & actual_return_number );
        /* 记录复位任务信息 */
        sprintf( LogName, "%s%s-fun:", SYSTEM_ERR_RESET_FLAG, "SysError" );
        if( actual_return_number > 0 )
        {
            /* 记录函数返回地址 */
            for( k = 0; k < ( INT )( actual_return_number - 1 ); k ++ )
            {
                sprintf( FunName, "0x%x-", function_return_address[k] );
                strcat( LogName, FunName );
            }
            sprintf( FunName, "0x%x", function_return_address[( actual_return_number - 1 )] );
            strcat( LogName, FunName );
        }
        /* 向时钟芯片RAM区写数据(Ferroelectric Nonvolatile RAM) */
        if( DEVI_SaveDataFNRam ( SYSTEM_ERR_START_ADDR, LogName, sizeof( LogName )) == 0 )
        {
            break;
        }
        saveNumber --;
    }
}
