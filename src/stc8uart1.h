#include "stc8.h"
#include "config.h"
#include "intrins.h"

#define BRT (65536 - MAIN_Fosc / 115200 / 4)

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
        uart1buffer = SBUF;
        uart1readflage = 0x0f;
    }
}

void UartInit()
{
    SCON = 0x50;
    TMOD = 0x00;
    TL1 = BRT;
    TH1 = BRT >> 8;
    TR1 = 1;
    AUXR |= 0x40;
    busy = 0;
}

void UartSend(char dat)
{
    while (busy)
        ;
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

//测试
// void main()
// {
//     UartInit();
//     ES = 1;
//     EA = 1;
//     UartSendStr("Uart Test !\r\n");

//     while (1)
//     {
//         if (rptr != wptr)
//         {
//             UartSend(buffer[rptr++]);
//             rptr &= 0x0f;
//         }
//     }
// }
