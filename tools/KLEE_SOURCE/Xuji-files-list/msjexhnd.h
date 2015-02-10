/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*                  1997-2001, All Rights Reserved                   	*/
/*									*/
/* MODULE NAME : msjexhnd.cpp						*/
/* PRODUCT(S)  : 							*/
/*									*/
/* MODULE DESCRIPTION : Unhandled Exception Hanlder			*/
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 05/14/02  DSF     02    Added support for PDBs			*/
/* 02/08/02  MDE     01    Added Debug Exception and Slog support	*/
/************************************************************************/

#ifndef __MSJEXHND_H__
#define __MSJEXHND_H__

#define	EXCEPTION_DIAGNOSTIC_DEBUG 	0xE0000001

#ifdef __cplusplus

class MSJExceptionHandler
  {
public:
	
  MSJExceptionHandler( );
  ~MSJExceptionHandler( );
	
  void SetLogFileName( PTSTR pszLogFileName );
#ifdef DEBUG_SISCO  
  static void SetLogCtrl (LOG_CTRL *pLogCtrl);
#endif  

private:

  // entry point where control comes on an unhandled exception
  static LONG WINAPI MSJUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo );

  // where report info is extracted and generated	
  static void GenerateExceptionReport( PEXCEPTION_POINTERS pExceptionInfo );

  // Helper functions
  static LPTSTR GetExceptionString( DWORD dwCode );
  static BOOL GetLogicalAddress( PVOID addr, PTSTR szModule, DWORD len,
				 DWORD& section, DWORD& offset );
  static void IntelStackWalk( PCONTEXT pContext );
#if 1
  static void ImagehlpStackWalk( PCONTEXT pContext );
#endif
  static int __cdecl _tprintf(const TCHAR * format, ...);

#if 1
  static BOOL InitImagehlpFunctions( void );
#endif
		
  // Variables used by the class
  static TCHAR m_szPath[MAX_PATH];
  static TCHAR m_szLogFileName[MAX_PATH];
  static LPTOP_LEVEL_EXCEPTION_FILTER m_previousFilter;
#ifdef DEBUG_SISCO  
  static LOG_CTRL *m_pLogCtrl;
#endif  
  static HANDLE m_hReportFile;
  static BOOL m_bDebugExcept;
  static BOOL m_bFirstTime;

#if 1
  // Make typedefs for some IMAGEHLP.DLL functions so that we can use them
  // with GetProcAddress
  typedef BOOL (__stdcall * SYMINITIALIZEPROC)( HANDLE, LPSTR, BOOL );
  typedef BOOL (__stdcall *SYMCLEANUPPROC)( HANDLE );

  typedef BOOL (__stdcall * STACKWALKPROC)
					( DWORD, HANDLE, HANDLE, LPSTACKFRAME, LPVOID,
					PREAD_PROCESS_MEMORY_ROUTINE,PFUNCTION_TABLE_ACCESS_ROUTINE,
					PGET_MODULE_BASE_ROUTINE, PTRANSLATE_ADDRESS_ROUTINE );

  typedef LPVOID (__stdcall *SYMFUNCTIONTABLEACCESSPROC)( HANDLE, DWORD );

  typedef DWORD (__stdcall *SYMGETMODULEBASEPROC)( HANDLE, DWORD );

  typedef BOOL (__stdcall *SYMGETSYMFROMADDRPROC) ( HANDLE, DWORD, PDWORD, PIMAGEHLP_SYMBOL );
  
  typedef BOOL (__stdcall *SYMGETSEARCHPATHPROC) ( HANDLE, PSTR, DWORD );
  typedef BOOL (__stdcall *SYMSETSEARCHPATHPROC) ( HANDLE, PSTR );
  
  typedef DWORD (__stdcall *SYMGETOPTIONSPROC) ();
  typedef DWORD (__stdcall *SYMSETOPTIONSPROC) (DWORD);
  typedef BOOL (__stdcall *SYMGETLINEFROMADDRPROC) (HANDLE, DWORD, PDWORD, PIMAGEHLP_LINE);

  static SYMINITIALIZEPROC _SymInitialize;
  static SYMCLEANUPPROC _SymCleanup;
  static STACKWALKPROC _StackWalk;
  static SYMFUNCTIONTABLEACCESSPROC _SymFunctionTableAccess;
  static SYMGETMODULEBASEPROC _SymGetModuleBase;
  static SYMGETSYMFROMADDRPROC _SymGetSymFromAddr;
  static SYMGETSEARCHPATHPROC _SymGetSearchPath;
  static SYMSETSEARCHPATHPROC _SymSetSearchPath;

  static SYMGETOPTIONSPROC _SymGetOptions;
  static SYMSETOPTIONSPROC _SymSetOptions;
  static SYMGETLINEFROMADDRPROC _SymGetLineFromAddr;
  
#endif

};

extern MSJExceptionHandler g_MSJExceptionHandler;	//  global instance of class

#endif

#ifdef __cplusplus
extern "C" {
#endif

void ExpInit ();
void ExpRaiseDebugException ();
#ifdef DEBUG_SISCO
void ExpSetLogCtrl (LOG_CTRL *pLogCtrl);
#endif

#ifdef __cplusplus
}
#endif

#endif
