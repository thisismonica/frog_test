#ifndef _xjfilehelp_defined_
#define _xjfilehelp_defined_

//ע���ļ�·��Ŀ¼֮��ķָ��Ϊ'/'��
//    Ŀ¼��������'/'��β��
//    �������ļ��Լ�Ŀ¼�ķ��ʱ���ʹ�þ���·�����ơ�

#define XJ_FILE_NAME_LEN_MAX    255

struct FileInformation
{
    char Name[XJ_FILE_NAME_LEN_MAX + 1];
    unsigned int Attribute; //�ļ���дȨ��S_IREAD��S_IWRITE
    unsigned long Time; //�ļ��޸�ʱ��
    long Size;
};

//�������ܣ�ɾ���ļ���
//��    ����path   �ļ�·����
//�� �� ֵ�������ɹ�����0������ʧ�ܷ���-1��
int RemoveFile( const char * path );

//�������ܣ�����Ŀ¼��
//��    ����path   Ŀ¼·����
//�� �� ֵ�������ɹ�����0������ʧ�ܷ���-1��
int MakeDir( const char * path );

//�������ܣ��������ļ���
//��    ����oldPath �ɵ��ļ�·����newPath �µ��ļ�·����
//�� �� ֵ�������ɹ�����0������ʧ�ܷ���-1��
int RenameFile( const char * oldPath, const char * newPath );

//�������ܣ�������Ŀ¼��
//��    ����oldPath �ɵ�Ŀ¼·����newPath �µ�Ŀ¼·����
//�� �� ֵ�������ɹ�����0������ʧ�ܷ���-1��
int RenameDir( const char * oldPath, const char * newPath );

//�������ܣ�ɾ��Ŀ¼��
//��    ����path Ŀ¼·����
//�� �� ֵ�������ɹ�����0������ʧ�ܷ���-1��
int RemoveDir( const char * path );

//�������ܣ������ļ���Ϣ��
//��    ����path �ļ�·����info �ļ���Ϣ��
//�� �� ֵ�������ɹ�����0������ʧ�ܷ���-1��
int SetFileInformation( const char * path, const struct FileInformation * info );

//�������ܣ���ȡ�ļ���Ϣ��
//��    ����path �ļ�·����info �ļ���Ϣ��
//�� �� ֵ�������ɹ�����0������ʧ�ܷ���-1��
int GetFileInformation( const char * path, struct FileInformation * info );

//�������ܣ���ѯ�ļ��Ƿ���ڡ�
//��    ����path �ļ�·����
//�� �� ֵ�����ڷ���1�������ڷ���0��
int FileExists( const char * path );

//�������ܣ���ѯĿ¼�Ƿ���ڡ�
//��    ����path Ŀ¼··����
//�� �� ֵ�����ڷ���1�������ڷ���0��
int DirExists( const char * path );

//�������ܣ���ѯ���̷������д洢�ռ��С��
//��    ����root ���̷�����·����
//�� �� ֵ�����̷������д洢�ռ��ֽ�����
int GetPrartitionFreeSpace( const char * root );

//�������ܣ�����ָ��Ŀ¼�µ��ļ�������ÿ���ļ��ص�ָ���ĺ�����
//��    ����dirPath Ŀ¼·���� filtercallback �ص�������filtercallbackcontext�ص�����������
//�� �� ֵ�����Ŀ¼���ڣ�����Ŀ¼���ļ��ĸ��������򣬷���-1��
int ListFile( const char * dirPath, 
    int (* filtercallback )( struct FileInformation * info, void * context ), 
    void * filtercallbackcontext );

//�������ܣ�����ָ��Ŀ¼�µ��ļ�������ÿ���ļ��ص�ָ���ĺ�����
//��    ����dirPath Ŀ¼·���� filtercallback �ص�������filtercallbackcontext�ص�����������
//�� �� ֵ�����Ŀ¼���ڣ�����Ŀ¼���ļ��ĸ��������򣬷���-1�
int ListFile_lock( const char * dirPath, 
    int (* filtercallback )( struct FileInformation * info, void * context ), 
    void * filtercallbackcontext );


//�������ܣ���ȡFLASH�����ڴ洢���״̬��
//��    ����root		FLASH������·����
//			retired	�����������
//			used		���ʹ�õĿ�����
//			empty	������еĿ�����
//�� �� ֵ������ʧ�ܷ���-1���ɹ�����0��
int GetFlashPrartitionBlockStatus( const char * root , int *retired, int *used,
	int *empty, int *page_size, int *pages_per_block, int *free_pages);

#endif //_xjfilehelp_defined_
