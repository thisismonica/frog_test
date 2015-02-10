#ifndef   _IED_DB_H
#define   _IED_DB_H

#define GW_NULL     0

typedef              void   GW_VOID;
typedef              char   GW_CHAR;
typedef     unsigned char   GW_UCHAR;
typedef     unsigned char   GW_BYTE;

typedef              int    GW_INT;
typedef     int             GW_SHORT;
typedef     unsigned int    GW_USHORT;

typedef     long            GW_LONG;
typedef     unsigned long   GW_ULONG;

typedef              int    GW_BOOL;

typedef              float  GW_FLOAT;
typedef              double GW_DOUBLE;

/* True or False */
#define GW_TRUE  (0==0)
#define GW_FALSE (0==1)

/* Ranges for various types defined above */
#define GW_BOOL_MIN    GW_FALSE
#define GW_BOOL_MAX    GW_TRUE

#define GW_BYTE_MIN    ((GW_BYTE)(  0))
#define GW_BYTE_MAX    ((GW_BYTE)(255))

#define GW_CHAR_MIN    ((GW_CHAR)(-127-1))
#define GW_CHAR_MAX    ((GW_CHAR)( 127))

#define GW_UCHAR_MIN   ((GW_UCHAR)(  0))
#define GW_UCHAR_MAX   ((GW_UCHAR)(255))

#define GW_SHORT_MIN   ((GW_SHORT)(-32767L-1L))
#define GW_SHORT_MAX   ((GW_SHORT)( 32767L))

#define GW_USHORT_MIN  ((GW_USHORT)(    0))
#define GW_USHORT_MAX  ((GW_USHORT)(65535UL))

#define GW_LONG_MIN    ((GW_LONG)(-2147483647L-1L))
#define GW_LONG_MAX    ((GW_LONG)( 2147483647L))

#define GW_ULONG_MIN   ((GW_ULONG)(         0))
#define GW_ULONG_MAX   ((GW_ULONG)(4294967295UL))

#define GW_SFLOAT_SMALLEST ((GW_SFLOAT)( 1.175494351E-38))
#define GW_SFLOAT_MIN      ((GW_SFLOAT)(-3.402823466E+38))
#define GW_SFLOAT_MAX      ((GW_SFLOAT)( 3.402823466E+38))

#define GW_DOUBLE_SMALLEST ((GW_DOUBLE)( 2.2250738585072014E-308))
#define GW_DOUBLE_MIN      ((GW_DOUBLE)(-1.7976931348623158E+308))
#define GW_DOUBLE_MAX      ((GW_DOUBLE)( 1.7976931348623158E+308))

#define GW_POINT_IV         0x40

#define GW_MASKBIT_8        0xff
#define GW_MASKBIT_16       0xffff
#define GW_MASKBIT_24       0xffffff

#define I3DEF_DCS_MASK      0x03
#define I3DEF_DCS_OFF       0x01 /*double command,off(open)*/
#define I3DEF_DCS_ON        0x02 /*double command,on(close)*/

#define I3DEFS_RCS_MASK     0x03
#define I3DEFS_RCS_OFF      0x01 /*regular command,down*/
#define I3DEFS_RCS_ON       0x02 /*regular command,up*/

#define I3DEF_DPI_IV        0x80 /*0-validate ,1-invalidate*/
#define I3DEF_DPI_NT        0x40 /*0-nowtime value, 1-not nowtime value*/
#define I3DEF_DPI_BL        0x20 /*0-not subsititute,1-subsiitute*/
#define I3DEF_DPI_SB        0x10 /*0-unlock,1-lock*/
#define I3DEF_DPI_MASK      0x03
#define I3DEF_DPI_OFF       0x01 /*double point,off(open)*/
#define I3DEF_DPI_ON        0x02 /*double point,on(close)*/

#define I3DEF_SPI_IV        0x80 /*0-validate ,1-invalidate*/
#define I3DEF_SPI_NT        0x40 /*0-nowtime value, 1-not nowtime value*/
#define I3DEF_SPI_BL        0x20 /*0-not subsititute,1-subsiitute*/
#define I3DEF_DPI_SB        0x10 /*0-unlock,1-lock*/
#define I3DEF_SPI_MASK      0x01
#define I3DEF_SPI_OFF       0x00 /*single point,off(open)*/
#define I3DEF_SPI_ON        0x01 /*single point,on(close)*/

#define GET_SECT(pid)       ((pid >> 16) & 0xff)
#define GET_FUN(pid)        ((pid >> 8) & 0xff)
#define GET_INF(pid)        (pid & 0xff)

#define   GW_INVALIDATE_PID 0x00000000

/*point id data type*/
typedef    GW_ULONG    GW_PID;
typedef    GW_ULONG    GW_IOA;
typedef    GW_UCHAR    GW_INF;
typedef    GW_UCHAR    GW_FUN;

typedef    GW_UCHAR    GW_SPI;
typedef    GW_UCHAR    GW_DPI;
typedef    GW_USHORT   GW_STI;
typedef    GW_FLOAT    GW_MEI;
typedef    GW_ULONG    GW_ITI;
typedef    GW_UCHAR    GW_DCC;
typedef    GW_UCHAR    GW_SCC;
typedef    GW_UCHAR    GW_CCC;

