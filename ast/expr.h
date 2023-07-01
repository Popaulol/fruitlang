//
// Created by paul on 25.06.23.
//

#ifndef FRUITLANG_EXPR_H
#define FRUITLANG_EXPR_H

#include "../Typechecker.h"
#include "ast.h"
namespace fruitlang {
    class expr : public ast {
        friend class call;
        friend class binary_op;
        friend class callable;
        friend class unary_operation;
        friend class Typechecker;

    protected:
        virtual llvm::Value *codegen() = 0;
        std::shared_ptr<Type> get_type(Typechecker&);
        virtual std::shared_ptr<Type> typecheck(Typechecker&) = 0;
        std::shared_ptr<Type> type;
    };
}// namespace fruitlang

#endif//FRUITLANG_EXPR_H
