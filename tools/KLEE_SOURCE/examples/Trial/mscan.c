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
 //���ܣ�MSCAN ��ʼ��
//���룺��
//�������
//������CAN0������  CAN1�����  CAN2���ڲ�
//********��MSCAN ���г�ʼ������****//
void MSCANInit(void)
{
    
//  char sj,p;
//  char t_seg1, t_seg2;

//  if (!CAN0CTL0&0x01)                  
//    CAN0CTL0 =0x01;// Active MSCAN initialization mode    
//  while (!CAN0CTL1&0x01);// Wait until the MSCAN is in initialization mode  
  
//  if (!CAN1CTL0&0x01)                  
//    CAN1CTL0 =0x01;// Active MSCAN initialization mode    
//  while (!CAN1CTL1&0x01);// Wait until the MSCAN is in initialization mode  
  
//  if (!CAN2CTL0&0x01)                  
//    CAN2CTL0 =0x01;// Active MSCAN initialization mode    
//  while (!CAN2CTL1&0x01);// Wait until the MSCAN is in initialization mode  
        
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
//  CAN0BTR0 = 0x03;           // 500k������, 16M bus clock
//  CAN0BTR1 = 0x23; 
  
  //CAN0BTR0 = 0x03;           // 250k������, 16M bus clock
  //CAN0BTR1 = 0x58;   
  
  //CAN0BTR0 = 0x07;           // 500k������, 16M bus clock
  //CAN0BTR1 = 0x7a; 
  
//  CAN1BTR0 = 0x03;           // 250k������, 16M bus clock
//  CAN1BTR1 = 0x58;  
//  CAN1BTR1 = 0x23; 
  
//  CAN2BTR0 = 0x03;           // 250k������, 16M bus clock
//  CAN2BTR1 = 0x58;  
  //CAN2BTR1 = 0x23;  
     
//  CAN0IDAC = 0x20; //ID 8λ����ģʽ  // Disables all the Filters  
//  CAN1IDAC = 0x20; //ID 8λ����ģʽ  
//  CAN2IDAC = 0x20; //ID 8λ����ģʽ  
                               
//  CAN0IDMR0 = 0xFF;
//  CAN0IDMR1 = 0xFF;
//  CAN0IDMR2 = 0xFF;
//  CAN0IDMR3 = 0xFF;
//  CAN0IDMR4 = 0xFF;
//  CAN0IDMR5 = 0xFF;
//  CAN0IDMR6 = 0xFF;
//  CAN0IDMR7 = 0xFF;
  
//  CAN1IDMR0 = 0xFF;
//  CAN1IDMR1 = 0xFF;
//  CAN1IDMR2 = 0xFF;
//  CAN1IDMR3 = 0xFF;
//  CAN1IDMR4 = 0xFF;
//  CAN1IDMR5 = 0xFF;
//  CAN1IDMR6 = 0xFF;
//  CAN1IDMR7 = 0xFF;
  
//  CAN2IDMR0 = 0xFF;
//  CAN2IDMR1 = 0xFF;
//  CAN2IDMR2 = 0xFF;
//  CAN2IDMR3 = 0xFF;
//  CAN2IDMR4 = 0xFF;
//  CAN2IDMR5 = 0xFF;
//  CAN2IDMR6 = 0xFF;
//  CAN2IDMR7 = 0xFF;
  
  // Enable MSCAN and normal operation and select the bus clock as MSCAN clock source
  //CAN0CTL1 = 0xC0;
  // Enable MSCAN and normal operation and select the oscillator clock as MSCAN clock source
//  CAN0CTL1 = 0x80;    //16M oscillator  as the bus clock
//  CAN1CTL1 = 0x80;    //16M oscillator  as the bus clock
//  CAN2CTL1 = 0x80;    //16M oscillator  as the bus clock
    
//  CAN0CTL0 = 0x00;// Active MSCAN Normal Operation
//  CAN1CTL0 = 0x00;
//  CAN2CTL0 = 0x00;
    
//  while(CAN0CTL1&0x01);// Wait until the MSCAN operates normally
//  while(CAN1CTL1&0x01);
//  while(CAN2CTL1&0x01);
    
//  while(!(CAN0CTL0&0x10));// Wait until MSCAN is synchronized to the CAN bus
//  while(!(CAN1CTL0&0x10));
//  while(!(CAN2CTL0&0x10));
        
//  CAN0RIER = 0x01;//Enable receiver interrupt����CAN���жϽ���
//  CAN1RIER = 0x01;
  //CAN2RIER = 0x01;
    
}
void TurnOnCan2Rx(void)//
{
//   CAN2RIER |= 0x01;
}
void TurnOffCan2Rx(void)//
{

//   CAN2RIER &= 0xfe;
}
 //���ܣ�MSCAN0 ������Ϣ�ӳ���
