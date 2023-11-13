#include "color.h"

const int sensorPin = 39;      // select the analog input pin for the photoresistor IO/ADC
const int bluePin = 21;        // select the digital output pin for blue LED
const int greenPin = 22;      // select the digital output pin for green LED 
const int redPin = 23;        // select the digital output pin for red LED

int maxblack[]={1744,1479,1990};//{1857,1736,2491}; // UPDATE the max reading obtained from a black surface for {RED,GREEN,BLUE}
int minwhite[]={1415,1469,2120};//{1392,1216,1986}; // UPDATE the min reading obtained from a white surface for {RED,GREEN,BLUE}

int color[]={0,0,0}; // array for the readings

// This function is used to read each of the colors individually. You will use the results of this
// function run on a pure white and pure black object to fill in the expected ranges in the 
// maxblack and minwhite functions arrays above.

void setupColorSensor() {
  // Declare the LED pins as an OUTPUT:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

// void calibrate_Black_Color() 
// {
//   Change_Color(HIGH,LOW,LOW);
//   delay(1000);
//   maxblack[0] = read_light_sensor();
//   Change_Color(LOW,LOW,LOW);
//   delay(1000);
//   Change_Color(LOW,HIGH,LOW);
//   delay(1000);
//   maxblack[1] = read_light_sensor();
//   Change_Color(LOW,LOW,LOW);
//   delay(1000);
//   Change_Color(LOW,LOW,HIGH);
//   delay(1000);
//   maxblack[2] = read_light_sensor();
//   Change_Color(LOW,LOW,LOW);
//   delay(1000);
// }

void calibration_debugging()
{
  servo3.write(50);
  int summer[] = {0,0,0};
  int sample = 50;
  Change_Color(HIGH,LOW,LOW);
  delay(1000);
  for(int i = 0; i< sample; i++)
  {
    summer[0] = summer[0] + read_light_sensor();
  }
  delay(1000);
  Change_Color(LOW,HIGH,LOW);
  delay(1000);
  for(int i = 0; i< sample; i++)
  {
    summer[1] = summer[1] + read_light_sensor();
  }
  delay(1000);
  Change_Color(LOW,LOW,HIGH);
  delay(1000);
  for(int i = 0; i< sample; i++)
  {
    summer[2] = summer[2] + read_light_sensor();
  }
  delay(1000);
  Change_Color(LOW,LOW,LOW);
  summer[0] = summer[0] / sample;
  summer[1] = summer[1] / sample;
  summer[2] = summer[2] / sample;
  for(int index = 0; index<3; index++)
  {
    Serial.println(summer[index]);
  }
  servo3.write(90);
}

// void calibrate_White_Color() 
// {
//   Change_Color(HIGH,LOW,LOW);
//   delay(2000);
//   minwhite[0] = read_light_sensor();
//   Change_Color(LOW,LOW,LOW);
//   delay(1000);
//   Change_Color(LOW,HIGH,LOW);
//   delay(1000);
//   minwhite[1] = read_light_sensor();
//   Change_Color(LOW,LOW,LOW);
//   delay(1000);
//   Change_Color(LOW,LOW,HIGH);
//   delay(1000);
//   minwhite[2] = read_light_sensor();
//   Change_Color(LOW,LOW,LOW);
//   delay(1000);
// }

void Change_Color(bool RedColor,bool GreenColor,bool BlueColor)
{
  //int sensorValue = 0;

  digitalWrite(redPin, RedColor); //red ON
  digitalWrite(greenPin, GreenColor);//green OFF
  digitalWrite(bluePin, BlueColor);//blue OFF

}
int read_light_sensor()
{
  int sensorValue = 0;
  sensorValue = analogRead(sensorPin); // read the photoresistor value
  Serial.println("Sensor Value: " + String(sensorValue)); // output the reading
  return sensorValue;
}

// int Read_Calibrated_color()
// {
//   int red = 0;
//   int blue = 0;
//   int sum = 0;
//   int sample_size = 20;
//   Change_Color(HIGH,LOW,LOW);
//   delay(1500);
//   for (int i = 0; i< sample_size ; i++)
//   {
//     sum = sum + read_light_sensor();
//   }
//     int avg_color = sum/sample_size;
//     if (abs(1750 - avg_color) < abs(2556- avg_color)) /// on the calibration screen this are the top for the three values the left side here is from red cylinder and right one is blue cylinder
//     {
//       red++;
//     }
//     else 
//     {
//       blue++;
//     }
//   int red_avg = avg_color;
//   sum = 0;
//   Change_Color(LOW,LOW,LOW);
//   Change_Color(LOW,LOW,HIGH);
//   delay(1500);
//   for (int i = 0; i< sample_size ; i++)
//   {
//     sum = sum + read_light_sensor();
//   }
//   avg_color = sum/sample_size;
//   if (abs(2658 - avg_color) < abs(2437 - avg_color))   /// on the calibration screen this are the bottom for the three values the left side here is from red cylinder and right one is blue cylinder
//   {
//     red++;
//   }
//   else 
//   {
//     blue++;
//   }
//   Change_Color(LOW,LOW,LOW);
//   int blue_avg = avg_color;
//   Serial.println(red);
//   Serial.println(blue);
//   if (red == blue)
//   {
//     D_println("color sensor returns 3");
//     return 3;
//   }
//   else if (red > blue)
//   {
//     return 0;
//   }
//   else
//   {
//     return 1;
//   }
// }

int Read_Calibrated_color2() { // this is a simplifed version of Read_Calibrated_color() which gives more consistant results
  int red = 2018; //1850
  int blue = 2814; //
  int sample_size = 10;

  Change_Color(HIGH,LOW,LOW);
  delay(1500);

  int sum = 0;
  for (int i = 0; i < sample_size; i++)
    sum = sum + read_light_sensor();

  int avg_color = sum/sample_size;

  Change_Color(LOW,LOW,LOW);

  if(abs(avg_color - red) < abs(avg_color - blue))
    return 0; //red
  else
    return 1; //blue

}

// void map_input_color(int rgb)
// {
//   int sensor_ouput = analogRead(sensorPin); // read the photoresistor value
//   color[rgb]=constrain(sensor_ouput,minwhite[rgb],maxblack[rgb]); //constrain the reading such that it is between the white and black values
//   color[rgb]=map(color[rgb],maxblack[rgb],minwhite[rgb],0,100); // map the reading between 0 and 100 such that black is 0, 100 is white
// }

// int readCalibratedColor()
// {
//   // This function will use the min and max values you found in the previous calibration step to scale the RGB values
//   // Turn on just the red LED
//   Change_Color(HIGH,LOW,LOW); // changes color to red
//   delay(1000); // wait for the photresistor value to settle
//   map_input_color(0); // maps input reading to scale of 0-100 red reflected light

//   Change_Color(LOW,HIGH,LOW); // changes color to green
//   delay(1000); // wait for the photresistor value to settle
//   map_input_color(1); // maps input reading to scale of 0-100 red reflected light
  
//   Change_Color(LOW,LOW,HIGH); // changes color to blue
//   delay(1000); // wait for the photresistor value to settle
//   map_input_color(2); // maps input reading to scale of 0-100 red reflected light
//   Change_Color(LOW,LOW,LOW);

//   //prints out amount of RGB light reflected
//   Serial.print("Calibrated R: "); Serial.print(color[0]);     Serial.print("   ");
//   Serial.print("G: ");            Serial.print(color[1]);     Serial.print("   ");
//   Serial.print("B: ");            Serial.print(color[2]);     Serial.println("   ");

//   int max_color = color[0];
//   int color_index = 0;
//   for (int i = 0; i < 3; i++)
//   {
//     if(color[i] > max_color)
//     {
//         max_color = color[i];
//         color_index = i;
//     }
//   }
//   return color_index;
// }

void Calibration_values()
{
  Serial.println("Calibration Values: ");
  for (int i = 0; i<3; i++)
  {
    Serial.print(maxblack[i]);
    Serial.print(", ");
  }
  Serial.println(" ");
  for (int i = 0; i<3; i++)
  {
    Serial.print(minwhite[i]);
    Serial.print(", ");
  }

}