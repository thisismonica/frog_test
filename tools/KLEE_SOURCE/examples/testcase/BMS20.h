//****************ÏµÍ³Í·ÎÄ¼þ*****************
//*******File name: BMS20.H
//******* Code: Judy
//*******Date : 2011,05,26
//*******************************************
#include <hidef.h>      /* common defines and macros */
#include <string.h>
//*******************************************
//*******************************************
#define  uchar unsigned char
#define  uint  unsigned int
#define  ulong unsigned long int
//*******************************************
//*******************************************
enum parameter_list
{
PARA_SOC_VALUE,    //SOCÊµ¼ÊÖµ
PARA_DISCHARGE_RATIO,//·Åµç±¶ÂÊC
PARA_SINGLE_CAPACITY,  //µ¥Ìåµç³ØÈÝÁ¿AH
PARA_WHOLE_CAPACITY,//µç³Ø×é×ÜÈÝÁ¿
PARA_BMU_NUMBER,//  µç³ØÊýÁ¿

PARA_DIFFERENCE_SINGLE_V, // µ¥ÌåµçÑ¹¾ùºâÖµ
PARA_DIFFERENCE_SINGLE_T, // µç³ØÎÂ¶È¾ùºâÖµ
PARA_BALANCE_ON_VOLTAGE, //¾ùºâ¿ªÆôµçÑ¹Öµ
PARA_BALANCE_OFF_VOLTAGE,//¾ùºâ¹Ø±ÕµçÑ¹Öµ
PARA_O_SOC_VALUE,// soc¸ß

PARA_OO_SOC_VALUE,// soc¹ý¸ß
PARA_L_SOC_VALUE,//socµÍ
PARA_LL_SOC_VALUE,//soc¹ýµÍ
PARA_O_WHOLE_VOLTAGE, // ×ÜµçÑ¹Öµ¸ß
PARA_OO_WHOLE_VOLTAGE, // ×ÜµçÑ¹Öµ¼«¸ß

PARA_L_WHOLE_VOLTAGE, // ×ÜµçÑ¹ÖµµÍ
PARA_LL_WHOLE_VOLTAGE, // ×ÜµçÑ¹Öµ¼«µÍ
PARA_O_CELL_VOLTAGE, // µ¥ÌåµçÑ¹¸ß£¨¹ýÑ¹Öµ£©
PARA_OO_CELL_VOLTAGE, // µ¥ÌåµçÑ¹¼«¸ß
PARA_L_CELL_VOLTAGE,//  µ¥ÌåµçÑ¹µÍ(Ç·Ñ¹Öµ)

PARA_LL_CELL_VOLTAGE,//  µ¥ÌåµçÑ¹¼«µÍ
PARA_O_TEMPERATURE,//¹ýÎÂ
PARA_OO_TEMPERATURE,//ÎÂ¶È¼«¸ß
PARA_L_TEMPERATURE, // ÎÂ¶ÈµÍ
PARA_LL_TEMPERATURE, // ÎÂ¶È¼«µÍ

PARA_O_CONSISTENCY_V, //µçÑ¹Ò»ÖÂÐÔ²î
PARA_OO_CONSISTENCY_V,//µçÑ¹Ò»ÖÂÐÔ¼«²î
PARA_O_CONSISTENCY_T, //ÎÂ¶ÈÒ»ÖÂÐÔ²î
PARA_OO_CONSISTENCY_T,//ÎÂ¶ÈÒ»ÖÂÐÔ¼«²î
PARA_CURRENT,//   ¹ýµçÁ÷Öµ

PARA_HIGHEST_RECHARGE_VOLTAGE,//ÔÊÐí×î¸ß³äµç¶ËµçÑ¹
PARA_HIGHEST_RECHARGE_CURRENT,//×î´óÔÊÐí³äµçµçÁ÷ 
PARA_HIGHEST_RECHARGE_CURRENT_TIME,//ÔÊÐí×î´ó³äµçµçÁ÷Ê±¼ä
PARA_HIGHEST_DISCHARGE_CURRENT,//×î´óÔÊÐí·ÅµçµçÁ÷ 
PARA_HIGHEST_CHARGE_CURRENT_TIME,//ÔÊÐí·ÅµçµçÁ÷×î´óµçÁ÷Ê±¼ä

PARA_L_ISOLATION_RESISTANCE, //  ¾øÔµµç×èµÍÖµ
PARA_LL_ISOLATION_RESISTANCE, //  ¾øÔµµç×è¹ýµÍÖµ

PARA_ENDFLAG
};
enum storage_list 
{
INDEX,
TOTAL_VOLTAGE,
TOTAL_CURRENT,
PARA_SOC_DISPLAY, // SOCÖµ
SYS_CONTACTORS_STATE,  //×ÜÕý×Ü¸º¼ÌµçÆ÷×´Ì¬

PARA_ERROR_LEVER,//¹ÊÕÏµÈ¼¶Öµ  1:1¼¶¹ÊÕÏ  2:2¼¶¹ÊÕÏ
CAUTION_FLAG_1,
CAUTION_FLAG_2,
CAUTION_FLAG_3,
CAUTION_FLAG_4,

SYS_REALTIME_SECOND,
SYS_REALTIME_MINUTE,
SYS_REALTIME_HOUR,
SYS_REALTIME_WEEK,
SYS_REALTIME_DAY,

SYS_REALTIME_MONTH,
SYS_REALTIME_YEAR,
CELL_MAX_TEM,
CELL_MIN_TEM,
CELL_AVERAGE_TEM,

CELL_MAX_VOLTAGE,
CELL_MIN_VOLTAGE,
CELL_AVERAGE_VOLTAGE,
VERIFICATION  //Ð£Ñé
};

