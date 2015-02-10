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
*       nucleus.h
*
*   COMPONENT
*
*       System Constants
*
*   DESCRIPTION
*
*       This file contains system constants common to both the
*       application and the actual Nucleus PLUS components.
*
*   DATA STRUCTURES
*
*       NU_DRIVER                           I/O Driver control block
*       NU_EVENT_GROUP                      Event group control block
*       NU_HISR                             HISR control block
*       NU_MAILBOX                          Mailbox control block
*       NU_MEMORY_POOL                      Memory Pool control block
*       NU_PARTITION_POOL                   Partition Pool control block
*       NU_PIPE                             Pipe control block
*       NU_QUEUE                            Queue control block
*       NU_SEMAPHORE                        Semaphore control block
*       NU_TASK                             Task control block
*       NU_TIMER                            Timer control block
*       NU_PROTECT                          Protection structure
*
*   DEPENDENCIES
*
*       esal_extern.h                       Embedded Software
*                                           Abstraction Layer external
*                                           interface
*       plus_cfg.h                          Nucleus PLUS configuration
*                                           settings
*
***********************************************************************/

/* Check to see if this file has been included already.  */

#ifndef         NUCLEUS

#ifdef          __cplusplus

/* C declarations in C++     */
extern          "C" {

#endif

#define         NUCLEUS

/* Include Embedded Software Abstraction Layer external interface */
#include        "esal/esal_extern.h"

/* Define Nucleus PLUS release string */
#define         PLUS_RELEASE_STRING                 "Nucleus PLUS 2.1"

/* Define major / minor release numbers for Nucleus PLUS */
#define         NU_PLUS_RELEASE_MAJOR_VERSION       2
#define         NU_PLUS_RELEASE_MINOR_VERSION       1
#define         NU_PLUS_RELEASE_PATCH_VERSION       0

/* Starting with PLUS 2.0, PLUS_VERSION_COMP reflects the actual
   version number.  PLUS_VERSION_COMP is MMNNPP, where MM is the major version,
   NN is the minor version, and PP is the patch version */

/* Macro for generating a release version from major, minor, and patch version */
#define         NU_PLUS_RELEASE_VERSION_MAKE(m,n,p) ((m*10000)+(n*100)+p)

/* Define Nucleus PLUS version numbering */
#define         PLUS_1_11                           1
#define         PLUS_1_13                           2
#define         PLUS_1_14                           3
#define         PLUS_1_15                           4
#define         PLUS_2_0                            NU_PLUS_RELEASE_VERSION_MAKE(2,0,0)
#define         PLUS_2_1                            NU_PLUS_RELEASE_VERSION_MAKE(2,1,0)

/* Current version */
#define         PLUS_VERSION_COMP                   NU_PLUS_RELEASE_VERSION_MAKE(       \
                                                        NU_PLUS_RELEASE_MAJOR_VERSION,  \
                                                        NU_PLUS_RELEASE_MINOR_VERSION,  \
                                                        NU_PLUS_RELEASE_PATCH_VERSION)

/* Define TRUE and FALSE values */
#define         NU_TRUE                             1
#define         NU_FALSE                            0

/* Common types utilized within Nucleus code */
typedef         UINT32                              UNSIGNED;
typedef         INT32                               SIGNED;
typedef         UINT8                               DATA_ELEMENT;
typedef         UINT8                               OPTION;
typedef         UINT8                               BOOLEAN;
typedef         INT                                 STATUS;
typedef         UINT8                               UNSIGNED_CHAR;
typedef         char                                CHAR;
typedef         UINT                                UNSIGNED_INT;
typedef         UINT32 *                            UNSIGNED_PTR;
typedef         UINT8 *                             BYTE_PTR;

/* Ensure HUGE / FAR pointer type specified - required for
   some 16-bit architectures for pointer crossing page boundaries */
#ifndef         HUGE
#define         HUGE                                ESAL_TS_HUGE_PTR_TYPE
#endif

#ifndef         FAR
#define         FAR                                 ESAL_TS_FAR_PTR_TYPE
#endif

/* Define register defines.  R1, R2, R3, and R4 are used in the Nucleus PLUS
   source code in front of variables that are referenced often.  In some
   ports, defining them as "register" will improve performance.  */
#define         R1                                  ESAL_REG
#define         R2                                  ESAL_REG
#define         R3                                  ESAL_REG
#define         R4                                  ESAL_REG

/* Define interrupt lockout and enable constants. */
#define         NU_DISABLE_INTERRUPTS               ESAL_GE_INT_DISABLE_BITS
#define         NU_ENABLE_INTERRUPTS                ESAL_GE_INT_ENABLE_BITS

/* Define the maximum object string name size in the system */
#define         NU_MAX_NAME                         8

/* Defines used to align / pad structure members of various Nucleus structures.  Certain
   architectures may require alignment */
#ifndef         ESAL_TS_PAD_1BYTE_ALIGN
#define         PAD_1                               0
#else
#define         PAD_1                               ESAL_TS_PAD_1BYTE_ALIGN
#endif  /* !ESAL_TS_PAD_1BYTE_ALIGN */

#ifndef         ESAL_TS_PAD_2BYTE_ALIGN
#define         PAD_2                               0
#else
#define         PAD_2                               ESAL_TS_PAD_2BYTE_ALIGN
#endif  /* !ESAL_TS_PAD_2BYTE_ALIGN */

#ifndef         ESAL_TS_PAD_2BYTE_ALIGN
#define         PAD_3                               0
#else
#define         PAD_3                               ESAL_TS_PAD_3BYTE_ALIGN
#endif  /* !ESAL_TS_PAD_3BYTE_ALIGN */

/* Include Nucleus PLUS configuration settings */
#include        "plus/plus_cfg.h"

/* Check if multiple timing tests being configured */
#if ((NU_TIME_TEST1MIN_ENABLE + NU_TIME_TEST1MAX_ENABLE +    \
      NU_TIME_TEST2_ENABLE + NU_TIME_TEST3_ENABLE) > 1)

/* Show an error - only 1 timing test can be configured */
#error          Only 1 timing test may be configured - see plus_cfg.h

#endif

/* Check if timing test 1 (minimum code / data) is enabled */
#if (NU_TIME_TEST1MIN_ENABLE == NU_TRUE)

/* Ensure all configuration settings build minimal code / data */
#undef  NU_ERROR_CHECKING
#define NU_ERROR_CHECKING               NU_FALSE
#undef  NU_PLUS_INLINING
#define NU_PLUS_INLINING                NU_FALSE
#undef  NU_MIN_RAM_ENABLED
#define NU_MIN_RAM_ENABLED              NU_TRUE
#undef  NU_PLUS_OBJECT_LISTS_INCLUDE
#define NU_PLUS_OBJECT_LISTS_INCLUDE    NU_FALSE
#undef  NU_NMI_SUPPORT_ENABLED
#define NU_NMI_SUPPORT_ENABLED          NU_FALSE

#endif  /* NU_TIME_TEST1MIN_ENABLE == NU_TRUE */

/* Check if timing test 1 (maximum code / data) is enabled */
#if (NU_TIME_TEST1MAX_ENABLE == NU_TRUE)

/* Ensure all configuration settings build maximum code / data */
#undef  NU_ERROR_CHECKING
#define NU_ERROR_CHECKING               NU_TRUE
#undef  NU_PLUS_INLINING
#define NU_PLUS_INLINING                NU_TRUE
#undef  NU_MIN_RAM_ENABLED
#define NU_MIN_RAM_ENABLED              NU_FALSE
#undef  NU_PLUS_OBJECT_LISTS_INCLUDE
#define NU_PLUS_OBJECT_LISTS_INCLUDE    NU_TRUE
#undef  NU_NMI_SUPPORT_ENABLED
#define NU_NMI_SUPPORT_ENABLED          NU_FALSE

#endif  /* NU_TIME_TEST1MAX_ENABLE == NU_TRUE */

/* Check if timing test 2 (API timing) or timing test 3 (interrupt latency)
   is enabled */
#if (NU_TIME_TEST2_ENABLE == NU_TRUE) || (NU_TIME_TEST3_ENABLE == NU_TRUE)

/* Ensure all configuration settings build for speed */
#undef  NU_ERROR_CHECKING
#define NU_ERROR_CHECKING               NU_FALSE
#undef  NU_PLUS_INLINING
#define NU_PLUS_INLINING                NU_TRUE
#undef  NU_MIN_RAM_ENABLED
#define NU_MIN_RAM_ENABLED              NU_FALSE

