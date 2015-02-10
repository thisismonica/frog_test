/*mpc8313 tsec definition,by shenchen@xjgc.com*/

#ifndef __TSEC_H
#define __TSEC_H

#include "types.h"
#include "immap_83xx.h"
#include "mpc83xx.h"
#include "e300.h"
#include "CFG_8313xj.h"

#define MAXCONTROLLERS  (2)

#ifndef CFG_TSEC1_OFFSET
#define CFG_TSEC1_OFFSET    (0x24000)
#endif

#define TSEC_SIZE   0x01000

/* FIXME:  Should these be pushed back to 83xx and 85xx config files? */
#if defined(CONFIG_MPC85xx) || defined(CONFIG_MPC86xx)
#define TSEC_BASE_ADDR  (CFG_IMMR + CFG_TSEC1_OFFSET)
#elif defined(CONFIG_MPC83XX)
#define TSEC_BASE_ADDR  (CFG_IMMR + CFG_TSEC1_OFFSET)
#endif

#define MAC_ADDR_LEN 6

/* MAC register bits */
#define MACCFG1_SOFT_RESET  0x80000000
#define MACCFG1_RESET_RX_MC 0x00080000
#define MACCFG1_RESET_TX_MC 0x00040000
#define MACCFG1_RESET_RX_FUN    0x00020000
#define MACCFG1_RESET_TX_FUN    0x00010000
#define MACCFG1_LOOPBACK    0x00000100
#define MACCFG1_RX_FLOW     0x00000020
#define MACCFG1_TX_FLOW     0x00000010
#define MACCFG1_SYNCD_RX_EN 0x00000008
#define MACCFG1_RX_EN       0x00000004
#define MACCFG1_SYNCD_TX_EN 0x00000002
#define MACCFG1_TX_EN       0x00000001

#define MACCFG2_INIT_SETTINGS   0x00007205
#define MACCFG2_FULL_DUPLEX 0x00000001
#define MACCFG2_IF              0x00000300
#define MACCFG2_GMII        0x00000200
#define MACCFG2_MII             0x00000100

#define ECNTRL_INIT_SETTINGS    0x00001000
#define ECNTRL_TBI_MODE         0x00000020
#define ECNTRL_R100     0x00000008
#define ECNTRL_SGMII_MODE   0x00000002

#define miim_end -2
#define miim_read -1

#ifndef CFG_TBIPA_VALUE
#define CFG_TBIPA_VALUE 0x1f
#endif
#define MIIMCFG_INIT_VALUE  0x00000003
#define MIIMCFG_RESET       0x80000000

#define MIIMIND_BUSY            0x00000001
#define MIIMIND_NOTVALID        0x00000004

#define MIIM_CONTROL            0x00
#define MIIM_CONTROL_RESET  0x00009140
#define MIIM_CONTROL_INIT       0x00001140
#define MIIM_CONTROL_RESTART    0x00001340
#define MIIM_ANEN               0x00001000

#define MIIM_CR                 0x00
#define MIIM_CR_RST     0x00008000
#define MIIM_CR_INIT            0x00001000

#define MIIM_STATUS     0x1
#define MIIM_STATUS_AN_DONE     0x00000020
#define MIIM_STATUS_LINK    0x0004
#define PHY_BMSR_AUTN_ABLE  0x0008
#define PHY_BMSR_AUTN_COMP  0x0020

#define MIIM_PHYIR1     0x2
#define MIIM_PHYIR2     0x3

#define MIIM_ANAR       0x4
#define MIIM_ANAR_INIT      0x1e1

#define MIIM_TBI_ANLPBPA    0x5
#define MIIM_TBI_ANLPBPA_HALF   0x00000040
#define MIIM_TBI_ANLPBPA_FULL   0x00000020

#define MIIM_TBI_ANEX       0x6
#define MIIM_TBI_ANEX_NP    0x00000004
#define MIIM_TBI_ANEX_PRX   0x00000002

#define MIIM_GBIT_CONTROL   0x9
#define MIIM_GBIT_CONTROL_INIT  0xe00

/* Broadcom BCM54xx -- taken from linux sungem_phy */
#define MIIM_BCM54xx_AUXSTATUS          0x19
#define MIIM_BCM54xx_AUXSTATUS_LINKMODE_MASK    0x0700
#define MIIM_BCM54xx_AUXSTATUS_LINKMODE_SHIFT   8

