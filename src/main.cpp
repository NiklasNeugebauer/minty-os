#include "SerialLogger.h"
#include "MintyBase.h"
#include "InteractionHandler.h"
#include "WatchFaces/AppSwitcher.h"

MintyBase base;
InteractionHandler interactionHandler;

WatchFace *watch_face;

void handleInput() {
    ActionState action = interactionHandler.getActions();
    SERIAL_LOG_D("Action State: ", action.print());
    if (action.BACK == LONG_PRESS && action.UP == LONG_PRESS) {
        base.reboot();
    } else {
        watch_face->handleInput(action);
    }

    base.initializeDisplay();
    watch_face->draw(&base.display);
    base.display.display(!watch_face->shouldDrawFull());
}

void setup () {
    base.wakeupRoutine();
    watch_face = new AppSwitcher();
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