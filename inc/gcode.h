#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "global.h"
#include "ff.h"
#include "fat16.h"


#ifndef __GCODE_H
#define __GCODE_H


//void openFileG(FATFS &FatFs, FIL &Fil);
//void readGCodeLine(FATFS*, FIL*);
void G0(double x_d,double y_d,double z_d,double a_d, double b_d,double *commanded);
void G1(double x_d,double y_d,double z_d,double a_d, double b_d, double* actual);
void G28(double *home, double *commanded);
void G281(double x_d,double y_d,double z_d,double a_d, double b_d, double *home);
void mode(int mode);
void G92(double x_d,double y_d,double z_d,double a_d, double b_d, double *actual);



#endif
