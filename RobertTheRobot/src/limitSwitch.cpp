//Lewis - this handles the limit swtich interupts
#include "limitSwitch.h"

#define pushButton_pin 39

bool isRobertStopped = false; //robert is moving and not stopped by trigger


void setupLimitSwitch1()
{
    pinMode(pushButton_pin, INPUT);
    attachInterrupt(pushButton_pin, Mytoggle, RISING);
    Serial.begin(115200);
}

//void IRAM_ATTR Mytoggle() // ISR function, takes no parameters and returns nothing
//{
//    Serial.println("OBJECT DETECTED");
//}

void IRAM_ATTR Mytoggle()
{
    state = AvoidObstacle;
    // int toggleState = digitalRead(pushButton_pin);
    // if(toggleState == 0) //if switch is pressed 
    // {
    //     isRobertStopped = !isRobertStopped; //robert is stopped 
    //     if (isRobertStopped)
    //     {
    //         Serial.println("Robert stopped "); 
    //         rotateLeftWheel(0); //robert should stop
    //         rotateRightWheel(0);

    //     }
    //     else
    //     {
    //         Serial.println("Robert keep going");
    //     }
    // }
}
//void  printInteruptID(int limitSwithchID) {
//    Serial.println("INTERRUPT From Limit Switch "+ limitSwithchID);
//}
