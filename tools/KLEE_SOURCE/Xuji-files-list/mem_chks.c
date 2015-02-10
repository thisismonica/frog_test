/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*              1986-2005 All Rights Reserved                   	*/
/*									*/
/* MODULE NAME : mem_chks.c						*/
/* PRODUCT(S)  : MMSEASE						*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*	This module contains enhanced memory allocation functions	*/
/*	for performing various integrity checks				*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 05/11/05  MDE     45    Minor logging 				*/
/* 05/23/05  EJV     44    Add memLogMaskMapCtrl & memDebugMapCtrl	*/
/*			    for parsing logcfg.xml			*/
/* 05/11/05  MDE     43    Now do list check for chk_calloc too		*/
/* 03/10/04  EJV     42    Use '%lu' and typecast to (ST_ULONG) for	*/
/*			     ST_UINT32 logging.				*/
/* 02/06/04  EJV     42    Added dyn_mem_ptr_status2.			*/
/*			   Use '%u' when logging memory statistics.	*/
/* 06/06/03  JRB     41    Use S_LOCK_RESOURCES (new util mutex).	*/
/* 04/08/03  DSF     40    1) No need to lock common resources in nd_	*/
/*			   functions or when checking if we've been	*/
/*			   initialized					*/
/*			   2) Use Microsoft CRT Debug calls when	*/
/*			   MEMCHK_WIN32_CRT_DEBUG is defined and	*/
/*			   m_mem_crt_debug is set			*/
/* 03/06/02  JRB     39    Add S_UNLOCK.. before ret in chk_alloc_ptr.	*/
/* 02/11/02  DSF     38	   Add a call to ExpInit ()			*/
/* 10/18/01  JRB     37    Eliminate warning. Fix *_memerr_fun calls.	*/
/* 10/10/01  JRB     36    Del unused valid_string function.		*/
/* 09/19/01  JRB     35    Add NULL ptr check in dyn_mem_ptr_status.	*/
/* 08/29/01  EJV     34    mem_report: prevented stack overflow.	*/
/* 08/20/01  JRB     33    If !DEBUG_SISCO, chk_* macros call nd_chk_*.	*/
/*			   If DEBUG_SISCO,  chk_* macros call x_chk_*.	*/
/*			   nd_* functions use CHK_DYN_MEM_NODE_NDEBUG	*/
/*			   Globals calling_file/line deleted (see args).*/
/*			   Del (x_)chk_free_wipe functions.		*/
/* 08/20/01  JRB     32    Del m_pad_string_len (use m_num_pad_bytes)	*/
/* 08/20/01  JRB     31    Default m_find_node_enable=FALSE (faster).	*/
/*			   Ignore m_track_prev_free (not effective).	*/
/* 08/16/01  JRB     30    Delete SAVE_CALLING_ADDRESS code for DOS.	*/
/* 04/02/01  JRB     29    ALWAYS call init_mem_chk automatically (not	*/
/*			   depending on DEBUG_SISCO or m_mem_debug).	*/
/*			   Fix previous chg to save m_mem_debug value.	*/
/* 03/27/01  GLB     28    Added MEMCHK_WIN32_CRT_DEBUG			*/
/* 03/03/01  GLB     27    save m_mem_debug in m_mem_debug_saved        */
/* 02/23/01  DSF     26    m_mem_debug should not be initialized to TRUE*/
/* 01/30/01  DSF     25    Added WIN32 Debug CRT memory checks		*/
/* 04/28/00  JRB     24    Lint cleanup.				*/
/* 04/19/00  JRB     23    Del SD_CONST from def_realloc_err arg.	*/
/*			   Fix "uninitialized" warnings in chk_realloc.	*/
/* 10/13/99  RKR     22    Cast file to a ST_CHAR *, fixed a prototype	*/
/* 09/13/99  MDE     21    Added SD_CONST modifiers			*/
/* 09/07/99  MDE     20    Minor optimization for calloc (1, x)		*/
/* 04/14/99  MDE     19    Cleaned up system dependency stuff a bit	*/
/* 04/14/99  MDE     18    Fixed 'chk_strdup' file/line			*/
/* 03/05/98  MDE     17    Added 'chk_strdup'				*/
/* 02/23/99  RKR     16    fixed the redefinition of new_mem_node	*/
/* 02/15/99  DSF     15    fixed a bug in the new chk_calloc() routine -*/
/*			   don't fill allocated buffer			*/
/* 02/09/99  DSF     14    chk_calloc () now makes direct calls to	*/
/*			   RTL calloc () - faster			*/
/* 02/02/99  JRB     13    Add "()" in free calls in chk_free:		*/
/*			    helps some "free" macros work.		*/
/*			   Don't init chk_debug_en in declaration:	*/
/*			    some compilers put it in ROM (unchangable).	*/
/*			   Don't use obsolete MEMCHK_ERR_PRINT.		*/
/* 12/08/98  MDE     12    Added 'ST_BOOLEAN m_auto_hw_log'		*/
/* 10/09/98  MDE     11    Changed 'MEMLOG_CHERR' usage per new macros	*/
/* 10/08/98  MDE     10    Migrated to updated SLOG interface		*/
/* 09/21/98  MDE     09    Minor lint cleanup				*/
/* 06/15/98  MDE     08    Changes to allow compile under C++		*/
/* 01/30/98  EJV     07    Added typecating (ST_VOID **) in list funcs.	*/
/* 10/14/97  DSF     06    m_bad_ptr_val is now a pointer		*/
/* 10/13/97  DSF     05    Initialize chk_debug_en = MEM_LOG_ERR	*/
/* 09/16/97  DSF     04    chk_debug_en is UINT				*/
/* 06/19/97  RKR     03    Changed return code from MEM_PTR_CHK_ERROR2	*/
/* 05/27/97  DSF     02    Minor bug fix				*/
/* 05/20/97  RKR     01    Changed default of m_mem_debug to SD_TRUE	*/
/* 04/02/97  DTL   7.00    MMSEASE 7.0 release. See MODL70.DOC for	*/
/*			   history.					*/
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "glbsem.h"

#define MEM_CHK					/* used in mem_chk.h 	*/
#include "mem_chk.h"
#include "memlog.h"
#include "gen_list.h"
#include <ctype.h>

#ifdef _WIN32
#include "msjexhnd.h"
#endif

/* WIN32 CRT Debug used for all but MMS-EASE Lite users			*/
/* MMS-EASE Lite users can define in project if desired ...		*/
#if defined (_WIN32) && defined (_DEBUG) && !defined(MMS_LITE)
#define MEMCHK_WIN32_CRT_DEBUG 
#endif

#if defined(MEMCHK_WIN32_CRT_DEBUG)
#include <crtdbg.h>
int mem_report (int reportType, char *userMessage, int *retVal);
ST_BOOLEAN m_mem_crt_debug;
#endif

/************************************************************************/
/************************************************************************/
/* These should be defined on the compiler command line			*/
/* #define DEBUG 							*/
/* #define NO_REALLOC_SMALLER 						*/

#if (((defined(MSDOS) || defined(__MSDOS__)) && !defined(_WIN32)) && !defined(_WINDOWS))
#define MSOFT_HEAP_CHECK
#endif

/************************************************************************/
/* For debug version, use a static pointer to avoid duplication of 	*/
/* __FILE__ strings.							*/

#ifdef DEBUG_SISCO
SD_CONST static ST_CHAR *SD_CONST thisFileName = __FILE__;
#endif

/************************************************************************/
/* These functions (with leading '_') are EXACTLY the same as the	*/
/* "nd_chk_*" functions, but they are static so they can only be	*/
/* called by the "x_chk_*" functions within this module, and		*/
/* only if DEBUG_SISCO is defined.					*/
/* This prevents the user from mixing debug and non-debug calling code.	*/
/*   If this module is compiled with DEBUG_SISCO defined, then ALL user	*/
/* code must be compiled with DEBUG_SISCO defined, and the user code	*/
/* will call "x_chk_*".							*/
/*   If this module is compiled with DEBUG_SISCO !defined, then ALL user*/
/* code must be compiled with DEBUG_SISCO !defined, and the user code	*/
/* will call "nd_chk_*".							*/
/************************************************************************/
#ifdef DEBUG_SISCO
static ST_VOID *_nd_chk_malloc (ST_UINT size);
static ST_VOID *_nd_chk_calloc (ST_UINT num, ST_UINT size);
static ST_VOID *_nd_chk_realloc (ST_VOID *ptr, ST_UINT size);
static ST_VOID  _nd_chk_free (ST_VOID *del_mem_ptr);
#endif

/************************************************************************/
/************************************************************************/
/* DEBUG related defines & variables (leave declared for both versions)	*/

ST_BOOLEAN m_mem_debug;
#ifdef DEBUG_SISCO
static ST_BOOLEAN m_mem_debug_saved;
#endif
ST_VOID *m_bad_ptr_val;

/* Use 4 bytes (default) at the end of each buffer for overwrite detect	*/
/* Default value used will be 0xDEADBEEF			 	*/
ST_CHAR *m_pad_string;
ST_INT m_num_pad_bytes;

