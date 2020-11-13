#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "gcode.h"
#include "ff.h"
#include "fat16.h"
#include "sd.h"
#include "spi.h"

FATFS FatFs;
FIL Fil;

char X[10];
char Y[10];
char Z[10];
char A[10];
char B[10];

int firstTime = 0;


void openFile(char* fileName)
{
	FRESULT res;

	int result = sd_initialize();

	if (result || 0)
		return;

	res = f_mount(&FatFs, "", 0);

	res = f_open(&Fil, "simple.gc", FA_READ | FA_OPEN_EXISTING);

	//if (res != FR_OK)
		//return;

}

//FATFS* FatFs, FIL Fil
bool readGCodeLine()
{
    char line[100] = {'\0'};

    memset(X,0,10);
    memset(Y,0,10);
    memset(Z,0,10);
    memset(A,0,10);
    memset(B,0,10);

	UINT btr = sizeof line / sizeof line[0];

    if(f_eof(&Fil))
    {
    	return true;
    }
    else
    {
        f_gets(line, btr, &Fil);
        CheckGCodeLine(line, true);
    	return false;
    }
}

//Functions

void CheckGCodeLine(char* line, bool source)
{

	//Source boolean - True if function comes from SD Card, false if UART

	char* data_ptr;
    char* eptrX;
    char* eptrY;
   	char* eptrZ;
   	char* eptrA;
   	char* eptrB;
	char G[10] = {0};
    char M[10] = {0};

    double x_d = 0;
    double y_d = 0;
    double z_d = 0;
    double a_d = 0;
    double b_d = 0;



    int line_index = 0;
    int data_ptr_index = 0;

    while(line[line_index] != '\0')
    {

        if(isalpha(line[line_index]))
        {
            switch(line[line_index])
            {
                case 'G': data_ptr = G;
                		  break;
                case 'M': data_ptr = M;
                		  break;
                case 'X': data_ptr = X;
                		  break;
                case 'Y': data_ptr = Y;
                		  break;
                case 'Z': data_ptr = Z;
                		  break;
                case 'A': data_ptr = A;
                		  break;
                case 'B': data_ptr = B;
                		  break;
            }
            data_ptr_index = 0;
        }
        else if((isalpha(line[line_index]) == 0) || (line[line_index] == '.'))
        {
        	data_ptr[data_ptr_index] = line[line_index];
            data_ptr_index++;
        }

        line_index++;

        x_d = strtod(X,&eptrX);
        y_d = strtod(Y,&eptrY);
        z_d = strtod(Z,&eptrZ);
        a_d = strtod(A,&eptrA);
        b_d = strtod(B,&eptrB);
    }

    //Determine which G Code function to call

    if(strncmp(line,"G0",2) == 0)
    {
       G0(x_d, y_d, z_d, a_d, b_d, source);

    }

    else if((strncmp(line,"G01",3) == 0) || (strncmp(line,"G1",2) == 0))
    {
        G1(x_d, y_d, z_d, a_d, b_d, source);

    }

    else if(strncmp(line,"G20",3) == 0)
    {
       if (source)
    	   readNextLineFlag = true;
    }

    else if(strncmp(line,"G21",3) == 0)
    {
       if (source)
    	   readNextLineFlag = true;
    }

    else if(strncmp(line,"G28",3) == 0)
    {
        G28(homePosition, source);

    }

    else if(strncmp(line,"G28.1",5) == 0)
    {
       G281(x_d,y_d,z_d,a_d,b_d,homePosition);
       if (source)
    	   readNextLineFlag = true;
    }

    else if(strncmp(line,"G90",3) == 0)
    {
        G90(ABSOLUTE);
        if (source)
     	   readNextLineFlag = true;
    }

    else if(strncmp(line,"G91",3) == 0)
    {
        G91(RELATIVE);
        if (source)
     	   readNextLineFlag = true;
    }

    else if(strncmp(line,"G92",3) == 0)
    {
       G92(x_d,y_d,z_d,a_d,b_d,actualPosition);
       if (source)
    	   readNextLineFlag = true;
    }

    else if(strncmp(line,"M00",3) == 0)
    {
    	M00();
        if (source)
     	   readNextLineFlag = true;
    }

    else if(strncmp(line,"M01",3) == 0)
    {
    	M01();
        if (source)
     	   readNextLineFlag = true;
    }

    else if(strncmp(line,"M17",3) == 0)
    {
    	M17(x_d, y_d, z_d, a_d, b_d);
        if (source)
     	   readNextLineFlag = true;
    }

    else if(strncmp(line,"M18",3) == 0)
    {
    	M18(x_d, y_d, z_d, a_d, b_d);
        if (source)
     	   readNextLineFlag = true;
    }

    else if(strncmp(line,"M92",3) == 0)
    {
    	M92(x_d, y_d, z_d, a_d, b_d);
        if (source)
     	   readNextLineFlag = true;
    }

    else if(strncmp(line,"M199",3) == 0)
    {
        M199();
    }

    else
    {
    	if (source)
        readNextLineFlag = true;
    }

    memset(line,0,100);
    memset(G,0,10);
    memset(M,0,10);

}

