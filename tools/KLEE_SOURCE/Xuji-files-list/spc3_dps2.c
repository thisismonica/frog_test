#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "spc3.h"
#include "spc3_dps2.h"
#include "nucleus.h"
#include "profidp.h"
#include "immap_83xx.h"
#include "cfg_8313xj.h"

#define spc3_lock_t                NU_SEMAPHORE
#define spc3_lock_init(lock,name)  (NU_Create_Semaphore(&(lock),name,1,NU_PRIORITY) == NU_SUCCESS ? 1 : 0)
#define spc3_lock(lock)            NU_Obtain_Semaphore(&(lock),NU_SUSPEND)
#define spc3_unlock(lock)          NU_Release_Semaphore(&(lock))
#define spc3_delay(n)              NU_Sleep(n);

struct spc3_plat_driver
{
    struct profidp_cfg cfg;
    struct spc3_dev dev;

    u8 output_buf[PROFIDP_MAX_OUTPUT_LEN];
    u8 config[PROFIDP_MAX_CONF_LEN]; 
    u8 config_len;
    u8 mintsdr;

    spc3_lock_t tx_input_lock;
    spc3_lock_t tx_diag_lock;
    u16 irq_mask;
};

struct spc3_board_data{
    u32 irq_mask;
    u32 base_addr;
};

static const struct spc3_board_data spc3_bd[SPC3_MAX] = {
    {   SPC3_0_IRQ_MASK, SPC3_0_BASE}, /*spc3_0*/
    {   SPC3_1_IRQ_MASK, SPC3_1_BASE}  /*spc3_1*/
};

struct spc3_plat_driver * spc3_drivers[SPC3_MAX];

static int cfg_check( struct profidp_cfg * cfg )
{
    if( (! cfg->input_data_len ) && (! cfg->output_data_len ) )
        return 0;

    if( ( cfg->input_data_len > 244 ) || ( cfg->output_data_len > 244 ) )
        return 0;

    if( ( cfg->input_data_len + cfg->output_data_len ) > 488 )
        return 0;

    /* Slave address should be a number in the range 1-125
       Address 127 is used for broadcast messages.
       and 126 is used for class2 master to set slave address */
    if ( cfg->station_address > 126 )
        return 0;
    else if( ( cfg->station_address == 126 ) && ( cfg->addr_change_cb == 0 ) )
        return 0;

    if( ! cfg->ident_number )
        return 0;

    return 1;
}

static void prepare_config( struct spc3_plat_driver * pdrv )
{
    u32 len;
    u8 config_byte_mask;
    u8 * config = pdrv->config;
    struct profidp_cfg * cfg = & pdrv->cfg;

    pdrv->config_len = 0;

    if( cfg->io_is_consistence )
        config_byte_mask = 0x80;
    else
        config_byte_mask = 0;

    if( cfg->io_is_word_size )
        config_byte_mask |= 0x40;

    len = cfg->input_data_len;
    for(; len >= 16; len -= 16 )
        config[pdrv->config_len ++ ] = 0x1F | config_byte_mask;
    if( len )
        config[pdrv->config_len ++ ] = 0x10 | ( len - 1 ) | config_byte_mask;

    len = cfg->output_data_len;
    for(; len >= 16; len -= 16 )
        config[pdrv->config_len ++ ] = 0x2F | config_byte_mask;
    if ( len )
        config[pdrv->config_len ++ ] = 0x20 | ( len - 1 ) | config_byte_mask;
}

#define  SPC3_DUMP_REGS  0

#if SPC3_DUMP_REGS  
#define SPC3_DUMP_REG_MAX   0x40
static u8 spc3_dumped_regs[SPC3_DUMP_REG_MAX];
static void spc3_dump_registers( struct spc3_dev * dev )
{
    u8 i;
    u32 base = dev->base_addr;
    memset( spc3_dumped_regs, 0, sizeof( spc3_dumped_regs ));
    for( i = 0; i < SPC3_DUMP_REG_MAX; i ++)
        spc3_dumped_regs[i] = REG_READ_UBYTE( base, i );
}
#endif

