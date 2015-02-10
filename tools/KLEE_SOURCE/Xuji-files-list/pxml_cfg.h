/**************************************************************
** Copyright (C) 2002-2007 基础技术开发部
** All right reserved
***************************************************************
**
** 项目名称：NPI801
** 功能简介：全局配置处理模块
**
** 原始作者：周劲羽
** 组织部门：许继集团 基础技术开发部
** 备    注：
** 建立时间：2007-05-25
** 完成时间：2007-06-20
** 版    本：1.0
***************************************************************
** 单元标识：$Id: pxml_cfg.h,v 1.3 2012/03/29 01:20:42 jyj Exp $
** 版本历史：
** 修 改 者：
** 最近修改：
**************************************************************/

#ifndef  PXML_CFG_INC
#define  PXML_CFG_INC

/* 不使用的保护通道号 */
#define UNUSED_PRO_ID                  0xFFFF

#define PXML_CFG_DATA_COUNT            "count"         /* 数据个数节点名称 */
#define PXML_CFG_CPU_ID_NODE_NAME      "cpuid"         /* cpuid 节点名称 */

/* SV数据接收 */
#define PXML_CFG_PRO_CHL_ID_NODE_NAME  "pro_chl_id"    /* pro_chl_id 节点名称 */
#define PXML_CFG_ABS_CHL_ID_NODE_NAME  "abs_chl_id"    /* abs_chl_id 节点名称 */

/* GOOSE数据接收 */
#define PXML_CFG_GR_GROUP_ID_NODE_NAME    "groupid"      /* groupid 节点名称 */
#define PXML_CFG_GR_MASK_BITS_NODE_NAME   "MaskBits"     /* MaskBits 节点名称 */
#define PXML_CFG_GR_T_MASK_BITS_NODE_NAME "MaskBitsT"    /* MaskBitsT 节点名称 */

/* goose接收SV数据节点定义 */
#define PXML_CFG_PRO_GSV_ID_NODE_NAME   "pro_gsv_id"   /* pro_gsv_id 节点名称 */
#define PXML_CFG_ABS_GSV_ID_NODE_NAME   "abs_gsv_id"   /* abs_gsv_id 节点名称 */

/* 初始化平台配置文件 */
int PXML_CFG_InitConfig( CHAR * fileName );
/* 获取通道信息,返回通道个数 */
INT PXML_CFG_GetChlInfo( INT iCpuID, INT * pProChlId, INT * pAbsChlId, INT iMaxCount );
/* 获取配置数据的CPU信息;返回值为CPU个数 */
INT PXML_CFG_GetCpuInfo( INT * pCpuId, INT iMaxCount );
/* 获取goose接收数据信息 */
INT PXML_CFG_GetGooseRecvInfo( INT GroupNo, UNSIGNED  * pMaskBits );
/* 获取GOOSE开出掩码初始化状态 */
INT PXML_CFG_GetGRecvInitStatus( VOID );
/* 获取带时标的goose接收数据信息 */
INT PXML_CFG_GetTimeGooseRecvInfo( INT GroupNo, UNSIGNED  * pMaskBitsT );

#endif
