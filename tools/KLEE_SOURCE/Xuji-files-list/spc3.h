#ifndef _SPC3_H
#define _SPC3_H

#include "types.h"

/*SPC3 register*/
#define REG_INT_REQ                           0x00 /* Int.-Request-Register */
#define REG_INT_REG_COM                       0x02 /* Int.-Register (read)*/
#define REG_INT_REG_ACK                       0x02 /* Int.-Acknowledge-Reg. (write)*/
#define REG_IS_REG_STATUS                     0x04 /* Status-Register (read) */
#define REG_IS_REG_MASK                       0x04 /* Interrupt-Mask-Register (write) */
#define REG_MODE_REG0                         0x06 
#define REG_DIN_BUFFER_SM                     0x08 
#define REG_NEW_DIN_BUF_CMD                   0x09 
#define REG_DOUT_BUFFER_SM                    0x0a 
#define REG_NEXT_DOUT_BUF_CMD                 0x0b 
#define REG_MODE_REG1_S                       0x08 
#define REG_MODE_REG1_R                       0x09 
#define REG_WD_BAUD_CTRL_VAL                  0x0a 
#define REG_MINTSDR_VAL                       0x0b 
#define REG_DIAG_BUFFER_SM                    0x0c 
#define REG_NEW_DIAG_BUFFER_CMD               0x0d 
#define REG_USER_PRM_DATA_OK                  0x0e 
#define REG_USER_PRM_DATA_NOK                 0x0f 
#define REG_USER_CFG_DATA_OK                  0x10 
#define REG_USER_CFG_DATA_NOK                 0x11 
#define REG_USER_DDB_PRM_DATA_OK              0x12 
#define REG_USER_DDB_PRM_DATA_NOK             0x13 
#define REG_SSA_BUFFER_FREE_CMD               0x14 
#define REG_RESERVED_15                       0x15 
#define REG_R_TS_ADR                          0x16 
#define REG_R_FDL_SAP_LIST_PTR                0x17 
#define REG_R_USER_WD_VALUE                   0x18 
#define REG_R_LEN_DOUT_BUF                    0x1a 
#define REG_R_DOUT_BUF_PTR                    0x1b 
#define REG_R_LEN_DIN_BUF                     0x1e 
#define REG_R_DIN_BUF_PTR                     0x1f 
#define REG_R_LEN_DIAG_BUF                    0x24 
#define REG_R_DIAG_BUF_PTR                    0x26 
#define REG_R_LEN_CNTRL_BUF                   0x28 
#define REG_R_AUX_BUF_SEL                     0x2a 
#define REG_R_AUX_BUF_PTR                     0x2b 
#define REG_R_LEN_SSA_BUF                     0x2d 
#define REG_R_SSA_BUF_PTR                     0x2e 
#define REG_R_LEN_PRM_BUF                     0x2f 
#define REG_R_PRM_BUF_PTR                     0x30 
#define REG_R_LEN_CFG_BUF                     0x31 
#define REG_R_CFG_BUF_PTR                     0x32 
#define REG_R_LEN_READ_CFG_BUF                0x33 
#define REG_R_READ_CFG_BUF_PTR                0x34 
#define REG_R_REAL_NO_ADD_CHANGE              0x39 
#define REG_R_IDENT_LOW                       0x3a 
#define REG_R_IDENT_HIGH                      0x3b 
#define REG_R_GC_COMMAND                      0x3c 
#define REG_R_LEN_SPEC_PRM_BUF                0x3d 
#define REG_RESERVED_3E_3F                    0x3e
#define REG_USER_SPACE_START                  0x40

/*Mode-Register 0*/
#define DIS_START_CONTROL   0x0001
#define DIS_STOP_CONTROL    0x0002
#define EN_FDL_DDB          0x0004
#define ENABLE_DEF_MINTSDR  0x0008
#define INT_POL_LOW         0x0000
#define INT_POL_HIGH        0x0010
#define EARLY_RDY           0x0020
#define SYNC_SUPPORTED      0x0040
#define FREEZE_SUPPORTED    0x0080
#define DP_MODE             0x0100
#define EOI_TIMEBASE_1u     0x0000
#define EOI_TIMEBASE_1m     0x0200
#define USER_TIMEBASE_1m    0x0000
#define USER_TIMEBASE_10m   0x0400
#define SPEC_PRM_BUF        0x1000
#define SPEC_CLEAR          0x2000

/*Mode-Register 1*/
#define START_SPC3          0x01
#define EOI_SPC3            0x02
#define GO_OFFLINE_SPC3     0x04
#define USER_LEAVE_MASTER   0x08
#define EN_CHG_CFG_BUFFER   0x10
#define RES_USER_WD         0x20