enum storage_cell_vol_list 
{  
CELL_VOLTAGE_0,
CELL_VOLTAGE_1,
CELL_VOLTAGE_2,
CELL_VOLTAGE_3,
CELL_VOLTAGE_4,
CELL_VOLTAGE_5,
CELL_VOLTAGE_6,
CELL_VOLTAGE_7,
CELL_VOLTAGE_8,
CELL_VOLTAGE_9,

CELL_VOLTAGE_10,
CELL_VOLTAGE_11,
CELL_VOLTAGE_12,
CELL_VOLTAGE_13,
CELL_VOLTAGE_14,
CELL_VOLTAGE_15,
CELL_VOLTAGE_16,
CELL_VOLTAGE_17,
CELL_VOLTAGE_18,
CELL_VOLTAGE_19,

CELL_VOLTAGE_20,
CELL_VOLTAGE_21,
CELL_VOLTAGE_22,
CELL_VOLTAGE_23,
CELL_VOLTAGE_24,
CELL_VOLTAGE_25,
CELL_VOLTAGE_26,
CELL_VOLTAGE_27,
CELL_VOLTAGE_28,
CELL_VOLTAGE_29,

CELL_VOLTAGE_30,
CELL_VOLTAGE_31,
CELL_VOLTAGE_32,
CELL_VOLTAGE_33,
CELL_VOLTAGE_34,
CELL_VOLTAGE_35,
CELL_VOLTAGE_36,
CELL_VOLTAGE_37,
CELL_VOLTAGE_38,
CELL_VOLTAGE_39,

CELL_VOLTAGE_40,
CELL_VOLTAGE_41,
CELL_VOLTAGE_42,
CELL_VOLTAGE_43,
CELL_VOLTAGE_44,
CELL_VOLTAGE_45,
CELL_VOLTAGE_46,
CELL_VOLTAGE_47,
CELL_VOLTAGE_48,
CELL_VOLTAGE_49,

CELL_VOLTAGE_50,
CELL_VOLTAGE_51,
CELL_VOLTAGE_52,
CELL_VOLTAGE_53,
CELL_VOLTAGE_54,
CELL_VOLTAGE_55,
CELL_VOLTAGE_56,
CELL_VOLTAGE_57,
CELL_VOLTAGE_58,
CELL_VOLTAGE_59,

CELL_VOLTAGE_60,
CELL_VOLTAGE_61,
CELL_VOLTAGE_62,
CELL_VOLTAGE_63,
CELL_VOLTAGE_64,
CELL_VOLTAGE_65,
CELL_VOLTAGE_66,
CELL_VOLTAGE_67,
CELL_VOLTAGE_68,
CELL_VOLTAGE_69,

CELL_VOLTAGE_70,
CELL_VOLTAGE_71,
CELL_VOLTAGE_72,
CELL_VOLTAGE_73,
CELL_VOLTAGE_74,
CELL_VOLTAGE_75,
CELL_VOLTAGE_76,
CELL_VOLTAGE_77,
CELL_VOLTAGE_78,
CELL_VOLTAGE_79,

CELL_VOLTAGE_80,
CELL_VOLTAGE_81,
CELL_VOLTAGE_82,
CELL_VOLTAGE_83,
CELL_VOLTAGE_84,
CELL_VOLTAGE_85,
CELL_VOLTAGE_86,
CELL_VOLTAGE_87,
CELL_VOLTAGE_88,
CELL_VOLTAGE_89,

CELL_VOLTAGE_90,
CELL_VOLTAGE_91,
CELL_VOLTAGE_92,
CELL_VOLTAGE_93,
CELL_VOLTAGE_94,
CELL_VOLTAGE_95,
CELL_VOLTAGE_96,
CELL_VOLTAGE_97,
CELL_VOLTAGE_98,
CELL_VOLTAGE_99,

CELL_VOLTAGE_100,
CELL_VOLTAGE_101,
CELL_VOLTAGE_102,
CELL_VOLTAGE_103,
CELL_VOLTAGE_104,
CELL_VOLTAGE_105,
CELL_VOLTAGE_106,
CELL_VOLTAGE_107,
CELL_VOLTAGE_108,
CELL_VOLTAGE_109,

CELL_VOLTAGE_110,
CELL_VOLTAGE_111,
CELL_VOLTAGE_112,
CELL_VOLTAGE_113,
CELL_VOLTAGE_114,
CELL_VOLTAGE_115,
CELL_VOLTAGE_116,
CELL_VOLTAGE_117,
CELL_VOLTAGE_118,
CELL_VOLTAGE_119
};
//                                                   
#define DEVICE_SERIAL_NUMBER 0x11    //	²¿¼þÐòÁÐºÅ 1byte
#define HARDWARE_VERSION 0x01    //	Ó²¼þ°æ±¾ µÍ4Î»ÓÐÐ§   
#define SOFTWARE_VERSION 0x01   // 	Èí¼þ°æ±¾ µÍ4Î»ÓÐÐ§
#define COMMUNICATION_VERSION 0x01  // Í¨Ñ¶Ð­Òé°æ±¾ µÍ4Î»ÓÐÐ§
//
#define BMU_NUMBER 2   // BMU¸öÊý
#define SIX802_NUMBER 5    // Ã¿¸öBMU¶ÔÓ¦µÄ6802¸öÊý
#define CELL_NUMBER 12   //Ã¿¸ö6802¶ÔÓ¦µÄµ¥Ìå¸öÊý
//
#define ParamNUMBER 38
#define STORE_NUMBER 24
#define ENDFLAG 92
#define ERROR_COUNT_ADDR 0x1e8000 //¸ÃµØÖ·Ô½ÍùºóÔ½ºÃ
#define ParaBaseAddr 0x00000	 //address in onchip flash
//#define DATA_ERROR_ADDR ParaBaseAddr+0x3FF  //´íÎóÊý¾Ý´æ´¢µÄÆðÊ¼µØÖ· ,0x3ffÖ®Ç°µÄ²¿·Ö£¬ÓÃÓÚ´æ´¢ÏµÍ³ãÐÖµµÈÄ¬ÈÏ²ÎÊý£ºg_defaultSysParameters
#define DATA_ERROR_ADDR2  0x210
//
#define TURN_ON 0
#define TURN_OFF 1
//
#define DISCHARGING 1   //¸ºµçÁ÷
#define RECHARGING 2  //ÕýµçÁ÷

