/**************************************************************
** Copyright (C) 2002-2007 ��������������
** All right reserved
***************************************************************
**
** ��Ŀ���ƣ�NPI801
** ���ܼ�飺ȫ�����ô���ģ��
**
** ԭʼ���ߣ��ܾ���
** ��֯���ţ���̼��� ��������������
** ��    ע��
** ����ʱ�䣺2007-05-25
** ���ʱ�䣺2007-06-20
** ��    ����1.0
***************************************************************
** ��Ԫ��ʶ��$Id: pxml_cfg.c,v 1.7 2012/03/29 01:20:42 jyj Exp $
** �汾��ʷ��
** �� �� �ߣ�
** ����޸ģ�
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

#define PXML_CFG_MAX_CPU_NUMBER      CPC_MAX_CPU_NUMBER                           /* װ������ģ����� */
#define PXML_INIT_FLAG               0x5A                                         /* ��ʼ����ɱ�ʶ */

#define PXML_CFG_CPU_COUNT_OFFSET    0        /* CPU��Ϣ���ݸ���ƫ�� */
#define PXML_CFG_CPU_DATA_OFFSET     1        /* CPU��Ϣ����ƫ�� */

/* �������ڵ� */
static ST_VOID _sCfg_SEFun ( SX_DEC_CTRL * sxDecCtrl );
/* SMVͨ������ */
static ST_VOID _smvrecv_SEFun ( SX_DEC_CTRL * sxDecCtrl );
static ST_VOID _smv_SERFun ( SX_DEC_CTRL * sxDecCtrl );
static ST_VOID _smv_chl_SERFun ( SX_DEC_CTRL * sxDecCtrl );
/* GOOSE����ͨ������ */
static ST_VOID _goose_recv_SEFun ( SX_DEC_CTRL * sxDecCtrl );
static ST_VOID _goose_SERFun ( SX_DEC_CTRL * sxDecCtrl );
static ST_VOID _goose_rc_SERFun ( SX_DEC_CTRL * sxDecCtrl );
/* goose����SVͨ������ */
static ST_VOID _GooseSVRecv_SEFun ( SX_DEC_CTRL * sxDecCtrl );  /* ���� GooseSV ���սڵ� */
static ST_VOID _GooseSV_SERFun ( SX_DEC_CTRL * sxDecCtrl );     /* ���� GooseSV ���� */
static ST_VOID _GooseSV_Chl_SERFun ( SX_DEC_CTRL * sxDecCtrl ); /* ���� GooseSV ͨ���� */

/* �� xml �����ж�ȡ���ò��� */
ST_RET PXML_CFG_LoadConfigFromXML( const ST_CHAR * xml, ST_INT len );
/* ȡ�� XML �������� */
ST_CHAR * PXML_CFG_GetXML( CHAR * fileName );

/* ÿ��SVͨ����Ϣ */
typedef struct EACH_CHL_INFO_struct
{
    INT iCpuId;      /* CpuId */
    INT iProChlID;   /* ����CPUͨ��ID */
    INT iAbsChlID;   /* ����ͨ��ID */
}EACH_CHL_INFO;

/* ÿGSVͨ����Ϣ */
typedef struct EACH_GSV_INFO_struct
{
    INT iCpuId;      /* CpuId */
    INT iProGSVID;   /* ����GSV_ID */
    INT iAbsGSVID;   /* װ��GSV_ID */
}EACH_GSV_INFO;

/* ÿ��goose������Ϣ */
typedef struct EACH_GOOSE_RECV_struct
{
    INT      iGroupID;    /* ��� */
    UNSIGNED uMaskBits;   /* ����λ */
    UNSIGNED uMaskBitsT;  /* ��ʱ���GOOSE����λ */
}EACH_GOOSE_RECV;

/* ͨ����Ϣ(dev) */
typedef struct PLAT_DEV_CHL_INFO_struct
{
    INT iInitFlag;   /* ��ʼ����ʶ */
    INT iMaxCount;   /* ���ͨ������ */
    INT iCount;      /* ͨ������ */
    INT iMallocErr;  /* ����ռ�ʧ�� */
    EACH_CHL_INFO    * pEachChlInfo;  /* ���ݻ����� */
}PLAT_DEV_CHL_INFO;