/* Use this to fill malloc'ed and freed memory with garbage */
ST_UCHAR m_fill_byte;
ST_BOOLEAN m_fill_en;

/* This variable used to enable MSOFT heap check calls on every alloc	*/
/* and free call.							*/
ST_BOOLEAN m_heap_check_enable;

/* This variable used to enable list validation and overwrite checking	*/
/* on every alloc  and free call. 					*/
ST_BOOLEAN m_check_list_enable;

/* Set this variable SD_FALSE to speed up the debug version. When SD_TRUE, it */
/* enables searching the memory list for the element before accessing	*/
/* the memory during chk_realloc and chk_free calls			*/
ST_BOOLEAN m_find_node_enable;

/* This variable will cause chk_realloc to not realloc when the new	*/
/* size is smaller than the old size					*/
ST_BOOLEAN m_no_realloc_smaller;

/* Dump memory table whenever high water mark is advanced */
ST_BOOLEAN m_auto_hw_log;

ST_BOOLEAN m_track_prev_free;	/* NOT USED. Only for backward compat.	*/

/* Bit masked logging control variable				 	*/
ST_UINT chk_debug_en;

#ifdef DEBUG_SISCO
SD_CONST ST_CHAR *SD_CONST _mem_log_err_logstr = "MEM_LOG_ERR";
SD_CONST ST_CHAR *SD_CONST _mem_log_calloc_logstr = "MEM_LOG_CALLOC";
SD_CONST ST_CHAR *SD_CONST _mem_log_malloc_logstr = "MEM_LOG_MALLOC";
SD_CONST ST_CHAR *SD_CONST _mem_log_realloc_logstr = "MEM_LOG_REALLOC";
SD_CONST ST_CHAR *SD_CONST _mem_log_free_logstr = "MEM_LOG_FREE";
SD_CONST ST_CHAR *SD_CONST _mem_log_nerr_logstr = "MEM_LOG_NERR";
SD_CONST ST_CHAR *SD_CONST _mem_log_dump_logstr = "MEM_LOG_DUMP";
LOGCFGX_VALUE_MAP memLogMaskMaps[] =
  {
    {"MEM_LOG_ERR",	MEM_LOG_ERR,	&chk_debug_en,	_LOGCFG_DATATYPE_UINT_MASK, "Error"},
    {"MEM_LOG_CALLOC",	MEM_LOG_CALLOC,	&chk_debug_en,	_LOGCFG_DATATYPE_UINT_MASK, "Calloc"},
    {"MEM_LOG_MALLOC",	MEM_LOG_MALLOC,	&chk_debug_en,	_LOGCFG_DATATYPE_UINT_MASK, "Malloc"},
    {"MEM_LOG_REALLOC",	MEM_LOG_REALLOC,&chk_debug_en,	_LOGCFG_DATATYPE_UINT_MASK, "Realloc"},
    {"MEM_LOG_FREE",	MEM_LOG_FREE,	&chk_debug_en,	_LOGCFG_DATATYPE_UINT_MASK, "Free"}
  };

LOGCFG_VALUE_GROUP memLogMaskMapCtrl =
  {
  {NULL,NULL},
  "",
  sizeof(memLogMaskMaps)/sizeof(LOGCFGX_VALUE_MAP),
  memLogMaskMaps
  };

LOGCFGX_VALUE_MAP memDebugMaps[] =
  {
/* Memory debugging */
    {"MemFillEnable",    	0, 			&m_fill_en, 	       		_LOGCFG_DATATYPE_BOOLEAN},
    {"MemHeapCheck",     	0, 			&m_heap_check_enable,  		_LOGCFG_DATATYPE_BOOLEAN},
    {"MemCheckList",	  	0, 			&m_check_list_enable,  		_LOGCFG_DATATYPE_BOOLEAN},
    {"MemFindNode",   		0, 			&m_find_node_enable,   		_LOGCFG_DATATYPE_BOOLEAN},
    {"MemNoReallocSmaller", 	0, 			&m_no_realloc_smaller, 		_LOGCFG_DATATYPE_BOOLEAN},

  /* Old names */
    {"CheckListEnable",  	0, 			&m_check_list_enable,  		_LOGCFG_DATATYPE_BOOLEAN},
    {"FindNodeEnable",   	0, 			&m_find_node_enable,   		_LOGCFG_DATATYPE_BOOLEAN},
    {"NoReallocSmaller", 	0, 			&m_no_realloc_smaller, 		_LOGCFG_DATATYPE_BOOLEAN}
  };

LOGCFG_VALUE_GROUP memDebugMapCtrl =
  {
  {NULL,NULL},
  "",
  sizeof(memDebugMaps)/sizeof(LOGCFGX_VALUE_MAP),
  memDebugMaps
  };
#endif /* DEBUG_SISCO */

/* Declarations for functions and array of structures used to keep track*/
/* of allocated memory.							*/
ST_UINT32 chk_curr_mem_count;
ST_UINT32 chk_max_dyn_mem_ptrs;
ST_UINT32 chk_max_mem_allocated;
ST_UINT32 chk_curr_mem_size;

/* This function pointer can be set to select a function to be called 	*/
/* in case of memory error.						*/
ST_VOID (*mem_chk_err)(ST_VOID);

/************************************************************************/
/* Internal variables							*/

/* Memory block header, used for DEBUG to track all allocated memory	*/

#if defined(DEBUG_SISCO)
typedef struct chk_dyn_mem_node
  {
  DBL_LNK chk_mem_node_link;
  ST_INT chk_alloc_mem_size;
  ST_VOID *chk_alloc_mem_ptr;
  ST_CHAR *file; 	/* File && line that allocated the memory	*/
  ST_INT line;
   ST_UINT16 overwrite_prev_detected;  /* flag to indicate that		*/
   				    /* overwrite previously detected 	*/
  SD_END_STRUCT
  } CHK_DYN_MEM_NODE;

static CHK_DYN_MEM_NODE *head_dyn_mem_list;

#endif	/* DEBUG_SISCO	*/

/* nd_* functions use CHK_DYN_MEM_NODE_NDEBUG. This saves overhead if 	*/
/* !DEBUG_SISCO or if !m_mem_debug.					*/
typedef struct chk_dyn_mem_node_ndebug
  {
  ST_INT chk_alloc_mem_size;
  SD_END_STRUCT
  } CHK_DYN_MEM_NODE_NDEBUG;

/* The 'mem_chk' tools must be initialized before being used, esp.	*/
/* in DEBUG mode, or we may write or flush a NULL stream		*/
static ST_INT mem_chk_initialized;

/************************************************************************/
/* DEBUG internal variables and functions				*/
#ifdef DEBUG_SISCO

/* padding variables							*/
static ST_UINT mem_overhead; 

static ST_RET check_overwrite (CHK_DYN_MEM_NODE *ptr);
static ST_VOID set_overwrite_value (CHK_DYN_MEM_NODE *ptr);

#endif

/************************************************************************/
/* Internal functions							*/

static ST_VOID *def_malloc_err (ST_UINT size);
static ST_VOID *def_calloc_err (ST_UINT num, ST_UINT size);  
static ST_VOID *def_realloc_err (ST_VOID *old, ST_UINT size);
static ST_VOID mem_fat_err (ST_VOID);

/************************************************************************/
/************************************************************************/
/*			def_xxx_err					*/
/* default fatal error service functions - just return			*/
/************************************************************************/

static ST_VOID *def_malloc_err (ST_UINT size)
  {
  mem_fat_err ();
  return (NULL);   
  }

static ST_VOID *def_calloc_err (ST_UINT num, ST_UINT size)
  {
  mem_fat_err ();
  return (NULL);   
  }

static ST_VOID *def_realloc_err (ST_VOID *old, ST_UINT size)
  {
  mem_fat_err ();
  return (NULL);   
  }

static ST_VOID mem_fat_err ()
  {
#ifdef DEBUG_SISCO

  if (m_mem_debug_saved)
    {
    MEMLOG_ERR0 ("Fatal Memory Allocation Error");
    MEMLOG_CERR1 ("Current number of allocated buffers  : %lu",
  		  (ST_ULONG) chk_curr_mem_count);
    MEMLOG_CERR1 ("Current amount of memory allocated   : %lu",
  		  (ST_ULONG) chk_curr_mem_size);
    MEMLOG_CERR1 ("Maximum number of pointers allocated : %lu",
  		  (ST_ULONG) chk_max_dyn_mem_ptrs);
    MEMLOG_CERR1 ("Maximum amount of memory allocated   : %lu",
  		  (ST_ULONG) chk_max_mem_allocated);
    MEMLOG_PAUSEERR ("");

    if (mem_chk_err)
      (*mem_chk_err)();

    dyn_mem_ptr_status ();
    }
#endif	/* no debug, do nothing. Can't printf because may be windowing. */

  exit (255);	   /* the default action includes exit!			*/
  }

/************************************************************************/
/* fatal error service function pointers, used for unrecoverable errors */

