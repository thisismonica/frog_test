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
** 单元标识：$Id: pxml_cfg.c,v 1.7 2012/03/29 01:20:42 jyj Exp $
** 版本历史：
** 修 改 者：
** 最近修改：
**************************************************************/
#include "sysincs.h"
#include "sx_defs.h"
#include "sx_log.h"
#include "mem_chk.h"
#include "plat/inc/esp_cfg.h"
#include "plat_specific/plat_specific.h"
#include "plat_specific/inc/cpc_defs.h"
#include "xml/inc/pxml_cfg.h"
#include "plat/inc/csmv_extr.h"

#define PXML_CFG_MAX_CPU_NUMBER      CPC_MAX_CPU_NUMBER                           /* 装置最大分模块个数 */
#define PXML_INIT_FLAG               0x5A                                         /* 初始化完成标识 */

#define PXML_CFG_CPU_COUNT_OFFSET    0        /* CPU信息数据个数偏移 */
#define PXML_CFG_CPU_DATA_OFFSET     1        /* CPU信息数据偏移 */

/* 解析主节点 */
static ST_VOID _sCfg_SEFun ( SX_DEC_CTRL * sxDecCtrl );
/* SMV通道解析 */
static ST_VOID _smvrecv_SEFun ( SX_DEC_CTRL * sxDecCtrl );
static ST_VOID _smv_SERFun ( SX_DEC_CTRL * sxDecCtrl );
static ST_VOID _smv_chl_SERFun ( SX_DEC_CTRL * sxDecCtrl );
/* GOOSE接收通道解析 */
static ST_VOID _goose_recv_SEFun ( SX_DEC_CTRL * sxDecCtrl );
static ST_VOID _goose_SERFun ( SX_DEC_CTRL * sxDecCtrl );
static ST_VOID _goose_rc_SERFun ( SX_DEC_CTRL * sxDecCtrl );
/* goose接收SV通道解析 */
static ST_VOID _GooseSVRecv_SEFun ( SX_DEC_CTRL * sxDecCtrl );  /* 解析 GooseSV 接收节点 */
static ST_VOID _GooseSV_SERFun ( SX_DEC_CTRL * sxDecCtrl );     /* 解析 GooseSV 子项 */
static ST_VOID _GooseSV_Chl_SERFun ( SX_DEC_CTRL * sxDecCtrl ); /* 解析 GooseSV 通道项 */

/* 从 xml 数据中读取配置参数 */
ST_RET PXML_CFG_LoadConfigFromXML( const ST_CHAR * xml, ST_INT len );
/* 取得 XML 配置数据 */
ST_CHAR * PXML_CFG_GetXML( CHAR * fileName );

/* 每个SV通道信息 */
typedef struct EACH_CHL_INFO_struct
{
    INT iCpuId;      /* CpuId */
    INT iProChlID;   /* 保护CPU通道ID */
    INT iAbsChlID;   /* 绝对通道ID */
}EACH_CHL_INFO;

/* 每GSV通道信息 */
typedef struct EACH_GSV_INFO_struct
{
    INT iCpuId;      /* CpuId */
    INT iProGSVID;   /* 保护GSV_ID */
    INT iAbsGSVID;   /* 装置GSV_ID */
}EACH_GSV_INFO;

/* 每组goose接收信息 */
typedef struct EACH_GOOSE_RECV_struct
{
    INT      iGroupID;    /* 组号 */
    UNSIGNED uMaskBits;   /* 屏蔽位 */
    UNSIGNED uMaskBitsT;  /* 带时标的GOOSE掩码位 */
}EACH_GOOSE_RECV;

/* 通道信息(dev) */
typedef struct PLAT_DEV_CHL_INFO_struct
{
    INT iInitFlag;   /* 初始化标识 */
    INT iMaxCount;   /* 最大通道个数 */
    INT iCount;      /* 通道个数 */
    INT iMallocErr;  /* 申请空间失败 */
    EACH_CHL_INFO    * pEachChlInfo;  /* 数据缓冲区 */
}PLAT_DEV_CHL_INFO;