/* goose����sv������Ϣ */
typedef struct PLAT_DEV_GSV_INFO_struct
{
    INT iInitFlag;   /* ��ʼ����ʶ */
    INT iMaxCount;   /* ���GSV���� */
    INT iCount;      /* GSV���� */
    INT iMallocErr;  /* ����ռ�ʧ�� */
    EACH_GSV_INFO    * pEachGSVInfo;  /* ���ݻ����� */
}PLAT_DEV_GSV_INFO;
/*------------------------------------------*/
/* ����GOOSE��Ϣ */
typedef struct PLAT_GOOSE_RC_INFO_struct
{
    INT iInitFlag;        /* ��ʼ����ʶ */
    INT iMaxCount;        /* ����goose������� */
    INT iCount;           /* ����goose���� */
    INT iMallocErr;       /* ����ռ�ʧ�� */
    EACH_GOOSE_RECV       * pEachGooseRecv;
}PLAT_GOOSE_RC_INFO;
/*------------------------------------------*/
PLAT_GOOSE_RC_INFO   PlatGooseRcInfo; /* XML�н���GOOSE��Ϣ */
PLAT_DEV_CHL_INFO    PlatDevChlInfo;  /* XML��SV������Ϣ */
PLAT_DEV_GSV_INFO    PlatDevGsvInfo;  /* XML��GSV������Ϣ(goose���շǵ���������Ϣ) */
/* �����ļ���������Ϣ�ṹ���� */
typedef struct PLAT_XML_INFO_struct
{
    INT iInitFlag;        /* ��ʼ����ʶ */
    INT iCpuCount;        /* Cpu���� */
    INT iIintErr;         /* ��ʼ������ */
    PLAT_GOOSE_RC_INFO   * pPlatGooseRcInfo; /* XML�н���GOOSE��Ϣ */
    PLAT_DEV_CHL_INFO    * pPlatDevChlInfo;  /* XML��SV������Ϣ */
    PLAT_DEV_GSV_INFO    * pPlatDevGsvInfo;  /* XML��GSV������Ϣ(goose���շǵ���������Ϣ) */
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
/* �׵�ַ�洢CPU����,�����洢��ͨ����Ϣ��CPUID */
INT  PlatCfgCpuInfo[PXML_CFG_MAX_CPU_NUMBER + 1];
/*------------------------------------------*/
static ST_INT _hex2dec ( ST_CHAR hex );
/* �ַ���ת��Ϊ����λ */
static ST_VOID _str2MaskBits ( ST_CHAR * str, UNSIGNED * MaskBits );
/* ��ʼ������(��Ҫ�ǻ�ȡCPU����) */
INT PXML_Initilize( VOID );
/*------------------------------------------*/
/* ���ڵ���Ϣ */
SX_ELEMENT sCfgStartElements[] =
{
    {   "PLAT", SX_ELF_CSTART | SX_ELF_CEND, _sCfg_SEFun}
};

/* PLAT �ڵ� */
SX_ELEMENT sCfgElements[] =
{
    {   "SMV_RECV", SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_OPT, _smvrecv_SEFun},
    {   "GOOSE_RECV", SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_OPT, _goose_recv_SEFun},
};

/* SMV �ڵ� */
SX_ELEMENT sCfgSmvElements[] =
{
    {   "SMV", SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_RPT | SX_ELF_OPT, _smv_SERFun}
};

/* SMV �ڵ� */
SX_ELEMENT sCfgSmvItemElements[] =
{
    {   "SMV_CHL", SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_RPT | SX_ELF_OPT, _smv_chl_SERFun}
};

/*-------------------------------------------------------*/
/* GOOSE���սڵ� */
SX_ELEMENT sCfgGooseRevcElements[] =
{
    {   "GOOSE", SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_RPT | SX_ELF_OPT, _goose_SERFun}
};

/* GOOSE���սڵ� */
SX_ELEMENT sCfgGooseRCItemElements[] =
{
    {   "GOOSE_RC", SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_RPT | SX_ELF_OPT, _goose_rc_SERFun}
};
/*-------------------------------------------------------*/
/* goose����SV�ڵ� */
SX_ELEMENT GooseSVRecvElements[] =
{
    {   "GOOSE_SV", SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_RPT | SX_ELF_OPT, _GooseSV_SERFun}
};

/* goose����SV�ڵ� */
SX_ELEMENT GooseSVCfgItemElements[] =
{
    {   "GSV_CHL", SX_ELF_CSTART | SX_ELF_CEND | SX_ELF_RPT | SX_ELF_OPT, _GooseSV_Chl_SERFun}
};
/*======================================================*/
/* ������ */
/*======================================================*/
/* ��ʼ��ƽ̨�����ļ� */
int PXML_CFG_InitConfig( CHAR * fileName )
{
    ST_CHAR * xml;
    ST_RET  iStatus = SX_USER_ERROR;

    /* �ж��ļ��Ƿ��Ѿ�������,������������ٽ��� */
    if( Plat_XmlInfo.iInitFlag == PXML_INIT_FLAG )
    {
        return( iStatus );
    }

    xml = PXML_CFG_GetXML( fileName );
    if( xml )
    {
        /* ��ʼ������(��Ҫ�ǻ�ȡCPU����) */
        if( PXML_Initilize() == NU_TRUE )
        {
            iStatus = PXML_CFG_LoadConfigFromXML( xml, strlen( xml ));
        }
        else
        {
            /* �ó�ʼ�������ʶ */
            Plat_XmlInfo.iIintErr ++;
        }
    }
    return( iStatus );
}

/* ȡ�� XML �������� */
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

/* �� xml �����ж�ȡ���ò��� */
ST_RET PXML_CFG_LoadConfigFromXML( const ST_CHAR * xml, ST_INT len )
{
    ST_RET rc;
    rc = sx_parse_mt ( len, ( ST_CHAR *)xml, sizeof( sCfgStartElements )/ sizeof( SX_ELEMENT ),
        sCfgStartElements, NULL, NULL, NULL );
    return rc;
}

/*======================================================*/
/* ���ڵ���� */
/*======================================================*/
/* �������ڵ� */
static ST_VOID _sCfg_SEFun ( SX_DEC_CTRL * sxDecCtrl )
{
    SXLOG_CFLOW0 ( "SX DEC: _sCfg_SEFun" );

    if ( sxDecCtrl->reason == SX_ELEMENT_START )
    {
        SX_PUSH ( sCfgElements );
    }
}

/* ���� SMV ���սڵ� */
static ST_VOID _smvrecv_SEFun ( SX_DEC_CTRL * sxDecCtrl )
{
    SXLOG_CFLOW0 ( "SX DEC: _smvrecv_SEFun" );

    if ( sxDecCtrl->reason == SX_ELEMENT_START )
    {
        SX_PUSH ( sCfgSmvElements );
    }
}

/* ���� SMV ���� */
static ST_VOID _smv_SERFun ( SX_DEC_CTRL * sxDecCtrl )
{
    /* ��ȡ���ݸ��� */
    ST_UINT           iCount = 0;
    VOID * p;
    INT  iSize;
    PLAT_XML_INFO     * this;
    PLAT_DEV_CHL_INFO * pPlatDevChlInfo;  /* XML��SV������Ϣ */

    this = & Plat_XmlInfo;
    pPlatDevChlInfo =  this->pPlatDevChlInfo;
    if ( sxDecCtrl->reason == SX_ELEMENT_START )
    {
        /* �жϳ�ʼ���Ƿ����,����Ѿ���ɲ��ٽ��л������ĳ�ʼ�� */
        if( pPlatDevChlInfo->iInitFlag == PXML_INIT_FLAG )
        {
            return;
        }

        /* ��ȡ���ݸ��� */
        if( sx_get_uint_attr( sxDecCtrl, PXML_CFG_DATA_COUNT, & iCount ) != SD_SUCCESS )
        {
            iCount = 0;
        }

        if( iCount > 0 )
        {
            /* �������ݿռ� */
            iSize = sizeof( EACH_CHL_INFO ) * iCount;
            p = malloc( iSize );
            /* �жϿռ��Ƿ�����ɹ� */
            if( p )
            {
                /* �建���� */
                memset( p, 0, iSize );
                /* ����������ݸ��� */
                pPlatDevChlInfo->iMaxCount = iCount;
                /* �ó�ʼ����ɱ�ʶ */
                pPlatDevChlInfo->iInitFlag = PXML_INIT_FLAG;
                /* �û����ַ */
                pPlatDevChlInfo->pEachChlInfo = p;
            }
            else
            {
                /* ����������ݸ��� */
                pPlatDevChlInfo->iMaxCount = 0;
                /* ������ռ�ʧ�� */
                pPlatDevChlInfo->iMallocErr ++;
            }
        }
        else
        {
            /* ����������ݸ��� */
            pPlatDevChlInfo->iMaxCount = 0;
            /* �ó�ʼ����ɱ�ʶ */
            pPlatDevChlInfo->iInitFlag = PXML_INIT_FLAG;
        }
        SX_PUSH( sCfgSmvItemElements );
    }
}

/* ���� SMV ͨ���� */
static ST_VOID _smv_chl_SERFun ( SX_DEC_CTRL * sxDecCtrl )
{
    ST_UINT cpuid, pro_chl_id, abs_chl_id;
    PLAT_XML_INFO     * this;
    PLAT_DEV_CHL_INFO * pPlatDevChlInfo;  /* XML��SV������Ϣ */
    EACH_CHL_INFO     * pEachChlInfo;     /* ���ݻ����� */

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
        /* �ж�cpuid,��������ʼ����ʶ�Ƿ�������Χ֮�� */
        if (( ( INT )cpuid <= this->iCpuCount ) && ( cpuid > 0 )
            && ( pPlatDevChlInfo->iCount < pPlatDevChlInfo->iMaxCount )
            &&( pPlatDevChlInfo->iInitFlag == PXML_INIT_FLAG ))
        {
            /* ��ȡͨ����Ϣ */
            if( sx_get_uint_attr( sxDecCtrl, PXML_CFG_PRO_CHL_ID_NODE_NAME, & pro_chl_id ) != SD_SUCCESS )
            {
                return;
            }
            if( sx_get_uint_attr( sxDecCtrl, PXML_CFG_ABS_CHL_ID_NODE_NAME, & abs_chl_id )!= SD_SUCCESS )
            {
                return;
            }
            /* �ж�ͨ����Ϣ�Ƿ�������Χ֮�� */
            if (( pro_chl_id < UNUSED_PRO_ID )
                && ( abs_chl_id < UNUSED_PRO_ID )
                && ( ( INT )pro_chl_id < CSMV_GET_PHY_ANALOG_NUMBER ))
            {
                /* ��¼������Ϣ */
                pEachChlInfo = pPlatDevChlInfo->pEachChlInfo + pPlatDevChlInfo->iCount;
                pEachChlInfo->iCpuId = cpuid;
                pEachChlInfo->iProChlID = pro_chl_id;   /* ����CPUͨ��ID */
                pEachChlInfo->iAbsChlID = abs_chl_id;   /* ����ͨ��ID */
                /* ����ͨ�����ݸ��� */
                pPlatDevChlInfo->iCount ++;
            }
        }
    }
}

