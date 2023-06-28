//
// Created by paul on 25.06.23.
//

#include "Type.h"
#include "../codegen.h"
#include "../utils.h"

namespace fruitlang {
    uint64_t Type::render_dot(std::ofstream &f) {
        uint64_t my_id = id();
        f << "id_" << my_id << " [label=\"Type: `" << name << "`\"];\n";
        return my_id;
    }
}// namespace fruitlang