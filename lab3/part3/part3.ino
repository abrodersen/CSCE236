#include <Servo.h>

Servo test;
const uint8_t button = 7;

void setup()
{
  test.attach(9);
  pinMode(button, INPUT);
  digitalWrite(button, HIGH);
}

void loop()
{
  uint8_t state = digitalRead(button);
  if(state == HIGH)
  {
   test.write(135); 
  }
  else
  {
   test.write(45); 
  }
}
