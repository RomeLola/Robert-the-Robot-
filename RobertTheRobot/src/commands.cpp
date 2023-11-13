//Lewis - This file contains all the functions for changing global variables and accessing information via the serial monitor
#include "commands.h"

void checkIfPromptedForCommandMenu(void) {
    if (Serial.available() > 0) {
        int incomingByte = Serial.read();
        if (incomingByte =='c')
            commandMenu(); //enters the command menu after 'c' is entered
    }
}

void commandMenu(void) {
    int inByte = 0;
    D_println("MENUS:");
    D_println("1 -> Brushless Motor \n2 -> RGB sensor \n3 -> Sonar Sensor \n4 -> Standard Servo \n5 -> Continous Roation Servo \n6 -> PathFinding \n7 -> leave");
    do {
        inByte = Serial.parseInt();
    }while (inByte == 0);
    switch (inByte) {
    case 1:
        brushlessMotorMenu();
        break;
    case 2:
        rgbSensorMenu();
        break;
    case 3:
        sonarSensorMenu();
        break;
    case 4:
        standardServoMenu();
        break;
    case 5:
        continousRoationServoMenu();
        break;
    case 6:
        pathFindingMenu();
    default:
        break;
    }
    
}

void brushlessMotorMenu(void) {
    int inByte = 0;
    double newSetpoint = 0.0;
    D_println("Brushless motor COMMANDS:");
    D_println("1 -> Display PID Values \n2 -> Change kp \n3 -> Change ki \n4 -> Change kd \n5 -> Change Angle Setpoint");
    do {
        inByte = Serial.parseInt();
    }while (inByte == 0);
    switch (inByte) {
    case 1:
        D_print("kp: "); D_print(kp); D_print(" ki: "); D_print(ki); D_print(" kd: "); D_println(kd);
        break;
    case 2:
        do{
            kp = 0;
            kp = Serial.parseFloat();
        }while(kp == 0);
        break;
    case 3:
        do{
            ki = 0;
            ki = Serial.parseFloat();
        }while(ki == 0);    
        break;
    case 4:
        do{
            kd = 0;
            kd = Serial.parseFloat();
        }while(kd == 0);    
        break;
    case 5:
        D_print("current Setpoint "); D_println(Check_Setpoint());
        do{
            newSetpoint = Serial.parseFloat();
        }while(newSetpoint == 0);
        
        D_println(newSetpoint);
        D_print("TIME:"); D_print("   SETPOINT:"); D_print("    INPUT:"); D_print("    OUTPUT:");     
        change_position(newSetpoint);
        for (int i = 0; i < 150; i++) {
            debug_values();
        }
              
        break;
    default:
        break;
    }

    setPIDgains1(kp, ki, kd); //if we change any of the gains we must update the PID controller
        for (int i = 0; i < 500; i++) {
            debug_values(); delay(5);
        }
    brushlessMotorMenu();
}


void rgbSensorMenu(void) {
int inByte = 0;
    D_println("RGB Sensor COMMANDS:");
    D_println("1 -> Sense Color \n2 -> Display Calibration Values \n3 -> Calibrate Black \n4 -> Calibrate White \n5 -> Calibration Debugging");
    do {
        inByte = Serial.parseInt();
    }while (inByte == 0);
    switch (inByte) {
    case 1:
        servo3.write(50);
        //cause the color sensor to read what color is infront of it and print to the serial monitor.
        switch (Read_Calibrated_color2())
        {
        case 0:
            Serial.println("RED");
            break;
        case 1: 
            Serial.println("BLUE");
            break;
        default:
            D_println("color returned defuault");
            break;
        }
        servo3.write(90);
        pause_system();
        rgbSensorMenu();
        break;
    case 2:
        //print out the calibration values
        //D_println("reached case 2");
        Calibration_values();
        pause_system();
        break;
    case 3:
        //initiate the calibration sequence for black
        //calibrate_Black_Color();
        pause_system();
        break;
    case 4:
        //initiate the calibration sequence for white
        //calibrate_White_Color();
        pause_system();
        break;   
    case 5:
        calibration_debugging();
        pause_system();
        rgbSensorMenu();
        break;          
    default:
        break;
    }
    commandMenu();
}

void sonarSensorMenu(void) {
int inByte = 0;
    D_println("Sonar Sensor COMMANDS:");
    D_println("1 -> Read sensor distance ");
do {
        inByte = Serial.parseInt();
    }while (inByte == 0);
    switch (inByte) {
    case 1:
        //Reads the distance the the sensor is seeing
        read_distance();
        pause_system();
        break;           
    default:
        break;
    }
    commandMenu();
}

