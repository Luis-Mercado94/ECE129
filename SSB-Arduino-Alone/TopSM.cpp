/* ************************************************************************** */
/** TopSM.h

  @File Name
    TopSM.cpp

  @Date of Creation
     Feb 5, 2020

  @Author
     George Leece

  @Description
 Contains the top level state machine of Smart Slug Bin:
 The Top level of Smart Slug Bin consists of 4 states that are changed by a User 
 Event , generated by the proximity sensor, an Item Event generated by a sudden 
 change in weight measured by the Load Cell, the weight is recorded in the for 
 comparison or a Timeout Event. The 4 States are IDLE, WAIT, ACTIVE, and LOCKED. 
 In active the state machine will enter the sub-state machine, wmSM (Waste
 Management State Machine).

  @Functions:
    SSB runTopSM(SSB top): 
  Called by the main script, passes the struct through the top level 
  state machine. Takes in the SSB struct and returns to the main script, 
  the SSB struct.
 * 
 */
/* ************************************************************************** */
// Included files 
#include "TopSM.h"
#include "wmSM.h"
#include "lmrSM.h"

// Local variables
const HX711 scale;

/**
  @Function
    SSB runTopSM(ssb top);

  @Summary
    This is the top level of the state machine.

  @Remarks
    Refer to the TopSM.h header for function usage details.
    the SSB's name is top. Input: SSB struct from the main script.
    Output: SSB struct returned to the main file. 
    
*/
SSB runTopSM(SSB top)
{
  switch (top.topSMState)
  {
    case TOPINIT: // Transitions to IDLE state. Establishes DC-DC converters on in IDLE. Initializes the scale
      top.topSMState = IDLE;
      // This is where power will come in with the level one settings and turning on of the device in order
      // Initialize the scale
      scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
      scale.set_scale(855.2f);                    
      scale.tare();
      break;
      
    case IDLE: // Checks for a user via proximity sensor. When user detected, transitions into WAIT & activates all DC-DC converters
      top.user = checkProximitySensor();
      if (top.user == USER) { // if a user is detected, then transition to WAIT
        // During transition to WAIT, remaining DC-DC converters will be enabled
//        Serial.write("Transition to WAIT\n");
        top.topSMState = WAIT; // next state 
        top.waitTimeStart = top.globalTime; // time stamp
      }
      break;
      
    case WAIT: // Checks the scale for a sudden change indicating an item inserted
      top.weight = scale.get_units(10); //ssb has a scale
      if ((top.globalTime - top.waitTimeStart) > WAIT_TIMEOUT)
      {
        top.topSMState = IDLE;
        Serial.write("Transition to IDLE\n");
        top.user == false;
      }
      else if (top.weight > BASE_WEIGHT) // Item detected by a sudden change in weight
      {
        // lockBin(); // Activate the solenoid to lock the bin
        // collect weight and store in the struct
        delay(500);
        top.weight = scale.get_units(10);
        top.topSMState = ACTIVE;
        // Enable Motor & Actuator power switches here to open CC
        top.waitTimeStart = top.globalTime;
        top.item = ITEM;  
      }
      break;
      
    case ACTIVE: // Passes the item is true then passes SSB struct into WM state machine
                 // If no item and not full, returns to WAIT; if no item and full transitions to LOCKED
      if (top.item == ITEM) { // Item has been detected and the SSB struct is passed to substate
//        top.topSMState = IDLE;
//        Serial.write("Transition to IDLE\n");
        top = runwmSM(top);
      }
      else if (top.item == NO_ITEM && top.fill == FULL) { // Item has been disposed, but bin is full; transition to LOCKED
        top.topSMState = LOCKED;
        top.command = "emailGS";
        Serial.println(top.command); // Send command to pi to transition to MESSAGE state to email Ground Services
        Serial.println(top.type); // Send the type to Pi
        Serial.println(top.weight); // Send the weight to Pi
        Serial.println(top.contamination); // Send the cotamination determination to the Pi
        top.waitTimeStart = top.globalTime;
//        lockBin(); // Activate the solenoid to lock the Bin
      }
      else if (top.item == NO_ITEM && top.fill == NOT_FULL) {
        top.topSMState = WAIT;
        Serial.write("Transition to WAIT\n");
        top.waitTimeStart = top.globalTime;

      }
      break;
      
    case LOCKED: // Bin will transition back to IDLE with a timeout currently for testing purposes
      if ((top.globalTime - top.waitTimeStart) > WAIT_TIMEOUT) {// Activate this transition for testing purposes only
        top.topSMState = IDLE;
        top.item = NO_ITEM;
        top.user = NO_USER;
      }
      break;
      
    default:
      break;
  }
  return top;
}

/* *****************************************************************************
  End of File
*/
