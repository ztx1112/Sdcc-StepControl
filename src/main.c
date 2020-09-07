#include <C:\Keil_v5\C51\INC\intrins.h>
#include <C:\Keil_v5\C51\INC\stdlib.h>
#include <C:\Keil_v5\C51\INC\string.h>
#include "stc8.h"
#include "Stc8Iap.h"
#include "stc8uart1.h"
#include "stc8uart2.h"

sbit Y0 = P2 ^ 5;
sbit Y1 = P2 ^ 4;
sbit Y2 = P2 ^ 3;
sbit Y3 = P2 ^ 2;
sbit Y4 = P4 ^ 2;
sbit Y5 = P2 ^ 0;

sbit X0 = P1 ^ 2;
sbit X1 = P1 ^ 3;
sbit X2 = P1 ^ 4;
sbit X3 = P4 ^ 4;
sbit X4 = P1 ^ 5;
sbit X5 = P1 ^ 6;
sbit X6 = P1 ^ 7;
sbit X7 = P3 ^ 2;
sbit X8 = P3 ^ 3;
sbit X9 = P3 ^ 4;

sbit PUL = P0 ^ 6;
sbit DIR = P0 ^ 4;

#define MB_CycleUp '3'
#define MB_CycleDown '4'
#define MB_HzDown '7'
#define MB_HzUp '6'

char Button_00[] = "[BN:0]";
char Button_01[] = "[BN:1]";
char Button_02[] = "[BN:2]";
char Button_03[] = "[BN:3]";
char Button_04[] = "[BN:4]";
char Button_05[] = "[BN:5]";
char Button_06[] = "[BN:6]";
char Button_07[] = "[BN:7]";
char Button_08[] = "[BN:8]";
char Button_09[] = "[BN:9]";

char tests[]="[BN:2]";

char Page_00[] = "PAGE0OK";
char Page_01[] = "PAGE1OK";
char Page_02[] = "PAGE2OK";
char Page_03[] = "PAGE3OK";

void Runmotor(unsigned int);
void Tohome();
void Run();
void UART_Read(u8);
void UART_Handle(char *);
void Rom_Init();
void ROM_Write();
void ButtonHandle(u8);
void CommandHandle();
void test();

unsigned char MotorHz;    //马达运行频率 addres=0x0010
unsigned char MotorCycle; //马达运行圈数 addres=0x0000
unsigned char home_cycle; //原点偏移 addres=0x0020

unsigned char Page = 0; //当前页面
unsigned char buf;

char uart1buff[20] = {0};
char uart2buff[20] = {0};
unsigned char uart1num = 0;
unsigned char uart2num = 0;
unsigned char uart1end=0;
unsigned char uart2end=0;

unsigned int i, ii;

u8 Button1;
u8 Button2;

u8 flag, flage2;

void Delay100ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 13;
	j = 45;
	k = 214;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void Delay100us() //@24.000MHz
{
     unsigned char i, j;
     i = 4;
     j = 27;
     do
     {
          while (--j)
               ;
     } while (--i);
}
void Delay1us() //@24.000MHz
{
     unsigned char i;

     i = 6;
     while (--i)
          ;
}

void UART_Read(u8 dat)
{
     if(uart1readflage)
     {
          if(uart2buffer=='D')IAP_CONTR = 0x60;
          uart1end=1;
          uart1readflage=0x00;
     }
     if (uart2readflage)
     {
          buf = uart2buffer;
          if (buf != 0x0d && buf != 0x0a && buf != 'K')
          {
               uart2buff[uart2num] = buf;
               uart2num += 1;
          }
          if (buf == 0x0d || buf == 0x0a || buf == 'K')
          {
               uart2buff[uart2num] = buf;
               uart2num += 1;
               uart2end=1;
          }
          uart2readflage=0x00;
     }
}

void Run()
{
     Y0 = 1;
     while (1)
     {
          if (X2 == 0)
          {
               Runmotor(MotorCycle * 400);
               Y0 = 0;
               Tohome();
               while (1)
               {
                    if (X3 != 0 && X2 != 0)
                         break;
               }
               Delay100us();
               ROM_Write();
               return;
          }
     }
}

void Runmotor(u16 num)
{
     i = 0;
     ii = 0;
     while (1)
     {
          if (i > num)
               break;
          PUL = 0;
          for (ii = 0; ii < MotorHz; ii++)
          {
               Delay1us();
          }
          PUL = 1;
          for (ii = 0; ii < MotorHz; ii++)
          {
               Delay1us();
          }
          i = i + 1;
     }
}

void Tohome()
{
     i = 0;
     for (i = 0; i < 100; i++)
     {
          PUL = 0;
          Delay100us();
          Delay100us();
          Delay100us();
          PUL = 1;
          Delay100us();
          Delay100us();
          Delay100us();
     }
     while (1)
     {
          if (X1 == 1)
               break;
          PUL = 0;
          Delay100us();
          Delay100us();
          Delay100us();
          Delay100us();
          Delay100us();
          PUL = 1;
          Delay100us();
          Delay100us();
          Delay100us();
          Delay100us();
          Delay100us();
     }
     Runmotor(home_cycle);
     return;
}

