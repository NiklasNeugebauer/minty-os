//
// Created by niklas on 7/2/23.
//

#ifndef MINTY_OS_TIMESERVICE_H
#define MINTY_OS_TIMESERVICE_H

#include "hal/WatchyRTC.h"

class TimeService {
private:
    static WatchyRTC RTC;
public:
    void update();
    time_t get_time_unix();
    tmElements_t get_time_formatted();
};


#endif //MINTY_OS_TIMESERVICE_H
