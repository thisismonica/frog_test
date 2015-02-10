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
/*      xj_cosc.h                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      COSM - Common Operate System Management                          */
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
/*      ZhideYang.      02-20-2008      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

/* Check to see if the file has been included already.  */

#ifndef XJ_COSC_H
#define XJ_COSC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include operate system configuration settings */
#include "os_cfg.h"

/*=======================================================================*/
/* Core processing functions by operating system.  */
typedef XJ_TASK (* _xj_create_task )
    ( CHAR * name, VOID (* task_entry )( UNSIGNED, VOID *),
    UNSIGNED argc, VOID * argv, VOID * stack_address,
    UNSIGNED stack_size, OPTION priority, UNSIGNED time_slice,
    OPTION preempt, OPTION auto_start );

typedef STATUS  (* _xj_delete_task )( XJ_TASK task );

typedef STATUS  (* _xj_reset_task )( XJ_TASK task, UNSIGNED argc,
    VOID * argv );

typedef STATUS  (* _xj_terminate_task )( XJ_TASK task );

typedef STATUS  (* _xj_resume_task )( XJ_TASK task );

typedef STATUS  (* _xj_suspend_task )( XJ_TASK task );

typedef VOID    (* _xj_relinquish )( VOID );

typedef VOID    (* _xj_sleep_task )( UNSIGNED ticks );

typedef OPTION  (* _xj_change_priority )( XJ_TASK task, OPTION new_priority );

typedef OPTION  (* _xj_change_preemption )( OPTION preempt );

typedef UNSIGNED (* _xj_change_time_slice )( XJ_TASK task,
    UNSIGNED time_slice );

typedef XJ_TASK (* _xj_current_task_pointer )( VOID );

typedef UNSIGNED (* _xj_established_tasks )( VOID );

typedef STATUS  (* _xj_task_information )( XJ_TASK task, CHAR * name,
    DATA_ELEMENT * status, UNSIGNED * scheduled_count,
    OPTION * priority, OPTION * preempt,
    UNSIGNED * time_slice, VOID ** stack_base,
    UNSIGNED * stack_size, UNSIGNED * minimum_stack );

typedef UNSIGNED (* _xj_task_pointers )( XJ_TASK * pointer_list,
    UNSIGNED maximum_pointers );

typedef XJ_MAILBOX (* _xj_create_mailbox )( CHAR * name, OPTION suspend_type );

typedef STATUS  (* _xj_delete_mailbox )( XJ_MAILBOX mailbox );

typedef STATUS  (* _xj_reset_mailbox )( XJ_MAILBOX mailbox );

typedef STATUS  (* _xj_send_to_mailbox )( XJ_MAILBOX mailbox, VOID * message,
    UNSIGNED suspend );

typedef STATUS  (* _xj_broadcast_to_mailbox )( XJ_MAILBOX mailbox,
    VOID * message, UNSIGNED suspend );

typedef STATUS  (* _xj_receive_from_mailbox )( XJ_MAILBOX mailbox,
    VOID * message, UNSIGNED suspend );

typedef UNSIGNED (* _xj_established_mailboxes )( VOID );

typedef STATUS  (* _xj_mailbox_information )( XJ_MAILBOX mailbox,
    CHAR * name, OPTION * suspend_type, OPTION * message_present,
    UNSIGNED * tasks_waiting, XJ_TASK * first_task );

typedef UNSIGNED (* _xj_mailbox_pointers )( XJ_MAILBOX * pointer_list,
    UNSIGNED maximum_pointers );

typedef XJ_QUEUE (* _xj_create_queue )( CHAR * name, VOID * start_address,
    UNSIGNED queue_size, OPTION message_type, UNSIGNED message_size,
    OPTION suspend_type );

typedef STATUS  (* _xj_delete_queue )( XJ_QUEUE queue );

typedef STATUS  (* _xj_reset_queue )( XJ_QUEUE queue );

typedef STATUS  (* _xj_send_to_front_of_queue )( XJ_QUEUE queue,
    VOID * message, UNSIGNED size, UNSIGNED suspend );

typedef STATUS  (* _xj_send_to_queue )( XJ_QUEUE queue, VOID * message,
    UNSIGNED size, UNSIGNED suspend );

typedef STATUS  (* _xj_broadcast_to_queue )( XJ_QUEUE queue, VOID * message,
    UNSIGNED size, UNSIGNED suspend );

typedef STATUS  (* _xj_receive_from_queue )( XJ_QUEUE queue, VOID * message,
    UNSIGNED size, UNSIGNED * actual_size, UNSIGNED suspend );

typedef UNSIGNED (* _xj_established_queues )( VOID );

typedef STATUS  (* _xj_queue_information )( XJ_QUEUE queue, CHAR * name,
    VOID ** start_address, UNSIGNED * queue_size,
    UNSIGNED * available, UNSIGNED * messages,
    OPTION * message_type, UNSIGNED * message_size,
    OPTION * suspend_type, UNSIGNED * tasks_waiting,
    XJ_TASK * first_task );

typedef UNSIGNED (* _xj_queue_pointers )( XJ_QUEUE * pointer_list,
    UNSIGNED maximum_pointers );

