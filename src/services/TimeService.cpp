//
// Created by niklas on 7/2/23.
//

#include "services/TimeService.h"

#include "services/WifiService.h"
#include "SerialLogger.h"

#include <NTPClient.h>
#include <WiFiUdp.h>

WatchyRTC TimeService::RTC;

void TimeService::init() {
    SERIAL_LOG_I("Starting TimeService...");
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