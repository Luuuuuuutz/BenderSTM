#include "global.h"

#ifndef __MOTORSUBSYSTEMCALLS_H
#define __MOTORSUBSYSTEMCALLS_H

void setupTimer3();
void setupTimer15();
void setupTimer16();
void setupButtons();
void setupGPIO();
void setupMotor();
void advanceMotor();

#define X_PLS 10
#define X_DIR 11
#define X_EN 12

#define Y_PLS 9
#define Y_DIR 8
#define Y_EN 9

#define Z_PLS 6
#define Z_DIR 7
#define Z_EN 8

#define A_PLS 2
#define A_DIR 10
#define A_EN 11

#define B_PLS 5
#define B_DIR 0
#define B_EN 1

#endif
