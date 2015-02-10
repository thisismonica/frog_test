/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*     		1999 - 2005, All Rights Reserved		        */
/*									*/
/* MODULE NAME : sx_dec.c						*/
/* PRODUCT(S)  : 							*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 07/26/05  JRB     72    Del global vars CODE_APOS_LEN, etc. Use #define.*/
/*			   Del unused global vars CHAR_APOS_LEN, etc.	*/
/* 05/23/05  EJV     71    Added sxLogMaskMapCtrl for parsing logcfg.xml*/
/* 05/06/05  EJV     70    sx_parseExx_mt: open file in read-only mode.	*/
/* 04/15/05  ASK     69    Fix unreferenced var in XmlStringToDuration	*/
/* 04/01/05  DWL     68    Duration fix.				*/
/* 02/23/05  DWL     67    Clean up a warning message on VXWORKS.	*/
/* 08/16/04  JRB     66    Del non-thread-safe sx_parse.. functions.	*/
/*			   Del unneeded system includes.		*/
/* 07/28/04  MDE     65    Do not reset attrCount for end tag 		*/
/* 07/14/04  DWL     64    Added handling for Duration type		*/
/* 			   Added decode functions for struct tm type.	*/
/* 04/19/04  DWL     63    Don't log improper nesting error if error	*/
/*			   condition already exists.			*/
/* 04/07/04  MDE     62    Switched to fopen for LINUX			*/
/* 03/31/04  MDE     61    Added the Exx functions			*/
/* 03/17/04  EJV     60    Use SISCO types in place of C types.		*/
/* 03/11/04  RWM     59    Fixed bug in file decoding. Init buffer used */
/*                         to NULLs					*/
/* 02/17/04  DSF     58    Use FLOW logging to log file open error	*/
/* 02/16/04  MDE     57    Eliminate false 'Invalid Nesting' log	*/
/* 11/21/03  DSF     56    Use FLOW logging to log file open error	*/
/* 10/24/03  DSF     55    Added missing functions			*/
/*			   Fixed SX_DATE_TIME functions			*/
/* 10/14/03  JRB     54    Eliminate compile warnings.			*/
/* 09/03/03  EJV     53    Added sx_get_string_YesNo,			*/
/*			         sx_get_string_OnOff_mask,		*/
/*                               sx_get_string_OnOff_bool. 		*/
/*			   Small changes in logging.			*/
/* 09/01/03  GLB     52    Added "sx_get_xtime"                         */
/* 06/09/03  DSF     51    Added SX_FILE_NOT_FOUND,			*/
/*			   changed SX_ERR_CONVERT			*/
/* 05/15/03  RWM     50    Renamed sx_format_string to                  */
/*                         sx_format_string_dec                         */
/* 05/05/03  DSF     49    Fixed sx_find_element_contents for empty	*/
/*			   elements					*/
/* 04/16/03  DWL/RWM 48    Fixed bug when can not find starting element */
/* 04/10/03  DSF     47    Use strnstr in place of strstr		*/
/* 04/08/03  RWM     46    Fixed bug where u_sx_el_start was being      */
/*                         passed as last parameter to sx_parseEx_mt    */
/*                         instead of u_sx_el_end in the function       */
/*                         sx_parseEx                                   */
/* 04/04/03  RWM     45    Removed unnecessary use of u_sx_el_start and */
/*                         u_sx_el_end, also fixed minor bug in         */
/*                         sxStartElement                               */
/* 04/03/03  DSF     44    Set tagStart in sx_rip_xml			*/
/* 03/21/03  RWM     43    Added function sx_get_uint16 and fixed minor */
/*                         sxStartElement bug                           */
/* 03/11/03  DSF     42    Added termFlag to SX_DEC_CTRL		*/
/*			   Corrected setting of xmlPos			*/
/* 07/02/02  DSF     41    Spelling					*/
/* 06/28/02  RWM     41    Added function sx_get_time_ex                */
/* 06/19/02  DSF     40    Rearranged DONE and NOT_DONE macros so	*/
/*			   BoundsChecker doesn't complain		*/
/* 04/30/02  GLB     39    reverse: sx_get_double: '%e' with '%le'     .*/
/* 02/27/02  EJV     38    sxStartElement, sxEndElement: init rc.	*/
/*			   sx_rip_xml: ret error if can't read from file*/
/*			   sx_rip_xml: '*xml' schould be used not 'c'	*/
/*			   sx_get_uint, sx_get_uint_attr: corr return	*/
/*			   sx_get_double: repl non-ANSI '%le' with '%e'.*/
/* 01/07/02  MDE     37    Now check SX_ELF_OPT bit when popping	*/
/* 12/04/01  DSF     36    Skip	white space between the '=' and an	*/
/*			   attribute value				*/
/* 11/28/01  DSF     35    Allow the user to ignore namespaces in	*/
/*			   element and attribute names			*/
/* 11/21/01  MDE     34    Moved elemBuf to SX_DEC_CONTROL		*/
/* 08/22/01  DSF     33    Set elUser before calling thru funcPtr	*/
/* 06/25/01  EJV     32    For thread safe operation:			*/
/*			   Replaced glb _stringBuf with elemBuf def in	*/
/*			   struct SX_DEC_ELEMENT_INFO.			*/
/*			   sxStartElement: changed _NERR to _CDEC log.	*/
/*			   Redefined MAX_BYTES_IN_CHUNK to correlate max*/
/*			   elem len with parsing XML in memory.		*/
/*			   Added twin funcs: sx_parse_mt, sx_parseEx_mt.*/
/* 06/29/01  RWM     31    Fixed parser to handle attributes delineated */
/*                         by single quotes                             */
/* 05/21/01  MDE     30    Save stack level during auto-pop		*/
/* 05/18/01  RKR     29    Changed sx_get_alloc_string to alloc a string*/
/* 05/11/01  JRB     28    Pass fileName, not (FILE *) to sx_parseEx.	*/
/* 04/23/01  RWM     27    Improved efficiency of sx_format_string      */
/* 04/18/01  RWM     26    Added function sx_format_string              */
/* 03/27/01  MDE     25    Changed C++ comment style to C		*/
/* 03/13/01  GLB     24    Only use calloc, malloc & free               */
/* 02/26/01  MDE     23    Now set u_sx_el_start/end to NULL when done  */
/* 02/12/01  RWM     22    Modified sx_rip_xml to handle files and      */
/*                         removed DEBUG_SISCO requirement for sx log   */
/*                         string constants                             */
/* 02/02/01  EJV     21    sx_get_time: changed t to time_t from ST_LONG*/
/* 01/04/01  EJV     20    sx_get_bitstring: substituted pow() with '<<'*/
/*			   sx_get_ulong, sx_get_ulong_attr:		*/
/*			     changed "%ul" to "%lu".			*/
/* 01/02/01  DWL     19    Removed UIB_DEFINED check			*/
/* 12/05/00  MDE     18    Now check for correct end tag		*/
/* 12/04/00  MDE     17    Corrected DOCTYPE handling			*/
/* 12/04/00  KCR     16    Modified Detail Logging to use SXLOG_DEC	*/
/* 11/13/00  DWL     15    Ignore DOCTYPE line in sx_rip_xml		*/
/* 10/30/00  MDE     14    Added sx_find_element_contents		*/
/* 10/30/00  MDE     13    Added sx_get_element_contents		*/
/* 10/30/00  MDE     12    Added unknown element handling		*/
/* 09/29/00  DWL     11    Created new error code to allow parse to 	*/
/*			   continue or stop				*/
/* 09/20/00  DWL     10    Added support for CDATA between tags		*/
/* 08/15/00  DWL     09    Added support for empty XML tags, removed	*/
/*			   check for MAX_MSG_SIZE			*/
/* 08/11/00  DWL     08    Added define UIB_DEFINED, to support		*/
/*			   sx_get_time function in UIB project		*/
/* 06/06/00  DWL     07    Removed TDL support, fixed buffer overread	*/
/* 05/30/00  DWL     06    Beefed up comment support			*/
/* 05/09/00  MDE     05    V1.00P3 changes				*/
/* 04/12/00  MDE     04    Changes for V1.00P2 release 			*/
/* 03/31/00  DWL     02    Removed C++ comments for QNX			*/
/* 03/20/00  DWL     02    Updated to allow TDL's with angle brackets	*/
/*			   within XML statements			*/
/* 09/15/99          01    Module created				*/
/************************************************************************/

#include "glbtypes.h"
#include "sysincs.h"
#include "mem_chk.h"
#include "sx_defs.h"
#include "sx_log.h"
#include "time_str.h"
#include "str_util.h"

/************************************************************************/
/* For debug version, use a static pointer to avoid duplication of 	*/
/* __FILE__ strings.							*/

#ifdef DEBUG_SISCO
SD_CONST static ST_CHAR *SD_CONST thisFileName = __FILE__;
#endif

/************************************************************************/

SD_CONST ST_CHAR *SD_CONST _sx_log_dec_logstr = "SX_LOG_DEC";
SD_CONST ST_CHAR *SD_CONST _sx_log_enc_logstr = "SX_LOG_ENC";
SD_CONST ST_CHAR *SD_CONST _sx_log_flow_logstr = "SX_LOG_FLOW";
SD_CONST ST_CHAR *SD_CONST _sx_log_debug_logstr = "SX_LOG_DEBUG";
SD_CONST ST_CHAR *SD_CONST _sx_log_err_logstr = "SX_LOG_ERR";
SD_CONST ST_CHAR *SD_CONST _sx_log_nerr_logstr = "SX_LOG_NERR";

