//
// Created by niklas on 7/2/23.
//

#include "ServiceManager.h"

ServiceManager* ServiceManager::instance = nullptr;

ServiceManager* ServiceManager::getInstance() {
    if(ServiceManager::instance == nullptr) {
        ServiceManager::instance = new ServiceManager();
    }
    return ServiceManager::instance;
}

void ServiceManager::resetInstance() {
    delete ServiceManager::instance;
    ServiceManager::instance = nullptr; // On the next access it will be recreated!
}

ServiceManager::ServiceManager() {
    time_service = new TimeService();
}

ServiceManager::~ServiceManager() {
    delete time_service;
}