/*======================================================*/
/* ���� GOOSE ���սڵ� */
static ST_VOID _goose_recv_SEFun ( SX_DEC_CTRL * sxDecCtrl )
{
    SXLOG_CFLOW0 ( "SX DEC: _goose_recv_SEFun" );

    if ( sxDecCtrl->reason == SX_ELEMENT_START )
    {
        SX_PUSH ( sCfgGooseRevcElements );
    }
}

/* ���� GOOSE���� ���� */
static ST_VOID _goose_SERFun ( SX_DEC_CTRL * sxDecCtrl )
{
    /* ��ȡ���ݸ��� */
    ST_UINT           iCount = 0;
    VOID * p;
    INT  iSize;
    PLAT_XML_INFO     * this;
    PLAT_GOOSE_RC_INFO   * pPlatGooseRcInfo; /* XML�н���GOOSE��Ϣ */

    this = & Plat_XmlInfo;
    pPlatGooseRcInfo = this->pPlatGooseRcInfo;

    if ( sxDecCtrl->reason == SX_ELEMENT_START )
    {
        /* �жϳ�ʼ���Ƿ����,����Ѿ���ɲ��ٽ��л������ĳ�ʼ�� */
        if( pPlatGooseRcInfo->iInitFlag == PXML_INIT_FLAG )
        {
            return;
        }
        /* ��ȡ���ݸ��� */
        if( sx_get_uint_attr( sxDecCtrl, PXML_CFG_DATA_COUNT, & iCount ) != SD_SUCCESS )
        {
            iCount = 0;
        }

        /* �жϸ����Ƿ�������Χ֮�� */
        if( iCount > CPC_MAX_INPUT_GROUPS )
        {
            iCount = CPC_MAX_INPUT_GROUPS;
        }

        if( iCount > 0 )
        {
            /* �������ݿռ� */
            iSize = sizeof( EACH_CHL_INFO ) * iCount;
            p = malloc( iSize );
            /* �жϿռ��Ƿ�����ɹ� */
            if( p )
            {
                /* �建���� */
                memset( p, 0, iSize );
                /* ����������ݸ��� */
                pPlatGooseRcInfo->iMaxCount = iCount;
                /* �ó�ʼ����ɱ�ʶ */
                pPlatGooseRcInfo->iInitFlag = PXML_INIT_FLAG;
                /* �û����ַ */
                pPlatGooseRcInfo->pEachGooseRecv = p;
            }
            else
            {
                /* ����������ݸ��� */
                pPlatGooseRcInfo->iMaxCount = 0;
                /* ������ռ�ʧ�� */
                pPlatGooseRcInfo->iMallocErr ++;
            }
        }
        else
        {
            /* ����������ݸ��� */
            pPlatGooseRcInfo->iMaxCount = 0;
            /* �ó�ʼ����ɱ�ʶ */
            pPlatGooseRcInfo->iInitFlag = PXML_INIT_FLAG;
        }
        SX_PUSH( sCfgGooseRCItemElements );
    }
}

