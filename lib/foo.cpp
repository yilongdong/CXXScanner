#include "utility/log.h"
#include <iostream>

void Hello() {
    std::cout << "Hello, World!" << std::endl;
    SET_LOGLEVEL("debug");
    LOG_INFO("info");
    LOG_DEBUG("debug");
    LOG_TRACE("trace");
    SET_LOGLEVEL("sfsakjfg");
    LOG_INFO("info after unkown log level");
}