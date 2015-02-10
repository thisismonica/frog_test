/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/04/15 17:47:08 $
 * $Revision: 1.5 $
 */

#ifndef __ansi_prefix__
#define __ansi_prefix__
#define __PPC_EABI__
#define __option(x) x
typedef int wchar_t;	

#include <os_enum.h>

#define __dest_os	__ppc_eabi

/*********************************************/
/* 		OS Specific Configuration Flags		 */
/*********************************************/

#ifndef _GCCPORT_
#define _GCCPORT_ 0
#endif

/* Console IO Support Configuration Flags */

#ifndef _MSL_CONSOLE_SUPPORT
#define _MSL_CONSOLE_SUPPORT 1
#endif

#ifndef _MSL_BUFFERED_CONSOLE
#define _MSL_BUFFERED_CONSOLE 1
#endif

#ifndef _MSL_CONSOLE_FILE_IS_DISK_FILE
#define _MSL_CONSOLE_FILE_IS_DISK_FILE 0
#endif

#ifndef _MSL_FILE_CONSOLE_ROUTINES
#define _MSL_FILE_CONSOLE_ROUTINES 0
#endif

#ifndef _MSL_NULL_CONSOLE_ROUTINES
	#if _MSL_CONSOLE_SUPPORT
		#define _MSL_NULL_CONSOLE_ROUTINES 0
	#else
		#define _MSL_NULL_CONSOLE_ROUTINES 1
	#endif
#endif

/* File IO Support Configuration Flags */

#ifndef _MSL_OS_DISK_FILE_SUPPORT
	#define _MSL_OS_DISK_FILE_SUPPORT	0
#endif

/* Time Support Configuration Flags */

#ifndef _MSL_OS_TIME_SUPPORT
	#define _MSL_OS_TIME_SUPPORT	0
#endif

#ifndef _MSL_CLOCK_T_AVAILABLE
	#define _MSL_CLOCK_T_AVAILABLE 	0
#endif

#ifndef _MSL_TIME_T_AVAILABLE
	#define _MSL_TIME_T_AVAILABLE	0
#endif

#if _MSL_OS_TIME_SUPPORT && ((!_MSL_CLOCK_T_AVAILABLE)  || (!_MSL_TIME_T_AVAILABLE))
	#error _MSL_CLOCK_T_AVAILABLE and _MSL_CLOCK_T_AVAILABLE must match _MSL_OS_TIME_SUPPORT
#endif

/* Thread Safety Configuration Flags */

#ifndef _MSL_THREADSAFE
	#define _MSL_THREADSAFE 0
#endif

#ifndef _MSL_PTHREADS
	#define _MSL_PTHREADS 0
#endif

#if _MSL_PTHREADS && !_MSL_THREADSAFE
	#error _MSL_PTHREADS and _MSL_THREADSAFE must match
#endif

#ifndef _MSL_LOCALDATA_AVAILABLE
	#define _MSL_LOCALDATA_AVAILABLE 1
#endif

/* Memory Allocation Flags */

/*  See alloc.c for more details on configuration options */

#ifndef _MSL_OS_ALLOC_SUPPORT
	#define _MSL_OS_ALLOC_SUPPORT 0
#endif
/*
#ifndef _MSL_OS_ALLOC_SUPPORT
	#define _MSL_OS_ALLOC_SUPPORT 1
#endif
*/

#ifndef _MSL_INLINE
	#if _GCCPORT_
		#define _MSL_INLINE __attribute__((weak))/* inline would have been better. But gcc linker throws error. */
	#endif
#endif

#ifndef _MSL_DO_NOT_INLINE
	#if _GCCPORT_
		#define _MSL_DO_NOT_INLINE __attribute__((weak))
	#endif
#endif



#if !_MSL_OS_ALLOC_SUPPORT && defined(_MSL_OS_DIRECT_MALLOC)
	#error Defining _MSL_OS_DIRECT_MALLOC requires _MSL_OS_ALLOC_SUPPORT
#endif