#endif  /* NU_TIME_TEST2_ENABLE == NU_TRUE */

/* MMU mode requires module support */
#if (NU_MMU_MODE == NU_TRUE)

#undef          NU_MODULE_SUPPORT
#define         NU_MODULE_SUPPORT                   NU_TRUE

#endif

/* Module support requires mode switching */
#if (NU_MODULE_SUPPORT == NU_TRUE)

#undef          NU_SUPERV_USER_MODE
#define         NU_SUPERV_USER_MODE                 NU_TRUE

#endif

/* Define macro to read OS timer count */
#define         NU_HW_TIMER_COUNT_READ()            ESAL_GE_TMR_OS_COUNT_READ()

/* Define constant for the OS timer clock rate */
#define         NU_HW_TIMER_CLOCK_RATE              ESAL_GE_TMR_OS_CLOCK_RATE

/* Define constant for number of hardware ticks per second */
#define         NU_HW_TIMER_TICKS_PER_SEC           (ESAL_GE_TMR_OS_CLOCK_RATE /    \
                                                     ESAL_GE_TMR_OS_CLOCK_PRESCALE)

/* Define the number of hardware timer ticks that occur for each Nucleus PLUS
   software tick */
#define         NU_HW_TIMER_TICKS_PER_SW_TICK       (NU_HW_TIMER_TICKS_PER_SEC /    \
                                                     NU_PLUS_TICKS_PER_SEC)

/* Define constants for use in service parameters.  */
#define         NU_AND                              2
#define         NU_AND_CONSUME                      3
#define         NU_DISABLE_TIMER                    4
#define         NU_ENABLE_TIMER                     5
#define         NU_FIFO                             6
#define         NU_FIXED_SIZE                       7
#define         NU_NO_PREEMPT                       8
#define         NU_NO_START                         9
#define         NU_NO_SUSPEND                       0
#define         NU_NULL                             0
#define         NU_OR                               0
#define         NU_OR_CONSUME                       1
#define         NU_PREEMPT                          10
#define         NU_PRIORITY                         11
#define         NU_START                            12
#define         NU_SUSPEND                          0xFFFFFFFFUL
#define         NU_VARIABLE_SIZE                    13

/* Define task suspension constants.  */
#define         NU_DRIVER_SUSPEND                   10
#define         NU_EVENT_SUSPEND                    7
#define         NU_FINISHED                         11
#define         NU_MAILBOX_SUSPEND                  3
#define         NU_MEMORY_SUSPEND                   9
#define         NU_PARTITION_SUSPEND                8
#define         NU_PIPE_SUSPEND                     5
#define         NU_PURE_SUSPEND                     1
#define         NU_QUEUE_SUSPEND                    4
#define         NU_READY                            0
#define         NU_SEMAPHORE_SUSPEND                6
#define         NU_SLEEP_SUSPEND                    2
#define         NU_TERMINATED                       12

/* Define service completion status constants.  */
#define         NU_SUCCESS                          0
#define         NU_END_OF_LOG                       -1
#define         NU_GROUP_DELETED                    -2
#define         NU_INVALID_DELETE                   -3
#define         NU_INVALID_DRIVER                   -4
#define         NU_INVALID_ENABLE                   -5
#define         NU_INVALID_ENTRY                    -6
#define         NU_INVALID_FUNCTION                 -7
#define         NU_INVALID_GROUP                    -8
#define         NU_INVALID_HISR                     -9
#define         NU_INVALID_MAILBOX                  -10
#define         NU_INVALID_MEMORY                   -11
#define         NU_INVALID_MESSAGE                  -12
#define         NU_INVALID_OPERATION                -13
#define         NU_INVALID_PIPE                     -14
#define         NU_INVALID_POINTER                  -15
#define         NU_INVALID_POOL                     -16
#define         NU_INVALID_PREEMPT                  -17
#define         NU_INVALID_PRIORITY                 -18
#define         NU_INVALID_QUEUE                    -19
#define         NU_INVALID_RESUME                   -20
#define         NU_INVALID_SEMAPHORE                -21
#define         NU_INVALID_SIZE                     -22
#define         NU_INVALID_START                    -23
#define         NU_INVALID_SUSPEND                  -24
#define         NU_INVALID_TASK                     -25
#define         NU_INVALID_TIMER                    -26
#define         NU_INVALID_VECTOR                   -27
#define         NU_MAILBOX_DELETED                  -28
#define         NU_MAILBOX_EMPTY                    -29
#define         NU_MAILBOX_FULL                     -30
#define         NU_MAILBOX_RESET                    -31
#define         NU_NO_MEMORY                        -32
#define         NU_NO_MORE_LISRS                    -33
#define         NU_NO_PARTITION                     -34
#define         NU_NOT_DISABLED                     -35
#define         NU_NOT_PRESENT                      -36
#define         NU_NOT_REGISTERED                   -37
#define         NU_NOT_TERMINATED                   -38
#define         NU_PIPE_DELETED                     -39
#define         NU_PIPE_EMPTY                       -40
#define         NU_PIPE_FULL                        -41
#define         NU_PIPE_RESET                       -42
#define         NU_POOL_DELETED                     -43
#define         NU_QUEUE_DELETED                    -44
#define         NU_QUEUE_EMPTY                      -45
#define         NU_QUEUE_FULL                       -46
#define         NU_QUEUE_RESET                      -47
#define         NU_SEMAPHORE_DELETED                -48
#define         NU_SEMAPHORE_RESET                  -49
#define         NU_TIMEOUT                          -50
#define         NU_UNAVAILABLE                      -51
#define         NU_INVALID_DESCRIPTION              -52
#define         NU_INVALID_REGION                   -53
#define         NU_MEMORY_CORRUPT                   -54
#define         NU_INVALID_DEBUG_ALLOCATION         -55
#define         NU_EMPTY_DEBUG_ALLOCATION_LIST      -56
#define         NU_HISR_ACTIVATION_COUNT_ERROR      -57
#define         NU_ZC_INVALID_PTR                   -58
#define         NU_ZC_INVALID_BUF_ID                -59
#define         NU_ZC_BUF_SEG_MISMATCH              -60
#define         NU_ZC_INVALID_SEG_OFFSET            -61
#define         NU_ZC_BUF_EMPTY                     -62
#define         NU_ZC_INSUFFICIENT_MEMORY           -63
#define         NU_ZC_INIT_ALREADY_COMPLETE         -64
#define         NU_SEMAPHORE_COUNT_ROLLOVER         -65

/* Define system errors.  */
#define         NU_ERROR_CREATING_TIMER_HISR        1
#define         NU_ERROR_CREATING_TIMER_TASK        2
#define         NU_STACK_OVERFLOW                   3
#define         NU_UNHANDLED_INTERRUPT              4
#define         NU_NOT_IN_SUPERVISOR_MODE           5
#define         NU_NOT_ENOUGH_DTLBS                 6
#define         NU_NOT_ENOUGH_ITLBS                 7
#define         NU_STACK_UNDERFLOW                  8
#define         NU_UNHANDLED_EXCEPTION              9

/* Define I/O driver constants.  */
#define         NU_IO_ERROR                         -1
#define         NU_INITIALIZE                       1
#define         NU_ASSIGN                           2
#define         NU_RELEASE                          3
#define         NU_INPUT                            4
#define         NU_OUTPUT                           5
#define         NU_STATUS                           6
#define         NU_TERMINATE                        7