/* ���� GOOSE���� ������ */
static ST_VOID _goose_rc_SERFun ( SX_DEC_CTRL * sxDecCtrl )
{
    ST_UINT                  iGroupID;
    PLAT_XML_INFO        * this;
    PLAT_GOOSE_RC_INFO   * pPlatGooseRcInfo; /* XML�н���GOOSE��Ϣ */
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

        /* �ж���������ʼ����ʶ�Ƿ�������Χ֮�� */
        if(( pPlatGooseRcInfo->iCount < pPlatGooseRcInfo->iMaxCount )
            &&( pPlatGooseRcInfo->iInitFlag == PXML_INIT_FLAG ))
        {
            /* �����洢λ�� */
            pEachGooseRecv = pPlatGooseRcInfo->pEachGooseRecv + pPlatGooseRcInfo->iCount;
            /* ��� */
            pEachGooseRecv->iGroupID = iGroupID;
            /* ����λ */
            if( sx_get_attr_ptr( sxDecCtrl, & buff, PXML_CFG_GR_MASK_BITS_NODE_NAME )!= SD_SUCCESS )
            {
                return;
            }

            /* �ַ���ת��Ϊ����λ */
            _str2MaskBits( buff, &( pEachGooseRecv->uMaskBits ));
            /* ��ʱ�������λ */
            if( sx_get_attr_ptr( sxDecCtrl, & buff, PXML_CFG_GR_T_MASK_BITS_NODE_NAME )!= SD_SUCCESS )
            {
                return;
            }

            /* �ַ���ת��Ϊ��ʱ�������λ */
            _str2MaskBits( buff, &( pEachGooseRecv->uMaskBitsT ));
            /* �������� */
            pPlatGooseRcInfo->iCount ++;
        }
    }
}

