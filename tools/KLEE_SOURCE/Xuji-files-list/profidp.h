#ifndef _PROFIBUS_H
#define _PROFIBUS_H
#include "types.h"

#define CFG_IS_BYTE_FORMAT          0x30
#define CFG_BF_LENGTH               0x0f
#define CFG_LENGTH_IS_WORD_FORMAT   0x40
#define CFG_BF_INP_EXIST            0x10
#define CFG_BF_OUTP_EXIST           0x20
#define CFG_SF_OUTP_EXIST           0x80
#define CFG_SF_INP_EXIST            0x40
#define CFG_SF_LENGTH               0x3f

#define FIXED_DIAG_SIZE  6  /*diagnostic data min data size,system data included in*/
#define FIXED_PARM_SIZE  7  /*prm data min size, syatem data included in the field*/

typedef void (* PROFIDP_OUTPUT_CB ) ( u8 * data, void * arg );
typedef void (* PROFIDP_STATION_ADR_CHANGE_CB ) ( u8 new_address, int enable_address_change, void * arg );

/*new_stat definiton: 1.state_wait_parm; 2.state_wait_cfg; 3.state_data_ex; 4.state_error*/
typedef void (* PROFIDP_STATE_CHANGE_CB ) ( u8 new_state, void * arg );
typedef void (* PROFIDP_GC_CB ) ( u8 global_command, void * arg );

/* return 1 if configure is allowed, else return 0*/
typedef int (* PROFIDP_CONFIG_CB ) ( u8 * cfg, u8 len, void * arg );

/* return 1 if parameter is allowed, else return 0*/
typedef int (* PROFIDP_PARAM_CB ) ( u8  * param_data, u8 param_len, void * arg );

struct profidp_cfg
{
    int dev_index;
    u16 ident_number;
    u8 station_address;
    u8 io_is_consistence;              
    u8 io_is_word_size;          
    u8 input_data_len;     
    u8 output_data_len;    
    u8 diag_buf_len;     
    u8 mintsdr;          

    /*callback function*/
    PROFIDP_OUTPUT_CB output_cb;
    PROFIDP_PARAM_CB param_cb;
    PROFIDP_CONFIG_CB config_cb;
    PROFIDP_STATION_ADR_CHANGE_CB addr_change_cb;
    PROFIDP_STATE_CHANGE_CB state_change_cb;
    PROFIDP_GC_CB gc_cb;
    void * arg;
};

int profidp_init( struct profidp_cfg * cfgs, int n );
int profidp_tx_input( int dev_index, u8 * data );
int profidp_tx_diag( int dev_index, u8 * data, int len );
int profidp_calculate_io_len( u8 * config, u8 len, u8 * input_len, u8 * output_len );

#endif
