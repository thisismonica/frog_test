#include <string.h>
#include "spc3.h"
#include "profidp.h"

u8 spc3_cfg_data_ok( u32 base )
{
    REG_WRITE_UBYTE( base, REG_MODE_REG1_R, EN_CHG_CFG_BUFFER );
    return REG_READ_UBYTE( base, REG_USER_CFG_DATA_OK );
}

u8 spc3_cfg_data_update( u32 base )
{
    REG_WRITE_UBYTE( base, REG_MODE_REG1_S, EN_CHG_CFG_BUFFER );
    REG_WRITE_UBYTE( base, REG_R_LEN_READ_CFG_BUF, REG_READ_UBYTE( base, REG_R_LEN_CFG_BUF ));
    return REG_READ_UBYTE( base, REG_USER_CFG_DATA_OK );
}

u8 spc3_cfg_data_not_ok( u32 base )
{
    REG_WRITE_UBYTE( base, REG_MODE_REG1_R, EN_CHG_CFG_BUFFER );
    return REG_READ_UBYTE( base, REG_USER_CFG_DATA_NOK );
}

u32 spc3_diag_update( u32 base )
{
    u8 temp = REG_READ_UBYTE( base, REG_NEW_DIAG_BUFFER_CMD ) & 0x03;

    switch( temp )
    {
    case 1:   /* buffer with Index 0 */
        return ( REG_READ_UBYTE( base, REG_R_DIAG_BUF_PTR ) << 3 );
    case 2:   /* buffer with Index 1 */
        return ( REG_READ_UBYTE( base, REG_R_DIAG_BUF_PTR + 1 ) << 3 );
    default:  /* no buffer */
        return 0;
    }
}

u8 spc3_set_io_data_len( struct spc3_dev * dev,  SPC3_IO_DATA_LEN  * len_ptr )
{
    u32 base = dev->base_addr;
    u8 ri_len, ro_len;   /* real lens */
    u8 ri_off;           /* input-buffer-offset */
    u8 i;

    if( ( len_ptr->outp_data_len > 244 ) || ( len_ptr->inp_data_len > 244 ) )
        return 0;
    if(( u16 )( len_ptr->inp_data_len + len_ptr->outp_data_len ) > dev->buf_cfg.din_dout_buf_len )
        return 0;

    REG_WRITE_UBYTE( base, REG_R_LEN_DOUT_BUF, len_ptr->outp_data_len );
    REG_WRITE_UBYTE( base, REG_R_LEN_DIN_BUF, len_ptr->inp_data_len );

    ro_len = (( len_ptr->outp_data_len ) + 7 ) >> 3;
    ri_len = (( len_ptr->inp_data_len ) + 7 ) >> 3;

    for( i = 0; i < SPC3_DOUT_BUF_NUM ; i ++)
        REG_WRITE_UBYTE( base, REG_R_DOUT_BUF_PTR + i, SPC3_BUF_START + ro_len * i );

    ri_off = SPC3_BUF_START + ro_len * SPC3_DOUT_BUF_NUM;
    for( i = 0; i < SPC3_DIN_BUF_NUM; i ++)
        REG_WRITE_UBYTE( base, REG_R_DIN_BUF_PTR + i, ri_off + ri_len * i );

    return 1;
}

u32 spc3_output_update_state( u32 base, u8 * state_ptr )
{
    u8 tmp = REG_READ_UBYTE( base, REG_NEXT_DOUT_BUF_CMD );
    * state_ptr = tmp & ( NEW_DOUT_BUF | DOUT_BUF_CLEARED );
    return ( REG_READ_UBYTE( base, REG_R_DOUT_BUF_PTR + ( tmp & 0x03 )- 1 ) << 3 );
}

u32 spc3_get_din_buf_ptr( u32 base )
{
    u8  uwIndex = ( REG_READ_UBYTE( base, REG_DIN_BUFFER_SM ) >> 4 ) & 0x03;
    if( uwIndex )
        return ( REG_READ_UBYTE( base, REG_R_DIN_BUF_PTR + uwIndex - 1 ) << 3 );
    else
        return 0;
}

