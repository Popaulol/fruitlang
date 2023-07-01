//
// Created by paul on 25.06.23.
//

#include "binary_operations.h"
#include "../utils.h"
uint64_t fruitlang::binary_op::render_dot(std::ofstream &file) {
    auto my_id = id();
    file << "id_" << my_id << " [label=\"" << op() << "\"];\n";

    auto lhs_id = lhs->render_dot(file);
    file << "id_" << my_id << " -> "
         << "id_" << lhs_id << " [label=\"lhs\"];\n";

    auto rhs_id = rhs->render_dot(file);
    file << "id_" << my_id << " -> "
         << "id_" << rhs_id << " [label=\"rhs\"];\n";

    return my_id;
}
llvm::Value *fruitlang::binary_op::codegen() {
    llvm::Value *L = lhs->codegen();
    llvm::Value *R = rhs->codegen();
    if (!L || !R)
        return nullptr;

    return sub_codegen(L, R);
}
std::shared_ptr<fruitlang::Type> fruitlang::binary_op::typecheck(fruitlang::Typechecker &typechecker) {
    auto lhs_type = lhs->typecheck(typechecker);
    auto rhs_type = lhs->typecheck(typechecker);
}
llvm::Value *fruitlang::plus::sub_codegen(llvm::Value *L, llvm::Value *R) {
    return ir_builder->CreateFAdd(L, R, "addtmp");
}
llvm::Value *fruitlang::minus::sub_codegen(llvm::Value *L, llvm::Value *R) {
    return ir_builder->CreateFSub(L, R, "subtmp");
}
llvm::Value *fruitlang::times::sub_codegen(llvm::Value *L, llvm::Value *R) {
    return ir_builder->CreateFMul(L, R, "multmp");
}
llvm::Value *fruitlang::div::sub_codegen(llvm::Value *L, llvm::Value *R) {
    return ir_builder->CreateFDiv(L, R, "divtmp");
}
llvm::Value *fruitlang::power::sub_codegen(llvm::Value *, llvm::Value *) {
    return CodegenError("Exponentiation is not implemented currently.");
}
llvm::Value *fruitlang::mod::sub_codegen(llvm::Value *, llvm::Value *) {
    return CodegenError("Modulus is not implemented currently.");
    ;
}
llvm::Value *fruitlang::comp_not_equal::sub_codegen(llvm::Value *L, llvm::Value *R) {
    L = ir_builder->CreateFCmpUNE(L, R, "neqtmp");
    return ir_builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*llvm_context), "booltmp");
}
llvm::Value *fruitlang::comp_equal::sub_codegen(llvm::Value *L, llvm::Value *R) {
    L = ir_builder->CreateFCmpUEQ(L, R, "eqtmp");
    return ir_builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*llvm_context), "booltmp");
}
llvm::Value *fruitlang::comp_greater::sub_codegen(llvm::Value *L, llvm::Value *R) {
    L = ir_builder->CreateFCmpUGT(L, R, "gttmp");
    return ir_builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*llvm_context), "booltmp");
}
llvm::Value *fruitlang::comp_greater_equal::sub_codegen(llvm::Value *L, llvm::Value *R) {
    L = ir_builder->CreateFCmpUGE(L, R, "gtetmp");
    return ir_builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*llvm_context), "booltmp");
}
llvm::Value *fruitlang::comp_less::sub_codegen(llvm::Value *L, llvm::Value *R) {
    L = ir_builder->CreateFCmpULT(L, R, "lttmp");
    return ir_builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*llvm_context), "booltmp");
}
llvm::Value *fruitlang::comp_less_equal::sub_codegen(llvm::Value *L, llvm::Value *R) {
    L = ir_builder->CreateFCmpULE(L, R, "ltetmp");
    return ir_builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*llvm_context), "booltmp");
}