static ST_INT _hex2dec ( ST_CHAR hex )
{
    if ( hex >= '0' && hex <= '9' )
        return hex - '0';
    else if ( hex >= 'a' && hex <= 'f' )
        return hex - 'a' + 10; /* 16����ת10���� */
    else if ( hex >= 'A' && hex <= 'F' )
        return hex - 'A' + 10; /* 16����ת10���� */
    else
        return 0;
}

/* �ַ���ת��Ϊ����λ */
static ST_VOID _str2MaskBits ( ST_CHAR * str, UNSIGNED * MaskBits )
{
    int i;
    int iLen;
    ST_CHAR * pTemp = str;
    UNSIGNED uData = 0;
    UNSIGNED uData1 = 1;  /* ����ת������ */

    iLen = strlen( str );
    if (( iLen <= 8 ) && ( iLen > 0 )) /* ����λ���� */
    {
        pTemp += ( iLen - 1 );          /* �������ַ�����β�����н��� */
        for ( i = 0; i < iLen; i ++)
        {
            /* ����λ���� */
            uData += _hex2dec( * pTemp -- ) * uData1 ;
            /* ����ת������ */
            uData1 *= 16;
        }
        if( MaskBits )
        {
            * MaskBits = uData;
        }
    }
}

/*======================================================*/
/* goose����SV���ڵ���� */
/*======================================================*/
/* ���� GooseSV ���սڵ� */
static ST_VOID _GooseSVRecv_SEFun ( SX_DEC_CTRL * sxDecCtrl )
{
    SXLOG_CFLOW0 ( "SX DEC: _GooseSVRecv_SEFun" );

    if ( sxDecCtrl->reason == SX_ELEMENT_START )
    {
        SX_PUSH ( GooseSVRecvElements );
    }
}

