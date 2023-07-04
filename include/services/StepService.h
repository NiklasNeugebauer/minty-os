//
// Created by niklas on 7/3/23.
//

#ifndef MINTY_OS_STEPSERVICE_H
#define MINTY_OS_STEPSERVICE_H

#include "hal/bma.h"

class StepService {
public:
    StepService();
    static void init();
    static void update();
    static uint32_t get_steps();
};

extern RTC_DATA_ATTR BMA423 sensor;

#endif //MINTY_OS_STEPSERVICE_H
