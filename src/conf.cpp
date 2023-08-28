//
// Created by niklas on 8/28/23.
//

#include "esp_attr.h"
#include "conf.h"

RTC_DATA_ATTR configuration globalConfig;

configuration Config::getGlobalConfig() {
    return globalConfig;
}