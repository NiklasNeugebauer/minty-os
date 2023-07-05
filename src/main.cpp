#include "SerialLogger.h"
#include "MintyBase.h"
#include "InteractionHandler.h"
#include "WatchFaces/HelloWorld.h"
#include "WatchFaces/BasicTime.h"
#include "WatchFaces/PebbleClock.h"

//#include "./apps/_experiments/hello_world.h"

MintyBase base;
InteractionHandler interactionHandler;

//HelloWorld watch_face;
//BasicTime watch_face;
PebbleClock watch_face;

void handleInput() {
    ButtonState action = interactionHandler.getActions();
    SERIAL_LOG_D("Action State: ", *(long int*)&action);
    if (action.BACK == LONG_PRESS && action.UP == LONG_PRESS) {
        base.reboot();
    } else {
        base.initializeDisplay();
        watch_face.draw(base.display);
    }
}

void setup () {
    base.wakeupRoutine();
}

void loop () {
    SERIAL_LOG_I("Loop");
    interactionHandler.poll();
    if (interactionHandler.finished()) {
        SERIAL_LOG_I("  done.");
        handleInput();
        base.deepSleep();
    }
    SERIAL_LOG_I("  InteractionHandler busy!");
    delay(50);
}