/* goose接收sv数据信息 */
typedef struct PLAT_DEV_GSV_INFO_struct
{
    INT iInitFlag;   /* 初始化标识 */
    INT iMaxCount;   /* 最大GSV个数 */
    INT iCount;      /* GSV个数 */
    INT iMallocErr;  /* 申请空间失败 */
    EACH_GSV_INFO    * pEachGSVInfo;  /* 数据缓冲区 */
}PLAT_DEV_GSV_INFO;
/*------------------------------------------*/
/* 接收GOOSE信息 */
typedef struct PLAT_GOOSE_RC_INFO_struct
{
    INT iInitFlag;        /* 初始化标识 */
    INT iMaxCount;        /* 接收goose最大组数 */
    INT iCount;           /* 接收goose组数 */
    INT iMallocErr;       /* 申请空间失败 */
    EACH_GOOSE_RECV       * pEachGooseRecv;
}PLAT_GOOSE_RC_INFO;
/*------------------------------------------*/
PLAT_GOOSE_RC_INFO   PlatGooseRcInfo; /* XML中接收GOOSE信息 */
PLAT_DEV_CHL_INFO    PlatDevChlInfo;  /* XML中SV接收信息 */
PLAT_DEV_GSV_INFO    PlatDevGsvInfo;  /* XML中GSV接收信息(goose接收非电量数据信息) */
/* 配置文件总数据信息结构定义 */
typedef struct PLAT_XML_INFO_struct
{
    INT iInitFlag;        /* 初始化标识 */
    INT iCpuCount;        /* Cpu个数 */
    INT iIintErr;         /* 初始化错误 */
    PLAT_GOOSE_RC_INFO   * pPlatGooseRcInfo; /* XML中接收GOOSE信息 */
    PLAT_DEV_CHL_INFO    * pPlatDevChlInfo;  /* XML中SV接收信息 */
    PLAT_DEV_GSV_INFO    * pPlatDevGsvInfo;  /* XML中GSV接收信息(goose接收非电量数据信息) */
}PLAT_XML_INFO;

PLAT_XML_INFO  Plat_XmlInfo =
{
    0,
    0,
    0,
    & PlatGooseRcInfo,
    & PlatDevChlInfo,
    & PlatDevGsvInfo,
};
/* 首地址存储CPU个数,其它存储有通道信息的CPUID */
INT  PlatCfgCpuInfo[PXML_CFG_MAX_CPU_NUMBER + 1];
/*------------------------------------------*/
static ST_INT _hex2dec ( ST_CHAR hex );
/* 字符串转换为屏蔽位 */
static ST_VOID _str2MaskBits ( ST_CHAR * str, UNSIGNED * MaskBits );
/* 初始化函数(主要是获取CPU个数) */
INT PXML_Initilize( VOID );
/*------------------------------------------*/
/* 根节点信息 */
SX_ELEMENT sCfgStartElements[] =
{
    {   "PLAT", SX_ELF_CSTART | SX_ELF_CEND, _sCfg_SEFun}
};

/* PLAT 节点 */
SX_ELEMENT sCfgElements[] =
{
    {   "SMV_RECV", SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_OPT, _smvrecv_SEFun},
    {   "GOOSE_RECV", SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_OPT, _goose_recv_SEFun},
};

/* SMV 节点 */
SX_ELEMENT sCfgSmvElements[] =
{
    {   "SMV", SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_RPT | SX_ELF_OPT, _smv_SERFun}
};

/* SMV 节点 */
SX_ELEMENT sCfgSmvItemElements[] =
{
    {   "SMV_CHL", SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_RPT | SX_ELF_OPT, _smv_chl_SERFun}
};

/*-------------------------------------------------------*/
/* GOOSE接收节点 */
SX_ELEMENT sCfgGooseRevcElements[] =
{
    {   "GOOSE", SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_RPT | SX_ELF_OPT, _goose_SERFun}
};

/* GOOSE接收节点 */
SX_ELEMENT sCfgGooseRCItemElements[] =
{
    {   "GOOSE_RC", SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_RPT | SX_ELF_OPT, _goose_rc_SERFun}
};
/*-------------------------------------------------------*/
/* goose接收SV节点 */
SX_ELEMENT GooseSVRecvElements[] =
{
    {   "GOOSE_SV", SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_RPT | SX_ELF_OPT, _GooseSV_SERFun}
};

/* goose接收SV节点 */
SX_ELEMENT GooseSVCfgItemElements[] =
{
    {   "GSV_CHL", SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_RPT | SX_ELF_OPT, _GooseSV_Chl_SERFun}
};
/*======================================================*/
/* 主函数 */
/*======================================================*/
/* 初始化平台配置文件 */
int PXML_CFG_InitConfig( CHAR * fileName )
{
    ST_CHAR * xml;
    ST_RET  iStatus = SX_USER_ERROR;

    /* 判断文件是否已经解析过,如果解析过不再解析 */
    if( Plat_XmlInfo.iInitFlag == PXML_INIT_FLAG )
    {
        return( iStatus );
    }

    xml = PXML_CFG_GetXML( fileName );
    if( xml )
    {
        /* 初始化函数(主要是获取CPU个数) */
        if( PXML_Initilize() == NU_TRUE )
        {
            iStatus = PXML_CFG_LoadConfigFromXML( xml, strlen( xml ));
        }
        else
        {
            /* 置初始化错误标识 */
            Plat_XmlInfo.iIintErr ++;
        }
    }
    return( iStatus );
}

