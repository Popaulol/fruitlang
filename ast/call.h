//
// Created by paul on 26.06.23.
//

#include "../includes.h"

#ifndef FRUITLANG_CALL_H
#define FRUITLANG_CALL_H

#include "expr.h"
#include "../codegen.h"

#include <utility>
#include <string>
#include <vector>
#include <memory>

namespace fruitlang {

    class call : public expr {
        friend class type_callable;
    private:
        std::string ident;
        std::vector<std::shared_ptr<expr>> args;

    protected:
        std::string render_dot(std::ofstream &file) override;
        llvm::Value *codegen(fruitlang::Typechecker &) override;

        fruitlang::Type typecheck(fruitlang::Typechecker &typechecker) override;

    public:
        ~call() override = default;
        call(std::string ident, std::vector<std::shared_ptr<expr>> args) : ident(std::move(ident)), args(std::move(args)){};
    };

}// namespace fruitlang

#endif//FRUITLANG_CALL_H
