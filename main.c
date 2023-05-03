
#include <msp.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcdLib_432.h"
#include "motors.h"
#include "buzzer.h"

#define THRESH 2000
volatile uint16_t rightSensor;
volatile uint16_t midSensor;
volatile uint16_t leftSensor;
volatile int running;

// ************ Function Prototypes ************
void randInt();

// ******************** MAIN *******************
int main(void)
    {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer

    /* Switch S1 */
    P1->DIR &= ~BIT1;  // Switch 1 at P1.1 set for input direction
    P1->REN |= BIT1;   // Enable Input Resistor
    P1->OUT |= BIT1;   // Set resistor to pull up

    //INIT LEDS
    P6->DIR |= BIT1;  // Switch 1 at P1.1 set for input direction
    P6->OUT &= ~BIT1;   // Set resistor to pull up

    // Interrupt Configuration
    P1->IE |=    BIT4 | BIT1;     // enable BIT0 and BIT1 as interrupts
    P1->IES &= ~(BIT4 | BIT1); // set BIT0 and BIT1 as rising edge, 0->1 or pull-down
    P1->IFG &= ~(BIT4 | BIT1); // clear interrupt flag

    // enable NVIC for Port 1
    NVIC->ISER[1] = 1 << ((PORT1_IRQn)&31);

    __enable_irq();


    // ******************** INIT ADC *******************

    // Configure GPIO
    P5->SEL1 |= BIT0 | BIT1 | BIT2; // Enable A/D channel A0-A3
    P5->SEL0 |= BIT0 | BIT1 | BIT2;

    // Enable global interrupt
    //__enable_irq();
    //NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 31);// Enable ADC interrupt in NVIC module

    // Turn on ADC14, extend sampling time to avoid overflow of results
    ADC14->CTL0 = ADC14_CTL0_ON | ADC14_CTL0_MSC | ADC14_CTL0_SHT0_7 | ADC14_CTL0_SHP | ADC14_CTL0_CONSEQ_3;
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_5; // ref+=AVcc, channel = A5 5.0
    ADC14->MCTL[1] = ADC14_MCTLN_INCH_4; // ref+=AVcc, channel = A4 5.1
    ADC14->MCTL[2] = ADC14_MCTLN_INCH_3 | ADC14_MCTLN_EOS; // ref+=AVcc, channel = A3 5.2
    //ADC14->MCTL[3] = ADC14_MCTLN_INCH_0| ADC14_MCTLN_EOS;// ref+=AVcc, channel = A1, end seq. 5.5

    //ADC14->IER0 = ADC14_IER0_IE3; // Enable ADC14IFG.3
    //SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk; // Wake up on exit from ISR

    // Ensures SLEEPONEXIT takes effect immediately
     //__DSB();
    SysTick_Init();
    lcdInit();
    lcdClear();
    motorInit();
    motorPWMInit();

    int bit;
    running=0;

     while(1)
    {

         if (running==1){

             // Start conversion-software trigger
             //while (ADC14->CTL0&0x00010000){}; // wait for ADC
             ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
             rightSensor = ADC14->MEM[0]; // Move A0 results, IFG is cleared
             midSensor = ADC14->MEM[1]; // Move A1 results, IFG is cleared
             leftSensor = ADC14->MEM[2]; // Move A2 results, IFG is cleared
             //backSensor = ADC14->MEM[3]; // Move A3 results, IFG is cleared

             //__sleep();
             //__no_operation(); // For debugger
             //__delay_cycles(200000);
             //printf("ADC Values: %d : %d : %d\n", rightSensor, midSensor, leftSensor);
             lcdClear();

             if (rightSensor < THRESH){
             lcdSetText("turn left",0,0);
             turnLeftDelay(500);
             delayms(500);
             }

             else if (leftSensor < THRESH){
             lcdSetText("turn right",0,0);
             turnRightDelay(500);
             delayms(500);
             }

             else if (midSensor < THRESH){
             lcdSetText("backup",0,0);
             goReverseDelay(500);

                 bit = randBit();
                 if (bit==0){
                    turnLeftDelay(500);
                 }

                 else{
                     turnRightDelay(500);
                 }
             }

             else{
                 goForward();
             }

        } // close if running

         else {
             stop();
         }

    } // close while loop

} //close main

int randBit(void){
    srand(time(NULL)); // seed the random number generator with current time
    int random_num = rand(); // generate a random integer number
    int random_bit = random_num % 2;
    return random_bit;
}

// ADC14 interrupt service routine
//void ADC14_IRQHandler(void)
//{
//    if (ADC14->IFGR0 & ADC14_IFGR0_IFG3)
//    {
//        rightSensor = ADC14->MEM[0]; // Move A0 results, IFG is cleared
//        midSensor = ADC14->MEM[1]; // Move A1 results, IFG is cleared
//        leftSensor = ADC14->MEM[2]; // Move A2 results, IFG is cleared
//        backSensor = ADC14->MEM[3]; // Move A3 results, IFG is cleared
//        //__no_operation(); // Set Breakpoint1 here
//    }
//}

void PORT1_IRQHandler()
{
    int i;
    uint8_t result = P1->IFG;


    if(result & BIT1)   // check button 1
    {

        if (running==0){ // disables SysTick
            running = 1;
            P6->OUT |= BIT1;
            lcdClear();
            lcdSetText("Starting!",0,0);

        }

        else if (running==1) {
           running = 0;
           P6->OUT &= ~BIT1;
           lcdClear();
           lcdSetText("Zzz...",0,0);
        }

        P1->IFG &= ~(result);
        delayms(500);

    }
}
