#ifndef SD_H
#define SD_H

#include "spi.h"
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "global.h"

#define SPI_DELAY 1000


void nano_wait(unsigned int);
uint8_t sd_initialize(void);
int sd_ready(void);
uint8_t sd_command(unsigned char, unsigned long);
uint8_t sd_acommand(unsigned char, unsigned long);
//uint8_t wait_start_block(void);
void fat16_seek(unsigned long offset);
char fat16_read(unsigned char bytes);

#define FAILED 1;
#define PASSED 0;


// SD card commands
/** GO_IDLE_STATE - init card in spi mode if CS low */
uint8_t static const CMD0 = 0x00;
/** SEND_IF_COND - verify SD Memory Card interface operating condition.*/
uint8_t static const CMD8 = 0x08;
/** SEND_CSD - read the Card Specific Data (CSD register) */
uint8_t static const CMD9 = 0x09;
/** SEND_CID - read the card identification information (CID register) */
uint8_t static const CMD10 = 0x0A;
/** SEND_STATUS - read the card status register */
uint8_t static const CMD13 = 0x0D;
/** READ_BLOCK - read a single data block from the card */
uint8_t static const CMD17 = 0x11;
/** WRITE_BLOCK - write a single data block to the card */
uint8_t static const CMD24 = 0x18;
/** WRITE_MULTIPLE_BLOCK - write blocks of data until a STOP_TRANSMISSION */
uint8_t static const CMD25 = 0x19;
/** ERASE_WR_BLK_START - sets the address of the first block to be erased */
uint8_t static const CMD32 = 0x20;
/** ERASE_WR_BLK_END - sets the address of the last block of the continuous
    range to be erased*/
uint8_t static const CMD33 = 0x21;
/** ERASE - erase all previously selected blocks */
uint8_t static const CMD38 = 0x26;
/** APP_CMD - escape for application specific command */
uint8_t static const CMD55 = 0x37;
/** READ_OCR - read the OCR register of a card */
uint8_t static const CMD58 = 0x3A;
/** SET_WR_BLK_ERASE_COUNT - Set the number of write blocks to be
     pre-erased before writing */
uint8_t static const ACMD23 = 0x17;
/** SD_SEND_OP_COMD - Sends host capacity support information and
    activates the card's initialization process */
uint8_t static const ACMD41 = 0x29;

/** status for card in the ready state */
uint8_t static const R1_READY_STATE = 0x00;
/** status for card in the idle state */
uint8_t static const R1_IDLE_STATE = 0x01;
/** status bit for illegal command */
uint8_t static const R1_ILLEGAL_COMMAND = 0x04;
/** start data token for read or write single block*/
uint8_t static const DATA_START_BLOCK = 0xFE;
/** stop token for write multiple blocks*/
uint8_t static const STOP_TRAN_TOKEN = 0xFD;
/** start data token for write multiple blocks*/
uint8_t static const WRITE_MULTIPLE_TOKEN = 0xFC;
/** mask for data response tokens after a write block operation */
uint8_t static const DATA_RES_MASK = 0x1F;
/** write data accepted token */
uint8_t static const DATA_RES_ACCEPTED = 0x05;


uint8_t status_;
uint8_t sd_type;
uint8_t inBlock_;
uint8_t offset_;
uint8_t partialBlockRead_;


#endif
