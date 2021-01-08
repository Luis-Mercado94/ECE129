
/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Smart Slug Bin

  @File Name
    configure.h

  @Summary
    Holds the necessary data for Bin

  @Description
    Describe the purpose of this file.
*/
/* ************************************************************************** */

#ifndef _CONFIGURE_H   /* Guard against multiple inclusion */
#define _CONFIGURE_H
#include "HX711.h"

//Pin #defines
#define PROXIMITY_PIN 22
#define LOADCELL_SCK_PIN 26
#define LOADCELL_DOUT_PIN 24
#define PING_TRIG_PIN  28
#define PING_ECHO_PIN  30
#define CV_INPUT_PIN0 32
#define CV_INPUT_PIN1 34

//Timer #defines
#define WAIT_TIMEOUT 25
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
#define BIN_2 25
#define BIN_3 30

//User classes
#define NO_USER 0
#define USER 1

//Item Classes
#define NO_ITEM 1
#define ITEM 0

//Contamination Classes
#define DIRTY 0
#define CLEAN 1

#define FALSE 0
#define TRUE 1

#define NOT_FULL 0
#define FULL 1

#define BASE_WEIGHT 3
/** SSB

  @Summary
    Brief one-line summary of the data type.

  @Description
    Full description, explaining the purpose and usage of the data type.
    <p>
    Additional description in consecutive paragraphs separated by HTML
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

  @Remarks
    Any additional remarks
    <p>
    Describe enumeration elements and structure and union members above each
    element or member.
*/
typedef enum {
  TOPINIT,
  IDLE,
  WAIT,
  ACTIVE,
  LOCKED
} topState;

typedef enum {
  WMINIT,
  CLASSIFY,
  TRASH,
  RECYCLE,
//  CONTAMINATION State no longer necessary with weight taken in top level at Active and passed through
} wmState;

typedef enum {
  LMRINIT,
  FORWARD,
  REVERSE,
  DROP
} lmrState;

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
  String command;
  String response;
} SSB;



#endif /* _CONFIGURE_H */
/* *****************************************************************************
  End of File
*/