u32 spc3_get_free_mem( struct spc3_dev * dev, int * buf_len )
{
    if( dev->total_buf_len )
    {
        * buf_len = SPC3_USER_SPACE_SIZE - dev->total_buf_len;
        return SPC3_USER_SPACE_SIZE + dev->total_buf_len;
    }
    else
    {
        * buf_len = 0;
        return 0;
    }
}

u32 spc3_get_diag_buf_ptr( u32 base )
{
    u8 sm = REG_READ_UBYTE( base, REG_DIAG_BUFFER_SM );
    if(( sm & 0x03 ) == 0x01 )
        return ( REG_READ_UBYTE( base, REG_R_DIAG_BUF_PTR ) << 3 );
    else if( ( sm & 0x0c ) == 0x04 )
        return ( REG_READ_UBYTE( base, REG_R_DIAG_BUF_PTR + 1 ) << 3 );
    else
        return 0;
}

u8 spc3_set_diag_len( u32 base, u8 len )
{
    u8 sm = REG_READ_UBYTE( base, REG_DIAG_BUFFER_SM );
    if( ( sm & 0x03 ) == 0x01 )
        return( REG_WRITE_UBYTE( base, REG_R_LEN_DIAG_BUF, len ));
    else if( ( sm & 0x0c ) == 0x04 )
        return( REG_WRITE_UBYTE( base, REG_R_LEN_DIAG_BUF + 1, len ));
    else
        return 0xff;
}

u8 spc3_set_diag_state( u32 base, u8 state )
{
    u32 offset = 0;
    u8 sm = REG_READ_UBYTE( base, REG_DIAG_BUFFER_SM );

    if( ( sm & 0x03 ) == 0x01 )
        offset = REG_READ_UBYTE( base, REG_R_DIAG_BUF_PTR );
    else if( ( sm & 0x0c ) == 0x04 )
        offset = REG_READ_UBYTE( base, REG_R_DIAG_BUF_PTR + 1 );
    if( offset )
    {
        offset <<= 3;
        REG_WRITE_UBYTE( base, offset, state );
        return REG_READ_UBYTE( base, offset );
    }
    return ~ state;
}

/*
  +------------------------------------------------------------------------+
  | Function:   a s s i g n _ a u x _ b u f ( )                            |
  +------------------------------------------------------------------------+
  | Description:                                                           |
  |  Based on a list of bufferlens the best len of the aux-buffers is      |
  |  calculated. The list's lens will be adjusted, the assignment-list     |
  |  will be calculated.                                                   |
  +------------------------------------------------------------------------+
  | Parameters:                                                            |
  | -lens:    Pointer to a list of buffer-lens                             |
  |           The calculated lens are inserted in this list.               |
  | -count:   length of list (2 to ASS_AUX_BUF)                            |
  | -assign:  pointer to an assignment-byte                                |
  |           For each len there is a corresponding bit for the            |
  |           assignment to AUX-buffer 1 or 2. 0 means AUX-buffer 1,       |
  |           1 means AUX-Buffer 2. Bit 0 ist for the first, Bit 7         |
  |           for the 8th length.                                          |
  | -aux_len: pointer to a 2-byte-array for AUX-buffer-lens                |
  |           The calculated lens of AUX-buffer 1 and 2 are stored here.   |
  |                                                                        |
  | Returnvalue: UWORD: total amount of used bytes (0 = error)             |
  +------------------------------------------------------------------------+
 */
