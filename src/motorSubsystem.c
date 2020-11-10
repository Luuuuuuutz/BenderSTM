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
#include "motorSubsystemCalls.h"
#include "global.h"

//Global Variables:
double moveX;           	//Difference in x to move in steps
double moveY;           	//Difference in y to move in steps
double moveZ;           	//Difference in z to move in steps
double moveA;           	//Difference in a to move in steps
double moveB;          	 	//Difference in b to move in steps
int steps[5];           	//Array of number of current steps moved for each position
double updateMovement[5];   //Value of one step to update


// Enable clock to timer3. Using timer3 because of general purpose usage.
// Setup prescalar and arr so that the interrupt is triggered every 500 us.
// Enable the timer 3 interrupt.
// Start the timer.
void setupTimer3()
{
    // Enable timer 3 here.
    // Make it generate an interrupt 2000 times per second.
    // Set the prescaler to 48 - 1 to divide 48MHz clock by 48
    // counter goes from 0 ... 499
    // Possibly change to another timer.
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->PSC = 48 - 1;
    TIM3->ARR = 500 - 1;
    TIM3->DIER |= TIM_DIER_UIE;
    NVIC->ISER[0] = (1<<TIM3_IRQn);

    NVIC_SetPriority(TIM3_IRQn,0);                                              //check priority, might need to change if another timer is needed for the pushbutton
}

void setupGPIO()                                                               //possibly change name, check with Eric
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
    GPIOA->MODER &= ~(3<<(2*X_PLS));                                               //POSSIBLY CHANGE BACK USING ~ IN FRONT OF (3 <<)            //Clears PA10 bit
    GPIOA->MODER |= (1<<(2*X_PLS));                                                            //Sets PA10 bit to output

    //PA11; X-DIR
    GPIOA->MODER &= ~(3<<(2*X_DIR));
    GPIOA->MODER |= (1<<(2*X_DIR));

    //PA12; X-EN
    GPIOA->MODER &= ~(3<<(2*X_EN));
    GPIOA->MODER |= (1<<(2*X_EN));


    //set PC9 to output (Y-Pls)
    GPIOC->MODER &= ~(3<<(2*Y_PLS));
    GPIOC->MODER |= (1<<(2*Y_PLS));

    //PA8; Y-DIR
    GPIOA->MODER &= ~(3<<(2*Y_DIR));
    GPIOA->MODER |= (1<<(2*Y_DIR));

    //PA9; Y-EN
    GPIOA->MODER &= ~(3<<(2*Y_EN));
    GPIOA->MODER |= (1<<(2*Y_EN));

    //set PC6 to output (Z-Pls)
    GPIOC->MODER &= ~(3<<(2*Z_PLS));
    GPIOC->MODER |= (1<<(2*Z_PLS));

    //PC7; Z-DIR
    GPIOC->MODER &= ~(3<<(2*Z_DIR));
    GPIOC->MODER |= (1<<(2*Z_DIR));

    //PC8; Z-EN
    GPIOC->MODER &= ~(3<<(2*Z_EN));
    GPIOC->MODER |= (1<<(2*Z_EN));

    //set PB2 to output (A-Pls)
    GPIOB->MODER &= ~(3<<(2*A_PLS));
    GPIOB->MODER |= (1<<(2*A_PLS));

    //PB10; A-DIR
    GPIOB->MODER &= ~(3<<(2*A_DIR));
    GPIOB->MODER |= (1<<(2*A_DIR));

    //PB11; A-EN
    GPIOB->MODER &= ~(3<<(2*A_EN));
    GPIOB->MODER |= (1<<(2*A_EN));

    //set PC5 to output (B-Pls)
    GPIOC->MODER &= ~(3<<(2*B_PLS));
    GPIOC->MODER |= (1<<(2*B_PLS));

    //PB0; B-DIR
    GPIOB->MODER &= ~(3<<(2*B_DIR));
    GPIOB->MODER |= (1<<(2*B_DIR));

    //PB1; B-EN
    GPIOB->MODER &= ~(3<<(2*B_EN));
    GPIOB->MODER |= (1<<(2*B_EN));


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

