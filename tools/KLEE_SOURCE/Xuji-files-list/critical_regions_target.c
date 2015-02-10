/*MSL critical regions code by shenchen,20080923*/

#include <assert.h>
#include "plat/es_plat.h"

void __init_critical_regions( void );
void __kill_critical_regions( void );
void __begin_critical_region( int region );
void __end_critical_region( int region );

#define NUM_CRITICAL_REGIONS  9  

static NU_SEMAPHORE semas[NUM_CRITICAL_REGIONS];

static char * sema_names[NUM_CRITICAL_REGIONS] = 
{
    "atexit",
    "malloc",
    "stdin",
    "stdout",
    "stderr",
    "files",
    "console",
    "signal",
    "thread"
};

void __init_critical_regions( void )
{
    int i;
    STATUS res;  
    for( i = 0; i < NUM_CRITICAL_REGIONS; i ++)
    {
        res = NU_Create_Semaphore(& semas[i], sema_names[i], 1, NU_FIFO );
        assert( res == NU_SUCCESS );    
    }   
}

void __kill_critical_regions( void )
{
    int i;
    for( i = 0; i < NUM_CRITICAL_REGIONS; i ++)
    {
        NU_Delete_Semaphore(& semas[i] );
    }
}

void __begin_critical_region( int region )
{
    XJP_Mutex_Obtain(& semas[region], NU_SUSPEND );
}

void __end_critical_region( int region )
{
    XJP_Mutex_Release( & semas[region] );
}
