/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-September-2020
  * @brief   3D Bender main program
  ******************************************************************************
*/

#include <motor.h>
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "string.h"

#include "global.h"
#include "usart.h"
#include "spi.h"
#include "ff.h"
#include "fat16.h"
#include "motor.h"
#include "gcode.h"
#include "flash.h"


int main(void)
{
	initBendCycleFlag = false;
	startBendCycle = false;
	inBendCycle = false;

	bool isEndLine;

	button1Flag = false;
	button2Flag = false;

    setupTimer3();
    setupGPIO();

    setupTimer15();
    setupTimer16();
    setupButtons();

    usart1Init(115200);         //Enable USART1 at 115200 baud
    usart1DMAInit();            //Initialize DMA for USART1
    softwareTimerInit();        //Start the timer to send data over USART

    //Set the default positions and parameters
    actualPosition[0] = 0.000;
    actualPosition[1] = 0.000;
    actualPosition[2] = 0.000;
    actualPosition[3] = 0.000;
    actualPosition[4] = 0.000;

    stepsPerMM[0] = 500;
    stepsPerMM[1] = 500;
    stepsPerMM[2] = 200;
    stepsPerMM[3] = 55.556;
    stepsPerMM[4] = 55.556;

    /*

    commandedPosition[0] = 30.000;
    commandedPosition[1] = -35.000;
    commandedPosition[2] = 80.000;
    commandedPosition[3] = 45.000;
    commandedPosition[4] = -30.000;

    setupMotor(false);

     */

    setupPVD();

    //int memory = 0x0800FC00;    //Starting location in memory

    //FLASH_Unlock();
    //FLASH_ErasePage(memory);
    //FLASH_Lock();

/*
    actualXWhole = 50;
    actualXDec = 123;
    actualYWhole = 60;
    actualYDec = 456;
    actualZWhole = -20;
    actualZDec = 105;
    actualAWhole = -100;
    actualADec = 967;
    actualBWhole = -2;
    actualBDec = 167;
*/
    //flashWritePosition();
    flashReadPosition();

    while(1)
    {
        //Various flags can call things in the main loop

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

    		if (progSource)     //If the program source is the SD card then proceed
    		{
    		    isEndLine = readGCodeLine();
    		    if (isEndLine)
    		    {
    		        inBendCycle = false;
    		        break;
    		    }
    		}
            else    //If the program source is USB then instead write the nextLine to usb
                usart1Send("nextLine\n");

    	}


    	if ((zFlag == true) && inBendCycle)
    	{
    		zFlag = false;

            if (progSource)     //If the program source is the SD card then proceed
            {
                isEndLine = readGCodeLine();
                if (isEndLine)
                {
                    inBendCycle = false;
                    break;
                }
            }
            else
                usart1Send("zFlag\n"); //Send the zflag over serial

    	}


    	if (button1Flag == true)
    	{
    	    button1Flag = false;    //Button 1 starts the bender
    	    progSource = true;

    	    if (inBendCycle == false)       //Only initiailize the bender if it isn't already started
    	        initBendCycleFlag = true;
    	}


        if (button2Flag == true)
        {
            button2Flag = false;    //Button 2 stops the bender
            M00();  //To stop the bender, simply call the M00 function
        }


        if (rxFlag == true)
        {
            rxFlag = false; //Reset the flag
            CheckGCodeLine(command, false);
        }


        if (txFlag == true)
        {
            //First build up the string to send
            //Since the STM32 is dumb we can't use sprintf with floats/doubles so we convert to ints

            //Actual positions
            actualXWhole = (int) actualPosition[0];
            actualXDec = (int) fabs((actualPosition[0] - actualXWhole) * 1000);

            actualYWhole = (int) actualPosition[1];
            actualYDec = (int) fabs((actualPosition[1] - actualYWhole) * 1000);

            actualZWhole = (int) actualPosition[2];
            actualZDec = (int) fabs((actualPosition[2] - actualZWhole) * 1000);

            actualAWhole = (int) actualPosition[3];
            actualADec = (int) fabs((actualPosition[3] - actualAWhole) * 1000);

            actualBWhole = (int) actualPosition[4];
            actualBDec = (int) fabs((actualPosition[4] - actualBWhole) * 1000);

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