/* Cicada Auxiliary Control/Status Register */
#define MIIM_CIS8201_AUX_CONSTAT        0x1c
#define MIIM_CIS8201_AUXCONSTAT_INIT    0x0004
#define MIIM_CIS8201_AUXCONSTAT_DUPLEX  0x0020
#define MIIM_CIS8201_AUXCONSTAT_SPEED   0x0018
#define MIIM_CIS8201_AUXCONSTAT_GBIT    0x0010
#define MIIM_CIS8201_AUXCONSTAT_100     0x0008

/* Cicada Extended Control Register 1 */
#define MIIM_CIS8201_EXT_CON1           0x17
#define MIIM_CIS8201_EXTCON1_INIT       0x0000

/* Cicada 8204 Extended PHY Control Register 1 */
#define MIIM_CIS8204_EPHY_CON       0x17
#define MIIM_CIS8204_EPHYCON_INIT   0x0006
#define MIIM_CIS8204_EPHYCON_RGMII  0x1100

/* Cicada 8204 Serial LED Control Register */
#define MIIM_CIS8204_SLED_CON       0x1b
#define MIIM_CIS8204_SLEDCON_INIT   0x1115

#define MIIM_GBIT_CON       0x09
#define MIIM_GBIT_CON_ADVERT    0x0e00

/* Entry for Vitesse VSC8244 regs starts here */
/* Vitesse VSC8244 Auxiliary Control/Status Register */
#define MIIM_VSC8244_AUX_CONSTAT        0x1c
#define MIIM_VSC8244_AUXCONSTAT_INIT    0x0000
#define MIIM_VSC8244_AUXCONSTAT_DUPLEX  0x0020
#define MIIM_VSC8244_AUXCONSTAT_SPEED   0x0018
#define MIIM_VSC8244_AUXCONSTAT_GBIT    0x0010
#define MIIM_VSC8244_AUXCONSTAT_100     0x0008
#define MIIM_CONTROL_INIT_LOOPBACK      0x4000

/* Vitesse VSC8244 Extended PHY Control Register 1 */
#define MIIM_VSC8244_EPHY_CON           0x17
#define MIIM_VSC8244_EPHYCON_INIT       0x0006

/* Vitesse VSC8244 Serial LED Control Register */
#define MIIM_VSC8244_LED_CON            0x1b
#define MIIM_VSC8244_LEDCON_INIT        0xF011

/* 88E1011 PHY Status Register */
#define MIIM_88E1011_PHY_STATUS         0x11
#define MIIM_88E1011_PHYSTAT_SPEED      0xc000
#define MIIM_88E1011_PHYSTAT_GBIT       0x8000
#define MIIM_88E1011_PHYSTAT_100        0x4000
#define MIIM_88E1011_PHYSTAT_DUPLEX     0x2000
#define MIIM_88E1011_PHYSTAT_SPDDONE    0x0800
#define MIIM_88E1011_PHYSTAT_LINK   0x0400

#define MIIM_88E1011_PHY_SCR        0x10
#define MIIM_88E1011_PHY_MDI_X_AUTO 0x0060

/* 88E1111 PHY LED Control Register */
#define MIIM_88E1111_PHY_LED_CONTROL   24
#define MIIM_88E1111_PHY_LED_DIRECT    0x4100
#define MIIM_88E1111_PHY_LED_COMBINE   0x411C

/* 88E1145 Extended PHY Specific Control Register */
#define MIIM_88E1145_PHY_EXT_CR 20
#define MIIM_M88E1145_RGMII_RX_DELAY    0x0080
#define MIIM_M88E1145_RGMII_TX_DELAY    0x0002

#define MIIM_88E1145_PHY_PAGE   29
#define MIIM_88E1145_PHY_CAL_OV 30

/* DM9161 Control register values */
#define MIIM_DM9161_CR_STOP 0x0400
#define MIIM_DM9161_CR_RSTAN    0x1200

#define MIIM_DM9161_SCR     0x10
#define MIIM_DM9161_SCR_INIT    0x0610

/* DM9161 Specified Configuration and Status Register */
#define MIIM_DM9161_SCSR    0x11
#define MIIM_DM9161_SCSR_100F   0x8000
#define MIIM_DM9161_SCSR_100H   0x4000
#define MIIM_DM9161_SCSR_10F    0x2000
#define MIIM_DM9161_SCSR_10H    0x1000