static setup_spc3( struct spc3_plat_driver * pdrv )
{
    struct profidp_cfg * cfg = & pdrv->cfg;
    struct spc3_dev * dev = & pdrv->dev;
    u32 base = dev->base_addr;
    SPC3_BUFINIT spc3_buf;
    SPC3_IO_DATA_LEN user_io_data_len;
    int i;

    SPC3_GO_OFFLINE( base );

    /*clear buffer*/
    for( i = 0x16; i < SPC3_SPACE_SIZE; i ++)
        REG_WRITE_UBYTE( base, i, 0 );

    SPC3_SET_IDENT_NUMBER_HIGH( base, cfg->ident_number >> 8 );
    SPC3_SET_IDENT_NUMBER_LOW( base, cfg->ident_number );
    SPC3_SET_STATION_ADDRESS( base, cfg->station_address ); /* Set the station address*/
    SPC3_SET_HW_MODE( base, SYNC_SUPPORTED | FREEZE_SUPPORTED | INT_POL_LOW | EOI_TIMEBASE_1m | USER_TIMEBASE_10m ); /* Set mode register 0*/
    SPC3_SET_IND( base,    /*Interrupt indication*/
        GO_LEAVE_DATA_EX      | /* Report enter or out data_ex state */
        BAUDRATE_DETECT       | /* Report baudrate detected */
        WD_DP_MODE_TIMEOUT    | /* Report watchdog timeout */
        USER_TIMER_CLOCK      | /* Report timer timeout */
        NEW_GC_COMMAND        | /* Report global command */
        NEW_CFG_DATA          | /* Report configuration received from master */
        NEW_PRM_DATA          | /* Report parameters received from master */
        DIAG_BUFFER_CHANGED   | /* Diagnostic-Buffer changed; SPC3 */
        DX_OUT                  /* Report output receveid from mater */
        );

    SPC3_SET_USER_WD_VALUE( base, 20000 ); /* Set user watchdog value*/

    /*spc3 buffer init*/
    spc3_buf.cfg_buf_len = PROFIDP_MAX_CONF_LEN;
    spc3_buf.diag_buf_len = PROFIDP_MAX_DIAG_DATA_LEN;
    spc3_buf.din_dout_buf_len = PROFIDP_MAX_DATA_LEN;;
    spc3_buf.prm_buf_len = PROFIDP_MAX_PARAM_LEN;
    if( (! cfg->addr_change_cb ) || (! PROFIDP_MAX_SSA_LEN ) )
    {
        spc3_buf.ssa_buf_len = 0;
        SPC3_SET_ADD_CHG_DISABLE( base );
    }
    else
    {
        spc3_buf.ssa_buf_len = PROFIDP_MAX_SSA_LEN;
        SPC3_SET_ADD_CHG_ENABLE( base );
    }

    if( spc3_buf_init( dev, & spc3_buf, 0 ) != SPC3_INIT_OK )
        return 0;

    SPC3_SET_READ_CFG_LEN( base, pdrv->config_len );
    spc3_buf_write( dev, SPC3_GET_READ_CFG_BUF_PTR( base ), pdrv->config, pdrv->config_len );

    SPC3_SET_BAUD_CNTRL( base, 44 ); /*set master watchdog value 44 * 44 * 10ms = 19s*/

    user_io_data_len.inp_data_len = cfg->input_data_len;
    user_io_data_len.outp_data_len = cfg->output_data_len;
    spc3_set_io_data_len( dev, & user_io_data_len );

    pdrv->mintsdr = cfg->mintsdr;

    dev->input_ptr = spc3_get_din_buf_ptr( base );
    dev->diag_ptr = spc3_get_diag_buf_ptr( base );
    dev->output_ptr = 0;
    dev->dp_state = SPC3_GET_DP_STATE( base );

#if SPC3_DUMP_REGS
    spc3_dump_registers( dev );
#endif

    SPC3_START( base );

    return 1;
}

static void new_parameters( struct spc3_plat_driver * pdrv )
{
    struct profidp_cfg * cfg = & pdrv->cfg;
    struct spc3_dev * dev = & pdrv->dev;
    u8 param_len, prm_result, parm_buf[PROFIDP_MAX_PARAM_LEN];
    u32 base = dev->base_addr;
    u32 new_prm_ptr;

    do
    {
        param_len = SPC3_GET_PRM_LEN( base );
        if( param_len   &&  cfg->param_cb )
        {
            new_prm_ptr = SPC3_GET_PRM_BUF_PTR( base );
            spc3_buf_read( dev, parm_buf, new_prm_ptr, param_len );
            if( ( param_len >= FIXED_PARM_SIZE ) &&  cfg->param_cb ( parm_buf + FIXED_PARM_SIZE, param_len - FIXED_PARM_SIZE, cfg->arg ) )
                prm_result = SPC3_SET_PRM_DATA_OK( base );
            else
                prm_result = SPC3_SET_PRM_DATA_NOT_OK( base );
        }
        else
            prm_result = SPC3_SET_PRM_DATA_OK( base );
    } while ( prm_result == SPC3_PRM_CONFLICT );

    pdrv->mintsdr = parm_buf[3]; 
}

