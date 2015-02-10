/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*		1993 - 2005, All Rights Reserved 			*/
/*									*/
/* MODULE NAME : stime.c  						*/
/* PRODUCT(S)  : 							*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*	This module implements functions that can be used in		*/
/*      differencial timimg.						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 06/10/05  EJV     22    Revised the repeating code.			*/
/*			   Linux, QNX: rpl gettimeofday() with times().	*/
/*			   Del MSDOS and __OS2__ code.			*/
/*			   Del sGetMsStartTime, sSetMsStartTime		*/
/*			   Renamed lastTime to initTime.		*/
/* 03/11/04  GLB     21    Remove "thisFileName"                    	*/
/* 12/09/03  JRB     20    Add LYNX support.				*/
/* 11/04/03  JRB     19    Add sMsSleep (replaces gs_sleep).		*/
/* 10/13/03  EJV     18    Chg defined(vms) to defined(__VMS)		*/
/* 04/08/03  EJV     17    Use S_LOCK_RESOURCES (better for non-MT apps)*/
/* 03/21/03  EJV     16    _WIN32: sGetMsTime, sGetSysMsTime must be	*/
/*			   protected by mutex,prevent incorect wrapCount*/
/* 03/18/03  EJV     15    Corrected __QNX4__ to __QNX__ (per JRB).	*/
/* 03/18/03  EJV     14    sGetMsTime: fixed lastTime casting problem	*/
/*			     (made lastTime ST_UINT on _WIN32).		*/ 
/* 02/17/03  CRM     13    Added "defined(linux)" code.			*/
/* 02/17/03  JRB     12    Add "sysincs.h" & del other includes.	*/
/*			   Del obsolete _WINDOWS, MSOS2 code.		*/
/* 02/17/03  EJV     11    sGetMsTime, sGetSysMsTime: fixed wrap counter*/
/*			   Auto-initialize funs with call to stimeInit()*/
/*			   Chg S_LOCK_RESOURCES to gs_get_sem,avoid loop*/
/* 06/05/02 MDE,EJV  10    sGetMsTime: workaround some math problem	*/
/* 11/29/01  EJV     09    Del code for old LATT, XENIX, ultrix, RMX86.	*/
/* 06/19/01  JRB     08    Chg S_LOCK_COMMON.. to S_LOCK_RESOURCES so	*/
/*			   glbsem doesn't log (caused infinite loop).	*/
/* 04/19/00  JRB     07    Lint cleanup.				*/
/* 11/18/99  EJV     06    Added code to capture wrapping of ms counter	*/
/*			   for _WIN32 and PHARLAP_ETS.			*/
/* 09/13/99  MDE     05    Added SD_CONST modifiers			*/
/* 11/05/97  DSF     04    Added sGetSysMsTime ()			*/
/* 09/22/97  JRB     03    Fixed some #if's.				*/
/* 09/09/97  JRB     02    Use GetTickCount for PHARLAP_ETS.		*/
/* 09/08/97  EJV     01    Added code for QNX.				*/
/* 04/02/97  DTL   7.00    MMSEASE 7.0 release. See MODL70.DOC for	*/
/*			   history.					*/
/************************************************************************/

#if defined (_WIN32)
#include <windows.h>
#include <mmsystem.h>
#if defined (PHARLAP_ETS)
#define timeGetTime	GetTickCount	/* timeGetTime not supported	*/
#endif		/* PHARLAP_ETS	*/
#endif

#include "glbtypes.h"
#include "sysincs.h"
#include "glbsem.h"
#include "nucleus.h"

#if defined(__VMS)
#include <starlet.h>
#endif

#if defined(linux) || defined(__QNX__) || defined(__LYNX)
#include <sys/times.h>			/* for struct tms		*/
#endif

/************************************************************************/
/* NOTE:  Users porting this code to new platform should not use the	*/
/*        gettimeofday() or similar functions. When the system time	*/
/*        is changed, for example during clock synchronization, the	*/
/*        gettimeofday() would also reflect this change.		*/
/*        Application should use times() function if available.		*/
/*									*/
/* DEBUG: On UNIX systems we should implement the sTime functions	*/
/*        the same way as on LINUX.					*/
/************************************************************************/

	/*------------------------------------------------------*/
	/*		stime module variables			*/
	/*------------------------------------------------------*/

#if defined(linux)
static ST_UINT64 wrapCount;	/* ms, cumulative wrapped time ticks elapsed since program start*/

#elif (defined (__QNX__) || defined(__LYNX))
static ST_DOUBLE wrapCount;	/* ms, cumulative wrapped time ticks elapsed since program start*/

#elif defined (_WIN32)
static ST_UINT64 wrapCount;	/* wrap count (see timeGetTime or GetTickTime)	*/

