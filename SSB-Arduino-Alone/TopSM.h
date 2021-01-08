/* ************************************************************************** */
/** TopSM.h

  @File Name
    TopSM.h

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
#include "configure.h"
#include "sensors.h"
#ifndef _TOPSM_H    /* Guard against multiple inclusion */
#define _TOPSM_H

    /**
      @Function
        SSB runTopSM(SSB top);

      @Summary
        This is the top level of the Arduino state machine. 
        This file is to declare the function.

      @Description
     The top level runs through 4 states: IDLE | WAIT | ACTIVE | LOCKED

     TOPINIT: Bin initializes into this state upon entering this state 
           machine for the first time. Transitions into IDLE.
       
     IDLE: The bin will be operating at minimal power in order to conserve 
           energy. The bin will transition into WAIT if an ssb.userEvent happens
           from the proximity sensor.
        
     WAIT: The bin has been activated by a ssb.userEvent which will cause the
           bin to power up and in preparation for an ssb.itemEvent. The bin will
           return to IDLE if it times out. An ssb.itemEvent will transition the
           the bin into ACTIVE.
        
     ACTIVE: The bin will enter wmSM upon transition from WAIT state. Bin will
           transition back into WAIT after the ssb.item has been consumed by 
           trashing or recycling the item.

     LOCKED: After completing an item disposal iteration, the struct is returned
           and the ping sensor has checked if the bin emptied into was full 
           or not full. Upon returning from the substates, if the bin was full,
           the state will transition to LOCKED. The command to email GS will be
           issued and solenoid activated. 

       @Remarks:
      Struct, #defines, enums are located in the configure.h file. 
     */
    SSB runTopSM(SSB top); //returns an SSB
    
#endif /* _TOPSM_H */

/* *****************************************************************************
 End of File
 */