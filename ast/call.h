//
// Created by paul on 26.06.23.
//

#ifndef FRUITLANG_CALL_H
#define FRUITLANG_CALL_H

#include <utility>

#include "expr.h"
namespace fruitlang {

    class call : public expr {
    private:
        std::string ident;
        std::vector<std::shared_ptr<expr>> args;
    protected:
        uint64_t render_dot(std::ofstream &) override;
        llvm::Value * codegen() override;
    public:
        ~call() override = default;
        call(std::string ident, std::vector<std::shared_ptr<expr>> args) : ident(std::move(ident)), args(std::move(args)) {};
    };

}// namespace fruitlang

#endif//FRUITLANG_CALL_H
