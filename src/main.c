#include "C:\Users\Administrator\Documents\Source\MCU\bujin\dependence\build-in\STD\stc8.h"
#include <C:\Keil_v5\C51\INC\intrins.h>


void Delay80ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 8;
	j = 76;
	k = 168;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
void Delay10ms()		//@24.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 1;
	j = 234;
	k = 113;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
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

int Run()
{
     Y0=1;
     while (1)
     {
          if(X1==0)
          {
               Delay80ms();
               Y1=1;
               while (1)
               {
                    if(X2==0)
                    {
                         Delay10ms();
                         Y2=1;
                         while (1)
                         {
                              if(X4==0)
                              {
                                   Delay80ms();
                                   Y2=0;
                                   Y1=0;
                                   Y0=0;
                                   return;
                              }
                         }
                         
                    }
               }
               
          }
     }
     
}

int main() 
{
     P0=0Xff;
     P2=0x00;
     P0M1=0x00;
     P0M0=0X00;
     P2M0=0xff;
     P2M1=0x00;
     Delay80ms();
     while(1)
     {
          if(X2!=0&&X1!=0)
          {
               if(X6==0)
               {
                    Run();
               }
          }
     }
     return 0;
}