static void allow_configure_change( struct spc3_plat_driver * pdrv )
{
    u8 new_cfg_len, cfg_result, new_cfg_buf[PROFIDP_MAX_CONF_LEN];
    SPC3_IO_DATA_LEN new_io_data_len;
    struct profidp_cfg * cfg = & pdrv->cfg;
    struct spc3_dev * dev = & pdrv->dev;
    u32 base = dev->base_addr;
    u32 new_cfg_ptr;

    enum
    {
        NO_CONFIG_CHANGE,
            CONFIG_CHANGE,
            CONFIG_FAULT
    } status;

    do /* Loop while no conflict exists */
    {
        new_cfg_ptr = SPC3_GET_CFG_BUF_PTR( base );
        new_cfg_len = SPC3_GET_CFG_LEN( base );
        spc3_buf_read( dev, new_cfg_buf, new_cfg_ptr, new_cfg_len );

        if ( new_cfg_len > PROFIDP_MAX_CONF_LEN )
            status = CONFIG_FAULT;
        else
        {
            if ( new_cfg_len != pdrv->config_len )
                status = CONFIG_CHANGE;
            else
            {
                if( memcmp( new_cfg_buf, pdrv->config, new_cfg_len ) != 0 )
                    status = CONFIG_CHANGE;
                else
                    status = NO_CONFIG_CHANGE;
            }

            if ( status == CONFIG_CHANGE )
            {
                if ( spc3_calculate_inp_outp_len( dev, new_cfg_buf, new_cfg_len, & new_io_data_len ) )
                {
                    if ( cfg->config_cb( new_cfg_buf, new_cfg_len, cfg->arg ))
                        spc3_set_io_data_len( dev, & new_io_data_len );
                    else
                        status = CONFIG_FAULT;
                }
                else
                    status = CONFIG_FAULT;
            }
        }

        switch ( status )
        {
        case NO_CONFIG_CHANGE:
            cfg_result = spc3_cfg_data_ok( base );
            break;
        case CONFIG_FAULT:
            cfg_result = spc3_cfg_data_not_ok( base );
            break;
        default:
            cfg_result = spc3_cfg_data_update( base );
            pdrv->config_len = new_cfg_len;
            cfg->input_data_len = new_io_data_len.inp_data_len;
            cfg->output_data_len = new_io_data_len.outp_data_len;
            memcpy( pdrv->config,  new_cfg_buf, new_cfg_len );
            break;
        }
    } while ( cfg_result == SPC3_CFG_CONFLICT );
}

static void disallow_configure_change( struct spc3_plat_driver * pdrv )
{
    u8 ok, new_cfg_len, cfg_result, new_cfg_buf[PROFIDP_MAX_CONF_LEN];
    struct profidp_cfg * cfg = & pdrv->cfg;
    struct spc3_dev * dev = & pdrv->dev;
    u32 base = dev->base_addr;
    u32 new_cfg_ptr;

    do
    {
        new_cfg_ptr = SPC3_GET_CFG_BUF_PTR( base ); 
        new_cfg_len = SPC3_GET_CFG_LEN( base );
        spc3_buf_read( dev, new_cfg_buf, new_cfg_ptr, new_cfg_len );

        ok = 0;
        if ( new_cfg_len == pdrv->config_len )
        {
            if( memcmp( new_cfg_buf, pdrv->config, new_cfg_len ) == 0 )
                ok = 1;
        }

        if ( ok )
            cfg_result = spc3_cfg_data_ok( base );
        else
            cfg_result = spc3_cfg_data_not_ok( base );
    } while ( cfg_result == SPC3_CFG_CONFLICT );
}

