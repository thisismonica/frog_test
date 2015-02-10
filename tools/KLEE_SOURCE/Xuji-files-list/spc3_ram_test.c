#include <stdio.h>
#include "spc3.h"

#define SPC3_REG_TEST_OFFSET 0x16
#define MIN_TEST_VAL         0
#define MAX_TEST_VAL         0xff

#define TEST_OK       0x55
#define TEST_FAILED   0x66

void spc3ramtest( u32 base );

void spc3ramtest( u32 base )
{   
    u8 readVal;
    u32 offset, writeVal;

    SPC3_GO_OFFLINE( base );

    for( offset = SPC3_REG_TEST_OFFSET; offset < SPC3_SPACE_SIZE; offset ++)
    {           
#if 1       
        for( writeVal = MIN_TEST_VAL; writeVal <= MAX_TEST_VAL; writeVal ++)
        {
            REG_WRITE_UBYTE( base, offset, writeVal );
            readVal = REG_READ_UBYTE( base, offset );
            if( readVal != writeVal )
                break;
        }

        if( writeVal <= MAX_TEST_VAL )
        {
            printf( "spc3 ram test failed.\n\r" );
            break;
        }
#else
        addr = base + ( offset << 1 ) ;
        *( volatile u16 *)( addr ) = 0xff;
        readVal = ( u8 )( (*( volatile u16 *)( addr )) & 0xff );
        if( readVal != 0xff )
            break;
#endif      
    }
}
