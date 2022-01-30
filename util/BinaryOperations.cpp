//
// Created by simi on 1/29/22.
//

#include "BinaryOperations.h"
#include "generator/generator.h"

std::map<std::string, int> BinaryOperations::binOpPrecedence = {
        {
                "<", 10
        },
        {
                "+", 20
        },
        {
                "-", 20
        },
        {
                "*", 40
        },
        {
                "=", 2
        },
        {
                "==", 9
        }
};

int BinaryOperations::getTokPrecedence(const std::string& token) {
    if(!binOpPrecedence.contains(token)) {
        return -1;
    }
    int tokPrec = binOpPrecedence.at(token);
    if(tokPrec <= 0) return -1;
    return tokPrec;
}

llvm::Value *BinaryOperations::CreateAdd(llvm::Value *lhs, llvm::Value *rhs) {
    if(lhs->getType()->isIntegerTy()) {
        if(!rhs->getType()->isIntegerTy()) {
            auto rhsCast = generator::builder->CreateIntCast(rhs, lhs->getType(), true, "tointtmp");
            return generator::builder->CreateAdd(lhs, rhsCast, "addtmp");
        }
        return generator::builder->CreateAdd(lhs, rhs, "addtmp");
    }
    return generator::builder->CreateFAdd(lhs, rhs, "addtmp");
}

llvm::Value *BinaryOperations::CreateSub(llvm::Value *lhs, llvm::Value *rhs) {
    if(lhs->getType()->isIntegerTy()) {
        if(!rhs->getType()->isIntegerTy()) {
            auto rhsCast = generator::builder->CreateIntCast(rhs, lhs->getType(), true, "tointtmp");
            return generator::builder->CreateSub(lhs, rhsCast, "subtmp");
        }
        return generator::builder->CreateSub(lhs, rhs, "subtmp");
    }
    return generator::builder->CreateFSub(lhs, rhs, "subtmp");
}

llvm::Value *BinaryOperations::CreateMul(llvm::Value *lhs, llvm::Value *rhs) {
    if(lhs->getType()->isIntegerTy()) {
        if(!rhs->getType()->isIntegerTy()) {
            auto rhsCast = generator::builder->CreateIntCast(rhs, lhs->getType(), true, "tointtmp");
            return generator::builder->CreateMul(lhs, rhsCast, "multmp");
        }
        return generator::builder->CreateMul(lhs, rhs, "multmp");
    }
    return generator::builder->CreateFMul(lhs, rhs, "multtmp");
}
