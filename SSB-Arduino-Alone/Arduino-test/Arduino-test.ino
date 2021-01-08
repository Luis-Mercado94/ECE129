// put your setup code here, to run once:
#include "configure.h"
#include "TopSM.h"
//#include "wmSM.h"
//#include "lmrSM.h"
#include "sensors.h"
#include "Arduino.h"


SSB ssb = {};
bool x = true;

void setup() {
  Serial.begin(9600);
  Serial.write("Welcome to Smart Slug Bin Arduino Test Harness.\n");
  Serial.write("Beginning Setup\n");

//  //Pin Setup:
//  pinMode(PROXIMITY_PIN, INPUT);
//  pinMode(CV_INPUT_PIN0, INPUT);
//  pinMode(PING_TRIG_PIN, OUTPUT);
//  pinMode(PING_ECHO_PIN, INPUT);

  cli();//stop interrupts
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 3124;// = (16*10^6) / (freq*1024) - 1 (must be <65536); Currently at 5Hz
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();//allow interrupts
  ssb.command = "0\n";

  Serial.write("Finished Setup\n");
}

void loop() {
  while (x==true){
    
    Serial.write("Beginning Test Harness\n");
    
    //Testing that there is no transition out of IDLE while user == NO_USER
    Serial.println("Testing no transition out of IDLE while user !=USER");
    delay(1000);
    ssb.user = NO_USER;           // Testing no transition with no user detected
    ssb = runTopSM(ssb);
    delay(200);
    ssb.weight = 10;              // weight is above item threshold
    ssb = runTopSM(ssb);
    delay(200);
    ssb.item = NO_ITEM;           // Testing no transition with no item detected
    ssb = runTopSM(ssb);
    delay(200);
    ssb.item = ITEM;              // Testing no transition with an item detected
    ssb = runTopSM(ssb);
    delay(200);
    ssb.contamination = DIRTY;    // Testing no transition with a Dirty Item
    ssb = runTopSM(ssb);
    delay(200);
    ssb.contamination = CLEAN;    // Testing no transition with a Clean Item
    ssb = runTopSM(ssb);
    delay(200);
    ssb.fill = FULL;              // Testing no transition with a Full Bin
    ssb = runTopSM(ssb);
    delay(200);
    ssb.fill = NOT_FULL;          // Testing no transition with a not Full Bin
    ssb = runTopSM(ssb);
    delay(200);
    ssb.type = "Waste";             // Testing no transition with a Waste typed item
    ssb = runTopSM(ssb);
    delay(200);
    ssb.type = "Aluminum";          // Testing no transition with a Aluminum typed item
    ssb = runTopSM(ssb);
    delay(200);
    ssb.type = "Glass";             // Testing no transition with a Glass typed item
    ssb = runTopSM(ssb);
    delay(200);
    ssb.type = "Plastic";           // Testing no transition with a Plastic typed item
    ssb = runTopSM(ssb);
    delay(200);
    ssb.weight = 1;               // weight is below item threshold
    ssb = runTopSM(ssb);
    delay(200);
    ssb.user = USER;              // Testing transition to WAIT with a user detected
    ssb = runTopSM(ssb);
    delay(200);
    
// Testing the transition from WAIT to back to IDLE
    Serial.println("Testing transition from WAIT to IDLE with Time");
    delay(1000);
    ssb.weight = 2;               // weight is below item threshold; no transition
    ssb = runTopSM(ssb);
    delay(200);
    ssb.user = NO_USER;           // Testing no transition with no user detected
    ssb = runTopSM(ssb);
    delay(200);
    ssb.item = NO_ITEM;           // Testing no transition with no item detected
    ssb = runTopSM(ssb);
    ssb.item = ITEM;              // Testing no transition with an item detected
    ssb = runTopSM(ssb);
    delay(200);
    ssb.contamination = DIRTY;    // Testing no transition with a Dirty Item
    ssb = runTopSM(ssb);
    delay(200);
    ssb.contamination = CLEAN;    // Testing no transition with a Clean Item
    ssb = runTopSM(ssb);
    delay(200);
    ssb.fill = FULL;              // Testing no transition with a Full Bin
    ssb = runTopSM(ssb);
    delay(200);
    ssb.fill = NOT_FULL;          // Testing no transition with a not Full Bin
    ssb = runTopSM(ssb);
    delay(200);
    ssb.type = "Waste";             // Testing no transition with a Waste typed item
    ssb = runTopSM(ssb);
    delay(200);
    ssb.type = "Aluminum";          // Testing no transition with a Aluminum typed item
    ssb = runTopSM(ssb);
    delay(200);
    ssb.type = "Glass";             // Testing no transition with a Glass typed item
    ssb = runTopSM(ssb);
    delay(200);
    ssb.type = "Plastic";           // Testing no transition with a Plastic typed item
    ssb = runTopSM(ssb);
    delay(5200);                 // Testing Timeout and transition from WAIT to IDLE
    ssb = runTopSM(ssb);
    Serial.println();
    
    // Transition back to WAIT
    Serial.println("Testing transition from IDLE back to WAIT to ACTIVE");
    delay(1000);
    ssb.user = USER;              // Transition to WAIT with a user detected
    // Serial.println("TRANSITION TO WAIT");
    ssb = runTopSM(ssb);
    delay(200);
    // Transition from WAIT to ACTIVE
    // Enters Waste Management substate machine. Testing of CLASSIFY: Transition to TRASH with a WASTE classification
    ssb.weight = 10;              // weight is above item threshold and transitions to ACTIVE
    // Serial.println("TRANSTION TO ACTIVE?");
    ssb = runTopSM(ssb);
    delay(200);
    Serial.println();

// Testing a WASTE item disposal
    Serial.println("Testing a WASTE item disposal and not full bin");
    delay(1000);
    ssb.fill = NOT_FULL;          // Testing no transition with a not Full Bin
    ssb.type = "Waste";             // Testing no transition with a Waste typed item
    // Serial.println("WHAT DO I DO?");
    ssb = runTopSM(ssb);          // In ACTIVE: Transition from Init to CLASSIFY; runCV model
    delay(200);
    // Serial.println("WHAT DO I DO?");
    ssb = runTopSM(ssb);          // In ACTIVE: Transition from CLASSIFY to TRASH b/c type == WASTE; send command to run display to Pi
    delay(2200);
    // Serial.println("WHAT DO I DO?");
    ssb = runTopSM(ssb);          // In ACTIVE: Transition from TRASH to INIT; pass back NO_ITEM
    delay(200);
    ssb = runTopSM(ssb);          // Transition from ACTIVE to WAIT
    Serial.println();

// Testing a Dirty Aluminum item disposal
    Serial.println("Testing an contaminated ALUMINUM item disposal and not full bin");
    delay(1000);
    ssb.fill = NOT_FULL;          // Testing no transition with a not Full Bin
    ssb.type = "Aluminum";          // Testing no transition with a Waste typed item
    ssb.weight = 100;             // Item is "contaminated" 
    // Serial.println("Transtion from WAIT to ACTIVE");
    ssb = runTopSM(ssb);          // Transtion from WAIT to ACTIVE
    delay(200);
    // Serial.println("In ACTIVE: Transition from Init to CLASSIFY; runCV model");
    ssb = runTopSM(ssb);          // In ACTIVE: Transition from Init to CLASSIFY; runCV model
    delay(200);
    // Serial.println("In ACTIVE: Transition from CLASSIFY to RECYCLE b/c type != WASTE");
    ssb = runTopSM(ssb);          // In ACTIVE: Transition from CLASSIFY to RECYCLE b/c type != WASTE
    delay(200);
    // Serial.println("In ACTIVE: Transition from RECYCLE to TRASH b/c weight > typeThreshold; send display information to Pi");
    ssb = runTopSM(ssb);          // In ACTIVE: Transition from RECYCLE to TRASH b/c weight > typeThreshold
    delay(200); 
    // Serial.println("In ACTIVE: Transition from TRASH to INIT; pass back NO_ITEM");
    ssb = runTopSM(ssb);           // In ACTIVE: Transition from TRASH to INIT; pass back NO_ITEM
    delay(2200);
    // Serial.println("Transition from ACTIVE to WAIT");
    ssb = runTopSM(ssb);          // Transition from ACTIVE to WAIT
    delay(200);
    Serial.println();

// Testing a Dirty Glass item disposal
    Serial.println("Testing an contaminated GLASS item disposal and not full bin");
    delay(1000);
    ssb.fill = NOT_FULL;          // Testing no transition with a not Full Bin
    ssb.type = "Glass";             // Testing no transition with a GLASS typed item
    ssb.weight = 100;             // Item is "contaminated" 
    // Serial.println("Transtion from WAIT to ACTIVE");
    ssb = runTopSM(ssb);          // Transtion from WAIT to ACTIVE
    delay(200);
    // Serial.println("In ACTIVE: Transition from Init to CLASSIFY; runCV model");
    ssb = runTopSM(ssb);          // In ACTIVE: Transition from Init to CLASSIFY; runCV model
    delay(200);
    // Serial.println("In ACTIVE: Transition from CLASSIFY to RECYCLE b/c type != WASTE");
    ssb = runTopSM(ssb);          // In ACTIVE: Transition from CLASSIFY to RECYCLE b/c type != WASTE
    delay(200);
    // Serial.println("In ACTIVE: Transition from RECYCLE to TRASH b/c weight > typeThreshold; send display information to Pi");
    ssb = runTopSM(ssb);          // In ACTIVE: Transition from RECYCLE to TRASH b/c weight > typeThreshold
    delay(200); 
    // Serial.println("In ACTIVE: Transition from TRASH to INIT; pass back NO_ITEM");
    ssb = runTopSM(ssb);           // In ACTIVE: Transition from TRASH to INIT; pass back NO_ITEM
    delay(2200);
    // Serial.println("Transition from ACTIVE to WAIT");
    ssb = runTopSM(ssb);          // Transition from ACTIVE to WAIT
    delay(200);
    Serial.println();

// Testing a Dirty Plastic item disposal
    Serial.println("Testing an contaminated Plastic item disposal and not full bin");
    delay(1000);
    ssb.fill = NOT_FULL;          // Testing no transition with a not Full Bin
    ssb.type = "Plastic";           // Testing no transition with a Plastic typed item
    ssb.weight = 100;             // Item is "contaminated" 
    // Serial.println("Transtion from WAIT to ACTIVE");
    ssb = runTopSM(ssb);          // Transtion from WAIT to ACTIVE
    delay(200);
    // Serial.println("In ACTIVE: Transition from Init to CLASSIFY; runCV model");
    ssb = runTopSM(ssb);          // In ACTIVE: Transition from Init to CLASSIFY; runCV model
    delay(200);
    // Serial.println("In ACTIVE: Transition from CLASSIFY to RECYCLE b/c type != WASTE");
    ssb = runTopSM(ssb);          // In ACTIVE: Transition from CLASSIFY to RECYCLE b/c type != WASTE
    delay(200);
    // Serial.println("In ACTIVE: Transition from RECYCLE to TRASH b/c weight > typeThreshold; send display information to Pi");
    ssb = runTopSM(ssb);          // In ACTIVE: Transition from RECYCLE to TRASH b/c weight > typeThreshold
    delay(200); 
    // Serial.println("In ACTIVE: Transition from TRASH to INIT; pass back NO_ITEM");
    ssb = runTopSM(ssb);           // In ACTIVE: Transition from TRASH to INIT; pass back NO_ITEM
    delay(2200);
    // Serial.println("Transition from ACTIVE to WAIT");
    ssb = runTopSM(ssb);          // Transition from ACTIVE to WAIT
    delay(200);
    Serial.println();
    
// Testing a Clean Aluminum item disposal
    Serial.println("Testing a clean Aluminum item disposal and not full bin");
    delay(1000);
    ssb.fill = NOT_FULL;          // Testing no transition with a not Full Bin
    ssb.type = "Aluminum";          // Testing no transition with a Aluminum typed item
    ssb.weight = 10;
//    Serial.println("WAIT TO ACTIVE");
    ssb = runTopSM(ssb);          // Transition from WAIT to ACTIVE
    delay(200);
//    Serial.println("IN ACTIVE; TRANSITION INTO CLASSIFY & RUNCV");
    ssb = runTopSM(ssb);          // In Active; Transition from Init to CLASSIFY & runCV command issued
    delay(200);
//    Serial.println("TRANSITION TO RECYCLE");
    ssb = runTopSM(ssb);          // In ACTIVE: Transition from CLASSIFY to RECYCLE
    delay(200);
//    Serial.println("IN ACTIVE: IN RECYCLE: LMR TRANSTION FROM INIT TO FORWARD");
    ssb = runTopSM(ssb);          // In Active: In RECYCLE: LMR transition from INIT to FORWARD
    delay(4100);
//    Serial.println("IN ACTIVE: IN RECYCLE: LMR TRANSTION FROM FORWARD TO DROP");
    ssb = runTopSM(ssb);          // In ACTIVE: In RECYCLE: LMR transition from FORWARD to DROP; Ping sensor checks bin fill
    delay(2100);
//    Serial.println("IN ACTIVE: IN RECYCLE: LMR TRANSTION FROM DROP TO REVERSE");
    ssb = runTopSM(ssb);          // In ACTIVE: In RECYCLE: LMR transition from DROP to REVERSE
    delay(4100);
//    Serial.println("TRANSTION LMR AND WM BACK TO INIT; PASS NO_ITEM");
    ssb = runTopSM(ssb);          // In ACTIVE: WM transitioned to INIT: LMR Transitioned back to INIT
    delay(200);     
//    Serial.println("TRANSTION TOP FROM ACTIVE BACK TO WAIT");
    ssb = runTopSM(ssb);          // Transition from ACTIVE to WAIT
    ssb = runTopSM(ssb);
    delay(200);
    Serial.println();

// Testing a clean GLASS item disposal
    Serial.println("Testing a clean glass item disposal and not full bin");
    delay(1000);
    ssb.fill = NOT_FULL;          // Testing no transition with a not Full Bin
    ssb.type = "Glass";          // Testing no transition with a glass typed item
    ssb.weight = 10;
//    Serial.println("WAIT TO ACTIVE");
    ssb = runTopSM(ssb);          // Transition from WAIT to ACTIVE
    delay(200);
//    Serial.println("IN ACTIVE; TRANSITION INTO CLASSIFY & RUNCV");
    ssb = runTopSM(ssb);          // In Active; Transition from Init to CLASSIFY & runCV command issued
    delay(200);
//    Serial.println("TRANSITION TO RECYCLE");
    ssb = runTopSM(ssb);          // In ACTIVE: Transition from CLASSIFY to RECYCLE
    delay(200);
//    Serial.println("IN ACTIVE: IN RECYCLE: LMR TRANSTION FROM INIT TO FORWARD");
    ssb = runTopSM(ssb);          // In Active: In RECYCLE: LMR transition from INIT to FORWARD
    delay(6500);
//    Serial.println("IN ACTIVE: IN RECYCLE: LMR TRANSTION FROM FORWARD TO DROP");
    ssb = runTopSM(ssb);          // In ACTIVE: In RECYCLE: LMR transition from FORWARD to DROP; Ping sensor checks bin fill
    delay(2100);
//    Serial.println("IN ACTIVE: IN RECYCLE: LMR TRANSTION FROM DROP TO REVERSE");
    ssb = runTopSM(ssb);          // In ACTIVE: In RECYCLE: LMR transition from DROP to REVERSE
    delay(6500);
//    Serial.println("TRANSTION LMR AND WM BACK TO INIT; PASS NO_ITEM");
    ssb = runTopSM(ssb);          // In ACTIVE: WM transitioned to INIT: LMR Transitioned back to INIT
    delay(200);     
//    Serial.println("TRANSTION TOP FROM ACTIVE BACK TO WAIT");
    ssb = runTopSM(ssb);          // Transition from ACTIVE to WAIT
    ssb = runTopSM(ssb);
    delay(200);
    Serial.println();


// Testing a clean PLASTIC item disposal
    Serial.println("Testing a clean plastic item disposal and not full bin");
    delay(1000);
    ssb.fill = NOT_FULL;          // Testing no transition with a not Full Bin
    ssb.type = "Plastic";             // Testing no transition with a PLASTIC typed item
    ssb.weight = 10;
//    Serial.println("WAIT TO ACTIVE");
    ssb = runTopSM(ssb);          // Transition from WAIT to ACTIVE
    delay(200);
//    Serial.println("IN ACTIVE; TRANSITION INTO CLASSIFY & RUNCV");
    ssb = runTopSM(ssb);          // In Active; Transition from Init to CLASSIFY & runCV command issued
    delay(200);
//    Serial.println("TRANSITION TO RECYCLE");
    ssb = runTopSM(ssb);          // In ACTIVE: Transition from CLASSIFY to RECYCLE
    delay(200);
//    Serial.println("IN ACTIVE: IN RECYCLE: LMR TRANSTION FROM INIT TO FORWARD");
    ssb = runTopSM(ssb);          // In Active: In RECYCLE: LMR transition from INIT to FORWARD
    delay(8100);
//    Serial.println("IN ACTIVE: IN RECYCLE: LMR TRANSTION FROM FORWARD TO DROP");
    ssb = runTopSM(ssb);          // In ACTIVE: In RECYCLE: LMR transition from FORWARD to DROP; Ping sensor checks bin fill
    delay(2100);
//    Serial.println("IN ACTIVE: IN RECYCLE: LMR TRANSTION FROM DROP TO REVERSE");
    ssb = runTopSM(ssb);          // In ACTIVE: In RECYCLE: LMR transition from DROP to REVERSE
    delay(8100);
//    Serial.println("TRANSTION LMR AND WM BACK TO INIT; PASS NO_ITEM");
    ssb = runTopSM(ssb);          // In ACTIVE: WM transitioned to INIT: LMR Transitioned back to INIT
    delay(200);     
//    Serial.println("TRANSTION TOP FROM ACTIVE BACK TO WAIT");
    ssb = runTopSM(ssb);          // Transition from ACTIVE to WAIT
    ssb = runTopSM(ssb);
    delay(200);
    Serial.println();
    
// Testing a WASTE item disposal
    Serial.println("Testing an Waste item disposal and full bin");
    delay(1000);
    ssb.fill = FULL;              // Testing transition with a Full Bin
    ssb.type = "Waste";             // Testing no transition with a Waste typed item
    Serial.println("Transition from ACTIVE to WAIT");
    ssb = runTopSM(ssb);          // Transition from WAIT to ACTIVE
    delay(200);
    Serial.println("In ACTIVE: Transition from Init to CLASSIFY; runCV model");
    ssb = runTopSM(ssb);          // In ACTIVE: Transition from Init to CLASSIFY; runCV model 
    delay(2200);
    Serial.println("In ACTIVE: Transition from CLASSIFY to TRASH b/c type == WASTE; send command to run display to Pi");
    ssb = runTopSM(ssb);          // In ACTIVE: Transition from CLASSIFY to TRASH b/c type == WASTE; send command to run display to Pi
    delay(200);
    ssb = runTopSM(ssb);          // In ACTIVE: Transition from TRASH to INIT; pass back NO_ITEM 
    ssb = runTopSM(ssb);          // Transition from ACTIVE to LOCKED
    ssb = runTopSM(ssb);
    delay(5100);
    ssb = runTopSM(ssb);          // Transition from LOCKED to IDLE
    Serial.println();

// Testing a Clean Aluminum item disposal and Full Bin
    Serial.println("Testing an clean aluminum item disposal and full bin");
    delay(1000);
    ssb.fill = FULL;              // Testing transition with a Full Bin
    ssb.type = "Aluminum";          // Testing no transition with a Plastic typed item
    ssb.weight = 10;
//    // Serial.println("WAIT TO ACTIVE");
    ssb = runTopSM(ssb);          // Transition from WAIT to ACTIVE
    delay(200);
//    Serial.println("IN ACTIVE; TRANSITION INTO CLASSIFY & RUNCV");
    ssb = runTopSM(ssb);          // In Active; Transition from Init to CLASSIFY & runCV command issued
    delay(200);
//    Serial.println("TRANSITION TO RECYCLE");
    ssb = runTopSM(ssb);          // In ACTIVE: Transition from CLASSIFY to RECYCLE
    delay(200);
//    Serial.println("IN ACTIVE: IN RECYCLE: LMR TRANSTION FROM INIT TO FORWARD");
    ssb = runTopSM(ssb);          // In Active: In RECYCLE: LMR transition from INIT to FORWARD
    delay(4100);
//    Serial.println("IN ACTIVE: IN RECYCLE: LMR TRANSTION FROM FORWARD TO DROP");
    ssb = runTopSM(ssb);          // In ACTIVE: In RECYCLE: LMR transition from FORWARD to DROP; Ping sensor checks bin fill
    delay(2100);
//    Serial.println("IN ACTIVE: IN RECYCLE: LMR TRANSTION FROM DROP TO REVERSE");
    ssb = runTopSM(ssb);          // In ACTIVE: In RECYCLE: LMR transition from DROP to REVERSE
    delay(4100);
//    Serial.println("TRANSTION LMR AND WM BACK TO INIT; PASS NO_ITEM");
    ssb = runTopSM(ssb);          // In ACTIVE: WM transitioned to INIT: LMR Transitioned back to INIT
    delay(200);     
//    Serial.println("TRANSTION TOP FROM ACTIVE BACK TO WAIT");
    ssb = runTopSM(ssb);          // Transition from ACTIVE to LOCKED
    ssb = runTopSM(ssb);
    delay(5100);
    ssb = runTopSM(ssb);          // Transition from LOCKED to IDLE
    Serial.println();


    x = false;
  }
  
}


//what if we put (TIMER1_COMPA_vect && !TIMER_TICK) ? that way we can't overwrite the loop?
//That's the point of the ISR. We want to be able to leave the loop to check for an event.
//Currently we have it set up that events are checked in the loop, but that may change.

ISR(TIMER1_COMPA_vect) { //timer1 interrupt at 5Hz
//  Serial.write("ISR\n");
  ssb.globalTime++;
//  ssb.response = Serial.read();
//  if(ssb.globalTime%5 ==0){ // seconds counter
//    i++;
//    Serial.println(i);
//  }
}
