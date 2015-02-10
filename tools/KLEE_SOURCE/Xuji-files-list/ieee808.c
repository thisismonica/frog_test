
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "plat/inc/xj_cosc.h"

UNSIGNED DEV_Float_To_IEEE( float fvalue );
float IEEE_To_Dev_Float( UNSIGNED udata );
/*=======================================================================*/
UNSIGNED DEV_Float_To_IEEE( float fvalue )
{
    float variable;
    UNSIGNED * p;

    variable = fvalue;
    p = ( UNSIGNED *)& variable;

    return( * p );
}

float IEEE_To_Dev_Float( UNSIGNED udata )
{
    UNSIGNED variable;
    float * p;

    variable = udata;
    p = ( float *)& variable;

    return( * p );
}
