#include"stc8.h"
#include"config.h"
#include "intrins.h"

#define BRT             (65536 - MAIN_Fosc / 115200 / 4)


void Uart2Isr() interrupt 8
{
    if (S2CON & 0x02)
    {
        S2CON &= ~0x02;
        busy = 0;
    }
    if (S2CON & S2RI)
    {
        S2CON &= ~S2RI;
        uart2buffer = S2BUF;
        uart2readflage = 0x0f;
    }
}

void Uart2Init()
{
    S2CON = 0x10;
    T2L = BRT;
    T2H = BRT >> 8;
    AUXR |= 0x14;
    busy = 0;
}

void Uart2Send(char dat)
{
    while (busy);
    busy = 1;
    S2BUF = dat;
}

void Uart2SendStr(char *p)
{
    while (*p)
    {
        Uart2Send(*p++);
    }
}

// void main()
// {
//     Uart2Init();
//     IE2 = 0x01;
//     EA = 1;
//     Uart2SendStr("Uart Test !\r\n");

//     while (1)
//     {
//         if (rptr != wptr)
//         {
//             Uart2Send(buffer[rptr++]);
//             rptr &= 0x0f;
//         }
//     }
// }

