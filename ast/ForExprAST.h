//
// Created by simi on 1/22/22.
//

#ifndef LANGRON_FOREXPRAST_H
#define LANGRON_FOREXPRAST_H


#include "ExprAST.h"

#include <utility>

class ForExprAST : public ExprAST {
    std::string var_name;
    std::unique_ptr<ExprAST> Start, End, Step, Body;
public:
    ForExprAST(std::string varName, std::unique_ptr<ExprAST> start, std::unique_ptr<ExprAST> end,
               std::unique_ptr<ExprAST> step, std::unique_ptr<ExprAST> body) : var_name(std::move(varName)),
                                                                                             Start(std::move(start)), End(std::move(end)),
                                                                                             Step(std::move(step)), Body(std::move(body)) {};
    llvm::Value* codegen() override {
        auto startVal = Start->codegen();
        if(!startVal) {
            return nullptr;
        }

        auto theFunction = generator::builder->GetInsertBlock()->getParent();

        auto alloca = generator::CreateEntryBlockAlloca(theFunction, var_name, startVal->getType());

        generator::builder->CreateStore(startVal, alloca);

        auto preheaderBB = generator::builder->GetInsertBlock();
        auto loopBB = llvm::BasicBlock::Create(*generator::theContext, "loop", theFunction);
        generator::builder->CreateBr(loopBB);

        generator::builder->SetInsertPoint(loopBB);
        auto variable = generator::builder->CreatePHI(startVal->getType(), 2, var_name);
        variable->addIncoming(startVal, preheaderBB);

        llvm::AllocaInst* oldVal = nullptr;
        if(generator::namedValues.contains(var_name)) {
            oldVal = generator::namedValues.at(var_name);
        }
        generator::namedValues[var_name] = alloca;

        auto endCond = End->codegen();
        if(!endCond) {
            return nullptr;
        }
        endCond = generator::builder->CreateFCmpONE(endCond, llvm::ConstantFP::get(*generator::theContext, llvm::APFloat(0.0)), "loopcond");


        auto bodyBB = llvm::BasicBlock::Create(*generator::theContext, "body", theFunction);
        generator::builder->SetInsertPoint(bodyBB);

        if(!Body->codegen()) {
            return nullptr;
        }

        auto latchBB = llvm::BasicBlock::Create(*generator::theContext, "latch", theFunction);

        generator::builder->CreateBr(latchBB);
        generator::builder->SetInsertPoint(latchBB);

        llvm::Value* stepVal;
        if(Step) {
            stepVal = Step->codegen();
            if(!stepVal) {
                return nullptr;
            }
        } else {
            stepVal = llvm::ConstantFP::get(*generator::theContext, llvm::APFloat(1.0));
        }
        auto curVar = generator::builder->CreateLoad(alloca->getAllocatedType(), alloca, var_name);
        auto nextVar = generator::builder->CreateFAdd(curVar, stepVal, "nextvar");
        generator::builder->CreateStore(nextVar, alloca);

        generator::builder->CreateBr(loopBB);
        generator::builder->SetInsertPoint(loopBB);
        auto afterBB = llvm::BasicBlock::Create(*generator::theContext, "afterloop", theFunction);
        generator::builder->CreateCondBr(endCond, bodyBB, afterBB);
        generator::builder->SetInsertPoint(afterBB);

        variable->addIncoming(nextVar, latchBB);
        if(oldVal) {
            generator::namedValues[var_name] = oldVal;
        } else {
            generator::namedValues.erase(var_name);
        }
        return llvm::Constant::getNullValue(llvm::Type::getDoubleTy(*generator::theContext));
    }
};


#endif //LANGRON_FOREXPRAST_H
