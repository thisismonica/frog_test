//****************ϵͳͷ�ļ�*****************
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
PARA_SOC_VALUE,    //SOCʵ��ֵ
PARA_DISCHARGE_RATIO,//�ŵ籶��C
PARA_SINGLE_CAPACITY,  //����������AH
PARA_WHOLE_CAPACITY,//�����������
PARA_BMU_NUMBER,//  �������

PARA_DIFFERENCE_SINGLE_V, // �����ѹ����ֵ
PARA_DIFFERENCE_SINGLE_T, // ����¶Ⱦ���ֵ
PARA_BALANCE_ON_VOLTAGE, //���⿪����ѹֵ
PARA_BALANCE_OFF_VOLTAGE,//����رյ�ѹֵ
PARA_O_SOC_VALUE,// soc��

PARA_OO_SOC_VALUE,// soc����
PARA_L_SOC_VALUE,//soc��
PARA_LL_SOC_VALUE,//soc����
PARA_O_WHOLE_VOLTAGE, // �ܵ�ѹֵ��
PARA_OO_WHOLE_VOLTAGE, // �ܵ�ѹֵ����

PARA_L_WHOLE_VOLTAGE, // �ܵ�ѹֵ��
PARA_LL_WHOLE_VOLTAGE, // �ܵ�ѹֵ����
PARA_O_CELL_VOLTAGE, // �����ѹ�ߣ���ѹֵ��
PARA_OO_CELL_VOLTAGE, // �����ѹ����
PARA_L_CELL_VOLTAGE,//  �����ѹ��(Ƿѹֵ)

PARA_LL_CELL_VOLTAGE,//  �����ѹ����
PARA_O_TEMPERATURE,//����
PARA_OO_TEMPERATURE,//�¶ȼ���
PARA_L_TEMPERATURE, // �¶ȵ�
PARA_LL_TEMPERATURE, // �¶ȼ���

PARA_O_CONSISTENCY_V, //��ѹһ���Բ�
PARA_OO_CONSISTENCY_V,//��ѹһ���Լ���
PARA_O_CONSISTENCY_T, //�¶�һ���Բ�
PARA_OO_CONSISTENCY_T,//�¶�һ���Լ���
PARA_CURRENT,//   ������ֵ

PARA_HIGHEST_RECHARGE_VOLTAGE,//������߳��˵�ѹ
PARA_HIGHEST_RECHARGE_CURRENT,//������������ 
PARA_HIGHEST_RECHARGE_CURRENT_TIME,//������������ʱ��
PARA_HIGHEST_DISCHARGE_CURRENT,//�������ŵ���� 
PARA_HIGHEST_CHARGE_CURRENT_TIME,//����ŵ����������ʱ��

PARA_L_ISOLATION_RESISTANCE, //  ��Ե�����ֵ
PARA_LL_ISOLATION_RESISTANCE, //  ��Ե�������ֵ

