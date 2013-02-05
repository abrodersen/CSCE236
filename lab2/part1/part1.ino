#include<avr/io.h>

#define BOARD_LED 13
#define RED 7
#define GREEN 9
#define BLUE 10

#define BUTTON 5

uint16_t previous = 0;
uint16_t count = 0;

/**
 * Init all of the LEDs and test them
 **/ 
void LEDInit(){
  pinMode(BOARD_LED, OUTPUT);     
  pinMode(RED, OUTPUT);     
  pinMode(GREEN, OUTPUT);     
  pinMode(BLUE, OUTPUT);     

   //Turn all off
  digitalWrite(RED,LOW);
  digitalWrite(GREEN,LOW);
  digitalWrite(BLUE,LOW);

  //Test LEDs
  Serial.print("Testing LEDs...");
  digitalWrite(RED,HIGH);
  delay(200);
  digitalWrite(GREEN,HIGH);
  delay(200);
  digitalWrite(BLUE,HIGH);
  delay(200);
  Serial.println("done.");
  
   //Turn all off
  digitalWrite(RED,LOW);
  digitalWrite(GREEN,LOW);
  digitalWrite(BLUE,LOW);
}

void setup() {                
  Serial.begin(9600);
  Serial.println("Starting up.");
  LEDInit();
  pinMode(BUTTON, INPUT);     
  
  //Init counter1
  TCCR1A = 0; //Normal mode 0xffff top, does not roll
  TCCR1B = (1 << CS12) | (1 << CS11); //Clock T1 falling edge
  //Set counter to zero, high byte first
  TCNT1H = 0;
  TCNT1L = 0;  
  
  Serial.println("Finished setup");
}

uint16_t lightTime = 0;
uint16_t lastTime = 0;

void loop(){
  if(TCNT1 > previous){
    delay(100);
    if(TCNT1 > previous + 1)
    {
     Serial.println("Bounce!");
     digitalWrite(RED, HIGH);
     delay(500);
     digitalWrite(RED, LOW);
    }
    count++;
    Serial.print("Count: ");
    Serial.println(count);
    
    previous = TCNT1;
    
  }
}
