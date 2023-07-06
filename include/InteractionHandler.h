//
// Created by niklas on 7/5/23.
//

#ifndef MINTY_OS_INTERACTIONHANDLER_H
#define MINTY_OS_INTERACTIONHANDLER_H

#include <sstream>

struct ButtonState {
    unsigned UP = 0;
    unsigned DOWN = 0;
    unsigned MENU = 0;
    unsigned BACK = 0;
};

enum ButtonAction{
    UNPRESSED,
    SHORT_PRESS,
    LONG_PRESS
};

struct ActionState {
    ButtonAction UP;
    ButtonAction DOWN;
    ButtonAction MENU;
    ButtonAction BACK;

    ActionState(ButtonAction up, ButtonAction down, ButtonAction menu, ButtonAction back)
    : UP(up), DOWN(down), MENU(menu), BACK(back)
    {}

    ActionState()
    : UP(UNPRESSED), DOWN(UNPRESSED), MENU(UNPRESSED), BACK(UNPRESSED)
    {}

    std::string print() {
        std::stringstream ss;

        ss << "UP: " << UP << "  DOWN: " << DOWN << "  MENU: " << MENU << "  BACK: " << BACK;

        std::string result = ss.str();
        return result;
    }

    bool operator==(const ActionState& other) const {
        return UP == other.UP &&
               DOWN == other.DOWN &&
               MENU == other.MENU &&
               BACK == other.BACK;
    }
};

class InteractionHandler {
private:
    ButtonState state;
    ActionState actions;

    void updateAction(ButtonAction &var, unsigned int state);
    void updateState(unsigned& var, bool active);
public:
    InteractionHandler();
    bool finished();
    void poll();
    ActionState getActions();
};


#endif //MINTY_OS_INTERACTIONHANDLER_H
