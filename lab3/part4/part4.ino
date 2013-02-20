#include <Servo.h>
#include <EEPROM.h>

#define TASK_LIST_SIZE 20
#include "data.h"

#define NONE 0
#define FORWARD 1
#define AFT 2

#define NO_VELOCITY ((uint8_t)90)
#define FORWARD_VELOCITY ((uint8_t)75)
#define AFT_VELOCITY ((uint8_t)105)

#define LEFT_MEMORY_OFFSET 0
#define RIGHT_MEMORY_OFFSET 3

#define BUTTON 3


TaskList taskList;

uint8_t count = 0;

Wheel left;
Wheel right;

boolean tasksEnabled = false;

void setup()
{
  Serial.begin(9600);
  
  pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON, HIGH);
  
  left.reversed = true;
  left.noneTuneValue = 0;
  left.forwardTuneValue = 0;
  left.aftTuneValue = 0;
  left.pin = 10;
  
  readWheelCalibration(&left);
  
  initialize(&left);
  
  
  right.reversed = false;
  right.noneTuneValue = -1;
  right.forwardTuneValue = 0;
  right.aftTuneValue = 0;
  right.pin = 6;
  
  readWheelCalibration(&right);
  
  initialize(&right);
  
  printWheelInfo(&left);
  printWheelInfo(&right);
  
  taskList.numTasks = 0;
  
  addTask(&taskList, &left, FORWARD, 100);
  addTask(&taskList, &right, FORWARD, 100);
  addTask(&taskList, &left, NONE, 2100);
  addTask(&taskList, &right, NONE, 2100);
  addTask(&taskList, &left, AFT, 2200);
  addTask(&taskList, &right, AFT, 2200);
  addTask(&taskList, &left, NONE, 4200);
  addTask(&taskList, &right, NONE, 4200);
    
  for(int i = 0; i < taskList.numTasks; i++)
  {
    Serial.print("Added task: Time: ");
    Serial.print(taskList.times[i]);
    Serial.print(" Direction: ");
    Serial.println(taskList.directions[i]);
  }

}

uint16_t previousTime = 0;
boolean buttonPressed = false;

uint16_t previousTaskTime = 0;
uint16_t currentTaskTime = 0;

Wheel *currentAdjustable = &right;
boolean adjustingForward = true;

void loop()
{
  uint8_t state = digitalRead(BUTTON);
  uint16_t currentTime = millis();
  if(state == LOW && !buttonPressed && (currentTime - previousTime) > 100)
  {
    previousTime = millis();
    buttonPressed = true;
    Serial.println("Pressed");
  }
  
  if(state == HIGH && buttonPressed && (currentTime - previousTime) > 100)
  {
    previousTime = millis();
    buttonPressed = false;
    Serial.println("Released");
    tasksEnabled = !tasksEnabled;
    if(tasksEnabled)
    {
      Serial.println("Tasks enabled.");
    }
    else
    {
      turnWheel(&left, NONE);
      turnWheel(&right, NONE);
    }
  }
  
  if(tasksEnabled)
  {
    processTasks(&taskList, previousTaskTime, currentTaskTime);
  }
  
  if(Serial.available())
  {
    char k = Serial.read();
    int8_t value = 0;
    switch(k)
    {
      case 'l':
        currentAdjustable = &left;
        Serial.println("Left wheel now adjustable.");
        break;
      case 'r':
        currentAdjustable = &right;
        Serial.println("Right wheel now adjustable.");
        break;
      case 'f':
        adjustingForward = true;
        Serial.println("Forward velocity now adjustable.");
        break;
      case 'a':
        adjustingForward = false;
        Serial.println("Aft velocity now adjustable.");
        break;
      case 'u':
        if(adjustingForward)
        {
          value = ++(currentAdjustable->forwardTuneValue);
        }
        else
        {
          value = ++(currentAdjustable->aftTuneValue);
        }
        Serial.print("Current adjustment: ");
        Serial.println(value);
        writeWheelCalibration(currentAdjustable);
        break;
      case 'd':
        if(adjustingForward)
        {
          value = --(currentAdjustable->forwardTuneValue);
        }
        else
        {
          value = --(currentAdjustable->aftTuneValue);
        }
        Serial.print("Current adjustment: ");
        Serial.println(value);
        writeWheelCalibration(currentAdjustable);
        break;
        
    }
  }
  
  previousTaskTime = currentTaskTime;
  currentTaskTime = millis();
  
  /*turnWheel(&right, FORWARD);
  delay(1500);
  turnWheel(&right, NONE);
  delay(100);
  turnWheel(&right, FORWARD);
  turnWheel(&left, FORWARD);
  delay(2000);
  turnWheel(&right, NONE);
  turnWheel(&left, NONE);
  delay(100);*/
}

