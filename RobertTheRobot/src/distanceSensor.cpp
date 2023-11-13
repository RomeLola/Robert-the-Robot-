#include "distanceSensor.h"

const float A_CALIBRATION_CONSTANT = 202002.6504;
const float B_CALIBRATION_CONSTANT = -1.26936599;

int irSensor1Pin = 34; 
int counter_of_5 = 0;

MedianFilter<float> medianFilter(1000);

// Sweep
// by BARRAGAN  

void setupDistanceSensor() {
  Serial.begin(115200);
  pinMode(irSensor1Pin, INPUT);
}

float convertVoltage2Distance(int medianValue)
{
  float distance = A_CALIBRATION_CONSTANT * std::pow(medianValue, B_CALIBRATION_CONSTANT);
  return distance;
}

//} 
//void loop() 
//{ 
// float median = 0.0;
//float distance = 0.0;

/// for (size_t iCount = 0; iCount < 1000; iCount++)
///	{
//   int irSensor1Value = analogRead(irSensor1Pin); 
//    median = medianFilter.AddValue(irSensor1Value);
//  }

/// Serial.print("IR1 Median: "); 
// Serial.print(median); 

// Serial.println();

// Serial.print("IR1 Distance: ");
// distance = convertVoltage2Distance(median);
// Serial.print(distance);


// Serial.println();
// Serial.println();
// Serial.println();

// delay(1000);

//}

float read_distance() 
{
 float median = 0.0;
 float distance = 0.0;

 for (size_t iCount = 0; iCount < 1000; iCount++)
	{
    int irSensor1Value = analogRead(irSensor1Pin); 

    median = medianFilter.AddValue(irSensor1Value);
  }

  distance = convertVoltage2Distance(median);
  Serial.print("Distance in cm: ");
  Serial.println(distance);
}