/* 取得 XML 配置数据 */
ST_CHAR * PXML_CFG_GetXML( CHAR * fileName )
{
    static char * cfg_xml = NULL;

    if ( cfg_xml == NULL )
    {
        FILE * fp = fopen( fileName, "rb" );
        if ( fp )
        {
            size_t size = 0;
            fseek( fp, 0, SEEK_END );
            size = ftell( fp );
            if ( size > 0 )
            {
                cfg_xml = calloc( size + 1, 1 );
                fseek( fp, 0, SEEK_SET );
                fread( cfg_xml, size, 1, fp );
            }
            fclose( fp );
        }
    }

    return cfg_xml;
}

/* 从 xml 数据中读取配置参数 */
ST_RET PXML_CFG_LoadConfigFromXML( const ST_CHAR * xml, ST_INT len )
{
    ST_RET rc;
    rc = sx_parse_mt ( len, ( ST_CHAR *)xml, sizeof( sCfgStartElements )/ sizeof( SX_ELEMENT ),
        sCfgStartElements, NULL, NULL, NULL );
    return rc;
}

/*======================================================*/
/* 各节点解析 */
/*======================================================*/
/* 解析主节点 */
static ST_VOID _sCfg_SEFun ( SX_DEC_CTRL * sxDecCtrl )
{
    SXLOG_CFLOW0 ( "SX DEC: _sCfg_SEFun" );

    if ( sxDecCtrl->reason == SX_ELEMENT_START )
    {
        SX_PUSH ( sCfgElements );
    }
}

/* 解析 SMV 接收节点 */
static ST_VOID _smvrecv_SEFun ( SX_DEC_CTRL * sxDecCtrl )
{
    SXLOG_CFLOW0 ( "SX DEC: _smvrecv_SEFun" );

    if ( sxDecCtrl->reason == SX_ELEMENT_START )
    {
        SX_PUSH ( sCfgSmvElements );
    }
}

/* 解析 SMV 子项 */
static ST_VOID _smv_SERFun ( SX_DEC_CTRL * sxDecCtrl )
{
    /* 获取数据个数 */
    ST_UINT           iCount = 0;
    VOID * p;
    INT  iSize;
    PLAT_XML_INFO     * this;
    PLAT_DEV_CHL_INFO * pPlatDevChlInfo;  /* XML中SV接收信息 */

    this = & Plat_XmlInfo;
    pPlatDevChlInfo =  this->pPlatDevChlInfo;
    if ( sxDecCtrl->reason == SX_ELEMENT_START )
    {
        /* 判断初始化是否完成,如果已经完成不再进行缓冲区的初始化 */
        if( pPlatDevChlInfo->iInitFlag == PXML_INIT_FLAG )
        {
            return;
        }

        /* 获取数据个数 */
        if( sx_get_uint_attr( sxDecCtrl, PXML_CFG_DATA_COUNT, & iCount ) != SD_SUCCESS )
        {
            iCount = 0;
        }

        if( iCount > 0 )
        {
            /* 申请数据空间 */
            iSize = sizeof( EACH_CHL_INFO ) * iCount;
            p = malloc( iSize );
            /* 判断空间是否申请成功 */
            if( p )
            {
                /* 清缓冲区 */
                memset( p, 0, iSize );
                /* 设置最大数据个数 */
                pPlatDevChlInfo->iMaxCount = iCount;
                /* 置初始化完成标识 */
                pPlatDevChlInfo->iInitFlag = PXML_INIT_FLAG;
                /* 置缓冲地址 */
                pPlatDevChlInfo->pEachChlInfo = p;
            }
            else
            {
                /* 设置最大数据个数 */
                pPlatDevChlInfo->iMaxCount = 0;
                /* 置申请空间失败 */
                pPlatDevChlInfo->iMallocErr ++;
            }
        }
        else
        {
            /* 设置最大数据个数 */
            pPlatDevChlInfo->iMaxCount = 0;
            /* 置初始化完成标识 */
            pPlatDevChlInfo->iInitFlag = PXML_INIT_FLAG;
        }
        SX_PUSH( sCfgSmvItemElements );
    }
}

