//
// Created by niklas on 7/13/23.
//

#include "services/VibrationService.h"
#include "hal/hal.h"

VibrationService::VibrationService() {

}

void VibrationService::init() {
}

void VibrationService::update() {
    pinMode(VIB_MOTOR_PIN, OUTPUT);
}

void VibrationService::vibrateMS(unsigned milliseconds) {
    bool motorOn = false;
    unsigned length = 2;
    for (int i = 0; i < length; i++) {
        motorOn = !motorOn;
        digitalWrite(VIB_MOTOR_PIN, motorOn);
        delay(milliseconds);
    }
}