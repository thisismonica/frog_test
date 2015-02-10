#include "profidp.h"
#include "spc3_dps2.h"

int profidp_calculate_io_len( u8 * config, u8 len, u8 * input_len, u8 * output_len )
{
    u8 temp_inp_data_len;
    u8 temp_outp_data_len;
    u8 length;
    u8 count;
    u8 specific_data_length;

    if((! config ) || (! len ) || (! input_len ) || (! output_len ) )
        return 0;

    temp_inp_data_len  = 0;
    temp_outp_data_len = 0;

    for ( ; len > 0; len -= count )
    {
        count = 0;
        if (* config & CFG_IS_BYTE_FORMAT )
        {
            count ++;

            /* cfg_ptr points to ID-byte, CFG_BF means "CFG_IS_BYTE_FORMAT" */
            length = ( u8 )( (* config & CFG_BF_LENGTH ) + 1 );

            if (* config & CFG_LENGTH_IS_WORD_FORMAT )
                length *= 2;
            if (* config & CFG_BF_OUTP_EXIST )
                temp_outp_data_len = temp_outp_data_len + length;
            if (* config & CFG_BF_INP_EXIST )
                temp_inp_data_len = temp_inp_data_len + length;
            config ++;
        }
        else
        {
            /* cfg_ptr points to the headerbyte of special ID-format */
            /* CFG_SF means "CFG_IS_SPECIAL_FORMAT" */
            if (* config & CFG_SF_OUTP_EXIST )
            {
                count ++;                /* next byte contains the length of outp_data */
                length = ( u8 )((*( config + count ) & CFG_SF_LENGTH ) + 1 );

                if (*( config + count ) & CFG_LENGTH_IS_WORD_FORMAT )
                    temp_outp_data_len = temp_outp_data_len + ( u8 )( 2 * length );
                else
                    temp_outp_data_len = temp_outp_data_len + length;
            }
            if (* config & CFG_SF_INP_EXIST )
            {
                count ++;                /* next byte contains the length of inp_data */
                length = ( u8 )((*( config + count ) & CFG_SF_LENGTH ) + 1 );

                if (*( config + count ) & CFG_LENGTH_IS_WORD_FORMAT )
                    temp_inp_data_len = temp_inp_data_len + ( u8 )( 2 * length );
                else
                    temp_inp_data_len = temp_inp_data_len + length;
            }
            specific_data_length = ( u8 )(* config & CFG_BF_LENGTH );

            if ( specific_data_length != 15 )
            {
                count = ( u8 )( count + 1 + specific_data_length );
                config = config + count;
            }
            else
                return 0;
        }
    }

    * input_len = temp_inp_data_len;
    * output_len = temp_outp_data_len;
    return 1;
}

int profidp_init( struct profidp_cfg * cfgs, int n )
{
    return spc3dps2_init( cfgs, n );
}

int profidp_tx_input( int dev_index, u8 * data )
{
    return spc3dps2_tx_input( dev_index, data );
}

int profidp_tx_diag( int dev_index, u8 * data, int len )
{
    return spc3dps2_tx_diag( dev_index, data, len );
}
