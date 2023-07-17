//
// Created by paul on 25.06.23.
//

#include "callable.h"

#include "../Typechecker.h"
#include "../utils.h"

namespace fruitlang {
    uint64_t callable::render_dot(std::ofstream &f) {
        uint64_t own_id = id();
        f << "id_" << own_id << " [label=\"" << kind() << "(";
        for (const auto &arg: m_arguments) {
            f << arg.name << ": " << arg.typ.name << ", ";
        }
        f << ") -> " << m_return_type.name << "\"];\n";
        if (!m_content) return own_id;
        auto other_id = m_content->render_dot(f);
        f << "id_" << own_id << " -> id_" << other_id << ";\n";
        return own_id;
    }
    llvm::Function *callable::TopLVLCodegen([[maybe_unused]] Typechecker &typechecker) {

        if (!m_content) return prototype(typechecker);

        llvm::Function *func = ir_module->getFunction(name);

        if (!func) func = prototype(typechecker);

        if (!func) return nullptr;

        if (!func->empty()) return (llvm::Function *) CodegenError(kind() + "` cannot be redefined!");

        llvm::BasicBlock *basic_block = llvm::BasicBlock::Create(*llvm_context, "entry", func);
        ir_builder->SetInsertPoint(basic_block);

        variables.clear();
        typechecker.push_scope();
        uint64_t i = 0;
        for (auto &arg: func->args()) {
            variables[arg.getName().str()] = &arg;
            typechecker.add_variable(arg.getName().str(), typechecker.get_type(m_arguments[i++].typ.name));
        }
        auto return_type = typechecker.get_type(m_return_type.name);
        if (m_content->get_type(typechecker) != return_type) {
            m_content = typechecker.promote_type_to(m_content, return_type);
        }

        if (llvm::Value *value = m_content->codegen(typechecker)) {
            typechecker.pop_scope();
            // Finish off the function.
            ir_builder->CreateRet(value);

            // Validate the generated code, checking for consistency.
            llvm::verifyFunction(*func);

            function_pass_manager->run(*func);

            return func;
        }

        func->eraseFromParent();
        typechecker.pop_scope();
        return nullptr;
    }
    llvm::Function *callable::prototype(Typechecker &typechecker) {
        std::vector<llvm::Type *> argument_llvm_types;
        std::vector<fruitlang::Type> args;
        fruitlang::Type return_type = typechecker.get_type(m_return_type.name);
        for (const auto &arg: m_arguments) {
            auto typ = typechecker.get_type(arg.typ.name);
            args.push_back(typ);
            argument_llvm_types.push_back(typ->to_llvm_type());
        }

        typechecker.add_function(name, args, return_type);

        llvm::FunctionType *FT =
                llvm::FunctionType::get(return_type->to_llvm_type(), argument_llvm_types, false);
        llvm::Function *F =
                llvm::Function::Create(FT, llvm::Function::ExternalLinkage, name, ir_module.get());

        uint64_t Idx = 0;
        for (auto &arg: F->args()) {
            arg.setName(m_arguments[Idx++].name);
        }
        return F;
    }
    std::vector<Type> callable::get_types(Typechecker &typechecker) {
        std::vector<fruitlang::Type> args;
        fruitlang::Type return_type = typechecker.get_type(m_return_type.name);

        for (const auto &arg: m_arguments)
            args.push_back(typechecker.get_type(arg.typ.name));

        typechecker.add_function(name, args, return_type);

        if (!m_content) return {};

        typechecker.push_scope();

        for (const auto &arg: m_arguments) typechecker.add_variable(arg.name, typechecker.get_type(arg.typ.name));

        auto t = m_content->get_type(typechecker);

        if (m_content->get_type(typechecker) != return_type) {
            m_content = typechecker.promote_type_to(m_content, return_type);
        }

        typechecker.pop_scope();
        return {t};
    }
}// namespace fruitlang