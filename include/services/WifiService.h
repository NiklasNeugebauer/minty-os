//
// Created by niklas on 7/7/23.
//

#ifndef MINTY_OS_WIFISERVICE_H
#define MINTY_OS_WIFISERVICE_H

#include "hal/hal.h"

#include <WiFi.h>

#define N_APS 2

struct WifiInfo final {
    struct APInfo {
        char APID[33];
        char PASS[65];
        uint8_t TPWRIndex;
    } AP[N_APS];                // Using APID to avoid internal confusion with SSID.
    wifi_power_t TransmitPower;
    wifi_event_id_t WiFiEventID;
};

class WifiService {
private:
public:
    static void init();
    static void update();
    static wl_status_t connect();
    static wl_status_t connect(char ssid[], char password[]);
    static wl_status_t disconnect();
};


#endif //MINTY_OS_WIFISERVICE_H
