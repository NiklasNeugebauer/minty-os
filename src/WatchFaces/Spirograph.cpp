// Spiral drawing code by rapaul from watchy-spiro at https://github.com/rapaul/watchy-spiro/blob/main/src/WatchySpiro.cpp

#include "WatchFaces/Spirograph.h"

void drawSpiral(Display *display, int strokeSize, int maxRadius, int maxRatio) {
    int cx = 100;
    int cy = 100;
    int r1 = random(5, maxRadius);
    int r2 = maxRadius - r1;
    int ratio1 = random(1, 5);
    int ratio2 = random(1, maxRatio);
    float pi = 3.14159;

    for (float i = 0; i < 2*pi; i += 0.001) {
        int x = cx + r1 * cos(i * ratio1) + r2 * cos(-i * ratio2);
        int y = cy + r1 * sin(i * ratio1) + r2 * sin(-i * ratio2);
        display->drawRect(x, y, strokeSize, strokeSize, GxEPD_BLACK);
    }

    char dimensions[50];
    sprintf(dimensions, "Dimensions: r1=%d r2=%d ratio1=%d ratio2=%d", r1, r2, ratio1, ratio2);
    Serial.println(dimensions);
}

void Spirograph::draw(Display *display) {
    display->setFullWindow();
    display->fillScreen(GxEPD_WHITE);

    drawSpiral(display, 3, 80, 20);
    drawSpiral(display, 4, 60, 10);
}