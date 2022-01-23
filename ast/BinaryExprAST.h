//
// Created by simi on 1/22/22.
//

#ifndef LANGRON_BINARYEXPRAST_H
#define LANGRON_BINARYEXPRAST_H


#include <memory>
#include <utility>
#include "ExprAST.h"

class BinaryExprAST : public ExprAST {
    std::string operation;
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(std::string op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs):
    operation(std::move(op)), LHS(std::move(lhs)), RHS(std::move(rhs)) {};

    llvm::Value *codegen() override {

        if(operation == "=") {
            auto lhse = dynamic_cast<VariableExprAST*>(LHS.get());
            if(!lhse) {
                return logError<llvm::Value*>("destination of '=' must be a variable");
            }
            auto val = RHS->codegen();
            if(!val) {
                return nullptr;
            }
            auto variable = generator::namedValues[lhse->getName()];
            if(!variable) {
                variable = generator::CreateEntryBlockAlloca(generator::builder->GetInsertBlock()->getParent(), lhse->getName());
                generator::namedValues[lhse->getName()] = variable;
            }
            generator::builder->CreateStore(val, variable);
            return val;
        }

        auto l = LHS->codegen();
        auto r = RHS->codegen();

        if(!l || !r) {
            return nullptr;
        }
        if(operation == "+") {
            return generator::builder->CreateFAdd(l, r, "addtmp");
        }
        if(operation == "-") {
            return generator::builder->CreateFSub(l, r, "subtmp");
        }
        if(operation == "*") {
            return generator::builder->CreateFMul(l, r, "multmp");
        }
        if(operation == "<") {
            l = generator::builder->CreateFCmpULT(l, r, "cmptmp");
            return generator::builder->CreateUIToFP(l, llvm::Type::getDoubleTy(*generator::theContext), "booltmp");
        }
        if(operation == "==") {
            l = generator::builder->CreateFCmpOEQ(l, r, "cmptmp");
            return generator::builder->CreateUIToFP(l, llvm::Type::getDoubleTy(*generator::theContext), "booltmp");
        }

        auto f = generator::theModule->getFunction(std::string("binary") + operation);
        assert(f && "binary operator not found!");

        auto ops = {l, r};
        return generator::builder->CreateCall(f, ops, "binop");
    }
};


#endif //LANGRON_BINARYEXPRAST_H