ST_UINT sx_debug_sel = SX_LOG_ERR | SX_LOG_NERR;

#ifdef DEBUG_SISCO
LOGCFGX_VALUE_MAP sxLogMaskMaps[] =
  {
    {"SX_LOG_ERR",	SX_LOG_ERR,	&sx_debug_sel, _LOGCFG_DATATYPE_UINT_MASK, "Error"},
    {"SX_LOG_NERR",	SX_LOG_NERR,	&sx_debug_sel, _LOGCFG_DATATYPE_UINT_MASK, "Notice"},
    {"SX_LOG_FLOW",	SX_LOG_FLOW,	&sx_debug_sel, _LOGCFG_DATATYPE_UINT_MASK, "Flow"},
    {"SX_LOG_DEC",	SX_LOG_DEC,	&sx_debug_sel, _LOGCFG_DATATYPE_UINT_MASK, "Decode"},
    {"SX_LOG_ENC",	SX_LOG_ENC,	&sx_debug_sel, _LOGCFG_DATATYPE_UINT_MASK, "Encode"},
    {"SX_LOG_DEBUG",	SX_LOG_DEBUG,	&sx_debug_sel, _LOGCFG_DATATYPE_UINT_MASK, "Debug"},
  };

LOGCFG_VALUE_GROUP sxLogMaskMapCtrl =
  {
  {NULL,NULL},
  "XML",
  sizeof(sxLogMaskMaps)/sizeof(LOGCFGX_VALUE_MAP),
  sxLogMaskMaps
  };
#endif /* DEBUG_SISCO */


/************************************************************************/

ST_BOOLEAN sxUseSax;
ST_BOOLEAN sxIgnoreNS;

ST_RET sx_rip_xml (SX_DEC_CTRL *sxDecCtrl);

SX_ELEMENT *_uibed_find_element (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tagName);

/************************************************************************/
/*				sx_parseExx_mt 				*/
/* Note: when making changes/correction to this function revise also	*/
/*       other sx_parse functions.					*/
/* Decode the XML located in file by loading the whole thing into	*/
/* memory first ...							*/
/************************************************************************/

ST_RET sx_parseExx_mt (ST_CHAR *fileName, ST_INT numItems, 
		   SX_ELEMENT *itemTbl, ST_VOID *usr,
                   ST_RET (*u_sx_el_start_fun) (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag),
                   ST_RET (*u_sx_el_end_fun) (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag))
  {
#if defined(_C6X) || defined(_C3X) || defined(_PPC)
    /* added by ZJY 2006-06-03 */
    /* todo: For C3X */
    return SD_SUCCESS;
#else
ST_RET rc;
ST_LONG fileSize;
ST_LONG bytesRead;
ST_CHAR *cfgData;
struct stat buf;
int result;
FILE *fp;

/* Get the size of the file */
  fp = fopen (fileName, "rt");
  if (fp == NULL)
    {
    SXLOG_FLOW1 ("XML File (%s) Open Error",fileName);
    return (SX_FILE_NOT_FOUND);
    }
  result = fstat (fileno (fp), &buf);
  if (result != 0)
    return (SD_FAILURE);

/* Allocate a buffer and read all into memory */
  fileSize = buf.st_size;
  cfgData = chk_malloc (fileSize);

  bytesRead = fread (cfgData, 1, fileSize, fp);
  fclose (fp);
  if (bytesRead <= 0)
    {
    chk_free (cfgData);
    SXLOG_FLOW1 ("Error: Could not read from '%s'", fileName);
    return (SX_FILE_NOT_FOUND);
    }

  rc = sx_parse_mt (bytesRead, cfgData, numItems, itemTbl, usr, 
  		    u_sx_el_start_fun, u_sx_el_end_fun); 
  if (rc != SD_SUCCESS)
    {
    SXLOG_FLOW1 ("ERROR: parsing failed, return code: '%d'", rc);
    }

  chk_free (cfgData);
  return (rc);
#endif
  }
  
/************************************************************************/
/*				sx_parse_mt				*/
/* Note: when making changes/correction to this function revise also	*/
/*       other sx_parse functions.					*/
/* Decode the XML located in memory (thread safe version).		*/
/************************************************************************/
ST_RET sx_parse_mt (ST_LONG lMsgLen, ST_CHAR *xml, ST_INT numItems, 
		 SX_ELEMENT *itemTbl, ST_VOID *usr,
                 ST_RET (*u_sx_el_start_fun) (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag),
                 ST_RET (*u_sx_el_end_fun) (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag))
{
SX_DEC_CTRL *sxDecCtrl;
ST_RET rc;

  SXLOG_FLOW0 ("Start Decoding XML");
  SXLOG_FLOWH ((ST_INT) lMsgLen, xml);

  sxDecCtrl = (SX_DEC_CTRL *) calloc (1, sizeof (SX_DEC_CTRL));
			/* !we need to use here the system calloc */

  sx_push (sxDecCtrl, numItems, itemTbl, SD_FALSE);
  sxDecCtrl->xmlStart = xml;
  sxDecCtrl->xmlLen = lMsgLen;
  sxDecCtrl->ignoreNS = sxIgnoreNS;
  sxDecCtrl->useFp = SD_FALSE;
  sxDecCtrl->usr = usr;
  sxDecCtrl->u_sx_el_start = u_sx_el_start_fun;
  sxDecCtrl->u_sx_el_end   = u_sx_el_end_fun;
 
  sx_rip_xml (sxDecCtrl);

  if (sxDecCtrl->xmlNestLevel != 0 && sxDecCtrl->errCode == SD_SUCCESS)
    {
    sxDecCtrl->errCode = SX_XML_MALFORMED;
    SXLOG_NERR0 ("Invalid XML nesting");
    }

  rc = sxDecCtrl->errCode;

  free (sxDecCtrl);

  return (rc);
}

/************************************************************************/
/*				sx_parseEx_mt				*/
/* Note: when making changes/correction to this function revise also	*/
/*       other sx_parse functions.					*/
/* Decode the XML located in file (thread safe version).		*/
/************************************************************************/
ST_RET sx_parseEx_mt (ST_CHAR *fileName, ST_INT numItems, 
                 SX_ELEMENT *itemTbl, ST_VOID *usr,
                 ST_RET (*u_sx_el_start_fun) (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag),
                 ST_RET (*u_sx_el_end_fun) (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag))
{
SX_DEC_CTRL *sxDecCtrl;
ST_RET rc;
FILE *fp;

  SXLOG_FLOW1 ("Start Decoding XML file %s", fileName);
  if ((fp = fopen (fileName,"r"))==NULL)
    {
    SXLOG_FLOW1 ("XML File (%s) Open Error",fileName);
    return (SX_FILE_NOT_FOUND);
    }
  
  sxDecCtrl = (SX_DEC_CTRL *) calloc (1, sizeof (SX_DEC_CTRL));
			/* !we need to use here the system calloc */

  sx_push (sxDecCtrl, numItems, itemTbl, SD_FALSE);
  sxDecCtrl->fp = fp;
  sxDecCtrl->ignoreNS = sxIgnoreNS;
  sxDecCtrl->useFp = SD_TRUE;
  sxDecCtrl->usr = usr;
  sxDecCtrl->u_sx_el_start = u_sx_el_start_fun;
  sxDecCtrl->u_sx_el_end   = u_sx_el_end_fun;
 
  sx_rip_xml (sxDecCtrl);

  if (sxDecCtrl->errCode == 0)
    {
    if (sxDecCtrl->xmlNestLevel != 0)
      {
      sxDecCtrl->errCode = SX_XML_MALFORMED;
      SXLOG_NERR0 ("Invalid XML nesting");
      }
    }

  rc = sxDecCtrl->errCode;

  free (sxDecCtrl);

  fclose (fp);
  return (rc);
}


/************************************************************************/
/*			sxStartElement 					*/
/************************************************************************/

