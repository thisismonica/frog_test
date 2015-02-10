
/*************************************************************************/
/*                                                                       */
/*        Copyright (c) 1999-2002 XJ ELECTRIC CO.LTD.                    */
/*                                                                       */
/* PROPRIETARY RIGHTS of XJ ELECTRIC CO.LTD are involved in the          */
/* subject matter of this material.  All manufacturing, reproduction,    */
/* use, and sales rights pertaining to this subject matter are governed  */
/* by the license agreement.  The recipient of this software implicitly  */
/* accepts the terms of the license.                                     */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*                                                                       */
/* FILE NAME                                            VERSION          */
/*                                                                       */
/*      tab_defs.h                                      Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      TAB - table structure                                            */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains function prototypes of all functions          */
/*      accessible to other components.                                  */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* DATA STRUCTURES                                                       */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* FUNCTIONS                                                             */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhideYang.      03-23-2007      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/

#include "plat/inc/pro_defs.h"

/* Check to see if this file has been included already.  */
#ifndef TAB_DEFS_H
#define TAB_DEFS_H

/* 数据表的名称 */
#define TAB_DEVICE_NAME     "device"        /* 装置公用数据结构表 */
#define TAB_SETTING_NAME    "setting"       /* 整定值数据结构表 */
#define TAB_CTRL_NAME       "ctrl"          /* 控制字数据结构表 */
#define TAB_SJUMPER_NAME    "sjumper"       /* 软压板数据结构表 */
#define TAB_HJUMPER_NAME    "hjumper"       /* 硬压板数据结构表 */
#define TAB_TRIP_NAME       "trip"          /* 跳闸矩阵数据结构表 */
#define TAB_BREAKER_NAME    "breaker"       /* 断路器数据结构表 */
#define TAB_RSTATE_NAME     "rstate"        /* 遥信状态量数据结构表 */
#define TAB_RUN_DATA_NAME   "rundata"       /* 实时数据结构表 */
#define TAB_REPORT_NAME     "report"        /* 动作报告数据结构表 */
#define TAB_RVALUE_NAME     "RValue"        /* 动作报告量值数据结构表 */
#define TAB_SECTOR_NAME     "sector"        /* 装置保护扇区数据结构表 */
#define TAB_RJUMPER_NAME    "rjumper"       /* 装置检修压板数据结构表 */
#define TAB_SYS_SETTING_NAME "sys_setting"  /* 系统定值数据结构表 */
#define TAB_FCK_INFO_NAME   "FCK_Info"      /* 测控信息数据结构表 */
#define TAB_CHANNEL_NAME    "channel"       /* 装置模拟量通道数据结构表 */
#define TAB_INPUT_NAME      "input"         /* 装置开入量数据结构表 */
#define TAB_OUTPUT_NAME     "output"        /* 装置开出量数据结构表 */
#define TAB_STATE_NAME      "state"         /* 录波状态量数据结构表 */
#define TAB_PROTECT_NAME    "protect"       /* 保护数据结构表 */
#define TAB_YC_GROUP_NAME   "ycGroup"       /* 遥测量分组数据结构表 */
#define TAB_YK_NAME         "yk"            /* 遥控数据结构表 */
#define TAB_PLS_NAME        "pls"           /* 电度数据结构表 */
#define TAB_MENU_CAP_NAME   "MenuCap"       /* 菜单标题数据结构表 */
#define TAB_GROUP_CAP_NAME  "GroupCap"      /* 分组标题数据结构表 */
#define TAB_REC_NAME        "rec"           /* 录波数据结构表 */
#define TAB_YC_MEMBER_NAME  "ycMember"      /* 测量组成员数据结构表 */
#define TAB_LEDRS_NAME      "LEDRs"         /* 复归LED数据结构表 */
#define TAB_SOUTPUT_NAME    "SOutput"       /* 装置软开出量数据结构表 */
#define TAB_CMEMBER_NAME    "cMember"       /* 复合数据成员分组表 */
#define TAB_CACCESS_NAME    "cAccess"       /* 访问点数据结构表 */
#define TAB_ENUM_TYPE_NAME  "enumType"      /* 枚举类型数据结构表 */
#define TAB_ENUM_VALUE_NAME "enumValue"     /* 枚举值数据结构表 */
#define TAB_DESC_SET_NAME   "DescSet"       /* 描述定值数据结构表 */
#define TAB_RUN_PARA_SET_NAME "RunParaSet"  /* 运行参数定值数据结构表 */
#define TAB_CHARGE_NAME       "charge"        /* 运行参数定值数据结构表 */
#define TAB_NPI_PHY_SET_NAME  "npiPhySet"   /* NPI板块的网口物理设置数据结构表 */
#define TAB_CHARGECOM_NAME    "chargecom"   /* 充电桩通讯数据结构表 */

/* 数据表的索引 */
#define TAB_DEVICE_ID       1               /* 装置公用数据表索引 */
#define TAB_SETTING_ID      2               /* 整定值数据结构表索引 */
#define TAB_CTRL_ID         3               /* 控制字数据结构表索引 */
#define TAB_SJUMPER_ID      4               /* 软压板数据结构表索引 */
#define TAB_HJUMPER_ID      5               /* 硬压板数据结构表索引 */
#define TAB_TRIP_ID         6               /* 跳闸矩阵数据结构表索引 */
#define TAB_BREAKER_ID      7               /* 断路器数据结构表索引 */
#define TAB_RSTATE_ID       8               /* 遥信状态量数据结构表索引 */
#define TAB_RUN_DATA_ID     9               /* 实时数据结构表索引 */
#define TAB_REPORT_ID       10              /* 动作报告数据结构表索引 */
#define TAB_RVALUE_ID       11              /* 动作报告量值数据结构表索引 */
#define TAB_SYS_SET_ID      12              /* 系统定值数据结构表索引 */
#define TAB_SECTOR_ID       13              /* 保护扇区数据结构表索引 */
#define TAB_RJUMPER_ID      14              /* 检修压板数据结构表索引 */
#define TAB_FCK_INFO_ID     15              /* 测控信息数据结构表索引 */
#define TAB_CHANNEL_ID      16              /* 装置模拟量通道数据结构表索引 */
#define TAB_INPUT_ID        17              /* 装置开入量数据结构表索引 */
#define TAB_OUTPUT_ID       18              /* 装置开出量数据结构表索引 */
#define TAB_STATE_ID        19              /* 录波状态量数据结构表索引 */
#define TAB_PROTECT_ID      20              /* 保护数据结构表索引 */
#define TAB_YC_GROUP_ID     21              /* 遥测量分组数据结构表索引 */
#define TAB_YK_ID           22              /* 遥控数据结构表索引 */
#define TAB_PLS_ID          23              /* 电度数据结构表索引 */
#define TAB_MENU_CAP_ID     24              /* 菜单标题数据结构表索引 */
#define TAB_GROUP_CAP_ID    25              /* 分组标题数据结构表索引 */
#define TAB_REC_ID          26              /* 录波数据结构表索引 */
#define TAB_YC_MEMBER_ID    27              /* 测量组成员数据结构表索引 */
#define TAB_LEDRS_ID        28              /* 复归LED数据结构表索引 */
#define TAB_SOUTPUT_ID      29              /* 装置软开出量数据结构表索引 */
#define TAB_CMEMBER_ID      30              /* 复合数据成员分组表索引 */
#define TAB_CACCESS_ID      31              /* 访问点数据结构表索引 */
#define TAB_ENUM_TYPE_ID    32              /* 枚举类型数据结构表索引 */
#define TAB_ENUM_VALUE_ID   33              /* 枚举值数据结构表索引 */
#define TAB_DESC_SET_ID     34              /* 描述定值数据结构表索引 */
#define TAB_RUN_PARA_SET_ID 35              /* 运行参数定值数据结构表索引 */
#define TAB_CHARGE_ID       36              /* 充电桩数据结构表索引 */
#define TAB_NPI_PHY_SET_ID  37              /* NPI板块的网口物理设置数据结构表索引 */
#define TAB_CHARGECOM_ID    38              /* 充电桩通讯数据结构表索引 */

/* 装置公用数据结构表成员名称 */
#define DEV_CNAME_NAME      "chinese"       /* Device_CName */
#define DEV_ENAME_NAME      "english"       /* Device_EName */
#define DEV_SOE_MODE        "SoeMode"       /* soe_mode */
#define DEV_PRINT_DELAY     "PDelayTime"    /* print_delay_time */
#define DEV_RESET_BIT       "ResetBit"      /* reset_bit */
#define DEV_COPY_SET_LOGIC  "CopySetLog"    /* copy_set_logic */
#define DEV_OTL_DIFF_LOGIC  "OtlDiffLog"    /* otl_diff_logic */
#define DEV_VERL            "verl"          /* Ver_Low */
#define DEV_VERH            "verh"          /* Ver_High */
#define DEV_CPU_NUM         "number"        /* Cpu_Number */
#define DEV_CPU_TYPE        "type"          /* Cpu_Type */
#define DEV_CPU_ZONE        "zone"          /* Cpu_Zone */
#define DEV_CPU_ZONE_VARINF "zoneVarinf"    /* Cpu_Zone_varinf */
#define DEV_T_SEC           "tSec"          /* t_sec */
#define DEV_T_FRAC          "tFrac"         /* t_frac */
#define DEV_T_Q_FLAG        "tQflag"        /* t_q_flag */
#define DEV_TIME_TYPE       "TimeType"      /* TimeType */
#define DEV_BREAKER_ENABLE  "BreakEnb"      /* BreakerEnable */
#define DEV_TIME_ZONE       "TimeZone"      /* TimeZone */
#define DEV_MIN_SET_ZONE    "MinSetZone"    /* MinSetZone */
#define DEV_SET_ZONE_NUM    "SetZoneNum"    /* SetZoneNum */
#define DEV_CUR_SET_ZONE    "CurSetZone"    /* CurSetZone */
#define DEV_EXT_RESET_BIT   "ExtResetBit"   /* ExtResetBit */
#define DEV_RULES_SELECT    "RulesSelect"   /* RulesSelect */
#define DEV_DEVICE_FUNCTYPE "FuncType"      /* FuncType */
#define DEV_DEVICE_FUNCSELECT "FuncSelect"  /* FuncSelect */
#define DEV_DEVICE_DESC_ID  "desc_id"       /* desc_id  */
#define DEV_DEVICE_YEAR     "Year"          /* Year  */
#define DEV_DEVICE_MONTH    "Month"         /* Month */
#define DEV_DEVICE_DAY      "Day"           /* Day */
#define DEV_DEVICE_HOUR     "Hour"          /* Hour */
#define DEV_DEVICE_MINUTE   "Minute"        /* Minute */
#define DEV_DEVICE_SECOND   "Second"        /* Second */
#define DEV_PARA_SHOW_TYPE  "ParaSHType"    /* ParaSHType  */
#define DEV_SETT_SET_TYPE   "SettSTType"    /* SettSTType  */
#define DEV_SET_CHINSE_CODE_GP "SetCCodeGp"    /* SetCCodeGp  */
#define DEV_SAMPLE_POINT       "SamplePoint" /* SamplePoint  */
#define DEV_SMV_BREAK_START_IN_BIT   "SmvBSInBit"  /* SmvBSInBit  */
#define DEV_GOOSE_BREAK_START_IN_BIT "GosBSInBit"  /* GosBSInBit  */
#define DEV_VLDTAG                   "vldTag"      /* vldTag */
#define DEV_HARD_IN_GR_NUM           "HInGrNum"    /* HInGrNum */
#define DEV_NPI_TYPE                 "NPIType"     /* NPI类型  */
#define DEV_HARD_OUT_GR_NUM          "HOutGrNum"   /* HOutGrNum */
#define DEV_SYN_SRC                  "SynSrc"      /* SynSrc */

/* 装置公用数据结构表成员ID */
#define DEV_CNAME_NAME_INDEX      1         /* Device_CName */
#define DEV_ENAME_NAME_INDEX      2         /* Device_EName */
#define DEV_SOE_MODE_INDEX        3         /* soe_mode */
#define DEV_PRINT_DELAY_INDEX     4         /* print_delay_time */
#define DEV_RESET_BIT_INDEX       5         /* reset_bit */
#define DEV_COPY_SET_LOGIC_INDEX  6         /* copy_set_logic */
#define DEV_OTL_DIFF_LOGIC_INDEX  7         /* otl_diff_logic */
#define DEV_VERL_INDEX            8         /* Ver_Low */
#define DEV_VERH_INDEX            9         /* Ver_High */
#define DEV_CPU_NUM_INDEX         10        /* Cpu_Number */
#define DEV_CPU_TYPE_INDEX        11        /* Cpu_Type */
#define DEV_CPU_ZONE_INDEX        12        /* Cpu_Zone */
#define DEV_CPU_ZONE_VARINF_INDEX 13        /* Cpu_Zone_varinf */
#define DEV_T_SEC_INDEX           14        /* t_sec */
#define DEV_T_FRAC_INDEX          15        /* t_frac */
#define DEV_T_Q_FLAG_INDEX        16        /* t_q_flag */
#define DEV_TIME_TYPE_INDEX       17        /* TimeType */
#define DEV_BREAKER_ENABLE_INDEX  18        /* BreakerEnable */
#define DEV_TIME_ZONE_INDEX       19        /* TimeZone */
#define DEV_MIN_SET_ZONE_INDEX    20        /* MinSetZone */
#define DEV_SET_ZONE_NUM_INDEX    21        /* SetZoneNum */
#define DEV_CUR_SET_ZONE_INDEX    22        /* CurSetZone */
#define DEV_EXT_RESET_BIT_INDEX   23        /* ExtResetBit */
#define DEV_RULES_SELECT_INDEX    24        /* RulesSelect */
#define DEV_FUNC_TYPE_INDEX       25        /* FuncType */
#define DEV_FUNC_SELECT_INDEX     26        /* FuncSelect */
#define DEV_DEVICE_DESC_INDEX     27        /* desc_id */
#define DEV_DEVICE_YEAR_INDEX     28        /* Year  */
#define DEV_DEVICE_MONTH_INDEX    29        /* Month */
#define DEV_DEVICE_DAY_INDEX      30        /* Day */
#define DEV_DEVICE_HOUR_INDEX     31        /* Hour */
#define DEV_DEVICE_MINUTE_INDEX   32        /* Minute */
#define DEV_DEVICE_SECOND_INDEX   33        /* Second */
#define DEV_PARA_SHOW_TYPE_INDEX  34        /* ParaShowType */
#define DEV_SETT_SET_TYPE_INDEX   35        /* SettSetType */
#define DEV_SET_CHINSE_CODE_GP_INDEX   36   /* SetCCodeGp */
#define DEV_SAMPLE_POINT_INDE     37        /* SamplePoint  */
#define DEV_SMV_BREAK_START_IN_BIT_INDE   38  /* SmvBSInBit  */
#define DEV_GOOSE_BREAK_START_IN_BIT_INDE 39  /* GosBSInBit  */
#define DEV_VLDTAG_INDEX          40          /* vldTag */
#define DEV_HARD_IN_GR_NUM_INDEX  41          /* HInGrNum */
#define DEV_NPI_TYPE_INDEX        42          /* NPI类型  */
#define DEV_HARD_OUT_GR_NUM_INDEX 43          /* HOutGrNum */
#define DEV_SYN_SRC_INDEX         44          /* SynSrc */

/* 定值数据结构表成员名称 */
#define SETN_PRO_ID         "proId"         /* pro_id */
#define SETN_SEC_ID         "secId"         /* sec_id */
#define SETN_CTR_ID         "CtrId"         /* ctr_id */
#define SETN_ID             "SetId"         /* set_id */
#define SETN_AVAILABLE      "available"     /* set_available */
#define SETN_NAME           "name"          /* set_name */
#define SETN_CALCULATE_TYPE "Caltype"       /* set_calculate_style */
#define SETN_DIMENSION      "dimension"     /* set_dimension */
#define SETN_ALIAS          "alias"         /* set_alias */
#define SETN_TYPE           "type"          /* set_type */
#define SETN_ENUM_TYPE      "enum_type"     /* set_enum_type */
#define SETN_COEFFICIENT    "coefficient"   /* set_calculate_coefficient */
#define SETN_LOWER_LIMIT    "LowerLimit"    /* set_lower_limit */
#define SETN_HIGH_LIMIT     "HighLimit"     /* set_high_limit */
#define SETN_LOWER_CFG      "LowerCfg"      /* set_lower_cfg */
#define SETN_HIGH_CFG       "HighCfg"       /* set_high_cfg */
#define SETN_STEP_SIZE      "stepSize"      /* set_stepSize */
#define SETN_COEFF_PRI      "CoeffPri"      /* set_calculate_coeff_pri */
#define SETN_LOWER_PRI      "LowerPri"      /* set_lower_limit_pri */
#define SETN_HIGH_PRI       "HighPri"       /* set_high_limit_pri */
#define SETN_STEP_SIZE_PRI  "StepSizePri"   /* set_stepSize_pri */
#define SETN_DIMEN_PRI      "DimenPri"      /* set_dimension_pri */
#define SETN_DO_NAME        "doName"        /* set_doName */
#define SETN_ASSOCIATE_CHL  "associaChl"    /* set_associate_channel */
#define SETN_ASSOCIATE_TYPE "associaType"   /* set_associate_type */
#define SETN_MEMBER_ID      "memId"         /* set_memId */
#define SET_CONVERSION_TYPE "conversType"   /* set_conversion_type */
#define SETN_LIMIT_COEFF    "LimitCoeff"    /* set_limit_coefficient */
#define SETN_VLDTAG         "vldTag"        /* vldTag */
#define SETN_VALUE          "value"         /* set_value */

