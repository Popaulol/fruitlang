//
// Created by paul on 15.06.23.
//
#include <cctype>

template<bool first_char>
static bool is_ident_valid(char c) {
    if (first_char) return std::isalpha(c) || c == '_' || c == '$';
    else return std::isalnum(c) || c == '_' || c == '$';
}


#include "Lexer.h"
fruitlang::Token fruitlang::Lexer::scanToken() {
    while (true) {
        if (is_at_end()) return {TokenType::EOF_, "", current_pos - 1};
        start = current_pos;
        char c = next_char();

        switch (c) {
            case '(':
                return {TokenType::LeftParentheses, source.substr(current_pos - 1, 1), current_pos - 1};
            case ')':
                return {TokenType::RightParentheses, source.substr(current_pos - 1, 1), current_pos - 1};
            case '{':
                return {TokenType::LeftBrace, source.substr(current_pos - 1, 1), current_pos - 1};
            case '}':
                return {TokenType::RightBrace, source.substr(current_pos - 1, 1), current_pos - 1};
            case '[':
                return {TokenType::LeftBracket, source.substr(current_pos - 1, 1), current_pos - 1};
            case ']':
                return {TokenType::RightBracket, source.substr(current_pos - 1, 1), current_pos - 1};
            case ',':
                return {TokenType::Comma, source.substr(current_pos - 1, 1), current_pos - 1};
            case '.':
                return {TokenType::Period, source.substr(current_pos - 1, 1), current_pos - 1};
            case '+':
                return {TokenType::Plus, source.substr(current_pos - 1, 1), current_pos - 1};
            case ';':
                return {TokenType::Semicolon, source.substr(current_pos - 1, 1), current_pos - 1};
            case '%':
                return {TokenType::Percent, source.substr(current_pos - 1, 1), current_pos - 1};
            case ':':
                return {TokenType::Colon, source.substr(current_pos - 1, 1), current_pos - 1};
            case '-':
                return match('>') ? Token(TokenType::Arrow, source.substr(current_pos - 2, 2), current_pos - 1) : Token(TokenType::Minus, source.substr(current_pos - 1, 1), current_pos - 1);
            case '*':
                return match('*') ? Token(TokenType::Power, source.substr(current_pos - 2, 2), current_pos - 1) : Token(TokenType::Star, source.substr(current_pos - 1, 1), current_pos - 1);
            case '!':
                return match('=') ? Token(TokenType::Bang_Equal, source.substr(current_pos - 2, 2), current_pos - 1) : Token(TokenType::Bang, source.substr(current_pos - 1, 1), current_pos - 1);
            case '=':
                return match('=') ? Token(TokenType::Equal_Equal, source.substr(current_pos - 2, 2), current_pos - 1) : Token(TokenType::Equal, source.substr(current_pos - 1, 1), current_pos - 1);
            case '<':
                return match('=') ? Token(TokenType::Less_Equal, source.substr(current_pos - 2, 2), current_pos - 1) : Token(TokenType::Less, source.substr(current_pos - 1, 1), current_pos - 1);
            case '>':
                return match('=') ? Token(TokenType::Greater_Equal, source.substr(current_pos - 2, 2), current_pos - 1) : Token(TokenType::Greater, source.substr(current_pos - 1, 1), current_pos - 1);
            case '/':
                if (match('/'))
                    while (peek() != '\n' && !is_at_end()) next_char();
                else
                    return {TokenType::Slash, source.substr(current_pos - 1, 1), current_pos - 1};
                break;
            case ' ':
            case '\r':
            case '\t':
                // Ignore whitespace.
                continue;
            case '\n':
                current_line++;
                return {TokenType::Newline, source.substr(current_pos - 1, 1), current_pos - 1};

            case '"':
                return string();

            case '\'':
                return chr();

            default:
                if (std::isdigit(c)) {
                    return number();
                } else if (is_ident_valid<true>(c)) {
                    return identifier();
                } else {
                    return {TokenType::ERROR, source.substr(current_pos - 1, 1), current_pos - 1};
                }
        }
    }
}
fruitlang::Token fruitlang::Lexer::current() {
    std::cout <<  "curr:\t";
    current_token.print();
    return this->current_token;
}
fruitlang::Token fruitlang::Lexer::prev() {
    std::cout << "last:\t";
    last_token.print();
    return this->last_token;
}
fruitlang::Token fruitlang::Lexer::next() {
    std::cout << "next:\t";
    next_token.print();
    return this->next_token;
}
fruitlang::Token fruitlang::Lexer::advance() {
    this->last_token = current_token;
    this->current_token = next_token;
    this->next_token = scanToken();
    std::cout <<  "adv:\t";
    current_token.print();
    return this->current_token;
}
bool fruitlang::Lexer::is_at_end() {
    return this->current_pos >= this->source.length();
}
char fruitlang::Lexer::next_char() {
    if (is_at_end()) return '\0';
    return this->source.at(current_pos++);
}
bool fruitlang::Lexer::match(char c) {
    if (is_at_end()) return false;
    if (source.at(current_pos) != c) return false;
    current_pos++;
    return true;
}
char fruitlang::Lexer::peek() {
    if (is_at_end()) return '\0';
    return this->source.at(current_pos);
}
fruitlang::Token fruitlang::Lexer::string() {
    while (peek() != '"' && !is_at_end()) {
        if (peek() == '\n') current_line++;
        next_char();
        // TODO: Escapes
    }
    if (is_at_end()) {
        return {TokenType::ERROR, source.substr(start, current_pos), current_pos - 1};
    }
    next_char();
    return {TokenType::string, source.substr(start, start - current_pos), start};
}
fruitlang::Token fruitlang::Lexer::chr() {
    while (peek() != '\'' && !is_at_end()) {
        if (peek() == '\n') current_line++;
        next_char();
        // TODO: Escapes
    }
    if (is_at_end()) {
        return {TokenType::ERROR, source.substr(start, current_pos - 1), current_pos - 1};
    }
    next_char();
    return {TokenType::character, source.substr(start, start - current_pos), start};

}
fruitlang::Token fruitlang::Lexer::number() {
    while(std::isdigit(peek())) next_char();
    if (peek() == '.' && isdigit(peek_next())) next_char();
    while(std::isdigit(peek())) next_char();
    return {TokenType::number, source.substr(start, current_pos - start), current_pos - 1};
}
char fruitlang::Lexer::peek_next() {
    if (is_at_end()) return '\0';
    return this->source.at(current_pos + 1);
}
fruitlang::Token fruitlang::Lexer::identifier() {
    while (is_ident_valid<false>(peek())) next_char();
    std::string text = source.substr(start, current_pos - start);
#include "keywords.inc"
    return {TokenType::Identifier, std::move(text), current_pos - 1};
}
