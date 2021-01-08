/* ************************************************************************** */
/** configure.h

  @File Name
    configure.h

  @Date of Creation
     Feb 5, 2020

  @Author
     George Leece

  @Description
     Holds all the #defines utilized throughout the entire project. The SSB struct
     is defined in

  @Functions:
  
*/
/* ************************************************************************** */

#ifndef _CONFIGURE_H   /* Guard against multiple inclusion */
#define _CONFIGURE_H
#include "HX711.h"

// Digital Pin #defines
#define PROXIMITY_PIN 22      
#define LOADCELL_SCK_PIN 26
#define LOADCELL_DOUT_PIN 24
#define PING_TRIG_PIN  28
#define PING_ECHO_PIN  30
#define CV_INPUT_PIN0 32
#define CV_INPUT_PIN1 34

// Timer #defines
#define WAIT_TIMEOUT 10
#define DROP_TIMEOUT 10
#define TRASH_TIMEOUT 10

//Item Classes
#define WASTE 0
#define ALUMINUM 1
#define GLASS 2
#define PLASTIC 3

//Class Weights
#define ALUMINUM_WEIGHT 25
#define GLASS_WEIGHT 50
#define PLASTIC_WEIGHT 50

//Bin Parameters for distance
#define BIN_1 20
#define BIN_2 30
#define BIN_3 40

//User classes
#define NO_USER 0
#define USER 1

// SSB.item bools
#define NO_ITEM 1
#define ITEM 0

// SSB.Contamination bools
#define DIRTY 0
#define CLEAN 1

// SSB.fill bools
#define NOT_FULL 0
#define FULL 1

// Threshold for item detection
#define BASE_WEIGHT 3
/** topState

  @Summary
    These are the state enumerated for the Top level state machine.

  @Description
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

  @Remarks
    
*/
typedef enum {
  TOPINIT,
  IDLE,
  WAIT,
  ACTIVE,
  LOCKED
} topState;

/** wmState

  @Summary
    These are the state enumerated for the Waste Management level state machine.

  @Description
    WMINIT: Bin initializes into this state upon entering this substate 
             machine for the first time. Transitions into CLASSIFY.
       
    CLASSIFY: A command is sent to the Pi to run the CV model.
        
    TRASH: The item has been classified by CV as a trash item or contaminated by exceeding
           weight threshold for that item type and will be disposed in the trash bin. The height
           of refuse in that bin is checked and stored in the struct.
        
    RECYCLE: The item has been deemed recyclable and disposed of in the appropriate bin.

  @Remarks
    
*/
typedef enum {
  WMINIT,
  CLASSIFY,
  TRASH,
  RECYCLE,
} wmState;

/** lmrState

  @Summary
    These are the state enumerated for the Linear Motion Rail level state machine.

  @Description
    WMINIT: Bin initializes into this state upon entering this substate 
             machine for the first time. Transitions into FORWARD with motors moving.
       
    FORWARD: Depending on the item class, the lmr will move above the appropriate bin.
        
    DROP: The item is dropped into the bin and the height of refuse is checked for 
        
    REVERSE: The LMR is returning to its original position under the CC and above Trash.
             Returns to the

  @Remarks
    
*/
typedef enum {
  LMRINIT,
  FORWARD,
  REVERSE,
  DROP
} lmrState;

/** SSB struct

  @Summary
    This is the SSB (Smart Slug Bin) struct that is instantiated in the main script.
    The struct houses all of the variables needed for the Arduino and is passed through
    all levels of the state machine.

  @Description
    topSMState: Current state in the top level state machine 
    wmSMState: Current state in the waste management state machine
    lmrSMState: Current state in the linear motion rail state machine
    globalTime: Time incrementing through the ISR at 5Hz
    wmtime: Used for time stamps in the waste management level
    lmrtime: Used for time stamps in the linear motion rail level
    waitTimeStart: Used at the Top level to time out a user not inserting an item
    weight: Stores the weight of the item
    user: true-"USER" if the proximity sensor is activated in IDLE
    item: true-"ITEM" if a sudden weight change is detected in WAIT
    contamination: true-"CLEAN" if item's type is a recyclable and weight below threshold 
    fill: true-"FULL" if ping sensor returns 10cm or less
    type: response from CV is changed to an int corresponding to #defines above
    command: String command to be sent to the Pi
    response: String response to received from the Pi

*/
typedef struct {
  topState topSMState;
  wmState wmSMState;
  lmrState lmrSMState;
  unsigned long globalTime;
  unsigned long wmtime;
  unsigned long lmrtime;
  unsigned long waitTimeStart;
  float weight;
  bool user;
  bool item;
  bool contamination;
  bool fill;
  String type;
  int bcWeight;
  String command;
  String response;  
} SSB;

#endif /* _CONFIGURE_H */
/* *****************************************************************************
  End of File
*/