//*******************************************
//***************mc9s12isr.c************************
#define KC 0.024  //Ð¡µçÁ÷µ÷ÕûÏµÊý
#define KX 0.054  //´óµçÁ÷µ÷ÕûÏµÊý
#define ADCLC (0)  //adcÆ«ÒÆÁ¿
#define ADCLX (0)
extern float ac[12];
extern float ax[12];
extern struct can_msg *msg;
extern unsigned char Vcu2bms_212_byte0;
extern unsigned char Chg2bms_6D0_byte[8]; //Ëù½ÓÊÕµ½µÄ³äµç»úµ½BMSµÄ±¨ÎÄ6D0
extern unsigned char Int_Flag;  // 0x01:ADÖÐ¶Ï 0x02:can0ÖÐ¶Ï 
extern unsigned int isrTimer0count_100ms;
extern unsigned char chargerStatusFlag;//³äµç×¼±¸¾ÍÐ÷±êÖ¾ 
extern unsigned char g_BMUcommuFlag;// ÄÚ²¿CANÍ¨ÐÅ±êÖ¾¡£=1£¬±íÊ¾È«²¿BMUÊý¾ÝÒÑ¾­ÊÕµ½£¬Õý³£¡£=0£»ÊÕ¾Ý»¹Ã»ÊÕÈ«¡
extern unsigned char paraChangedFlag; // ²ÎÊý¸Ä±ä±êÖ¾
extern unsigned char  paraReadFlag;  //²ÎÊý¶ÁÈ¡±êÖ¾
extern unsigned char g_errorRecordRead_flag;//¹ÊÕÏÐÅÏ¢¶ÁÈ¡±êÖ¾
//*******************************************
//************ Process.c ********************
extern float g_defaultSysParameters[ParamNUMBER];
extern float g_sysPara[ParamNUMBER];
extern unsigned int g_storageSysVariable[STORE_NUMBER];
extern unsigned int g_storageSysVariableCell[120];
extern unsigned int g_storageSysVariableOut[STORE_NUMBER];

