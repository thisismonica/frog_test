/*npu808 npi channel tsec driver by shenchen@xjgc.com,20101103*/

#include <string.h>
#include <stdlib.h>
#include "nucleus.h"
#include "tsec_npi_if.h"

static int tsec_if_transmint( struct eth_npi_if * pif, u8 * data, u32 length )
{
    tsec_if_t * ptsecif = ( tsec_if_t *)pif;
    ptsecif->last_tx_time = NU_Retrieve_Clock();
    return tsec_transmint( ptsecif, data, length );
}

static int tsec_tx_int_vector_id, tsec_rx_int_vector_id, tsec_err_int_vector_id;
static int init_int_vectors(int index)
{
	volatile immap_t * im = ( volatile immap_t *)CFG_IMMR;
    u32 cpu_revision = im->sysconf.spridr;

	if((cpu_revision & 0xfff00000) == 0x80c00000) /*mpc837x*/
	{
		if(index == 0)
		{
			tsec_tx_int_vector_id = 9;
			tsec_rx_int_vector_id = 10;
			tsec_err_int_vector_id = 11;
		}
		else
		{
			tsec_tx_int_vector_id = 12;
			tsec_rx_int_vector_id = 13;
			tsec_err_int_vector_id = 14;
		}
	}
	else if((cpu_revision & 0xfff00000) == 0x80b00000) /*mpc8313x*/
	{
		if( cpu_revision == 0x10 )  /*1.xx*/
		{
			if(index == 0)
			{
				tsec_tx_int_vector_id = 14;
				tsec_rx_int_vector_id = 13;
				tsec_err_int_vector_id = 12;
			}
			else
			{
				tsec_tx_int_vector_id = 11;
				tsec_rx_int_vector_id = 10;
				tsec_err_int_vector_id = 9;           
			}
		}
		else /*2.xx or other*/
		{       
			if(index == 0)
			{
				tsec_tx_int_vector_id = 9;
				tsec_rx_int_vector_id = 10;
				tsec_err_int_vector_id = 11;
			}
			else
			{
				tsec_tx_int_vector_id = 12;
				tsec_rx_int_vector_id = 13;
				tsec_err_int_vector_id = 14;      
			}
		}
	}
    return 0;
}

#define TSEC_EVENT_TIMEOUT     200    /*200ms*/
#define TSEC_TX_EVENT          0x10
#define TSEC_RX_EVENT          0x20
#define TSEC_ERR_EVENT         0x40
#define TSEC_EVENTS_MASK       (TSEC_TX_EVENT | TSEC_RX_EVENT | TSEC_ERR_EVENT)
static NU_EVENT_GROUP          tsec_events;

static int tsec_if_restart( tsec_if_t * ptsecif )
{
    int res;
    UNSIGNED events;

    ESAL_GE_INT_Disable( tsec_tx_int_vector_id );
    ESAL_GE_INT_Disable( tsec_rx_int_vector_id );
    ESAL_GE_INT_Disable( tsec_err_int_vector_id );
    res = tsec_initialize( ptsecif );
    NU_Retrieve_Events(& tsec_events, TSEC_EVENTS_MASK, NU_OR_CONSUME, & events, 0 );
    ESAL_GE_INT_Enable( tsec_tx_int_vector_id, ESAL_TRIG_RISING_EDGE  , 0 );
    ESAL_GE_INT_Enable( tsec_rx_int_vector_id, ESAL_TRIG_RISING_EDGE  , 0 );
    ESAL_GE_INT_Enable( tsec_err_int_vector_id, ESAL_TRIG_RISING_EDGE  , 0 );

    return res;
}

u32 tx_reset_now_time = 0;
u32 tx_reset_last_time = 0;
u32 rx_reset_now_time = 0;
u32 rx_reset_last_time = 0;

static void tsec_events_task( UNSIGNED argc, VOID * argv )
{
    UNSIGNED events;
    u32 rx_packets;
    tsec_if_t * ptsecif = ( tsec_if_t *)argv;
    while( 1 )
    {
        rx_packets = 0;
        if( NU_Retrieve_Events(& tsec_events, TSEC_EVENTS_MASK,
            NU_OR_CONSUME, & events, TSEC_EVENT_TIMEOUT ) == NU_SUCCESS )
        {
            if( events & TSEC_ERR_EVENT )
            {
                rx_packets += tsec_error_event_handle( ptsecif );
                ESAL_GE_INT_Enable( tsec_err_int_vector_id, ESAL_TRIG_RISING_EDGE  , 0 );
            }

            if( events & TSEC_TX_EVENT )
            {
                tsec_tx_event_handle( ptsecif );
                ESAL_GE_INT_Enable( tsec_tx_int_vector_id, ESAL_TRIG_RISING_EDGE  , 0 );
                ptsecif->last_tx_time = 0;
            }

            if( events & TSEC_RX_EVENT )
            {
                rx_packets += tsec_rx_event_handle( ptsecif );
                ESAL_GE_INT_Enable( tsec_rx_int_vector_id, ESAL_TRIG_RISING_EDGE  , 0 );
            }
        }

        if(! ptsecif->link )
        {
            tsec_link_update( ptsecif );
            if( ptsecif->link )
            {
                ptsecif->last_tx_time = 0;
                ptsecif->last_rx_time = NU_Retrieve_Clock();
            }
        }
        else
        {
            u32 tx_time = ptsecif->last_tx_time;
            u32 now_time = NU_Retrieve_Clock();
            int reset_flag = 0;

            if( rx_packets )
                ptsecif->last_rx_time = now_time;

            if( ( u32 )( now_time - ptsecif->last_rx_time ) > ETH_NPI_RX_TIMEOUT )
            {
                reset_flag ++;
                ptsecif->rx_timeout ++;
                rx_reset_now_time = now_time;;
                rx_reset_last_time = ptsecif->last_rx_time;
            }

            if( tx_time && ( ( u32 )( now_time - tx_time )  > ETH_NPI_TX_TIMEOUT ) )
            {
                reset_flag ++;
                ptsecif->tx_timeout ++;
                tx_reset_now_time = now_time;
                tx_reset_last_time = tx_time;
            }

            if( reset_flag )
            {
                tsec_if_restart( ptsecif );
                memset(& ptsecif->stats, 0, sizeof( ptsecif->stats ));
                memset(& ptsecif->extra_stats, 0 , sizeof( ptsecif->extra_stats ));
                ptsecif->reset_times ++;
            }
        }
    }
}

