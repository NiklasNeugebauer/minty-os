//
// Created by niklas on 7/9/23.
//

#ifndef MINTY_OS_DEBUGFACE_H
#define MINTY_OS_DEBUGFACE_H

#include "WatchFace.h"

class DebugFace : public WatchFace{
public:
    void draw(Display* display) override;
    virtual void handleInput(ActionState actionState);
};


#endif //MINTY_OS_DEBUGFACE_H
