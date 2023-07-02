//
// Created by niklas on 7/2/23.
//

#include "services/TimeService.h"

WatchyRTC TimeService::RTC;

tmElements_t TimeService::get_time_formatted() {
    RTC.init();

    tmElements_t current_time;
    RTC.read(current_time);
    return current_time;
}

time_t TimeService::get_time_unix() {
    tmElements_t currentTime = get_time_formatted();
    return makeTime(currentTime);
}

void TimeService::update() {

}