/* DM9161 10BT Configuration/Status */
#define MIIM_DM9161_10BTCSR 0x12
#define MIIM_DM9161_10BTCSR_INIT    0x7800

/* LXT971 Status 2 registers */
#define MIIM_LXT971_SR2              0x11  /* Status Register 2  */
#define MIIM_LXT971_SR2_SPEED_MASK 0x4200
#define MIIM_LXT971_SR2_10HDX      0x0000  /*  10 Mbit half duplex selected */
#define MIIM_LXT971_SR2_10FDX      0x0200  /*  10 Mbit full duplex selected */
#define MIIM_LXT971_SR2_100HDX     0x4000  /* 100 Mbit half duplex selected */
#define MIIM_LXT971_SR2_100FDX     0x4200  /* 100 Mbit full duplex selected */

/* DP83865 Control register values */
#define MIIM_DP83865_CR_INIT    0x9200

/* DP83865 Link and Auto-Neg Status Register */
#define MIIM_DP83865_LANR   0x11
#define MIIM_DP83865_SPD_MASK   0x0018
#define MIIM_DP83865_SPD_1000   0x0010
#define MIIM_DP83865_SPD_100    0x0008
#define MIIM_DP83865_DPX_FULL   0x0002

#define MIIM_READ_COMMAND       0x00000001

#define MTU_INIT_SETTINGS       0x00000600
#define MRBLR_INIT_SETTINGS     0x00000600
#define MINFLR_INIT_SETTINGS    0x00000040

#define DMACTRL_INIT_SETTINGS   0x000000c3
#define DMACTRL_GRS             0x00000010
#define DMACTRL_GTS             0x00000008

#define TSTAT_CLEAR_THALT       0x80000000
#define RSTAT_CLEAR_RHALT       0x00800000

#define IEVENT_INIT_CLEAR   0xffffffff
#define IEVENT_BABR     0x80000000
#define IEVENT_RXC      0x40000000
#define IEVENT_BSY      0x20000000
#define IEVENT_EBERR    0x10000000
#define IEVENT_MSRO     0x04000000
#define IEVENT_GTSC     0x02000000
#define IEVENT_BABT     0x01000000
#define IEVENT_TXC      0x00800000
#define IEVENT_TXE      0x00400000
#define IEVENT_TXB      0x00200000
#define IEVENT_TXF      0x00100000
#define IEVENT_LC       0x00040000
#define IEVENT_CRL      0x00020000
#define IEVENT_XFUN     0x00010000
#define IEVENT_RXB0     0x00008000
#define IEVENT_GRSC     0x00000100
#define IEVENT_RXF0     0x00000080
#define IEVENT_FIR      0x00000008
#define IEVENT_FIQ      0x00000004
#define IEVENT_DPE      0x00000002
#define IEVENT_PERR     0x00000001
#define IEVENT_RX_MASK          (IEVENT_RXB0 | IEVENT_RXF0)
#define IEVENT_TX_MASK          (IEVENT_TXB | IEVENT_TXF)
#define IEVENT_ERR_MASK         \
(IEVENT_RXC | IEVENT_BSY | IEVENT_EBERR | IEVENT_MSRO | \
 IEVENT_BABT | IEVENT_TXC | IEVENT_TXE | IEVENT_LC \
 | IEVENT_CRL | IEVENT_XFUN | IEVENT_DPE | IEVENT_PERR)

#define IMASK_INIT_CLEAR  0x00000000
#define IMASK_BABR        0x80000000
#define IMASK_RXC         0x40000000
#define IMASK_BSY         0x20000000
#define IMASK_EBERR       0x10000000
#define IMASK_MSRO        0x04000000
#define IMASK_GRSC        0x02000000
#define IMASK_BABT        0x01000000
#define IMASK_TXC         0x00800000
#define IMASK_TXEEN       0x00400000
#define IMASK_TXBEN       0x00200000
#define IMASK_TXFEN       0x00100000
#define IMASK_LC          0x00040000
#define IMASK_CRL         0x00020000
#define IMASK_XFUN        0x00010000
#define IMASK_RXB0        0x00008000
#define IMASK_GTSC        0x00000100
#define IMASK_RXFEN0      0x00000080
#define IMASK_FIR         0x00000008
#define IMASK_FIQ         0x00000004
#define IMASK_DPE         0x00000002
#define IMASK_PERR        0x00000001
#define IMASK_RX_DISABLED ~(IMASK_RXFEN0 | IMASK_BSY)
#define IMASK_DEFAULT  (IMASK_TXEEN | IMASK_TXFEN | \
        IMASK_RXFEN0 | IMASK_BSY | IMASK_EBERR | IMASK_BABR | \
        IMASK_XFUN | IMASK_RXC | IMASK_BABT | IMASK_DPE \
        | IMASK_PERR)
        
        
