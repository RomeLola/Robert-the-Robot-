#pragma once

#include "common.h"
#include <ESP32Servo.h>
#include "debug.h"

extern Servo servo1;
extern Servo servo2;
extern Servo servo3;
extern Servo servo4;

void setupServos(void);
void rotateLeftWheel(double normalizedSpeed);
void rotateRightWheel(double normalizedSpeed);