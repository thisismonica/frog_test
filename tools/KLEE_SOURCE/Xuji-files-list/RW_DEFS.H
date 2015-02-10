
#ifndef     _RW_DEFS_H
#define     _RW_DEFS_H

#include "plat/inc/xj_cosc.h"
#include "plat/net_inc/lc_defs.h"
#include "plat_specific/plat_specific.h"
#include "plat/net_inc/ieddb.h"

#define GW_RW_SIM 0 /* 测试宏定义 */
#define RW_TARG_OP_TIMEOUT      100      /* 超时时间定义 */
#define SIGNED_16_MAX_POSITIVE (32767)   /* 16位最大正数 */
#define SIGNED_16_MIN_NEGATIVE (-32768)  /* 16位最大负数 */
#define GW_DIST_MAX_TEMP_DVAL  72        /* 最大点数 */

#define RW_MAX_DISTURBANCE         8      /* 最大扰动表个数 */
#define GW_RW_CIRCLE_TIME          20000  /* 周期(微妙) */
#define GW_RW_PRECISION            0.0005 /* 时间精度值 */
#define GW_TIME_MS_MAX             60000  /* 最大毫秒数 */
#define GW_TIME_MINUTE_MAX         59     /* 分钟最大值 */
#define GW_TIME_HOUR_MAX           23     /* 小时最大值 */
#define GW_RW_DTAG_WORD_BITS       32     /* 每组状态量个数 */
#define GW_RW_INVALID_CHL_INDEX    -1     /* 无效通道索引号 */

#define GW_RW_MAX_DTAG             24     /* 最大数据个数(初始值) */
#define GW_RW_MAX_LOGS             48     /* 最大逻辑数据个数(初始值) */
#define GW_RW_MAX_POINT            24     /* 最大采样点数(初始值) */
#define GW_DIST_MAX_TEMP_DTAG      96     /* 最大采样点数临时值(初始值) */

/* 录波进行到哪一步宏定义 */
#define GWDIST_M_STEP_READY_DATA   1 /* 启动故障录波数据上送命令 */
#define GWDIST_M_STEP_READY_DTAG   2 /* 请求发送数据 */
#define GWDIST_M_STEP_INIT_DTAG    3 /* 请求发送状态量 */
#define GWDIST_M_STEP_TRANS_DTAG   4 /* 状态量传送失败 */
#define GWDIST_M_STEP_SKIP_DTAG    5 /* 终止发送状态量 */
#define GWDIST_M_STEP_END_DTAG     6 /* 结束 */
#define GWDIST_M_STEP_SKIP_CHNL    7 /* 退出本路数据发送 */
#define GWDIST_M_STEP_INIT_CHNL    8 /* 本路模拟量发送失败 */
#define GWDIST_M_STEP_READY_CHNL   9  /* 准备通道数据 */
#define GWDIST_M_STEP_TRANS_CHNL   10 /* 发送通道数据 */
#define GWDIST_M_STEP_END_CHNL     11 /* 结束通道数据 */
#define GWDIST_M_STEP_ACK_CHNL     12 /* 请求通道数据 */
#define GWDIST_M_STEP_FINISH_TRANS 13 /* 终止数据传输 */
#define GWDIST_M_STEP_CONFIRM_EXIT 14 /* 退出 */
#define GWDIST_M_STEP_END_LOGS     15 /* 结束发送状态量 */
#define GWDIST_M_STEP_SKIP_LOGS    16 /* 放弃包括逻辑量的开关量 */

#if     (GW_RW_SIM)
#define GW_RW_READ          gwRW_Sim_Read
#define GW_RW_FIND          gwRW_Sim_Find
#else
#define GW_RW_READ          CWave.Read
#define GW_RW_FIND          CWave.Find
#endif

/* 状态量最大组数 */
#define GW_RW_MAX_DTAG_GROUP  \
    (CPC_MAX_PROTECT_GROUPS+CPC_MAX_INPUT_GROUPS+CPC_MAX_OUTPUT_GROUPS+CPC_MAX_PROTECT_LOG_GROUPS)
/* 最大状态量信息序号 */
#define GW_RW_MAX_DTAG_INF   (GW_RW_MAX_DTAG_GROUP*16)
typedef struct STRUCT_GW_RW_CB
{
    GW_REQUEST_CB   head;
    GW_UCHAR        rw_fun;
    GW_UCHAR        rw_TOO;
    GW_USHORT       rw_Fault_Number;
    GW_SHORT        rw_Step_M;

    UNSIGNED        rw_Transmit_Position;
    UNSIGNED        rw_Transmit_Backup;

    UNSIGNED        rw_DTAG_Max;
    UNSIGNED        rw_DTAG_Total;
    UNSIGNED        rw_DTAG_Group_Total;
    /*状态量掩码*/
    UNSIGNED        rw_DTAG_Mask[GW_RW_MAX_DTAG_GROUP];
    /*状态量备份数据*/
    UNSIGNED        rw_DTAG_Backup[GW_RW_MAX_DTAG_GROUP];
    /*状态量变位发送标志*/
    UNSIGNED        rw_DTAG_C_B[GW_RW_MAX_DTAG_GROUP];
    /*状态量当前数据*/
    UNSIGNED        rw_DTAG_D[GW_RW_MAX_DTAG_GROUP];
    /*状态量信息序号*/
    UNSIGNED        rw_DTAG_Inf_Group[GW_RW_MAX_DTAG_INF];
    /*采样点总个数*/
    UNSIGNED        rw_Total_Point;
    /*状态量当前发送位*/
    UNSIGNED        rw_DTAG_Bits;

    UNSIGNED        rw_CHNL_Total;
    GW_UCHAR        rw_chl_inf;
    INT             rw_CHNL_Index;
}GW_RW_CB;

typedef UNSIGNED (* DIST_GET_INF )( UNSIGNED group, UNSIGNED mask_bit );

#endif
