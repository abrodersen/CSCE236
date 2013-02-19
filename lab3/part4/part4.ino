#include <Servo.h>
#include "data.h"

#define NONE 0
#define FORWARD 1
#define AFT 2

#define NO_VELOCITY ((uint8_t)90)
#define FORWARD_VELOCITY ((uint8_t)75)
#define AFT_VELOCITY ((uint8_t)105)

#define TASK_LIST_SIZE 20

Transition taskList[TASK_LIST_SIZE];



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
