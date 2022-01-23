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
        generator::builder->CreateCondBr(condV, thenBB, elseBB);

        generator::builder->SetInsertPoint(thenBB);
        auto thenV = Then->codegen();
        if(!thenV) {
            return nullptr;
        }
        generator::builder->CreateBr(mergeBB);
        thenBB = generator::builder->GetInsertBlock();

        theFunction->getBasicBlockList().push_back(elseBB);
        generator::builder->SetInsertPoint(elseBB);
        auto elseV = Else->codegen();
        if(!elseV) {
            return nullptr;
        }

        generator::builder->CreateBr(mergeBB);
        elseBB = generator::builder->GetInsertBlock();

        theFunction->getBasicBlockList().push_back(mergeBB);
        generator::builder->SetInsertPoint(mergeBB);
        auto PN = generator::builder->CreatePHI(llvm::Type::getDoubleTy(*generator::theContext), 2, "iftmp");
        PN->addIncoming(thenV, thenBB);
        PN->addIncoming(elseV, elseBB);
        return PN;
    };
};

#endif //LANGRON_IFEXPRAST_H
