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
#include "string.h"

#include "global.h"
#include "usart.h"
#include "spi.h"
#include "ff.h"
#include "fat16.h"

#include "gcode.h"


FATFS FatFs;
FIL Fil;

int main(void)
{

    usart1Init(115200);         //Enable USART1 at 115200 baud
    //usart1DMAInit(messageRX);   //Enable DMA for USART1 to capture first message

    //softwareTimerInit();        //Start the timer to send data over USART

    //spi_initialize();

    //&FatFs, Fil
    openFile();

    //&FatFs, Fil
    readGCodeLine();

    //TEST CODE MAIN BLA BLA

    actualPosition[0] = 14.123;
    actualPosition[1] = 67.456;
    actualPosition[2] = 87.789;
    actualPosition[3] = 9.234;
    actualPosition[4] = 19.567;

    while(1)
    {


    	if (zFlag == true)
    	{
    		//Read next file line and do Vidya stuff
    	}



        if (rxFlag == true)
        {
            rxFlag = false; //Reset the flag
            //Do work
            //Call Vidya's function to interpret G Code
        }

        if (txFlag == true)
        {
            //First build up the string to send
            //Since the STM32 is dumb we can't use sprintf with floats/doubles so we convert to ints

            //Actual positions
            int actualXWhole = (int) actualPosition[0];
            int actualXDec = (actualPosition[0] - actualXWhole) * 100;

            int actualYWhole = (int) actualPosition[1];
            int actualYDec = (actualPosition[1] - actualYWhole) * 100;

            int actualZWhole = (int) actualPosition[2];
            int actualZDec = (actualPosition[2] - actualZWhole) * 100;

            int actualAWhole = (int) actualPosition[3];
            int actualADec = (actualPosition[3] - actualAWhole) * 100;

            int actualBWhole = (int) actualPosition[4];
            int actualBDec = (actualPosition[4] - actualBWhole) * 100;

            //Commanded positions
            int commandedXWhole = (int) commandedPosition[0];
            int commandedXDec = (commandedPosition[0] - commandedXWhole) * 100;

            int commandedYWhole = (int) commandedPosition[1];
            int commandedYDec = (commandedPosition[1] - commandedYWhole) * 100;

            int commandedZWhole = (int) commandedPosition[2];
            int commandedZDec = (commandedPosition[2] - commandedZWhole) * 100;

            int commandedAWhole = (int) commandedPosition[3];
            int commandedADec = (commandedPosition[3] - commandedAWhole) * 100;

            int commandedBWhole = (int) commandedPosition[4];
            int commandedBDec = (commandedPosition[4] - commandedBWhole) * 100;

            char dataPacket[240];

            sprintf(dataPacket, "%3d.%03d,%3d.%03d,%3d.%03d,%3d.%03d,%3d.%03d;%3d.%03d,%3d.%03d,%3d.%03d,%3d.%03d,%3d.%03d\n",
                    actualXWhole, actualXDec, actualYWhole, actualYDec, actualZWhole, actualZDec, actualAWhole, actualADec,
                    actualBWhole, actualBDec,commandedXWhole, commandedXDec, commandedYWhole, commandedYDec, commandedZWhole,
                    commandedZDec, commandedAWhole, commandedADec, commandedBWhole, commandedBDec);

            usart1Send(dataPacket);

            txFlag = false; //Reset the flag

        }


    }

	for(;;);
}