void processTasks(TaskList *list, uint16_t previousTime, uint16_t currentTime)
{
  if(list->numTasks > 0)
  {
    uint16_t lastTime = list->times[list->numTasks - 1];
    uint16_t previousAdjusted = previousTime % lastTime;
    uint16_t currentAdjusted = currentTime % lastTime;
    for(int i = 0; i < list->numTasks; i++)
    {
      if(currentAdjusted >= previousAdjusted)
      {
        if(list->times[i] < currentAdjusted && list->times[i] >= previousAdjusted)
        {
          turnWheel(list->wheels[i], list->directions[i]);
          Serial.print("New direction: ");
          Serial.println(list->directions[i]);
        }
      }
      else
      {
        if(list->times[i] < currentAdjusted || list->times[i] >= previousAdjusted)
        {
          turnWheel(list->wheels[i], list->directions[i]);
          Serial.print("New direction: ");
          Serial.println(list->directions[i]);
        }
      }
    }
  }
  
}

void addTask(TaskList *list, Wheel* wheel, uint8_t direction, uint16_t time)
{
  if(list->numTasks == 0)
  {
    list->times[0] = time;
    list->wheels[0] = wheel;
    list->directions[0] = direction;
    list->numTasks++;
  }
  else if(list->numTasks < TASK_LIST_SIZE)
  {
    for(int i = list->numTasks - 1; i >= 0; i--)
    {
      if(list->times[i] > time)
      {
        list->times[i + 1] = list->times[i];
        list->wheels[i + 1] = list->wheels[i];
        list->directions[i + 1] = list->directions[i];
        if(i == 0)
        {
          list->times[0] = time;
          list->wheels[0] = wheel;
          list->directions[0] = direction;
          list->numTasks++;
          return;
        }
      }
      else
      {
        list->times[i + 1] = time;
        list->wheels[i + 1] = wheel;
        list->directions[i + 1] = direction;
        list->numTasks++;
        return;
      }
    }
  }
}

void initialize(Wheel *wheel)
{
  wheel->state = NONE;
  wheel->motor.attach(wheel->pin);
  wheel->motor.write(NO_VELOCITY + wheel->noneTuneValue);
}

void turnWheel(Wheel *wheel, uint8_t direction)
{
  uint8_t newState = NO_VELOCITY;
  if(direction == FORWARD)
  {
    newState = wheel->reversed ?
      AFT_VELOCITY + wheel->aftTuneValue :
      FORWARD_VELOCITY + wheel->forwardTuneValue; 
  }
  else if(direction == AFT)
  {
    newState = wheel->reversed ?
      FORWARD_VELOCITY + wheel->forwardTuneValue :
      AFT_VELOCITY + wheel->aftTuneValue;
  }
  else
  {
    newState = NO_VELOCITY + wheel->noneTuneValue; 
  }
  wheel->motor.write(newState);
  wheel->state = direction;
}

uint8_t getOffset(Wheel *wheel)
{
  if(wheel == &left)
  {
    return LEFT_MEMORY_OFFSET;
  }
  else if(wheel == &right)
  {
    return RIGHT_MEMORY_OFFSET;
  }
  return 0xff;
}

uint8_t readWheelCalibration(Wheel *wheel)
{
  uint8_t offset = getOffset(wheel);
  if(offset == 0xff)
  {
    return 1;
  }
  
  wheel->forwardTuneValue = EEPROM.read(offset);
  wheel->aftTuneValue = EEPROM.read(offset + sizeof(int8_t));
  wheel->noneTuneValue = EEPROM.read(offset + 2 * sizeof(int8_t));
  return 0;
}

uint8_t writeWheelCalibration(Wheel *wheel)
{
  uint8_t offset = getOffset(wheel);
  if(offset == 0xff)
  {
    return 1;
  }
  EEPROM.write(offset, wheel->forwardTuneValue);
  EEPROM.write(offset + sizeof(uint8_t), wheel->aftTuneValue);
  EEPROM.write(offset + 2 * sizeof(uint8_t), wheel->noneTuneValue);
  return 0;
}

void printWheelInfo(Wheel *wheel)
{
  Serial.println("Wheel information:");
  Serial.print("Pin: ");
  Serial.println(wheel->pin);
  Serial.print("Reversed: ");
  Serial.println(wheel->reversed);
  Serial.print("Forward tune value: ");
  Serial.println(wheel->forwardTuneValue);
  Serial.print("Aft tune value: ");
  Serial.println(wheel->aftTuneValue);
  Serial.print("Stopped tune value: ");
  Serial.println(wheel->noneTuneValue);
  
}