#ifndef _MSL_HEAP_EXTERN_PROTOTYPES
	#if _GCCPORT_
	#define _MSL_HEAP_EXTERN_PROTOTYPES \
		__attribute__((section(".init"))) extern char _heap_addr[];	/* starting address for heap */ \
		__attribute__((section(".init"))) extern char _heap_end[];	/* address after end byte of heap */
	#else
		#define _MSL_HEAP_EXTERN_PROTOTYPES \
		__declspec(section ".init") extern char _heap_addr[];	/* starting address for heap */ \
		__declspec(section ".init") extern char _heap_end[];	/* address after end byte of heap */
	#endif
#endif

#ifndef _MSL_HEAP_START
	#define _MSL_HEAP_START _heap_addr
#endif

#ifndef _MSL_HEAP_SIZE
	#define _MSL_HEAP_SIZE (_heap_end - _heap_addr)
#endif

/* #define _MSL_NO_MALLOC_DESIGN_2 */ /* lower overhead but slower allocation */
	
/* #define _MSL_NO_FIX_MALLOC_POOLS */ /* For heaps that are less than 64k */


/********************************************/
/* 		Additional Portability Flags 		*/
/********************************************/


#ifndef _MSL_LONGLONG
	#define _MSL_LONGLONG 1
#endif

#ifndef _MSL_FLOATING_POINT
	#define _MSL_FLOATING_POINT 1
#endif

#ifndef _MSL_WIDE_CHAR
	#define _MSL_WIDE_CHAR 1
#endif

#ifndef _MSL_USES_SUN_MATH_LIB
	#define _MSL_USES_SUN_MATH_LIB 1
#endif

#ifndef _MSL_POSIX
	#define _MSL_POSIX 0
#endif

#ifndef _MSL_NEEDS_EXTRAS
	#define _MSL_NEEDS_EXTRAS 0
#endif

#ifndef _MSL_COMPLEX
	#define _MSL_COMPLEX 0
#endif

#ifndef _MSL_IEC_599_ADDITIONS
	#define _MSL_IEC_599_ADDITIONS 1
#endif

#ifndef _MSL_SUPPORTS_PRINTING_QUIET_NANS
	#define _MSL_SUPPORTS_PRINTING_QUIET_NANS 0
#endif

/************************************************/
/* 		Flags to reduce code and data size 		*/
/*												*/
/*	use ansi_prefix.PPCEABI.bare.SZ.h to set	*/
/*	flags for code and data size reduction		*/
/************************************************/

#ifndef _MSL_STRERROR_KNOWS_ERROR_NAMES
	#define _MSL_STRERROR_KNOWS_ERROR_NAMES	1	/* 0: for reduced code and data size by reducing MSL functionality */
#endif

#ifndef _MSL_ASSERT_DISPLAYS_FUNC
	#define _MSL_ASSERT_DISPLAYS_FUNC	1		/* 0: for reduced code and data size by reducing MSL functionality */
#endif

#ifndef _MSL_C_LOCALE_ONLY
	#define _MSL_C_LOCALE_ONLY	0				/* 1: for reduced code and data size by reducing MSL functionality */
#endif

#ifndef _MSL_C99
	#define _MSL_C99	1						/* 0: for reduced code and data size by reducing MSL functionality */
#endif

#ifndef _MSL_C99_MATH_LEAK
	#define _MSL_C99_MATH_LEAK	0
#endif

#ifndef _MSL_INCOMPLETE_IEEE_EXCEPTION_MODEL
	#ifndef _MSL_C99
		#error _MSL_C99 must be defined.
	#endif
	#ifndef _MSL_C99_MATH_LEAK
		#error _MSL_C99_MATH_LEAK must be defined.
	#endif
	#if (defined(__POWERPC__) && defined(__SPE__)) && (_MSL_C99 || _MSL_C99_MATH_LEAK)
		#define _MSL_INCOMPLETE_IEEE_EXCEPTION_MODEL 1
	#else
		#define _MSL_INCOMPLETE_IEEE_EXCEPTION_MODEL 0
	#endif
#endif

#ifndef _MSL_C_HAS_CPP_EXTENSIONS
	#define _MSL_C_HAS_CPP_EXTENSIONS 1