ST_VOID *(*m_memerr_fun)(ST_UINT size);
ST_VOID *(*c_memerr_fun)(ST_UINT num, ST_UINT size);
ST_VOID *(*r_memerr_fun)(ST_VOID *ptr, ST_UINT size);

/************************************************************************/
/*                       init_mem_chk					*/
/************************************************************************/

ST_VOID init_mem_chk ()
  {

  if (mem_chk_initialized)
    {					/* do not allow reinitialize	*/
    return;
    }

  S_LOCK_RESOURCES ();
  
#ifdef _WIN32
  //ExpInit ();
#endif

#if defined(MEMCHK_WIN32_CRT_DEBUG)
  if (m_mem_crt_debug)
    m_mem_debug = SD_TRUE;
#endif

#ifdef DEBUG_SISCO

  /* Save the current value of "m_mem_debug" to control debugging.	*/
  /* If it is changed later, it has no impact.				*/
  /* This prevents crashes caused by changing allocation mode in the	*/
  /* middle of a program.						*/

  m_mem_debug_saved = m_mem_debug;

  chk_debug_en |= MEM_LOG_ERR;

  if (m_bad_ptr_val == NULL)
    m_bad_ptr_val = (ST_VOID *) -1;
  if (m_pad_string == NULL)
    m_pad_string = "\336\255\276\357"; /* 0xDEADBEEF */
  if (m_num_pad_bytes == 0)
    m_num_pad_bytes = 4;
  if (m_fill_byte == 0)
    m_fill_byte = (ST_UCHAR) 0xCC;

/* Calculate the debug memory overhead					*/
  mem_overhead = sizeof (CHK_DYN_MEM_NODE) + m_num_pad_bytes; 
  
#if defined(MEMCHK_WIN32_CRT_DEBUG)
/* hook a debug report function */

  _CrtSetReportHook (mem_report);

  {
  int tmpFlag = _CrtSetDbgFlag (_CRTDBG_REPORT_FLAG);
  tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
    
  if (m_heap_check_enable)
    tmpFlag |= _CRTDBG_CHECK_ALWAYS_DF;
    
/* Set the new state for the flag */
  _CrtSetDbgFlag (tmpFlag);
  }
  
#endif

#endif

  if (!m_memerr_fun)
    m_memerr_fun = def_malloc_err; 
  if (!c_memerr_fun)
    c_memerr_fun = def_calloc_err;
  if (!r_memerr_fun)
    r_memerr_fun = def_realloc_err;
  mem_chk_initialized = SD_TRUE;
  S_UNLOCK_RESOURCES ();
  }
  
#if defined(MEMCHK_WIN32_CRT_DEBUG)
/************************************************************************/
/*			mem_report					*/
/* Called by CRT Library to report a memory condition.			*/
/************************************************************************/

int mem_report (int reportType, char *userMessage, int *retVal)
  {
static int   fileCreated = SD_FALSE;
static FILE *fp = NULL;

  if (fp)
    {
    /* if file already open that means that we are called again		*/
    /* from the fprintf() below,					*/
    /* exit to prevent stack overflow in fprintf when _HEAPBADNODE err	*/
    *retVal = 0;
    return FALSE;
    }

  if (!fileCreated)
    {
    fp = fopen ("MemReport.log", "w+");
    fileCreated = SD_TRUE;
    }
  else  
    fp = fopen ("MemReport.log", "a+");

  fprintf (fp, "%s", userMessage);

  fclose (fp);
  fp = NULL;
    
  *retVal = 0;
  return FALSE;
  }

#endif


/************************************************************************/
/*			x_chk_malloc					*/
/* get memory via malloc, check for error condition			*/
/* if error, invoke the fatal error function via pointer		*/
/************************************************************************/

#if defined(DEBUG_SISCO)
ST_VOID *x_chk_malloc  (ST_UINT size,  
			SD_CONST ST_CHAR *SD_CONST calling_file, 
			SD_CONST ST_INT calling_line)
  {
CHK_DYN_MEM_NODE *new_mem_node;

/* Verify mem_check tools have been initialized, do it if not		*/
  if (!mem_chk_initialized)
    init_mem_chk ();

  if (m_mem_debug_saved)
    {
    S_LOCK_RESOURCES ();

/* Do linked list && overwrite checks, if enabled			*/
    if (m_check_list_enable)
      check_mem_list ();	

/* Perform heap validation, if enabled and the system supports it 	*/
#if defined(DEBUG_SISCO) && defined (MSOFT_HEAP_CHECK) 
    if (m_heap_check_enable)
      msoft_heap_check (0); 		/* do not print the heap!	*/
#endif

/* It is not OK to malloc a buffer of size 0 - check for this		*/
    if (!size)
      {
      MEMLOG_ERR0 ("Attempting to malloc with size == 0");
      MEMLOG_CALLSTACK (NULL);
      if (mem_chk_err)
        (*mem_chk_err)();
      size++;
      }

/* Allocate the buffer using the standard 'malloc' call			*/

#if defined(MEMCHK_WIN32_CRT_DEBUG)
    if (m_mem_crt_debug)
      new_mem_node = (CHK_DYN_MEM_NODE *) _malloc_dbg (size + mem_overhead, _NORMAL_BLOCK, 
      				calling_file, calling_line);
    else
      new_mem_node = (CHK_DYN_MEM_NODE *) malloc (size + mem_overhead);
#else    
    new_mem_node = (CHK_DYN_MEM_NODE *) malloc (size + mem_overhead);
#endif

    if (new_mem_node == NULL)
      {  		/* see if we can call a function to get some memory back*/
      if (!(new_mem_node = (CHK_DYN_MEM_NODE *) 
				(*m_memerr_fun)(size + mem_overhead)))
        {
        MEMLOG_ERR0 ("WARNING : Returning NULL Pointer");
        MEMLOG_CALLSTACK (NULL);
        if (mem_chk_err)
          (*mem_chk_err)();

        S_UNLOCK_RESOURCES ();
        return (NULL);	/* if we can't get any back we return a NULL	*/
        }
      }

/* Set up the memory control block header 				*/
    new_mem_node -> chk_alloc_mem_size = size;
    new_mem_node -> chk_alloc_mem_ptr = (ST_VOID *)(new_mem_node+1);

/* Fill memory with garbage */
    if (m_fill_en)
      memset (new_mem_node -> chk_alloc_mem_ptr, m_fill_byte, size);

/* Save file and line numbers						*/
    new_mem_node->file = (ST_CHAR *)calling_file;
    new_mem_node->line = calling_line;

/* Set the overwrite detect value at the end of the buffer		*/
    set_overwrite_value (new_mem_node);

/* Now add this element to the allocation control list 			*/
    list_add_first ((ST_VOID **)&head_dyn_mem_list, new_mem_node);     

/* Update memory allocation statistics parameters			*/
    chk_curr_mem_count++;
    chk_curr_mem_size += size;
    if (chk_curr_mem_count > chk_max_dyn_mem_ptrs)
      chk_max_dyn_mem_ptrs = chk_curr_mem_count;
    if (chk_curr_mem_size > chk_max_mem_allocated)
      {
      chk_max_mem_allocated = chk_curr_mem_size;
      if (m_auto_hw_log)
        dyn_mem_ptr_status ();
      }
/* log allocation info, if enabled					*/
    MEMLOG_MALLOC2 ("chk_malloc : size = %05u, ptr = %08lx"
                                       ,size, (ST_VOID *)(new_mem_node+1));
    if (calling_line != -1)
      {
      MEMLOG_CMALLOC2 ("File : %s, Line %d",calling_file, calling_line);
      }

/* return the user pointer						*/
    S_UNLOCK_RESOURCES ();
    return ((ST_VOID *)(new_mem_node+1));
    }
  else	/* !m_mem_debug_saved	*/
    return (_nd_chk_malloc (size));
  }
#endif	/* DEBUG_SISCO	*/

/************************************************************************/
/*			nd_chk_malloc					*/
/* This function is called if (!DEBUG_SISCO) OR (!m_mem_debug_saved).	*/
/************************************************************************/
#ifdef DEBUG_SISCO	/* Use "static" for DEBUG, so user can't call.	*/
static ST_VOID *_nd_chk_malloc (ST_UINT size)
#else
ST_VOID *nd_chk_malloc (ST_UINT size)
#endif
  {
CHK_DYN_MEM_NODE_NDEBUG *new_mem_node;

/* Verify mem_check tools have been initialized, do it if not		*/
  if (!mem_chk_initialized)
    init_mem_chk ();

/* Update memory allocation statistics parameters			*/
  chk_curr_mem_count++;
  if (chk_curr_mem_count > chk_max_dyn_mem_ptrs)
    chk_max_dyn_mem_ptrs = chk_curr_mem_count;

  new_mem_node = 
    (CHK_DYN_MEM_NODE_NDEBUG *) malloc (size + sizeof(CHK_DYN_MEM_NODE_NDEBUG));
  if (new_mem_node == NULL)
    {				/* malloc failed. Try err_fun to get mem*/
    new_mem_node = (CHK_DYN_MEM_NODE_NDEBUG *)
        (*m_memerr_fun)(size + sizeof(CHK_DYN_MEM_NODE_NDEBUG));
    if (!new_mem_node)
      {
      return (NULL);	/* can't get any back, so return NULL	*/
      }
    }
  new_mem_node -> chk_alloc_mem_size = size;
  return ((ST_VOID *)(new_mem_node+1));
  }

