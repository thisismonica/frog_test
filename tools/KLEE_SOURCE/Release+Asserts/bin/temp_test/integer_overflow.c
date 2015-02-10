#include<stdio.h>
int loop(int a){
	int num1;
	
	num1 = a + a;

	return num1;
}

#include "klee.h"
#include "ansi_prefix.PPCEABI.bare.h"

int main() {
	int a1;
	klee_make_symbolic(&a1,sizeof(a1),"a1");		
	int result;
	result = loop(a1);
	return 0;
	
}