#endif

#ifndef _MSL_C99_PRINTF_SCANF
	#define _MSL_C99_PRINTF_SCANF	1			/* 0: for reduced code and data size by reducing MSL functionality */
#endif

#ifndef _MSL_FLOATING_POINT_IO
	#define _MSL_FLOATING_POINT_IO	1			/* 0: for reduced code and data size by reducing MSL functionality */
#endif

#ifndef _MSL_USES_EXIT_PROCESS
	#define _MSL_USES_EXIT_PROCESS 1
#endif

#ifndef _MSL_FLT_EVAL_METHOD
	#define _MSL_FLT_EVAL_METHOD  0
#endif

#if defined(__cplusplus) && __embedded_cplusplus == 0
    #define _MSL_USING_NAMESPACE	
#endif

#define __ANSI_OVERLOAD__

#define _MSL_INTEGRAL_MATH

#define _MSL_FLOAT_HEX 0

#define __CW_MAX_PROCESSES__ 1		/* see ExceptionPPC.cp */

/* __VEC__ is defined to 1 if altivec_model is on */
#ifndef __VEC__
	#undef __ALTIVEC__					/*- cc 020907 -*/
#endif

#ifdef __PPCGENERIC__
	#define __PPC_GENERIC__			/* backwards compatibility */
#endif

/******************************************************** 
 * Platform Specific Configuration:						*
 * va_list.h - Platform specific typedef of _va_list	*
 ********************************************************/
#if _GCCPORT_
	typedef __builtin_va_list va_list;
	#define _MSL_VA_LIST_TYPE va_list;
	#define _MSL_VA_LIST_DEFINED
#else
typedef struct __va_list_struct {
	char gpr;
	char fpr;
	char reserved[2];
	char *input_arg_area;
	char *reg_save_area;
} __va_list[1];

#define _MSL_VA_LIST_TYPE __va_list
#endif


/************************************************************ 
 * Platform Specific Configuration:						    *
 * cloat - Platform specific define of _MSL_DOUBLE_SIZE		*
 ************************************************************/
#if __option(double_is_4_bytes) == 1
	#ifndef _MSL_DOUBLE_SIZE
		#define _MSL_DOUBLE_SIZE 32
	#endif

	#ifndef _MSL_LONG_DOUBLE_SIZE
		#define _MSL_LONG_DOUBLE_SIZE 32
	#endif

	#ifndef _MSL_USES_SUN_DP_MATH_LIB
		#define _MSL_USES_SUN_DP_MATH_LIB 0
	#endif

	#ifndef _MSL_USES_SUN_SP_MATH_LIB
		#define _MSL_USES_SUN_SP_MATH_LIB 1
	#endif

	#ifndef _MSL_ONLY_SINGLE_PRECISION_MATH_LIB
		#define _MSL_ONLY_SINGLE_PRECISION_MATH_LIB 1
	#endif
#else
	/* use defaults in cfloat */
#endif

/************************************************************ 
 * Platform Specific Configuration:						    *
 * ansi_fp.h - Platform specific define of SIGDIGLEN		*
 ************************************************************/

#define _MSL_SIGDIGLEN		 36



/************************************************************ 
 * Platform Specific Configuration:	floating support		*
 ************************************************************/

/* Two macros determine what the floating support will be 	*/
/* in the MSL and runtime support.							*/
/*															*/
/*	Define _MSL_FLOATING_POINT is defined to 0 if 			*/
/*  we don't want any floating point.  It is defined to 1 	*/
/*	for software and hardware support.						*/
/*															*/
/*	_No_Floating_Point_Regs	is defined unless we actually	*/
/*							want hardware floating point	*/
/*							because the chip has an FPU.	*/
/*							Note that the 8xx chips do NOT	*/
/*							have an FPU.					*/