/* ���� GooseSV ���� */
static ST_VOID _GooseSV_SERFun ( SX_DEC_CTRL * sxDecCtrl )
{
    /* ��ȡ���ݸ��� */
    ST_UINT           iCount = 0;
    VOID * p;
    INT  iSize;
    PLAT_XML_INFO     * this;
    PLAT_DEV_GSV_INFO * pPlatDevGsvInfo;  /* XML��SV������Ϣ */

    this = & Plat_XmlInfo;
    pPlatDevGsvInfo =  this->pPlatDevGsvInfo;
    if ( sxDecCtrl->reason == SX_ELEMENT_START )
    {
        /* �жϳ�ʼ���Ƿ����,����Ѿ���ɲ��ٽ��л������ĳ�ʼ�� */
        if( pPlatDevGsvInfo->iInitFlag == PXML_INIT_FLAG )
        {
            return;
        }

        /* ��ȡ���ݸ��� */
        if( sx_get_uint_attr( sxDecCtrl, PXML_CFG_DATA_COUNT, & iCount ) != SD_SUCCESS )
        {
            iCount = 0;
        }

        if( iCount > 0 )
        {
            /* �������ݿռ� */
            iSize = sizeof( EACH_GSV_INFO ) * iCount;
            p = malloc( iSize );
            /* �жϿռ��Ƿ�����ɹ� */
            if( p )
            {
                /* �建���� */
                memset( p, 0, iSize );
                /* ����������ݸ��� */
                pPlatDevGsvInfo->iMaxCount = iCount;
                /* �ó�ʼ����ɱ�ʶ */
                pPlatDevGsvInfo->iInitFlag = PXML_INIT_FLAG;
                /* �û����ַ */
                pPlatDevGsvInfo->pEachGSVInfo = p;
            }
            else
            {
                /* ����������ݸ��� */
                pPlatDevGsvInfo->iMaxCount = 0;
                /* ������ռ�ʧ�� */
                pPlatDevGsvInfo->iMallocErr ++;
            }
        }
        else
        {
            /* ����������ݸ��� */
            pPlatDevGsvInfo->iMaxCount = 0;
            /* �ó�ʼ����ɱ�ʶ */
            pPlatDevGsvInfo->iInitFlag = PXML_INIT_FLAG;
        }
        SX_PUSH( GooseSVCfgItemElements );
    }
}

