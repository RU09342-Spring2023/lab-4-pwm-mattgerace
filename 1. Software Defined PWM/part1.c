#include <msp430.h> 


/**
 * main.c
 */

unsigned short DutyCycle = 1000;
    int main(void)
    {
        WDTCTL = WDTPW | WDTHOLD;                    // Stop WDT
        // Configure GPIO
        P1DIR |= BIT0;
        P6DIR |= BIT6;
        P2DIR &= ~BIT3;
        P4DIR &= ~BIT1;

        P4REN |= BIT1;               // Enable Resistor on P4.1
        P4OUT |= BIT1;               // Configure Resistor on P4.1 to be Pullup
        P2REN |= BIT3;                  // Enable resistor on P2.3
        P2OUT |= BIT3;                  // Configure resistor on P2.3 to be a pullup

        P2IES &= ~BIT3;                         // P2.3 Low --> High edge    (changed the edge sensitivity)
        P2IE |= BIT3;                           // P2.3 interrupt enabled
        P4IES &= ~BIT1; //edge sensitivity
        P4IE |= BIT1; //enable interrupt



        // Disable the GPIO power-on default high-impedance mode to activate
        // previously configured port settings
        PM5CTL0 &= ~LOCKLPM5;
        // Configure Timer_A
        TB0CTL = TBSSEL_2 | MC_1 | TBCLR | TBIE;      // SMCLK, continuous mode, clear TBR, enable interrupt
        TB0CCTL1 |= CCIE;
        TB0CCR0 = DutyCycle;
        TB0CCR1 = DutyCycle/2;                          // Set CCR1 to the value to set the duty cycle

        TB1CTL = TBSSEL_2 | MC_1 | TBCLR | TBIE;      // SMCLK, continuous mode, clear TBR, enable interrupt
        TB1CCTL1 |= CCIE;
        TB1CCR0 = DutyCycle;
        TB1CCR1 = DutyCycle/2;                          // Set CCR1 to the value to set the duty cycle

        __bis_SR_register(LPM3_bits | GIE);           // Enter LPM3, enable interrupts
    }

    // Timer0_B3 Interrupt Vector (TBIV) handler
    #if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
    #pragma vector=TIMER0_B1_VECTOR
    __interrupt void TIMER0_B1_ISR(void)
    #elif defined(__GNUC__)
    void __attribute__ ((interrupt(TIMER0_B1_VECTOR))) TIMER0_B1_ISR (void)
    #else
    #error Compiler not supported!
    #endif
    {
        switch(__even_in_range(TB0IV,TB0IV_TBIFG))
        {
            case TB0IV_NONE:
                break;                               // No interrupt
            case TB0IV_TBCCR1:
                P1OUT &= ~BIT0;
                break;                               // CCR1 Set the pin to a 0
            case TB0IV_TBCCR2:
                break;                               // CCR2 not used
            case TB0IV_TBIFG:
                P1OUT |= BIT0;                       // overflow Set the pin to a 1
                break;
            default:
                break;
        }
    }

    // Timer0_B3 Interrupt Vector (TBIV) handler
    #if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
    #pragma vector=TIMER1_B1_VECTOR
    __interrupt void TIMER1_B1_ISR(void)
    #elif defined(__GNUC__)
    void __attribute__ ((interrupt(TIMER1_B1_VECTOR))) TIMER1_B1_ISR (void)
    #else
    #error Compiler not supported!
    #endif
    {
        switch(__even_in_range(TB1IV,TB1IV_TBIFG))
        {
            case TB1IV_NONE:
                break;                               // No interrupt
            case TB1IV_TBCCR1:
                P6OUT &= ~BIT6;
                break;                               // CCR1 Set the pin to a 0
            case TB1IV_TBCCR2:
                break;                               // CCR2 not used
            case TB1IV_TBIFG:
                P6OUT |= BIT6;                       // overflow Set the pin to a 1
                break;
            default:
                break;
        }
    }

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{

    if(TB0CCR1 >= 1000)
        TB0CCR1 = 0;
    else
        TB0CCR1 += 100;
    P2IFG &= ~BIT3;


}

#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{

    if(TB1CCR1 >= 1000)
        TB1CCR1 = 0;
    else
        TB1CCR1 += 100;
    P4IFG &= ~BIT1;


}

