/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	  1986 - 2005, All Rights Reserved.		        */
/*									*/
/*		    PROPRIETARY AND CONFIDENTIAL			*/
/*									*/
/* MODULE NAME : sysincs.h						*/
/* PRODUCT(S)  : MMSEASE						*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*	The purpose of this include file is to bring in include files	*/
/*	that come with one of the various C compilers.			*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 08/10/05  JRB     35    Fixed a few newline characters.		*/
/* 07/15/05  DSF     34    Fixed SASSERT warnings for release builds	*/
/* 05/24/05  EJV     33    UNIX, Linux: added include <sys/resource.h>	*/
/* 03/18/05  DSF     32    Log asserts					*/
/* 12/07/04  JRB     31    Define larger FD_SETSIZE for QNX.		*/
/* 09/22/04  EJV     30    Added INADDR_NONE (needed on sun)		*/
/* 07/08/04  DSF     29    Fixed SASSERT macro				*/
/* 01/26/04  JRB     28    Fix LYNX includes.				*/
/* 01/22/04  DSF     27    Added/modified ASSERT macros			*/
/* 01/19/04  EJV     26    __hpux 10.20 does not have sys/select.h.	*/
/* 12/09/03  JRB     25    Add LYNX support. Add () to max,min macros.	*/
/* 10/23/03  JRB     24    Move socket includes to here.		*/
/*			   For _WIN32, add windows.h, process.h		*/
/*			   Del unused CDECL define.			*/
/* 04/28/03  DSF     23    Make sure expression inside VERIFY is always	*/
/*			   executed					*/
/* 04/15/03  DSF     22    Don't define/undef NDEBUG 			*/
/* 03/18/03  JRB     21    Fix QNX #ifdef.				*/
/* 02/20/03  JRB     20    Del PSOS code.				*/
/* 02/17/03  CRM     19    Added "defined(linux)" code.			*/
/* 02/17/03  JRB     18	   Stop using SYSTEM_SEL define.		*/
/*			   Add conio.h for WIN32.			*/
/*			   Add sys/time.h, sys/select.h for UNIX.	*/
/*			   Add termio.h for UNIX & QNX.			*/
/* 02/27/02  EJV     17    Added limits.h to most of systems.		*/
/*			   Check if LONG_MAX and INT_MAX defined.	*/
/* 07/27/01  DSF     16    crtdbg.h should only be included if _DEBUG	*/
/*			   is defined					*/
/* 03/16/01  DSF     15    Make sure VERIFY is not defined before	*/
/*			   defining it					*/
/* 02/02/01  DSF     14    Added asserts				*/
/* 01/19/01  EJV     13    Added <ctype.h>  for      SYSTEM_SEL_SYS_5.	*/
/* 01/03/01  EJV     12    Added <fcntl.h>  for      SYSTEM_SEL_SYS_5.	*/
/* 10/13/00  EJV     11    Added <unistd.h> for some SYSTEM_SEL_SYS_5.	*/
/* 05/05/00  JRB     10    Added "select.h" for some systems.		*/
/* 04/20/00  EJV     09    Added stdarg.h for SYSTEM_SEL_SYS_5.		*/
/* 02/04/99  JRB     08    Added types.h & stat.h for most systems.	*/
/* 07/01/98  JRB     07    Added stdarg.h & ctype.h for some systems.	*/
/* 12/29/97  JRB     06    Cleaned up OS2 (include os2.h). More PSOS.	*/
/* 12/11/97  JRB     05    Cleaned up PSOS includes. Use SYS_PSOS.	*/
/* 12/11/97  JRB     04    Deleted EMBEDDED_6800 includes.		*/
/* 10/29/97  EJV     03    Corrected max maro definition		*/
/* 10/28/97  EJV     02    Added *.h files specific for SYS_5		*/
/* 08/15/97  EJV     01    Added *.h files specific for SYS_QNX4	*/
/* 04/02/97  DTL   7.00    MMSEASE 7.0 release. See MODL70.DOC for	*/
/*			   history.					*/
/************************************************************************/

