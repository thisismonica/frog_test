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
#include <stdio.h>
int main(){
	int returnval=fibo(5);
    printf("Return value is %d\n",returnval);
	return;
}