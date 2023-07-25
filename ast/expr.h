//
// Created by paul on 25.06.23.
//

#include "../includes.h"

#ifndef FRUITLANG_EXPR_H
#define FRUITLANG_EXPR_H

#include "ast.h"

#include <utility>

namespace fruitlang {
    class expr : public ast {
        friend class call;
        friend class binary_op;
        friend class callable;
        friend class unary_operation;
        friend class negate;
        friend class invert;
        friend class inserted_cast;
        friend class Typechecker;
        friend class type_callable;

    protected:
        virtual fruitlang::Type typecheck(Typechecker &) = 0;
        fruitlang::Type type;

    public:
        virtual llvm::Value *codegen(Typechecker &) = 0;
        fruitlang::Type get_type(Typechecker &, std::string callsite = "");
    };

    class inserted_cast : public expr {
        std::shared_ptr<expr> child;

    protected:
        llvm::Value *codegen(Typechecker &) override;
        fruitlang::Type typecheck(fruitlang::Typechecker &) override;
        std::string render_dot(std::ofstream &file) override;

    public:
        inserted_cast(fruitlang::Type typ, std::shared_ptr<expr> child) : child(std::move(child)) { type = typ; };
    };
}// namespace fruitlang

#endif//FRUITLANG_EXPR_H
