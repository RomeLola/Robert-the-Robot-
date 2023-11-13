#ifndef LIMITSWITCH_H
#define LIMITSWITCH_H

#include <Arduino.h>
#include "debug.h"
#include "state.h"

void setupLimitSwitch1();
void IRAM_ATTR Mytoggle();
//void printInteruptID(int limitSwithchID);

#endif