#ifndef SYSINCS_INCLUDED
#define SYSINCS_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__OS2__)
#define INCL_BASE	/* include all OS2 definitions.			*/
#include <os2.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#ifdef kbhit		/* The OS/2 2.0 version is not what we want	*/
#undef kbhit
#endif
#include <process.h>		/* for _beginthread, _endthread	*/
#include <signal.h>		/* for "kill", etc.		*/
/* Sockets related includes	*/
#include <sys/socket.h>
#include <netinet/in.h>		/* IPPROTO_*, etc.			*/
#include <netdb.h>		/* gethostbyname, etc.			*/
#include <sys/ioctl.h>		/* defines FIONBIO			*/
#endif	/* OS2	*/

#if defined (_WIN32)
#include <windows.h>
#include <process.h>		/* for _beginthread, _endthread	*/
#endif

#if defined(_WIN32) || defined(MSDOS) || defined(__MSDOS__)
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <conio.h>
#endif

#if defined(VXWORKS)
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <selectLib.h>
#include <limits.h>
#include <signal.h>		/* for "kill", etc.		*/
/* Sockets related includes	*/
#include <sys/socket.h>
#include <ioLib.h>
#include <sockLib.h>
#include <pipeDrv.h>
#include <sysLib.h>
#include <usrLib.h>
#include <netinet/in.h>		/* IPPROTO_*, etc.			*/
#include <arpa/inet.h>		/* inet_addr, etc.			*/
#include <netinet/tcp.h>	/* TCP_NODELAY, etc.			*/
#endif

#if defined(__QNX__)
/* FD_SETSIZE default is only 32 on QNX. Must define larger value	*/
/* here (before any system includes) to allow more TCP connections.	*/
#define FD_SETSIZE	600
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <sys/types.h>
#include <sys/stat.h>		/* S_IFIFO, S_IRUSR, S_IWUSR, etc.	*/
#include <sys/ioctl.h>
#include <sys/select.h>
#include <unistd.h>		/* mknod, etc.				*/
#include <fcntl.h>		/* open, O_RDONLY, O_WRONLY, etc.	*/
#include <process.h>		/* execlp, etc.				*/
#include <errno.h>
#include <stdarg.h>
#include <ctype.h>
#include <limits.h>
#include <termio.h>
#include <signal.h>		/* for "kill", etc.		*/
/* Sockets related includes	*/
#include <sys/socket.h>
#include <netdb.h>		/* gethostbyname, etc.			*/
#include <netinet/in.h>		/* IPPROTO_*, etc.			*/
#include <arpa/inet.h>		/* inet_addr, etc.			*/
#include <netinet/tcp.h>	/* TCP_NODELAY, etc.			*/
#endif

/* UNIX or "UNIX-like" systems	*/
#if defined(_AIX) || defined(sun) || defined(__hpux) || defined(linux) \
    || (defined(__alpha) && !defined(__VMS)) || defined(__LYNX)
#include <unistd.h>	/* SHOULD be before any other include files 	*/
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#if (!defined(__LYNX))
#include <sys/time.h>
#include <sys/resource.h>
#endif
#define	max(a,b)	(((a) > (b)) ? (a) : (b))
#define	min(a,b)	(((a) < (b)) ? (a) : (b))
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <fcntl.h>		/* open, O_RDONLY, O_WRONLY, etc.	*/
#include <ctype.h>
#include <limits.h>
#include <termio.h>
#if (!defined (__hpux)) && (!defined(__LYNX))
#include <sys/select.h>
#endif
#include <signal.h>		/* for "kill", etc.		*/
#include <sys/ioctl.h>
/* Sockets related includes	*/
#if defined(__LYNX)
#include <socket.h>
#else
#include <sys/socket.h>
#endif
#include <netdb.h>		/* gethostbyname, etc.			*/
#include <netinet/in.h>		/* IPPROTO_*, etc.			*/
/* Forward references are supplied to eliminate xlC_r compiler warnings	*/
struct ether_addr;		/* forward reference			*/
struct sockaddr_dl;		/* forward reference			*/
#include <arpa/inet.h>		/* inet_addr, etc.			*/
#include <netinet/tcp.h>	/* TCP_NODELAY, etc.			*/
#ifndef INADDR_NONE
#define INADDR_NONE   ((in_addr_t) 0xffffffff)
#endif
#endif

