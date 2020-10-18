/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <math.h>
#include <stdint.h>
#include "global.h"

//Global Variables:
double moveX;           //Difference in x to move in mm
double moveY;           //Difference in y to move in mm
double moveZ;           //Difference in z to move in mm
double moveA;           //Difference in a to move in mm
double moveB;           //Difference in b to move in mm
int steps[5];           //Array of number of steps to move for each position
double updateMovement[5];       //Value of one step to update


// Enable clock to timer3. Using timer3 because of general purpose usage.
// Setup prescalar and arr so that the interrupt is triggered every 500 us.
// Enable the timer 3 interrupt.
// Start the timer.
void setup_timer3()
{
    // Enable timer 3 here.
    // Make it generate an interrupt 2000 times per second.
    // Set the prescaler to 48 so that the free-running
    // counter goes from 0 ... 499
    // Possibly change to another timer.
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->PSC = 47;
    TIM3->ARR = 499;
    TIM3->DIER |= TIM_DIER_UIE;
    NVIC->ISER[0] = (1<<TIM3_IRQn);

    NVIC_SetPriority(TIM3_IRQn,0);                                              //check priority, might need to change if another timer is needed for the pushbutton
}

void setup_GPIO()                                                               //possibly change name, check with Eric
{
    //enable clock to GPIOA, GPIOB, and GPIOC
    //set output mode for each pin

                                                                                //POSSIBLY CHECK IF THE GPIO USART SETUP NEEDS TO GO HERE AS WELL!!!!
    //Clock to GPIOA
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    //Clock to GPIOB
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    //Clock to GPIOC
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;                                          //possibly check, should be fine

    //set PA10 to output (X-Pls)
    GPIOA->MODER &= ~(3<<(2*10));                                               //POSSIBLY CHANGE BACK USING ~ IN FRONT OF (3 <<)            //Clears PA10 bit
    GPIOA->MODER |= (1<<(2*10));                                                            //Sets PA10 bit to output

    //PA11; X-DIR
    GPIOA->MODER &= ~(3<<(2*11));
    GPIOA->MODER |= (1<<(2*11));

    //PA12; X-EN
    GPIOA->MODER &= ~(3<<(2*12));
    GPIOA->MODER |= (1<<(2*12));

    //set PC9 to output (Y-Pls)
    GPIOC->MODER &= ~(3<<(2*9));
    GPIOC->MODER |= (1<<(2*9));

    //PA8; Y-DIR
    GPIOA->MODER &= ~(3<<(2*8));
    GPIOA->MODER |= (1<<(2*8));

    //PA9; Y-EN
    GPIOA->MODER &= ~(3<<(2*9));
    GPIOA->MODER |= (1<<(2*9));

    //set PC6 to output (Z-Pls)
    GPIOC->MODER &= ~(3<<(2*6));
    GPIOC->MODER |= (1<<(2*6));

    //PC7; Z-DIR
    GPIOC->MODER &= ~(3<<(2*7));
    GPIOC->MODER |= (1<<(2*7));

    //PC8; Z-EN
    GPIOC->MODER &= ~(3<<(2*8));
    GPIOC->MODER |= (1<<(2*8));

    //set PB2 to output (A-Pls)
    GPIOB->MODER &= ~(3<<(2*2));
    GPIOB->MODER |= (1<<(2*2));

    //PB10; A-DIR
    GPIOB->MODER &= ~(3<<(2*10));
    GPIOB->MODER |= (1<<(2*10));

    //PB11; A-EN
    GPIOB->MODER &= ~(3<<(2*11));
    GPIOB->MODER |= (1<<(2*11));

    //set PC5 to output (B-Pls)
    GPIOC->MODER &= ~(3<<(2*5));
    GPIOC->MODER |= (1<<(2*5));

    //PB0; B-DIR
    GPIOB->MODER &= ~(3<<(2*0));
    GPIOB->MODER |= (1<<(2*0));

    //PB1; B-EN
    GPIOB->MODER &= ~(3<<(2*1));
    GPIOB->MODER |= (1<<(2*1));


    /*
    //set PA10 to push/pull (X-Pls)
    GPIOA->OTYPER &= ~(3<<(2*10));                                                          //Clears PA10 bit
    //PA11; X-DIR
    GPIOA->OTYPER &= ~(3<<(2*11));
    //PA12; X-EN
    GPIOA->OTYPER &= ~(3<<(2*12));

    //set PC9 to output (Y-Pls)
    GPIOC->OTYPER &= ~(3<<(2*9));
    //PA8; Y-DIR
    GPIOA->OTYPER &= ~(3<<(2*8));
    //PA9; Y-EN
    GPIOA->OTYPER &= ~(3<<(2*9));

    //set PC6 to output (Z-Pls)
    GPIOC->OTYPER &= ~(3<<(2*6));
    //PC7; Z-DIR
    GPIOC->OTYPER &= ~(3<<(2*7));
    //PC8; Z-EN
    GPIOC->OTYPER &= ~(3<<(2*8));

    //set PB2 to output (A-Pls)
    GPIOB->OTYPER &= ~(3<<(2*2));
    //PB10; A-DIR
    GPIOB->OTYPER &= ~(3<<(2*10));
    //PB11; A-EN
    GPIOB->OTYPER &= ~(3<<(2*11));

    //set PC5 to output (B-Pls)
    GPIOC->OTYPER &= ~(3<<(2*5));
    //PB0; B-DIR
    GPIOB->OTYPER &= ~(3<<(2*0));
    //PB1; B-EN
    GPIOB->OTYPER &= ~(3<<(2*1));

    //set PA10 to pull-up/pull-down (X-Pls)
    GPIOA->PUPDR &= ~(3<<(2*10));                                               //Clears PA10 bit
    //PA11; X-DIR
    GPIOA->PUPDR &= ~(3<<(2*11));
    //PA12; X-EN
    GPIOA->PUPDR &= ~(3<<(2*12));

    //set PC9 to output (Y-Pls)
    GPIOC->PUPDR &= ~(3<<(2*9));
    //PA8; Y-DIR
    GPIOA->PUPDR &= ~(3<<(2*8));
    //PA9; Y-EN
    GPIOA->PUPDR &= ~(3<<(2*9));

    //set PC6 to output (Z-Pls)
    GPIOC->PUPDR &= ~(3<<(2*6));
    //PC7; Z-DIR
    GPIOC->PUPDR &= ~(3<<(2*7));
    //PC8; Z-EN
    GPIOC->PUPDR &= ~(3<<(2*8));

    //set PB2 to output (A-Pls)
    GPIOB->PUPDR &= ~(3<<(2*2));
    //PB10; A-DIR
    GPIOB->PUPDR &= ~(3<<(2*10));
    //PB11; A-EN
    GPIOB->PUPDR &= ~(3<<(2*11));

    //set PC5 to output (B-Pls)
    GPIOC->PUPDR &= ~(3<<(2*5));
    //PB0; B-DIR
    GPIOB->PUPDR &= ~(3<<(2*0));
    //PB1; B-EN
    GPIOB->PUPDR &= ~(3<<(2*1));

*/
}

