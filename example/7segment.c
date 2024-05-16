#define BIT0 (0x0001)
#define BIT1 (0x0002)
#define BIT2 (0x0004)
#define BIT3 (0x0008)
#define BIT4 (0x0010)
#define BIT5 (0x0020)
#define BIT6 (0x0040)
#define BIT7 (0x0080)
#define BIT8 (0x0100)
#define BIT9 (0x0200)
#define BITA (0x0400)
#define BITB (0x0800)
#define BITC (0x1000)
#define BITD (0x2000)
#define BITE (0x4000)
#define BITF (0x8000)

#include <msp430.h>
//7segment pin ->f c dp b a g e d
const unsigned char segment_map[10] = { 0b11011011, // 0 ok
        0b01010000, // 1 ok
        0b00011111, // 2 ok
        0b01011101, // 3 ok
        0b11010100, // 4 ok
        0b11001101, // 5 ok
        0b11001111, // 6 ok
        0b01011000, // 7 ok
        0b11011111, // 8 ok
        0b11011100  // 9 ok
        };

void main(void)
{

    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

    P3DIR |= 0xFF; // P3.0-P3.7 output 설정
    P3OUT &= ~0xFF; // P3.0-P3.7 clear
    P4DIR |= (BIT0);
    P4OUT &= ~BIT0;

    __bis_SR_register(GIE);

    volatile unsigned char num = 0;
    volatile unsigned char digit = 0;
    while (1)
    {

        P3OUT = segment_map[num];
        __delay_cycles(100000);
        if (num > 8)
        {
            num = 0;
        }
        else
        {
            num++;
        }
        if()
    }
}