/* 定值数据结构表成员ID */
#define SET_PROID_INDEX           1         /* pro_id */
#define SET_SECID_INDEX           2         /* sec_id */
#define SET_CTRID_INDEX           3         /* ctr_id */
#define SET_ID_INDEX              4         /* set_id */
#define SET_AVAILABLE_INDEX       5         /* set_available */
#define SET_NAME_INDEX            6         /* set_name */
#define SET_CALTYPE_INDEX         7         /* set_calculate_style */
#define SET_DIMENSIO_INDEX        8         /* set_dimension */
#define SET_ALIAS_INDEX           9         /* set_alias */
#define SET_TYPE_INDEX            10        /* set_type */
#define SET_ENUM_TYPE_INDEX       11        /* set_enum_type */
#define SET_COEF_INDEX            12        /* set_calculate_coefficient */
#define SET_LLIMIT_INDEX          13        /* set_lower_limit */
#define SET_HLIMIT_INDEX          14        /* set_high_limit */
#define SET_LLIMIT_CFG_INDEX      15        /* set_lower_cfg */
#define SET_HLIMIT_CFG_INDEX      16        /* set_high_cfg */
#define SET_STEP_SIZE_INDEX       17        /* set_stepSize */
#define SETN_COEFF_PRI_INDEX      18        /* set_calculate_coeff_pri */
#define SETN_LOWER_PRI_INDEX      19        /* set_lower_limit_pri */
#define SETN_HIGH_PRI_INDEX       20        /* set_high_limit_pri */
#define SETN_STEP_SIZE_PRI_INDEX  21        /* set_stepSize_pri */
#define SETN_DIMEN_PRI_INDEX      22        /* set_dimension_pri */
#define SET_DO_NAME_INDEX         23        /* set_doName */
#define SET_ASSOCIATE_CHL_INDEX   24        /* set_associate_channel */
#define SET_ASSOCIATE_TYPE_INDEX  25        /* set_associate_type */
#define SET_MEMBER_ID_INDEX       26        /* set_memId */
#define SET_CONVERSION_TYPE_INDEX 27        /* set_conversion_type */
#define SET_LIMIT_COEFF_INDEX     28        /* set_limit_coefficient */
#define SETN_VLDTAG_INDEX         29        /* vldTag */
#define SET_VALUE_INDEX           101       /* set_value */

/* 系统定值数据结构表成员名称 */
#define SSETN_PRO_ID         "proId"        /* pro_id */
#define SSETN_SEC_ID         "secId"        /* sec_id */
#define SSETN_ID             "SetId"        /* set_id */
#define SSETN_AVAILABLE      "available"    /* set_available */
#define SSETN_NAME           "name"         /* set_name */
#define SSETN_CALCULATE_TYPE "Caltype"      /* set_calculate_style */
#define SSETN_DIMENSION      "dimension"    /* set_dimension */
#define SSETN_ALIAS          "alias"        /* set_alias */
#define SSETN_TYPE           "type"         /* set_type */
#define SSETN_ENUM_TYPE      "enum_type"    /* set_enum_type */
#define SSETN_COEFFICIENT    "coefficient"  /* set_calculate_coefficient */
#define SSETN_LOWER_LIMIT    "LowerLimit"   /* set_lower_limit */
#define SSETN_HIGH_LIMIT     "HighLimit"    /* set_high_limit */
#define SSETN_VALUE          "value"        /* set_value */
#define SSETN_STEP_SIZE      "stepSize"     /* set_stepSize */
#define SSETN_DO_NAME        "doName"       /* set_doName */
#define SSETN_ID_TYPE        "IdType"       /* set_id_type */
#define SSETN_VLDTAG         "vldTag"       /* vldTag */

/* 系统定值数据结构表成员ID */
#define SSET_PRO_ID_INDEX         1         /* pro_id */
#define SSET_SEC_ID_INDEX         2         /* sec_id */
#define SSET_ID_INDEX             3         /* set_id */
#define SSET_AVAILABLE_INDEX      4         /* set_available */
#define SSET_NAME_INDEX           5         /* set_name */
#define SSET_CALTYPE_INDEX        6         /* set_calculate_style */
#define SSET_DIMENSIO_INDEX       7         /* set_dimension */
#define SSET_ALIAS_INDEX          8         /* set_alias */
#define SSET_TYPE_INDEX           9         /* set_type */
#define SSET_ENUM_TYPE_INDEX      10        /* set_enum_type */
#define SSET_COEF_INDEX           11        /* set_calculate_coefficient */
#define SSET_LLIMIT_INDEX         12        /* set_lower_limit */
#define SSET_HLIMIT_INDEX         13        /* set_high_limit */
#define SSET_VALUE_INDEX          14        /* set_value */
#define SSET_STEP_SIZE_INDEX      15        /* set_stepSize */
#define SSET_DO_NAME_INDEX        16        /* set_doName */
#define SSET_ID_TYPE              17        /* set_associate_type */
#define SSETN_VLDTAG_INDEX        18        /* vldTag */

/* 运行参数定值数据结构表成员名称 */
#define RUN_PARA_PRO_ID         "proId"        /* pro_id */
#define RUN_PARA_SEC_ID         "secId"        /* sec_id */
#define RUN_PARA_ID             "SetId"        /* set_id */
#define RUN_PARA_DESC_ID        "desc_id"      /* desc_id */
#define RUN_PARA_AVAILABLE      "available"    /* set_available */
#define RUN_PARA_NAME           "name"         /* set_name */
#define RUN_PARA_CALCULATE_TYPE "Caltype"      /* set_calculate_style */
#define RUN_PARA_DIMENSION      "dimension"    /* set_dimension */
#define RUN_PARA_ALIAS          "alias"        /* set_alias */
#define RUN_PARA_TYPE           "type"         /* set_type */
#define RUN_PARA_ENUM_TYPE      "enum_type"    /* set_enum_type */
#define RUN_PARA_COEFFICIENT    "coefficient"  /* set_calculate_coefficient */
#define RUN_PARA_LOWER_LIMIT    "LowerLimit"   /* set_lower_limit */
#define RUN_PARA_HIGH_LIMIT     "HighLimit"    /* set_high_limit */
#define RUN_PARA_VALUE          "value"        /* set_value */
#define RUN_PARA_STEP_SIZE      "stepSize"     /* set_stepSize */
#define RUN_PARA_VLDTAG         "vldTag"       /* vldTag */

/* 运行参数定值数据结构表成员ID */
#define RUN_PARA_PRO_ID_INDEX         1         /* pro_id */
#define RUN_PARA_SEC_ID_INDEX         2         /* sec_id */
#define RUN_PARA_ID_INDEX             3         /* set_id */
#define RUN_PARA_DESC_ID_INDEX        4         /* desc_id */
#define RUN_PARA_AVAILABLE_INDEX      5         /* set_available */
#define RUN_PARA_NAME_INDEX           6         /* set_name */
#define RUN_PARA_CALTYPE_INDEX        7         /* set_calculate_style */
#define RUN_PARA_DIMENSIO_INDEX       8         /* set_dimension */
#define RUN_PARA_ALIAS_INDEX          9         /* set_alias */
#define RUN_PARA_TYPE_INDEX           10        /* set_type */
#define RUN_PARA_ENUM_TYPE_INDEX      11        /* set_enum_type */
#define RUN_PARA_COEF_INDEX           12        /* set_calculate_coefficient */
#define RUN_PARA_LLIMIT_INDEX         13        /* set_lower_limit */
#define RUN_PARA_HLIMIT_INDEX         14        /* set_high_limit */
#define RUN_PARA_VALUE_INDEX          15        /* set_value */
#define RUN_PARA_STEP_SIZE_INDEX      16        /* set_stepSize */
#define RUN_PARA_VLDTAG_INDEX         17        /* vldTag */

/* 控制字定值数据结构表成员名称 */
#define CTRN_PRO_ID         "proId"         /* pro_id */
#define CTRN_SEC_ID         "secId"         /* sec_id */
#define CTRN_CTR_ID         "CtrId"         /* ctr_id */
#define CTRN_BITS           "bits"          /* ctr_bits */
#define CTRN_VLDTAG         "vldTag"        /* vldTag */
#define CTRN_NAME           "name"          /* ctr_name */

/* 控制字定值数据结构表成员ID */
#define CTRN_PRO_ID_INDEX         1         /* pro_id */
#define CTRN_SEC_ID_INDEX         2         /* sec_id */
#define CTRN_CTR_ID_INDEX         3         /* ctr_id */
#define CTRN_BITS_INDEX           4         /* ctr_bits */
#define CTRN_VLDTAG_INDEX         5         /* vldTag */
#define CTRN_NAME_INDEX           6         /* ctr_name */

/* 软压板数据结构表成员名称 */
#define SJPN_PRO_ID         "proId"         /* pro_id */
#define SJPN_SEC_ID         "secId"         /* sec_id */
#define SJPN_ID             "JumId"         /* jum_id */
#define SJPN_DESC_ID        "desc_id"       /* desc_id */
#define SJPN_AVAILABLE      "available"     /* jum_available */
#define SJPN_INF            "inf"           /* jum_inf */
#define SJPN_VARINF         "VarInf"        /* jum_variinf */
#define SJPN_NAME           "name"          /* jum_name */
#define SJPN_ALIAS          "alias"         /* jum_alias */
#define SJPN_STATUS         "status"        /* jum_status */
#define SJPN_PROSTATUS      "Prostatus"     /* pro_status */
#define SJPN_DO             "doName"        /* DO */
#define SJPN_Q              "Q"             /* q */
#define SJPN_T_SEC          "tSec"          /* t_sec */
#define SJPN_T_FRAC         "tFrac"         /* t_frac */
#define SJPN_T_Q_FLAG       "tQflag"        /* t_q_flag */
#define SJPN_SUB_ENA        "subEna"        /* subEna */
#define SJPN_SUB_VAL        "subVal"        /* subVal */
#define SJPN_SUB_Q          "subQ"          /* subQ */
#define SJPN_SUB_ID         "subID"         /* subID */
#define SJPN_CTL_MODEL      "ctlModel"      /* ctlModel */
#define SJPN_OPER_CTL_VAL   "operCtlVal"    /* Oper_ctlVal */
#define SJPN_OPER_ORIGIN    "operOrCat"    /* Oper_origin */
#define SJPN_OPER_ORIGIN_IDENT "operOrId" /* Oper_originIdent */
#define SJPN_OPER_CTL_NUM   "operCtlNum"    /* Oper_ctlNum */
#define SJPN_OPER_T_SEC     "operTSec"      /* Oper_T_sec */
#define SJPN_OPER_T_FRAC    "operTFrac"     /* Oper_T_frac */
#define SJPN_OPER_T_QFLAG   "operTQflag"    /* Oper_T_qflag */
#define SJPN_OPER_TEST      "operTest"      /* Oper_Test */
#define SJPN_OPER_CHECK     "operCheck"     /* Oper_Check */
#define SJPN_SELECT         "select"        /* jum_select */
#define SJPN_CANAEL         "cancel"        /* jum_cancel */
#define SJPN_DATA_TYPE      "dataType"      /* dataType */
#define SJPN_GROUP          "group"         /* group */
#define SJPN_VLDTAG         "vldTag"        /* vldTag */

/* 软压板数据结构表成员ID */
#define SJPN_PRO_ID_INDEX         1         /* pro_id */
#define SJPN_SEC_ID_INDEX         2         /* sec_id */
#define SJPN_ID_INDEX             3         /* jum_id */
#define SJPN_DESC_ID_INDEX        4         /* desc_id */
#define SJPN_AVAILABLE_INDEX      5         /* jum_available */
#define SJPN_INF_INDEX            6         /* jum_inf */
#define SJPN_VARINF_INDEX         7         /* jum_variinf */
#define SJPN_NAME_INDEX           8         /* jum_name */
#define SJPN_ALIAS_INDEX          9         /* jum_alias */
#define SJPN_STATUS_INDEX         10        /* jum_status */
#define SJPN_PROSTATUS_INDEX      11        /* pro_status */
#define SJPN_DO_INDEX             12        /* DO */
#define SJPN_Q_INDEX              13        /* q */
#define SJPN_T_SEC_INDEX          14        /* t_sec */
#define SJPN_T_FRAC_INDEX         15        /* t_frac */
#define SJPN_T_Q_FLAG_INDEX       16        /* t_q_flag */
#define SJPN_SUB_ENA_INDEX        17        /* subEna */
#define SJPN_SUB_VAL_INDEX        18        /* subVal */
#define SJPN_SUB_Q_INDEX          19        /* subQ */
#define SJPN_SUB_ID_INDEX         20        /* subID */
#define SJPN_CTL_MODEL_INDEX      21        /* ctlModel */
#define SJPN_OPER_CTL_VAL_INDEX   22        /* Oper_ctlVal */
#define SJPN_OPER_ORIGIN_INDEX    23        /* Oper_origin */
#define SJPN_OPER_ORIGIN_IDENT_INDEX 24     /* Oper_originIdent */
#define SJPN_OPER_CTL_NUM_INDEX   25        /* Oper_ctlNum */
#define SJPN_OPER_T_SEC_INDEX     26        /* Oper_T_sec */
#define SJPN_OPER_T_FRAC_INDEX    27        /* Oper_T_frac */
#define SJPN_OPER_T_QFLAG_INDEX   28        /* Oper_T_qflag */
#define SJPN_OPER_TEST_INDEX      29        /* Oper_Test */
#define SJPN_OPER_CHECK_INDEX     30        /* Oper_Check */
#define SJPN_SELECT_INDEX         31        /* jum_select */
#define SJPN_CANAEL_INDEX         32        /* jum_cancel */
#define SJPN_DATA_TYPE_INDEX      33        /* dataType */
#define SJPN_GROUP_INDEX          34        /* group */
#define SJPN_VLDTAG_INDEX         35        /* vldTag */

/* 硬压板数据结构表成员名称 */
#define HJPN_PRO_ID         "proId"         /* pro_id */
#define HJPN_SEC_ID         "secId"         /* sec_id */
#define HJPN_ID             "JumId"         /* jum_id */
#define HJPN_DESC_ID        "desc_id"       /* desc_id */
#define HJPN_AVAILABLE      "available"     /* jum_available */
#define HJPN_BITS           "bits"          /* jum_bits */
#define HJPN_VARINF         "VarInf"        /* jum_variinf */
#define HJPN_NAME           "name"          /* jum_name */
#define HJPN_ALIAS          "alias"         /* jum_alias */
#define HJPN_STATUS         "status"        /* jum_status */
#define HJPN_DO             "doName"        /* DO */
#define HJPN_Q              "Q"             /* q */
#define HJPN_T_SEC          "tSec"          /* t_sec */
#define HJPN_T_FRAC         "tFrac"         /* t_frac */
#define HJPN_T_Q_FLAG       "tQflag"        /* t_q_flag */
#define HJPN_SUB_ENA        "subEna"        /* subEna */
#define HJPN_SUB_VAL        "subVal"        /* subVal */
#define HJPN_SUB_Q          "subQ"          /* subQ */
#define HJPN_SUB_ID         "subID"         /* subID */
#define HJPN_GROUP          "group"         /* group */
#define HJPN_VLDTAG         "vldTag"        /* vldTag */

/* 硬压板数据结构表成员ID */
#define HJPN_PRO_ID_INDEX         1         /* pro_id */
#define HJPN_SEC_ID_INDEX         2         /* sec_id */
#define HJPN_ID_INDEX             3         /* jum_id */
#define HJPN_DESC_ID_INDEX        4         /* desc_id */
#define HJPN_AVAILABLE_INDEX      5         /* jum_available */
#define HJPN_BITS_INDEX           6         /* jum_bits */
#define HJPN_VARINF_INDEX         7         /* jum_variinf */
#define HJPN_NAME_INDEX           8         /* jum_name */
#define HJPN_ALIAS_INDEX          9         /* jum_alias */
#define HJPN_STATUS_INDEX         10        /* jum_status */
#define HJPN_DO_INDEX             11        /* DO */
#define HJPN_Q_INDEX              12        /* q */
#define HJPN_T_SEC_INDEX          13        /* t_sec */
#define HJPN_T_FRAC_INDEX         14        /* t_frac */
#define HJPN_T_Q_FLAG_INDEX       15        /* t_q_flag */
#define HJPN_SUB_ENA_INDEX        16        /* subEna */
#define HJPN_SUB_VAL_INDEX        17        /* subVal */
#define HJPN_SUB_Q_INDEX          18        /* subQ */
#define HJPN_SUB_ID_INDEX         19        /* subID */
#define HJPN_GROUP_INDEX          20        /* group */
#define HJPN_VLDTAG_INDEX         21        /* vldTag */

