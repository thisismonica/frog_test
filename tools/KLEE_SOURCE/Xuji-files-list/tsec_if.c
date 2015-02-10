/*npu808 npi channel tsec driver by shenchen@xjgc.com,20101103*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tsec_if.h"
#include "phy.h"

static void mii_phy_reg_write( tsec_if_t * pif, uint regnum, uint value )
{
    volatile tsec_t * regbase = pif->phyRegs;
    u32 start_tm,interval;
    regbase->miimadd = ( pif->phyaddr << 8 ) | regnum;
    regbase->miimcon = value;
    asm( "sync" );
    start_tm = NU_Retrieve_Clock();
    while(regbase->miimind & MIIMIND_BUSY )
    {
		interval = NU_Retrieve_Clock() - start_tm;
		if(interval > 1)
			break;
    }
}

static uint mii_phy_reg_read( tsec_if_t * pif, uint regnum )
{
    uint value;
    volatile tsec_t * regbase = pif->phyRegs;
    u32 start_tm,interval;
    regbase->miimadd = ( pif->phyaddr << 8 ) | regnum;
    regbase->miimcom = 0;
    regbase->miimcom = MIIM_READ_COMMAND;
    asm( "sync" );
    start_tm = NU_Retrieve_Clock();
    while(regbase->miimind & ( MIIMIND_NOTVALID | MIIMIND_BUSY ) )
    {
		interval = NU_Retrieve_Clock() - start_tm;
		if(interval > 1)
			break;
    }
    value = regbase->miimstat;
    return value;
}

static int phy_probe( tsec_if_t * pif )
{
    uint phy_reg, phy_ID;

    phy_reg = mii_phy_reg_read( pif, MIIM_PHYIR1 );
    phy_ID = ( phy_reg & 0xffff ) << 16;
    phy_reg = mii_phy_reg_read( pif, MIIM_PHYIR2 );
    phy_ID |= ( phy_reg & 0xffff );
    phy_ID &= 0xfffffff0; /*remove chip revision number*/
    if( ( phy_ID == 0x00221510 ) /*ksz801*/
		|| (phy_ID == 0x20005CE0) /*dp83640*/
		|| (phy_ID == 0x00221610) ) /*ksz800*/
        return 0;
    else
        return - 1;
}

/* The mii regs is shareed by the two tsec,so just reset once. */
static void config_mii( tsec_if_t * pif )
{
    static int mii_reseted = 0;
    volatile tsec_t * regs = pif->regs;
    if(! mii_reseted )
    {
        regs->miimcfg = MIIMCFG_RESET;
        asm( "sync" );
        regs->miimcfg = MIIMCFG_INIT_VALUE;
        asm( "sync" );
        while ( pif->phyRegs->miimind & MIIMIND_BUSY ) ;
        mii_reseted = 1;
    }
}

static int phy_init( tsec_if_t * pif )
{
    u16 val = 0xffff;

    config_mii( pif );

    /*Reset phy after when startup*/
    mii_phy_reg_write( pif, MII_BMCR, BMCR_RESET );
    while(( mii_phy_reg_read( pif, MII_BMCR ) & BMCR_RESET ) && val )
        val --;

    if( phy_probe( pif )) //not correct phy
        return - 1;

	/*Config auto negotiation*/
	mii_phy_reg_write(pif, MII_ADVERTISE, ADVERTISE_ALL | ADVERTISE_PAUSE_CAP 
		| ADVERTISE_PAUSE_ASYM | ADVERTISE_CSMA);
	mii_phy_reg_write( pif, MII_BMCR, BMCR_ANENABLE | BMCR_ANRESTART);
	
    return 0;
}

void tsec_link_update( tsec_if_t * pif )
{
    int adv, status, lpa;
    volatile tsec_t * regs = pif->regs;

    status = mii_phy_reg_read( pif, MII_BMSR );
    if (!( status & BMSR_ANEGCOMPLETE )) /*auto negotiation not complete*/
        return;
	
    if (( status & BMSR_LSTATUS ) == 0 )
        pif->link = 0;
    else
        pif->link = 1;

    if( pif->link )
    {
        if ( pif->duplexity )
            regs->maccfg2 |= MACCFG2_FULL_DUPLEX;
        else
            regs->maccfg2 &= ~( MACCFG2_FULL_DUPLEX );

        regs->maccfg2 = (( regs->maccfg2 & ~( MACCFG2_IF )) | MACCFG2_MII );
        if ( pif->speed == 100 )
            regs->ecntrl |= ECNTRL_R100;
        else
            regs->ecntrl &= ~( ECNTRL_R100 );
    }
}

