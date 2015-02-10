/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*      	  1993 - 2005, All Rights Reserved.		        */
/*									*/
/*		    PROPRIETARY AND CONFIDENTIAL			*/
/*									*/
/* MODULE NAME : slog.h							*/
/* PRODUCT(S)  : SLOG							*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 08/10/05  MDE     50    Added slog_start, slog_end, index file	*/
/* 08/05/05  EJV     49    Added extern ssleLogMaskMapCtrl.     	*/
/* 08/02/05  MDE     48    Added IOS ports				*/
/* 06/30/05  EJV     47    Added extern genlistDebugMapCtrl.		*/
/*			   Arranged xxxMaskMapCtrl in alphabetical order*/
/*			   Del duplicate logCfgAddMaskGroup proto.	*/
/*			   Del logCfgAddAll proto (user must add all).	*/
/*			   Del logcfgx (use logcfgx_ex instead)		*/
/* 04/29/05  EJV     46    Added IPC_LOG_PORT_RFC1006_LISTENER def.	*/
/* 03/31/05  MDE     45    Added more SLOGALWAYS macros			*/
/* 02/22/05  JRB     44    slogIpc: add SD_CONST to some args.		*/
/* 02/16/05  JRB     43    Del #if around forward references.		*/
/* 02/10/05  MDE     42    Added STRINGBUF and DOUBLE DATATYPE's	*/
/* 02/02/05  MDE     41    Smart mode work				*/
/* 01/27/05  MDE     40    LINUX warning cleanup			*/
/* 01/19/05  MDE     39    Added LogCfg defines, etc..			*/
/* 01/19/05  EJV     38    AIX:del extern in struct tag_GEN_SOCK,C++ err*/
/* 12/06/04  ASK     37    Added SLOG IPC "smart" client features. Added*/
/*			   RUINT define for new logcfgx.		*/
/* 11/22/04  JRB     36    Add max_msg_size & msg_buf to LOG_CTRL.	*/
/*			   Add slog_max_msg_size_set proto.		*/
/*			   Add slog_max_msg_size_get macro.		*/
/*			   slogDelBuf: add (LOG_CTRL *) arg.		*/
/* 10/12/04  MDE     35    Added logcfgx mask extension mechanism 	*/
/* 08/04/04  EJV     34    LOGMEM_ITEM: chg log time,related to prev chg*/
/*		           Del slogTime, slogMs global variables.	*/
/* 07/09/04  EJV     33    Del DATETIME_MILLI_EN,add 'obsolete' comments*/
/* 06/24/04  DSF     32    Added DATETIME_MILLI_EN			*/
/* 06/24/04  EJV     31    IPC_LOG_CTRL: add portUsed.			*/
/* 06/21/04  EJV     30    IPC_LOG_CTRL: add portCnt, appId fields.	*/
/* 05/26/04  EJV     29    Added comment to keep track on used IPC ports*/
/* 05/20/04  EJV     28    IPC_LOG_CTRL: limited log message queue.	*/
/* 05/18/04  MDE     27    Removed LOG_IPC_SUPPORT #ifdef's		*/
/* 05/11/04  EJV     26    Revised LOG_IPC_SUPPORT code to use gensock2.*/
/*			   Added SLOG_MAX_LOGTYPESTR, SLOG_MAX_HDR.	*/
/*			   Added slogSetHdr() proto.			*/
/* 02/10/04  KCR     25    Added slogDelBuf()                           */
/* 10/24/03  JRB     24    Move gs_debug_sel from glbsem.h to here.	*/
/*			   Move clnp_debug_sel from clnp_usr.h to here.	*/
/*			   Del windows.h (see sysincs.h).		*/
/* 06/20/03  JRB     23    Del ;  from SLOGALWAYS* too.			*/
/* 05/08/03  JRB     22    Del {} from SLOGALWAYS* so they work in	*/
/*			   almost any context.				*/
/* 05/07/03  DSF     21    Increased the value of SLOG_MAX_FNAME	*/
/* 			   Added support for sErrLogCtrl		*/
/* 10/17/02  JRB     20    Add "Base" macros SLOG_*, SLOGC_*, SLOGH.	*/
/* 03/11/02  JRB     19    SLOG_PAUSE, SLOG_LF, SLOG_CLRSCR do nothing.	*/
/* 04/26/01  DSF     18    Define SOCKET if not yet defined		*/
/* 04/16/01  DSF     17    Include windows.h instead of including	*/
/*			   winsock.h directly				*/
/* 03/23/01  MDE     16    Added _slogStr, SLOGALWAYSS 			*/
/* 11/27/00  JRB     15    Added read_log_cfg_file, logcfgx protos.	*/
/* 08/22/00  KCR     14    Added FIL_CTRL_NO_LOG_HDR			*/
/* 03/09/00  MDE     13    Added SD_CONST modifiers to LOGMEM_ITEM	*/
/* 09/13/99  MDE     12    Added SD_CONST modifiers			*/
/* 11/11/98  DSF     11    Minor changes to _slog_dyn_log_fun		*/
/* 10/16/98  DSF     10    Spelling					*/
/* 10/08/98  MDE     09    Migrated to updated SLOG interface		*/
/* 06/15/98  NAV     08    Conditionally include winsock.h or winsock2.h*/
/* 06/03/98  MDE     07    Replaced 'winsock.h' with 'windows.h'	*/
/* 12/12/97  MDE     06    Added DEBUG_SISCO versions of SLOGALWAYSx	*/
/* 11/05/97  DSF     05    Added SYSTIME_EN				*/
/* 09/12/97  DSF     04    Expose slogSetTimeText ()			*/
/* 06/11/97  MDE     03    Include time.h for all			*/
/* 05/27/97  DSF     02    Added IPC logging capability			*/
/* 05/27/97  DSF     01    Include time.h for sun			*/
/* 04/02/97  DTL   7.00    MMSEASE 7.0 release. See MODL70.DOC for	*/
/*			   history.					*/
/************************************************************************/

#ifndef SLOG_INCLUDED
#define SLOG_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "gen_list.h"
#include <stdio.h>
#include <time.h>	/* for time_t */

#define TIME_BUF_LEN 30

/************************************************************************/
/* Memory logging string buffer size limit				*/
/* NOTE : Do not change this lightly! QMEM.C should be looked at	*/
/************************************************************************/

#define SLOG_MEM_BUF_SIZE 125

/* Memory logging item control						*/
/* A table of these is established at initialization time		*/

#define LMF_USED	0x0001
#define LMF_HEADER	0x0002
#define LMF_HEX		0x0004


typedef struct logmem_item
  {
  ST_UINT flags;		/* see LMF_xxx flags			*/
  ST_INT logType;		/* log type storage			*/
  SD_CONST ST_CHAR *logTypeStr;	/* log type string			*/
  ST_INT lineNum;		/* source file line number		*/
  SD_CONST ST_CHAR *sourceFile;	/* source file name, NULL for none	*/
  ST_CHAR slogTimeText[TIME_BUF_LEN];  
  ST_CHAR *string;		/* text						*/
  } LOGMEM_ITEM;


/************************************************************************/
/* File Logging State flags						*/
/************************************************************************/

#define FIL_STATE_OPEN 	       0x0001
#define FIL_STATE_NEED_WIPE    0x0002

/* File Logging Control flags						*/

#define FIL_CTRL_WIPE_EN       0x0001
#define FIL_CTRL_WRAP_EN       0x0002
#define FIL_CTRL_MSG_HDR_EN    0x0004	
#define FIL_CTRL_NO_LOG_HDR    0x0040	

/* File output mode flags						*/

#define FIL_CTRL_NO_APPEND     0x0008	
#define FIL_CTRL_HARD_FLUSH    0x0010	
#define FIL_CTRL_SETBUF_EN     0x0020	