/*gateway time tag*/
typedef struct STRUCT_GW_TIME
{
    GW_USHORT  millionsecond;
    GW_BYTE    minute;
    GW_BYTE    hour;
    GW_BYTE    day;
    GW_BYTE    month;
    GW_USHORT  year;
}   GW_TIME;

/*enum point type*/
typedef enum  ENUM_GW_POINT_TYPE
{
    GW_POINT_TYPE_UNDEF,    /*reserve for user define*/
    GW_POINT_TYPE_M_DP,     /*double point information in monitor direction*/
    GW_POINT_TYPE_C_DP,     /*double point information in control direction*/
    GW_POINT_TYPE_M_SP,     /*single point information in monitor direction*/
    GW_POINT_TYPE_C_SP,     /*single point information in monitor direction*/
    GW_POINT_TYPE_M_TAP,    /*step information in monitor direction*/
    GW_POINT_TYPE_C_TAP,    /*step information in control direction*/
    GW_POINT_TYPE_M_ME,     /*measure in monitor direction*/
    GW_POINT_TYPE_M_TI,     /*counter in monitor direction*/
    GW_POINT_TYPE_MAX
}   GW_POINT_TYPE;

/*VLD与接口交换的数据类型*/
typedef enum  ENUM_GW_VLD_DATA_TYPE
{
    GW_VLD_DATA_TYPE_SWITCH = 0,/*开关量*/
    GW_VLD_DATA_TYPE_ANALOG = 1,/*模拟量*/
    GW_VLD_DATA_TYPE_MAX
}   GW_VLD_DATA_TYPE;

/*enum protocol type,only IEC103 slave*/
typedef enum    ENUM_GW_PROTOCOL_TYPE
{
    GW_PROTOCOL_TYPE_S103,
    GW_PROTOCOL_TYPE_MAX
}   GW_PROTOCOL_TYPE;

/*point transmit direction*/
typedef enum   ENUM_GW_POINT_DIR
{
    GW_POINT_DIR_C,/*control*/
    GW_POINT_DIR_M,/*monitor*/
    GW_POINT_DIR_P /*Peer to peer,not support now*/
}   GW_POINT_DIR;

/**/
typedef struct STRUCT_GW_S103_POINT_CONFIG
{
    GW_POINT_DIR    dir;        /*transmit direction*/
    GW_UCHAR        addr;       /*address*/
    GW_UCHAR        sect;       /*section*/
    GW_UCHAR        fun;        /*function,for monitor direction*/
    GW_UCHAR        inf;        /*information number,for monitor direction*/
    GW_UCHAR        control_fun;/*function,for control direction*/
    GW_UCHAR        control_inf;/*information number,for control direction*/
    GW_ULONG        create_timeout;
}   GW_S103_POINT_CONFIG;

typedef union   UNION_GW_POINT_CONFIG
{
    GW_S103_POINT_CONFIG    s103_config;
}   GW_POINT_CONFIG;

/*enum cause of transmition*/
typedef enum
{
    GW_COT_M_CIRCLE,    /*periodic, cyclic,in monitor direction*/
    GW_COT_M_OUTBURST,  /*spontaneous,in monitor direction*/
    GW_COT_C_REQUEST,   /*require data,in control direction*/
    GW_COT_M_REPLY      /*replay command,in monitor direction*/
}   GW_COT;

/*
#define   GW_MAX_UNDEF_SIZE     2
typedef struct STRUCT_POINT_UNDEF
{
    GW_UCHAR        size;
    GW_UCHAR        data[GW_MAX_UNDEF_SIZE];
}   GW_POINT_UNDEF;

*/

typedef union UNION_DATA_VALUE
{
    GW_SPI              pvSP;
    GW_DPI              pvDP;
    GW_USHORT           pvTAP;
    GW_DCC              pvDC;
    GW_SCC              pvSC;
    GW_CCC              pvCC;
    GW_ULONG            pvME;
    GW_ULONG            pvIT;
    GW_ULONG            pvUndef;
}   GW_DATA_VALUE;

typedef struct STRUCT_POINT_VALUE
{
    GW_PID              pid;
    GW_DATA_VALUE       data;
}GW_POINT_VALUE;