/* added by ZJY for C3X system 2006-06-03 */
#elif ( defined (_C3X) || defined (_PPC) )
static ST_DOUBLE wrapCount;	/* wrap count */
/* added by XYS for C6X system 2007-11-23 */
#elif defined (_C6X)
static ST_DOUBLE wrapCount;	/* wrap count */
#endif

static ST_ULONG  ticksPerSec;	/* system dependent value, returned by sysconf(_SC_CLK_TCK)	*/
static ST_ULONG  lastCheckTime;	/* last time checked, used to detect clock_t overflow		*/
static ST_DOUBLE initMsTime;	/* in ms, store initialization time	*/
static ST_INT    initialized;	/* SD_TRUE if stimeInit func successful	*/


/************************************************************************/
/*		                _sGetSysMsTime				*/
/*----------------------------------------------------------------------*/
/* Combined code from sGetMsTime and sGetSysMsTime, now also called by	*/
/* stimeInit.								*/
/*									*/
/*  NOTE: this new implementation on LINUX uses the times() function	*/
/*        to obtain the "number of clock ticks that have elapsed since	*/
/*        an arbitrary point in the past. For Linux this point is the	*/
/*        moment the system was booted. The return from times() may	*/
/*        overflow the possible range of clock_t."			*/
/*        The revision of the IEEE Std 1003.1, 2004 allows the that	*/
/*        reference point can be the start-up time of the process	*/
/*        rather than system start-up time.				*/
/*        IMPORTANT:							*/
/*	  This implementation is assuming that POSIX compliant clock_t	*/
/*        type is defined as long (4 bytes integer). If this is not the	*/
/*	  case then the	code needs to be revised!			*/
/*									*/
/* DEBUG:  In the future LINUX implementation should be used for all	*/
/*         UNIX systems.						*/
/************************************************************************/

