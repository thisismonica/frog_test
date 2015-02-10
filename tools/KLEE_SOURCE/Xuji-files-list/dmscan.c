
#include <stdio.h>
#include "plat/inc/xj_cosc.h"

#define MAX_DM_SCAN_NUMBER      32
NU_MEMORY_POOL * _dmScanTable[MAX_DM_SCAN_NUMBER];
INT _dmScanNumber = 0;
unsigned long DMScan_available[ MAX_DM_SCAN_NUMBER ];

void DMScan_Init();
void DMScan_Exec();

void DMScan_Init()
{
    UNSIGNED require;
    require = NU_Established_Memory_Pools();
    if ( require > MAX_DM_SCAN_NUMBER )
    {
        require = MAX_DM_SCAN_NUMBER;
    }
    _dmScanNumber = NU_Memory_Pool_Pointers( _dmScanTable, require );
}

void DMScan_Exec()
{
    INT i;
    STATUS ret;
    CHAR name[NU_MAX_NAME];
    VOID * start_address;
    UNSIGNED pool_size;
    UNSIGNED min_allocation;
    UNSIGNED available;
    OPTION suspend_type;
    UNSIGNED tasks_waiting;
    NU_TASK * first_task;

    for ( i = 0 ; i < _dmScanNumber ; i ++)
    {
        ret = NU_Memory_Pool_Information( _dmScanTable[i],
            name, & start_address, & pool_size,
            & min_allocation, & available,
            & suspend_type, & tasks_waiting,
            & first_task );
        DMScan_available[i] = available;
    }
}
