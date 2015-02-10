/////////////////////////////////////////////////////////////////////////////////////////
//
// Sample for Freescale EVB9S08DZ60
//
//--------------------------------------------------------------------------------------
//
// This project has been written for CodeWarrior 5.0 for HC(S)08
//
//--------------------------------------------------------------------------------------
//
// Copyright (c) 2006 SofTec Microsystems
// http://www.softecmicro.com/
//
/////////////////////////////////////////////////////////////////////////////////////////

#include "derivative.h" /* include peripheral declarations */
#include "mscan.h"
#include "BMS20.h"

//unsigned char toCanID[4];
unsigned char  RelayReq =0;    //BMS Request Vehicle to switch Main contactors: 0=BMS self checking; 1=BMS request vehicles to disconect 
unsigned char  BmsCtlStat =0;   //BMS control status: bit0= negative Contactor status(0:open;1:closed);  bit1=Positive contactor status; bit2:cooling relay status.
unsigned char  ReqCtlToCHG =0;   //BMS Request Charger to control Main contactors:
unsigned char  rechargeType = 1; //charging type
unsigned char  caution_flag_CHG_1=0; //fault flag 1
unsigned char  caution_flag_CHG_2=0; //fault flag 2
unsigned char  caution_flag_CHG_3=0; //fault flag 3
float heatingCurrentValue = 8;        //the current value when heating.
unsigned char  heatingSta = 0;       //heating status. 0=stop 1=run
unsigned int rechargeTime = 0;       //charging time, unit is second.
 //功能：MSCAN 初始化
//输入：无
//输出：无
//描述：CAN0：车载  CAN1：充电  CAN2：内部
//********对MSCAN 进行初始化设置****//
void MSCANInit(void)
{
    
//  char sj,p;
//  char t_seg1, t_seg2;

  if (!CAN0CTL0&0x01)                  
    CAN0CTL0 =0x01;// Active MSCAN initialization mode    
  while (!CAN0CTL1&0x01);// Wait until the MSCAN is in initialization mode  
  
  if (!CAN1CTL0&0x01)                  
    CAN1CTL0 =0x01;// Active MSCAN initialization mode    
  while (!CAN1CTL1&0x01);// Wait until the MSCAN is in initialization mode  
  
  if (!CAN2CTL0&0x01)                  
    CAN2CTL0 =0x01;// Active MSCAN initialization mode    
  while (!CAN2CTL1&0x01);// Wait until the MSCAN is in initialization mode  
        
  /*
  sj = (SJW-1)<<6;
  p = (BRP-1);
  
  // Configures SJW and Tq clock Baud Rate Prescaler
  CAN0BTR0 = (sj|p);
  
  t_seg1 = (TSEG1-1);
  t_seg2 = (TSEG2-1)<<4;
  
  // Configure Time Segment 1 and 2, and one Sample per bit
  CAN0BTR1 = (t_seg1 | t_seg2);
  */
  CAN0BTR0 = 0x03;           // 500k波特率, 16M bus clock
  CAN0BTR1 = 0x23; 
  
  //CAN0BTR0 = 0x03;           // 250k波特率, 16M bus clock
  //CAN0BTR1 = 0x58;   
  
  //CAN0BTR0 = 0x07;           // 500k波特率, 16M bus clock
  //CAN0BTR1 = 0x7a; 
  
  CAN1BTR0 = 0x03;           // 250k波特率, 16M bus clock
  CAN1BTR1 = 0x58;  
//  CAN1BTR1 = 0x23; 
  
  CAN2BTR0 = 0x03;           // 250k波特率, 16M bus clock
  CAN2BTR1 = 0x58;  
  //CAN2BTR1 = 0x23;  
     
  CAN0IDAC = 0x20; //ID 8位接收模式  // Disables all the Filters  
  CAN1IDAC = 0x20; //ID 8位接收模式  
  CAN2IDAC = 0x20; //ID 8位接收模式  
                               
  CAN0IDMR0 = 0xFF;
  CAN0IDMR1 = 0xFF;
  CAN0IDMR2 = 0xFF;
  CAN0IDMR3 = 0xFF;
  CAN0IDMR4 = 0xFF;
  CAN0IDMR5 = 0xFF;
  CAN0IDMR6 = 0xFF;
  CAN0IDMR7 = 0xFF;
  
  CAN1IDMR0 = 0xFF;
  CAN1IDMR1 = 0xFF;
  CAN1IDMR2 = 0xFF;
  CAN1IDMR3 = 0xFF;
  CAN1IDMR4 = 0xFF;
  CAN1IDMR5 = 0xFF;
  CAN1IDMR6 = 0xFF;
  CAN1IDMR7 = 0xFF;
  
  CAN2IDMR0 = 0xFF;
  CAN2IDMR1 = 0xFF;
  CAN2IDMR2 = 0xFF;
  CAN2IDMR3 = 0xFF;
  CAN2IDMR4 = 0xFF;
  CAN2IDMR5 = 0xFF;
  CAN2IDMR6 = 0xFF;
  CAN2IDMR7 = 0xFF;
  
  // Enable MSCAN and normal operation and select the bus clock as MSCAN clock source
  //CAN0CTL1 = 0xC0;
  // Enable MSCAN and normal operation and select the oscillator clock as MSCAN clock source
  CAN0CTL1 = 0x80;    //16M oscillator  as the bus clock
  CAN1CTL1 = 0x80;    //16M oscillator  as the bus clock
  CAN2CTL1 = 0x80;    //16M oscillator  as the bus clock
    
  CAN0CTL0 = 0x00;// Active MSCAN Normal Operation
  CAN1CTL0 = 0x00;
  CAN2CTL0 = 0x00;
    
  while(CAN0CTL1&0x01);// Wait until the MSCAN operates normally
  while(CAN1CTL1&0x01);
  while(CAN2CTL1&0x01);
    
  while(!(CAN0CTL0&0x10));// Wait until MSCAN is synchronized to the CAN bus
  while(!(CAN1CTL0&0x10));
  while(!(CAN2CTL0&0x10));
        
  CAN0RIER = 0x01;//Enable receiver interrupt所有CAN都中断接收
  CAN1RIER = 0x01;
  //CAN2RIER = 0x01;
    
}
void TurnOnCan2Rx(void)//
{
   CAN2RIER |= 0x01;
}
void TurnOffCan2Rx(void)//
{

   CAN2RIER &= 0xfe;
}
 //功能：MSCAN0 发送消息子程序
