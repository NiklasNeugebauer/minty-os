//
// Created by niklas on 7/2/23.
//

#ifndef MINTY_OS_SERVICEMANAGER_H
#define MINTY_OS_SERVICEMANAGER_H

#include "services/TimeService.h"

class ServiceManager {
private:
    static ServiceManager* instance;
public:
    TimeService* time_service;

    static ServiceManager* getInstance();
    static void resetInstance();

    ServiceManager();
    ~ServiceManager();
};


#endif //MINTY_OS_SERVICEMANAGER_H
