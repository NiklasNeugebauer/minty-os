//
// Created by niklas on 7/2/23.
//
// NTP code is based off SQFMI's Watchy.cpp at https://github.com/sqfmi/Watchy/blob/master/src/Watchy.cpp

#include "services/TimeService.h"

#include "services/VibrationService.h"
#include "services/WifiService.h"
#include "SerialLogger.h"

#include <NTPClient.h>
#include <WiFiUdp.h>

#include "conf.h"

#define N_ALARMS 4

// TODO consider storing the current time for the current interval for less lookups

WatchyRTC TimeService::RTC;
RTC_DATA_ATTR bool minuteWake;
RTC_DATA_ATTR WatchAlarm watchAlarms[N_ALARMS + 1];
RTC_DATA_ATTR tmElements_t nightSleepStart;
RTC_DATA_ATTR tmElements_t nightSleepEnd;
RTC_DATA_ATTR bool needsSync;

void TimeService::init() {
    SERIAL_LOG_I("Starting TimeService...");
    RTC.config("");
    needsSync = true;
    minuteWake = true;

    nightSleepStart.Hour = Config::getGlobalConfig().night_begin_hour;
    nightSleepStart.Minute = 00;
    nightSleepStart.Second = 0;
    nightSleepEnd.Hour = Config::getGlobalConfig().night_end_hour;
    nightSleepEnd.Minute = 7;
    nightSleepEnd.Second = 0;

    // morning alarm
    std::array<bool, 7> full_repeat = {true,true,true,true,true,true,true};
    setAlarm(0, nextTimeHourMinute(nightSleepEnd.Hour, nightSleepEnd.Minute), full_repeat);

    // TODO store and load alarm data into NVS
}

tmElements_t TimeService::get_time_formatted() {
    tmElements_t current_time;
    RTC.read(current_time);
    return current_time;
}

time_t TimeService::get_time_unix() {
    tmElements_t currentTime = get_time_formatted();
    return makeTime(currentTime);
}

void TimeService::update() {
    if (needsSync) {
        syncNTP(Config::getGlobalConfig().gmt_offset_sec, NTP_SERVER);
        needsSync = false;
    }
    updateWatchAlarms();
}

void TimeService::updateWatchAlarms() {
    SERIAL_LOG_D("Checking alarms...");
    for (unsigned alarm_index = 0; alarm_index < N_ALARMS; alarm_index++) {
        WatchAlarm *watchAlarm = &watchAlarms[alarm_index];
        if (!watchAlarm->active) {
            SERIAL_LOG_D("Alarm ", alarm_index, " not active.");
            continue;
        }
        SERIAL_LOG_D("Still waiting for alarm ", alarm_index, " at ", watchAlarm->next_time.Hour, ":",  watchAlarm->next_time.Minute, ":" , watchAlarm->next_time.Second , " on Day ", watchAlarm->next_time.Day);

        // find the next alarm time
        tmElements_t current_time = get_time_formatted();

        if (isPast(watchAlarm->next_time)) {
            SERIAL_LOG_D("Alarm has passed.");
            triggerAlarm();

            unsigned current_weekday = current_time.Wday;
            bool repeat_set = false;
            for (int i = 1; i < 7; i++) {
                unsigned next_weekday = (current_weekday + i) % 7;
                if (watchAlarm->repeat_days[next_weekday] == true) {
                    SERIAL_LOG_D("Weekday repeat triggered on week day ", next_weekday);
                    time_t past_time_unix = makeTime(watchAlarm->next_time);
                    breakTime(past_time_unix + i*SECS_PER_DAY, watchAlarm->next_time);
                    repeat_set = true;
                    break;
                }
            }

            watchAlarm->active = repeat_set;
            if (repeat_set) {
                SERIAL_LOG_D("Reset alarm to ", watchAlarm->next_time.Hour, ":",  watchAlarm->next_time.Minute, ":" , watchAlarm->next_time.Second , " on Day ", watchAlarm->next_time.Day);
            } else {
                SERIAL_LOG_D("No reset time found. Alarm is inactive now.");
            }
        }
    }
}

bool TimeService::syncNTP(long gmt, String ntpServer) {
    // NTP sync - call after connecting to WiFi
    if (WifiService::connect() == WL_CONNECTED) {
        WiFiUDP ntpUDP;
        NTPClient timeClient(ntpUDP, ntpServer.c_str(), gmt);
        timeClient.begin();
        if (!timeClient.forceUpdate()) {
            return false; // NTP sync failed
        }
        tmElements_t tm;
        breakTime((time_t)timeClient.getEpochTime(), tm);
        RTC.set(tm);
        return true;
    }
    return false;
}

