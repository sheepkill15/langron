//
// Created by simi on 1/22/22.
//

#ifndef LANGRON_BINARYEXPRAST_H
#define LANGRON_BINARYEXPRAST_H


#include <memory>
#include <utility>
#include "ExprAST.h"
#include "ArrayAccessExprAST.h"

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
                auto lhseArr = dynamic_cast<ArrayAccessExprAST*>(LHS.get());
                if(!lhseArr) {
                    return logError<llvm::Value*>("destination of '=' must be a variable");
                }

                auto val = RHS->codegen();
                if(!val) {
                    return nullptr;
                }
                auto ptr = lhseArr->codegen();
                if(!ptr) {
                    return nullptr;
                }
                auto finalVal = type_system::generate_cast(val, ptr->getType()->getContainedType(0), lhseArr->arrName);
                generator::builder->CreateStore(finalVal, ptr);
                return val;
            }
            auto val = RHS->codegen();
            if(!val) {
                return nullptr;
            }
            auto variable = generator::namedValues[lhse->getName()];
            if(!variable) {
                variable = generator::CreateEntryBlockAlloca(generator::builder->GetInsertBlock()->getParent(), lhse->getName(), val->getType());
                generator::namedValues[lhse->getName()] = variable;
            } else {
                val = type_system::generate_cast(val, variable->getType()->getContainedType(0), lhse->getName());
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
                return BinaryOperations::CreateAdd(l, r);
        }
        if(operation == "-") {
            return BinaryOperations::CreateSub(l, r);
        }
        if(operation == "*") {
            return BinaryOperations::CreateMul(l, r);
        }
        if(operation == "<") {
            return BinaryOperations::CreateLT(l, r);
        }
        if(operation == ">") {
            return BinaryOperations::CreateGT(l, r);
        }
        if(operation == "==") {
            return BinaryOperations::CreateEQ(l, r);
        }

        auto f = generator::theModule->getFunction(std::string("binary") + operation);
        assert(f && "binary operator not found!");

        auto ops = {l, r};
        return generator::builder->CreateCall(f, ops, "binop");
    }
};


#endif //LANGRON_BINARYEXPRAST_H