void Rom_Init()
{
     MotorCycle = IapRead(0x0000);
     MotorHz = IapRead(0x0010);
     home_cycle = IapRead(0x0020);
}

void UART_Handle(char *dat)
{
     if(uart1end)
     {
          uart1num=0;
          uart1end=0;
          // i=sizeof(uart1buff)/sizeof(uart1buff[0]);
          // for(ii=0;ii<i;ii++)
          // {
          //      uart1buff[ii]=0;
          // }


     }

     if(uart2end)
     {
          if(strstr(uart2buff,Button_02))
          {
               switch (Page)
               {
               case 0:
               {                    
                    Run();
                    break;
               }
               default:
                    break;
               }
          }
          if(strstr(uart2buff,Button_03))
          {
               Y0=1;
               switch (Page)
               {
               case 1:
               {    
                    MotorCycle+=1;
                    UartSend(MotorCycle);
                    break;
               }
               default:
                    break;
               }
               Y0=0;
          }
          if(strstr(uart2buff,Button_04))
          {
               switch(Page)
               {
                    case 0:
                    {
                         IAP_CONTR = 0x60;
                         break;
                    }
                    case 1:
                    {
                         MotorCycle-=1;
                         UartSend(MotorCycle);
                         break;
                    }
                    default:
                         break;
               }
          }
          if(strstr(uart2buff,Button_06))
          {
               switch (Page)
               {
               case 2:
                    {
                         home_cycle+=1;
                         UartSend(home_cycle);
                         break;
                    }
               default:
                    break;
               }
          }
          if(strstr(uart2buff,Button_07))
          {
               switch(Page)
               {
                    case 1:
                    {
                         MotorHz+=5;
                         UartSend(MotorHz);
                         break;
                    }
                    case 2:
                    {
                         home_cycle-=1;
                         UartSend(home_cycle);
                         break;
                    }
                    default:
                         break;
               }
          }
          if(strstr(uart2buff,Button_08))
          {
               switch(Page)
               {
                    case 1:
                    {
                         MotorHz-=5;
                         UartSend(MotorHz);
                         break;
                    }
                    default:
                         break;
               }
          }

          if(strstr(uart2buff,Page_00))
          {
               Page=0;
          }
          if(strstr(uart2buff,Page_01))
          {
               Page=1;
          }
          if(strstr(uart2buff,Page_02))
          {
               Page=2;
          }


          uart2num=0;
          uart2end=0;
          i=sizeof(uart2buff)/sizeof(uart2buff[0]);
          for(ii=0;ii<i;ii++)
          {
               uart2buff[ii]=0;
          }
     }
#pragma region 
     // switch (dat[0])
     // {
     // case MB_CycleUp:
     // {
     //      MotorCycle = MotorCycle + 1;
     //      break;
     // }
     // case MB_CycleDown:
     // {
     //      MotorCycle = MotorCycle - 1;
     //      break;
     // }
     // case MB_HzUp:
     // {
     //      MotorHz = MotorHz + 1;
     //      break;
     // }
     // case MB_HzDown:
     // {
     //      MotorHz = MotorHz - 1;
     //      break;
     // }
     // case 'z':
     // {
     //      MotorHz = 160;
     //      MotorCycle = 2;
     //      break;
     // }
     // case 'm':
     // {
     //      IAP_CONTR |= 0x60;
     //      return;
     // }
     // case 's':
     // {
     //      // databit = IapRead(0x0000);
     //      // Delay100us();
     //      // UartSend(databit);
     //      // delay_ms(100);
     //      // databit = IapRead(0x0010);
     //      // Delay100us();
     //      // UartSend(databit);
     //      // delay_ms(100);
     //      // UartSend(MotorHz);
     //      // delay_ms(100);
     //      // UartSend(MotorCycle);
     //      return;
     // }
     // }
#pragma endregion
}

void ROM_Write()
{
     IapErase(0x0000);
     Delay100us();
     IapProgram(0x0000, MotorCycle);
     Delay100us();
     IapProgram(0x0010, MotorHz);
     Delay100us();
     IapProgram(0x0020, home_cycle);
}

int main()
{
     Rom_Init();
     P2 = 0x00;
     P0M1 = 0x00;
     P0M0 = 0X00;
     P2M0 = 0xff;
     P2M1 = 0x00;
     UartInit();
     Uart2Init();
     AUXR = 0X54;
     EA = 1;
     ES = 1;
     IE2 = 0x01;
     Tohome();
     while (1)
     {
          if (X2 != 0)
          {
               if (X0 == 0)
               {
                    Run();
               }
          }
          UART_Read(0);
          UART_Handle(0);
     }

     return 0;
}