/* ���� GooseSV ͨ���� */
static ST_VOID _GooseSV_Chl_SERFun ( SX_DEC_CTRL * sxDecCtrl )
{
    ST_UINT cpuid, pro_gsv_id, abs_gsv_id;
    PLAT_XML_INFO     * this;
    PLAT_DEV_GSV_INFO * pPlatDevGsvInfo;  /* XML��SV������Ϣ */
    EACH_GSV_INFO     * pEachGSVInfo;     /* ���ݻ����� */

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

        /* �ж�cpuid,��������ʼ����ʶ�Ƿ�������Χ֮�� */
        if (( ( INT )cpuid <= this->iCpuCount ) && ( cpuid > 0 )
            && ( pPlatDevGsvInfo->iCount < pPlatDevGsvInfo->iMaxCount )
            &&( pPlatDevGsvInfo->iInitFlag == PXML_INIT_FLAG ))
        {
            /* ��ȡͨ����Ϣ */
            if( sx_get_uint_attr( sxDecCtrl, PXML_CFG_PRO_GSV_ID_NODE_NAME, & pro_gsv_id )!= SD_SUCCESS )
            {
                return;
            }

            if( sx_get_uint_attr( sxDecCtrl, PXML_CFG_ABS_GSV_ID_NODE_NAME, & abs_gsv_id )!= SD_SUCCESS )
            {
                return;
            }

            /* ��¼������Ϣ */
            pEachGSVInfo = pPlatDevGsvInfo->pEachGSVInfo + pPlatDevGsvInfo->iCount;
            pEachGSVInfo->iCpuId = cpuid;
            pEachGSVInfo->iProGSVID = pro_gsv_id;   /* ����CPUͨ��ID */
            pEachGSVInfo->iAbsGSVID = abs_gsv_id;   /* ����ͨ��ID */
            /* ����ͨ�����ݸ��� */
            pPlatDevGsvInfo->iCount ++;
        }
    }
}

