#include "Lexer.h"
#include "Parser.h"
#include <iostream>

int main() {
    fruitlang::InitializeLLVM();
    auto ast = fruitlang::Parser::parse("fn sin(a: float) -> float;"
                                        "fn cos(a: float) -> float;"
                                        "fn bla(a: float) -> float: cos(a + 3.14159);"
                                        "fn cosf(a: float) -> float;"
                                        "fn main() -> float: cosf(3.14159);\n"
                                        "");// ** 6.6 ");
    ast->render_dot("ast.dot");
    ast->TopLVLCodegen();
    fruitlang::ir_module->print(llvm::errs(), nullptr);

    auto mod = fruitlang::jit_compiler->addModule({std::move(fruitlang::ir_module), std::move(fruitlang::llvm_context)});
    auto main_fn = cantFail(fruitlang::jit_compiler->lookup("main"));
    auto main_FP = (double (*)())(intptr_t) main_fn.getAddress();
    std::cout << "Result: " << main_FP() << "\n";
}