static void spc3_events_handle( int index )
{
    struct spc3_plat_driver * pdrv = spc3_drivers[index];
    struct spc3_dev * dev = & pdrv->dev;
    struct profidp_cfg * cfg = & pdrv->cfg;
    u32 base = dev->base_addr;
    u16 indications;

    indications = SPC3_GET_INDICATION( base );
    if(! indications )
        return;

    if( indications & BAUDRATE_DETECT )
    {
        if( ( dev->dp_state  == SPC3_DP_STATE_WAIT_CFG ) || ( dev->dp_state  == SPC3_DP_STATE_DATA_EX ))
            SPC3_SET_MINTSDR( base, pdrv->mintsdr );
    }

    if( indications & USER_TIMER_CLOCK )
        SPC3_RESET_USER_WD( base );

    if( indications & DIAG_BUFFER_CHANGED )
        dev->diag_ptr = spc3_get_diag_buf_ptr( base );

    if( indications & GO_LEAVE_DATA_EX )
    {
        dev->dp_state = SPC3_GET_DP_STATE( base );
        if ( cfg->state_change_cb )
            cfg->state_change_cb ( dev->dp_state, cfg->arg );
    }

    if( indications & WD_DP_MODE_TIMEOUT )
        spc3dps2_tx_diag( index, 0, 0 );

    if( indications & NEW_GC_COMMAND )
    {
        if( cfg->gc_cb )
            cfg->gc_cb ( SPC3_GET_GC_COMMAND(base), cfg->arg );
    }

    if( indications & NEW_PRM_DATA )
    {
        new_parameters( pdrv );
        dev->dp_state = SPC3_GET_DP_STATE( base );
    }

    if( indications & NEW_CFG_DATA )
    {
        if ( cfg->config_cb )
            allow_configure_change( pdrv );
        else
            disallow_configure_change( pdrv );

        dev->dp_state = SPC3_GET_DP_STATE( base );
        dev->input_ptr = SPC3_INPUT_UPDATE( base );
    }

    if( indications & DX_OUT )
    {
        dev->output_ptr = SPC3_OUTPUT_UPDATE( base );
        if( dev->dp_state ==  SPC3_DP_STATE_DATA_EX )
        {
            if ( cfg->output_cb )
            {
                spc3_buf_read( dev, pdrv->output_buf, dev->output_ptr, cfg->output_data_len );
                cfg->output_cb ( pdrv->output_buf, cfg->arg );
            }
        }
    }

    indications &= ~( NEW_PRM_DATA | NEW_CFG_DATA );
    SPC3_IND_CONFIRM( base, indications );
    SPC3_SET_EOI( base );
}

static NU_TASK tskSPC3CB;
#define SPC3_TASK_PRI           8
#define SPC3_TASK_STACK_SIZE    0x4000

static NU_HISR hisrSPC3CB;
#define SPC3_HISR_PRI           2 
#define SPC3_HISR_STACK_SIZE    0x1000

static NU_SEMAPHORE semSPC3Tsk;

static void spc3_task_routine( UNSIGNED argc, VOID * argv )
{
    volatile u16 * const irq_mask_reg = ( u16 *)SPC3_IRQ_MASK_REG_ADDR;
    volatile u16 * const irq_status_reg = ( u16 *)SPC3_IRQ_STATUS_REG_ADDR;
    u16 mask, irqs, i;  
    struct spc3_plat_driver * pdrv;

    while( 1 )
    {
        NU_Obtain_Semaphore(& semSPC3Tsk, NU_SUSPEND );

        mask = * irq_mask_reg;
        irqs = (* irq_status_reg ) & mask;
        * irq_mask_reg = 0;
        * irq_status_reg = irqs;

        for( i = 0; i < SPC3_MAX; i ++)
        {
            pdrv = spc3_drivers[i];         
            if( pdrv && ( irqs & pdrv->irq_mask ))
                spc3_events_handle( i );
        }

        * irq_mask_reg = mask;
#ifndef WIN32
        ESAL_GE_INT_Enable( SPC3_IRQ_NUM, ESAL_TRIG_FALLING_EDGE, 0 );
#endif
    }
}

static void spc3_hisr_routine(void)
{
    NU_Release_Semaphore( & semSPC3Tsk );
}

static void spc3_lisr_routine( int irq )
{
#ifndef WIN32
    volatile immap_t * im = ( volatile immap_t *)CFG_IMMR;
    volatile ipic83xx_t * ipic;
    ipic = & im->ipic;
    ipic->sepnr = 0x08000000; 

    ESAL_GE_INT_Disable( irq );
    NU_Activate_HISR( & hisrSPC3CB ); 
#endif 
}

static int init_driver_service()
{
    char * ptr;
#ifndef WIN32
    VOID (* old_lisr )( INT );
#endif

    if( NU_Create_Semaphore(& semSPC3Tsk, "semspc3", 0, NU_PRIORITY ) != NU_SUCCESS )
        return 0;

    ptr = malloc( SPC3_HISR_STACK_SIZE );
    if(! ptr )
        return 0;
    if( NU_Create_HISR(& hisrSPC3CB, "hisrspc3", spc3_hisr_routine, SPC3_HISR_PRI,
        ptr, SPC3_HISR_STACK_SIZE ) != NU_SUCCESS )
        return 0;
#ifndef WIN32
    if( NU_Register_LISR( SPC3_IRQ_NUM, spc3_lisr_routine,& old_lisr ) != NU_SUCCESS )
        return 0;
#endif
    ptr = malloc( SPC3_TASK_STACK_SIZE );
    if(! ptr )
        return 0;
    if( NU_Create_Task(& tskSPC3CB, "tskspc3", spc3_task_routine, 0, NU_NULL, 
        ptr, SPC3_TASK_STACK_SIZE, SPC3_TASK_PRI, 1, NU_PREEMPT, NU_START ) != NU_SUCCESS ) 
        return 0;

    return 1;
}

