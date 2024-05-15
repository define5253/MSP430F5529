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
#include <stdio.h>

volatile signed int count = 0;

void main(void)
{

    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

    //인코더
    P1DIR &= ~(BIT2 | BIT3); //P1.2 P1.3 input으로 설정
    P1REN |= (BIT2 | BIT3); //P1.2 P1.3 pull-up, pull-down resistor
    P1OUT |= (BIT2 | BIT3); //pull-up resistor 로 설정
    P1IE |= (BIT2 | BIT3); //enable interrupt
    P1IES &= (BIT2 | BIT3); //0일때 RISING EDGE, 1일때 FALLING EDGE
    P1IFG &= ~(BIT2 | BIT3); //clear interrupt flag

    //모터
    P2DIR |= (BIT4 | BIT5);
    P2SEL |= (BIT4 | BIT5);

    TA2CTL = TASSEL_2 + MC_1;
    TA2CCR0 = 1000;
    TA2CCTL1 = OUTMOD_6;
    TA2CCR1 = 0;
    TA2CCTL2 = OUTMOD_6;
    TA2CCR2 = 0;

    __bis_SR_register(GIE);

    while (1)
    {
        TA2CCR1 = 1000;
        TA2CCR0 = 0;
        __delay_cycles(1000);
        printf("%d\n", count);
    }
}
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if ((P1IFG & BIT2) == 1) //)P1.2 rising edge
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
    if ((P1IFG & BIT3) == 1) //)P1.3 rising edge
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
}

