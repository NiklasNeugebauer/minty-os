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

// TODO consider storing the current time for the current interval for less lookups

WatchyRTC TimeService::RTC;
RTC_DATA_ATTR bool minuteWake;
RTC_DATA_ATTR WatchAlarm watchAlarm;
RTC_DATA_ATTR WatchAlarm morningAlarm;
RTC_DATA_ATTR bool needsSync;

void TimeService::init() {
    SERIAL_LOG_I("Starting TimeService...");
    RTC.config("");
    needsSync = true;
    minuteWake = true;

    morningAlarm.active = true;
    bool full_repeat[7] = {true,true,true,true,true,true,true};
    memccpy(morningAlarm.repeat_days, full_repeat, 7, sizeof(bool));
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
        syncNTP(GMT_OFFSET_SEC, NTP_SERVER);
        needsSync = false;
    }
    updateWatchAlarms();
}

void TimeService::updateWatchAlarms() {
    SERIAL_LOG_D("Checking alarms...");
    if (!watchAlarm.active) {
        SERIAL_LOG_D("No active alarms.");
        return;
    }

    SERIAL_LOG_D("Still waiting for alarm at ", watchAlarm.next_time.Hour, ":",  watchAlarm.next_time.Minute, " on Day ", watchAlarm.next_time.Day);

    // find the next alarm time
    tmElements_t current_time = get_time_formatted();
    if (matchUpToSeconds(current_time, watchAlarm.next_time)) {
        triggerAlarm();
    }

    if (isPast(watchAlarm.next_time)) {
        SERIAL_LOG_D("Alarm has passed.");
        unsigned current_weekday = current_time.Wday;
        bool repeat_set = false;
        for (int i = 1; i < 7; i++) {
            unsigned next_weekday = (current_weekday + 1 % 7);
            if (watchAlarm.repeat_days[next_weekday] == true) {
                // TODO check for month rollover or directly do computation using time_t
                watchAlarm.next_time.Day += i;
                repeat_set = true;
            }
        }

        watchAlarm.active = repeat_set;
        if (repeat_set) {
            SERIAL_LOG_D("Reset alarm to ", watchAlarm.next_time.Hour, ":",  watchAlarm.next_time.Minute, " on Day ", watchAlarm.next_time.Day);
        } else {
            SERIAL_LOG_D("No reset time found. Alarm is inactive now.");
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
}

void TimeService::setAlarm(tmElements_t next_time, bool repeat_days[7]) {
    watchAlarm.next_time = next_time;
    memccpy(watchAlarm.repeat_days, repeat_days, 7, sizeof(bool));
    watchAlarm.active = true;
    SERIAL_LOG_I("Set an alarm at ", watchAlarm.next_time.Hour, ":",  watchAlarm.next_time.Minute, " on Day ", watchAlarm.next_time.Day);
}

void TimeService::stopAlarm() {
    watchAlarm.active = false;
    SERIAL_LOG_I("Stopped alarm");
}

WatchAlarm TimeService::getAlarm() {
    return watchAlarm;
}

time_t TimeService::timeToAlarm() {
    /// returns time until the next alarm in seconds
    /// returns value <= 0 if alarm has already passed or is inactive

    if (!watchAlarm.active) {
        return -1;
    }

    return makeTime(watchAlarm.next_time) - get_time_unix();
}

void TimeService::setRtcInterrupt() {
    if (minuteWake) {
        SERIAL_LOG_D("Waking at the next minute");
        RTC.setAlarmNextMinute();
    }
}

tmElements_t TimeService::nextAlarm() {
    /// Determines the next closest alarm to be fired

   if (watchAlarm.active && !isPast(watchAlarm.next_time)) {
       return watchAlarm.next_time;
   }
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