/* 跳闸矩阵数据结构表成员名称 */
#define TRIPN_PRO_ID        "proId"         /* pro_id */
#define TRIPN_SEC_ID        "secId"         /* sec_id */
#define TRIPN_ID            "TripId"        /* trip_id */
#define TRIPN_AVAILABLE     "available"     /* trip_available */
#define TRIPN_ENABLE_BITS   "EnableBits"    /* trip_bits_enable */
#define TRIPN_NAME          "name"          /* trip_name */
#define TRIPN_ALIAS         "alias"         /* trip_alias */
#define TRIPN_TYPE          "type"          /* trip_type */
#define TRIPN_FLAG          "flag"          /* trip_flag */
#define TRIPN_HFLAG         "hflag"         /* trip_hflag */
#define TRIPN_START_FLAG    "Startflag"     /* trip_startup_flag */
#define TRIPN_BREAKER_BITS  "BreakerBits"   /* trip_bits */
#define TRIPN_BREAKER_VLDTAG "vldTag"        /* vldTag */

/* 跳闸矩阵数据结构表成员ID */
#define TRIPN_PRO_ID_INDEX        1         /* pro_id */
#define TRIPN_SEC_ID_INDEX        2         /* sec_id */
#define TRIPN_ID_INDEX            3         /* trip_id */
#define TRIPN_AVAILABLE_INDEX     4         /* trip_available */
#define TRIPN_ENABLE_BITS_INDEX   5         /* trip_bits_enable */
#define TRIPN_NAME_INDEX          6         /* trip_name */
#define TRIPN_ALIAS_INDEX         7         /* trip_alias */
#define TRIPN_TYPE_INDEX          8         /* trip_type */
#define TRIPN_FLAG_INDEX          9         /* trip_flag */
#define TRIPN_HFLAG_INDEX         10        /* trip_hflag */
#define TRIPN_START_FLAG_INDEX    11        /* trip_startup_flag */
#define TRIPN_BREAKER_BITS_INDEX  12        /* trip_bits */
#define TRIPN_BREAKER_VLDTAG_INDEX 13       /* vldTag */

/* 断路器数据结构表成员名称 */
#define BREAKN_PRO_ID       "proId"         /* pro_id */
#define BREAKN_SEC_ID       "secId"         /* sec_id */
#define BREAKN_ID           "BreakerId"     /* brk_id */
#define BREAKN_ID_BIT       "BrkIdBit"      /* brk_id_bit */
#define BREAKN_AVAILABLE    "available"      /* brk_available */
#define BREAKN_ENABLE_BITS  "EnableBits"    /* brk_bits_enable */
#define BREAKN_NAME         "name"          /* brk_name */
#define BREAKN_ALIAS        "alias"         /* brk_alias */
#define BREAKN_TYPE         "type"          /* brk_type */
#define BREAKN_GROUP        "group"         /* brk_group */
#define BREAKN_BITS         "Bits"          /* brk_bits */
#define BREAKN_MASKS        "Masks"         /* brk_masks */
#define BREAKN_VLDTAG       "vldTag"        /* vldTag */

/* 断路器数据结构表成员ID */
#define BREAKN_PRO_ID_INDEX       1         /* pro_id */
#define BREAKN_SEC_ID_INDEX       2         /* sec_id */
#define BREAKN_ID_INDEX           3         /* brk_id */
#define BREAKN_ID_BIT_INDEX       4         /* brk_id_bit */
#define BREAKN_AVAILABLE_INDEX    5         /* brk_available */
#define BREAKN_ENABLE_BITS_INDEX  6         /* brk_bits_enable */
#define BREAKN_NAME_INDEX         7         /* brk_name */
#define BREAKN_ALIAS_INDEX        8         /* brk_alias */
#define BREAKN_TYPE_INDEX         9         /* brk_type */
#define BREAKN_GROUP_INDEX        10        /* brk_group */
#define BREAKN_BITS_INDEX         11        /* brk_bits */
#define BREAKN_MASKS_INDEX        12        /* brk_masks */
#define BREAKN_VLDTAG_INDEX       13        /* vldTag */

/* 遥信状态量数据结构表成员名称 */
#define RSTATEN_PRO_ID      "proId"         /* pro_id */
#define RSTATEN_SEC_ID      "secId"         /* sec_id */
#define RSTATEN_SEC_INF     "SecInf"        /* sec_inf */
#define RSTATEN_ID          "RstateId"      /* rstate_id */
#define RSTATEN_DESC_ID     "desc_id"       /* desc_id */
#define RSTATEN_AVAILABLE   "available"      /* rstate_available */
#define RSTATEN_INF         "inf"           /* rstate_inf */
#define RSTATEN_VARINF      "VarInf"        /* rstate_variinf */
#define RSTATEN_NAME        "name"          /* rstate_name */
#define RSTATEN_ALIAS       "alias"         /* rstate_alias */
#define RSTATEN_STATUS      "status"        /* rstate_status */
#define RSTATEN_TYPE        "type"          /* type */
#define RSTATEN_DONAME      "doName"        /* DO */
#define RSTATEN_SUBENA      "subEna"        /* subEna */
#define RSTATEN_SUBVAL      "subVal"        /* subVal */
#define RSTATEN_SUBQ        "subQ"          /* subQ */
#define RSTATEN_SUBID       "subID"         /* subID */
#define RSTATEN_Q           "q"             /* q */
#define RSTATEN_T_SEC       "tSec"          /* t_sec */
#define RSTATEN_T_FRAC      "tFrac"         /* t_frac */
#define RSTATEN_T_QFLAG     "tQflag"        /* t_qflag */
#define RSTATEN_VALUE       "value"         /* value */
#define RSTATEN_VLDTAG      "vldTag"        /* vldTag */
#define RSTATEN_DATATYPE    "dataType"      /* dataType */
#define RSTATEN_POSTFIX_ACT "PostFixAct"    /* PostFixAct */
#define RSTATEN_POSTFIX_RET "PostFixRet"    /* PostFixRet */

/* 遥信状态量数据结构表成员ID */
#define RSTATEN_PRO_ID_INDEX      1         /* pro_id */
#define RSTATEN_SEC_ID_INDEX      2         /* sec_id */
#define RSTATEN_SEC_INF_INDEX     3         /* sec_inf */
#define RSTATEN_ID_INDEX          4         /* rstate_id */
#define RSTATEN_DESC_ID_INDEX     5         /* desc_id */
#define RSTATEN_AVAILABLE_INDEX   6         /* rstate_available */
#define RSTATEN_INF_INDEX         7         /* rstate_inf */
#define RSTATEN_VARINF_INDEX      8         /* rstate_variinf */
#define RSTATEN_NAME_INDEX        9         /* rstate_name */
#define RSTATEN_ALIAS_INDEX       10        /* rstate_alias */
#define RSTATEN_STATUS_INDEX      11        /* rstate_status */
#define RSTATEN_TYPE_INDEX        12        /* iType */
#define RSTATEN_DONAME_INDEX      13        /* DO */
#define RSTATEN_SUBENA_INDEX      14        /* subEna */
#define RSTATEN_SUBVAL_INDEX      15        /* subVal */
#define RSTATEN_SUBQ_INDEX        16        /* subQ */
#define RSTATEN_SUBID_INDEX       17        /* subID */
#define RSTATEN_Q_INDEX           18        /* q */
#define RSTATEN_T_SEC_INDEX       19        /* t_sec */
#define RSTATEN_T_FRAC_INDEX      20        /* t_frac */
#define RSTATEN_T_QFLAG_INDEX     21        /* t_qflag */
#define RSTATEN_VALUE_INDEX       22        /* value */
#define RSTATEN_VLDTAG_INDEX      23        /* vldTag */
#define RSTATEN_DATATYPE_INDEX    24        /* dataType */
#define RSTATEN_POSTFIX_ACT_INDEX 25        /* PostFixAct */
#define RSTATEN_POSTFIX_RET_INDEX 26        /* PostFixRet */

/* 实时数据结构表成员名称 */
#define RUNN_PRO_ID         "proId"         /* pro_id */
#define RUNN_SEC_ID         "secId"         /* sec_id */
#define RUNN_ID             "ValueId"       /* value_id */
#define RUNN_DESC_ID        "desc_id"       /* desc_id */
#define RUNN_AVAILABLE      "available"      /* value_available */
#define RUNN_NAME           "name"          /* value_name */
#define RUNN_ALIAS          "alias"         /* value_alias */
#define RUNN_CAL_TYPE       "CalType"       /* value_calculate_style */
#define RUNN_DIMENSION      "dimension"     /* value_dimension */
#define RUNN_TYPE           "type"          /* value_type */
#define RUNN_COEFFICIENT    "coefficient"   /* value_calculate_coefficient */
#define RUNN_MEASURE        "measure"       /* value_measure */
#define RUNN_REAL           "real"          /* value_real */
#define RUNN_IMAGINARY      "imaginary"     /* value_imaginary */
#define RUNN_SEND_ENA       "sendEna"       /* sendEna */
#define RUNN_RATE           "rate"          /* rate */
#define RUNN_DO             "doName"        /* DO */
#define RUNN_DO_TYPE        "doType"        /*Dotype*/
#define RUNN_GRPID          "grpId"         /*grpId*/
#define RUNN_MEMID          "memID"         /*memID*/
#define RUNN_Q              "q"             /*q*/
#define RUNN_T_SEC          "tSec"          /*t_sec*/
#define RUNN_T_FRAC         "tFrac"         /*t_frac*/
#define RUNN_T_QFLAG        "tQflag"        /*t_qflag*/
#define RUNN_SUBENA         "subEna"        /*subEna取代使能*/
#define RUNN_SUBVAL         "subVal"        /*subVal取代值*/
#define RUNN_SUBQ           "subQ"          /*subQ取代品质因数*/
#define RUNN_SUBID          "subID"         /*subID取代ID*/
#define RUNN_DB             "db"            /*db死区*/
#define RUNN_ZERODB         "zeroDb"        /*zeroDb零死区*/
#define RUNN_RANGEC_MAX     "rangeCmaxf"    /*rangeCmaxf最大值*/
#define RUNN_RANGEC_HHF     "rangeChhf"     /*rangeChhf高高值*/
#define RUNN_RANGEC_HF      "rangeChf"      /*rangeChf高值*/
#define RUNN_RANGEC_LF      "rangeClf"      /*rangeClf低值*/
#define RUNN_RANGEC_LLF     "rangeCllf"     /*rangeCllf低低值*/
#define RUNN_RANGEC_MIN     "rangeCminf"    /*rangeCminf最小值*/
#define RUNN_MAG_SVCS       "magSVCs"       /*magSVCs模比例因子*/
#define RUNN_MAG_SVCO       "magSVCo"       /*magSVCo模偏移量*/
#define RUNN_ANG_SVCS       "angSVCs"       /*angSVCs角度比例因子*/
#define RUNN_ANG_SVCO       "angSVCo"       /*angSVCo角度偏移量*/
#define RUNN_ANG_REF        "angRef"        /*angRef角度配置*/
#define RUNN_SMP_RATE       "smpRate"       /*smpRate采样频率*/
#define RUNN_ASDU           "Asdu"          /* Asdu */
#define RUNN_INF            "Inf"           /* Inf */
#define RUNN_COMMADDR       "CommAddr"      /* CommAddr */
#define RUNN_FUNCTYPE       "FuncType"      /* FuncType */
#define RUNN_ISTAP          "isTap"         /*isTap是否档位*/
#define RUNN_SEC_TYPE       "secType"       /*保护,测控:Vld自动生成,根据扇区类型属性*/
#define RUNN_DIMEN_PRI      "DimenPri"      /* value_dimension_pri */
#define RUNN_CHANNEL_NUM    "ChlNum"        /* ChlNum */
#define RUNN_SEV_A_PORT     "SevPort"       /* SevAPort */
#define RUNN_SEV_B_PORT     "SevBPort"      /* SevBPort */
#define RUNN_VLDTAG         "vldTag"        /* vldTag */

/* 实时数据结构表成员ID */
#define RUNN_PRO_ID_INDEX         1         /* pro_id */
#define RUNN_SEC_ID_INDEX         2         /* sec_id */
#define RUNN_ID_INDEX             3         /* value_id */
#define RUNN_DESC_ID_INDEX        4         /* desc_id */
#define RUNN_AVAILABLE_INDEX      5         /* value_available */
#define RUNN_NAME_INDEX           6         /* value_name */
#define RUNN_ALIAS_INDEX          7         /* value_alias */
#define RUNN_CAL_TYPE_INDEX       8         /* value_calculate_style */
#define RUNN_DIMENSION_INDEX      9         /* value_dimension */
#define RUNN_TYPE_INDEX           10        /* value_type */
#define RUNN_COEFFICIENT_INDEX    11        /* value_calculate_coefficient */
#define RUNN_MEASURE_INDEX        12        /* value_measure */
#define RUNN_REAL_INDEX           13        /* value_real */
#define RUNN_IMAGINARY_INDEX      14        /* value_imaginary */
#define RUNN_SEND_ENA_INDEX       15        /* sendEna */
#define RUNN_RATE_INDEX           16        /* rate */
#define RUNN_DO_INDEX             17        /* DO */
#define RUNN_DO_TYPE_INDEX        18        /*Dotype*/
#define RUNN_GRPID_INDEX          19        /*grpId*/
#define RUNN_MEMID_INDEX          20        /*memID*/
#define RUNN_Q_INDEX              21        /*q*/
#define RUNN_T_SEC_INDEX          22        /*t_sec*/
#define RUNN_T_FRAC_INDEX         23        /*t_frac*/
#define RUNN_T_QFLAG_INDEX        24        /*t_qflag*/
#define RUNN_SUBENA_INDEX         25        /*subEna取代使能*/
#define RUNN_SUBVAL_INDEX         26        /*subVal取代值*/
#define RUNN_SUBQ_INDEX           27        /*subQ取代品质因数*/
#define RUNN_SUBID_INDEX          28        /*subID取代ID*/
#define RUNN_DB_INDEX             29        /*db死区*/
#define RUNN_ZERODB_INDEX         30        /*zeroDb零死区*/
#define RUNN_RANGEC_MAX_INDEX     31        /*rangeCmaxf最大值*/
#define RUNN_RANGEC_HHF_INDEX     32        /*rangeChhf高高值*/
#define RUNN_RANGEC_HF_INDEX      33        /*rangeChf高值*/
#define RUNN_RANGEC_LF_INDEX      34        /*rangeClf低值*/
#define RUNN_RANGEC_LLF_INDEX     35        /*rangeCllf低低值*/
#define RUNN_RANGEC_MIN_INDEX     36        /*rangeCminf最小值*/
#define RUNN_MAG_SVCS_INDEX       37        /*magSVCs模比例因子*/
#define RUNN_MAG_SVCO_INDEX       38        /*magSVCo模偏移量*/
#define RUNN_ANG_SVCS_INDEX       39        /*angSVCs角度比例因子*/
#define RUNN_ANG_SVCO_INDEX       40        /*angSVCo角度偏移量*/
#define RUNN_ANG_REF_INDEX        41        /*angRef角度配置*/
#define RUNN_SMP_RATE_INDEX       42        /*smpRate采样频率*/
#define RUNN_ASDU_INDEX           43        /* Asdu */
#define RUNN_INF_INDEX            44        /* Inf */
#define RUNN_COMM_ADDR_INDEX      45        /* CommAddr */
#define RUNN_FUNC_TYPE_INDEX      46        /* FuncType */
#define RUNN_ISTAP_INDEX          47        /*isTap是否档位*/
#define RUNN_SEC_TYPE_INDEX       48        /*保护,测控:Vld自动生成,根据扇区类型属性*/
#define RUNN_DIMEN_PRI_INDEX      49        /* value_dimension_pri */
#define RUNN_CHANNEL_NUM_INDEX    50        /* ChlNum */
#define RUNN_SEV_A_PORT_INDEX     51        /* SevAPort */
#define RUNN_SEV_B_PORT_INDEX     52        /* SevBPort */
#define RUNN_VLDTAG_INDEX         53        /* vldTag */

/* 动作报告数据结构表成员名称 */
#define REPORTN_PRO_ID      "proId"         /* pro_id */
#define REPORTN_SEC_ID      "secId"         /* sec_id */
#define REPORTN_ID          "ReportId"      /* report_id */
#define REPORTN_DESC_ID     "desc_id"       /* desc_id */
#define REPORTN_AVAILABLE   "available"      /* report_available */
#define REPORTN_TYPE        "type"          /* report_type */
#define REPORTN_FAULTNUM    "FaultNum"      /* report_fault_number */
#define REPORTN_INF         "inf"           /* report_inf */
#define REPORTN_NAME        "name"          /* report_name */
#define REPORTN_Q           "q"             /* 品质因素 */
#define REPORTN_DIRECTION   "direction"     /* 动作方向 */
#define REPORTN_DISTANCE    "distance"      /* report_distance */
#define REPORTN_DPI         "dpi"           /* report_dpi */
#define REPORTN_REL_TIME    "RelTime"       /* report_relative_time */
#define REPORTN_YEAR        "year"          /* report_year */
#define REPORTN_MONTH       "month"         /* report_month */
#define REPORTN_DAY         "day"           /* report_day */
#define REPORTN_HOUR        "hour"          /* report_hour */
#define REPORTN_MINUTE      "minute"        /* report_minute */
#define REPORTN_MILLSECOND  "MillSecond"    /* report_millionsecond */
#define REPORTN_SIN         "SIN"           /* report_SIN */
#define REPORTN_PHASE       "phase"         /* report_phase */
#define REPORTN_DATA_NUMBER "DataNumber"    /* report_data_number */
#define REPORTN_DATA_ID     "DataId"        /* report_data_id */
#define REPORTN_T_SEC       "tSec"          /* t_sec */
#define REPORTN_T_FRAC      "tFrac"         /* t_frac */
#define REPORTN_T_QFLAG     "tQflag"        /* t_qflag */
#define REPORTN_TRIP_PHASE  "tripPhase"     /* trip_phase */
#define REPORTN_DO          "doName"        /* DO */
#define REPORTN_VLDTAG      "vldTag"        /* vldTag */

