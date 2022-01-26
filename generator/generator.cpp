//
// Created by simi on 1/22/22.
//

#include "generator.h"

#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Utils.h>

void generator::initialize() {
    theContext = std::make_unique<llvm::LLVMContext>();
    theModule = std::make_unique<llvm::Module>("my cool jit", *theContext);
    builder = std::make_unique<llvm::IRBuilder<>>(*theContext);

    theFPM = std::make_unique<llvm::legacy::FunctionPassManager>(theModule.get());

    theFPM->add(llvm::createInstructionCombiningPass());
    theFPM->add(llvm::createReassociatePass());
    theFPM->add(llvm::createGVNPass());
    theFPM->add(llvm::createCFGSimplificationPass());
    theFPM->add(llvm::createPromoteMemoryToRegisterPass());
    theFPM->doInitialization();
}

llvm::AllocaInst *generator::CreateEntryBlockAlloca(llvm::Function *theFunction, const std::string &var_name, llvm::Type* type) {
    llvm::IRBuilder<> TmpB(&theFunction->getEntryBlock(),
                           theFunction->getEntryBlock().begin());
    auto alloca = TmpB.CreateAlloca(type, nullptr, var_name);
    alloca->mutateType(type);
    return alloca;
}
