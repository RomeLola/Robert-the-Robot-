#ifndef COLOR_H
#define COLOR_H

#include <Arduino.h>
#include "debug.h"
#include "servo_motors.h"

void setupColorSensor();
void calibrate_Black_Color();
void calibration_debugging();
void calibrate_White_Color();
void Calibration_values();
int readCalibratedColor();
int Read_Calibrated_color2();
void Change_Color(bool RedColor,bool GreenColor,bool BlueColor);
int read_light_sensor();
void map_input_color(int rgb);
int Read_Calibrated_color();
int conduct_Color_Reading();


extern int maxblack[3]; // UPDATE the max reading obtained from a black surface for {RED,GREEN,BLUE}
extern int minwhite[3]; // UPDATE the min reading obtained from a white surface for {RED,GREEN,BLUE}
extern int color[3]; // array for the readings

#endif