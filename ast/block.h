//
// Created by paul on 25.06.23.
//

#ifndef FRUITLANG_BLOCK_H
#define FRUITLANG_BLOCK_H

#include "expr.h"
#include <utility>
#include <vector>
namespace fruitlang {

    class block : public expr {
        std::vector<expr> contents;
    public:
        explicit block(std::vector<expr> contents) : contents(std::move(contents)) {};
    };

}// namespace fruitlang

#endif//FRUITLANG_BLOCK_H
