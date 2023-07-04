//
// Created by niklas on 7/2/23.
//

#ifndef MINTY_OS_TIMESERVICE_H
#define MINTY_OS_TIMESERVICE_H

#include "hal/WatchyRTC.h"

class TimeService {
public:
    static void init();
    static void update();
    static time_t get_time_unix();
    static tmElements_t get_time_formatted();
};


#endif //MINTY_OS_TIMESERVICE_H