/* 解析 SMV 通道项 */
static ST_VOID _smv_chl_SERFun ( SX_DEC_CTRL * sxDecCtrl )
{
    ST_UINT cpuid, pro_chl_id, abs_chl_id;
    PLAT_XML_INFO     * this;
    PLAT_DEV_CHL_INFO * pPlatDevChlInfo;  /* XML中SV接收信息 */
    EACH_CHL_INFO     * pEachChlInfo;     /* 数据缓冲区 */

    this = & Plat_XmlInfo;
    pPlatDevChlInfo = this->pPlatDevChlInfo;
    if( this->iInitFlag != PXML_INIT_FLAG )
    {
        return;
    }

    if ( sxDecCtrl->reason == SX_ELEMENT_START )
    {
        if( sx_get_uint_attr( sxDecCtrl, PXML_CFG_CPU_ID_NODE_NAME, & cpuid ) != SD_SUCCESS )
        {
            return;
        }
        /* 判断cpuid,个数及初始化标识是否在允许范围之内 */
        if (( ( INT )cpuid <= this->iCpuCount ) && ( cpuid > 0 )
            && ( pPlatDevChlInfo->iCount < pPlatDevChlInfo->iMaxCount )
            &&( pPlatDevChlInfo->iInitFlag == PXML_INIT_FLAG ))
        {
            /* 获取通道信息 */
            if( sx_get_uint_attr( sxDecCtrl, PXML_CFG_PRO_CHL_ID_NODE_NAME, & pro_chl_id ) != SD_SUCCESS )
            {
                return;
            }
            if( sx_get_uint_attr( sxDecCtrl, PXML_CFG_ABS_CHL_ID_NODE_NAME, & abs_chl_id )!= SD_SUCCESS )
            {
                return;
            }
            /* 判断通道信息是否在允许范围之内 */
            if (( pro_chl_id < UNUSED_PRO_ID )
                && ( abs_chl_id < UNUSED_PRO_ID )
                && ( ( INT )pro_chl_id < CSMV_GET_PHY_ANALOG_NUMBER ))
            {
                /* 记录数据信息 */
                pEachChlInfo = pPlatDevChlInfo->pEachChlInfo + pPlatDevChlInfo->iCount;
                pEachChlInfo->iCpuId = cpuid;
                pEachChlInfo->iProChlID = pro_chl_id;   /* 保护CPU通道ID */
                pEachChlInfo->iAbsChlID = abs_chl_id;   /* 绝对通道ID */
                /* 调整通道数据个数 */
                pPlatDevChlInfo->iCount ++;
            }
        }
    }
}

/*======================================================*/
/* 解析 GOOSE 接收节点 */
static ST_VOID _goose_recv_SEFun ( SX_DEC_CTRL * sxDecCtrl )
{
    SXLOG_CFLOW0 ( "SX DEC: _goose_recv_SEFun" );

    if ( sxDecCtrl->reason == SX_ELEMENT_START )
    {
        SX_PUSH ( sCfgGooseRevcElements );
    }
}

/* 解析 GOOSE接收 子项 */
static ST_VOID _goose_SERFun ( SX_DEC_CTRL * sxDecCtrl )
{
    /* 获取数据个数 */
    ST_UINT           iCount = 0;
    VOID * p;
    INT  iSize;
    PLAT_XML_INFO     * this;
    PLAT_GOOSE_RC_INFO   * pPlatGooseRcInfo; /* XML中接收GOOSE信息 */

    this = & Plat_XmlInfo;
    pPlatGooseRcInfo = this->pPlatGooseRcInfo;

    if ( sxDecCtrl->reason == SX_ELEMENT_START )
    {
        /* 判断初始化是否完成,如果已经完成不再进行缓冲区的初始化 */
        if( pPlatGooseRcInfo->iInitFlag == PXML_INIT_FLAG )
        {
            return;
        }
        /* 获取数据个数 */
        if( sx_get_uint_attr( sxDecCtrl, PXML_CFG_DATA_COUNT, & iCount ) != SD_SUCCESS )
        {
            iCount = 0;
        }

        /* 判断个数是否在允许范围之内 */
        if( iCount > CPC_MAX_INPUT_GROUPS )
        {
            iCount = CPC_MAX_INPUT_GROUPS;
        }

        if( iCount > 0 )
        {
            /* 申请数据空间 */
            iSize = sizeof( EACH_CHL_INFO ) * iCount;
            p = malloc( iSize );
            /* 判断空间是否申请成功 */
            if( p )
            {
                /* 清缓冲区 */
                memset( p, 0, iSize );
                /* 设置最大数据个数 */
                pPlatGooseRcInfo->iMaxCount = iCount;
                /* 置初始化完成标识 */
                pPlatGooseRcInfo->iInitFlag = PXML_INIT_FLAG;
                /* 置缓冲地址 */
                pPlatGooseRcInfo->pEachGooseRecv = p;
            }
            else
            {
                /* 设置最大数据个数 */
                pPlatGooseRcInfo->iMaxCount = 0;
                /* 置申请空间失败 */
                pPlatGooseRcInfo->iMallocErr ++;
            }
        }
        else
        {
            /* 设置最大数据个数 */
            pPlatGooseRcInfo->iMaxCount = 0;
            /* 置初始化完成标识 */
            pPlatGooseRcInfo->iInitFlag = PXML_INIT_FLAG;
        }
        SX_PUSH( sCfgGooseRCItemElements );
    }
}

