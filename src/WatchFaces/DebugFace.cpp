//
// Created by niklas on 7/9/23.
//

#include "WatchFaces/DebugFace.h"

#include "Fonts/FreeMonoBold9pt7b.h"

#include "services/TimeService.h"
#include "services/StepService.h"
#include "services/BatteryManager.h"

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
    display->printf(" Datum: %02d.%02d.%04d\n", current_time.Day, current_time.Month, 1970 + current_time.Year);

    // TIME
    display->printf(" Zeit: %02d:%02d\n", current_time.Hour, current_time.Minute);

    // WEEKDAY
    display->printf(" Tag: %s\n", weekdays[current_time.Wday].c_str());

    // STEPS
    uint32_t steps = StepService::get_steps();
    display->printf(" Step: %05d\n", steps);

    // BATTERY
    display->printf(" Akku: %03d (%.2fV)\n", BatteryManager::getBatteryLevel(), BatteryManager::getBatteryVoltage());
}

void DebugFace::handleInput(ActionState actionState) {

}