/* Define history entry IDs.  */
#define         NU_USER_ID                          1
#define         NU_CREATE_TASK_ID                   2
#define         NU_DELETE_TASK_ID                   3
#define         NU_RESET_TASK_ID                    4
#define         NU_TERMINATE_TASK_ID                5
#define         NU_RESUME_TASK_ID                   6
#define         NU_SUSPEND_TASK_ID                  7
#define         NU_RELINQUISH_ID                    8
#define         NU_SLEEP_ID                         9
#define         NU_CHANGE_PRIORITY_ID               10
#define         NU_CHANGE_PREEMPTION_ID             11
#define         NU_CREATE_MAILBOX_ID                12
#define         NU_DELETE_MAILBOX_ID                13
#define         NU_RESET_MAILBOX_ID                 14
#define         NU_SEND_TO_MAILBOX_ID               15
#define         NU_BROADCAST_TO_MAILBOX_ID          16
#define         NU_RECEIVE_FROM_MAILBOX_ID          17
#define         NU_CREATE_QUEUE_ID                  18
#define         NU_DELETE_QUEUE_ID                  19
#define         NU_RESET_QUEUE_ID                   20
#define         NU_SEND_TO_FRONT_OF_QUEUE_ID        21
#define         NU_SEND_TO_QUEUE_ID                 22
#define         NU_BROADCAST_TO_QUEUE_ID            23
#define         NU_RECEIVE_FROM_QUEUE_ID            24
#define         NU_CREATE_PIPE_ID                   25
#define         NU_DELETE_PIPE_ID                   26
#define         NU_RESET_PIPE_ID                    27
#define         NU_SEND_TO_FRONT_OF_PIPE_ID         28
#define         NU_SEND_TO_PIPE_ID                  29
#define         NU_BROADCAST_TO_PIPE_ID             30
#define         NU_RECEIVE_FROM_PIPE_ID             31
#define         NU_CREATE_SEMAPHORE_ID              32
#define         NU_DELETE_SEMAPHORE_ID              33
#define         NU_RESET_SEMAPHORE_ID               34
#define         NU_OBTAIN_SEMAPHORE_ID              35
#define         NU_RELEASE_SEMAPHORE_ID             36
#define         NU_CREATE_EVENT_GROUP_ID            37
#define         NU_DELETE_EVENT_GROUP_ID            38
#define         NU_SET_EVENTS_ID                    39
#define         NU_RETRIEVE_EVENTS_ID               40
#define         NU_CREATE_PARTITION_POOL_ID         41
#define         NU_DELETE_PARTITION_POOL_ID         42
#define         NU_ALLOCATE_PARTITION_ID            43
#define         NU_DEALLOCATE_PARTITION_ID          44
#define         NU_CREATE_MEMORY_POOL_ID            45
#define         NU_DELETE_MEMORY_POOL_ID            46
#define         NU_ALLOCATE_MEMORY_ID               47
#define         NU_DEALLOCATE_MEMORY_ID             48
#define         NU_CONTROL_SIGNALS_ID               49
#define         NU_RECEIVE_SIGNALS_ID               50
#define         NU_REGISTER_SIGNAL_HANDLER_ID       51
#define         NU_SEND_SIGNALS_ID                  52
#define         NU_REGISTER_LISR_ID                 53
#define         NU_CREATE_HISR_ID                   54
#define         NU_DELETE_HISR_ID                   55
#define         NU_CREATE_TIMER_ID                  56
#define         NU_DELETE_TIMER_ID                  57
#define         NU_CONTROL_TIMER_ID                 58
#define         NU_RESET_TIMER_ID                   59
#define         NU_CREATE_DRIVER_ID                 60
#define         NU_DELETE_DRIVER_ID                 61
#define         NU_REQUEST_DRIVER_ID                62
#define         NU_RESUME_DRIVER_ID                 63
#define         NU_SUSPEND_DRIVER_ID                64
#define         NU_CHANGE_TIME_SLICE_ID             65
#define         NU_ASSERT_ID                        66
#define         NU_ALLOCATE_ALIGNED_ID              67
#define         NU_NMI_ID                           68
#define         NU_ZC_ID                            69


/* Include error management interface to allow access to
   NU_CHECK and NU_ASSERT macros as well as ERC_System_Error */
#include        "plus/inc/er_extr.h"

#if (NU_LV_ENABLE == NU_FALSE) || (defined(NU_SOURCE_FILE))

/* Define the basic data structure templates.  */
#include        "plus/inc/cs_defs.h"
#include        "plus/inc/tm_defs.h"
#include        "plus/inc/tc_defs.h"
#include        "plus/inc/mb_defs.h"
#include        "plus/inc/qu_defs.h"
#include        "plus/inc/pi_defs.h"
#include        "plus/inc/sm_defs.h"
#include        "plus/inc/ev_defs.h"
#include        "plus/inc/pm_defs.h"
#include        "plus/inc/dm_defs.h"

/* Define application data types to actual internal data structures */
typedef         TC_TCB                              NU_TASK;
typedef         TC_HCB                              NU_HISR;
typedef         MB_MCB                              NU_MAILBOX;
typedef         QU_QCB                              NU_QUEUE;
typedef         PI_PCB                              NU_PIPE;
typedef         SM_SCB                              NU_SEMAPHORE;
typedef         EV_GCB                              NU_EVENT_GROUP;
typedef         PM_PCB                              NU_PARTITION_POOL;
typedef         DM_PCB                              NU_MEMORY_POOL;
typedef         TM_APP_TCB                          NU_TIMER;
typedef         TC_PROTECT                          NU_PROTECT;

#else

/* Define maximum size (in unsigned longs) of a Nucleus PLUS data structure */
#define         NU_MAX_STRUCT_SIZE                  50

/* Define one application data structure for LV */
typedef struct
{
    UNSIGNED    words[NU_MAX_STRUCT_SIZE];

} NU_DATA_STRUCT;

/* Define application data types to LV data structures */
typedef         NU_DATA_STRUCT                      NU_TASK;
typedef         NU_DATA_STRUCT                      NU_HISR;
typedef         NU_DATA_STRUCT                      NU_MAILBOX;
typedef         NU_DATA_STRUCT                      NU_QUEUE;
typedef         NU_DATA_STRUCT                      NU_PIPE;
typedef         NU_DATA_STRUCT                      NU_SEMAPHORE;
typedef         NU_DATA_STRUCT                      NU_EVENT_GROUP;
typedef         NU_DATA_STRUCT                      NU_PARTITION_POOL;
typedef         NU_DATA_STRUCT                      NU_MEMORY_POOL;
typedef         NU_DATA_STRUCT                      NU_TIMER;
typedef         NU_DATA_STRUCT                      NU_PROTECT;

#endif  /* NU_LV_ENABLE == NU_FALSE || defined(NU_SOURCE_FILE) */

/* Define the number of UNSIGNED words in the NU_DRIVER structure.  */
#define         NU_DRIVER_SIZE                      5

/* Define I/O driver request structures.  */
struct NU_INITIALIZE_STRUCT
{
    VOID                        *nu_io_address;             /* Base IO address          */
    UNSIGNED                    nu_logical_units;           /* Number of logical units  */
    VOID                        *nu_memory;                 /* Generic memory pointer   */
    INT                         nu_vector;                  /* Interrupt vector number  */
};

struct NU_ASSIGN_STRUCT
{
    UNSIGNED                    nu_logical_unit;            /* Logical unit number      */
    INT                         nu_assign_info;             /* Additional assign info   */
};

struct NU_RELEASE_STRUCT
{
    UNSIGNED                    nu_logical_unit;            /* Logical unit number      */
    INT                         nu_release_info;            /* Additional release info  */
};

struct NU_INPUT_STRUCT
{
    UNSIGNED                    nu_logical_unit;            /* Logical unit number      */
    UNSIGNED                    nu_offset;                  /* Offset of input          */
    UNSIGNED                    nu_request_size;            /* Requested input size     */
    UNSIGNED                    nu_actual_size;             /* Actual input size        */
    VOID                        *nu_buffer_ptr;             /* Input buffer pointer     */
};

struct NU_OUTPUT_STRUCT
{
    UNSIGNED                    nu_logical_unit;            /* Logical unit number      */
    UNSIGNED                    nu_offset;                  /* Offset of output         */
    UNSIGNED                    nu_request_size;            /* Requested output size    */
    UNSIGNED                    nu_actual_size;             /* Actual output size       */
    VOID                        *nu_buffer_ptr;             /* Output buffer pointer    */
};

struct NU_STATUS_STRUCT
{
    UNSIGNED                    nu_logical_unit;            /* Logical unit number      */
    VOID                        *nu_extra_status;           /* Additional status ptr    */
};

struct NU_TERMINATE_STRUCT
{
    UNSIGNED                    nu_logical_unit;            /* Logical unit number      */
};


/* Define a union of all the different types of request structures. */
typedef  union NU_REQUEST_INFO_UNION
{
    struct NU_INITIALIZE_STRUCT nu_initialize;
    struct NU_ASSIGN_STRUCT     nu_assign;
    struct NU_RELEASE_STRUCT    nu_release;
    struct NU_INPUT_STRUCT      nu_input;
    struct NU_OUTPUT_STRUCT     nu_output;
    struct NU_STATUS_STRUCT     nu_status;
    struct NU_TERMINATE_STRUCT  nu_terminate;

} nu_request_info_union;


