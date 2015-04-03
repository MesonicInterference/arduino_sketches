#include <Time.h>

#include <TimeAlarms.h>

// connect LEDs to the indicated digital outputs
int green_led  = 0;
int yellow_led = 1;
int red_led    = 2;

// connect buzzer to digital output 7
int buzzer     = 7;

// a variable for the time when the sketch starts running
// it gets initialized in setup()
time_t start_time;



void setup() {                
  // initialize the digital pins as output.
  pinMode(green_led,  OUTPUT);
  pinMode(yellow_led, OUTPUT);
  pinMode(red_led,    OUTPUT);
  pinMode(buzzer,     OUTPUT);
  
  // initialize the time to now
  start_time = now();
  
  // set the system time
  setTime(start_time);
  
}


// turn an LED on, wait five minutes, then turn it off
void lightLED(int led) {
  // turn on the LED
  digitalWrite(led,HIGH);
  
  // wait for 4 minutes, 59.5 seconds
  delay(299500);         
  
  // if this is the red LED, activate the buzzer for 500 ms
  if (led == red_led) { activateBuzzer();}
  // if this isn't the red LED, wait for 500 ms
  // and finish up the loop
  else {delay(500);}
  
  // turn off the LED
  digitalWrite(led,LOW);
}


// activate the buzzer for half a second
void activateBuzzer() {
  digitalWrite(buzzer,HIGH);
  delay(500);
  digitalWrite(buzzer,LOW);
}


void loop() {
  lightLED(green_led);
  lightLED(yellow_led);
  lightLED(red_led);
}
