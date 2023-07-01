//
// Created by paul on 01.07.23.
//

#include "Typechecker.h"
#include <iostream>

struct make_shared_enabler : public fruitlang::Type {
    template<typename... Args>
    explicit make_shared_enabler(Args &&...args) : fruitlang::Type(std::forward<Args>(args)...) {}
};

std::shared_ptr<fruitlang::Type> fruitlang::Typechecker::define_type(const std::string &name) {
    if (types.contains(name)) return Error("Type `" + name + "` Defined more than Once.");
    auto typ = std::make_shared<make_shared_enabler>(name);
    types[name] = typ;
    return typ;
}
fruitlang::Typechecker::Typechecker() {
    Typecheck_error = define_type("TYPECHECK_ERROR");
    ints = {
            define_type("bool"),
            define_type("i8"),
            define_type("i16"),
            define_type("i32"),
            define_type("i64"),
    };

    floats = {
            define_type("f32"),
            define_type("f64"),
    };
}
std::shared_ptr<fruitlang::Type> fruitlang::Typechecker::Error(const std::string &msg) {
    std::cerr << msg;
    return Typecheck_error;
}
std::shared_ptr<fruitlang::Type> fruitlang::Typechecker::get_type(std::string const &name) {
    if (!types.contains(name)) return Error("Type `" + name + "` isn't defined.");
    return types[name];
}
std::shared_ptr<fruitlang::expr> fruitlang::Typechecker::promote_type_to(const std::shared_ptr<fruitlang::expr> &expression, std::shared_ptr<fruitlang::Type> type) {
    auto expr_type = expression->get_type(this);
    if (expr_type == type) return expression;
    if (std::find(floats.begin(), floats.end(), type)) {

    }
}
