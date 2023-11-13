#include "decisionDecider.h"

//GAME SETTINGS!!!
int Homex = 100;                    // home location on the field
int Homey = Homex;
Color myCylinderColor = BLUE;    // Red or blue?

int capturedCylinders = 0;
int enemycylinderdropped = 0;
Color targetColor = UNKNOWN;        // this determines if robert is going after nuetral or unkown cylinders

BallPosition enemyCylinders[NUM_BALLS];
BallPosition validCylinder[NUM_BALLS];//in order of close to far

int distanceBetween(BallPosition a, BallPosition b) {
    return sqrt(pow(a.x-b.x, 2)+pow(a.y-b.y, 2));
}

int distanceBetween(BallPosition a, RobotPose b) {
    return sqrt(pow(a.x-b.x, 2)+pow(a.y-b.y, 2));
}

void addEnemyCylinder(BallPosition pose) {
    for(int i = 0; i < NUM_BALLS; i++) {
        if(enemyCylinders[i].x == 0 && enemyCylinders[i].y == 0) {//check if index i is empty. if it is then add it.
            enemyCylinders[i] = pose;
            return;
        }
    }
    D_println("enemyCilynders are bellow");
    printBallPositions(2, enemyCylinders);
}

void updateValidCylinderArray() { //gets cylinders from communications and removes any that are in the enemyCylinder array or are in the home location.
    RobotPose robertLocation = getRobotPose(RobotSelfID);
    BallPosition cylinders[NUM_BALLS];
    int ballCount = getBallPositions(cylinders);

    //reset the array
    for(int i = 0; i < NUM_BALLS; i++) 
    {
        validCylinder[i].x = 0;
        validCylinder[i].y = 0;
    }

    const int filterMagnitude = 100;

    int counter = 0;

    if(ballCount == -2)
        return;

    printBallPositions(3, cylinders);

    boolean enemy;
    for(int i = 0; i < ballCount; i++) { //copy nonduplicate cylinderPose to validCylinder
        enemy = false;
        for(int j = 0; j < NUM_BALLS; j++) { //compare cylinders to those in the enemy array as well as ignoring any at the home
            if(distanceBetween(cylinders[i], enemyCylinders[j]) < filterMagnitude || isCylinderHome(cylinders[i])) {
                enemy = true;
                break;
            } 
        }

        if(enemy == false) {
            D_print("enemy is False i = "); D_println(i);
            validCylinder[counter] = cylinders[i];
            counter++;
        }
    }
    printBallPositions(2, validCylinder);
    D_println("enemyCilynders are bellow");
    printBallPositions(2, enemyCylinders);
}

BallPosition getNearestValidCylinder(Color c) { //what happens if there are no cylinders to go to?
    RobotPose robertLocation = getRobotPose(RobotSelfID);
    int shortestDistance = 4000;
    struct BallPosition closestCylinder;
    closestCylinder.x = -1;
    if (capturedCylinders < 3)
    {
        for(int i = 0; i < NUM_BALLS; i++) {
            if(distanceBetween(validCylinder[i], robertLocation) < shortestDistance && validCylinder[i].hue == c && validCylinder[i].x > 0 && validCylinder[i].y > 0) {
             shortestDistance = distanceBetween(validCylinder[i], robertLocation);
             closestCylinder = validCylinder[i];
            }
        }
    }
    else
    {
        for(int i = 0; i < NUM_BALLS; i++) {
            if(distanceBetween(validCylinder[i], robertLocation) < shortestDistance && validCylinder[i].hue == GREEN && validCylinder[i].x > 0 && validCylinder[i].y > 0) {
             shortestDistance = distanceBetween(validCylinder[i], robertLocation);
             closestCylinder = validCylinder[i];
            }
        }  
    }
    return closestCylinder;
}

BallPosition getNearestCylinder(RobotPose robertLocation) {
    BallPosition cylinders[NUM_BALLS];
    int ballCount = getBallPositions(cylinders);

    BallPosition closestBall = cylinders[0];

    for (int i = 1; i < ballCount; i++) {
        if(distanceBetween(cylinders[i], robertLocation) < distanceBetween(closestBall, robertLocation));
            closestBall = cylinders[i];
    }
    return closestBall;
}

boolean isRobertHome() { //returns true if robert is within the home location
    RobotPose robertLocation = getRobotPose(RobotSelfID);
    if(Homex < 1000 && Homey < 1000) { //bottom left of the field
        if(robertLocation.x < Homex && robertLocation.y < Homey)
            return true;
        return false;
    }
    if(Homex > 1000 && Homey < 1000) { //bottom right
        if(robertLocation.x > Homex && robertLocation.y < Homey)
            return true;
        return false;
    }
    if(Homex < 1000 && Homey > 1000) { //top left
        if(robertLocation.x < Homex && robertLocation.y > Homey)
            return true;
        return false;
    }
    if(Homex > 1000 && Homey > 1000) { //top right
        if(robertLocation.x > Homex && robertLocation.y > Homey)
            return true;
        return false;
    }
    return false;
}

boolean isCylinderHome(BallPosition pose) {
    int AproxHomexPlus = Homex + 100; 
    int AproxHomeyPlus = Homey + 100; 
    int AproxHomexMinus = Homex - 100; 
    int AproxHomeyMinus = Homey - 100; 

    if(AproxHomexPlus < 1000 && AproxHomeyPlus < 1000) { //bottom left of the field
        if(pose.x < AproxHomexPlus && pose.y < AproxHomeyPlus)
            return true;
        return false;
    }
    if(AproxHomexMinus > 1000 && AproxHomeyPlus < 1000) { //bottom right
        if(pose.x > AproxHomexMinus && pose.y < AproxHomeyPlus)
            return true;
        return false;
    }
    if(AproxHomexPlus < 1000 && AproxHomeyMinus > 1000) { //top left
        if(pose.x < AproxHomexPlus && pose.y > AproxHomeyMinus)
            return true;
        return false;
    }
    if(AproxHomeyMinus > 1000 && AproxHomeyMinus > 1000) { //top right
        if(pose.x > AproxHomeyMinus && pose.y > AproxHomeyMinus)
            return true;
        return false;
    }
    return false;
}

//checks to see if there are 3 unkown color cylinders in our home base
boolean whichColorToTarget() {
    BallPosition cylinders[NUM_BALLS];
    int ballCount = getBallPositions(cylinders);
    int count = 0;
    for(int i = 0; i < ballCount; i++) {
        if(isCylinderHome(cylinders[i]) && cylinders[i].hue == 0);
            count++;
    }
    if(count > 3)
        return true;
    return false;
}

