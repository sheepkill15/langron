//
// Created by simi on 1/29/22.
//

#ifndef LANGRON_ARRAYACCESSEXPRAST_H
#define LANGRON_ARRAYACCESSEXPRAST_H


#include "ExprAST.h"
#include "NumberExprAST.h"

class ArrayAccessExprAST : public ExprAST {
    std::string arrName;
    std::unique_ptr<ExprAST> index;
public:
    ArrayAccessExprAST(std::string arrName, std::unique_ptr<ExprAST> index) : arrName(std::move(arrName)), index(std::move(index)) {};
    llvm::Value* codegen() override {
        if(!generator::namedValues.contains(arrName)) {
            return logError<llvm::Value*>("Unknown variable name");
        }
        llvm::AllocaInst *v = generator::namedValues.at(arrName);
        auto zero = llvm::ConstantInt::get(*generator::theContext, llvm::APInt(64, 0, true));
        auto intIndex = generator::builder->CreateFPToUI(index->codegen(), llvm::Type::getInt32Ty(*generator::theContext), "casttmp");
        auto gep = generator::builder->CreateGEP(v,
                {zero, intIndex}, "reftmp");
        if(asPointer) {
            return gep;
        }
        return generator::builder->CreateLoad(gep->getType()->getContainedType(0), gep, "loadtmp");
    }

    bool asPointer = false;
};


#endif //LANGRON_ARRAYACCESSEXPRAST_H