static int assign_aux_buf( u8 * lens, u8 count, u8 * assign, int * aux_len )
{
    u8 pos[ASS_AUX_BUF];     /* storage for former position of lens    */
    u8 lensx[ASS_AUX_BUF];   /* working-array for calculated lens      */
    u8 step;                 /* counter for done step                  */
    u8 lx;                   /* temp-var for lenarray-sorting          */
    u8 px;                   /* temp-var dor position-sorting          */
    int min_len = 0xffff;    /* calculated min-len                     */
    u8 min_step = 0;         /* step at reached min-len                */
    int i, j;

    if(( count < 2 ) || ( count > ASS_AUX_BUF ))
        return 0;

    /* init position-array */
    for( i = 0; i < count; i ++)
        pos[i] = i;

    /* init working-array */
    for( i = 0; i < count; i ++)
        lensx[i] = lens[i];

    /* round up lens to SPC3-lens (8-byte-granularity) */
    for( i = 0; i < count; i ++)
        lensx[i] = ( lensx[i] + 7 ) & 0xf8;

    /* sorting of lens: gratest len to index 0 */
    for( i = 0; i < count - 1; i ++)
    {
        for( j = i + 1; j < count; j ++)
        {
            if( lensx[i] < lensx[j] )
            {
                /* greater len found */
                lx = lensx[i];          /* xchg lens */
                lensx[i] = lensx[j];
                lensx[j] = lx;
                px = pos[i];            /* xchg position */
                pos[i] = pos[j];
                pos[j] = px;
            }
        }
    }
    /* remove NULL-lens of list */
    for( i = count - 1; i >= 0; i --)
    {
        if( lensx[i] == 0 )
            count --;
    }

    if( count == 0 )
        min_len = 0;     /* error: no lens specified */

    /* stepwise assignment to the AUX-buffers */
    for( step = 0; step < count; step ++)
    {
        /* determine total len for AUX-buffer 1 */
        aux_len[0] = 0;
        for( i = step; i < count; i ++)
        {
            if( aux_len[0] < lensx[i] )
                aux_len[0] = lensx[i];
        }
        aux_len[0] = aux_len[0] * ( count - step + 1 );

        /* determine total len for AUX-buffer 2 */
        aux_len[1] = 0;
        for( i = 0; i < step; i ++)
        {
            if( aux_len[1] < lensx[i] )
                aux_len[1] = lensx[i];
        }
        aux_len[1] = aux_len[1] * ( step + 1 );

        if(( aux_len[0] + aux_len[1] ) < min_len )
        {
            /* neue Minimal-Laenge gefunden */
            min_len = aux_len[0] + aux_len[1];
            min_step = step;
        }
    }

    /*  calculation of len for AUX-buffer 1 */
    aux_len[0] = 0;

    for( i = min_step; i < count; i ++)
    {
        if( aux_len[0] < lensx[i] )
            aux_len[0] = lensx[i];
    }
    /*  setup lens for AUX-buffer 1 */
    for( i = min_step; i < count; i ++)
        lens[pos[i]] = aux_len[0];

    /*  calculation of len for AUX-buffer 2 */
    aux_len[1] = 0;

    for( i = 0; i < min_step; i ++)
    {
        if( aux_len[1] < lensx[i] )
            aux_len[1] = lensx[i];
    }
    /*  setup lens for AUX-buffer 2 */
    /*  setup assignment-list */
    * assign = 0;    /* initial all buffers assigned to AUX-buffer 1 */

    for( i = 0; i < min_step; i ++)
    {
        lens[pos[i]] = aux_len[1];
        * assign |= 0x1 << pos[i];
    }

    return min_len;
}

#define RBL_PRM     0
#define RBL_CFG     1
#define RBL_SSA     2

enum SPC3_INIT_RET spc3_buf_init( struct spc3_dev * dev, SPC3_BUFINIT * spc3_bptr, u8 spec_prm )
{
    u32 base = dev->base_addr;
    u8 real_buf_len[ASS_AUX_BUF], buf_ptr;
    int aux_buf_len[2];         /* calculated lens of AUX-buffers */
    u8 aux_ass;                 /* var. for AUX-buffer-assignment */
    int r_din_dout_buf_len;     /* var. for real IO-data-lens */
    enum SPC3_INIT_RET ret = SPC3_INIT_OK;