typedef struct file_log_ctrl
  {
/* User sets these elements						*/
  ST_ULONG  maxSize;
  ST_CHAR  *fileName;		/* destination filename			*/
  ST_UINT ctrl;			/* see FIL_CTRL_xxx flags		*/
  ST_LONG wipeFilePos;

/* Internal use only							*/
  ST_UINT state;		/* see FIL_STATE_xxx flags		*/
  FILE  *fp;
  } FILE_LOG_CTRL;

/* Memory Logging State flags						*/

#define MEM_STATE_INIT        0x0001

/* Memory Logging Control flags						*/

#define MEM_CTRL_MSG_HDR_EN   0x0001
#define MEM_CTRL_AUTODUMP_EN  0x0002
#define MEM_CTRL_HEX_LOG      0x0004

typedef struct mem_log_ctrl
  {
/* User sets these elements						*/
  ST_INT maxItems;	 /* # items to allocate at powerup		*/
  ST_CHAR *dumpFileName; /* where memory dump goes			*/
  ST_UINT ctrl;	    	 /* see MEM_CTRL_xxx flags			*/

/* Internal use only							*/
  ST_UINT state;	/* see MEM_STATE_xxx flags			*/
  LOGMEM_ITEM *item; 	/* Item table					*/
  ST_INT nextPut; 	/* Current position (where last was put		*/
  } MEM_LOG_CTRL;

/************************************************************************/
/*		Socket Slogging Subsystem				*/
/************************************************************************/

/* default parameters for socket logging subsystem			*/
#define IPC_LOG_BASE_PORT		55147

/* ports below are taken by following SISCO applications:		*/
#define IPC_LOG_PORT_AXS4ICCP		IPC_LOG_BASE_PORT+1 /* 55148 */
#define IPC_LOG_PORT_OSILL2		IPC_LOG_BASE_PORT+2 /* 55149 */
#define IPC_LOG_PORT_OSILL2_IPC		IPC_LOG_BASE_PORT+3 /* 55150 */
#define IPC_LOG_PORT_AXS4MMS		IPC_LOG_BASE_PORT+4 /* 55151 */
#define IPC_LOG_PORT_AXS4IEC61850	IPC_LOG_BASE_PORT+5 /* 55152 */
#define IPC_LOG_PORT_AXS4GOOSE		IPC_LOG_BASE_PORT+6 /* 55153 */
#define IPC_LOG_PORT_RFC1006_LISTENER	IPC_LOG_BASE_PORT+7 /* 55154 */

/* UIB ports: reserve 10:			           55157-55167 */	
#define IPC_LOG_PORT_UIBBASE		IPC_LOG_BASE_PORT+10  /* 55157 */
#define IPC_LOG_NUMPORTS_UIB		10

/* USPS IOS ports: reserve 100 :			   55347-55447 */	
#define IPC_LOG_IOS_MPECOM		IPC_LOG_BASE_PORT+200 /* 55347 */
#define IPC_LOG_IOS_MPE			IPC_LOG_BASE_PORT+201 /* 55348 */

#define IPC_LOG_MAX_CONNECTIONS		10
#define IPC_LOG_MAX_QUEUE_CNT		100

/* SMART MODE */
/* Format of smart message is 3 unsigned long integers:			*/
/* 	unsigned long	magicNumber = SLOGIPC_MAGIC_NUMBER;		*/
/* 	unsigned long	messageLen = dataLength + 4 (messageType size)	*/
/* 	unsigned long	messageType = SLOGIPC_MSG_TYPE_XXX		*/
/* 	char 		messageData[dataLength]				*/

#define SLOGIPC_MAGIC_NUMBER    0x1F2E3D4C

/* header size for smart clients */
#define SLOGIPC_SMRT_HDR_SIZE   (3 * sizeof (ST_ULONG))
#define SLOGIPC_HDR_PARAM_SIZE  sizeof (ST_ULONG)
#define SLOGIPC_MSG_TYPE_SIZE   sizeof (ST_ULONG)

/* message types */
#define SLOGIPC_MSG_TYPE_LOG      0x00000000
#define SLOGIPC_MSG_TYPE_RESERVED 0x00000001
#define SLOGIPC_MSG_TYPE_APPID    0x00000002
#define SLOGIPC_MSG_TYPE_START    0x00000003
#define SLOGIPC_MSG_TYPE_STOP     0x00000004

#define SLOGIPC_MSG_TYPE_CMD      0x00000100

/* configuration parameters for socket logging & command subsystem	*/
struct tag_GEN_SOCK;		/* forward reference	*/
struct tag_GEN_SOCK_DATA;	/* forward reference	*/

typedef struct ipc_log_ctrl_tag
  {
  /* Used for client (calling) connections */
  ST_CHAR   *callingIp;		/* IP or host name of host to connect to*/
  ST_UINT16  callingPort;	/* port of host to connect to		*/

  /* Used for server (called) connections */
  ST_UINT16  port;		/* listening port number		*/
  ST_UINT16  portCnt;		/* num of listening ports starting with	*/
				/* base port, for multiple app instances*/

  ST_UINT16  portUsed;		/* this the listening port actually used*/
  ST_UINT    maxConns;		/* max connections that can be accepted	*/

  /* Misc parameters */
  ST_UINT    maxQueCnt;		/* max num of logs that can be queued	*/

  ST_CHAR   *appId;		/* pointer to NULL terminated string	*/
				/* identifying the application, sent to	*/
				/* IPC Logging Client in first message	*/

  /* command processing function pointer - to be set by user application  */
  ST_VOID (*slog_ipc_cmd_fun) (struct tag_GEN_SOCK *pSock, struct tag_GEN_SOCK_DATA *sockData); 

  } IPC_LOG_CTRL;

/************************************************************************/
/* LOG CONTROL								*/
/* A structure of this type is used for each independent logging entity	*/
/************************************************************************/

/* Log control mask values						*/
#define LOG_MEM_EN	     	0x0001L
#define LOG_FILE_EN	     	0x0002L
#define LOG_TIME_EN	     	0x0008L
#define LOG_TIMEDATE_EN	     	0x0010L		/* obsolete - ignored	*/
#define LOG_DIFFTIME_EN	     	0x0020L		/* obsolete - ignored	*/
#define LOG_TIME_INITIALIZED 	0x0040L		/* obsolete - ignored	*/
#define LOG_SYSTIME_EN	     	0x0100L		/* obsolete - ignored	*/

#define LOG_NO_HEADER_CR     	0x0200L
#define LOG_FILENAME_SUPPRESS 	0x0400L
#define LOG_LOGTYPE_SUPPRESS  	0x0800L

#define LOG_IPC_LISTEN_EN      	0x1000L
#define LOG_IPC_CALL_EN     	0x2000L
#define LOG_IPC_SMART     	0x4000L
#define LOG_IPC_EN   (LOG_IPC_LISTEN_EN|LOG_IPC_CALL_EN)

#define SLOG_NORMAL 	 0
#define SLOG_CONT 	-1
/* Dynamic logging standard command definitions				*/
#define	SLOG_DYN_PAUSE  -10	/* OBSOLETE	*/
#define	SLOG_DYN_LF     -11	/* OBSOLETE	*/
#define	SLOG_DYN_CLRSCR -12	/* OBSOLETE	*/

typedef struct log_ctrl
  {
/* User sets these elements						*/
  ST_UINT32 logCtrl; 	/* Logging Control flags - see LOG_xxx defines	*/
  FILE_LOG_CTRL fc;	/* File logging control				*/
  MEM_LOG_CTRL  mc;	/* Memory logging control			*/
  IPC_LOG_CTRL  ipc;	/* IPC logging control				*/

/* Application specific information					*/
  ST_UINT32 logMask1;	/* These bit masked variables are used by the	*/
  ST_UINT32 logMask2;	/* user application to determine whether a item	*/
  ST_UINT32 logMask3;	/* is to be logged. 6 ST_INT32's == 192 bits	*/
  ST_UINT32 logMask4;
  ST_UINT32 logMask5;
  ST_UINT32 logMask6;

/* Internal variables.							*/
  ST_INT max_msg_size;	/* max allowed log msg size.			*/
  ST_CHAR *msg_buf;	/* buffer for log msg				*/
  } LOG_CTRL;


