//
// Created by paul on 25.06.23.
//

#ifndef FRUITLANG_AST_H
#define FRUITLANG_AST_H

#include <fstream>
#include <iostream>
#include <llvm/IR/Value.h>
namespace fruitlang {

    class ast {
        friend class binary_op;
        friend class unary_operation;
        friend class Module;
        friend class callable;
        friend class call;

    protected:
        virtual uint64_t render_dot(std::ofstream &) = 0;

    public:
        virtual ~ast() = 0;
        void render_dot(const std::string &);
        virtual llvm::Function *TopLVLCodegen() {
            std::cerr << "ast::TopLVLCodegen should never be called.\n";
            return nullptr;
        }
    };

}// namespace fruitlang

#endif//FRUITLANG_AST_H