static ST_DOUBLE _sGetSysMsTime (ST_VOID)
  {
ST_DOUBLE sysMsTime = (ST_DOUBLE) 0.0;	/* system time to be returned	*/

#if defined(linux)
  {
  struct tms tmsTime; /* must be passed to times() but we do not use it	*/
  ST_ULONG   sysTime;

  sysTime = (ST_ULONG) times (&tmsTime);
  if (sysTime != (ST_ULONG) -1)
    {
    /* NOTE: The value returned from the times() function (number of ticks	*/
    /*       from some fixed point in time) will wrap.				*/
    /*       Wrapping depends on the frequency (ticksPerSec) and can be as	*/
    /*       often as every 39 minutes. This function needs to be called	*/
    /*       frequently enough to detect when the value wraps.			*/
#if 0  /* DEBUG: code to test wrapping, */
      /*         trigger the wrapping every 60 seconds if this funct is called	*/
    if ( (sysTime > ticksPerSec) &&	/* don't wrap before 1-st second elapses*/
        ((sysTime / ticksPerSec) % 60 == 0) )	
      lastCheckTime=sysTime+1;			/* tweek the lastCheckTime */
#endif
    if (sysTime < lastCheckTime)
      /* the ticks counter wrapped to 0 */
      wrapCount += ((ST_UINT64) 1 << 32);	/* increment the high part of ST_UINT64	*/
    /* convert the sys time to ms accounting for possible wrap			*/
    /* (switch math operations order / with * for better precission, avoid floating point math)	*/
    sysMsTime = (ST_DOUBLE)((wrapCount + (ST_UINT64) sysTime) * 1000 / (ST_UINT64) ticksPerSec);
    lastCheckTime = sysTime;			/* save for next check	*/
    }
  else
    {
    ST_INT err;
    err = errno;	/* can't log this error but we could see it in debugger	*/
    }
  }
#elif defined(_AIX) || defined(sun) || defined(__hpux) \
    || (defined(__alpha) && !defined(__VMS))
  {
  struct timeval tp;
  struct timezone tzp;
  gettimeofday (&tp, &tzp);			/* avoid floating math		*/
  sysMsTime = (ST_DOUBLE) ( (ST_UINT64) ((ST_ULONG)  tp.tv_sec) * (ST_UINT64) 1000 +
                            (ST_UINT64) ((ST_ULONG) tp.tv_usec) / (ST_UINT64) 1000 );
  }
#elif defined (VXWORKS)
  {
  sysMsTime = (ST_DOUBLE) time (NULL) * 1000;
  }
#elif (defined (__QNX__)  || defined(__LYNX))
  {
  struct tms tmsTime; /* must be passed to times() but we do not use it	*/
  ST_ULONG   sysTime;

  sysTime = (ST_ULONG) times (&tmsTime);
  if (sysTime != (ST_ULONG) -1)
    {
    /* NOTE: The value returned from the times() function (number of ticks	*/
    /*       from some fixed point in time) will wrap.				*/
    /*       Wrapping depends on the frequency (ticksPerSec) and can be as	*/
    /*       often as every 39 minutes. This function needs to be called	*/
    /*       frequently enough to detect when the value wraps.			*/
    if (sysTime < lastCheckTime)
      /* the ticks counter wrapped to 0 */
      wrapCount += (ST_DOUBLE) (ULONG_MAX);
    /* convert the sys time to ms accounting for possible wrap,			*/
    /* note that we do not have ST_INT64 support on QNX or LYNX systems		*/
    sysMsTime = (ST_DOUBLE)((wrapCount + (ST_DOUBLE) sysTime) * 1000.0 / (ST_DOUBLE) ticksPerSec);
    lastCheckTime = sysTime;			/* save for next check	*/
    }
  }
#elif defined(__VMS)
  {
  ST_INT32 vmsTime = 0;
  /* DEBUG: possible problem with sys$gettim conversion to ms, revise if needed */
  sys$gettim (&vmsTime);
  sysMsTime = (ST_DOUBLE) (vmsTime / 10000);
  }
#elif defined (_WIN32)
  {
  ST_ULONG msTime;

  /* the timeGetTime function retrieves the system time, in milliseconds.*/
  /* The system time is the time elapsed since Windows was started.	 */

  /* NOTE: The timeGetTime and GetTickCount (used for PHARLAP_ETS)	*/
  /*       return DWORD value that wraps to 0 every 49 days.		*/
  /*       We need to detect the wrap up moment and save it.		*/
  msTime = (ST_ULONG) timeGetTime ();
  if ( msTime < lastCheckTime )
    /* the ms counter wrapped to 0 */
    wrapCount += ( (ST_UINT64) 1 << 32 );	/* increment the high part of ST_UINT64	*/
  sysMsTime = (ST_DOUBLE) (wrapCount + (ST_UINT64) msTime);
  lastCheckTime = msTime;			/* save for next check	*/
  }
/* added by ZJY for C3X system 2006-06-03 */
#elif ( defined (_C3X) || defined (_PPC) )
  {
#ifndef TMD_System_Clock
  extern volatile UNSIGNED TMD_System_Clock;
#endif
  ST_ULONG   sysTime;
  sysTime = TMD_System_Clock;
  if (sysTime < lastCheckTime)
    wrapCount += (ST_DOUBLE) 0xFFFFFFFF + (ST_DOUBLE) 1;	/* increment the high part of ST_UINT64	*/
  sysMsTime = wrapCount + (ST_DOUBLE) sysTime;
  lastCheckTime = sysTime;	/* save for next check	*/
  }
/* added by XYS for C3X system 2007-11-23 */
#elif defined (_C6X)
  {
  ST_ULONG   sysTime;

  /* ע������tickӦ��ת���ɺ��� */
  sysTime = TSK_time();
  if (sysTime < lastCheckTime)
    wrapCount += (ST_DOUBLE) 0xFFFFFFFF + (ST_DOUBLE) 1;	/* increment the high part of ST_UINT64	*/
  sysMsTime = wrapCount + (ST_DOUBLE) sysTime;
  lastCheckTime = sysTime;	/* save for next check	*/
  }
#else
#error Missing stime module implementation for this platform
#endif

  return (sysMsTime);
  }

/************************************************************************/
/*			_stimeReset					*/
/*----------------------------------------------------------------------*/
/* Resets stime module variables.					*/
/************************************************************************/

static ST_RET _stimeReset (ST_VOID)
  {
ST_RET rtn = SD_SUCCESS;

#if (defined(linux) || defined(_WIN32))
  wrapCount = 0;
  lastCheckTime = 0;
#elif (defined(__QNX__) || defined(__LYNX) || defined(_C3X) || defined(_C6X))
  wrapCount = 0.0;
  lastCheckTime = 0;
#endif

  initMsTime = _sGetSysMsTime ();

  return (rtn);
  }

/************************************************************************/
/*			stimeInit					*/
/*----------------------------------------------------------------------*/
/* Initialize stime module variables.					*/
/************************************************************************/

ST_RET stimeInit (ST_VOID)
  {
ST_RET rtn = SD_SUCCESS;

  /* Allow multiple calls */
  if (initialized)
    return (rtn);

  S_LOCK_RESOURCES ();	/* NOTE: this macro is now using non-logging gs_get_sem() */

#if defined(linux) || defined(__QNX__) || defined(__LYNX)
  {
  ticksPerSec = 0;

  ticksPerSec = (ST_ULONG) sysconf(_SC_CLK_TCK);
  if (ticksPerSec == (ST_ULONG) -1L || ticksPerSec == 0)
  {
    ticksPerSec = 10;
    rtn = SD_FAILURE;    /* some error getting the number of clock ticks per second	*/
  }
  }
#endif

  if (rtn == SD_SUCCESS)
    {
    rtn = _stimeReset ();
    if (rtn == SD_SUCCESS)
      initialized = SD_TRUE;
    }

  S_UNLOCK_RESOURCES ();

  return (rtn);
  }