void G0(double x_d,double y_d,double z_d,double a_d, double b_d, bool source)
{
	//Move Rapid

	commandedPosition[0] = x_d;
	commandedPosition[1] = y_d;
	commandedPosition[2] = z_d;
	commandedPosition[3] = a_d;
	commandedPosition[4] = b_d;

    setupMotor(source);

}

void G1(double x_d, double y_d, double z_d, double a_d, double b_d, bool source)
{
	//Move Controlled

	commandedPosition[0] = x_d;
	commandedPosition[1] = y_d;
	commandedPosition[2] = z_d;
	commandedPosition[3] = a_d;
	commandedPosition[4] = b_d;

    setupMotor(source);
}

void G28(double *homePosition, bool source)
{
	//Move to Home

	commandedPosition[0] = homePosition[0];
	commandedPosition[1] = homePosition[1];
	commandedPosition[2] = homePosition[2];
	commandedPosition[3] = homePosition[3];
	commandedPosition[4] = homePosition[4];

    setupMotor(source);
}

void G281(double x_d,double y_d,double z_d,double a_d, double b_d, double *homePosition)
{
	//Set Home Position

    homePosition[0] = x_d;
    homePosition[1] = y_d;
    homePosition[2] = z_d;
    homePosition[3] = a_d;
    homePosition[4] = b_d;
}

void G90(int mode)
{
	//Absolute Mode

    mode = 1;

}

void G91(int mode)
{
	//Relative Mode

    mode = 0;

}

void G92(double x_d,double y_d,double z_d,double a_d, double b_d, double *actualPosition)
{
	//Set Current Position

    actualPosition[0] = x_d;
    actualPosition[1] = y_d;
    actualPosition[2] = z_d;
    actualPosition[3] = a_d;
    actualPosition[4] = b_d;
}

void M00()
{
	//Program Stop

	inBendCycle = false;

	//Set the commanded positions to actual positions so nothing moves after the 'stop' is done
	commandedPosition[0]= actualPosition[0];
	commandedPosition[1]= actualPosition[1];
	commandedPosition[2]= actualPosition[2];
	commandedPosition[3]= actualPosition[3];
	commandedPosition[4]= actualPosition[4];

	TIM3->CR1 &= ~TIM_CR1_CEN;
}

void M01()
{
	//Program End

	inBendCycle = false;

	TIM3->CR1 &= ~TIM_CR1_CEN;
}

void M17(double x_d,double y_d,double z_d,double a_d, double b_d)
{
	//Enable Stepper Motors

	bool x = (bool) x_d;
	bool y = (bool) y_d;
	bool z = (bool) z_d;
	bool a = (bool) a_d;
	bool b = (bool) b_d;

	enableSteppers (x, y, z, a, b);
}

void M18(double x_d,double y_d,double z_d,double a_d, double b_d)
{
	//Disable Stepper Motors

	bool x = (bool) x_d;
	bool y = (bool) y_d;
	bool z = (bool) z_d;
	bool a = (bool) a_d;
	bool b = (bool) b_d;

	disableSteppers (x, y, z, a, b);
}

void M92(double x_d,double y_d,double z_d,double a_d, double b_d)
{
	//Set Steps per MM

	stepsPerMM[0] = x_d;
	stepsPerMM[1] = y_d;
	stepsPerMM[2] = z_d;
	stepsPerMM[3] = a_d;
	stepsPerMM[4] = b_d;

}

void M199()
{
    //Start bend cycle, custom M199 command
    initBendCycleFlag = 1;
}
