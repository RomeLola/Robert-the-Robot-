// check out the custom functions (.cpp) and library (.h) files in this project folder
//    - feel free to #include the libraries if you feel like they will help, especially the debug one
//    - the dc_motors.cpp file has been commented out because a lot of this code was pulled from it and there is variable overlap
#include <Arduino.h>
#include <PID_v1.h>
#include "debug.h"
#include "MotorPID.h"

// Set up the hardware for timer interrupts
hw_timer_t * encoder_timer = NULL;
hw_timer_t * PID_timer = NULL;

// declare setup functions
void setupEncoderInterrupt();
void IRAM_ATTR onEncoderTimer();
void setupPIDInterrupt();
void IRAM_ATTR onPIDTimer();
void SetupDCMotor();


// declare global variables for PWM control of motor
const int PWMFreq = 500; /* 5 KHz */
const int PWMResolution = 12;
const int MAX_DUTY_CYCLE = (int)(pow(2, PWMResolution) - 1);

// declare rate to sample PID function
int pidSampleTime = 10; // milliseconds

// Global variables for keeping track of position
const int MOT_CH_IN1 = 4;
const int MOT_CH_IN2 = 5;
const char ENC_CHANA_PIN = 14;
const char ENC_CHANB_PIN = 27;
const char MOT_IN1_PIN = 13;
const char MOT_IN2_PIN = 12;
volatile long position = 0;
volatile bool lastEncA = 0;
volatile bool lastEncB = 0;
volatile bool newEncA = 0;
volatile bool newEncB = 0;
volatile bool error;
const int TICS_PER_REV = 2800;

static portMUX_TYPE setpointMutex = portMUX_INITIALIZER_UNLOCKED;

// Global variables used by the controller. 
//    input - the measured value from the encoders
//    output - the controller output effort from the PID computation
//    setpoint - the desired value of the output, sometimes called the reference 
//                (in this case we are doing rotational position control)
// **IMPORTANT**: the units of setpoint and input need to be the same so that the error computation 
//                inside the PID object has the same units
double input=0, output=0, setpoint=0;

PID myPID(&input, &output, &setpoint,kp,ki,kd, DIRECT);

// add these to the main function




// Timer interrupt function for encoder on dc motor----------------------------------
void IRAM_ATTR onEncoderTimer() { 
  // WARNING: ISR FUNCTIONS DON'T WORK WITH FLOATS USE DOUBLES IF YOU WANT DECIMALS
  newEncA = digitalRead(ENC_CHANA_PIN); //read encoder value
  newEncB = digitalRead(ENC_CHANB_PIN); //read encoder value
  position += (newEncA ^ lastEncB) - (lastEncA ^ newEncB); // determine new position from encoder readings
  if((lastEncA ^ newEncA) & (lastEncB ^ newEncB)){
      error = true;
  }
  lastEncA = newEncA;
  lastEncB = newEncB;  
}

// PID timer interrupt-------------------------------------------------------------
void IRAM_ATTR onPIDTimer() { 
  // WARNING: ISR FUNCTIONS DON'T WORK WITH FLOATS USE DOUBLES IF YOU WANT DECIMALS
  // PID COMPUTATION
  input = position * (360.0 / TICS_PER_REV);
  myPID.Compute();
      if (output > 0){ // drive motor based off pid output
      ledcWrite(MOT_CH_IN1, abs(output));
      ledcWrite(MOT_CH_IN2, 0);
  }
  else{
      ledcWrite(MOT_CH_IN2, abs(output));
      ledcWrite(MOT_CH_IN1, 0);
  }  
}

// encoder timer interrupt setup--------------------------------------------------
void setupEncoderInterrupt() {  
  encoder_timer = timerBegin(0, 80, true);  // timer 0, prescalewr of 80 give 1 microsecond tiks
  timerAttachInterrupt(encoder_timer, &onEncoderTimer, true); // connect interrupt function to hardware with pointer
  timerAlarmWrite(encoder_timer, 10, true);  // 10 microsecond timer interrupt
  timerAlarmEnable(encoder_timer);
}

// PID timer interrupt setup--------------------------------------------------
void setupPIDInterrupt() {  
  PID_timer = timerBegin(1, 80, true);  // timer 1, prescalewr of 80 give 1 microsecond tiks
  timerAttachInterrupt(PID_timer, &onPIDTimer, true); // connect interrupt function to hardware with pointer
  timerAlarmWrite(PID_timer, 10000, true);  // 10 millisecond timer interrupt
  timerAlarmEnable(PID_timer);
}

// sets up for PWM outputs for motor----------------------------------------
void SetupDCMotor()
{  
  pinMode(ENC_CHANA_PIN, INPUT_PULLDOWN);
  pinMode(ENC_CHANB_PIN, INPUT_PULLDOWN);
  ledcSetup(MOT_CH_IN1, PWMFreq, PWMResolution);
  ledcSetup(MOT_CH_IN2, PWMFreq, PWMResolution);
  ledcAttachPin(MOT_IN1_PIN, MOT_CH_IN1);
  ledcAttachPin(MOT_IN2_PIN, MOT_CH_IN2);
  ledcWrite(MOT_CH_IN1, 0);
  ledcWrite(MOT_CH_IN2, 0);
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(pidSampleTime);
  myPID.SetOutputLimits(-4095,4095);
}

// void printMotorDebugValues(void) {
//     D_print(getSetpoint1()); D_print("   "); D_print(getPosition1()); D_print("   "); D_print(getError1()); D_print("   "); D_println(getOutput1());
// }

void setPIDgains1(double propk, double intk, double derivek)
{
  double kp1 = propk;
  double ki1 = intk;
  double kd1 = derivek;

  myPID.SetTunings(kp1,ki1,kd1);
}

void change_position(int position_input)
{
  taskENTER_CRITICAL(&setpointMutex);
  setpoint += position_input;
  taskEXIT_CRITICAL(&setpointMutex);
}

double Check_Setpoint()
{
  return setpoint;
}

void debug_values()
{
  D_print(millis()); D_print(" ");
  D_print("SETPOINT: ");
  D_print(setpoint); D_print(" ");
  D_print("ERROR: ");
  D_print(setpoint-input);    D_print(" ");
  D_print("OUTPUT: ");
  D_print(output);   D_print(" ");
  D_println("");
}