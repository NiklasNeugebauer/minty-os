//
// Created by niklas on 7/2/23.
//
// NTP code is based off SQFMI's Watchy.cpp at https://github.com/sqfmi/Watchy/blob/master/src/Watchy.cpp

#include "services/TimeService.h"

#include "services/WifiService.h"
#include "SerialLogger.h"

#include <NTPClient.h>
#include <WiFiUdp.h>

WatchyRTC TimeService::RTC;
RTC_DATA_ATTR WatchAlarm watchAlarm;

void TimeService::init() {
    SERIAL_LOG_I("Starting TimeService...");
    RTC.config("");
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
    syncNTP(GMT_OFFSET_SEC, NTP_SERVER);
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
}

void TimeService::setRtcInterrupt() {
    tmElements_t next_wakeup = nextAlarm();
    SERIAL_LOG_D("Setting next wakeup to ", next_wakeup.Hour, ":",  next_wakeup.Minute, " on Day ", next_wakeup.Day);
    RTC.setAlarm(next_wakeup);
}

tmElements_t TimeService::nextAlarm() {
    /// Determines the next closest alarm to be fired
    int nextAlarmMinute = 0;
    tmElements_t current_time = get_time_formatted();
    tmElements_t nextMinute = current_time;
    nextMinute.Minute = (++current_time.Minute) % 60;
    nextMinute.Second = 0;

    return nextMinute;
}
