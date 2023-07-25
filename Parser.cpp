//
// Created by paul on 26.06.23.
//

#include "Parser.h"
#include "ast/Literal.h"
#include "ast/access.h"
#include "ast/ast_if.h"
#include "ast/binary_operations.h"
#include "ast/call.h"
#include "ast/module.h"
#include "ast/unary_operations.h"
#include <memory>
#include <vector>

namespace fruitlang {
    std::shared_ptr<ast> Parser::parse(const std::string &source) {
        Parser p = Parser(source);
        p.lexer.advance();
        return p.Mod();
    }
    std::shared_ptr<Module> Parser::Mod() {
        std::vector<std::shared_ptr<ast>> defs;
        consume(TokenType::Identifier, "Module needs a name");
        Token name = lexer.prev();
        while (match({TokenType::Newline, TokenType::Semicolon, TokenType::fn, TokenType::proc, TokenType::mod})) {
            switch (lexer.prev().Type) {
                case TokenType::Newline:
                case TokenType::Semicolon:
                    continue;
                case TokenType::fn:
                    defs.push_back(callable_definition<true>());
                    break;
                case TokenType::proc:
                    defs.push_back(callable_definition<false>());
                    break;
                case TokenType::mod: {
                    consume(TokenType::LeftBrace, "Expected `{`");
                    auto next_module = Mod();
                    defs.push_back(next_module);
                    consume(TokenType::RightBrace, "Expected `}`");
                    break;
                }
                default:
                    __builtin_unreachable();
            }
        }
        return std::make_shared<Module>(name.content, defs);
    }
    template<bool is_fn>
    std::shared_ptr<ast> Parser::callable_definition() {
        consume(TokenType::Identifier, is_fn ? "Function needs a name" : "Procedure needs a name");
        auto name = lexer.prev().content;
        consume(TokenType::LeftParentheses, "Missing (");
        std::vector<Argument> args;
        if (!match({TokenType::RightParentheses})) {
            do {
                consume(TokenType::Identifier, "Expected Parameter Name");
                auto ident = lexer.prev().content;
                consume(TokenType::Colon, "Expected `:`");
                auto typ = type();
                args.emplace_back(ident, typ);
            } while (match({TokenType::Comma}));
            consume(TokenType::RightParentheses, "Expected )");
        }
        consume(TokenType::Arrow, "Expected Return ast_Type specifier");
        auto return_type = type();
        if (match({TokenType::Semicolon, TokenType::Newline})) {
            if constexpr (is_fn) return std::make_shared<func>(name, nullptr, args, return_type);
            else
                return std::make_shared<proc>(name, nullptr, args, return_type);
        }
        consume(TokenType::Colon, "Expected `;`, `:` or end of Line.");
        auto content = expression();
        if (!match({TokenType::Semicolon, TokenType::Newline})) consume(TokenType::ERROR, "Expected `;` or end of Line.");
        if constexpr (is_fn) return std::make_shared<func>(name, content, args, return_type);
        else
            return std::make_shared<proc>(name, content, args, return_type);
    }


