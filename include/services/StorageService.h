//
// Created by niklas on 8/28/23.
//

#ifndef MINTY_OS_STORAGESERVICE_H
#define MINTY_OS_STORAGESERVICE_H

#include "Service.h"

class StorageService : Service{
public:
    static void init();
    static void update();

    static void saveStepData(unsigned steps);
    static unsigned loadStepData();
};


#endif //MINTY_OS_STORAGESERVICE_H