/************************************************************************/
/*			x_chk_calloc					*/
/* get memory via calloc, check for error condition			*/
/* if error, invoke the fatal error function via pointer		*/
/************************************************************************/

#if defined(DEBUG_SISCO)
ST_VOID *x_chk_calloc  (ST_UINT num, 
			ST_UINT size,  
			SD_CONST ST_CHAR *SD_CONST calling_file, 
			SD_CONST ST_INT calling_line)
  {
CHK_DYN_MEM_NODE *new_mem_node;
ST_INT	buf_size;

/* Verify mem_check tools have been initialized, do it if not		*/
  if (!mem_chk_initialized)
    init_mem_chk ();

  if (m_mem_debug_saved)
    {
    S_LOCK_RESOURCES ();

/* Do linked list && overwrite checks, if enabled			*/
    if (m_check_list_enable)
      check_mem_list ();	

/* It is not OK to calloc a buffer of size 0 - check for this		*/
    if (!size)
      {
      MEMLOG_ERR0 ("Attempting to calloc with size == 0");
      MEMLOG_CALLSTACK (NULL);
      if (mem_chk_err)
        (*mem_chk_err)();
      size++;
      }
    if (!num)
      {
      MEMLOG_ERR0 ("Attempting to calloc with num == 0");
      MEMLOG_CALLSTACK (NULL);
      if (mem_chk_err)
        (*mem_chk_err)();
      num++;
      }

/* Calculate the size required ... Save a multiply? 		*/
    if (num == 1)
      buf_size = size;
    else if (size == 1)
      buf_size = num;
    else
      buf_size = num * size; 

/* get the buffer							*/
#if defined(MEMCHK_WIN32_CRT_DEBUG)
    if (m_mem_crt_debug)
      new_mem_node = (CHK_DYN_MEM_NODE *) _calloc_dbg (1, buf_size + mem_overhead, _NORMAL_BLOCK,
      			calling_file, calling_line);
    else
      new_mem_node = (CHK_DYN_MEM_NODE *) calloc (1, buf_size + mem_overhead);
#else
    new_mem_node = (CHK_DYN_MEM_NODE *) calloc (1, buf_size + mem_overhead);
#endif    

/* verify NULL not returned						*/
    if (!new_mem_node)
      {
      MEMLOG_ERR0 ("WARNING : Returning NULL Pointer");
      MEMLOG_CERR5 ("Parameters: num:%d size: %d file:%s line:%d %s", 
      			num, size, calling_file, calling_line, "");
      MEMLOG_CALLSTACK (NULL);
      if (mem_chk_err)
        (*mem_chk_err)();
      S_UNLOCK_RESOURCES (); 
      return (NULL);
      }

/* Set up the memory control block header 				*/
    new_mem_node -> chk_alloc_mem_size = buf_size;
    new_mem_node -> chk_alloc_mem_ptr = (ST_VOID *)(new_mem_node+1);

/* Save file and line numbers						*/
    new_mem_node->file = (ST_CHAR *)calling_file;
    new_mem_node->line = calling_line;

/* Set the overwrite detect value at the end of the buffer		*/
    set_overwrite_value (new_mem_node);

/* Now add this element to the allocation control list 			*/
    list_add_first ((ST_VOID **)&head_dyn_mem_list, new_mem_node);     

/* Update memory allocation statistics parameters			*/
    chk_curr_mem_count++;
    chk_curr_mem_size += buf_size;
    if (chk_curr_mem_count > chk_max_dyn_mem_ptrs)
      chk_max_dyn_mem_ptrs = chk_curr_mem_count;
    if (chk_curr_mem_size > chk_max_mem_allocated)
      {
      chk_max_mem_allocated = chk_curr_mem_size;
      if (m_auto_hw_log)
        dyn_mem_ptr_status ();
      }

/* log allocation info, if enabled					*/
    MEMLOG_CALLOC3 ("chk_calloc : num = %05u, size = %05u Ptr = %08lx",num,size,new_mem_node+1);
    if (calling_line != -1)
      {
      MEMLOG_CCALLOC2 ("File : %s, Line %d",calling_file, calling_line);
      }

/* return the user pointer						*/
    S_UNLOCK_RESOURCES (); 
    return ((ST_VOID *)(new_mem_node+1));
    }
  else	/* !m_mem_debug_saved	*/
    return (_nd_chk_calloc (num, size));
  }
#endif	/* DEBUG_SISCO	*/

/************************************************************************/
/*			nd_chk_calloc					*/
/* This function is called if (!DEBUG_SISCO) OR (!m_mem_debug_saved).	*/
/************************************************************************/
#ifdef DEBUG_SISCO	/* Use "static" for DEBUG, so user can't call.	*/
static ST_VOID *_nd_chk_calloc (ST_UINT num, ST_UINT size)
#else
ST_VOID *nd_chk_calloc (ST_UINT num, ST_UINT size)
#endif
  {
CHK_DYN_MEM_NODE_NDEBUG *new_mem_node;
ST_INT	buf_size;

/* Verify mem_check tools have been initialized, do it if not		*/
  if (!mem_chk_initialized)
    init_mem_chk ();

/* Update memory allocation statistics parameters			*/
  chk_curr_mem_count++;
  if (chk_curr_mem_count > chk_max_dyn_mem_ptrs)
    chk_max_dyn_mem_ptrs = chk_curr_mem_count;

  buf_size = num * size;      		/* chk_malloc adds overhead	*/
  new_mem_node = 
    (CHK_DYN_MEM_NODE_NDEBUG *) calloc (1, buf_size + sizeof(CHK_DYN_MEM_NODE_NDEBUG));
  if (new_mem_node == NULL)
    {				/* calloc failed. Try err_fun to get mem*/
    new_mem_node = (CHK_DYN_MEM_NODE_NDEBUG *)
        (*c_memerr_fun)(1, buf_size + sizeof(CHK_DYN_MEM_NODE_NDEBUG));
    if (!new_mem_node)
      {
      return (NULL);	/* can't get any back, so return NULL	*/
      }
    }
  new_mem_node -> chk_alloc_mem_size = buf_size;
  return ((ST_VOID *)(new_mem_node+1));
  }

/************************************************************************/
/*			x_chk_realloc					*/
/* get memory via calloc, check for error condition			*/
/* if error, invoke the fatal error function via pointer		*/
/************************************************************************/

