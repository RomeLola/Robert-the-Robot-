#ifndef DECISIONDECIDER_H
#define DECISIONDECIDER_H

#include <Arduino.h>
#include "debug.h"
#include "communications.h"
#include "pathFinder.h"

enum Color {
    UNKNOWN, // = 0
    GREEN, // = 1
    RED, // = 2
    BLUE, // = 3
};
//note, make it so that any cylinders in our base are assumed to be captured and are ignored based off their position.

extern int capturedCylinders;
extern Color targetColor;
extern int Homex;
extern int Homey;
extern Color myCylinderColor;
extern int enemycylinderdropped;

/*
- We want to go for the cylinders that are closest to the robot.
- We want to avoid spending time checking the color of cylinders that we already checked
- We only get the positons of the cylinders and the h perameter. h = 1 means orange and h = 0 means it is red or blue.
- We will keep track of how many cylinders we captured in capturedCyliners to tell us when we can pick up green cylinders.
- To keep track of known enemy cylinders that we checked the color of we will use an array enemyCyliners which holds the color and location of 
    each non-capturable color.
- We will periodicly (perhaps whenever the robot is returning with a cylinder) get the location of all the cylinders from wifi and 
    check their location against enemyCyliners to see if it is close to any of cylinders of known colors. If they are aproximately 
    in the same spot then we assume they are the same cylinder and thus the same color. Then we sort the cylinders by distence from 
    the robot position with the shortest being at the front of the array. This allows us to quickly look up the next closest cylinder 
    that is closest and most likely to be one of ours.
*/


int distanceBetween(BallPosition a, BallPosition b);
int distanceBetween(BallPosition a, RobotPose b);
void addEnemyCylinder(BallPosition pose);
void updateValidCylinderArray();
BallPosition getNearestValidCylinder(Color c);
BallPosition getNearestCylinder(RobotPose robertLocation);
boolean isRobertHome();
boolean isCylinderHome(BallPosition pose);
boolean whichColorToTarget();

#endif