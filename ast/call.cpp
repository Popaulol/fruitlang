//
// Created by paul on 26.06.23.
//

#include "call.h"
#include "../codegen.h"
#include "../utils.h"

namespace fruitlang {
    uint64_t call::render_dot(std::ofstream &f) {
        uint64_t own_id = id();
        f << "id_" << own_id << " [label=\"Call: " << ident << "\"];\n";
        for (const auto &arg: args) {
            auto arg_id = arg->render_dot(f);
            f << "id_" << own_id << " -> id_" << arg_id << " [lebel=\"Argument\"];\n";
        }
        return own_id;
    }
    llvm::Value *call::codegen() {
        llvm::Function *CalleeF = ir_module->getFunction(ident);
        if (!CalleeF)
            return CodegenError("Unknown function referenced");

        // If argument mismatch error.
        if (CalleeF->arg_size() != args.size())
            return CodegenError("Incorrect # arguments passed");

        std::vector<llvm::Value *> codegenned_args;
        for (auto &arg: args) {
            codegenned_args.push_back(arg->codegen());
            if (!codegenned_args.back())
                return nullptr;
        }

        return ir_builder->CreateCall(CalleeF, codegenned_args, "calltmp");
    }
}// namespace fruitlang