#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "flash.h"

void setupPVD()
{
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;  //Enable clock to power control
   /*
    *             @arg PWR_PVDLevel_0: PVD detection level set to 1.9V
    *             @arg PWR_PVDLevel_1: PVD detection level set to 2.1V
    *             @arg PWR_PVDLevel_2: PVD detection level set to 2.3V
    *             @arg PWR_PVDLevel_3: PVD detection level set to 2.5V
    *             @arg PWR_PVDLevel_4: PVD detection level set to 2.7V
    *             @arg PWR_PVDLevel_5: PVD detection level set to 2.9V
    *             @arg PWR_PVDLevel_6: PVD detection level set to 3.1V
    *             @arg PWR_PVDLevel_7: PVD detection level set to 3.3V
    */
    PWR->CR |= PWR_CR_PLS_LEV4; //Set threshold level
    PWR->CR |= PWR_CR_PVDE;

    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    EXTI->IMR |= EXTI_IMR_MR16;
    EXTI->RTSR |= EXTI_RTSR_TR16;   //Enable rising edge interrupt for EXTI 16: PVD

    NVIC->ISER[0] = 1<<PVD_IRQn;
    NVIC_SetPriority(PVD_IRQn,0);   //Set to highest priority
}

void PVD_IRQHandler()
{
    GPIOC->BSRR |= 1<<9;
    //flashWritePosition();
}

void EXTI16_IRQHandler()
{
    //GPIOC->BSRR |= 1<<9;
}

int flashWritePosition()
{
    int memory = 0x0800FC00;    //Starting location in memory

    FLASH_Unlock();
    FLASH_ErasePage(memory);

    FLASH_ProgramWord(memory + 0 , actualXWhole);
    FLASH_ProgramWord(memory + 4, actualXDec);
    FLASH_ProgramWord(memory + 8, actualYWhole);
    FLASH_ProgramWord(memory + 12, actualYDec);
    FLASH_ProgramWord(memory + 16, actualZWhole);
    FLASH_ProgramWord(memory + 20, actualZDec);
    FLASH_ProgramWord(memory + 24, actualAWhole);
    FLASH_ProgramWord(memory + 28, actualADec);
    FLASH_ProgramWord(memory + 32, actualBWhole);
    FLASH_ProgramWord(memory + 36, actualBDec);
    FLASH_Lock();

    return 0;
}

int flashReadPosition()
{
    int memory = 0x0800FC00;    //Starting location in memory

    FLASH_Unlock();

    int *mem = (int *)memory;

    actualXWhole = mem[0];
    actualXDec = mem[1];
    actualYWhole = mem[2];
    actualYDec = mem[3];
    actualZWhole = mem[4];
    actualZDec = mem[5];
    actualAWhole = mem[6];
    actualADec = mem[7];
    actualBWhole = mem[8];
    actualBDec = mem[9];
    FLASH_Lock();

    return 0;
}
