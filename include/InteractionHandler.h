//
// Created by niklas on 7/5/23.
//

#ifndef MINTY_OS_INTERACTIONHANDLER_H
#define MINTY_OS_INTERACTIONHANDLER_H

struct ButtonState {
    unsigned UP = 0;
    unsigned DOWN = 0;
    unsigned MENU = 0;
    unsigned BACK = 0;
};

enum BUTTON_ACTION{
    UNPRESSED,
    SHORT_PRESS,
    LONG_PRESS
};

class InteractionHandler {
private:
    ButtonState state;
    ButtonState actions;

    void updateAction(unsigned int &var, unsigned int state);
    void updateState(unsigned& var, bool active);
public:
    InteractionHandler();
    bool finished();
    void poll();
    ButtonState getActions();
};


#endif //MINTY_OS_INTERACTIONHANDLER_H