    std::shared_ptr<expr> Parser::expression() {
        return control_flow();
    }
    std::shared_ptr<expr> Parser::control_flow() {
        while (match({TokenType::if_, TokenType::while_, TokenType::for_, TokenType::do_})) {
            switch (lexer.prev().Type) {
                case TokenType::if_: {
                    auto condition = equality();
                    consume(TokenType::Colon, "Expected `:`");
                    auto body = equality();
                    if (match({TokenType::else_})) {
                        consume(TokenType::Colon, "Expected `:`");
                        auto else_block = equality();
                        return std::make_shared<ast_if>(condition, body, else_block);
                    }
                    return std::make_shared<ast_if>(condition, body);
                }
                default:
                    __builtin_unreachable();
            }
        }
        return equality();
    }
    std::shared_ptr<expr> Parser::equality() {
        auto expr = comparison();

        while (match({TokenType::Bang_Equal, TokenType::Equal_Equal})) {
            switch (lexer.prev().Type) {
                case TokenType::Bang_Equal: {
                    auto right = comparison();
                    expr = std::make_shared<comp_not_equal>(expr, right);
                    break;
                }
                case TokenType::Equal_Equal: {
                    auto right = comparison();
                    expr = std::make_shared<comp_equal>(expr, right);
                    break;
                }
                default:
                    __builtin_unreachable();
            }
        }
        return expr;
    }
    std::shared_ptr<expr> Parser::comparison() {
        auto expr = term();

        while (match({TokenType::Greater, TokenType::Greater_Equal, TokenType::Less, TokenType::Less_Equal})) {
            switch (lexer.prev().Type) {
                case TokenType::Greater: {
                    auto right = term();
                    expr = std::make_shared<comp_greater>(expr, right);
                    break;
                }
                case TokenType::Greater_Equal: {
                    auto right = term();
                    expr = std::make_shared<comp_greater_equal>(expr, right);
                    break;
                }
                case TokenType::Less: {
                    auto right = term();
                    expr = std::make_shared<comp_less>(expr, right);
                    break;
                }
                case TokenType::Less_Equal: {
                    auto right = term();
                    expr = std::make_shared<comp_less_equal>(expr, right);
                    break;
                }
                default:
                    __builtin_unreachable();
            }
        }
        return expr;
    }
    std::shared_ptr<expr> Parser::term() {
        auto expr = factor();

        while (match({TokenType::Minus, TokenType::Plus})) {
            switch (lexer.prev().Type) {
                case TokenType::Minus: {
                    auto right = factor();
                    expr = std::make_shared<minus>(expr, right);
                    break;
                }
                case TokenType::Plus: {
                    auto right = factor();
                    expr = std::make_shared<plus>(expr, right);
                    break;
                }
                default:
                    __builtin_unreachable();
            }
        }
        return expr;
    }
    std::shared_ptr<expr> Parser::factor() {
        auto expr = exponent();

        while (match({TokenType::Slash, TokenType::Star, TokenType::Percent})) {
            switch (lexer.prev().Type) {
                case TokenType::Slash: {
                    auto right = exponent();
                    expr = std::make_shared<div>(expr, right);
                    break;
                }
                case TokenType::Star: {
                    auto right = exponent();
                    expr = std::make_shared<times>(expr, right);
                    break;
                }
                case TokenType::Percent: {
                    auto right = exponent();
                    expr = std::make_shared<mod>(expr, right);
                    break;
                }
                default:
                    __builtin_unreachable();
            }
        }
        return expr;
    }
    std::shared_ptr<expr> Parser::exponent() {
        auto expr = unary();

        while (match({TokenType::Power})) {
            auto right = unary();
            expr = std::make_shared<power>(expr, right);
        }
        return expr;
    }
    std::shared_ptr<expr> Parser::unary() {
        if (match({TokenType::Bang, TokenType::Minus})) {
            switch (lexer.prev().Type) {
                case TokenType::Bang: {
                    auto right = primary();
                    return std::make_shared<invert>(right);
                }
                case TokenType::Minus: {
                    auto right = primary();
                    return std::make_shared<negate>(right);
                }
                default:
                    __builtin_unreachable();
            }
        }
        return primary();
    }

    std::shared_ptr<expr> Parser::primary() {
        if (match({TokenType::number})) {
            if (lexer.prev().content.find('.') != std::string::npos) {
                return std::make_shared<Float_Literal>(std::stod(lexer.prev().content));
            }
            return std::make_shared<Int_Literal>(std::stoll(lexer.prev().content));
        }
        if (match({TokenType::LeftParentheses})) {
            auto expr = expression();
            consume(TokenType::RightParentheses, "Expect ')' after expression.");
            return expr;
        }
        if (match({TokenType::LeftBrace})) {
            std::vector<std::shared_ptr<expr>> exprs;

            while (match({TokenType::Newline, TokenType::Semicolon})) {}
            while (!match({TokenType::RightBrace})) {
                exprs.push_back(expression());
                while (match({TokenType::Newline, TokenType::Semicolon})) {}
                if (is_at_end()) error("Expect '}' at end of Block.");
            }
            return std::make_shared<block>(exprs);
        }
        if (match({TokenType::Identifier})) {
            std::string name = lexer.prev().content;
            if (match({TokenType::LeftParentheses})) {
                std::vector<std::shared_ptr<expr>> args;
                if (!match({TokenType::RightParentheses})) {
                    do {
                        args.push_back(expression());
                    } while (match({TokenType::Comma}));
                    consume(TokenType::RightParentheses, "Expected `)`");
                }
                return std::make_shared<call>(name, args);
            }
            return std::make_shared<fruitlang::access>(name);
        }

        error("Expect Expression");
    }

    ast_Type Parser::type() {
        consume(TokenType::Identifier, "Expected ast_Type name");
        return ast_Type(lexer.prev().content);
    }

    bool Parser::match(std::initializer_list<TokenType> types) {
        if (std::any_of(types.begin(), types.end(), [this](TokenType type) {
                return check(type);
            })) {
            lexer.advance();
            return true;
        }
        return false;
    }
    bool Parser::check(TokenType type) {
        if (is_at_end()) return false;
        return lexer.current().Type == type;
    }
    bool Parser::is_at_end() {
        return lexer.current().Type == TokenType::EOF_;
    }

    Token Parser::consume(TokenType type, const std::string &msg) {
        if (check(type)) return lexer.advance();
        error(msg);
    }
    [[noreturn]] void Parser::error(const std::string &msg) {
        std::cerr << lexer.line() << ": " << msg << "\n";
        exit(1);
    }

}// namespace fruitlang