//
// Created by niklas on 7/13/23.
//

#ifndef MINTY_OS_VIBRATIONSERVICE_H
#define MINTY_OS_VIBRATIONSERVICE_H

#include "Service.h"

class VibrationService : Service{
public:
    VibrationService();
    static void init();
    static void update();
    static void vibrateMS(unsigned milliseconds);
};


#endif //MINTY_OS_VIBRATIONSERVICE_H
