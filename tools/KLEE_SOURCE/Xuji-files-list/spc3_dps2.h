#ifndef _SPC3_DPS3_H
#define _SPC3_DPS3_H
#include "types.h"
#include "profidp.h"

#define PROFIDP_MAX_SSA_LEN         0
#define PROFIDP_MAX_CONF_LEN        16
#define PROFIDP_MAX_PARAM_LEN       16
#define PROFIDP_MAX_DIAG_DATA_LEN   32
#define PROFIDP_MAX_INPUT_LEN       128
#define PROFIDP_MAX_OUTPUT_LEN      128
#define PROFIDP_MAX_DATA_LEN        (PROFIDP_MAX_INPUT_LEN + PROFIDP_MAX_OUTPUT_LEN)

/*SPC3 board data*/
#define SPC3_MAX                        2
#define SPC3_IRQ_NUM                    ESAL_PR_IRQ4_INT_VECTOR_ID
#define SPC3_IRQ_STATUS_REG_ADDR        0xFA000030  /*w1c*/
#define SPC3_IRQ_MASK_REG_ADDR          0xFA000032      

#define SPC3_0_BASE         0xFA020000
#define SPC3_0_IRQ_MASK     (1<<0)

#define SPC3_1_BASE         0xFA030000
#define SPC3_1_IRQ_MASK     (1<<1)

int spc3dps2_init( struct profidp_cfg * cfgs, int n );
int spc3dps2_tx_input( int index, u8 * data );
int spc3dps2_tx_diag( int index, u8 * data, int len );

#endif /*_SPC3_DPS3_H*/