static struct spc3_plat_driver * create_driver( int index, struct profidp_cfg * cfg )
{
    struct spc3_dev * dev;
    struct spc3_plat_driver * pdrv = malloc( sizeof( struct spc3_plat_driver ));

    if( pdrv )
    {
        memset( pdrv, 0, sizeof( struct spc3_plat_driver ));
        memcpy(& pdrv->cfg, cfg, sizeof( struct profidp_cfg ));
        pdrv->irq_mask = spc3_bd[index].irq_mask;
        dev = & pdrv->dev;
        dev->base_addr = spc3_bd[index].base_addr;
        dev->total_buf_len = 0;

        if( spc3_lock_init( pdrv->tx_input_lock, "spc3in" ) && spc3_lock_init( pdrv->tx_diag_lock, "spc3diag" ) )
            return pdrv;
    }

    return 0;
}

int spc3dps2_init( struct profidp_cfg * cfgs, int n )
{
    int i, dev_index;
    struct spc3_plat_driver * pdrv;
    struct profidp_cfg * cfg;
    volatile u16 * const irq_mask_reg = ( u16 *)SPC3_IRQ_MASK_REG_ADDR;
    volatile u16 * const irq_status_reg = ( u16 *)SPC3_IRQ_STATUS_REG_ADDR;

    if(! init_driver_service())
        return 0;

    * irq_mask_reg = 0;
    * irq_status_reg = 0xffff;
    memset(& spc3_drivers, 0, sizeof( spc3_drivers ));

    for( i = 0, cfg = cfgs; i < n; i ++, cfg ++)
    {
        dev_index = cfg->dev_index;

        if( dev_index >= SPC3_MAX )
            continue;
        if( spc3_drivers[dev_index] )
            continue;

        if( cfg_check( cfg ))
        {
            pdrv = create_driver( dev_index, cfg );
            if( pdrv )
            {
                prepare_config( pdrv );
                * irq_mask_reg |= pdrv->irq_mask;
                setup_spc3( pdrv );
                spc3_drivers[dev_index] = pdrv;
            }
        }
    }
#ifndef WIN32
    ESAL_GE_INT_Enable( SPC3_IRQ_NUM, ESAL_TRIG_FALLING_EDGE, 0 );
#endif
    return 1;
}

int spc3dps2_tx_input( int index, u8 * data )
{
    struct spc3_plat_driver * pdrv = spc3_drivers[index];
    struct spc3_dev * dev = & pdrv->dev;

    if( pdrv && dev /*&& (dev->dp_state == SPC3_DP_STATE_DATA_EX)*/ && dev->input_ptr && data  )
    {
        spc3_lock( pdrv->tx_input_lock );
        spc3_buf_write( dev, dev->input_ptr, data, pdrv->cfg.input_data_len );
        dev->input_ptr = SPC3_INPUT_UPDATE( dev->base_addr );
        spc3_unlock( pdrv->tx_input_lock );
        return 1;
    }
    else
        return 0;
}

int spc3dps2_tx_diag( int index, u8 * data, int len )
{
    struct spc3_plat_driver * pdrv = spc3_drivers[index];
    struct spc3_dev * dev = & pdrv->dev;
    u32 base = dev->base_addr;

    if( ( ! dev->diag_ptr ) || (! pdrv ) || (! dev )
        || ( len > pdrv->cfg.diag_buf_len ) 
        || ( len && (! data ) ) )
        return 0;

    spc3_lock( pdrv->tx_diag_lock );
    if( len )
    {
        spc3_set_diag_state( base, SPC3_EXT_DIAG );
        spc3_buf_write( dev, dev->diag_ptr + FIXED_DIAG_SIZE, data, len );
    }
    else
        spc3_set_diag_state( base, 0 );

    spc3_set_diag_len( base, len + FIXED_DIAG_SIZE );
    spc3_diag_update( base );
    dev->diag_ptr = 0;

    spc3_unlock( pdrv->tx_diag_lock );
    return 1;
}
