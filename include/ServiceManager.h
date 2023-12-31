//
// Created by niklas on 7/2/23.
//

#ifndef MINTY_OS_SERVICEMANAGER_H
#define MINTY_OS_SERVICEMANAGER_H

#include "services/TimeService.h"
#include "services/StepService.h"
#include "services/WifiService.h"
#include "services/BatteryManager.h"
#include "services/VibrationService.h"
#include "services/StorageService.h"

class ServiceManager {
public:
    static void init() {
        StorageService::init();
        BatteryManager::init();
        VibrationService::init();
        TimeService::init();
        StepService::init();
        WifiService::init();

        update();
    }

    static void update() {
        StorageService::update();
        BatteryManager::update();
        VibrationService::update();
        TimeService::update();
        StepService::update();
        WifiService::update();
    }
};


#endif //MINTY_OS_SERVICEMANAGER_H
