/*************************************************************************
* File Name          : wendyjan_2015-12-04_Hand_Control
* Author             : Wendy Jansson
* Credit to          : Jasen (Makeblock programmer) for sample code 
*                      to use Makeblock.h
* Version            : V1.0.0
* Date               : 2015-12-04
* Description        : Allows a human to control the robot every few feet by swiping:
*                       -> Stand behind robot.
*                       -> Make sure your hand travels in front of the ultrasonic sensor, which looks like his eyes.
*                       -> He will travel a second or two, based on the direction you give him:
*                           Turn left then go forward: Swipe towards him.
*                           Turn right then go forward: Swipe away from him (it helps to swipe down diagonally away from him).
*                           Back: Hold your hand for 2 seconds in front of him.
**************************************************************************/
#include <Makeblock.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <stdlib.h>

// Motors and sensors in the robot. 
MeDCMotor MotorL(M1);  
MeDCMotor MotorR(M2);
// US sensor could also go on 3, 4, 6, 7, 8 if robot was rewired.
MeUltrasonicSensor UltrasonicSensor(PORT_3);
// Robot's speed.
int moveSpeed = 80;
int turnSpeed = 80;
// Info about the swipe direction.
int d1, d2; 
int gesture_recog_cutoff = 35;
int gesture_pause_cutoff = 7;
// Length of time robot will travel after command.
int MOVE_TIME = 40;
int TURN_TIME = 30;
int move_count;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
  d1 = UltrasonicSensor.distanceCm();
  if (d1 > 0 && d1 < gesture_recog_cutoff) {
    delay(100); // 100 mS is min time delay of this sensor.
    d2 = UltrasonicSensor.distanceCm();
    if (0 < d1 && d1 < gesture_recog_cutoff*1.5 && 0 < d2 && d2 < gesture_recog_cutoff*1.5) {
      if (abs(d1 - d2) < gesture_pause_cutoff) {
        Serial.println("Back up!");
        Backward();
      }
      else if (d1 < d2) { 
        Serial.println("Go right!"); 
        MoveLeft();
      }
      else { 
        Serial.println("Go left!");
        MoveRight();
      }
    d1 = 0;
    d2 = 0;
    }
  }
}

void MoveRight()
{
  Serial.println("Roger right");
  move_count = 0;
  while (move_count++ < TURN_TIME) {
    delay(50);
    MotorL.run(-moveSpeed);
    MotorR.run(moveSpeed);
   }
  Forward();
}
void MoveLeft()
{
  Serial.println("Roger left");
  move_count = 0;
  while (move_count++ < TURN_TIME) {
    delay(50);
    MotorL.run(moveSpeed);
    MotorR.run(-moveSpeed);
   }
   Forward();
}
void Backward()
{
  Serial.println("Roger back");
  move_count = 0;
  while (move_count++ < MOVE_TIME) {
    delay(50);
    MotorL.run(-moveSpeed);
    MotorR.run(-moveSpeed);
   }
   Stop();
}
void Forward()
{
  move_count = 0;
  while (move_count++ < MOVE_TIME) {
    delay(50);
    MotorL.run(moveSpeed);
    MotorR.run(moveSpeed);
   }
  Stop();
}
void Stop()
{
  MotorL.run(0);
  MotorR.run(0);
}