void setupMotor(void)            //possibly change
{
    moveX = (fabs(actualPosition[0] - commandedPosition[0])) * stepsPerMM[0];
    moveY = (fabs(actualPosition[1] - commandedPosition[1])) * stepsPerMM[1];
    moveZ = (fabs(actualPosition[2] - commandedPosition[2])) * stepsPerMM[2];
    moveA = (fabs(actualPosition[3] - commandedPosition[3])) * stepsPerMM[3];
    moveB = (fabs(actualPosition[4] - commandedPosition[4])) * stepsPerMM[4];

    updateMovement[0] = 1 / stepsPerMM[0];
    updateMovement[1] = 1 / stepsPerMM[1];
    updateMovement[2] = 1 / stepsPerMM[2];
    updateMovement[3] = 1 / stepsPerMM[3];
    updateMovement[4] = 1 / stepsPerMM[4];

    steps[0] = 0;
    steps[1] = 0;
    steps[2] = 0;
    steps[3] = 0;
    steps[4] = 0;

    GPIOA->BSRR |= (1<<12);                             //Enables X driver
    GPIOA->BSRR |= (1<<9);                              //Enables Y driver
    GPIOC->BSRR |= (1<<8);                              //Enables Z driver
    GPIOB->BSRR |= (1<<11);                             //Enables A driver
    GPIOB->BSRR |= (1<<1);                              //Enables B driver

    if(actualPosition[0] > commandedPosition[0])        //Checks direction, move backwards
    {
        GPIOA->BSRR |= (1<<11);                         //Should set BS11 to 1 for X-DIR
        updateMovement[0] = -updateMovement[0];                                 //Might need to move to else
    }
    else                                                //Move forwards
    {
        GPIOA->BRR |= (1<<11);
    }

    if(actualPosition[1] > commandedPosition[1])
    {
        GPIOA->BSRR |= (1<<8);                          //Should set BS8 to 1 for Y-DIR
        updateMovement[1] = -updateMovement[1];
    }
    else                                                //Move forwards
    {
        GPIOA->BRR |= (1<<8);
    }

    if(actualPosition[2] > commandedPosition[2])
    {
        GPIOC->BSRR |= (1<<7);                          //Should set BS7 to 1 for Z-DIR
        updateMovement[2] = -updateMovement[2];
    }
    else                                                //Move forwards
    {
        GPIOC->BRR |= (1<<7);
    }

    if(actualPosition[3] > commandedPosition[3])
    {
        GPIOB->BSRR |= (1<<10);                         //Should set BS10 to 1 for A-DIR
        updateMovement[3] = -updateMovement[3];
    }
    else                                                //Move forwards
    {
        GPIOB->BRR |= (1<<10);
    }

    if(actualPosition[4] > commandedPosition[4])
    {
        GPIOB->BSRR |= (1<<0);                          //Should set BS0 to 1 for B-DIR
        updateMovement[4] = -updateMovement[4];
    }
    else                                                //Move forwards
    {
        GPIOB->BRR |= (1<<0);
    }

    TIM3->CR1 |= TIM_CR1_CEN;
}



