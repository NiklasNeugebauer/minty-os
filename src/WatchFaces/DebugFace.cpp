//
// Created by niklas on 7/9/23.
//

#include "WatchFaces/DebugFace.h"

#include "Fonts/FreeMonoBold9pt7b.h"

#include "services/TimeService.h"
#include "services/StepService.h"
#include "services/BatteryManager.h"
#include "services/VibrationService.h"

#include <ArduinoNvs.h>

#include "SerialLogger.h"

RTC_DATA_ATTR int string_state = 0;

const String test_strings[3] = {"A", "B", "C"};

void DebugFace::draw(GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *display) {
    tmElements_t current_time = TimeService::get_time_formatted();

    display->setFullWindow();
    display->fillScreen(GxEPD_BLACK);

    display->setFont(&FreeMonoBold9pt7b);
    display->setTextColor(GxEPD_WHITE);
    display->setCursor(0, 20);

    // UNIX:
    long unix_time = makeTime(current_time);
    display->printf(" Unix: %010d\n", unix_time);

    // DATE
    const String weekdays[] = {"Sonntag", "Montag", "Dienstag", "Mittwoch",
                               "Donnerstag", "Freitag", "Samstag"};
    display->printf(" Datum: %02d.%02d.%04d\n", current_time.Day, current_time.Month, tmYearToCalendar(current_time.Year));

    // TIME
    display->printf(" Zeit: %02d:%02d\n", current_time.Hour, current_time.Minute);

    // WEEKDAY
    display->printf(" Tag: %s\n", weekdays[current_time.Wday - 1].c_str());

    // STEPS
    uint32_t steps = StepService::get_steps();
    display->printf(" Step: %05d\n", steps);

    // BATTERY
    display->printf(" Akku: %03d (%.2fV)\n", BatteryManager::getBatteryLevel(), BatteryManager::getBatteryVoltage());

    // Storage
    display->printf("  RAM: %s\n", test_strings[string_state].c_str());

    display->printf("  NVS: ");
    String test_data = NVS.getString("StorageTest");
    if (test_data.isEmpty()) {
        display->printf("Empty\n");
    } else {
        display->printf("%s\n", test_data.c_str());
    }
}

void DebugFace::handleInput(ActionState actionState) {
    if(actionState == ActionState(UNPRESSED, SHORT_PRESS, UNPRESSED, UNPRESSED)) {
        string_state = (string_state + 1) % 3;
    } else if (actionState == ActionState(UNPRESSED, UNPRESSED, LONG_PRESS, UNPRESSED)) {
        NVS.setString("StorageTest", test_strings[string_state].c_str());
        NVS.commit();
    } else {
        SERIAL_LOG_D("brr");
        VibrationService::vibrateMS(100);
    }
}