/* 动作报告数据结构表成员ID */
#define REPORTN_PRO_ID_INDEX      1         /* pro_id */
#define REPORTN_SEC_ID_INDEX      2         /* sec_id */
#define REPORTN_ID_INDEX          3         /* report_id */
#define REPORTN_DESC_ID_INDEX     4        /* desc_id */
#define REPORTN_AVAILABLE_INDEX   5         /* report_available */
#define REPORTN_TYPE_INDEX        6         /* report_type */
#define REPORTN_FAULTNUM_INDEX    7         /* report_fault_number */
#define REPORTN_INF_INDEX         8         /* report_inf */
#define REPORTN_NAME_INDEX        9         /* report_name */
#define REPORTN_Q_INDEX           10        /* 品质因素 */
#define REPORTN_DIRECTION_INDEX   11        /* 动作方向 */
#define REPORTN_DISTANCE_INDEX    12        /* report_distance */
#define REPORTN_DPI_INDEX         13        /* report_dpi */
#define REPORTN_REL_TIME_INDEX    14        /* report_relative_time */
#define REPORTN_YEAR_INDEX        15        /* report_year */
#define REPORTN_MONTH_INDEX       16        /* report_month */
#define REPORTN_DAY_INDEX         17        /* report_day */
#define REPORTN_HOUR_INDEX        18        /* report_hour */
#define REPORTN_MINUTE_INDEX      19        /* report_minute */
#define REPORTN_MILLSECOND_INDEX  20        /* report_millionsecond */
#define REPORTN_SIN_INDEX         21        /* report_SIN */
#define REPORTN_PHASE_INDEX       22        /* report_phase */
#define REPORTN_DATA_NUMBER_INDEX 23        /* report_data_number */
#define REPORTN_DATA_ID_INDEX     24        /* report_data_id */
#define REPORTN_T_SEC_INDEX       25        /* t_sec */
#define REPORTN_T_FRAC_INDEX      26        /* t_frac */
#define REPORTN_T_QFLAG_INDEX     27        /* t_qflag */
#define REPORTN_TRIP_PHASE_INDEX  28        /* trip_phase */
#define REPORTN_DO_NAME_INDEX     29        /* DO */
#define REPORTN_VLDTAG_INDEX      30        /* vldTag */

/* 动作报告量值数据结构表成员名称 */
#define RVALUEN_PRO_ID      "proId"         /* pro_id */
#define RVALUEN_SEC_ID      "secId"         /* sec_id */
#define RVALUEN_ID          "ValueId"       /* value_id */
#define RVALUEN_AVAILABLE   "available"      /* value_available */
#define RVALUEN_NAME        "name"          /* value_name */
#define RVALUEN_ALIAS       "alias"         /* value_alias */
#define RVALUEN_CAL_TYPE    "CalType"       /* value_calculate_style */
#define RVALUEN_DIMENSION   "dimension"     /* value_dimension */
#define RVALUEN_TYPE        "type"          /* value_type */
#define RVALUEN_COEFFICIENT "coefficient"   /* value_calculate_coefficient */
#define RVALUEN_MEASURE     "measure"       /* value_measure */
#define RVALUEN_REAL        "real"          /* value_real */
#define RVALUEN_IMAGINARY   "imaginary"     /* value_imaginary */
#define RVALUEN_DIMEN_PRI   "dimenPri"      /* value_dimension_pri */
#define RVALUEN_VLDTAG      "vldTag"        /* vldTag */

/* 动作报告量值数据结构表成员ID */
#define RVALUEN_PRO_ID_INDEX      1         /* pro_id */
#define RVALUEN_SEC_ID_INDEX      2         /* sec_id */
#define RVALUEN_ID_INDEX          3         /* value_id */
#define RVALUEN_AVAILABLE_INDEX   4         /* value_available */
#define RVALUEN_NAME_INDEX        5         /* value_name */
#define RVALUEN_ALIAS_INDEX       6         /* value_alias */
#define RVALUEN_CAL_TYPE_INDEX    7         /* value_calculate_style */
#define RVALUEN_DIMENSION_INDEX   8         /* value_dimension */
#define RVALUEN_TYPE_INDEX        9         /* value_type */
#define RVALUEN_COEFFICIENT_INDEX 10        /* value_calculate_coefficient */
#define RVALUEN_MEASURE_INDEX     11        /* value_measure */
#define RVALUEN_REAL_INDEX        12        /* value_real */
#define RVALUEN_IMAGINARY_INDEX   13        /* value_imaginary */
#define RVALUEN_DIMEN_PRI_INDEX   14        /* value_dimension_pri */
#define RVALUEN_VLDTAG_INDEX      15        /* vldTag */

/* 保护扇区数据结构表成员名称 */
#define TSECTOR_ID          "Id"            /* sector_id */
#define TSECTOR_AVAILABLE   "available"     /* sector_available */
#define TSECTOR_NAME        "name"          /* sector_name */
#define TSECTOR_INF         "inf"           /* sector_inf */
#define TSECTOR_DO_NAME     "doName"        /* sector_doName */
#define TSECTOR_LLN0_MODE   "lln0Mod"       /* sector_lln0Mode */
#define TSECTOR_LLN0_BEH    "lln0Beh"       /* sector_lln0Beh */
#define TSECTOR_LLN0_HEALTH "lln0Health"    /* sector_lln0Health */
#define TSECTOR_LLN0_PLAT   "lln0NamPlat"   /* sector_lln0NamPlat */
#define TSECTOR_LPHD_PROXY  "lphdProxy"     /* sector_lphdProxy */
#define TSECTOR_LPHD_HEALTH "lphdHealth"    /* sector_lphdHealth */
#define TSECTOR_LPHD_NAME   "lphdPhyName"   /* sector_lphdPhyName */
#define TSECTOR_LPHD_VENDOR "lphdVendor"    /* sector_lphdVendor */
#define TSECTOR_LPHD_SW_REV "lphdswRev"     /* sector_lphdswRev */
#define TSECTOR_LPHD_MODAL  "lphdModal"     /* sector_lphdModal */
#define TSECTOR_SEC_TYPE    "secType"       /* sector_secType */
#define TSECTOR_ACS_ID      "acsId"         /* sector_acsId */
#define TSECTOR_VLDTAG      "vldTag"        /* vldTag */

/* 保护扇区数据结构表成员ID */
#define TSECTOR_ID_INDEX          1         /* sector_id */
#define TSECTOR_AVAILABLE_INDEX   2         /* sector_available */
#define TSECTOR_NAME_INDEX        3         /* sector_name */
#define TSECTOR_INF_INDEX         4         /* sector_inf */
#define TSECTOR_DO_NAME_INDEX     5         /* sector_doName */
#define TSECTOR_LLN0_MODE_INDEX   6         /* sector_lln0Mode */
#define TSECTOR_LLN0_BEH_INDEX    7         /* sector_lln0Beh */
#define TSECTOR_LLN0_HEALTH_INDEX 8         /* sector_lln0Health */
#define TSECTOR_LLN0_PLAT_INDEX   9         /* sector_lln0NamPlat */
#define TSECTOR_LPHD_PROXY_INDEX  10        /* sector_lphdProxy */
#define TSECTOR_LPHD_HEALTH_INDEX 11        /* sector_lphdHealth */
#define TSECTOR_LPHD_NAME_INDEX   12        /* sector_lphdPhyName */
#define TSECTOR_LPHD_VENDOR_INDEX 13        /* sector_lphdVendor */
#define TSECTOR_LPHD_SW_REV_INDEX 14        /* sector_lphdswRev */
#define TSECTOR_LPHD_MODAL_INDEX  15        /* sector_lphdModal */
#define TSECTOR_SEC_TYPE_INDEX    16        /* sector_secType */
#define TSECTOR_ACS_ID_INDEX      17        /* sector_acsId */
#define TSECTOR_VLDTAG_INDEX      18        /* vldTag */

/* 检修压板数据结构表成员名称 */
#define RJPN_PRO_ID         "proId"         /* pro_id */
#define RJPN_SEC_ID         "secId"         /* sec_id */
#define RJPN_DESC_ID        "desc_id"       /* desc_id */
#define RJPN_BITS           "bits"          /* jum_bits */
#define RJPN_VARINF         "VarInf"        /* jum_variinf */
#define RJPN_NAME           "name"          /* jum_name */
#define RJPN_ALIAS          "alias"         /* jum_alias */
#define RJPN_STATUS         "status"        /* jum_status */
#define RJPN_DO             "doName"        /* DO */
#define RJPN_Q              "Q"             /* q */
#define RJPN_T_SEC          "tSec"          /* t_sec */
#define RJPN_T_FRAC         "tFrac"         /* t_frac */
#define RJPN_T_Q_FLAG       "tQflag"        /* t_q_flag */
#define RJPN_SUB_ENA        "subEna"        /* subEna */
#define RJPN_SUB_VAL        "subVal"        /* subVal */
#define RJPN_SUB_Q          "subQ"          /* subQ */
#define RJPN_SUB_ID         "subID"         /* subID */
#define RJPN_DATA_TYPE      "dataType"      /* dataType */
#define RJPN_AVAILABLE      "available"     /* available */
#define RJPN_VLDTAG         "vldTag"        /* vldTag */

/* 检修压板数据结构表成员ID */
#define RJPN_PRO_ID_INDEX         1         /* pro_id */
#define RJPN_SEC_ID_INDEX         2         /* sec_id */
#define RJPN_DESC_ID_INDEX        3         /* desc_id */
#define RJPN_BITS_INDEX           4         /* jum_bits */
#define RJPN_VARINF_INDEX         5         /* jum_variinf */
#define RJPN_NAME_INDEX           6         /* jum_name */
#define RJPN_ALIAS_INDEX          7         /* jum_alias */
#define RJPN_STATUS_INDEX         8         /* jum_status */
#define RJPN_DO_INDEX             9         /* DO */
#define RJPN_Q_INDEX              10        /* q */
#define RJPN_T_SEC_INDEX          11        /* t_sec */
#define RJPN_T_FRAC_INDEX         12        /* t_frac */
#define RJPN_T_Q_FLAG_INDEX       13        /* t_q_flag */
#define RJPN_SUB_ENA_INDEX        14        /* subEna */
#define RJPN_SUB_VAL_INDEX        15        /* subVal */
#define RJPN_SUB_Q_INDEX          16        /* subQ */
#define RJPN_SUB_ID_INDEX         17        /* subID */
#define RJPN_DATA_TYPE_INDEX      18        /* dataType */
#define RJPM_AVAILABLE_INDEX      19        /* available */
#define RJPN_VLDTAG_INDEX         20        /* vldTag */

/* 测控(测控单点,测控双点,测控档位,电度)数据结构表成员名称 */
#define FCK_INFO_TYPE         "type"        /* type */
#define FCK_INFO_PRO_ID       "proId"       /* pro_id */
#define FCK_INFO_SECT_ID      "secId"       /* sec_id */
#define FCK_INFO_ID           "FckInfoID"   /* FckInfoID */
#define FCK_INFO_DESC_ID      "desc_id"     /* desc_id */
#define FCK_INFO_AVAILABLE    "available"    /* vailable */
#define FCK_INFO_INF          "inf"         /* inf */
#define FCK_INFO_NAME         "name"        /* name */
#define FCK_INFO_ASDU         "Asdu"        /* Asdu */
#define FCK_INFO_FUNCTYPE     "FuncType"    /* FuncType */
#define FCK_INFO_SRC_TABLE_ID "SrcTableID"  /* SrcTableID */
#define FCK_INFO_SRC_DATA_ID1 "SrcDataID1"  /* SrcDataID1 */
#define FCK_INFO_SRC_DATA_ID2 "SrcDataID2"  /* SrcDataID2 */
#define FCK_INFO_STATUS       "status"      /* status */
#define FCK_INFO_DO           "doName"      /* DO */
#define FCK_INFO_SUB_ENA      "subEna"      /* subEna */
#define FCK_INFO_SUB_VAL      "subVal"      /* subVal */
#define FCK_INFO_SUB_Q        "subQ"        /* subQ */
#define FCK_INFO_SUB_ID       "subID"       /* subID */
#define FCK_INFO_Q            "q"           /* q */
#define FCK_INFO_T_SEC        "tSec"        /* tSec */
#define FCK_INFO_T_FRAC       "tFrac"       /* tFrac */
#define FCK_INFO_T_QFLAG      "tQflag"      /* tQflag */
#define FCK_INFO_TAP_MAX      "tapMax"      /* tapMax */
#define FCK_INFO_TAP_MIN      "tapMin"      /* tapMin */
#define FCK_INFO_SRC_DATA_TYPE "srcDataType" /* SrcDataType */
#define FCK_INFO_EVENT_TYPE    "EventType"   /* EventType */
#define FCK_INFO_SRC_COM_CHL_TYPE "SrcComChlT" /* SrcComChlT */
#define FCK_INFO_SEV_A_PORT       "SevAPort "  /* SevAPort  */
#define FCK_INFO_SEV_B_PORT       "SevBPort "  /* SevBPort  */
#define FCK_INFO_VLDTAG           "vldTag"     /* vldTag */

/* 测控(测控单点,测控双点,测控档位)数据结构表成员ID */
#define FCK_INFO_TYPE_INDEX       1         /* type */
#define FCK_INFO_PRO_ID_INDEX     2         /* pro_id */
#define FCK_INFO_SECT_ID_INDEX    3         /* sec_id */
#define FCK_INFO_ID_INDEX         4         /* FckInfoID */
#define FCK_INFO_DESC_ID_INDEX    5         /* desc_id */
#define FCK_INFO_AVAILABLE_INDEX  6         /* vailable */
#define FCK_INFO_INF_INDEX        7         /* inf */
#define FCK_INFO_NAME_INDEX       8         /* name */
#define FCK_INFO_ASDU_INDEX       9         /* Asdu */
#define FCK_INFO_FUNCTYPE_INDEX   10        /* FuncType */
#define FCK_INFO_SRCTABLEID_INDEX 11        /* SrcTableID */
#define FCK_INFO_SRCDATAID1_INDEX 12        /* SrcDataID1 */
#define FCK_INFO_SRCDATAID2_INDEX 13        /* SrcDataID2 */
#define FCK_INFO_STATUS_INDEX     14        /* status */
#define FCK_INFO_DO_INDEX         15        /* DO */
#define FCK_INFO_SUBENA_INDEX     16        /* subEna */
#define FCK_INFO_SUBVAL_INDEX     17        /* subVal */
#define FCK_INFO_SUBQ_INDEX       18        /* subQ */
#define FCK_INFO_SUBID_INDEX      19        /* subID */
#define FCK_INFO_Q_INDEX          20        /* q */
#define FCK_INFO_T_SEC_INDEX      21        /* tSec */
#define FCK_INFO_T_FRAC_INDEX     22        /* tFrac */
#define FCK_INFO_T_QFLAG_INDEX    23        /* tQflag */
#define FCK_INFO_TAP_MAX_INDEX    24        /* tapMax */
#define FCK_INFO_TAP_MIN_INDEX    25        /* tapMin */
#define FCK_INFO_SRC_DATA_TYPE_INDEX 26     /* SrcDataType */
#define FCK_INFO_EVENT_TYPEE_INDEX 27       /* EventType */
#define FCK_INFO_SRC_COM_CHL_TYPE_INDEX 28  /* SrcComChlT */
#define FCK_INFO_SEV_A_PORT_INDEX  29       /* SevAPort  */
#define FCK_INFO_SEV_B_PORT_INDEX  30       /* SevBPort  */
#define FCK_INFO_VLDTAG_INDEX      31       /* vldTag */

