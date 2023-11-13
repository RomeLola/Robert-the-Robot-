//Lewis - Calculates the required wheel speed to reach the target and
#include "pathFinder.h"

double pathDistanceGain = 0.00175;
double pAngleGain = .7;
double dAngleGain = 1;
double maxSpeed = 0;
double minSpeed = 1000;
double maxAngle = 0;
double minAngle = 1000;
boolean isRobertOnTheField = false;
double lastErrorAngle = 0;
double changeInAngleError = 0;

const int approachModeDistence = 400; //mm away from target at which the robot will slow down as it approaches

int robertReliability = 0; //positive means he is on the field, negative means he is not

double getDistanceError(int xTarget, int yTarget) {
    struct RobotPose robertLocation = getRobotPose(RobotSelfID);
    isRobertOnTheField = robertLocation.valid;
    //D_print("GetDistanceError Called: returned "); D_println(sqrt(pow(robertLocation.x-xTarget, 2)+pow(robertLocation.y-yTarget, 2)));
    return sqrt(pow(robertLocation.x-xTarget, 2)+pow(robertLocation.y-yTarget, 2));
}

double getAngleError(int xTarget, int yTarget){      //bounded within -pi and pi
    struct RobotPose robertLocation = getRobotPose(RobotSelfID);
    
    double thetaR = robertLocation.theta*0.001; 

    double thetaD = atan2(yTarget-robertLocation.y, xTarget-robertLocation.x);
    double thetaE = thetaD - thetaR;

    changeInAngleError = thetaE-lastErrorAngle;
    lastErrorAngle = thetaE;

    D_print("getAngleError Called: Robert's Angle "); D_print(thetaR); D_print(" ThetaD "); D_print(thetaD); D_print(" ThetaE "); D_println(thetaE);
    //D_print("Yt-Yr "); D_print(yTarget-robertLocation.y); D_print(" Xt-Xr"); D_println(xTarget-robertLocation.x);

    if(thetaE > 3.14) {
        //D_println("OUT OF BOUNDS. Theta > pi.");
        return thetaE - 6.28;
    }
    if(thetaE < -3.14) {
        //D_println("OUT OF BOUNDS. Theta < -pi.");
        return thetaE + 6.28;
    }

    return thetaE;
}


double getRightWheelSpeed(int xTarget, int yTarget) { //returns normalized speed between -1 and 1.
        double distError = getDistanceError(xTarget, yTarget);
        double speed = 0;
    if (distError > approachModeDistence) 
        speed = (1 + pAngleGain*getAngleError(xTarget, yTarget) + dAngleGain*changeInAngleError)*0.5;
    else
        speed = (pathDistanceGain*distError + pAngleGain*getAngleError(xTarget, yTarget) + dAngleGain*changeInAngleError)*0.5;
    if(detectLackOfRobertOnField() == true) {
        //D_print("robertReliablitly = "); D_println(robertReliability);
        return speed;
    }
    else
        return 0;
}

double getLeftWheelSpeed(int xTarget, int yTarget) {  //returns normalized speed between -1 and 1.
    double distError = getDistanceError(xTarget, yTarget);
    double speed = 0;
    if (distError > approachModeDistence) 
        speed = (1 - pAngleGain*getAngleError(xTarget, yTarget) + dAngleGain*changeInAngleError)*0.5;
    else
        speed = (pathDistanceGain*distError - pAngleGain*getAngleError(xTarget, yTarget) + dAngleGain*changeInAngleError)*0.5;
    if(detectLackOfRobertOnField() == true) {
        //D_print("detectLackOfRobertOnField returned "); D_println(detectLackOfRobertOnField());
        return speed;
    }
    else
        return 0;
}

double approachTarget(int xTarget, int yTarget) {
    rotateLeftWheel(getLeftWheelSpeed(xTarget,yTarget));
    rotateRightWheel(getRightWheelSpeed(xTarget,yTarget));
    return getDistanceError(xTarget,yTarget);
}

void goToTarget(int xTarget, int yTarget, int stopRadius) {
    //boolean clearPath;
    //BallPosition obstacle;
    int distError;

    //use isPathObstructed() to determine the location of a cylinder obstruction
    //use avoidObstruction() to determine which side to drive to to avoid the obstruction

    do
    {
        distError = approachTarget(xTarget, yTarget);
        /********************************/
        if (isRobertStopped)
        {
            state = AvoidObstacle; 
            break;
        }
        /********************************/
    } while(distError > stopRadius);  
}

boolean detectLackOfRobertOnField() {
    int absentBufferSize = -2; 
    int presentBufferSize = 2; //increase this value if you see studders due to wifi data dropping
    if(isRobertOnTheField == true && robertReliability < presentBufferSize)
        robertReliability++;

    else if(isRobertOnTheField == false && robertReliability > absentBufferSize)
        robertReliability--;
    
    if(robertReliability < 0) {
        //D_println("Donde esta Roberto?????");
        return false;
    }
    else
        return true;
}

