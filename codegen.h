//
// Created by paul on 28.06.23.
//

#include "includes.h"

#ifndef FRUITLANG_CODEGEN_H
#define FRUITLANG_CODEGEN_H
#include <iostream>
#include <map>
#include <memory>

// #include "/usr/share/doc/llvm-14-examples/examples/Kaleidoscope/include/KaleidoscopeJIT.h"
#include "/home/paul/llvm-project-llvmorg-14.0.6/llvm/examples/Kaleidoscope/include/KaleidoscopeJIT.h"
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>

namespace fruitlang {
    extern std::unique_ptr<llvm::LLVMContext> llvm_context;
    extern std::unique_ptr<llvm::IRBuilder<>> ir_builder;
    extern std::unique_ptr<llvm::Module> ir_module;
    extern std::map<std::string, llvm::Value *> variables;
    extern std::unique_ptr<llvm::legacy::FunctionPassManager> function_pass_manager;
    extern std::unique_ptr<llvm::orc::KaleidoscopeJIT> jit_compiler;

    extern llvm::ExitOnError ExitOnErr;

    llvm::Value *CodegenError(const std::string &msg);
    void InitializeLLVM();
}// namespace fruitlang
#endif//FRUITLANG_CODEGEN_H