/* We use the project settings to determine what type of	*/
/* of floating point support to enable.	 This is a 2 step	*/
/* process since __option() will only return 0 or 1 and 	*/
/* there are 3 states to test.								*/
/* __option(floatingpoint) will return 0 if you have 		*/
/* selected no floating point support and 1 if you have		*/
/* selected either software or hardware floating point.		*/
/*		0 - no floating point								*/
/*		1 - software or hardware floating point				*/
/* __option(sfp_emulation) will return 1 if you have 		*/
/* selected software floating point support and 0			*/
/* otherwise.		*/
/*		0 - no floating point or hardware floating point	*/
/*		1 - software floating point							*/
	
#if __option(floatingpoint)==0

	/* No floating point support 		*/
	#if _MSL_FLOATING_POINT
		#undef _MSL_FLOATING_POINT
		#define _MSL_FLOATING_POINT	0
	#endif
	#if _MSL_FLOATING_POINT_IO
		#undef _MSL_FLOATING_POINT_IO
		#define _MSL_FLOATING_POINT_IO	0
	#endif
	
	#define _No_Floating_Point_Regs
	#define _MSL_NO_MATH_LIB

#elif __option(floatingpoint)==1

	/* Define to 0 if faster, non-IEEE 	*/
	/* routines are desired 			*/
	#ifndef __USING_IEEE_MATH__
		#define __USING_IEEE_MATH__ 1
	#endif

	/* define as a bit combination of MATH_ERRNO (to set errno on error)
	/*	and/or MATH_ERREXCEPT (to depend on an enabled FP exception
	/*	and OS support) */
	
#if __option(sfp_emulation)==1

	/* Software floating point support 	*/
	#define _No_Floating_Point_Regs

#else

	/* Hardware floating point support */
	#if !__USING_IEEE_MATH__ && __supports(target_instruction, "frsqrte")

	/* This intrinsic is fast but not IEEE accurate	*/
    /* It is also not available on the 500 series processors */
		#ifndef __USING_INTRINSIC_SQRT__
		#define __USING_INTRINSIC_SQRT__ 1
		#endif

				/* Enable for faster but slightly less accurate sqrtf() */
		#ifndef __LESS_ACCURATE_FP__
		#define __LESS_ACCURATE_FP__ 0
		#endif

	#endif

	/* This intrinsic is fast and 	 	*/
	/* accurate							*/
	#ifndef __USING_INTRINSIC_FABS__
	#define __USING_INTRINSIC_FABS__ 1
	#endif

	#endif	/*__option(sfp_emulation) */

#endif/* __option(floatingpoint)*/
#endif /* __ansi_prefix__ */

/* Change record:
 * MEA 970720 Changed __ppc_eabi_bare to __ppc_eabi.  Added
 * 			  specific defines for OS services.
 * MEA 970804 Added automatic support for floating point.
 * MEA 980405 Undefined windows only __tls
 * MEA 990824 Removed outdated defines and added defines and comments
 *			  for new allocation methods.
 * MEA 991025 Added _No_Altivec when Altivec isn't enabled
 * MEA 010104 namespaces are enabled by defining _MSL_USING_NAMESPACE
 * MEA 011101 allow control of IEEE or faster math routines
 * TU  010730 added __PPC_GENERIC__
 * cc  010927 Removed circular dependency 
 * cc  011022 Added #define _MSL_USES_EXIT_PROCESS 1
 * cc  011211 Added _MSL_SIGDIGLEN
 * hh  020214 Renamed _MSL_PRO4_MALLOC to _MSL_CLASSIC_MALLOC
 * cc  020709 Changed the define _No_Altivec to undef __ALTIVEC__ to keep common source base
 * ejs 030110 Removed #undef __ALTIVEC__ -- not necessary since __VEC__ is correct flag
 * ejs 030122 Ported to msl_core_2
 * cc  040217 Changed _No_Disk_File_OS_Support to _MSL_OS_DISK_FILE_SUPPORT
 * JWW 040401 Changed _No_Alloc_OS_Support to _MSL_OS_ALLOC_SUPPORT
 * JWW 040405 Changed _Unbuffered_Console to _MSL_BUFFERED_CONSOLE
 * cc  041115 Turned on _MSL_BUFFERED_CONSOLE to increase speed of console 
 */