/* 装置模拟量通道数据结构表成员名称 */
#define CHANNELN_NUM        "ChlNum"        /* chl_Num */
#define CHANNELN_INF        "ChlInf"        /* chl_inf */
#define CHANNELN_DEV_NAME   "ChlDevName"    /* chl_device_name */
#define CHANNELN_NAME       "ChlName"       /* chl_name */
#define CHANNELN_TYPE       "ChlType"       /* chl_type */
#define CHANNELN_A_CALSTYLE "ChlaCalStyl"   /* chla_calculate_style */
#define CHANNELN_B_CALSTYLE "ChlbCalStyl"   /* chlb_calculate_style */
#define CHANNELN_PRIM_DIM   "ChlPrimDim"    /* chl_primary_dimension */
#define CHANNELN_SEC_DIM    "ChlSecDim"     /* chl_secondary_dimension */
#define CHANNELN_CAL_COEFF  "CalCoeff"      /* chl_calculate_coefficient */
#define CHANNELN_SAMP_COEFF "SampCoeff"     /* chl_sample_coefficient */
#define CHANNELN_CHL_COEFF  "ChlCoeff"      /* chl_channel_coefficient */
#define CHANNELN_CHL_EXCU   "ChlExcu"       /* chl_channel_excursion */
#define CHANNELN_CORR_CHL   "CorrChl"       /* chl_correlation_channel */
#define CHANNELN_PRIM_ID    "ChlPrimId"     /* chl_primary_id */
#define CHANNELN_SEC_ID     "ChlSecId"      /* chl_secondary_id */
#define CHANNELN_PRIM_RATE  "ChlPrimRate"   /* chl_primary_rate */
#define CHANNELN_SEC_RATE   "ChlSecRate"    /* chl_secondary_rate */
#define CHANNELN_CAL_RATE   "ChlCalRate"    /* chl_calculate_rate */
#define CHANNELN_RATE_COEFF "ChlRateCoef"   /* chl_rate_coefficient */
#define CHANNELN_PHASE      "ChlPhase"      /* chl_channel_phase */
#define CHANNELN_VIRT_VAL   "ChlVirtVal"    /* chl_virtual_value */
#define CHANNELN_PHAS_FACTOR3 "Phasfactor3" /* chl_phase_factor3 */
#define CHANNELN_DZ_NAME    "dzName"        /* dzName */
#define CHANNELN_WAVE_COEFF "WaveCoeff"     /* chl_wave_coefficient */
#define CHANNELN_DESC_ID    "desc_id"       /* desc_id */
#define CHANNELN_WAVE_CONVER "WaveConver"   /* chl_wave_conver_coef */
#define CHANNELN_VLDTAG      "vldTag"       /* vldTag */
//
#define CHANNELN_A_DC_VALUE "ChlaDCValue"   /* chla_DC_component */
#define CHANNELN_A_FD_REAL  "ChlafdReal"    /* chla_fundamental_real */
#define CHANNELN_A_FD_IMAG  "ChlafdImag"    /* chla_fundamental_imaginary */
#define CHANNELN_A_2ND_REAL "Chla2ndReal"   /* chla_2nd_harmonic_real */
#define CHANNELN_A_2ND_IMAG "Chla2ndImag"   /* chla_2nd_harmonic_imaginary */
#define CHANNELN_A_3ND_REAL "Chla3ndReal"   /* chla_3rd_harmonic_real */
#define CHANNELN_A_3ND_IMAG "Chla3ndImag"   /* chla_3rd_harmonic_imaginary */
#define CHANNELN_A_5TH_REAL "Chla5thReal"   /* chla_5th_harmonic_real */
#define CHANNELN_A_5TH_IMAG "Chla5thImag"   /* chla_5th_harmonic_imaginary */
#define CHANNELN_A_HFD_REAL "ChlaHfdReal"   /* chla_hfundamental_real */
#define CHANNELN_A_HFD_IMAG "ChlaHfdImag"   /* chla_hfundamental_imaginary */
//
#define CHANNELN_B_DC_VALUE "ChlbDCValue"   /* chlb_DC_component */
#define CHANNELN_B_FD_REAL  "ChlbfdReal"    /* chlb_fundamental_real */
#define CHANNELN_B_FD_IMAG  "ChlbfdImag"    /* chlb_fundamental_imaginary */
#define CHANNELN_B_2ND_REAL "Chlb2ndReal"   /* chlb_2nd_harmonic_real */
#define CHANNELN_B_2ND_IMAG "Chlb2ndImag"   /* chlb_2nd_harmonic_imaginary */
#define CHANNELN_B_3ND_REAL "Chlb3ndReal"   /* chlb_3rd_harmonic_real */
#define CHANNELN_B_3ND_IMAG "Chlb3ndImag"   /* chlb_3rd_harmonic_imaginary */
#define CHANNELN_B_5TH_REAL "Chlb5thReal"   /* chlb_5th_harmonic_real */
#define CHANNELN_B_5TH_IMAG "Chlb5thImag"   /* chlb_5th_harmonic_imaginary */
#define CHANNELN_B_HFD_REAL "ChlbHfdReal"   /* chlb_hfundamental_real */
#define CHANNELN_B_HFD_IMAG "ChlbHfdImag"   /* chlb_hfundamental_imaginary */

/* 装置模拟量通道数据结构表成员ID */
#define CHANNELN_NUM_INDEX          1       /* chl_Num */
#define CHANNELN_INF_INDEX          2       /* chl_inf */
#define CHANNELN_DEVNAME_INDEX      3       /* chl_device_name */
#define CHANNELN_NAME_INDEX         4       /* chl_name */
#define CHANNELN_TYPE_INDEX         5       /* chl_type */
#define CHANNELN_A_CALSTYLE_INDEX   6       /* chla_calculate_style */
#define CHANNELN_B_CALSTYLE_INDEX   7       /* chlb_calculate_style */
#define CHANNELN_PRIMDIM_INDEX      8       /* chl_primary_dimension */
#define CHANNELN_SECDIM_INDEX       9       /* chl_secondary_dimension */
#define CHANNELN_CALCOEFF_INDEX     10      /* chl_calculate_coefficient */
#define CHANNELN_SAMP_COEFF_INDEX   11      /* chl_sample_coefficient */
#define CHANNELN_CHLCOEFF_INDEX     12      /* chl_channel_coefficient */
#define CHANNELN_CHLEXCU_INDEX      13      /* chl_channel_excursion */
#define CHANNELN_CORRCHL_INDEX      14      /* chl_correlation_channel */
#define CHANNELN_PRIMRATE_ID_INDEX  15      /* chl_primary_id */
#define CHANNELN_SECRATE_ID_INDEX   16      /* chl_secondary_id */
#define CHANNELN_PRIMRATE_INDEX     17      /* chl_primary_rate */
#define CHANNELN_SECRATE_INDEX      18      /* chl_secondary_rate */
#define CHANNELN_CALRATE_INDEX      19      /* chl_calculate_rate */
#define CHANNELN_RATECOEFF_INDEX    20      /* chl_rate_coefficient */
#define CHANNELN_PHASEINDEX         21      /* chl_channel_phase */
#define CHANNELN_VIRTVAL_INDEX      22      /* chl_virtual_value */
#define CHANNELN_PHASFACTOR3_INDEX  23      /* chl_phase_factor3 */
#define CHANNELN_DZ_NAME_INDEX      24      /* dzName */
#define CHANNELN_WAVECOEFF_INDEX    25      /* chl_wave_coefficient */
#define CHANNELN_DESC_ID_INDEX      26      /* desc_id */
#define CHANNELN_WAVE_CONVER_INDEX  27      /* chl_wave_conver_coef */
#define CHANNELN_VLDTAG_INDEX       28      /* vldTag */
//
#define CHANNELN_A_DC_VALUE_INDEX   29      /* chla_DC_component */
#define CHANNELN_A_FD_REAL_INDEX    30      /* chla_fundamental_real */
#define CHANNELN_A_FD_IMAG_INDEX    31      /* chla_fundamental_imaginary */
#define CHANNELN_A_2ND_REAL_INDEX   32      /* chla_2nd_harmonic_real */
#define CHANNELN_A_2ND_IMAG_INDEX   33      /* chla_2nd_harmonic_imaginary */
#define CHANNELN_A_3ND_REAL_INDEX   34      /* chla_3rd_harmonic_real */
#define CHANNELN_A_3ND_IMAG_INDEX   35      /* chla_3rd_harmonic_imaginary */
#define CHANNELN_A_5TH_REAL_INDEX   36      /* chla_5th_harmonic_real */
#define CHANNELN_A_5TH_IMAG_INDEX   37      /* chla_5th_harmonic_imaginary */
#define CHANNELN_A_HFD_REAL_INDEX   38      /* chla_hfundamental_real */
#define CHANNELN_A_HFD_IMAG_INDEX   39      /* chla_hfundamental_imaginary */
//
#define CHANNELN_B_DC_VALUE_INDEX   40      /* chlb_DC_component */
#define CHANNELN_B_FD_REAL_INDEX    41      /* chlb_fundamental_real */
#define CHANNELN_B_FD_IMAG_INDEX    42      /* chlb_fundamental_imaginary */
#define CHANNELN_B_2ND_REAL_INDEX   43      /* chlb_2nd_harmonic_real */
#define CHANNELN_B_2ND_IMAG_INDEX   44      /* chlb_2nd_harmonic_imaginary */
#define CHANNELN_B_3ND_REAL_INDEX   45      /* chlb_3rd_harmonic_real */
#define CHANNELN_B_3ND_IMAG_INDEX   46      /* chlb_3rd_harmonic_imaginary */
#define CHANNELN_B_5TH_REAL_INDEX   47      /* chlb_5th_harmonic_real */
#define CHANNELN_B_5TH_IMAG_INDEX   48      /* chlb_5th_harmonic_imaginary */
#define CHANNELN_B_HFD_REAL_INDEX   49      /* chlb_hfundamental_real */
#define CHANNELN_B_HFD_IMAG_INDEX   50      /* chlb_hfundamental_imaginary */

/* 开入量数据结构表成员名称 */
#define INPUTN_ID             "Id"          /* input_id */
#define INPUTN_DESC_ID        "desc_id"     /* desc_id */
#define INPUTN_AVAILABLE      "available"   /* input_available */
#define INPUTN_PRO_GROUP      "ProGroup"    /* input_protect_group */
#define INPUTN_SUB_PRO        "SubPro"      /* input_sub_protect */
#define INPUTN_INF            "inf"         /* input_inf */
#define INPUTN_VARI_INF       "VariInf"     /* input_variinf */
#define INPUTN_NAME           "name"        /* input_name */
#define INPUTN_ALIAS          "alias"       /* input_alias */
#define INPUTN_TYPE           "type"        /* input_type */
#define INPUTN_BITS           "bits"        /* input_bits */
#define INPUTN_DZ_NAME        "dzName"      /* dzName */
#define INPUTN_VALUE          "value"       /* value */
#define INPUTN_VLDTAG         "vldTag"      /* vldTag */
#define INPUTN_GROUP          "group"       /* group */

/* 开入量数据结构表成员ID */
#define INPUTN_ID_INDEX             1       /* input_id */
#define INPUTN_DESC_ID_INDEX        2       /* desc_id */
#define INPUTN_AVAILABLE_INDEX      3       /* input_available */
#define INPUTN_PRO_GROUP_INDEX      4       /* input_protect_group */
#define INPUTN_SUB_PRO_INDEX        5       /* input_sub_protect */
#define INPUTN_INF_INDEX            6       /* input_inf */
#define INPUTN_VARI_INF_INDEX       7       /* input_variinf */
#define INPUTN_NAME_INDEX           8       /* input_name */
#define INPUTN_ALIAS_INDEX          9       /* input_alias */
#define INPUTN_TYPE_INDEX           10      /* input_type */
#define INPUTN_BITS_INDEX           11      /* input_bits */
#define INPUTN_DZ_NAME_INDEX        12      /* dzName */
#define INPUTN_VALUE_INDEX          13      /* value */
#define INPUTN_VLDTAG_INDEX         14      /* vldTag */
#define INPUTN_GROUP_INDEX          15      /* group */

/* 开出量数据结构表成员名称 */
#define OUTPUTN_ID            "Id"          /* output_id */
#define OUTPUTN_DESC_ID       "desc_id"     /* desc_id */
#define OUTPUTN_AVAILABLE     "available"   /* output_available */
#define OUTPUTN_PRO_GROUP     "ProGroup"    /* output_protect_group */
#define OUTPUTN_SUB_PRO       "SubPro"      /* output_sub_protect */
#define OUTPUTN_INF           "inf"         /* output_inf */
#define OUTPUTN_VARI_INF      "VariInf"     /* output_variinf */
#define OUTPUTN_NAME          "name"        /* output_name */
#define OUTPUTN_ALIAS         "alias"       /* output_alias */
#define OUTPUTN_TYPE          "type"        /* output_type */
#define OUTPUTN_BITS          "bits"        /* output_bits */
#define OUTPUTN_SEC_ID        "secId"       /* sec_id */
#define OUTPUTN_DZ_NAME       "dzName"      /* dzName */
#define OUTPUTN_VLDTAG        "vldTag"      /* vldTag */

/* 开出量数据结构表成员ID */
#define OUTPUTN_ID_INDEX            1       /* output_id */
#define OUTPUTN_DESC_ID_INDEX       2       /* desc_id */
#define OUTPUTN_AVAILABLE_INDEX     3       /* output_available */
#define OUTPUTN_PRO_GROUP_INDEX     4       /* output_protect_group */
#define OUTPUTN_SUB_PRO_INDEX       5       /* output_sub_protect */
#define OUTPUTN_INF_INDEX           6       /* output_inf */
#define OUTPUTN_VARI_INF_INDEX      7       /* output_variinf */
#define OUTPUTN_NAME_INDEX          8       /* output_name */
#define OUTPUTN_ALIAS_INDEX         9       /* output_alias */
#define OUTPUTN_TYPE_INDEX          10      /* output_type */
#define OUTPUTN_BITS_INDEX          11      /* output_bits */
#define OUTPUTN_SEC_ID_INDEX        12      /* sec_id */
#define OUTPUTN_DZ_NAME_INDEX       13      /* dzName */
#define OUTPUTN_VLDTAG_INDEX        14      /* vldTag */

/* 录波状态量数据结构表成员名称 */
#define STATEN_PRO_ID         "proId"       /* pro_id */
#define STATEN_SEC_ID         "secId"       /* sec_id */
#define STATEN_ID             "StateId"     /* state_id */
#define STATEN_DESC_ID        "desc_id"     /* desc_id */
#define STATEN_AVAILABLE      "available"    /* state_available */
#define STATEN_GROUP          "group"       /* state_protect_group */
#define STATEN_SUB_GROUP      "SubGroup"    /* state_sub_protect */
#define STATEN_INF            "inf"         /* state_inf */
#define STATEN_NAME           "name"        /* state_soe_name */
#define STATEN_ALIAS          "alias"       /* state_alias */
#define STATEN_VLDTAG         "vldTag"      /* vldTag */

/* 录波状态量数据结构表成员ID */
#define STATEN_PRO_ID_INDEX         1       /* pro_id */
#define STATEN_SEC_ID_INDEX         2       /* sec_id */
#define STATEN_ID_INDEX             3       /* state_id */
#define STATEN_DESC_ID_INDEX        4       /* desc_id */
#define STATEN_AVAILABLE_INDEX      5       /* state_available */
#define STATEN_GROUP_INDEX          6       /* state_protect_group */
#define STATEN_SUB_GROUP_INDEX      7       /* state_sub_protect */
#define STATEN_INF_INDEX            8       /* state_inf */
#define STATEN_NAME_INDEX           9       /* state_soe_name */
#define STATEN_ALIAS_INDEX          10      /* state_alias */
#define STATEN_VLDTAG_INDEX         11      /* vldTag */

/* 保护数据结构表成员名称 */
#define PROTECT_SEC_ID        "secId"       /* sec_id */
#define PROTECT_PRO_ID        "proId"       /* pro_id */
#define PROTECT_AVAILABLE     "available"    /* pro_available */
#define PROTECT_DEV_NAME      "DevName"     /* pro_device_name */
#define PROTECT_NAME          "name"        /* pro_name */
#define PROTECT_CONFIG        "config"      /* pro_config */
#define PROTECT_START_ADDR    "StartAddr"   /* pro_start_addr  */
#define PROTECT_END_ADDR      "EndAddr"     /* pro_end_addr */
#define PROTECT_DO_NAME       "doName"      /* pro_doName */
#define PROTECT_PRE_FIX_NAME  "preFix"      /* pro_preFix */
#define PROTECT_VLDTAG        "vldTag"      /* vldTag */

/* 保护数据结构表成员ID */
#define PROTECT_SEC_ID_INDEX        1       /* sec_id */
#define PROTECT_PRO_ID_INDEX        2       /* pro_id */
#define PROTECT_PRO_AVAILABLE_INDEX 3       /* pro_available */
#define PROTECT_DEV_NAME_INDEX      4       /* pro_device_name */
#define PROTECT_NAME_INDEX          5       /* pro_name */
#define PROTECT_CONFIG_INDEX        6       /* pro_config */
#define PROTECT_START_ADDR_INDEX    7       /* pro_start_addr */
#define PROTECT_END_ADDR_INDEX      8       /* pro_end_addr */
#define PROTECT_DO_NAME_INDEX       9       /* pro_doName */
#define PROTECT_PRE_FIX_NAME_INDEX  10      /* pro_preFix */
#define PROTECT_VLDTAG_INDEX        11      /* vldTag */

