//
// Created by paul on 25.06.23.
//

#include "block.h"
#include "../utils.h"

namespace fruitlang {
    std::string block::render_dot(std::ofstream &f) {
        auto own_id = id();

        f  << own_id << "[label=\"Block\"];\n";
        for (const auto &expr: contents) {
            auto expr_id = expr->render_dot(f);
            f << own_id << " -> " << expr_id << ";\n";
        }

        return own_id;
    }
    llvm::Value *block::codegen(Typechecker &typechecker) {
        llvm::Value *ret;
        for (const auto &expr: contents) {
            ret = expr->codegen(typechecker);
        }
        return ret;
    }
    Type block::typecheck(Typechecker &typechecker) {
        Type return_type;
        for (const auto &expr: contents) {
            return_type = expr->get_type(typechecker);
        }
        return return_type;
    }
}// namespace fruitlang