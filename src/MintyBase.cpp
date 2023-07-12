//
// Created by niklas on 6/30/23.
//

#include "../include/MintyBase.h"

#include "hal/pins.h"
#include "SerialLogger.h"
#include "ServiceManager.h"

#include "services/TimeService.h"

#include "InteractionHandler.h"
#include "WatchFaces/AppSwitcher.h"

#include "bitmaps/mintyOS.h"

InteractionHandler interactionHandler;

WatchFace *watch_face;

//RTC_DATA_ATTR int guiState;
//RTC_DATA_ATTR int menuIndex;
//RTC_DATA_ATTR BMA423 sensor;
//RTC_DATA_ATTR bool WIFI_CONFIGURED;
//RTC_DATA_ATTR bool BLE_CONFIGURED;
RTC_DATA_ATTR bool isFirstStartup = true;
//RTC_DATA_ATTR long gmtOffset = 0;
//RTC_DATA_ATTR bool alreadyInMenu         = true;
//RTC_DATA_ATTR tmElements_t bootTime;

GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> MintyBase::display(
        WatchyDisplay(DISPLAY_CS, DISPLAY_DC, DISPLAY_RES, DISPLAY_BUSY));


void MintyBase::wakeupRoutine() {
    if (isFirstStartup) {
        Wire.begin(SDA, SCL); // init i2c
        Serial.begin(115200);
        SERIAL_LOG_I("Welcome to Minty-OS!");
        initializeDisplay();
        display.drawBitmap(0,0,epd_bitmap_BootLogo,200,200,GxEPD_WHITE);
        display.display(false);
        ServiceManager::init();
        esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
    } else {
        SERIAL_LOG_I("Running wakeup routine!");
        Serial.begin(115200);
        //Wire.begin(SDA, SCL); // init i2c
    }
    SERIAL_LOG_D("Wakeup reason: ", esp_sleep_get_wakeup_cause());
    watch_face = new AppSwitcher();
}

void MintyBase::loop() {
    SERIAL_LOG_I("Loop");
    interactionHandler.poll();
    if (interactionHandler.finished()) {
        SERIAL_LOG_I("  done.");
        handleInput();
        deepSleep();
    }
    SERIAL_LOG_I("  InteractionHandler busy!");
    delay(50);
}

void MintyBase::initializeDisplay() {
    // Init the display here for all cases, if unused, it will do nothing
    display.epd2.selectSPI(SPI, SPISettings(20000000, MSBFIRST, SPI_MODE0)); // Set SPI to 20Mhz (default is 4Mhz)
    display.init(0, isFirstStartup, 10,
                 true); // 10ms by spec, and fast pulldown reset
    display.epd2.setBusyCallback(displayBusyCallback);
}

void MintyBase::deepSleep() {
    SERIAL_LOG_I("Getting sleepy...");
    display.hibernate();
    SERIAL_LOG_I("1");
    TimeService::setRtcInterrupt();
    SERIAL_LOG_I("2");
    // Set GPIOs 0-39 to input to avoid power leaking out
    const uint64_t ignore = 0b11110001000000110000100111000010; // Ignore some GPIOs due to resets
    for (int i = 0; i < GPIO_NUM_MAX; i++) {
        if ((ignore >> i) & 0b1)
            continue;
        pinMode(i, INPUT);
    }
    SERIAL_LOG_I("3");
    esp_sleep_enable_ext0_wakeup((gpio_num_t)RTC_INT_PIN,
                                 0); // enable deep sleep wake on RTC interrupt
    SERIAL_LOG_I("4");
    esp_sleep_enable_ext1_wakeup(
            BTN_PIN_MASK,
            ESP_EXT1_WAKEUP_ANY_HIGH); // enable deep sleep wake on button press
    SERIAL_LOG_I("5");
    if (isFirstStartup) {
        isFirstStartup = false;
    }

    esp_deep_sleep_start();
}

void MintyBase::handleInput() {
    ActionState action = interactionHandler.getActions();
    SERIAL_LOG_D("Action State: ", action.print());
    if (action.BACK == LONG_PRESS && action.UP == LONG_PRESS) {
        reboot();
    } else {
        watch_face->handleInput(action);
    }

    initializeDisplay();
    watch_face->draw(&display);
    display.display(!(watch_face->shouldDrawFull() || isFirstStartup));
}

void MintyBase::displayBusyCallback(const void *) {
    gpio_wakeup_enable((gpio_num_t)DISPLAY_BUSY, GPIO_INTR_LOW_LEVEL);
    esp_sleep_enable_gpio_wakeup();
    esp_light_sleep_start();
}

void MintyBase::reboot() {
    SERIAL_LOG_I("Rebooting...");
    ESP.restart();
}