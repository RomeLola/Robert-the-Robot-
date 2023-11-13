#ifndef MOTORPID_H
#define MOTORPID_H

//include any applicable header files here. You probably want the two I added below
#include <Arduino.h>
#include "debug.h"
#include "common.h"
#include "main.h"

//variable declarations

//gains
extern double kp; extern double ki; extern double kd;

//function declarations
void setupEncoderInterrupt();
void IRAM_ATTR onEncoderTimer();
void setupPIDInterrupt();
void IRAM_ATTR onPIDTimer();
void SetupDCMotor();
void setPIDgains1(double propk, double intk, double derivek);
void change_position(int postion);
double Check_Setpoint(void);
void debug_values(void);

#endif