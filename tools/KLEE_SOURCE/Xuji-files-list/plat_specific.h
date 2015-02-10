
#ifndef PLAT_SPECIFIC_H
#define PLAT_SPECIFIC_H

#define ON                              1           /* Ͷ�� */
#define OFF                             0           /* �˳� */

/* ���Ӳ��ѡ�� */
#ifdef WIN32
#define PLAT_SWITCH_WIN32               ON           /* windows���� */
#define PLAT_SWITCH_NPU808              OFF          /* NPU808Ӳ�� */
#define PLAT_SWITCH_NPI807              OFF          /* NPI807Ӳ�� */
#define PLAT_SWITCH_DTS568              OFF          /* DTS568Ӳ�� */
#define PLAT_SWITCH_NPU810              OFF          /* NPU810Ӳ�� */
#define PLAT_SWITCH_NPU824              OFF          /* NPU824Ӳ�� */
#define PLAT_SWITCH_NPU8103             OFF          /* NPU8103Ӳ�� */
#endif

#ifdef NPU808
#define PLAT_SWITCH_WIN32               OFF          /* windows���� */
#define PLAT_SWITCH_NPU808              ON           /* NPU808Ӳ�� */
#define PLAT_SWITCH_NPI807              OFF          /* NPI807Ӳ�� */
#define PLAT_SWITCH_DTS568              OFF          /* DTS568Ӳ�� */
#define PLAT_SWITCH_NPU810              OFF          /* NPU810Ӳ�� */
#define PLAT_SWITCH_NPU824              OFF          /* NPU824Ӳ�� */
#define PLAT_SWITCH_NPU8103             OFF          /* NPU8103Ӳ�� */
#endif

#ifdef NPI807
#define PLAT_SWITCH_WIN32               OFF          /* windows���� */
#define PLAT_SWITCH_NPU808              OFF          /* NPU808Ӳ�� */
#define PLAT_SWITCH_NPI807              ON           /* NPI807Ӳ�� */
#define PLAT_SWITCH_DTS568              OFF          /* DTS568Ӳ�� */
#define PLAT_SWITCH_NPU810              OFF          /* NPU810Ӳ�� */
#define PLAT_SWITCH_NPU824              OFF          /* NPU824Ӳ�� */
#define PLAT_SWITCH_NPU8103             OFF          /* NPU8103Ӳ�� */
#endif

#ifdef DTS568
#define PLAT_SWITCH_WIN32               OFF          /* windows���� */
#define PLAT_SWITCH_NPU808              OFF          /* NPU808Ӳ�� */
#define PLAT_SWITCH_NPI807              OFF          /* NPI807Ӳ�� */
#define PLAT_SWITCH_DTS568              ON           /* DTS568Ӳ�� */
#define PLAT_SWITCH_NPU810              OFF          /* NPU810Ӳ�� */
#define PLAT_SWITCH_NPU824              OFF          /* NPU824Ӳ�� */
#define PLAT_SWITCH_NPU8103             OFF          /* NPU8103Ӳ�� */
#endif

#ifdef NPU810
#define PLAT_SWITCH_WIN32               OFF          /* windows���� */
#define PLAT_SWITCH_NPU808              OFF          /* NPU808Ӳ�� */
#define PLAT_SWITCH_NPI807              OFF          /* NPI807Ӳ�� */
#define PLAT_SWITCH_DTS568              OFF          /* DTS568Ӳ�� */
#define PLAT_SWITCH_NPU810              ON           /* NPU810Ӳ�� */
#define PLAT_SWITCH_NPU824              OFF          /* NPU824Ӳ�� */
#define PLAT_SWITCH_NPU8103             OFF          /* NPU8103Ӳ�� */
#endif

#ifdef NPU824
#define PLAT_SWITCH_WIN32               OFF          /* windows���� */
#define PLAT_SWITCH_NPU808              OFF          /* NPU808Ӳ�� */
#define PLAT_SWITCH_NPI807              OFF          /* NPI807Ӳ�� */
#define PLAT_SWITCH_DTS568              OFF          /* DTS568Ӳ�� */
#define PLAT_SWITCH_NPU810              OFF          /* NPU810Ӳ�� */
#define PLAT_SWITCH_NPU824              ON           /* NPU824Ӳ�� */
#define PLAT_SWITCH_NPU8103             OFF          /* NPU8103Ӳ�� */
#endif