int getRelativeLocation(int xTarget, int yTarget, RobotPose robertLocation) {
    int xDistance = xTarget-robertLocation.x;
    int yDistance = yTarget-robertLocation.y;
    int proximityCutoff = 15;

    if(xDistance > 0 && yDistance > 0) {//Bottom Left
        //D_println("Bottom Left");
        return 1;
    }
    if(xDistance > 0 && yDistance < 0) {//Top Left
        //D_println("Top Left");
        return 2;
    }
    if(xDistance <0 && yDistance < 0) {//Top Right
        //D_println("Top Right");
        return 3;
    }
    else{                              //Bottom Right
        //D_println("Bottom Right");
        return 4;
    }
}

double getRightWheelSpeed_angledturn(double targetAngle) { //returns normalized speed between -1 and 1.
    struct RobotPose robertLocation = getRobotPose(RobotSelfID);
    double thetaR = robertLocation.theta*0.001;

    double speed = 0;

    speed = (1 + pAngleGain * (thetaR + targetAngle) + dAngleGain*changeInAngleError)*0.5;

    if(detectLackOfRobertOnField() == true)
        return speed;
    else
        return 0;
}

double getLeftWheelSpeed_angledturn(double targetAngle) {  //returns normalized speed between -1 and 1.
    struct RobotPose robertLocation = getRobotPose(RobotSelfID);
    double thetaR = robertLocation.theta*0.001;

    double speed = 0;

    speed = (1 - pAngleGain * (thetaR + targetAngle) + dAngleGain*changeInAngleError)*0.5;

    if(detectLackOfRobertOnField() == true) {
        //D_print("detectLackOfRobertOnField returned "); D_println(detectLackOfRobertOnField());
        return speed;
    }
    else
        return 0;
}

double turnrobottoward(double targetAngle) {
            rotateLeftWheel(getLeftWheelSpeed_angledturn(targetAngle));
            rotateRightWheel(getRightWheelSpeed_angledturn(targetAngle));
            //delay(100);
            return 0;
}

//Returns true if there is a cylinder in the path of the robot, else false
//Updates BallPosition & Obstacle with the location of the obstacle so that it can be avoided
//The function looks along the streightline path between the robot and the target and attempts to locate any cylinders in close proximity to the path;
boolean isPathObstructed(int xTarget, int yTarget, BallPosition &obstacle) { 
    struct RobotPose robertLocation = getRobotPose(RobotSelfID);
    BallPosition cylinders[NUM_BALLS];
    int ballCount = getBallPositions(cylinders);
    
    int detectionRadius = 50; //how large is the radius used to check for nearby cylinders
    int detectionCirclesPerMeter = 10; //this is a gain used to control how many detection circles are used along the path per meter.

    int distanceToTarget = getDistanceError(xTarget,yTarget);
    if(distanceToTarget < 200)
        return false;
    
    int checkQuantity = distanceToTarget * detectionCirclesPerMeter / 1000;
    int xChange = (xTarget - robertLocation.x) / checkQuantity; // this determines how fine the resolution is for the line check
    int yChange = (yTarget - robertLocation.y) / checkQuantity;
    RobotPose pointOnPath;
    BallPosition closestCylinder;

    int x = robertLocation.x;
    int y = robertLocation.y;

    for(int i = 0; i < checkQuantity; i++) {
        x = x + xChange;
        y = y + yChange;

        pointOnPath.y = (yTarget-robertLocation.y)/(xTarget-robertLocation.x)*(x -robertLocation.x)+robertLocation.y;
        pointOnPath.x = (xTarget-robertLocation.x)/(yTarget-robertLocation.y)*(y -robertLocation.y)+robertLocation.x;
        
        closestCylinder = getNearestCylinder(pointOnPath);
        if(distanceBetween(closestCylinder, pointOnPath) < detectionRadius) {
            D_println("Obstacle Detected!");
            obstacle.x = closestCylinder.x;
            obstacle.y = closestCylinder.y;
            return true;
        }
    }
    return false;
}

//give it a obstacle, returns a ballposition to drive to
//it will look to the left and right of the obstacle and determine which side to drive to.
//if there is another obstacle nearby it will give you a position exactly between the two to allow you to thread the needle

// BallPosition avoidObstruction(BallPosition obstacle) {

// }

//will move around the cylinder geometry may be off will tweak it later
void avoiddroppedcylinder()
{
    updateValidCylinderArray();
    BallPosition nextball = getNearestValidCylinder(targetColor);
    double angleError = getAngleError(nextball.x, nextball.y);
    D_print("ball pose");
    D_println(nextball.x); D_println(nextball.y);
    if (abs(angleError) < .5) {
        if (angleError < 0)
        {
            rotateLeftWheel(1);
            D_println("Turning Right");
        }
        else
        {
            rotateRightWheel(1);
            D_println("Turning Right");
        }

        delay(350);
        rotateLeftWheel(1);
        rotateRightWheel(1);
        delay(1800);
        rotateLeftWheel(0);
        rotateRightWheel(0);
    }
}