typedef XJ_PIPE (* _xj_create_pipe )( CHAR * name, VOID * start_address,
    UNSIGNED pipe_size, OPTION message_type, UNSIGNED message_size,
    OPTION suspend_type );

typedef STATUS  (* _xj_delete_pipe )( XJ_PIPE pipe );

typedef STATUS  (* _xj_reset_pipe )( XJ_PIPE pipe );

typedef STATUS  (* _xj_send_to_front_of_pipe )( XJ_PIPE pipe, VOID * message,
    UNSIGNED size, UNSIGNED suspend );

typedef STATUS  (* _xj_send_to_pipe )( XJ_PIPE pipe, VOID * message,
    UNSIGNED size, UNSIGNED suspend );

typedef STATUS  (* _xj_broadcast_to_pipe )( XJ_PIPE pipe, VOID * message,
    UNSIGNED size, UNSIGNED suspend );

typedef STATUS  (* _xj_receive_from_pipe )( XJ_PIPE pipe, VOID * message,
    UNSIGNED size, UNSIGNED * actual_size, UNSIGNED suspend );

typedef UNSIGNED (* _xj_established_pipes )( VOID );

typedef STATUS  (* _xj_pipe_information )( XJ_PIPE pipe, CHAR * name,
    VOID ** start_address, UNSIGNED * pipe_size,
    UNSIGNED * available, UNSIGNED * messages,
    OPTION * message_type, UNSIGNED * message_size,
    OPTION * suspend_type, UNSIGNED * tasks_waiting,
    XJ_TASK * first_task );

typedef UNSIGNED (* _xj_pipe_pointers )( XJ_PIPE * pointer_list,
    UNSIGNED maximum_pointers );

typedef XJ_SEMAPHORE (* _xj_create_semaphore )( CHAR * name,
    UNSIGNED initial_count, OPTION suspend_type );

typedef STATUS  (* _xj_delete_semaphore )( XJ_SEMAPHORE semaphore );

typedef STATUS  (* _xj_reset_semaphor )( XJ_SEMAPHORE semaphore,
    UNSIGNED initial_count );

typedef STATUS  (* _xj_obtain_semaphore )( XJ_SEMAPHORE semaphore,
    UNSIGNED suspend );

typedef STATUS  (* _xj_release_semaphore )( XJ_SEMAPHORE semaphore );

typedef UNSIGNED (* _xj_established_semaphores )( VOID );

typedef STATUS  (* _xj_semaphore_information )( XJ_SEMAPHORE semaphore,
    CHAR * name, UNSIGNED * current_count, OPTION * suspend_type,
    UNSIGNED * tasks_waiting, XJ_TASK * first_task );

typedef UNSIGNED (* _xj_semaphore_pointers )( XJ_SEMAPHORE * pointer_list,
    UNSIGNED maximum_pointer );

typedef XJ_EVENT_GROUP (* _xj_create_event_group )( CHAR * name );

typedef STATUS  (* _xj_delete_event_group )( XJ_EVENT_GROUP group );

typedef STATUS  (* _xj_set_events )( XJ_EVENT_GROUP group, UNSIGNED events,
    OPTION operation );

typedef STATUS  (* _xj_retrieve_events )( XJ_EVENT_GROUP group,
    UNSIGNED requested_flags, OPTION operation,
    UNSIGNED * retrieved_flags, UNSIGNED suspend );

typedef UNSIGNED (* _xj_established_event_groups )( VOID );

typedef STATUS  (* _xj_event_group_information )( XJ_EVENT_GROUP group,
    CHAR * name, UNSIGNED * event_flags, UNSIGNED * tasks_waiting,
    XJ_TASK * first_task );

typedef UNSIGNED (* _xj_event_group_pointers )( XJ_EVENT_GROUP * pointer_list,
    UNSIGNED maximum_pointers );

typedef XJ_PARTITION_POOL (* _xj_create_partition_pool )( CHAR * name,
    VOID * start_address, UNSIGNED pool_size,
    UNSIGNED partition_size, OPTION suspend_type );

typedef STATUS  (* _xj_delete_partition_pool )( XJ_PARTITION_POOL pool );

typedef STATUS  (* _xj_allocate_partition )( XJ_PARTITION_POOL pool,
    VOID ** return_pointer, UNSIGNED suspend );

typedef STATUS  (* _xj_deallocate_partition )( VOID * partition );

typedef UNSIGNED (* _xj_established_partition_pools )( VOID );

typedef STATUS  (* _xj_partition_pool_information )( XJ_PARTITION_POOL pool,
    CHAR * name, VOID ** start_address, UNSIGNED * pool_size,
    UNSIGNED * partition_size, UNSIGNED * available,
    UNSIGNED * allocated, OPTION * suspend_type,
    UNSIGNED * tasks_waiting, XJ_TASK * first_task );

typedef UNSIGNED (* _xj_partition_pool_pointers )( XJ_PARTITION_POOL * pointer_list,
    UNSIGNED maximum_pointers );

