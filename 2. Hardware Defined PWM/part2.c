#include <msp430.h>



/**
 * main.c
 *
 * Created on: Feb 22, 2023
 * Author: Matthew Gerace
 *
 */

int RG = 1; //initiating flags
int GB = 0;
int BR = 0;


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; // disable the GPIO power-on default high-impedance mode to activate previously configured port settings
    init();
    timerinit();



    while(1)
    {
        TB3CCR1 = 0; // start red on
        TB3CCR2 = 1000; // start green off
        TB3CCR3 = 1000; // start blue off

        if(RG == 1)
        {
            while(TB3CCR1 < 1000)
            {
                TB3CCR1++;
                TB3CCR2--;
                __delay_cycles(300);
            }
            RG = 0;
            GB = 1;
        }
        if(GB == 1)
        {
            while(TB3CCR2 < 1000)
            {
                TB3CCR2++;
                TB3CCR3--;
                __delay_cycles(300);
            }
            GB = 0;
            BR = 1;
        }
        if(BR == 1)
        {
            while(TB3CCR3 < 1000)
            {
                TB3CCR3++;
                TB3CCR1--;
                __delay_cycles(300);
            }
            BR = 0;
            RG = 1;
        }
    }


}

void init()
{
    P6OUT &= ~BIT0;
    P6OUT &= ~BIT1;
    P6OUT &= ~BIT2;

    P6DIR |= BIT0;
    P6DIR |= BIT1;
    P6DIR |= BIT2;

    P6SEL0 |= BIT0;
    P6SEL0 |= BIT1;
    P6SEL0 |= BIT2;

    P6SEL1 &= ~BIT0;
    P6SEL1 &= ~BIT1;
    P6SEL1 &= ~BIT2;
}

void timerinit()
{
    TB3CCR0 = 1000;
    TB3CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR
    TB3CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TB3CCTL2 = OUTMOD_7;                      //CCR2 reset /set
    TB3CCTL3 = OUTMOD_7;                    //CCR3 reset/set

}
