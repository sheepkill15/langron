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
    [[nodiscard]] const std::string& getName() const {return name;}
    llvm::Value *codegen() override {
        if(!generator::namedValues.contains(name)) {
            return logError<llvm::Value*>("Unknown variable name");
        }
        llvm::Value *v = generator::namedValues.at(name);
        return generator::builder->CreateLoad(v->getType()->getContainedType(0), v, name);
    }
};


#endif //LANGRON_VARIABLEEXPRAST_H