static void init_registers( volatile tsec_t * regs )
{
    /* Clear IEVENT */
    regs->ievent = IEVENT_INIT_CLEAR;
    regs->imask = IMASK_INIT_CLEAR;

    /* Init hash registers to zero */
    regs->igaddr0 =  0xffffffff;
    regs->igaddr1 =  0xffffffff;
    regs->igaddr2 =  0xffffffff;
    regs->igaddr3 =  0xffffffff;
    regs->igaddr4 =  0xffffffff;
    regs->igaddr5 =  0xffffffff;
    regs->igaddr6 =  0xffffffff;
    regs->igaddr7 =  0xffffffff;

    regs->gaddr0 = 0xffffffff;
    regs->gaddr1 = 0xffffffff;
    regs->gaddr2 = 0xffffffff;
    regs->gaddr3 = 0xffffffff;
    regs->gaddr4 = 0xffffffff;
    regs->gaddr5 = 0xffffffff;
    regs->gaddr6 = 0xffffffff;
    regs->gaddr7 = 0xffffffff;

    regs->rctrl = RCTRL_EXTHASH | RCTRL_EMEN;

    /* Init RMON mib registers */
    memset(( void *)&( regs->rmon ), 0, sizeof( rmon_mib_t ));
    regs->rmon.cam1 = 0xffffffff;
    regs->rmon.cam2 = 0xffffffff;
    regs->mrblr = MRBLR_INIT_SETTINGS;
    regs->minflr = MINFLR_INIT_SETTINGS;  //not support
    regs->maxfrm = MTU_INIT_SETTINGS;
    regs->attr = ATTR_INIT_SETTINGS;
    regs->attreli = ATTRELI_INIT_SETTINGS;
}

/* Set up the buffers and their descriptors, and bring up the
 * interface
 */
static void startup( tsec_if_t * pif )
{
    int i;
    volatile tsec_t * regs = pif->regs;

    /* Point to the buffer's point high*/
    regs->tbdbph = ( u32 )pif->txbufs;
    regs->rbdbph = ( u32 )pif->rxbufs;

    /* Point to the buffer descriptors high*/
    regs->tbaseh = ( u32 )pif->txbds;
    regs->rbaseh = ( u32 )pif->rxbds;

    /* Point to the ring descriptor's point*/
    regs->tbase0 = ( u32 )pif->txbds;
    regs->rbase0 = ( u32 )pif->rxbds;

    /* Initialize the Rx Buffer descriptors */
    for ( i = 0; i < RX_BUF_CNT; i ++) {
        pif->rxbds[i].status = ( RXBD_EMPTY | RXBD_INTERRUPT );
        pif->rxbds[i].length = 0;
        pif->rxbds[i].bufPtr = ( uint )pif->rxbufs[i];
    }
    pif->rxbds[RX_BUF_CNT - 1].status |= RXBD_WRAP;
    pif->cur_rxbd = 0;

    /* Initialize the TX Buffer Descriptors */
    for ( i = 0; i < TX_BUF_CNT; i ++) {
        pif->txbds[i].status = 0;
        pif->txbds[i].length = 0;
        pif->txbds[i].bufPtr = ( uint )pif->txbufs[i];
    }
    pif->txbds[TX_BUF_CNT - 1].status |= TXBD_WRAP;
    pif->dirty_txbd = pif->cur_txbd = 0;

    /* Configure the coalescing not support */
    regs->txic = 0;
    regs->rxic = 0;

    /* Enable Transmit and Receive */
    regs->maccfg1 =  MACCFG1_RX_EN | MACCFG1_TX_EN | MACCFG1_TX_FLOW | MACCFG1_RX_FLOW;

    /* Tell the DMA it is clear to go */
    regs->dmactrl |= DMACTRL_INIT_SETTINGS;
    regs->dmactrl &= ~( DMACTRL_GRS | DMACTRL_GTS );
    regs->tstat = TSTAT_CLEAR_THALT;
    regs->rstat = RSTAT_CLEAR_RHALT;

    /* Unmask the interrupts we look for */
    regs->ievent = IEVENT_INIT_CLEAR;
    regs->imask =  IMASK_DEFAULT;
}