//���룺can_msg msg
//�����UCHAR,Ture or False
//********MSCAN0 ������Ϣ�ӳ���****//
//Bool MSCAN0SendMsg(struct can_msg msg)
//{

//  unsigned char n_tx_buf, i;
   
//  if(msg.len > MAX_LEN)// Checks len validity
//    return(FALSE);
 
//  if(!(CAN0CTL0&0x10))// Checks synchronization to CAN bus
//    return(FALSE);

//  n_tx_buf = 0;
  //do
  //{    
    // Looks for a free buffer
//    n_tx_buf =  CAN0TFLG;
//    CAN0TBSEL=n_tx_buf;
//    n_tx_buf=CAN0TBSEL;
  //}while(!n_tx_buf); 
//    if(!n_tx_buf)
//       return(FALSE);
    
//  CAN0TIDR0 = (unsigned char)(msg.id>>3);// Write standard Identifier
//  CAN0TIDR1 = (unsigned char)(msg.id<<5);
  
  //CAN0TIDR0 = (unsigned char)(msg.id>>21);//Write Extended Identifier
  //CAN0TIDR1 = ((unsigned char)(msg.id>>13)&0xe0) |  0x18 | ((unsigned char)(msg.id>>15)&0x07);
  //CAN0TIDR2 = (unsigned char)(msg.id>>7);
  //CAN0TIDR3 = (unsigned char)(msg.id<<1);
  
  //if(msg.RTR)
    // RTR=Recessive
  //  CAN0TIDR1 |= 0x10;
      
//  for(i = 0; i < msg.len; i++)// Write Data Segment
//    *((&CAN0TDSR0)+i) = msg.data[i];
      
//  CAN0TDLR = msg.len; // Write Data Length
    
  //CAN0TTBPR = msg.prty;// Write Priority
    
//  CAN0TFLG = n_tx_buf;// Clear TXx Flag (buffer ready to transmission)
  
  //do
  //{    
    // Looks for a free buffer
//    n_tx_buf =  CAN0TFLG;      //�ⲿ���Ƿ����ã������ɾ������Ҫ��֤
//    CAN0TBSEL=n_tx_buf;
//    n_tx_buf=CAN0TBSEL;
  //}while(!n_tx_buf); 
  
//  if(!n_tx_buf)
//       return(FALSE);
  
//  return(TRUE);
  
//}
 //���ܣ�MSCAN1 ������Ϣ�ӳ���
//���룺can_msg msg
//�����UCHAR,Ture or False
//********MSCAN1 ������Ϣ�ӳ���****//
//Bool MSCAN1SendMsg(struct can_msg msg)
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
 //���ܣ�MSCAN1 ������Ϣ�ӳ���
