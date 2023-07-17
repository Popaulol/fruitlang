//
// Created by paul on 01.07.23.
//

#include "includes.h"

#ifndef FRUITLANG_TYPECHECKER_H
#define FRUITLANG_TYPECHECKER_H

#include "ast/call.h"
#include "ast/expr.h"
#include "codegen.h"

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
namespace fruitlang {
    class Type_ {
        friend class Typechecker;

    public:
        std::string name;

    protected:
        llvm::Type *llvm_type;

        explicit Type_(std::string name) : Type_(std::move(name), nullptr){};
        Type_(std::string name, llvm::Type *llvm_typ) : name(std::move(name)), llvm_type(llvm_typ) { };

    public:
        virtual fruitlang::Type check_call(Typechecker &typechecker, fruitlang::call &caller);

        llvm::Type *to_llvm_type() {
            if (llvm_type) return llvm_type;
            std::cerr << "Type: " << name << " was tried to convert to llvm type but didn't have one assiciated with it!\n";
            return nullptr;
        };
    };

    class type_callable : public Type_ {
    protected:
        std::vector<fruitlang::Type> arguments;
        fruitlang::Type return_type;
        fruitlang::Type check_call(Typechecker &typechecker, fruitlang::call &caller) override;
        bool proc;

    public:
        type_callable(const std::string &name, std::vector<fruitlang::Type> args, fruitlang::Type ret, bool proc=false) : Type_(name), arguments(std::move(args)), return_type(std::move(ret)), proc(proc){};
    };

    class Scope {
        friend class Typechecker;

        std::optional<std::shared_ptr<Scope>> outer_scope;
        std::unordered_map<std::string, fruitlang::Type> variables;
        bool is_function_scope;

    public:
        Scope(std::optional<std::shared_ptr<Scope>> scope, std::unordered_map<std::string, fruitlang::Type> vars, bool is_function=false) : outer_scope(std::move(scope)), variables(std::move(vars)) {is_function_scope = is_function;}
        Scope() : outer_scope({}), variables({}){is_function_scope = false;};

        void define_variable(const std::string &name, fruitlang::Type type) {
            variables[name] = std::move(type);
        };

        fruitlang::Type get(const std::string &name, Typechecker &typechecker);
        bool check_global(const std::string &name);

        void pop();

        void push(bool is_function = false) {
            outer_scope = std::make_shared<Scope>(outer_scope, std::move(variables), is_function);
            variables = {};
        }
    };

    class Typechecker {
        std::unordered_map<std::string, fruitlang::Type> types;
        fruitlang::Type Typecheck_error;
        std::vector<fruitlang::Type> ints;
        std::vector<fruitlang::Type> floats;

        Scope scope;

    public:
        bool had_side_effects = false;
        Typechecker();
        fruitlang::Type define_type(const std::string &name, llvm::Type * = nullptr);
        fruitlang::Type get_type(const std::string &name);

        bool is_float(const fruitlang::Type &);
        bool is_int(const fruitlang::Type &);

        std::shared_ptr<fruitlang::expr> promote_type_to(const std::shared_ptr<fruitlang::expr> &, fruitlang::Type);

        fruitlang::Type Error(const std::string &msg);
        fruitlang::Type TODO(const std::string &caller = __builtin_FUNCTION());
        void add_variable(const std::string &, fruitlang::Type);
        fruitlang::Type get_variable(const std::string &);
        bool check_global(const std::string &name);
        void push_scope();
        void pop_scope();
        void add_function(const std::string &name, std::vector<fruitlang::Type> args, fruitlang::Type ret);

    public:
        ~Typechecker() {
            if (scope.outer_scope) {
                std::cerr << "Not all Scopes popped when Scope was destroyed!"
                          << "\n";
            }
        };
    };
}// namespace fruitlang

#endif//FRUITLANG_TYPECHECKER_H
