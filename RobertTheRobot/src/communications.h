#pragma once

#include "WiFi.h"
#include "AsyncUDP.h"
#include "debug.h"

//#define DEBUG_RAW_UDP         // when defined raw_UDP information from UDP broadcast will be printed
//#define DEBUG_COMMUNICATIONS    // when defined ball and robot info from camera will be printed Line 160

/**************************************************************************************************************/
/* Information specific to this semester's robot and ball positions */
#define NUM_ROBOTS 20
#define NUM_BALLS 20

struct RobotPose
{
  boolean valid;
  int16_t ID;
  int16_t x;
  int16_t y;
  int16_t theta;
  int16_t state;
};

struct BallPosition
{
  int16_t x;
  int16_t y; 
  int16_t hue; 
};

extern int RobotSelfID;

int getNumRobots();
RobotPose getRobotPose (int robotID);
int getBallPositions (BallPosition (&pos)[NUM_BALLS]);
int16_t convert_bytes_to_int16(uint8_t* buf);
void PrintHex8(uint8_t *data, uint8_t length);
void updateRobotPoseAndBallPositions (uint8_t* lbuf);
void setupCommunications();
void printRobotPose (RobotPose pose);
void printBallPositions(int num, BallPosition (&pos)[NUM_BALLS]);