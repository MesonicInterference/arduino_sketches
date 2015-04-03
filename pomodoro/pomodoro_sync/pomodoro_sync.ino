#include <Time.h>
#include <TimeAlarms.h>

#define TIME_HEADER 'T'

// connect LEDs to the indicated digital outputs
int green_led  = 2;
int yellow_led = 3;
int red_led    = 4;

// connect buzzer to output 5
int buzzer     = 5;

// The current minute.  Used in all of this sketch's logic.
int current_minute;

// A variable for holding a Unix time sent to the Arduino via its serial connection.
// Used to set the system time.
unsigned long pc_time;

/////////////////////////////////////////////////////////////////////////////

void setup() {  
  Serial.begin(9600);
// Wait for the serial connection.  Sidesteps a hardware quirk of the Arduino Leonardo.
  while (!Serial);

// Initialize the needed pins as output.
  pinMode(green_led,  OUTPUT);
  pinMode(yellow_led, OUTPUT);
  pinMode(red_led,    OUTPUT);
  pinMode(buzzer,     OUTPUT);

// Set the system time.  This will be overwritten if a properly-formatted
// time is received over the serial connection.
  setTime(now());

}

/////////////////////////////////////////////////////////////////////////////

// turn one LED on while turning off the others
void lightLED(int led) {


  // turn on the given LED
  digitalWrite(led,HIGH);

  // turn off the other LEDs
  for (int counter = green_led; counter <= red_led; counter++) {
    if (led != counter) { 
      digitalWrite(counter,LOW);
    }
  }
  
// turn off the board LED if it's been turned on after a sync
  digitalWrite(13,LOW);
}

/////////////////////////////////////////////////////////////////////////////

// activate the buzzer for half a second
void activateBuzzer() {
  Serial.println("Buzzer activated.");
  digitalWrite(buzzer,HIGH);
  Alarm.delay(500);
  digitalWrite(buzzer,LOW);
}

/////////////////////////////////////////////////////////////////////////////

void loop() {

// Set time using the serial connection.
//
// This can be easily formatted to have the appropriate header by executing
//        date +T%s > /dev/ttyXYZ
// where /dev/ttyXYZ is the Arduino's serial connection.
  if (Serial.available() > 0) {
    char c = Serial.read();
    if( c == TIME_HEADER) { 
      
      // get the Unix epoch time from serial and set Arduino system time by it
      pc_time = Serial.parseInt();
      setTime(pc_time);
      
      // confirm sync with serial message and powering the board LED
      Serial.print("\n\nReceived time sync.\n");
      Serial.print("Current Arduino system time: \t");
      Serial.print(now());
      Serial.print("\n\n");
      digitalWrite(13,HIGH);  // be sure to turn this off with the other LEDs
    }
  }

// Set the current minute.
  current_minute = minute();

// Perform the necessary checks to figure out what sort of output to send.
  if ( checkGreen()  ) { lightLED(green_led); }
  if ( checkYellow() ) { lightLED(yellow_led); }
  if ( checkRed()    ) { lightLED(red_led); }
  if ( checkBuzzer() ) { activateBuzzer(); }

// Wait 500 ms before looping.  Since we only care about the minutes here,
// half of a second should be accurate enough for our purposes.
  Alarm.delay(500);
}

/////////////////////////////////////////////////////////////////////////////

// The green LED indicates that between 0 and 5 minutes have elapsed.
boolean checkGreen() {
  if ( (current_minute >=  0 && current_minute <  5) ||
       (current_minute >= 15 && current_minute < 20) ||
       (current_minute >= 30 && current_minute < 35) ||
       (current_minute >= 45 && current_minute < 50) ) {
    return true; 
  }
  else {
    return false; 
  }
}

/////////////////////////////////////////////////////////////////////////////

// The yellow LED indicates that between 5 and 10 minutes have elapsed.
boolean checkYellow() {
  if ( (current_minute >=  5 && current_minute < 10) ||
       (current_minute >= 20 && current_minute < 25) ||
       (current_minute >= 35 && current_minute < 40) ||
       (current_minute >= 50 && current_minute < 55) ) {
    return true; 
  }
  else {
    return false; 
  }
}

/////////////////////////////////////////////////////////////////////////////

// The red LED indicates that between 10 and 15 minutes have elapsed.
boolean checkRed() {
  if ( (current_minute >= 10 && current_minute < 15) ||
       (current_minute >= 25 && current_minute < 30) ||
       (current_minute >= 40 && current_minute < 45) ||
       (current_minute >= 55 && current_minute < 60) ) {
    return true; 
  }
  else {
    return false; 
  }
}

/////////////////////////////////////////////////////////////////////////////

// Activate the buzzer five times every 15 minutes.
boolean checkBuzzer() {
  if ( ( (current_minute ==  0) ||
         (current_minute == 15) ||
         (current_minute == 30) ||
         (current_minute == 45) )
       &&
       (second() < 5) ) {
    return true; 
  }
  else {
    return false; 
  }
}

