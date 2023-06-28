//
// Created by paul on 28.06.23.
//

#ifndef FRUITLANG_CODEGEN_H
#define FRUITLANG_CODEGEN_H
#include <iostream>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <map>
#include <memory>
namespace fruitlang {
    extern std::unique_ptr<llvm::LLVMContext> TheContext;
    extern std::unique_ptr<llvm::IRBuilder<>> Builder;
    extern std::unique_ptr<llvm::Module> TheModule;
    extern std::map<std::string, llvm::Value *> NamedValues;

    llvm::Value *CodegenError(const std::string &msg);
    void InitializeModule();
}// namespace fruitlang
#endif//FRUITLANG_CODEGEN_H