ST_VOID sxStartElement (SX_DEC_CTRL *sxDecCtrl) 
  {
SX_ELEMENT *item;
SX_DEC_ELEMENT_INFO *sxDecElInfo;
ST_CHAR *tag;
ST_RET rc = SD_FAILURE;
ST_INT stackLevelSave;

  sxDecElInfo = &sxDecCtrl->sxDecElInfo;
  tag = sxDecElInfo->tag;
  SXLOG_DEC1 ("Start element '%s'", tag);
  if (sxDecCtrl->errCode != SD_SUCCESS && sxDecCtrl->errCode != SX_ERR_CONVERT)
    {
    return;
    }

  item = _uibed_find_element (sxDecCtrl, tag);
  
  stackLevelSave = sxDecCtrl->itemStackLevel;
  while (item == NULL && sxDecCtrl->itemStackLevel > 0)
    {
    if (sxDecCtrl->auto_pop[sxDecCtrl->itemStackLevel-1] == SD_TRUE)
      {
      sx_pop (sxDecCtrl);
      item = _uibed_find_element (sxDecCtrl, tag);
      }
    else
      break;
    }

  if (item != NULL)
    {
    if (item->numOcc != 0 && 
        ((item->elementFlags & SX_ELF_RPT) == 0))
      {
      sxDecCtrl->errCode = SX_DUPLICATE_NOT_ALLOWED;
      SXLOG_NERR1 ("Duplicate of element '%s' not allowed",  tag);
      return;       
      }
    ++item->numOcc;
    if (item->numOcc > 1)
      {
      SXLOG_CDEC1 ("Number occurences: %d", item->numOcc);
      }

  /* Save the item for later */
    ++sxDecCtrl->xmlNestLevel;
    sxDecCtrl->elTbl[sxDecCtrl->xmlNestLevel] = item;

  /* Call the user function, if there is one ... */
    sxDecCtrl->item = item;
    sxDecCtrl->reason = SX_ELEMENT_START;
    if ((item->elementFlags & SX_ELF_CSTART) != 0)
	{
		if (item->funcPtr != NULL)
        {
			sxDecCtrl->elUser = item->user;
			(item->funcPtr)(sxDecCtrl);
		}
		else
        {
			SXLOG_CDEC0 ("No state function for this element");
        }
	}
  }
  else if (sxDecCtrl->u_sx_el_start != NULL)
  {
	  sxDecCtrl->itemStackLevel = stackLevelSave;
	  
	  if (sxDecCtrl->u_sx_el_start != NULL)
		  rc = (*(sxDecCtrl->u_sx_el_start)) (sxDecCtrl, tag);
	  if (rc == SD_SUCCESS)
      {
		  ++sxDecCtrl->xmlNestLevel;
		  sxDecCtrl->elTbl[sxDecCtrl->xmlNestLevel] = NULL;
      }
	  else
      {
		  sxDecCtrl->errCode = SX_STRUCT_NOT_FOUND;
		  SXLOG_CDEC1 ("u_sx_el_start failed for element '%s'", tag);
      }
  }
  else
  {
	  sxDecCtrl->errCode = SX_STRUCT_NOT_FOUND;
	  SXLOG_NERR1 ("Could not find element '%s' in element table", tag);
  }
}

/************************************************************************/
/*			sxEndElement 					*/
/************************************************************************/

ST_VOID sxEndElement (SX_DEC_CTRL *sxDecCtrl)
  {
SX_DEC_ELEMENT_INFO *sxDecElInfo;
SX_ELEMENT *item;
ST_CHAR *tag;
ST_RET rc = SD_FAILURE;

  sxDecElInfo = &sxDecCtrl->sxDecElInfo;
  tag = sxDecElInfo->tag;
  SXLOG_DEC1 ("End element   '%s'", tag);
  if (sxDecCtrl->errCode != SD_SUCCESS && sxDecCtrl->errCode != SX_ERR_CONVERT)
    {
    return;
    }

  item = sxDecCtrl->elTbl[sxDecCtrl->xmlNestLevel];
  --sxDecCtrl->xmlNestLevel;

  if (item != NULL)
    {
    if (strcmp (tag, item->tag) != 0) /* verify end tag */
      {
      sxDecCtrl->errCode = SX_XML_MALFORMED;
      SXLOG_NERR2 ("XML malformed: found </%s>, expected </%s>", tag, item->tag);
      }
    else
      {
      if ((item->elementFlags & SX_ELF_CEND) != 0)
        {
        sxDecCtrl->item = item;
        sxDecCtrl->reason = SX_ELEMENT_END;
        if (item->funcPtr != NULL)
	  {
  	  sxDecCtrl->elUser = item->user;
          (item->funcPtr)(sxDecCtrl);
	  }
        else
          {
          SXLOG_CDEC0 ("No state function for this element");
          }
        }
      }
    }
  else
    {
    if (sxDecCtrl->u_sx_el_end != NULL)
      {
      if (sxDecCtrl->u_sx_el_end != NULL)
        rc = (*(sxDecCtrl->u_sx_el_end)) (sxDecCtrl, sxDecCtrl->sxDecElInfo.tag);
      if (rc != SD_SUCCESS)
        {
        sxDecCtrl->errCode = SX_STRUCT_NOT_FOUND;
        SXLOG_NERR1 ("u_sx_el_start failed for element '%s'", tag);
        }
      }
    }
  }

/************************************************************************/
/************************************************************************/
/*				sx_push 				*/
/************************************************************************/

ST_VOID sx_push (SX_DEC_CTRL *sxDecCtrl, ST_INT numItems, SX_ELEMENT *itemTbl,
		ST_BOOLEAN auto_pop)
  {
ST_INT i;
SX_ELEMENT_TBL_CTRL *itemTblCtrl;

  if (sxDecCtrl->itemStackLevel >= SX_MAX_STACK_LEVEL)
    return;

  itemTblCtrl = &sxDecCtrl->items[sxDecCtrl->itemStackLevel];
  itemTblCtrl->itemTbl= itemTbl;
  itemTblCtrl->numItems= numItems;
  sxDecCtrl->auto_pop[sxDecCtrl->itemStackLevel] = auto_pop;
  ++sxDecCtrl->itemStackLevel;

  for (i = 0; i < numItems; ++i)
    itemTbl[i].numOcc = 0;
  }

/************************************************************************/
/*				sx_pop 					*/
/************************************************************************/

ST_VOID sx_pop (SX_DEC_CTRL *sxDecCtrl)
  {
SX_ELEMENT_TBL_CTRL *itemTblCtrl;
SX_ELEMENT *item;
ST_INT i;

/* Check for mandatory elements */
  if (sxDecCtrl->itemStackLevel > 0)
    {
    --sxDecCtrl->itemStackLevel;
    itemTblCtrl = &sxDecCtrl->items[sxDecCtrl->itemStackLevel];
    for (i = 0; i < itemTblCtrl->numItems; ++i)
      {
      item = &itemTblCtrl->itemTbl[i];
      if (item->numOcc == 0 && 
          ((item->elementFlags & SX_ELF_OPT) == 0) &&
          (sxDecCtrl->u_sx_el_start == NULL))
        {
        sxDecCtrl->errCode = SX_REQUIRED_TAG_NOT_FOUND;
        SXLOG_NERR1 ("Mandatory element '%s' not found",  item->tag);
        break;       
        }
      }
    }
  }

/************************************************************************/
/************************************************************************/

SX_ELEMENT *_uibed_find_element (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *tag)
  {
SX_ELEMENT_TBL_CTRL *itemTblCtrl;
ST_INT numItems;
SX_ELEMENT *item;
ST_INT i;

if (0 == sxDecCtrl->itemStackLevel)
{
	return NULL;
}
  itemTblCtrl = &sxDecCtrl->items[sxDecCtrl->itemStackLevel-1];
  item = itemTblCtrl->itemTbl;
  numItems = itemTblCtrl->numItems;

/* See if this element is in our table */
  for (i = 0; i < numItems; ++i, ++item)
    {
    if (strcmp (tag, item->tag) == 0)
      return (item);
    }

  return (NULL);
  }



/************************************************************************/
/************************************************************************/
/************************************************************************/

ST_RET sx_get_element_contents (SX_DEC_CTRL *sxDecCtrl, 
		      ST_CHAR *destBuf, ST_INT destLen,
		      ST_INT *lenOut)
  {
ST_CHAR *src;
ST_INT len;
ST_INT rc;

  rc = sx_find_element_contents (sxDecCtrl, &src, &len);
  if (rc != SD_SUCCESS)
    return (rc);

  if (destLen < len)
    {
    SXLOG_NERR0("sx_get_element_contents: dest too small");
    sxDecCtrl->errCode = SD_FAILURE;
    return (SD_FAILURE);
    }

  memcpy (destBuf, src, len);
  *lenOut = len;
  return (SD_SUCCESS);
  }

/************************************************************************/

