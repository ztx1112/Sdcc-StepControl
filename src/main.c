#include "C:\Users\Administrator\Documents\Source\MCU\bujin\dependence\build-in\STD\stc8.h"
#include <C:\Keil_v5\C51\INC\intrins.h>
#include "C:\Users\Administrator\Documents\Source\MCU\bujin\dependence\build-in\STD\USART.h"
#include "C:\Users\Administrator\Documents\Source\MCU\bujin\dependence\build-in\STD\EEPROM.h"
#include <C:\Keil_v5\C51\INC\stdlib.h>
#include <C:\Keil_v5\C51\INC\string.h>


#define BRT             (65536 - MAIN_Fosc / 115200 / 4)
bit busy;
char wptr;
char rptr;
char buffer[16];

unsigned char databit;

void UartIsr() interrupt 4
{
     if (TI)
     {
          TI = 0;
          busy = 0;
     }
     if (RI)
     {
          RI = 0;
          buffer[wptr++] = SBUF;
          wptr &= 0x0f;
     }
}

void UartInit()
{
     SCON = 0x50;
     TMOD = 0x00;
     TL1 = BRT;
     TH1 = BRT >> 8;
     TR1 = 1;
     AUXR = 0x40;
     wptr = 0x00;
     rptr = 0x00;
     busy = 0;
}

void UartSend(char dat)
{
     while (busy);
     busy = 1;
     SBUF = dat;
}

void UartSendStr(char *p)
{
     while (*p)
     {
          UartSend(*p++);
     }
}

#define WT_30M          0x80
#define WT_24M          0x81
#define WT_20M          0x82
#define WT_12M          0x83
#define WT_6M           0x84
#define WT_3M           0x85
#define WT_2M           0x86
#define WT_1M           0x87

void IapIdle()
{
     IAP_CONTR = 0;                              //锟截憋拷IAP锟斤拷锟斤拷
     IAP_CMD = 0;                                //锟斤拷锟斤拷锟斤拷锟侥达拷锟斤拷
     IAP_TRIG = 0;                               //锟斤拷锟斤拷锟斤拷锟斤拷拇锟斤拷锟�
     IAP_ADDRH = 0x80;                           //锟斤拷锟斤拷址锟斤拷锟矫碉拷锟斤拷IAP锟斤拷锟斤拷
     IAP_ADDRL = 0;
}

char IapRead(int addr)
{
     IAP_CONTR = WT_24M;                         //使锟斤拷IAP
     IAP_CMD = 1;                                //锟斤拷锟斤拷IAP锟斤拷锟斤拷锟斤拷
     IAP_ADDRL = addr;                           //锟斤拷锟斤拷IAP锟酵碉拷址
     IAP_ADDRH = addr >> 8;                      //锟斤拷锟斤拷IAP锟竭碉拷址
     IAP_TRIG = 0x5a;                            //写锟斤拷锟斤拷锟斤拷锟斤拷(0x5a)
     IAP_TRIG = 0xa5;                            //写锟斤拷锟斤拷锟斤拷锟斤拷(0xa5)
     _nop_();
     databit = IAP_DATA;                             //锟斤拷IAP锟斤拷锟斤拷
     IapIdle();                                  //锟截憋拷IAP锟斤拷锟斤拷

     return databit;
}

void IapProgram(int addr, unsigned char dat)
{
     IAP_CONTR = WT_24M;                         //使锟斤拷IAP
     IAP_CMD = 2;                                //锟斤拷锟斤拷IAP写锟斤拷锟斤拷
     IAP_ADDRL = addr;                           //锟斤拷锟斤拷IAP锟酵碉拷址
     IAP_ADDRH = addr >> 8;                      //锟斤拷锟斤拷IAP锟竭碉拷址
     IAP_DATA = dat;                             //写IAP锟斤拷锟斤拷
     IAP_TRIG = 0x5a;                            //写锟斤拷锟斤拷锟斤拷锟斤拷(0x5a)
     IAP_TRIG = 0xa5;                            //写锟斤拷锟斤拷锟斤拷锟斤拷(0xa5)
     _nop_();
     IapIdle();                                  //锟截憋拷IAP锟斤拷锟斤拷
}

void IapErase(int addr)
{
     IAP_CONTR = WT_24M;                         //使锟斤拷IAP
     IAP_CMD = 3;                                //锟斤拷锟斤拷IAP锟斤拷锟斤拷锟斤拷锟斤拷
     IAP_ADDRL = addr;                           //锟斤拷锟斤拷IAP锟酵碉拷址
     IAP_ADDRH = addr >> 8;                      //锟斤拷锟斤拷IAP锟竭碉拷址
     IAP_TRIG = 0x5a;                            //写锟斤拷锟斤拷锟斤拷锟斤拷(0x5a)
     IAP_TRIG = 0xa5;                            //写锟斤拷锟斤拷锟斤拷锟斤拷(0xa5)
     _nop_();                                    //
     IapIdle();                                  //锟截憋拷IAP锟斤拷锟斤拷
}




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

