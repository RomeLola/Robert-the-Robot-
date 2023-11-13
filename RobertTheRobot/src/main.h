#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "debug.h"
#include "common.h"
#include "dc_motors.h"
#include "MotorPID.h"
#include "servo_motors.h"
#include "communications.h"
#include "stdint.h"

#include "state.h"
#include "Color.h"
#include "commands.h"
#include "DistanceSensor.h"
#include "limitSwitch.h"

extern double kp; extern double ki; extern double kd;
extern double setpoint;
extern int pos;
extern bool isRobertStopped;

//extern void setupColorSensor();
//extern void commandMenu();


#endif