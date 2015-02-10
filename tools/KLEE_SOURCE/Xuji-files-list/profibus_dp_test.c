#include <string.h>
#include <stdio.h>
#include "profidp.h"
#include "spc3_dps2.h"
#include "nucleus.h"

static int output_changed[SPC3_MAX] = {0};
static u8 output_data[SPC3_MAX][PROFIDP_MAX_OUTPUT_LEN] = {0};
static u8 dp_state[SPC3_MAX] = {0};      
static u8 input_len[SPC3_MAX] = {0};
static u8 output_len[SPC3_MAX] = {0};
static u8 diag_len[SPC3_MAX] = {0};

static void on_output( u8 * buffer, void * arg )
{      
    int index = ( int )arg;
    if( memcmp( output_data[index], buffer, output_len[index] ))
    {
        memcpy( output_data[index], buffer, output_len[index] );
        output_changed[index] = 1;      
    }
}

static void on_state_change( u8 new_state, void * arg )
{
    int index = ( int )arg;
    dp_state[index] = new_state; 
}

static int on_param( u8 * parm, u8 len, void * arg )
{
    return 1;
}

static int on_config( u8 * config, u8 len, void * arg )
{
    int index = ( int )arg;
    u8 i_len, o_len;
    if( profidp_calculate_io_len( config, len, & i_len, & o_len ))
    {
        input_len[index] = i_len;
        output_len[index] = o_len;
        return 1;
    }
    return 0;
}

static struct profidp_cfg dpcfg[SPC3_MAX] = {
    {
        0,                     /*dev index*/
            0x8138,                /*ident_number*/
            1,                     /*station_address*/
            0,                     /*io_is_consistence*/
            0,                     /*io_is_word_size*/
            8,                    /*input_data_len*/
            8,                    /*output_data_len*/
            32,                    /*diag_buf_len*/
            11,                    /*mintsdr*/

            on_output,
            on_param,
            on_config,
            NULL,                   
            on_state_change,
            NULL,
            ( void *)0
    },
    {
        1,                     /*dev index*/
            0x8138,                /*ident_number*/
            3,                     /*station_address*/
            0,                     /*io_is_consistence*/
            0,                     /*io_is_word_size*/
            8,                    /*input_data_len*/
            8,                    /*output_data_len*/
            32,                    /*diag_buf_len*/
            11,                    /*mintsdr*/

            on_output,
            on_param,
            on_config,
            NULL,                   
            on_state_change,
            NULL,
            ( void *)1
    }
};

#if 0
void dp_user_main();

void dp_user_main()
{    
    int i, dev_index;

    for( i = 0; i < SPC3_MAX; i ++)
    {
        dev_index = dpcfg[i].dev_index;
        input_len[dev_index] =  dpcfg[i].input_data_len; 
        output_len[dev_index] =  dpcfg[i].output_data_len; 
        diag_len[dev_index] = dpcfg[i].diag_buf_len; 
    }

    profidp_init( dpcfg, 2 );

    while( 1 )
    {
        NU_Sleep( 2 );      

        for( i = 0; i < SPC3_MAX; i ++)
        {
            dev_index = dpcfg[i].dev_index;
            if ( output_changed[dev_index] )
            {           
                if(! profidp_tx_input( dev_index, output_data[dev_index] ))
                    puts( "tx input failed." );

                if( ! profidp_tx_diag( dev_index, output_data[dev_index], output_len[dev_index] ) )
                    puts( "tx diag failed." );  
                output_changed[dev_index] = 0;
            }
        }
    }
}
#endif
