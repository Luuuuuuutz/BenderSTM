#include "sd.h"
#include "spi.h"
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}

uint8_t sd_initialize(){
	inBlock_ = offset_ = partialBlockRead_ = 0; //initialize variables
	int timeout = 0;    //timeout
	sd_ready();       //bit bang 150 clock cycles to start the card
    spi_initialize();         //initialize SPI1 settings

    while((status_=sd_command(CMD0, 0x00000000)) != R1_IDLE_STATE)	//cmd0 to go into idle state
    {
    	timeout++;
    	if(timeout > 100)   //up to 100 retries before failing
    		goto fail;
    }

    micro_wait(5);      //wait a moment before continuing

    sd_command(CMD8, 0x000001aa);	//cmd8 to check SD card version

    micro_wait(5);

    timeout = 0;
    while((status_ = sd_acommand(ACMD41, 0x00000000)) != R1_READY_STATE)	//send acmd41 to initialize sd card
    {
    	timeout++;
    	if(timeout > 1000)
    		goto fail;
    }

    return PASSED;

    fail:

	return FAILED;
}


int sd_ready(){
	const int NSS = 1<<4;
	const int SCK = 1<<5;
	const int MOSI = 1<<7;

	GPIOA->MODER &= ~(3<<(4*2));
	GPIOA->MODER |= 1<<(4*2);

	GPIOA->MODER &= ~(3<<(5*2));
	GPIOA->MODER |= 1<<(5*2);

	GPIOA->MODER &= ~(3<<(7*2));
	GPIOA->MODER |= 1<<(7*2);

	GPIOA->BSRR = NSS;
	GPIOA->BSRR = MOSI;

	for(int i = 0; i < 150; i++){
		GPIOA->BSRR = SCK;
		nano_wait(SPI_DELAY);
		GPIOA->BRR = SCK;
		nano_wait(SPI_DELAY);
	}

	return 1;
}

uint8_t sd_command(unsigned char command, unsigned long argument){
	uint8_t status;

	char crc = 0xff;
	if(command == CMD0)
		crc = 0x95;
	else if(command == CMD8)
		crc = 0x87;

	spi_write_read(0xff);

	spi_write_read(command | 0x40);
	spi_write_read(argument>>24);
	spi_write_read(argument>>16);
	spi_write_read(argument>>8);
	spi_write_read(argument);
	spi_write_read(crc);

	micro_wait(5);

	for(int i = 0;(((status = spi_write_read(0xff)) & 0xf0) && ( i != 8)); i++);

	micro_wait(5);

	return status;
}

uint8_t sd_acommand(unsigned char command, unsigned long argument){
	sd_command(CMD55,0);					//Definition of accomand is to send command 55 first
	return sd_command(command,argument);
}


