#include "global.h"

#include "sd.h"
#include "ff.h"
#include "fat16.h"

#ifndef __GCODE_H
#define __GCODE_H

void openFile(void);
void readGCodeLine(void);
void G0(double x_d,double y_d,double z_d,double a_d, double b_d,double *commanded);
void G1(double x_d,double y_d,double z_d,double a_d, double b_d, double* actual);
void G28(double *home, double *commanded);
void G281(double x_d,double y_d,double z_d,double a_d, double b_d, double *home);
void mode(int mode);
void G92(double x_d,double y_d,double z_d,double a_d, double b_d, double *actual);


#endif
