//
// Created by niklas on 7/1/23.
//

// Calibrated ADC code by charles-haynes
// (https://github.com/charles-haynes/Watchy-Screen/blob/master/src/battery.cpp)

#include "services/BatteryManager.h"

#include "Arduino.h"
#include "hal/pins.h"

#include <driver/adc.h>
#include <esp_adc_cal.h>

RTC_DATA_ATTR float maxVoltage;
RTC_DATA_ATTR float minVoltage;

void BatteryManager::init()
{
    maxVoltage = 4;
    minVoltage = 3.5;
}

void BatteryManager::update()
{
}

esp_adc_cal_characteristics_t *getADCCharacteristics()
{
    static esp_adc_cal_characteristics_t *adc_chars; // default is nullptr
    if (!adc_chars)
    {
        // only initialize it if we're actually going to use it.
        adc_chars = new esp_adc_cal_characteristics_t();
        adc1_config_width(ADC_WIDTH_BIT_12);
        adc1_config_channel_atten(BATT_ADC_CHANNEL, ADC_ATTEN_DB_11);
        esp_adc_cal_value_t cal = esp_adc_cal_characterize(
            ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, adc_chars);
        if (cal == ESP_ADC_CAL_VAL_DEFAULT_VREF)
        {
            log_w("adc calibration is using default vref. Accuracy will suffer.");
        }
    }
    return adc_chars;
}

unsigned BatteryManager::getBatteryLevel()
{
    /// computes battery percentage using ZeroKelvinKeyboards magic LiPo discharge formula
    float voltage = getBatteryVoltage();
    float v = pow(voltage / 3.7, 80);
    int percentage = 123 - (123 / pow(1 + v, 0.165));
    return min(100, max(0, percentage));
}

float BatteryManager::getBatteryVoltage()
{
    // make sure ADC is initialized before we read it.
    esp_adc_cal_characteristics_t *adcChar = getADCCharacteristics();
    // average 4 samples to reduce noise
    int raw =
        (adc1_get_raw(BATT_ADC_CHANNEL) + adc1_get_raw(BATT_ADC_CHANNEL) +
         adc1_get_raw(BATT_ADC_CHANNEL) + adc1_get_raw(BATT_ADC_CHANNEL) +
         2) /
        4;
    return esp_adc_cal_raw_to_voltage(raw, adcChar) * 2.0 / 1000.0;
}