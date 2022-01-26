//
// Created by simi on 1/22/22.
//

#ifndef LANGRON_FUNCTIONAST_H
#define LANGRON_FUNCTIONAST_H


#include <memory>
#include "PrototypeAST.h"
#include "ExprAST.h"
#include "util/bin_op.h"

#include <llvm/IR/Verifier.h>

class FunctionAST {
    std::unique_ptr<PrototypeAST> prototype;
    std::unique_ptr<ExprAST> body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> proto, std::unique_ptr<ExprAST> body)
    :prototype(std::move(proto)), body(std::move(body)) {};

    llvm::Function* codegen() {
        llvm::Function* theFunction = generator::theModule->getFunction(prototype->getName());
        if(!theFunction) {
            theFunction = prototype->codegen();
        }
        if(!theFunction) {
            return nullptr;
        }
        if(!theFunction->empty()) {
            return logError<llvm::Function*>("Function cannot be redefined");
        }

        if(prototype->isBinaryOp()) {
            binOpPrecedence[prototype->getOperatorName()] = (int)prototype->getBinaryPrecedence();
        }

        llvm::BasicBlock *bb = llvm::BasicBlock::Create(*generator::theContext, "entry", theFunction);
        generator::builder->SetInsertPoint(bb);
        generator::namedValues.clear();

        for(auto& arg: theFunction->args()) {
            auto alloca = generator::CreateEntryBlockAlloca(theFunction, arg.getName().str(), arg.getType());
            generator::builder->CreateStore(&arg, alloca);
            generator::namedValues[(std::string)arg.getName()] = alloca;
        }
        if(auto retVal = body->codegen()) {
            if(!llvm::isa<llvm::ReturnInst>(generator::builder->GetInsertBlock()->back())) {
                generator::builder->CreateRet(retVal);
            }
            llvm::verifyFunction(*theFunction);
            generator::theFPM->run(*theFunction);
            return theFunction;
        }
        theFunction->eraseFromParent();
        return nullptr;
    }
};


#endif //LANGRON_FUNCTIONAST_H
