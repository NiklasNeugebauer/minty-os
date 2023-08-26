//
// Created by niklas on 7/11/23.
//

#ifndef MINTY_OS_TIMERFACE_H
#define MINTY_OS_TIMERFACE_H

#include "WatchFace.h"

class TimerFace : public WatchFace {
public:
    void draw(GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *display) override;
    void handleInput(ActionState actionState) override;
};


#endif //MINTY_OS_TIMERFACE_H
