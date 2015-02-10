/*npu808 npi channel tsec driver by shenchen@xjgc.com,20101103*/

#ifndef _TSEC_IF_H
#define _TSEC_IF_H

#include "nucleus.h"
#include "tsec.h"
#include "eth_npi_if.h"

#define TX_BUF_CNT      16
#define TX_BUF_SIZE     MRBLR_INIT_SETTINGS

#define RX_BUF_CNT      16
#define RX_BUF_SIZE     MRBLR_INIT_SETTINGS

#define tsec_lock_t                NU_SEMAPHORE
#define tsec_lock_init(lock,name)  (NU_Create_Semaphore(&(lock),name,1,NU_PRIORITY) == NU_SUCCESS ? 0 : -1)
#define tsec_lock(lock)            NU_Obtain_Semaphore(&(lock),NU_SUSPEND)
#define tsec_unlock(lock)          NU_Release_Semaphore(&(lock))

struct tsec_if_stat
{
    u32 rx_packets;     /* total packets received */
    u32 tx_packets;     /* total packets transmitted */
    u32 tx_packets_ok;  /* really send packet*/
    u32 rx_bytes;       /* total bytes received */
    u32 tx_bytes;       /* total bytes transmitted  */
    u32 rx_errors;      /* bad packets received */
    u32 tx_errors;      /* packet transmit problems */
    u32 rx_dropped;     /* no space available */
    u32 tx_dropped;     /* no space available */
};

typedef struct tsec_if
{
    struct eth_npi_if tif;      /*tsec inter face*/
    volatile tsec_t * regs;
    volatile tsec_t * phyRegs;
    u32 phyaddr;
    u32 link;
    u32 duplexity;
    u32 speed;
    txbd8_t txbds[TX_BUF_CNT] __attribute__ (( aligned ( 8 )));
    u8      txbufs[TX_BUF_CNT][TX_BUF_SIZE] __attribute__ (( aligned ( 64 )));
    u32     cur_txbd;
    u32     dirty_txbd;
    rxbd8_t rxbds[RX_BUF_CNT] __attribute__ (( aligned ( 8 )));
    u8      rxbufs[RX_BUF_CNT][RX_BUF_SIZE] __attribute__ (( aligned ( 64 )));
    u32     cur_rxbd;
    struct tsec_if_stat stats;
    struct gfar_extra_stats extra_stats;
    tsec_lock_t  tx_lock;

    u32 last_tx_time;  /*last tx time*/
    u32 last_rx_time;  /*last rx time*/
    u32 reset_times;   /*if tx/rx timeout, reset interface*/
    u32 rx_timeout;
    u32 tx_timeout;
}tsec_if_t;

void tsec_link_update( tsec_if_t * pif );
int tsec_initialize( tsec_if_t * pif );
int tsec_transmint( tsec_if_t * pif, u8 * data, u32 length );
void tsec_tx_event_handle( tsec_if_t * pif );
int tsec_rx_event_handle( tsec_if_t * pif );
int tsec_error_event_handle( tsec_if_t * pif );

#endif /*_TSEC_NPI_H*/