typedef struct NU_DRIVER_REQUEST_STRUCT
{
    INT                         nu_function;                /* I/O request function     */
    UNSIGNED                    nu_timeout;                 /* Timeout on request       */
    STATUS                      nu_status;                  /* Status of request        */
    UNSIGNED                    nu_supplemental;            /* Supplemental information */
    VOID                        *nu_supplemental_ptr;       /* Supplemental info pointer*/
    nu_request_info_union       nu_request_info;

} NU_DRIVER_REQUEST;

typedef struct NU_DRIVER_STRUCT
{
    UNSIGNED                    words[NU_DRIVER_SIZE];      /* CS_NODE_STRUCT */
    CHAR                        nu_driver_name[NU_MAX_NAME];
    VOID                        *nu_info_ptr;
    UNSIGNED                    nu_driver_id;
    VOID                        (*nu_driver_entry)(struct NU_DRIVER_STRUCT *,
                                                   NU_DRIVER_REQUEST *);
} NU_DRIVER;

/* Define Nucleus PLUS system interface */
VOID            Application_Initialize(NU_MEMORY_POOL*, NU_MEMORY_POOL*);

/* External declarations */
extern          INT                                 TCD_Interrupt_Level;
extern          volatile INT                        TMD_Timer_State;

/*********** START BACKWARDS COMPATIBILITY DEFINITIONS FOR NUCLEUS PLUS 2.0 **********/

/* NOTE:  The following definitions were created as part of
          Nucleus PLUS 2.0 to allow backwards compatibility
          of all Nucleus PLUS internal functions accessed
          by applications and any configuration settings modified
          by Nucleus PLUS 2.0                                            */

/* Hardware timer / clock related backwards compatibility */
/* 2008-12-17 修改支持CodeWarrior编译工具 */
//#define         NU_Retrieve_Hardware_Clock(val)     val = NU_HW_TIMER_COUNT_READ()
#define         NU_Retrieve_Hardware_Clock(val)     ESAL_TS_RTE_DEC_READ_VAL(val)
#define         NU_System_Clock_Frequency           NU_HW_TIMER_CLOCK_RATE
#define         NU_PLUS_Ticks_Per_Second            NU_PLUS_TICKS_PER_SEC
#define         NU_HW_Ticks_Per_Second              NU_HW_TIMER_TICKS_PER_SEC
#define         NU_HW_Ticks_Per_SW_Tick             NU_HW_TIMER_TICKS_PER_SW_TICK

#if (ESAL_GE_TMR_OS_COUNT_DIR == ESAL_COUNT_DOWN)

#define         NU_COUNT_DOWN

#else

#undef          NU_COUNT_DOWN

#endif  /* ESAL_GE_TMR_OS_COUNT_DIR == ESAL_COUNT_DOWN */

/* Nucleus PLUS compile-time switch backwards compatibility */
/* Old error checking define */
#ifdef          NU_NO_ERROR_CHECKING

#undef          NU_ERROR_CHECKING
#define         NU_ERROR_CHECKING                   NU_FALSE

#endif  /* NU_NO_ERROR_CHECKING */

/* Old inline define */
#ifdef          NU_INLINE

#undef          NU_PLUS_INLINING
#define         NU_PLUS_INLINING                    NU_TRUE

#endif  /* NU_INLINE */

/* Old stack checking define */
#ifdef          NU_ENABLE_STACK_CHECK

#undef          NU_STACK_CHECKING
#define         NU_STACK_CHECKING                   NU_TRUE

#endif  /* NU_ENABLE_STACK_CHECK */

/* Old history logging define */
#ifdef          NU_ENABLE_HISTORY

#undef          NU_HISTORY_LOGGING
#define         NU_HISTORY_LOGGING                  NU_TRUE

#endif  /* NU_ENABLE_HISTORY */

/* Old debug define */
#ifdef          NU_DEBUG

#undef          NU_DEBUG_ENABLE
#define         NU_DEBUG_ENABLE                     NU_TRUE

#endif  /* NU_DEBUG */

/* Old maximum vector number and number of LISRs */
#ifndef         NU_MAX_VECTORS
#define         NU_MAX_VECTORS                      ESAL_GE_INT_MAX_VECTOR_ID_GET()
#endif  /* NU_MAX_VECTORS */

#ifndef         NU_MAX_LISRS
#define         NU_MAX_LISRS                        (ESAL_GE_INT_MAX_VECTOR_ID_GET() + 1)
#endif  /* NU_MAX_LISRS */

/* Old POSIX include define */
#define         INCLUDE_NU_POSIX                    NU_POSIX_INCLUDED

/* Re-map old internal timer and thread control function names to new internal names */
#define         TCC_Create_Task                     TCCT_Create_Task
#define         TCC_Create_HISR                     TCCT_Create_HISR
#define         TCC_Reset_Task                      TCCT_Reset_Task
#define         TCC_Register_LISR                   TCCT_Register_LISR
#define         TCS_Send_Signals                    TCST_Send_Signals
#define         TCT_Control_Interrupts              TCCT_Control_Interrupts
#define         TCT_Local_Control_Interrupts        ESAL_GE_INT_Global_Set
#define         TCT_Restore_Interrupts()            ESAL_GE_INT_Global_Set(TCD_Interrupt_Level)
#define         TCT_Check_Stack                     TCCT_Check_Stack
#define         TCT_Schedule                        TCCT_Schedule
#define         TCT_Control_To_System               TCCT_Control_To_System
#define         TCT_Signal_Exit                     TCCT_Signal_Exit
#define         TCT_Current_Thread                  TCCT_Current_Thread
#define         TCT_Set_Execute_Task                TCCT_Set_Execute_Task
#define         TCT_Unprotect_Specific              TCCT_Unprotect_Specific
#define         TCT_Set_Current_Protect             TCCT_Set_Current_Protect
#define         TCT_Protect_Switch                  TCCT_Protect_Switch
#define         TCT_Interrupt_Context_Restore       TCCT_Interrupt_Context_Restore
#define         TCT_Activate_HISR                   TCCT_Activate_HISR
#define         TCT_HISR_Shell                      TCCT_HISR_Shell
#define         TCT_Protect                         TCCT_Protect
#define         TCT_Unprotect                       TCCT_Unprotect
#define         TCT_Get_Current_Protect             TCCT_Get_Current_Protect
#define         TCT_System_Protect                  TCCT_System_Protect
#define         TCT_System_Unprotect                TCCT_System_Unprotect
#define         TMT_Set_Clock                       TMCT_Set_Clock
#define         TMT_Retrieve_Clock                  TMCT_Retrieve_Clock
#define         TMT_Read_Timer                      TMCT_Read_Timer
#define         TMT_Enable_Timer                    TMCT_Enable_Timer
#define         TMT_Adjust_Timer                    TMCT_Adjust_Timer
#define         TMT_Disable_Timer()                 TMD_Timer_State = TM_NOT_ACTIVE
#define         TMT_Retrieve_TS_Task                TMCT_Retrieve_TS_Task
#define         TCD_System_Stack                    ESAL_GE_STK_System_SP

/*********** END BACKWARDS COMPATIBILITY DEFINITIONS FOR NUCLEUS PLUS 2.0 **********/

/* The following area is only applicable to application files and is skipped
   during compilation of Nucleus PLUS source files.  */
#ifndef         NU_SOURCE_FILE

/* Re-map task control functions depending on whether or not error checking
   is specified.  */
#if             (NU_ERROR_CHECKING == NU_FALSE)

