float mid( float x, float y, float z )
{
    float m;
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
float max( float x)
{
    return x;
}

#include <stdio.h>
int main(){
	float returnval=mid(0.0, 0.0, 0.0);
    printf("Return value is %f\n",returnval);
	return;
}