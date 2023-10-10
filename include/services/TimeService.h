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
    static unsigned nextAlarmIndex();
    static tmElements_t earliest(tmElements_t tm1, tmElements_t tm2);
    static bool matchUpToSeconds(tmElements_t tm1, tmElements_t tm2);
    static bool earlier(tmElements_t tm1, tmElements_t tm2);
    static bool isPast(tmElements_t tm);
    static void updateWatchAlarms();
    static void triggerAlarm();
    static tmElements_t nextMinute();
    static bool validAlarmIndex(unsigned index);
    static tmElements_t nextTimeHourMinute(unsigned hour, unsigned minute);
public:
    static void init();
    static void update();
    static time_t get_time_unix();
    static tmElements_t get_time_formatted();
    static void setAlarm(unsigned alarm_index, tmElements_t next_time, bool repeat_days[7]);
    static WatchAlarm getAlarm(unsigned alarm_index);
    static void stopAlarm(unsigned alarm_index);
    static time_t timeToAlarm(unsigned alarm_index);
    static void setRtcInterrupt();
    static bool isCurrentlyNighttime();
};


#endif //MINTY_OS_TIMESERVICE_H
