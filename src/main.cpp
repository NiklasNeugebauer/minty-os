#include <Arduino.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Arduino_JSON.h>
#include <GxEPD2_BW.h>
#include <Wire.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "DSEG7_Classic_Bold_53.h"
#include "Display.h"
#include "WatchyRTC.h"
#include "BLE.h"
#include "bma.h"
#include "config.h"

//#include "./apps/_experiments/hello_world.h"

#define WATCHY_DISPLAY GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT>

//Weather Settings
#define CITY_ID "5128581" //New York City https://openweathermap.org/current#cityid
#define OPENWEATHERMAP_APIKEY "f058fe1cad2afe8e2ddc5d063a64cecb" //use your own API key :)
#define OPENWEATHERMAP_URL "http://api.openweathermap.org/data/2.5/weather?id=" //open weather api
#define TEMP_UNIT "metric" //metric = Celsius , imperial = Fahrenheit
#define TEMP_LANG "en"
#define WEATHER_UPDATE_INTERVAL 30 //must be greater than 5, measured in minutes
//NTP Settings
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC 3600 * -5 //New York is UTC -5 EST, -4 EDT, will be overwritten by weather data

RTC_DATA_ATTR int guiState;
RTC_DATA_ATTR int menuIndex;
RTC_DATA_ATTR BMA423 sensor;
RTC_DATA_ATTR bool WIFI_CONFIGURED;
RTC_DATA_ATTR bool BLE_CONFIGURED;
RTC_DATA_ATTR bool displayFullInit       = true;
RTC_DATA_ATTR long gmtOffset = 0;
RTC_DATA_ATTR bool alreadyInMenu         = true;
RTC_DATA_ATTR tmElements_t bootTime;

//OswAppSwitcher mainAppSwitcher(BUTTON_1, LONG_PRESS, true, true, &main_currentAppIndex);
//OswAppSwitcher watchFaceSwitcher(BUTTON_1, SHORT_PRESS, false, false, &main_watchFaceIndex);

void setup () {
    WATCHY_DISPLAY display = WatchyDisplay(DISPLAY_CS, DISPLAY_DC, DISPLAY_RES, DISPLAY_BUSY);
    // Init the display here for all cases, if unused, it will do nothing
    display.epd2.selectSPI(SPI, SPISettings(20000000, MSBFIRST, SPI_MODE0)); // Set SPI to 20Mhz (default is 4Mhz)
    display.init(0, displayFullInit, 10,
                 true); // 10ms by spec, and fast pulldown reset
    //display.epd2.setBusyCallback(displayBusyCallback);
    display.setFullWindow();
    display.fillScreen(GxEPD_WHITE);
    display.setFont(&DSEG7_Classic_Bold_53);
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(5, 53+5);

    display.println("Hello World!");
    display.display(false);
}

void loop () {

}
