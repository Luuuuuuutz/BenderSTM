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
double nextCommandedPosition[5];

int RELATIVE;
int ABSOLUTE;

bool initBendCycleFlag;
bool startBendCycle;
bool inBendCycle;
bool progSource;

//SD Card Variables
bool readNextLineFlag;
bool nextLineReady;

//USART variables
char messageRX[RXLENGTH];
char command[RXLENGTH];
int rx_index;
bool rxFlag;

int actualXWhole;
int actualXDec;
int actualYWhole;
int actualYDec;
int actualZWhole;
int actualZDec;
int actualAWhole;
int actualADec;
int actualBWhole;
int actualBDec;

//Timer variables
bool txFlag;

//Stepper control variables
bool zFlag;
double stepsPerMM[5];                   //Array of the number of steps per mm for each axis motor

//External button variables
bool button1Flag;
bool button2Flag;

#else
//If GLOBAL already exists when this header is called then reference the extern variables
extern double actualPosition[5];
extern double commandedPosition[5];
extern double homePosition[5];
extern double nextCommandedPosition[5];
extern int RELATIVE;
extern int ABSOLUTE;

extern bool initBendCycleFlag;
extern bool startBendCycle;
extern bool inBendCycle;
extern bool progSource;

//SD Card Variables
extern bool readNextLineFlag;
extern bool nextLineReady;

//USART variables
extern char messageRX[RXLENGTH];
extern char command[RXLENGTH];
extern int rx_index;
extern bool rxFlag;

//Timer variables
extern bool txFlag;

//Stepper control variables
extern bool zFlag;
extern double stepsPerMM[5];                   //Array of the number of steps per mm for each axis motor

//External button variables
extern bool button1Flag;
extern bool button2Flag;

#endif
