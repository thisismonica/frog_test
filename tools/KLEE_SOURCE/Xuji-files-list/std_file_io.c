#include <stdio.h>
#include <stdlib.h>
#include <critical_regions.h>
#include "plat/inc/xj_cosc.h"

static NU_SEMAPHORE stdio_fopen_sema;
extern FILE * __find_unopened_file( void );

void stdio_open_init()
{
#ifndef WIN32
    NU_Create_Semaphore(& stdio_fopen_sema, "fopen", 1, NU_PRIORITY );
#endif
}

#ifndef WIN32
FILE * fopen( const char * name, const char * mode )
{
    FILE * file,* ret;

    NU_Obtain_Semaphore(& stdio_fopen_sema, NU_SUSPEND );

    __begin_critical_region( files_access );
    file = __find_unopened_file();                              /*- mm 040429 -*/
    __end_critical_region( files_access );
    ret = freopen( name, mode, file );  

    NU_Release_Semaphore(& stdio_fopen_sema );

    return ret;
}

int fclose( FILE * file )
{
    int flush_result, close_result;

    if ( file == NULL )               /*- MM 960625 -*/
        return( EOF );                /*- MM 960625 -*/
    if ( file->mode.file_kind == __closed_file )
        return( 0 );

    flush_result = fflush( file );

    close_result = (* file->close_proc )( file->handle );

    //file->mode.file_kind = __closed_file;
    file->handle         = 0;

    if ( file->state.free_buffer )
#if _MSL_OS_DISK_FILE_SUPPORT
        free( file->buffer );
#else    
    return(- 1 );
#endif
    file->mode.file_kind = __closed_file;
    return(( flush_result || close_result ) ? EOF : 0 );
}
#endif
