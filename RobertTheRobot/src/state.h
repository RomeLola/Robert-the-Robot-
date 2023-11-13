#ifndef STATE_H
#define STATE_H

#include <Arduino.h>
#include "debug.h"
#include "color.h"
#include "servo_motors.h"
#include "distanceSensor.h"
#include "pathFinder.h"
#include "decisionDecider.h"
#include "limitSwitch.h"
#include "commands.h"

enum StateMachineState {
  DriveToFlag = 0,
  BlockOpponent = 1,
  AvoidObstacle= 2,
  LeaveFlag = 3,
  PickupFlag = 4,
  ReturnFlagToBase = 5,
  DropFlag = 6,
  TestState = 7,//can only be entered or exited though serial comands
};

extern StateMachineState state;

enum Obstacle {
  Wall = 0,
  Cylinder = 1,
};

extern Obstacle type;

void switchState();
void runDriveToFlag();
void runBlockOpponent();
void runAvoidObstacle();
void runLeaveFlag();
void runPickupFlag();
void runReturnFlagToBase();
void runDropFlag();
void runTestState();

#endif