ST_RET sx_find_element_contents (SX_DEC_CTRL *sxDecCtrl, 
		      ST_CHAR **elStartOut, ST_INT *lenOut)
  {
SX_DEC_ELEMENT_INFO *sxDecElInfo;
ST_CHAR *start;
ST_CHAR *end;
ST_CHAR endTagBuf[100];
ST_INT len;

  sxDecElInfo = &sxDecCtrl->sxDecElInfo;
  start = sxDecCtrl->xmlPos;
/* Check to see if this is an empty element */
  if (*(start - 2) == '/')
    {
    *lenOut = 0;
    return (SD_SUCCESS);
    }
    
  *elStartOut = start;
  len = sxDecCtrl->xmlLen - (start - sxDecCtrl->xmlStart);

/* We need to find the closing element for this start element */
  endTagBuf[0] = '<';
  endTagBuf[1] = '/';
  strcpy (&endTagBuf[2], sxDecElInfo->tag);
  strcat (endTagBuf, ">");

  end = strnstr (start, endTagBuf, len);
  if (end == NULL)
    {
    sxDecCtrl->errCode = SD_FAILURE;
    return (SD_FAILURE);
    }
  *lenOut = end - start;

  sxDecCtrl->xmlPos = end;

  SXLOG_CDEC0("sx_find_element_contents got data:");
  SXLOG_DECH (*lenOut, *elStartOut);
  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
/*			sx_get_entity 					*/
/************************************************************************/

ST_RET sx_get_entity (SX_DEC_CTRL *sxDecCtrl, 
		      ST_CHAR *destBuf, ST_INT destLen,
		      ST_INT *lenOut)
  {
SX_DEC_ELEMENT_INFO *sxDecElInfo;
ST_INT i;
ST_CHAR *dest;
ST_CHAR *src;
ST_CHAR *end;

  sxDecElInfo = &sxDecCtrl->sxDecElInfo;
  src = sxDecElInfo->entityStart;
  dest = destBuf;
  end = sxDecElInfo->entityEnd;
  for (i = 0; i < destLen && src < end; ++i, ++src, ++dest)
    *dest = *src;

  *lenOut = i;

  if (src < end)
    {
    SXLOG_NERR1("sx_get_entity: dest too small (max %d bytes expected)", destLen);
    sxDecCtrl->errCode = SD_FAILURE;
    return (SD_FAILURE);
    }

  SXLOG_CDEC0("sx_get_entity got data:");
  SXLOG_DECH (i, destBuf);
  return (SD_SUCCESS);
  }


/************************************************************************/
/*                       sx_get_bool					*/
/************************************************************************/

ST_RET sx_get_bool (SX_DEC_CTRL *sxDecCtrl, ST_BOOLEAN *out_ptr)
  {
ST_INT d;
ST_RET rc;

  rc = sx_get_value (sxDecCtrl, "%d", &d);
  if (rc != SD_SUCCESS)
    return (rc);

  if (d == 0)
    *out_ptr = SD_FALSE;
  else
    *out_ptr = SD_TRUE;

  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_int					*/
/************************************************************************/

ST_RET sx_get_int (SX_DEC_CTRL *sxDecCtrl, ST_INT *out_ptr)
  {
  return (sx_get_value (sxDecCtrl, "%d", out_ptr));
  }

/************************************************************************/
/*                       sx_get_float					*/
/************************************************************************/

ST_RET sx_get_float (SX_DEC_CTRL *sxDecCtrl, ST_FLOAT *out_ptr)
  {
  return (sx_get_value (sxDecCtrl, "%f", out_ptr));
  }

/************************************************************************/
/*                       sx_get_double					*/
/************************************************************************/

ST_RET sx_get_double (SX_DEC_CTRL *sxDecCtrl, ST_DOUBLE *out_ptr)
  {
  /* Note: must use the '%le' format specifier for double */
  return (sx_get_value (sxDecCtrl, "%le", out_ptr));
  }

/************************************************************************/
/*                       sx_get_uchar					*/
/************************************************************************/

ST_RET sx_get_uchar (SX_DEC_CTRL *sxDecCtrl, ST_UCHAR *out_ptr)
  {
  return (sx_get_value (sxDecCtrl, "%c", out_ptr));
  }

/************************************************************************/
/*                       sx_get_int16					*/
/************************************************************************/

ST_RET sx_get_int16 (SX_DEC_CTRL *sxDecCtrl, ST_INT16 *out_ptr)
  {
ST_INT i;
ST_RET rc;

  rc = sx_get_value (sxDecCtrl, "%d", &i);
  *out_ptr = (ST_INT16) i;
  return (rc);
  }


/************************************************************************/
/*                       sx_get_uint16					*/
/************************************************************************/

ST_RET sx_get_uint16 (SX_DEC_CTRL *sxDecCtrl, ST_UINT16 *out_ptr)
  {
  return (sx_get_value (sxDecCtrl, "%hu", out_ptr));
  }

/************************************************************************/
/*                       sx_get_long					*/
/************************************************************************/

ST_RET sx_get_long (SX_DEC_CTRL *sxDecCtrl, ST_LONG *out_ptr)
  {
  return (sx_get_value (sxDecCtrl, "%ld", out_ptr));
  }

/************************************************************************/
/*                       sx_get_ulong					*/
/************************************************************************/

ST_RET sx_get_ulong (SX_DEC_CTRL *sxDecCtrl, ST_UINT32 *out_ptr)
  {
  return (sx_get_value (sxDecCtrl, "%lu", out_ptr));
  }

/************************************************************************/
/*                       sx_get_uint					*/
/************************************************************************/

ST_RET sx_get_uint (SX_DEC_CTRL *sxDecCtrl, ST_UINT *out_ptr)
  {
  return (sx_get_value (sxDecCtrl, "%u", out_ptr));
  }

/************************************************************************/
/*                       sx_get_value					*/
/************************************************************************/

ST_RET sx_get_value (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *format_string, ST_VOID *out_ptr)
  {
ST_CHAR *str;
ST_INT strLen;
ST_RET rc;

  strLen = 0;
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return (rc);

/* Convert to desired data format*/
  if (!sscanf (str, format_string, out_ptr))
    {
    SXLOG_ERR0 ("Data Conversion Error");
    sxDecCtrl->errCode = SD_FAILURE;
    return (SD_FAILURE);
    }
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       *sx_get_alloc_string				*/
/************************************************************************/

ST_RET sx_get_alloc_string (SX_DEC_CTRL *sxDecCtrl, ST_CHAR **strOut)
  {
ST_CHAR *str;
ST_INT strLen;
ST_RET rc;

  strLen = 0;
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return (rc);

  *strOut = M_STRDUP(NULL,str);
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       *sx_get_string					*/
/************************************************************************/

ST_RET sx_get_string (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *dest, ST_INT *lenOut)
  {
ST_CHAR *str;
ST_INT strLen;
ST_RET rc;

  strLen = 0;
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return (rc);

  if (*lenOut != 0)
    {
    if (strLen > *lenOut)
      {
      SXLOG_NERR0 ("Error: String too long");
      sxDecCtrl->errCode = SD_FAILURE;
      return (SD_FAILURE);
      }
    }

  sx_format_string_dec (dest, str);
  *lenOut = strLen;
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_format_string_dec			        */
/************************************************************************/

ST_VOID sx_format_string_dec (ST_CHAR *dest, ST_CHAR *src)
  {
ST_CHAR *srcPtr;
ST_CHAR *srcPtrLast;
ST_CHAR *dstPtr;
ST_INT  diff;

  dstPtr = dest;
  srcPtrLast = src;
  srcPtr = strchr (src, '&');

  if (srcPtr == NULL)
    {
    strcpy (dest, src);
    return;
    }

  diff = srcPtr - srcPtrLast;
  strncpy (dstPtr, srcPtrLast, diff);
  dstPtr += diff;

  /* parse the source string and generate the dest string */
  while (srcPtr != NULL)
    {
    /* srcPtr points at a & */
    /* lets find out if the following characters are what we are looking for */
    if (strncmp (srcPtr, CODE_APOS, CODE_APOS_LEN) == 0)
      {
      *dstPtr++ = CHAR_APOS;
      srcPtr += CODE_APOS_LEN;
      }
    else if (strncmp (srcPtr, CODE_QUOT, CODE_QUOT_LEN) == 0)
      {
      *dstPtr++ = CHAR_QUOT;
      srcPtr += CODE_QUOT_LEN;
      }
    else if (strncmp (srcPtr, CODE_AMP, CODE_AMP_LEN) == 0)
      {
      *dstPtr++ = CHAR_AMP;
      srcPtr += CODE_AMP_LEN;
      }
    else if (strncmp (srcPtr, CODE_LT, CODE_LT_LEN) == 0)
      {
      *dstPtr++ = CHAR_LT;
      srcPtr += CODE_LT_LEN;
      }
    else if (strncmp (srcPtr, CODE_GT, CODE_GT_LEN) == 0)
      {
      *dstPtr++ = CHAR_GT;
      srcPtr += CODE_GT_LEN;
      }
    else
      {
      *dstPtr++ = '&';
      srcPtr++;
      }

    srcPtrLast = srcPtr;
    srcPtr = strchr (srcPtr, '&');

    if (srcPtr == NULL)
      {
      /* copy the remaining section of the string */
      strcpy (dstPtr, srcPtrLast);
      }
    else
      {
      diff = srcPtr - srcPtrLast;
      strncpy (dstPtr, srcPtrLast, diff);
      dstPtr += diff;
      }
    }
  }


/************************************************************************/
/************************************************************************/
/************************************************************************/
/*                       sx_get_bool_attr				*/
/************************************************************************/

ST_RET sx_get_bool_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_BOOLEAN *out_ptr)
  {
ST_INT d;
ST_RET rc;

  rc = sx_get_attr_value (sxDecCtrl, name, "%d", &d);
  if (rc != SD_SUCCESS)
    return (rc);

  if (d == 0)
    *out_ptr = SD_FALSE;
  else
    *out_ptr = SD_TRUE;

  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_int_attr 				*/
/************************************************************************/

ST_RET sx_get_int_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_INT *out_ptr)
  {
  return (sx_get_attr_value (sxDecCtrl, name, "%d", out_ptr));
  }

/************************************************************************/
/*                       sx_get_float_attr 				*/
/************************************************************************/

ST_RET sx_get_float_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_FLOAT *out_ptr)
  {
  return (sx_get_attr_value (sxDecCtrl, name, "%f", out_ptr));
  }

/************************************************************************/
/*                       sx_get_double_attr 				*/
/************************************************************************/

ST_RET sx_get_double_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_DOUBLE *out_ptr)
  {
  return (sx_get_attr_value (sxDecCtrl, name, "%le", out_ptr));
  }

/************************************************************************/
/*                       sx_get_uchar_attr 				*/
/************************************************************************/

ST_RET sx_get_uchar_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_UCHAR *out_ptr)
  {
  return (sx_get_attr_value (sxDecCtrl, name, "%c", out_ptr));
  }

/************************************************************************/
/*                       sx_get_int16_attr 				*/
/************************************************************************/

ST_RET sx_get_int16_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_INT16 *out_ptr)
  {
ST_INT i;
ST_RET rc;

  rc = sx_get_attr_value (sxDecCtrl, name, "%d", &i);
  *out_ptr = (ST_INT16) i;
  return (rc);
  }