/* 解析 GOOSE接收 掩码项 */
static ST_VOID _goose_rc_SERFun ( SX_DEC_CTRL * sxDecCtrl )
{
    ST_UINT                  iGroupID;
    PLAT_XML_INFO        * this;
    PLAT_GOOSE_RC_INFO   * pPlatGooseRcInfo; /* XML中接收GOOSE信息 */
    EACH_GOOSE_RECV      * pEachGooseRecv;
    char                 * buff = NULL;

    this = & Plat_XmlInfo;
    pPlatGooseRcInfo = this->pPlatGooseRcInfo;
    if( this->iInitFlag != PXML_INIT_FLAG )
    {
        return;
    }

    if ( sxDecCtrl->reason == SX_ELEMENT_START )
    {
        if( sx_get_uint_attr( sxDecCtrl, PXML_CFG_GR_GROUP_ID_NODE_NAME, & iGroupID )!= SD_SUCCESS )
        {
            return;
        }

        /* 判断组数及初始化标识是否在允许范围之内 */
        if(( pPlatGooseRcInfo->iCount < pPlatGooseRcInfo->iMaxCount )
            &&( pPlatGooseRcInfo->iInitFlag == PXML_INIT_FLAG ))
        {
            /* 调整存储位置 */
            pEachGooseRecv = pPlatGooseRcInfo->pEachGooseRecv + pPlatGooseRcInfo->iCount;
            /* 组号 */
            pEachGooseRecv->iGroupID = iGroupID;
            /* 屏蔽位 */
            if( sx_get_attr_ptr( sxDecCtrl, & buff, PXML_CFG_GR_MASK_BITS_NODE_NAME )!= SD_SUCCESS )
            {
                return;
            }

            /* 字符串转换为屏蔽位 */
            _str2MaskBits( buff, &( pEachGooseRecv->uMaskBits ));
            /* 带时标的屏蔽位 */
            if( sx_get_attr_ptr( sxDecCtrl, & buff, PXML_CFG_GR_T_MASK_BITS_NODE_NAME )!= SD_SUCCESS )
            {
                return;
            }

            /* 字符串转换为带时标的屏蔽位 */
            _str2MaskBits( buff, &( pEachGooseRecv->uMaskBitsT ));
            /* 组数调整 */
            pPlatGooseRcInfo->iCount ++;
        }
    }
}

static ST_INT _hex2dec ( ST_CHAR hex )
{
    if ( hex >= '0' && hex <= '9' )
        return hex - '0';
    else if ( hex >= 'a' && hex <= 'f' )
        return hex - 'a' + 10; /* 16进制转10进制 */
    else if ( hex >= 'A' && hex <= 'F' )
        return hex - 'A' + 10; /* 16进制转10进制 */
    else
        return 0;
}

/* 字符串转换为屏蔽位 */
static ST_VOID _str2MaskBits ( ST_CHAR * str, UNSIGNED * MaskBits )
{
    int i;
    int iLen;
    ST_CHAR * pTemp = str;
    UNSIGNED uData = 0;
    UNSIGNED uData1 = 1;  /* 数据转换因子 */

    iLen = strlen( str );
    if (( iLen <= 8 ) && ( iLen > 0 )) /* 屏蔽位长度 */
    {
        pTemp += ( iLen - 1 );          /* 调整到字符串的尾部进行解析 */
        for ( i = 0; i < iLen; i ++)
        {
            /* 屏蔽位数据 */
            uData += _hex2dec( * pTemp -- ) * uData1 ;
            /* 数据转换因子 */
            uData1 *= 16;
        }
        if( MaskBits )
        {
            * MaskBits = uData;
        }
    }
}

