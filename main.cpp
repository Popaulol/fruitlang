#include "Parser.h"
#include <sstream>
int main() {
    fruitlang::InitializeLLVM();

    std::ifstream inp;
    std::ostringstream sstr;
    inp.open("../file.fruitlang");
    sstr << inp.rdbuf();
    //sstr << "\nfn _start() -> f64: main();\n";

    auto ast = fruitlang::Parser::parse(sstr.str());// ** 6.6 ");

    auto typechecker = fruitlang::Typechecker();
    ast->render_dot("ast.dot");
    ast->get_types(typechecker);
    ast->render_dot("typed_ast.dot");
    ast->TopLVLCodegen(typechecker);
    fruitlang::ir_module->print(llvm::errs(), nullptr);

    fruitlang::ExitOnErr(fruitlang::jit_compiler->addModule({std::move(fruitlang::ir_module), std::move(fruitlang::llvm_context)}));
    auto main_fn = cantFail(fruitlang::jit_compiler->lookup("main"));
    auto main_FP = (double (*)())(intptr_t) main_fn.getAddress();
    std::cout << "Result: " << main_FP() << "\n";
}
