#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "spi.h"

//WRITE COMMENTS
//SEE IF CAN WRITE A MACROS FILE

void spi_initialize()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	GPIOA->MODER &= ~(3<<(3*2));
	GPIOA->MODER |= 1<<(3*2);

	//PA4 - NSS
	GPIOA->MODER &= ~(3<<(4*2));
	GPIOA->MODER |= 2<<(4*2);

	//PA5 - SCK
	GPIOA->MODER &= ~(3<<(5*2));
	GPIOA->MODER |= 2<<(5*2);

	//PA6 - MISO
	GPIOA->MODER &= ~(3<<(6*2));
	GPIOA->MODER |= 2<<(6*2);

	//PA7 - MOSI
	GPIOA->MODER &= ~(3<<(7*2));
	GPIOA->MODER |= 2<<(7*2);

	//Alternate Function Reset
	GPIOA->AFR[0] &= ~(0xf << (4*4));
	GPIOA->AFR[0] &= ~(0xf << (4*5));
	GPIOA->AFR[0] &= ~(0xf << (4*6));
	GPIOA->AFR[0] &= ~(0xf << (4*7));

	//SPI System

	//Register 1
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	SPI1->CR1 &= ~SPI_CR1_BIDIMODE;
	SPI1->CR1 &= ~SPI_CR1_BIDIOE;
	SPI1->CR1 &= ~SPI_CR1_CRCEN;
	SPI1->CR1 &= ~SPI_CR1_CRCNEXT;
	SPI1->CR1 &= ~SPI_CR1_CRCL;
	SPI1->CR1 &= ~SPI_CR1_RXONLY;
	SPI1->CR1 &= ~SPI_CR1_SSM;
	SPI1->CR1 &= ~SPI_CR1_SSI;
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
	SPI1->CR1 &= ~SPI_CR1_BR;           //Reset Baud Rate
	SPI1->CR1 |= SPI_CR1_BR_1;			//Set Baud rate to fpclk/128
	SPI1->CR1 |= SPI_CR1_BR_2;
	SPI1->CR1 |= SPI_CR1_MSTR;
	SPI1->CR1 &= ~SPI_CR1_CPOL;
	SPI1->CR1 &= ~SPI_CR1_CPHA;

	//Register 2
	SPI1->CR2 |= SPI_CR2_FRXTH;
	SPI1->CR2 &= ~SPI_CR2_DS;
	SPI1->CR2 |= SPI_CR2_TXEIE;
	SPI1->CR2 |= SPI_CR2_RXNEIE;
	SPI1->CR2 &= ~SPI_CR2_ERRIE;
	SPI1->CR2 &= ~SPI_CR2_FRF;
	SPI1->CR2 |= SPI_CR2_NSSP;
	SPI1->CR2 &= ~SPI_CR2_SSOE;
	SPI1->CR2 &= ~SPI_CR2_TXDMAEN;
	SPI1->CR2 &= ~SPI_CR2_RXDMAEN;

	SPI1->CR1 |= SPI_CR1_SPE;
}

void spi_speed()
{
	SPI1->CR1 &= ~SPI_CR1_SPE;
	SPI1->CR1 &= ~SPI_CR1_BR;		//Reset Baud Rate
	SPI1->CR1 |= SPI_CR1_BR_1;		//Set Baud rate to 6MHz
	SPI1->CR1 |= SPI_CR1_SPE;
}

char spi_write_read(char data)
{
	int timeout = 0;
	while(!(SPI1->SR & SPI_SR_TXE))
	{
		timeout++;
		if(timeout > 1000)
		{
			return 0xff;
		}
	}

	*((uint8_t *) &(SPI1->DR)) = data;

	timeout = 0;

	//Reading Section
	while(((SPI1->SR &(SPI_SR_RXNE | SPI_SR_TXE)) == 0) || (SPI1->SR &SPI_SR_BSY))
	{
		timeout++;
		if(timeout > 1000)
		{
			return 0xff;
		}
	}

	return (uint8_t)SPI1->DR;

}