void standardServoMenu(void) {
int inByte = 0;
    D_println("Standard Servo COMMANDS:");
    D_println("1 -> Center \n2 -> Sweep");
    do {
        inByte = Serial.parseInt();
    }while (inByte == 0);
    switch (inByte) {
    case 1:
        servo3.write(90);
        servo4.write(90);
        break;
    case 2:
        servo3.write(0);
        servo4.write(0);   
        delay(2000);    
        servo3.write(180);
        servo4.write(180);   
        break;
    default:
        break;
    }
    servo3.write(90);
    servo4.write(90);
    commandMenu();
}

void continousRoationServoMenu(void) {
    int inByte = 0;
    double speed = 0;
    D_println("Continous Rotation Servo COMMANDS:");
    D_println("1 -> Stop Moving \n2 -> Move Forward \n3 -> Spin \n4 -> Custom Speed");
    do {
        inByte = Serial.parseInt();
    }while (inByte == 0);
    switch (inByte) {
    case 1:
        break;
    case 2:
        rotateLeftWheel(1);
        rotateRightWheel(1);
        delay(5000);
        break;
    case 3:
        rotateLeftWheel(1);
        rotateRightWheel(-1);
        delay(5000);
        rotateLeftWheel(-1);
        rotateRightWheel(1);
        delay(5000);  
    case 4:
        do {
            speed = Serial.parseFloat();
        }while (speed == 0);
        
        rotateLeftWheel(speed);
        rotateRightWheel(speed);
        delay(2000);
    default:
        break;
    }
        rotateLeftWheel(0);
        rotateRightWheel(0);
    commandMenu();
}

void pathFindingMenu(void) {
    RobotPose robertLocation = getRobotPose(RobotSelfID);
    int inByte = 0;
    int cylinderSelection = 0;
    struct BallPosition ballArray[NUM_BALLS];
    int numOfCylinder = 0;
    double disError = 0;
    double errorAngle = 0;

    D_println("Path Finding COMMANDS:");
    D_println("1 -> Go to a Cylinder \n2 -> Go to Nearest Cylinder \n3 -> Display Controller Gains \n4 -> Display Network Data \n5 -> Edit Distence Gain \n6 -> Edit Angle Gain");

    do {
        inByte = Serial.parseInt();
    }while (inByte == 0);
    switch (inByte) {
    case 1:
        numOfCylinder = getBallPositions(ballArray);
        D_print("Choose a Cylinder Between 1 and "); D_println(numOfCylinder);
        do {
            cylinderSelection = Serial.parseInt();
        }while (cylinderSelection == 0);
        cylinderSelection--;

        do{
            disError = approachTarget(ballArray[cylinderSelection].x, ballArray[cylinderSelection].y);
        }while(disError > 100); //update this after testing to avoid weirdness as it gets closer to the targetC
        rotateLeftWheel(0);
        rotateRightWheel(0);
        D_println("Target Reached!!! "); 
        break;
    case 2:
        do{
            BallPosition closeBall = getNearestCylinder(robertLocation);
            if(closeBall.x != 0)
                disError = approachTarget(closeBall.x, closeBall.y);
        }while(disError > 100); //update this after testing to avoid weirdness as it gets closer to the targetC
        rotateLeftWheel(0);
        rotateRightWheel(0);
        D_println("Target Reached!!! "); 
        break;
    case 3:
        D_print("pathDistanceGain: "); D_println(pathDistanceGain);
        D_print("pAngleGain: "); D_println(pAngleGain);
        delay(1000);
        break;
    case 4:
        if (inByte == 4)
        {
            updateValidCylinderArray();
            BallPosition closeBall = getNearestValidCylinder(myCylinderColor);
            D_println(closeBall.x);  D_println(closeBall.y);
            errorAngle = getAngleError(closeBall.x, closeBall.y);
            D_println(errorAngle);
        }
        break;
    case 5:
        D_print("Old Distance Gain: "); D_println(pathDistanceGain);
        do{
            pathDistanceGain = 0;
            pathDistanceGain = Serial.parseFloat();
        }while(pathDistanceGain == 0);
        break;
    case 6:
        D_print("Old Angle Gain: "); D_println(pAngleGain);
        do{
            pAngleGain = 0;
            pAngleGain = Serial.parseFloat();
        }while(pAngleGain == 0);
        break;
    default:
        break;
    }
    commandMenu();
}

void pause_system(void)
{
  int input  = 0;
  Serial.println("Enter any # value to continue: ");
  do
  {
    input = Serial.parseInt();
  }while(input == 0);
}