/* 遥控数据结构表成员名称 */
#define RCTRL_PRO_ID            "proId"       /* proId */
#define RCTRL_SEC_ID            "secId"       /* secId */
#define RCTRL_YK_ID             "ykId"        /* ykId */
#define RCTRL_YK_NAME           "ykName"      /* ykName */
#define RCTRL_YK_TYPE           "ykType"      /* ykType */
#define RCTRL_YK_OPER           "ykOper"      /* ykOper */
#define RCTRL_YK_LK_DI          "ykLkDI"      /* ykLkDI */
#define RCTRL_YK_LK_LOG_ID      "ykLkLogicId"  /* ykLkLogicId */
#define RCTRL_YK_DO1            "ykDO1"       /* ykDO1 */
#define RCTRL_YK_DO2            "ykDO2"       /* ykDO2 */
#define RCTRL_YK_KEEP_TIME      "ykKeepTime"  /* ykKeepTime */
#define RCTRL_YK_ASDU           "ykAsdu"      /* ykAsdu */
#define RCTRL_YK_FUNC_TYPE      "ykFuncType"  /* ykFuncType */
#define RCTRL_YK_INF            "ykInf"       /* ykInf */
#define RCTRL_YK_ST_VIEW        "ykStView"    /* ykStView */
#define RCTRL_YK_ST_DI          "ykStDi"      /* ykStDi */
#define RCTRL_CK_PROG_STR       "ckProgStr"   /* ckProgStr */
#define RCTRL_CK_START_ST       "ckStartSt"   /* ckStartSt */
#define RCTRL_CK_STOP_ST        "ckStopSt"    /* ckStopSt */
#define RCTRL_CK_INTER_VAL      "ckInterval"  /* ckInterval */
#define RCTRL_CK_TIMEOUT        "ckTimeout"   /* ckTimeout */
#define RCTRL_CK_DO             "doName"          /* DO */
#define RCTRL_ALIAS             "alias"       /* alias */
#define RCTRL_YT_POS            "ytPos"       /* ytPos */
#define RCTRL_OPER_CTRL_VAL     "operCtlVal"  /* Oper_ctlVal */
#define RCTRL_OPER_ORIGIN       "operOrCat"  /* Oper_origin */
#define RCTRL_OPER_ORIGIN_IDENT "operOrId" /* Oper_originIdent */
#define RCTRL_OPER_CTL_NUM      "operCtlNum"  /* Oper_ctlNum */
#define RCTRL_OPER_T_SEC        "operTSec"    /* Oper_T_sec */
#define RCTRL_OPER_T_FRAC       "operTFrac"   /* Oper_T_frac */
#define RCTRL_OPER_T_QFLAG      "operTQflag"  /* Oper_T_qflag */
#define RCTRL_OPER_TEST         "operTest"    /* Oper_Test */
#define RCTRL_OPER_CHECK        "operCheck"   /* Oper_Check */
#define RCTRL_SELECT            "Select"      /* Select */
#define RCTRL_OPERATION         "Operation"   /* Operation */
#define RCTRL_CANCEL            "Cancel"      /* Cancel */
#define RCTRL_AVAILABLE         "available"   /* available */
#define RCTRL_YT_CKYX           "ykStCk"      /* ykStCk */
#define RCTRL_Q                 "Q"           /* Q */
#define RCTRL_T_SEC             "tSec"        /* tSec */
#define RCTRL_T_FRAC            "tFrac"       /* tFrac */
#define RCTRL_T_QFLAG           "tQflag"      /* tQflag */
#define RCTRL_CTL_MODEL         "ctlModel"    /* ctlModel */
#define RCTRL_VAL_WTR_PVAL      "valWtrPVal"  /* valWtrPVal */
#define RCTRL_VAL_WTR_TIND      "valWtrTInd"  /* valWtrTInd */
#define RCTRL_PERSISTENT        "persistent"  /* persistent */
#define RCTRL_SBO_TIMEOUT       "sboTimeout"  /* sboTimeout */
#define RCTRL_SBO_CLASS         "sboClass"    /* sboClass */
#define RCTRL_MIN_VAL           "minVal"      /* minVal */
#define RCTRL_MAX_VAL           "maxVal"      /* maxVal */
#define RCTRL_STEP_SIZE         "stepSize"    /* stepSize */
#define RCTRL_ST_SELD           "stSeld"      /* stSeld */
#define RCTRL_ST_VALUE          "stValue"     /* stValue */
#define RCTRL_ST_VLDTAG         "vldTag"      /* vldTag */

/* 遥控数据结构表成员ID */
#define RCTRL_PRO_ID_INDEX             1      /* proId */
#define RCTRL_SEC_ID_INDEX             2      /* secId */
#define RCTRL_YK_ID_INDEX              3      /* ykId */
#define RCTRL_YK_NAME_INDEX            4      /* ykName */
#define RCTRL_YK_TYPE_INDEX            5      /* ykType */
#define RCTRL_YK_OPER_INDEX            6      /* ykOper */
#define RCTRL_YK_LK_DI_INDEX           7      /* ykLkDI */
#define RCTRL_YK_LK_LOG_ID_INDEX       8      /* ykLkLogicId */
#define RCTRL_YK_DO1_INDEX             9      /* ykDO1 */
#define RCTRL_YK_DO2_INDEX             10     /* ykDO2 */
#define RCTRL_YK_KEEP_TIME_INDEX       11     /* ykKeepTime */
#define RCTRL_YK_ASDU_INDEX            12     /* ykAsdu */
#define RCTRL_YK_FUNC_TYPE_INDEX       13     /* ykFuncType */
#define RCTRL_YK_INF_INDEX             14     /* ykInf */
#define RCTRL_YK_ST_VIEW_INDEX         15     /* ykStView */
#define RCTRL_YK_ST_DI_INDEX           16     /* ykStDi */
#define RCTRL_CK_PROG_STR_INDEX        17     /* ckProgStr */
#define RCTRL_CK_START_ST_INDEX        18     /* ckStartSt */
#define RCTRL_CK_STOP_ST_INDEX         19     /* ckStopSt */
#define RCTRL_CK_INTER_VAL_INDEX       20     /* ckInterval */
#define RCTRL_CK_TIMEOUT_INDEX         21     /* ckTimeout */
#define RCTRL_CK_DO_INDEX              22     /* DO */
#define RCTRL_ALIAS_INDEX              23     /* alias */
#define RCTRL_YT_POS_INDEX             24     /* ytPos */
#define RCTRL_OPER_CTRL_VAL_INDEX      25     /* Oper_ctlVal */
#define RCTRL_OPER_ORIGIN_INDEX        26     /* Oper_origin */
#define RCTRL_OPER_ORIGIN_IDENT_INDEX  27     /* Oper_originIdent */
#define RCTRL_OPER_CTL_NUM_INDEX       28     /* Oper_ctlNum */
#define RCTRL_OPER_T_SEC_INDEX         29     /* Oper_T_sec */
#define RCTRL_OPER_T_FRAC_INDEX        30     /* Oper_T_frac */
#define RCTRL_OPER_T_QFLAG_INDEX       31     /* Oper_T_qflag */
#define RCTRL_OPER_TEST_INDEX          32     /* Oper_Test */
#define RCTRL_OPER_CHECK_INDEX         33     /* Oper_Check */
#define RCTRL_SELECT_INDEX             34     /* Select */
#define RCTRL_OPERATION_INDEX          35     /* Operation */
#define RCTRL_CANCEL_INDEX             36     /* Cancel */
#define RCTRL_AVAILABLE_INDEX          37     /* available */
#define RCTRL_YT_CKYX_INDEX            38     /* ykStCk */
#define RCTRL_Q_INDEX                  39     /* Q */
#define RCTRL_T_SEC_INDEX              40     /* tSec */
#define RCTRL_T_FRAC_INDEX             41     /* tFrac */
#define RCTRL_T_QFLAG_INDEX            42     /* tQflag */
#define RCTRL_CTL_MODEL_INDEX          43     /* ctlModel */
#define RCTRL_VAL_WTR_PVAL_INDEX       44     /* valWtrPVal */
#define RCTRL_VAL_WTR_TIND_INDEX       45     /* valWtrTInd */
#define RCTRL_PERSISTENT_INDEX         46     /* persistent */
#define RCTRL_SBO_TIMEOUT_INDEX        47     /* sboTimeout */
#define RCTRL_SBO_CLASS_INDEX          48     /* sboClass */
#define RCTRL_MIN_VAL_INDEX            49     /* minVal */
#define RCTRL_MAX_VAL_INDEX            50     /* maxVal */
#define RCTRL_STEP_SIZE_INDEX          51     /* stepSize */
#define RCTRL_ST_SELD_INDEX            52     /* stSeld */
#define RCTRL_ST_VALUE_INDEX           53     /* stValue */
#define RCTRL_ST_VLDTAG_INDEX          54     /* vldTag */

/* 脉冲电度数据结构表成员名称 */
#define PULSE_PRO_ID          "proId"       /* proId */
#define PULSE_SEC_ID          "secId"       /* secId */
#define PULSE_PLS_ID          "plsId"       /* plsId */
#define PULSE_NAME            "name"        /* name */
#define PULSE_ALIAS           "alias"       /* alias */
#define PULSE_DO              "doName"      /* DO */
#define PULSE_ACT_VALUE       "actVal"      /* actVal */
#define PULSE_FR_VALUE        "frVal"       /* frVal */
#define PULSE_FR_TM_SEC       "frTmSec"     /* frTmSec */
#define PULSE_FR_TM_FRAC      "frTmFrac"    /* frTmFrac */
#define PULSE_FR_TM_QFLAG     "frTmQflag"   /* frTmQflag */
#define PULSE_Q               "q"           /* q */
#define PULSE_T_SEC           "tSec"        /* tSec */
#define PULSE_T_FRAC          "tFrac"       /* tFrac */
#define PULSE_T_QFLAG         "tQflag"      /* tQflag */
#define PULSE_PULS_QTY        "pulsQty"     /* pulsQty */
#define PULSE_FR_ENA          "frEna"       /* frEna */
#define PULSE_STR_TM_SEC      "strTmSec"    /* strTmSec */
#define PULSE_STR_TM_FRAC     "strTmFrac"   /* strTmFrac */
#define PULSE_STR_TM_QFLAG    "strTmQflag"  /* strTmQflag */
#define PULSE_FR_PD           "frPd"        /* frPd */
#define PULSE_FR_RS           "frRs"        /* frRs */
#define PULSE_VALUE           "value"       /* value */
#define PULSE_RUN_INDEX       "runIndex"    /* runIndex */
#define PULSE_AVAILABLE       "available"   /* available */
#define PULSE_ASDU            "Asdu"        /* Asdu */
#define PULSE_FUNCTYPE        "FuncType"    /* FuncType */
#define PULSE_INF             "Inf"         /* Inf */
#define PULSE_GROUP           "group"       /* group */
#define PULSE_VLDTAG          "vldTag"      /* vldTag */

/* 脉冲电度数据结构表成员ID */
#define PULSE_PRO_ID_INDEX          1       /* proId */
#define PULSE_SEC_ID_INDEX          2       /* secId */
#define PULSE_PLS_ID_INDEX          3       /* plsId */
#define PULSE_NAME_INDEX            4       /* name */
#define PULSE_ALIAS_INDEX           5       /* alias */
#define PULSE_DO_INDEX              6       /* DO */
#define PULSE_ACT_VALUE_INDEX       7       /* actVal */
#define PULSE_FR_VALUE_INDEX        8       /* frVal */
#define PULSE_FR_TM_SEC_INDEX       9       /* frTmSec */
#define PULSE_FR_TM_FRAC_INDEX      10      /* frTmFrac */
#define PULSE_FR_TM_QFLAG_INDEX     11      /* frTmQflag */
#define PULSE_Q_INDEX               12      /* q */
#define PULSE_T_SEC_INDEX           13      /* tSec */
#define PULSE_T_FRAC_INDEX          14      /* tFrac */
#define PULSE_T_QFLAG_INDEX         15      /* tQflag */
#define PULSE_PULS_QTY_INDEX        16      /* pulsQty */
#define PULSE_FR_ENA_INDEX          17      /* frEna */
#define PULSE_STR_TM_SEC_INDEX      18      /* strTmSec */
#define PULSE_STR_TM_FRAC_INDEX     19      /* strTmFrac */
#define PULSE_STR_TM_QFLAG_INDEX    20      /* strTmQflag */
#define PULSE_FR_PD_INDEX           21      /* frPd */
#define PULSE_FR_RS_INDEX           22      /* frRs */
#define PULSE_VALUE_INDEX           23      /* value */
#define PULSE_RUNINDEX_INDEX        24      /* runIndex */
#define PULSE_AVAILABLE_INDEX       25      /* available */
#define PULSE_ASDU_INDEX            26      /* Asdu */
#define PULSE_FUNCTYPE_INDEX        27      /* FuncType */
#define PULSE_INF_INDEX             28      /* Inf */
#define PULSE_GROUP_INDEX           29      /* group */
#define PULSE_VLDTAG_INDEX          30      /* vldTag */

/* 菜单标题数据结构表成员名称 */
#define MENU_CAP_ID           "MenuId"      /* MenuID */
#define MENU_CAP_TAB_ID       "TableId"     /* TableID */
#define MENU_CAP_CPU_NUMBER   "CpuNumber"   /* CpuNumber */
#define MENU_CAP_TYPE         "Type"        /* Type */
#define MENU_CAP_GROUP_COUNT  "GroupCount"  /* GroupCount */
#define MENU_CAP_GROUP_NUM    "GroupNum"    /* GroupNum */
#define MENU_CAP_NAME         "Name"        /* Name */
#define MENU_CAP_VLDTAG       "vldTag"      /* vldTag */
#define MENU_CAP_AVAILABLE    "available"   /* available */

/* 菜单标题数据结构表成员ID */
#define MENU_CAP_ID_INDEX           1       /* MenuID */
#define MENU_CAP_TAB_ID_INDEX       2       /* TableID */
#define MENU_CAP_CPU_NUMBER_INDEX   3       /* CpuNumber */
#define MENU_CAP_TYPE_INDEX         4       /* Type */
#define MENU_CAP_GROUP_COUNT_INDEX  5       /* GroupCount */
#define MENU_CAP_GROUP_NUM_INDEX    6       /* GroupNum */
#define MENU_CAP_NAME_INDEX         7       /* Name */
#define MENU_CAP_VLDTAG_INDEX       8       /* vldTag */
#define MENU_CAP_AVAILABLE_INDEX    9       /* available */

/* 分组标题数据结构表成员名称 */
#define GROUP_CAP_TAB_ID      "TableId"     /* TableID */
#define GROUP_CAP_ID          "GroupId"     /* GroupID */
#define GROUP_CAP_GROUP_NUM   "GroupNum"    /* GroupNum */
#define GROUP_CAP_NAME        "Name"        /* Name */
#define GROUP_CAP_VLDTAG      "vldTag"      /* vldTag */

/* 分组标题数据结构表成员ID */
#define GROUP_CAP_TAB_ID_INDEX      1       /* TableID */
#define GROUP_CAP_ID_INDEX          2       /* GroupID */
#define GROUP_CAP_GROUP_NUM_INDEX   3       /* GroupNum */
#define GROUP_CAP_NAME_INDEX        4       /* Name */
#define GROUP_CAP_VLDTAG_INDEX      5       /* vldTag */

/* 遥测量分组数据结构表成员名称 */
#define YC_GROUPN_SEC_ID      "secId"       /* secId */
#define YC_GROUPN_ID          "grpId"       /* groupId */
#define YC_GROUPN_NAME        "name"        /* grpName */
#define YC_GROUPN_DO_NAME     "doName"      /* grpDoName */
#define YC_GROUPN_TYPE        "type"        /* grpType */
#define YC_GROUPN_AVAILABLE   "available"   /* available */
#define YC_GROUPN_VLDTAG      "vldTag"      /* vldTag */

/* 遥测量分组数据结构表成员ID */
#define YC_GROUP_SEC_ID_INDEX       1       /* secId */
#define YC_GROUP_ID_INDEX           2       /* groupId */
#define YC_GROUP_NAME_INDEX         3       /* grpName */
#define YC_GROUP_DO_NAME_INDEX      4       /* grpDoName */
#define YC_GROUP_TYPE_INDEX         5       /* grpType */
#define YC_GROUP_AVAILABLE_INDEX    6       /* available */
#define YC_GROUPN_VLDTAG_INDEX      7       /* vldTag */

/* 遥测量组成员数据结构表成员名称 */
#define YC_MEMBERN_SEC_ID     "secId"       /* secId */
#define YC_MEMBERN_GROUP_ID   "grpId"       /* groupId */
#define YC_MEMBERN_ID         "memId"       /* memId */
#define YC_MEMBERN_TYPE       "memType"     /* memType */
#define YC_MEMBERN_ANG_REF    "angRef"      /* angRef */
#define YC_MEMBERN_D          "d"           /* d */
#define YC_MEMBERN_DU         "dU"          /* dU */
#define YC_MEMBERN_NAME       "memName"     /* 成员名称 */
#define YC_MEMBERN_AVAILABLE  "available"   /* available */
#define YC_MEMBERN_VLDTAG     "vldTag"      /* vldTag */

/* 遥测量组成员数据结构表成员ID */
#define YC_MEMBER_SEC_ID_INDEX      1       /* secId */
#define YC_MEMBER_GROUP_ID_INDEX    2       /* groupId */
#define YC_MEMBER_ID_INDEX          3       /* memId */
#define YC_MEMBER_TYPE_INDEX        4       /* memType */
#define YC_MEMBER_ANG_REF_INDEX     5       /* angRef */
#define YC_MEMBER_D_INDEX           6       /* d */
#define YC_MEMBER_DU_INDEX          7       /* dU */
#define YC_MEMBER_NAME_INDEX        8       /* 成员名称 */
#define YC_MEMBER_AVAILABLE_INDEX   9       /* available */
#define YC_MEMBERN_VLDTAG_INDEX     10      /* vldTag */