typedef XJ_MEMORY_POOL (* _xj_create_memory_pool )( CHAR * name,
    VOID * start_address, UNSIGNED pool_size,
    UNSIGNED min_allocation, OPTION suspend_type );

typedef STATUS  (* _xj_delete_memory_pool )( XJ_MEMORY_POOL pool );

typedef STATUS  (* _xj_allocate_memory )( XJ_MEMORY_POOL pool,
    VOID ** return_pointer, UNSIGNED size, UNSIGNED suspend );

typedef STATUS  (* _xj_deallocate_memory )( VOID * memory );

typedef STATUS  (* _xj_allocate_aligned_memory )( XJ_MEMORY_POOL pool_ptr,
    VOID ** return_pointer, UNSIGNED size, UNSIGNED alignment, UNSIGNED suspend );

typedef UNSIGNED (* _xj_established_memory_pools )( VOID );

typedef STATUS  (* _xj_memory_pool_information )( XJ_MEMORY_POOL pool, CHAR * name,
    VOID ** start_address, UNSIGNED * pool_size,
    UNSIGNED * min_allocation, UNSIGNED * available,
    OPTION * suspend_type, UNSIGNED * tasks_waiting,
    XJ_TASK * first_task );

typedef UNSIGNED (* _xj_memory_pool_pointers )( XJ_MEMORY_POOL * pointer_list,
    UNSIGNED maximum_pointers );

typedef UNSIGNED (* _xj_control_signals )( UNSIGNED signal_enable_mask );

typedef UNSIGNED (* _xj_receive_signals )( VOID );

typedef STATUS  (* _xj_register_signal_handler )( VOID (* signal_handler )( UNSIGNED ));

typedef STATUS  (* _xj_send_signals )( XJ_TASK task, UNSIGNED signals );

typedef STATUS  (* _xj_register_LISR )( INT vector,
    VOID (* new_lisr )( INT ), VOID (** old_lisr )( INT ));

typedef XJ_HISR (* _xj_create_HISR )( CHAR * name, VOID (* hisr_entry )( VOID ),
    OPTION priority, VOID * stack_address, UNSIGNED stack_size );

typedef STATUS  (* _xj_activate_HISR )( XJ_HISR hisr );

typedef STATUS  (* _xj_delete_HISR )( XJ_HISR hisr );

typedef XJ_HISR (* _xj_current_HISR_pointer )( VOID );

typedef UNSIGNED (* _xj_established_HISRs )( VOID );

typedef UNSIGNED (* _xj_HISR_pointers )( XJ_HISR * pointer_list,
    UNSIGNED maximum_pointers );

typedef STATUS  (* _xj_HISR_information )( XJ_HISR hisr, CHAR * name,
    UNSIGNED * scheduled_count, DATA_ELEMENT * priority,
    VOID ** stack_base, UNSIGNED * stack_size,
    UNSIGNED * minimum_stack );

typedef XJ_PROTECT (* _xj_create_protect )( VOID );

typedef VOID    (* _xj_protect )( XJ_PROTECT protect_struct );

typedef VOID    (* _xj_unprotect )( VOID );

typedef INT     (* _xj_control_interrupts )( INT new_level );

typedef INT     (* _xj_local_control_interrupts )( INT new_level );

typedef UNSIGNED (* _xj_retrieve_clock )( VOID );

typedef XJ_TIMER (* _xj_create_timer )( CHAR * name,
    VOID (* expiration_routine )( UNSIGNED ), UNSIGNED id,
    UNSIGNED initial_time, UNSIGNED reschedule_time,
    OPTION enable );

typedef STATUS  (* _xj_delete_timer )( XJ_TIMER timer );

typedef STATUS  (* _xj_control_timer )( XJ_TIMER timer, OPTION enable );

typedef STATUS  (* _xj_reset_timer )( XJ_TIMER timer,
    VOID (* expiration_routine )( UNSIGNED ),
    UNSIGNED initial_time, UNSIGNED reschedule_timer,
    OPTION enable );

typedef UNSIGNED (* _xj_established_timers )( VOID );

typedef UNSIGNED (* _xj_timer_pointers )( XJ_TIMER * pointer_list,
    UNSIGNED maximum_pointers );

typedef STATUS  (* _xj_timer_information )( XJ_TIMER timer, CHAR * name,
    OPTION * enable, UNSIGNED * expirations, UNSIGNED * id,
    UNSIGNED * initial_time, UNSIGNED * reschedule_time );

typedef STATUS  (* _xj_get_remaining_time )( XJ_TIMER timer_ptr,
    UNSIGNED * remaining_time );

typedef VOID    (* _xj_disable_history_saving )( VOID );

typedef VOID    (* _xj_enable_history_saving )( VOID );

typedef VOID    (* _xj_make_history_entry )( UNSIGNED param1, UNSIGNED param2,
    UNSIGNED param3 );

typedef STATUS  (* _xj_retrieve_history_entry )( DATA_ELEMENT * id,
    UNSIGNED * param1, UNSIGNED * param2, UNSIGNED * param3,
    UNSIGNED * time, XJ_TASK * task, XJ_HISR * hisr );

