#include "C:\Users\Administrator\Documents\Source\MCU\bujin\dependence\build-in\STD\stc8.h"
#include <C:\Keil_v5\C51\INC\intrins.h>
#include "C:\Users\Administrator\Documents\Source\MCU\bujin\dependence\build-in\STD\USART.h"
//#include "C:\Users\Administrator\Documents\Source\MCU\bujin\dependence\build-in\STD\timer.h"
#include "C:\Users\Administrator\Documents\Source\MCU\bujin\dependence\build-in\STD\EEPROM.h"


sbit Y0=P2^5;
sbit Y1=P2^4;
sbit Y2=P2^3;
sbit Y3=P2^2;
sbit Y4=P4^2;
sbit Y5=P2^0;

sbit X0=P1^2;
sbit X1=P1^3;
sbit X2=P1^4;
sbit X3=P4^4;
sbit X4=P1^5;
sbit X5=P1^6;
sbit X6=P1^7;
sbit X7=P3^2;
sbit X8=P3^3;
sbit X9=P3^4;


sbit PUL=P0^6;
sbit DIR=P0^4;

enum UART_flag{MODE=1,BITS,ADDRESS,DAT,ENDBITS,RESULT}Flags;

typedef xdata struct
{
     u8 mode;     //模式
     u8 data_bits;     //数据位数
     u8 datas[64];   //数据缓存
     u8 data_now;   //当前数据位数
     u8 address;  //数据地址
     u8 flag;     //标志
}UART_Status;

void Runmotor(unsigned int);
void Tohome();
void Run();
void	UART_Init();
void UART_Read(u8 port);
void UART_Handle(u8);
//void Timer_Init(u16);
void Rom_Init();
char Isbuttonon();

//TIM_InitTypeDef xdata TIM_InitStructure;					//结构定义
u16 MotorHZ;   //马达运行频率 addres=0x0000
u16 MotorCycle;     //马达运行圈数 addres=0x0010

UART_Status Status;

unsigned int i,ii;

u8 Button1=0;
u8 Button2=0;

u16 dd;
u8 dh,dl;
u8 datas[2];
u8 temp[64];

#define WRITE 'w'  //写入模式
#define READ 'r'    //读取模式
#define ENDBIT 0x18 //结束位





void  delay_ms(unsigned int ms)
{
     unsigned int i;
	 do{
	      i = 24000000L / 13000;
		  while(--i)	;   //14T per loop
     }while(--ms);
}

void Delay100us()		//@24.000MHz
{
	unsigned char i, j;
	i = 4;
	j = 27;
	do
	{
		while (--j);
	} while (--i);
}
void Delay1us()		//@24.000MHz
{
	unsigned char i;

	i = 6;
	while (--i);
}




int main() 
{
     P2=0x00;
     P0M1=0x00;
     P0M0=0X00;
     P2M0=0xff;
     P2M1=0x00;
     delay_ms(5);
     UART_Init();
     delay_ms(5);
     Rom_Init();
     delay_ms(100);
     Tohome();
     while(1)
     {
          if(X3!=1&&X2!=0)
          {
               if(Isbuttonon())Run();
          }
          UART_Read(0);
          Rom_Init();
     }

     return 0;
}

void Run()
{
     Y0=1;
     while(1)
     {
          if(X2==0)
          {
               delay_ms(10);
               Runmotor(MotorHZ);
               Y0=0;
               Tohome();
               while (1)
               {
                    if(X3!=0&&X2!=0)break;
               }
               return;
          }
     }
}


void Runmotor(unsigned int num)
{
     i=0;
     ii=0;
     while(1)
     {
          if(i>num)break;
          PUL=0;
          for(ii=0;ii<MotorHZ;ii++)
          {
               Delay1us();
          }
          PUL=1;
          for(ii=0;ii<MotorHZ;ii++)
          {
               Delay1us();
          }
          i=i+1;
     }
}

void Tohome()
{
     Runmotor(100);
     while(1)
     {
          if(X3==1)return;
          PUL=0;
          Delay100us();
          Delay100us();
          Delay100us();
          Delay100us();
          Delay100us();
          PUL=1;
          Delay100us();
          Delay100us();
          Delay100us();
          Delay100us();
          Delay100us();
     }
}