#if defined(__VMS)
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <types.h>
#include <stat.h>
#define	max(a,b)	((a > b) ? a : b)
#define	min(a,b)	((a < b) ? a : b)
#endif

/************************************************************************/
/* Added by Zhou Jingyu for C3X SYSTEM                                  */
/************************************************************************/
#if defined(_C3X)
#include <clib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <limits.h>
#include <errno.h>

#include "nucleus.h"
#include "dsp_tcpip.h"

#undef  IPPROTO_TCP
#define IPPROTO_TCP 0

#undef  FAR
#define FAR

#endif
/************************************************************************/
/* Added by Zhou Jingyu End                                             */
/************************************************************************/

/************************************************************************/
/* Added by XYS for C6X SYSTEM                                  */
/************************************************************************/
#if defined(_C6X)
#include <std.h>
#include <tsk.h>
#include <prd.h>
#include <sem.h>
#include <swi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <limits.h>
#include <errno.h>

#include "dsp_tcpip.h"

#undef  IPPROTO_TCP
#define IPPROTO_TCP 0

#undef  FAR
#define FAR

#endif /* _C6X */
/************************************************************************/
/* Added by XYS End                                             */
/************************************************************************/

/************************************************************************/
/* Added by LIJIE for POWERPC SYSTEM                                  */
/************************************************************************/
#if defined(_PPC)
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <limits.h>
#include <errno.h>

#undef  IPPROTO_TCP
#define IPPROTO_TCP 0

#undef  FAR
#define FAR

#endif /* _PPC */
/************************************************************************/
/* Added by POWERPC End                                             */
/************************************************************************/

#if (!defined(INT_MAX)) || (!defined(LONG_MAX))
#error INT_MAX and LONG_MAX must be defined. Usually defined in limits.h
#endif


/************************************************************************/
/************************************************************************/
/*		Assert stuff						*/
/************************************************************************/
/************************************************************************/

#include <assert.h>

#if defined (_WIN32) && defined (_DEBUG)
#include <crtdbg.h>
#endif

#if !defined (C_ASSERT)
#define C_ASSERT(e) typedef char __C_ASSERT__[(e)?1:-1]
#endif

#if !defined (_ASSERT)
#define _ASSERT(e)	C_ASSERT(0)
#endif

#if !defined (_ASSERTE)
#define _ASSERTE(e)	C_ASSERT(0)
#endif

#if !defined (ASSERT)
#define ASSERT		_ASSERT
#endif

#if !defined (ASSERTE)
#define ASSERTE		_ASSERTE
#endif


#if !defined (_SASSERT)
#define _SASSERT(e) \
	{ \
	int assertResult = (e) ? 1 : 0; \
	if (!assertResult) {SLOGALWAYS3("Assertion failed: '%s', file %s, line %d", #e, __FILE__, __LINE__);} \
	assert (assertResult); \
	}
#endif

#if !defined (_SASSERTE)
#define _SASSERTE(e) \
	{ \
	int assertResult = (e) ? 1 : 0; \
	if (!assertResult) {SLOGALWAYS3("Assertion failed: '%s', file %s, line %d", #e, __FILE__, __LINE__);} \
	assert (assertResult); \
	}
#endif

#if !defined (SASSERT)
#define SASSERT		_SASSERT
#endif

#if !defined (SASSERTE)
#define SASSERTE	_SASSERTE
#endif

#if !defined (VERIFY)
#if !defined (NDEBUG)
#define VERIFY(e)	_SASSERTE(e)
#else
#define VERIFY(e)	((void) (e))
#endif
#endif

#if !defined (C_ASSERT)
#define C_ASSERT(e) typedef char __C_ASSERT__[(e)?1:-1]
#endif

#if (defined (_C3X) || defined (_C6X))
#undef assert
#define assert(exp)
#endif

#ifdef __cplusplus
}
#endif

#endif


