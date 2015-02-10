/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*		1994 - 2003, All Rights Reserved			*/
/*									*/
/* MODULE NAME : time_str.h						*/
/* PRODUCT(S)  : 							*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 07/27/04  DWL     12    Added tstrTmToString (struct tm)		*/
/* 07/16/04  DWL     11    Added tstrStringToTm (struct tm)		*/
/* 10/30/03  EJV     10    Added dataLen param to UtcValueToXmlString	*/
/* 10/15/03  JRB     09    Del _WIN32 ifdef.				*/
/* 09/01/03  GLB     08    Added "XmlStringToUtcValue" & 		*/
/*                          "UtcValueToXmlString"                       */
/* 07/12/02  NAV     07    Add UtcValsToString and UtcStringToVals      */
/* 02/02/01  EJV     06    tstrStringToTime: chg (long *) to (time_t *) */ 
/* 07/13/98  NAV     05    Add _cplusplus support			*/
/* 10/15/97  NAV     04    Add Btime4 Support Functions			*/
/* 10/08/97  NAV     03    Add seconds to TSTR_DEF_TIME_FORMAT		*/
/* 09/04/97  NAV     02    Add Btime6 Conversion routines		*/
/* 11/08/94  MDE     01    New						*/
/************************************************************************/

/************************************************************************/
/*		TIME STRING FORM FOR READ DATA				*/
/*									*/
/*  Date{ws}Time							*/
/*  Time{ws}Date	(? should we allow this)			*/
/*  Time only	(uses current time)					*/
/*  Date only	(uses current date)					*/
/*									*/
/*	Date forms :							*/
/*	 12/01/56							*/
/*	 12/01/1956							*/
/*	 12-01-56 							*/
/*	 12-01-1956 							*/
/*	 12-1-56 							*/
/*	 12-1-56	 						*/
/* 	 26-Aug-1994							*/
/* 	 26-AUG-1994							*/
/* 	 AUG-26-1993							*/
/*									*/
/*	Time forms :							*/
/*	 7:21:43P							*/
/*	 7:21:43p							*/
/*	 7:21:43a							*/
/*	 7:21:43A							*/
/*	 07:21:43a							*/
/*	 7:21:43							*/
/*	 21:21:43							*/
/*									*/
/************************************************************************/
#include "sx_defs.h"

#ifdef __cplusplus
extern "C" {
#endif


#define MAX_TIME_STRING_LEN 256

#define TSTR_DEF_TIME_FORMAT  "%m-%d-%Y %H:%M:%S"

extern char *tstrTimeFormat;

int tstrStringToTime (char *src, time_t *out);
int tstrStringToTm (char *src, struct tm *out);
int tstrTimeToString (time_t t, char *dest);
int tstrTmToString (struct tm *t, char *dest);

#define BTIME_DEF_TIME_FORMAT "%m-%d-%Y %H:%M:%S"

ST_RET Btime6StringToVals (char *src, ST_INT32 *numDays, ST_INT32 *numMSec);
ST_RET Btime6ValsToString (char *dest, ST_INT32 numDays, ST_INT32 numMSec);

ST_RET Btime4StringToVals (char *src, ST_INT32 *numMSec);
ST_RET Btime4ValsToString (char *dest, ST_INT32 numMSec);

ST_RET UtcStringToVals (char *src, ST_UINT32 *pSecs, ST_UINT32 *pFraction, 
                        ST_UINT32 *pQflags );
ST_RET UtcValsToString (char *dest, ST_UINT32 secs, ST_UINT32 fraction, 
                        ST_UINT32 qflags);
ST_RET UtcStringToValues (char *src, ST_UINT32 *pSecs, ST_UINT32 *pFraction);
ST_RET UtcValuesToString (char *dest, ST_UINT32 secs, ST_UINT32 fraction);
ST_RET XmlStringToUtcValue (ST_CHAR *src, SX_DATE_TIME *sxDateTime);
ST_RET UtcValueToXmlString (ST_CHAR *dest, ST_UINT destLen, SX_DATE_TIME *sxDateTime);
ST_DOUBLE CalculateTimeZoneOffset (ST_VOID);
#define UTC_DEF_TIME_FORMAT "%Y-%m-%dT%H:%M:%S"

#ifdef __cplusplus
} /* End of 'C' functions	*/

#endif

