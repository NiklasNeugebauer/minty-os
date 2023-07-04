//
// Created by niklas on 7/2/23.
//

#ifndef MINTY_OS_HAL_H
#define MINTY_OS_HAL_H

#include "Display.h"
#include <GxEPD2_BW.h>

//#include <Arduino.h>
//#include <WiFiManager.h>
//#include <HTTPClient.h>
//#include <NTPClient.h>
//#include <WiFiUdp.h>
//#include <Arduino_JSON.h>
//#include <Wire.h>
//#include <Fonts/FreeMonoBold9pt7b.h>
//#include "DSEG7_Classic_Bold_53.h"
//#include "BLE.h"
//#include "bma.h"
#include "conf.h"
#include "pins.h"
#include "WatchyRTC.h"

#define Display GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT>

class HAL {
public:
    static uint16_t readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len) {
        Wire.beginTransmission(address);
        Wire.write(reg);
        Wire.endTransmission();
        Wire.requestFrom((uint8_t)address, (uint8_t)len);
        uint8_t i = 0;
        while (Wire.available()) {
            data[i++] = Wire.read();
        }
        return 0;
    }

    static uint16_t writeRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len) {
        Wire.beginTransmission(address);
        Wire.write(reg);
        Wire.write(data, len);
        return (0 != Wire.endTransmission());
    }
};

#endif //MINTY_OS_HAL_H