#define RCTRL_PAL_MASK		0x001f0000
#define RCTRL_VLEX		0x00002000
#define RCTRL_FILREN		0x00001000
#define RCTRL_GHTX		0x00000400
#define RCTRL_IPCSEN		0x00000200
#define RCTRL_TUCSEN		0x00000100
#define RCTRL_PRSDEP_MASK	0x000000c0
#define RCTRL_PRSDEP_INIT	0x000000c0
#define RCTRL_PROM		0x00000008
#define RCTRL_EMEN		0x00000002
#define RCTRL_CHECKSUMMING	(RCTRL_IPCSEN \
		| RCTRL_TUCSEN | RCTRL_PRSDEP_INIT)
#define RCTRL_EXTHASH		(RCTRL_GHTX)
#define RCTRL_VLAN		(RCTRL_PRSDEP_INIT)
#define RCTRL_PADDING(x)	((x << 16) & RCTRL_PAL_MASK)        

/* Default Attribute fields */
#define ATTR_INIT_SETTINGS     0x000000c0
#define ATTRELI_INIT_SETTINGS  0x00000000

/* TxBD status field bits */
#define TXBD_READY      0x8000
#define TXBD_PADCRC     0x4000
#define TXBD_WRAP       0x2000
#define TXBD_INTERRUPT      0x1000
#define TXBD_LAST       0x0800
#define TXBD_CRC        0x0400
#define TXBD_DEF        0x0200
#define TXBD_HUGEFRAME      0x0080
#define TXBD_LATECOLLISION  0x0080
#define TXBD_RETRYLIMIT     0x0040
#define TXBD_RETRYCOUNTMASK 0x003c
#define TXBD_UNDERRUN       0x0002
#define TXBD_STATS              0x03ff

/* RxBD status field bits */
#define RXBD_EMPTY      0x8000
#define RXBD_RO1        0x4000
#define RXBD_WRAP       0x2000
#define RXBD_INTERRUPT      0x1000
#define RXBD_LAST       0x0800
#define RXBD_FIRST      0x0400
#define RXBD_MISS       0x0100
#define RXBD_BROADCAST      0x0080
#define RXBD_MULTICAST      0x0040
#define RXBD_LARGE      0x0020
#define RXBD_NONOCTET       0x0010
#define RXBD_SHORT      0x0008
#define RXBD_CRCERR     0x0004
#define RXBD_OVERRUN        0x0002
#define RXBD_TRUNCATED      0x0001
#define RXBD_STATS      (RXBD_LARGE | RXBD_SHORT | RXBD_NONOCTET \
                                                | RXBD_CRCERR | RXBD_OVERRUN | RXBD_TRUNCATED)          //0x01ff

typedef struct txbd8
{
    ushort       status;         /* Status Fields */
    ushort       length;         /* Buffer length */
    uint         bufPtr;         /* Buffer Pointer */
} txbd8_t;

typedef struct rxbd8
{
    ushort       status;         /* Status Fields */
    ushort       length;         /* Buffer Length */
    uint         bufPtr;         /* Buffer Pointer */
} rxbd8_t;

