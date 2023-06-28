//
// Created by paul on 25.06.23.
//

#ifndef FRUITLANG_EXPR_H
#define FRUITLANG_EXPR_H

#include "ast.h"
namespace fruitlang {

    class expr : public ast {
        friend class call;
        friend class binary_op;
        friend class callable;
        friend class unary_operation;
    protected:
        virtual llvm::Value *codegen() = 0;
    };

} // namespace fruitlang

#endif//FRUITLANG_EXPR_H
