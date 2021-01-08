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

#ifndef _SENSORS_H   /* Guard against multiple inclusion */
#define _SENSORS_H

#include "Arduino.h"
#include "configure.h"


bool checkProximitySensor();
bool checkItem(int times);


//Activates the ping sensor. Returns: FULL || NOT_FULL
bool checkBin();


//Sends signal to R-Pi to activate the CV
int runCV();

//Sends signal to R-Pi to message Grounds Services
//bool emailGS();

//Activates IR Spectrometer.
//__ runIRSpectrometer();

//
//Locks the bin
//bool lockBin();

void printState(SSB ssb);

#endif 
