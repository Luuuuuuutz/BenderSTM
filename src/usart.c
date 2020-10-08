#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "string.h"
#include "usart.h"

void usart1Init(int baud)
{
    //ENABLE GPIO
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    GPIOB->MODER &= ~(3<<(6*2));        //Set PB6 to alternate function mode
    GPIOB->MODER |= 2<<(6*2);

    GPIOB->MODER &= ~(3<<(7*2));        //Set PB7 to alternate function mode
    GPIOB->MODER |= 2<<(7*2);

    GPIOB->AFR[0] &= ~(0xf << (4*6));   //Set alternate function to USART1 TX
    GPIOB->AFR[0] |= 0x0 << (4*6);

    GPIOB->AFR[0] &= ~(0xf << (4*7));   //Set alternate function to USART1 RX
    GPIOB->AFR[0] |= 0x0 << (4*7);

    //Enable USART1
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    USART1->CR1 &= ~USART_CR1_UE;           //Disable USART1
    USART1->CR1 &= ~USART_CR1_M;            //Word length to be 8 bits, 1 start, 1 stop, no parity
    USART1->CR1 &= ~(USART_CR1_M << 16);    //Clear both M bits
    USART1->CR1 &= ~USART_CR1_OVER8;        //Oversample by x16
    USART1->CR1 &= ~USART_CR1_RXNEIE;       //Enable receiver interrupt
    USART1->CR1 |= USART_CR1_RXNEIE;
    USART1->CR1 |= USART_CR1_RTOIE;         //Enable receiver timeout interrupt
    USART1->CR1 |= USART_CR1_TE;            //Enable TX
    USART1->CR1 |= USART_CR1_RE;            //Enable RX

    USART1->CR2 |= USART_CR2_RTOEN;         //Enable receiver timeout
    USART1->CR2 |= 0x0a << 24;              //Character match '\n' newline
    USART1->CR2 &= ~USART_CR2_CPOL;         //Clock polarity
    USART1->CR2 &= ~USART_CR2_CPHA;         //Clock phase

    USART1->BRR = (48000000+1)/(baud-1);    //Set baud rate

    USART1->RTOR |= USART_RTOR_RTO & 2;     //Receiver timeout limit

    USART1->CR1 |= USART_CR1_UE;            //Enable USART1

    NVIC->ISER[0] = 1<<USART1_IRQn;         //Enable interrupts for USART1

    while((USART1->ISR & USART_ISR_TEACK) == 0);    //Wait for TX & RX to be ready
    while((USART1->ISR & USART_ISR_REACK) == 0);

}

void usart1DMAInit(char* memory)
{

    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    /*
    //DMA Channel3 for RX
    DMA1_Channel3->CCR &= ~DMA_CCR_EN;                  //Disable DMA
    //DMA1_Channel3->CMAR = (uint32_t) memory;            //Memory location to store rx message
    DMA1_Channel3->CPAR = (uint32_t) (&(USART1->RDR));  //Peripheral location
    //DMA1_Channel3->CNDTR = RXLENGTH;                    //Amount of data to transfer
    DMA1_Channel3->CCR &= ~DMA_CCR_DIR;                 //Direction is peripheral to memory
    DMA1_Channel3->CCR &= ~DMA_CCR_MSIZE;               //Reset to 8 bit
    DMA1_Channel3->CCR &= ~DMA_CCR_PSIZE;               //Reset to 8 bit
    DMA1_Channel3->CCR |= DMA_CCR_MINC;                 //Memory increment mode
    DMA1_Channel3->CCR &= ~DMA_CCR_PINC;                //Disable peripheral increment mode
    DMA1_Channel3->CCR &= ~DMA_CCR_CIRC;                 //Enable circular mode
    DMA1_Channel3->CCR |= DMA_CCR_TCIE;                 //Enable transfer complete interrupt
    //DMA1_Channel3->CCR |= DMA_CCR_EN;                   //Enable DMA Channel
    */

    //DMA Channel2 for TX
    DMA1_Channel2->CCR &= ~DMA_CCR_EN;      //Disable DMA
    DMA1_Channel2->CCR |= DMA_CCR_DIR;      //Read from memory
    DMA1_Channel2->CCR &= ~DMA_CCR_MSIZE;   //Reset to 8 bit
    DMA1_Channel2->CCR &= ~DMA_CCR_PSIZE;   //Reset to 8 bit
    DMA1_Channel2->CCR |= DMA_CCR_MINC;     //Memory increment mode
    DMA1_Channel2->CCR &= ~DMA_CCR_PINC;    //Disable peripheral increment mode
    DMA1_Channel2->CCR &= ~DMA_CCR_CIRC;    //Disable circular mode
    DMA1_Channel2->CCR &= ~DMA_CCR_MEM2MEM; //Memory to peripheral
    DMA1_Channel2->CCR &= ~DMA_CCR_PL;      //Low priority

    DMA1_Channel2->CCR |= DMA_CCR_TEIE | DMA_CCR_TCIE;  //Enable interrupts

    USART1->CR3 |= USART_CR3_DMAT;          //Enable USART DMA for transmit

    NVIC->ISER[0] = 1<<DMA1_Channel2_3_IRQn;  //ENABLE INTERRUPTS FOR CHANNELS 2 & 3
}