/*======================================================*/
/* goose接收SV各节点解析 */
/*======================================================*/
/* 解析 GooseSV 接收节点 */
static ST_VOID _GooseSVRecv_SEFun ( SX_DEC_CTRL * sxDecCtrl )
{
    SXLOG_CFLOW0 ( "SX DEC: _GooseSVRecv_SEFun" );

    if ( sxDecCtrl->reason == SX_ELEMENT_START )
    {
        SX_PUSH ( GooseSVRecvElements );
    }
}

/* 解析 GooseSV 子项 */
static ST_VOID _GooseSV_SERFun ( SX_DEC_CTRL * sxDecCtrl )
{
    /* 获取数据个数 */
    ST_UINT           iCount = 0;
    VOID * p;
    INT  iSize;
    PLAT_XML_INFO     * this;
    PLAT_DEV_GSV_INFO * pPlatDevGsvInfo;  /* XML中SV接收信息 */

    this = & Plat_XmlInfo;
    pPlatDevGsvInfo =  this->pPlatDevGsvInfo;
    if ( sxDecCtrl->reason == SX_ELEMENT_START )
    {
        /* 判断初始化是否完成,如果已经完成不再进行缓冲区的初始化 */
        if( pPlatDevGsvInfo->iInitFlag == PXML_INIT_FLAG )
        {
            return;
        }

        /* 获取数据个数 */
        if( sx_get_uint_attr( sxDecCtrl, PXML_CFG_DATA_COUNT, & iCount ) != SD_SUCCESS )
        {
            iCount = 0;
        }

        if( iCount > 0 )
        {
            /* 申请数据空间 */
            iSize = sizeof( EACH_GSV_INFO ) * iCount;
            p = malloc( iSize );
            /* 判断空间是否申请成功 */
            if( p )
            {
                /* 清缓冲区 */
                memset( p, 0, iSize );
                /* 设置最大数据个数 */
                pPlatDevGsvInfo->iMaxCount = iCount;
                /* 置初始化完成标识 */
                pPlatDevGsvInfo->iInitFlag = PXML_INIT_FLAG;
                /* 置缓冲地址 */
                pPlatDevGsvInfo->pEachGSVInfo = p;
            }
            else
            {
                /* 设置最大数据个数 */
                pPlatDevGsvInfo->iMaxCount = 0;
                /* 置申请空间失败 */
                pPlatDevGsvInfo->iMallocErr ++;
            }
        }
        else
        {
            /* 设置最大数据个数 */
            pPlatDevGsvInfo->iMaxCount = 0;
            /* 置初始化完成标识 */
            pPlatDevGsvInfo->iInitFlag = PXML_INIT_FLAG;
        }
        SX_PUSH( GooseSVCfgItemElements );
    }
}

/* 解析 GooseSV 通道项 */
static ST_VOID _GooseSV_Chl_SERFun ( SX_DEC_CTRL * sxDecCtrl )
{
    ST_UINT cpuid, pro_gsv_id, abs_gsv_id;
    PLAT_XML_INFO     * this;
    PLAT_DEV_GSV_INFO * pPlatDevGsvInfo;  /* XML中SV接收信息 */
    EACH_GSV_INFO     * pEachGSVInfo;     /* 数据缓冲区 */

    this = & Plat_XmlInfo;
    pPlatDevGsvInfo = this->pPlatDevGsvInfo;
    if( this->iInitFlag != PXML_INIT_FLAG )
    {
        return;
    }

    if ( sxDecCtrl->reason == SX_ELEMENT_START )
    {
        if( sx_get_uint_attr( sxDecCtrl, PXML_CFG_CPU_ID_NODE_NAME, & cpuid )!= SD_SUCCESS )
        {
            return;
        }

        /* 判断cpuid,个数及初始化标识是否在允许范围之内 */
        if (( ( INT )cpuid <= this->iCpuCount ) && ( cpuid > 0 )
            && ( pPlatDevGsvInfo->iCount < pPlatDevGsvInfo->iMaxCount )
            &&( pPlatDevGsvInfo->iInitFlag == PXML_INIT_FLAG ))
        {
            /* 获取通道信息 */
            if( sx_get_uint_attr( sxDecCtrl, PXML_CFG_PRO_GSV_ID_NODE_NAME, & pro_gsv_id )!= SD_SUCCESS )
            {
                return;
            }

            if( sx_get_uint_attr( sxDecCtrl, PXML_CFG_ABS_GSV_ID_NODE_NAME, & abs_gsv_id )!= SD_SUCCESS )
            {
                return;
            }

            /* 记录数据信息 */
            pEachGSVInfo = pPlatDevGsvInfo->pEachGSVInfo + pPlatDevGsvInfo->iCount;
            pEachGSVInfo->iCpuId = cpuid;
            pEachGSVInfo->iProGSVID = pro_gsv_id;   /* 保护CPU通道ID */
            pEachGSVInfo->iAbsGSVID = abs_gsv_id;   /* 绝对通道ID */
            /* 调整通道数据个数 */
            pPlatDevGsvInfo->iCount ++;
        }
    }
}

