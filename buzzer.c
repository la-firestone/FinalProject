# include "buzzer.h"

void buzzerInit(void) {
    P2->DIR |= BIT7; // P2.7 set as GPIO Output
    P2->SEL0 &= ~(BIT7);
    P2->SEL1 &= ~(BIT7);
}

void playNote(uint16_t frequency, int duration_ms)
{
    //
    uint32_t period = 1000000 / frequency;  // Calculate the period in microseconds
    uint32_t halfPeriod = period / 2;        // Calculate the half-period in microseconds

    int i = 0;
    int j = 0;

    //while(duration <= halfPeriod*)
    for (j=0;j<duration_ms*1000/period;j++)
    {
        P2->OUT ^= BIT7;       // Toggle the buzzer pin
        for (i =0;i<halfPeriod;i++)
        {
        __delay_cycles(1);
        }
        i = 0;
    }
    P2->OUT &= ~(BIT7);
}