/* ��ȡ�������к�����������ֵ�ﵽ���ֵʱ��ת */
ST_ULONG sGetSysTick(ST_VOID)
{
 ST_ULONG   sysTick;
#if defined(linux)
  {
  sysTick = (ST_ULONG)sGetSysMsTime();
  }
#elif defined (_WIN32)
  {
  sysTick = (ST_ULONG) timeGetTime ();
  }
#elif ( defined (_C3X) || defined (_PPC) )
  {
#ifndef TMD_System_Clock
  extern volatile UNSIGNED TMD_System_Clock;
#endif
  sysTick = TMD_System_Clock;
  }
#elif defined (_C6X)
  {
  /* ע������tickӦ��ת���ɺ��� */
  sysTick = TSK_time();
  }volatile UNSIGNED 
#else
#error Missing stime module implementation for this platform
#endif

  return (sysTick);

}
/************************************************************************/
/*			sGetMsTime					*/
/*----------------------------------------------------------------------*/
/* Returns time in milliseconds form the start of the program.		*/
/************************************************************************/

ST_DOUBLE sGetMsTime (ST_VOID)
  {
ST_DOUBLE sysMsTime, retMsTime;

  if (!initialized)
    if (stimeInit () != SD_SUCCESS)
      return ((ST_DOUBLE)0.0);

  S_LOCK_RESOURCES ();

  sysMsTime = _sGetSysMsTime();
  retMsTime = sysMsTime - initMsTime;

  S_UNLOCK_RESOURCES ();

  return (retMsTime);
  }

/************************************************************************/
/*			sGetSysMsTime					*/
/*----------------------------------------------------------------------*/
/* Returns system time in milliseconds (time from an arbitrary point	*/
/* like system start or program start).					*/
/************************************************************************/

ST_DOUBLE sGetSysMsTime (ST_VOID)
  {
ST_DOUBLE sysMsTime;

  if (!initialized)
    if (stimeInit () != SD_SUCCESS)
      return ((ST_DOUBLE)0.0);

  S_LOCK_RESOURCES ();

  sysMsTime = _sGetSysMsTime();

  S_UNLOCK_RESOURCES ();

  return (sysMsTime);
  }

/************************************************************************/
/*			sResetMsTime					*/
/*----------------------------------------------------------------------*/
/* Reset stime module variables.					*/
/************************************************************************/

ST_VOID sResetMsTime (ST_VOID)
  {	
  if (!initialized)
    {
    stimeInit ();			/* calls the _stimeReset()	*/
    return;
    }

  S_LOCK_RESOURCES ();

  _stimeReset ();

  S_UNLOCK_RESOURCES ();
  }

/************************************************************************/
/*			stimeExit					*/
/*----------------------------------------------------------------------*/
/* Terminate timer use.							*/
/************************************************************************/

ST_VOID	stimeExit (ST_VOID)
  {
  if (!initialized)
      return;

  S_LOCK_RESOURCES ();

  initialized = SD_FALSE;

  S_UNLOCK_RESOURCES ();
  }

/************************************************************************/
/*			sMsSleep					*/
/*----------------------------------------------------------------------*/
/* Calls system function to suspend current task or thread for		*/
/* specified amount of time.						*/
/* Parameters:								*/
/*	ST_LONG		ms	Number of milliseconds to sleep		*/
/* Return values:							*/
/*	none								*/
/************************************************************************/

#if defined(_WIN32)

ST_VOID sMsSleep (ST_LONG ms)
  {
  Sleep (ms);
  }
#elif ( defined(_C3X) || defined (_PPC) )

/* add by ZJY for C3X system 2006-06-03 */
ST_VOID sMsSleep (ST_LONG ms)
  {
    NU_Sleep (ms);
  }
#elif defined(_C6X)

/* add by XYS for C6X system 2007-11-23 */
ST_VOID sMsSleep (ST_LONG ms)
  {
    TSK_sleep(ms);
  }
#else	/* all other systems	*/

/* This version of the function uses the POSIX nanosleep function.	*/
ST_VOID sMsSleep (ST_LONG ms)
  {
struct timespec rqtp;

  rqtp.tv_sec = ms / 1000;
  rqtp.tv_nsec =  (ms % 1000) * 1000000;    /* 1000 ms/us, 1000 us/ns ... 	*/
  nanosleep (&rqtp, NULL);	/* should be available on AIX v5.x	*/
  }

#endif	/* all other systems	*/
