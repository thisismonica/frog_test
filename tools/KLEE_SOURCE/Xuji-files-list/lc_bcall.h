/* 通信任务把要长时间执行的函数的入口地址和参数通过队列传递给报告处理任务,
报告处理任务执行后,把执行结果通过队列传递给通信任务,通讯任务发送执行结果*/

#include "plat/net_inc/lc_defs.h"
#include "plat/net_inc/gwmem.h"
#include "plat/net_inc/lc_defs.h"

/* Check to see if the file has been included already.  */
#ifndef LC_BCALL_H
#define LC_BCALL_H

#define LCC_BACKDATA_MAXLENGTH   LC_MAX_FRAME_DATA_SIZE   /* 通讯数据最大长度 */
#define LCC_MAXCOMM_TYPE         0xFF   /* 后台最大通讯命令类型 */
#define LCC_QUE_SEND_DELAY       200    /* 消息队列发送延时时间 */

/* 回调结构 */
typedef struct STRUCT_LC_BLOCK_CB
{
    GWAPP_ACK_FUNCTION  pCallback;
    VOID                * pSevData;
    VOID                * pSendData;
}LC_BLOCK_CB;

/* 注册后台回调函数的最大个数 */
extern LC_BLOCK_CB LC_BCallFunBuff[LCC_MAXCOMM_TYPE];

/* 注册后台执行的就地命令 */
INT LCC_RegcBackGroundComm( GW_CHANNEL * pChl, UNSIGNED_CHAR iCommType, VOID * CallBack );
/* 通讯任务中关于注册后台的通讯命令的处理函数 */
INT LC_BCallManage( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code );
/* 后台任务执行回调函数 */
VOID  LC_Block( VOID );
/* 通讯任务中检测是否有回调函数执行完,是否需要回复报文 */
INT LC_BCall_Ret( GW_CHANNEL * pChl, UNSIGNED_CHAR * dst,
    UNSIGNED_CHAR * dst_port );
/* 在执行命令的过程中,如果执行失败则将命令注册到后台监控任务中去执行 */
INT LCC_Run_RegcBackGroundComm( GW_CHANNEL * pChl, VOID * Dst, VOID * Src, UNSIGNED_CHAR code,
    UNSIGNED_CHAR iCommType, VOID * CallBack );
/* 经后台任务执行的命令,将报文执行结束后,将报文直接填入消息队列中;
长度为char的个数,缓冲区的数据存储为首地址为控制域,第二个数据为数据长度(不包括控制域及长度本身) */
INT  LCC_BCall_BackTaskSendMessData( INT iNum, UNSIGNED_CHAR * pBuffer );
/* 后台任务处理的通讯报文消息队列初始化函数 */
INT LCC_BackTaskBCallBufferInit( VOID );
/* 检测后台命名报文缓冲区中是否有报文 */
INT LC_BCallCheckBackTaskMess( UNSIGNED_CHAR * dst, UNSIGNED_CHAR * dst_port );
#endif
