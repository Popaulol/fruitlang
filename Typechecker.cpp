//
// Created by paul on 01.07.23.
//

#include "Typechecker.h"
#include "ast/unary_operations.h"
#include "utils.h"
#include <iostream>
#include <utility>
#include "codegen.h"

struct make_shared_enabler : public fruitlang::Type_ {
    template<typename... Args>
    explicit make_shared_enabler(Args &&...args) : fruitlang::Type_(std::forward<Args>(args)...) {}
};

fruitlang::Type fruitlang::Typechecker::define_type(const std::string &name, llvm::Type *llvm_type) {
    if (types.contains(name)) return Error("Type_ `" + name + "` Defined more than Once.");
    auto typ = std::make_shared<make_shared_enabler>(name, llvm_type);
    types[name] = typ;
    return typ;
}
fruitlang::Typechecker::Typechecker() {
    Typecheck_error = define_type("TYPECHECK_ERROR");
    ints = {
            define_type("bool", llvm::Type::getInt1Ty(*llvm_context)),
            define_type("i8", llvm::Type::getInt8Ty(*llvm_context)),
            define_type("i16", llvm::Type::getInt16Ty(*llvm_context)),
            define_type("i32", llvm::Type::getInt32Ty(*llvm_context)),
            define_type("i64", llvm::Type::getInt64Ty(*llvm_context)),
    };

    floats = {
            define_type("f32", llvm::Type::getFloatTy(*llvm_context)),
            define_type("f64", llvm::Type::getDoubleTy(*llvm_context)),
    };
}
fruitlang::Type fruitlang::Typechecker::Error(const std::string &msg) {
    for (const auto &type: this->types) {
        std::cerr << type.first << "\n";
    }
    std::cerr << msg;
    return Typecheck_error;
}
fruitlang::Type fruitlang::Typechecker::get_type(std::string const &name) {
    if (!types.contains(name)) return Error("Type_ `" + name + "` isn't defined.");
    return types[name];
}
std::shared_ptr<fruitlang::expr> fruitlang::Typechecker::promote_type_to(const std::shared_ptr<fruitlang::expr> &expression, fruitlang::Type type) {
    auto root_type = expression->get_type(*this);

    if (root_type == type) {
        Error("Typechecking should never require to promote a type to itself!");
        return nullptr;
    }

    if (is_float(root_type)) {
        std::cerr << "Root is float!\n";
        if (!is_float(type)) {
            Error("Floats can only ever be promoted to bigger floats");
            return nullptr;
        }
        if (std::find(floats.begin(), floats.end(), type) < std::find(floats.begin(), floats.end(), root_type)) {
            Error("Floats can only ever be promoted to bigger floats");
            return nullptr;
        }

        return std::make_shared<inserted_cast>(type, expression);
    } else if (is_int(root_type)) {
        std::cerr << "Root is int!\n";
        if (is_float(type)) {
            std::cerr << "type is float!\n";
            return std::make_shared<inserted_cast>(type, expression);
        } else if (is_int(type)) {
            std::cerr << "type is intt!\n";
            if (std::find(ints.begin(), ints.end(), type) < std::find(ints.begin(), ints.end(), root_type)) {
                Error("Ints can only ever be promoted to bigger Ints or Floats");
                return nullptr;
            } else {
                return std::make_shared<inserted_cast>(type, expression);
            }
        } else {
            Error("Currently Promotion is reserved to builtin ints and floats");
            return nullptr;
        }
    } else {
        Error("Currently Promotion is reserved to builtin ints and floats");
        return nullptr;
    }

    return nullptr;
}
fruitlang::Type fruitlang::Typechecker::TODO(const std::string &caller) {
    return Error("TODO: " + caller + "\n");
}
void fruitlang::Typechecker::add_variable(const std::string &name, fruitlang::Type type) {
    scope.define_variable(name, std::move(type));
}
void fruitlang::Typechecker::push_scope() {
    scope.push();
}
void fruitlang::Typechecker::pop_scope() {
    scope.pop();
}
bool fruitlang::Typechecker::is_float(const fruitlang::Type &type) {
    return std::find(floats.begin(), floats.end(), type) != floats.end();
}
bool fruitlang::Typechecker::is_int(const fruitlang::Type &type) {
    return std::find(ints.begin(), ints.end(), type) != ints.end();
}
void fruitlang::Typechecker::add_function(const std::string &name, std::vector<fruitlang::Type> args, fruitlang::Type ret) {
    auto typ = std::make_shared<type_callable>(name + id(), std::move(args), std::move(ret));
    scope.define_variable(name, typ);
}
fruitlang::Type fruitlang::Typechecker::get_variable(const std::string &name) {
    return scope.get(name, *this);
}
bool fruitlang::Typechecker::check_global(const std::string &name) {
    return scope.check_global(name);
}
fruitlang::Type fruitlang::Scope::get(const std::string &name, fruitlang::Typechecker &typechecker) {
    auto var = variables[name];
    if (var) return var;
    if (outer_scope)
        return outer_scope.value()->get(name, typechecker);
    else
        return typechecker.Error("Variable `" + name + "` not found.");
}
void fruitlang::Scope::pop() {
    if (outer_scope) {
        variables = outer_scope.value()->variables;
        outer_scope = outer_scope.value()->outer_scope;
    } else {
        std::cerr << "Tried to pop Non-Existant Scope"
                  << "\n";
    }
}
bool fruitlang::Scope::check_global(const std::string &name) {
    auto var = variables[name];
    if (var) return !is_function_scope;
    if (!outer_scope)
        return true;
    return outer_scope.value()->check_global(name);
}
fruitlang::Type fruitlang::Type_::check_call(Typechecker &typechecker, [[maybe_unused]] fruitlang::call &caller) {
    return typechecker.Error("Don't try to call a variable like a function!");
}
fruitlang::Type fruitlang::type_callable::check_call(Typechecker &typechecker, fruitlang::call &caller) {
    if (proc) typechecker.had_side_effects  = true;
    if (arguments.size() != caller.args.size()) return typechecker.Error("Argument numbers don't match!");
    for (std::vector<fruitlang::Type>::size_type i = 0; i < arguments.size(); i++) {
        if(caller.args[i]->get_type(typechecker) != arguments[i]) {
            caller.args[i] = typechecker.promote_type_to(caller.args[i], arguments[i]);
        }
    }
    return return_type;
}