void DMA1_Channel2_3_IRQHandler(void)
{

    //CHANNEL 2 (TX)
    if (DMA1->ISR & DMA_ISR_GIF2)
        DMA1->IFCR |= DMA_IFCR_CGIF2;       //Clear global interrupt

    if (DMA1->ISR & DMA_ISR_TCIF2)
        DMA1->IFCR |= DMA_IFCR_CTCIF2;      //Clear transfer complete interrupt

    if (DMA1->ISR & DMA_ISR_HTIF2)
        DMA1->IFCR |= DMA_IFCR_CHTIF2;      //Clear half transfer complete interrupt

    if (DMA1->ISR & DMA_ISR_TEIF2)
        DMA1->IFCR |= DMA_IFCR_CTEIF2;      //Clear transfer error interrupt


    /*
    //Channel 3 (RX)
    if (DMA1->ISR & DMA_ISR_TCIF3)
    {
        DMA1->IFCR |= DMA_IFCR_CTCIF3;      //Clear transfer complete interrupt

        DMA1_Channel3->CCR &= ~DMA_CCR_EN;      //Disable DMA Channel3
        memset(command, 0, 50);
        strcpy(command, messageRX);     //Copy received message into command char array
        rxFlag = true;                  //Set the flag to true

        USART1->CR3 &= ~USART_CR3_DMAR;
        USART1->RQR |= USART_RQR_RXFRQ;     //CLEAR INTERRUPT
        USART1->CR1 |= USART_CR1_RXNEIE;   //Re-enable the RXNE interrupt

    }
    */
}

void dmaRestart(char* memory, char length)
{
    //USART1->CR1 &= ~USART_CR1_UE;           //Disable USART1
    DMA1_Channel3->CCR &= ~DMA_CCR_EN;      //Disable DMA Channel3
    DMA1_Channel3->CMAR = (uint32_t) memory;            //Memory location to store rx message
    DMA1_Channel3->CPAR = (uint32_t) (&(USART1->RDR));  //Peripheral location
    DMA1_Channel3->CNDTR = length;                    //Amount of data to transfer
    //USART1->CR1 |= USART_CR1_UE;            //Enable USART1

    DMA1_Channel3->CCR |= DMA_CCR_EN;       //Enable DMA Channel 3

    USART1->CR3 |= USART_CR3_DMAR;  //Enable DMA receiver
}

void USART1_IRQHandler()
{

    if(USART1->ISR & USART_ISR_ORE)
        USART1->ICR |= USART_ICR_ORECF;     //Clear overrun interrupt

    if (USART1->ISR & USART_ISR_RXNE)      //RX register not empty interrupt
    {
        char c = (uint8_t)(USART1->RDR);
        messageRX[rx_index] = c;
        rx_index++;

        //For DMA mode
        /*
        char messageLength = (uint8_t)(USART1->RDR);

        if (messageLength > 0)
        {
            USART1->CR1 &= ~USART_CR1_RXNEIE;   //Disable the RXNE interrupt
            USART1->RQR |= USART_RQR_RXFRQ;     //CLEAR INTERRUPT
            dmaRestart(messageRX, messageLength);
        }
        */
    }

    if(USART1->ISR & USART_ISR_RTOF)    //RX line timeout interrupt
    {
        USART1->ICR |= USART_ICR_RTOCF;

        rx_index = 0;
        strcpy(command, messageRX);     //Copy received message into command array
        memset(messageRX, 0, RXLENGTH); //Clear memory

        rxFlag = true;

        //For DMA mode
        /*
        USART1->CR3 &= ~USART_CR3_DMAR;  //Disable DMA receiver
        memset(command, 0, 50);
        strcpy(command, messageRX);     //Copy received message into command char array
        memset(messageRX, 0, 50); //Clear messageRX
        rxFlag = true;                  //Set the flag to true

        dmaRestart(messageRX);                  //Restart DMA to prepare for next message to be received
        USART1->ICR |= USART_ICR_RTOCF;
    */
    }

}

void usart1Send(const char *buffer)
{
    uint32_t timeout = 0;

    while(DMA1->ISR & DMA_ISR_TCIF2);           //Wait for transmission buffer to be empty
    while(!(USART1->ISR & USART_ISR_TC))        //Wait for transmission complete
    {
        timeout++;
        if(timeout > 1000)
            return;
    }

    DMA1_Channel2->CCR &= ~DMA_CCR_EN;              //Disable DMA
    DMA1_Channel2->CMAR = (uint32_t) buffer;        //Set memory location to buffer char array
    DMA1_Channel2->CPAR = (uint32_t) &(USART1->TDR);//Set peripheral location to USART TX line
    DMA1_Channel2->CNDTR = strlen(buffer);          //Amount of data to send is length of char array
    DMA1_Channel2->CCR |= DMA_CCR_EN;               //Enable DMA to start transfer

    //Can probably move this to the beginning for more "efficient" code
    timeout = 0;
    while(!(USART1->ISR & USART_ISR_TC))        //Wait for the transmission to be complete
    {
        timeout++;
        if(timeout > 10000)
            return;
    }
    return;

}
