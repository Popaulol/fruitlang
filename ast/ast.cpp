//
// Created by paul on 25.06.23.
//

#include "ast.h"

namespace fruitlang {
    ast::~ast() = default;
    void ast::render_dot(const std::string &name) {
        std::ofstream file;
        file.open(name);
        file << "digraph ast { \n";
        render_dot(file);
        file << "}\n";
        file.close();
    }
}// namespace fruitlang