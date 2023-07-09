//
// Created by niklas on 7/1/23.
//

#ifndef MINTY_OS_BATTERYMANAGER_H
#define MINTY_OS_BATTERYMANAGER_H

#include "Service.h"

class BatteryManager : Service{
private:
    static float getBatteryVoltage();
public:
    static void init();
    static void update();
    static unsigned getBatteryLevel();
};


#endif //MINTY_OS_BATTERYMANAGER_H
