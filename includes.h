//
// Created by Dell on 13.07.2023.
//

#ifndef FRUITLANG_INCLUDES_H
#define FRUITLANG_INCLUDES_H

#include <memory>

namespace fruitlang {
    class Lexer;
    class Parser;
    class Token;
    class Typechecker;
    class Argument;
    class Type_;
    class type_callable;
    class ast;
    class callable;
    class proc;
    class func;
    class ast_Type;
    class Module;
    class definition;
    class expr;
    class block;
    class call;
    class inserted_cast;
    class access;
    class Literal;
    class Int_Literal;
    class Float_Literal;
    class unary_operation;
    class negate;
    class invert;
    class binary_op;
    class times;
    class div;
    class plus;
    class minus;
    class comp_less;
    class comp_less_equal;
    class comp_greater;
    class comp_greater_equal;
    class comp_not_equal;
    class mod;
    class comp_equal;
    class power;

    using Type = std::shared_ptr<Type_>;
}

#endif//FRUITLANG_INCLUDES_H