//���룺can_msg msg
//�����UCHAR,Ture or False
//********MSCAN1 ������Ϣ�ӳ���****//
//Bool MSCAN2SendMsg(struct can_msg msg)
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
//*****************������BMS����������Ϣ�ӳ���*****************************************
//*************************************************************************************
//���ܣ�BMS���ͱ���45x������
//���룺��
//�������
//�����xx:0,1,2,3,4,5,6,7. 0:�����ź� 1��SOC 2.3:������������� 
// 4.5����������ŵ���� 6:�������¶Ȳ� 7����ر���
//********BMS���ͱ���45x������****//
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
    mg.data[0] = life;   //�����ź�

	  buff = (unsigned int)(g_socValue*100*2);
    mg.data[1] =(unsigned char)buff;// ��غɵ�״̬SOC �ֱ���0.4,��λ%
       
    rc = PulseRechargeCurrentAdjust();
    buff = (unsigned int)((rc+400)*50);	//�������������
	  mg.data[2]= buff>>8;//���ֽ� 
	  mg.data[3]= (unsigned char)buff;//���ֽ� 
    
    buff = (unsigned int)((g_uppestDischargeCurrent+400)*50);	//��������ŵ����
	  mg.data[4]= buff>>8;//���ֽ� 
	  mg.data[5]= (unsigned char)buff;//���ֽ� 
	   
    mg.data[6] = g_highestTemperature - g_lowestTemperature;//������ �¶Ȳ�
    mg.data[7] = 0x23;//���ϵͳ���룺0~3��о������ 4~7 BMS����
    
    mg.id= 0x450+g_stringID;
    MSCAN0SendMsg(mg);    
    
    mg.id= 0x000c0450+g_stringID;
    MSCAN1SendMsg(mg);
}
//���ܣ�BMS���ͱ���46x������
//���룺��
//�������
//�����xx:0,1,2,3,4,5,6,7. 0.1:����鵱ǰ���� 2.3:�����鵱ǰ���?
//4.5:��ص�����ߵ�ѹ 6.7:��ص�����͵��ѹ
//********BMS���ͱ���46x������****//                         
void bmsToVcuInfo46x(void)
{
    struct can_msg mg;
    unsigned int buff;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    //if(g_systemCurrent<-400)  //��ֹ�������
    //  g_systemCurrent = 0;
    buff = (unsigned int)((g_systemCurrent+400)*50);	
	  mg.data[0]= buff>>8;//����鵱ǰ�������ֽ� 
  	mg.data[1]= (unsigned char)buff;//����鵱ǰ�������ֽ� 

	  buff = (unsigned int)(g_systemVoltage*100);
	  mg.data[2]= buff>>8;//����鵱ǰ��ѹ���ֽ� 
	  mg.data[3]= (unsigned char)buff;//����鵱ǰ��ѹ���ֽ� 
	  
	  buff = (unsigned int)(g_highestCellVoltage*10000);
	  mg.data[4]= buff>>8;//��ص�����ߵ�ѹ���ֽ� 
	  mg.data[5]= (unsigned char)buff;//��ص�����ߵ�ѹ���ֽ�
	  
	  buff = (unsigned int)(g_lowestCellVoltage*10000);
	  mg.data[6]= buff>>8;//��ص�����͵�ѹ���ֽ�
	  mg.data[7]= (unsigned char)buff;//��ص�����͵�ѹ���ֽ�

    mg.id= 0x460+g_stringID; 
	  MSCAN0SendMsg(mg);
	   
	  mg.id= 0x000c0460+g_stringID; 
	  MSCAN1SendMsg(mg);
}
//���ܣ�BMS���ͱ���47x������
//���룺��
//�������
//�����xx:0,1,2,3,4,5,6,7. 0:�̵������� 1:BMS����״̬ 2.3:����������¶� 4.5.6.7����״̬
//********BMS���ͱ���47x������****//
void bmsToVcuInfo47x(void)
{
    struct can_msg mg;
    unsigned long tem;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0]= RelayReq & 0x0f; //�̵�����������  bit0, bit1, ���ˣ�bit2,bit3,����
    mg.data[1]= BmsCtlStat & 0x0f;//BMS����״̬
    mg.data[2]= (g_highestTemperature-40)+48;//�������¶� 
	  mg.data[3]= (g_lowestTemperature-40)+48;//�������¶� 
	  
    mg.data[4]= g_caution_Flag_1; //����״̬
    mg.data[5]= g_caution_Flag_2;
    mg.data[6]= g_caution_Flag_3;
    mg.data[7]= g_caution_Flag_4;
    
    mg.id= 0x470+g_stringID; 
	  MSCAN0SendMsg(mg);
	  
	  mg.id= 0x000c0470+g_stringID; 
	  MSCAN1SendMsg(mg);
}

