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

#include "klee.h"
int main() {
	float a0;
	klee_make_symbolic(&a0,sizeof(a0),"a0");
	float a1;
	klee_make_symbolic(&a1,sizeof(a1),"a1");
	float a2;
	klee_make_symbolic(&a2,sizeof(a2),"a2");
	float result=mid(a0,a1,a2);
	return;
}