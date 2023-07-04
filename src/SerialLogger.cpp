// Taken and adapted from OswLogger.cpp which is part of Open-Smartwatch-OS
// Link: https://github.com/Open-Smartwatch/open-smartwatch-os/blob/master/.github/include/OswLogger.cpp

#include <SerialLogger.h>

std::unique_ptr<SerialLogger> SerialLogger::instance = nullptr;