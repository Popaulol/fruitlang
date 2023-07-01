//
// Created by paul on 26.06.23.
//

#include "unary_operations.h"
#include "../codegen.h"
#include "../utils.h"
uint64_t fruitlang::unary_operation::render_dot(std::ofstream &file) {
    auto my_id = id();
    file << "id_" << my_id << " [label=\"" << op() << "\"];\n";

    auto operand_id = operand->render_dot(file);
    file << "id_" << my_id << " -> "
         << "id_" << operand_id << " [label=\"lhs\"];\n";

    return my_id;
}
llvm::Value *fruitlang::unary_operation::codegen() {
    llvm::Value *op = operand->codegen();
    if (!op) return nullptr;
    return sub_codegen(op);
}
llvm::Value *fruitlang::negate::sub_codegen(llvm::Value *op) {
    return ir_builder->CreateFNeg(op, "negtmp");
}
llvm::Value *fruitlang::invert::sub_codegen(llvm::Value *op) {
    op = ir_builder->CreateFCmpUEQ(op, llvm::ConstantFP::get(*llvm_context, llvm::APFloat(0.0f)), "invetmp");
    return ir_builder->CreateUIToFP(op, llvm::Type::getDoubleTy(*llvm_context), "booltmp");
}
