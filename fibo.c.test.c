int fibo(int n)
{
    // f(0) = 0
    if(n==0)
    {
        return 0;
    }
    // f(1) = 1
    else if (n==1)
    {
        return 1;
    }
    // Calling fibo recursively
    else if (n>1)
    {
        return fibo(n-1) + fibo(n-2);
    }
    // Error condition
    else
    {
        return -1;
    }
}
#include "klee.h"
int main() {
	int a0;
	klee_make_symbolic(&a0,sizeof(a0),"a0");
	int result=fibo(a0);
	return;
}