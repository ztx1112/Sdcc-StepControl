#include "stc8.h"
#include <intrins.h>

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


void Runmotor(unsigned int);
void Tohome();
void Run();
int Ishome();

unsigned int i;

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




int main() 
{
     P2=0x00;
     P0M1=0x00;
     P0M0=0X00;
     P2M0=0xff;
     P2M1=0x00;
     delay_ms(100);
     Tohome();
     while(1)
     {
          if(X3==0&&X4!=0&&X2!=0)
          {
               delay_ms(5);
               if(X3==0)Run();
          }
     }

     return 0;
}

void Run()
{
     Y0=1;
     while(1)
     {
          if(X4==0)
          {
               delay_ms(10);
               Runmotor(2000);
               Y0=0;
               Tohome();
               while (1)
               {
                    if(X4!=0&&X2!=0)break;
               }
               return;
          }
     }
}


void Runmotor(unsigned int num)
{
     i=0;
     while(1)
     {
          if(i>num)break;
          PUL=0;
          Delay100us();
          Delay100us();
          Delay100us();
          PUL=1;
          Delay100us();
          Delay100us();
          Delay100us();
          i=i+1;
     }
}

void Tohome()
{
     Runmotor(100);
     while(1)
     {
          if(X2==1)return;
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