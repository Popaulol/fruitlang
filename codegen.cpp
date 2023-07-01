//
// Created by paul on 28.06.23.
//

#include "codegen.h"
namespace fruitlang {
    std::unique_ptr<llvm::LLVMContext> llvm_context;
    std::unique_ptr<llvm::IRBuilder<>> ir_builder;
    std::unique_ptr<llvm::Module> ir_module;
    std::map<std::string, llvm::Value *> variables;
    std::unique_ptr<llvm::legacy::FunctionPassManager> function_pass_manager;
    std::unique_ptr<llvm::orc::KaleidoscopeJIT> jit_compiler;

    llvm::Value *CodegenError(std::string const &msg) {
        std::cerr << "Codegen Error: " << msg << "\n";
        return nullptr;
    }

    void InitializeLLVM() {
        llvm::InitializeNativeTarget();
        llvm::InitializeNativeTargetAsmPrinter();
        llvm::InitializeNativeTargetAsmParser();

        jit_compiler = cantFail(llvm::orc::KaleidoscopeJIT::Create());
        llvm_context = std::make_unique<llvm::LLVMContext>();
        ir_builder = std::make_unique<llvm::IRBuilder<>>(*llvm_context);
        ir_module = std::make_unique<llvm::Module>("my cool jit", *llvm_context);

        ir_module->setDataLayout(jit_compiler->getDataLayout());

        function_pass_manager = std::make_unique<llvm::legacy::FunctionPassManager>(ir_module.get());


        function_pass_manager->add(llvm::createInstructionCombiningPass());
        function_pass_manager->add(llvm::createReassociatePass());
        function_pass_manager->add(llvm::createGVNPass());
        function_pass_manager->add(llvm::createCFGSimplificationPass());

        function_pass_manager->doInitialization();
    }
}// namespace fruitlang