typedef enum ENUM_SOE_TYPE
{
    GW_SOE_TYPE_UNDEF,
    GW_SOE_TYPE_MEASURE,                /*遥测*/
    GW_SOE_TYPE_IT,                     /*电度*/
    GW_SOE_TYPE_M_SP,                   /*单点遥信*/
    GW_SOE_TYPE_M_DP,                   /*双点遥信*/
    GW_SOE_TYPE_M_TAP,
    GW_SOE_TYPE_BREAKER_SELECT_FAIL,    /*断路器选择失败*/
    GW_SOE_TYPE_BREAKER_SELECT_CONFIRM, /*断路器选择成功*/
    GW_SOE_TYPE_BREAKER_EXEC_FAIL,
    GW_SOE_TYPE_BREAKER_EXEC_CONFIRM,
    GW_SOE_TYPE_BREAKER_CANCEL_FAIL,
    GW_SOE_TYPE_BREAKER_CANCEL_CONFIRM,
    GW_SOE_TYPE_BREAKER_FINISH,
    GW_SOE_TYPE_TAP_SELECT_FAIL,
    GW_SOE_TYPE_TAP_SELECT_CONFIRM,
    GW_SOE_TYPE_TAP_EXEC_FAIL,
    GW_SOE_TYPE_TAP_EXEC_CONFIRM,
    GW_SOE_TYPE_TAP_CANCEL_FAIL,
    GW_SOE_TYPE_TAP_CANCEL_CONFIRM,
    GW_SOE_TYPE_TAP_FINISH,
    GW_SOE_TYPE_SYN_SELECT_FAIL,
    GW_SOE_TYPE_SYN_SELECT_CONFIRM,
    GW_SOE_TYPE_SYN_EXEC_FAIL,
    GW_SOE_TYPE_SYN_EXEC_CONFIRM,
    GW_SOE_TYPE_SYN_CANCEL_FAIL,
    GW_SOE_TYPE_SYN_CANCEL_CONFIRM,
    GW_SOE_TYPE_SYN_FINISH,
    GW_SOE_TYPE_VLD_DATA,
    GW_SOE_TYPE_LOCK_DATA,
    GW_SOE_TYPE_MAX
}GW_SOE_TYPE;

#define   GW_SOE_MAX_DATA       38
typedef struct STRUCT_GW_SOE
{
    GW_UCHAR        dest_addr;
    GW_UCHAR        Serial_L;
    GW_UCHAR        Serial_H;
    GW_SOE_TYPE     type;
    GW_COT          cot;
    GW_BOOL         cont;
    GW_UCHAR        RII;
    GW_TIME         time;
    GW_UCHAR        Sector;
    GW_UCHAR        FuncType;
    GW_ULONG        CondValue;
    GW_ULONG        TableID;
    GW_UCHAR        CmdType;
    GW_UCHAR        num;
    GW_ULONG        q;      /* 数据品质因素 */
    GW_ULONG        t_sec;  /* t.sec */
    GW_ULONG        t_frac; /* t.frac */
    GW_ULONG        t_qflag;/* t.qflag */
    GW_POINT_VALUE  data[GW_SOE_MAX_DATA];
}GW_SOE;

typedef enum  ENUM_COMMAND_TYPE
{
    GW_COMMAND_TYPE_UNDEF,
    GW_COMMAND_TYPE_BREAKER_SELECT,
    GW_COMMAND_TYPE_BREAKER_EXEC,
    GW_COMMAND_TYPE_BREAKER_CANCEL,
    GW_COMMAND_TYPE_TAP_SELECT,
    GW_COMMAND_TYPE_TAP_EXEC,
    GW_COMMAND_TYPE_TAP_CANCEL,
    GW_COMMAND_TYPE_SYN_SELECT,
    GW_COMMAND_TYPE_SYN_EXEC,
    GW_COMMAND_TYPE_SYN_CANCEL,
    GW_COMMAND_TYPE_CI_NOTFREEZE_NOTRESET,/*无冻结或复位*/
    GW_COMMAND_TYPE_CI_FREEZE_NOTRESET,   /*冻结无复位*/
    GW_COMMAND_TYPE_CI_FREEZE_RESET,      /*冻结带复位*/
    GW_COMMAND_TYPE_CI_RESET,             /*复位*/
    GW_COMMAND_TYPE_CI_SET,               /*电度设置*/
    GW_COMMAND_TYPE_VLD_HEAD,
    GW_COMMAND_TYPE_VLD_DATA,
    GW_COMMAND_TYPE_LOCK_DATA,
    GW_COMMAND_TYPE_MAX
}   GW_COMMAND_TYPE;

#define   GW_COMMAND_MAX_DATA       24
typedef struct STRUCT_GW_COMMAND
{
    GW_UCHAR            src_addr;
    GW_UCHAR            Serial_L;
    GW_UCHAR            Serial_H;
    GW_COMMAND_TYPE     type;
    GW_UCHAR            RII;
    GW_UCHAR            num;
    GW_POINT_VALUE      data[GW_COMMAND_MAX_DATA];
}GW_COMMAND;

typedef enum ENUM_GW_STATUS
{
    GW_STATUS_SUCCESS,
    GW_STATUS_INVALIDATE_PID,
    GW_STATUS_INVALIDATE_PTYPE,
    GW_STATUS_READ_TIMEOUT,
    GW_STATUS_WRITE_TIMEOUT,
    GW_STATUS_NO_SOE,
    GW_STATUS_NO_COMMAND,
    GW_STATUS_DB_EXIST
}   GW_STATUS;

typedef GW_SHORT (* GWAPP_ACK_FUNCTION )
    ( GW_VOID * pChl, GW_VOID * p_dst, GW_VOID * p_src, GW_UCHAR dst_port );
#endif