/* Map directly to control function (skip error checking) */
#define         NU_Create_Task                      TCCT_Create_Task
#define         NU_Delete_Task                      TCC_Delete_Task
#define         NU_Reset_Task                       TCCT_Reset_Task
#define         NU_Terminate_Task                   TCC_Terminate_Task
#define         NU_Resume_Task                      TCC_Resume_Service
#define         NU_Suspend_Task                     TCC_Suspend_Service
#define         NU_Relinquish                       TCC_Relinquish
#define         NU_Sleep                            TCC_Task_Sleep
#define         NU_Change_Priority                  TCS_Change_Priority
#define         NU_Change_Preemption                TCS_Change_Preemption
#define         NU_Change_Time_Slice                TCS_Change_Time_Slice
#define         NU_Check_Stack                      TCCT_Check_Stack
#define         NU_Current_Task_Pointer             TCC_Current_Task_Pointer
#define         NU_Established_Tasks                TCF_Established_Tasks
#define         NU_Task_Information                 TCF_Task_Information
#define         NU_Task_Pointers                    TCF_Task_Pointers
#define         NU_Create_Mailbox                   MBC_Create_Mailbox
#define         NU_Delete_Mailbox                   MBC_Delete_Mailbox
#define         NU_Reset_Mailbox                    MBS_Reset_Mailbox
#define         NU_Send_To_Mailbox                  MBC_Send_To_Mailbox
#define         NU_Broadcast_To_Mailbox             MBS_Broadcast_To_Mailbox
#define         NU_Receive_From_Mailbox             MBC_Receive_From_Mailbox
#define         NU_Established_Mailboxes            MBF_Established_Mailboxes
#define         NU_Mailbox_Information              MBF_Mailbox_Information
#define         NU_Mailbox_Pointers                 MBF_Mailbox_Pointers
#define         NU_Create_Queue                     QUC_Create_Queue
#define         NU_Delete_Queue                     QUC_Delete_Queue
#define         NU_Reset_Queue                      QUS_Reset_Queue
#define         NU_Send_To_Front_Of_Queue           QUS_Send_To_Front_Of_Queue
#define         NU_Send_To_Queue                    QUC_Send_To_Queue
#define         NU_Broadcast_To_Queue               QUS_Broadcast_To_Queue
#define         NU_Receive_From_Queue               QUC_Receive_From_Queue
#define         NU_Established_Queues               QUF_Established_Queues
#define         NU_Queue_Information                QUF_Queue_Information
#define         NU_Queue_Pointers                   QUF_Queue_Pointers
#define         NU_Create_Pipe                      PIC_Create_Pipe
#define         NU_Delete_Pipe                      PIC_Delete_Pipe
#define         NU_Reset_Pipe                       PIS_Reset_Pipe
#define         NU_Send_To_Front_Of_Pipe            PIS_Send_To_Front_Of_Pipe
#define         NU_Send_To_Pipe                     PIC_Send_To_Pipe
#define         NU_Broadcast_To_Pipe                PIS_Broadcast_To_Pipe
#define         NU_Receive_From_Pipe                PIC_Receive_From_Pipe
#define         NU_Established_Pipes                PIF_Established_Pipes
#define         NU_Pipe_Information                 PIF_Pipe_Information
#define         NU_Pipe_Pointers                    PIF_Pipe_Pointers
#define         NU_Create_Semaphore                 SMC_Create_Semaphore
#define         NU_Delete_Semaphore                 SMC_Delete_Semaphore
#define         NU_Reset_Semaphore                  SMS_Reset_Semaphore
#define         NU_Obtain_Semaphore                 SMC_Obtain_Semaphore
#define         NU_Release_Semaphore                SMC_Release_Semaphore
#define         NU_Established_Semaphores           SMF_Established_Semaphores
#define         NU_Semaphore_Information            SMF_Semaphore_Information
#define         NU_Semaphore_Pointers               SMF_Semaphore_Pointers
#define         NU_Create_Event_Group               EVC_Create_Event_Group
#define         NU_Delete_Event_Group               EVC_Delete_Event_Group
#define         NU_Set_Events                       EVC_Set_Events
#define         NU_Retrieve_Events                  EVC_Retrieve_Events
#define         NU_Established_Event_Groups         EVF_Established_Event_Groups
#define         NU_Event_Group_Information          EVF_Event_Group_Information
#define         NU_Event_Group_Pointers             EVF_Event_Group_Pointers
#define         NU_Create_Partition_Pool            PMC_Create_Partition_Pool
#define         NU_Delete_Partition_Pool            PMC_Delete_Partition_Pool
#define         NU_Allocate_Partition               PMC_Allocate_Partition
#define         NU_Deallocate_Partition             PMC_Deallocate_Partition
#define         NU_Established_Partition_Pools      PMF_Established_Partition_Pools
#define         NU_Partition_Pool_Information       PMF_Partition_Pool_Information
#define         NU_Partition_Pool_Pointers          PMF_Partition_Pool_Pointers
#define         NU_Create_Memory_Pool               DMC_Create_Memory_Pool
#define         NU_Delete_Memory_Pool               DMC_Delete_Memory_Pool
#define         NU_Allocate_Memory                  DMC_Allocate_Memory
#define         NU_Deallocate_Memory                DMC_Deallocate_Memory
#define         NU_Allocate_Aligned_Memory          DMS_Allocate_Aligned_Memory
#define         NU_Established_Memory_Pools         DMF_Established_Memory_Pools
#define         NU_Memory_Pool_Information          DMF_Memory_Pool_Information
#define         NU_Memory_Pool_Pointers             DMF_Memory_Pool_Pointers
#define         NU_Control_Signals                  TCS_Control_Signals
#define         NU_Receive_Signals                  TCS_Receive_Signals
#define         NU_Register_Signal_Handler          TCS_Register_Signal_Handler
#define         NU_Send_Signals                     TCST_Send_Signals
#define         NU_Register_LISR                    TCCT_Register_LISR
#define         NU_Activate_HISR                    TCCT_Activate_HISR
#define         NU_Create_HISR                      TCCT_Create_HISR
#define         NU_Delete_HISR                      TCC_Delete_HISR
#define         NU_Current_HISR_Pointer             TCC_Current_HISR_Pointer
#define         NU_Established_HISRs                TCF_Established_HISRs
#define         NU_HISR_Pointers                    TCF_HISR_Pointers
#define         NU_HISR_Information                 TCF_HISR_Information
#define         NU_Protect                          TCCT_Protect
#define         NU_Unprotect                        TCCT_Unprotect
#define         NU_Control_Interrupts               TCCT_Control_Interrupts
#define         NU_Local_Control_Interrupts         ESAL_GE_INT_Global_Set
#define         NU_Restore_Interrupts()             ESAL_GE_INT_Global_Set(TCD_Interrupt_Level)
#define         NU_Set_Clock                        TMCT_Set_Clock
#define         NU_Retrieve_Clock                   TMCT_Retrieve_Clock
#define         NU_Create_Timer                     TMS_Create_Timer
#define         NU_Delete_Timer                     TMS_Delete_Timer
#define         NU_Control_Timer                    TMS_Control_Timer
#define         NU_Reset_Timer                      TMS_Reset_Timer
#define         NU_Established_Timers               TMF_Established_Timers
#define         NU_Timer_Pointers                   TMF_Timer_Pointers
#define         NU_Timer_Information                TMF_Timer_Information
#define         NU_Get_Remaining_Time               TMF_Get_Remaining_Time
#define         NU_Release_Information()            PLUS_RELEASE_STRING
#define         NU_Get_Release_Version              RLC_Get_Version
#define         NU_Disable_History_Saving           HIC_Disable_History_Saving
#define         NU_Enable_History_Saving            HIC_Enable_History_Saving
#define         NU_Make_History_Entry               HIC_Make_History_Entry_Service
#define         NU_Retrieve_History_Entry           HIC_Retrieve_History_Entry
#define         NU_Create_Driver                    IOC_Create_Driver
#define         NU_Delete_Driver                    IOC_Delete_Driver
#define         NU_Request_Driver                   IOC_Request_Driver
#define         NU_Resume_Driver                    IOC_Resume_Driver
#define         NU_Suspend_Driver                   IOC_Suspend_Driver
#define         NU_Established_Drivers              IOF_Established_Drivers
#define         NU_Driver_Pointers                  IOF_Driver_Pointers
#define         NU_NMI_Init_Status_Get              NMI_Init_Status_Get
#define         NU_NMI_Init_Status_Set              NMI_Init_Status_Set
#define         NU_NMI_Wait_For_Init                NMI_Wait_For_Init
#define         NU_NMI_Cleanup_Invoke               NMI_Cleanup_Invoke

#else

