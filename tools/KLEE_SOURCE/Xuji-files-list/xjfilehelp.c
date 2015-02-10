#include "xjfilehelp.h"
#include "yaffsfs.h"

int RemoveFile( const char * path )
{
    return yaffs_unlink( path );
}

int MakeDir( const char * path )
{
    return yaffs_mkdir( path , 0 );
}

int RenameFile( const char * oldPath, const char * newPath )
{
    return yaffs_rename( oldPath, newPath );
}

int RenameDir( const char * oldPath, const char * newPath )
{
    return yaffs_rename( oldPath, newPath );
}

int RemoveDir( const char * path )
{
    return yaffs_rmdir( path );
}

int FileExists( const char * path )
{
    yaffs_Object * obj;

    yaffsfs_Lock();
    obj = yaffsfs_FindObject( NULL, path, 0 );
    if( obj )
    {
        if( obj->variantType != YAFFS_OBJECT_TYPE_FILE )
            obj = 0;
    }
    yaffsfs_Unlock();

    return( obj ? 1 : 0 );
}

int DirExists( const char * path )
{
    yaffs_Object * obj;

    yaffsfs_Lock();
    obj = yaffsfs_FindObject( NULL, path, 0 );
    if( obj )
    {
        if( obj->variantType != YAFFS_OBJECT_TYPE_DIRECTORY )
            obj = 0;
    }
    yaffsfs_Unlock();

    return( obj ? 1 : 0 );
}

int GetPrartitionFreeSpace( const char * root )
{
    return(( int )yaffs_freespace( root ));
}

int SetFileInformation( const char * path, const struct FileInformation * info )
{ 
    struct iattr  attr;
    yaffs_Object * obj;
    unsigned int len;
    int ret;

    if((! path ) || (! info ))
        return - 1;

    yaffsfs_Lock();
    obj = yaffsfs_FindObject( NULL, path, 0 );
    yaffsfs_Unlock();  
    if(! obj )
        return - 1;

    yaffsfs_Lock();
    ret = yaffs_GetAttributes( obj,& attr );
    yaffsfs_Unlock();
    if( ret != YAFFS_OK )
        return - 1;

    attr.ia_valid = 0;
    if( info->Attribute & S_IREAD )
        attr.ia_mode |= S_IREAD;
    else 
        attr.ia_mode &= (~ S_IREAD );

    if( info->Attribute & S_IWRITE )
        attr.ia_mode |= S_IWRITE;
    else 
        attr.ia_mode &= (~ S_IWRITE );   
    attr.ia_valid |= ATTR_MODE;                     
	
    Y_TIME_CONVERT( attr.ia_mtime ) = info->Time;
    attr.ia_valid |= ATTR_MTIME;
	
    yaffsfs_Lock();
    ret = yaffs_SetAttributes( obj,& attr );
    yaffsfs_Unlock();

    if( ret == YAFFS_OK )
        return 0; 
	
    return - 1;
}

int GetFileInformation( const char * path, struct FileInformation * info )
{
    struct yaffs_stat s;    
    unsigned int len;

    if((! info ) || (! path[0] ) )
        return - 1;
    if( yaffs_stat( path, & s ) == 0 )
    {       
        len = strlen( path );
        len = len > XJ_FILE_NAME_LEN_MAX ? XJ_FILE_NAME_LEN_MAX : len;
        memcpy( info->Name, path, len );
        info->Name[len] = 0; 
        info->Attribute = s.yst_mode;     
        info->Time = s.yst_mtime;      
        info->Size = s.yst_size;        
        return 0;        
    }
    return - 1;
}

int GetFlashPrartitionBlockStatus( const char * root , int *retired, int *used,
	int *empty, int *page_size, int *pages_per_block, int *free_pages)
{
	yaffs_Device * dev = NULL;
	char * dummy;
	yaffs_BlockInfo *blockinfo;
	int i,blocks;

	if( (!root) || (!retired)  || (!used) || (!empty) )
		return -1;

	dev = yaffsfs_FindDevice( root,& dummy );
	if(! dev)
		return -1;

	*retired = *used = *empty = 0;
	blocks = dev->endBlock - dev->startBlock + 1;
	blockinfo = dev->blockInfo;
	for(i=0; i<blocks; i++,blockinfo ++)
	{
		if(blockinfo->blockState == YAFFS_BLOCK_STATE_EMPTY)
		{
			if(empty)
				*empty += 1;
		}
		else if( (blockinfo->blockState == YAFFS_BLOCK_STATE_ALLOCATING)
			|| (blockinfo->blockState == YAFFS_BLOCK_STATE_DIRTY)
			|| (blockinfo->blockState == YAFFS_BLOCK_STATE_FULL) )
		{
			if(used)
				*used += 1;
		}
		else if( (blockinfo->blockState == YAFFS_BLOCK_STATE_DEAD)
			|| (blockinfo->blockState == YAFFS_BLOCK_STATE_UNKNOWN) )
		{
			if(retired)
				*retired += 1;
		}
	}

	if(page_size)
		*page_size = dev->nBytesPerChunk;
	if(pages_per_block)
		*pages_per_block = dev->nChunksPerBlock;
	if(free_pages)
		*free_pages = dev->nFreeChunks;

	return 0;
}

int ListFile( const char * dirPath, 
    int (* filtercallback )( struct FileInformation * info, void * context ), 
    void * filtercallbackcontext )
{   
	int files, dir_path_len;
	struct list_head * i, * j;
	struct yaffs_stat s; 
	struct FileInformation inf;
	yaffs_Object * entry, * root;
	char dir_path[XJ_FILE_NAME_LEN_MAX + 1];   

	strncpy(dir_path, dirPath, XJ_FILE_NAME_LEN_MAX);
	dir_path[XJ_FILE_NAME_LEN_MAX] = 0;
	dir_path_len = strlen(dir_path);
	if(dir_path_len && dir_path[dir_path_len - 1] == '/')
	{
		dir_path[dir_path_len -1] = 0;
		dir_path_len --;
	}
	if(!dir_path_len)
		return -1;
	

	files = -1;
	yaffsfs_Lock();
	root = yaffsfs_FindObject( NULL, dir_path, 0 );
	if(root && (root->variantType == YAFFS_OBJECT_TYPE_DIRECTORY ) )
	{
		files = 0;
	    list_for_each_safe( i, j, & root->variant.directoryVariant.children )
	    {
	        entry = ( i ) ?  list_entry( i, yaffs_Object, siblings ) : NULL;

	        if( !entry )
				break;
			if( yaffsfs_DoStat(entry,&s) )
				break;

			inf.Attribute = s.yst_mode; 
			strcpy(inf.Name, dir_path);
			strcat(inf.Name, "/");
			yaffs_GetObjectName(entry, inf.Name + dir_path_len + 1 , XJ_FILE_NAME_LEN_MAX - dir_path_len - 1 );
			inf.Size = s.yst_size;
			inf.Time = s.yst_mtime;

			if( filtercallback )
	        {
	            if(! filtercallback(& inf, filtercallbackcontext ))
	                break;
	        }

			files ++;
	    }
	}
	yaffsfs_Unlock();

	return files;

}