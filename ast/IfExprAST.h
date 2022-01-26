//
// Created by simi on 1/22/22.
//

#ifndef LANGRON_IFEXPRAST_H
#define LANGRON_IFEXPRAST_H

#include "ExprAST.h"

class IfExprAST : public ExprAST {
    std::unique_ptr<ExprAST> Cond, Then, Else;
public:
    IfExprAST(std::unique_ptr<ExprAST> Cond, std::unique_ptr<ExprAST> Then, std::unique_ptr<ExprAST> Else)
    :Cond(std::move(Cond)), Then(std::move(Then)), Else(std::move(Else)) {};

    llvm::Value* codegen() override {
        auto condV = Cond->codegen();
        if(!condV) {
            return nullptr;
        }
        condV = generator::builder->CreateFCmpONE(condV, llvm::ConstantFP::get(*generator::theContext, llvm::APFloat(0.0)), "ifcond");
        auto theFunction = generator::builder->GetInsertBlock()->getParent();

        auto thenBB = llvm::BasicBlock::Create(*generator::theContext, "then", theFunction);
        auto elseBB = llvm::BasicBlock::Create(*generator::theContext, "else");
        auto mergeBB = llvm::BasicBlock::Create(*generator::theContext, "ifcont");
        generator::builder->CreateCondBr(condV, thenBB, Else == nullptr ? mergeBB : elseBB);

        generator::builder->SetInsertPoint(thenBB);
        auto thenV = Then->codegen();
        if(!thenV) {
            return nullptr;
        }
        if(!llvm::isa<llvm::ReturnInst>(thenBB->back())) {
            generator::builder->CreateBr(mergeBB);
        }
        thenBB = generator::builder->GetInsertBlock();

        llvm::Value *elseV = nullptr;
        if(Else) {
            theFunction->getBasicBlockList().push_back(elseBB);
            generator::builder->SetInsertPoint(elseBB);
            elseV = Else->codegen();
            if(!elseV) {
                return nullptr;
            }
            if(!llvm::isa<llvm::ReturnInst>(elseBB->back())) {
                generator::builder->CreateBr(mergeBB);
            }
            elseBB = generator::builder->GetInsertBlock();
        }
        theFunction->getBasicBlockList().push_back(mergeBB);
        generator::builder->SetInsertPoint(mergeBB);
        auto PN = generator::builder->CreatePHI(llvm::Type::getDoubleTy(*generator::theContext), elseV == nullptr ? 1 : 2, "iftmp");
        PN->addIncoming(thenV, thenBB);
        if(elseV) {
            PN->addIncoming(elseV, elseBB);
        }
        return PN;
        return llvm::Constant::getNullValue(llvm::Type::getDoubleTy(*generator::theContext));
    };
};

#endif //LANGRON_IFEXPRAST_H