/* Max size of any single log statement (sprintf)			*/

#define MAX_LOG_SIZE 500

/* Set the following variable to a number larger than the default of    */
/* MAX_LOG_SIZE if you want to log more than 500 bytes in a single      */
/* message.                                                             */

extern ST_INT sl_max_msg_size;

/************************************************************************/
/* Main entry points into the SLOG library				*/
/************************************************************************/

ST_VOID slog (LOG_CTRL *lc, 
	      SD_CONST ST_INT logType, 
	      SD_CONST ST_CHAR *SD_CONST sourceFile, 
	      SD_CONST ST_INT lineNum, 
	      SD_CONST ST_CHAR *format, ...);

ST_VOID slogx (ST_UINT32 doit, LOG_CTRL *lc, 
	       SD_CONST  ST_INT logType, 
	       SD_CONST ST_CHAR *SD_CONST sourceFile, 
	       SD_CONST ST_INT lineNum, 
	       SD_CONST ST_CHAR *format, ...);

ST_VOID slogHex (LOG_CTRL *lc, 
		 SD_CONST ST_INT logType, 
		 SD_CONST ST_CHAR *SD_CONST fileName, 
		 SD_CONST ST_INT lineNum,
	  	 SD_CONST ST_INT numBytes, 
		 SD_CONST ST_VOID *hexData);

ST_VOID _slog (LOG_CTRL *lc, 
	       SD_CONST ST_CHAR *SD_CONST logTypeStr, 
	       SD_CONST ST_CHAR *SD_CONST sourceFile, 
	       SD_CONST ST_INT	lineNum, 
	       SD_CONST ST_CHAR *format, ...);
ST_VOID _slogc (LOG_CTRL *lc, SD_CONST ST_CHAR *format, ...);

ST_VOID _slogx (ST_UINT32 doit, LOG_CTRL *lc, 
		SD_CONST ST_CHAR *SD_CONST logTypeStr, 
		SD_CONST ST_CHAR *SD_CONST sourceFile, 
		SD_CONST ST_INT	lineNum, 
		SD_CONST ST_CHAR *format, ...);

ST_VOID _slogHex (LOG_CTRL *lc, 
		  ST_INT numBytes, 
		  SD_CONST ST_VOID *hexData);
ST_VOID _slogStr (LOG_CTRL *lc, ST_CHAR *strData);

ST_RET slog_start (LOG_CTRL *lc, ST_INT max_msg_size);
ST_RET slog_end (LOG_CTRL *lc);

ST_RET slog_max_msg_size_set (LOG_CTRL *lc, ST_INT max_msg_size);
/* Use macro to get max msg size. Faster.	*/
#define slog_max_msg_size_get(log_ctrl)		(log_ctrl->max_msg_size)

/************************************************************************/
/* Main entry points into the SLOG library 				*/
/************************************************************************/

/* File Logging Utility Functions					*/

ST_VOID slogCloseFile (LOG_CTRL *lc);
ST_VOID slogCloneFile (LOG_CTRL *lc, SD_CONST ST_CHAR *newfile);
ST_VOID slogCloneFileEx (LOG_CTRL *lc, SD_CONST ST_CHAR *newFile);
ST_VOID slogDeleteFile (LOG_CTRL *lc);
ST_VOID slogCallStack (LOG_CTRL *lc, SD_CONST ST_CHAR *txt);
ST_VOID slogDelBuf (LOG_CTRL *lc);

/* Memory Logging Utility Functions					*/

ST_VOID slogDumpMem (LOG_CTRL *lc);
ST_VOID slogResetMem (LOG_CTRL *lc);
ST_CHAR *slogGetMemMsg (LOG_CTRL *lc, SD_CONST ST_INT msgNum);
ST_INT slogGetMemCount (LOG_CTRL *lc);

/* Internal functions							*/

ST_VOID slogFile (LOG_CTRL *lc, SD_CONST ST_INT logType, 
		SD_CONST ST_CHAR *SD_CONST logTypeStr, 
		SD_CONST ST_CHAR *SD_CONST sourceFile, 
		SD_CONST ST_INT lineNum, 
		SD_CONST ST_INT bufLen, SD_CONST ST_CHAR *buf);

ST_VOID slogMem (LOG_CTRL *lc, 
		SD_CONST ST_INT logType, 
		SD_CONST ST_CHAR *SD_CONST logTypeStr, 
		SD_CONST ST_CHAR *SD_CONST sourceFile, 
		SD_CONST ST_INT lineNum, 
		ST_INT bufLen, SD_CONST ST_CHAR *buf);

ST_RET  slogIpcInit (LOG_CTRL *lc);
ST_RET  slogIpcStop (ST_VOID);
ST_VOID slogIpc (LOG_CTRL *lc, ST_INT logType,
		 SD_CONST ST_CHAR *SD_CONST logTypeStr, 
		 SD_CONST ST_CHAR *SD_CONST sourceFile,
		 ST_INT lineNum, ST_INT bufLen, 
                 ST_CHAR *buf);

ST_VOID slogMemInit (LOG_CTRL *lc);
ST_RET  read_log_cfg_file (ST_CHAR *cfg_filename);
ST_VOID slog_get_index_file_name (LOG_CTRL *lc, ST_CHAR *dest);

/************************************************************************/

/* dataType defines */
#define _LOGCFG_DATATYPE_UINT_MASK	0
#define _LOGCFG_DATATYPE_UINT32_MASK	1
#define _LOGCFG_DATATYPE_RUINT32_MASK	2
#define _LOGCFG_DATATYPE_BOOLEAN	3
#define _LOGCFG_DATATYPE_INT		4
#define _LOGCFG_DATATYPE_LONG		5
#define _LOGCFG_DATATYPE_INT16		6
#define _LOGCFG_DATATYPE_INT32		7
#define _LOGCFG_DATATYPE_UINT		8
#define _LOGCFG_DATATYPE_ULONG		9
#define _LOGCFG_DATATYPE_UINT16	       10
#define _LOGCFG_DATATYPE_UINT32	       11
#define _LOGCFG_DATATYPE_STRING	       12
#define _LOGCFG_DATATYPE_STRINGBUF     13
#define _LOGCFG_DATATYPE_DOUBLE	       14
#define _LOGCFG_DATATYPE_FILENAME      15
#define _LOGCFG_DATATYPE_CALLBACK      16


typedef struct 
  {
  ST_CHAR *tag;
  ST_UINT  mask;
  ST_VOID *addr;
  ST_INT   dataType;
  ST_CHAR *description;	/* NULL for no description */
  } LOGCFGX_VALUE_MAP;

typedef struct 
  {
  DBL_LNK 	 l;
  ST_CHAR 	*name;
  ST_INT         numMaskMap;
  LOGCFGX_VALUE_MAP *maskMapTbl;
  } LOGCFG_VALUE_GROUP;

ST_RET logcfgx_ex (LOG_CTRL *destLogCtrl, ST_CHAR *logFileName, ST_CHAR *fileNamePrefix, 
		   ST_BOOLEAN masksOnly, ST_BOOLEAN saveTagVals);
ST_VOID logCfgAddMaskGroup (LOGCFG_VALUE_GROUP *logMaskGroup);
ST_VOID logCfgRemoveMaskGroup (LOGCFG_VALUE_GROUP *logMaskGroup);
ST_VOID logCfgRemoveAllMaskGroups (ST_VOID);

/* ST_VOID (*logCfgCallbackFun) (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag, LOGCFGX_VALUE_MAP *valMap); */


