//
// Created by simi on 2/6/22.
//

#ifndef LANGRON_STRINGEXPRAST_H
#define LANGRON_STRINGEXPRAST_H


#include "ExprAST.h"

#include <utility>

class StringExprAST : public ExprAST{
    std::string value;
public:
    explicit StringExprAST(std::string value) : value(std::move(value)) {}

    llvm::Value* codegen() override {
        return generator::builder->CreateGlobalString(value, "conststring");
    }
};


#endif //LANGRON_STRINGEXPRAST_H