typedef struct rmon_mib
{
    /* Transmit and Receive Counters */
    uint    tr64;       /* Transmit and Receive 64-byte Frame Counter */
    uint    tr127;      /* Transmit and Receive 65-127 byte Frame Counter */
    uint    tr255;      /* Transmit and Receive 128-255 byte Frame Counter */
    uint    tr511;      /* Transmit and Receive 256-511 byte Frame Counter */
    uint    tr1k;       /* Transmit and Receive 512-1023 byte Frame Counter */
    uint    trmax;      /* Transmit and Receive 1024-1518 byte Frame Counter */
    uint    trmgv;      /* Transmit and Receive 1519-1522 byte Good VLAN Frame */
    /* Receive Counters */
    uint    rbyt;       /* Receive Byte Counter */
    uint    rpkt;       /* Receive Packet Counter */
    uint    rfcs;       /* Receive FCS Error Counter */
    uint    rmca;       /* Receive Multicast Packet (Counter) */
    uint    rbca;       /* Receive Broadcast Packet */
    uint    rxcf;       /* Receive Control Frame Packet */
    uint    rxpf;       /* Receive Pause Frame Packet */
    uint    rxuo;       /* Receive Unknown OP Code */
    uint    raln;       /* Receive Alignment Error */
    uint    rflr;       /* Receive Frame Length Error */
    uint    rcde;       /* Receive Code Error */
    uint    rcse;       /* Receive Carrier Sense Error */
    uint    rund;       /* Receive Undersize Packet */
    uint    rovr;       /* Receive Oversize Packet */
    uint    rfrg;       /* Receive Fragments */
    uint    rjbr;       /* Receive Jabber */
    uint    rdrp;       /* Receive Drop */
    /* Transmit Counters */
    uint    tbyt;       /* Transmit Byte Counter */
    uint    tpkt;       /* Transmit Packet */
    uint    tmca;       /* Transmit Multicast Packet */
    uint    tbca;       /* Transmit Broadcast Packet */
    uint    txpf;       /* Transmit Pause Control Frame */
    uint    tdfr;       /* Transmit Deferral Packet */
    uint    tedf;       /* Transmit Excessive Deferral Packet */
    uint    tscl;       /* Transmit Single Collision Packet */
    /* (0x2_n700) */
    uint    tmcl;       /* Transmit Multiple Collision Packet */
    uint    tlcl;       /* Transmit Late Collision Packet */
    uint    txcl;       /* Transmit Excessive Collision Packet */
    uint    tncl;       /* Transmit Total Collision */

    uint    res2;

    uint    tdrp;       /* Transmit Drop Frame */
    uint    tjbr;       /* Transmit Jabber Frame */
    uint    tfcs;       /* Transmit FCS Error */
    uint    txcf;       /* Transmit Control Frame */
    uint    tovr;       /* Transmit Oversize Frame */
    uint    tund;       /* Transmit Undersize Frame */
    uint    tfrg;       /* Transmit Fragments Frame */
    /* General Registers */
    uint    car1;       /* Carry Register One */
    uint    car2;       /* Carry Register Two */
    uint    cam1;       /* Carry Register One Mask */
    uint    cam2;       /* Carry Register Two Mask */
} rmon_mib_t;

typedef struct tsec_hash_regs
{
    uint    iaddr0;     /* Individual Address Register 0 */
    uint    iaddr1;     /* Individual Address Register 1 */
    uint    iaddr2;     /* Individual Address Register 2 */
    uint    iaddr3;     /* Individual Address Register 3 */
    uint    iaddr4;     /* Individual Address Register 4 */
    uint    iaddr5;     /* Individual Address Register 5 */
    uint    iaddr6;     /* Individual Address Register 6 */
    uint    iaddr7;     /* Individual Address Register 7 */
    uint    res1[24];
    uint    gaddr0;     /* Group Address Register 0 */
    uint    gaddr1;     /* Group Address Register 1 */
    uint    gaddr2;     /* Group Address Register 2 */
    uint    gaddr3;     /* Group Address Register 3 */
    uint    gaddr4;     /* Group Address Register 4 */
    uint    gaddr5;     /* Group Address Register 5 */
    uint    gaddr6;     /* Group Address Register 6 */
    uint    gaddr7;     /* Group Address Register 7 */
    uint    res2[24];
} tsec_hash_t;