extern LOGCFG_VALUE_GROUP acseLogMaskMapCtrl;	/* MMS-LITE, ICCP-LITE,           MMSEASE_MOSI */
extern LOGCFG_VALUE_GROUP adlcLogMaskMapCtrl;	/* MMS-LITE                                    */
extern LOGCFG_VALUE_GROUP asn1LogMaskMapCtrl;	/* MMS-LITE, ICCP-LITE, MMS-EASE, MMSEASE_MOSI */
extern LOGCFG_VALUE_GROUP clnpLogMaskMapCtrl;	/* MMS-LITE, ICCP-LITE                         */
extern LOGCFG_VALUE_GROUP genlistDebugMapCtrl;	/* MMS-LITE, ICCP-LITE, MMS-EASE, MMSEASE_MOSI */
extern LOGCFG_VALUE_GROUP gsLogMaskMapCtrl;	/* MMS-LITE, ICCP-LITE, MMS-EASE, MMSEASE_MOSI */
extern LOGCFG_VALUE_GROUP icfgLogMaskMapCtrl;	/*           ICCP-LITE                         */
extern LOGCFG_VALUE_GROUP logcfgMaskMapCtrl;	/* MMS-LITE, ICCP-LITE, MMS-EASE, MMSEASE_MOSI */
extern LOGCFG_VALUE_GROUP logCfgLogCtrlMapCtrl;	/* MMS-LITE, ICCP-LITE, MMS-EASE, MMSEASE_MOSI */
extern LOGCFG_VALUE_GROUP memDebugMapCtrl;	/* MMS-LITE, ICCP-LITE, MMS-EASE, MMSEASE_MOSI */
extern LOGCFG_VALUE_GROUP memLogMaskMapCtrl;	/* MMS-LITE, ICCP-LITE, MMS-EASE, MMSEASE_MOSI */
extern LOGCFG_VALUE_GROUP miLogMaskMapCtrl;	/*           ICCP-LITE                         */
extern LOGCFG_VALUE_GROUP mmsLogMaskMapCtrl;	/* MMS-LITE, ICCP-LITE, MMS-EASE, MMSEASE_MOSI */
extern LOGCFG_VALUE_GROUP mvlLogMaskMapCtrl;	/* MMS-LITE, ICCP-LITE                         */
extern LOGCFG_VALUE_GROUP secLogMaskMapCtrl;	/* MMS-LITE, ICCP-LITE, MMS-EASE, MMSEASE_MOSI */
extern LOGCFG_VALUE_GROUP ssleLogMaskMapCtrl;   /* MMS-LITE, ICCP-LITE, MMS-EASE, MMSEASE_MOSI */
extern LOGCFG_VALUE_GROUP smpLogMaskMapCtrl;	/* MMS-LITE                                    */
extern LOGCFG_VALUE_GROUP sockLogMaskMapCtrl;	/* MMS-LITE, ICCP-LITE, MMS-EASE, MMSEASE_MOSI */
extern LOGCFG_VALUE_GROUP suicLogMaskMapCtrl;	/*                      MMS-EASE, MMSEASE_MOSI */
extern LOGCFG_VALUE_GROUP sxLogMaskMapCtrl;	/* MMS-LITE, ICCP-LITE, MMS-EASE, MMSEASE_MOSI */
extern LOGCFG_VALUE_GROUP tp4LogMaskMapCtrl; 	/* MMS-LITE, ICCP-LITE,           MMSEASE_MOSI */
extern LOGCFG_VALUE_GROUP usrLogMaskMapCtrl;	/* MMS-LITE                                    */

typedef struct
  {
  DBL_LNK l;
  ST_CHAR *tag;
  ST_INT dataType;
  union
    {
    ST_BOOLEAN 	b;
    ST_UINT 	uInt;
    ST_UINT 	uLong;
    ST_UINT 	uInt16;
    ST_UINT 	uInt32;
    ST_CHAR    *str;
    }u;
  } LOGCFGX_TAG_VAL;

LOGCFGX_TAG_VAL *logCfgFindTagVal (ST_CHAR *tag);
ST_VOID logCfgClearTagVals (ST_VOID);


/************************************************************************/
/************************************************************************/
/* logcfg_debug_sel bit assignments					*/
#define LOGCFG_ERR  	0x0001   
#define LOGCFG_NERR 	0x0002   
#define LOGCFG_FLOW  	0x0004   

extern ST_UINT logcfg_debug_sel;

/* Log type strings */
extern SD_CONST ST_CHAR *SD_CONST _logcfg_log_err_logstr;
extern SD_CONST ST_CHAR *SD_CONST _logcfg_log_nerr_logstr;
extern SD_CONST ST_CHAR *SD_CONST _logcfg_log_flow_logstr;
 
/************************************************************************/
/* Err log macros 			*/
#define LOGCFG_ERR0(a) \
    SLOG_0 (logcfg_debug_sel & LOGCFG_ERR,_logcfg_log_err_logstr,a)
#define LOGCFG_ERR1(a,b) \
    SLOG_1 (logcfg_debug_sel & LOGCFG_ERR,_logcfg_log_err_logstr,a,b)
#define LOGCFG_ERR2(a,b,c) \
    SLOG_2 (logcfg_debug_sel & LOGCFG_ERR,_logcfg_log_err_logstr,a,b,c)
#define LOGCFG_ERR3(a,b,c,d) \
    SLOG_3 (logcfg_debug_sel & LOGCFG_ERR,_logcfg_log_err_logstr,a,b,c,d)
#define LOGCFG_ERR4(a,b,c,d,e) \
    SLOG_4 (logcfg_debug_sel & LOGCFG_ERR,_logcfg_log_err_logstr,a,b,c,d,e)

/* Err continuation log macros		*/
#define LOGCFG_CERR0(a) \
    SLOGC_0 (logcfg_debug_sel & LOGCFG_ERR,a)
#define LOGCFG_CERR1(a,b) \
    SLOGC_1 (logcfg_debug_sel & LOGCFG_ERR,a,b)
#define LOGCFG_CERR2(a,b,c) \
    SLOGC_2 (logcfg_debug_sel & LOGCFG_ERR,a,b,c)
#define LOGCFG_CERR3(a,b,c,d) \
    SLOGC_3 (logcfg_debug_sel & LOGCFG_ERR,a,b,c,d)
#define LOGCFG_CERR4(a,b,c,d,e) \
    SLOGC_4 (logcfg_debug_sel & LOGCFG_ERR,a,b,c,d,e)

/************************************************************************/
/* NErr log macros 			*/
#define LOGCFG_NERR0(a) \
    SLOG_0 (logcfg_debug_sel & LOGCFG_NERR,_logcfg_log_nerr_logstr,a)
#define LOGCFG_NERR1(a,b) \
    SLOG_1 (logcfg_debug_sel & LOGCFG_NERR,_logcfg_log_nerr_logstr,a,b)
#define LOGCFG_NERR2(a,b,c) \
    SLOG_2 (logcfg_debug_sel & LOGCFG_NERR,_logcfg_log_nerr_logstr,a,b,c)
#define LOGCFG_NERR3(a,b,c,d) \
    SLOG_3 (logcfg_debug_sel & LOGCFG_NERR,_logcfg_log_nerr_logstr,a,b,c,d)
#define LOGCFG_NERR4(a,b,c,d,e) \
    SLOG_4 (logcfg_debug_sel & LOGCFG_NERR,_logcfg_log_nerr_logstr,a,b,c,d,e)

/* NErr continuation log macros		*/
#define LOGCFG_CNERR0(a) \
    SLOGC_0 (logcfg_debug_sel & LOGCFG_NERR,a)
#define LOGCFG_CNERR1(a,b) \
    SLOGC_1 (logcfg_debug_sel & LOGCFG_NERR,a,b)
#define LOGCFG_CNERR2(a,b,c) \
    SLOGC_2 (logcfg_debug_sel & LOGCFG_NERR,a,b,c)
#define LOGCFG_CNERR3(a,b,c,d) \
    SLOGC_3 (logcfg_debug_sel & LOGCFG_NERR,a,b,c,d)
#define LOGCFG_CNERR4(a,b,c,d,e) \
    SLOGC_4 (logcfg_debug_sel & LOGCFG_NERR,a,b,c,d,e)

