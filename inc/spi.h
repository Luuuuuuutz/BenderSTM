#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "global.h"

#ifndef SPI_H
#define SPI_H

void spi_initialize(void);
void spi_speed(void);
char spi_write_read(char);

#endif
