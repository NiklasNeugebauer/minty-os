//
// Created by niklas on 7/2/23.
//

#ifndef MINTY_OS_SERVICEMANAGER_H
#define MINTY_OS_SERVICEMANAGER_H

#include "services/TimeService.h"
#include "services/StepService.h"
#include "services/WifiService.h"
#include "services/BluetoothService.h"
#include "services/BatteryManager.h"
#include "services/VibrationService.h"

class ServiceManager {
public:
    static void init() {
        BatteryManager::init();
        VibrationService::init();
        TimeService::init();
        StepService::init();
        WifiService::init();
        BluetoothService::init();

        update();
    }

    static void update() {
        BatteryManager::update();
        VibrationService::update();
        TimeService::update();
        StepService::update();
        WifiService::update();
        BluetoothService::update();
    }
};


#endif //MINTY_OS_SERVICEMANAGER_H
