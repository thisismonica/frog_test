//===-- klee_div_zero_check.c ---------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <klee/klee.h>

void klee_div_zero_check(long long z) {
  if (z == 0)
    klee_report_error(__FILE__, __LINE__, "divide by zero", "div.err");
}


//===klee_int_overflow_check.c ====//
//put here temporiraly since I donot know how to change MAKEFILE
#define MONICA

#ifdef MONICA

#define MAX_INTEGER 16

//#include <klee/klee.h>

void klee_int_overflow_check(long long z) {
  if (z > MAX_INTEGER)
    klee_report_error(__FILE__, __LINE__, "integer overflow", "div.err");
}

#endif
