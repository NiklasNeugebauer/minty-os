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


#endif //MINTY_OS_HAL_H
