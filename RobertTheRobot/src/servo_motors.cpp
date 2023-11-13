#include "servo_motors.h"

const char SERVO1_PIN = 26;
const char SERVO2_PIN = 25;
const char SERVO3_PIN = 33;
const char SERVO4_PIN = 17;

const int ContinousRotationMinPWM = 1300;
const int ContinousRotationMaxPWM = 1700;
const int StandardMinPWM = 500;
const int StandardMaxPWM = 2500;

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

void setupServos(void)
{   
    ESP32PWM::allocateTimer(3);
    servo1.setPeriodHertz(50);    // standard 50 hz servo
    servo2.setPeriodHertz(50);    // standard 50 hz servo
    servo3.setPeriodHertz(50);    // standard 50 hz servo
    servo4.setPeriodHertz(50);    // standard 50 hz servo
    
    servo1.attach(SERVO1_PIN, ContinousRotationMinPWM, ContinousRotationMaxPWM);  //continous rotation
    servo2.attach(SERVO2_PIN, ContinousRotationMinPWM, ContinousRotationMaxPWM);  //continous rotation
    servo3.attach(SERVO3_PIN, StandardMinPWM, StandardMaxPWM); //Standard 
    servo4.attach(SERVO4_PIN, StandardMinPWM, StandardMaxPWM); //Standard 

    servo3.writeMicroseconds(1500); //centers the servo
    servo4.writeMicroseconds(1500); //centers the servo
}

//the input normalizedSpeed is a value between -1 and 1. -1 is full reverse while +1 is full forward. 0 is stationary.
void rotateLeftWheel(double normalizedSpeed) {
    if (normalizedSpeed <= 1 && normalizedSpeed >= -1) {
        servo1.write(map((int)(normalizedSpeed*100),-100,100,ContinousRotationMinPWM, ContinousRotationMaxPWM));
        D_print("rotateLeftWheel ran with speed "); D_print(normalizedSpeed); D_print(" pwm "); D_println(map((int)(normalizedSpeed*100),-100,100,ContinousRotationMinPWM, ContinousRotationMaxPWM));
    }
        else if(normalizedSpeed <= 1) {
        servo1.write(ContinousRotationMinPWM);
        D_print("Error: RotateRightWheel Speed is out of bounds. Speed = "); D_println(normalizedSpeed);
    }
        else {
        servo1.write(ContinousRotationMaxPWM);
        D_print("Error: RotateLeftWheel Speed is out of bounds. Speed = "); D_println(normalizedSpeed);
    }
}
void rotateRightWheel(double normalizedSpeed) {
    if (normalizedSpeed <= 1 && normalizedSpeed >= -1) {
        servo2.write(map((int)(normalizedSpeed*100),-100,100,ContinousRotationMaxPWM, ContinousRotationMinPWM));
        D_print("rotateRightWheel ran with speed "); D_print(normalizedSpeed); D_print(" pwm "); D_println(map((int)(normalizedSpeed*100),-100,100,ContinousRotationMaxPWM, ContinousRotationMinPWM));
    }
    else if(normalizedSpeed <= 1) {
        servo2.write(ContinousRotationMaxPWM);
        D_print("Error: RotateRightWheel Speed is out of bounds. Speed = "); D_println(normalizedSpeed);
    }
        else {
        servo2.write(ContinousRotationMinPWM);
        D_print("Error: RotateRightWheel Speed is out of bounds. Speed = "); D_println(normalizedSpeed);
    }
}
