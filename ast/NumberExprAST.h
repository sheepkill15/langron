//
// Created by simi on 1/22/22.
//

#ifndef LANGRON_NUMBEREXPRAST_H
#define LANGRON_NUMBEREXPRAST_H


#include "ExprAST.h"

class NumberExprAST : public ExprAST {
    double value;
public:
    explicit NumberExprAST(double val): value(val) {};
    llvm::Value *codegen() override {
        return llvm::ConstantFP::get(*generator::theContext, llvm::APFloat(value));
    }
};


#endif //LANGRON_NUMBEREXPRAST_H
