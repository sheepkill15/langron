//
// Created by simi on 1/22/22.
//

#ifndef LANGRON_PROTOTYPEAST_H
#define LANGRON_PROTOTYPEAST_H


#include <string>
#include <vector>
#include "TypeExprAST.h"

class PrototypeAST {
    std::string name;
    std::vector<std::pair<std::string, std::unique_ptr<TypeExprAST>>> args;

    bool isOperator;
    unsigned precedence;

    std::unique_ptr<TypeExprAST> type;

public:
    PrototypeAST(std::string name, std::unique_ptr<TypeExprAST> type, std::vector<std::pair<std::string, std::unique_ptr<TypeExprAST>>> args, bool isOperator = false, unsigned prec = 0)
    : name(std::move(name)), type(std::move(type)), args(std::move(args)), isOperator(isOperator), precedence(prec) {};

    [[nodiscard]] const std::string &getName() const { return name; }

    [[nodiscard]] bool isUnaryOp() const { return isOperator && args.size() == 1; }
    [[nodiscard]] bool isBinaryOp() const { return isOperator && args.size() == 2; }

    [[nodiscard]] std::string getOperatorName() const {
        assert(isUnaryOp() || isBinaryOp());
        if(isUnaryOp()) {
            return name.substr(5);
        }
        if(isBinaryOp()) {
            return name.substr(6);
        }
        return name.substr(name.size() - 1);
    }

    [[nodiscard]] unsigned getBinaryPrecedence() const { return precedence; }


    llvm::Function* codegen() {
        std::vector<llvm::Type*> types(args.size());
        size_t i = 0;
        for(auto& pair : args) {
            if(pair.second->isArray) {
                pair.second->isReference = true;
            }
            types[i++] = pair.second->typegen();
        }
        auto fType = llvm::Type::getDoubleTy(*generator::theContext);
        if(type) {
            fType = type->typegen();
        }
        llvm::FunctionType* ft = llvm::FunctionType::get(fType, types, false);
        llvm::Function *f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, name, generator::theModule.get());
        unsigned idx = 0;
        for(auto& arg : f->args()) {
            arg.setName(args[idx++].first);
        }
        return f;
    }

};


#endif //LANGRON_PROTOTYPEAST_H
