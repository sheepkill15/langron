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
                ">", 10
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
        },
        {
                "<=", 10
        },
        {
            "&&", 9
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
    if(lhs->getType()->isPointerTy()) {
        lhs = type_system::depointify(lhs, lhs->getName().str());
    }
    if(rhs->getType()->isPointerTy()) {
        rhs = type_system::depointify(rhs, rhs->getName().str());
    }
    rhs = type_system::generate_cast(rhs, lhs->getType(), "casttmp");
    if(lhs->getType()->isIntegerTy()) {
        return generator::builder->CreateAdd(lhs, rhs, "addtmp");
    }
    return generator::builder->CreateFAdd(lhs, rhs, "addtmp");
}

llvm::Value *BinaryOperations::CreateSub(llvm::Value *lhs, llvm::Value *rhs) {
    if(lhs->getType()->isPointerTy()) {
        lhs = type_system::depointify(lhs, lhs->getName().str());
    }
    if(rhs->getType()->isPointerTy()) {
        rhs = type_system::depointify(rhs, rhs->getName().str());
    }
    rhs = type_system::generate_cast(rhs, lhs->getType(), "casttmp");
    if(lhs->getType()->isIntegerTy()) {
        return generator::builder->CreateSub(lhs, rhs, "subtmp");
    }
    return generator::builder->CreateFSub(lhs, rhs, "subtmp");
}

llvm::Value *BinaryOperations::CreateMul(llvm::Value *lhs, llvm::Value *rhs) {
    if(lhs->getType()->isPointerTy()) {
        lhs = type_system::depointify(lhs, lhs->getName().str());
    }
    if(rhs->getType()->isPointerTy()) {
        rhs = type_system::depointify(rhs, rhs->getName().str());
    }
    rhs = type_system::generate_cast(rhs, lhs->getType(), "casttmp");
    if(lhs->getType()->isIntegerTy()) {
        return generator::builder->CreateMul(lhs, rhs, "multmp");
    }
    return generator::builder->CreateFMul(lhs, rhs, "multtmp");
}

llvm::Value *BinaryOperations::CreateLT(llvm::Value *lhs, llvm::Value *rhs) {
    if(lhs->getType()->isPointerTy()) {
        lhs = type_system::depointify(lhs, lhs->getName().str());
    }
    if(rhs->getType()->isPointerTy()) {
        rhs = type_system::depointify(rhs, rhs->getName().str());
    }
    rhs = type_system::generate_cast(rhs, lhs->getType(), "casttmp");
    llvm::Value* res;
    if(lhs->getType()->isIntegerTy()) {
        res = generator::builder->CreateICmpSLT(lhs, rhs, "cmptmp");
    } else {
        res = generator::builder->CreateFCmpOLT(lhs, rhs, "cmptmp");
    }
    return generator::builder->CreateUIToFP(res, llvm::Type::getDoubleTy(*generator::theContext), "booltmp");
}

llvm::Value *BinaryOperations::CreateEQ(llvm::Value *lhs, llvm::Value *rhs) {
    if(lhs->getType()->isPointerTy()) {
        lhs = type_system::depointify(lhs, lhs->getName().str());
    }
    if(rhs->getType()->isPointerTy()) {
        rhs = type_system::depointify(rhs, rhs->getName().str());
    }
    rhs = type_system::generate_cast(rhs, lhs->getType(), "casttmp");
    llvm::Value* res;
    if(lhs->getType()->isIntegerTy()) {
        res = generator::builder->CreateICmpEQ(lhs, rhs, "cmptmp");
    } else {
        res = generator::builder->CreateFCmpOEQ(lhs, rhs, "cmptmp");
    }
    return generator::builder->CreateUIToFP(res, llvm::Type::getDoubleTy(*generator::theContext), "booltmp");
}

llvm::Value *BinaryOperations::CreateGT(llvm::Value *lhs, llvm::Value *rhs) {
    if(lhs->getType()->isPointerTy()) {
        lhs = type_system::depointify(lhs, lhs->getName().str());
    }
    if(rhs->getType()->isPointerTy()) {
        rhs = type_system::depointify(rhs, rhs->getName().str());
    }
    rhs = type_system::generate_cast(rhs, lhs->getType(), "casttmp");
    llvm::Value* res;
    if(lhs->getType()->isIntegerTy()) {
        res = generator::builder->CreateICmpSGT(lhs, rhs, "cmptmp");
    } else {
        res = generator::builder->CreateFCmpOGT(lhs, rhs, "cmptmp");
    }
    return generator::builder->CreateUIToFP(res, llvm::Type::getDoubleTy(*generator::theContext), "booltmp");
}

llvm::Value *BinaryOperations::CreateLTE(llvm::Value *lhs, llvm::Value *rhs) {
    if(lhs->getType()->isPointerTy()) {
        lhs = type_system::depointify(lhs, lhs->getName().str());
    }
    if(rhs->getType()->isPointerTy()) {
        rhs = type_system::depointify(rhs, rhs->getName().str());
    }
    rhs = type_system::generate_cast(rhs, lhs->getType(), "casttmp");
    llvm::Value* res;
    if(lhs->getType()->isIntegerTy()) {
        res = generator::builder->CreateICmpSLE(lhs, rhs, "cmptmp");
    } else {
        res = generator::builder->CreateFCmpOLE(lhs, rhs, "cmptmp");
    }
    return generator::builder->CreateUIToFP(res, llvm::Type::getDoubleTy(*generator::theContext), "booltmp");
}

llvm::Value *BinaryOperations::CreateAND(llvm::Value *lhs, llvm::Value *rhs) {
    if(lhs->getType()->isPointerTy()) {
        lhs = type_system::depointify(lhs, lhs->getName().str());
    }
    if(rhs->getType()->isPointerTy()) {
        rhs = type_system::depointify(rhs, rhs->getName().str());
    }
    rhs = type_system::generate_cast(rhs, llvm::Type::getInt32Ty(*generator::theContext), "casttmp");
    lhs = type_system::generate_cast(lhs, llvm::Type::getInt32Ty(*generator::theContext), "casttmp");
    auto res = generator::builder->CreateAnd(lhs, rhs, "andtmp");
    return generator::builder->CreateUIToFP(res, llvm::Type::getDoubleTy(*generator::theContext), "booltmp");
}
