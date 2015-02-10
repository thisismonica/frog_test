#include "derivative.h" /* include peripheral declarations */
//#include "BMS20.h" 

#define  uchar unsigned char
#define  uint  unsigned int
#define  ulong unsigned long int
//����sda��λ����
#define sda_in	    DDRJ &= 0xbf   	   //������������
#define sda_out	    DDRJ |= 0x40       //�����������
#define sda_setb    PTJ |= 0x40        //��������      
#define sda_clr     PTJ &= 0xbf        //��������      
#define sda_r       (PTJ & 0x40)       //���ݶ�ȡ  
//ʱ��scl��λ����
#define scl_in	    DDRJ &= 0x7f       //ʱ����������
#define scl_out	    DDRJ |= 0x80       //ʱ���������
#define scl_setb    PTJ |= 0x80        //ʱ������      
#define scl_clr     PTJ &= 0x7f        //ʱ������      
#define scl_r       (PTJ & 0x80)       //ʱ�Ӷ�ȡ 
//===============
#define true  1
#define false 0
//===============
unsigned char CurrentTime[7];// the real time: [0]=second; [1]= minute;[2]=hour; [3]=week; .....

void WriteTimeOn(void);
void WriteTimeOff(void);
//====================================================================
/*��ʱ�ӳ���*/
void delay_ms(uint time)    	       //11.0592M,time=1��ʾ��ʱ1ms
{
 uint i;
 for(;time>0;time--)
  for(i=0;i<1571;i++);
  asm("nop");
  asm("nop");
  asm("nop");
}
void delay_1us(void)        	       //11.0592M, 1us��ʱ����
  {
   asm("nop");
  }
void delay_us(uint time)     	       //11.0592M, Nus��ʱ����
  {
   uint i;
   for (i=0;i<time;i++)
   asm("nop");
  }  
//=====================================================================  
//���ܣ�I2C��������
//���룺��
//�����UCHAR, True or False
/********����SD2400��I2C����********/
uchar I2CStart(void)
{
    	sda_out;
	scl_out;
	delay_1us();
    	sda_setb;	
	scl_setb;	
	delay_us(5);
	sda_in;
	delay_us(5);
	if(!sda_r)return false;		//SDA��Ϊ�͵�ƽ������æ,�˳�
	sda_out;
	delay_1us();
	sda_clr;	
	delay_us(10);
	sda_in;    	
	delay_us(10);
	while(sda_r)return false;	//SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
	scl_clr;	
	delay_us(5);
	return true;

}

//���ܣ�I2C��ֹ����
//���룺��
//�������
//********�ر�SD2400��I2C����****//
void I2CStop(void)
{
    scl_out;
	sda_out;
	delay_1us();
	sda_clr;
	delay_1us();
	scl_clr;
	delay_us(5);
	scl_setb;
	delay_us(2);
	sda_setb;
}

//���ܣ�����ACK�ź�
//���룺��
//�������
//*********���� ACK****//
void I2CAck(void)
{	
    scl_out;
	sda_out;
	delay_1us();
	sda_clr;	
	delay_1us();
	scl_clr;	
	delay_us(5);
	scl_setb;	
	delay_us(5);
	scl_clr;	
}

//���ܣ����ͷ�ACK�ź�
//���룺��
//�������
/*********����NO ACK*********/
void I2CNoAck(void)
{	
    scl_out;
	sda_out;
	delay_1us();
	sda_setb;	
	delay_1us();
	scl_clr;	
	delay_us(5);
	scl_setb;	
	delay_us(5);
	scl_clr;	
}

//���ܣ�����ACK�ź�
//���룺��
//���������Ϊ:1=��ACK,0=��ACK
/*********��ȡ ACK*********/
uchar I2CWaitAck(void) 			 
{
	uchar errtime=255;
	scl_out;
	sda_in;
	delay_1us();
	scl_clr;	
	delay_us(5);
	scl_setb;	
	delay_1us();
	sda_in;
	delay_1us();
	while(sda_r)	
	{
		errtime--;
		delay_1us();
		if(!errtime)
		{delay_us(10);
		scl_clr;
		return false;
                 }
	}
	scl_clr;	
	return true;
}
//���ܣ�MCU����һ���ֽ�
//���룺demand һ����������
//�������
/************MCU��SD2400����һ���ֽ�*************/             
void I2CSendByte(unsigned char demand)		 //���ݴӸ�λ����λ
{
	uchar bd=8;                                                        
	sda_out;
	scl_out;
		while(bd--)
		{
			scl_clr;  
			delay_1us();
			if(demand&0x80)
			{
			 sda_setb;
			}
			 else
			 {
			  sda_clr;
			 }
			demand<<=1;
			delay_us(3);
			scl_setb;
			delay_us(3);
		}
		scl_clr;	 
	
     
}
 //���ܣMMCU����һ�ֽ�
