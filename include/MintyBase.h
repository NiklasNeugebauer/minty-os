//
// Created by niklas on 6/30/23.
//

#include "hal/hal.h"

#ifndef MINTY_OS_MINTYBASE_H
#define MINTY_OS_MINTYBASE_H

#define WATCHY_DISPLAY GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT>


class MintyBase {
public:
    static WatchyRTC RTC;
    static WATCHY_DISPLAY display;

    void deepSleep();
    void wakeupRoutine();
    void displayBusyCallback(const void *);
};


#endif //MINTY_OS_MINTYBASE_H
