//
// Created by paul on 25.06.23.
//

#ifndef FRUITLANG_TYPE_H
#define FRUITLANG_TYPE_H

#include <utility>

#include "ast.h"
namespace fruitlang {

    class Type : public ast {
    protected:
        uint64_t render_dot(std::ofstream &) override;
    public:
        explicit Type(std::string name) : name(std::move(name)) {};
        ~Type() override = default;
        std::string name;
    };

}// namespace fruitlang

#endif//FRUITLANG_TYPE_H
