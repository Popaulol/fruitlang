//
// Created by paul on 15.06.23.
//

#ifndef FRUITLANG_TOKEN_H
#define FRUITLANG_TOKEN_H

#include <iostream>
#include <string>
#include <utility>

namespace fruitlang {
    enum class TokenType {
        // This Token is invalid
        ERROR,

        // Used to signify End of Input
        EOF_,

        // Operators
        Plus,
        Minus,
        Star,
        Slash,
        Power,
        Percent,
        Less,
        Greater,

        Bang,

        Equal,

        Equal_Equal,
        Bang_Equal,
        Less_Equal,
        Greater_Equal,


        // Brackets
        LeftParentheses,
        RightParentheses,
        LeftBracket,
        RightBracket,
        LeftBrace,
        RightBrace,

        // Syntax Elements
        Colon,
        Comma,
        Period,
        Arrow,

        Newline,
        Semicolon,

        // Keywords
        if_,
        else_,

        for_,
        while_,
        do_,

        mod,

        fn,
        proc,

        // "Literals"
        string,
        character,
        number,
        Identifier,
    };

    struct Token {
        TokenType Type;
        std::string content;
        uint64_t pos;

        Token() : Type(TokenType::ERROR), pos(0){};

        Token(TokenType type, std::string content, uint64_t pos) : Type(type), content(std::move(content)), pos(pos){};

        void print() const {
            std::cout << "TOKEN:\ttype = " << (int) Type << "\tcontent: `" << content << "`\tpos: " << pos << std::endl;
        }
    };
}// namespace fruitlang

#endif//FRUITLANG_TOKEN_H
