//
// Created by paul on 27.06.23.
//

#include "utils.h"

uint64_t fruitlang::id() {
    static uint64_t count = 0;
    return count++;
}
