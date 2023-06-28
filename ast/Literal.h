//
// Created by paul on 26.06.23.
//

#ifndef FRUITLANG_LITERAL_H
#define FRUITLANG_LITERAL_H

#include "../codegen.h"
#include "ast.h"
#include "expr.h"
#include <cstdint>
namespace fruitlang {

    class Literal : public expr {};

    class Int_Literal : public Literal {
    private:
        uint64_t value;

    protected:
        uint64_t render_dot(std::ofstream &) override;
        llvm::Value *codegen() override { return CodegenError("Integer sind aktuell nicht implementiert!"); };

    public:
        ~Int_Literal() override = default;
        explicit Int_Literal(uint64_t value) : value(value){};
    };
    class Float_Literal : public Literal {
    private:
        double value;

    protected:
        uint64_t render_dot(std::ofstream &) override;
        llvm::Value *codegen() override;

    public:
        ~Float_Literal() override = default;
        explicit Float_Literal(double value) : value(value){};
    };
}// namespace fruitlang

#endif//FRUITLANG_LITERAL_H