//输入：can_msg msg
//输出：UCHAR,Ture or False
//********MSCAN0 发送消息子程序****//
Bool MSCAN0SendMsg(struct can_msg msg)
{

  unsigned char n_tx_buf, i;
   
  if(msg.len > MAX_LEN)// Checks len validity
    return(FALSE);
 
  if(!(CAN0CTL0&0x10))// Checks synchronization to CAN bus
    return(FALSE);

  n_tx_buf = 0;
  //do
  //{    
    // Looks for a free buffer
    n_tx_buf =  CAN0TFLG;
    CAN0TBSEL=n_tx_buf;
    n_tx_buf=CAN0TBSEL;
  //}while(!n_tx_buf); 
    if(!n_tx_buf)
       return(FALSE);
    
  CAN0TIDR0 = (unsigned char)(msg.id>>3);// Write standard Identifier
  CAN0TIDR1 = (unsigned char)(msg.id<<5);
  
  //CAN0TIDR0 = (unsigned char)(msg.id>>21);//Write Extended Identifier
  //CAN0TIDR1 = ((unsigned char)(msg.id>>13)&0xe0) |  0x18 | ((unsigned char)(msg.id>>15)&0x07);
  //CAN0TIDR2 = (unsigned char)(msg.id>>7);
  //CAN0TIDR3 = (unsigned char)(msg.id<<1);
  
  //if(msg.RTR)
    // RTR=Recessive
  //  CAN0TIDR1 |= 0x10;
      
  for(i = 0; i < msg.len; i++)// Write Data Segment
    *((&CAN0TDSR0)+i) = msg.data[i];
      
  CAN0TDLR = msg.len; // Write Data Length
    
  //CAN0TTBPR = msg.prty;// Write Priority
    
  CAN0TFLG = n_tx_buf;// Clear TXx Flag (buffer ready to transmission)
  
  //do
  //{    
    // Looks for a free buffer
    n_tx_buf =  CAN0TFLG;      //这部分是否有用，或可以删掉，需要验证
    CAN0TBSEL=n_tx_buf;
    n_tx_buf=CAN0TBSEL;
  //}while(!n_tx_buf); 
  
  if(!n_tx_buf)
       return(FALSE);
  
  return(TRUE);
  
}
 //功能：MSCAN1 发送消息子程序
