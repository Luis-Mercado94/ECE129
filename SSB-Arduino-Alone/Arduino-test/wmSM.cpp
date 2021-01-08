/* ************************************************************************** */
/** Descriptive File Name

  @Company
  Smart Slug Bin

  @File Name
    wmSM.c

  @Summary


  @Description

*/

/**
  @Function
    SSB updateOLEDWM(SSB wm);

  @Summary


  @Remarks

*/
#include "wmSM.h"
#include "lmrSM.h"
#include "TopSM.h"


SSB runwmSM(SSB wm)
{
  switch (wm.wmSMState) {
    case WMINIT:
      wm.wmSMState = CLASSIFY;
      Serial.print("Transition into CLASSIFY\n");
      wm.command = "runCV";
      Serial.println(wm.command); // Send command to pi to transition to CV state to classify object with CV model
      break;
    case CLASSIFY:
      //      if (Serial.available() > 0) {
      //        wm.response = Serial.readStringUntil("\n");
      //        Serial.println(wm.response);
      //        wm.type = classifyResponse(wm.response);
      if (wm.type == "Waste") {
        wm.wmtime = wm.globalTime;
        wm.wmSMState = TRASH;
        Serial.print("Transition into TRASH\n");
        wm.command = "display Trash Dirty 0"; // command type contamination weight
        Serial.println(wm.command); // sends command to transition to display
      } else if (wm.type != "Waste") {
        wm.wmSMState = RECYCLE;
        Serial.print("Transition to RECYCLE\n");
      }
      //      }
      break;
    case TRASH:
      if ((wm.globalTime - wm.wmtime) > TRASH_TIMEOUT) {
        wm.wmSMState = WMINIT;
        Serial.print("Transition wm into INIT & pass back NO_ITEM\n");
//        wm.fill = checkBin();
        wm.item = NO_ITEM;
      }
      break;
    /*case CONTAMINATION:
      if (wm.type == BUTTON_EVENT_1DOWN) {
        wm.wmSMState = RECYCLE;
        wm.contamination = CLEAN;
        wm.type = FALSE;
        updateOLEDWM(wm);
      } else if (wm.type == BUTTON_EVENT_4DOWN) {
        wm.wmSMState = TRASH;
        wm.wmtime = wm.globalTime;
        wm.contamination = DIRTY;
        wm.type = FALSE;
        updateOLEDWM(wm);
      }
      break;
    */
    case RECYCLE:
      if (wm.type == "Aluminum" && wm.weight < ALUMINUM_WEIGHT || wm.type == "Glass" && wm.weight < GLASS_WEIGHT || wm.type == "Plastic" && wm.weight < PLASTIC_WEIGHT) {
//        wm.command = "display";
//        Serial.println(wm.command); // sends the command to run the display with this information
//        wm.command = "response";
//        Serial.println(wm.command); // sends the type
//        wm.command = "Clean";
//        Serial.println(wm.command); // sends the contamination as clean
        wm = runlmrSM(wm);
      } else {
        wm.wmSMState = TRASH;
        Serial.print("Transition to TRASH\n");
        wm.command = "display " + wm.type + " Dirty " + String(wm.weight); // command type contamination weight
        Serial.println(wm.command); // sends command to transition to display
      }
      break;
    default:
      break;
  }
  return wm;
}


/* *****************************************************************************
  End of File
*/
