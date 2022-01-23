//
// Created by simi on 1/22/22.
//

#ifndef LANGRON_VARIABLEEXPRAST_H
#define LANGRON_VARIABLEEXPRAST_H


#include <string>
#include <utility>
#include "ExprAST.h"

class VariableExprAST : public ExprAST {
    std::string name;

public:
    explicit VariableExprAST(std::string  name): name(std::move(name)) {};

    llvm::Value *codegen() override {
        llvm::Value *v = generator::namedValues.at(name);
        if(!v) {
            logError<llvm::Value*>("Unknown variable name");
        }
        return v;
    }
};


#endif //LANGRON_VARIABLEEXPRAST_H
