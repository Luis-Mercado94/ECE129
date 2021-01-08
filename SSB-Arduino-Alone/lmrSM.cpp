/* ************************************************************************** */
/** lmrSM.cpp

  @Company
    Smart Slug Bin

  @File Name
    lmrSM.cpp

  @Description
   Contains the linear motion rail state machine of Smart Slug Bin:
   This sub state machine of Smart Slug Bin consists of 3 states that are changed by a Time 
   Events, incremented throught the ISR. The 3 states are FORWARD, DROP, and REVERSE.

  @Functions:
    SSB runTopSM(SSB top): 
  Called by the script, passes the struct through the top level 
  state machine. Takes in the SSB struct and returns to the main script, 
  the SSB struct.
 * 
*/
/* ************************************************************************** */
#include "TopSM.h"
#include "wmSM.h"
#include "lmrSM.h"

/**
  @Function
    SSB runlmrSM(SSB lmr)

  @Summary
    This is the top level of the state machine.

  @Remarks
    Refer to the TopSM.h header for function usage details.
*/
SSB runlmrSM(SSB lmr)
{
  switch (lmr.lmrSMState) {
    case LMRINIT:
      lmr.lmrSMState = FORWARD;
      lmr.lmrtime = lmr.globalTime;
      lmr.command = "display " + lmr.type + " Clean " + String(lmr.weight); // command type contamination weight
      Serial.println(lmr.command); // sends command to transition to display
      Serial.print("Transition into FORWARD\n");
    case FORWARD:
      if (lmr.type == "Aluminum" && (lmr.globalTime - lmr.lmrtime > BIN_1)) {
        lmr.lmrSMState = DROP;
        Serial.print("Transition into DROP\n");
        lmr.lmrtime = lmr.globalTime;
      } else if (lmr.type == "Glass" && (lmr.globalTime - lmr.lmrtime > BIN_2)) {
        lmr.lmrSMState = DROP;
        Serial.print("Transition into DROP\n");
        lmr.lmrtime = lmr.globalTime;
      } else if (lmr.type == "Plastic" && (lmr.globalTime - lmr.lmrtime > BIN_3)) {
        lmr.lmrSMState = DROP;
        Serial.print("Transition into DROP\n");
        lmr.lmrtime = lmr.globalTime;
      }
      break;
    case DROP:
      if (lmr.globalTime - lmr.lmrtime > DROP_TIMEOUT) {
        lmr.lmrSMState = REVERSE;
        Serial.print("Transition into REVERSE\n");
        lmr.fill = checkBin();
      }
      break;
    case REVERSE:
      if (lmr.type == "Aluminum" && (lmr.globalTime - lmr.lmrtime > BIN_1)) {
        lmr.lmrSMState = LMRINIT;
        lmr.wmSMState = WMINIT;
        Serial.print("Transition lmr to INIT; Transition wm to INIT;Pass back NO_ITEM\n");
        lmr.item = NO_ITEM;
      } else if (lmr.type == "Glass" && (lmr.globalTime - lmr.lmrtime > BIN_2)) {
        lmr.lmrSMState = LMRINIT;
        lmr.wmSMState = WMINIT;
        lmr.item = NO_ITEM;
        Serial.print("Transition lmr to INIT; Transition wm to INIT;Pass back NO_ITEM\n");
      } else if (lmr.type == "Plastic" && (lmr.globalTime - lmr.lmrtime > BIN_3)) {
        lmr.lmrSMState = LMRINIT;
        lmr.wmSMState = WMINIT;
        lmr.item = NO_ITEM;
        Serial.print("Transition lmr to INIT; Transition wm to INIT;Pass back NO_ITEM\n");
      }
      break;
    default:
      break;
  }
  return lmr;
}

/* *****************************************************************************
  End of File
*/