void TIM3_IRQHandler(void)
{
    TIM3->SR &= ~0x1;
    advance_Motor();

}

void advance_Motor(void)                                             //check on how to pass arrays, might need to include zFlag
{
    if(steps[0] < (2 * moveX))                                                          //Checks if actualX is different than commandedX
    {
        steps[0]++;
        actualPosition[0] = actualPosition[0] + 0.5*updateMovement[0];                  //Updates actualPosition[0]

        int bitcheck;
        bitcheck = (GPIOA->ODR >> 10) & 1;                                              //Finds the value of the x-pulse bit.
        GPIOA->ODR ^= (~bitcheck ^ (GPIOA->ODR >> 10)) & (1 << 10);             //check, Should check if the bitvalue for x-pulse is either a 1 or 0 and flips it.
        //Two lines above came from Vitorbnc via https://gist.github.com/Vitorbnc/e35f1ff1485d660edf365241dacfa387

                                                                                //MENTION IN DESIGN DOC ALGORITHM!!

        //possibly change above line to if-statement
    }
    if(steps[1] < (2 * moveY))                                                          //Checks if actualY is different than commandedY
    {
        steps[1]++;
        actualPosition[1] = actualPosition[1] + 0.5*updateMovement[1];                  //Updates actualPosition[1]

        int bitcheck;
        bitcheck = (GPIOC->ODR >> 9) & 1;                                               //Finds the value of the y-pulse bit.
        GPIOC->ODR ^= (~bitcheck ^ (GPIOC->ODR >> 9)) & (1 << 9);               //Should check if the bitvalue for y-pulse is either a 1 or 0 and flips it.
        //Two lines above came from Vitorbnc via https://gist.github.com/Vitorbnc/e35f1ff1485d660edf365241dacfa387
    }
    if(steps[2] < (2 * moveZ))                                                          //Checks if actualZ is different than commandedZ
    {
        steps[2]++;
        actualPosition[2] = actualPosition[2] + 0.5*updateMovement[2];                  //Updates actualPosition[2]

        int bitcheck;
        bitcheck = (GPIOC->ODR >> 6) & 1;                                               //Finds the value of the z-pulse bit.
        GPIOC->ODR ^= (~bitcheck ^ (GPIOC->ODR >> 6)) & (1 << 6);                       //Should check if the bitvalue for z-pulse is either a 1 or 0 and flips it.
        //Two lines above came from Vitorbnc via https://gist.github.com/Vitorbnc/e35f1ff1485d660edf365241dacfa387

        if(steps[2] >= (2 * moveZ))                                             //Checks if zFlag is reached. If reached, sets zFlag so next line of G-Code can be called.
        {
            zFlag = true;                                                       //CHECK, MIGHT MOVE STEPS[] = 0 HERE
        }

    }
    if(steps[3] < (2 * moveA))                                                          //Checks if actualA is different than commandedA
    {
        steps[3]++;
        actualPosition[3] = actualPosition[3] + 0.5*updateMovement[3];                               //Updates actualPosition[3]

        int bitcheck;
        bitcheck = (GPIOB->ODR >> 2) & 1;                                               //Finds the value of the A-pulse bit.
        GPIOB->ODR ^= (~bitcheck ^ (GPIOB->ODR >> 2)) & (1 << 2);               //Should check if the bitvalue for A-pulse is either a 1 or 0 and flips it.
        //Two lines above came from Vitorbnc via https://gist.github.com/Vitorbnc/e35f1ff1485d660edf365241dacfa387
    }
    if(steps[4] < (2 * moveB))                                                          //Checks if actualB is different than commandedB
    {
        steps[4]++;
        actualPosition[4] = actualPosition[4] + 0.5*updateMovement[4];                               //Updates actualPosition[4]

        int bitcheck;
        bitcheck = (GPIOC->ODR >> 5) & 1;                                               //Finds the value of the x-pulse bit.
        GPIOC->ODR ^= (~bitcheck ^ (GPIOC->ODR >> 5)) & (1 << 5);               //Should check if the bitvalue for x-pulse is either a 1 or 0 and flips it.
        //Two lines above came from Vitorbnc via https://gist.github.com/Vitorbnc/e35f1ff1485d660edf365241dacfa387
    }
}