#if defined(DEBUG_SISCO)
ST_VOID *x_chk_realloc (ST_VOID *ptr, 
			ST_UINT size, 
			SD_CONST ST_CHAR *SD_CONST calling_file, 
			SD_CONST ST_INT calling_line)
  {
CHK_DYN_MEM_NODE *old_mem_node, *new_mem_node;
ST_VOID *old_mem_ptr;
ST_UINT old_mem_size;
ST_BOOLEAN no_realloc;

/* Verify mem_check tools have been initialized, do it if not		*/
  if (!mem_chk_initialized)
    init_mem_chk ();

  if (m_mem_debug_saved)
    {
    S_LOCK_RESOURCES ();
/* Do linked list && overwrite checks, if enabled			*/
    if (m_check_list_enable)
      check_mem_list ();	

/* Perform heap validation, if enabled and the system supports it 	*/
#if defined(DEBUG_SISCO) && defined (MSOFT_HEAP_CHECK) 
    if (m_heap_check_enable)
      msoft_heap_check (0); 		/* do not print the heap!	*/
#endif

/* It is not OK to pass in a NULL pointer, check for this		*/
    if (!ptr)
      {
      MEMLOG_ERR2 ("File : %s, Line %d",
                                      calling_file, calling_line);
      MEMLOG_CERR0 ("Attempting to realloc NULL pointer");
      MEMLOG_CALLSTACK (NULL);
      MEMLOG_PAUSEERR ("");
      if (mem_chk_err)
        (*mem_chk_err)();
      S_UNLOCK_RESOURCES (); 
      return (NULL);
      }

/* It is not OK to re-alloc a buffer of size 0 - check for this		*/
    if (!size)
      {
      MEMLOG_ERR0 ( "Attempting to realloc with size = 0");
      MEMLOG_CALLSTACK (NULL);
      if (mem_chk_err)
        (*mem_chk_err)();
      size++;
      }

/* Get the pointer to the control information				*/
    old_mem_node = ((CHK_DYN_MEM_NODE *)ptr)-1;

/* Verify that this node is on the list before accessing it		*/
    if (m_find_node_enable == SD_TRUE)
      {
      if (list_find_node (head_dyn_mem_list, old_mem_node) != SD_SUCCESS)
        {
        goto MEM_REALLOC_ERROR;
        }
      }

/* extract information from memory block header				*/
    old_mem_ptr = old_mem_node -> chk_alloc_mem_ptr;
    old_mem_size = old_mem_node -> chk_alloc_mem_size;

    if (old_mem_ptr != ptr) /* verify that the ptr passed in matches 	*/
      {
MEM_REALLOC_ERROR :
      MEMLOG_ERR2 ("\n File : %s, Line %d",
                                      calling_file, calling_line);
      MEMLOG_CERR1 (
                 "Invalid realloc (%08lx)",ptr);

      MEMLOG_CALLSTACK (NULL);
      MEMLOG_PAUSEERR ("");
      if (mem_chk_err)
        (*mem_chk_err)();
      S_UNLOCK_RESOURCES (); 
      return (ptr);	/* just hope realloc was for smaller		*/
      }

/* Update memory allocation statistics parameters			*/
    chk_curr_mem_count--;				/* update the node count*/
    chk_curr_mem_size -= old_mem_size;		/* update the mem size	*/


/* Check for 'no realloc smaller' option				*/
/* This is controlled either by compile time define or by variable	*/
/* If new size is less than old size, just don't use the system realloc	*/

    no_realloc = SD_FALSE;

#if defined(NO_REALLOC_SMALLER)
    if (size <= (ST_UINT) old_mem_node -> chk_alloc_mem_size)
      {
      no_realloc = SD_TRUE;
      }
#else
    if (m_no_realloc_smaller)
      {
      if (size <= (ST_UINT) old_mem_node -> chk_alloc_mem_size)
        {
        no_realloc = SD_TRUE;
        }  
      }  
#endif

    if (no_realloc)
      new_mem_node = old_mem_node;	/* Use original buffer.		*/
    else
      {
/* invalidate the old block						*/
      old_mem_node->chk_alloc_mem_ptr = NULL;    

/* remove the old memory block from the mem_chk system	 		*/
      list_unlink ((ST_VOID **)&head_dyn_mem_list, old_mem_node); /* unlink the old node	*/

/* Use the system realloc to get the new pointer			*/
#if defined(MEMCHK_WIN32_CRT_DEBUG)
      if (m_mem_crt_debug)
        new_mem_node = (CHK_DYN_MEM_NODE *) 
              _realloc_dbg ((ST_VOID *) old_mem_node, size + mem_overhead, _NORMAL_BLOCK,
      			calling_file, calling_line);
      else
        new_mem_node = (CHK_DYN_MEM_NODE *) 
              realloc ((ST_VOID *) old_mem_node, size + mem_overhead);
#else
      new_mem_node = (CHK_DYN_MEM_NODE *) 
              realloc ((ST_VOID *) old_mem_node, size + mem_overhead);
#endif    

      if (new_mem_node == NULL)
        {
        if (!(new_mem_node = (CHK_DYN_MEM_NODE *)
			(*r_memerr_fun)((ST_VOID *) old_mem_node,
                                              size + mem_overhead)))
          {
          MEMLOG_ERR0 ("WARNING : Returning NULL Pointer");
          MEMLOG_CALLSTACK (NULL);
          if (mem_chk_err)
            (*mem_chk_err)();
          S_UNLOCK_RESOURCES (); 
          return( NULL );	/* if we can't get any back we return a NULL	*/
          }
        }

/* Now add this element to the allocation control list 			*/
      list_add_first ((ST_VOID **)&head_dyn_mem_list, new_mem_node);
      }

/* Now we have a new memory block, initialize the mem_chk header and	*/
/* put on the active list						*/
    new_mem_node -> chk_alloc_mem_size = size;/* update the header	*/
    new_mem_node -> chk_alloc_mem_ptr = (ST_VOID *)(new_mem_node+1);

    new_mem_node->file = (ST_CHAR *)calling_file;
    new_mem_node->line = calling_line;

/* Initialize the overwrite value					*/
    set_overwrite_value (new_mem_node);

/* Update memory allocation statistics parameters			*/
    chk_curr_mem_size += size;		/* add the new size	*/
    chk_curr_mem_count++;			/* update the node count*/
    if (chk_curr_mem_size > chk_max_mem_allocated)
      {
      chk_max_mem_allocated = chk_curr_mem_size;
      if (m_auto_hw_log)
        dyn_mem_ptr_status ();
      }
/* log allocation info, if enabled					*/
    MEMLOG_REALLOC3 ("chk_realloc : ptr = %08lx, size = %05u, ptr = %08lx",
					  ptr,size, (ST_VOID *)(new_mem_node+1));
    if (calling_line != -1)
      {
      MEMLOG_CREALLOC2 ("File : %s. Line %d",
			      calling_file,
			      calling_line);
      }
    S_UNLOCK_RESOURCES (); 
    return ((ST_VOID *)(new_mem_node+1));
    }
  else	/* !m_mem_debug_saved	*/
    return (_nd_chk_realloc (ptr, size));
  }
#endif	/* DEBUG_SISCO	*/

/************************************************************************/
/*			nd_chk_realloc					*/
/* This function is called if (!DEBUG_SISCO) OR (!m_mem_debug_saved).	*/
/************************************************************************/
#ifdef DEBUG_SISCO	/* Use "static" for DEBUG, so user can't call.	*/
static ST_VOID *_nd_chk_realloc (ST_VOID *ptr, ST_UINT size)
#else
ST_VOID *nd_chk_realloc (ST_VOID *ptr, ST_UINT size)
#endif
  {
CHK_DYN_MEM_NODE_NDEBUG *old_mem_node;
CHK_DYN_MEM_NODE_NDEBUG *new_mem_node;
ST_BOOLEAN no_realloc;

/* Verify mem_check tools have been initialized, do it if not		*/
  if (!mem_chk_initialized)
    init_mem_chk ();

/* Get the pointer to the control information				*/
  old_mem_node = ((CHK_DYN_MEM_NODE_NDEBUG *)ptr)-1;

/* Check for 'no realloc smaller' option				*/
/* This is controlled either by compile time define or by variable	*/
/* If new size is less than old size, just don't use the system realloc	*/

  no_realloc = SD_FALSE;

#if defined(NO_REALLOC_SMALLER)
  if (size <= (ST_UINT) old_mem_node -> chk_alloc_mem_size)
    {
    no_realloc = SD_TRUE;
    }
#else
  if (m_no_realloc_smaller)
    {
    if (size <= (ST_UINT) old_mem_node -> chk_alloc_mem_size)
      {
      no_realloc = SD_TRUE;
      }  
    }  
#endif

  if (no_realloc)
    new_mem_node = old_mem_node;	/* Use original buffer.		*/
  else
    {
/* Use the system realloc to get the new pointer			*/
    if ((new_mem_node = (CHK_DYN_MEM_NODE_NDEBUG *) 
            realloc ((ST_VOID *) old_mem_node, 
                  size + sizeof (CHK_DYN_MEM_NODE_NDEBUG))) == NULL)
      {
      if (!(new_mem_node = 
	    (CHK_DYN_MEM_NODE_NDEBUG *) (*r_memerr_fun)((ST_VOID *) old_mem_node,
                                size + sizeof (CHK_DYN_MEM_NODE_NDEBUG))))
        {
        return( NULL );	/* if we can't get any back we return a NULL	*/
        }
      }
    }

  new_mem_node -> chk_alloc_mem_size = size; /* update the header	*/
  return ((ST_VOID *)(new_mem_node+1));
  }

/************************************************************************/
/*			x_chk_free					*/
/************************************************************************/