/************************************************************************/
/*                       sx_get_uint16_attr 				*/
/************************************************************************/

ST_RET sx_get_uint16_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_UINT16 *out_ptr)
  {
ST_UINT i;
ST_RET rc;

  rc = sx_get_attr_value (sxDecCtrl, name, "%u", &i);
  *out_ptr = (ST_UINT16) i;
  return (rc);
  }

/************************************************************************/
/*                       sx_get_long_attr					*/
/************************************************************************/

ST_RET sx_get_long_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_LONG *out_ptr)
  {
  return (sx_get_attr_value (sxDecCtrl, name, "%ld", out_ptr));
  }

/************************************************************************/
/*                       sx_get_ulong_attr				*/
/************************************************************************/

ST_RET sx_get_ulong_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_UINT32 *out_ptr)
  {
  return (sx_get_attr_value (sxDecCtrl, name, "%lu", out_ptr));
  }

/************************************************************************/
/*                       sx_get_uint_attr 				*/
/************************************************************************/

ST_RET sx_get_uint_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, ST_UINT *out_ptr)
  {
  return (sx_get_attr_value (sxDecCtrl, name, "%u", out_ptr));
  }

/************************************************************************/
/*                       XmlStringToDuration 				*/
/************************************************************************/

ST_RET XmlStringToDuration (ST_CHAR *src, SX_DURATION *out_ptr)
  {
ST_CHAR *p;
ST_CHAR temp[50]; /* arbitrary length */
ST_CHAR temp2[50]; /* arbitrary length */
ST_BOOLEAN foundP = SD_FALSE;
ST_BOOLEAN foundT = SD_FALSE;
ST_BOOLEAN foundDot = SD_FALSE;
ST_UINT i;
ST_BOOLEAN something = SD_FALSE;  /* Make sure we got SOMETHING out of */
				  /* this, any of the fields can be missing */

  out_ptr->inUse = SD_TRUE;
  p = src;
  memset (temp, 0, sizeof(temp));
  memset (temp2, 0, sizeof(temp2));
  for (i = 0; i < strlen(src); i++, p++)
    {
    /* check all error conditions I can think of */
    if ((*p == '-' && foundP) || 
        (isdigit(*p) && !foundP) ||
        (*p == 'Y' && !foundP) ||
        (*p == 'M' && !foundP) ||
        (*p == 'D' && !foundP) ||
        (!foundP && foundT) ||
        (*p == 'H' && !foundT) ||
        (*p == 'S' && !foundT) ||
	(*p != 'P' && *p != 'T' && isalpha(*p) && strlen(temp) == 0))
      return (SD_FAILURE);
      
    /* check for negative */
    else if (*p == '-')
      out_ptr->negative = SD_TRUE;
    else if (*p == '.')
      foundDot = SD_TRUE;
    /* We found a number */
    else if (isdigit(*p))
      {
      if (foundDot)
	strncat (temp2, p, 1);
      else
	strncat (temp, p, 1);
      }
    /* We found a letter */
    else if (isalpha(*p))
      {
      switch (*p)
	{
	case 'P':
	  foundP = SD_TRUE;
	  break;
	case 'T':
	  foundT = SD_TRUE;
	  break;
	case 'Y':
	  sscanf(temp, "%d", &out_ptr->years);
	  temp[0] = 0;
	  something = SD_TRUE;
	  break;
	case 'M':
	  if (foundT)
	    sscanf(temp, "%d", &out_ptr->minutes);
	  else
	    sscanf(temp, "%d", &out_ptr->months);
	  temp[0] = 0;
  	  something = SD_TRUE;
	  break;
	case 'D':
	  sscanf(temp, "%d", &out_ptr->days);
	  temp[0] = 0;
  	  something = SD_TRUE;
	  break;
	case 'H':
	  sscanf(temp, "%d", &out_ptr->hours);
	  temp[0] = 0;
  	  something = SD_TRUE;
	  break;
	case 'S':
	  sscanf(temp, "%d", &out_ptr->seconds);
	  temp[0] = 0;
	  sscanf(temp2, "%ld", &out_ptr->microseconds);
	  temp2[0] = 0;
  	  something = SD_TRUE;
	  break;
	default:
          return (SD_FAILURE);
	}
      }
    }
  if (something == SD_FALSE)
    return (SD_FAILURE);

  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_duration_attr 				*/
/************************************************************************/

ST_RET sx_get_duration_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, SX_DURATION *out_ptr)
  {
ST_RET rc;
ST_CHAR *sxDuration;

  memset (out_ptr, 0, sizeof (*out_ptr));
  
  rc = sx_get_attr_ptr (sxDecCtrl, &sxDuration, name);
  if (rc)
    return (rc);

  rc = XmlStringToDuration (sxDuration, out_ptr);
  if (rc != SD_SUCCESS)
    {
    SXLOG_NERR0 ("ERROR: String to Duration conversion ");
    sxDecCtrl->errCode = SD_FAILURE;
    return (SD_FAILURE);
    }
      
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_xtime_attr 				*/
/************************************************************************/

ST_RET sx_get_xtime_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, SX_DATE_TIME *out_ptr)
  {
ST_RET rc;
ST_CHAR *sxDateTime;

  memset (out_ptr, 0, sizeof (*out_ptr));
  
  rc = sx_get_attr_ptr (sxDecCtrl, &sxDateTime, name);
  if (rc)
    return (rc);
    
  rc = XmlStringToUtcValue (sxDateTime, out_ptr);
  if (rc != SD_SUCCESS)
    {
    SXLOG_NERR0 ("ERROR: String to Value date/time conversion ");
    sxDecCtrl->errCode = SD_FAILURE;
    return (rc);
    }

  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_tm_attr 				*/
/************************************************************************/

ST_RET sx_get_tm_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, struct tm *out_ptr)
  {
ST_RET rc;
ST_CHAR *theTime;

  memset (out_ptr, 0, sizeof (*out_ptr));
  
  rc = sx_get_attr_ptr (sxDecCtrl, &theTime, name);
  if (rc)
    return (rc);
    
  rc = tstrStringToTm (theTime, out_ptr);
  if (rc != SD_SUCCESS)
    {
    SXLOG_NERR0 ("ERROR: String to Value (struct tm) date/time conversion ");
    sxDecCtrl->errCode = SD_FAILURE;
    return (rc);
    }

  return (SD_SUCCESS);
  }

/************************************************************************/
/*			sx_get_attr_value 				*/
/************************************************************************/

ST_RET sx_get_attr_value (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *name, 
			  ST_CHAR *format_string, ST_VOID *out_ptr)
  {
ST_UINT i;

  for (i = 0; i < sxDecCtrl->sxDecElInfo.attrCount; ++i)
    {
    if (strcmp (sxDecCtrl->sxDecElInfo.attr[i].name, name) == 0)
      {
    /* Convert to desired data format*/
      if (!sscanf (sxDecCtrl->sxDecElInfo.attr[i].value, 
			format_string, out_ptr))
        {
        SXLOG_ERR0 ("Attribute Data Conversion Error");
        sxDecCtrl->errCode = SD_FAILURE;
        return (SD_FAILURE);
        }
      return (SD_SUCCESS);
      }
    }
  return (SD_FAILURE);
  }

/************************************************************************/
/*                       sx_get_attr					*/
/************************************************************************/

ST_RET sx_get_attr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR *value, ST_CHAR *name)
  {
ST_UINT i;

  for (i = 0; i < sxDecCtrl->sxDecElInfo.attrCount; ++i)
    {
    if (strcmp (sxDecCtrl->sxDecElInfo.attr[i].name, name) == 0)
      {
      strcpy (value, sxDecCtrl->sxDecElInfo.attr[i].value);
      return (SD_SUCCESS);
      }
    }
  return (SD_FAILURE);
  }

/************************************************************************/
/*                       sx_get_attr_ptr				*/
/************************************************************************/

ST_RET sx_get_attr_ptr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR **ptrOut, ST_CHAR *name)
  {
ST_UINT i;

  for (i = 0; i < sxDecCtrl->sxDecElInfo.attrCount; ++i)
    {
    if (strcmp (sxDecCtrl->sxDecElInfo.attr[i].name, name) == 0)
      {
      *ptrOut = sxDecCtrl->sxDecElInfo.attr[i].value;
      return (SD_SUCCESS);
      }
    }
  return (SD_FAILURE);
  }

/************************************************************************/
/*                       *sx_get_bitstring				*/
/************************************************************************/

