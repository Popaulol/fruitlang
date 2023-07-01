//
// Created by paul on 01.07.23.
//

#ifndef FRUITLANG_TYPECHECKER_H
#define FRUITLANG_TYPECHECKER_H

#include "ast/expr.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
namespace fruitlang {
    class Type {
        friend class Typechecker;

    protected:
        std::string name;
        explicit Type(std::string name) : name(std::move(name)){};
    };

    class Typechecker {
        std::unordered_map<std::string, std::shared_ptr<Type>> types;
        std::shared_ptr<Type> Typecheck_error;
        std::vector<std::shared_ptr<Type>> ints;
        std::vector<std::shared_ptr<Type>> floats;

    public:
        Typechecker();
        std::shared_ptr<fruitlang::Type> define_type(const std::string &name);
        std::shared_ptr<fruitlang::Type> get_type(const std::string &name);

        std::shared_ptr<fruitlang::expr> promote_type_to(const std::shared_ptr<fruitlang::expr>&, std::shared_ptr<Type>);

        std::shared_ptr<fruitlang::Type> Error(const std::string &msg);
    };
}// namespace fruitlang

#endif//FRUITLANG_TYPECHECKER_H