/* 录波表数据结构表成员名称 */
#define WAVE_RCDN_SEC_ID      "secId"       /* secId */
#define WAVE_RCDN_PRO_ID      "proId"       /* proId */
#define WAVE_RCDN_MADE        "rcdMade"     /* rcdMade */
#define WAVE_RCDN_MADE_Q      "rcdQ"        /* rcdMade_q */
#define WAVE_RCDN_MADE_TSEC   "rcdTSec"     /* rcdMade_TSec */
#define WAVE_RCDN_MADE_TFRAC  "rcdTFrac"    /* rcdMade_TFrac */
#define WAVE_RCDN_MADE_TQFLAG "rcdTQflag"   /* rcdMade_TQflag */
#define WAVE_RCDN_FLT_NUM     "fltNum"      /* fltNum */
#define WAVE_RCDN_FLT_Q       "fltQ"        /* fltNum_q */
#define WAVE_RCDN_FLT_TSEC    "fltTSec"     /* fltNum_TSec */
#define WAVE_RCDN_FLT_TFRAC   "fltTFrac"    /* fltNum_TFrac */
#define WAVE_RCDN_FLT_TQFLAG  "fltTQflag"   /* fltNum_TQflag */
#define WAVE_RCDN_GRI_NUM     "GriFltNum"   /* GriFltNum */
#define WAVE_RCDN_GRI_Q       "griQ"        /* GriFltNum_q */
#define WAVE_RCDN_GRI_TSEC    "griTSec"     /* GriFltNum_TSec */
#define WAVE_RCDN_GRI_TFRAC   "griTFrac"    /* GriFltNum_TFrac */
#define WAVE_RCDN_GRI_TQFLAG  "griTQflag"   /* GriFltNum_TQflag */
#define WAVE_RCDN_AVAILABLE   "available"   /* available */
#define WAVE_RCDN_VLDTAG      "vldTag"      /* vldTag */

/* 录波表数据结构表成员ID */
#define WAVE_RCDN_SEC_ID_INDEX      1       /* secId */
#define WAVE_RCDN_PRO_ID_INDEX      2       /* proId */
#define WAVE_RCDN_MADE_INDEX        3       /* rcdMade */
#define WAVE_RCDN_MADE_Q_INDEX      4       /* rcdMade_q */
#define WAVE_RCDN_MADE_TSEC_INDEX   5       /* rcdMade_TSec */
#define WAVE_RCDN_MADE_TFRAC_INDEX  6       /* rcdMade_TFrac */
#define WAVE_RCDN_MADE_TQFLAG_INDEX 7       /* rcdMade_TQflag */
#define WAVE_RCDN_FLT_NUM_INDEX     8       /* fltNum */
#define WAVE_RCDN_FLT_Q_INDEX       9       /* fltNum_q */
#define WAVE_RCDN_FLT_TSEC_INDEX    10      /* fltNum_TSec */
#define WAVE_RCDN_FLT_TFRAC_INDEX   11      /* fltNum_TFrac */
#define WAVE_RCDN_FLT_TQFLAG_INDEX  12      /* fltNum_TQflag */
#define WAVE_RCDN_GRI_NUM_INDEX     13      /* GriFltNum */
#define WAVE_RCDN_GRI_Q_INDEX       14      /* GriFltNum_q */
#define WAVE_RCDN_GRI_TSEC_INDEX    15      /* GriFltNum_TSec */
#define WAVE_RCDN_GRI_TFRAC_INDEX   16      /* GriFltNum_TFrac */
#define WAVE_RCDN_GRI_TQFLAG_INDEX  17      /* GriFltNum_TQflag */
#define WAVE_RCDN_AVAILABLE_INDEX   18      /* AVAILABLE */
#define WAVE_RCDN_VLDTAG_INDEX      19      /* vldTag */

/* 复归LED数据结构表成员名称 */
#define LEDN_PRO_ID         "proId"         /* pro_id */
#define LEDN_SEC_ID         "secId"         /* sec_id */
#define LEDN_ID             "ledId"         /* ID */
#define LEDN_AVAILABLE      "available"     /* available */
#define LEDN_INF            "inf"           /* inf */
#define LEDN_VARINF         "VarInf"        /* variinf */
#define LEDN_NAME           "name"          /* name */
#define LEDN_ALIAS          "alias"         /* alias */
#define LEDN_STATUS         "status"        /* status */
#define LEDN_DO             "doName"        /* DO */
#define LEDN_Q              "Q"             /* q */
#define LEDN_T_SEC          "tSec"          /* t_sec */
#define LEDN_T_FRAC         "tFrac"         /* t_frac */
#define LEDN_T_Q_FLAG       "tQflag"        /* t_q_flag */
#define LEDN_SUB_ENA        "subEna"        /* subEna */
#define LEDN_SUB_VAL        "subVal"        /* subVal */
#define LEDN_SUB_Q          "subQ"          /* subQ */
#define LEDN_SUB_ID         "subID"         /* subID */
#define LEDN_CTL_MODEL      "ctlModel"      /* ctlModel */
#define LEDN_OPER_CTL_VAL   "operCtlVal"    /* Oper_ctlVal */
#define LEDN_OPER_ORIGIN    "operOrCat"    /* Oper_origin */
#define LEDN_OPER_ORIGIN_IDENT "operOrId" /* Oper_originIdent */
#define LEDN_OPER_CTL_NUM   "operCtlNum"    /* Oper_ctlNum */
#define LEDN_OPER_T_SEC     "operTSec"      /* Oper_T_sec */
#define LEDN_OPER_T_FRAC    "operTFrac"     /* Oper_T_frac */
#define LEDN_OPER_T_QFLAG   "operTQflag"    /* Oper_T_qflag */
#define LEDN_OPER_TEST      "operTest"      /* Oper_Test */
#define LEDN_OPER_CHECK     "operCheck"     /* Oper_Check */
#define LEDN_OPER_VLDTAG    "vldTag"        /* vldTag */

/* 复归LED数据结构表成员ID */
#define LEDN_PRO_ID_INDEX         1         /* pro_id */
#define LEDN_SEC_ID_INDEX         2         /* sec_id */
#define LEDN_ID_INDEX             3         /* id */
#define LEDN_AVAILABLE_INDEX      4         /* available */
#define LEDN_INF_INDEX            5         /* inf */
#define LEDN_VARINF_INDEX         6         /* variinf */
#define LEDN_NAME_INDEX           7         /* name */
#define LEDN_ALIAS_INDEX          8         /* alias */
#define LEDN_STATUS_INDEX         9         /* status */
#define LEDN_DO_INDEX             10        /* DO */
#define LEDN_Q_INDEX              11        /* q */
#define LEDN_T_SEC_INDEX          12        /* t_sec */
#define LEDN_T_FRAC_INDEX         13        /* t_frac */
#define LEDN_T_Q_FLAG_INDEX       14        /* t_q_flag */
#define LEDN_SUB_ENA_INDEX        15        /* subEna */
#define LEDN_SUB_VAL_INDEX        16        /* subVal */
#define LEDN_SUB_Q_INDEX          17        /* subQ */
#define LEDN_SUB_ID_INDEX         18        /* subID */
#define LEDN_CTL_MODEL_INDEX      19        /* ctlModel */
#define LEDN_OPER_CTL_VAL_INDEX   20        /* Oper_ctlVal */
#define LEDN_OPER_ORIGIN_INDEX    21        /* Oper_origin */
#define LEDN_OPER_ORIGIN_IDENT_INDEX 22     /* Oper_originIdent */
#define LEDN_OPER_CTL_NUM_INDEX   23        /* Oper_ctlNum */
#define LEDN_OPER_T_SEC_INDEX     24        /* Oper_T_sec */
#define LEDN_OPER_T_FRAC_INDEX    25        /* Oper_T_frac */
#define LEDN_OPER_T_QFLAG_INDEX   26        /* Oper_T_qflag */
#define LEDN_OPER_TEST_INDEX      27        /* Oper_Test */
#define LEDN_OPER_CHECK_INDEX     28        /* Oper_Check */
#define LEDN_OPER_VLDTAG_INDEX    29        /* vldTag */

/* 数字开出量(软开出单点,软开出双点)数据结构表成员名称 */
#define SOUTPUT_TYPE          "type"        /* type */
#define SOUTPUT_PRO_ID        "proId"       /* pro_id */
#define SOUTPUT_SECT_ID       "secId"       /* sec_id */
#define SOUTPUT_ID            "outID"       /* OutID */
#define SOUTPUT_AVAILABLE     "available"   /* vailable */
#define SOUTPUT_INF           "inf"         /* inf */
#define SOUTPUT_NAME          "name"        /* name */
#define SOUTPUT_ASDU          "Asdu"        /* Asdu */
#define SOUTPUT_FUNCTYPE      "FuncType"    /* FuncType */
#define SOUTPUT_SRC_TABLE_ID  "SrcTableID"  /* SrcTableID */
#define SOUTPUT_SRC_DATA_ID1  "SrcDataID1"  /* SrcDataID1 */
#define SOUTPUT_SRC_DATA_ID2  "SrcDataID2"  /* SrcDataID2 */
#define SOUTPUT_STATUS        "status"      /* status */
#define SOUTPUT_DO            "doName"      /* DO */
#define SOUTPUT_SUB_ENA       "subEna"      /* subEna */
#define SOUTPUT_SUB_VAL       "subVal"      /* subVal */
#define SOUTPUT_SUB_Q         "subQ"        /* subQ */
#define SOUTPUT_SUB_ID        "subID"       /* subID */
#define SOUTPUT_Q             "q"           /* q */
#define SOUTPUT_T_SEC         "tSec"        /* tSec */
#define SOUTPUT_T_FRAC        "tFrac"       /* tFrac */
#define SOUTPUT_T_QFLAG       "tQflag"      /* tQflag */
#define SOUTPUT_SRC_DATA_TYPE "srcDataType" /* SrcDataType */
#define SOUTPUT_SRC_COM_CHL_TYPE "SrcComChlT" /* SrcComChlT */
#define SOUTPUT_VLDTAG           "vldTag"     /* vldTag */

/* 数字开出量(软开出单点,软开出双点)数据结构表成员ID */
#define SOUTPUT_TYPE_INDEX        1         /* type */
#define SOUTPUT_PRO_ID_INDEX      2         /* pro_id */
#define SOUTPUT_SECT_ID_INDEX     3         /* sec_id */
#define SOUTPUT_ID_INDEX          4         /* OutID */
#define SOUTPUT_AVAILABLE_INDEX   5         /* vailable */
#define SOUTPUT_INF_INDEX         6         /* inf */
#define SOUTPUT_NAME_INDEX        7         /* name */
#define SOUTPUT_ASDU_INDEX        8         /* Asdu */
#define SOUTPUT_FUNCTYPE_INDEX    9         /* FuncType */
#define SOUTPUT_SRCTABLEID_INDEX  10        /* SrcTableID */
#define SOUTPUT_SRCDATAID1_INDEX  11        /* SrcDataID1 */
#define SOUTPUT_SRCDATAID2_INDEX  12        /* SrcDataID2 */
#define SOUTPUT_STATUS_INDEX      13        /* status */
#define SOUTPUT_DO_INDEX          14        /* DO */
#define SOUTPUT_SUBENA_INDEX      15        /* subEna */
#define SOUTPUT_SUBVAL_INDEX      16        /* subVal */
#define SOUTPUT_SUBQ_INDEX        17        /* subQ */
#define SOUTPUT_SUBID_INDEX       18        /* subID */
#define SOUTPUT_Q_INDEX           19        /* q */
#define SOUTPUT_T_SEC_INDEX       20        /* tSec */
#define SOUTPUT_T_FRAC_INDEX      21        /* tFrac */
#define SOUTPUT_T_QFLAG_INDEX     22        /* tQflag */
#define SOUTPUT_SRC_DATA_TYPE_INDEX 23      /* SrcDataType */
#define SOUTPUT_SRC_COM_CHL_TYPE_INDEX 24   /* SrcComChlT */
#define SOUTPUT_VLDTAG_INDEX      25        /* vldTag */

/* 复合数据成员分组表成员名称 */
#define CMEMBN_PRO_ID       "proId"         /* pro_id */
#define CMEMBN_SEC_ID       "secId"         /* sec_id */
#define CMEMBN_ID           "memId"         /* mem_id */
#define CMEMBN_AVAILABLE    "available"     /* available */
#define CMEMBN_TAB_ID       "tabId"         /* tab_id */
#define CMEMBN_TYPE         "memType"       /* mem_type */
#define CMEMBN_REF          "memRef"        /* mem_ref */
#define CMEMBN_D            "memd"          /* mem_d */
#define CMEMBN_DU           "memdU"         /* mem_dU */
#define CMEMBN_NAME         "memName"       /* mem_name */
#define CMEMBN_CLASS        "memClass"      /* mem_class */
#define CMEMBN_VLDTAG       "vldTag"        /* vldTag */

/* 复合数据成员分组表成员ID */
#define CMEMBN_PRO_ID_INDEX       1         /* pro_id */
#define CMEMBN_SEC_ID_INDEX       2         /* sec_id */
#define CMEMBN_ID_INDEX           3         /* mem_id */
#define CMEMBN_AVAILABLE_INDEX    4         /* available */
#define CMEMBN_TAB_ID_INDEX       5         /* tab_id */
#define CMEMBN_TYPE_INDEX         6         /* mem_type */
#define CMEMBN_REF_INDEX          7         /* mem_ref */
#define CMEMBN_D_INDEX            8         /* mem_d */
#define CMEMBN_DU_INDEX           9         /* mem_dU */
#define CMEMBN_NAME_INDEX         10        /* mem_name */
#define CMEMBN_CLASS_INDEX        11        /* mem_class */
#define CMEMBN_VLDTAG_INDE        12        /* vldTag */

/* 访问点数据结构表成员名称 */
#define CACSN_ID            "acsId"         /* Id */
#define CACSN_AVAILABLE     "available"     /* available */
#define CACSN_NAME          "name"          /* name */
#define CACSN_DO            "doName"        /* DO */
#define CACSN_TYPE          "acsType"       /* acsType */
#define CACSN_VLDTAG        "vldTag"        /* vldTag */

/* 访问点数据结构表成员ID */
#define CACSN_ID_INDEX            1         /* Id */
#define CACSN_AVAILABLE_INDEX     2         /* available */
#define CACSN_NAME_INDEX          3         /* name */
#define CACSN_DO_INDEX            4         /* DO */
#define CACSN_TYPE_INDEX          5         /* acsType */
#define CACSN_VLDTAG_INDEX        6         /* vldTag */

/* 枚举类型数据结构表成员名称 */
#define ENUMTN_TYPE         "type"          /* type */
#define ENUMTN_NAME         "name"          /* name */
#define ENUMTN_TABLE_ID     "tableId"       /* table ID */
#define ENUMTN_VLDTAG       "vldTag"        /* vldTag */

/* 枚举类型数据结构表成员ID */
#define ENUMTN_TYPE_INDEX         1         /* type */
#define ENUMTN_NAME_INDEX         2         /* name */
#define ENUMTN_TABLE_ID_INDEX     3         /* table ID */
#define ENUMTN_VLDTAG_INDEX       4         /* vldTag */

/* 枚举值数据结构表成员名称 */
#define ENUMVN_TYPE         "type"          /* type */
#define ENUMVN_VALUE        "value"         /* value */
#define ENUMVN_NAME         "name"          /* name */
#define ENUMVN_AVAILABLE    "available"     /* available */
#define ENUMVN_VLDTAG       "vldTag"        /* vldTag */

/* 枚举值数据结构表成员ID */
#define ENUMVN_TYPE_INDEX         1         /* type */
#define ENUMVN_VALUE_INDEX        2         /* value */
#define ENUMVN_NAME_INDEX         3         /* name */
#define ENUMVN_AVAILABLE_INDEX    4         /* available */
#define ENUMVN_VLDTAG_INDEX       5         /* vldTag */

/* 描述定值数据结构表成员名称 */
#define DESC_SET_ID               "Id"          /* Id */
#define DESC_SET_GROUP_TYPE       "group_type"  /* group_type */
#define DESC_SET_AVAILABLE        "available"   /* available */
#define DESC_SET_NAME             "name"        /* name */
#define DESC_SET_ALAIS            "alias"       /* alias */
#define DESC_SET_VALUE            "Value"       /* Value */
#define DESC_SET_VLDTAG           "vldTag"      /* vldTag */

/* 描述定值数据结构表成员ID */
#define DESC_SET_ID_INDEX          1   /* Id */
#define DESC_SET_GROUP_TYPE_INDEX  2   /* group_type */
#define DESC_SET_AVAILABLE_INDEX   3   /* available */
#define DESC_SET_NAME_INDEX        4   /* name */
#define DESC_SET_ALAIS_INDEX       5   /* alias */
#define DESC_SET_VALUE_INDEX       6   /* Value */
#define DESC_SET_VLDTAG_INDEX      7   /* vldTag */