typedef XJ_DRIVER (* _xj_create_driver )( CHAR * name,
    VOID (* driver_entry )( XJ_DRIVER, XJ_DRIVER_REQUEST * ));

typedef STATUS  (* _xj_delete_driver )( XJ_DRIVER driver );

typedef STATUS  (* _xj_request_driver )( XJ_DRIVER driver,
    XJ_DRIVER_REQUEST * request );

typedef STATUS  (* _xj_resume_driver )( XJ_TASK task );

typedef STATUS  (* _xj_suspend_driver )( VOID (* terminate_routine )( VOID *),
    VOID * information, UNSIGNED timeout );

typedef UNSIGNED (* _xj_established_drivers )( VOID );

typedef UNSIGNED (* _xj_driver_pointers )( XJ_DRIVER * pointer_list,
    UNSIGNED maximum_pointers );
/*=======================================================================*/
/* 以下为保持版本兼容的封装函数 */
typedef STATUS  (* _nu_create_task )
    ( XJ_TASK task, CHAR * name,
    VOID (* task_entry )( UNSIGNED, VOID *), UNSIGNED argc,
    VOID * argv, VOID * stack_address, UNSIGNED stack_size,
    OPTION priority, UNSIGNED time_slice,
    OPTION preempt, OPTION auto_start );

typedef STATUS  (* _nu_create_mailbox )( XJ_MAILBOX mailbox, CHAR * name,
    OPTION suspend_type );

typedef STATUS  (* _nu_create_queue )( XJ_QUEUE queue, CHAR * name,
    VOID * start_address, UNSIGNED queue_size,
    OPTION message_type, UNSIGNED message_size,
    OPTION suspend_type );

typedef STATUS  (* _nu_create_pipe )( XJ_PIPE pipe, CHAR * name,
    VOID * start_address, UNSIGNED pipe_size,
    OPTION message_type, UNSIGNED message_size,
    OPTION suspend_type );

typedef STATUS  (* _nu_create_semaphore )( XJ_SEMAPHORE semaphore, CHAR * name,
    UNSIGNED initial_count, OPTION suspend_type );

typedef STATUS  (* _nu_create_event_group )( XJ_EVENT_GROUP group, CHAR * name );

typedef STATUS  (* _nu_create_partition_pool )( XJ_PARTITION_POOL pool, CHAR * name,
    VOID * start_address, UNSIGNED pool_size,
    UNSIGNED partition_size, OPTION suspend_type );

typedef STATUS  (* _nu_create_memory_pool )( XJ_MEMORY_POOL pool, CHAR * name,
    VOID * start_address, UNSIGNED pool_size,
    UNSIGNED min_allocation, OPTION suspend_type );

typedef STATUS  (* _nu_create_HISR )( XJ_HISR hisr, CHAR * name,
    VOID (* hisr_entry )( VOID ), OPTION priority,
    VOID * stack_address, UNSIGNED stack_size );

typedef STATUS  (* _nu_create_timer )( XJ_TIMER timer, CHAR * name,
    VOID (* expiration_routine )( UNSIGNED ), UNSIGNED id,
    UNSIGNED initial_time, UNSIGNED reschedule_time,
    OPTION enable );

typedef STATUS  (* _nu_create_driver )( XJ_DRIVER driver, CHAR * name,
    VOID (* driver_entry )( XJ_DRIVER , XJ_DRIVER_REQUEST * ));
