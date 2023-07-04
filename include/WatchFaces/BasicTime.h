//
// Created by niklas on 7/2/23.
//

#ifndef MINTY_OS_BASICTIME_H
#define MINTY_OS_BASICTIME_H

#include "WatchFace.h"

class BasicTime : WatchFace{
private:
public:
    void draw(Display display) override;
};


#endif //MINTY_OS_BASICTIME_H
