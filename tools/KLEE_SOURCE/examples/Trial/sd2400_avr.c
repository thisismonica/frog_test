#include "derivative.h" /* include peripheral declarations */
//#include "BMS20.h" 

#define  uchar unsigned char
#define  uint  unsigned int
#define  ulong unsigned long int
//数据sda的位设置
#define sda_in	    DDRJ &= 0xbf   	   //数据设置输入
#define sda_out	    DDRJ |= 0x40       //数据设置输出
#define sda_setb    PTJ |= 0x40        //数据拉高      
#define sda_clr     PTJ &= 0xbf        //数据拉低      
#define sda_r       (PTJ & 0x40)       //数据读取  
//时钟scl的位设置
#define scl_in	    DDRJ &= 0x7f       //时钟设置输入
#define scl_out	    DDRJ |= 0x80       //时钟设置输出
#define scl_setb    PTJ |= 0x80        //时钟拉高      
#define scl_clr     PTJ &= 0x7f        //时钟拉低      
#define scl_r       (PTJ & 0x80)       //时钟读取 
//===============
#define true  1
#define false 0
//===============
unsigned char CurrentTime[7];// the real time: [0]=second; [1]= minute;[2]=hour; [3]=week; .....

void WriteTimeOn(void);
void WriteTimeOff(void);
//====================================================================
/*延时子程序*/
void delay_ms(uint time)    	       //11.0592M,time=1表示延时1ms
{
 uint i;
 for(;time>0;time--)
  for(i=0;i<1571;i++);
  asm("nop");
  asm("nop");
  asm("nop");
}
void delay_1us(void)        	       //11.0592M, 1us延时函数
  {
   asm("nop");
  }
void delay_us(uint time)     	       //11.0592M, Nus延时函数
  {
   uint i;
   for (i=0;i<time;i++)
   asm("nop");
  }  
//=====================================================================  
//功能：I2C开启条件
//输入：无
//输出：UCHAR, True or False
/********开启SD2400的I2C总线********/
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
	if(!sda_r)return false;		//SDA线为低电平则总线忙,退出
	sda_out;
	delay_1us();
	sda_clr;	
	delay_us(10);
	sda_in;    	
	delay_us(10);
	while(sda_r)return false;	//SDA线为高电平则总线出错,退出
	scl_clr;	
	delay_us(5);
	return true;

}

//功能：I2C终止条件
//输入：无
//输出：无
//********关闭SD2400的I2C总线****//
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

//功能：发送ACK信号
//输入：无
//输出：无
//*********发送 ACK****//
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

//功能：发送非ACK信号
//输入：无
//输出：无
/*********发送NO ACK*********/
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

//功能：读出ACK信号
//输入：无
//输出：返回为:1=有ACK,0=无ACK
/*********读取 ACK*********/
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
//功能：MCU发送一个字节
//输入：demand 一个命令数据
//输出：无
/************MCU向SD2400发送一个字节*************/             
void I2CSendByte(unsigned char demand)		 //数据从高位到低位
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
 //功能MCU读入一字节
//输入：无
//输出：ddata 
/*********MCU从SD2400读入一字节*********/
uchar I2CReceiveByte(void) 		//数据从高位到低位//
{
	uchar bl=8;
	uchar ddata=0;
	scl_out;
	delay_1us();
	delay_1us();
	sda_in;			 	//c51中为SDA=1;端口切换为输入状态！！
	delay_1us();
	while(bl--)
	{
		ddata<<=1;              //数据从高位开始读取
		delay_1us();
		scl_clr;
		delay_us(5);	        //从高位开始 ddata|=SDA;ddata<<=1
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
 //功能 读实时数据寄存器
//输入：无
//输出：无
/******读SD2400实时数据寄存器******/
void I2CReadDate(void)
{
	unsigned char n;
	I2CStart();
	I2CSendByte(0x65); 
	I2CWaitAck();
	for(n=0;n<7;n++)
	{
		CurrentTime[n]=I2CReceiveByte()&0x7f;
		if (n!=6)        	 //最后一个数据不应答
		{
			I2CAck();
		}
	}
	I2CNoAck();
	delay_1us();
	I2CStop();
}
  //功能 写允许程序
//输入：无
//输出：无
/******写SD2400允许程序******/
void WriteTimeOn(void)
{		
	I2CStart();
	I2CSendByte(0x64);      
	I2CWaitAck();   
    	I2CSendByte(0x10);		//设置写地址10H      
	I2CWaitAck();	
	I2CSendByte(0x80);		//置WRTC1=1      
	I2CWaitAck();
	I2CStop(); 
	
	I2CStart();
	I2CSendByte(0x64);      
	I2CWaitAck();   
    	I2CSendByte(0x0F);		//设置写地址0FH      
	I2CWaitAck();	
	I2CSendByte(0x84);		//置WRTC2,WRTC3=1      
	I2CWaitAck();
	I2CStop(); 
	
}
//功能 写实时数据寄存器
//输入：*pSecDa 要写入的时间的起始地址
//输出：无
/******写SD2400实时数据寄存器******/
void I2CWriteDate(unsigned char *pSecDa)
{		

        WriteTimeOn();

	I2CStart();
	I2CSendByte(0x64);      
	I2CWaitAck();   
    	I2CSendByte(0x00);		//设置写起始地址      
	I2CWaitAck();	
	I2CSendByte(*pSecDa);		// second     
	I2CWaitAck();	
	I2CSendByte(*(pSecDa+1));		//minute      
	I2CWaitAck();	
	I2CSendByte(0x80|*(pSecDa+2));		//hour ,二十四小时制     
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
    	I2CSendByte(0x12);		//设置写起始地址      
	I2CWaitAck();		
    	I2CSendByte(0x00);		//清零数字调整寄存器      
	I2CWaitAck();        
	I2CStop();  
	
	WriteTimeOff();      
}
  //功能 写禁止程序
//输入：无
//输出：无
/******写SD2400禁止程序******/
void WriteTimeOff(void)
{		
	I2CStart();
	I2CSendByte(0x64);
	I2CWaitAck();   
   	I2CSendByte(0x0F);		//设置写地址0FH      
	I2CWaitAck();	
	I2CSendByte(0x0) ;		//置WRTC2,WRTC3=0      
	I2CWaitAck();
	I2CSendByte(0x0) ;		//置WRTC1=0(10H地址)      
	I2CWaitAck();
	I2CStop(); 
}