ST_RET sx_get_bitstring (SX_DEC_CTRL *sxDecCtrl, ST_INT *dest, ST_INT *lenOut)
  {
ST_CHAR *str;
ST_INT strLen;
ST_INT *bits;
ST_CHAR *pStr;
ST_RET rc;
ST_INT i;

  strLen = 0;
  bits = dest; 
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return (rc);

  if (*lenOut != 0)
    {
    if (strLen > *lenOut)
      {
      SXLOG_NERR0 ("Error: String too long");
      sxDecCtrl->errCode = SD_FAILURE;
      return (SD_FAILURE);
      }
    }

  pStr = str;
  pStr += strlen(str) - 1;
  for (i=0;i<(ST_INT)strlen(str);++i,--pStr)
    {
    if (strncmp(pStr, "0", 1) != 0 &&
        strncmp(pStr, "1", 1) != 0)
      {
      SXLOG_NERR0 ("Error: Not a bitstring");
      sxDecCtrl->errCode = SD_FAILURE;
      return (SD_FAILURE);
      }

    if (strncmp(pStr, "1", 1) == 0)
      {
      *bits |= (1 << i);
      }
    }

  *lenOut = strLen;
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_string_ptr   				*/
/************************************************************************/

ST_RET sx_get_string_ptr (SX_DEC_CTRL *sxDecCtrl, ST_CHAR **strOut, ST_INT *lenOut)
  {
ST_INT vLen;
ST_RET rc;

  rc = sx_get_entity (sxDecCtrl, sxDecCtrl->elemBuf, sizeof(sxDecCtrl->elemBuf), &vLen);
  if (rc != SD_SUCCESS)
    return (SD_FAILURE);

/* Convert to desired data format*/
  sxDecCtrl->elemBuf[vLen] = 0;
  *strOut = sxDecCtrl->elemBuf;
  *lenOut = vLen;
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_string_YesNo   				*/
/*----------------------------------------------------------------------*/
/* This user helper function parses element value for the string	*/
/* "Yes"/"No" and sets the dest to SD_TRUE/SD_FALSE respectively.	*/
/* The string comparison is not case sensitive.				*/
/* Parameters:								*/
/*   SX_DEC_CTRL *sxDecCtrl	pointer to SX decoding control		*/
/*   ST_BOOLEAN  *dest		pointer to dest to SD_TRUE/SD_FALSE	*/
/*   ST_RET       errCode       SX err code to set in sxDecCtrl->errCode*/
/*			          in case of invalid element value,	*/
/*			        SX_ERR_CONVERT allows continue parsing.	*/
/* Return:								*/
/*	SD_SUCCESS		if function successful			*/
/*	SD_FAILURE		otherwise,and the sxDecCtrl->errCode is	*/
/*                              set to errCode.				*/
/************************************************************************/
ST_RET sx_get_string_YesNo (SX_DEC_CTRL *sxDecCtrl, ST_BOOLEAN *dest,
                            ST_RET errCode)
  {
ST_RET   rc;
ST_CHAR *str;
ST_INT   strLen;

  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc == SD_SUCCESS)
    {
    if (stricmp (str, "Yes") == 0)
      *dest = SD_TRUE;
    else if (stricmp (str, "No") == 0)
      *dest = SD_FALSE;
    else
      {
      SXLOG_ERR2 ("SX DEC ERROR: invalid %s value '%s' (Yes/No expected)",
                  sxDecCtrl->sxDecElInfo.tag, str);
      sxDecCtrl->errCode = errCode;
      rc = SD_FAILURE;
      }
    }

  return (rc);
  }


/************************************************************************/
/*			sx_get_string_OnOff_mask			*/
/*----------------------------------------------------------------------*/
/* This user helper function parses element value for the string	*/
/* "On"/"Off" and sets/resets a maskBit in the mask.			*/
/* The string comparison is not case sensitive.				*/
/* Parameters:								*/
/*   SX_DEC_CTRL *sxDecCtrl	pointer to SX decoding control		*/
/*   ST_UINT     *mask		pointer to mask to set/reset a bit	*/
/*   ST_UINT      maskBit       mask bit to set/reset if ON/OFF	found	*/
/*   ST_RET       errCode       SX err code to set in sxDecCtrl->errCode*/
/*			          in case of invalid element value,	*/
/*			        SX_ERR_CONVERT allows continue parsing.	*/
/* Return:								*/
/*	SD_SUCCESS		if function successful			*/
/*	SD_FAILURE		otherwise,and the sxDecCtrl->errCode is	*/
/*                              set to errCode				*/
/************************************************************************/

ST_RET sx_get_string_OnOff_mask (SX_DEC_CTRL *sxDecCtrl, ST_UINT *mask,
                                ST_UINT maskBit, ST_RET errCode)
  {
ST_RET   rc;
ST_CHAR *str;
ST_INT   strLen;

  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc == SD_SUCCESS)
    {
    if (stricmp (str, "On") == 0)
      {
      *mask |= maskBit;		/* turn on this bit in the mask		*/
      }						       
    else if (stricmp (str, "Off") == 0)
      {						       
      *mask &= ~maskBit;	/* turn off this bit in the mask	*/
      }
    else
      {
      SXLOG_ERR2 ("SX DEC ERROR: invalid %s value '%s' (On/Off expected)",
                  sxDecCtrl->sxDecElInfo.tag, str);
      sxDecCtrl->errCode = errCode;
      rc = SD_FAILURE;
      }
    }
  return (rc);
  }

/************************************************************************/
/*			sx_get_string_OnOff_bool			*/
/*----------------------------------------------------------------------*/
/* This user helper function parses element value for the string	*/
/* "On"/"Off" and sets the dest to SD_TRUE/SD_FALSE respectively.	*/
/* The string comparison is not case sensitive.				*/
/* Parameters:								*/
/*   SX_DEC_CTRL *sxDecCtrl	pointer to SX decoding control		*/
/*   ST_BOOLEAN  *dest		pointer to dest to SD_TRUE/SD_FALSE	*/
/*   ST_RET       errCode       SX err code to set in sxDecCtrl->errCode*/
/*			          in case of invalid element value,	*/
/*			        SX_ERR_CONVERT allows continue parsing.	*/
/* Return:								*/
/*	SD_SUCCESS		if function successful			*/
/*	SD_FAILURE		otherwise,and the sxDecCtrl->errCode is	*/
/*                              set to errCode				*/
/************************************************************************/

ST_RET sx_get_string_OnOff_bool (SX_DEC_CTRL *sxDecCtrl, ST_BOOLEAN *dest,
                                ST_RET errCode)
  {
ST_RET   rc;
ST_CHAR *str;
ST_INT   strLen;

  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc == SD_SUCCESS)
    {
    if (stricmp (str, "On") == 0)
      *dest = SD_TRUE;
    else if (stricmp (str, "Off") == 0)
      *dest = SD_FALSE;
    else
      {
      SXLOG_ERR2 ("SX DEC ERROR: invalid %s value '%s' (On/Off expected)",
                  sxDecCtrl->sxDecElInfo.tag, str);
      sxDecCtrl->errCode = errCode;
      rc = SD_FAILURE;
      }
    }
  return (rc);
  }

