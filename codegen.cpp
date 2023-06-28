//
// Created by paul on 28.06.23.
//

#include "codegen.h"
namespace fruitlang {
    std::unique_ptr<llvm::LLVMContext> TheContext;
    std::unique_ptr<llvm::IRBuilder<>> Builder;
    std::unique_ptr<llvm::Module> TheModule;
    std::map<std::string, llvm::Value *> NamedValues;

    llvm::Value *CodegenError(std::string const &msg) {
        std::cerr << "Codegen Error: " << msg << "\n";
        return nullptr;
    }

    void InitializeModule() {
        TheContext = std::make_unique<llvm::LLVMContext>();
        TheModule = std::make_unique<llvm::Module>("my cool jit", *TheContext);
        Builder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
    }
}
