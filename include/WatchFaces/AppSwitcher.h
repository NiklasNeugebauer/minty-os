//
// Created by niklas on 6/30/23.
//

#ifndef MINTY_OS_APPSWITCHER_H
#define MINTY_OS_APPSWITCHER_H

#include "WatchFace.h"


class AppSwitcher : public WatchFace {
private:
    const static unsigned N_FACES = 3;
    WatchFace* faces[N_FACES];
    WatchFace* face;
    bool fullDraw;

    WatchFace* currentFace();
public:
    AppSwitcher();
    ~AppSwitcher();
    void draw(Display *display) override;
    void handleInput(ActionState actionState) override;
    bool shouldDrawFull() override;
    bool keepAwake() override;
};


#endif //MINTY_OS_APPSWITCHER_H
