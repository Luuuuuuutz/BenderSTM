#ifndef __GLOBAL_H
#define __GLOBAL_H

//If GLOBAL hasn't been compiled yet then make new variables
#define RXLENGTH 50

typedef int bool;
#define true 1
#define false 0

//General variables
double actualPosition[5];
double commandedPosition[5];
double homePosition[5];

int RELATIVE;
int ABSOLUTE;

//SD Card Variables


//USART variables
char messageRX[RXLENGTH];
char command[RXLENGTH];
int rx_index;
bool rxFlag;

//Timer variables
bool txFlag;

//Stepper control variables
bool zFlag;
double stepsPerMM[5];                   //Array of the number of steps per mm for each axis motor

#else
//If GLOBAL already exists when this header is called then reference the extern variables
extern double actualPosition[5];
extern double commandedPosition[5];
extern double homePosition[5];
extern int RELATIVE;
extern int ABSOLUTE;


extern char messageRX[RXLENGTH];
extern char command[RXLENGTH];
extern int rx_index;
extern bool rxFlag;
extern bool txFlag;
extern bool zFlag;
extern double stepsPerMM[5];                   //Array of the number of steps per mm for each axis motor

#endif
