#ifndef DISTANCESENSOR_H
#define DISTANCESENSOR_H

#include <Arduino.h> // This is needed for the ESP32 to work with base Arduino libraries
#include <cmath>
#include "MedianFilterLib.h"

extern int irSensor1Pin; 

extern int counter_of_5;

extern int inputVal;

void setupDistanceSensor();
float convertVoltage2Distance(int medianValue);
float read_distance();  

#endif