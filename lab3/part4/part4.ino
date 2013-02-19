#include <Servo.h>
#include "data.h"


#define LEFT 0
#define RIGHT 1

#define NONE 0
#define FORWARD 1
#define AFT 2

#define NO_VELOCITY ((uint8_t)90)
#define FORWARD_VELOCITY ((uint8_t)75)
#define AFT_VELOCITY ((uint8_t)105)

#define LEFT_TUNE ((int8_t)0)
#define RIGHT_TUNE ((int8_t)-3)

uint8_t count = 0;

Wheel left;
Wheel right;

void setup()
{
  Serial.begin(9600);
  
  left.reversed = true;
  left.tuneValue = 0;
  left.pin = 10;
  initialize(&left);
  
  right.reversed = false;
  right.tuneValue = -3;
  right.pin = 6;
  initialize(&right);
  
  //turnWheel(&left, FORWARD);
  //turnWheel(&right, FORWARD);
}

void loop()
{
  if(Serial.available())
  {
    char val = Serial.read();
    if(val == 'u')
    {
      //right.motor.write(NO_VELOCITY + ++count);
      count++;
    }
    else if(val == 'd')
    {
      //right.motor.write(NO_VELOCITY + --count);
      count--;
    }
    else if(val == 'g')
    {
     turnWheel(&right, FORWARD);
     delay(count * 100);
     turnWheel(&right, NONE); 
    }
    else if(val == 'b')
    {
     turnWheel(&right, AFT);
     delay(count * 100);
     turnWheel(&right, NONE); 
    }
    Serial.print("Count: ");
    Serial.println(count);
  }
  
  /*if(Serial.read() == 'l')
  {
    rightWheel.write(80);
    delay(1000);
    rightWheel.write(90);
  }
  else if(Serial.read() == 'r')
  {
    
  }*/
  /*turnWheel(&leftWheel, LEFT, FORWARD);
  delay(2000);
  turnWheel(&leftWheel, LEFT, NONE);
  delay(2000);
  turnWheel(&leftWheel, LEFT, AFT);
  delay(2000);
  turnWheel(&leftWheel, LEFT, NONE);
  delay(2000);
  
  turnWheel(&rightWheel, RIGHT, FORWARD);
  delay(2000);
  turnWheel(&rightWheel, RIGHT, NONE);
  delay(2000);
  turnWheel(&rightWheel, RIGHT, AFT);
  delay(2000);
  turnWheel(&rightWheel, RIGHT, NONE);
  delay(2000);*/
}

void initialize(Wheel *wheel)
{
  wheel->motor.attach(wheel->pin);
  wheel->motor.write(NO_VELOCITY + wheel->tuneValue);
}

void turnWheel(Wheel *wheel, uint8_t direction)
{
  uint8_t newState = NO_VELOCITY;
  if(direction == FORWARD)
  {
    newState = wheel->reversed ?
      AFT_VELOCITY + wheel->tuneValue :
      FORWARD_VELOCITY + wheel->tuneValue; 
  }
  else if(direction == AFT)
  {
    newState = wheel->reversed ?
      FORWARD_VELOCITY + wheel->tuneValue :
      AFT_VELOCITY + wheel->tuneValue;
  }
  else
  {
    newState = NO_VELOCITY + wheel->tuneValue; 
  }
  wheel->motor.write(newState);
  wheel->state = direction;
}
