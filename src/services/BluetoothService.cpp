//
// Created by niklas on 8/29/23.
//

#include "services/BluetoothService.h"
#include "SerialLogger.h"

#include <NimBLEDevice.h>

void BluetoothService::init()
{
    SERIAL_LOG_I("Starting Bluetooth Service");
    startBluetooth();
}

void BluetoothService::update()
{
    waitForConnection();
//    if(pServer->getConnectedCount()) {
//        NimBLEService* pSvc = pServer->getServiceByUUID("ABCD");
//        if(pSvc) {
//            NimBLECharacteristic* pChr = pSvc->getCharacteristic("1234");
//            if(pChr) {
//                pChr->notify(true);
//            }
//        }
//    } else {
//        waitForConnection();
//    }
}

void BluetoothService::startBluetooth() {
    NimBLEDevice::init(BLE_DEVICE_NAME);
    SERIAL_LOG_D("BLE device initialized as ", BLE_DEVICE_NAME);
    NimBLEServer *pServer = NimBLEDevice::createServer();
    SERIAL_LOG_D("BLE server created");

    String serviceUUID = "ABCD";
    NimBLEService *pService = pServer->createService(serviceUUID.c_str());
    SERIAL_LOG_D("BLE service created with UUID ", serviceUUID);

    String characteristicUUID = "1234";
    NimBLECharacteristic *pCharacteristic = pService->createCharacteristic(characteristicUUID.c_str());
    SERIAL_LOG_D("BLE characteristic created with UUID ", characteristicUUID);

    pService->start();
    SERIAL_LOG_D("Bluetooth service started");
    pCharacteristic->setValue("Hello BLE");


    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID("ABCD");
    pAdvertising->start();
    SERIAL_LOG_D("Started bluetooth advertising");
}

void BluetoothService::waitForConnection() {
//    if(!pServer->getConnectedCount()) {
        delay(5000);
//    }
}