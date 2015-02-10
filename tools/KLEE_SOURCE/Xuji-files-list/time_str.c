/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*		1994 - 2003, All Rights Reserved			*/
/*									*/
/* MODULE NAME : time_str.c						*/
/* PRODUCT(S)  : 							*/
/*									*/
/* MODULE DESCRIPTION : 				  		*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 07/27/04  DWL     26    Added tstrTmToString (struct tm)		*/
/* 07/16/04  DWL     25    Added tstrStringToTm (struct tm)		*/
/* 01/27/04  nav     24    UtcValsToString: check for gmtime failure	*/
/* 12/09/03  JRB     24    Btime6String..: stop using "daylight" global,*/
/*			   use tm_isdst=0 to treat input as standard time.*/
/* 10/30/03  GLB &   23    Reworked "XmlStringToUtcValue" & 		*/
/*           EJV               "UtcValueToXmlString"                    */
/* 10/24/03  DSF     22    Fixed SX_DATE_TIME functions			*/
/* 10/15/03  JRB     21    Del _WIN32 ifdef.				*/
/* 09/01/03  GLB     20    Added "XmlStringToUtcValue" & 		*/
/*                          "UtcValueToXmlString"                       */
/* 06/03/03  DSF     19    More compiler warnings			*/
/* 04/14/03  JRB     18    Eliminate compiler warnings.			*/
/* 09/26/02  NAV     17    UtcValsToString: use gmtime                  */
/* 08/05/02  NAV     16    Make Btime4 conversion funcs ansi compatible */
/* 07/12/02  NAV     15    Add UtcValsToString and UtcStringToVals      */
/* 06/07/02  NAV     14    sprintf msec to %03d                         */
/* 02/25/02  JRB     13    Don't use "daylight" global on VXWORKS.	*/
/*			   Fix "thisFil..", sprintf call. Add str_util.h*/
/* 12/27/01  GLB     12    Remove embedded comment in prior log 11      */
/* 12/19/01  EJV     11    Converted comments from double slash to      */
/*                          slash star                                  */
/*			   Substituted AXS4_LOG_xxx with SLOGALWAYS.	*/
/*			   Replaced sys includes with sysincs.h		*/
/* 06/26/01  EJV     10    Eliminated globals to make thread safe.	*/
/* 02/02/01  EJV     09    tstrStringToTime: chg (long *) to (time_t *) */ 
/* 11/21/00  MDE     08    Changes for QNX (Btime4 for Win32 only)	*/
/* 07/17/98  NAV     07    Modify century calculation for y2k		*/
/* 10/15/97  NAV     06    Add Btime4 Support Functions			*/
/* 10/08/97  NAV     05    Add seconds to Btime4 and Gtime		*/
/* 10/06/97  NAV     04    Handle daylight savings time problem - Btime6*/
/* 09/04/97  NAV     03    Add Btime6 Conversion routines		*/
/* 08/01/96  NAV     02    Lint CleanUp					*/
/* 11/08/94  MDE     01    New						*/
/************************************************************************/
#include "glbtypes.h"
#include "sysincs.h"
#include "slog.h"
#include "time_str.h"
#include "str_util.h"

#ifdef DEBUG_SISCO
SD_CONST static ST_CHAR *SD_CONST thisFileName = __FILE__;
#endif

/************************************************************************/

char *tstrTimeFormat = TSTR_DEF_TIME_FORMAT;
char *BtimeTimeFormat = BTIME_DEF_TIME_FORMAT;

/************************************************************************/
/************************************************************************/
/*                       tstrTimeToString  				*/
/************************************************************************/