#if defined(DEBUG_SISCO)
ST_VOID  x_chk_free    (ST_VOID *del_mem_ptr, 
			SD_CONST ST_CHAR *SD_CONST calling_file, 
			SD_CONST ST_INT calling_line)
  {
CHK_DYN_MEM_NODE *chk_mem_node;

/* Verify mem_check tools have been initialized, do it if not		*/
  if (!mem_chk_initialized)
    init_mem_chk ();

  if (m_mem_debug_saved)
    {
    S_LOCK_RESOURCES ();

/* Do linked list && overwrite checks, if enabled			*/
    if (m_check_list_enable)
      check_mem_list ();	/* check linked list. Works for all systems.	*/

/* Perform heap validation, if enabled and the system supports it 	*/
#if defined(DEBUG_SISCO) && defined (MSOFT_HEAP_CHECK) 
    if (m_heap_check_enable)
      msoft_heap_check (0); 		/* do not print the heap!	*/
#endif

/* log allocation info, if enabled					*/
    MEMLOG_FREE1 ("chk_free   : ptr = %08lx",del_mem_ptr);

    if (calling_line != -1)
      {
      MEMLOG_CFREE2 ("File : %s. Line %d",
			      calling_file,
			      calling_line);
      }

/* verify that a non-NULL pointer was passed in				*/
    if (!del_mem_ptr)
      {
      goto MEM_FREE_ERROR; 
      }

/* Get the pointer to the control information				*/
    chk_mem_node = ((CHK_DYN_MEM_NODE *)del_mem_ptr)-1;

/* Verify that this node is on the list before accessing it		*/
    if (m_find_node_enable == SD_TRUE)
      {
      if (list_find_node (head_dyn_mem_list, chk_mem_node) != SD_SUCCESS)
        {
        goto MEM_FREE_ERROR;
        }
      }

/* verify that the pointer matches the expected pointer			*/
    if (chk_mem_node->chk_alloc_mem_ptr == del_mem_ptr)
      {
      if (check_overwrite (chk_mem_node))
        {
        MEMLOG_ERR0 ("Memory overwrite at end of buffer detected");
        MEMLOG_CERR5 (
          "Node = %lx, Prev = %lx, Next = %lx, Len = %u, Curr = %lx",
                 chk_mem_node, 
                 chk_mem_node->chk_mem_node_link.prev, 
                 chk_mem_node->chk_mem_node_link.next,
    	   chk_mem_node->chk_alloc_mem_size, 
    	   chk_mem_node->chk_alloc_mem_ptr);
        MEMLOG_CHERR (chk_mem_node->chk_alloc_mem_size + 
		      sizeof (CHK_DYN_MEM_NODE) +
		      m_num_pad_bytes, chk_mem_node);

/* Take care of this memory block					*/
        chk_mem_node->chk_alloc_mem_ptr = NULL;    /* invalidate the block */
        chk_curr_mem_size -= chk_mem_node -> chk_alloc_mem_size;
        chk_curr_mem_count--;
        list_unlink ((ST_VOID **)&head_dyn_mem_list, chk_mem_node);

        MEMLOG_CERR2 ("Pointer was allocated at file: %s, line %d ",
	   		  chk_mem_node->file,chk_mem_node->line);
        goto MEM_FREE_ERROR2; 	/* print the call stack			*/
        }
      else	/* This is a good free, do it!				*/
        {
        chk_mem_node->chk_alloc_mem_ptr = NULL;    /* invalidate the block	*/
        chk_curr_mem_size -= chk_mem_node -> chk_alloc_mem_size;
        chk_curr_mem_count--;
        list_unlink ((ST_VOID **)&head_dyn_mem_list, chk_mem_node);

/* Fill memory with garbage before freeing it */
#ifdef MEM_FILL_CONTROL
        if (m_fill_en)
          memset (chk_mem_node, m_fill_byte, 
	          chk_mem_node->chk_alloc_mem_size + 
		  sizeof (CHK_DYN_MEM_NODE));
#else
        if (m_fill_en)
          memset ((ST_CHAR *) chk_mem_node + sizeof (CHK_DYN_MEM_NODE), 
	          m_fill_byte, 
	          chk_mem_node->chk_alloc_mem_size);
#endif

#if defined(MEMCHK_WIN32_CRT_DEBUG)
        if (m_mem_crt_debug)
          _free_dbg ((ST_VOID *)chk_mem_node, _NORMAL_BLOCK);
	else
          free ((ST_VOID *)chk_mem_node);
#else
        free ((ST_VOID *)chk_mem_node);
#endif	
        }
      }  /* end if pointer matched expected */
    else		/* Whoops, free error					*/
      {
      if (chk_debug_en & MEM_LOG_ERR)
        {
MEM_FREE_ERROR :

        MEMLOG_ERR1 (
              "Invalid Free (%08lx)",del_mem_ptr);

MEM_FREE_ERROR2 :
        MEMLOG_CERR2 ("Free call made from file : %s, Line %d ",
                                        calling_file,
                                        calling_line);
        }
      MEMLOG_CALLSTACK (NULL);
      MEMLOG_PAUSEERR ("");
      if (mem_chk_err)
        (*mem_chk_err)();
      }

/* Do linked list && overwrite checks, if enabled			*/
    if (m_check_list_enable)
      check_mem_list ();	/* check linked list. Works for all systems.	*/

/* Perform heap validation, if enabled and the system supports it 	*/
#if defined(DEBUG_SISCO) && defined (MSOFT_HEAP_CHECK) 
    if (m_heap_check_enable)
      msoft_heap_check (0); 		/* do not print the heap!	*/
#endif
    S_UNLOCK_RESOURCES ();
    }
  else	/* !m_mem_debug_saved	*/
    _nd_chk_free (del_mem_ptr);
  }
#endif	/* DEBUG_SISCO	*/


/************************************************************************/
/*			nd_chk_free					*/
/* This function is called if (!DEBUG_SISCO) OR (!m_mem_debug_saved).	*/
/************************************************************************/
#ifdef DEBUG_SISCO	/* Use "static" for DEBUG, so user can't call.	*/
static ST_VOID _nd_chk_free (ST_VOID *del_mem_ptr)
#else
ST_VOID nd_chk_free (ST_VOID *del_mem_ptr)
#endif
  {
/* Verify mem_check tools have been initialized, do it if not		*/
  if (!mem_chk_initialized)
    init_mem_chk ();

/* Update memory allocation statistics parameters			*/
  chk_curr_mem_count--;
    /* Extra () helps some "free" macros work.	*/
  free ((((CHK_DYN_MEM_NODE_NDEBUG *)del_mem_ptr)-1));
  }

#if defined(DEBUG_SISCO)
/************************************************************************/
/*                     x_chk_strdup					*/
/************************************************************************/

ST_CHAR *x_chk_strdup  (ST_CHAR *str, 
			SD_CONST ST_CHAR *SD_CONST file, 
			SD_CONST ST_INT line)
  {
ST_CHAR *new_str;

  new_str = x_chk_malloc (strlen (str) + 1, file, line);
  strcpy (new_str, str);
  return (new_str);
  }

#else	/* !DEBUG_SISCO	*/
/************************************************************************/
/*			nd_chk_strdup					*/
/************************************************************************/

ST_CHAR *nd_chk_strdup (ST_CHAR *str)
  {
ST_CHAR *new_str;

  new_str = nd_chk_malloc (strlen (str) + 1);
  strcpy (new_str, str);
  return (new_str);
  }
#endif	/* !DEBUG_SISCO	*/

/***************************************************************************/
/***************************************************************************/
/*                        dyn_mem_ptr_status				   */
/***************************************************************************/

ST_VOID dyn_mem_ptr_status ()
  {
#ifdef DEBUG_SISCO
  dyn_mem_ptr_status2 (NULL);
#endif
  }

/***************************************************************************/
/*                        dyn_mem_ptr_status2				   */
/* This function will log all nodes up to marker_ptr node and return       */
/* count of logged nodes. If market_ptr is NULL the function will log all  */
/* memory nodes.							   */
/***************************************************************************/

ST_UINT dyn_mem_ptr_status2 (ST_VOID *marker_ptr)
  {
  ST_UINT logged_count = 0;
#ifdef DEBUG_SISCO
  CHK_DYN_MEM_NODE *chk_mem_node;
  ST_UINT32 count = 1;
  ST_INT line  = 1;

  if (m_mem_debug_saved)
    {
    S_LOCK_RESOURCES (); 
    chk_mem_node = head_dyn_mem_list;
    MEMLOG_LF ();
    if (marker_ptr)
      {
      MEMLOG_DUMP0 ("DYNAMIC MEMORY POINTER TABLE ABOVE MARKER");
      MEMLOG_CDUMP0 ("-----------------------------------------  ");
      }
    else
      {
      MEMLOG_DUMP0 ("DYNAMIC MEMORY POINTER TABLE");
      MEMLOG_CDUMP0 ("----------------------------  ");
      }
    while (count <= chk_curr_mem_count)
      {
      if (chk_mem_node == NULL)
        {
        MEMLOG_CDUMP0 ("Dump halted due to NULL pointer. Table corrupted.");
        break;		/* Break out of loop.		*/
        }
      if (marker_ptr && chk_mem_node->chk_alloc_mem_ptr == marker_ptr)
        break;
      ++logged_count;
#if defined(MEMCHK_WIN32_CRT_DEBUG)
      {
      int req_num = -1;
      _CrtIsMemoryBlock(chk_mem_node,
                        chk_mem_node->chk_alloc_mem_size + mem_overhead, 
			&req_num, NULL, NULL);
      MEMLOG_CDUMP5 ("Ptr %08lx  Size % 5u  File : %-12s #%04d   {%04d}",
		              chk_mem_node -> chk_alloc_mem_ptr,
		              chk_mem_node -> chk_alloc_mem_size,
			      chk_mem_node->file,
    			      chk_mem_node->line,
    			      req_num);
      }
#else
      MEMLOG_CDUMP4 ("Ptr %08lx  Size % 5u  File : %-12s #%04d",
		              chk_mem_node -> chk_alloc_mem_ptr,
		              chk_mem_node -> chk_alloc_mem_size,
			      chk_mem_node->file,
    			      chk_mem_node->line);
#endif
	
      chk_mem_node = (CHK_DYN_MEM_NODE *)
                      chk_mem_node -> chk_mem_node_link.next;
      count++;

      line++;
      if (line > 10)
        {
        MEMLOG_PAUSE ("---- More Follows Press any key ----");
        line = 1;
        }
      }
 
    MEMLOG_CDUMP1 ("Total number of allocated pointers:   %lu",
  		  (ST_ULONG) chk_curr_mem_count);
    MEMLOG_CDUMP1 ("Total amount of memory allocated:     %lu bytes ",
  		  (ST_ULONG) chk_curr_mem_size);
    MEMLOG_CDUMP1 ("Maximum number of pointers allocated: %lu",
  		  (ST_ULONG) chk_max_dyn_mem_ptrs);
    MEMLOG_CDUMP1 ("Maximum amount of memory allocated:   %lu bytes",
  		  (ST_ULONG) chk_max_mem_allocated);
    MEMLOG_PAUSE ("");
    S_UNLOCK_RESOURCES (); 
    }
#endif			/* end of DEBUG_SISCO					   */
  return (logged_count);
  }

