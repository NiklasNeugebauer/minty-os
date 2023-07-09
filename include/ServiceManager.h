//
// Created by niklas on 7/2/23.
//

#ifndef MINTY_OS_SERVICEMANAGER_H
#define MINTY_OS_SERVICEMANAGER_H

#include "services/TimeService.h"
#include "services/StepService.h"
#include "services/WifiService.h"

class ServiceManager {
public:
    static void init() {
        TimeService::init();
        StepService::init();
        WifiService::init();

        update();
    }

    static void update() {
        TimeService::update();
        StepService::update();
        WifiService::update();
    }
};


#endif //MINTY_OS_SERVICEMANAGER_H
