

#define BUTTON 3
#define LED 5

boolean isPressed = false;
uint16_t pressedTime = 0;

SIGNAL(INT1_vect)
{
  if(!isPressed)
  {
    isPressed = true;
    digitalWrite(LED, HIGH);
    pressedTime = millis();
    EICRA = (1 << ISC11) | (1 << ISC10);
  }
  if(isPressed && millis() - pressedTime > 20)
  {
    isPressed = false;
    digitalWrite(LED, LOW);
    EICRA = (1 << ISC11);
  }
}


void setup()
{
  pinMode(BUTTON, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(BUTTON, HIGH);
  digitalWrite(LED, LOW);
  
  EICRA = (1 << ISC11);
  EIMSK |= (1 << INT1);
  
}

void loop()
{
  /*uint16_t time = millis();
  if(time - pressedTime > 50 && isPressed && digitalRead(BUTTON) == HIGH)
  {
    digitalWrite(LED, LOW);
    isPressed = false;
  }*/
  
}