//输入：can_msg msg
//输出：UCHAR,Ture or False
//********MSCAN1 发送消息子程序****//
Bool MSCAN1SendMsg(struct can_msg msg)
{

  unsigned char n_tx_buf, i;
  
  if(msg.len > MAX_LEN)// Checks len validity
    return(FALSE);
  
  if(!(CAN1CTL0&0x10))// Checks synchronization to CAN bus
    return(FALSE);

  n_tx_buf = 0;
  //do
  //{    
    // Looks for a free buffer
    n_tx_buf =  CAN1TFLG;
    CAN1TBSEL=n_tx_buf;
    n_tx_buf=CAN1TBSEL;
  //}while(!n_tx_buf); 
    if(!n_tx_buf)
       return(FALSE);
   
  //CAN1TIDR0 = (unsigned char)(msg.id>>3);// Write standard Identifier
  //CAN1TIDR1 = (unsigned char)(msg.id<<5);
    
  CAN1TIDR0 = (unsigned char)(msg.id>>21);//Write Extended Identifier
  CAN1TIDR1 = ((unsigned char)(msg.id>>13)&0xe0) |  0x18 | ((unsigned char)(msg.id>>15)&0x07);
  CAN1TIDR2 = (unsigned char)(msg.id>>7);
  CAN1TIDR3 = (unsigned char)(msg.id<<1);
  
  //if(msg.RTR)
    // RTR=Recessive
  //  CAN1TIDR1 |= 0x10;
     
  for(i = 0; i < msg.len; i++)// Write Data Segment
    *((&CAN1TDSR0)+i) = msg.data[i];
     
  CAN1TDLR = msg.len;// Write Data Length 
   
  //CAN1TTBPR = msg.prty;// Write Priority
   
  CAN1TFLG = n_tx_buf;// Clear TXx Flag (buffer ready to transmission)
  
  return(TRUE);
  
}
 //功能：MSCAN1 发送消息子程序
