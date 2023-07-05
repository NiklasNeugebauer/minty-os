//
// Created by niklas on 7/2/23.
//

#include "WatchFaces/PebbleClock.h"
#include "bitmaps/pebble.h"
#include "Fonts/FreeMonoBold24pt7b.h"
#include "Fonts/FreeMonoBold18pt7b.h"

#include "services/TimeService.h"
#include "services/StepService.h"

void PebbleClock::draw(Display display) {

    tmElements_t current_time = TimeService::get_time_formatted();
    long unix_time = makeTime(current_time);

    display.setFullWindow();
    display.fillScreen(GxEPD_BLACK);
    display.drawBitmap(0,0,epd_bitmap_pebbleBackground,200,200,GxEPD_WHITE);
    display.drawBitmap(25,10,epd_bitmap_PebbleCal,50,50,GxEPD_BLACK);
    display.drawBitmap(125,10,epd_bitmap_PebbleStep,50,50,GxEPD_BLACK);

    display.setFont(&FreeMonoBold24pt7b);
    display.setTextColor(GxEPD_BLACK);
    // TIME
    display.setCursor(28, 158);
    display.printf("%02d:%02d", current_time.Hour, current_time.Minute);

    display.setFont(&FreeMonoBold18pt7b);
    // DATE
    uint8_t current_day = current_time.Day;
    int16_t day_x_pos = 40;
    display.setCursor(day_x_pos, 50);
    display.printf("%d", current_time.Day);

    String weekdays[] = {"SO", "MO", "DI", "MI", "DO", "FR", "SA"};
    display.setCursor(30, 90);
    display.printf("%s", weekdays[current_time.Wday - 1].c_str());

    // STEPS
    uint32_t steps = StepService::get_steps();
    display.setCursor(105, 90);
    display.printf("%04d", steps);
}