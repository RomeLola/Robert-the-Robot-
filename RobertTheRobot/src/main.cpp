#include "main.h"

double kp =1920.0; double ki=106666.0; double kd=24; // kd should go to around 50 for a better final value
int pos = 0;
bool isRobertStopped = false; // robert is not stopped

void setup() {
  D_println("Robert The Robot is Alive!");
  state = DriveToFlag;
  //state = TestState; // for debug and calibration only
  Serial.begin(115200);
  setupEncoderInterrupt();
  setupPIDInterrupt(); // added interrupts to more properly control the motor
  SetupDCMotor();
  setupServos();
  setupColorSensor();
  setupCommunications(); // if you get ...... in the serial monitor then comment out this line to disable the wifi part. If you are working at home this is required
  setupDistanceSensor();
  setupLimitSwitch1();

  setPIDgains1(kp,ki,kd);
}

void loop() {
  D_print("State number is = "); D_println(state);
  rotateLeftWheel(0);
  rotateRightWheel(0);
  /************************************/
  if(isRobertStopped)
  {
    state = AvoidObstacle;
  }
  /***********************************/
  switchState();
}