/************************************************************************/
/*                       sx_get_time					*/
/************************************************************************/
ST_RET sx_get_time (SX_DEC_CTRL *sxDecCtrl, time_t *out_ptr)
  {
ST_CHAR *str;
ST_RET rc;
time_t t;
ST_INT strLen;

  strLen = 0;
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return (rc);


  rc = tstrStringToTime (str, &t);
  if (rc != SD_SUCCESS)
    {
    SXLOG_NERR1 ("Error: Time conversion (0x%04x)", rc);
    sxDecCtrl->errCode = SD_FAILURE;
    return (rc);
    }

  *out_ptr = t;
  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_tm					*/
/************************************************************************/
ST_RET sx_get_tm (SX_DEC_CTRL *sxDecCtrl, struct tm *out_ptr)
  {
ST_CHAR *str;
ST_RET rc;
ST_INT strLen;

  strLen = 0;
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return (rc);

  rc = tstrStringToTm (str, out_ptr);
  if (rc != SD_SUCCESS)
    {
    SXLOG_NERR1 ("Error: Time conversion (0x%04x)", rc);
    sxDecCtrl->errCode = SD_FAILURE;
    return (rc);
    }

  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_tm_ex					*/
/************************************************************************/
ST_RET sx_get_tm_ex (SX_DEC_CTRL *sxDecCtrl, struct tm *out_ptr, ST_LONG *microseconds)
  {
ST_CHAR *str;
ST_RET rc;
ST_INT strLen;
ST_CHAR *strMicro = NULL;	/* init to invalid value*/
char *periodLoc;

  strLen = 0;
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return (rc);

  periodLoc = strchr (str, '.');
  if (periodLoc != NULL)
    {
    if ((periodLoc + 1) == '\0')
      {
      SXLOG_NERR1 ("Error: Time conversion (0x%04x)", rc);
      sxDecCtrl->errCode = SD_FAILURE;
      return (rc);
      }
    *periodLoc = '\0';
    strMicro = periodLoc + 1;
    }

  rc = tstrStringToTm (str, out_ptr);
  if (rc != SD_SUCCESS)
    {
    SXLOG_NERR1 ("Error: Time conversion (0x%04x)", rc);
    sxDecCtrl->errCode = SD_FAILURE;
    return (rc);
    }

  if (strMicro != NULL)
    {
    while (strlen(strMicro) < 6)
      strcat (strMicro, "0");
    while (strlen(strMicro) > 6)
      strMicro[strlen(strMicro)-1] = '\0';
    *microseconds = atol(strMicro);
    }
  else
    *microseconds = 0;

  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_duration				*/
/************************************************************************/
/* Store duration string found in an xml file into the structure	*/
/* "SX_DURATION".                               			*/
/************************************************************************/

ST_RET sx_get_duration (SX_DEC_CTRL *sxDecCtrl, SX_DURATION *sxDuration)
  {
ST_CHAR *str;
ST_RET rc;
ST_INT strLen;

  memset (sxDuration, 0, sizeof (*sxDuration));
  		
  strLen = 0;
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return (rc);

  rc = XmlStringToDuration (str, sxDuration);
  if (rc != SD_SUCCESS)
    {
    SXLOG_NERR0 ("ERROR: String to Duration conversion ");
    sxDecCtrl->errCode = SD_FAILURE;
    return (rc);
    }

  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_xtime					*/
/************************************************************************/
/* Store date and time string found in an xml file into the structure   */
/* "SX_DATE_TIME" specifying the number of seconds from                 */
/* 1/1/1970 (UTC time), the number of microseconds in a decimal         */
/* fraction if it is specified and the number of minutes in the time    */
/* zone offset if it is specified.                                      */
/************************************************************************/

ST_RET sx_get_xtime (SX_DEC_CTRL *sxDecCtrl, SX_DATE_TIME *sxDateTime)
  {
ST_CHAR *str;
ST_RET rc;
ST_INT strLen;

  memset (sxDateTime, 0, sizeof (*sxDateTime));
  		
  strLen = 0;
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return (rc);

  /* convert input xml string to date and time */
  /* store data in "SX_DATE_TIME" structure    */
  rc = XmlStringToUtcValue (str, sxDateTime);
  if (rc != SD_SUCCESS)
    {
    SXLOG_NERR0 ("ERROR: String to Value date/time conversion ");
    sxDecCtrl->errCode = SD_FAILURE;
    return (rc);
    }

  return (SD_SUCCESS);
  }

/************************************************************************/
/*                       sx_get_time_ex					*/
/************************************************************************/
ST_RET sx_get_time_ex (SX_DEC_CTRL *sxDecCtrl, time_t *out_ptr, ST_LONG *microseconds)
  {
ST_CHAR *str;
ST_CHAR strMicro[10];
char *periodLoc;
ST_RET rc;
time_t t;
ST_INT strLen;

  strLen = 0;
  rc = sx_get_string_ptr (sxDecCtrl, &str, &strLen);
  if (rc != SD_SUCCESS)
    return (rc);

  periodLoc = strchr (str, '.');
  if (periodLoc != NULL)
    {
    if ((periodLoc + 1) == '\0')
      {
      SXLOG_NERR1 ("Error: Time conversion (0x%04x)", rc);
      sxDecCtrl->errCode = SD_FAILURE;
      return (rc);
      }
    strcpy(strMicro, periodLoc + 1);
    *periodLoc = '\0';
    }

  rc = tstrStringToTime (str, &t);
  if (rc != SD_SUCCESS)
    {
    SXLOG_NERR1 ("Error: Time conversion (0x%04x)", rc);
    sxDecCtrl->errCode = SD_FAILURE;
    return (rc);
    }

  *out_ptr = t;
  if (periodLoc != NULL)
    {
    while (strlen(strMicro) < 6)
      strcat (strMicro, "0");
    while (strlen(strMicro) > 6)
      strMicro[strlen(strMicro)-1] = '\0';
    *microseconds = atol(strMicro);
    }
  else
    *microseconds = 0;

  return (SD_SUCCESS);
  }

/************************************************************************/
/************************************************************************/
#define MAX_BYTES_IN_CHUNK  (2*SX_MAX_ELEM_LEN)	/* MUST BE EVEN */

#define SX_LOAD_CHAR(a)  {\
                         if (sxDecCtrl->useFp == SD_TRUE)\
                           {\
                           sx_load_characters (lineBuf, &eof, sxDecCtrl->fp, a);\
                           sxDecElInfo->entityStart-= a;\
                           sxDecElInfo->entityEnd-= a;\
                           }\
                         else\
                           {\
                           xml+= a;\
                           }\
                         }
                         
#define SX_RIP_NOT_DONE (!sxDecCtrl->termFlag && (((sxDecCtrl->useFp == SD_FALSE) && (xml < xmlEnd)) || ((sxDecCtrl->useFp == SD_TRUE) && (*xml != '\0'))))

#define SX_RIP_DONE (sxDecCtrl->termFlag || ((sxDecCtrl->useFp == SD_FALSE) && (xml >= xmlEnd)) || ((sxDecCtrl->useFp == SD_TRUE) && (*xml == '\0')))


/************************************************************************/
/*			sx_rip_xml 					*/
/************************************************************************/

ST_RET sx_rip_xml (SX_DEC_CTRL *sxDecCtrl)
  {
SX_DEC_ELEMENT_INFO *sxDecElInfo;
ST_CHAR *xml;
ST_INT  nestLevel;
ST_CHAR *tagDest;
ST_CHAR *attribNameDest;
ST_CHAR *attribValDest;
ST_CHAR attribValCopy[SX_MAX_ATTR_VALUE];
ST_INT len;
ST_INT midBuff;
ST_INT numChRead;
ST_LONG xmlLen;
ST_CHAR *xmlEnd = NULL;		/* init to avoid compiler warning	*/
ST_CHAR c;
ST_BOOLEAN bEmptyTag;
ST_BOOLEAN eof;
ST_BOOLEAN sawStartTag = SD_FALSE;
ST_CHAR lineBuf[MAX_BYTES_IN_CHUNK];

  sxDecElInfo = &sxDecCtrl->sxDecElInfo;
  
  bEmptyTag = SD_FALSE;
  
  if (sxDecCtrl->useFp == SD_TRUE)
    {
    memset (lineBuf, 0, MAX_BYTES_IN_CHUNK);
    eof = SD_FALSE;
    midBuff = MAX_BYTES_IN_CHUNK / 2;
  
    /* get the first (MAX_BTYES_IN_CHUNK / 2) chunk from the file */
    numChRead = fread(lineBuf + midBuff, 1, MAX_BYTES_IN_CHUNK - midBuff, sxDecCtrl->fp);
    if (numChRead > 0)
      xml = lineBuf + midBuff;
    else
      {
      SXLOG_NERR0 ("SX decode error: could not read xml from file");
      sxDecCtrl->errCode = SD_FAILURE;
      return (SD_FAILURE);
      }
    }
  else
    {
    sxDecElInfo = &sxDecCtrl->sxDecElInfo;
    xml = sxDecCtrl->xmlStart;
    xmlLen = sxDecCtrl->xmlLen;
    xmlEnd = xml + xmlLen;
    }

  while (SX_RIP_NOT_DONE)
    {
/* Find a begin or end tag */
    while (*xml != '<' && SX_RIP_NOT_DONE)
      SX_LOAD_CHAR (1);

    if (SX_RIP_DONE)
      break;
   
   sawStartTag = SD_TRUE;

   sxDecElInfo->tagStart = xml;
   
/* OK, this should be the start of a start tag, an end tag, or a comment */
/* or block of binary CDATA						 */
    SX_LOAD_CHAR (1);
    if (strncmp (xml, "!--", 3) == 0 || *xml == '?') /* Comment */
      {
      while (strncmp (xml, "-->", 3) != 0 && 
           strncmp (xml, "?>", 2)  != 0 &&
           SX_RIP_NOT_DONE)
        {
        SX_LOAD_CHAR (1);
        }
      }
    else if (strncmp (xml, "![CDATA[", 8) == 0) /* CDATA */
      {
      while (strncmp (xml, "]]>", 3) != 0 &&
	     SX_RIP_NOT_DONE)
	{
	SX_LOAD_CHAR (1);
	}
      }
    else if (strncmp (xml, "!DOCTYPE", 8) == 0) /* DocType */
      {
      nestLevel = 0;
      while (SD_TRUE)
        {
        if (*xml == '>')
          {
          if (nestLevel == 0)
            break;
          else
            --nestLevel;
          }
        if (*xml == '<')
          ++nestLevel;

        SX_LOAD_CHAR (1);
        if (SX_RIP_DONE)
          {
          SXLOG_NERR0 ("SX decode error: could not find DOCTYPE end");
          sxDecCtrl->errCode = SD_FAILURE;
          return (SD_FAILURE);
	  }
	}
      SX_LOAD_CHAR (1);
      }
    else if (*xml != '/')	/* Begin tag */
      {
    /* We have a element tag start, get the tag  first  */
      tagDest = sxDecElInfo->tag;
      len = 0;
      while (SX_RIP_NOT_DONE)
        {
        c = *xml;
        if (c == '>' || c == ' ' || c == '/' || c == 10 || c == 9 || c == 13) 	/* Found the end of the tag */
          break;

        *(tagDest++) = c;
        ++len;
        if (len >= SX_MAX_TAG_LEN)
          {
          SXLOG_NERR0 ("SX decode error: tag too long");
          sxDecCtrl->errCode = SD_FAILURE;
          return (SD_FAILURE);
          }
        SX_LOAD_CHAR (1);
	if (sxDecCtrl->ignoreNS && c == ':')
	  {
          tagDest = sxDecElInfo->tag;
      	  len = 0;
	  }
        }
      if (SX_RIP_DONE)
        {
        SXLOG_NERR0 ("SX decode error: could not find tag end");
        sxDecCtrl->errCode = SD_FAILURE;
        return (SD_FAILURE);
        }
      *tagDest = 0;		/* terminate the tag */

    /* Now look for attributes */         
      sxDecElInfo->attrCount = 0;
      while (*xml != '>' && *xml != '/') /* we could have attributes! */
        {
      /* skip any whitespace before the start of the attribute name */
        while (*xml == ' ' && SX_RIP_NOT_DONE)
          SX_LOAD_CHAR (1);

        if (SX_RIP_DONE)
          {
          SXLOG_NERR0 ("SX decode error: could not find attribute name");
          sxDecCtrl->errCode = SD_FAILURE;
          return (SD_FAILURE);
          }

        if (*xml != '>' && *xml != '/')
          {
          if (sxDecElInfo->attrCount >= SX_MAX_ATTRIB)
            {
            SXLOG_NERR0 ("SX decode error: too many attributes. Look at SX_MAX_ATTRIB define");
            sxDecCtrl->errCode = SD_FAILURE;
            return (SD_FAILURE);
            }

        /* This should be the start of an attribute name */ 
          attribNameDest = sxDecElInfo->attr[sxDecElInfo->attrCount].name;
          len = 0;
          while (SX_RIP_NOT_DONE)
            {
            c = *xml;
            if (c == '=' || c == ' ')	/* Found the end of the name */
              break;

            if (c == '>') /* this isn't really an attribute like we first thought */
              break;
       
            if (c != 9 && c != 10 && c != 13) /* dont include these characters */
              {
              *(attribNameDest++) = c;
              ++len;
              }

            if (len >= SX_MAX_ATTR_NAME)
              {
              SXLOG_NERR0 ("SX decode error: attribute name too long");
              sxDecCtrl->errCode = SD_FAILURE;
              return (SD_FAILURE);
              }
            SX_LOAD_CHAR (1);
	    if (sxDecCtrl->ignoreNS && c == ':')
	      {
              attribNameDest = sxDecElInfo->attr[sxDecElInfo->attrCount].name;
              len = 0;
	      }
            }

          if (SX_RIP_DONE)
            {
            SXLOG_NERR0 ("SX decode error: could not find attribute name end");
            sxDecCtrl->errCode = SD_FAILURE;
            return (SD_FAILURE);
            }

          if (*xml == '>') /* this isn't really an attribute like we first thought */
            break;

          *attribNameDest = 0;		/* terminate the attrib name */

        /* skip to the attribute '=' */
          while (*xml != '=' && SX_RIP_NOT_DONE)
            SX_LOAD_CHAR (1);
          if (SX_RIP_DONE)
            {
            SXLOG_NERR0 ("SX decode error: could not find attribute '='");
            sxDecCtrl->errCode = SD_FAILURE;
            return (SD_FAILURE);
            }
          SX_LOAD_CHAR (1);
	  
        /* skip white space */
          while ((*xml == ' ' || *xml == '\t') && SX_RIP_NOT_DONE)
            SX_LOAD_CHAR (1);
	    
          /* OK, get the attrib value */
          if (*(xml) != '"' && *(xml) != '\'')		/* skip the opening " */
            {
            SXLOG_NERR0 ("SX decode error: could not find leading attribute value '\"'");
            sxDecCtrl->errCode = SD_FAILURE;
            return (SD_FAILURE);
            }

          SX_LOAD_CHAR (1);
        
           /* At the start of the attribute value */ 
          attribValDest = sxDecElInfo->attr[sxDecElInfo->attrCount].value;
          len = 0;
          while (SX_RIP_NOT_DONE)
            {
            c = *xml;
            if (c == '"' || c == '\'')	/* Found the end of the attrib */
              break;
          
            if (c != 9 && c != 10 && c != 13) /* dont include these characters */
              {
              *(attribValDest++) = c;
              ++len;
              }

            if (len >= SX_MAX_ATTR_VALUE)
              {
              SXLOG_NERR0 ("SX decode error: attribute value too long");
              sxDecCtrl->errCode = SD_FAILURE;
              return (SD_FAILURE);
              }
            SX_LOAD_CHAR (1);
            }
          if (SX_RIP_DONE)
            {
            SXLOG_NERR0 ("SX decode error: could not find closing attribute value '\"'");
            sxDecCtrl->errCode = SD_FAILURE;
            return (SD_FAILURE);
            }

          if (*(xml) != '"' && *(xml) != '\'')		/* skip the closing " */
            {
            SXLOG_NERR0 ("SX decode error: could not find closing attribute value '\"'");
            sxDecCtrl->errCode = SD_FAILURE;
            return (SD_FAILURE);
            }
          SX_LOAD_CHAR (1);

          *attribValDest = 0;	/* terminate the attrib value */
          strcpy (attribValCopy, sxDecElInfo->attr[sxDecElInfo->attrCount].value);
          sx_format_string_dec (sxDecElInfo->attr[sxDecElInfo->attrCount].value, attribValCopy);
          ++sxDecElInfo->attrCount;
          }

        if (SX_RIP_DONE)
          {
          SXLOG_NERR0 ("SX decode error: could not find tag end");
          sxDecCtrl->errCode = SD_FAILURE;
          return (SD_FAILURE);
          }
        }

      /* Could be empty tag */
      if (*xml == '/')
	{
	bEmptyTag = SD_TRUE;
	while (*xml != '>')
          SX_LOAD_CHAR (1);
	}
      SX_LOAD_CHAR (1); /* skip the '>' */

    /* OK, now call the element start function */
      sxDecElInfo->entityStart = xml;
      sxDecElInfo->entityEnd = xml;
      sxDecCtrl->xmlPos  = xml;	/* Save current dec position 	*/

      sxStartElement (sxDecCtrl); 
      /* Fail on any error except convert error */
      if (sxDecCtrl->errCode != SD_SUCCESS && sxDecCtrl->errCode != SX_ERR_CONVERT)
	{
	return (SD_FAILURE);
	}

      if (bEmptyTag)
	{
        /* OK, now call the element end function */
	sxEndElement (sxDecCtrl);
	bEmptyTag = SD_FALSE;
	}
      SX_LOAD_CHAR (sxDecCtrl->xmlPos - xml);
      sxDecCtrl->xmlPos  = xml;	/* Save current dec position 	*/
      }
    else		/* End tag */
      {
      sxDecElInfo->entityEnd = xml - 1;
      SX_LOAD_CHAR (1);
      tagDest = sxDecElInfo->tag;
      len = 0;
      while (SX_RIP_NOT_DONE)
        {
        c = *xml;
        if (c == '>' || c == ' ')	/* Found the end of the tag */
          break;

        *(tagDest++) = c;
        ++len;
        if (len >= SX_MAX_TAG_LEN)
          {
          SXLOG_NERR0 ("SX decode error: tag too long");
          sxDecCtrl->errCode = SD_FAILURE;
          return (SD_FAILURE);
          }
        SX_LOAD_CHAR (1);
	if (sxDecCtrl->ignoreNS && c == ':')
	  {
          tagDest = sxDecElInfo->tag;
          len = 0;
	  }
        }
      if (SX_RIP_DONE)
        {
        SXLOG_NERR0 ("SX decode error: could not find tag end");
        sxDecCtrl->errCode = SD_FAILURE;
        return (SD_FAILURE);
        }
      *tagDest = 0;		/* terminate the tag */


      /* any white space up to the end of the tag name */
      while (*xml != '>' && SX_RIP_NOT_DONE)
        SX_LOAD_CHAR (1);
      if (SX_RIP_DONE)
        {
        SXLOG_NERR0 ("SX decode error: could not find tag end");
        sxDecCtrl->errCode = SD_FAILURE;
        return (SD_FAILURE);
        }
      SX_LOAD_CHAR (1);

      sxDecCtrl->xmlPos  = xml;	/* Save current dec position 	*/
      
    /* OK, now call the element end function */
      sxEndElement (sxDecCtrl);
      /* Fail on any error except convert error */
      if (sxDecCtrl->errCode != SD_SUCCESS && sxDecCtrl->errCode != SX_ERR_CONVERT)
	{
	return (SD_FAILURE);
	}
      }
    }
    
  if (!sawStartTag)
    {
    SXLOG_ERR0 ("SX decode error: could not find start tag");
    sxDecCtrl->errCode = SD_FAILURE;
    return (SD_FAILURE);
    }
  else
    return (SD_SUCCESS);
  }

ST_VOID sx_load_characters (ST_CHAR *lineBuf, ST_BOOLEAN *eof, FILE *fp, ST_INT numToRead)
{
ST_LONG numChRead;
ST_LONG i;

  if (numToRead > 0)
    {
    /* first shift the characters by numToRead */
    memmove (lineBuf, &lineBuf[numToRead], MAX_BYTES_IN_CHUNK - numToRead);
  
    if (*eof != SD_TRUE)
      {
      numChRead = fread(lineBuf + (MAX_BYTES_IN_CHUNK - numToRead), 1, numToRead, fp);
      if (numChRead == 0)
        {
        *eof = SD_TRUE;
        for (i = 1; i < numToRead + 1; i++)
          {
          lineBuf[MAX_BYTES_IN_CHUNK - i] = '\0';
          }
        }
      }
    }
  }