    dev->total_buf_len = 0;

    if ( SPC3_GET_OFF_PASS( base )) /*END: SPC3 is not Offline*/
    return SPC3_INITF_NOFF;

    memcpy(& dev->buf_cfg, spc3_bptr, sizeof( SPC3_BUFINIT ));

    if( spc3_bptr->din_dout_buf_len > 488 )
    ret = SPC3_INITF_DIN_DOUT_LEN;
    if(( spc3_bptr->diag_buf_len < 6 ) || ( spc3_bptr->diag_buf_len > 244 ))
    ret = SPC3_INITF_DIAG_LEN;
    if(( spc3_bptr->prm_buf_len < 7 ) || ( spc3_bptr->prm_buf_len > 244 ))
    ret = SPC3_INITF_PRM_LEN;
    if(( spc3_bptr->cfg_buf_len < 1 ) || ( spc3_bptr->cfg_buf_len > 244 ))
    ret = SPC3_INITF_PRM_LEN;
    if( spc3_bptr->ssa_buf_len == 0 )
    SPC3_SET_ADD_CHG_DISABLE( base );
    else
    {
    /* SSA-buffer is used */
    if(( spc3_bptr->ssa_buf_len < 4 ) || ( spc3_bptr->ssa_buf_len > 244 ))
    ret = SPC3_INITF_SSA_LEN;
    }
    if( ret != SPC3_INIT_OK )
    {
    /* error occured, no memory used*/
    dev->total_buf_len = 0;
    return ret;
    }

    /* Bufferlens seem to be ok --> calculate total amount of memory */
    if( spec_prm )
    real_buf_len[RBL_PRM] = 0; /* Prm. via AUX */
    else
    real_buf_len[RBL_PRM] = spc3_bptr->prm_buf_len;

    real_buf_len[RBL_CFG] = spc3_bptr->cfg_buf_len;
    real_buf_len[RBL_SSA] = spc3_bptr->ssa_buf_len;
    dev->total_buf_len = assign_aux_buf( real_buf_len, sizeof( real_buf_len ),
    & aux_ass , aux_buf_len );
    dev->total_buf_len += real_buf_len[RBL_CFG]; /* wg. Read-Config */

    /* amount for IO-data, diagnosticbuffer and spec.Prm.buffer */
    r_din_dout_buf_len = ( spc3_bptr->din_dout_buf_len + 7 ) & 0xfff8;
    dev->total_buf_len += (( spc3_bptr->diag_buf_len + 7 ) & 0xf8 ) * 2;

    if( spec_prm )
    {
    /* spec_prm-buffer is used */
    real_buf_len[RBL_PRM] = ( spc3_bptr->prm_buf_len + 7 ) & 0xf8;
    dev->total_buf_len += real_buf_len[RBL_PRM];
    REG_WRITE_UBYTE( base, REG_R_LEN_SPEC_PRM_BUF, spc3_bptr->prm_buf_len );
    }
    else
    REG_WRITE_UBYTE( base, REG_R_LEN_SPEC_PRM_BUF, 0 );

    dev->total_buf_len += r_din_dout_buf_len * 3;

    if( dev->total_buf_len > SPC3_USER_SPACE_SIZE )
    { /* not enough memory */
    dev->total_buf_len = 0;
    return SPC3_INITF_LESS_MEM;
    }

    /* arm the pointers */
    REG_WRITE_UBYTE( base, REG_R_AUX_BUF_SEL, aux_ass );

    buf_ptr = SPC3_BUF_START + (( r_din_dout_buf_len * 3 ) >> 3 );
    REG_WRITE_UBYTE( base, REG_R_DIAG_BUF_PTR, buf_ptr );
    buf_ptr += (( spc3_bptr->diag_buf_len + 7 ) & 0xf8 ) >> 3 ;
    REG_WRITE_UBYTE( base, REG_R_DIAG_BUF_PTR + 1, buf_ptr );

