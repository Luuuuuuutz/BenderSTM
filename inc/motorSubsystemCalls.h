#ifndef __MOTORSUBSYSTEMCALLS_H                                                 //NOT SURE IF I NEED EITHER OF THESE, CHECK
#define __MOTORSUBSYSTEMCALLS_H

void setup_timer3();
void setup_GPIO();                                                              //might need to change name
void setupMotor();
void advance_Motor();
//void setupMotor(double *moveX, double *moveY, double *moveZ, double *moveA, double *moveB, int *steps[], double stepsPerMM[]);
//void advance_Motor(double moveX, double moveY, double moveZ, double moveA, double moveB, int *steps[]);          //might need to include zFlag

#endif
