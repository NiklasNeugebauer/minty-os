//
// Created by niklas on 7/2/23.
//

#ifndef MINTY_OS_PEBBLECLOCK_H
#define MINTY_OS_PEBBLECLOCK_H

#include "WatchFace.h"

class PebbleClock : public WatchFace {
public:
    void draw(GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *display);
    void handleInput(ActionState actionState);
};


#endif //MINTY_OS_PEBBLECLOCK_H
