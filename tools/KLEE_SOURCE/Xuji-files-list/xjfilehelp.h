#ifndef _xjfilehelp_defined_
#define _xjfilehelp_defined_

//注：文件路径目录之间的分割符为'/'。
//    目录名不能以'/'结尾。
//    对所有文件以及目录的访问必须使用绝对路径名称。

#define XJ_FILE_NAME_LEN_MAX    255

struct FileInformation
{
    char Name[XJ_FILE_NAME_LEN_MAX + 1];
    unsigned int Attribute; //文件读写权限S_IREAD和S_IWRITE
    unsigned long Time; //文件修改时间
    long Size;
};

//函数功能：删除文件。
//参    数：path   文件路径。
//返 回 值：操作成功返回0；错误失败返回-1。
int RemoveFile( const char * path );

//函数功能：创建目录。
//参    数：path   目录路径。
//返 回 值：操作成功返回0；错误失败返回-1。
int MakeDir( const char * path );

//函数功能：重命名文件。
//参    数：oldPath 旧的文件路径；newPath 新的文件路径。
//返 回 值：操作成功返回0，错误失败返回-1。
int RenameFile( const char * oldPath, const char * newPath );

//函数功能：重命名目录。
//参    数：oldPath 旧的目录路径；newPath 新的目录路径。
//返 回 值：操作成功返回0，错误失败返回-1。
int RenameDir( const char * oldPath, const char * newPath );

//函数功能：删除目录。
//参    数：path 目录路径。
//返 回 值：操作成功返回0，错误失败返回-1。
int RemoveDir( const char * path );

//函数功能：设置文件信息。
//参    数：path 文件路径；info 文件信息。
//返 回 值：操作成功返回0；错误失败返回-1。
int SetFileInformation( const char * path, const struct FileInformation * info );

//函数功能：获取文件信息。
//参    数：path 文件路径；info 文件信息。
//返 回 值：操作成功返回0；错误失败返回-1。
int GetFileInformation( const char * path, struct FileInformation * info );

//函数功能：查询文件是否存在。
//参    数：path 文件路径。
//返 回 值：存在返回1，不存在返回0。
int FileExists( const char * path );

//函数功能：查询目录是否存在。
//参    数：path 目录路路径。
//返 回 值：存在返回1，不存在返回0。
int DirExists( const char * path );

//函数功能：查询磁盘分区空闲存储空间大小。
//参    数：root 磁盘分区根路径。
//返 回 值：磁盘分区空闲存储空间字节数。
int GetPrartitionFreeSpace( const char * root );

//函数功能：遍历指定目录下的文件，并对每个文件回调指定的函数。
//参    数：dirPath 目录路径； filtercallback 回调函数；filtercallbackcontext回调函数参数。
//返 回 值：如果目录存在，返回目录下文件的个数；否则，返回-1。
int ListFile( const char * dirPath, 
    int (* filtercallback )( struct FileInformation * info, void * context ), 
    void * filtercallbackcontext );

//函数功能：遍历指定目录下的文件，并对每个文件回调指定的函数。
//参    数：dirPath 目录路径； filtercallback 回调函数；filtercallbackcontext回调函数参数。
//返 回 值：如果目录存在，返回目录下文件的个数；否则，返回-1�
int ListFile_lock( const char * dirPath, 
    int (* filtercallback )( struct FileInformation * info, void * context ), 
    void * filtercallbackcontext );


//函数功能：读取FLASH分区内存储块的状态。
//参    数：root		FLASH分区根路径。
//			retired	输出坏块数。
//			used		输出使用的块数。
//			empty	输出空闲的块数。
//返 回 值：操作失败返回-1，成功返回0。
int GetFlashPrartitionBlockStatus( const char * root , int *retired, int *used,
	int *empty, int *page_size, int *pages_per_block, int *free_pages);

#endif //_xjfilehelp_defined_