/*=======================================================================*/
/* Define the data structure for use within the core processing functions.  */
typedef struct OS_FUN_STRUCT
{
    /*  Standard protect struct information.   Changes made
    to this area of the structure can have undesirable side effects. */

    _xj_create_task                     create_task;            /* create task. */
    _xj_delete_task                     delete_task;            /* delete task. */
    _xj_reset_task                      reset_task;             /* reset task. */
    _xj_terminate_task                  terminate_task;         /* terminate task. */
    _xj_resume_task                     resume_task;            /* resume task. */
    _xj_suspend_task                    suspend_task;           /* suspend task. */
    _xj_relinquish                      relinquish;             /* relinquish time slice. */
    _xj_sleep_task                      sleep_task;             /* task sleep. */
    _xj_change_priority                 change_priority;        /* change task priority. */
    _xj_change_preemption               change_preemption;      /* change task preemption. */
    _xj_change_time_slice               change_time_slice;      /* change time slice. */
    _xj_current_task_pointer            current_task_pointer;   /* current task pointer. */
    _xj_established_tasks               established_tasks;      /* established tasks. */
    _xj_task_information                task_information;       /* task information. */
    _xj_task_pointers                   task_pointers;          /* task pointers. */
    _xj_create_mailbox                  create_mailbox;         /* create mailbox. */
    _xj_delete_mailbox                  delete_mailbox;         /* delete mailbox. */
    _xj_reset_mailbox                   reset_mailbox;          /* reset mailbox. */
    _xj_send_to_mailbox                 send_to_mailbox;        /* send to mailbox. */
    _xj_broadcast_to_mailbox            broadcast_to_mailbox;   /* broadcast to mailbox. */
    _xj_receive_from_mailbox            receive_from_mailbox;   /* receive from mailbox. */
    _xj_established_mailboxes           established_mailboxes;  /* established mailboxes. */
    _xj_mailbox_information             mailbox_information;    /* mailbox information. */
    _xj_mailbox_pointers                mailbox_pointers;       /* mailbox pointers. */
    _xj_create_queue                    create_queue;           /* create queue. */
    _xj_delete_queue                    delete_queue;           /* delete queue. */
    _xj_reset_queue                     reset_queue;            /* reset queue. */
    _xj_send_to_front_of_queue          send_to_front_of_queue; /* send to front of queue. */
    _xj_send_to_queue                   send_to_queue;          /* send to queue. */
    _xj_broadcast_to_queue              broadcast_to_queue;     /* broadcast to queue. */
    _xj_receive_from_queue              receive_from_queue;     /* receive from queue. */
    _xj_established_queues              established_queues;     /* established queues. */
    _xj_queue_information               queue_information;      /* queue information. */
    _xj_queue_pointers                  queue_pointers;         /* queue pointers. */
    _xj_create_pipe                     create_pipe;            /* create pipe. */
    _xj_delete_pipe                     delete_pipe;            /* delete pipe. */
    _xj_reset_pipe                      reset_pipe;             /* reset pipe. */
    _xj_send_to_front_of_pipe           send_to_front_of_pipe;  /* send to front of pipe. */
    _xj_send_to_pipe                    send_to_pipe;           /* send to pipe. */
    _xj_broadcast_to_pipe               broadcast_to_pipe;      /* broadcast to pipe. */
    _xj_receive_from_pipe               receive_from_pipe;      /* receive from pipe. */
    _xj_established_pipes               established_pipes;      /* established pipes. */
    _xj_pipe_information                pipe_information;       /* pipe information. */
    _xj_pipe_pointers                   pipe_pointers;          /* pipe pointers. */
    _xj_create_semaphore                create_semaphore;       /* create semaphore. */
    _xj_delete_semaphore                delete_semaphore;       /* delete semaphore. */
    _xj_reset_semaphor                  reset_semaphor;         /* reset semaphor. */
    _xj_obtain_semaphore                obtain_semaphore;       /* obtain semaphore. */
    _xj_release_semaphore               release_semaphore;      /* release semaphore. */
    _xj_established_semaphores          established_semaphores; /* established semaphores. */
    _xj_semaphore_information           semaphore_information;  /* semaphore information. */
    _xj_semaphore_pointers              semaphore_pointers;     /* semaphore pointers. */
    _xj_create_event_group              create_event_group;     /* create event group. */
    _xj_delete_event_group              delete_event_group;     /* delete event group. */
    _xj_set_events                      set_events;             /* set events. */
    _xj_retrieve_events                 retrieve_events;        /* retrieve events. */
    _xj_established_event_groups        established_event_groups;/* established event groups. */
    _xj_event_group_information         event_group_information;/* event group information. */
    _xj_event_group_pointers            event_group_pointers;   /* event group pointers. */
    _xj_create_partition_pool           create_partition_pool;  /* create partition pool. */
    _xj_delete_partition_pool           delete_partition_pool;  /* delete partition pool. */
    _xj_allocate_partition              allocate_partition;     /* allocate partition. */
    _xj_deallocate_partition            deallocate_partition;   /* deallocate partition. */
    _xj_established_partition_pools     established_partition_pools;/* established partition pools. */
    _xj_partition_pool_information      partition_pool_information; /* partition pool information. */
    _xj_partition_pool_pointers         partition_pool_pointers;/* partition pool pointers. */
    _xj_create_memory_pool              create_memory_pool;     /* create memory pool. */
    _xj_delete_memory_pool              delete_memory_pool;     /* delete memory pool. */
    _xj_allocate_memory                 allocate_memory;        /* allocate memory. */
    _xj_deallocate_memory               deallocate_memory;      /* deallocate memory. */
    _xj_allocate_aligned_memory         allocate_aligned_memory; /* allocate_aligned_memory */
    _xj_established_memory_pools        established_memory_pools;/* established memory pools. */
    _xj_memory_pool_information         memory_pool_information;/* memory pool information. */
    _xj_memory_pool_pointers            memory_pool_pointers;   /* memory pool pointers. */
    _xj_control_signals                 control_signals;        /* control signals. */
    _xj_receive_signals                 receive_signals;        /* receive signals. */
    _xj_register_signal_handler         register_signal_handler;/* register signal handler. */
    _xj_send_signals                    send_signals;           /* send_signals. */
    _xj_register_LISR                   register_LISR;          /* register LISR. */
    _xj_activate_HISR                   activate_HISR;          /* activate HISR. */
    _xj_create_HISR                     create_HISR;            /* create HISR. */
    _xj_delete_HISR                     delete_HISR;            /* delete HISR. */
    _xj_current_HISR_pointer            current_HISR_pointer;   /* current HISR_pointer. */
    _xj_established_HISRs               established_HISRs;      /* established HISRs. */
    _xj_HISR_pointers                   HISR_pointers;          /* HISR pointers. */
    _xj_HISR_information                HISR_information;       /* HISR information. */
    _xj_create_protect                  create_protect;         /* create protect */
    _xj_protect                         protect;                /* data protect. */
    _xj_unprotect                       unprotect;              /* data unprotect. */
    _xj_control_interrupts              control_interrupts;     /* control interrupts. */
    _xj_local_control_interrupts        local_control_interrupts;/* local control interrupts. */
    _xj_retrieve_clock                  retrieve_clock;         /* retrieve clock */
    _xj_create_timer                    create_timer;           /* create timer. */
    _xj_delete_timer                    delete_timer;           /* delete timer. */
    _xj_control_timer                   control_timer;          /* control timer. */
    _xj_reset_timer                     reset_timer;            /* reset timer. */
    _xj_established_timers              established_timers;     /* established timers. */
    _xj_timer_pointers                  timer_pointers;         /* timer pointers. */
    _xj_timer_information               timer_information;      /* timer information. */
    _xj_get_remaining_time              get_remaining_time;     /* get_remaining_time */
    _xj_disable_history_saving          disable_history_saving; /* disable history saving. */
    _xj_enable_history_saving           enable_history_saving;  /* enable history saving. */
    _xj_make_history_entry              make_history_entry;     /* make history entry. */
    _xj_retrieve_history_entry          retrieve_history_entry; /* retrieve history entry. */
    _xj_create_driver                   create_driver;          /* create driver. */
    _xj_delete_driver                   delete_driver;          /* delete driver. */
    _xj_request_driver                  request_driver;         /* request driver. */
    _xj_resume_driver                   resume_driver;          /* resume driver. */
    _xj_suspend_driver                  suspend_driver;         /* suspend driver. */
    _xj_established_drivers             established_drivers;    /* established drivers. */
    _xj_driver_pointers                 driver_pointers;        /* driver pointers. */
    /*=======================================================================*/
    /* 以下为保持版本兼容的封装函数 */
    _nu_create_task                     nu_create_task;         /* create task. */
    _nu_create_mailbox                  nu_create_mailbox;      /* create mailbox. */
    _nu_create_queue                    nu_create_queue;        /* create queue. */
    _nu_create_pipe                     nu_create_pipe;         /* create pipe. */
    _nu_create_semaphore                nu_create_semaphore;    /* create semaphore. */
    _nu_create_event_group              nu_create_event_group;  /* create event group. */
    _nu_create_partition_pool           nu_create_partition_pool; /* create partition pool. */
    _nu_create_memory_pool              nu_create_memory_pool;  /* create memory pool. */
    _nu_create_HISR                     nu_create_HISR;         /* create HISR. */
    _nu_create_timer                    nu_create_timer;        /* create timer. */
    _nu_create_driver                   nu_create_driver;       /* create driver. */
}OS_FUN;

