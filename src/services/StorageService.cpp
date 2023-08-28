//
// Created by niklas on 8/28/23.
//

#include "services/StorageService.h"
#include <ArduinoNvs.h>
#include "hal/hal.h"


void StorageService::init() {
    NVS.begin();
}

void StorageService::update() {

}