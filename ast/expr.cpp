//
// Created by paul on 25.06.23.
//
#include "expr.h"

#include "../Typechecker.h"
#include "../codegen.h"
#include "../utils.h"

namespace fruitlang {
    fruitlang::Type expr::get_type(Typechecker &typechecker) {
        std::cerr << "got type\n";
        if (type) return type;
        std::cerr << "uncached\n";
        type = typecheck(typechecker);
        return type;
    }
    llvm::Value *inserted_cast::codegen(Typechecker &typechecker) {
        auto child_type = child->get_type(typechecker);
        auto child_value = child->codegen(typechecker);
        auto own_type = get_type(typechecker);
        if (typechecker.is_float(child_type)) {
            if (typechecker.is_float(own_type)) return ir_builder->CreateFPExt(child_value, own_type->to_llvm_type());
            CodegenError("Tried to cast float to non float!");
        } else if (typechecker.is_int(child_type)) {
            if (typechecker.is_float(own_type)) {
                return ir_builder->CreateSIToFP(child_value, own_type->to_llvm_type());
            } else if (typechecker.is_int(own_type)) {
                return ir_builder->CreateSExt(child_value, own_type->to_llvm_type());
            }
        }
        CodegenError("Invalid inserted Cast");
        return child_value;
    }
    fruitlang::Type inserted_cast::typecheck(Typechecker &typechecker) {
        return typechecker.Error("An inserted cast should never be Typechecked, since it's always generated in a valid state by the typechecker itself!");
    }
    std::string inserted_cast::render_dot(std::ofstream &file) {
        auto my_id = id();
        file << my_id << " [label=\"Inserted Cast"
             << "\\n"
             << "Type: " << type->name << "\"];\n";
        auto child_id = child->render_dot(file);
        file << my_id << " -> " << child_id << ";\n";
        return my_id;
    }


}// namespace fruitlang