void enableSteppers(bool x, bool y, bool z, bool a, bool b)
{
	if (x)
		GPIOA->BRR |= (1<<X_EN);                            	//Enables X driver
	if (y)
		GPIOA->BRR |= (1<<Y_EN);                              	//Enables Y driver
	if (z)
		GPIOC->BRR |= (1<<Z_EN);                              	//Enables Z driver
	if (a)
		GPIOB->BRR |= (1<<A_EN);                             	//Enables A driver
    if (b)
    	GPIOB->BRR |= (1<<B_EN);                              	//Enables B driver
}

void disableSteppers(bool x, bool y, bool z, bool a, bool b)
{
	if (x)
		GPIOA->BSRR |= (1<<X_EN);                            	//Disables X driver
	if (y)
		GPIOA->BSRR |= (1<<Y_EN);                              	//Disables Y driver
	if (z)
		GPIOC->BSRR |= (1<<Z_EN);                              	//Disables Z driver
	if (a)
		GPIOB->BSRR |= (1<<A_EN);                             	//Disables A driver
    if (b)
    	GPIOB->BSRR |= (1<<B_EN);                              	//Disables B driver
}

void setupMotor(void)            //possibly change
{
    //First disable the timer so it doesn't trigger while setting up the motor
	TIM3->CR1 &= ~TIM_CR1_CEN;

	//Compute the distance to move in steps
	moveX = (fabs(actualPosition[0] - commandedPosition[0])) * stepsPerMM[0];
    moveY = (fabs(actualPosition[1] - commandedPosition[1])) * stepsPerMM[1];
    moveZ = (fabs(actualPosition[2] - commandedPosition[2])) * stepsPerMM[2];
    moveA = (fabs(actualPosition[3] - commandedPosition[3])) * stepsPerMM[3];
    moveB = (fabs(actualPosition[4] - commandedPosition[4])) * stepsPerMM[4];

    updateMovement[0] = 0.5 / stepsPerMM[0];
    updateMovement[1] = 0.5 / stepsPerMM[1];
    updateMovement[2] = 0.5 / stepsPerMM[2];
    updateMovement[3] = 0.5 / stepsPerMM[3];
    updateMovement[4] = 0.5 / stepsPerMM[4];

    steps[0] = 0;
    steps[1] = 0;
    steps[2] = 0;
    steps[3] = 0;
    steps[4] = 0;


    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    if(actualPosition[0] > commandedPosition[0])        //Checks direction, move backwards
    {
        GPIOA->BSRR |= (1<<X_DIR);                         //Should set BS11 to 1 for X-DIR
        updateMovement[0] = -updateMovement[0];                                 //Might need to move to else
    }
    else                                                //Move forwards
    {
        GPIOA->BRR |= (1<<X_DIR);
    }

    //YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
    if(actualPosition[1] > commandedPosition[1])
    {
        GPIOA->BSRR |= (1<<Y_DIR);                          //Should set BS8 to 1 for Y-DIR
        updateMovement[1] = -updateMovement[1];
    }
    else                                                //Move forwards
    {
        GPIOA->BRR |= (1<<Y_DIR);
    }

    //ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
    if(actualPosition[2] > commandedPosition[2])
    {
        GPIOC->BRR |= (1<<Z_DIR);                          //Should set BS7 to 1 for Z-DIR
        updateMovement[2] = -updateMovement[2];
    }
    else                                                //Move forwards
    {
        GPIOC->BSRR |= (1<<Z_DIR);
    }

    //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    if(actualPosition[3] > commandedPosition[3])
    {
        GPIOB->BSRR |= (1<<A_DIR);                         //Should set BS10 to 1 for A-DIR
        updateMovement[3] = -updateMovement[3];
    }
    else                                                //Move forwards
    {
        GPIOB->BRR |= (1<<A_DIR);
    }

    //BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
    if(actualPosition[4] > commandedPosition[4])
    {
        GPIOB->BSRR |= (1<<B_DIR);                          //Should set BS0 to 1 for B-DIR
        updateMovement[4] = -updateMovement[4];
    }
    else                                                //Move forwards
    {
        GPIOB->BRR |= (1<<B_DIR);
    }

    TIM3->CR1 |= TIM_CR1_CEN;

}


void TIM3_IRQHandler(void)
{

    TIM3->SR = 0;
    advanceMotor();

}

