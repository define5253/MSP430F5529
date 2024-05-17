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

volatile unsigned int count = 0;
volatile unsigned int display = 0;
volatile unsigned int digitnum = 0;

void Display()
{
    static unsigned int display1, display2, display3, display4;
    switch (digitnum)
    {
    case 0:
        display1 = display % 1000 % 100 % 10;
        P3OUT = segment_map[display1];
        P4OUT |= (BIT1 | BIT2 | BIT3);
        P4OUT &= ~BIT0;
        digitnum = 1;
        break;
    case 1:
        display2 = display % 1000 % 100 / 10;
        P3OUT = segment_map[display2];
        P4OUT |= (BIT2 | BIT3 | BIT0);
        P4OUT &= ~BIT1;
        digitnum = 2;
        break;
    case 2:
        display3 = display % 1000 / 100;
        P3OUT = segment_map[display3];
        P4OUT |= (BIT3 | BIT0 | BIT1);
        P4OUT &= ~BIT2;
        digitnum = 3;
        break;
    case 3:
        display4 = display / 1000;
        P3OUT = segment_map[display4];
        P4OUT |= (BIT0 | BIT1 | BIT2);
        P4OUT &= ~BIT3;
        digitnum = 0;
        break;
    }
}
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

    //인코더
    P1DIR &= ~(BIT2 | BIT3); //P1.2 P1.3 input으로 설정
    P1REN |= (BIT2 | BIT3); //P1.2 P1.3 pull-up, pull-down resistor
    P1OUT &= ~(BIT2 | BIT3); //pull-up resistor 로 설정
    P1IE |= (BIT2 | BIT3); //enable interrupt
    P1IES &= ~(BIT2 | BIT3); //0일때 RISING EDGE, 1일때 FALLING EDGE
    P1IFG &= ~(BIT2 | BIT3); //clear interrupt flag

    //모터
    P2DIR |= (BIT4 | BIT5);
    P2SEL |= (BIT4 | BIT5);

    TA0CTL = TASSEL_2 + MC_2 + ID_3 + TACLR;
    TA0CCTL = CCIE;
    TA2CTL = TASSEL_2 + MC_1;
    TA2CCR0 = 1000;
    TA2CCTL1 = OUTMOD_6;
    TA2CCR1 = 500;
    TA2CCTL2 = OUTMOD_6;
    TA2CCR2 = 0;

    //7SEGMENT
    P3DIR |= 0xFF; // P3.0-P3.7 output 설정
    P3OUT &= ~0xFF; // P3.0-P3.7 clear
    P4DIR |= (BIT0 | BIT1 | BIT2 | BIT3);
    P4OUT = BIT1 + BIT2 + BIT3;

    __bis_SR_register(GIE);

    while (1)
    {
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    Display();
    if ((P1IFG & BIT2) == 1) //P1.2 rising edge
    {
        if ((P1IN & BIT3) == 0) //P1.3 = 0
        {
            count--;
        }
        else //P1.3 = 1
        {
            count++;
        }
        P1IFG &= ~BIT2; //P1.2 interrupt flag clear
    }
    if ((P1IFG & BIT3) == 1) //P1.3 rising edge
    {
        if ((P1IN & BIT2) == 0) //P1.2 = 0
        {
            count--;
        }
        else //P1.2 = 1
        {
            count++;
        }
        P1IFG &= ~BIT3; //P1.3 interrupt flag clear
    }
    count++;
    display = count / 100;
}

