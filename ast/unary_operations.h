//
// Created by paul on 26.06.23.
//

#ifndef FRUITLANG_UNARY_OPERATIONS_H
#define FRUITLANG_UNARY_OPERATIONS_H

#include <memory>
#include <utility>

#include "expr.h"
namespace fruitlang {

    class unary_operation : public expr {
    protected:
        std::shared_ptr<expr> operand;

        uint64_t render_dot(std::ofstream &) override;
        virtual std::string op() = 0;

        llvm::Value * codegen() override;
        virtual llvm::Value * sub_codegen(llvm::Value *op) = 0;

        explicit unary_operation(std::shared_ptr<expr> operand) : operand(std::move(operand)) {};
    };

class negate : public unary_operation {
    protected:
        std::string op() override {return "-";}
        llvm::Value * sub_codegen(llvm::Value *op) override;
    public:
        explicit negate(std::shared_ptr<expr> operand) : unary_operation(std::move(operand)) {};
        ~negate() override = default;
};

class invert : public unary_operation {
    protected:
        std::string op() override {return "!";}
        llvm::Value * sub_codegen(llvm::Value *op) override;
    public:
        explicit invert(std::shared_ptr<expr> operand) : unary_operation(std::move(operand)) {};
        ~invert() override = default;
};
}

#endif//FRUITLANG_UNARY_OPERATIONS_H
