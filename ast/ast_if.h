//
// Created by paul on 25.07.23.
//

#ifndef FRUITLANG_AST_IF_H
#define FRUITLANG_AST_IF_H

#include <utility>

#include "../includes.h"
#include "expr.h"
namespace fruitlang {

    class ast_if : public expr {
        std::shared_ptr<expr> condition;
        std::shared_ptr<expr> body;
        std::shared_ptr<expr> else_block;

    public:
        ~ast_if() override = default;

        ast_if(std::shared_ptr<expr> condition,
               std::shared_ptr<expr> body,
               std::shared_ptr<expr> else_block) : condition(std::move(condition)), body(std::move(body)), else_block(std::move(else_block)){};

        ast_if(std::shared_ptr<expr> condition,
               std::shared_ptr<expr> body) : ast_if(std::move(condition), std::move(body), nullptr){};


    protected:
        std::string render_dot(std::ofstream &ofstream) override;
        llvm::Value *codegen(Typechecker &typechecker) override;
        Type typecheck(Typechecker &typechecker) override;
    };

}// namespace fruitlang

#endif//FRUITLANG_AST_IF_H