/************************************************************************/
/* Flow log macros 			*/
#define LOGCFG_FLOW0(a) \
    SLOG_0 (logcfg_debug_sel & LOGCFG_FLOW,_logcfg_log_flow_logstr,a)
#define LOGCFG_FLOW1(a,b) \
    SLOG_1 (logcfg_debug_sel & LOGCFG_FLOW,_logcfg_log_flow_logstr,a,b)
#define LOGCFG_FLOW2(a,b,c) \
    SLOG_2 (logcfg_debug_sel & LOGCFG_FLOW,_logcfg_log_flow_logstr,a,b,c)
#define LOGCFG_FLOW3(a,b,c,d) \
    SLOG_3 (logcfg_debug_sel & LOGCFG_FLOW,_logcfg_log_flow_logstr,a,b,c,d)
#define LOGCFG_FLOW4(a,b,c,d,e) \
    SLOG_4 (logcfg_debug_sel & LOGCFG_FLOW,_logcfg_log_flow_logstr,a,b,c,d,e)

/* Flow continuation log macros		*/
#define LOGCFG_CFLOW0(a) \
    SLOGC_0 (logcfg_debug_sel & LOGCFG_FLOW,a)
#define LOGCFG_CFLOW1(a,b) \
    SLOGC_1 (logcfg_debug_sel & LOGCFG_FLOW,a,b)
#define LOGCFG_CFLOW2(a,b,c) \
    SLOGC_2 (logcfg_debug_sel & LOGCFG_FLOW,a,b,c)
#define LOGCFG_CFLOW3(a,b,c,d) \
    SLOGC_3 (logcfg_debug_sel & LOGCFG_FLOW,a,b,c,d)
#define LOGCFG_CFLOW4(a,b,c,d,e) \
    SLOGC_4 (logcfg_debug_sel & LOGCFG_FLOW,a,b,c,d,e)


/************************************************************************/
/************************************************************************/
/* Global variables for the SLOG library.				*/
/************************************************************************/

/* Log masks for different components of SISCO products.		*/
extern ST_UINT gs_debug_sel;
extern ST_UINT clnp_debug_sel;


extern LOG_CTRL *sLogCtrl;
extern LOG_CTRL *sErrLogCtrl;

/************************************************************************/
/* These elements are used for creating time strings			*/
/************************************************************************/

extern ST_CHAR slogTimeText[TIME_BUF_LEN];  
ST_VOID slogSetTimeText (LOG_CTRL *lc);

/************************************************************************/
/* slog header defines and buffer					*/
/************************************************************************/

#define SLOG_MAX_FNAME    	32
#define SLOG_MAX_LOGTYPESTR	40			/* logTypeStr cutoff if too long*/
#define SLOG_MAX_HDR		(2 +			\
				TIME_BUF_LEN +		\
				SLOG_MAX_LOGTYPESTR+1 +	\
				SLOG_MAX_FNAME+14 +	\
				1 + 2)

/*				(2	 		 NL , NL			*/
/*				TIME_BUF_LEN +		 slogTimeText buf size		*/
/*				SLOG_MAX_LOGTYPESTR+1 +	 limit for logTypeStr		*/
/*				SLOG_MAX_FNAME+14 +	 fName & lineNum formatting	*/
/*				1 + 2)			 NL + 2 spaces before msg	*/

ST_VOID slogTrimFileName (ST_CHAR *dest, SD_CONST ST_CHAR *fullName);
ST_VOID slogSetHdr (LOG_CTRL *lc, SD_CONST ST_INT logType, 
                    SD_CONST ST_CHAR *SD_CONST logTypeStr, 
                    SD_CONST ST_CHAR *SD_CONST sourceFile, 
                    SD_CONST ST_INT lineNum, 
                    ST_CHAR *slogHdr, ST_CHAR *lineBreak);

/************************************************************************/
/* dynamic loging function						*/
/************************************************************************/

extern ST_VOID (*slog_dyn_log_fun) (LOG_CTRL *lc, 
			SD_CONST ST_INT logType, 
                        SD_CONST ST_CHAR *SD_CONST sourceFile, 
			SD_CONST ST_INT lineNum,
                        SD_CONST ST_INT bufLen, 
			SD_CONST ST_CHAR *buf);
extern ST_VOID (*_slog_dyn_log_fun) (LOG_CTRL *lc, 
			SD_CONST ST_CHAR *timeStr, 
			SD_CONST ST_INT logType, 
			SD_CONST ST_CHAR *SD_CONST logTypeStr,
                        SD_CONST ST_CHAR *SD_CONST sourceFile, 
			SD_CONST ST_INT lineNum,
                        SD_CONST ST_INT bufLen, 
			SD_CONST ST_CHAR *buf);

/************************************************************************/
/* This function pointer is invoked during slow SLOG operations, such	*/
/* as 'clone' and 'find oldest message'					*/

extern ST_VOID (*slog_service_fun) (ST_VOID);

/************************************************************************/
/* Remote logging function						*/
/************************************************************************/

extern ST_VOID (*slog_remote_fun) (ST_UINT32 slog_remote_flags,
			LOG_CTRL *lc, 
			SD_CONST ST_INT logType, 
                        SD_CONST ST_CHAR *SD_CONST sourceFile, 
			SD_CONST ST_INT lineNum,
                        SD_CONST ST_INT bufLen, 
			SD_CONST ST_CHAR *buf);

extern ST_VOID (*_slog_remote_fun) (ST_UINT32 slog_remote_flags,
			LOG_CTRL *lc, 
			SD_CONST ST_INT logType, 
                        SD_CONST ST_CHAR *SD_CONST logTypeStr, 
                        SD_CONST ST_CHAR *SD_CONST sourceFile, 
			SD_CONST ST_INT lineNum,
                        SD_CONST ST_INT bufLen, 
			SD_CONST ST_CHAR *buf);

/* Parameter for use with remote logging function pointer		*/
/* SLOG will pass this global variable to the slog_remote_fun function	*/
extern ST_UINT32 slogRemoteFlags;

/************************************************************************/
/* Standard dynamic logging control definitions				*/
/************************************************************************/
/* These macros are OBSOLETE. They now do nothing. Previously they	*/
/* passed logType values of SLOG_DYN_PAUSE, SLOG_DYN_LF, SLOG_DYN_CLRSCR*/
/* respectively to slog.						*/

#define SLOG_PAUSE(lc,msg)
#define SLOG_LF(lc)
#define SLOG_CLRSCR(lc)

/************************************************************************/
/* Base macro types, useful for typical user logging, to reduce user	*/
/* macro verbosity							*/
/************************************************************************/

#define SLOGH1(lc,mask,id,x,y) {\
                           if (lc->logMask1 & mask)\
                             slogHex (sLogCtrl,id, thisFileName,__LINE__,x,y);}


#define SLOGH2(lc,mask,id,x,y) {\
                           if (lc->logMask2 & mask)\
                             slogHex (sLogCtrl,id, thisFileName,__LINE__,x,y);}

#define SLOGH3(lc,mask,id,x,y) {\
                           if (lc->logMask3 & mask)\
                             slogHex (sLogCtrl,id, thisFileName,__LINE__,x,y);}

#define SLOGH4(lc,mask,id,x,y) {\
                           if (lc->logMask4 & mask)\
                             slogHex (sLogCtrl,id, thisFileName,__LINE__,x,y);}

#define SLOGH5(lc,mask,id,x,y) {\
                           if (lc->logMask5 & mask)\
                             slogHex (sLogCtrl,id, thisFileName,__LINE__,x,y);}

#define SLOGH6(lc,mask,id,x,y) {\
                           if (lc->logMask6 & mask)\
                             slogHex (sLogCtrl,id, thisFileName,__LINE__,x,y);}



/* ******************* LOG MASK 1 MACROS *************************	*/

#define SLOG1_0(lc,mask,id,a) {\
                            if (lc->logMask1 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a);\
                            }