/***************************************************************************/
/*                        dyn_mem_ptr_statistics			   */
/***************************************************************************/

ST_VOID dyn_mem_ptr_statistics (ST_BOOLEAN log_to_screen)
  {
#ifdef DEBUG_SISCO
  CHK_DYN_MEM_NODE *chk_mem_node;
  ST_UINT32 count = 1;

  if (m_mem_debug_saved)
    {
    S_LOCK_RESOURCES (); 
    chk_mem_node = head_dyn_mem_list;
    if ( log_to_screen == SD_TRUE )
      {
#ifndef _WINDOWS
      printf("\nDYNAMIC MEMORY POINTER TABLE ");
      printf("\n-----------------------------  ");
#endif
      }
    else
      {
      MEMLOG_LF ();
      MEMLOG_DUMP0 ("DYNAMIC MEMORY POINTER TABLE ");
      MEMLOG_CDUMP0 ("-----------------------------  ");
      }
    while (count <= chk_curr_mem_count)
      {
      chk_mem_node = (CHK_DYN_MEM_NODE *)
                      chk_mem_node -> chk_mem_node_link.next;
      count++;
      }
 
    if ( log_to_screen == SD_TRUE )
      {
#if !defined(_WINDOWS) || defined(_WIN32)
      printf ("\nTotal number of allocated pointers:   %lu",
             (ST_ULONG) chk_curr_mem_count);
      printf ("\nTotal amount of memory allocated:     %lu bytes ",
             (ST_ULONG) chk_curr_mem_size);
      printf ("\nMaximum number of pointers allocated: %lu",
             (ST_ULONG) chk_max_dyn_mem_ptrs);
      printf ("\nMaximum amount of memory allocated:   %lu bytes",
             (ST_ULONG) chk_max_mem_allocated);
#endif
      }
    else
      {
      MEMLOG_CDUMP1 ("Total number of allocated pointers:   %lu",
                    (ST_ULONG) chk_curr_mem_count);
      MEMLOG_CDUMP1 ("Total amount of memory allocated:     %lu bytes ",
                    (ST_ULONG) chk_curr_mem_size);
      MEMLOG_CDUMP1 ("Maximum number of pointers allocated: %lu",
                    (ST_ULONG) chk_max_dyn_mem_ptrs);
      MEMLOG_CDUMP1 ("Maximum amount of memory allocated:   %lu bytes",
                    (ST_ULONG) chk_max_mem_allocated);
      MEMLOG_PAUSE ("");
      }
    S_UNLOCK_RESOURCES (); 
    return;
    }
#endif			/* end of DEBUG_SISCO					   */
  }

/************************************************************************/
/************************************************************************/
/* 			chk_hp						*/
/* Perform system specific heap validation 				*/
/************************************************************************/

ST_RET chk_hp (ST_INT16 prt_mask, ST_CHAR *file, ST_INT line)
  {
ST_RET ret;

  ret = SD_SUCCESS;

#if defined(DEBUG_SISCO) && defined (MSOFT_HEAP_CHECK) 
  S_LOCK_RESOURCES (); 
  if (ret = msoft_heap_check (prt_mask))
    {
    MEMLOG_ERR2 ("\nFILE : %s, LINE %d",file,line);
    if (mem_chk_err)
      (*mem_chk_err)();
    }
  S_UNLOCK_RESOURCES (); 
#endif

  return (ret);
  }

/************************************************************************/
/* 			msoft_heap_check				*/
/* Perform Microsoft specific heap validation 				*/
/* Also sets all unused heap to 0x55					*/
/************************************************************************/

ST_RET msoft_heap_check (ST_INT16 prt_mask)
  {
#if defined(DEBUG_SISCO) && defined (MSOFT_HEAP_CHECK) 
struct _heapinfo entry;
ST_INT i;
ST_INT ret;
ST_BOOLEAN error;
ST_INT32 total_size;
ST_INT num_ptrs;

  S_LOCK_RESOURCES (); 
  entry._pentry = NULL;		/* start at the top of the list		*/
  error = SD_FALSE;
  total_size = 0;
  num_ptrs = 0;
  for (i = 0; SD_TRUE; ++i)
    {
    ret = _heapwalk (&entry);
    switch (ret)
      {
      case _HEAPBADPTR :
        MEMLOG_ERR0 ("BAD HEAP : POINTER OUTSIDE HEAP");
        error = SD_TRUE;
      break;
      case _HEAPBADBEGIN :
        MEMLOG_ERR0 ("BAD HEAP : INITIAL ENTRY");
        error = SD_TRUE;
      break;
      case _HEAPBADNODE :
        MEMLOG_ERR0 ("BAD HEAP : BAD NODE ");
        error = SD_TRUE;
      break;
      }

    if (prt_mask & DEBUG_HEAP_ENTRY)
      {
      if (ret == _HEAPEMPTY)
        {
        MEMLOG_DUMP0 ("Heap Empty");
        }
      else if (ret == _HEAPEND)
        {
        MEMLOG_DUMP0 ("End Of Heap");
        }
      else
        {
        MEMLOG_CDUMP1 (" Heap Entry %d :",i);
        MEMLOG_CDUMP1 ("    Pointer        : %08lX",entry._pentry);
        MEMLOG_CDUMP1 ("    Size (decimal) : %d",entry._size);
        MEMLOG_CDUMP1 ("    Use Flag       : %s",
  		entry._useflag == _USEDENTRY ? "USED" : "FREE");
        num_ptrs++;
        total_size += entry._size;
        }

      MEMLOG_PAUSE ("Hit any key to continue - ");
      }

    if (ret == _HEAPEND || ret == _HEAPEMPTY || error)
      break;
    }

  if (prt_mask & DEBUG_HEAP_SUM)
    {
    MEMLOG_DUMP1 ("Total # Pointers : %d",num_ptrs);
    MEMLOG_CDUMP1 ("Total memory malloc'd : %ld",total_size);
    MEMLOG_PAUSE ("Hit any key to continue - ");
    }

  _heapset ('\x55');		/* write all unused memory with 55's	*/

  S_UNLOCK_RESOURCES (); 
  return (SD_FALSE);
#elif defined(DEBUG_SISCO) && defined(MEMCHK_WIN32_CRT_DEBUG)
  return _CrtCheckMemory () ? SD_FALSE : SD_TRUE;
#else    
  return (SD_FALSE);
#endif    
  }



/************************************************************************/
/************************************************************************/
/*			chk_alloc_ptr					*/
/************************************************************************/

ST_RET chk_alloc_ptr (ST_VOID *mem_ptr)
  {
#ifdef DEBUG_SISCO
CHK_DYN_MEM_NODE *chk_mem_node;

  if (m_mem_debug_saved)
    {
    S_LOCK_RESOURCES (); 
/* verify that a non-NULL pointer was passed in				*/
    if (!mem_ptr)
      {
      if (chk_debug_en & MEM_LOG_ERR)
        {
        MEMLOG_ERR0 ("NULL pointer in chk_alloc_ptr");
        goto MEM_PTR_CHK_ERROR; 
        }
      S_UNLOCK_RESOURCES (); 
      return(SD_FAILURE);
      }

/* verify that the pointer matches the expected pointer			*/
    chk_mem_node = ((CHK_DYN_MEM_NODE *)mem_ptr)-1;

/* Verify that this node is on the list before accessing it		*/
    if (list_find_node (head_dyn_mem_list, chk_mem_node) != SD_SUCCESS)
      {
      if (chk_debug_en & MEM_LOG_ERR)
        {
        MEMLOG_ERR0 (" chk_alloc_ptr - pointer not on list");
        goto MEM_PTR_CHK_ERROR; 
        }
      S_UNLOCK_RESOURCES (); 
      return(SD_FAILURE);
      }

    if (chk_mem_node->chk_alloc_mem_ptr == mem_ptr)
      {
      if (chk_mem_node->overwrite_prev_detected == SD_FALSE)
        {
        if (check_overwrite (chk_mem_node))
          {
          MEMLOG_ERR0 ("Memory overwrite at end of buffer detected");
          chk_mem_node->overwrite_prev_detected=SD_TRUE;
          MEMLOG_CERR5 (
            "Node = %lx, Prev = %lx, Next = %lx, Len = %u, Curr = %lx",
                   chk_mem_node, 
                   chk_mem_node->chk_mem_node_link.prev, 
                   chk_mem_node->chk_mem_node_link.next,
   		   chk_mem_node->chk_alloc_mem_size, 
  		   chk_mem_node->chk_alloc_mem_ptr);
          MEMLOG_CERR2 ("Pointer was allocated at file: %s, line %d ",
				chk_mem_node->file,chk_mem_node->line);
          MEMLOG_CHERR (chk_mem_node->chk_alloc_mem_size + 
      		        sizeof (CHK_DYN_MEM_NODE) +
		        m_num_pad_bytes, chk_mem_node);
          goto MEM_PTR_CHK_ERROR2;
          }
        } /* end if overwrite not detected for this node			*/ 
	
#if defined(MEMCHK_WIN32_CRT_DEBUG)
      if (_CrtIsMemoryBlock (chk_mem_node, chk_mem_node->chk_alloc_mem_size + mem_overhead,
      			     NULL, NULL, NULL) == FALSE)
        {
        MEMLOG_ERR0 ("Buffer not a valid memory block");
        MEMLOG_CERR5 (
            "Node = %lx, Prev = %lx, Next = %lx, Len = %u, Curr = %lx",
                 chk_mem_node, 
                 chk_mem_node->chk_mem_node_link.prev, 
                 chk_mem_node->chk_mem_node_link.next,
   	         chk_mem_node->chk_alloc_mem_size, 
  		 chk_mem_node->chk_alloc_mem_ptr);
        MEMLOG_CERR2 ("Pointer was allocated at file: %s, line %d ",
		 	chk_mem_node->file,chk_mem_node->line);
        MEMLOG_CHERR (chk_mem_node->chk_alloc_mem_size + 
      		      sizeof (CHK_DYN_MEM_NODE) +
		      m_num_pad_bytes, chk_mem_node);
        goto MEM_PTR_CHK_ERROR2;
	}
#endif
      }
    else		/* Pointer does not match header			*/
      {
MEM_PTR_CHK_ERROR :

      MEMLOG_ERR1 ("Pointer Does Not Exist (%08lx)",mem_ptr);

MEM_PTR_CHK_ERROR2 :
      MEMLOG_CALLSTACK (NULL);
      MEMLOG_PAUSEERR ("");
      if (mem_chk_err)
        (*mem_chk_err)();
      S_UNLOCK_RESOURCES (); 
      return(SD_FAILURE);
      }
    S_UNLOCK_RESOURCES (); 
    }
#endif
  return(SD_SUCCESS);
  }