/*=======================================================================*/
/* 操作函数                                                              */
/*=======================================================================*/
/* Define external inner-component global data references.  */
/* 操作系统函数表 */
extern OS_FUN * OS_System_List;

#define OS_SYSTEM_PTR                   OS_System_List
/* Re-map task control functions depending on whether or not error checking
   is specified.  */
#define         XJ_Create_Task                  (OS_SYSTEM_PTR->create_task)
#define         XJ_Delete_Task                  (OS_SYSTEM_PTR->delete_task)
#define         XJ_Reset_Task                   (OS_SYSTEM_PTR->reset_task)
#define         XJ_Terminate_Task               (OS_SYSTEM_PTR->terminate_task)
#define         XJ_Resume_Task                  (OS_SYSTEM_PTR->resume_task)
#define         XJ_Suspend_Task                 (OS_SYSTEM_PTR->suspend_task)
#define         XJ_Relinquish                   (OS_SYSTEM_PTR->relinquish)
#define         XJ_Sleep                        (OS_SYSTEM_PTR->sleep_task)
#define         XJ_Change_Priority              (OS_SYSTEM_PTR->change_priority)
#define         XJ_Change_Preemption            (OS_SYSTEM_PTR->change_preemption)
#define         XJ_Change_Time_Slice            (OS_SYSTEM_PTR->change_time_slice)
#define         XJ_Current_Task_Pointer         (OS_SYSTEM_PTR->current_task_pointer)
#define         XJ_Established_Tasks            (OS_SYSTEM_PTR->established_tasks)
#define         XJ_Task_Information             (OS_SYSTEM_PTR->task_information)
#define         XJ_Task_Pointers                (OS_SYSTEM_PTR->task_pointers)
#define         XJ_Create_Mailbox               (OS_SYSTEM_PTR->create_mailbox)
#define         XJ_Delete_Mailbox               (OS_SYSTEM_PTR->delete_mailbox)
#define         XJ_Reset_Mailbox                (OS_SYSTEM_PTR->reset_mailbox)
#define         XJ_Send_To_Mailbox              (OS_SYSTEM_PTR->send_to_mailbox)
#define         XJ_Broadcast_To_Mailbox         (OS_SYSTEM_PTR->broadcast_to_mailbox)
#define         XJ_Receive_From_Mailbox         (OS_SYSTEM_PTR->receive_from_mailbox)
#define         XJ_Established_Mailboxes        (OS_SYSTEM_PTR->established_mailboxes)
#define         XJ_Mailbox_Information          (OS_SYSTEM_PTR->mailbox_information)
#define         XJ_Mailbox_Pointers             (OS_SYSTEM_PTR->mailbox_pointers)
#define         XJ_Create_Queue                 (OS_SYSTEM_PTR->create_queue)
#define         XJ_Delete_Queue                 (OS_SYSTEM_PTR->delete_queue)
#define         XJ_Reset_Queue                  (OS_SYSTEM_PTR->reset_queue)
#define         XJ_Send_To_Front_Of_Queue       (OS_SYSTEM_PTR->send_to_front_of_queue)
#define         XJ_Send_To_Queue                (OS_SYSTEM_PTR->send_to_queue)
#define         XJ_Broadcast_To_Queue           (OS_SYSTEM_PTR->broadcast_to_queue)
#define         XJ_Receive_From_Queue           (OS_SYSTEM_PTR->receive_from_queue)
#define         XJ_Established_Queues           (OS_SYSTEM_PTR->established_queues)
#define         XJ_Queue_Information            (OS_SYSTEM_PTR->queue_information)
#define         XJ_Queue_Pointers               (OS_SYSTEM_PTR->queue_pointers)
#define         XJ_Create_Pipe                  (OS_SYSTEM_PTR->create_pipe)
#define         XJ_Delete_Pipe                  (OS_SYSTEM_PTR->delete_pipe)
#define         XJ_Reset_Pipe                   (OS_SYSTEM_PTR->reset_pipe)
#define         XJ_Send_To_Front_Of_Pipe        (OS_SYSTEM_PTR->send_to_front_of_pipe)
#define         XJ_Send_To_Pipe                 (OS_SYSTEM_PTR->send_to_pipe)
#define         XJ_Broadcast_To_Pipe            (OS_SYSTEM_PTR->broadcast_to_pipe)
#define         XJ_Receive_From_Pipe            (OS_SYSTEM_PTR->receive_from_pipe)
#define         XJ_Established_Pipes            (OS_SYSTEM_PTR->established_pipes)
#define         XJ_Pipe_Information             (OS_SYSTEM_PTR->pipe_information)
#define         XJ_Pipe_Pointers                (OS_SYSTEM_PTR->pipe_pointers)
#define         XJ_Create_Semaphore             (OS_SYSTEM_PTR->create_semaphore)
#define         XJ_Delete_Semaphore             (OS_SYSTEM_PTR->delete_semaphore)
#define         XJ_Reset_Semaphore              (OS_SYSTEM_PTR->reset_semaphor)
#define         XJ_Obtain_Semaphore             (OS_SYSTEM_PTR->obtain_semaphore)
#define         XJ_Release_Semaphore            (OS_SYSTEM_PTR->release_semaphore)
#define         XJ_Established_Semaphores       (OS_SYSTEM_PTR->established_semaphores)
#define         XJ_Semaphore_Information        (OS_SYSTEM_PTR->semaphore_information)
#define         XJ_Semaphore_Pointers           (OS_SYSTEM_PTR->semaphore_pointers)
#define         XJ_Create_Event_Group           (OS_SYSTEM_PTR->create_event_group)
#define         XJ_Delete_Event_Group           (OS_SYSTEM_PTR->delete_event_group)
#define         XJ_Set_Events                   (OS_SYSTEM_PTR->set_events)
#define         XJ_Retrieve_Events              (OS_SYSTEM_PTR->retrieve_events)
#define         XJ_Established_Event_Groups     (OS_SYSTEM_PTR->established_event_groups)
#define         XJ_Event_Group_Information      (OS_SYSTEM_PTR->event_group_information)
#define         XJ_Event_Group_Pointers         (OS_SYSTEM_PTR->event_group_pointers)
#define         XJ_Create_Partition_Pool        (OS_SYSTEM_PTR->create_partition_pool)
#define         XJ_Delete_Partition_Pool        (OS_SYSTEM_PTR->delete_partition_pool)
#define         XJ_Allocate_Partition           (OS_SYSTEM_PTR->allocate_partition)
#define         XJ_Deallocate_Partition         (OS_SYSTEM_PTR->deallocate_partition)
#define         XJ_Established_Partition_Pools  (OS_SYSTEM_PTR->established_partition_pools)
#define         XJ_Partition_Pool_Information   (OS_SYSTEM_PTR->partition_pool_information)
#define         XJ_Partition_Pool_Pointers      (OS_SYSTEM_PTR->partition_pool_pointers)
#define         XJ_Create_Memory_Pool           (OS_SYSTEM_PTR->create_memory_pool)
#define         XJ_Delete_Memory_Pool           (OS_SYSTEM_PTR->delete_memory_pool)
#define         XJ_Allocate_Memory              (OS_SYSTEM_PTR->allocate_memory)
#define         XJ_Deallocate_Memory            (OS_SYSTEM_PTR->deallocate_memory)
#define         XJ_Allocate_Aligned_Memory      (OS_SYSTEM_PTR->allocate_aligned_memory)
#define         XJ_Established_Memory_Pools     (OS_SYSTEM_PTR->established_memory_pools)
#define         XJ_Memory_Pool_Information      (OS_SYSTEM_PTR->memory_pool_information)
#define         XJ_Memory_Pool_Pointers         (OS_SYSTEM_PTR->memory_pool_pointers)
#define         XJ_Control_Signals              (OS_SYSTEM_PTR->control_signals)
#define         XJ_Receive_Signals              (OS_SYSTEM_PTR->receive_signals)
#define         XJ_Register_Signal_Handler      (OS_SYSTEM_PTR->register_signal_handler)
#define         XJ_Send_Signals                 (OS_SYSTEM_PTR->send_signals)
#define         XJ_Register_LISR                (OS_SYSTEM_PTR->register_LISR)
#define         XJ_Activate_HISR                (OS_SYSTEM_PTR->activate_HISR)
#define         XJ_Create_HISR                  (OS_SYSTEM_PTR->create_HISR)
#define         XJ_Delete_HISR                  (OS_SYSTEM_PTR->delete_HISR)
#define         XJ_Current_HISR_Pointer         (OS_SYSTEM_PTR->current_HISR_pointer)
#define         XJ_Established_HISRs            (OS_SYSTEM_PTR->established_HISRs)
#define         XJ_HISR_Pointers                (OS_SYSTEM_PTR->HISR_pointers)
#define         XJ_HISR_Information             (OS_SYSTEM_PTR->HISR_information)
#define         XJ_Create_Protect               (OS_SYSTEM_PTR->create_protect)
#define         XJ_Protect                      (OS_SYSTEM_PTR->protect)
#define         XJ_Unprotect                    (OS_SYSTEM_PTR->unprotect)
#define         XJ_Control_Interrupts           (OS_SYSTEM_PTR->control_interrupts)
#define         XJ_Local_Control_Interrupts     (OS_SYSTEM_PTR->local_control_interrupts)
#define         XJ_Retrieve_Clock               (OS_SYSTEM_PTR->retrieve_clock)
#define         XJ_Create_Timer                 (OS_SYSTEM_PTR->create_timer)
#define         XJ_Delete_Timer                 (OS_SYSTEM_PTR->delete_timer)
#define         XJ_Control_Timer                (OS_SYSTEM_PTR->control_timer)
#define         XJ_Reset_Timer                  (OS_SYSTEM_PTR->reset_timer)
#define         XJ_Established_Timers           (OS_SYSTEM_PTR->established_timers)
#define         XJ_Timer_Pointers               (OS_SYSTEM_PTR->timer_pointers)
#define         XJ_Timer_Information            (OS_SYSTEM_PTR->timer_information)
#define         XJ_Get_Remaining_Time           (OS_SYSTEM_PTR->get_remaining_time)
#define         XJ_Disable_History_Saving       (OS_SYSTEM_PTR->disable_history_saving)
#define         XJ_Enable_History_Saving        (OS_SYSTEM_PTR->enable_history_saving)
#define         XJ_Make_History_Entry           (OS_SYSTEM_PTR->make_history_entry)
#define         XJ_Retrieve_History_Entry       (OS_SYSTEM_PTR->retrieve_history_entry)
#define         XJ_Create_Driver                (OS_SYSTEM_PTR->create_driver)
#define         XJ_Delete_Driver                (OS_SYSTEM_PTR->delete_driver)
#define         XJ_Request_Driver               (OS_SYSTEM_PTR->request_driver)
#define         XJ_Resume_Driver                (OS_SYSTEM_PTR->resume_driver)
#define         XJ_Suspend_Driver               (OS_SYSTEM_PTR->suspend_driver)
#define         XJ_Established_Drivers          (OS_SYSTEM_PTR->established_drivers)
#define         XJ_Driver_Pointers              (OS_SYSTEM_PTR->driver_pointers)
/* 以下为保持版本兼容的封装函数 */
#define         XJ_NU_Create_Task               (OS_SYSTEM_PTR->nu_create_task)
#define         XJ_NU_Create_Mailbox            (OS_SYSTEM_PTR->nu_create_mailbox)
#define         XJ_NU_Create_Queue              (OS_SYSTEM_PTR->nu_create_queue)
#define         XJ_NU_Create_Pipe               (OS_SYSTEM_PTR->nu_create_pipe)
#define         XJ_NU_Create_Semaphore          (OS_SYSTEM_PTR->nu_create_semaphore)
#define         XJ_NU_Create_Event_Group        (OS_SYSTEM_PTR->nu_create_event_group)
#define         XJ_NU_Create_Partition_Pool     (OS_SYSTEM_PTR->nu_create_partition_pool)
#define         XJ_NU_Create_Memory_Pool        (OS_SYSTEM_PTR->nu_create_memory_pool)
#define         XJ_NU_Create_HISR               (OS_SYSTEM_PTR->nu_create_HISR)
#define         XJ_NU_Create_Timer              (OS_SYSTEM_PTR->nu_create_timer)
#define         XJ_NU_Create_Driver             (OS_SYSTEM_PTR->nu_create_driver)

#ifdef __cplusplus
}
#endif

#endif