extern unsigned char g_group;  //BMU1 ×éºÅ£¬ ´Ó0~15
extern unsigned int g_singleCellVoltage[BMU_NUMBER][SIX802_NUMBER][CELL_NUMBER];  //BMU1ºÅ, 6802£¨×é£©ºÅ, µ¥Ìåµç³ØºÅ
extern unsigned char g_singleCellTemperature[BMU_NUMBER][SIX802_NUMBER][CELL_NUMBER];//
extern unsigned char g_cellToVehicleVoltage[15][8];
extern unsigned char g_cellTemperature[15][8];
extern float g_highestCellVoltage;
extern float g_lowestCellVoltage;
extern float g_averageVoltage; //Æ½¾ùµ¥ÌåµçÑ¹
extern unsigned char g_highestTemperature;
extern unsigned char g_lowestTemperature;
extern unsigned char g_averageTemperature;

extern unsigned int g_groupUVflag[BMU_NUMBER][SIX802_NUMBER];// BUM1ºÅ£¬6802ºÅ
extern unsigned int g_groupOVflag[BMU_NUMBER][SIX802_NUMBER];// BUM1ºÅ£¬6802ºÅ

extern unsigned char g_bmu2_number_v[BMU_NUMBER];
extern unsigned char g_cell_number_v[BMU_NUMBER][5];
extern unsigned char g_bmu2_number_t[BMU_NUMBER];
extern unsigned char g_cell_number_t[BMU_NUMBER][5];

extern float  g_socValue;
extern float  g_displaySocValue;//¸ÃSOCÖµÓÃÓÚÏÔÊ¾»ò·¢ËÍ¸øÕû³µ¡£ÊÇÐÞÕýºóµÄSOC
extern float  g_energyOfUsed;
extern float  g_leftEnergy;
extern float  g_usefulCapacity;//¿ÉÓÃµç³ØÈÝÁ¿
extern unsigned char g_isolationP_high;//Õý¸º¾øÔµÖµºÍµÈ¼¶
extern unsigned char g_isolationP_low;
extern unsigned char g_isolationN_high;
extern unsigned char g_isolationN_low;
extern unsigned char g_isolationLever;
extern unsigned char g_chargeStateFlag;//³ä·Åµç×´Ì¬±êÖ¾

