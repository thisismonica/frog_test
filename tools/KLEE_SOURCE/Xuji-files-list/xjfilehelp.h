#ifndef _xjfilehelp_defined_
#define _xjfilehelp_defined_

//×¢£ºÎÄ¼şÂ·¾¶Ä¿Â¼Ö®¼äµÄ·Ö¸î·ûÎª'/'¡£
//    Ä¿Â¼Ãû²»ÄÜÒÔ'/'½áÎ²¡£
//    ¶ÔËùÓĞÎÄ¼şÒÔ¼°Ä¿Â¼µÄ·ÃÎÊ±ØĞëÊ¹ÓÃ¾ø¶ÔÂ·¾¶Ãû³Æ¡£

#define XJ_FILE_NAME_LEN_MAX    255

struct FileInformation
{
    char Name[XJ_FILE_NAME_LEN_MAX + 1];
    unsigned int Attribute; //ÎÄ¼ş¶ÁĞ´È¨ÏŞS_IREADºÍS_IWRITE
    unsigned long Time; //ÎÄ¼şĞŞ¸ÄÊ±¼ä
    long Size;
};

//º¯Êı¹¦ÄÜ£ºÉ¾³ıÎÄ¼ş¡£
//²Î    Êı£ºpath   ÎÄ¼şÂ·¾¶¡£
//·µ »Ø Öµ£º²Ù×÷³É¹¦·µ»Ø0£»´íÎóÊ§°Ü·µ»Ø-1¡£
int RemoveFile( const char * path );

//º¯Êı¹¦ÄÜ£º´´½¨Ä¿Â¼¡£
//²Î    Êı£ºpath   Ä¿Â¼Â·¾¶¡£
//·µ »Ø Öµ£º²Ù×÷³É¹¦·µ»Ø0£»´íÎóÊ§°Ü·µ»Ø-1¡£
int MakeDir( const char * path );

//º¯Êı¹¦ÄÜ£ºÖØÃüÃûÎÄ¼ş¡£
//²Î    Êı£ºoldPath ¾ÉµÄÎÄ¼şÂ·¾¶£»newPath ĞÂµÄÎÄ¼şÂ·¾¶¡£
//·µ »Ø Öµ£º²Ù×÷³É¹¦·µ»Ø0£¬´íÎóÊ§°Ü·µ»Ø-1¡£
int RenameFile( const char * oldPath, const char * newPath );

//º¯Êı¹¦ÄÜ£ºÖØÃüÃûÄ¿Â¼¡£
//²Î    Êı£ºoldPath ¾ÉµÄÄ¿Â¼Â·¾¶£»newPath ĞÂµÄÄ¿Â¼Â·¾¶¡£
//·µ »Ø Öµ£º²Ù×÷³É¹¦·µ»Ø0£¬´íÎóÊ§°Ü·µ»Ø-1¡£
int RenameDir( const char * oldPath, const char * newPath );

//º¯Êı¹¦ÄÜ£ºÉ¾³ıÄ¿Â¼¡£
//²Î    Êı£ºpath Ä¿Â¼Â·¾¶¡£
//·µ »Ø Öµ£º²Ù×÷³É¹¦·µ»Ø0£¬´íÎóÊ§°Ü·µ»Ø-1¡£
int RemoveDir( const char * path );

//º¯Êı¹¦ÄÜ£ºÉèÖÃÎÄ¼şĞÅÏ¢¡£
//²Î    Êı£ºpath ÎÄ¼şÂ·¾¶£»info ÎÄ¼şĞÅÏ¢¡£
//·µ »Ø Öµ£º²Ù×÷³É¹¦·µ»Ø0£»´íÎóÊ§°Ü·µ»Ø-1¡£
int SetFileInformation( const char * path, const struct FileInformation * info );

//º¯Êı¹¦ÄÜ£º»ñÈ¡ÎÄ¼şĞÅÏ¢¡£
//²Î    Êı£ºpath ÎÄ¼şÂ·¾¶£»info ÎÄ¼şĞÅÏ¢¡£
//·µ »Ø Öµ£º²Ù×÷³É¹¦·µ»Ø0£»´íÎóÊ§°Ü·µ»Ø-1¡£
int GetFileInformation( const char * path, struct FileInformation * info );

