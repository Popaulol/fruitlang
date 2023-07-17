//
// Created by paul on 25.06.23.
//

#include "../includes.h"

#ifndef FRUITLANG_AST_H
#define FRUITLANG_AST_H

#include "../codegen.h"

#include <fstream>
#include <iostream>

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
        virtual llvm::Function *TopLVLCodegen([[maybe_unused]] Typechecker&) {
            std::cerr << "ast::TopLVLCodegen should never be called.\n";
            return nullptr;
        }

        virtual std::vector<Type> get_types([[maybe_unused]] Typechecker&) {
                std::cerr << "This shouldn't be called ever\n";
                return {};
        };
    };

}// namespace fruitlang

#endif//FRUITLANG_AST_H
