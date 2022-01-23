//
// Created by simi on 1/22/22.
//

#ifndef LANGRON_PROTOTYPEAST_H
#define LANGRON_PROTOTYPEAST_H


#include <string>
#include <vector>

class PrototypeAST {
    std::string name;
    std::vector<std::string> args;

    bool isOperator;
    unsigned precedence;

public:
    PrototypeAST(std::string name, std::vector<std::string> args, bool isOperator = false, unsigned prec = 0)
    : name(std::move(name)), args(std::move(args)), isOperator(isOperator), precedence(prec) {};

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
        std::vector<llvm::Type*> doubles(args.size(), llvm::Type::getDoubleTy(*generator::theContext));
        llvm::FunctionType* ft = llvm::FunctionType::get(llvm::Type::getDoubleTy(*generator::theContext), doubles, false);
        llvm::Function *f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, name, generator::theModule.get());
        unsigned idx = 0;
        for(auto& arg : f->args()) {
            arg.setName(args[idx++]);
        }
        return f;
    }

};


#endif //LANGRON_PROTOTYPEAST_H
