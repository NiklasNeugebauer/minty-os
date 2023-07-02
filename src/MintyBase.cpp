//
// Created by niklas on 6/30/23.
//

#include "../include/MintyBase.h"

#include "hal/pins.h"
#include "conf.h"

//RTC_DATA_ATTR int guiState;
//RTC_DATA_ATTR int menuIndex;
//RTC_DATA_ATTR BMA423 sensor;
//RTC_DATA_ATTR bool WIFI_CONFIGURED;
//RTC_DATA_ATTR bool BLE_CONFIGURED;
RTC_DATA_ATTR bool displayFullInit       = true;
//RTC_DATA_ATTR long gmtOffset = 0;
//RTC_DATA_ATTR bool alreadyInMenu         = true;
//RTC_DATA_ATTR tmElements_t bootTime;

WatchyRTC MintyBase::RTC;
GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> MintyBase::display(
        WatchyDisplay(DISPLAY_CS, DISPLAY_DC, DISPLAY_RES, DISPLAY_BUSY));

void MintyBase::wakeupRoutine() {
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause(); // get wake up reason
    Wire.begin(SDA, SCL);                         // init i2c

    // Init the display here for all cases, if unused, it will do nothing
    display.epd2.selectSPI(SPI, SPISettings(20000000, MSBFIRST, SPI_MODE0)); // Set SPI to 20Mhz (default is 4Mhz)
    display.init(0, displayFullInit, 10,
                 true); // 10ms by spec, and fast pulldown reset
    //display.epd2.setBusyCallback(displayBusyCallback);
}

void MintyBase::deepSleep() {
    display.hibernate();
    if (displayFullInit) // For some reason, seems to be enabled on first boot
        esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
    displayFullInit = false; // Notify not to init it again
    RTC.clearAlarm();        // resets the alarm flag in the RTC

    // Set GPIOs 0-39 to input to avoid power leaking out
    const uint64_t ignore = 0b11110001000000110000100111000010; // Ignore some GPIOs due to resets
    for (int i = 0; i < GPIO_NUM_MAX; i++) {
        if ((ignore >> i) & 0b1)
            continue;
        pinMode(i, INPUT);
    }
    esp_sleep_enable_ext0_wakeup((gpio_num_t)RTC_INT_PIN,
                                 0); // enable deep sleep wake on RTC interrupt
    esp_sleep_enable_ext1_wakeup(
            BTN_PIN_MASK,
            ESP_EXT1_WAKEUP_ANY_HIGH); // enable deep sleep wake on button press
    esp_deep_sleep_start();
}

void MintyBase::displayBusyCallback(const void *) {
    gpio_wakeup_enable((gpio_num_t)DISPLAY_BUSY, GPIO_INTR_LOW_LEVEL);
    esp_sleep_enable_gpio_wakeup();
    esp_light_sleep_start();
}