//
// Created by paul on 25.06.23.
//

#include "callable.h"
#include "../utils.h"
#include <llvm/IR/Verifier.h>

namespace fruitlang {
    uint64_t callable::render_dot(std::ofstream &f) {
        uint64_t own_id = id();
        f << "id_" << own_id << " [label=\"" << kind() << "(";
        for (const auto &arg: m_arguments) {
            f << arg.name << ": " << arg.typ.name << ", ";
        }
        f << ") -> " << m_return_type.name << "\"];\n";
        auto other_id = m_content->render_dot(f);
        f << "id_" << own_id << " -> id_" << other_id << ";\n";
        return own_id;
    }
    llvm::Function *callable::TopLVLCodegen() {

        if(!m_content) return prototype();

        llvm::Function *func = TheModule->getFunction(name);

        if(!func) func = prototype();

        if(!func) return nullptr;

        if (!func->empty()) return (llvm::Function*) CodegenError("Function `" + name + "` cannot be redefined!");

        llvm::BasicBlock *basic_block = llvm::BasicBlock::Create(*TheContext, "entry", func);
        Builder->SetInsertPoint(basic_block);

        NamedValues.clear();
        for (auto &arg : func->args())
            NamedValues[arg.getName().str()] = &arg;

        if (llvm::Value *value = m_content->codegen()) {
            // Finish off the function.
            Builder->CreateRet(value);

            // Validate the generated code, checking for consistency.
            llvm::verifyFunction(*func);

            return func;
        }

        func->eraseFromParent();
        return nullptr;

    }
    llvm::Function *callable::prototype() {
        std::vector<llvm::Type *> Doubles(m_arguments.size(), llvm::Type::getDoubleTy(*TheContext));
        llvm::FunctionType *FT =
                llvm::FunctionType::get(llvm::Type::getDoubleTy(*TheContext), Doubles, false);
        llvm::Function *F =
                llvm::Function::Create(FT, llvm::Function::ExternalLinkage, name, TheModule.get());
        uint64_t Idx = 0;
        for (auto &arg : F->args())
            arg.setName(m_arguments[Idx++].name);

        return F;
    }
}// namespace fruitlang