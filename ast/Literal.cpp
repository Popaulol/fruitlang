//
// Created by paul on 26.06.23.
//

#include "Literal.h"
#include "../utils.h"
#include "../codegen.h"

uint64_t fruitlang::Float_Literal::render_dot(std::ofstream &file) {
    auto my_id = id();
    file << "id_" << my_id << " [label=\"" << value << "f\"];\n";
    return my_id;
}
llvm::Value *fruitlang::Float_Literal::codegen() {
    return llvm::ConstantFP::get(*TheContext, llvm::APFloat(value));
}

uint64_t fruitlang::Int_Literal::render_dot(std::ofstream &file) {
    auto my_id = id();
    file << "id_" << my_id << " [label=\"" << value << "\"];\n";
    return my_id;
}
