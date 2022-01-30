//
// Created by simi on 1/22/22.
//

#ifndef LANGRON_CALLEXPRAST_H
#define LANGRON_CALLEXPRAST_H


#include <string>
#include <utility>
#include <vector>
#include <memory>
#include "ExprAST.h"

class CallExprAST : public ExprAST {
private:
    std::string func_name;
    std::vector<std::unique_ptr<ExprAST>> args;

public:
    CallExprAST(std::string funcName, std::vector<std::unique_ptr<ExprAST>> args)
            : func_name(std::move(funcName)), args(std::move(args)) {}

    llvm::Value *codegen() override {
        llvm::Function* calleeF = generator::theModule->getFunction(func_name);
        if(!calleeF) {
            return logError<llvm::Value*>("Unknown function referenced");
        }
        if(calleeF->arg_size() != args.size()) {
            return logError<llvm::Value*>("Incorrect # arguments passed");
        }
        std::vector<llvm::Value*> argsV;
        for(auto & arg : args) {
            auto argV = arg->codegen();
            if(!argV) {
                return nullptr;
            }
            argsV.push_back(argV);
        }
        return generator::builder->CreateCall(calleeF, argsV, "calltmp");
    }
};


#endif //LANGRON_CALLEXPRAST_H
