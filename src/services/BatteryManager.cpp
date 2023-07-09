//
// Created by niklas on 7/1/23.
//

#include "services/BatteryManager.h"

#include "Arduino.h"
#include "hal/pins.h"

RTC_DATA_ATTR float maxVoltage;
RTC_DATA_ATTR float minVoltage;

void BatteryManager::init() {
    maxVoltage = 4;
    minVoltage = 3.5;
}

void BatteryManager::update() {

}

unsigned BatteryManager::getBatteryLevel() {
    /// computes battery percentage using ZeroKelvinKeyboards magic LiPo discharge formula
    float voltage = getBatteryVoltage();
    float v = pow(voltage / 3.7, 80);
    int percentage = 123 - (123 / pow(1 + v, 0.165));
    return min(100, max(0, percentage));
}

float BatteryManager::getBatteryVoltage() {
    return analogReadMilliVolts(BATT_ADC_PIN) / 1000.0f * 2.0f;
}