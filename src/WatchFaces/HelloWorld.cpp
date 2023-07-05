//
// Created by niklas on 7/2/23.
//

#include "WatchFaces/HelloWorld.h"

void HelloWorld::draw(Display display) {
    display.setFullWindow();
    display.fillScreen(GxEPD_WHITE);
    display.setFont(&FreeMonoBold9pt7b);
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(5, 53+5);

    display.printf("Wakeup: %d", 42);
}