#define MB_CycleUp '3'
#define MB_CycleDown '4'
#define MB_HzDown '7'
#define MB_HzUp '6'

// char Button_00[]={'[','B','N',':','0',']'};
// char Button_01[]="[BN:1]";
// char Button_02[]="[BN:2]";
// char Button_03[]="[BN:3]";
// char Button_04[]={'[','B','N',':','4',']'};
// char Button_05[]="[BN:5]";
// char Button_06[]="[BN:6]";
// char Button_07[]="[BN:7]";
// char Button_08[]="[BN:8]";
// char Button_09[]="[BN:9]";
// char Button_12[]="[BN:12]";
// char Button_13[]="[BN:13]";

// char Page_00[]={'P','A','G','E','0','0'};
// char Page_01[]={'P','A','G','E','0','1'};
// char Page_02[]={'P','A','G','E','0','2'};
// char Page_10[]="PAGE10";

// char Button_begin='[';
// char Button_end=']';
// char Number_begin='{';
// char Number_end='}';


void Runmotor(unsigned int);
void Tohome();
void Run();
void	UART_Init();
void UART_Read(u8);
void UART_Handle(u8);
//void Timer_Init(u16);
void Rom_Init();
void ROM_Write();
char Isbuttonon();
void ButtonHandle(u8);
void CommandHandle();

//TIM_InitTypeDef xdata TIM_InitStructure;					//结构定义
unsigned char MotorHz;   //马达运行频率 addres=0x0000
unsigned char MotorCycle;     //马达运行圈数 addres=0x0000

unsigned int i,ii;

u8 Button1;
u8 Button2;

u8 flag;

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


void Run()
{
     Y0=1;
     while(1)
     {
          if(X2==0)
          {
               delay_ms(10);
               Runmotor(MotorCycle*400);
               Y0=0;
               Tohome();
               while (1)
               {
                    if(X3!=0&&X2!=0)break;
               }
               Delay100us();
               ROM_Write();
               return;
          }
     }
}


void Runmotor(u16 num)
{
     i=0;
     ii=0;
     while(1)
     {
          if(i>num)break;
          PUL=0;
          for(ii=0;ii<MotorHz;ii++)
          {
               Delay1us();
          }
          PUL=1;
          for(ii=0;ii<MotorHz;ii++)
          {
               Delay1us();
          }
          i=i+1;
     }
}

void Tohome()
{
     i=0;
     for(i=0;i<100;i++)
     {
          PUL=0;
          Delay100us();
          Delay100us();
          Delay100us();
          PUL=1;
          Delay100us();
          Delay100us();
          Delay100us();
     }
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

     if (rptr != wptr)
     {
          UART_Handle(buffer[rptr++]);
          rptr &= 0x0f;
     }
}


void Rom_Init()
{
     MotorCycle=IapRead(0x0000);
     MotorHz=IapRead(0x0010);
}


void UART_Handle(u8 dat)
{
     switch (dat)
     {
     case MB_CycleUp: 
     {
          MotorCycle=MotorCycle+1;
          break;
     }
     case MB_CycleDown:
     {
          MotorCycle=MotorCycle-1;
          break;
     }
     case MB_HzUp:
     {
          MotorHz=MotorHz+1;
          break;
     }
     case MB_HzDown:
     {
          MotorHz=MotorHz-1;
          break;
     }
     case 'z':
     {
          MotorHz=160;
          MotorCycle=2;
          break;
     }
     case 'm':
     {
          IAP_CONTR |= 0x60;
          return;
     }
     case 's':
     {
          databit=IapRead(0x0000);
          Delay100us();
          UartSend(databit);
          delay_ms(100);
          databit=IapRead(0x0010);
          Delay100us();
          UartSend(databit);
          delay_ms(100);
          UartSend(MotorHz);
          delay_ms(100);
          UartSend(MotorCycle);
          return;
     }
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

void ROM_Write()
{
     IapErase(0x0000);
     Delay100us();
     IapProgram(0x0000,MotorCycle);
     Delay100us();
     IapProgram(0x0010,MotorHz);
}


int main() 
{
     P2=0x00;
     P0M1=0x00;
     P0M0=0X00;
     P2M0=0xff;
     P2M1=0x00;
     //delay_ms(5);
     //UART_Init();
     delay_ms(5);
     UartInit();
     EA=1;
     ES=1;
     Rom_Init();
     delay_ms(1000);
     Tohome();
     while(1)
     {
          if(X2!=0)
          {
               if(X0==0&&X1==0)
               {
                    Run();
               }
          }
          UART_Read(0);
     }

     return 0;
}