    buf_ptr += (( spc3_bptr->diag_buf_len + 7 ) & 0xf8 ) >> 3  ;
    REG_WRITE_UBYTE( base, REG_R_CFG_BUF_PTR, buf_ptr );

    buf_ptr +=  real_buf_len[RBL_CFG] >> 3 ;
    REG_WRITE_UBYTE( base, REG_R_READ_CFG_BUF_PTR, buf_ptr );

    buf_ptr += real_buf_len[RBL_CFG] >> 3;
    REG_WRITE_UBYTE( base, REG_R_PRM_BUF_PTR, buf_ptr );

    buf_ptr += real_buf_len[RBL_PRM] >> 3;
    REG_WRITE_UBYTE( base, REG_R_AUX_BUF_PTR, buf_ptr );
    buf_ptr += aux_buf_len[0] >> 3;
    REG_WRITE_UBYTE( base, REG_R_AUX_BUF_PTR + 1, buf_ptr );

    if( real_buf_len[RBL_SSA] == 0 )/* SSA not supported */
    REG_WRITE_UBYTE( base, REG_R_SSA_BUF_PTR, 0 );
    else /* SSA supported */
    REG_WRITE_UBYTE( base, REG_R_SSA_BUF_PTR , buf_ptr + ( aux_buf_len[1] >> 3 ) );

    /* set buffer-lens */
    REG_WRITE_UBYTE( base, REG_R_LEN_DIAG_BUF, 6 );
    REG_WRITE_UBYTE( base, REG_R_LEN_DIAG_BUF + 1, 6 );
    REG_WRITE_UBYTE( base, REG_R_LEN_CFG_BUF, spc3_bptr->cfg_buf_len );
    REG_WRITE_UBYTE( base, REG_R_LEN_PRM_BUF, spc3_bptr->prm_buf_len );
    REG_WRITE_UBYTE( base, REG_R_LEN_CNTRL_BUF, aux_buf_len[0] );
    REG_WRITE_UBYTE( base, REG_R_LEN_CNTRL_BUF + 1, aux_buf_len[1] );
    REG_WRITE_UBYTE( base, REG_R_LEN_SSA_BUF, spc3_bptr->ssa_buf_len );
    REG_WRITE_UBYTE( base, REG_R_LEN_DIN_BUF, 0 );
    REG_WRITE_UBYTE( base, REG_R_LEN_DOUT_BUF, 0 );

    return ret;
}

int spc3_calculate_inp_outp_len ( struct spc3_dev * dev, u8 * cfg_ptr, u8 cfg_len, SPC3_IO_DATA_LEN * io_data_len )
{
    u8 i_len, o_len;
    u16 io_len;

    if( cfg_len && ( cfg_len <= dev->buf_cfg.cfg_buf_len ))
    {
        if( profidp_calculate_io_len( cfg_ptr, cfg_len, & i_len, & o_len ))
        {
            io_len = ( i_len + 7 ) & 0xfff8;
            io_len += ( o_len + 7 ) & 0xfff8;
            if ( io_len <= dev->buf_cfg.din_dout_buf_len )
            {
                io_data_len->inp_data_len = i_len;
                io_data_len->outp_data_len = o_len;
                return 1;
            }
        }
    }

    return 0;
}

void spc3_buf_read( struct spc3_dev * dev, u8 * buf, u32 offset, u32 len )
{
    u32 base = dev->base_addr;
    for(; len; len --)
    {
        * buf = REG_READ_UBYTE( base, offset );
        buf ++;
        offset ++;
    }
}

void spc3_buf_write( struct spc3_dev * dev, u32 offset, u8 * data, u32 len )
{
    u32 base = dev->base_addr;
    for(; len; len --)
    {
        REG_WRITE_UBYTE( base, offset, * data );
        data ++;
        offset ++;
    }
}