#define SLOG1_1(lc,mask,id,a,b) {\
                            if (lc->logMask1 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b);\
                            }

#define SLOG1_2(lc,mask,id,a,b,c) {\
                            if (lc->logMask1 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c);\
                            }

#define SLOG1_3(lc,mask,id,a,b,c,d) {\
                            if (lc->logMask1 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d);\
                            }

#define SLOG1_4(lc,mask,id,a,b,c,d,e) {\
                            if (lc->logMask1 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e);\
                            }

#define SLOG1_5(lc,mask,id,a,b,c,d,e,f) {\
                            if (lc->logMask1 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f);\
                            }

#define SLOG1_6(lc,mask,id,a,b,c,d,e,f,g) {\
                            if (lc->logMask1 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g);\
                            }

#define SLOG1_7(lc,mask,id,a,b,c,d,e,f,g,h) {\
                            if (lc->logMask1 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h);\
                            }

#define SLOG1_8(lc,mask,id,a,b,c,d,e,f,g,h,i) {\
                            if (lc->logMask1 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h,i);\
                            }



/* ******************* LOG MASK 2 MACROS *************************	*/
#define SLOG2_0(lc,mask,id,a) {\
                            if (lc->logMask2 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a);\
                            }

#define SLOG2_1(lc,mask,id,a,b) {\
                            if (lc->logMask2 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b);\
                            }

#define SLOG2_2(lc,mask,id,a,b,c) {\
                            if (lc->logMask2 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c);\
                            }

#define SLOG2_3(lc,mask,id,a,b,c,d) {\
                            if (lc->logMask2 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d);\
                            }

#define SLOG2_4(lc,mask,id,a,b,c,d,e) {\
                            if (lc->logMask2 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e);\
                            }

#define SLOG2_5(lc,mask,id,a,b,c,d,e,f) {\
                            if (lc->logMask2 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f);\
                            }

#define SLOG2_6(lc,mask,id,a,b,c,d,e,f,g) {\
                            if (lc->logMask2 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g);\
                            }

#define SLOG2_7(lc,mask,id,a,b,c,d,e,f,g,h) {\
                            if (lc->logMask2 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h);\
                            }

#define SLOG2_8(lc,mask,id,a,b,c,d,e,f,g,h,i) {\
                            if (lc->logMask2 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h,i);\
                            }



/* ******************* LOG MASK 3 MACROS *************************	*/
#define SLOG3_0(lc,mask,id,a) {\
                            if (lc->logMask3 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a);\
                            }

#define SLOG3_1(lc,mask,id,a,b) {\
                            if (lc->logMask3 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b);\
                            }

#define SLOG3_2(lc,mask,id,a,b,c) {\
                            if (lc->logMask3 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c);\
                            }

#define SLOG3_3(lc,mask,id,a,b,c,d) {\
                            if (lc->logMask3 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d);\
                            }

#define SLOG3_4(lc,mask,id,a,b,c,d,e) {\
                            if (lc->logMask3 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e);\
                            }

#define SLOG3_5(lc,mask,id,a,b,c,d,e,f) {\
                            if (lc->logMask3 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f);\
                            }

#define SLOG3_6(lc,mask,id,a,b,c,d,e,f,g) {\
                            if (lc->logMask3 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g);\
                            }

#define SLOG3_7(lc,mask,id,a,b,c,d,e,f,g,h) {\
                            if (lc->logMask3 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h);\
                            }

#define SLOG3_8(lc,mask,id,a,b,c,d,e,f,g,h,i) {\
                            if (lc->logMask3 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h,i);\
                            }



/* ******************* LOG MASK 4 MACROS ************************* 	*/
#define SLOG4_0(lc,mask,id,a) {\
                            if (lc->logMask4 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a);\
                            }

#define SLOG4_1(lc,mask,id,a,b) {\
                            if (lc->logMask4 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b);\
                            }

#define SLOG4_2(lc,mask,id,a,b,c) {\
                            if (lc->logMask4 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c);\
                            }

#define SLOG4_3(lc,mask,id,a,b,c,d) {\
                            if (lc->logMask4 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d);\
                            }

#define SLOG4_4(lc,mask,id,a,b,c,d,e) {\
                            if (lc->logMask4 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e);\
                            }

#define SLOG4_5(lc,mask,id,a,b,c,d,e,f) {\
                            if (lc->logMask4 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f);\
                            }

#define SLOG4_6(lc,mask,id,a,b,c,d,e,f,g) {\
                            if (lc->logMask4 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g);\
                            }

#define SLOG4_7(lc,mask,id,a,b,c,d,e,f,g,h) {\
                            if (lc->logMask4 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h);\
                            }

#define SLOG4_8(lc,mask,id,a,b,c,d,e,f,g,h,i) {\
                            if (lc->logMask4 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h,i);\
                            }




/* ******************* LOG MASK 5 MACROS *************************	*/
#define SLOG5_0(lc,mask,id,a) {\
                            if (lc->logMask5 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a);\
                            }

#define SLOG5_1(lc,mask,id,a,b) {\
                            if (lc->logMask5 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b);\
                            }

#define SLOG5_2(lc,mask,id,a,b,c) {\
                            if (lc->logMask5 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c);\
                            }

#define SLOG5_3(lc,mask,id,a,b,c,d) {\
                            if (lc->logMask5 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d);\
                            }

#define SLOG5_4(lc,mask,id,a,b,c,d,e) {\
                            if (lc->logMask5 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e);\
                            }

#define SLOG5_5(lc,mask,id,a,b,c,d,e,f) {\
                            if (lc->logMask5 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f);\
                            }

#define SLOG5_6(lc,mask,id,a,b,c,d,e,f,g) {\
                            if (lc->logMask5 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g);\
                            }

#define SLOG5_7(lc,mask,id,a,b,c,d,e,f,g,h) {\
                            if (lc->logMask5 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h);\
                            }

#define SLOG5_8(lc,mask,id,a,b,c,d,e,f,g,h,i) {\
                            if (lc->logMask5 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h,i);\
                            }



/* ******************* LOG MASK 6 MACROS ************************* 	*/
#define SLOG6_0(lc,mask,id,a) {\
                            if (lc->logMask6 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a);\
                            }

#define SLOG6_1(lc,mask,id,a,b) {\
                            if (lc->logMask6 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b);\
                            }

#define SLOG6_2(lc,mask,id,a,b,c) {\
                            if (lc->logMask6 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c);\
                            }

#define SLOG6_3(lc,mask,id,a,b,c,d) {\
                            if (lc->logMask6 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d);\
                            }

#define SLOG6_4(lc,mask,id,a,b,c,d,e) {\
                            if (lc->logMask6 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e);\
                            }

#define SLOG6_5(lc,mask,id,a,b,c,d,e,f) {\
                            if (lc->logMask6 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f);\
                            }

#define SLOG6_6(lc,mask,id,a,b,c,d,e,f,g) {\
                            if (lc->logMask6 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g);\
                            }

#define SLOG6_7(lc,mask,id,a,b,c,d,e,f,g,h) {\
                            if (lc->logMask6 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h);\
                            }

#define SLOG6_8(lc,mask,id,a,b,c,d,e,f,g,h,i) {\
                            if (lc->logMask6 & mask)\
                               slog (sLogCtrl,id, thisFileName,__LINE__,a,b,c,d,e,f,g,h,i);\
                            }



/************************************************************************/
/************************************************************************/
#if defined(DEBUG_SISCO)
/************************************************************************/

extern ST_CHAR *_slogAlwaysLogTypeStr;

#define SLOGALWAYS0(a) \
    _slog (sLogCtrl,_slogAlwaysLogTypeStr, thisFileName,__LINE__,a)
#define SLOGALWAYS1(a,b) \
    _slog (sLogCtrl,_slogAlwaysLogTypeStr, thisFileName,__LINE__,a,b)
#define SLOGALWAYS2(a,b,c) \
    _slog (sLogCtrl,_slogAlwaysLogTypeStr, thisFileName,__LINE__,a,b,c)
