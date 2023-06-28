//
// Created by paul on 15.06.23.
//

#ifndef FRUITLANG_LEXER_H
#define FRUITLANG_LEXER_H

#include "token.h"
#include <string>
#include <utility>
namespace fruitlang {
    class Lexer {
    private:
        std::string source;
        uint64_t start;
        uint64_t current_pos;
        uint64_t current_line;

        Token current_token;
        Token last_token;
        Token next_token;

        bool is_at_end();
        bool match(char c);
        char peek();
        char peek_next();

        char next_char();

        Token scanToken();
        Token string();
        Token chr();
        Token number();
        Token identifier();

    public:
        explicit Lexer(std::string source) : source(std::move(source)) {
            this->start = 0;
            this->current_pos = 0;
            this->current_line = 1;
            this->last_token = Token(TokenType::ERROR, "", 0);
            this->current_token = Token(TokenType::ERROR, "", 0);
            this->next_token = Token(TokenType::Identifier, "<top-level>", 0);
        };

        Token current();
        Token prev();
        Token next();
        Token advance();

        [[nodiscard]] uint64_t line() const {return current_line;};

    };
}// namespace fruitlang

#endif//FRUITLANG_LEXER_H
