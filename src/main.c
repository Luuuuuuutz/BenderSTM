/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-September-2020
  * @brief   3D Bender main program
  ******************************************************************************
*/

#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "string.h"

#include "global.h"
#include "usart.h"
#include "motorSubsystemCalls.h"
#include "spi.h"
#include "ff.h"
#include "fat16.h"

#include "gcode.h"


int main(void)
{
	initBendCycleFlag = false;
	startBendCycle = false;
	inBendCycle = false;

	bool isEndLine;

    setupTimer3();
    setupGPIO();

/*

    actualPosition[0] = 0.000;
    actualPosition[1] = 0.000;
    actualPosition[2] = 0.000;
    actualPosition[3] = 0.000;
    actualPosition[4] = 0.000;

    commandedPosition[0] = 0.000;
    commandedPosition[1] = 0.000;
    commandedPosition[2] = -200.000;
    commandedPosition[3] = 0.000;
    commandedPosition[4] = 0.000;

    stepsPerMM[0] = 500;
    stepsPerMM[1] = 500;
    stepsPerMM[2] = 200;
    stepsPerMM[3] = 55.556;
    stepsPerMM[4] = 55.556;

    setupMotor();

*/

    usart1Init(115200);         //Enable USART1 at 115200 baud
    usart1DMAInit();            //Initialize DMA for USART1
    softwareTimerInit();        //Start the timer to send data over USART

    while(1)
    {
    	if (initBendCycleFlag == true)
    	{
    		initBendCycleFlag = false;

    		inBendCycle = true;

    		//First open the file
    	    openFile();
    	    isEndLine = readGCodeLine();	//Then read the first line to begin the process

    	    //We have to find a way to read the first few lines to set up the bender before
    	    //starting the bend cycle

    	}

    	if ((readNextLineFlag == true) && inBendCycle)
    	{
    		readNextLineFlag = false;
    		isEndLine = readGCodeLine();
    		if (isEndLine)
    		{
    			inBendCycle = false;
    			break;
    		}
    	}

    	if ((zFlag == true) && inBendCycle)
    	{
    		zFlag = false;

    		isEndLine = readGCodeLine();

    		if (isEndLine)
    		{
    			inBendCycle = false;
    			break;
    		}

    		/*
    		int timeout = 0;


    		while(nextLineReady == false)
    		{
    			timeout++;
				if (timeout > 10000)
				{
					break;
				}
    		}

			commandedPosition[0] = nextCommandedPosition[0];
			commandedPosition[1] = nextCommandedPosition[1];
			commandedPosition[2] = nextCommandedPosition[2];
			commandedPosition[3] = nextCommandedPosition[3];
			commandedPosition[4] = nextCommandedPosition[4];


    		//Read the next line of G-Code then call the motor subsystem
    		isEndLine = readGCodeLine();
    		*/

    	}

        if (rxFlag == true)
        {
            rxFlag = false; //Reset the flag
            //Do work
            //Call Vidya's function to interpret G Code
            CheckGCodeLine(messageRX, false);
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






