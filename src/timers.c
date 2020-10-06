#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "timers.h"

void softwareTimerInit()
{
    //Timer 14 for reporting back to computer software
    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;     //ENABLE TIM14

    TIM14->PSC = 4800 - 1;
    TIM14->ARR = 1000 - 1;       //50 interrupts per second (was 200)

    TIM14->DIER = TIM_DIER_UIE;  //Enable the interrupt

    TIM14->CR1 |= TIM_CR1_CEN;   //Start timer14

    NVIC->ISER[0] = 1<<TIM14_IRQn;
}

void TIM14_IRQHandler()
{
    txFlag = true;

    TIM14->SR = 0;          //CLEAR INTERRUPT
}