//输入：can_msg msg
//输出：UCHAR,Ture or False
//********MSCAN1 发送消息子程序****//
Bool MSCAN2SendMsg(struct can_msg msg)
{

  unsigned char n_tx_buf, i;
  
  
  if(msg.len > MAX_LEN)// Checks len validity
    return(FALSE);
  
  if(!(CAN2CTL0&0x10))// Checks synchronization to CAN bus
    return(FALSE);

  n_tx_buf = 0;
  //do
  //{    
    // Looks for a free buffer
    n_tx_buf =  CAN2TFLG;
    CAN2TBSEL=n_tx_buf;
    n_tx_buf=CAN2TBSEL;
  //}while(!n_tx_buf); 
    if(!n_tx_buf)
       return(FALSE);
   
  //CAN2TIDR0 = (unsigned char)(msg.id>>3);// Write standard Identifier
  //CAN2TIDR1 = (unsigned char)(msg.id<<5);
  
  CAN2TIDR0 = (unsigned char)(msg.id>>21);//Write Extended Identifier
  CAN2TIDR1 = ((unsigned char)(msg.id>>13)&0xe0) |  0x18 | ((unsigned char)(msg.id>>15)&0x07);
  CAN2TIDR2 = (unsigned char)(msg.id>>7);
  CAN2TIDR3 = (unsigned char)(msg.id<<1);
  
  //if(msg.RTR)
    // RTR=Recessive
  //  CAN2TIDR1 |= 0x10;
      
  for(i = 0; i < msg.len; i++)
    *((&CAN2TDSR0)+i) = msg.data[i];// Write Data Segment
     
  CAN2TDLR = msg.len; // Write Data Length  
  //CAN2TTBPR = msg.prty;// Write Priority
    
  CAN2TFLG = n_tx_buf;// Clear TXx Flag (buffer ready to transmission)
  
  return(TRUE);
  
}
//*************************************************************************************
//*************************************************************************************
//*****************以下是BMS给整车发消息子程序*****************************************
//*************************************************************************************
//功能：BMS发送报文45x给整车
//输入：无
//输出：无
//描述x:0,1,2,3,4,5,6,7. 0:生命信号 1：SOC 2.3:最大允许充电电流 
// 4.5：最大允许放电电流 6:单体电池温度差 7：电池编码
//********BMS发送报文45x给整车****//
void bmsToVcuInfo45x(void)
{
    struct can_msg mg;
    unsigned int buff;
    float rc = 0;
    static unsigned char life=0;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    life++;
    mg.data[0] = life;   //生命信号

	  buff = (unsigned int)(g_socValue*100*2);
    mg.data[1] =(unsigned char)buff;// 电池荷电状态SOC 分辨率0.4,单位%
       
    rc = PulseRechargeCurrentAdjust();
    buff = (unsigned int)((rc+400)*50);	//最大允许充电电流
	  mg.data[2]= buff>>8;//高字节 
	  mg.data[3]= (unsigned char)buff;//低字节 
    
    buff = (unsigned int)((g_uppestDischargeCurrent+400)*50);	//最大允许放电电流
	  mg.data[4]= buff>>8;//高字节 
	  mg.data[5]= (unsigned char)buff;//低字节 
	   
    mg.data[6] = g_highestTemperature - g_lowestTemperature;//单体电池 温度差
    mg.data[7] = 0x23;//电池系统编码：0~3电芯厂编码 4~7 BMS编码
    
    mg.id= 0x450+g_stringID;
    MSCAN0SendMsg(mg);    
    
    mg.id= 0x000c0450+g_stringID;
    MSCAN1SendMsg(mg);
}
//功能：BMS发送报文46x给整车
//输入：无
//输出：无
//描述x:0,1,2,3,4,5,6,7. 0.1:电池组当前电流 2.3:旱绯刈榈鼻暗缪?
//4.5:电池单体最高电压 6.7:电池单体最低点电压
//********BMS发送报文46x给整车****//                         
void bmsToVcuInfo46x(void)
{
    struct can_msg mg;
    unsigned int buff;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    //if(g_systemCurrent<-400)  //防止负数溢出
    //  g_systemCurrent = 0;
    buff = (unsigned int)((g_systemCurrent+400)*50);	
	  mg.data[0]= buff>>8;//电池组当前电流高字节 
  	mg.data[1]= (unsigned char)buff;//电池组当前电流低字节 

	  buff = (unsigned int)(g_systemVoltage*100);
	  mg.data[2]= buff>>8;//电池组当前电压高字节 
	  mg.data[3]= (unsigned char)buff;//电池组当前电压低字节 
	  
	  buff = (unsigned int)(g_highestCellVoltage*10000);
	  mg.data[4]= buff>>8;//电池单体最高电压高字节 
	  mg.data[5]= (unsigned char)buff;//电池单体最高电压低字节
	  
	  buff = (unsigned int)(g_lowestCellVoltage*10000);
	  mg.data[6]= buff>>8;//电池单体最低电压高字节
	  mg.data[7]= (unsigned char)buff;//电池单体最低电压低字节

    mg.id= 0x460+g_stringID; 
	  MSCAN0SendMsg(mg);
	   
	  mg.id= 0x000c0460+g_stringID; 
	  MSCAN1SendMsg(mg);
}
//功能：BMS发送报文47x给整车
//输入：无
//输出：无
//描述x:0,1,2,3,4,5,6,7. 0:继电器控制 1:BMS控制状态 2.3:电池最高最低温度 4.5.6.7故障状态
//********BMS发送报文47x给整车****//
void bmsToVcuInfo47x(void)
{
    struct can_msg mg;
    unsigned long tem;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0]= RelayReq & 0x0f; //继电器控制请求  bit0, bit1, 负端；bit2,bit3,正端
    mg.data[1]= BmsCtlStat & 0x0f;//BMS控制状态
    mg.data[2]= (g_highestTemperature-40)+48;//电池最高温度 
	  mg.data[3]= (g_lowestTemperature-40)+48;//电池最低温度 
	  
    mg.data[4]= g_caution_Flag_1; //故障状态
    mg.data[5]= g_caution_Flag_2;
    mg.data[6]= g_caution_Flag_3;
    mg.data[7]= g_caution_Flag_4;
    
    mg.id= 0x470+g_stringID; 
	  MSCAN0SendMsg(mg);
	  
	  mg.id= 0x000c0470+g_stringID; 
	  MSCAN1SendMsg(mg);
}

