//
// Created by paul on 26.06.23.
//

#include "includes.h"

#ifndef FRUITLANG_PARSER_H
#define FRUITLANG_PARSER_H

#include "Lexer.h"
#include "ast/ast.h"
#include "ast/callable.h"
#include "ast/definition.h"
#include "ast/expr.h"
#include "ast/module.h"
#include <memory>
namespace fruitlang {

    class Parser {
    private:
        Lexer lexer;

        explicit Parser(const std::string &source) : lexer(source) {}
        std::shared_ptr<Module> Mod();
        template<bool is_fn>
        std::shared_ptr<ast> callable_definition();
        std::shared_ptr<expr> expression();
        std::shared_ptr<expr> equality();
        std::shared_ptr<expr> comparison();
        std::shared_ptr<expr> term();
        std::shared_ptr<expr> factor();
        std::shared_ptr<expr> exponent();
        std::shared_ptr<expr> unary();
        std::shared_ptr<expr> primary();

        ast_Type type();

        bool match(std::initializer_list<TokenType>);
        bool check(TokenType);
        bool is_at_end();
        Token consume(TokenType, const std::string &);
        [[noreturn]] void error(const std::string &);


    public:
        static std::shared_ptr<ast> parse(const std::string &source);
    };

}// namespace fruitlang

#endif//FRUITLANG_PARSER_H