/* Map to error checking function */
#define         NU_Create_Task                      TCCE_Create_Task
#define         NU_Delete_Task                      TCCE_Delete_Task
#define         NU_Reset_Task                       TCCE_Reset_Task
#define         NU_Terminate_Task                   TCCE_Terminate_Task
#define         NU_Resume_Task                      TCCE_Resume_Service
#define         NU_Suspend_Task                     TCCE_Suspend_Service
#define         NU_Relinquish                       TCCE_Relinquish
#define         NU_Sleep                            TCCE_Task_Sleep
#define         NU_Change_Priority                  TCSE_Change_Priority
#define         NU_Change_Preemption                TCSE_Change_Preemption
#define         NU_Change_Time_Slice                TCSE_Change_Time_Slice
#define         NU_Check_Stack                      TCCT_Check_Stack
#define         NU_Current_Task_Pointer             TCC_Current_Task_Pointer
#define         NU_Established_Tasks                TCF_Established_Tasks
#define         NU_Task_Information                 TCFE_Task_Information
#define         NU_Task_Pointers                    TCF_Task_Pointers
#define         NU_Create_Mailbox                   MBCE_Create_Mailbox
#define         NU_Delete_Mailbox                   MBCE_Delete_Mailbox
#define         NU_Reset_Mailbox                    MBSE_Reset_Mailbox
#define         NU_Send_To_Mailbox                  MBCE_Send_To_Mailbox
#define         NU_Broadcast_To_Mailbox             MBSE_Broadcast_To_Mailbox
#define         NU_Receive_From_Mailbox             MBCE_Receive_From_Mailbox
#define         NU_Established_Mailboxes            MBF_Established_Mailboxes
#define         NU_Mailbox_Information              MBF_Mailbox_Information
#define         NU_Mailbox_Pointers                 MBF_Mailbox_Pointers
#define         NU_Create_Queue                     QUCE_Create_Queue
#define         NU_Delete_Queue                     QUCE_Delete_Queue
#define         NU_Reset_Queue                      QUSE_Reset_Queue
#define         NU_Send_To_Queue                    QUCE_Send_To_Queue
#define         NU_Send_To_Front_Of_Queue           QUSE_Send_To_Front_Of_Queue
#define         NU_Broadcast_To_Queue               QUSE_Broadcast_To_Queue
#define         NU_Receive_From_Queue               QUCE_Receive_From_Queue
#define         NU_Established_Queues               QUF_Established_Queues
#define         NU_Queue_Information                QUF_Queue_Information
#define         NU_Queue_Pointers                   QUF_Queue_Pointers
#define         NU_Create_Pipe                      PICE_Create_Pipe
#define         NU_Delete_Pipe                      PICE_Delete_Pipe
#define         NU_Reset_Pipe                       PISE_Reset_Pipe
#define         NU_Send_To_Front_Of_Pipe            PISE_Send_To_Front_Of_Pipe
#define         NU_Send_To_Pipe                     PICE_Send_To_Pipe
#define         NU_Broadcast_To_Pipe                PISE_Broadcast_To_Pipe
#define         NU_Receive_From_Pipe                PICE_Receive_From_Pipe
#define         NU_Established_Pipes                PIF_Established_Pipes
#define         NU_Pipe_Information                 PIF_Pipe_Information
#define         NU_Pipe_Pointers                    PIF_Pipe_Pointers
#define         NU_Create_Semaphore                 SMCE_Create_Semaphore
#define         NU_Delete_Semaphore                 SMCE_Delete_Semaphore
#define         NU_Reset_Semaphore                  SMSE_Reset_Semaphore
#define         NU_Obtain_Semaphore                 SMCE_Obtain_Semaphore
#define         NU_Release_Semaphore                SMCE_Release_Semaphore
#define         NU_Established_Semaphores           SMF_Established_Semaphores
#define         NU_Semaphore_Information            SMF_Semaphore_Information
#define         NU_Semaphore_Pointers               SMF_Semaphore_Pointers
#define         NU_Create_Event_Group               EVCE_Create_Event_Group
#define         NU_Delete_Event_Group               EVCE_Delete_Event_Group
#define         NU_Set_Events                       EVCE_Set_Events
#define         NU_Retrieve_Events                  EVCE_Retrieve_Events
#define         NU_Established_Event_Groups         EVF_Established_Event_Groups
#define         NU_Event_Group_Information          EVF_Event_Group_Information
#define         NU_Event_Group_Pointers             EVF_Event_Group_Pointers
#define         NU_Create_Partition_Pool            PMCE_Create_Partition_Pool
#define         NU_Delete_Partition_Pool            PMCE_Delete_Partition_Pool
#define         NU_Allocate_Partition               PMCE_Allocate_Partition
#define         NU_Deallocate_Partition             PMCE_Deallocate_Partition
#define         NU_Established_Partition_Pools      PMF_Established_Partition_Pools
#define         NU_Partition_Pool_Information       PMF_Partition_Pool_Information
#define         NU_Partition_Pool_Pointers          PMF_Partition_Pool_Pointers
#define         NU_Create_Memory_Pool               DMCE_Create_Memory_Pool
#define         NU_Delete_Memory_Pool               DMCE_Delete_Memory_Pool
#define         NU_Allocate_Memory                  DMCE_Allocate_Memory
#define         NU_Allocate_Aligned_Memory          DMS_Allocate_Aligned_Memory
#define         NU_Deallocate_Memory                DMCE_Deallocate_Memory
#define         NU_Established_Memory_Pools         DMF_Established_Memory_Pools
#define         NU_Memory_Pool_Information          DMF_Memory_Pool_Information
#define         NU_Memory_Pool_Pointers             DMF_Memory_Pool_Pointers
#define         NU_Control_Signals                  TCSE_Control_Signals
#define         NU_Receive_Signals                  TCSE_Receive_Signals
#define         NU_Register_Signal_Handler          TCSE_Register_Signal_Handler
#define         NU_Send_Signals                     TCSE_Send_Signals
#define         NU_Register_LISR                    TCCE_Register_LISR
#define         NU_Activate_HISR                    TCCE_Activate_HISR
#define         NU_Create_HISR                      TCCE_Create_HISR
#define         NU_Delete_HISR                      TCCE_Delete_HISR
#define         NU_Current_HISR_Pointer             TCC_Current_HISR_Pointer
#define         NU_Established_HISRs                TCF_Established_HISRs
#define         NU_HISR_Pointers                    TCF_HISR_Pointers
#define         NU_HISR_Information                 TCF_HISR_Information
#define         NU_Protect                          TCCT_Protect
#define         NU_Unprotect                        TCCT_Unprotect
#define         NU_Control_Interrupts               TCCT_Control_Interrupts
#define         NU_Local_Control_Interrupts         ESAL_GE_INT_Global_Set
#define         NU_Restore_Interrupts()             ESAL_GE_INT_Global_Set(TCD_Interrupt_Level)
#define         NU_Set_Clock                        TMCT_Set_Clock
#define         NU_Retrieve_Clock                   TMCT_Retrieve_Clock
#define         NU_Create_Timer                     TMSE_Create_Timer
#define         NU_Delete_Timer                     TMSE_Delete_Timer
#define         NU_Control_Timer                    TMSE_Control_Timer
#define         NU_Reset_Timer                      TMSE_Reset_Timer
#define         NU_Established_Timers               TMF_Established_Timers
#define         NU_Timer_Pointers                   TMF_Timer_Pointers
#define         NU_Timer_Information                TMF_Timer_Information
#define         NU_Get_Remaining_Time               TMF_Get_Remaining_Time
#define         NU_Release_Information()            PLUS_RELEASE_STRING
#define         NU_Get_Release_Version              RLC_Get_Version
#define         NU_Disable_History_Saving           HIC_Disable_History_Saving
#define         NU_Enable_History_Saving            HIC_Enable_History_Saving
#define         NU_Make_History_Entry               HIC_Make_History_Entry_Service
#define         NU_Retrieve_History_Entry           HIC_Retrieve_History_Entry
#define         NU_Create_Driver                    IOCE_Create_Driver
#define         NU_Delete_Driver                    IOCE_Delete_Driver
#define         NU_Request_Driver                   IOCE_Request_Driver
#define         NU_Resume_Driver                    IOCE_Resume_Driver
#define         NU_Suspend_Driver                   IOCE_Suspend_Driver
#define         NU_Established_Drivers              IOF_Established_Drivers
#define         NU_Driver_Pointers                  IOF_Driver_Pointers
#define         NU_NMI_Init_Status_Get              NMIE_Init_Status_Get
#define         NU_NMI_Init_Status_Set              NMIE_Init_Status_Set
#define         NU_NMI_Wait_For_Init                NMIE_Wait_For_Init
#define         NU_NMI_Cleanup_Invoke               NMIE_Cleanup_Invoke

#endif  /* NU_ERROR_CHECKING == NU_FALSE */

/* Define task control functions.  */
STATUS          NU_Create_Task(NU_TASK *task, CHAR *name,
                               VOID (*task_entry)(UNSIGNED, VOID *), UNSIGNED argc,
                               VOID *argv, VOID *stack_address, UNSIGNED stack_size,
                               OPTION priority, UNSIGNED time_slice,
                               OPTION preempt, OPTION auto_start);