//功能：BMS发送数据给PC
//输入：无
//输出：无
//********BMS发送可用容量等数据给PC****//
void bmsToPCforEnergy120(void)  //发送BMS的可用容量等数据 给PC监控
{
   struct can_msg mg;
   unsigned int buff=0;
	
	mg.RTR= FALSE;  
  mg.len = 8;
  mg.prty = 0;
  
  buff= (unsigned int)(g_displaySocValue*100*2.5);
  mg.data[0] = (unsigned char)buff;  // 实际soc
  mg.data[1] = (unsigned char)(g_leftEnergy/3600);//可用容量
  
  mg.data[2] = g_isolationP_high; // insulation value for Positive pole.
  mg.data[3] = g_isolationP_low;
  mg.data[4] = g_isolationN_high;
  mg.data[5] = g_isolationN_low; //insulation value for Negative pole
  mg.data[6] = g_isolationLever;
  mg.data[7] = 0x00;
  	  
	mg.id=0x0120;
  MSCAN0SendMsg(mg);

}
//**************************************************************
//**************************************************************
//*********************BMS TO 上位机参数************************
//**************************************************************
//功能：发送参数给PC
//输入：无
//输出：无
//********发送参数给PC1,PC2,PC3,PC4,PC5****//
void sendParameterToPC1(void) 
{
   	struct can_msg mg;
   	
   	mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
   	
   	mg.data[0]= (((unsigned int)g_defaultSysParameters[PARA_O_WHOLE_VOLTAGE]) & 0xff00) >> 8;   //高字节   the upper limitation of the whole system voltage
	  mg.data[1]= (unsigned int)g_defaultSysParameters[PARA_O_WHOLE_VOLTAGE] & 0x00ff ;   //低字节
	  mg.data[2]= ((unsigned int)g_defaultSysParameters[PARA_L_WHOLE_VOLTAGE] & 0xff00) >> 8;   //高字节;   the lower limitation  of the whole system voltage
	  mg.data[3]= (unsigned int)g_defaultSysParameters[PARA_L_WHOLE_VOLTAGE] & 0x00ff;   //低字节;
	  mg.data[4]= (g_defaultSysParameters[PARA_O_CELL_VOLTAGE]+1.75)*100;//过压值  The upper limitation of the cell voltage
	  mg.data[5]= (g_defaultSysParameters[PARA_L_CELL_VOLTAGE]+1.75)*100; //欠压值 The lower limitation of the cell voltage
	  mg.data[6]= g_defaultSysParameters[PARA_O_TEMPERATURE]+40; //温度过高    The upper limitation of the cell Temperature
	  mg.data[7]= g_defaultSysParameters[PARA_L_TEMPERATURE]+40; //温度过低   The lower limitation of the cell Temperature
	  
	  mg.id = 0xC01F028;
	  MSCAN0SendMsg(mg);

}
void sendParameterToPC2(void) 
{
   	struct can_msg mg;
   	
   	mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
   	mg.data[0]= g_defaultSysParameters[PARA_HIGHEST_RECHARGE_CURRENT];    
	  mg.data[1]= g_defaultSysParameters[PARA_HIGHEST_RECHARGE_CURRENT_TIME];   
	  mg.data[2]= g_defaultSysParameters[PARA_HIGHEST_DISCHARGE_CURRENT];// 
	  mg.data[3]= g_defaultSysParameters[PARA_HIGHEST_CHARGE_CURRENT_TIME]; //
	  mg.data[4]= g_defaultSysParameters[PARA_O_SOC_VALUE];//
	  mg.data[5]= g_defaultSysParameters[PARA_L_SOC_VALUE]; // 
	  mg.data[6]= g_defaultSysParameters[PARA_L_ISOLATION_RESISTANCE];
	  mg.data[7]= g_defaultSysParameters[PARA_O_CONSISTENCY_V]; 
	  
	  mg.id = 0xC01F029;
	  MSCAN0SendMsg(mg);

}