//���룺��
//�����ddata 
/*********MCU��SD2400����һ�ֽ�*********/
uchar I2CReceiveByte(void) 		//���ݴӸ�λ����λ//
{
	uchar bl=8;
	uchar ddata=0;
	scl_out;
	delay_1us();
	delay_1us();
	sda_in;			 	//c51��ΪSDA=1;�˿��л�Ϊ����״̬����
	delay_1us();
	while(bl--)
	{
		ddata<<=1;              //���ݴӸ�λ��ʼ��ȡ
		delay_1us();
		scl_clr;
		delay_us(5);	        //�Ӹ�λ��ʼ ddata|=SDA;ddata<<=1
		scl_setb;
		delay_us(5);	
		if(sda_r)
		{
			ddata|=0x01;
		}
		
		
	}
	scl_clr;
	return ddata;
}
 //���� ��ʵʱ���ݼĴ���
//���룺��
//�������
/******��SD2400ʵʱ���ݼĴ���******/
void I2CReadDate(void)
{
	unsigned char n;
	I2CStart();
	I2CSendByte(0x65); 
	I2CWaitAck();
	for(n=0;n<7;n++)
	{
		CurrentTime[n]=I2CReceiveByte()&0x7f;
		if (n!=6)        	 //���һ�����ݲ�Ӧ��
		{
			I2CAck();
		}
	}
	I2CNoAck();
	delay_1us();
	I2CStop();
}
  //���� д�������
//���룺��
//�������
/******дSD2400�������******/
void WriteTimeOn(void)
{		
	I2CStart();
	I2CSendByte(0x64);      
	I2CWaitAck();   
    	I2CSendByte(0x10);		//����д��ַ10H      
	I2CWaitAck();	
	I2CSendByte(0x80);		//��WRTC1=1      
	I2CWaitAck();
	I2CStop(); 
	
	I2CStart();
	I2CSendByte(0x64);      
	I2CWaitAck();   
    	I2CSendByte(0x0F);		//����д��ַ0FH      
	I2CWaitAck();	
	I2CSendByte(0x84);		//��WRTC2,WRTC3=1      
	I2CWaitAck();
	I2CStop(); 
	
}
//���� дʵʱ���ݼĴ���
//���룺*pSecDa Ҫд���ʱ�����ʼ��ַ
//�������
/******дSD2400ʵʱ���ݼĴ���******/
void I2CWriteDate(unsigned char *pSecDa)
{		

        WriteTimeOn();

	I2CStart();
	I2CSendByte(0x64);      
	I2CWaitAck();   
    	I2CSendByte(0x00);		//����д��ʼ��ַ      
	I2CWaitAck();	
	I2CSendByte(*pSecDa);		// second     
	I2CWaitAck();	
	I2CSendByte(*(pSecDa+1));		//minute      
	I2CWaitAck();	
	I2CSendByte(0x80|*(pSecDa+2));		//hour ,��ʮ��Сʱ��     
	I2CWaitAck();	
	I2CSendByte(*(pSecDa+3));		//week      
	I2CWaitAck();	
	I2CSendByte(*(pSecDa+4));		//day      
	I2CWaitAck();	
	I2CSendByte(*(pSecDa+5));		//month      
	I2CWaitAck();	
	I2CSendByte(*(pSecDa+6));		//year      
	I2CWaitAck();	
	I2CStop();

	I2CStart();
	I2CSendByte(0x64);      
	I2CWaitAck();   
    	I2CSendByte(0x12);		//����д��ʼ��ַ      
	I2CWaitAck();		
    	I2CSendByte(0x00);		//�������ֵ����Ĵ���      
	I2CWaitAck();        
	I2CStop();  
	
	WriteTimeOff();      
}
  //���� д��ֹ����
//���룺��
//�������
/******дSD2400��ֹ����******/
void WriteTimeOff(void)
{		
	I2CStart();
	I2CSendByte(0x64);
	I2CWaitAck();   
   	I2CSendByte(0x0F);		//����д��ַ0FH      
	I2CWaitAck();	
	I2CSendByte(0x0) ;		//��WRTC2,WRTC3=0      
	I2CWaitAck();
	I2CSendByte(0x0) ;		//��WRTC1=0(10H��ַ)      
	I2CWaitAck();
	I2CStop(); 
}