/*Status-Register 0*/
#define SPC3_OFFLINE_PASS   0x01
#define FDL_IND_ST          0x02
#define SPC3_DIAG_FLAG      0x04
#define ACCESS_VIOLATION    0x08

/*Diagnostic-control*/
#define SPC3_EXT_DIAG       0x01
#define SPC3_STAT_DIAG      0x02
#define SPC3_EXT_DIAG_OVF   0x04

/*Global-Control-command*/
#define SPC3_CLEAR_DATA     0x02
#define SPC3_UNFREEZE       0x04
#define SPC3_FREEZE         0x08
#define SPC3_UNSYNC         0x10
#define SPC3_SYNC           0x20

/*IR-Register*/
#define MAC_RESET             0x0001 /* Offline; SPC3 */
#define GO_LEAVE_DATA_EX      0x0002 /* Leave DataExcahnge; SPC3 */
#define BAUDRATE_DETECT       0x0004 /* Baudrate found; SPC3 */
#define WD_DP_MODE_TIMEOUT    0x0008 /* DP-Watchdog expired; SPC3 */
#define USER_TIMER_CLOCK      0x0010 /* Timer-Interval; SPC3 */
#define NEW_GC_COMMAND        0x0100 /* Global Control; SPC3 */
#define NEW_SSA_DATA          0x0200 /* Set Slave Address; SPC3 */
#define NEW_CFG_DATA          0x0400 /* Config Data; SPC3 */
#define NEW_PRM_DATA          0x0800 /* Param. Data; SPC3 */
#define DIAG_BUFFER_CHANGED   0x1000 /* Diagnostic-Buffer changed; SPC3 */
#define DX_OUT                0x2000 /* new output-data; SPC3 */

/*Return codes of SET_CFG_DATA.*/
#define SPC3_CFG_FINISHED     0x00
#define SPC3_CFG_CONFLICT     0x01
#define SPC3_CFG_NOT_ALLOWED  0x03

/*Return codes of SET_PRM_DATA.*/
#define SPC3_PRM_FINISHED     0x00
#define SPC3_PRM_CONFLICT     0x01
#define SPC3_PRM_NOT_ALLOWED  0x03

/*DP-Statemachine*/
#define SPC3_DP_STATE_WAIT_PRM  0x00
#define SPC3_DP_STATE_WAIT_CFG  0x01
#define SPC3_DP_STATE_DATA_EX   0x02
#define SPC3_DP_STATE_ERROR     0x03

/*WD-Statemachine*/
#define SPC3_WD_STATE_BAUD_SEARCH   0x00
#define SPC3_WD_STATE_BAUD_CONTROL  0x01
#define SPC3_WD_STATE_DP_MODE       0x02
#define SPC3_WD_STATE_ERROR         0x03

/*States of output buffer*/
#define NEW_DOUT_BUF      0x04
#define DOUT_BUF_CLEARED  0x08

/*Codes for Baud-Rate*/
#define BD_12M      0       /*  12    MBd */
#define BD_6M       1       /*   6    MBd */
#define BD_3M       2       /*   3    MBd */
#define BD_1_5M     3       /*   1.5  MBd */
#define BD_500k     4       /* 500    kBd */
#define BD_187_5k   5       /* 187.5  kBd */
#define BD_93_75k   6       /*  93.75 kBd */
#define BD_45_45k   7       /*  45.45 kBd */
#define BD_19_2k    8       /*  19.2  kBd */
#define BD_9_6k     9       /*   9.6  kBd */

#define ASS_AUX_BUF             3 /*cout of buffers assigned to AUX-buffer*/
#define SPC3_DIN_BUF_NUM        3
#define SPC3_DOUT_BUF_NUM       3
#define SPC3_SPACE_SIZE         0x600 /*1024 * 1.5k*/
#define SPC3_REG_SIZE           0x40
#define SPC3_USER_SPACE_SIZE    0x5c0 /*0x600 - 0x40*/

#define SPC3_BUF_START          0x40

/*Register access method*/
#define INTEL_MODE

#ifdef INTEL_MODE
#define _IML    0
#define _IMH    1
#else /*Motorola*/
#define _IML    1
#define _IMH    0
#endif

#define REG_WRITE_UBYTE(base, reg, val)     *(volatile u16 *)(base + ((reg)<<1) ) = (val) & 0xff
#define REG_READ_UBYTE(base, reg)           (u8)( (*(volatile u16 *)(base + ((reg) << 1)) ) & 0xff )
#define REG_WRITE_UWORD(base, reg, val)     do{ REG_WRITE_UBYTE(base, reg + _IML, val); REG_WRITE_UBYTE(base, reg + _IMH, (val) >> 8); }while(0)
#define REG_READ_UWORD(base, reg)           (u16)(REG_READ_UBYTE(base, reg + _IML) + (REG_READ_UBYTE(base, reg + _IMH) << 8 ))

