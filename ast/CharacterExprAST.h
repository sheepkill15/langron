//
// Created by simi on 1/22/22.
//

#ifndef LANGRON_CHARACTEREXPRAST_H
#define LANGRON_CHARACTEREXPRAST_H


#include "ExprAST.h"

class CharacterExprAST : public ExprAST {
    char value;
public:
    explicit CharacterExprAST(char val): value(val) {};
    llvm::Value *codegen() override {
        return llvm::ConstantInt::get(*generator::theContext, llvm::APInt(8, value));
    }
};


#endif //LANGRON_CHARACTEREXPRAST_H
