//
// Created by niklas on 7/7/23.
//

#include "services/WifiService.h"
#include "SerialLogger.h"
#include "wifiSettings.h"

// copy of GSRWireless from Watchy_GSR https://github.com/GuruSR/Watchy_GSR/blob/3dacfa0b2fd3c0bdca33fc4ed79a24780757a52a/src/1.4.7/Watchy_GSR.cpp
RTC_DATA_ATTR WifiInfo wifiInfo;

void WifiService::init() {
    // Load access point information from storage
    // TODO put this into NVS once available
    SERIAL_LOG_I("Starting WiFi Service");
    SERIAL_LOG_I("\tLoading AP information:");
    char ssid[] = WIFI_SSID_1;
    char pw[] = WIFI_PW_1;
    SERIAL_LOG_D("\t\t", ssid, " | ", pw);
    strcpy(wifiInfo.AP[0].APID, ssid);
    strcpy(wifiInfo.AP[0].PASS, pw);
    SERIAL_LOG_D("\t\t", wifiInfo.AP[0].APID, " | ", wifiInfo.AP[0].PASS);
}

void WifiService::update() {
    // TODO probably unused
}

wl_status_t WifiService::connect() {
    /// Try to connect to a saved Access Point
    wl_status_t status = WiFi.status();
    if (status == WL_CONNECTED) {
        return status;
    }

    for (uint8_t i = 0; i < N_APS; i++) {
        char *ssid = wifiInfo.AP[i].APID;
        char *password = wifiInfo.AP[i].PASS;

        SERIAL_LOG_D("Trying to connect to ", ssid, " with password", password, " ...");

        // check if the SSID is empty
        if (ssid[0] == 0) {
            continue;
        }

        status = WiFi.begin(ssid, password);
        if (status == WL_CONNECT_FAILED) {
            continue;
        } else {
            status = (wl_status_t)WiFi.waitForConnectResult();
            if (status == WL_CONNECTED) {
                SERIAL_LOG_I("WiFi Connection Success ( SSID ", wifiInfo.AP[i].APID, " )");
                return status;
            }
        }
    }

    SERIAL_LOG_I("WiFi Connection Failed");
    return status;
}

wl_status_t WifiService::connect(char ssid[], char password[]) {
    /// Try to connect to a specific Access Point
    SERIAL_LOG_I("Connecting to WiFi...");
    return WiFi.begin(ssid, password);
}

wl_status_t WifiService::disconnect() {
    /// Disconnect from current Access Point
    SERIAL_LOG_I("Disconnecting from WiFi...");
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    return WiFi.status();
}
