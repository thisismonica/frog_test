/*ethernet npi interface on npu, only one interface support
defined by shenchen@xjgc.com,20101103*/

#ifndef _ETH_NPI_IF_H
#define _ETH_NPI_IF_H

#include "types.h"

#define ETH_NPI_TX_TIMEOUT      1000    /*1000ms*/
#define ETH_NPI_RX_TIMEOUT      3000    /*3000ms*/

struct eth_npi_if_stat
{
    u32 rx_npi_packets; /* total npi packets received */
    u32 tx_npi_packets; /* total npi packets transmitted*/
    u32 rx_npi_bytes;   /* total npi bytes received*/
    u32 tx_npi_bytes;   /* total npi bytes transmitted */
};

struct eth_npi_if
{
    int (*initialize)(struct eth_npi_if *pif, void *arg); /*Initialize, return -1 if failed, else return 0.*/
    int (*send)(struct eth_npi_if *pif, u8 *data, u32 length ); /*Tx packet, return real send length.*/
    u8  mac_addr[6];   /*interface mac address*/
    u32 ready;         /*ready to use*/
    struct eth_npi_if_stat stat; /*runtime status stats*/
};

typedef void (* NPI_MSG_HANLDER )(u8 *data, u32 len);

/*ethernet npi interface initialize*/
int eth_npi_if_initialize(NPI_MSG_HANLDER handler);

/*send packet,return -1 if failed, else return send length. */
int eth_npi_if_send(u8 *data, u32 length );

/*when a packet is received from an interface, the function is called to handle the packet*/
int eth_npi_if_on_receive(u8 *packet, u32 length );


#define GWS_NPI_FUN_LIST_NAME "GwsNpiFunList"

struct gws_npi_fun_list
{
	int (*set_handle)(NPI_MSG_HANLDER handler); /*set npi frame handler*/
	int (*send)(u8 *data, u32 length);
};

#endif /* _ETH_NPI_IF_H */

