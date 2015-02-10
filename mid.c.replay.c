int mid( int x, int y, int z )
{
    int m;
    m=z;
    if(y<z)
        if(x<y)
            m=y;
        else if(x<z)
            m=y;
        else if(x>y)
            m=y;
        else if(x>z)
            m=x;
    return m;
}
int max( int x)
{
    return x;
}

#include <stdio.h>
int main(){
	int returnval=mid(0, -2147483648, 2147483647);
    printf("Return value is %d\n",returnval);
	return;
}