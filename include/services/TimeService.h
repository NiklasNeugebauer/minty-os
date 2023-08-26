//
// Created by niklas on 7/2/23.
//

#ifndef MINTY_OS_TIMESERVICE_H
#define MINTY_OS_TIMESERVICE_H

#include "Service.h"
#include "hal/WatchyRTC.h"

struct WatchAlarm {
    bool active;
    tmElements_t next_time;
    bool repeat_days[7];
};

class TimeService : Service{
private:
    static WatchyRTC RTC;

    static bool syncNTP(long gmt, String ntpServer);
    static tmElements_t nextAlarm();
    static tmElements_t earliest(tmElements_t tm1, tmElements_t tm2);
    static bool matchUpToSeconds(tmElements_t tm1, tmElements_t tm2);
    static bool earlier(tmElements_t tm1, tmElements_t tm2);
    static bool isPast(tmElements_t tm);
    static void updateWatchAlarms();
    static void triggerAlarm();
    static tmElements_t nextMinute();
public:
    static void init();
    static void update();
    static time_t get_time_unix();
    static tmElements_t get_time_formatted();
    static void setAlarm(tmElements_t next_time, bool repeat_days[7]);
    static WatchAlarm getAlarm();
    static void stopAlarm();
    static time_t timeToAlarm();
    static void setRtcInterrupt();
};


#endif //MINTY_OS_TIMESERVICE_H
