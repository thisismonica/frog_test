/*************************************************************************/
/*                                                                       */
/*        Copyright (c) 1999-2002 XJ ELECTRIC CO.LTD.                    */
/*                                                                       */
/* PROPRIETARY RIGHTS of Accelerated Technology are involved in the      */
/* subject matter of this material.  All manufacturing, reproduction,    */
/* use, and sales rights pertaining to this subject matter are governed  */
/* by the license agreement.  The recipient of this software implicitly  */
/* accepts the terms of the license.                                     */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*                                                                       */
/* FILE NAME                                            VERSION          */
/*                                                                       */
/*      xj_file.h                                       Ver  1.0         */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      FILE - file system                                               */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains function prototypes of all functions          */
/*      accessible to other components.                                  */
/*                                                                       */
/* AUTHOR                                                                */
/*                                                                       */
/*      ZhiDeYang, XJ ELECTRIC CO.LTD.                                   */
/*                                                                       */
/* DATA STRUCTURES                                                       */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* FUNCTIONS                                                             */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*                                                                       */
/* HISTORY                                                               */
/*                                                                       */
/*         NAME            DATE                    REMARKS               */
/*                                                                       */
/*      ZhideYang.      04-12-2007      Created initial version 1.0      */
/*                                                                       */
/*************************************************************************/
#include "plat/inc/xj_cosc.h"
#include "plat/inc/pfile_defs.h"

/* Check to see if the file has been included already.  */

#ifndef XJ_FILE_H
#define XJ_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Define external inner-component global data references.  */
extern PFILE_FUN_LIST PlatFileFunctionList;

/* 文件系统管理函数 */
#define xj_fopen                (PlatFileFunctionList.fopen)
#define xj_fclose               (PlatFileFunctionList.fclose)
#define xj_fread                (PlatFileFunctionList.fread)
#define xj_fwrite               (PlatFileFunctionList.fwrite)
#define xj_vfscanf              (PlatFileFunctionList.vfscanf)
#define xj_fgetc                (PlatFileFunctionList.fgetc)
#define xj_fgets                (PlatFileFunctionList.fgets)
#define xj_fputc                (PlatFileFunctionList.fputc)
#define xj_fputs                (PlatFileFunctionList.fputs)
#define xj_fgetpos              (PlatFileFunctionList.fgetpos)
#define xj_ftell                (PlatFileFunctionList.ftell)
#define xj_fsetpos              (PlatFileFunctionList.fsetpos)
#define xj_fseek                (PlatFileFunctionList.fseek)
#define xj_vfprintf             (PlatFileFunctionList.vfprintf)
#define xj_perror               (PlatFileFunctionList.perror)
#define xj_fflush               (PlatFileFunctionList.fflush)
#define xj_rewind               (PlatFileFunctionList.rewind)

#define xj_RemoveFile           (PlatFileFunctionList.RemoveFile)
#define xj_MakeDir              (PlatFileFunctionList.MakeDir)
#define xj_RenameFile           (PlatFileFunctionList.RenameFile)
#define xj_RenameDir            (PlatFileFunctionList.RenameDir)
#define xj_RemoveDir            (PlatFileFunctionList.RemoveDir)
#define xj_SetFileInformation   (PlatFileFunctionList.SetFileInformation)
#define xj_GetFileInformation   (PlatFileFunctionList.GetFileInformation)
#define xj_FileExists           (PlatFileFunctionList.FileExists)
#define xj_DirExists            (PlatFileFunctionList.DirExists)
#define xj_GetFreeSpace         (PlatFileFunctionList.GetFreeSpace)
#define xj_ListFile             (PlatFileFunctionList.ListFile)

#define xj_GetFlashPrartitionBlockStatus (PlatFileFunctionList.GetFlashPrartitionBlockStatus)

#ifdef __cplusplus
}
#endif

#endif