//���ܣ�BMS�������ݸ�PC
//���룺��
//�������
//********BMS���Ϳ������������ݸ�PC****//
void bmsToPCforEnergy120(void)  //����BMS�Ŀ������������� ��PC���
{
   struct can_msg mg;
   unsigned int buff=0;
	
	mg.RTR= FALSE;  
  mg.len = 8;
  mg.prty = 0;
  
  buff= (unsigned int)(g_displaySocValue*100*2.5);
  mg.data[0] = (unsigned char)buff;  // ʵ��soc
  mg.data[1] = (unsigned char)(g_leftEnergy/3600);//��������
  
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
//*********************BMS TO ��λ������************************
//**************************************************************
//���ܣ����Ͳ�����PC
//���룺��
//�������
//********���Ͳ�����PC1,PC2,PC3,PC4,PC5****//
void sendParameterToPC1(void) 
{
   	struct can_msg mg;
   	
   	mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
   	
   	mg.data[0]= (((unsigned int)g_defaultSysParameters[PARA_O_WHOLE_VOLTAGE]) & 0xff00) >> 8;   //���ֽ�   the upper limitation of the whole system voltage
	  mg.data[1]= (unsigned int)g_defaultSysParameters[PARA_O_WHOLE_VOLTAGE] & 0x00ff ;   //���ֽ�
	  mg.data[2]= ((unsigned int)g_defaultSysParameters[PARA_L_WHOLE_VOLTAGE] & 0xff00) >> 8;   //���ֽ�;   the lower limitation  of the whole system voltage
	  mg.data[3]= (unsigned int)g_defaultSysParameters[PARA_L_WHOLE_VOLTAGE] & 0x00ff;   //���ֽ�;
	  mg.data[4]= (g_defaultSysParameters[PARA_O_CELL_VOLTAGE]+1.75)*100;//��ѹֵ  The upper limitation of the cell voltage
	  mg.data[5]= (g_defaultSysParameters[PARA_L_CELL_VOLTAGE]+1.75)*100; //Ƿѹֵ The lower limitation of the cell voltage
	  mg.data[6]= g_defaultSysParameters[PARA_O_TEMPERATURE]+40; //�¶ȹ���    The upper limitation of the cell Temperature
	  mg.data[7]= g_defaultSysParameters[PARA_L_TEMPERATURE]+40; //�¶ȹ���   The lower limitation of the cell Temperature
	  
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
 //���ܣ�����BMSID�͵�ǰʱ���BMU
//���룺��
//�������
//********����BMSID�͵�ǰʱ���BMU****//
void sendBMSIDtoBMU(void)   //����������BMS ��ʶ��Ϣ��Ӳ���汾�������汾��ͨѶЭ��汾�ȣ�
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
void sendRealtimeToBMU(void) //��BMU����BMS��ǰʵʱʱ��
{
  struct can_msg mg;
	 
	mg.RTR= FALSE;  
  mg.len = 8;
  mg.prty = 0;
  
  mg.data[0]= CurrentTime[6]; //��
  mg.data[1]= CurrentTime[5]; //��
  mg.data[2]= CurrentTime[4]; //��
  mg.data[3]= CurrentTime[2]; //ʱ
  mg.data[4]= CurrentTime[1]; //��
  mg.data[5]= CurrentTime[0]; //�� 
  mg.data[6]= 0;
	mg.data[7]= 0;
	
	mg.id = 0x98FF0200;
	MSCAN2SendMsg(mg);

}
//**************************************************************
//**************************************************************
//*********************BMS TO ����****************************
//**************************************************************
 //���ܣ�BMS���ͱ���650������
//���룺��
//�������
//����:���İ���������������ֽڣ�������ͣ�������Ϣ��
//********BMS���ͱ���650������****//
/*
void bmsToCHGinfo650(void) 
{   
    struct can_msg mg;
    static unsigned char life=0;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    life++;
    
    mg.data[0]= ReqCtlToCHG;   //�������������
    mg.data[1]= rechargeType & 0x03; //��� �� ����
    mg.data[2]= caution_flag_CHG_1;    //����
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
 //���ܣ�BMS���ͱ���651������
//���룺��
//�������
//���������İ������ȵ�������ͼ���״̬
//********BMS���ͱ���651������****//
/*
void bmsToCHGinfo651(void)   //  heat infomation
{
    struct can_msg mg;
    unsigned int buff;
    unsigned char i;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    buff = (unsigned int)((heatingCurrentValue+500)*10);	//���ȵ���
	  mg.data[0]= buff>>8;//���ֽ� 
  	mg.data[1]= (unsigned char)buff;//���ֽ� 
  	
  	for(i=2;i<7;i++)
  	    mg.data[i]= 0x00;
  	    
  	mg.data[7]= heatingSta & 0x01; //����״̬   
    
    mg.id = 0x651;
	  MSCAN0SendMsg(mg);
}
*/
//**************************************************************
//**************************************************************
 //���ܣ�BMS���ͱ���652������
//���룺��
//�������
//���������İ����������״̬��ʣ����ʱ�䣬������߳��˵�ѹ��
//********BMS���ͱ���652������****//
/*
void bmsToCHGinfo652(void)   // charger infomation
{
    struct can_msg mg;
    unsigned int buff = 0;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0]= ChgeStat & 0x01; //�������״̬��0��δ��磻1�����
    mg.data[1]= 0x00;
    
    buff = (unsigned int)(g_defaultSysParameters[PARA_HIGHEST_RECHARGE_CURRENT_TIME]-rechargeTime); //ʣ����ʱ����ֽ�
    mg.data[2]= buff>>8;  //���ֽ�
    mg.data[3]= buff;     //���ֽ�

    buff = (unsigned int)(g_defaultSysParameters[PARA_HIGHEST_RECHARGE_VOLTAGE]*10); //������߳��˵�ѹ���ֽ�
    mg.data[4]= buff>>8; //��
    mg.data[5]= buff;    //��
    
    g_uppestRechargeCurrent = ChgeAmpReq;
    buff = (unsigned int)((g_uppestRechargeCurrent+500)*10); //������������
    mg.data[6]= buff>>8; 
    mg.data[7]= buff;    
    
    
    mg.id = 0x652;
	  MSCAN0SendMsg(mg);
}
*/
//**************************************************************
//**************************************************************
//*********************BMS TO PC �������***********************
//**************************************************************
//
 //���ܣ���BMS��¼����ʷ���ݷ�����PC
//���룺��
//�������
//�������������Ϣ���¶ȴ�����Ϣ��ʱ�䣬�����ѹ��ֵ�������ѹ��ţ�������־�� 
//********��BMS��¼����ʷ���ݷ�����PC****//
void RecordSystem(void)
{
    struct can_msg mg;
    
    mg.RTR= FALSE;  
    mg.len = 8;
    mg.prty = 0;
    
    mg.data[0]= g_storageSysVariableOut[INDEX]>>8;
    mg.data[1]= g_storageSysVariableOut[INDEX];
    
	  mg.data[2]= g_storageSysVariableOut[TOTAL_VOLTAGE]>>8;//����鵱ǰ��ѹ���ֽ� 
	  mg.data[3]= g_storageSysVariableOut[TOTAL_VOLTAGE];//����鵱ǰ��ѹ���ֽ� 
	  	
	  mg.data[4]= g_storageSysVariableOut[TOTAL_CURRENT]>>8;//����鵱ǰ�������ֽ� 
  	mg.data[5]= g_storageSysVariableOut[TOTAL_CURRENT];//����鵱ǰ�������ֽ� 
  	
    mg.data[6] = g_storageSysVariableOut[PARA_SOC_DISPLAY];// ��غɵ�״̬SOC �ֱ���0.4,��λ%

    mg.data[7]= g_storageSysVariableOut[SYS_CONTACTORS_STATE];//BMS����״̬

    
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
    mg.data[5]= g_storageSysVariableOut[CELL_MAX_TEM];//�������¶� 
    mg.data[6]= g_storageSysVariableOut[CELL_MIN_TEM];//�������¶� 
    mg.data[7]= g_storageSysVariableOut[CELL_AVERAGE_TEM];//���ƽ���¶�
    
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
void RecordEndFlag(void)//��ʾ�������
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