/*Macros for initializations and hardware-dependent settings*/
#define SPC3_SET_HW_MODE(base, MODE)               REG_WRITE_UWORD(base, REG_MODE_REG0, (MODE) | DP_MODE & (~EN_FDL_DDB) )
#define SPC3_SET_IND(base, INDS)                   REG_WRITE_UWORD(base, REG_IS_REG_MASK,~(INDS))
#define SPC3_SET_STATION_ADDRESS(base, ADDRESS)    REG_WRITE_UBYTE(base, REG_R_TS_ADR, ADDRESS)
#define SPC3_SET_MINTSDR(base, MINTSDR)            REG_WRITE_UBYTE(base, REG_MINTSDR_VAL,MINTSDR)
#define SPC3_START(base)                           REG_WRITE_UBYTE(base, REG_MODE_REG1_S,START_SPC3)
#define SPC3_GO_OFFLINE(base)                      REG_WRITE_UBYTE(base, REG_MODE_REG1_S, GO_OFFLINE_SPC3 )
#define SPC3_SET_IDENT_NUMBER_LOW(base, NR)        REG_WRITE_UBYTE(base, REG_R_IDENT_LOW,NR)
#define SPC3_SET_IDENT_NUMBER_HIGH(base, NR)       REG_WRITE_UBYTE(base, REG_R_IDENT_HIGH,NR)
#define SPC3_SET_ADD_CHG_DISABLE(base)             REG_WRITE_UBYTE(base, REG_R_REAL_NO_ADD_CHANGE,0xff)
#define SPC3_SET_ADD_CHG_ENABLE(base)              REG_WRITE_UBYTE(base, REG_R_REAL_NO_ADD_CHANGE,0)
#define SPC3_USER_LEAVE_MASTER(base)               REG_WRITE_UBYTE(base, REG_MODE_REG1_S,USER_LEAVE_MASTER)

/*Macros for indications*/
#define SPC3_GET_INDICATION(base)           REG_READ_UWORD(base, REG_INT_REG_COM)
#define SPC3_IND_CONFIRM(base,CON)          REG_WRITE_UWORD(base, REG_INT_REG_ACK, CON)
#define SPC3_SET_EOI(base)                  REG_WRITE_UBYTE(base, REG_MODE_REG1_S,EOI_SPC3)
#define SPC3_POLL_INDICATION(base)          REG_READ_UWORD(base, REG_INT_REQ)

/*Macros for ponfiguration*/
#define SPC3_GET_CFG_LEN(base)              REG_READ_UBYTE(base, REG_R_LEN_CFG_BUF)
#define SPC3_GET_CFG_BUF_PTR(base)          (REG_READ_UBYTE(base,REG_R_CFG_BUF_PTR) << 3)
#define SPC3_SET_READ_CFG_LEN(base, LEN)    REG_WRITE_UBYTE(base,REG_R_LEN_READ_CFG_BUF,LEN)
#define SPC3_GET_READ_CFG_BUF_PTR(base)     (REG_READ_UBYTE(base,REG_R_READ_CFG_BUF_PTR) << 3)

/*Macros for parametrization*/
#define SPC3_GET_PRM_LEN(base)              REG_READ_UBYTE(base, REG_R_LEN_PRM_BUF)
#define SPC3_GET_PRM_BUF_PTR(base)          (REG_READ_UBYTE(base, REG_R_PRM_BUF_PTR) << 3)
#define SPC3_SET_PRM_DATA_OK(base)          REG_READ_UBYTE(base, REG_USER_PRM_DATA_OK)
#define SPC3_SET_PRM_DATA_NOT_OK(base)      REG_READ_UBYTE(base, REG_USER_PRM_DATA_NOK)

/*Macros for diagnosis*/
#define SPC3_GET_DIAG_FLAG(base)            (REG_READ_UBYTE(base, REG_IS_REG_STATUS + _IML) & SPC3_DIAG_FLAG)

/*Macros for Set-Slave-Address*/
#define SPC3_GET_SSA_LEN(base)              REG_READ_UBYTE(base, REG_R_LEN_SSA_BUF)
#define SPC3_GET_SSA_BUF_PTR(base)          (REG_READ_UBYTE(base, REG_R_SSA_BUF_PTR) << 3)
#define SPC3_FREE_SSA_BUF(base)             REG_READ_UBYTE(base, REG_SSA_BUFFER_FREE_CMD)

