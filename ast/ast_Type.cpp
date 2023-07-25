//
// Created by paul on 25.06.23.
//

#include "ast_Type.h"

#include "../codegen.h"
#include "../utils.h"

namespace fruitlang {
    std::string ast_Type::render_dot(std::ofstream &f) {
        auto my_id = id();
        f << my_id << " [label=\"ast_Type: `" << name << "`\"];\n";
        return my_id;
    }
}// namespace fruitlang