void UART_Read(u8 port)
{
     Delay100us();
     // if(COM1.RX_TimeOut > 0)		//超时计数
	// 	{
	// 		if(--COM1.RX_TimeOut == 0)
	// 		{
	// 			if(COM1.RX_Cnt > 0)
	// 			{
	// 				for(i=0; i<COM1.RX_Cnt; i++)
     //                     {
     //                          UART_Handle(RX1_Buffer[i]);
     //                     }
	// 			}
	// 			COM1.RX_Cnt = 0;
	// 		}
	// 	}
	if(COM2.RX_TimeOut > 0)		//超时计数
		{
			if(--COM2.RX_TimeOut == 0)
			{
				if(COM2.RX_Cnt > 0)
				{
					for(i=0; i<COM2.RX_Cnt; i++)
                         {
                              UART_Handle(RX2_Buffer[i]);
                         }
				}
				COM2.RX_Cnt = 0;
			}
		}
}



void	UART_Init()
{
	COMx_InitDefine		COMx_InitStructure;					//结构定义
	// COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	// COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//使用波特率,   BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	// COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
	// COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	// COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
	// COMx_InitStructure.UART_Interrupt = ENABLE;				//中断允许,   ENABLE或DISABLE
	// COMx_InitStructure.UART_Polity    = PolityLow;			//中断优先级, PolityLow,PolityHigh
	// COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
	// COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
	// USART_Configuration(USART1, &COMx_InitStructure);		//初始化串口1 USART1,USART2

	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率,     110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	COMx_InitStructure.UART_Interrupt = DISABLE;				//中断允许,   ENABLE或DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//中断优先级, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART2_SW_P10_P11;	//切换端口,   UART2_SW_P10_P11,UART2_SW_P46_P47
	USART_Configuration(USART2, &COMx_InitStructure);		//初始化串口2 USART1,USART2
}

// void Timer_Init(u16 time)
// {
// 	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
// 	TIM_InitStructure.TIM_Polity    = PolityLow;			//指定中断优先级, PolityHigh,PolityLow
// 	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE
// 	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;			//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
// 	TIM_InitStructure.TIM_ClkOut    = ENABLE;				//是否输出高速脉冲, ENABLE或DISABLE
// 	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 2*time);		//初值,
// 	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
// 	Timer_Inilize(Timer4,&TIM_InitStructure);				//初始化Timer0	  Timer0,Timer1,Timer2
// }

void Rom_Init()
{
     EEPROM_read_n(0x00,temp,2);
     MotorHZ=temp[1];
     MotorHZ<<8;
     MotorHZ &= temp[0];
     EEPROM_read_n(0x10,temp,2);
     MotorCycle=temp[1];
     MotorCycle<<8;
     MotorCycle &= temp[0];
}


void UART_Handle(u8 dat)
{
     switch (dat)
     {
     case 'w':
          Status.mode='w';
          Status.flag=MODE;
          break;
     case 'r':
          Status.mode='r';
          Status.flag=MODE;
          break;
     default:
          switch (Status.flag)
          {
          case MODE:
               Status.data_bits=dat;
               Status.flag=BITS;
               break;
          case BITS:
               Status.address=dat;
               Status.flag=ADDRESS;
               break;
          case ADDRESS:
               Status.address=dat;
               Status.flag=ENDBITS;
               break;
          case ENDBITS:
               if(dat!=ENDBIT)
               {
                    Status.datas[Status.data_now]=dat;
                    Status.data_now++;
                    break;
               }
               if(dat==ENDBIT)
               {
                    Status.flag=RESULT;
                    if(Status.mode==READ)
                    {
                         EEPROM_read_n(Status.address,Status.datas,Status.data_bits);
                    }
                    if(Status.mode==WRITE)
                    {
                         EEPROM_write_n(Status.address,Status.datas,Status.data_bits);
                    }
                    break;
               }
          default:
               break;
          }
          break;
     }
}

char Isbuttonon()
{
     char cc,ccc;
     Button1=X0;
     Button2=X1;
     if(X0==0)
     {
          Delay100us();
          if(Button1!=X0)
               cc=1;
          else
               cc=0;
     }
     if(X1==0)
     {
          Delay100us();
          if(Button2!=X1)
               ccc=1;
          else
               ccc=0;
          
     }
     if(cc==1&&ccc==1)return 1;
     return 0;
}