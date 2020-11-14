#include "global.h"

#ifndef __GCODE_H
#define __GCODE_H


void openFile();
bool readGCodeLine();
void CheckGCodeLine(char*, bool);

void G0(double, double, double, double, double, bool);
void G1(double, double, double, double, double, bool);
void G28(double*, bool);
void G281(double x_d,double y_d,double z_d,double a_d, double b_d, double *home);
void G90(int mode);
void G91(int mode);
void G92(double x_d,double y_d,double z_d,double a_d, double b_d, double *actual);
void M00(void);
void M01(void);
void M02(void);
void M17(double x_d,double y_d,double z_d,double a_d, double b_d);
void M18(double x_d,double y_d,double z_d,double a_d, double b_d);
void M92(double x_d,double y_d,double z_d,double a_d, double b_d);
void M199(void);
void M200(void);

#endif