void advanceMotor(void)                                             //check on how to pass arrays, might need to include zFlag
{
    if(steps[0] < (2 * moveX))                                                          //Checks if actualX is different than commandedX
    {
        steps[0]++;
        actualPosition[0] = actualPosition[0] + updateMovement[0];                  //Updates actualPosition[0]

        int bitcheck;
        bitcheck = (GPIOA->ODR >> X_PLS) & 1;                                              //Finds the value of the x-pulse bit.
        GPIOA->ODR ^= (~bitcheck ^ (GPIOA->ODR >> X_PLS)) & (1 << X_PLS);             //check, Should check if the bitvalue for x-pulse is either a 1 or 0 and flips it.
        //Two lines above came from Vitorbnc via https://gist.github.com/Vitorbnc/e35f1ff1485d660edf365241dacfa387

                                                                                //MENTION IN DESIGN DOC ALGORITHM!!

        //possibly change above line to if-statement
    }

    if(steps[1] < (2 * moveY))                                                          //Checks if actualY is different than commandedY
    {
        steps[1]++;
        actualPosition[1] = actualPosition[1] + updateMovement[1];                  //Updates actualPosition[1]

        int bitcheck;
        bitcheck = (GPIOC->ODR >> Y_PLS) & 1;                                               //Finds the value of the y-pulse bit.
        GPIOC->ODR ^= (~bitcheck ^ (GPIOC->ODR >> Y_PLS)) & (1 << Y_PLS);               //Should check if the bitvalue for y-pulse is either a 1 or 0 and flips it.
        //Two lines above came from Vitorbnc via https://gist.github.com/Vitorbnc/e35f1ff1485d660edf365241dacfa387
    }

    if(steps[2] < (2 * moveZ))                                                          //Checks if actualZ is different than commandedZ
    {
        steps[2]++;
        actualPosition[2] = actualPosition[2] + updateMovement[2];                  //Updates actualPosition[2]

        int bitcheck;
        bitcheck = (GPIOC->ODR >> Z_PLS) & 1;                                               //Finds the value of the z-pulse bit.
        GPIOC->ODR ^= (~bitcheck ^ (GPIOC->ODR >> Z_PLS)) & (1 << Z_PLS);                       //Should check if the bitvalue for z-pulse is either a 1 or 0 and flips it.
        //Two lines above came from Vitorbnc via https://gist.github.com/Vitorbnc/e35f1ff1485d660edf365241dacfa387

    }

    if(steps[2] >= (2 * moveZ))                                             //Checks if zFlag is reached. If reached, sets zFlag so next line of G-Code can be called.
    {
        zFlag = true;                                                       //CHECK, MIGHT MOVE STEPS[] = 0 HERE
    }

    if(steps[3] < (2 * moveA))                                                          //Checks if actualA is different than commandedA
    {
        steps[3]++;
        actualPosition[3] = actualPosition[3] + updateMovement[3];                               //Updates actualPosition[3]

        int bitcheck;
        bitcheck = (GPIOB->ODR >> A_PLS) & 1;                                               //Finds the value of the A-pulse bit.
        GPIOB->ODR ^= (~bitcheck ^ (GPIOB->ODR >> A_PLS)) & (1 << A_PLS);               //Should check if the bitvalue for A-pulse is either a 1 or 0 and flips it.
        //Two lines above came from Vitorbnc via https://gist.github.com/Vitorbnc/e35f1ff1485d660edf365241dacfa387
    }

    if(steps[4] < (2 * moveB))                                                          //Checks if actualB is different than commandedB
    {
        steps[4]++;
        actualPosition[4] = actualPosition[4] + updateMovement[4];                               //Updates actualPosition[4]

        int bitcheck;
        bitcheck = (GPIOC->ODR >> B_PLS) & 1;                                               //Finds the value of the x-pulse bit.
        GPIOC->ODR ^= (~bitcheck ^ (GPIOC->ODR >> B_PLS)) & (1 << B_PLS);               //Should check if the bitvalue for x-pulse is either a 1 or 0 and flips it.
        //Two lines above came from Vitorbnc via https://gist.github.com/Vitorbnc/e35f1ff1485d660edf365241dacfa387
    }
}


