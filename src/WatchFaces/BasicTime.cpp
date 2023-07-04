//
// Created by niklas on 7/2/23.
//

#include "WatchFaces/BasicTime.h"
#include <Fonts/FreeMonoBold9pt7b.h>

#include "services/TimeService.h"

void BasicTime::draw(Display display) {
    tmElements_t current_time = TimeService::get_time_formatted();
    long unix_time = makeTime(current_time);

    display.setFullWindow();
    display.fillScreen(GxEPD_WHITE);
    display.setFont(&FreeMonoBold9pt7b);
    display.setTextColor(GxEPD_BLACK);

    display.setCursor(0, 10);
    display.printf("Unix time: %016d\n", unix_time);
    display.printf("Time: %02d:%02d\n", current_time.Hour, current_time.Minute);
    display.printf("Time: %02d.%02d\n", current_time.Day, current_time.Month);
    display.display(false);
}