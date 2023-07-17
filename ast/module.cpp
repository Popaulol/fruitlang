//
// Created by paul on 25.06.23.
//

#include "module.h"

#include "../utils.h"

namespace fruitlang {
    uint64_t Module::render_dot(std::ofstream &f) {
        uint64_t own_id = id();
        f << "id_" << own_id << " [label=\"mod:  " << name << "\"];\n";
        for (const auto &def: contents) {
            auto def_id = def->render_dot(f);
            f << "id_" << own_id << " -> id_" << def_id << ";\n";
        }
        return own_id;
    }
    llvm::Function *Module::TopLVLCodegen([[maybe_unused]] Typechecker &typechecker) {
        typechecker.push_scope();
        for (const auto &member: contents) member->TopLVLCodegen(typechecker);
        typechecker.pop_scope();
        return nullptr;
    }
    std::vector<Type> Module::get_types(Typechecker &typechecker) {
        std::vector<Type> typs;
        typechecker.push_scope();
        for (const auto &member: contents)
            for (const auto &typ: member->get_types(typechecker))
                typs.push_back(typ);

        typechecker.pop_scope();
        return typs;
    }
}// namespace fruitlang