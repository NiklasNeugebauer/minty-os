//
// Created by niklas on 8/29/23.
//

#include "services/BluetoothService.h"
#include "SerialLogger.h"

#include NimBLEDevice.h

void BluetoothService::init()
{
    SERIAL_LOG_I("Starting Bluetooth Service");
}

void BluetoothService::update()
{
    // TODO probably unused
}