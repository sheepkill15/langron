//
// Created by simi on 1/23/22.
//

#ifndef LANGRON_UNARYEXPRAST_H
#define LANGRON_UNARYEXPRAST_H


#include "ExprAST.h"

class UnaryExprAST : public ExprAST {
    char opcode;
    std::unique_ptr<ExprAST> operand;
public:
    UnaryExprAST(char opcode, std::unique_ptr<ExprAST> operand)
    : opcode(opcode), operand(std::move(operand)) {};

    llvm::Value *codegen() override {
        auto operandV = operand->codegen();
        if(!operandV) {
            return nullptr;
        }
        auto f = generator::theModule->getFunction(std::string("unary") + opcode);
        if(!f) {
            return logError<llvm::Value*>("Unknown unary operator");
        }
        return generator::builder->CreateCall(f, operandV, "unop");
    };
};


#endif //LANGRON_UNARYEXPRAST_H
