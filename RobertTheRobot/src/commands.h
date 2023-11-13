#ifndef COMMANDS_H
#define COMMANDS_H

#include <Arduino.h>
#include "debug.h"
#include "decisionDecider.h"
#include "state.h" //color.h is included through state.h
#include "dc_motors.h"
#include "MotorPID.h"

extern double kp; extern double ki; extern double kd;

void checkIfPromptedForCommandMenu(void);
void commandMenu(void);
void brushlessMotorMenu(void);
void rgbSensorMenu(void);
void sonarSensorMenu(void);
void standardServoMenu(void);
void continousRoationServoMenu(void);
void pathFindingMenu(void);
void pause_system(void);

#endif 