extern float  g_systemCurrent; //ÏµÍ³µ±Ç°µçÁ÷Öµ
extern float	g_systemVoltage;  //ÏµÍ³×ÜµçÑ¹
extern unsigned int   g_chargerCurrent;//³äµçÐèÒªµÄµçÁ÷
extern unsigned int   g_chargerVoltage;//³äµçÐèÒªµÄµçÑ¹
extern float g_uppestRechargeCurrent;//×î´óÔÊÐí³äµçµçÁ÷
extern float g_uppestDischargeCurrent;//×î´óÔÊÐí·ÅµçµçÁ÷

extern unsigned char  g_caution_Flag_1;  //¹ÊÕÏ×´Ì¬
extern unsigned char  g_caution_Flag_2;
extern unsigned char  g_caution_Flag_3;
extern unsigned char  g_caution_Flag_4;
extern unsigned int g_errorCounter;

extern unsigned char ChgeStat;//³äµç×´Ì¬
extern unsigned char Htr2ChgeDly;  //¼ÓÈÈµ½³äµç×ª»»Ê±¼ä¼ä¸ô
extern unsigned char ChgeAmpReq; //³äµçµçÁ÷Öµ

extern unsigned int g_isrTimes;//RTI¶¨Ê±Æ÷ÖÐ¶Ï´ÎÊý
extern unsigned long g_BMU_CIRCLE_FLAG; //ÓÃÓÚÅäÖÃBMU¸öÊý
extern unsigned long g_circleFlag; //Ò»¸öÑ­»·Íê³ÉµÄ±êÖ¾
extern unsigned long g_configFlag;//ÊÕµ½ÅäÖÃÐÅÏ¢±êÖ¾£¬ÓÃÓÚÅÐ¶ÏBMU¸öÊý£¬6805×éÊýºÍµ¥Ìå¸öÊý 

extern unsigned long g_mboxID;
extern unsigned char g_mboxData[1][8];// CAN2½ÓÊÕµ½µÄ±¨ÎÄÊý¾Ý£¬Ã¿¸ö±¨ÎÄ8¸ö×Ö½Ú

extern unsigned char can2receiveIsReady;
extern unsigned char CurrentTime[7];

extern float ReadEndFlag(void);
extern void InitDefaultParam(void);
extern void LoadParamter(void);
extern void StoreParameter(void);
extern void StoreSocRealvalue(void);
extern void StoreSysVariable(void);
extern unsigned char ReadOutErrorRecord(unsigned int counter);
extern unsigned int ReadErrorIndex(void);
extern void RecordsProcessure(void);
extern void InitialBMS(void);
extern void InitialSoc(void);
extern void BigRechargeCurrentAdjust(void); //×î´ó³äµçµçÁ÷ÐÞÕý£¨¸ø³äµç»ú£©
extern void BigDischargeCurrentAdjust(void);
extern float PulseRechargeCurrentAdjust(void);   //ÔÙÉúÂö³å×î´ó³äµçµçÁ÷  £¨¸øVCU£©
extern float TemperatureAdjustForCap(float capacity,unsigned char tem);//·Å³öÈÝÁ¿ÐÞÕý 
extern void SocRechargeAdjust(void);//³äµçÐÞÕý×Ó³ÌÐò
extern void SocProcessure(void);
extern void BMU_Processure(void);
extern unsigned char bmuProcess2(void);
extern void ParameterSetting(void);
extern void Recharge_Processure(void);
extern void VehiclePowerOnLogic(void);
extern void VehiclePowerOffLogic(void);
extern void ADC_Processure(void);
extern unsigned char ErrorProcessToBms(void);
extern unsigned int g_ParaEndFlag;
//*******************************************
//************ RTC.c ************************* 
extern long absoluteTime;
extern void RTIInit(void);
extern void startTimeBase(void);

//*******************************************
//********** adc.c ****************************
extern void ADCInitial(void);
extern void turnOnADC(void);
extern void turnOffADC(void);
//*******************************************
//************** spi.c ********************
extern void SPI_Init(void);  
extern void SPI_Write_Byte(unsigned char val);
extern unsigned char SPI_Read_Byte(void);

//*******************************************
//************** xgate.c ********************
extern void SetupXGATE(void);