STATUS          NU_Delete_Task(NU_TASK *task);
STATUS          NU_Reset_Task(NU_TASK *task, UNSIGNED argc, VOID *argv);
STATUS          NU_Terminate_Task(NU_TASK *task);
STATUS          NU_Resume_Task(NU_TASK *task);
STATUS          NU_Suspend_Task(NU_TASK *task);
VOID            NU_Relinquish(VOID);
VOID            NU_Sleep(UNSIGNED ticks);
OPTION          NU_Change_Priority(NU_TASK *task, OPTION new_priority);
OPTION          NU_Change_Preemption(OPTION preempt);
UNSIGNED        NU_Change_Time_Slice(NU_TASK *task, UNSIGNED time_slice);
UNSIGNED        NU_Check_Stack(VOID);
NU_TASK         *NU_Current_Task_Pointer(VOID);
UNSIGNED        NU_Established_Tasks(VOID);
STATUS          NU_Task_Information(NU_TASK *task, CHAR *name,
                                    DATA_ELEMENT *status, UNSIGNED *scheduled_count,
                                    OPTION *priority, OPTION *preempt,
                                    UNSIGNED *time_slice, VOID **stack_base,
                                    UNSIGNED *stack_size, UNSIGNED *minimum_stack);
UNSIGNED        NU_Task_Pointers(NU_TASK **pointer_list,
                                 UNSIGNED maximum_pointers);

/* Define Mailbox management functions.  */
STATUS          NU_Create_Mailbox(NU_MAILBOX *mailbox, CHAR *name,
                                  OPTION suspend_type);
STATUS          NU_Delete_Mailbox(NU_MAILBOX *mailbox);
STATUS          NU_Reset_Mailbox(NU_MAILBOX *mailbox);
STATUS          NU_Send_To_Mailbox(NU_MAILBOX *mailbox, VOID *message,
                                   UNSIGNED suspend);
STATUS          NU_Broadcast_To_Mailbox(NU_MAILBOX *mailbox, VOID *message,
                                        UNSIGNED suspend);
STATUS          NU_Receive_From_Mailbox(NU_MAILBOX *mailbox, VOID *message,
                                        UNSIGNED suspend);
UNSIGNED        NU_Established_Mailboxes(VOID);
STATUS          NU_Mailbox_Information(NU_MAILBOX *mailbox, CHAR *name,
                  OPTION *suspend_type, OPTION *message_present,
                  UNSIGNED *tasks_waiting, NU_TASK **first_task);
UNSIGNED        NU_Mailbox_Pointers(NU_MAILBOX **pointer_list,
                                    UNSIGNED maximum_pointers);

/* Define Queue management functions.  */
STATUS          NU_Create_Queue(NU_QUEUE *queue, CHAR *name,
                                VOID *start_address, UNSIGNED queue_size,
                                OPTION message_type, UNSIGNED message_size,
                                OPTION suspend_type);
STATUS          NU_Delete_Queue(NU_QUEUE *queue);
STATUS          NU_Reset_Queue(NU_QUEUE *queue);
STATUS          NU_Send_To_Front_Of_Queue(NU_QUEUE *queue, VOID *message,
                                          UNSIGNED size, UNSIGNED suspend);
STATUS          NU_Send_To_Queue(NU_QUEUE *queue, VOID *message,
                                 UNSIGNED size, UNSIGNED suspend);
STATUS          NU_Broadcast_To_Queue(NU_QUEUE *queue, VOID *message,
                                      UNSIGNED size, UNSIGNED suspend);
STATUS          NU_Receive_From_Queue(NU_QUEUE *queue, VOID *message,
                                      UNSIGNED size, UNSIGNED *actual_size,
                                      UNSIGNED suspend);
UNSIGNED        NU_Established_Queues(VOID);
STATUS          NU_Queue_Information(NU_QUEUE *queue, CHAR *name,
                                     VOID **start_address, UNSIGNED *queue_size,
                                     UNSIGNED *available, UNSIGNED *messages,
                                     OPTION *message_type, UNSIGNED *message_size,
                                     OPTION *suspend_type, UNSIGNED *tasks_waiting,
                                     NU_TASK **first_task);
UNSIGNED        NU_Queue_Pointers(NU_QUEUE **pointer_list,
                                  UNSIGNED maximum_pointers);

/* Define Pipe management functions.  */
STATUS          NU_Create_Pipe(NU_PIPE *pipe, CHAR *name,
                               VOID *start_address, UNSIGNED pipe_size,
                               OPTION message_type, UNSIGNED message_size,
                               OPTION suspend_type);
STATUS          NU_Delete_Pipe(NU_PIPE *pipe);
STATUS          NU_Reset_Pipe(NU_PIPE *pipe);
STATUS          NU_Send_To_Front_Of_Pipe(NU_PIPE *pipe, VOID *message,
                                         UNSIGNED size, UNSIGNED suspend);
STATUS          NU_Send_To_Pipe(NU_PIPE *pipe, VOID *message,
                                UNSIGNED size, UNSIGNED suspend);
STATUS          NU_Broadcast_To_Pipe(NU_PIPE *pipe, VOID *message,
                                     UNSIGNED size, UNSIGNED suspend);
STATUS          NU_Receive_From_Pipe(NU_PIPE *pipe, VOID *message,
                                     UNSIGNED size, UNSIGNED *actual_size,
                                     UNSIGNED suspend);
UNSIGNED        NU_Established_Pipes(VOID);
STATUS          NU_Pipe_Information(NU_PIPE *pipe, CHAR *name,
                                    VOID **start_address, UNSIGNED *pipe_size,
                                    UNSIGNED *available, UNSIGNED *messages,
                                    OPTION *message_type, UNSIGNED *message_size,
                                    OPTION *suspend_type, UNSIGNED *tasks_waiting,
                                    NU_TASK **first_task);
UNSIGNED        NU_Pipe_Pointers(NU_PIPE **pointer_list,
                                 UNSIGNED maximum_pointers);

/* Define Semaphore management functions.  */
STATUS          NU_Create_Semaphore(NU_SEMAPHORE *semaphore, CHAR *name,
                                    UNSIGNED initial_count, OPTION suspend_type);
STATUS          NU_Delete_Semaphore(NU_SEMAPHORE *semaphore);
STATUS          NU_Reset_Semaphore(NU_SEMAPHORE *semaphore,
                                   UNSIGNED initial_count);
STATUS          NU_Obtain_Semaphore(NU_SEMAPHORE *semaphore, UNSIGNED suspend);
STATUS          NU_Release_Semaphore(NU_SEMAPHORE *semaphore);
UNSIGNED        NU_Established_Semaphores(VOID);
STATUS          NU_Semaphore_Information(NU_SEMAPHORE *semaphore, CHAR *name,
                                         UNSIGNED *current_count, OPTION *suspend_type,
                                         UNSIGNED *tasks_waiting, NU_TASK **first_task);
UNSIGNED        NU_Semaphore_Pointers(NU_SEMAPHORE **pointer_list,
                                      UNSIGNED maximum_pointers);

/* Define Event Group management functions.  */
STATUS          NU_Create_Event_Group(NU_EVENT_GROUP *group, CHAR *name);
STATUS          NU_Delete_Event_Group(NU_EVENT_GROUP *group);
STATUS          NU_Set_Events(NU_EVENT_GROUP *group, UNSIGNED events,
                              OPTION operation);
STATUS          NU_Retrieve_Events(NU_EVENT_GROUP *group,
                                   UNSIGNED requested_flags, OPTION operation,
                                   UNSIGNED *retrieved_flags, UNSIGNED suspend);
UNSIGNED        NU_Established_Event_Groups(VOID);
STATUS          NU_Event_Group_Information(NU_EVENT_GROUP *group, CHAR *name,
                                           UNSIGNED *event_flags, UNSIGNED *tasks_waiting,
                                           NU_TASK **first_task);
UNSIGNED        NU_Event_Group_Pointers(NU_EVENT_GROUP **pointer_list,
                                        UNSIGNED maximum_pointers);

/* Define Signal processing functions.  */
UNSIGNED        NU_Control_Signals(UNSIGNED signal_enable_mask);
UNSIGNED        NU_Receive_Signals(VOID);
STATUS          NU_Register_Signal_Handler(VOID (*signal_handler)(UNSIGNED));
STATUS          NU_Send_Signals(NU_TASK *task, UNSIGNED signals);