/*======================================================*/
/* 对外接口函数 */
/* 获取通道信息,返回通道个数 */
INT PXML_CFG_GetChlInfo( INT iCpuID, INT * pProChlId, INT * pAbsChlId, INT iMaxCount )
{
    INT               iCount = 0;
    INT               i;
    PLAT_XML_INFO     * this;
    PLAT_DEV_CHL_INFO * pPlatDevChlInfo;  /* XML中SV接收信息 */
    EACH_CHL_INFO     * pEachChlInfo;     /* 数据缓冲区 */

    this = & Plat_XmlInfo;
    pPlatDevChlInfo = this->pPlatDevChlInfo;

    if( ( this->iInitFlag != PXML_INIT_FLAG )
        || ( pPlatDevChlInfo->iInitFlag != PXML_INIT_FLAG ))
    {
        return( iCount );
    }

    this = & Plat_XmlInfo;
    pPlatDevChlInfo = this->pPlatDevChlInfo;
    pEachChlInfo = pPlatDevChlInfo->pEachChlInfo;

    for( i = 0; i < pPlatDevChlInfo->iCount; i ++)
    {
        if( iCount >= iMaxCount )
        {
            break;
        }
        if( pEachChlInfo->iCpuId == iCpuID )
        {
            * pProChlId ++ =  pEachChlInfo->iProChlID;
            * pAbsChlId ++ =  pEachChlInfo->iAbsChlID;
            iCount ++;
        }
        pEachChlInfo ++;
    }
    return( iCount );
}

/* 获取配置数据的CPU信息;返回值为CPU个数 */
INT PXML_CFG_GetCpuInfo( INT * pCpuId, INT iMaxCount )
{
    INT  iCount = 0;
    INT  i;
    INT  iOffset;
    static INT iInitFlag = NU_FALSE;
    PLAT_XML_INFO     * this;
    PLAT_DEV_CHL_INFO * pPlatDevChlInfo;  /* XML中SV接收信息 */
    EACH_CHL_INFO     * pEachChlInfo;     /* 数据缓冲区 */
    INT               iChlCount[PXML_CFG_MAX_CPU_NUMBER + 1];

    this = & Plat_XmlInfo;
    pPlatDevChlInfo = this->pPlatDevChlInfo;
    /* 仅初始化一次 */
    if(( iInitFlag == NU_FALSE )
        && ( pPlatDevChlInfo->iInitFlag == PXML_INIT_FLAG ))
    {
        memset( PlatCfgCpuInfo, 0, sizeof( PlatCfgCpuInfo ));
        memset( iChlCount, 0, sizeof( iChlCount ));
        pEachChlInfo = pPlatDevChlInfo->pEachChlInfo;
        /* 判断CPUid是否在允许范围之内 */
        for( i = 0; i < pPlatDevChlInfo->iCount; i ++)
        {
            if( pEachChlInfo->iCpuId <= PXML_CFG_MAX_CPU_NUMBER )
            {
                iChlCount[pEachChlInfo->iCpuId] ++;
            }
        }

        for( i = 1; i <= PXML_CFG_MAX_CPU_NUMBER; i ++)
        {
            /* 判断哪个CPU有通道号信息 */
            if( iChlCount[i] )
            {
                iOffset = PlatCfgCpuInfo[PXML_CFG_CPU_COUNT_OFFSET];
                PlatCfgCpuInfo[iOffset + PXML_CFG_CPU_DATA_OFFSET] = i;
                /* 调整数据个数 */
                PlatCfgCpuInfo[PXML_CFG_CPU_COUNT_OFFSET] ++;
            }
        }
        iInitFlag = NU_TRUE;
    }

    iCount = PlatCfgCpuInfo[PXML_CFG_CPU_COUNT_OFFSET];
    if( iCount > iMaxCount )
    {
        iCount = iMaxCount;
    }
    memcpy( pCpuId,& PlatCfgCpuInfo[PXML_CFG_CPU_DATA_OFFSET], iCount * sizeof( INT ));
    return( iCount );
}

