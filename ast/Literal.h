//
// Created by paul on 26.06.23.
//

#include "../includes.h"

#ifndef FRUITLANG_LITERAL_H
#define FRUITLANG_LITERAL_H

#include "ast.h"
#include "expr.h"
#include "../codegen.h"

#include <cstdint>

namespace fruitlang {

    class Literal : public expr {};

    class Int_Literal : public Literal {
    private:
        uint64_t value;

    protected:
        std::string render_dot(std::ofstream &file) override;
        llvm::Value *codegen(fruitlang::Typechecker &) override;
        fruitlang::Type typecheck(fruitlang::Typechecker &) override;

    public:
        ~Int_Literal() override = default;
        explicit Int_Literal(uint64_t value) : value(value){};
    };
    class Float_Literal : public Literal {
    private:
        double value;

    protected:
        std::string render_dot(std::ofstream &file) override;
        llvm::Value *codegen(fruitlang::Typechecker &) override;
        fruitlang::Type typecheck(fruitlang::Typechecker &) override;

    public:
        ~Float_Literal() override = default;
        explicit Float_Literal(double value) : value(value){};
    };
}// namespace fruitlang

#endif//FRUITLANG_LITERAL_H