typedef struct tsec {
    u32 tsec_id;    /* 0x.000 - Controller ID register */
    u8  res1[12];
    u32 ievent;     /* 0x.010 - Interrupt Event Register */
    u32 imask;      /* 0x.014 - Interrupt Mask Register */
    u32 edis;       /* 0x.018 - Error Disabled Register */
    u8  res2[4];
    u32 ecntrl;     /* 0x.020 - Ethernet Control Register */
    u32 minflr;     /* 0x.024 - Minimum Frame Length Register */
    u32 ptv;        /* 0x.028 - Pause Time Value Register */
    u32 dmactrl;    /* 0x.02c - DMA Control Register */
    u32 tbipa;      /* 0x.030 - TBI PHY Address Register */
    u8  res3[88];
    u32 fifo_tx_thr;    /* 0x.08c - FIFO transmit threshold register */
    u8  res4[8];
    u32 fifo_tx_starve; /* 0x.098 - FIFO transmit starve register */
    u32 fifo_tx_starve_shutoff; /* 0x.09c - FIFO transmit starve shutoff register */
    u8  res5[4];
    u32 fifo_rx_pause;  /* 0x.0a4 - FIFO receive pause threshold register */
    u32 fifo_rx_alarm;  /* 0x.0a8 - FIFO receive alarm threshold register */
    u8  res6[84];
    u32 tctrl;      /* 0x.100 - Transmit Control Register */
    u32 tstat;      /* 0x.104 - Transmit Status Register */
    u32 dfvlan;     /* 0x.108 - Default VLAN Control word */
    u32 tbdlen;     /* 0x.10c - Transmit Buffer Descriptor Data Length Register */
    u32 txic;       /* 0x.110 - Transmit Interrupt Coalescing Configuration Register */
    u32 tqueue;     /* 0x.114 - Transmit queue control register */
    u8  res7[40];
    u32 tr03wt;     /* 0x.140 - TxBD Rings 0-3 round-robin weightings */
    u32 tr47wt;     /* 0x.144 - TxBD Rings 4-7 round-robin weightings */
    //    u8  res8[52];
    //    u32 tbdbph;     /* 0x.17c - Tx data buffer pointer high */
    //    u8  res9a[4];

    u8  res8[56];
    u32 tbdbph;     /* 0x.180 - Tx data buffer pointer high */

    u32 tbptr0;     /* 0x.184 - TxBD Pointer for ring 0 */
    u8  res9b[4];
    u32 tbptr1;     /* 0x.18c - TxBD Pointer for ring 1 */
    u8  res9c[4];
    u32 tbptr2;     /* 0x.194 - TxBD Pointer for ring 2 */
    u8  res9d[4];
    u32 tbptr3;     /* 0x.19c - TxBD Pointer for ring 3 */
    u8  res9e[4];
    u32 tbptr4;     /* 0x.1a4 - TxBD Pointer for ring 4 */
    u8  res9f[4];
    u32 tbptr5;     /* 0x.1ac - TxBD Pointer for ring 5 */
    u8  res9g[4];
    u32 tbptr6;     /* 0x.1b4 - TxBD Pointer for ring 6 */
    u8  res9h[4];
    u32 tbptr7;     /* 0x.1bc - TxBD Pointer for ring 7 */
    u8  res9[64];
    u32 tbaseh;     /* 0x.200 - TxBD base address high */
    u32 tbase0;     /* 0x.204 - TxBD Base Address of ring 0 */
    u8  res10a[4];
    u32 tbase1;     /* 0x.20c - TxBD Base Address of ring 1 */
    u8  res10b[4];
    u32 tbase2;     /* 0x.214 - TxBD Base Address of ring 2 */
    u8  res10c[4];
    u32 tbase3;     /* 0x.21c - TxBD Base Address of ring 3 */
    u8  res10d[4];
    u32 tbase4;     /* 0x.224 - TxBD Base Address of ring 4 */
    u8  res10e[4];
    u32 tbase5;     /* 0x.22c - TxBD Base Address of ring 5 */
    u8  res10f[4];
    u32 tbase6;     /* 0x.234 - TxBD Base Address of ring 6 */
    u8  res10g[4];
    u32 tbase7;     /* 0x.23c - TxBD Base Address of ring 7 */
    u8  res10[192];
    u32 rctrl;      /* 0x.300 - Receive Control Register */
    u32 rstat;      /* 0x.304 - Receive Status Register */
    u8  res12[8];
    u32 rxic;       /* 0x.310 - Receive Interrupt Coalescing Configuration Register */
    u32 rqueue;     /* 0x.314 - Receive queue control register */
    u8  res13[24];
    u32 rbifx;      /* 0x.330 - Receive bit field extract control register */
    u32 rqfar;      /* 0x.334 - Receive queue filing table address register */
    u32 rqfcr;      /* 0x.338 - Receive queue filing table control register */
    u32 rqfpr;      /* 0x.33c - Receive queue filing table property register */
    u32 mrblr;      /* 0x.340 - Maximum Receive Buffer Length Register */
    //    u8  res14[56];
    //    u32 rbdbph;     /* 0x.37c - Rx data buffer pointer high */
    //    u8  res15a[4];
    u8  res14[60];
    u32 rbdbph;     /* 0x.380 - Rx data buffer pointer high */

    u32 rbptr0;     /* 0x.384 - RxBD pointer for ring 0 */
    u8  res15b[4];
    u32 rbptr1;     /* 0x.38c - RxBD pointer for ring 1 */
    u8  res15c[4];
    u32 rbptr2;     /* 0x.394 - RxBD pointer for ring 2 */
    u8  res15d[4];
    u32 rbptr3;     /* 0x.39c - RxBD pointer for ring 3 */
    u8  res15e[4];
    u32 rbptr4;     /* 0x.3a4 - RxBD pointer for ring 4 */
    u8  res15f[4];
    u32 rbptr5;     /* 0x.3ac - RxBD pointer for ring 5 */
    u8  res15g[4];
    u32 rbptr6;     /* 0x.3b4 - RxBD pointer for ring 6 */
    u8  res15h[4];
    u32 rbptr7;     /* 0x.3bc - RxBD pointer for ring 7 */
    u8  res16[64];
    u32 rbaseh;     /* 0x.400 - RxBD base address high */
    u32 rbase0;     /* 0x.404 - RxBD base address of ring 0 */
    u8  res17a[4];
    u32 rbase1;     /* 0x.40c - RxBD base address of ring 1 */
    u8  res17b[4];
    u32 rbase2;     /* 0x.414 - RxBD base address of ring 2 */
    u8  res17c[4];
    u32 rbase3;     /* 0x.41c - RxBD base address of ring 3 */
    u8  res17d[4];
    u32 rbase4;     /* 0x.424 - RxBD base address of ring 4 */
    u8  res17e[4];
    u32 rbase5;     /* 0x.42c - RxBD base address of ring 5 */
    u8  res17f[4];
    u32 rbase6;     /* 0x.434 - RxBD base address of ring 6 */
    u8  res17g[4];
    u32 rbase7;     /* 0x.43c - RxBD base address of ring 7 */
    u8  res17[192];
    u32 maccfg1;    /* 0x.500 - MAC Configuration 1 Register */
    u32 maccfg2;    /* 0x.504 - MAC Configuration 2 Register */
    u32 ipgifg;     /* 0x.508 - Inter Packet Gap/Inter Frame Gap Register */
    u32 hafdup;     /* 0x.50c - Half Duplex Register */
    u32 maxfrm;     /* 0x.510 - Maximum Frame Length Register */
    u8  res18[12];

    //  u8    gfar_mii_regs[24];  /* See gianfar_phy.h */
    u32 miimcfg;    /* MII Management: Configuration */
    u32 miimcom;    /* MII Management: Command */
    u32 miimadd;    /* MII Management: Address */
    u32 miimcon;    /* MII Management: Control */
    u32 miimstat;   /* MII Management: Status */
    u32 miimind;    /* MII Management: Indicators */    

    u8  res19[4];
    u32 ifstat;     /* 0x.53c - Interface Status Register */
    u32 macstnaddr1;    /* 0x.540 - Station Address Part 1 Register */
    u32 macstnaddr2;    /* 0x.544 - Station Address Part 2 Register */
    u32 mac01addr1; /* 0x.548 - MAC exact match address 1, part 1 */
    u32 mac01addr2; /* 0x.54c - MAC exact match address 1, part 2 */
    u32 mac02addr1; /* 0x.550 - MAC exact match address 2, part 1 */
    u32 mac02addr2; /* 0x.554 - MAC exact match address 2, part 2 */
    u32 mac03addr1; /* 0x.558 - MAC exact match address 3, part 1 */
    u32 mac03addr2; /* 0x.55c - MAC exact match address 3, part 2 */
    u32 mac04addr1; /* 0x.560 - MAC exact match address 4, part 1 */
    u32 mac04addr2; /* 0x.564 - MAC exact match address 4, part 2 */
    u32 mac05addr1; /* 0x.568 - MAC exact match address 5, part 1 */
    u32 mac05addr2; /* 0x.56c - MAC exact match address 5, part 2 */
    u32 mac06addr1; /* 0x.570 - MAC exact match address 6, part 1 */
    u32 mac06addr2; /* 0x.574 - MAC exact match address 6, part 2 */
    u32 mac07addr1; /* 0x.578 - MAC exact match address 7, part 1 */
    u32 mac07addr2; /* 0x.57c - MAC exact match address 7, part 2 */
    u32 mac08addr1; /* 0x.580 - MAC exact match address 8, part 1 */
    u32 mac08addr2; /* 0x.584 - MAC exact match address 8, part 2 */
    u32 mac09addr1; /* 0x.588 - MAC exact match address 9, part 1 */
    u32 mac09addr2; /* 0x.58c - MAC exact match address 9, part 2 */
    u32 mac10addr1; /* 0x.590 - MAC exact match address 10, part 1*/
    u32 mac10addr2; /* 0x.594 - MAC exact match address 10, part 2*/
    u32 mac11addr1; /* 0x.598 - MAC exact match address 11, part 1*/
    u32 mac11addr2; /* 0x.59c - MAC exact match address 11, part 2*/
    u32 mac12addr1; /* 0x.5a0 - MAC exact match address 12, part 1*/
    u32 mac12addr2; /* 0x.5a4 - MAC exact match address 12, part 2*/
    u32 mac13addr1; /* 0x.5a8 - MAC exact match address 13, part 1*/
    u32 mac13addr2; /* 0x.5ac - MAC exact match address 13, part 2*/
    u32 mac14addr1; /* 0x.5b0 - MAC exact match address 14, part 1*/
    u32 mac14addr2; /* 0x.5b4 - MAC exact match address 14, part 2*/
    u32 mac15addr1; /* 0x.5b8 - MAC exact match address 15, part 1*/
    u32 mac15addr2; /* 0x.5bc - MAC exact match address 15, part 2*/
    u8  res20[192];
    struct rmon_mib rmon;   /* 0x.680-0x.73c */
    u32 rrej;       /* 0x.740 - Receive filer rejected packet counter */
    u8  res21[188];
    u32 igaddr0;    /* 0x.800 - Indivdual/Group address register 0*/
    u32 igaddr1;    /* 0x.804 - Indivdual/Group address register 1*/
    u32 igaddr2;    /* 0x.808 - Indivdual/Group address register 2*/
    u32 igaddr3;    /* 0x.80c - Indivdual/Group address register 3*/
    u32 igaddr4;    /* 0x.810 - Indivdual/Group address register 4*/
    u32 igaddr5;    /* 0x.814 - Indivdual/Group address register 5*/
    u32 igaddr6;    /* 0x.818 - Indivdual/Group address register 6*/
    u32 igaddr7;    /* 0x.81c - Indivdual/Group address register 7*/
    u8  res22[96];
    u32 gaddr0;     /* 0x.880 - Group address register 0 */
    u32 gaddr1;     /* 0x.884 - Group address register 1 */
    u32 gaddr2;     /* 0x.888 - Group address register 2 */
    u32 gaddr3;     /* 0x.88c - Group address register 3 */
    u32 gaddr4;     /* 0x.890 - Group address register 4 */
    u32 gaddr5;     /* 0x.894 - Group address register 5 */
    u32 gaddr6;     /* 0x.898 - Group address register 6 */
    u32 gaddr7;     /* 0x.89c - Group address register 7 */
    u8  res23a[352];
    u32 fifocfg;    /* 0x.a00 - FIFO interface config register */
    u8  res23b[252];
    u8  res23c[248];
    u32 attr;       /* 0x.bf8 - Attributes Register */
    u32 attreli;    /* 0x.bfc - Attributes Extract Length and Extract Index Register */
    u8  res24[1024];
}tsec_t;

/* This flag currently only has
 * meaning if we're using the eTSEC */
#define TSEC_REDUCED (1 << 1)

struct gfar_extra_stats {
    u32 kernel_dropped;
    u32 rx_large;
    u32 rx_short;
    u32 rx_nonoctet;
    u32 rx_crcerr;
    u32 rx_overrun;
    u32 rx_bsy;
    u32 rx_babr;
    u32 rx_trunc;
    u32 eberr;
    u32 tx_babt;
    u32 tx_underrun;
    u32 rx_skbmissing;
    u32 tx_timeout;
};    

#endif /* __TSEC_H */
