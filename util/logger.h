//
// Created by simi on 1/22/22.
//

#ifndef LANGRON_LOGGER_H
#define LANGRON_LOGGER_H

#include <iostream>

template <typename T>
T logError(const char* str) {
    std::cerr << str << '\n';
    return nullptr;
}

#endif //LANGRON_LOGGER_H