void sendParameterToPC3(void) 
{
    struct can_msg mg;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0] = g_defaultSysParameters[PARA_BALANCE_ON_VOLTAGE];
    mg.data[1] = g_defaultSysParameters[PARA_BALANCE_OFF_VOLTAGE];
    mg.data[2] = g_defaultSysParameters[PARA_DIFFERENCE_SINGLE_V];
    mg.data[3] = 0x55;
    mg.data[4] = 0x55;
    mg.data[5] = 0x55;
    mg.data[6] = 0x55;
    mg.data[7] = 0x55;
    
    mg.id = 0xC01F02a ;  
	  MSCAN0SendMsg(mg);
}
void sendParameterToPC4(void) 
{
    struct can_msg mg;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.id = 0xC01F02b ;
	  MSCAN0SendMsg(mg);
}
void sendParameterToPC5(void) 
{

}
//**************************************************************
//**************************************************************
//*********************BMS TO BMU********************************
//**************************************************************
 //功能：发送BMSID和当前时间给BMU
//输入：无
//输出：无
//********发送BMSID和当前时间给BMU****//
void sendBMSIDtoBMU(void)   //报文描述：BMS 标识信息；硬件版本、软件版本、通讯协议版本等；
{
  struct can_msg mg;
	unsigned char i;
	static unsigned char lifeSign =0;
	
	mg.RTR= FALSE;  
  mg.len = 8;
  mg.prty = 0;
  
  lifeSign++;

	mg.data[0]= DEVICE_SERIAL_NUMBER;
	mg.data[1]= HARDWARE_VERSION|(SOFTWARE_VERSION<<4);
	mg.data[2]= COMMUNICATION_VERSION;
	mg.data[3]= lifeSign;
	for(i=4;i<8;i++)
		mg.data[i]=0;

	mg.id = 0x98FF0100;
	MSCAN2SendMsg(mg);

}
void sendRealtimeToBMU(void) //给BMU发送BMS当前实时时间
{
  struct can_msg mg;
	 
	mg.RTR= FALSE;  
  mg.len = 8;
  mg.prty = 0;
  
  mg.data[0]= CurrentTime[6]; //年
  mg.data[1]= CurrentTime[5]; //月
  mg.data[2]= CurrentTime[4]; //日
  mg.data[3]= CurrentTime[2]; //时
  mg.data[4]= CurrentTime[1]; //分
  mg.data[5]= CurrentTime[0]; //秒 
  mg.data[6]= 0;
	mg.data[7]= 0;
	
	mg.id = 0x98FF0200;
	MSCAN2SendMsg(mg);

}
//**************************************************************
//**************************************************************
//*********************BMS TO 充电机****************************
//**************************************************************
 //功能：BMS发送报文650给充电机
//输入：无
//输出：无
//描述:报文包括充电机请求控制字节，充电类型，故障信息。
//********BMS发送报文650给充电机****//
/*
void bmsToCHGinfo650(void) 
{   
    struct can_msg mg;
    static unsigned char life=0;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    life++;
    
    mg.data[0]= ReqCtlToCHG;   //充电机请求控制字
    mg.data[1]= rechargeType & 0x03; //快充 或 慢充
    mg.data[2]= caution_flag_CHG_1;    //故障
    mg.data[3]= caution_flag_CHG_2;
    mg.data[4]= caution_flag_CHG_3;
    mg.data[5]= 0x00;
    mg.data[6]= 0x00;
    mg.data[7]= life;
            
    mg.id = 0x650;
	  MSCAN0SendMsg(mg);

}
*/
//**************************************************************
//**************************************************************
 //功能：BMS发送报文651给充电机
