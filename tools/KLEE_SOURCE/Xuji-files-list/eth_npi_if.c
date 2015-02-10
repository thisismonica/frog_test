/*ethernet npi interface on npu, only one interface support
defined by shenchen@xjgc.com,20101103*/

#include <string.h>
#include <stdlib.h>
#include "eth_npi_if.h"
#include "tsec_npi_if.h"
#include "npi_protocol.h"
#include "Devi_extr.h"
#include "es_plat.h"

#define NPI_FRAME_LENGTH_MAX    1536
#define ETHERNET_MAC_SIZE       12  /*dst and src mac address*/

static u8  *npi_buffer = 0;
struct eth_npi_if *npi_if = (struct eth_npi_if *)&tsec_npi_if; 
static NPI_MSG_HANLDER user_handler = NULL;

struct gws_npi_fun_list *npiFunctionList = NULL;

/*ethernet npi interface initialize*/
int eth_npi_if_initialize(NPI_MSG_HANLDER handler)
{
	int tsec_index;

	npiFunctionList = (struct gws_npi_fun_list *)XJP_Function_Table_Retrieve(GWS_NPI_FUN_LIST_NAME);
	if(npiFunctionList)
	{
		if(npiFunctionList->set_handle(handler))
		{
			npiFunctionList = NULL;
			return -1;
		}
		else
			return 0;
	}
	else
	{
	    npi_if->ready = 0;
		npi_buffer = malloc(NPI_FRAME_LENGTH_MAX);
		if(!npi_buffer)
			return -1;	
	    user_handler = handler;
	    memset(npi_buffer,0xff,6); /*dst mac address*/
	    memcpy(npi_buffer + 6, npi_if->mac_addr, 6); /*src mac address*/
		tsec_index = DEVI_GetIdentificationCode() == XJ_IDC_NPU823 ? 0 : 1;
	    if(npi_if->initialize(npi_if, (void*)tsec_index) == 0)
	    {
	        npi_if->ready = 1;
	        return 0;
	    }
	}
	
    return -1;
}

/*send packet,return -1 if failed, else return send length. */
int eth_npi_if_send(u8 *data, u32 length )
{
    int len;

	if(npiFunctionList)
		return npiFunctionList->send(data, length);
	else if(npi_if->ready && data && length)
    {
        len = npi_frame_pack( data, length, 
            npi_buffer + ETHERNET_MAC_SIZE, NPI_FRAME_LENGTH_MAX - ETHERNET_MAC_SIZE);
        if(len)
        {
            len += ETHERNET_MAC_SIZE;
            if( npi_if->send(npi_if, npi_buffer, len) == len)
            {
                npi_if->stat.tx_npi_packets ++;
                npi_if->stat.tx_npi_bytes += length;
                return length;
            }
        }
    }
    
    return -1;
}

/*when a packet is received from an interface, the function is called to handle the packet*/
int eth_npi_if_on_receive(u8 *packet, u32 length )
{
	u8 *data;
    u32 len;

    data = npi_frame_unpack(packet + ETHERNET_MAC_SIZE, length - ETHERNET_MAC_SIZE, &len);
    if(data)
    {
        npi_if->stat.rx_npi_packets ++;
        npi_if->stat.rx_npi_bytes += len;

        if(user_handler)
            user_handler(data,len);
        
        return 0;
    }
    return -1;
}



