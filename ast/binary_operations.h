//
// Created by paul on 25.06.23.
//

#ifndef FRUITLANG_BINARY_OPERATIONS_H
#define FRUITLANG_BINARY_OPERATIONS_H
#include <memory>
#include <utility>

#include "../codegen.h"
#include "expr.h"
namespace fruitlang {

    class binary_op : public expr {
    protected:
        std::shared_ptr<expr> lhs;
        std::shared_ptr<expr> rhs;

        binary_op(std::shared_ptr<expr> lhs, std::shared_ptr<expr> rhs) : lhs(std::move(lhs)), rhs(std::move(rhs)){};
        uint64_t render_dot(std::ofstream &) override;

        llvm::Value *codegen() override;

        virtual llvm::Value *sub_codegen(llvm::Value *L, llvm::Value *R) = 0;

        virtual std::string op() = 0;

        std::shared_ptr<Type> typecheck(fruitlang::Typechecker &) override;
    };

    class plus : public binary_op {
    protected:
        std::string op() override { return "+"; }
        llvm::Value *sub_codegen(llvm::Value *L, llvm::Value *R) override;

    public:
        plus(std::shared_ptr<expr> lhs, std::shared_ptr<expr> rhs) : binary_op(std::move(lhs), std::move(rhs)) {}
        ~plus() override = default;
    };

    class minus : public binary_op {
    protected:
        std::string op() override { return "-"; }
        llvm::Value *sub_codegen(llvm::Value *L, llvm::Value *R) override;

    public:
        minus(std::shared_ptr<expr> lhs, std::shared_ptr<expr> rhs) : binary_op(std::move(lhs), std::move(rhs)) {}
        ~minus() override = default;
    };

    class times : public binary_op {
    protected:
        std::string op() override { return "*"; }
        llvm::Value *sub_codegen(llvm::Value *L, llvm::Value *R) override;

    public:
        times(std::shared_ptr<expr> lhs, std::shared_ptr<expr> rhs) : binary_op(std::move(lhs), std::move(rhs)) {}
        ~times() override = default;
    };

    class div : public binary_op {
    protected:
        std::string op() override { return "/"; }
        llvm::Value *sub_codegen(llvm::Value *L, llvm::Value *R) override;

    public:
        div(std::shared_ptr<expr> lhs, std::shared_ptr<expr> rhs) : binary_op(std::move(lhs), std::move(rhs)) {}
        ~div() override = default;
    };

    class power : public binary_op {
    protected:
        std::string op() override { return "**"; }
        llvm::Value *sub_codegen(llvm::Value *L, llvm::Value *R) override;

    public:
        power(std::shared_ptr<expr> lhs, std::shared_ptr<expr> rhs) : binary_op(std::move(lhs), std::move(rhs)) {}
        ~power() override = default;
    };

    class mod : public binary_op {
    protected:
        std::string op() override { return "%"; }
        llvm::Value *sub_codegen(llvm::Value *L, llvm::Value *R) override;

    public:
        mod(std::shared_ptr<expr> lhs, std::shared_ptr<expr> rhs) : binary_op(std::move(lhs), std::move(rhs)) {}
        ~mod() override = default;
    };

    class comp_not_equal : public binary_op {
    protected:
        std::string op() override { return "!="; }
        llvm::Value *sub_codegen(llvm::Value *L, llvm::Value *R) override;

    public:
        comp_not_equal(std::shared_ptr<expr> lhs, std::shared_ptr<expr> rhs) : binary_op(std::move(lhs), std::move(rhs)) {}
        ~comp_not_equal() override = default;
    };

    class comp_equal : public binary_op {
    protected:
        std::string op() override { return "=="; }
        llvm::Value *sub_codegen(llvm::Value *L, llvm::Value *R) override;

    public:
        comp_equal(std::shared_ptr<expr> lhs, std::shared_ptr<expr> rhs) : binary_op(std::move(lhs), std::move(rhs)) {}
        ~comp_equal() override = default;
    };

    class comp_greater : public binary_op {
    protected:
        std::string op() override { return ">"; }
        llvm::Value *sub_codegen(llvm::Value *L, llvm::Value *R) override;

    public:
        comp_greater(std::shared_ptr<expr> lhs, std::shared_ptr<expr> rhs) : binary_op(std::move(lhs), std::move(rhs)) {}
        ~comp_greater() override = default;
    };

    class comp_greater_equal : public binary_op {
    protected:
        std::string op() override { return ">="; }
        llvm::Value *sub_codegen(llvm::Value *L, llvm::Value *R) override;

    public:
        comp_greater_equal(std::shared_ptr<expr> lhs, std::shared_ptr<expr> rhs) : binary_op(std::move(lhs), std::move(rhs)) {}
        ~comp_greater_equal() override = default;
    };

    class comp_less : public binary_op {
    protected:
        std::string op() override { return "<"; }
        llvm::Value *sub_codegen(llvm::Value *L, llvm::Value *R) override;

    public:
        comp_less(std::shared_ptr<expr> lhs, std::shared_ptr<expr> rhs) : binary_op(std::move(lhs), std::move(rhs)) {}
        ~comp_less() override = default;
    };

    class comp_less_equal : public binary_op {
    protected:
        std::string op() override { return "<="; }
        llvm::Value *sub_codegen(llvm::Value *L, llvm::Value *R) override;

    public:
        comp_less_equal(std::shared_ptr<expr> lhs, std::shared_ptr<expr> rhs) : binary_op(std::move(lhs), std::move(rhs)) {}
        ~comp_less_equal() override = default;
    };


}// namespace fruitlang

#endif//FRUITLANG_BINARY_OPERATIONS_H
