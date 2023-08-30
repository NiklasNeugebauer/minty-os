//
// Created by niklas on 6/30/23.
//

#include "WatchFaces/AppSwitcher.h"
#include "SerialLogger.h"
#include "WatchFaces/DebugFace.h"
#include "WatchFaces/PebbleClock.h"
#include "WatchFaces/TimerFace.h"

RTC_DATA_ATTR unsigned short FACE_INDEX = 0;

AppSwitcher::AppSwitcher() {
    // REMEMBER TO CHANGE N_FACES IN AppSwitcher.h !
    faces[0] = new PebbleClock();
    faces[1] = new DebugFace();
    faces[2] = new TimerFace();
    // REMEMBER TO CHANGE N_FACES IN AppSwitcher.h !

    fullDraw = false;
};

AppSwitcher::~AppSwitcher() {
    for (int i = 0; i < N_FACES; i++) {
        delete faces[i];
    }
}

void AppSwitcher::draw(GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *display) {
    SERIAL_LOG_D("Handling Draw");
    currentFace()->draw(display);
}

void AppSwitcher::handleInput(ActionState actionState) {
    SERIAL_LOG_D("Handling Button");
    if (actionState == ActionState(LONG_PRESS, UNPRESSED, UNPRESSED, UNPRESSED)) {
        FACE_INDEX = (FACE_INDEX + 1) % N_FACES;
        SERIAL_LOG_D("Set face to index '", FACE_INDEX, "'");
        fullDraw = true;
    } else if (actionState == ActionState(UNPRESSED, LONG_PRESS, UNPRESSED, UNPRESSED)) {
        FACE_INDEX = (FACE_INDEX + N_FACES - 1) % N_FACES;
        SERIAL_LOG_D("Set face to index '", FACE_INDEX, "'");
        fullDraw = true;
    } else {
        SERIAL_LOG_D("Relaying input to watch face");
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

bool AppSwitcher::shouldDrawFull() {
    return fullDraw || currentFace()->shouldDrawFull();
}