/* 充电桩数据结构表成员名称 */
#define CHARGE_ID                   "Id"              /* Id */
#define CHARGE_PRO_ID               "proId"           /* pro_id           */
#define CHARGE_SEC_ID               "secId"           /* sec_id           */
#define CHARGE_AVAILABLE            "available"       /* charge_available */
#define CHARGE_CARD_ID              "cardId"          /* Card_id          */
#define CHARGE_PASSWORD             "password"        /* Card_password    */
#define CHARGE_REM_MONEY            "remMoney"        /* Remain_money     */
#define CHARGE_PREP_MONEY           "prepMoney"       /* Prepaid_money    */
#define CHARGE_CARD_STATE           "cardState"       /* Card_state       */
#define CHARGE_STOPCHA_TYPE         "StopChaType"     /* Terminal_type    */
#define CHARGE_EXPEND_SET           "ExpendSet"       /* Charge_expend    */
#define CHARGE_KWH_SET              "KWHSet"          /* Charge_KWH       */
#define CHARGE_RESERV_TIME_H        "resvTimeH"       /* reserveTimeH     */
#define CHARGE_RESERV_TIME_M        "resvTimeM"       /* reserveTimeM     */
#define CHARGE_OPER_OVER_TIME       "operOverT"       /* operateOverT     */
#define CHARGE_SYS_VOLTAGE          "sysVoltage"      /* sysVoltage       */
#define CHARGE_SYS_CURRENT          "sysCurrent"      /* sysCurrent       */
#define CHARGE_START_TIME           "chgStartT"       /* charge_FinalT    */
#define CHARGE_FINAL_TIME           "chgFinalT"       /* charge_FinalT    */
#define CHARGE_PERIOD               "chgPeriod"       /* Charge_period    */
#define CHARGE_START_VALUE          "StartValue"      /* Charge_TotalStartValue  */
#define CHARGE_R1START_VALUE        "R1StartVal"      /* Charge_Rate1StartValue  */
#define CHARGE_R2START_VALUE        "R2StartVal"      /* Charge_Rate2StartValue  */
#define CHARGE_R3START_VALUE        "R3StartVal"      /* Charge_Rate3StartValue  */
#define CHARGE_R4START_VALUE        "R4StartVal"      /* Charge_Rate4StartValue  */
#define CHARGE_FINAL_VALUE          "FinalValue"      /* Charge_TotalFinalValue  */
#define CHARGE_R1FINAL_VALUE        "R1FinallVal"     /* Charge_Rate1TerminalValue */
#define CHARGE_R2FINAL_VALUE        "R2FinallVal"     /* Charge_Rate2TerminalValue */
#define CHARGE_R3FINAL_VALUE        "R3FinallVal"     /* Charge_Rate3TerminalValue */
#define CHARGE_R4FINAL_VALUE        "R4FinallVal"     /* Charge_Rate4TerminalValue */
#define CHARGE_EXPEND_USED          "ExpendUsed"      /* Charge_ ExpendUsed */
#define CHARGE_KWH_USED             "KWHUsed"         /* Charge_ KWHUsed    */
#define CHARGE_TIME_USED            "TimeUsed"        /* ChargeTime_Unused  */
#define CHARGE_USER_VOL             "UserVol"         /* Charge_Voltage     */
#define CHARGE_USER_CUR             "UserCur"         /* Charge_Current     */
#define CHARGE_BAT_SOC              "batSOC"          /* BatterySOC         */
#define CHARGE_SET_OVER             "SetOver"         /* ChargeSetOver      */
#define CHARGE_PAGE_STATE           "pageState"       /* Page_State         */
#define CHARGE_MODE                 "chgMode"         /* Charge_Mode        */
#define CHARGE_TYPE                 "chgType"         /* Charge_Type        */
#define CHARGE_CHECK_CYCLE          "CheckCycle"      /* SysCheck_Cycle     */
#define CHARGE_METER_COMMSTA        "MComState"       /* Meter_CommState    */
#define CHARGE_CARD_COMMSTA         "CComState"       /* Card_CommState     */
#define CHARGE_SAVECHARGE_FLAG      "SaveCFlag"       /* SaveCharge_Flag    */
#define CHARGE_PAGEOPER_FLAG        "POperFlag"       /* Page_OperFlag      */
#define CHARGE_PSWITCH_FLAG         "PSwitch"         /* Page_Switch        */
#define CHARGE_SYS_STATUS           "SysStatus"       /* Page_Switch        */
#define CHARGE_PRESYS_STA           "PreSysStatu"     /* Page_Switch        */
#define CHARGE_PRECARDID            "PreCardId"       /* Page_Switch        */
#define CHARGE_CANBMS_STA           "CanBmsStatu"     /* Page_Switch        */
#define CHARGE_CANBRT_STA           "CanRtStatu"      /* Page_Switch        */
#define CHARGE_SETCHARGE_H          "SetChargeH"      /* Page_Switch        */
#define CHARGE_SETCHARGE_M          "SetChargeM"      /* Page_Switch        */
#define CHARGE_RESERVE_I1           "ReserveI1"       /* reserve int 1      */
#define CHARGE_RESERVE_I2           "ReserveI2"       /* reserve int 2      */
#define CHARGE_RESERVE_F1           "ReserveF1"       /* reserve float 1    */
#define CHARGE_RESERVE_F2           "ReserveF2"       /* reserve float 2    */
#define CHARGE_RESERVE_S1           "ReserveS1"       /* reserve string 1   */
#define CHARGE_RESERVE_S2           "ReserveS2"       /* reserve string 2   */
#define CHARGE_VLDTAG               "vldTag"          /* vldTag */
#define CHARGE_RESVYEAR             "resvYear"        /* resvYear */
#define CHARGE_RESVMONTH            "resvMonth"       /* resvMonth */
#define CHARGE_RESVDAY              "resvDay"         /* resvDay */

/* 充电桩数据结构表成员ID */
#define CHARGE_ID_INDEX                   1      /* Id */
#define CHARGE_PRO_ID_INDEX               2      /* pro_id           */
#define CHARGE_SEC_ID_INDEX               3      /* sec_id           */
#define CHARGE_AVAILABLE_INDEX            4      /* charge_available */
#define CHARGE_CARD_ID_INDEX              5      /* Card_id          */
#define CHARGE_PASSWORD_INDEX             6      /* Card_password    */
#define CHARGE_REM_MONEY_INDEX            7      /* Remain_money     */
#define CHARGE_PREP_MONEY_INDEX           8      /* Prepaid_money    */
#define CHARGE_CARD_STATE_INDEX           9      /* Card_state       */
#define CHARGE_STOPCHA_TYPE_INDEX         10     /* Terminal_type    */
#define CHARGE_EXPEND_SET_INDEX           11     /* Charge_expend    */
#define CHARGE_KWH_SET_INDEX              12     /* Charge_KWH       */
#define CHARGE_RESERV_TIME_H_INDEX        13     /* reserveTimeH     */
#define CHARGE_RESERV_TIME_M_INDEX        14     /* reserveTimeM     */
#define CHARGE_OPER_OVER_TIME_INDEX       15     /* operateOverT     */
#define CHARGE_SYS_VOLTAGE_INDEX          16     /* sysVoltage       */
#define CHARGE_SYS_CURRENT_INDEX          17     /* sysCurrent       */
#define CHARGE_START_TIME_INDEX           18     /* charge_FinalT    */
#define CHARGE_FINAL_TIME_INDEX           19     /* charge_FinalT    */
#define CHARGE_PERIOD_INDEX               20     /* Charge_period    */
#define CHARGE_START_VALUE_INDEX          21     /* Charge_TotalStartValue  */
#define CHARGE_R1START_VALUE_INDEX        22     /* Charge_Rate1StartValue  */
#define CHARGE_R2START_VALUE_INDEX        23     /* Charge_Rate2StartValue  */
#define CHARGE_R3START_VALUE_INDEX        24     /* Charge_Rate3StartValue  */
#define CHARGE_R4START_VALUE_INDEX        25     /* Charge_Rate4StartValue  */
#define CHARGE_FINAL_VALUE_INDEX          26     /* Charge_TotalFinalValue  */
#define CHARGE_R1FINAL_VALUE_INDEX        27     /* Charge_Rate1TerminalValue */
#define CHARGE_R2FINAL_VALUE_INDEX        28     /* Charge_Rate2TerminalValue */
#define CHARGE_R3FINAL_VALUE_INDEX        29     /* Charge_Rate3TerminalValue */
#define CHARGE_R4FINAL_VALUE_INDEX        30     /* Charge_Rate4TerminalValue */
#define CHARGE_EXPEND_USED_INDEX          31     /* Charge_ ExpendUsed */
#define CHARGE_KWH_USED_INDEX             32     /* Charge_ KWHUsed    */
#define CHARGE_TIME_USED_INDEX            33     /* ChargeTime_Used   */
#define CHARGE_USER_VOL_INDEX             34     /* Charge_Voltage     */
#define CHARGE_USER_CUR_INDEX             35     /* Charge_Current     */
#define CHARGE_BAT_SOC_INDEX              36     /* BatterySOC         */
#define CHARGE_SET_OVER_INDEX             37     /* ChargeSetOver      */
#define CHARGE_PAGE_STATE_INDEX           38     /* Page_State         */
#define CHARGE_MODE_INDEX                 39     /* Charge_Mode        */
#define CHARGE_TYPE_INDEX                 40     /* Charge_Type        */
#define CHARGE_CHECK_CYCLE_INDEX          41     /* SysCheck_Cycle     */
#define CHARGE_METER_COMMSTA_INDEX        42     /* Meter_CommState    */
#define CHARGE_CARD_COMMSTA_INDEX         43     /* Card_CommState     */
#define CHARGE_SAVECHARGE_FLAG_INDEX      44     /* SaveCharge_Flag    */
#define CHARGE_PAGEOPER_FLAG_INDEX        45     /* Page_OperFlag      */
#define CHARGE_PAGESWITCH_FLAG_INDEX      46     /* Page_OperFlag      */
#define CHARGE_SYS_STATUS_INDEX           47     /* Page_Switch        */
#define CHARGE_PRESYS_STA_INDEX           48     /* Page_Switch        */
#define CHARGE_PRECARDID_INDEX            49     /* Page_Switch        */
#define CHARGE_CANBMS_STA_INDEX           50     /* Page_Switch        */
#define CHARGE_CANBRT_STA_INDEX           51     /* Page_Switch        */
#define CHARGE_SETCHARGE_H_INDEX          52     /* Page_Switch        */
#define CHARGE_SETCHARGE_M_INDEX          53     /* Page_Switch        */
#define CHARGE_RESERVE_I1_INDEX           54     /* reserve int 1      */
#define CHARGE_RESERVE_I2_INDEX           55     /* reserve int 2      */
#define CHARGE_RESERVE_F1_INDEX           56     /* reserve float 1    */
#define CHARGE_RESERVE_F2_INDEX           57     /* reserve float 2    */
#define CHARGE_RESERVE_S1_INDEX           58     /* reserve string 1   */
#define CHARGE_RESERVE_S2_INDEX           59     /* reserve string 2   */
#define CHARGE_VLDTAG_INDEX               60     /* vldTag */
#define CHARGE_RESVYEAR_INDEX             61     /* resvYear */
#define CHARGE_RESVMONTH_INDEX            62     /* resvMonth */
#define CHARGE_RESVDAY_INDEX              63     /* resvDay */

/* NPI板块的网口物理设置结构表成员名称 */
#define NPI_PHY_SET_ID               "Id"           /* Id */
#define NPI_PHY_SET_AVAILABLE        "available"    /* available */
#define NPI_PHY_SET_NPIID            "npiId"        /* npiId */
#define NPI_PHY_SET_NETID            "netId"        /* netId */
#define NPI_PHY_SET_TYPE             "type"         /* type */
#define NPI_PHY_SET_NAME             "name"         /* name */
#define NPI_PHY_SET_VLDTAG           "vldTag"       /* vldTag */

/* NPI板块的网口物理设置结构表成员ID */
#define NPI_PHY_SET_ID_INDEX         1              /* Id */
#define NPI_PHY_SET_AVAILABLE_INDEX  2              /* available */
#define NPI_PHY_SET_NPIID_INDEX      3              /* npiId */
#define NPI_PHY_SET_NETID_INDEX      4              /* netId */
#define NPI_PHY_SET_TYPE_INDEX       5              /* type */
#define NPI_PHY_SET_NAME_INDEX       6              /* name */
#define NPI_PHY_SET_VLDTAG_INDEX     7              /* vldTag */

/* Define device block data structure. */
typedef struct TAB_DEVICE_STRUCT
{
    RCHAR       Device_CName[RCHARL( DEVICE_CNAME )];   /* device name(中文名称) */
    RCHAR       Device_EName[RCHARL( DEVICE_ENAME )];   /* device name(英文名称) */
    UNSIGNED    soe_mode;                               /*soe_mode */
    UNSIGNED    print_delay_time;                       /*print_delay_time */
    UNSIGNED    reset_bit;                              /*reset_bit */
    UNSIGNED    copy_set_logic;                         /*copy_set_logic */
    UNSIGNED    otl_diff_logic;                         /*otl_diff_logic */
    UNSIGNED    Ver_Low;                                /* version low word */
    UNSIGNED    Ver_High;                               /* version high word */
    UNSIGNED    Cpu_Number;                             /* cpu_number */
    UNSIGNED    Cpu_Type;                               /* cpu type */
    UNSIGNED    Cpu_Zone;                               /* cpu zone */
    UNSIGNED    Cpu_Zone_varinf;                        /* cpu zone change inf */
    UNSIGNED    t_sec;                                  /* t.sec */
    UNSIGNED    t_frac;                                 /* t.tFrac */
    UNSIGNED    t_qflag;                                /* t.qflag */
    UNSIGNED    TimeType;                               /* TimeType */
    INT         BreakerEnable;                          /* BreakerEnable */
    INT         TimeZone;                               /* TimeZone */
    UNSIGNED    MinSetZone;                             /* 起始定值区号 */
    UNSIGNED    SetZoneNum;                             /* 定值区个数 */
    UNSIGNED    CurSetZone;                             /* 当前定值区号 */
    UNSIGNED    ExtResetBit;                            /* 扩展复归开入位 */
    UNSIGNED    RulesSelect;                            /* 规约选择 */
    INT         FuncType;                               /* FuncType功能类型 */
    UNSIGNED    FuncSelect;                             /* 装置功能选择字段 */
    UNSIGNED    desc_id;                                /* 描述定值ID */
    INT         Year;                                   /* 程序完成时间:年*/
    INT         Month;                                  /* 程序完成时间:月*/
    INT         Day;                                    /* 程序完成时间:日*/
    INT         Hour;                                   /* 程序完成时间:时*/
    INT         Minute;                                 /* 程序完成时间:分*/
    INT         Second;                                 /* 程序完成时间:秒*/
    INT         ParaShowType;                           /* 参数显示类型 */
    INT         SettSetType;                            /* 定值整定类型 */
    INT         SetCCodeGp;                             /* 定值汉字编码组号 */
    INT         SamplePoint;                            /* 每周波采样点数 */
    INT         SmvBSInBit;                             /* SMV断链告警起始开入位 */
    INT         GosBSInBit;                             /* GOOSE断链告警起始开入位 */
    INT         vldTag;
    INT         HInGrNum;                               /* 装置硬开入组数 */
    UNSIGNED    NPIType;                                /* NPI型号 */
    INT         HOutGrNum;                              /* 装置硬开出组数 */
    UNSIGNED    SynSrc;                                 /* 对时来源 */
}TAB_DEVICE;

/*------------------------------------------------------*/

/* 装置数据表内存首址 */
extern TAB_DEVICE * pDeviceTab;
/* 录波状态量数据表内存首址 */
extern STATE * pWaveStateTab;
/* 录波状态量数据表的数据个数 */
extern INT WaveStateTabNumber;
/* 开入量数据表内存首址 */
extern INPUT * pInputTab;
/* 开入量数据表的数据个数 */
extern INT InputTabNumber;
/* 开出量数据表内存首址 */
extern OUTPUT * pOutputTab;
/* 开出量数据表的数据个数 */
extern INT OutputTabNumber;
/* 检修压板数据表内存首址 */
extern RJUMPER * pRJumperTab;
/* 检修压板数据表的数据个数 */
extern INT RJumperTabNumber;

#define GET_DEV_TAB_MEMORY         pDeviceTab           /* 获取数据表内存首址 */
#define GET_WAVE_STATE_TAB_MEMORY  pWaveStateTab        /* 录波状态量数据表内存首址 */
#define GET_WAVE_STATE_TAB_NUMBER  WaveStateTabNumber   /* 录波状态量数据表的数据个数 */
#define GET_INPUT_TAB_MEMORY       pInputTab            /* 开入量数据表内存首址 */
#define GET_INPUT_TAB_NUMBER       InputTabNumber       /* 开入量数据表的数据个数 */
#define GET_OUTPUT_TAB_MEMORY      pOutputTab           /* 开出量数据表内存首址 */
#define GET_OUTPUT_TAB_NUMBER      OutputTabNumber      /* 开出量数据表的数据个数 */
#define GET_CHANNEL_TAB_MEMORY     pChannelTab          /* 模拟量通道数据表内存首址 */
#define GET_CHANNEL_TAB_NUMBER     ChannelTabNumber     /* 模拟量通道数据表的数据个数 */
#define GET_RJUMPER_TAB_NUMBER     RJumperTabNumber     /* 检修压板数据表的数据个数 */
#define GET_RJUMPER_TAB_MEMORY     pRJumperTab          /* 检修压板数据表内存首址 */

#endif
