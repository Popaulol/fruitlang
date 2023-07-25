//
// Created by paul on 26.06.23.
//

#include "Literal.h"

#include "../Typechecker.h"
#include "../utils.h"

std::string fruitlang::Float_Literal::render_dot(std::ofstream &file) {
    auto my_id = id();
    std::string t_name = "NULL";
    if (type) {
        t_name = type->name;
    }
    file << my_id << " [label=\"" << value << "f"
         << "\\n"
         << "Type: " << t_name << "\"];\n";
    return my_id;
}
llvm::Value *fruitlang::Float_Literal::codegen(fruitlang::Typechecker &) {
    return ir_builder->CreateFPTrunc(llvm::ConstantFP::get(*llvm_context, llvm::APFloat(value)), llvm::Type::getFloatTy(*llvm_context));
}
fruitlang::Type fruitlang::Float_Literal::typecheck(fruitlang::Typechecker &typechecker) {
    std::cerr << "blub!\n";
    return typechecker.get_type("f32");
}

std::string fruitlang::Int_Literal::render_dot(std::ofstream &file) {
    auto my_id = id();
    file << my_id << " [label=\"" << value << "\"];\n";
    return my_id;
}
fruitlang::Type fruitlang::Int_Literal::typecheck(fruitlang::Typechecker &typechecker) {
    if (value < std::numeric_limits<bool>::max()) return typechecker.get_type("bool");
    if (value < std::numeric_limits<int8_t>::max()) return typechecker.get_type("i8");
    if (value < std::numeric_limits<int16_t>::max()) return typechecker.get_type("i16");
    if (value < std::numeric_limits<int32_t>::max()) return typechecker.get_type("i32");
    if (value < std::numeric_limits<int64_t>::max()) return typechecker.get_type("i64");
    return typechecker.Error(std::to_string(value) + " out of type range!\n");
}
llvm::Value *fruitlang::Int_Literal::codegen(fruitlang::Typechecker &) {
    if (value < std::numeric_limits<bool>::max()) return llvm::ConstantInt::get(*llvm_context, llvm::APInt(1, value));
    if (value < std::numeric_limits<int8_t>::max()) return llvm::ConstantInt::get(*llvm_context, llvm::APInt(8, value));
    if (value < std::numeric_limits<int16_t>::max()) return llvm::ConstantInt::get(*llvm_context, llvm::APInt(16, value));
    if (value < std::numeric_limits<int32_t>::max()) return llvm::ConstantInt::get(*llvm_context, llvm::APInt(32, value));
    if (value < std::numeric_limits<int64_t>::max()) return llvm::ConstantInt::get(*llvm_context, llvm::APInt(64, value));
    return CodegenError(std::to_string(value) + " out of type range!\n");
}
