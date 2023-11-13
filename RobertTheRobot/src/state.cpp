//Lewis - This defines functions for the state machine. Much of the logic for controlling the robot is here.
#include "state.h"

StateMachineState state;
Obstacle type;

void switchState() {
    switch (state)
    {
    case DriveToFlag:
        runDriveToFlag();
        break;
    case BlockOpponent:
        runBlockOpponent();
        break;
    case AvoidObstacle:
        runAvoidObstacle();
        break;
    case LeaveFlag:
        runLeaveFlag();
        break;
    case PickupFlag:
        runPickupFlag();
        break;
    case ReturnFlagToBase:
        runReturnFlagToBase();
        break;
    case DropFlag:
        runDropFlag();
        break;
    case TestState:
        runTestState();
        break;                                     
    default:
        D_println("ERROR: NO STATE SELECTED");
        break;
    }
}

void runDriveToFlag() {
    // get the position of the closest valid cylinder
    updateValidCylinderArray();
    BallPosition pose = getNearestValidCylinder(targetColor);
    D_print("Closest Cylinder "); D_print(pose.x); D_print(", "); D_println(pose.y);
    // go to that cylinder
    int distError;
    if(pose.y > 0 && pose.x > 0) {
        goToTarget(pose.x, pose.y, 125);
        state = PickupFlag;
    }
}
void runBlockOpponent() {

}
/*************************************************************************************/
void runAvoidObstacle() 
{
    float distance45 = 0;  
    float distance135 = 0;
   //robert backs up 
    rotateLeftWheel(-1);
    rotateRightWheel(-1);
    delay(1000);
    //robert stops
    rotateLeftWheel(0);
    rotateRightWheel(0);
    servo4.write(100); // moves the servo to 45 degrees to scan distance
    delay(500);
    //reads the diatance 
    distance45 = read_distance();

    servo4.write(200);// moves the servo to 135 degrees to scan distance
    delay(500);
    //reads distance at 135 degrees 
    distance135 = read_distance();

    if (distance45 > distance135)
    {
        //checks which distance is greater and move towards it
        
    }



    //scans for the longest distance and goes towards it with a slightly larger angle
    //to avoid hitting the edge and goes around wall.
    //go back to state from other function 
    //write a funtions that has a state go back to the previous state
    //}
}
/************************************************************************************/
void runLeaveFlag() {

}

void runPickupFlag() {
    int cylinder_color;
    //Make sure the robot is stopped
    rotateLeftWheel(0);
    rotateRightWheel(0);

    // then closes the claw
    servo3.write(50);
    delay(500);
    if (read_light_sensor() > 4000 ) { //checks if cylinder was grabbed
        if(capturedCylinders < 3)  // change back to whichColorToTarget()
            cylinder_color = Read_Calibrated_color2(); //checks color
        else {
            state = ReturnFlagToBase;
            return;
        } 

        switch (cylinder_color) {
        case 0: 
            if(myCylinderColor == RED) {
                Serial.println("RED");
                state = ReturnFlagToBase;
                break;
            }
            else {
                Serial.println("RED");
                enemycylinderdropped = 1;
                state = DropFlag;
                break;    
            }        
            break;
        case 1: 
            if(myCylinderColor == BLUE) {
                Serial.println("BLUE");
                state = ReturnFlagToBase;
                break;
            }
            else {
                Serial.println("BLUE");
                enemycylinderdropped = 1;
                state = DropFlag;
                break;
            }                     
            break;
        default:      
                state = DropFlag; 
            break;
        } 
    }
    else
    state = DropFlag;
}

void runReturnFlagToBase() {
    //go to the home location
    capturedCylinders++;
    int distError;
    //go near the corner of home to avoid getting too close to the wall
    goToTarget(Homex+150, Homey+150, 300);
    //then go to the actual home location
    goToTarget(Homex, Homey, 180);
    //once you arrive change state to dropFlag
    state = DropFlag;
}
void runDropFlag() {
    //release the cylinder
    servo3.write(90);
    delay(200);
    //backup and turn toward the center of the field
    rotateLeftWheel(-1);
    rotateRightWheel(-1);
    delay(800);
    rotateLeftWheel(0);
    rotateRightWheel(0);
    if (enemycylinderdropped == 1) {
        RobotPose robertLocation = getRobotPose(RobotSelfID);
        //get dropped cylinder position and save it to the enemy array
        for(int i = 0; i < 5; i++) {  // Prevents camera gliches, probably unneccesary. 
            BallPosition enemyCylinderPose = getNearestValidCylinder(UNKNOWN);
            if (distanceBetween(enemyCylinderPose, robertLocation) < 250) {
                addEnemyCylinder(enemyCylinderPose);
                break;
            }
        }
        // create a maneuver around state
        avoiddroppedcylinder();
        enemycylinderdropped = 0;
    }
    //turnrobottoward(1000, 1000);
    state = DriveToFlag;
}

//In this function we will demo each sensor/actuator
void runTestState() {
    D_println("Test State\nHit C + enter to enter the command menu");
    checkIfPromptedForCommandMenu();
}