#define SLOGALWAYS3(a,b,c,d) \
    _slog (sLogCtrl,_slogAlwaysLogTypeStr, thisFileName,__LINE__,a,b,c,d)
#define SLOGALWAYS4(a,b,c,d,e) \
    _slog (sLogCtrl,_slogAlwaysLogTypeStr, thisFileName,__LINE__,a,b,c,d,e)
#define SLOGALWAYS5(a,b,c,d,e,f) \
    _slog (sLogCtrl,_slogAlwaysLogTypeStr, thisFileName,__LINE__,a,b,c,d,e,f)
#define SLOGALWAYS6(a,b,c,d,e,f,g) \
    _slog (sLogCtrl,_slogAlwaysLogTypeStr, thisFileName,__LINE__,a,b,c,d,e,f,g)
#define SLOGALWAYS7(a,b,c,d,e,f,g,h) \
    _slog (sLogCtrl,_slogAlwaysLogTypeStr, thisFileName,__LINE__,a,b,c,d,e,f,g,h)
#define SLOGALWAYS8(a,b,c,d,e,f,g,h,i) \
    _slog (sLogCtrl,_slogAlwaysLogTypeStr, thisFileName,__LINE__,a,b,c,d,e,f,g,h,i)
#define SLOGALWAYS9(a,b,c,d,e,f,g,h,i,j) \
    _slog (sLogCtrl,_slogAlwaysLogTypeStr, thisFileName,__LINE__,a,b,c,d,e,f,g,h,i,j)
#define SLOGALWAYS10(a,b,c,d,e,f,g,h,i,j,k) \
    _slog (sLogCtrl,_slogAlwaysLogTypeStr, thisFileName,__LINE__,a,b,c,d,e,f,g,h,i,j,k)
#define SLOGALWAYS11(a,b,c,d,e,f,g,h,i,j,k,l) \
    _slog (sLogCtrl,_slogAlwaysLogTypeStr, thisFileName,__LINE__,a,b,c,d,e,f,g,h,i,j,k,l)

#define SLOGCALWAYS0(a) \
    _slogc (sLogCtrl,a)
#define SLOGCALWAYS1(a,b) \
    _slogc (sLogCtrl,a,b)
#define SLOGCALWAYS2(a,b,c) \
    _slogc (sLogCtrl,a,b,c)
#define SLOGCALWAYS3(a,b,c,d) \
    _slogc (sLogCtrl,a,b,c,d)
#define SLOGCALWAYS4(a,b,c,d,e) \
    _slogc (sLogCtrl,a,b,c,d,e)
#define SLOGCALWAYS5(a,b,c,d,e,f) \
    _slogc (sLogCtrl,a,b,c,d,e,f)
#define SLOGCALWAYS6(a,b,c,d,e,f,g) \
    _slogc (sLogCtrl,a,b,c,d,e,f,g)
#define SLOGCALWAYS7(a,b,c,d,e,f,g,h) \
    _slogc (sLogCtrl,a,b,c,d,e,f,g,h)
#define SLOGCALWAYS8(a,b,c,d,e,f,g,h,i) \
    _slogc (sLogCtrl,a,b,c,d,e,f,g,h,i)

#define SLOGALWAYSH(numBytes,dataPtr) \
    _slogHex (sLogCtrl, numBytes, dataPtr)

#define SLOGALWAYSS(dataPtr) \
    _slogStr (sLogCtrl, dataPtr)

/************************************************************************/
#else	/* #if defined(DEBUG_SISCO) */
/************************************************************************/

#define SLOGALWAYS0(a)
#define SLOGALWAYS1(a,b)
#define SLOGALWAYS2(a,b,c)
#define SLOGALWAYS3(a,b,c,d)
#define SLOGALWAYS4(a,b,c,d,e)
#define SLOGALWAYS5(a,b,c,d,e,f)
#define SLOGALWAYS6(a,b,c,d,e,f,g)
#define SLOGALWAYS7(a,b,c,d,e,f,g,h)
#define SLOGALWAYS8(a,b,c,d,e,f,g,h,i)
#define SLOGALWAYS9(a,b,c,d,e,f,g,h,i,j)
#define SLOGALWAYS10(a,b,c,d,e,f,g,h,i,j,k)
#define SLOGALWAYS11(a,b,c,d,e,f,g,h,i,j,k,l)
#define SLOGCALWAYS0(a)
#define SLOGCALWAYS1(a,b)
#define SLOGCALWAYS2(a,b,c)
#define SLOGCALWAYS3(a,b,c,d)
#define SLOGCALWAYS4(a,b,c,d,e)
#define SLOGCALWAYS5(a,b,c,d,e,f)
#define SLOGCALWAYS6(a,b,c,d,e,f,g)
#define SLOGCALWAYS7(a,b,c,d,e,f,g,h)
#define SLOGCALWAYS8(a,b,c,d,e,f,g,h,i)
#define SLOGALWAYSH(numBytes,dataPtr)
#define SLOGALWAYSS(dataPtr)

#endif
/************************************************************************/

/************************************************************************/
/* "Base" set of macros.						*/
/* All other LOG macros should call these macros.			*/
/* TRICK: These macros all use "do{...}while(0)".			*/
/*        This works in any context without adding extra {}.		*/
/************************************************************************/

#ifdef DEBUG_SISCO
/* Basic log macros (all call _slog)	*/
#define SLOG_0(ena,str,fmt) \
	do {if (ena) _slog (sLogCtrl,str,thisFileName,__LINE__,fmt);} while(0)

#define SLOG_1(ena,str,fmt,a) \
	do {if (ena) _slog (sLogCtrl,str,thisFileName,__LINE__,fmt,a);} while(0)

#define SLOG_2(ena,str,fmt,a,b) \
	do {if (ena) _slog (sLogCtrl,str,thisFileName,__LINE__,fmt,a,b);} while(0)

#define SLOG_3(ena,str,fmt,a,b,c) \
	do {if (ena) _slog (sLogCtrl,str,thisFileName,__LINE__,fmt,a,b,c);} while(0)

#define SLOG_4(ena,str,fmt,a,b,c,d) \
	do {if (ena) _slog (sLogCtrl,str,thisFileName,__LINE__,fmt,a,b,c,d);} while(0)

#define SLOG_5(ena,str,fmt,a,b,c,d,e) \
	do {if (ena) _slog (sLogCtrl,str,thisFileName,__LINE__,fmt,a,b,c,d,e);} while(0)

#define SLOG_6(ena,str,fmt,a,b,c,d,e,f) \
	do {if (ena) _slog (sLogCtrl,str,thisFileName,__LINE__,fmt,a,b,c,d,e,f);} while(0)

#define SLOG_7(ena,str,fmt,a,b,c,d,e,f,g) \
	do {if (ena) _slog (sLogCtrl,str,thisFileName,__LINE__,fmt,a,b,c,d,e,f,g);} while(0)

#define SLOG_8(ena,str,fmt,a,b,c,d,e,f,g,h) \
	do {if (ena) _slog (sLogCtrl,str,thisFileName,__LINE__,fmt,a,b,c,d,e,f,g,h);} while(0)

/* Continuation macros (all call _slogc).	*/
#define SLOGC_0(ena,fmt) \
	do {if (ena) _slogc (sLogCtrl,fmt);} while(0)

#define SLOGC_1(ena,fmt,a) \
	do {if (ena) _slogc (sLogCtrl,fmt,a);} while(0)

#define SLOGC_2(ena,fmt,a,b) \
	do {if (ena) _slogc (sLogCtrl,fmt,a,b);} while(0)

#define SLOGC_3(ena,fmt,a,b,c) \
	do {if (ena) _slogc (sLogCtrl,fmt,a,b,c);} while(0)

#define SLOGC_4(ena,fmt,a,b,c,d) \
	do {if (ena) _slogc (sLogCtrl,fmt,a,b,c,d);} while(0)