/************************************************************************/
/* 			check_mem_list					*/
/************************************************************************/

ST_VOID check_mem_list ()
  {
#ifdef DEBUG_SISCO
CHK_DYN_MEM_NODE *node;
CHK_DYN_MEM_NODE *prev;
CHK_DYN_MEM_NODE *next;
ST_INT len;
ST_VOID *curr;
ST_UINT32 j;

  if (m_mem_debug_saved)
    {
    S_LOCK_RESOURCES (); 
    if ((node = head_dyn_mem_list) == NULL)
      {
      S_UNLOCK_RESOURCES (); 
      return;				/* list empty. Nothing to check	*/
      }
    for (j = 0;  j < chk_max_dyn_mem_ptrs;  j++)
      {
      /* should break out when j = chk_curr_mem_count	*/
      prev = (CHK_DYN_MEM_NODE *) node->chk_mem_node_link.prev;
      next = (CHK_DYN_MEM_NODE *) node->chk_mem_node_link.next;
      len = node->chk_alloc_mem_size;
      curr = node->chk_alloc_mem_ptr;

      /* check ptr to this node	*/
      if (curr != (ST_VOID *) (node + 1))
        {
        MEMLOG_ERR1 ("Current node pointer corrupted on node #%d", j);
        MEMLOG_CERR5 (
          "Node = %lx, Prev = %lx, Next = %lx, Len = %u, Curr = %lx",
          node, prev, next, len, curr);
        MEMLOG_CALLSTACK (NULL);
        MEMLOG_PAUSEERR ("");
        if (mem_chk_err)
          (*mem_chk_err)();
        }

      /* check link list ptrs	*/
      if (node != (CHK_DYN_MEM_NODE *) next->chk_mem_node_link.prev)
        {
        MEMLOG_ERR1 ("\nLinked list corrupted at node #%d", j);
        MEMLOG_CERR6 (
            "Node=%lx, Prev=%lx, Next=%lx, Len=%5u, Curr=%lx, Next node's prev=%lx",
            node, prev, next, len, curr, next->chk_mem_node_link.prev);
        MEMLOG_CALLSTACK (NULL);
        MEMLOG_PAUSEERR ("");
        if (mem_chk_err)
          (*mem_chk_err)();
        }

      /* Check for buffer overwrite					*/
      if (check_overwrite (node))
        {
        if(node->overwrite_prev_detected==SD_FALSE)
          {
          MEMLOG_ERR0 ("Memory overwrite at end of buffer detected");
          MEMLOG_CERR5 (
            "Node = %lx, Prev = %lx, Next = %lx, Len = %u, Curr = %lx",
            node, prev, next, len, curr);
          MEMLOG_CERR2 ("Pointer was allocated at file: %s, line %d ",
     				  node->file,node->line);
          MEMLOG_CHERR (node->chk_alloc_mem_size + 
        		sizeof (CHK_DYN_MEM_NODE) +
		        m_num_pad_bytes, node);
          MEMLOG_CALLSTACK (NULL);
          MEMLOG_PAUSEERR ("");
          if (mem_chk_err)
            (*mem_chk_err)();
          node->overwrite_prev_detected=SD_TRUE;
          }
        }
#if defined(MEMCHK_WIN32_CRT_DEBUG)
      else
        {
        if (_CrtIsMemoryBlock (node, node->chk_alloc_mem_size + mem_overhead,
      			       NULL, NULL, NULL) == FALSE)
          {
          MEMLOG_ERR0 ("Buffer not a valid memory block");
          MEMLOG_CERR5 (
              "Node = %lx, Prev = %lx, Next = %lx, Len = %u, Curr = %lx",
                   node, 
                   node->chk_mem_node_link.prev, 
                   node->chk_mem_node_link.next,
   	           node->chk_alloc_mem_size, 
  		   node->chk_alloc_mem_ptr);
          MEMLOG_CERR2 ("Pointer was allocated at file: %s, line %d ",
		 	 node->file,node->line);
          MEMLOG_CHERR (node->chk_alloc_mem_size + 
      		        sizeof (CHK_DYN_MEM_NODE) +
		        m_num_pad_bytes, node);
	  }		
	}
#endif

      if (next == head_dyn_mem_list)	/* is this last node on list?	*/
        break;

      node = next;		/* point to next node in linked list	*/
      }	/* end "for"	*/

    if (j >= chk_max_dyn_mem_ptrs)
      {
      MEMLOG_ERR1 (
          "Last node (Node #%d) does not point back to first.", j - 1);
      MEMLOG_CALLSTACK (NULL);
      MEMLOG_PAUSEERR ("");
      if (mem_chk_err)
        (*mem_chk_err)();
      }
    else if (j + 1 != chk_curr_mem_count)
      {
      MEMLOG_CERR0 ("Linked list current count corrupted.");
      MEMLOG_CERR2 ("Count = %lu,  Should be = %lu.",
          (ST_ULONG) chk_curr_mem_count, (ST_ULONG) (j + 1));
      MEMLOG_CALLSTACK (NULL);
      MEMLOG_PAUSEERR ("");
      if (mem_chk_err)
        (*mem_chk_err)();
      }
    S_UNLOCK_RESOURCES (); 
    return;
    }
#endif	/* DEBUG_SISCO	*/
  }

/************************************************************************/
/*			check_overwrite 				*/
/************************************************************************/

#ifdef DEBUG_SISCO
static ST_RET check_overwrite (CHK_DYN_MEM_NODE *node)
  {
ST_CHAR *bptr;
ST_CHAR *eptr;
ST_INT i;

/* Get a pointer to the byte after the user's data			*/
  bptr = (ST_CHAR *) (node +1);
  bptr += node->chk_alloc_mem_size;
  eptr = bptr + m_num_pad_bytes;

/* Verify the signature							*/
  i = 0;
  while (bptr < eptr)
    {
    if (*(bptr++) != m_pad_string[i])
      return (SD_TRUE);	/* Overwrite detected, return the error		*/

    if (++i >= m_num_pad_bytes) /* if we have wrapped the string  	*/
      i = 0;
    }  

/* All is OK with the end of this buffer				*/
  return (SD_FALSE);
  }
#endif

/************************************************************************/
/*			set_overwrite_value				*/
/************************************************************************/

#ifdef DEBUG_SISCO
static ST_VOID set_overwrite_value (CHK_DYN_MEM_NODE *node)
  {
ST_CHAR *bptr;
ST_CHAR *eptr;
ST_INT i;

/* Get a pointer to the byte after the user's data			*/
  bptr = (ST_CHAR *)(node+1);
  bptr += node->chk_alloc_mem_size;
  eptr = bptr + m_num_pad_bytes;

/* Set the signature							*/
  i = 0;
  while (bptr < eptr)
    {
    *(bptr++) = m_pad_string[i];
    if (++i >= m_num_pad_bytes) /* if we have wrapped the string	*/
      i = 0;
    }

  node->overwrite_prev_detected=SD_FALSE;
  }
#endif


