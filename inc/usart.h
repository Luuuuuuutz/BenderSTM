#include "global.h"

void usart1Init(int);
void usart1DMAInit();
void dmaRestart(char*, char);
void usart1Send(const char*);
void softwareTimerInit(void);