int tstrTimeToString (time_t t, char *dest)
  {
  strftime (dest, MAX_TIME_STRING_LEN, tstrTimeFormat, localtime (&t));
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/

/* Converted globals into this struct and added params to parseXXX functions */
typedef struct
  {
  int month;
  int day;
  int year;
  int hour;
  int min;
  int sec;
  int mSec;
  int dateFound;
  int timeFound;
  } _TS_DATETIME;

static int parseDateString (char *s, _TS_DATETIME *dt);
static int parseTimeString (char *s, _TS_DATETIME *dt);
static int parseBtimeString (char *s, _TS_DATETIME *dt);
static int strToMonth (char *s, int *monthOut);

/************************************************************************/
/*                       tstrStringToTime				*/
/************************************************************************/

int tstrStringToTime (char *src, time_t *out)
  {
struct tm tm_val;
time_t tm_rslt;
char workBuf[MAX_TIME_STRING_LEN+1];
char *p;
char *s1;
char *s2;
_TS_DATETIME dt;

  /* Make a working copy */
  strcpy (workBuf, src);

  /* Now replace all '/' with '-' */
  while ((p = strpbrk (workBuf,"/")))
    *p = '-';

  /* Now get pointer to the second substring, if present */
  s1 = workBuf;
  s2 = workBuf;

  /* Find the first whitespace or end of string */
  while (*s2 != ' ' &&  *s2 != '\t' && *s2)
    {
    ++s2;
    }

  if (!*s2)
    s2 = NULL;
  else		/* Find the next non-whitespace */
    {
    *s2 = 0;	/* terminate s1 */
    ++s2;	
    while (*s2 == ' ' ||
           *s2 == '\t') 
      {
      ++s2;
      }
    if (!isdigit (*s2))	/* better be a digit */
      s2 = NULL;
    }

  /* Now we have one or two substrings */
  /* First deal with s1 */
  if (strpbrk (s1,"-"))	  	/* must be a date string */
    {
    if (parseDateString (s1, &dt))
      return (SD_FAILURE);
    }
  else if (strpbrk (s1,":")) 	/* must be a time string */
    {
    if (parseTimeString (s1, &dt))
      return (SD_FAILURE);
    }
  else
    {
    return (SD_FAILURE);
    }

  /* Now deal with s2, if present */
  if (s2)
    {
    if (strpbrk (s2,"-"))      	/* must be a date string */
      {
      if (parseDateString (s2, &dt))
        return (SD_FAILURE);
      }
    else if (strpbrk (s2,":")) 	/* must be a time string */
      {
      if (parseTimeString (s2, &dt))
        return (SD_FAILURE);
      }
    else
      {
      return (SD_FAILURE);
      }
    }

  /* If the time was ommitted, use the current time */
  if (!dt.timeFound)
    return (SD_FAILURE);   /* must have both for now */

  /* If the date was ommitted, use the current date */
  if (!dt.dateFound)
    return (SD_FAILURE);   /* must have both for now */

  /* Now convert the components to long time */
  tm_val.tm_wday = 0;		/* this is an output parameter */
  tm_val.tm_yday = 0;		/* this is an output parameter */

  tm_val.tm_year = dt.year - 1900;
  tm_val.tm_mon = dt.month;
  tm_val.tm_mday = dt.day;
  tm_val.tm_hour = dt.hour;
  tm_val.tm_min = dt.min;
  tm_val.tm_sec = dt.sec;
  tm_val.tm_isdst = -1;  	/* let function guess */
  tm_rslt = mktime(&tm_val);
  if (tm_rslt == -1)
    return (SD_FAILURE);

  *out = tm_rslt;
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       tstrStringToTm					*/
/* Convert string to struct tm.						*/
/************************************************************************/

int tstrStringToTm (char *src, struct tm *out)
  {
struct tm tm_val;
char workBuf[MAX_TIME_STRING_LEN+1];
char *p;
char *s1;
char *s2;
_TS_DATETIME dt;

  /* Make a working copy */
  strcpy (workBuf, src);

  /* Now replace all '/' with '-' */
  while ((p = strpbrk (workBuf,"/")))
    *p = '-';

  /* Now get pointer to the second substring, if present */
  s1 = workBuf;
  s2 = workBuf;

  /* Find the first whitespace or end of string */
  while (*s2 != ' ' &&  *s2 != '\t' && *s2)
    {
    ++s2;
    }

  if (!*s2)
    s2 = NULL;
  else		/* Find the next non-whitespace */
    {
    *s2 = 0;	/* terminate s1 */
    ++s2;	
    while (*s2 == ' ' ||
           *s2 == '\t') 
      {
      ++s2;
      }
    if (!isdigit (*s2))	/* better be a digit */
      s2 = NULL;
    }

  /* Now we have one or two substrings */
  /* First deal with s1 */
  if (strpbrk (s1,"-"))	  	/* must be a date string */
    {
    if (parseDateString (s1, &dt))
      return (SD_FAILURE);
    }
  else if (strpbrk (s1,":")) 	/* must be a time string */
    {
    if (parseTimeString (s1, &dt))
      return (SD_FAILURE);
    }
  else
    {
    return (SD_FAILURE);
    }

  /* Now deal with s2, if present */
  if (s2)
    {
    if (strpbrk (s2,"-"))      	/* must be a date string */
      {
      if (parseDateString (s2, &dt))
        return (SD_FAILURE);
      }
    else if (strpbrk (s2,":")) 	/* must be a time string */
      {
      if (parseTimeString (s2, &dt))
        return (SD_FAILURE);
      }
    else
      {
      return (SD_FAILURE);
      }
    }

  /* If the time was ommitted, use the current time */
  if (!dt.timeFound)
    return (SD_FAILURE);   /* must have both for now */

  /* If the date was ommitted, use the current date */
  if (!dt.dateFound)
    return (SD_FAILURE);   /* must have both for now */

  /* Now convert the components to long time */
  tm_val.tm_wday = 0;		/* this is an output parameter */
  tm_val.tm_yday = 0;		/* this is an output parameter */

  tm_val.tm_year = dt.year - 1900;
  tm_val.tm_mon = dt.month;
  tm_val.tm_mday = dt.day;
  tm_val.tm_hour = dt.hour;
  tm_val.tm_min = dt.min;
  tm_val.tm_sec = dt.sec;
  tm_val.tm_isdst = -1;  	/* let function guess */

  *out = tm_val;
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       tstrTmToString					*/
/* Convert struct tm to string.						*/
/************************************************************************/

int tstrTmToString (struct tm *t, char *dest)
  {
  char *timeFormatStr = "%m-%d-%Y %H:%M:%S";
  strftime (dest, 30, timeFormatStr, t);
  return (SD_SUCCESS);
  }

/************************************************************************/
/*			parseDateString					*/
/************************************************************************/

static int parseDateString (char *s, _TS_DATETIME *dt)
  {
char *p;
char *d1;
char *d2;
char *d3;
int century;

  dt->dateFound = SD_TRUE;

  /* break into three substrings */
  d1 = s;
  p =  strpbrk (s,"-");
  *p = 0;
  d2 = ++p;
  p =  strpbrk (d2,"-");
  if (!p)
    return (SD_FAILURE);
  *p = 0;
  d3 = ++p;

  /* Now process each date sub-substring seperately  */

  /* Check for alpha month forms */
  if (isalpha (*d1))
    {
    if (strToMonth (d1, &dt->month))
      return (SD_FAILURE);
    if (!sscanf (d2, "%d", &dt->day))
      return (SD_FAILURE);
    }
  else if (isalpha (*d2))
    {
    if (!sscanf (d1, "%d", &dt->day))
      return (SD_FAILURE);
    if (strToMonth (d2, &dt->month))
      return (SD_FAILURE);
    }
  else 	/* Not an ALPHA month form */
    {
    /* Numeric month is d1 */
    if (!sscanf (d1, "%d", &dt->month))
      return (SD_FAILURE);
    --dt->month;			/* we use 0-11 for month */

    /* Numeric day is d2 */
    if (!sscanf (d2, "%d", &dt->day))
      return (SD_FAILURE);
    }

  /* Numeric year is always d3 */
  if (!sscanf (d3, "%d", &dt->year))
    return (SD_FAILURE);
  if (strlen (d3) == 2)
    {
    /* any year < 1984 is assumed to be in the 21st century	*/
    century = (dt->year < 84) ? 2000 : 1900;
    dt->year += century;
    }
  return (SD_SUCCESS);
  }


/************************************************************************/
/*			parseTimeString					*/
/************************************************************************/

static int parseTimeString (char *s, _TS_DATETIME *dt)
  {
int pm;
char *p, *t1, *t2, *t3;

  dt->timeFound = SD_TRUE;

  /* break into two substrings */
  p =  strpbrk (s,":");
  t1 = s;  		/* hours are here */
  *p = 0;
  t2 = ++p;		/* minutes are here */
  if (!p)
    return (SD_FAILURE);

  p = strpbrk (t2, ":");
  if (!p)
    t3 = NULL;
  else
    {
    *p = 0;
    t3 = ++p;
    }

  pm = SD_FALSE;    
  if (t3)
    {
    if (t3[2] == 'A' || t3[2] == 'a')
      {
      t3[2] = 0;
      }
    else if (t3[2] == 'P' || t3[2] == 'p')
      {
      pm = SD_TRUE;    
      t3[2] = 0;
      }
    }

  if (!sscanf (t1, "%d", &dt->hour))
    return (SD_FAILURE);

  if (!sscanf (t2, "%d", &dt->min))
    return (SD_FAILURE);

  if (t3)
    sscanf (t3, "%d", &dt->sec);
  else
    dt->sec = 0;

  if (pm)
    dt->hour += 12;

  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/

static char *monthStrings[12] =
  {
  "Jan",
  "Feb",
  "Mar",
  "Apr",
  "May",
  "Jun",
  "Jul",
  "Aug",
  "Sep",
  "Oct",
  "Nov",
  "Dec"
  };


static int strToMonth (char *s, int *monthOut)
  {
int i;

  for (i = 0; i < 12; ++i)
    {
    if (!stricmp (s, monthStrings[i]))
      {
      *monthOut = i;
      return (SD_SUCCESS);
      }
    }
  return (SD_FAILURE);
  }




/************************************************************************/
/* Btime6 Conversion Routines:						*/
/************************************************************************/

#define SEC_PER_MIN	60
#define SEC_PER_HOUR	60 * SEC_PER_MIN
#define SEC_PER_DAY	24 * SEC_PER_HOUR
#define MSEC_PER_SEC	1000

/************************************************************************/
/* Btime6StringToVals:  Receive string in BTIME_DEF_TIME_FORMAT+mSec	*/
/*			and return the number of days and msecs 	*/
/*			since Jan 1, 1984 midnight			*/
/* NOTE: output values are NOT adjusted for timezone or daylight savings.*/
/************************************************************************/

ST_RET Btime6StringToVals (char *src, ST_INT32 *numDays, ST_INT32 *numMSec)
  {
struct tm tmJan84, tmThis;
time_t tJan84, tThis;
char workBuf[MAX_TIME_STRING_LEN+1];
char *p;
char *s1;
char *s2;
double elapsedTime;
long nDays, nMsec;
ldiv_t divResult;
_TS_DATETIME dt;

  /* make sure struct is clear */
  memset (&dt, 0, sizeof(_TS_DATETIME));

  /* Make a working copy */
  strcpy (workBuf, src);

  /* Now replace all '/' with '-' */
  while ((p = strpbrk (workBuf,"/")))
    *p = '-';

  /* Now get pointer to the second substring, if present */
  s1 = workBuf;
  s2 = workBuf;

  /* Find the first whitespace or end of string */
  while (*s2 != ' ' &&  *s2 != '\t' && *s2)
    {
    ++s2;
    }

  if (!*s2)
    s2 = NULL;
  else		/* Find the next non-whitespace */
    {
    *s2 = 0;	/* terminate s1 */
    ++s2;	
    while (*s2 == ' ' ||
           *s2 == '\t') 
      {
      ++s2;
      }
    if (!isdigit (*s2))		/* better be a digit */
      s2 = NULL;
    }

  /* Now we have one or two substrings */
  /* First deal with s1 */
  if (strpbrk (s1,"-"))	  	/* must be a date string */
    {
    if (parseDateString (s1, &dt))
      return (SD_FAILURE);
    }
  else if (strpbrk (s1,":")) 	/* must be a time string */
    {
    if (parseBtimeString (s1, &dt))
      return (SD_FAILURE);
    }
  else
    {
    return (SD_FAILURE);
    }

  /* Now deal with s2, if present */
  if (s2)
    {
    if (strpbrk (s2,"-"))      	/* must be a date string */
      {
      if (parseDateString (s2, &dt))
        return (SD_FAILURE);
      }
    else if (strpbrk (s2,":")) 	/* must be a time string */
      {
      if (parseBtimeString (s2, &dt))
        return (SD_FAILURE);
      }
    else
      {
      return (SD_FAILURE);
      }
    }

  /* check for valid data */
  if (!dt.timeFound)
    return (SD_FAILURE);   /* must have both for now */

  if (!dt.dateFound)
    return (SD_FAILURE);   /* must have both for now */

  if (dt.year < 1984)	   /* must be later than 01/01/84 */
    return (SD_FAILURE);

  /* get jan 1, 84 as a long time */
  tmJan84.tm_wday = 0;		/* this is an output parameter */
  tmJan84.tm_yday = 0;		/* this is an output parameter */

  tmJan84.tm_year = 84;
  tmJan84.tm_mon = 0;
  tmJan84.tm_mday = 1;
  tmJan84.tm_hour = 0;
  tmJan84.tm_min = 0;
  tmJan84.tm_sec = 0;
  tmJan84.tm_isdst = 0;	/* assume time is NOT adjusted for daylight savings*/
  tJan84 = mktime(&tmJan84);
  if (tJan84 == -1)
    return (SD_FAILURE);

  /* get this data as a long */
  tmThis.tm_wday = 0;		/* this is an output parameter */
  tmThis.tm_yday = 0;		/* this is an output parameter */

  tmThis.tm_year = dt.year - 1900;
  tmThis.tm_mon = dt.month;
  tmThis.tm_mday = dt.day;
  tmThis.tm_hour = dt.hour;
  tmThis.tm_min = dt.min;
  tmThis.tm_sec = dt.sec;
  tmThis.tm_isdst = 0;	/* assume time is NOT adjusted for daylight savings*/

  tThis = mktime(&tmThis);

  if (tThis == -1)
    return (SD_FAILURE);

  /* find the number of seconds between the two long dates and */
  /* convert into days and mSec */
  elapsedTime = difftime (tThis, tJan84);
  divResult = ldiv ((long) elapsedTime, SEC_PER_DAY);
  nDays = divResult.quot;
  nMsec = (divResult.rem * MSEC_PER_SEC) + dt.mSec;

  *numDays = nDays;
  *numMSec = nMsec;

  return (SD_SUCCESS);

  }

/************************************************************************/
/* Btime6ValsToString:  Receive the number of days and msecs since	*/
/*			Jan 1, 1984 - midnight and return a string in	*/
/*			BTIME_DEF_TIME_FORMAT+mSec			*/
/************************************************************************/

ST_RET Btime6ValsToString (char *dest, ST_INT32 numDays, ST_INT32 numMSec)
  {
struct tm tmVal;
time_t tmRslt;
long numSeconds, balMSec;
char stash[MAX_TIME_STRING_LEN+1];
ldiv_t divResult;
_TS_DATETIME dt;

  divResult = ldiv (numMSec, MSEC_PER_SEC);
  numSeconds = divResult.quot;
  balMSec = divResult.rem;

  divResult = ldiv (numSeconds, SEC_PER_DAY);
  dt.day = divResult.quot + numDays;
  numSeconds = divResult.rem;

  divResult = ldiv (numSeconds, SEC_PER_HOUR);
  dt.hour = divResult.quot;
  numSeconds = divResult.rem;

  divResult = ldiv (numSeconds, SEC_PER_MIN);
  dt.min = divResult.quot;
  numSeconds = divResult.rem;

  dt.sec = numSeconds;

  /* set up the struct tm	*/
  tmVal.tm_wday = 0;		/* this is an output parameter */
  tmVal.tm_yday = 0;		/* this is an output parameter */

  tmVal.tm_year = 84;
  tmVal.tm_mon = 0;
  tmVal.tm_mday = dt.day+1;
  tmVal.tm_hour = dt.hour;
  tmVal.tm_min = dt.min;
  tmVal.tm_sec = dt.sec;
  tmVal.tm_isdst = -1;  	/* let function guess */
  tmRslt = mktime(&tmVal);
  if (tmRslt == -1)
    return (SD_FAILURE);

  /* now turn it into a string	with out mSeconds 			*/
  strftime (stash, MAX_TIME_STRING_LEN, BtimeTimeFormat, localtime (&tmRslt));

  /* now append the mSeconds to the string				*/
  sprintf (dest, "%s.%03ld", stash, balMSec);

  return (SD_SUCCESS);
  }


/************************************************************************/
/* Btime4StringToVals:  Receive string in BTIME_DEF_TIME_FORMAT.mSec	*/
/*			and return the number of msecs since midnight	*/
/************************************************************************/

ST_RET Btime4StringToVals (char *src, ST_INT32 *numMSec)
  {
int curDay, curMonth, curYear;
char workBuf[MAX_TIME_STRING_LEN+1];
char *p;
char *s1;
char *s2;
int totalMsec, maxMsec;
_TS_DATETIME dt;
struct tm *curTime;
time_t theTime;

  /* make sure struct is clear */
  memset (&dt, 0, sizeof(_TS_DATETIME));

  /* figure out todays date to use for validity check	*/
  theTime = time (NULL);
  curTime = localtime (&theTime);
  curDay = curTime->tm_mday;
  curMonth = curTime->tm_mon;
  curYear = curTime->tm_year + 1900;

  /* Make a working copy */
  strcpy (workBuf, src);

  /* Now replace all '/' with '-' */
  while ((p = strpbrk (workBuf,"/")))
    *p = '-';

  /* Now get pointer to the second substring, if present */
  s1 = workBuf;
  s2 = workBuf;

  /* Find the first whitespace or end of string */
  while (*s2 != ' ' &&  *s2 != '\t' && *s2)
    {
    ++s2;
    }

  if (!*s2)
    s2 = NULL;
  else		/* Find the next non-whitespace */
    {
    *s2 = 0;	/* terminate s1 */
    ++s2;	
    while (*s2 == ' ' ||
           *s2 == '\t') 
      {
      ++s2;
      }
    if (!isdigit (*s2))		/* better be a digit */
      s2 = NULL;
    }

  /* Now we have one or two substrings */
  /* First deal with s1 */
  if (strpbrk (s1,"-"))	  	/* must be a date string */
    {
    if (parseDateString (s1, &dt))
      return (SD_FAILURE);
    }
  else if (strpbrk (s1,":")) 	/* must be a time string */
    {
    if (parseBtimeString (s1, &dt))
      return (SD_FAILURE);
    }
  else
    {
    return (SD_FAILURE);
    }

  /* Now deal with s2, if present */
  if (s2)
    {
    if (strpbrk (s2,"-"))      	/* must be a date string */
      {
      if (parseDateString (s2, &dt))
        return (SD_FAILURE);
      }
    else if (strpbrk (s2,":")) 	/* must be a time string */
      {
      if (parseBtimeString (s2, &dt))
        return (SD_FAILURE);
      }
    else
      {
      return (SD_FAILURE);
      }
    }

  /* check for valid data */
  if (!dt.timeFound)
    return (SD_FAILURE);   /* must have both for now */

  if (!dt.dateFound)
    return (SD_FAILURE);   /* must have both for now */

  if ( (dt.day != curDay) || (dt.month != curMonth) || (dt.year != curYear) )
    {
    SLOGALWAYS0 ("Btime4 Conversion Error:  Input Date must be todays date");
    return (SD_FAILURE);
    }

  /* get mSecs from hours/min/secs					*/
  totalMsec = (dt.hour * SEC_PER_HOUR * MSEC_PER_SEC);
  totalMsec+= (dt.min * SEC_PER_MIN * MSEC_PER_SEC);
  totalMsec+= (dt.sec * MSEC_PER_SEC);
  totalMsec+= dt.mSec;

  /* ensure mSecs does not exceen num mSecs in a day			*/
  maxMsec = (SEC_PER_DAY * MSEC_PER_SEC) - 1;
  if (totalMsec > maxMsec)
    {
    SLOGALWAYS1 ("Btime4 Conversion Error:  too many milliseconds (%d).", totalMsec);
    return (SD_FAILURE);
    }

  *numMSec = totalMsec;
  
  return (SD_SUCCESS);
  }

/************************************************************************/
/* Btime4ValsToString: Receive number of msecs since midnight and return*/
/*		       string in BTIM_DEF_TIME_FORMAT.mSec		*/
/************************************************************************/

ST_RET Btime4ValsToString (char *dest, ST_INT32 numMSec)
  {
int curDay, curMonth, curYear;
struct tm tmVal, *curTime;
time_t tmRslt;
long numSeconds, balMSec;
char stash[MAX_TIME_STRING_LEN+1];
ldiv_t divResult;
_TS_DATETIME dt;
time_t theTime;

  /* figure out todays date */
  theTime = time (NULL);
  curTime = localtime (&theTime);
  curDay = curTime->tm_mday;
  curMonth = curTime->tm_mon;
  curYear = curTime->tm_year;

  divResult = ldiv (numMSec, MSEC_PER_SEC);
  numSeconds = divResult.quot;
  balMSec = divResult.rem;

  divResult = ldiv (numSeconds, SEC_PER_HOUR);
  dt.hour = divResult.quot;
  numSeconds = divResult.rem;

  divResult = ldiv (numSeconds, SEC_PER_MIN);
  dt.min = divResult.quot;
  numSeconds = divResult.rem;

  dt.sec = numSeconds;

  /* set up the struct tm	*/
  tmVal.tm_wday = 0;		/* this is an output parameter */
  tmVal.tm_yday = 0;		/* this is an output parameter */

  tmVal.tm_year = curYear;
  tmVal.tm_mon = curMonth;
  tmVal.tm_mday = curDay;
  tmVal.tm_hour = dt.hour;
  tmVal.tm_min = dt.min;
  tmVal.tm_sec = dt.sec;
  tmVal.tm_isdst = -1;  	/* let function guess */
  tmRslt = mktime(&tmVal);
  if (tmRslt == -1)
    return (SD_FAILURE);

  /* now turn it into a string	with out mSeconds 			*/
  strftime (stash, MAX_TIME_STRING_LEN, BtimeTimeFormat, localtime (&tmRslt));

  /* now append the mSeconds to the string				*/
  sprintf (dest, "%s.%03ld", stash, balMSec);

  return (SD_SUCCESS);
  }



/************************************************************************/
/* parseBtimeString:  parse HH:MM:SS:msec				*/
/************************************************************************/

static int parseBtimeString (char *s, _TS_DATETIME *dt)
  {
char stash[MAX_TIME_STRING_LEN+1];
char toFind[] = ":.";
char *token;
int count = 0;

  dt->timeFound = SD_TRUE;

  /* make a copy first */
  strcpy (stash, s);

  /* set default values */
  dt->hour = dt->min = dt->sec = dt->mSec = 0;

  /* do the token thing to separate the string */
  token = strtok (stash, toFind);
  while (token != NULL)
    {
    switch (count)
      {
      case 0:
        dt->hour = atoi (token);
	break;
      case 1:
        dt->min = atoi (token);
	break;
      case 2:
        dt->sec = atoi (token);
	break;
      case 3:
        dt->mSec = atoi (token);
	break;
      }
    count++;
    /* get next token */
    token = strtok (NULL, toFind);
    }
  
  return (SD_SUCCESS);
  }


/************************************************************************/
/* UTC Time Conversion Functions:                                       */
/*        Format = YYYY-MM-DDThh:mm:ss.000000000Z(qual=b,b,b,b,n)       */
/************************************************************************/
/* UtcStringToVals:                                                     */
/************************************************************************/

ST_RET UtcStringToVals (char *src, ST_UINT32 *pSecs, ST_UINT32 *pFraction, 
                        ST_UINT32 *pQflags)
  {
_TS_DATETIME dt;
ST_INT b0, b1, b2, b3, rest;
ST_DOUBLE fr;
struct tm tmTemp;
time_t tTemp;
ST_UINT32 frTemp;
ST_UINT32 qTemp = 0;
char *pDate, *pTime, *pFract, *pflags;
time_t currTime, local_t, utc_t;
struct tm *pJunkTm, localTm, utcTm;
ST_DOUBLE timeAdjust;


  /* figure out diff between utc and local time */
  currTime = time (NULL);
  pJunkTm = gmtime (&currTime);
  memcpy (&utcTm, pJunkTm, sizeof (utcTm));
  pJunkTm = localtime (&currTime);
  memcpy (&localTm, pJunkTm, sizeof (localTm));
  utc_t = mktime (&utcTm);
  local_t = mktime (&localTm);
  timeAdjust = difftime (local_t, utc_t);

  memset (&dt, 0, sizeof(_TS_DATETIME));

  /* split up the string  */
  pDate = src;
  pTime = strchr (src, 'T');
  if (!pTime)
    {
    SLOGALWAYS1 ("StringToUtc: Unable to identify time in '%s'.", src);
    return SD_FAILURE;
    }
  pTime++;

  pFract = strchr (pTime, '.');
  if (!pFract)
    {
    SLOGALWAYS1 ("StringToUtc: Unable to identify fraction in '%s'.", src);
    return SD_FAILURE;
    }

  pflags = strchr (pFract, 'Z');
  if (!pflags)
    {
    SLOGALWAYS1 ("StringToUtc: Unable to identify quality in '%s'.", src);
    return SD_FAILURE;
    }
  pflags++;

  /* parse the pieces */
  if (sscanf (pDate, "%d-%d-%d", &dt.year, &dt.month, &dt.day) != 3)
    {
    SLOGALWAYS1 ("StringToUtc: Unable to parse date in '%s'.", src);
    return SD_FAILURE;
    }

  if (sscanf (pTime, "%d:%d:%d", &dt.hour, &dt.min, &dt.sec) != 3)
    {
    SLOGALWAYS1 ("StringToUtc: Unable to parse time in '%s'.", src);
    return SD_FAILURE;
    }

  fr = atof (pFract);

  if (strlen (pflags) < 14)
    {
    SLOGALWAYS1 ("StringToUtc: Unable to parse qflags in '%s'.", src);
    return SD_FAILURE;
    }
  b0 = (pflags[6] == '1') ? 1 : 0;
  b1 = (pflags[8] == '1') ? 1 : 0;
  b2 = (pflags[10] == '1') ? 1 : 0;
  b3 = (pflags[12] == '1') ? 1 : 0;
  rest = atoi (&pflags[14]);

  /* handle seconds */
  memset (&tmTemp, 0, sizeof (struct tm));
  tmTemp.tm_year = dt.year - 1900;
  tmTemp.tm_mon = dt.month - 1;
  tmTemp.tm_mday = dt.day;
  tmTemp.tm_hour = dt.hour;
  tmTemp.tm_min = dt.min;
  tmTemp.tm_sec = dt.sec;
  tmTemp.tm_isdst = 0;  	
  tTemp = mktime (&tmTemp);     /* this is local time */
  if (tTemp == -1)
    {
    SLOGALWAYS1 ("ERROR: StringToUtc: Unable to convert time '%s'.", src);
    return (SD_FAILURE);
    }	    

  tTemp += (time_t) timeAdjust;

  *pSecs = tTemp;


  /* handle fraction  */
  frTemp = (ST_UINT32) (fr * (ST_DOUBLE) 0x00FFFFFF);
  *pFraction = frTemp;

  /* handle qflags    */
  b0 <<= 7;
  b1 <<= 6;
  b2 <<= 5;
  b3 <<= 4;
  qTemp = (b0 | b1 | b2 | b3 | rest);
  *pQflags = qTemp;

  return SD_SUCCESS;
  }

/************************************************************************/
/* UtcValsToString:                                                     */
/************************************************************************/

ST_RET UtcValsToString (char *dest, ST_UINT32 secs, ST_UINT32 fraction, 
                        ST_UINT32 qflags)
  {
ST_CHAR theDate[MAX_TIME_STRING_LEN];
ST_CHAR theFraction[25];
ST_CHAR theQual[25];
ST_DOUBLE dFraction;
char *pFract;
ST_INT b0, b1, b2, b3, rest;
time_t t = secs;
struct tm *pTm;

  /* get the date portion */
  pTm = gmtime (&t);
  if (!pTm)
    {
    SLOGALWAYS0 ("UtcValsToString:  conversion failure - invalid seconds.");
    return SD_FAILURE;
    }

  strftime (theDate, MAX_TIME_STRING_LEN, UTC_DEF_TIME_FORMAT, gmtime (&t));

  /* get the fraction portion */
  dFraction = ((ST_DOUBLE) fraction / (ST_DOUBLE) 0x00FFFFFF);
  sprintf (theFraction, " %#0.09f", dFraction);
  pFract = strchr (theFraction, '.');
  if (!pFract)
    {
    SLOGALWAYS1 ("UtcToString - unable to convert fraction %d", fraction);
    return SD_FAILURE;
    }

  /* get the qflags     */
  b0 = (qflags & 0x00000080);
  b1 = (qflags & 0x00000040);
  b2 = (qflags & 0x00000020);
  b3 = (qflags & 0x00000010);
  rest = (qflags &0x0000000F);

  b0 >>= 7;
  b1 >>= 6;
  b2 >>= 5;
  b3 >>= 4;

  sprintf (theQual, "Z(qual=%d,%d,%d,%d,%d)", b0, b1, b2, b3, rest);

  /* put em all together  */
  sprintf (dest, "%s%s%s", theDate, pFract, theQual);

  return SD_SUCCESS;
  }

/************************************************************************/
/* XmlStringToUtcValue                                                  */
/************************************************************************/
/*   An input time and date string is converted to the number of        */
/*   seconds since 1/1/1970.                                            */
/*                                                                      */ 
/*   Any of the following strings are valid input to this subroutine:	*/
/*									*/
/*     "yyyy-mm-ddThh:mm:ss.fffff+/-hh:mm"   				*/
/*     "yyyy-mm-ddThh:mm:ss+/-hh:mm"	      				*/
/*									*/
/*     "yyyy-mm-ddThh:mm:ss.fffffZ"	 				*/
/*     "yyyy-mm-ddThh:mm:ssZ"		 				*/
/*									*/
/*     "yyyy-mm-ddThh:mm:ss.fffff"	 				*/
/*     "yyyy-mm-ddThh:mm:ss"		  				*/
/*									*/
/*     Note:	                                                        */
/*       Decimal fraction for microseconds:                     .fffff  */
/*       East time zone offset from GMT (Greenwich Mean Time):  +hh:mm  */
/*       West time zone offset from GMT (Greenwich Mean Time):  -hh:mm  */
/*									*/
/*                                                                      */
/* Output is stored in SX_DATE_TIME structure as:                       */
/*                                                                      */
/*    dateTime	        stored as number of seconds elapsed since       */
/*    		        midnight (00:00:00) January 1, 1970,            */
/*		        UTC (Coordinated Universal Time), according     */
/*		        to the system clock                             */
/*    useMicroseconds   indicates decimal fraction for seconds was      */
/*                      specified                                       */
/*    microseconds      specified decimal fraction of seconds stored as */
/*                      microseconds                                    */
/*    useTZ             indicates a time zone offset is present         */
/*    tz                time zone offset specified as minutes           */
/*									*/
/* Time zone offset "tz" and time zone presence "useTZ" will be         */
/* specified in output as follows:                                      */
/*     									*/
/*  "yyyy-mm-ddThh:mm:ss.fffff+/-hh:mm" 'useTZ = SD_TRUE'               */
/*                                      'tz = +/-seconds'               */	        
/*  "yyyy-mm-ddThh:mm:ss+/-hh:mm"       'useTZ = SD_TRUE'               */ 
/*                                      'tz = +/-seconds'	        */
/*									*/
/*  "yyyy-mm-ddThh:mm:ss.fffffZ"	'useTZ = SD_TRUE'  'tz = 0' 	*/
/*  "yyyy-mm-ddThh:mm:ssZ"	        'useTZ = SD_TRUE'  'tz = 0' 	*/
/*								 	*/
/*  "yyyy-mm-ddThh:mm:ss.fffff"	        'useTZ = SD_FALSE' 'tz ignored' */
/*  "yyyy-mm-ddThh:mm:ss"	        'useTZ = SD_FALSE' 'tz ignored' */
/*                                  					*/
/************************************************************************/

ST_RET XmlStringToUtcValue (char *src, SX_DATE_TIME *sxDateTime)
  {
ST_CHAR   *pDate, *pTime, *pFraction;
ST_CHAR   *pTimeZone;
ST_DOUBLE  timeZoneAdjustment;
ST_INT     zoneHours, zoneMinutes; /* used to calculate minutes in specified time zone offset from UTC time */
_TS_DATETIME dateTime;	           /* temporarily stores date/time to convert to UTC time                   */
struct tm tmTemp;	           /* holds date/time values converted to UTC time                          */
time_t tTemp;                      /* holds local time in seconds                                           */

  /* split up the string input from XML file so we can store */
  /* number of seconds, number of microseconds (decimal      */
  /* fraction of seconds) and number of minutes in time zone */
  /* offset separately                                       */

  /* get the date */
  pDate = src;
  if (sscanf (pDate, "%d-%d-%d", &dateTime.year, &dateTime.month, &dateTime.day) != 3)
    {
    SLOGALWAYS1 ("ERROR: Unable to parse date in '%s'.", src);
    return SD_FAILURE;
    }
 
  /* get the time */
  pTime = strchr (src, 'T');
  if (!pTime)
    {
    SLOGALWAYS1 ("ERROR: Unable to identify time in '%s'.", src);
    return SD_FAILURE;
    }
  pTime++;

  if (sscanf (pTime, "%d:%d:%d", &dateTime.hour, &dateTime.min, &dateTime.sec) != 3)
    {
    SLOGALWAYS1 ("ERROR: Unable to parse time in '%s'.", src);
    return SD_FAILURE;
    }
  else
    dateTime.timeFound = SD_TRUE;  /* currently not used */

  /* get the decimal fraction of seconds if present */
  sxDateTime->useMicroseconds = SD_FALSE;
  pFraction = strchr (pTime, '.');
  if (pFraction)
    {
    ++pFraction;
    if (sscanf (pFraction, "%ld", &sxDateTime->microseconds) != 1)
      {
      SLOGALWAYS1 ("ERROR: Unable to parse fraction in '%s'.", pTime);
      return SD_FAILURE;
      }
    else
      sxDateTime->useMicroseconds = SD_TRUE;
    }
 
  /* get the time zone offset in minutes if present */
  sxDateTime->useTZ = SD_FALSE;
  sxDateTime->tz = 0;

  /* if "Z" is present in input string then no time zone offset will be specified */
  if (strchr (pTime,'Z'))
    {
    sxDateTime->useTZ = SD_TRUE;
    sxDateTime->tz = 0;
    }
  else
    {
    pTimeZone = strchr (pTime, '+');
    if (!pTimeZone)
      pTimeZone = strchr (pTime, '-');
    if (pTimeZone)
      {
      if (sscanf (pTimeZone, "%d:%d", &zoneHours, &zoneMinutes) != 2)
        {
        SLOGALWAYS1 ("ERROR: Unable to parse East time zone in '%s'.", pTime);
        return SD_FAILURE;
        }
      else
        {
        /* calculate total minutes in specified time zone offset */
        sxDateTime->useTZ = SD_TRUE;
	if (zoneHours < 0)
	  zoneMinutes = - zoneMinutes;
        sxDateTime->tz = ((zoneHours * 60) + zoneMinutes);
        }
      }
    }

  /* calculate total seconds in UTC time so we can store  */
  /* it as a "long" value or number of seconds since      */
  /* 1/1/1970                                             */ 
  memset (&tmTemp, 0, sizeof (struct tm));
  tmTemp.tm_year = dateTime.year - 1900;   /* current year minus 1900 */
  tmTemp.tm_mon = dateTime.month - 1;	   /* 0 - 11 */
  tmTemp.tm_mday = dateTime.day;	   /* 0 - 31 */
  tmTemp.tm_hour = dateTime.hour;	   /* 0 - 23 hours from midnight */
  tmTemp.tm_min = dateTime.min;		   /* 0 - 59 */
  tmTemp.tm_sec = dateTime.sec;		   /* 0 - 59 */
  if (sxDateTime->useTZ && sxDateTime->tz == 0)
    tmTemp.tm_isdst = 0;
  else
    tmTemp.tm_isdst = -1; /*  0 indicates standard time */
                          /* -1 run-time system determines daylight savings or standard time */	
			  /*  1 indicates daylight savings time */
  /* we use the "mktime" function to return local time        */
  /* because there is no function of this type to return a    */
  /* UTC/GMT/Zulu time                                        */
  tTemp = mktime (&tmTemp);    
  if (tTemp == -1)
    {
    SLOGALWAYS1 ("ERROR: Unable to convert time '%s'.", src);
    return (SD_FAILURE);
    }
  else	    
    {
    /* calculate the UTC/GMT/Zulu time                         */

    if (sxDateTime->useTZ && sxDateTime->tz == 0)
      {
      /* find difference between local time and UTC/GMT/Zulu time */
      timeZoneAdjustment = CalculateTimeZoneOffset(); 

      /* add the difference to local time to obtain a UTC/GMT/Zulu time */
      tTemp += (time_t) timeZoneAdjustment;
      }

    /* save the calculated time value */
    sxDateTime->dateTime = tTemp;
    }

  return SD_SUCCESS;
  }

/************************************************************************/
/* UtcValueToXmlString                                                  */
/************************************************************************/  
/*  The specified number of seconds since 1/1/1970 is converted to a    */
/*  time and date string.                                               */
/*                                                                      */
/*  Input is stored in "SX_DATE_TIME" as:                               */
/*                                                                      */
/*    dateTime	        stored as number of seconds elapsed since       */
/*    		        midnight (00:00:00) January 1, 1970,            */
/*		        UTC (Coordinated Universal Time), according     */
/*		        to the system clock                             */
/*    useMicroseconds   indicates decimal fraction of seconds was       */
/*                      specified                                       */
/*    microseconds      decimal fraction of seconds specified stored as */
/*                      microseconds                                    */
/*    useTZ             indicates a time zone offset is present         */
/*    tz                time zone offset from GMT (Greenwich Mean Time) */
/*                      specified as minutes                            */
/*									*/
/*  Output string format will depend upon values present in the         */
/*  "SX_DATE_TIME" structure as follows:                                */
/*                                                                      */
/*  'useTZ = SD_TRUE' 'tz = nn..n'  "yyyy-mm-ddThh:mm:ss.fffff+/-hh:mm" */
/*  'useTZ = SD_TRUE' 'tz = nn..n'  "yyyy-mm-ddThh:mm:ss+/-hh:mm"       */
/*									*/
/*  'useTZ = SD_TRUE'  'tz = 0'     "yyyy-mm-ddThh:mm:ss.fffffZ"	*/
/*  'useTZ = SD_TRUE'  'tz = 0'     "yyyy-mm-ddThh:mm:ssZ"		*/
/*									*/
/*  'useTZ = SD_FALSE' 'tz ignored' "yyyy-mm-ddThh:mm:ss.fffff"		*/
/*  'useTZ = SD_FALSE' 'tz ignored' "yyyy-mm-ddThh:mm:ss"		*/
/*									*/
/************************************************************************/

ST_RET UtcValueToXmlString (ST_CHAR *dest, ST_UINT destLen,
                            SX_DATE_TIME *sxDateTime)
  {
ST_CHAR    theFraction[64], theTZ[64];

#define MAX_DATE_TIME_STR_LEN	32	/* longest output format:		*/
					/*  "yyyy-mm-ddThh:mm:ss.fffff+/-hh:mm"	*/

  if (destLen < MAX_DATE_TIME_STR_LEN)
    {
    SLOGALWAYS2 ("ERROR: Buffer %d bytes may be too small for XML string (min=%d)",
      destLen, MAX_DATE_TIME_STR_LEN);
    return (SD_FAILURE);
    }

  /* get the date and time from the "dateTime" or 
     number of seconds from 1/1/1970 (UTC time) 
     stored in "sxDateTime" and place it in a
     structure of type "tm" returned from the call
     to "gmtime" as:
        sec
        min
        hour
        day of month
        month 
        year
        day of week
        day of year                                     */
  /* from the data stored in the structure utilize 
     "strftime" to format a date and time string using 
     the following output format:
	%Y is year with century as a decimal number
	%m is month as a decimal number (01-12)
	%d is day of month as a decimal number (01-31)
	%H is hour in 24 hour format (00-23)
	%M is minute as decimal number (00-59)
	%S is second as decimal number (00-59)	        */
  if (sxDateTime->useTZ && sxDateTime->tz == 0)
    strftime (dest, destLen, "%Y-%m-%dT%H:%M:%S", gmtime (&sxDateTime->dateTime));
  else
    strftime (dest, destLen, "%Y-%m-%dT%H:%M:%S", localtime (&sxDateTime->dateTime));

  /* then get number of microseconds stored as an       */
  /* "long" value in the "sxDateTime" structure and     */
  /* format it as a string with a leading decimal point */
  if (sxDateTime->useMicroseconds)
    {
    sprintf (theFraction, ".%05ld", sxDateTime->microseconds);
    strcat (dest, theFraction);
    }

  /* now get the number of minutes stored as an integer */
  /* for time zone offset and place it in a string      */
  /* in the format of "+hh:mm" or "-hh:mm"              */
  if (sxDateTime->useTZ)
    {
    if (sxDateTime->tz == 0)
      /* output format:  "yyyy-mm-ddThh:mm:ss.fffffZ"           	*/
      strcat (dest, "Z");
    else
      {
      /* output format:  "yyyy-mm-ddThh:mm:ss.fffff+/-hh:mm"           */
      if (sxDateTime->tz > 0)
        sprintf (theTZ, "%+2.2d:%2.2d", sxDateTime->tz / 60, sxDateTime->tz % 60);
      else
        sprintf (theTZ, "%+2.2d:%2.2d", sxDateTime->tz / 60, -(sxDateTime->tz % 60));
      strcat (dest, theTZ);
      }
    }

  return SD_SUCCESS;
  }				


/************************************************************************/
/* CalculateTimeZoneOffset                                              */
/************************************************************************/
/*  Figures out the difference between UTC/GMT/Zulu time and            */
/*  local time.                                                         */
/*  This difference can be used after the "mktime"                      */
/*  function is called. The "mktime" function returns local time        */
/*  and to convert this time to a UTC time this calculated              */
/*  adjustment must be added to the local time value returned           */
/*  from "mktime".                                                      */
/************************************************************************/

ST_DOUBLE CalculateTimeZoneOffset (ST_VOID)
  {
time_t     currTime, local_t, utc_t;
struct tm *pJunkTm, localTm, utcTm;
ST_DOUBLE  timeZoneAdjustment;

  currTime = time (NULL);		          /* get the current system time */
  pJunkTm = gmtime (&currTime);		          /* convert current time value in seconds to a structure */
  memcpy (&utcTm, pJunkTm, sizeof (utcTm));	  /* save current UTC time */
  pJunkTm = localtime (&currTime);		  /* convert current time value and correct for local time zone */
  memcpy (&localTm, pJunkTm, sizeof (localTm));	  /* save local UTC time */
  utc_t = mktime (&utcTm);                        /* convert UTC time to UTC seconds */
  local_t = mktime (&localTm);			  /* convert local time to UTC seconds */
  timeZoneAdjustment = difftime (local_t, utc_t); /* find the difference or time zone offset */

  return (timeZoneAdjustment);
  }
