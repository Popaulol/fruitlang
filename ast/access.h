//
// Created by paul on 28.06.23.
//

#ifndef FRUITLANG_ACCESS_H
#define FRUITLANG_ACCESS_H

#include <utility>

#include "expr.h"
namespace fruitlang {

    class access : public expr {
    private:
        std::string name;
    protected:
        uint64_t render_dot(std::ofstream &) override;
        llvm::Value * codegen() override;
    public:
        explicit access(std::string name) : name(std::move(name)) {};
        ~access() override = default;
    };

}// namespace fruitlang

#endif//FRUITLANG_ACCESS_H