#ifdef NPU8103
#define PLAT_SWITCH_WIN32               OFF          /* windows���� */
#define PLAT_SWITCH_NPU808              OFF          /* NPU808Ӳ�� */
#define PLAT_SWITCH_NPI807              OFF          /* NPI807Ӳ�� */
#define PLAT_SWITCH_DTS568              OFF          /* DTS568Ӳ�� */
#define PLAT_SWITCH_NPU810              OFF          /* NPU810Ӳ�� */
#define PLAT_SWITCH_NPU824              OFF          /* NPU824Ӳ�� */
#define PLAT_SWITCH_NPU8103             ON           /* NPU8103Ӳ�� */
#endif

#if PLAT_SWITCH_NPU808
#include "plat_specific/plat_npu808/plat_npu808_defs.h"
#include "plat_specific/plat_npu808/inc/gps808.h"
#include "plat_specific/inc/cpc_defs.h"
#include "plat_specific/inc/pmi_extr.h"
#include "plat_specific/inc/cem_extr.h"
#include "plat_specific/inc/crm_extr.h"
#include "plat_specific/inc/cwm_extr.h"
#endif

#if PLAT_SWITCH_NPI807
#include "plat_specific/plat_npi807/plat_npi807_defs.h"
#include "plat_specific/plat_npi807/inc/gps807.h"
#include "plat_specific/inc/cpc_defs.h"
#include "plat_specific/inc/pmi_extr.h"
#include "plat_specific/inc/cem_extr.h"
#include "plat_specific/inc/crm_extr.h"
#include "plat_specific/inc/cwm_extr.h"
#endif

#if PLAT_SWITCH_DTS568
#include "plat_specific/plat_dts568/plat_dts568_defs.h"
#include "plat_specific/plat_dts568/src/meter_pulse.h"
#include "plat_specific/plat_dts568/inc/gps568.h"
#include "plat_specific/inc/cpc_defs.h"
#include "plat_specific/inc/pmi_extr.h"
#include "plat_specific/inc/cem_extr.h"
#include "plat_specific/inc/crm_extr.h"
#include "plat_specific/inc/cwm_extr.h"
#endif

#if PLAT_SWITCH_NPU810
#include "plat_specific/plat_npu810/plat_npu810_defs.h"
#include "plat_specific/plat_npu810/inc/gps1.h"
#include "plat_specific/inc/cpc_defs.h"
#include "plat_specific/inc/pmi_extr.h"
#include "plat_specific/inc/cem_extr.h"
#include "plat_specific/inc/crm_extr.h"
#include "plat_specific/inc/cwm_extr.h"
#endif

#if PLAT_SWITCH_NPU824
#include "plat_specific/plat_npu824/plat_npu824_defs.h"
#include "plat_specific/plat_npu824/inc/gps824.h"
#include "plat_specific/inc/cpc_defs.h"
#include "plat_specific/inc/pmi_extr.h"
#include "plat_specific/inc/cem_extr.h"
#include "plat_specific/inc/crm_extr.h"
#include "plat_specific/inc/cwm_extr.h"
#endif

#if PLAT_SWITCH_NPU8103
#include "plat_specific/plat_npu8103/plat_npu8103_defs.h"
#include "plat_specific/plat_npu8103/inc/gps8103.h"
#include "plat_specific/inc/cpc_defs.h"
#include "plat_specific/inc/pmi_extr.h"
#include "plat_specific/inc/cem_extr.h"
#include "plat_specific/inc/crm_extr.h"
#include "plat_specific/inc/cwm_extr.h"
#endif

#if PLAT_SWITCH_WIN32
#include "plat_specific/plat_win32/plat_win32_defs.h"
#include "plat_specific/inc/cpc_defs.h"
#include "plat_specific/inc/pmi_extr.h"
#include "plat_specific/inc/cem_extr.h"
#include "plat_specific/inc/crm_extr.h"
#include "plat_specific/inc/cwm_extr.h"
#endif

#endif
