//
// Created by paul on 25.07.23.
//

#include "ast_if.h"

#include "../utils.h"
#include "../Typechecker.h"
#include "Literal.h"

namespace fruitlang {
    std::string ast_if::render_dot(std::ofstream &f) {
        auto own_id = id();

        f << "subgraph cluster_" << id() << " {\n";
        f <<  own_id << " [label=\"if\"]\n;";

        auto condition_id = condition->render_dot(f);
        auto body_id = body->render_dot(f);

        f << own_id << " -> " << condition_id << " [label=\"condition\"];\n";
        f << own_id << " -> " << body_id << " [label=\"body\"];\n";

        if (else_block) {
            auto else_id = else_block->render_dot(f);
            f << own_id << " -> " << else_id << " [label=\"else\"];\n";
        }
        f << "}\n";
        return own_id;
    }
    llvm::Value *ast_if::codegen(Typechecker &typechecker) {
        llvm::Value* condition_value = condition->codegen(typechecker);
        llvm::Function* current_function = ir_builder->GetInsertBlock()->getParent();

        llvm::BasicBlock* body_basic_block = llvm::BasicBlock::Create(*llvm_context, "body", current_function);
        llvm::BasicBlock* else_basic_block = llvm::BasicBlock::Create(*llvm_context, "else");
        llvm::BasicBlock* merge_basic_block = llvm::BasicBlock::Create(*llvm_context, "merge");
        ir_builder->CreateCondBr(condition_value, body_basic_block, else_basic_block);

        ir_builder->SetInsertPoint(body_basic_block);
        llvm::Value* body_value = body->codegen(typechecker);
        if (!body_value) return nullptr;
        ir_builder->CreateBr(merge_basic_block);
        body_basic_block = ir_builder->GetInsertBlock();

        current_function->getBasicBlockList().push_back(else_basic_block);
        ir_builder->SetInsertPoint(else_basic_block);
        llvm::Value* else_value = else_block->codegen(typechecker);
        if (!else_value) return nullptr;
        ir_builder->CreateBr(merge_basic_block);
        else_basic_block = ir_builder->GetInsertBlock();

        current_function->getBasicBlockList().push_back(merge_basic_block);
        ir_builder->SetInsertPoint(merge_basic_block);
        llvm::PHINode* phi_node = ir_builder->CreatePHI(get_type(typechecker)->to_llvm_type(), 2, "iftmp");
        phi_node->addIncoming(body_value, body_basic_block);
        phi_node->addIncoming(else_value, else_basic_block);
        return phi_node;
    }
    Type ast_if::typecheck(Typechecker &typechecker) {
        auto condition_type = condition->get_type(typechecker, "cond ");
        auto body_type = body->get_type(typechecker, "body ");

        if (condition_type != typechecker.get_type("bool")) typechecker.Error("if condition needs to be a boolean value.");

        if (else_block) {
            auto else_type = else_block->get_type(typechecker, "else ");
            if (body_type == else_type) return body_type;
            auto new_else_block = typechecker.promote_type_to(else_block, body_type);
            if (new_else_block) {
                else_block = new_else_block;
                return body_type;
            }
            auto new_body = typechecker.promote_type_to(body, else_type);
            if (new_body) {
                body = new_body;
                return else_type;
            }
            return typechecker.Error("Types of else block and body need to match or one needs to be ");
        }
        else {
            else_block = typechecker.promote_type_to(std::make_shared<Int_Literal>(0), body_type);
        }
        return body_type;
    }
}// namespace fruitlang