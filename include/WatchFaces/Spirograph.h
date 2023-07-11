//
// Created by niklas on 7/2/23.
//

#ifndef MINTY_OS_SPIROGRAPH_H
#define MINTY_OS_SPIROGRAPH_H

#include "WatchFace.h"

class Spirograph :  public WatchFace{
public:
    void draw(Display *display) override;
};


#endif //MINTY_OS_SPIROGRAPH_H
