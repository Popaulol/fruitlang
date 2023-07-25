//
// Created by paul on 27.06.23.
//

#include "utils.h"
#include <unordered_map>

std::string fruitlang::id(const std::string& name) {
    static uint64_t count = 0;
    static std::unordered_map<std::string, std::string> ids{};
    if (!name.empty()) {
        if (ids.contains(name)) return ids[name];
        ids[name] = "id_" + std::to_string(count);
    }
    return "id_" + std::to_string(count++);
}
