#include <stdio.h>

int divide_zero(int den){

	return 10/den;
}

#include "klee.h"
#include "ansi_prefix.PPCEABI.bare.h"

int main() {
    	int a0;
	klee_make_symbolic(&a0,sizeof(a0),"a0");

	int result;
	result = divide_zero(a0);

	return 0;
}

