//
// Created by niklas on 7/2/23.
//

#ifndef MINTY_OS_WATCHFACE_H
#define MINTY_OS_WATCHFACE_H

#include "hal/hal.h"
# include "InteractionHandler.h"

class WatchFace {
public:
    WatchFace(){}
    virtual ~WatchFace(){}
    virtual void draw(Display *display) = 0;
    virtual void handleInput(ActionState actionState) {};
    virtual bool shouldDrawFull() {return false;};
    virtual bool keepAwake() {return false;};
};


#endif //MINTY_OS_WATCHFACE_H