/*======================================================*/
/* ����ӿں��� */
/* ��ȡͨ����Ϣ,����ͨ������ */
INT PXML_CFG_GetChlInfo( INT iCpuID, INT * pProChlId, INT * pAbsChlId, INT iMaxCount )
{
    INT               iCount = 0;
    INT               i;
    PLAT_XML_INFO     * this;
    PLAT_DEV_CHL_INFO * pPlatDevChlInfo;  /* XML��SV������Ϣ */
    EACH_CHL_INFO     * pEachChlInfo;     /* ���ݻ����� */

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

/* ��ȡ�������ݵ�CPU��Ϣ;����ֵΪCPU���� */
INT PXML_CFG_GetCpuInfo( INT * pCpuId, INT iMaxCount )
{
    INT  iCount = 0;
    INT  i;
    INT  iOffset;
    static INT iInitFlag = NU_FALSE;
    PLAT_XML_INFO     * this;
    PLAT_DEV_CHL_INFO * pPlatDevChlInfo;  /* XML��SV������Ϣ */
    EACH_CHL_INFO     * pEachChlInfo;     /* ���ݻ����� */
    INT               iChlCount[PXML_CFG_MAX_CPU_NUMBER + 1];

    this = & Plat_XmlInfo;
    pPlatDevChlInfo = this->pPlatDevChlInfo;
    /* ����ʼ��һ�� */
    if(( iInitFlag == NU_FALSE )
        && ( pPlatDevChlInfo->iInitFlag == PXML_INIT_FLAG ))
    {
        memset( PlatCfgCpuInfo, 0, sizeof( PlatCfgCpuInfo ));
        memset( iChlCount, 0, sizeof( iChlCount ));
        pEachChlInfo = pPlatDevChlInfo->pEachChlInfo;
        /* �ж�CPUid�Ƿ�������Χ֮�� */
        for( i = 0; i < pPlatDevChlInfo->iCount; i ++)
        {
            if( pEachChlInfo->iCpuId <= PXML_CFG_MAX_CPU_NUMBER )
            {
                iChlCount[pEachChlInfo->iCpuId] ++;
            }
        }

        for( i = 1; i <= PXML_CFG_MAX_CPU_NUMBER; i ++)
        {
            /* �ж��ĸ�CPU��ͨ������Ϣ */
            if( iChlCount[i] )
            {
                iOffset = PlatCfgCpuInfo[PXML_CFG_CPU_COUNT_OFFSET];
                PlatCfgCpuInfo[iOffset + PXML_CFG_CPU_DATA_OFFSET] = i;
                /* �������ݸ��� */
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

/* ��ȡgoose����������Ϣ */
INT PXML_CFG_GetGooseRecvInfo( INT GroupNo, UNSIGNED  * pMaskBits )
{
    INT     iCount = 0;
    INT     iStatus = NU_FALSE;
    INT     i;
    PLAT_XML_INFO        * this;
    PLAT_GOOSE_RC_INFO   * pPlatGooseRcInfo; /* XML�н���GOOSE��Ϣ */
    EACH_GOOSE_RECV      * pEachGooseRecv;
    UNSIGNED uMaskBits = 0;

    this = & Plat_XmlInfo;
    pPlatGooseRcInfo = this->pPlatGooseRcInfo;

    /* �жϳ�ʼ���Ƿ�ɹ� */
    if( pPlatGooseRcInfo->iInitFlag == PXML_INIT_FLAG )
    {
        /* ��¼����λ */
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

/* ��ȡGOOSE���������ʼ��״̬ */
INT PXML_CFG_GetGRecvInitStatus( VOID )
{
    INT     iStatus = NU_FALSE;
    PLAT_XML_INFO        * this;
    PLAT_GOOSE_RC_INFO   * pPlatGooseRcInfo; /* XML�н���GOOSE��Ϣ */

    this = & Plat_XmlInfo;
    pPlatGooseRcInfo = this->pPlatGooseRcInfo;
    /* �жϳ�ʼ���Ƿ�ɹ� */
    if( pPlatGooseRcInfo->iInitFlag == PXML_INIT_FLAG )
    {
        iStatus = NU_TRUE;
    }

    return( iStatus );
}

/* ��ʼ������(��Ҫ�ǻ�ȡCPU����) */
INT PXML_Initilize( VOID )
{
    static INT iInitFlag = NU_FALSE;
    static INT ActCpuNumber = 1;

    if( iInitFlag == NU_FALSE )
    {
        /* �ó�ʼ����ɱ�ʶ */
        Plat_XmlInfo.iInitFlag = PXML_INIT_FLAG;
        /* ����Cpu���� */
        Plat_XmlInfo.iCpuCount = ActCpuNumber;
        /* �ó�ʼ����ɱ�ʶ */
        iInitFlag = NU_TRUE;
    }
    return( iInitFlag );
}

/* ��ȡ��ʱ���goose����������Ϣ */
INT PXML_CFG_GetTimeGooseRecvInfo( INT GroupNo, UNSIGNED  * pMaskBitsT )
{
    INT     iCount = 0;
    INT     iStatus = NU_FALSE;
    INT     i;
    PLAT_XML_INFO        * this;
    PLAT_GOOSE_RC_INFO   * pPlatGooseRcInfo; /* XML�н���GOOSE��Ϣ */
    EACH_GOOSE_RECV      * pEachGooseRecv;
    UNSIGNED uMaskBits = 0;

    this = & Plat_XmlInfo;
    pPlatGooseRcInfo = this->pPlatGooseRcInfo;

    /* �жϳ�ʼ���Ƿ�ɹ� */
    if( pPlatGooseRcInfo->iInitFlag == PXML_INIT_FLAG )
    {
        /* ��¼����λ����� */
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