#define SLOGC_5(ena,fmt,a,b,c,d,e) \
	do {if (ena) _slogc (sLogCtrl,fmt,a,b,c,d,e);} while(0)

#define SLOGC_6(ena,fmt,a,b,c,d,e,f) \
	do {if (ena) _slogc (sLogCtrl,fmt,a,b,c,d,e,f);} while(0)

#define SLOGC_7(ena,fmt,a,b,c,d,e,f,g) \
	do {if (ena) _slogc (sLogCtrl,fmt,a,b,c,d,e,f,g);} while(0)

#define SLOGC_8(ena,fmt,a,b,c,d,e,f,g,h) \
	do {if (ena) _slogc (sLogCtrl,fmt,a,b,c,d,e,f,g,h);} while(0)

/* Hex logging macro (calls _slogHex)	*/
#define SLOGH(ena,num,ptr) \
	do {if (ena) _slogHex (sLogCtrl,num,ptr);} while(0)

#else	/* !DEBUG_SISCO	*/

/* Non-DEBUG macros for EVERYONE! Yeah!! Never have to create another one.*/
/* Basic log macros (do nothing)	*/
#define SLOG_0(ena,str,fmt)
#define SLOG_1(ena,str,fmt,a)
#define SLOG_2(ena,str,fmt,a,b)
#define SLOG_3(ena,str,fmt,a,b,c)
#define SLOG_4(ena,str,fmt,a,b,c,d)
#define SLOG_5(ena,str,fmt,a,b,c,d,e)
#define SLOG_6(ena,str,fmt,a,b,c,d,e,f)
#define SLOG_7(ena,str,fmt,a,b,c,d,e,f,g)
#define SLOG_8(ena,str,fmt,a,b,c,d,e,f,g,h)

/* Continuation macros (do nothing).	*/
#define SLOGC_0(ena,fmt)
#define SLOGC_1(ena,fmt,a)
#define SLOGC_2(ena,fmt,a,b)
#define SLOGC_3(ena,fmt,a,b,c)
#define SLOGC_4(ena,fmt,a,b,c,d)
#define SLOGC_5(ena,fmt,a,b,c,d,e)
#define SLOGC_6(ena,fmt,a,b,c,d,e,f)
#define SLOGC_7(ena,fmt,a,b,c,d,e,f,g)
#define SLOGC_8(ena,fmt,a,b,c,d,e,f,g,h)

/* Hex logging macro (do nothing)	*/
#define SLOGH(ena,num,ptr)
#endif	/* !DEBUG_SISCO	*/


/************************************************************************/
/* "Base" set of macros for error log					*/
/************************************************************************/

#ifdef DEBUG_SISCO
/* Basic log macros (all call _slog)	*/
#define SELOG_0(ena,str,fmt) \
	do {if (ena) _slog (sErrLogCtrl,str,thisFileName,__LINE__,fmt);} while(0)

#define SELOG_1(ena,str,fmt,a) \
	do {if (ena) _slog (sErrLogCtrl,str,thisFileName,__LINE__,fmt,a);} while(0)

#define SELOG_2(ena,str,fmt,a,b) \
	do {if (ena) _slog (sErrLogCtrl,str,thisFileName,__LINE__,fmt,a,b);} while(0)

#define SELOG_3(ena,str,fmt,a,b,c) \
	do {if (ena) _slog (sErrLogCtrl,str,thisFileName,__LINE__,fmt,a,b,c);} while(0)

#define SELOG_4(ena,str,fmt,a,b,c,d) \
	do {if (ena) _slog (sErrLogCtrl,str,thisFileName,__LINE__,fmt,a,b,c,d);} while(0)

#define SELOG_5(ena,str,fmt,a,b,c,d,e) \
	do {if (ena) _slog (sErrLogCtrl,str,thisFileName,__LINE__,fmt,a,b,c,d,e);} while(0)

#define SELOG_6(ena,str,fmt,a,b,c,d,e,f) \
	do {if (ena) _slog (sErrLogCtrl,str,thisFileName,__LINE__,fmt,a,b,c,d,e,f);} while(0)

#define SELOG_7(ena,str,fmt,a,b,c,d,e,f,g) \
	do {if (ena) _slog (sErrLogCtrl,str,thisFileName,__LINE__,fmt,a,b,c,d,e,f,g);} while(0)

#define SELOG_8(ena,str,fmt,a,b,c,d,e,f,g,h) \
	do {if (ena) _slog (sErrLogCtrl,str,thisFileName,__LINE__,fmt,a,b,c,d,e,f,g,h);} while(0)

/* Continuation macros (all call _slogc).	*/
#define SELOGC_0(ena,fmt) \
	do {if (ena) _slogc (sErrLogCtrl,fmt);} while(0)

#define SELOGC_1(ena,fmt,a) \
	do {if (ena) _slogc (sErrLogCtrl,fmt,a);} while(0)

#define SELOGC_2(ena,fmt,a,b) \
	do {if (ena) _slogc (sErrLogCtrl,fmt,a,b);} while(0)

#define SELOGC_3(ena,fmt,a,b,c) \
	do {if (ena) _slogc (sErrLogCtrl,fmt,a,b,c);} while(0)

#define SELOGC_4(ena,fmt,a,b,c,d) \
	do {if (ena) _slogc (sErrLogCtrl,fmt,a,b,c,d);} while(0)

#define SELOGC_5(ena,fmt,a,b,c,d,e) \
	do {if (ena) _slogc (sErrLogCtrl,fmt,a,b,c,d,e);} while(0)

#define SELOGC_6(ena,fmt,a,b,c,d,e,f) \
	do {if (ena) _slogc (sErrLogCtrl,fmt,a,b,c,d,e,f);} while(0)

#define SELOGC_7(ena,fmt,a,b,c,d,e,f,g) \
	do {if (ena) _slogc (sErrLogCtrl,fmt,a,b,c,d,e,f,g);} while(0)

#define SELOGC_8(ena,fmt,a,b,c,d,e,f,g,h) \
	do {if (ena) _slogc (sErrLogCtrl,fmt,a,b,c,d,e,f,g,h);} while(0)

/* Hex logging macro (calls _slogHex)	*/
#define SELOGH(ena,num,ptr) \
	do {if (ena) _slogHex (sErrLogCtrl,num,ptr);} while(0)

#else	/* !DEBUG_SISCO	*/

/* Non-DEBUG macros for EVERYONE! Yeah!! Never have to create another one.*/
/* Basic log macros (do nothing)	*/
#define SELOG_0(ena,str,fmt)
#define SELOG_1(ena,str,fmt,a)
#define SELOG_2(ena,str,fmt,a,b)
#define SELOG_3(ena,str,fmt,a,b,c)
#define SELOG_4(ena,str,fmt,a,b,c,d)
#define SELOG_5(ena,str,fmt,a,b,c,d,e)
#define SELOG_6(ena,str,fmt,a,b,c,d,e,f)
#define SELOG_7(ena,str,fmt,a,b,c,d,e,f,g)
#define SELOG_8(ena,str,fmt,a,b,c,d,e,f,g,h)

/* Continuation macros (do nothing).	*/
#define SELOGC_0(ena,fmt)
#define SELOGC_1(ena,fmt,a)
#define SELOGC_2(ena,fmt,a,b)
#define SELOGC_3(ena,fmt,a,b,c)
#define SELOGC_4(ena,fmt,a,b,c,d)
#define SELOGC_5(ena,fmt,a,b,c,d,e)
#define SELOGC_6(ena,fmt,a,b,c,d,e,f)
#define SELOGC_7(ena,fmt,a,b,c,d,e,f,g)
#define SELOGC_8(ena,fmt,a,b,c,d,e,f,g,h)

/* Hex logging macro (do nothing)	*/
#define SELOGH(ena,num,ptr)
#endif	/* !DEBUG_SISCO	*/


#ifdef __cplusplus
}
#endif

#endif  /* end of 'already included' 	*/