//输入：无
//输出：无
//描述：报文包括加热电流请求和加热状态
//********BMS发送报文651给充电机****//
/*
void bmsToCHGinfo651(void)   //  heat infomation
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char i;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    buff = (unsigned int)((heatingCurrentValue+500)*10);	//加热电流
	  mg.data[0]= buff>>8;//高字节 
  	mg.data[1]= (unsigned char)buff;//低字节 
  	
  	for(i=2;i<7;i++)
  	    mg.data[i]= 0x00;
  	    
  	mg.data[7]= heatingSta & 0x01; //加热状态   
    
    mg.id = 0x651;
	  MSCAN0SendMsg(mg);
}
*/
//**************************************************************
//**************************************************************
 //功能：BMS发送报文652给充电机
//输入：无
//输出：无
//描述：报文包括充电机充电状态，剩余充电时间，允许最高充电端电压。
//********BMS发送报文652给充电机****//
/*
void bmsToCHGinfo652(void)   // charger infomation
{
    struct can_msg mg;
    unsigned int buff = 0;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0]= ChgeStat & 0x01; //充电机充电状态：0，未充电；1，充电
    mg.data[1]= 0x00;
    
    buff = (unsigned int)(g_defaultSysParameters[PARA_HIGHEST_RECHARGE_CURRENT_TIME]-rechargeTime); //剩余充电时间高字节
    mg.data[2]= buff>>8;  //高字节
    mg.data[3]= buff;     //低字节

    buff = (unsigned int)(g_defaultSysParameters[PARA_HIGHEST_RECHARGE_VOLTAGE]*10); //允许最高充电端电压高字节
    mg.data[4]= buff>>8; //高
    mg.data[5]= buff;    //低
    
    g_uppestRechargeCurrent = ChgeAmpReq;
    buff = (unsigned int)((g_uppestRechargeCurrent+500)*10); //允许最大充电电流
    mg.data[6]= buff>>8; 
    mg.data[7]= buff;    
    
    
    mg.id = 0x652;
	  MSCAN0SendMsg(mg);
}
*/
//**************************************************************
//**************************************************************
//*********************BMS TO PC 错误故障***********************
//**************************************************************
//
 //功能：将BMS记录的历史数据发送至PC