//Currently not using a pushbutton interrupt. Might code later.
// Disable timer 3. Currently waits for a keyboard input, needs to wait for next instructions
/*
void freeze(void)                                                   //Possibly use to stop timer3 if z-flag is reached, link to push buttons
{
    TIM3->CR1 &= ~TIM_CR1_CEN;
    //while(!available())                                             //possibly use external interrupt (EXTI) (part of GPIO)
      //  ;                                                           //Need to find a way to interrupt using z-flag
    //getchar();
    TIM3->CR1 |= TIM_CR1_CEN;

                                                                    //Might need to use TIE for push button in tim3 setup (instead of UIE), check DIER
}
*/


/*
// An array of "voices".  Each voice can be used to play a different note.
// Each voice can be associated with a channel (explained later).
// Each voice has a step size and an offset into the wave table.
struct {
        uint8_t in_use;
        uint8_t note;
        uint8_t chan;
        uint8_t volume;
        int     step;
        int     offset;
        int     last_sample;
        int16_t *wavetable;
    } voice[15];
  */

/*
void init_TIM2(int n)
{
    // Enable timer 2 here.
    // Make it generate an interrupt n times per second.
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 47;
    TIM2->ARR = n-1;
    TIM2->DIER |= TIM_DIER_UIE;

    TIM2->CR1 |= TIM_CR1_ARPE;                      //check location

    TIM2->CR1 |= TIM_CR1_CEN;
    NVIC->ISER[0] = (1<<TIM2_IRQn);

    NVIC_SetPriority(TIM2_IRQn,1);

}
*/

/*
void TIM2_IRQHandler(void)
{
    // TODO: remember to acknowledge the interrupt here!
    TIM2->SR &= ~0x1;
    midi_play();
}
*/

/*
int main(void)
{
    setup_timer3();


    for(;;)                                     //Possibly call timer interrupt here
    {
        if (mp->nexttick >= MAXTICKS)           //need to change, not sure what goes here
                   mp = midi_init(midifile);
          asm("wfi");
    }


      return 0;



}
*/
