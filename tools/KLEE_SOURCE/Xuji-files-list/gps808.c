
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "plat/inc/xj_cosc.h"
#include "plat/inc/devi_extr.h"
#include "plat/es_plat.h"
#include "plat_specific/plat_specific.h"

VOID (* GPS1_PulseInterruptLisr)( INT );
INT (* GPS1_PulseInterruptClear)( VOID );


/* GPS脉冲中断处理函数 */
VOID GPS1_PulseInterrupt( INT handle );

/* 注册脉冲中断函数 */
VOID GPS1_RegisterPluseInterrupt( VOID (* new_lisr)( INT ), INT type )
{
	VOID (* oldlisr )( int );
			
    ESAL_GE_INT_Disable( ESAL_PR_IRQ1_INT_VECTOR_ID );
    if( new_lisr )
    {
        if( type & CTM_TYPE_GPS_IRIGB )
        	GPS1_PulseInterruptClear = DEVI_ClearIRIGB;
        else
            GPS1_PulseInterruptClear = DEVI_ClearGPS;
        GPS1_PulseInterruptLisr = new_lisr;
    	XJ_Register_LISR( ESAL_PR_IRQ1_INT_VECTOR_ID, GPS1_PulseInterrupt, & oldlisr );
    	ESAL_GE_INT_Enable( ESAL_PR_IRQ1_INT_VECTOR_ID, ESAL_TRIG_FALLING_EDGE  , 2 );
    }
    else
    {
        if( GPS1_PulseInterruptLisr )
        {
        	XJ_Register_LISR( ESAL_PR_IRQ1_INT_VECTOR_ID, NU_NULL, & oldlisr );
        }
    	GPS1_PulseInterruptLisr = new_lisr;
    	if( type & CTM_TYPE_GPS_IRIGB )
        	GPS1_PulseInterruptClear = DEVI_ClearIRIGB;
        else
            GPS1_PulseInterruptClear = DEVI_ClearGPS;
    }
}

/* GPS脉冲中断处理函数 */
VOID GPS1_PulseInterrupt( INT handle )
{
	/* 脉冲中断处理函数 */
	if( GPS1_PulseInterruptLisr )
	{
	    /* 清除GPS脉冲标志 */
	    if( GPS1_PulseInterruptClear )
	        GPS1_PulseInterruptClear();
		GPS1_PulseInterruptLisr( handle );
	}
}

