#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <Arduino.h>
#include "debug.h"
#include "communications.h"
#include "servo_motors.h"
#include "decisionDecider.h"
#include "pathFinder.h"
#include "main.h"
#include "state.h" 

extern double pathDistanceGain;
extern double pAngleGain;
extern double maxSpeed;
extern double minSpeed;
extern double maxAngle;
extern double minAngle;
extern boolean isRobertOnTheField;

double getDistanceError(int xTarget, int yTarget);
double getAngleError(int xTarget, int yTarget);      //bounded within -pi and pi
double getRightWheelSpeed_angledturn(double targetAngle);
double getRightWheelSpeed(int xTarget, int yTarget); //returns normalized speed between -1 and 1.
double getLeftWheelSpeed_angledturn(double targetAngle);
double getLeftWheelSpeed(int xTarget, int yTarget);  //returns normalized speed between -1 and 1.
double approachTarget(int xTarget, int yTarget);
void goToTarget(int xTarget, int yTarget, int stopRadius);
double turnrobottoward(double targetAngle);
boolean detectLackOfRobertOnField(); //provides a buffer functionality so that drops in wifi data do not cause the robot to think it is off the field.
int getRelativeLocation(int xTarget, int yTarget, RobotPose robertLocation);
boolean isPathObstructed(int xTarget, int yTarget, BallPosition &obstacle);
void avoiddroppedcylinder(); // Reece This will allow the bot to move around the cylinder after it is dropped (tested : [])
//BallPosition avoidObstruction(BallPosition obstacle);

#endif