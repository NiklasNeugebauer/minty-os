//
// Created by niklas on 7/2/23.
//

#ifndef MINTY_OS_WATCHFACE_H
#define MINTY_OS_WATCHFACE_H

#include "hal/hal.h"

class WatchFace {
private:
public:
    virtual void draw(Display display) = 0;
};


#endif //MINTY_OS_WATCHFACE_H
