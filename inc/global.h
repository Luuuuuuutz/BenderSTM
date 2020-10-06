#ifndef GLOBAL_H
#define GLOBAL_H

//If GLOBAL hasn't been compiled yet then make new variables
#define RXLENGTH 50

typedef int bool;
#define true 1
#define false 0

//General variables
double actualPosition[5];
double commandedPosition[5];

//USART variables
char messageRX[RXLENGTH];
char command[RXLENGTH];
int rx_index;
bool rxFlag;

//Timer variables
bool txFlag;

#else
//If GLOBAL already exists when this header is called then reference the extern variables
extern double actualPosition[5];
extern double commandedPosition[5];

extern char messageRX[RXLENGTH];
extern char command[RXLENGTH];
extern int rx_index;
extern bool rxFlag;

#endif
