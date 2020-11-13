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

    usart1Init(115200);         //Enable USART1 at 115200 baud
    usart1DMAInit();            //Initialize DMA for USART1
    softwareTimerInit();        //Start the timer to send data over USART



    actualPosition[0] = 0.000;
    actualPosition[1] = 0.000;
    actualPosition[2] = 0.000;
    actualPosition[3] = 0.000;
    actualPosition[4] = 0.000;

    commandedPosition[0] = 30.000;
    commandedPosition[1] = -35.000;
    commandedPosition[2] = 80.000;
    commandedPosition[3] = 45.000;
    commandedPosition[4] = -30.000;

    stepsPerMM[0] = 500;
    stepsPerMM[1] = 500;
    stepsPerMM[2] = 200;
    stepsPerMM[3] = 55.556;
    stepsPerMM[4] = 55.556;

    setupMotor(false);



    while(1)
    {
    	if (initBendCycleFlag == true)
    	{
    		initBendCycleFlag = false;

    		inBendCycle = true;

    		//First open the file
    	    openFile();
    	    isEndLine = readGCodeLine();	//Then read the first line to begin the process

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

    	}

        if (rxFlag == true)
        {
            rxFlag = false; //Reset the flag
            //Do work
            //Call Vidya's function to interpret G Code
            CheckGCodeLine(command, false);
        }

        if (txFlag == true)
        {
            //First build up the string to send
            //Since the STM32 is dumb we can't use sprintf with floats/doubles so we convert to ints

            //Actual positions
            int actualXWhole = (int) actualPosition[0];
            int actualXDec = (int) fabs((actualPosition[0] - actualXWhole) * 1000);

            int actualYWhole = (int) actualPosition[1];
            int actualYDec = (int) fabs((actualPosition[1] - actualYWhole) * 1000);

            int actualZWhole = (int) actualPosition[2];
            int actualZDec = (int) fabs((actualPosition[2] - actualZWhole) * 1000);

            int actualAWhole = (int) actualPosition[3];
            int actualADec = (int) fabs((actualPosition[3] - actualAWhole) * 1000);

            int actualBWhole = (int) actualPosition[4];
            int actualBDec = (int) fabs((actualPosition[4] - actualBWhole) * 1000);

            //Commanded positions
            int commandedXWhole = (int) commandedPosition[0];
            int commandedXDec = (int) fabs((commandedPosition[0] - commandedXWhole) * 1000);

            int commandedYWhole = (int) commandedPosition[1];
            int commandedYDec = (int) fabs((commandedPosition[1] - commandedYWhole) * 1000);

            int commandedZWhole = (int) commandedPosition[2];
            int commandedZDec = (int) fabs((commandedPosition[2] - commandedZWhole) * 1000);

            int commandedAWhole = (int) commandedPosition[3];
            int commandedADec = (int) fabs((commandedPosition[3] - commandedAWhole) * 1000);

            int commandedBWhole = (int) commandedPosition[4];
            int commandedBDec = (int) fabs((commandedPosition[4] - commandedBWhole) * 1000);

            char dataPacket[240];

            sprintf(dataPacket, "%d.%03d,%d.%03d,%d.%03d,%d.%03d,%d.%03d;%d.%03d,%d.%03d,%d.%03d,%d.%03d,%d.%03d\n",
                    actualXWhole, actualXDec, actualYWhole, actualYDec, actualZWhole, actualZDec, actualAWhole, actualADec,
                    actualBWhole, actualBDec,commandedXWhole, commandedXDec, commandedYWhole, commandedYDec, commandedZWhole,
                    commandedZDec, commandedAWhole, commandedADec, commandedBWhole, commandedBDec);

            usart1Send(dataPacket);

            txFlag = false; //Reset the flag

        }


    }

	for(;;);
}






