//
// Created by paul on 25.06.23.
//

#include "module.h"
#include "../utils.h"

namespace fruitlang {
    uint64_t Module::render_dot(std::ofstream &f) {
        uint64_t own_id = id();
        f << "id_" << own_id << " [label=\"mod:  " << name << "\"];\n";
        for (const auto& def : contents) {
            auto def_id = def->render_dot(f);
            f << "id_" << own_id << " -> id_"<< def_id << ";\n";
        }
        return own_id;
    }
    llvm::Function *Module::TopLVLCodegen() {
        for (const auto& member : contents) member->TopLVLCodegen();
        return nullptr;
    }
}// namespace fruitlang