//
// Created by paul on 28.06.23.
//

#include "../includes.h"

#ifndef FRUITLANG_ACCESS_H
#define FRUITLANG_ACCESS_H


#include "expr.h"

#include <utility>

namespace fruitlang {

    class access : public expr {
    private:
        std::string name;

    protected:
        std::string render_dot(std::ofstream &file) override;
        llvm::Value *codegen(fruitlang::Typechecker &) override;

        fruitlang::Type typecheck(fruitlang::Typechecker &typechecker) override;

    public:
        explicit access(std::string name) : name(std::move(name)){};
        ~access() override = default;
    };

}// namespace fruitlang

#endif//FRUITLANG_ACCESS_H
