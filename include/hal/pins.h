#ifndef CONFIG_H
#define CONFIG_H

#define MENU_BTN_PIN 26
#define BACK_BTN_PIN 25
#define DOWN_BTN_PIN 4
#define DISPLAY_CS 5
#define DISPLAY_RES 9
#define DISPLAY_DC 10
#define DISPLAY_BUSY 19
#define ACC_INT_1_PIN 14
#define ACC_INT_2_PIN 12
#define VIB_MOTOR_PIN 13
#define RTC_INT_PIN 27

#if defined (ARDUINO_WATCHY_V10)
    #define UP_BTN_PIN 32
    #define BATT_ADC_PIN 33
    #define UP_BTN_MASK  GPIO_SEL_32
    #define RTC_TYPE 1 //DS3231
#elif defined (ARDUINO_WATCHY_V15)
    #define UP_BTN_PIN 32
    #define BATT_ADC_PIN 35
    #define UP_BTN_MASK  GPIO_SEL_32
    #define RTC_TYPE 2 //PCF8563
#elif defined (ARDUINO_WATCHY_V20)
#define UP_BTN_PIN 35
#define BATT_ADC_PIN 34
#define UP_BTN_MASK  GPIO_SEL_35
#define RTC_TYPE 2 //PCF8563
#endif

#define MENU_BTN_MASK GPIO_SEL_26
#define BACK_BTN_MASK GPIO_SEL_25
#define DOWN_BTN_MASK GPIO_SEL_4
#define ACC_INT_MASK  GPIO_SEL_14
#define BTN_PIN_MASK  MENU_BTN_MASK|BACK_BTN_MASK|UP_BTN_MASK|DOWN_BTN_MASK

#endif
