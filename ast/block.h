//
// Created by paul on 25.06.23.
//

#include "../includes.h"

#ifndef FRUITLANG_BLOCK_H
#define FRUITLANG_BLOCK_H

#include "expr.h"

#include <utility>
#include <vector>

namespace fruitlang {

    class block : public expr {
        std::vector<std::shared_ptr<expr>> contents;

    protected:
        std::string render_dot(std::ofstream &file) override;
        llvm::Value *codegen(Typechecker &typechecker) override;

    public:
        explicit block( std::vector<std::shared_ptr<expr>> contents) : contents(std::move(contents)){}
        ~block() override = default;

    protected:
        Type typecheck(Typechecker &typechecker) override;

    };

}// namespace fruitlang

#endif//FRUITLANG_BLOCK_H
