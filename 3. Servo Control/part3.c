/*
 * part3.c
 *
 *  Created on: May 2, 2023
 *      Author: Matthew Gerace
 */

#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

    P2DIR &= ~BIT3;                 //initialize 2.3 input
    P2IES |= BIT3;                 // P2.3 High --> Low edge
    P2IE |= BIT3;                   //interrupt enabled

    P4DIR &= ~BIT1;                 //initialize 4.1 input
    P4IES |= BIT1;                  // P4.1 High --> Low edge
    P4IE |= BIT1;                   //interrupt enabled


    P6DIR |= BIT0;                     // set p6.6 to output
    P6SEL0 |= BIT0;
    P6SEL1 &= ~BIT0;


    TB0CCR0 = 20000;       //ccr0 set to 20000
    TB0CCTL1 = OUTMOD_7;
    TB0CCR1 = 20000*0.075;
    TB0CTL = TBSSEL__SMCLK | MC__UP | TBCLR; //smclk up mode



    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM0_bits + LPM3_bits + GIE);     // Enter LPM
    __enable_interrupt();                               // interupts
}


#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~BIT3; //clear interrupt flag

    if(TB0CCR1 >= 20000) //hold at max
    {
        TB0CCR1 = 20000;
    }
    else {
        TB0CCR1 += 100;                     //increase duty cycle
    }
}


#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    P4IFG &= ~BIT1; //clear interrupt flag

    if(TB0CCR1 <= 1) //hold at minimum
    {
        TB0CCR1 = 1;
    }
    else {
        TB0CCR1 -= 100;                     //decrease duty cycle
    }
}