PARA_ENDFLAG
};
enum storage_list 
{
INDEX,
TOTAL_VOLTAGE,
TOTAL_CURRENT,
PARA_SOC_DISPLAY, // SOCֵ
SYS_CONTACTORS_STATE,  //�����ܸ��̵���״̬

PARA_ERROR_LEVER,//���ϵȼ�ֵ  1:1������  2:2������
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
VERIFICATION  //У��
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
#define DEVICE_SERIAL_NUMBER 0x11    //	�������к� 1byte
#define HARDWARE_VERSION 0x01    //	Ӳ���汾 ��4λ��Ч   
#define SOFTWARE_VERSION 0x01   // 	����汾 ��4λ��Ч
#define COMMUNICATION_VERSION 0x01  // ͨѶЭ��汾 ��4λ��Ч
//
#define BMU_NUMBER 2   // BMU����
#define SIX802_NUMBER 5    // ÿ��BMU��Ӧ��6802����
#define CELL_NUMBER 12   //ÿ��6802��Ӧ�ĵ������
//
#define ParamNUMBER 38
#define STORE_NUMBER 24
#define ENDFLAG 92
#define ERROR_COUNT_ADDR 0x1e8000 //�õ�ַԽ����Խ��
#define ParaBaseAddr 0x00000	 //address in onchip flash
//#define DATA_ERROR_ADDR ParaBaseAddr+0x3FF  //�������ݴ洢����ʼ��ַ ,0x3ff֮ǰ�Ĳ��֣����ڴ洢ϵͳ��ֵ��Ĭ�ϲ�����g_defaultSysParameters
#define DATA_ERROR_ADDR2  0x210
//
#define TURN_ON 0
#define TURN_OFF 1
//
#define DISCHARGING 1   //������
#define RECHARGING 2  //������

//*******************************************
//***************mc9s12isr.c************************
#define KC 0.024  //С��������ϵ��
#define KX 0.054  //���������ϵ��
#define ADCLC (0)  //adcƫ����
#define ADCLX (0)
extern float ac[12];
extern float ax[12];
extern struct can_msg *msg;
extern unsigned char Vcu2bms_212_byte0;
extern unsigned char Chg2bms_6D0_byte[8]; //�����յ��ĳ�����BMS�ı���6D0
extern unsigned char Int_Flag;  // 0x01:AD�ж� 0x02:can0�ж� 
extern unsigned int isrTimer0count_100ms;
extern unsigned char chargerStatusFlag;//���׼��������־ 
extern unsigned char g_BMUcommuFlag;// �ڲ�CANͨ�ű�־��=1����ʾȫ��BMU�����Ѿ��յ���������=0���վݻ�û��ȫ�
extern unsigned char paraChangedFlag; // �����ı��־
extern unsigned char  paraReadFlag;  //������ȡ��־
extern unsigned char g_errorRecordRead_flag;//������Ϣ��ȡ��־
//*******************************************
//************ Process.c ********************
extern float g_defaultSysParameters[ParamNUMBER];
extern float g_sysPara[ParamNUMBER];
extern unsigned int g_storageSysVariable[STORE_NUMBER];
extern unsigned int g_storageSysVariableCell[120];
extern unsigned int g_storageSysVariableOut[STORE_NUMBER];

extern unsigned char g_group;  //BMU1 ��ţ� ��0~15
extern unsigned int g_singleCellVoltage[BMU_NUMBER][SIX802_NUMBER][CELL_NUMBER];  //BMU1��, 6802���飩��, �����غ�
extern unsigned char g_singleCellTemperature[BMU_NUMBER][SIX802_NUMBER][CELL_NUMBER];//
extern unsigned char g_cellToVehicleVoltage[15][8];
extern unsigned char g_cellTemperature[15][8];
extern float g_highestCellVoltage;
extern float g_lowestCellVoltage;
extern float g_averageVoltage; //ƽ�������ѹ
extern unsigned char g_highestTemperature;
extern unsigned char g_lowestTemperature;
extern unsigned char g_averageTemperature;

extern unsigned int g_groupUVflag[BMU_NUMBER][SIX802_NUMBER];// BUM1�ţ�6802��
extern unsigned int g_groupOVflag[BMU_NUMBER][SIX802_NUMBER];// BUM1�ţ�6802��

extern unsigned char g_bmu2_number_v[BMU_NUMBER];
extern unsigned char g_cell_number_v[BMU_NUMBER][5];
extern unsigned char g_bmu2_number_t[BMU_NUMBER];
extern unsigned char g_cell_number_t[BMU_NUMBER][5];

extern float  g_socValue;
extern float  g_displaySocValue;//��SOCֵ������ʾ���͸����������������SOC
extern float  g_energyOfUsed;
extern float  g_leftEnergy;
extern float  g_usefulCapacity;//���õ������
extern unsigned char g_isolationP_high;//������Եֵ�͵ȼ�
extern unsigned char g_isolationP_low;
extern unsigned char g_isolationN_high;
extern unsigned char g_isolationN_low;
extern unsigned char g_isolationLever;
extern unsigned char g_chargeStateFlag;//��ŵ�״̬��־

extern float  g_systemCurrent; //ϵͳ��ǰ����ֵ
extern float	g_systemVoltage;  //ϵͳ�ܵ�ѹ
extern unsigned int   g_chargerCurrent;//�����Ҫ�ĵ���
extern unsigned int   g_chargerVoltage;//�����Ҫ�ĵ�ѹ
extern float g_uppestRechargeCurrent;//������������
extern float g_uppestDischargeCurrent;//�������ŵ����

extern unsigned char  g_caution_Flag_1;  //����״̬
extern unsigned char  g_caution_Flag_2;
extern unsigned char  g_caution_Flag_3;
extern unsigned char  g_caution_Flag_4;
extern unsigned int g_errorCounter;

extern unsigned char ChgeStat;//���״̬
extern unsigned char Htr2ChgeDly;  //���ȵ����ת��ʱ����
extern unsigned char ChgeAmpReq; //������ֵ

extern unsigned int g_isrTimes;//RTI��ʱ���жϴ���
extern unsigned long g_BMU_CIRCLE_FLAG; //��������BMU����
extern unsigned long g_circleFlag; //һ��ѭ����ɵı�־
extern unsigned long g_configFlag;//�յ�������Ϣ��־�������ж�BMU������6805�����͵������ 

extern unsigned long g_mboxID;
extern unsigned char g_mboxData[1][8];// CAN2���յ��ı������ݣ�ÿ������8���ֽ�

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
extern void BigRechargeCurrentAdjust(void); //����������������������
extern void BigDischargeCurrentAdjust(void);
extern float PulseRechargeCurrentAdjust(void);   //����������������  ����VCU��
extern float TemperatureAdjustForCap(float capacity,unsigned char tem);//�ų��������� 
extern void SocRechargeAdjust(void);//��������ӳ���
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
extern void bmsToPCforEnergy120(void);  //����BMS�Ŀ������������� ��PC���
extern void sendParameterToPC1(void); 
extern void sendParameterToPC2(void); 
extern void sendParameterToPC3(void); 
extern void sendParameterToPC4(void); 
//extern void bmsToPcInfo0x000c450(void);
//extern void bmsToPcuInfo0x000c451(void);
//extern void bmsToPcuInfo0x000c452(void);
extern void sendBMSIDtoBMU(void);
extern void sendRealtimeToBMU(void); //��BMU����BMS��ǰʵʱʱ��
extern void RecordSystem(void);
extern void RecordFaultTemperture(void);
extern void RecordRealtime(void);
extern void RecordExtremCellVoltage(void);
extern void RecordCellVoltage(unsigned char number);
extern void RecordEndFlag();//��ʾ�������
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