/* 获取goose接收数据信息 */
INT PXML_CFG_GetGooseRecvInfo( INT GroupNo, UNSIGNED  * pMaskBits )
{
    INT     iCount = 0;
    INT     iStatus = NU_FALSE;
    INT     i;
    PLAT_XML_INFO        * this;
    PLAT_GOOSE_RC_INFO   * pPlatGooseRcInfo; /* XML中接收GOOSE信息 */
    EACH_GOOSE_RECV      * pEachGooseRecv;
    UNSIGNED uMaskBits = 0;

    this = & Plat_XmlInfo;
    pPlatGooseRcInfo = this->pPlatGooseRcInfo;

    /* 判断初始化是否成功 */
    if( pPlatGooseRcInfo->iInitFlag == PXML_INIT_FLAG )
    {
        /* 记录屏蔽位 */
        if( pMaskBits )
        {
            pEachGooseRecv = pPlatGooseRcInfo->pEachGooseRecv;
            for( i = 0; i < pPlatGooseRcInfo->iCount; i ++)
            {
                if( pEachGooseRecv->iGroupID == GroupNo )
                {
                    uMaskBits = pEachGooseRecv->uMaskBits;
                    break;
                }
                pEachGooseRecv ++;
            }

            * pMaskBits = uMaskBits;
        }

        iStatus = NU_TRUE;
    }

    return( iStatus );
}

/* 获取GOOSE开出掩码初始化状态 */
INT PXML_CFG_GetGRecvInitStatus( VOID )
{
    INT     iStatus = NU_FALSE;
    PLAT_XML_INFO        * this;
    PLAT_GOOSE_RC_INFO   * pPlatGooseRcInfo; /* XML中接收GOOSE信息 */

    this = & Plat_XmlInfo;
    pPlatGooseRcInfo = this->pPlatGooseRcInfo;
    /* 判断初始化是否成功 */
    if( pPlatGooseRcInfo->iInitFlag == PXML_INIT_FLAG )
    {
        iStatus = NU_TRUE;
    }

    return( iStatus );
}

/* 初始化函数(主要是获取CPU个数) */
INT PXML_Initilize( VOID )
{
    static INT iInitFlag = NU_FALSE;
    static INT ActCpuNumber = 1;

    if( iInitFlag == NU_FALSE )
    {
        /* 置初始化完成标识 */
        Plat_XmlInfo.iInitFlag = PXML_INIT_FLAG;
        /* 设置Cpu个数 */
        Plat_XmlInfo.iCpuCount = ActCpuNumber;
        /* 置初始化完成标识 */
        iInitFlag = NU_TRUE;
    }
    return( iInitFlag );
}

/* 获取带时标的goose接收数据信息 */
INT PXML_CFG_GetTimeGooseRecvInfo( INT GroupNo, UNSIGNED  * pMaskBitsT )
{
    INT     iCount = 0;
    INT     iStatus = NU_FALSE;
    INT     i;
    PLAT_XML_INFO        * this;
    PLAT_GOOSE_RC_INFO   * pPlatGooseRcInfo; /* XML中接收GOOSE信息 */
    EACH_GOOSE_RECV      * pEachGooseRecv;
    UNSIGNED uMaskBits = 0;

    this = & Plat_XmlInfo;
    pPlatGooseRcInfo = this->pPlatGooseRcInfo;

    /* 判断初始化是否成功 */
    if( pPlatGooseRcInfo->iInitFlag == PXML_INIT_FLAG )
    {
        /* 记录屏蔽位及组号 */
        if( pMaskBitsT )
        {
            pEachGooseRecv = pPlatGooseRcInfo->pEachGooseRecv;
            for( i = 0; i < pPlatGooseRcInfo->iCount; i ++)
            {
                if( pEachGooseRecv->iGroupID == GroupNo )
                {
                    uMaskBits = pEachGooseRecv->uMaskBitsT;
                    break;
                }
                pEachGooseRecv ++;
            }

            * pMaskBitsT = uMaskBits;
        }

        iStatus = NU_TRUE;
    }

    return( iStatus );
}
