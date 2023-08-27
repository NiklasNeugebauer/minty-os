//
// Created by niklas on 7/11/23.
//

#include "WatchFaces/TimerFace.h"
#include "Fonts/FreeMonoBold18pt7b.h"

#include "services/TimeService.h"

RTC_DATA_ATTR unsigned duration_minutes;
RTC_DATA_ATTR unsigned time_set_index;
RTC_DATA_ATTR unsigned timer_index;

void drawCountdown(GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *display){
    unsigned display_duration_seconds = 0;

    time_t time_to_alarm = TimeService::timeToAlarm(0);
    bool timer_active = time_to_alarm > 0;

    if (timer_active) {
        display_duration_seconds = max((time_t)0, time_to_alarm);
    } else {
        display_duration_seconds = duration_minutes * 60;
    }

    unsigned second_part = display_duration_seconds % 60;
    unsigned tmp_rest_duration = display_duration_seconds / 60;
    unsigned minute_part = tmp_rest_duration % 60;
    unsigned hour_part = tmp_rest_duration / 60;

    display->setCursor(15, 90);

    display->printf("%02d:%02d:%02d", hour_part, minute_part, second_part);
    unsigned character_slots[] = {0,1, 3,4, 6,7};
    display->drawRect(15 + 21 * character_slots[time_set_index], 100, 18, 3, GxEPD_WHITE);

    if (timer_active) {
        display->drawRect(13, 65, 174, 30, GxEPD_WHITE);
    }
}

void drawAlarm(GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *display) {

    display->setCursor(15, 90);
}

void TimerFace::draw(GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *display) {

    display->setFullWindow();
    display->fillScreen(GxEPD_BLACK);

    display->setFont(&FreeMonoBold18pt7b);
    display->setTextColor(GxEPD_WHITE);

    drawCountdown(display);

}

void TimerFace::handleInput(ActionState actionState) {
    time_t time_to_alarm_s = TimeService::timeToAlarm(0);
    time_t time_to_alarm_min = time_to_alarm_s / 60;
    bool timer_active = time_to_alarm_s > 0;
    bool step_size_min;
    switch (time_set_index) {
        case 3: step_size_min = 1; break;
        case 2: step_size_min = 10; break;
        case 1: step_size_min = 60; break;
        case 0: step_size_min = 600; break;
    }

    if (actionState == ActionState(UNPRESSED, UNPRESSED, SHORT_PRESS, UNPRESSED)) {
        time_set_index = --time_set_index % 4;
    } else if (actionState == ActionState(UNPRESSED, SHORT_PRESS, UNPRESSED, UNPRESSED)){
        time_set_index = ++time_set_index % 4;
    } else if (actionState == ActionState(SHORT_PRESS, UNPRESSED, UNPRESSED, UNPRESSED)){
        time_t display_duration;
        if (timer_active) {
            TimeService::stopAlarm(0);
            display_duration = time_to_alarm_min;
        } else {
            display_duration = duration_minutes;
        }

        duration_minutes = (display_duration - step_size_min) > 0 ? (display_duration - step_size_min) : 0;
    } else if (actionState == ActionState(UNPRESSED, UNPRESSED, UNPRESSED, SHORT_PRESS)){
        time_t display_duration;
        if (timer_active) {
            TimeService::stopAlarm(0);
            display_duration = time_to_alarm_min;
        } else {
            display_duration = duration_minutes;
        }

        duration_minutes = display_duration + step_size_min;
    } else if (actionState == ActionState(UNPRESSED, UNPRESSED, LONG_PRESS, UNPRESSED)){
        time_t current_time = TimeService::get_time_unix();
        tmElements_t alarmTime;
        breakTime(current_time + duration_minutes * 60, alarmTime);
        bool repeatDays[7] = {false, false, false, false, false, false};
        TimeService::setAlarm(0, alarmTime, repeatDays);
    }
}