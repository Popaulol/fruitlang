//
// Created by paul on 25.06.23.
//

#ifndef FRUITLANG_AST_TYPE_H
#define FRUITLANG_AST_TYPE_H

#include <utility>

#include "ast.h"
namespace fruitlang {

    class ast_Type : public ast {
    protected:
        uint64_t render_dot(std::ofstream &) override;

    public:
        explicit ast_Type(std::string name) : name(std::move(name)){};
        ~ast_Type() override = default;
        std::string name;
    };

}// namespace fruitlang

#endif//FRUITLANG_AST_TYPE_H
