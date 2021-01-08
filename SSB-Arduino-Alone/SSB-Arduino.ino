/* ************************************************************************** */
/** SSB-Arduino.ino

  @File Name
    SSB-Arduino.ino

  @Date of Creation
     Feb 5, 2020

  @Author
     George Leece

  @Description
 Contains the main script for the Arduino of Smart Slug Bin:
 There are 3 fundamental components: initialization, main loop, and the ISR.
 In initialization, Serial communication, pin connections, and the ISR are setup
 The ISR runs at 5Hz and is used to enter the state machine every 200ms. The
 main loop runs through the state and substate machines.

  @Functions:
    void setup():
  Serial communication is established, the ISR set activated to 5Hz and pins connected
    void loop():
  Main loop where the state machine is entered every 200 ms.
    ISR(TIMER1_COMPA_vect):
  ISR where the globaltime variable is incremented for time keeping and TIMER_TICK is 
  set to true to pass the struct throught the state machine, starting at the top level.
 * 
 */
/* ************************************************************************** */
#include "configure.h"
#include "TopSM.h"
#include "wmSM.h"
#include "lmrSM.h"
#include "sensors.h"
#include "Arduino.h"


SSB ssb = {}; // Struct in configure.h is instantiated
static int TIMER_TICK = 0; // Local variable used to cycle through the state machine every 200 ms
static int i = 0; // Local variable for testing the ISR running at 200 ms.

void setup() {
//  Establish Serial communication
  Serial.begin(9600);

//  Establish pin connections
//  pinMode(PROXIMITY_PIN, INPUT);
//  pinMode(PING_TRIG_PIN, OUTPUT);
//  pinMode(PING_ECHO_PIN, INPUT);

//  Initialize the ISR
  cli();// Stop interrupts
  //Set timer1 interrupt at 1Hz
  TCCR1A = 0;// Set entire TCCR1A register to 0
  TCCR1B = 0;// Same for TCCR1B
  TCNT1  = 0;// Initialize counter value to 0
  // Set compare match register for 1hz increments
  OCR1A = 3124;// = (16*10^6) / (freq*1024) - 1 (must be <65536); Currently at 5Hz
  // Rurn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // Enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();// Allow interrupts
}

void loop() {
  if (TIMER_TICK) {  //When ever the ISR Runs, the state machine will run, i.e. every .2seconds
    ssb = runTopSM(ssb);
    TIMER_TICK = false;
  }
}


//what if we put (TIMER1_COMPA_vect && !TIMER_TICK) ? that way we can't overwrite the loop?
//That's the point of the ISR. We want to be able to leave the loop to check for an event.
//Currently we have it set up that events are checked in the loop, but that may change.

ISR(TIMER1_COMPA_vect) { //timer1 interrupt at 5Hz
//  ssb.globalTime++;
//  if (ssb.globalTime % 5 == 0) { // seconds counter
//    i++;
//    Serial.println(i);
//  }
  //  Serial.println(ssb.globalTime);
  TIMER_TICK = true;
}
