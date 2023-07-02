//
// Created by niklas on 7/2/23.
//

#include "WatchFaces/PebbleClock.h"
#include "bitmaps/backgrounds/pebbleBackground.h"
#include "Fonts/FreeMonoBold24pt7b.h"

void PebbleClock::draw(Display display) {
    TimeService* time_service = ServiceManager::getInstance()->time_service;
    tmElements_t current_time = time_service->get_time_formatted();
    long unix_time = makeTime(current_time);

    display.setFullWindow();
    display.fillScreen(GxEPD_BLACK);
    display.drawBitmap(0,0,epd_bitmap_pebbleBackground,200,200,GxEPD_WHITE);

    display.setFont(&FreeMonoBold24pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(28, 158);
    display.printf("%02d:%02d\n", current_time.Hour, current_time.Minute);
    display.display(false);
}