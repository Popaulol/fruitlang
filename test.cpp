//
// Created by paul on 24.07.23.
//

#include "Parser.h"
#include <sstream>

template<typename ret_type, typename arg1_type>
bool run_test(std::string name, std::string src, ret_type expected_value, arg1_type arg) {
    fruitlang::InitializeLLVM();
    auto ast = fruitlang::Parser::parse(src);
    auto typechecker = fruitlang::Typechecker();

    ast->get_types(typechecker);
    ast->TopLVLCodegen(typechecker);

    ast->render_dot(name + ".dot");

    fruitlang::ir_module->print(llvm::errs(), nullptr);
    fruitlang::ExitOnErr(fruitlang::jit_compiler->addModule({std::move(fruitlang::ir_module), std::move(fruitlang::llvm_context)}));
    auto test_fn = cantFail(fruitlang::jit_compiler->lookup("test"));
    auto test_FP = (ret_type (*)(arg1_type))(intptr_t) test_fn.getAddress();
    ret_type value;
    if ((value = test_FP(arg)) != expected_value) {
        std::cerr << "Test: " << name << " Failed with value `" << value << "` instead of `" << expected_value << "`\n";
        return false;
    } else {
        std::cerr << "Test: " << name << " passed.\n";
        return true;
    }
}

int main() {
    run_test("simple float arithmatic", "fn test(a: f32) -> f32: a + 1.0 - 2.0 * 3.0 / 4.0 % 5.0;", 0.5f, 1.0f);
    run_test("simple double arithmatic", "fn make_f64() -> f64: 1; fn test(a: f64) -> f64: a + (1.0 * make_f64()) - (2.0 * make_f64()) * (3.0 * make_f64()) / (4.0 * make_f64()) % (5.0 * make_f64());", 0.5, 1.0);
}
