int isLeapYear(int year)
{
    if ( year%400 == 0)
    {
        printf("%d is a leap year.\n", year);
        return 1;
    }

    if ( year%100 == 0)
    {
        printf("%d is not a leap year.\n", year);
        return 0;
    }

    if ( year%4 == 0 )
    {
        printf("%d is a leap year.\n", year);
        return 1;
    }

    else
    {
        printf("%d is not a leap year.\n", year);
        return 0;
    }
}
#include "klee.h"
int main() {
	int a0;
	klee_make_symbolic(&a0,sizeof(a0),"a0");
	int result=isLeapYear(a0);
	return;
}