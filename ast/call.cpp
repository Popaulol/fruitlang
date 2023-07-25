//
// Created by paul on 26.06.23.
//

#include "call.h"
#include "../Typechecker.h"
#include "../utils.h"

namespace fruitlang {
    std::string call::render_dot(std::ofstream &f) {
        auto own_id = id();
        std::string t_name = "NULL";
        if (type) {
            t_name = type->name;
        }
        f << own_id << " [label=\"Call: " << ident << "\\n"
          << "Type: " << t_name << "\"];\n";

        auto ident_id = id(ident);

        f << own_id << " -> " << ident_id << " [constraint=false, style=dotted];\n";

        for (const auto &arg: args) {
            auto arg_id = arg->render_dot(f);
            f  << own_id << " -> " << arg_id << " [label=\"Argument\"];\n";
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