//
// Created by paul on 26.06.23.
//

#include "Literal.h"
#include "../utils.h"

uint64_t fruitlang::Float_Literal::render_dot(std::ofstream &file) {
    auto my_id = id();
    file << "id_" << my_id << " [label=\"" << value << "f\"];\n";
    return my_id;
}
llvm::Value *fruitlang::Float_Literal::codegen() {
    return llvm::ConstantFP::get(*llvm_context, llvm::APFloat(value));
}
std::shared_ptr<fruitlang::Type> fruitlang::Float_Literal::typecheck(fruitlang::Typechecker &typechecker) {
    return typechecker.get_type("f64");
}

uint64_t fruitlang::Int_Literal::render_dot(std::ofstream &file) {
    auto my_id = id();
    file << "id_" << my_id << " [label=\"" << value << "\"];\n";
    return my_id;
}
std::shared_ptr<fruitlang::Type> fruitlang::Int_Literal::typecheck(fruitlang::Typechecker &typechecker) {
    return typechecker.get_type("i64");
}