//º¯Êı¹¦ÄÜ£º²éÑ¯ÎÄ¼şÊÇ·ñ´æÔÚ¡£
//²Î    Êı£ºpath ÎÄ¼şÂ·¾¶¡£
//·µ »Ø Öµ£º´æÔÚ·µ»Ø1£¬²»´æÔÚ·µ»Ø0¡£
int FileExists( const char * path );

//º¯Êı¹¦ÄÜ£º²éÑ¯Ä¿Â¼ÊÇ·ñ´æÔÚ¡£
//²Î    Êı£ºpath Ä¿Â¼Â·Â·¾¶¡£
//·µ »Ø Öµ£º´æÔÚ·µ»Ø1£¬²»´æÔÚ·µ»Ø0¡£
int DirExists( const char * path );

//º¯Êı¹¦ÄÜ£º²éÑ¯´ÅÅÌ·ÖÇø¿ÕÏĞ´æ´¢¿Õ¼ä´óĞ¡¡£
//²Î    Êı£ºroot ´ÅÅÌ·ÖÇø¸ùÂ·¾¶¡£
//·µ »Ø Öµ£º´ÅÅÌ·ÖÇø¿ÕÏĞ´æ´¢¿Õ¼ä×Ö½ÚÊı¡£
int GetPrartitionFreeSpace( const char * root );

//º¯Êı¹¦ÄÜ£º±éÀúÖ¸¶¨Ä¿Â¼ÏÂµÄÎÄ¼ş£¬²¢¶ÔÃ¿¸öÎÄ¼ş»Øµ÷Ö¸¶¨µÄº¯Êı¡£
//²Î    Êı£ºdirPath Ä¿Â¼Â·¾¶£» filtercallback »Øµ÷º¯Êı£»filtercallbackcontext»Øµ÷º¯Êı²ÎÊı¡£
//·µ »Ø Öµ£ºÈç¹ûÄ¿Â¼´æÔÚ£¬·µ»ØÄ¿Â¼ÏÂÎÄ¼şµÄ¸öÊı£»·ñÔò£¬·µ»Ø-1¡£
int ListFile( const char * dirPath, 
    int (* filtercallback )( struct FileInformation * info, void * context ), 
    void * filtercallbackcontext );

//º¯Êı¹¦ÄÜ£º±éÀúÖ¸¶¨Ä¿Â¼ÏÂµÄÎÄ¼ş£¬²¢¶ÔÃ¿¸öÎÄ¼ş»Øµ÷Ö¸¶¨µÄº¯Êı¡£
//²Î    Êı£ºdirPath Ä¿Â¼Â·¾¶£» filtercallback »Øµ÷º¯Êı£»filtercallbackcontext»Øµ÷º¯Êı²ÎÊı¡£
//·µ »Ø Öµ£ºÈç¹ûÄ¿Â¼´æÔÚ£¬·µ»ØÄ¿Â¼ÏÂÎÄ¼şµÄ¸öÊı£»·ñÔò£¬·µ»Ø-1¡
int ListFile_lock( const char * dirPath, 
    int (* filtercallback )( struct FileInformation * info, void * context ), 
    void * filtercallbackcontext );


//º¯Êı¹¦ÄÜ£º¶ÁÈ¡FLASH·ÖÇøÄÚ´æ´¢¿éµÄ×´Ì¬¡£
//²Î    Êı£ºroot		FLASH·ÖÇø¸ùÂ·¾¶¡£
//			retired	Êä³ö»µ¿éÊı¡£
//			used		Êä³öÊ¹ÓÃµÄ¿éÊı¡£
//			empty	Êä³ö¿ÕÏĞµÄ¿éÊı¡£
//·µ »Ø Öµ£º²Ù×÷Ê§°Ü·µ»Ø-1£¬³É¹¦·µ»Ø0¡£
int GetFlashPrartitionBlockStatus( const char * root , int *retired, int *used,
	int *empty, int *page_size, int *pages_per_block, int *free_pages);

#endif //_xjfilehelp_defined_
