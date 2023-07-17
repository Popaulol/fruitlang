//
// Created by paul on 26.06.23.
//

#include "call.h"
#include "../Typechecker.h"
#include "../utils.h"

namespace fruitlang {
    uint64_t call::render_dot(std::ofstream &f) {
        uint64_t own_id = id();
        f << "id_" << own_id << " [label=\"Call: " << ident << "\\n"
          << "Type: " << type->name << "\"];\n";
        for (const auto &arg: args) {
            auto arg_id = arg->render_dot(f);
            f << "id_" << own_id << " -> id_" << arg_id << " [lebel=\"Argument\"];\n";
        }
        return own_id;
    }
    llvm::Value *call::codegen(fruitlang::Typechecker &typechecker) {
        llvm::Function *CalleeF = ir_module->getFunction(ident);
        if (!CalleeF)
            return CodegenError("Unknown function referenced");

        // If argument mismatch error.
        if (CalleeF->arg_size() != args.size())
            return CodegenError("Incorrect # arguments passed");

        std::vector<llvm::Value *> codegenned_args;
        for (auto &arg: args) {
            codegenned_args.push_back(arg->codegen(typechecker));
            if (!codegenned_args.back())
                return nullptr;
        }

        return ir_builder->CreateCall(CalleeF, codegenned_args, "calltmp");
    }
    fruitlang::Type call::typecheck(Typechecker &typechecker) {
        return typechecker.get_variable(ident)->check_call(typechecker, *this);
    }
}// namespace fruitlang