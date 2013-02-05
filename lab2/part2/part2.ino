// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 8;     // the number of the pushbutton pin
const int ledPin =  7;      // the number of the LED pin

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState = LOW;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
uint32_t lastDebounceTime = 0;  // the last time the output pin was toggled
uint32_t debounceDelay = 100;    // the debounce time; increase if the output flickers

boolean fallingEdge = false;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(buttonPin, HIGH);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
  //Init counter1
  TCCR1A = 0; //Normal mode 0xffff top, does not roll
  TCCR1B = (1 << CS12) | (0 << CS11) | (1 << CS10);//; //Clock T1 falling edge
  //Set counter to zero, high byte first
  TCNT1H = 0;
  TCNT1L = 0;
  ICR1 = 0;
  Serial.println("Finished setup");;
   
   
}
uint8_t count = 0;
void loop() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);
  //Serial.print("ICR1: ");
  //Serial.println(ICR1);

  // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

    //Check a transition as falling edge
    if(reading == LOW && lastButtonState == HIGH){
      // reset the debouncing timer
      lastDebounceTime = millis();
      fallingEdge = true; // Set variable that monitor falling edge
      
    }
 
  if(ICR1 != 0)
  {
    Serial.print("Bounce of ");
    uint32_t bounce = ICR1;
    Serial.print(bounce * 64); //We must use this level of prescaler because bounce values are consistently above 16000
    Serial.println(" microseconds.");
    TCNT1H = 0; // Set TCNT1 and ICR1 to 0
    TCNT1L = 0;
    ICR1 = 0; 
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    buttonState = reading;
    if(reading == LOW && fallingEdge){
      fallingEdge = false;
      Serial.println(++count);
      if(ICR1 != 0){ // If there was a bounce
       
      }
    }
  }
  
  // set the LED using the state of the button:
  //digitalWrite(ledPin, buttonState);

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
}