/* Define Partition memory management functions.  */
STATUS          NU_Create_Partition_Pool(NU_PARTITION_POOL *pool, CHAR *name,
                                         VOID *start_address, UNSIGNED pool_size,
                                         UNSIGNED partition_size, OPTION suspend_type);
STATUS          NU_Delete_Partition_Pool(NU_PARTITION_POOL *pool);
STATUS          NU_Allocate_Partition(NU_PARTITION_POOL *pool,
                                      VOID **return_pointer, UNSIGNED suspend);
STATUS          NU_Deallocate_Partition(VOID *partition);
UNSIGNED        NU_Established_Partition_Pools(VOID);
STATUS          NU_Partition_Pool_Information(NU_PARTITION_POOL *pool,
                                              CHAR *name,
                                              VOID **start_address,
                                              UNSIGNED *pool_size,
                                              UNSIGNED *partition_size,
                                              UNSIGNED *available,
                                              UNSIGNED *allocated,
                                              OPTION *suspend_type,
                                              UNSIGNED *tasks_waiting,
                                              NU_TASK **first_task);
UNSIGNED        NU_Partition_Pool_Pointers(NU_PARTITION_POOL **pointer_list,
                                           UNSIGNED maximum_pointers);

/* Define Dynamic memory management functions.  */
STATUS          NU_Create_Memory_Pool(NU_MEMORY_POOL *pool, CHAR *name,
                                      VOID *start_address, UNSIGNED pool_size,
                                      UNSIGNED min_allocation, OPTION suspend_type);
STATUS          NU_Delete_Memory_Pool(NU_MEMORY_POOL *pool);

STATUS          NU_Allocate_Memory(NU_MEMORY_POOL *pool, VOID **return_pointer,
                                   UNSIGNED size, UNSIGNED suspend);


STATUS          NU_Allocate_Aligned_Memory(NU_MEMORY_POOL *pool_ptr,
                                           VOID **return_pointer, UNSIGNED size,
                                           UNSIGNED alignment, UNSIGNED suspend);

STATUS          NU_Deallocate_Memory(VOID *memory);
UNSIGNED        NU_Established_Memory_Pools(VOID);
STATUS          NU_Memory_Pool_Information(NU_MEMORY_POOL *pool, CHAR *name,
                                           VOID **start_address, UNSIGNED *pool_size,
                                           UNSIGNED *min_allocation, UNSIGNED *available,
                                           OPTION *suspend_type, UNSIGNED *tasks_waiting,
                                           NU_TASK **first_task);
UNSIGNED        NU_Memory_Pool_Pointers(NU_MEMORY_POOL **pointer_list,
                                        UNSIGNED maximum_pointers);

/* Define Interrupt management functions.  */
INT             NU_Control_Interrupts(INT new_level);
STATUS          NU_Register_LISR(INT vector,
                                 VOID (*new_lisr)(INT),
                                 VOID (**old_lisr)(INT));
STATUS          NU_Activate_HISR(NU_HISR *hisr);
STATUS          NU_Create_HISR(NU_HISR *hisr, CHAR *name,
                               VOID (*hisr_entry)(VOID), OPTION priority,
                               VOID *stack_address, UNSIGNED stack_size);
STATUS          NU_Delete_HISR(NU_HISR *hisr);
NU_HISR         *NU_Current_HISR_Pointer(VOID);
UNSIGNED        NU_Established_HISRs(VOID);
STATUS          NU_HISR_Information(NU_HISR *hisr, CHAR *name,
                                    UNSIGNED *scheduled_count, DATA_ELEMENT *priority,
                                    VOID **stack_base, UNSIGNED *stack_size,
                                    UNSIGNED *minimum_stack);
UNSIGNED        NU_HISR_Pointers(NU_HISR **pointer_list,
                                 UNSIGNED maximum_pointers);
VOID            NU_Protect(NU_PROTECT *protect_struct);
VOID            NU_Unprotect(VOID);

/* Timer management functions.  */
STATUS          NU_Create_Timer(NU_TIMER *timer, CHAR *name,
                                VOID (*expiration_routine)(UNSIGNED), UNSIGNED id,
                                UNSIGNED initial_time, UNSIGNED reschedule_time,
                                OPTION enable);
STATUS          NU_Delete_Timer(NU_TIMER *timer);
STATUS          NU_Reset_Timer(NU_TIMER *timer,
                               VOID (*expiration_routine)(UNSIGNED),
                               UNSIGNED initial_time, UNSIGNED reschedule_timer,
                               OPTION enable);
STATUS          NU_Control_Timer(NU_TIMER *timer, OPTION enable);
UNSIGNED        NU_Established_Timers(VOID);
STATUS          NU_Timer_Information(NU_TIMER *timer, CHAR *name,
                                     OPTION *enable, UNSIGNED *expirations, UNSIGNED *id,
                                     UNSIGNED *initial_time, UNSIGNED *reschedule_time);
STATUS          NU_Get_Remaining_Time(NU_TIMER *timer_ptr, UNSIGNED *remaining_time);
UNSIGNED        NU_Timer_Pointers(NU_TIMER **pointer_list,
                                  UNSIGNED maximum_pointers);

#if (ESAL_AR_32BIT_ACCESS > 1)

VOID            NU_Set_Clock(UNSIGNED new_value);
UNSIGNED        NU_Retrieve_Clock(VOID);

#else

/* Include tm_extr.h for retrieve / set clock macros */
#include        "plus/inc/tm_extr.h"

#endif  /* ESAL_AR_32BIT_ACCESS > 1 */

/* Development support functions.  */
VOID            NU_Get_Release_Version(UINT* major, UINT* minor);
VOID            NU_Disable_History_Saving(VOID);
VOID            NU_Enable_History_Saving(VOID);
VOID            NU_Make_History_Entry(UNSIGNED param1, UNSIGNED param2,
                                      UNSIGNED param3);
STATUS          NU_Retrieve_History_Entry(DATA_ELEMENT *id,
                                          UNSIGNED *param1, UNSIGNED *param2, UNSIGNED *param3,
                                          UNSIGNED *time, NU_TASK **task, NU_HISR **hisr);

/* Input/Output Driver functions.  */
STATUS          NU_Create_Driver(NU_DRIVER *driver, CHAR *name,
                                 VOID (*driver_entry)(NU_DRIVER *, NU_DRIVER_REQUEST *));
STATUS          NU_Delete_Driver(NU_DRIVER *driver);
STATUS          NU_Request_Driver(NU_DRIVER *driver,
                                  NU_DRIVER_REQUEST *request);
STATUS          NU_Resume_Driver(NU_TASK *task);
STATUS          NU_Suspend_Driver(VOID (*terminate_routine)(VOID *),
                                  VOID *information, UNSIGNED timeout);
UNSIGNED        NU_Established_Drivers(VOID);
UNSIGNED        NU_Driver_Pointers(NU_DRIVER **pointer_list,
                                   UNSIGNED maximum_pointers);

#endif  /* !NU_SOURCE_FILE */

#if             (NU_MODULE_SUPPORT == NU_FALSE)
#define         NU_MODULE_SETUP()                   ((VOID) 0)
#endif

#if             (NU_SUPERV_USER_MODE == NU_FALSE)

/* Define Supervisor and User mode functions */
#define         NU_IS_SUPERVISOR_MODE()             (NU_TRUE)
#define         NU_SUPERVISOR_MODE()                ((VOID) 0)
#define         NU_USER_MODE()                      ((VOID) 0)
#define         NU_SUPERV_USER_VARIABLES
#define         NU_SUPERVISOR_MODE_ISR()            ((VOID) 0)
#define         NU_USER_MODE_ISR()                  ((VOID) 0)
#define         NU_MIC_INITIALIZE()                 ((VOID) 0)

#else

/* Include supervisor / user definitions */
#include        "module/inc/su_extr.h"

#endif /* NU_SUPERV_USER_MODE == NU_FALSE */

/* Include Zero Copy external interface */
#include        "plus/inc/zc_extr.h"

/* Check if agent support is enabled */
#if (NU_AGENT_ENABLE == NU_TRUE)

/* Include agent definitions */
#include        "agent/inc/nucleus_agent.h"

#endif /* AGENT_NMI_INIT == NU_TRUE */

#ifdef          __cplusplus

/* End of C declarations */
}

#endif  /* __cplusplus */

#endif  /* !NUCLEUS */
