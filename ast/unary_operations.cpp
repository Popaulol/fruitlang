//
// Created by paul on 26.06.23.
//

#include "unary_operations.h"

#include "../Typechecker.h"
#include "../codegen.h"
#include "../utils.h"

std::string fruitlang::unary_operation::render_dot(std::ofstream &file) {
    auto my_id = id();
    file << my_id << " [label=\"" << op() << "\"];\n";

    auto operand_id = operand->render_dot(file);
    file << my_id << " -> "
         << operand_id << " [label=\"lhs\"];\n";

    return my_id;
}
llvm::Value *fruitlang::unary_operation::codegen(fruitlang::Typechecker &typechecker) {
    llvm::Value *op = operand->codegen(typechecker);
    if (!op) return nullptr;
    return sub_codegen(op, typechecker);
}

llvm::Value *fruitlang::negate::sub_codegen(llvm::Value *op, Typechecker &typechecker) {
    if (typechecker.is_float(get_type(typechecker)))
        return ir_builder->CreateFNeg(op, "fnegtmp");
    else if (typechecker.is_int(get_type(typechecker)))
        return ir_builder->CreateNeg(op, "negtmp");

    return CodegenError("Only ints and Floats may be negated!");
}
fruitlang::Type fruitlang::negate::typecheck(fruitlang::Typechecker &typechecker) {
    auto op_typ = operand->get_type(typechecker);
    if (typechecker.is_int(op_typ) || typechecker.is_float(op_typ))
        return operand->get_type(typechecker);

    return typechecker.Error("Only ints and floats may be negated!");
}
llvm::Value *fruitlang::invert::sub_codegen(llvm::Value *op, [[maybe_unused]] Typechecker &typechecker) {
    return ir_builder->CreateICmpEQ(op, llvm::ConstantInt::get(*llvm_context, llvm::APInt(1, 0.0)), "invetmp");
}
fruitlang::Type fruitlang::invert::typecheck(fruitlang::Typechecker &typechecker) {
    auto op_typ = operand->get_type(typechecker);
    if (op_typ != typechecker.get_type("bool"))
        return typechecker.Error("Only booleans may be inverted!");
    return op_typ;
}
