//
// Created by paul on 25.06.23.
//

#include "../includes.h"

#ifndef FRUITLANG_CALLABLE_H
#define FRUITLANG_CALLABLE_H

#include "ast.h"
#include "ast_Type.h"
#include "expr.h"
#include "block.h"
#include "../codegen.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace fruitlang {

    class Argument {
    public:
        const std::string name;
        const ast_Type typ;

        Argument(std::string name, const ast_Type &typ) : name(std::move(name)), typ(typ){};
    };

    class callable : public ast {
    private:
        llvm::Function *prototype(Typechecker&);

    protected:
        std::string name;
        std::shared_ptr<expr> m_content;
        std::vector<Argument> m_arguments;
        ast_Type m_return_type;

        std::string render_dot(std::ofstream &file) override;
        virtual std::string kind() = 0;

        llvm::Function *TopLVLCodegen([[maybe_unused]] Typechecker&) override;

        callable(std::string name, std::shared_ptr<expr> content, std::vector<Argument> args, const ast_Type &return_type) : name(std::move(name)), m_content(std::move(content)), m_arguments(std::move(args)), m_return_type(return_type){};

        std::vector<Type> get_types(fruitlang::Typechecker &) override;
    };

    class proc : public callable {

    protected:
        std::string kind() override {
            return "proc <" + name + ">";
        }

    public:
        ~proc() override = default;
        proc(std::string name, std::shared_ptr<expr> content, std::vector<Argument> args, const ast_Type &return_type) : callable(std::move(name), std::move(content), std::move(args), return_type){};
    };

    class func : public callable {
    protected:
        std::string kind() override {
            return "fn <" + name + ">";
        }

    public:
        ~func() override = default;
        func(std::string name, std::shared_ptr<expr> content, std::vector<Argument> args, const ast_Type &return_type) : callable(std::move(name), std::move(content), std::move(args), return_type){};
    };

}// namespace fruitlang

#endif//FRUITLANG_CALLABLE_H
