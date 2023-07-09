//
// Created by niklas on 7/2/23.
//

#ifndef MINTY_OS_HELLOWORLD_H
#define MINTY_OS_HELLOWORLD_H

#include "WatchFace.h"
#include <Fonts/FreeMonoBold9pt7b.h>

class HelloWorld : public WatchFace {
public:
    void draw(Display *display) override;
};


#endif //MINTY_OS_HELLOWORLD_H
