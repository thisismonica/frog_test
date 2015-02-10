/* MSL
 * Copyright © 1995-2007 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2009/11/10 04:02:15 $
 * $Revision: 1.3 $
 */

/* Include your OS dependent include files */
#include <stdio.h>
#include "file_io.h"
#include "yaffsfs.h"

#if 0
typedef struct {
    /* change these members to whatever is appropriate */
    short   refnum;
    void *  spec;
} temp_file_info;

static temp_file_info   temp_info[FOPEN_MAX];

static temp_file_info * find_temp_info( short refnum )
{
    int                             i;
    temp_file_info *    p;

    for ( i = 0, p = temp_info; i < FOPEN_MAX; i ++, p ++)
        if ( p->refnum == refnum )
        return( p );

    return( 0 );
}
#endif

int __open_file( const char * name, __file_modes mode, __file_handle * handle )
{   
    int fd;
    int openFlags, openModes;

    openModes = 0;
    if( mode.open_mode == __create_if_necessary )
        openFlags = O_CREAT;        
    else if( mode.open_mode == __create_or_truncate )
        openFlags = ( O_TRUNC | O_CREAT );
    else //if(mode.open_mode == __must_exist)
        openFlags = 0;
  
  /*  

    switch( mode.io_mode )
    {
    case __read:
        openFlags |= O_RDONLY;
        break;
    case __write:
        openFlags |= O_WRONLY;
        break;
    case __read_write:
        openFlags |= O_RDWR;
        break;
    case __append:
        openFlags |= O_APPEND;
        break;
    default:
        break;
    }   
    
*/

    if(mode.io_mode & __read_write)
        openFlags |= O_RDWR;
    else
    {
	    if(mode.io_mode & __read)
	        openFlags |= O_RDONLY;
	    
	    if(mode.io_mode & __write)
	        openFlags |= O_WRONLY;
    }
    
    if(mode.io_mode & __append)
        openFlags |= O_APPEND;    

    
    if( openFlags & O_CREAT )
        openModes = ( S_IREAD | S_IWRITE );

    fd = yaffs_open( name, openFlags, openModes );
    if( fd >= 0 ) 
    {
        * handle = ( __file_handle )fd;
        return( __no_io_error );        
    }
    else
        return( __io_error );   
}

int __open_temp_file( __file_handle * handle )
{   
    return( __io_error );
#if 0
    char                    temp_name[L_tmpnam];
    temp_file_info *        info;
    void *                  spec;
    int                     ioresult;

    __temp_file_name( temp_name, & spec );

    if (!( info = find_temp_info( 0 )))
        return( __io_error );

    ioresult = __open_file( temp_name, __temp_file_mode, handle );

    if ( ioresult == __no_io_error )
    {
        info->refnum = * handle;
        info->spec   = spec;
    }

    return( ioresult );
#endif
}

int __read_file( __file_handle handle, unsigned char * buffer, size_t * count, __ref_con ref_con )
{
    int len = yaffs_read(( int )handle, buffer,* count );       
    if( len == - 1 )
        return( __io_error );
    else
    {
        * count = len;
        if( len == 0 )
            return( __io_EOF );
        else
            return( __no_io_error );        
    }
}

int __write_file( __file_handle handle, unsigned char * buffer, size_t * count, __ref_con ref_con )
{   
    int len = yaffs_write(( int )handle, buffer,* count );      
    if( len == - 1 )
        return( __io_error );
    else
    {
        * count = len;
        if( len == 0 )
            return( __io_EOF );
        else
            return( __no_io_error );        
    }
}

int __position_file( __file_handle handle, fpos_t * position, int mode, __ref_con ref_con )
{
    int pos = yaffs_lseek(( int )handle,* position, mode );
    if( pos == - 1 )
        return( __io_error );
    else
    {
        * position = pos;
        return( __no_io_error );
    }   
}

int __close_file( __file_handle handle )
{
    if( yaffs_close(( int )handle ) == 0 )
        return( __no_io_error );
    else
        return( __io_error );
}

void __temp_file_name( char * name_str, void * filespec )
{       
    name_str[0] = 0;
}

int __delete_file( const char * name )
{
    if( yaffs_unlink( name ) == 0 )
        return( __no_io_error );
    else
        return( __io_error );   
}

int __rename_file( const char * old_name, const char * new_name )
{
    if( yaffs_rename( old_name, new_name ) == 0 )
        return( __no_io_error );
    else
        return( __io_error );   
}