void TimeService::setAlarm(unsigned alarm_index, tmElements_t next_time, std::array<bool,7> repeat_days) {
    WatchAlarm *watchAlarm = &watchAlarms[alarm_index];

    watchAlarm->next_time = next_time;
    watchAlarm->repeat_days = repeat_days;
    watchAlarm->active = true;
    SERIAL_LOG_I("Set alarm", alarm_index , " at ", watchAlarm->next_time.Hour, ":",  watchAlarm->next_time.Minute, ":" ,
                                                    watchAlarm->next_time.Second , " on Day ", watchAlarm->next_time.Day);
    SERIAL_LOG_D("Repeat days: ", repeat_days[0], ", ", repeat_days[1], ", ", repeat_days[2], ", ", repeat_days[3], ", ",
                repeat_days[4], ", ", repeat_days[5], ", ", repeat_days[6]);
}

void TimeService::stopAlarm(unsigned alarm_index) {
    WatchAlarm *watchAlarm = &watchAlarms[alarm_index];
    watchAlarm->active = false;
    SERIAL_LOG_I("Stopped alarm");
}

WatchAlarm TimeService::getAlarm(unsigned alarm_index) {
    return watchAlarms[alarm_index];
}

time_t TimeService::timeToAlarm(unsigned alarm_index) {
    /// returns time until the next alarm in seconds
    /// returns value <= 0 if alarm has already passed or is inactive
    WatchAlarm watchAlarm = watchAlarms[alarm_index];

    if (!watchAlarm.active) {
        return -1;
    }

    return makeTime(watchAlarm.next_time) - get_time_unix();
}

void TimeService::setRtcInterrupt() {
    WatchAlarm next_alarm = watchAlarms[nextAlarmIndex()];
    if ((next_alarm.active && (minuteWake && earlier(next_alarm.next_time, nextMinute()))) ||
            (!minuteWake) ||
            TimeService::isCurrentlyNighttime()) {
        SERIAL_LOG_D("Setting wakeup to ", next_alarm.next_time.Hour, ":",  next_alarm.next_time.Minute, ":" , next_alarm.next_time.Second, " on Day ", next_alarm.next_time.Day);
        RTC.setAlarm(next_alarm.next_time);
    } else if (minuteWake) {
        SERIAL_LOG_D("Setting wakeup to next minute");
        RTC.setAlarmNextMinute();
    } else {
        SERIAL_LOG_D("No interrupt set. The watch will sleep until a button is pressed.");
    }
}

unsigned TimeService::nextAlarmIndex() {
    /// Determines the next closest alarm to be fired
    time_t min_time = LONG_MAX;
    unsigned next_alarm_index = 0;
    for (unsigned alarm_index = 0; alarm_index < N_ALARMS; alarm_index++) {
        WatchAlarm watchAlarm = watchAlarms[alarm_index];
       if (watchAlarm.active && !isPast(watchAlarm.next_time)) {
           time_t alarm_i_time = makeTime(watchAlarm.next_time);
           if (alarm_i_time < min_time) {
            min_time = alarm_i_time;
               next_alarm_index = alarm_index;
           }
       }
    }
    return next_alarm_index;
}

tmElements_t TimeService::nextMinute() {
    /// returns the formatted time of the next full minute

    time_t current_time = get_time_unix();
    time_t current_plus_minute = current_time + 60;
    tmElements_t nextMinute;
    breakTime(current_plus_minute - (current_plus_minute%60), nextMinute);

    return nextMinute;
}

void TimeService::triggerAlarm() {
    SERIAL_LOG_I("Alarm triggered! Brrr");
    VibrationService::vibrateMS(100);
}

// helper functions

bool TimeService::matchUpToSeconds(tmElements_t tm1, tmElements_t tm2) {
    time_t time1 = makeTime(tm1);
    time_t time2 = makeTime(tm2);

    return time1 - (time1 % 60) == time2 - (time2 % 60);
}

bool TimeService::earlier(tmElements_t tm1, tmElements_t tm2) {
    time_t time1 = makeTime(tm1);
    time_t time2 = makeTime(tm2);

    return time1 < time2;
}

tmElements_t TimeService::earliest(tmElements_t tm1, tmElements_t tm2) {
    return earlier(tm1, tm2) ? tm1 : tm2;
}

bool TimeService::isPast(tmElements_t tm) {
    tmElements_t current_time = get_time_formatted();
    return makeTime(tm) <= makeTime(current_time);
}

bool TimeService::validAlarmIndex(unsigned int index) {
    return index < N_ALARMS;
}

bool TimeService::isCurrentlyNighttime(){
    tmElements_t nextNightStart = nextTimeHourMinute(nightSleepStart.Hour, nightSleepStart.Minute);
    tmElements_t nextNightEnd = nextTimeHourMinute(nightSleepEnd.Hour, nightSleepEnd.Minute);
    return earlier(nextNightEnd, nextNightStart);
}

tmElements_t TimeService::nextTimeHourMinute(unsigned hour, unsigned minute) {
    tmElements_t current_time = get_time_formatted();
    tmElements_t candidateToday = current_time;
    tmElements_t nextTime;
    candidateToday.Hour = hour;
    candidateToday.Minute = minute;
    candidateToday.Second = 0;
    if (isPast(candidateToday)) {
        time_t candidateTodayUnix = makeTime(candidateToday);
        breakTime(candidateTodayUnix + SECS_PER_DAY, nextTime);
    } else {
        nextTime = candidateToday;
    } 

    return nextTime;
}