//输入：无
//输出：无
//描述：电池组信息，温度错误信息，时间，单体电压极值，单体电压编号，结束标志。 
//********将BMS记录的历史数据发送至PC****//
void RecordSystem(void)
{
    struct can_msg mg;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0]= g_storageSysVariableOut[INDEX]>>8;
    mg.data[1]= g_storageSysVariableOut[INDEX];
    
	  mg.data[2]= g_storageSysVariableOut[TOTAL_VOLTAGE]>>8;//电池组当前电压高字节 
	  mg.data[3]= g_storageSysVariableOut[TOTAL_VOLTAGE];//电池组当前电压低字节 
	  	
	  mg.data[4]= g_storageSysVariableOut[TOTAL_CURRENT]>>8;//电池组当前电流高字节 
  	mg.data[5]= g_storageSysVariableOut[TOTAL_CURRENT];//电池组当前电流低字节 
  	
    mg.data[6] = g_storageSysVariableOut[PARA_SOC_DISPLAY];// 电池荷电状态SOC 分辨率0.4,单位%

    mg.data[7]= g_storageSysVariableOut[SYS_CONTACTORS_STATE];//BMS控制状态

    
    mg.id = 0x18ff6000;
	  MSCAN2SendMsg(mg);
}
//*********************************************************************
void RecordFaultTemperture(void)//
{
    struct can_msg mg;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0]= 0x00;
    mg.data[1]= g_storageSysVariableOut[CAUTION_FLAG_1];
    mg.data[2]= g_storageSysVariableOut[CAUTION_FLAG_2];
    mg.data[3]= g_storageSysVariableOut[CAUTION_FLAG_3];
    mg.data[4]= g_storageSysVariableOut[CAUTION_FLAG_4];
    mg.data[5]= g_storageSysVariableOut[CELL_MAX_TEM];//电池最高温度 
    mg.data[6]= g_storageSysVariableOut[CELL_MIN_TEM];//电池最低温度 
    mg.data[7]= g_storageSysVariableOut[CELL_AVERAGE_TEM];//电池平均温度
    
    mg.id = 0x18ff6001;
	  MSCAN2SendMsg(mg);

}
//*********************************************************************
void RecordRealtime(void)//
{
    struct can_msg mg;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;

    mg.data[0]= g_storageSysVariableOut[SYS_REALTIME_SECOND];
    mg.data[1]= g_storageSysVariableOut[SYS_REALTIME_MINUTE];
    mg.data[2]= g_storageSysVariableOut[SYS_REALTIME_HOUR];
    mg.data[3]= g_storageSysVariableOut[SYS_REALTIME_WEEK];
    mg.data[4]= g_storageSysVariableOut[SYS_REALTIME_DAY];
    mg.data[5]= g_storageSysVariableOut[SYS_REALTIME_MONTH];// 
    mg.data[6]= g_storageSysVariableOut[SYS_REALTIME_YEAR];//
    mg.data[7]= 0x00;//

    mg.id = 0x18ff6002;
	  MSCAN2SendMsg(mg);
}
//*********************************************************************
void RecordExtremCellVoltage(void)//
{

    struct can_msg mg;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0]= g_storageSysVariableOut[CELL_MAX_VOLTAGE]>>8;
    mg.data[1]= g_storageSysVariableOut[CELL_MAX_VOLTAGE];
    mg.data[2]= g_storageSysVariableOut[CELL_MIN_VOLTAGE]>>8;
    mg.data[3]= g_storageSysVariableOut[CELL_MIN_VOLTAGE];
    mg.data[4]= g_storageSysVariableOut[CELL_AVERAGE_VOLTAGE]>>8;
    mg.data[5]= g_storageSysVariableOut[CELL_AVERAGE_VOLTAGE];
    mg.data[6]= 0x00;
    mg.data[7]= 0x00;//
    
    mg.id = 0x18ff6003;
	  MSCAN2SendMsg(mg); 
}
//*********************************************************************

void RecordCellVoltage(unsigned char number)//
{

    struct can_msg mg;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0]= g_storageSysVariable[CELL_VOLTAGE_0+4*number]>>8;
    mg.data[1]= g_storageSysVariable[CELL_VOLTAGE_0+4*number];
    mg.data[2]= g_storageSysVariable[CELL_VOLTAGE_1+4*number]>>8;
    mg.data[3]= g_storageSysVariable[CELL_VOLTAGE_1+4*number];
    mg.data[4]= g_storageSysVariable[CELL_VOLTAGE_2+4*number]>>8;
    mg.data[5]= g_storageSysVariable[CELL_VOLTAGE_2+4*number];
    mg.data[6]= g_storageSysVariable[CELL_VOLTAGE_3+4*number]>>8;;
    mg.data[7]= g_storageSysVariable[CELL_VOLTAGE_3+4*number];//
    
    mg.id = 0x18ff6100+number;
	  MSCAN2SendMsg(mg); 

}
void RecordEndFlag(void)//表示发送完成
{
    struct can_msg mg;
    unsigned char i=0;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;

    for(i=0;i<8;i++)
        mg.data[i]= 0xEE;
    
    mg.id = 0x18ff611e;
	  MSCAN2SendMsg(mg); 
}
//*********************************************************************
//*********************************************************************
//************************  end file  **********************************
//*********************************************************************

int main() {
    struct can_msg msg;
    klee_make_symbolic(&can_msg msg,sizeof(can_msg msg),"can_msg msg");
    return Bool MSCAN0SendMsg(can_msg msg);
}