/* Stop the interface */
static void halt( tsec_if_t * pif )
{
    volatile tsec_t * regs = pif->regs;
    u32 start_tm,interval;
    regs->dmactrl &= ~( DMACTRL_GRS | DMACTRL_GTS );
    regs->dmactrl |= ( DMACTRL_GRS | DMACTRL_GTS );
    
    start_tm = NU_Retrieve_Clock();
    while (!( regs->ievent & ( IEVENT_GRSC | IEVENT_GTSC )))
    {
		interval = NU_Retrieve_Clock() - start_tm;
		if(interval > 1)
			break;
    }
    
    regs->maccfg1 &= ~( MACCFG1_TX_EN | MACCFG1_RX_EN );
}

static void tx_completion( tsec_if_t * pif )
{
    struct tsec_if_stat * stats = & pif->stats;
    txbd8_t * bdp;

    bdp = & pif->txbds[pif->dirty_txbd];
    while (!( bdp->status & TXBD_READY ) )
    {
        if ( pif->dirty_txbd == pif->cur_txbd )
            break;
        bdp->status &= TXBD_WRAP;

        if ( bdp->status & TXBD_WRAP )
            pif->dirty_txbd = 0;
        else
            pif->dirty_txbd ++;

        bdp = & pif->txbds[pif->dirty_txbd];

        stats->tx_packets_ok ++;
    }
}

static int receive_packets( tsec_if_t * pif )
{
    int length, packets;
    volatile tsec_t * regs = pif->regs;
    struct tsec_if_stat * stats = & pif->stats;

    packets = 0;
    while(!( pif->rxbds[pif->cur_rxbd].status & RXBD_EMPTY ))
    {
        length = pif->rxbds[pif->cur_rxbd].length - 4;
        if (!( pif->rxbds[pif->cur_rxbd].status & RXBD_STATS ))
        {
            stats->rx_packets ++;;
            stats->rx_bytes += length;
            packets ++;
            if( eth_npi_if_on_receive( pif->rxbufs[pif->cur_rxbd], length ))
                stats->rx_dropped ++;
        }
        else
            stats->rx_errors ++;

        pif->rxbds[pif->cur_rxbd].length = 0;
        if( pif->cur_rxbd == ( RX_BUF_CNT - 1 ))
        {
            pif->rxbds[pif->cur_rxbd].status = ( RXBD_EMPTY | RXBD_INTERRUPT | RXBD_WRAP );
            pif->cur_rxbd = 0;
        }
        else
        {
            pif->rxbds[pif->cur_rxbd].status = ( RXBD_EMPTY | RXBD_INTERRUPT );
            pif->cur_rxbd ++;
        }
    }
    return packets;
}

int tsec_initialize( tsec_if_t * pif )
{
    volatile tsec_t * regs = pif->regs;

    uint tempval, i;
    char tmpbuf[MAC_ADDR_LEN];

    /* Make sure the controller is stopped */
    halt( pif );

    pif->duplexity = 1;
    pif->speed = 100;
    pif->link = 0;

    memset(& pif->stats, 0, sizeof( pif->stats ));
    memset(& pif->extra_stats, 0, sizeof( pif->extra_stats ));

    /* Reset the MAC */
    regs->maccfg1 = MACCFG1_SOFT_RESET;
    regs->maccfg1 = 0;

    /* Init MACCFG2.  Defaults to GMII */
    regs->maccfg2 = MACCFG2_INIT_SETTINGS;

    /* Init ECNTRL */
    regs->ecntrl = ECNTRL_INIT_SETTINGS;

    /* Copy the station address into the dev structure, */
    for ( i = 0; i < MAC_ADDR_LEN; i ++)
        tmpbuf[MAC_ADDR_LEN - 1 - i] = pif->tif.mac_addr[i];
    regs->macstnaddr1 = *(( uint *) ( tmpbuf ));
    tempval = *(( uint *) ( tmpbuf + 4 ));
    regs->macstnaddr2 = tempval;

    /* Clear out (for the most part) the other registers */
    init_registers( regs );

    if( phy_init( pif ))
        return - 1;

    /* Ready the device for tx/rx */
    startup( pif );

    return 0;
}

