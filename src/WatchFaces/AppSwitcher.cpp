//
// Created by niklas on 6/30/23.
//

#include "WatchFaces/AppSwitcher.h"
#include "SerialLogger.h"
#include "WatchFaces/HelloWorld.h"
#include "WatchFaces/PebbleClock.h"
#include "WatchFaces/BasicTime.h"

RTC_DATA_ATTR unsigned FACE_INDEX = 0;

AppSwitcher::AppSwitcher() {
    faces[0] = new PebbleClock();
    faces[1] = new HelloWorld();
    faces[2] = new BasicTime();
};

AppSwitcher::~AppSwitcher() {
    for (int i = 0; i < N_FACES; i++) {
        delete faces[i];
    }
}

void AppSwitcher::draw(GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *display) {
    SERIAL_LOG_D("Handling Draw");
    currentFace()->draw(display);
    SERIAL_LOG_D("Draw Done.");
}

void AppSwitcher::handleInput(ActionState actionState) {
    SERIAL_LOG_D("Handling Button");
    if (actionState == ActionState(LONG_PRESS, UNPRESSED, UNPRESSED, UNPRESSED)) {
        FACE_INDEX = (FACE_INDEX + 1) % N_FACES;
    } else if (actionState == ActionState(UNPRESSED, LONG_PRESS, UNPRESSED, UNPRESSED)) {
        FACE_INDEX = (FACE_INDEX - 1) % N_FACES;
    } else {
        currentFace()->handleInput(actionState);
    }
}

WatchFace* AppSwitcher::currentFace() {
    if (FACE_INDEX < N_FACES) {
        return faces[FACE_INDEX];
    } else {
        return faces[0];
    }
}