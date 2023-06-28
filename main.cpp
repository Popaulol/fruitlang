#include "Lexer.h"
#include "Parser.h"
#include <iostream>

int main() {
    /* fruitlang::Lexer l{"+ - * / ** % // Comment\n"
                       "< > ! = != == <= >= ()[]{}\n"
                       ":,.\n"
                       "; if else for while do mod fn \"dfadsfafdsf\" \'dsadsfsd dgf\' 445 45.676 dsfdsdsfdsfds\n"};
    while (true) {
        fruitlang::Token tok = l.advance();
        tok.print();
        if (tok.Type == fruitlang::TokenType::EOF_) break;
    }*/

    fruitlang::InitializeModule();
    auto ast = fruitlang::Parser::parse("fn main(a: float) -> int:  1.1 + 2.2 - 3.3 * 4.4 / 5.5"); // ** 6.6 ");
    ast->render_dot("ast.dot");
    ast->TopLVLCodegen();
    fruitlang::TheModule->print(llvm::errs(), nullptr);
}
