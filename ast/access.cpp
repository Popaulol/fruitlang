//
// Created by paul on 28.06.23.
//

#include "access.h"

#include "../Typechecker.h"
#include "../utils.h"

namespace fruitlang {
    std::string access::render_dot(std::ofstream &f) {
        std::string t_name = "NULL";
        if (type) {
            t_name = type->name;
        }
        auto own_id = id();
        f << own_id << " [label=\"Access: " << name << "\\n"
          << "Type: " << t_name << "\"];\n";
        return own_id;
    }
    llvm::Value *access::codegen(fruitlang::Typechecker &) {
        llvm::Value *V = variables[name];
        if (!V) CodegenError("Unknown variable name");
        return V;
    }
    fruitlang::Type access::typecheck(Typechecker &typechecker) {
        if (typechecker.check_global(name)) typechecker.had_side_effects = true;
        return typechecker.get_variable(name);
    }
}// namespace fruitlang