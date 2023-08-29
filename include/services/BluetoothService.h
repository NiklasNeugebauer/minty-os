//
// Created by niklas on 8/29/23.
//

#ifndef MINTY_OS_BLUETOOTHSERVICE_H
#define MINTY_OS_BLUETOOTHSERVICE_H

#include "Service.h"
#include "hal/hal.h"

class BluetoothService : Service
{
private:
public:
    static void init();
    static void update();
};

#endif // MINTY_OS_WIFISERVICE_H