/* This returns the status bits of the device.  The return value
 * is never checked, and this is what the 8260 driver did, so we
 * do the same.  Presumably, this would be zero if there were no
 * errors
 */
int tsec_transmint( tsec_if_t * pif, u8 * data, unsigned int length )
{
    volatile tsec_t * regs = pif->regs;
    struct tsec_if_stat * stats = & pif->stats;
	int old_irqs;

    if(! pif->link )
        return 0;

    //tsec_lock( pif->tx_lock );
    old_irqs = NU_Control_Interrupts(0);
    if(( length < TX_BUF_SIZE ) && (!( pif->txbds[pif->cur_txbd].status & TXBD_READY ) ) )
    {
        memcpy( pif->txbufs[pif->cur_txbd], data, length );
        pif->txbds[pif->cur_txbd].length = ( u16 )length;
        if( pif->cur_txbd == ( TX_BUF_CNT - 1 ))
        {
            pif->txbds[pif->cur_txbd].status = ( TXBD_WRAP | TXBD_READY | TXBD_LAST | TXBD_CRC | TXBD_INTERRUPT );
            pif->cur_txbd = 0;
        }
        else
        {
            pif->txbds[pif->cur_txbd].status = ( TXBD_READY | TXBD_LAST | TXBD_CRC | TXBD_INTERRUPT );
            pif->cur_txbd ++;
        }

        /* Tell the DMA to go */
        regs->tstat = TSTAT_CLEAR_THALT;
    }
    else
        length = 0;
    //tsec_unlock( pif->tx_lock );

    if( length )
    {
        stats->tx_bytes += length;
        stats->tx_packets ++;
    }
    else
        stats->tx_dropped ++;

	NU_Control_Interrupts(old_irqs);

    return ( int )length;
}

void tsec_tx_event_handle( tsec_if_t * pif )
{
    volatile tsec_t * regs = pif->regs;
    regs->ievent = IEVENT_TX_MASK;
    tx_completion( pif );
}

int tsec_rx_event_handle( tsec_if_t * pif )
{
    volatile tsec_t * regs = pif->regs;
    int packets;

    regs->imask &= IMASK_RX_DISABLED; //disable rx interrupt
    regs->ievent =  IEVENT_RX_MASK; //clear flag
    packets = receive_packets( pif );
    regs->rstat = RSTAT_CLEAR_RHALT;
    regs->imask =  IMASK_DEFAULT;
    return packets;
}

/* tsec error interrupt handler */
int tsec_error_event_handle( tsec_if_t * pif )
{
    volatile tsec_t * regs = pif->regs;
    struct tsec_if_stat * stats = & pif->stats;
    u32 events = regs->ievent;
    int packets = 0;

    regs->ievent = IEVENT_ERR_MASK; //clear flags
    if ( events & IEVENT_TXE )
    {
        stats->tx_errors ++;
        if ( events & IEVENT_XFUN )
        {
            stats->tx_dropped ++;
            pif->extra_stats.tx_underrun ++;
            regs->tstat = TSTAT_CLEAR_THALT;
        }
    }
    if ( events & IEVENT_BSY )
    {
        stats->rx_errors ++;
        pif->extra_stats.rx_bsy ++;

        regs->imask &= IMASK_RX_DISABLED; //disable rx interrupt
        packets = receive_packets( pif );
        regs->rstat =  RSTAT_CLEAR_RHALT;
        regs->imask =  IMASK_DEFAULT;
    }
    if ( events & IEVENT_BABR )
    {
        stats->rx_errors ++;
        pif->extra_stats.rx_babr ++;
    }
    if ( events & IEVENT_EBERR )
        pif->extra_stats.eberr ++;

    if ( events & IEVENT_BABT )
        pif->extra_stats.tx_babt ++;

    return packets;
}