#define TSEC_HISR_STACK_SIZE 0x1000
#define TSEC_HISR_PRIORITY 2
static NU_HISR tsecTxHisr, tsecRxHisr, tsecErrHisr;

static void tsec_tx_hisr_routine()
{
    NU_Set_Events(& tsec_events, TSEC_TX_EVENT, NU_OR );
}

static void tsec_rx_hisr_routine()
{
    NU_Set_Events(& tsec_events, TSEC_RX_EVENT, NU_OR );
}

static void tsec_err_hisr_routine()
{
    NU_Set_Events(& tsec_events, TSEC_ERR_EVENT, NU_OR );
}

static void tsec_lisr_routine( int irq )
{
	ESAL_GE_INT_Disable( irq );
    if( irq == tsec_tx_int_vector_id )
        NU_Activate_HISR(& tsecTxHisr );
    else if( irq == tsec_rx_int_vector_id )
        NU_Activate_HISR(& tsecRxHisr );
    else if( irq == tsec_err_int_vector_id )
        NU_Activate_HISR(& tsecErrHisr );
}

#define TSEC_TASK_STACK_SIZE    0x2000
static NU_TASK                  TSEC_Task_CB;
static int tsec_if_initialize( struct eth_npi_if * pif, void *arg )
{
    VOID (* old_lisr )( INT );
    tsec_if_t * ptsecif = ( tsec_if_t *)pif;
	int index = (int)arg;
	char *stack;

    if( ( index < 2) && init_int_vectors(index) )
        return - 1;

    if( tsec_lock_init( ptsecif->tx_lock, "tsecnpi" ))
        return - 1;

	stack = malloc(TSEC_HISR_STACK_SIZE * 3);
	if(!stack)
		return -1;
    if( NU_Create_HISR(& tsecTxHisr, "ptsect", tsec_tx_hisr_routine, TSEC_HISR_PRIORITY,
        stack, TSEC_HISR_STACK_SIZE ) != NU_SUCCESS )
        return - 1;
    if( NU_Create_HISR(& tsecRxHisr, "ptsecr", tsec_rx_hisr_routine, TSEC_HISR_PRIORITY,
        stack + TSEC_HISR_STACK_SIZE, TSEC_HISR_STACK_SIZE ) != NU_SUCCESS )
        return - 1;
    if( NU_Create_HISR(& tsecErrHisr, "ptsece", tsec_err_hisr_routine, TSEC_HISR_PRIORITY,
        stack + TSEC_HISR_STACK_SIZE * 2, TSEC_HISR_STACK_SIZE ) != NU_SUCCESS )
        return - 1;

    if( NU_Register_LISR( tsec_tx_int_vector_id, tsec_lisr_routine,& old_lisr ) != NU_SUCCESS )
        return - 1;
    if( NU_Register_LISR( tsec_rx_int_vector_id, tsec_lisr_routine,& old_lisr ) != NU_SUCCESS )
        return - 1;
    if( NU_Register_LISR( tsec_err_int_vector_id, tsec_lisr_routine,& old_lisr ) != NU_SUCCESS )
        return - 1;

    if( NU_Create_Event_Group(& tsec_events, "ptsec" ) != NU_SUCCESS )
        return - 1;

	stack = malloc(TSEC_TASK_STACK_SIZE);
	if(!stack)
		return -1;
    if( NU_Create_Task(& TSEC_Task_CB, "ptsec", tsec_events_task, 1, ptsecif,
        stack, TSEC_TASK_STACK_SIZE, 3, 2, NU_PREEMPT, NU_START ) != NU_SUCCESS )
        return - 1;

	if(index == 1)
	{
		ptsecif->regs = (tsec_t *)(TSEC_BASE_ADDR + TSEC_SIZE);
		ptsecif->phyaddr = TSEC2_PHY_ADDR;
	}

    if( tsec_initialize( ptsecif ))
        return - 1;

    ptsecif->last_rx_time = NU_Retrieve_Clock();
    ptsecif->last_tx_time = 0;
    ptsecif->reset_times = 0;
    memset(& ptsecif->stats, 0, sizeof( ptsecif->stats ));
    memset(& ptsecif->extra_stats, 0 , sizeof( ptsecif->extra_stats ));

    ESAL_GE_INT_Enable( tsec_tx_int_vector_id, ESAL_TRIG_RISING_EDGE  , 0 );
    ESAL_GE_INT_Enable( tsec_rx_int_vector_id, ESAL_TRIG_RISING_EDGE  , 0 );
    ESAL_GE_INT_Enable( tsec_err_int_vector_id, ESAL_TRIG_RISING_EDGE  , 0 );

    return 0;
}

tsec_if_t tsec_npi_if =
{
    {
    tsec_if_initialize,
    tsec_if_transmint,
    {   0x00, 0x6e, 0x70, 0x75, 0x38, 0x38},
    },
    ( tsec_t *)( TSEC_BASE_ADDR ),
    ( tsec_t *)( TSEC_BASE_ADDR ),
    TSEC1_PHY_ADDR,
};