//*******************************************
//************** mscan.c ********************
extern unsigned char  RelayReq;
extern unsigned char  BmsCtlStat;
extern unsigned char  ReqCtlToCHG;
extern unsigned char  rechargeType;
extern unsigned char  caution_flag_CHG_1;
extern unsigned char  caution_flag_CHG_2;
extern unsigned char  caution_flag_CHG_3;
extern float heatingCurrentValue;
extern unsigned char  heatingSta;
extern unsigned int rechargeTime;

extern void MSCANInit(void);
extern void TurnOnCan2Rx(void);
extern void TurnOffCan2Rx(void);
extern Bool MSCAN0SendMsg(struct can_msg msg);
extern Bool MSCAN1SendMsg(struct can_msg msg);
extern Bool MSCAN2SendMsg(struct can_msg msg);

extern void bmsToVcuInfo450(void);
extern void bmsToVcuInfo451(void);
extern void bmsToVcuInfo452(void);

extern void bmsToCHGinfo650(void);
extern void bmsToCHGinfo651(void); 
extern void bmsToCHGinfo652(void); 

//extern void sendCellVoltageToCAR(unsigned char number);
//extern void sendCellTemperatureToCAR(unsigned char number);
extern void bmsToPCforEnergy120(void);  //·¢ËÍBMSµÄ¿ÉÓÃÈÝÁ¿µÈÊý¾Ý ¸øPC¼à¿Ø
extern void sendParameterToPC1(void); 
extern void sendParameterToPC2(void); 
extern void sendParameterToPC3(void); 
extern void sendParameterToPC4(void); 
//extern void bmsToPcInfo0x000c450(void);
//extern void bmsToPcuInfo0x000c451(void);
//extern void bmsToPcuInfo0x000c452(void);
extern void sendBMSIDtoBMU(void);
extern void sendRealtimeToBMU(void); //¸øBMU·¢ËÍBMSµ±Ç°ÊµÊ±Ê±¼ä
extern void RecordSystem(void);
extern void RecordFaultTemperture(void);
extern void RecordRealtime(void);
extern void RecordExtremCellVoltage(void);
extern void RecordCellVoltage(unsigned char number);
extern void RecordEndFlag();//±íÊ¾·¢ËÍÍê³É
//*******************************************
//************** mc9s12gpio.c ******************
extern void gpioInit(void);
extern void turnOnLED0(void);
extern void turnOffLED0(void);
extern void turnOnSW_Power(void);
extern void turnOffSW_Power(void);
extern unsigned char input1_state(void);
extern unsigned char input2_state(void);
extern unsigned char input3_state(void);
extern unsigned char inputP_state(void);
extern unsigned char inputN_state(void);
extern unsigned char input4_state(void);
extern unsigned char inputH_state(void);
extern unsigned char inputAll_state(void);

//*******************************************
//***************sd2405.c*******************
extern unsigned char CurrentTime[7];//
extern void I2CWriteDate(unsigned char *pSecDa);
extern void I2CReadDate(void);

//*******************************************
//**************74HC595.C*********************
extern unsigned char Relay_State;
extern void InitialHc595(void);
extern void Write_Hc595(unsigned char data);
extern void TurnOff_ALL(void);
extern void TurnOn_INA2K(void);
extern void TurnOff_INA2K(void);
extern void TurnOn_INA1K(void);
extern void TurnOff_INA1K(void);
extern void TurnOn_INHK(void);
extern void TurnOff_INHK(void);
extern void TurnOn_INBK(void);
extern void TurnOff_INBK(void);
extern void TurnOn_INFK(void);
extern void TurnOff_INFK(void);
extern void TurnOn_INZK(void);
extern void TurnOff_INZK(void);
//***********************************************
//******************AT45DB161D********************
extern void InitialAt45db161d(void);
extern void Wrt_Flash(ulong Offset,uchar *str,unsigned char count);
extern void Rd_Flash(ulong Offset,uchar *str,unsigned char count);
extern void F_test(void);
//*******************************************
//*****************dflash.c*********************
extern unsigned int databuffer0;
extern void DFlash_Init(void);
extern void DFlash_Write_Word(unsigned int ADDR16);
extern unsigned int FSL_ReadDFlash (unsigned int destination);
extern void DFlash_Erase_Sector(unsigned int ADDR16);
//*******************************************
//*******************************************



