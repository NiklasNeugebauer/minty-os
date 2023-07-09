//
// Created by niklas on 7/5/23.
//

#include "InteractionHandler.h"

#include "config.h"
#include "hal/hal.h"
#include "SerialLogger.h"

InteractionHandler::InteractionHandler() {
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();

    if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT1) {
        uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
        state.MENU = !(wakeupBit & MENU_BTN_MASK);
        state.BACK = !(wakeupBit & BACK_BTN_MASK);
        state.UP = !(wakeupBit & UP_BTN_MASK);
        state.DOWN = !(wakeupBit & DOWN_BTN_MASK);
    }
}

bool InteractionHandler::finished() {
    /// The handler is finished if all buttons are up or there has been a long press
    return !(state.MENU || state.BACK || state.UP || state.DOWN)
            || actions.MENU == LONG_PRESS
            || actions.BACK == LONG_PRESS
            || actions.UP == LONG_PRESS
            || actions.DOWN == LONG_PRESS;
}

void InteractionHandler::poll() {
    updateState(state.MENU, digitalRead(MENU_BTN_PIN));
    updateState(state.BACK, digitalRead(BACK_BTN_PIN));
    updateState(state.UP, digitalRead(UP_BTN_PIN));
    updateState(state.DOWN, digitalRead(DOWN_BTN_PIN));

    SERIAL_LOG_D("MENU: ", state.MENU, ", BACK: ", state.BACK, ", UP: ", state.UP, ", DOWN: ", state.DOWN);

    updateAction(actions.MENU, state.MENU);
    updateAction(actions.BACK, state.BACK);
    updateAction(actions.UP, state.UP);
    updateAction(actions.DOWN, state.DOWN);

    pullUpActions();
}

ActionState InteractionHandler::getActions() {
    return actions;
}

void InteractionHandler::pullUpActions() {
    /// This function's purpose is to create double long presses
    /// even if not both buttons have been pressed long enough
    /// This also enables finishing before letting go of the buttons
    const unsigned SECONDARY_FACTOR = 2;

    bool anyLongPress = actions.MENU == LONG_PRESS
                        || actions.BACK == LONG_PRESS
                        || actions.UP == LONG_PRESS
                        || actions.DOWN == LONG_PRESS;

    if (anyLongPress) {
        updateAction(actions.MENU, SECONDARY_FACTOR * state.MENU);
        updateAction(actions.BACK, SECONDARY_FACTOR * state.BACK);
        updateAction(actions.UP, SECONDARY_FACTOR * state.UP);
        updateAction(actions.DOWN, SECONDARY_FACTOR * state.DOWN);
    }
}

void InteractionHandler::updateState(unsigned int &var, bool active) {
    var = active * var + active;
}

void InteractionHandler::updateAction(ButtonAction &var, unsigned int state) {
    if (state > HOLD_TICKS && var < LONG_PRESS) {
        var = LONG_PRESS;
    } else if (state > 0 && var < SHORT_PRESS) {
        var = SHORT_PRESS;
    }
}