/*Macros for output*/
#define SPC3_OUTPUT_UPDATE(base)    (REG_READ_UBYTE(base, REG_R_DOUT_BUF_PTR + (REG_READ_UBYTE(base, REG_NEXT_DOUT_BUF_CMD) & 0x03) -1 ) << 3 )

/*Macros for input*/
#define SPC3_INPUT_UPDATE(base)     (REG_READ_UBYTE(base, REG_R_DIN_BUF_PTR + (REG_READ_UBYTE(base, REG_NEW_DIN_BUF_CMD) & 0x03) -1 ) << 3 )

/*Macros for Global-Control-command*/
#define SPC3_GET_GC_COMMAND(base)          REG_READ_UBYTE(base, REG_R_GC_COMMAND)

/*Macros for User-Watchdog*/
#define SPC3_SET_USER_WD_VALUE(base, VALUE) REG_WRITE_UWORD(base, REG_R_USER_WD_VALUE, VALUE)
#define SPC3_RESET_USER_WD(base)            REG_WRITE_UBYTE(base, REG_MODE_REG1_S,RES_USER_WD)

/*General macros*/
#define SPC3_GET_WD_STATE(base)             ((REG_READ_UBYTE(base, REG_IS_REG_STATUS + _IML) >> 6) & 0x03)
#define SPC3_GET_BAUD(base)                 (REG_READ_UBYTE(base, REG_IS_REG_STATUS + _IML) & 0x0f)
#define SPC3_GET_OFF_PASS(base)             (REG_READ_UBYTE(base, REG_IS_REG_STATUS + _IML) & SPC3_OFFLINE_PASS)
#define SPC3_GET_ACCESS_VIOLATION(base)     (REG_READ_UBYTE(base, REG_IS_REG_STATUS + _IML) & ACCESS_VIOLATION)
#define SPC3_SET_BAUD_CNTRL(base, VAL)      REG_WRITE_UBYTE(base, REG_WD_BAUD_CTRL_VAL,VAL)
#define SPC3_GET_DP_STATE(base)             ( ( REG_READ_UBYTE(base, REG_IS_REG_STATUS + _IML) >> 4 ) & 0x03)

typedef struct {
    u16 din_dout_buf_len; /* total len of I/O data */
    u8  diag_buf_len;     /* len of diagnosticdatabuffer */
    u8  prm_buf_len;      /* len of parameterbuffer */
    u8  cfg_buf_len;      /* len of config-databuffer */
    u8  ssa_buf_len;      /* len of set-slave-address-buffer */
} SPC3_BUFINIT;

typedef struct {
    u8 inp_data_len;
    u8 outp_data_len;
} SPC3_IO_DATA_LEN;

enum SPC3_INIT_RET {SPC3_INIT_OK, SPC3_INITF_LESS_MEM, SPC3_INITF_NOFF,
        SPC3_INITF_DIN_DOUT_LEN, SPC3_INITF_DIAG_LEN,
        SPC3_INITF_PRM_LEN, SPC3_INITF_CFG_LEN, SPC3_INITF_SSA_LEN,
        FDL_INITF_IVP, FDL_INITF_SCNT
};

struct spc3_dev
{
    u32 base_addr;
    SPC3_BUFINIT buf_cfg;
    u32 total_buf_len;
    u32 input_ptr;
    u32 output_ptr;
    u32 diag_ptr;
    u8 dp_state;
};

/*Function  prototypes*/
u8 spc3_cfg_data_ok( u32 base );
u8 spc3_cfg_data_update( u32 base );
u8 spc3_cfg_data_not_ok( u32 base );
u8 spc3_set_diag_len( u32 base,  u8 len );
u8 spc3_set_diag_state( u32 base, u8 state );
u8 spc3_set_io_data_len( struct spc3_dev * dev, SPC3_IO_DATA_LEN * len_ptr );
u32 spc3_diag_update( u32 base );
u32 spc3_get_diag_buf_ptr( u32 base );
u32 spc3_output_update_state( u32 base, u8 * state_ptr );
u32 spc3_get_din_buf_ptr( u32 base );
u32 spc3_get_free_mem( struct spc3_dev * dev, int * buf_len );
enum SPC3_INIT_RET spc3_buf_init( struct spc3_dev * dev, SPC3_BUFINIT * spc3_bptr, u8 spec_prm );
int spc3_calculate_inp_outp_len ( struct spc3_dev * dev, u8 * cfg_ptr, u8 cfg_len, SPC3_IO_DATA_LEN * io_data_len );
void spc3_buf_write( struct spc3_dev * dev, u32 offset, u8 * data, u32 len );
void spc3_buf_read( struct spc3_dev * dev, u8 * buf, u32 offset, u32 len );

#endif /*_SPC3_H*/
