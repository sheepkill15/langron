//
// Created by simi on 1/22/22.
//

#ifndef LANGRON_GENERATOR_H
#define LANGRON_GENERATOR_H

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <map>
#include <util/logger.h>
#include "types/type_system.h"

class generator {
public:
    generator() = delete;

    inline static std::unique_ptr<llvm::LLVMContext> theContext;
    inline static std::unique_ptr<llvm::IRBuilder<>> builder;
    inline static std::unique_ptr<llvm::Module> theModule;
    inline static std::map<std::string, llvm::AllocaInst*> namedValues{};

    inline static std::unique_ptr<llvm::legacy::FunctionPassManager> theFPM;

    static void initialize();
    static llvm::AllocaInst* CreateEntryBlockAlloca(llvm::Function* theFunction, const std::string& var_name, llvm::Type* type);
};


#endif //LANGRON_GENERATOR_H
