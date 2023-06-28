//
// Created by paul on 25.06.23.
//

#include "binary_operations.h"
#include "../utils.h"
uint64_t fruitlang::binary_op::render_dot(std::ofstream &file) {
    auto my_id = id();
    file << "id_" << my_id << " [label=\"" << op() << "\"];\n";

    auto lhs_id = lhs->render_dot(file);
    file << "id_" << my_id << " -> " << "id_" << lhs_id << " [label=\"lhs\"];\n";

    auto rhs_id = rhs->render_dot(file);
    file << "id_" << my_id << " -> " << "id_" << rhs_id << " [label=\"rhs\"];\n";

    return my_id;
}
llvm::Value *fruitlang::binary_op::codegen() {
    llvm::Value *L = lhs->codegen();
    llvm::Value *R = rhs->codegen();
    if (!L || !R)
        return nullptr;

    return sub_codegen(L, R);
}
llvm::Value *fruitlang::plus::sub_codegen(llvm::Value *L, llvm::Value *R) {
    return Builder->CreateFAdd(L, R, "addtmp");
}
llvm::Value *fruitlang::minus::sub_codegen(llvm::Value *L, llvm::Value *R) {
    return Builder->CreateFSub(L, R, "subtmp");
}
llvm::Value *fruitlang::times::sub_codegen(llvm::Value *L, llvm::Value *R) {
    return Builder->CreateFMul(L, R, "multmp");
}
llvm::Value *fruitlang::div::sub_codegen(llvm::Value *L, llvm::Value *R) {
    return Builder->CreateFDiv(L, R, "divtmp");
}
llvm::Value *fruitlang::power::sub_codegen(llvm::Value *L, llvm::Value *R) {
    return CodegenError("Exponentiation is not implemented currently.");
}
llvm::Value *fruitlang::mod::sub_codegen(llvm::Value *L, llvm::Value *R) {
    return CodegenError("Modulus is not implemented currently.");;
}
llvm::Value *fruitlang::comp_not_equal::sub_codegen(llvm::Value *L, llvm::Value *R) {
    L = Builder->CreateFCmpUNE(L, R, "neqtmp");
    return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext), "booltmp");
}
llvm::Value *fruitlang::comp_equal::sub_codegen(llvm::Value *L, llvm::Value *R) {
    L = Builder->CreateFCmpUEQ(L, R, "eqtmp");
    return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext), "booltmp");
}
llvm::Value *fruitlang::comp_greater::sub_codegen(llvm::Value *L, llvm::Value *R) {
    L = Builder->CreateFCmpUGT(L, R, "gttmp");
    return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext), "booltmp");
}
llvm::Value *fruitlang::comp_greater_equal::sub_codegen(llvm::Value *L, llvm::Value *R) {
    L = Builder->CreateFCmpUGE(L, R, "gtetmp");
    return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext), "booltmp");
}
llvm::Value *fruitlang::comp_less::sub_codegen(llvm::Value *L, llvm::Value *R) {
    L = Builder->CreateFCmpULT(L, R, "lttmp");
    return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext), "booltmp");
}
llvm::Value *fruitlang::comp_less_equal::sub_codegen(llvm::Value *L, llvm::Value *R) {
    L = Builder->CreateFCmpULE(L, R, "ltetmp");
    return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext), "booltmp");
}
