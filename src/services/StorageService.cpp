//
// Created by niklas on 8/28/23.
//

#include "services/StorageService.h"
#include <ArduinoNvs.h>
#include "hal/hal.h"
#include "SerialLogger.h"

void StorageService::init() {
    SERIAL_LOG_I("Initializing Non Volatile Storage");
    NVS.begin();
    // check if storage has been initialized
    String version_num = NVS.getString("OS_Version");
    if (version_num.isEmpty()) {
        // version number was not saved, assume storage is empty
        SERIAL_LOG_D("No previous version info found.");
    } else {
        SERIAL_LOG_D("Previous Storage from version ", version_num);
    }

    NVS.setString("OS_Version", MINTY_OS_VERSION_NUM);
    configuration current_config = Config::getGlobalConfig();
    const configuration saved_config;
    bool config_found = NVS.getBlob("Config", (uint8_t*)&saved_config, sizeof(configuration));
    if (config_found) {
        SERIAL_LOG_D("Previous configuration found! Restoring...");
        memcpy(&current_config, &saved_config, sizeof(configuration));
    } else {
        SERIAL_LOG_D("No previous configuration found! Using default configuration...");
        NVS.setBlob("Config", (uint8_t*)&current_config, sizeof(configuration));
    }

    NVS.commit();
}

void StorageService::update() {
    NVS.begin();
}