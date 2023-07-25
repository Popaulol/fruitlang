//
// Created by paul on 25.06.23.
//

#include "binary_operations.h"

#include "../Typechecker.h"
#include "../utils.h"

std::string fruitlang::binary_op::render_dot(std::ofstream &file) {
    auto my_id = id();
    std::string t_name = "NULL";
    if (type) {
        t_name = type->name;
    }
    file << "subgraph cluster_" << id() << " {\n";
    file << my_id << " [label=\"" << op() << "\\n"
         << "Type: " << t_name << "\"];\n";

    auto lhs_id = lhs->render_dot(file);
    file  << my_id << " -> "
         << lhs_id << " [label=\"lhs\"];\n";

    auto rhs_id = rhs->render_dot(file);
    file << my_id << " -> "
         << rhs_id << " [label=\"rhs\"];\n";

    file << "}\n";

    return my_id;
}
llvm::Value *fruitlang::binary_op::codegen(fruitlang::Typechecker &typechecker) {
    llvm::Value *L = lhs->codegen(typechecker);
    llvm::Value *R = rhs->codegen(typechecker);
    if (!L || !R)
        return nullptr;

    return sub_codegen(L, R, typechecker);
}
fruitlang::Type fruitlang::binary_op::typecheck(fruitlang::Typechecker &typechecker) {
    auto lhs_type = lhs->get_type(typechecker);
    auto rhs_type = rhs->get_type(typechecker);
    if (lhs_type == rhs_type) return lhs_type;


    auto new_lhs = typechecker.promote_type_to(lhs, rhs_type);
    if (new_lhs) {
        lhs = new_lhs;
        return validate_and_fix_type(rhs_type, typechecker);
    }

    auto new_rhs = typechecker.promote_type_to(rhs, lhs_type);
    if (new_rhs) {
        rhs = new_rhs;
        return validate_and_fix_type(lhs_type, typechecker);
    }

    return typechecker.Error("Complex Promotions aren't implemented at this point.");
}
llvm::Value *fruitlang::plus::sub_codegen(llvm::Value *L, llvm::Value *R, fruitlang::Typechecker &typechecker) {
    if (typechecker.is_float(get_type(typechecker)))
        return ir_builder->CreateFAdd(L, R, "faddtmp");
    if (typechecker.is_int(get_type(typechecker)))
        return ir_builder->CreateAdd(L, R, "addtmp");

    return CodegenError("Add of invalid types");
}
llvm::Value *fruitlang::minus::sub_codegen(llvm::Value *L, llvm::Value *R, fruitlang::Typechecker &typechecker) {
    if (typechecker.is_float(get_type(typechecker)))
        return ir_builder->CreateFSub(L, R, "fsubtmp");
    if (typechecker.is_int(get_type(typechecker)))
        return ir_builder->CreateSub(L, R, "subtmp");

    return CodegenError("Sub of invalid types");
}
llvm::Value *fruitlang::times::sub_codegen(llvm::Value *L, llvm::Value *R, fruitlang::Typechecker &typechecker) {
    if (typechecker.is_float(get_type(typechecker)))
        return ir_builder->CreateFMul(L, R, "fmultmp");
    if (typechecker.is_int(get_type(typechecker)))
        return ir_builder->CreateMul(L, R, "multmp");

    return CodegenError("Mul of invalid types");
}
llvm::Value *fruitlang::div::sub_codegen(llvm::Value *L, llvm::Value *R, fruitlang::Typechecker &typechecker) {
    if (typechecker.is_float(get_type(typechecker)))
        return ir_builder->CreateFDiv(L, R, "fdivtmp");
    if (typechecker.is_int(get_type(typechecker)))
        return ir_builder->CreateSDiv(L, R, "divtmp");

    return CodegenError("Div of invalid types");
}
llvm::Value *fruitlang::power::sub_codegen(llvm::Value *, llvm::Value *, fruitlang::Typechecker &) {
    return CodegenError("Exponentiation is not implemented currently.");
}
llvm::Value *fruitlang::mod::sub_codegen(llvm::Value *L, llvm::Value *R, fruitlang::Typechecker &typechecker) {
    if (typechecker.is_float(get_type(typechecker)))
        return ir_builder->CreateFRem(L, R, "fmodtemp");
    if (typechecker.is_int(get_type(typechecker)))
        return ir_builder->CreateSRem(L, R, "modtemp");
    return CodegenError("Mod of invalid types");
}
llvm::Value *fruitlang::comp_not_equal::sub_codegen(llvm::Value *L, llvm::Value *R, fruitlang::Typechecker &typechecker) {
    if (typechecker.is_float(get_type(typechecker)))
        return ir_builder->CreateFCmpUNE(L, R, "fneqtmp");
    if (typechecker.is_int(get_type(typechecker)))
        return ir_builder->CreateICmpNE(L, R, "neqtmp");
    return CodegenError("Comparison of Invalid Types");
}
fruitlang::Type fruitlang::comp_not_equal::validate_and_fix_type([[maybe_unused]] fruitlang::Type typ, fruitlang::Typechecker &typechecker) {
    return typechecker.get_type("bool");
}
llvm::Value *fruitlang::comp_equal::sub_codegen(llvm::Value *L, llvm::Value *R, fruitlang::Typechecker &typechecker) {
    if (typechecker.is_float(get_type(typechecker)))
        return ir_builder->CreateFCmpUEQ(L, R, "feqtmp");
    if (typechecker.is_int(get_type(typechecker)))
        return ir_builder->CreateICmpEQ(L, R, "eqtmp");
    return CodegenError("Comparison of Invalid Types");
}
fruitlang::Type fruitlang::comp_equal::validate_and_fix_type([[maybe_unused]] fruitlang::Type typ, fruitlang::Typechecker &typechecker) {
    return typechecker.get_type("bool");
}
llvm::Value *fruitlang::comp_greater::sub_codegen(llvm::Value *L, llvm::Value *R, fruitlang::Typechecker &typechecker) {
    if (typechecker.is_float(get_type(typechecker)))
        return ir_builder->CreateFCmpUGT(L, R, "fgttmp");
    if (typechecker.is_int(get_type(typechecker)))
        return ir_builder->CreateICmpSGT(L, R, "gttmp");
    return CodegenError("Comparison of Invalid Types");
}
fruitlang::Type fruitlang::comp_greater::validate_and_fix_type([[maybe_unused]] fruitlang::Type typ, fruitlang::Typechecker &typechecker) {
    return typechecker.get_type("bool");
}
llvm::Value *fruitlang::comp_greater_equal::sub_codegen(llvm::Value *L, llvm::Value *R, fruitlang::Typechecker &typechecker) {
    if (typechecker.is_float(get_type(typechecker)))
        return ir_builder->CreateFCmpUGE(L, R, "fgetmp");
    if (typechecker.is_int(get_type(typechecker)))
        return ir_builder->CreateICmpSGE(L, R, "getmp");
    return CodegenError("Comparison of Invalid Types");
}
fruitlang::Type fruitlang::comp_greater_equal::validate_and_fix_type([[maybe_unused]] fruitlang::Type typ, fruitlang::Typechecker &typechecker) {
    return typechecker.get_type("bool");
}
llvm::Value *fruitlang::comp_less::sub_codegen(llvm::Value *L, llvm::Value *R, fruitlang::Typechecker &typechecker) {
    if (typechecker.is_float(get_type(typechecker)))
        return ir_builder->CreateFCmpULT(L, R, "flttmp");
    if (typechecker.is_int(get_type(typechecker)))
        return ir_builder->CreateICmpSLT(L, R, "lttmp");
    return CodegenError("Comparison of Invalid Types");
}
fruitlang::Type fruitlang::comp_less::validate_and_fix_type([[maybe_unused]] fruitlang::Type typ, fruitlang::Typechecker &typechecker) {
    return typechecker.get_type("bool");
}
llvm::Value *fruitlang::comp_less_equal::sub_codegen(llvm::Value *L, llvm::Value *R, fruitlang::Typechecker &typechecker) {
    if (typechecker.is_float(get_type(typechecker)))
        return ir_builder->CreateFCmpULE(L, R, "fletmp");
    if (typechecker.is_int(get_type(typechecker)))
        return ir_builder->CreateICmpSLE(L, R, "letmp");
    return CodegenError("Comparison of Invalid Types");
}
fruitlang::Type fruitlang::comp_less_equal::validate_and_fix_type([[maybe_unused]] fruitlang::Type typ, fruitlang::Typechecker &typechecker) {
    return typechecker.get_type("bool");
}
