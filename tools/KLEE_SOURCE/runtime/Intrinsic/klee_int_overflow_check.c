//===-- klee_int_overflow_check.c ---------------------------------------------===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is created by Monica Wang to let KLEE check for integer overflow
//
//===----------------------------------------------------------------------===//
#define MAX_INTEGER 16

#include <klee/klee.h>

void klee_int_overflow_check(long long z) {
  if (z > MAX_INTEGER)
    klee_report_error(__FILE__, __LINE__, "divide by zero", "div.err");
}
