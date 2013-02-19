/*
 * CSCE236 Embedded Systems Spring
 * Carrick Detweiler
 */

#include <avr/io.h>

// Pin 13 has an LED connected on most Arduino boards:
#define BOARD_LED 13
#define RED 8
#define GREEN 9
#define BLUE 10

#define BUTTON 5

/**
 * Init all of the LEDs and test them
 **/ 
void LEDInit(){
  Serial.begin(9600);
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
  digitalWrite(RED,LOW);
  digitalWrite(GREEN,HIGH);
  delay(200);
  digitalWrite(GREEN,LOW);
  digitalWrite(BLUE,HIGH);
  delay(200);
  digitalWrite(BLUE,LOW);
  Serial.println("done.");
  }

void setup() {                
  Serial.begin(9600);
  Serial.println("Starting up.");
  LEDInit();
  pinMode(BUTTON, INPUT);  
  //Enable pullup  
  digitalWrite(BUTTON, HIGH);  
  
  TCCR1A = 0; //Clear register
  TCCR1A |= (1 << WGM11);
  TCCR1A |=   (1 << COM1B1) | (0 << COM1B0); //Enable fast PWM
  TCCR1B = 0;
  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  TCCR1B = (1 << CS12) | (0 << CS11) | (0 << CS10); //Use clk/256 prescaler
  TCCR1C = 0;
  
  ICR1 = 64;
  OCR1B = 10;
    
  Serial.println("Finished setup");
}


void loop() {
  //Button Pressed
  if((digitalRead(BUTTON) == LOW)){
    //Turn LED off briefly to dim
    OCR1B = 16;
  }
  else
  {
   OCR1B = 63; 
  }
  
  //Turn on LED
  //digitalWrite(RED,HIGH);
  //Delay here so that when the button is pressed there
  //will be some time that the LED is kept on
  //delay(10);
  
  //delay(100);
}


